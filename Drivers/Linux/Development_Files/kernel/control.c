/*=============================================================================
Title        :
Description  : QuickUSB Linux Driver
Notes        : None
History      :

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

=============================================================================*/
#include "driver.h"



// Get string descriptors
long qusb_GetStringDescriptor(struct file *filp, UCHAR index, PVOID buffer, ULONG length) {
    struct qusb_dev *dev;
    int result;//, k;
    void* kBuffer;

    //QUSB_PRINTK(("Retrieving String #%d, length=%d\n", (int)index, (int)length));

    // Validate the device
    dev = filp->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        return -ENODEV;
    }

#if IMPLEMENT_ASYNC
    // Do not allow concurrent transactions to be issued
    result = mutex_lock_interruptible(&dev->io_mutex);
    if (result < 0) {
        return result;
    }
#endif

    // Check if qusb_disconnect was called
    if (dev->interface == NULL) {
#if IMPLEMENT_ASYNC
        mutex_unlock(&dev->io_mutex);
#endif
        return -ENODEV;
    }

    kBuffer = kmalloc(MAX_STRING_DESCRIPTOR_OUT_LENGTH, GFP_KERNEL);
    // Retrieve string descriptor
    result = usb_control_msg(
        dev->udev,                        // Device
        usb_rcvctrlpipe(dev->udev, 0),    // Pipe
        USB_REQ_GET_DESCRIPTOR,           // Request
        USB_DIR_IN,                       // Request type
        (USB_DT_STRING<<8)|index,         // Value
        0,                                // Index
        kBuffer,                           // Data
        MAX_STRING_DESCRIPTOR_OUT_LENGTH, // Size
        dev->timeout);                    // Timeout

    memcpy(buffer, kBuffer, MAX_STRING_DESCRIPTOR_OUT_LENGTH);
    kfree(kBuffer);

#if IMPLEMENT_ASYNC
    // Release the IO hold so subsequent transactions may be issued
    mutex_unlock(&dev->io_mutex);
#endif

    // Check the result of the control transfer
    if (result < 0) {
        QUSB_PRINTK(("String Desc #%d: Failed with error: %i \n", (int)index, -result));
        return result;
    }

    //QUSB_PRINTK(("String Desc #%d: ", (int)index));
    //for (k=2; k<length && ((char *)buffer)[k] != 0; k+=2) {
    //    printk("%c", ((char *)buffer)[k]);
    //}
    //printk("\n");

    return result;
}



// Get the USB device desc info
long qusb_GetDeviceDescriptor(struct file *filp, struct usb_device_descriptor *device_descriptor) {
    struct qusb_dev *dev;
    int result;
    void* devDescBuffer;

    // Validate the device
    dev = filp->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        return -ENODEV;
    }

    // Validate parameters
    if (device_descriptor == NULL) {
        QUSB_PRINTK(("Invalid device descriptor.\n"));
        return -EINVAL;
    }

#if IMPLEMENT_ASYNC
    // Do not allow concurrent transactions to be issued
    result = mutex_lock_interruptible(&dev->io_mutex);
    if (result < 0) {
        return result;
    }
#endif

    // Check if qusb_disconnect was called
    if (dev->interface == NULL) {
#if IMPLEMENT_ASYNC
        mutex_unlock(&dev->io_mutex);
#endif
        return -ENODEV;
    }

    devDescBuffer = kmalloc(sizeof(struct usb_device_descriptor), GFP_KERNEL);
    // Retrieve device descriptor
    result = usb_control_msg(dev->udev,       // Device
        usb_rcvctrlpipe(dev->udev, 0),        // Pipe
        USB_REQ_GET_DESCRIPTOR,               // Request
        USB_DIR_IN,                           // Request type
        (USB_DT_DEVICE<<8),                   // Value
        0,                                    // Index
        devDescBuffer,                        // Data
        sizeof(struct usb_device_descriptor), // Size
        dev->timeout);                        // Timeout

    memcpy(device_descriptor, devDescBuffer, sizeof(struct usb_device_descriptor));
    kfree(devDescBuffer);

#if IMPLEMENT_ASYNC
    // Release the IO hold so subsequent transactions may be issued
    mutex_unlock(&dev->io_mutex);
#endif

    // Check the result of the control transfer
    if (result < 0) {
        QUSB_PRINTK(("usb_control_msg failed with error: %i\n", -result));
        return result;
    }

    return result;
}



// Perform vendor requests (control requests)
long qusb_VendorRequest(struct file *filp, QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL *vcrc, BYTE compat) {
    struct qusb_dev *dev;
    dma_addr_t dma;
    UCHAR requestType;
    unsigned int ctrl_pipe;
    void *buffer;
    ULONG length;
    int result;
    //int k;

    buffer = NULL;

    // Validate the device
    dev = filp->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        return -ENODEV;
    }

    // Validate parameters
    if (vcrc == NULL) {
        QUSB_PRINTK(("Invalid request control.\n"));
        return -EINVAL;
    }

#if IMPLEMENT_ASYNC
    // Do not allow concurrent transactions to be issued
    /*result = mutex_lock_interruptible(&dev->io_mutex);
    if (result < 0) {
        return result;
    }*/
    mutex_lock(&dev->io_mutex);
#endif

    // Check if qusb_disconnect was called
    if (dev->interface == NULL) {
#if IMPLEMENT_ASYNC
        mutex_unlock(&dev->io_mutex);
#endif
        return -ENODEV;
    }

    // Get request type
    requestType = (vcrc->direction<<7)|(vcrc->requestType<<5)|(vcrc->recepient);
    
    // Allocate buffer memory
    length = vcrc->length;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
    buffer = usb_buffer_alloc(dev->udev, length, GFP_KERNEL, &dma);
#else
    buffer = usb_alloc_coherent(dev->udev, length, GFP_KERNEL, &dma);
#endif
    if (buffer == NULL) {
        QUSB_PRINTK(("Unable to allocate memory (Device=0x%p, Len=%li)\n", dev->udev, length));
#if IMPLEMENT_ASYNC
        mutex_unlock(&dev->io_mutex);
#endif
        return -ENOMEM;
    }

    // Get control pipe
    if (vcrc->direction == 0) { // Out
        ctrl_pipe = usb_sndctrlpipe(dev->udev, 0);

        // Copy data from user space to write
        if ((compat && copy_from_user(buffer, compat_ptr((ULONG)vcrc->data), length)) || (!compat && copy_from_user(buffer, vcrc->data, length))) {
            QUSB_PRINTK(("Unable to copy data from user land\n"));
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
            usb_buffer_free(dev->udev, length, buffer, dma);
#else
            usb_free_coherent(dev->udev, length, buffer, dma);
#endif
#if IMPLEMENT_ASYNC
            mutex_unlock(&dev->io_mutex);
#endif
            return -EFAULT;
        }

        //for (k=0; k<length; ++k) {
        //    QUSB_PRINTK(("data[%i]: 0x%x\n", k, ((unsigned char *)buffer)[k]));
        //}

    }
    else { // In
        ctrl_pipe = usb_rcvctrlpipe(dev->udev, 0);
    }
    
#if IMPLEMENT_ASYNC
    // Command reads and writes access the GPIF.  Since we may not overlap GPIF data R/Ws
    // with GPIF command R/Ws we must serialize them.
    if (vcrc->request == 0xB2) {
        // Wait for reads
        result = usb_wait_anchor_empty_timeout(&dev->pendingAsyncReadUrbs, dev->timeout);
        if (result == 0) {
            QUSB_PRINTK(("usb_wait_anchor_empty_timeout timed out for command R/W\n"));
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
            usb_buffer_free(dev->udev, length, buffer, dma);
#else
            usb_free_coherent(dev->udev, length, buffer, dma);
#endif
            mutex_unlock(&dev->io_mutex);
            return -ETIMEDOUT;
        }

        // Wait for writes
        result = usb_wait_anchor_empty_timeout(&dev->pendingAsyncWriteUrbs, dev->timeout);
        if (result == 0) {
            QUSB_PRINTK(("usb_wait_anchor_empty_timeout timed out for command R/W\n"));
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
            usb_buffer_free(dev->udev, length, buffer, dma);
#else
            usb_free_coherent(dev->udev, length, buffer, dma);
#endif
            mutex_unlock(&dev->io_mutex);
            return -ETIMEDOUT;
        }
    }
#endif
            
    // Submit vendor request
    //QUSB_PRINTK(("vcrc->request=%x\n", vcrc->request));
    //QUSB_PRINTK(("requestType=%i\n", requestType));
    //QUSB_PRINTK(("vcrc->value=%i\n", vcrc->value));
    //QUSB_PRINTK(("vcrc->index=%i\n", vcrc->index));
    //QUSB_PRINTK(("length=%li\n", length));
    result = usb_control_msg(dev->udev, // Device
            ctrl_pipe,                  // Pipe
            vcrc->request,              // Request
            requestType,                // Request type
            vcrc->value,                // Value
            vcrc->index,                // Index
            buffer,                     // Data
            length,                     // Size
            dev->timeout);              // Timeout
    //QUSB_PRINTK(("result=%i\n", result));

#if IMPLEMENT_ASYNC
    // Release the IO hold so subsequent transactions may be issued
    mutex_unlock(&dev->io_mutex);
#endif

    // Check the result of the control transfer
    if (result < 0) {
        QUSB_PRINTK(("usb_control_msg failed with error: %i\n", -result));
        vcrc->length = 0;
        
        // Free allocated memory
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
        usb_buffer_free(dev->udev, length, buffer, dma);
#else
        usb_free_coherent(dev->udev, length, buffer, dma);
#endif

        return result;
    }

    // Copy data to user space
    if (vcrc->direction == 1) { // In
        if ((compat && copy_to_user(compat_ptr((ULONG)vcrc->data), buffer, length)) ||
            (!compat && copy_to_user(vcrc->data, buffer, length))) {
            QUSB_PRINTK(("Unable to copy data back to user land"));

        // Free allocated memory
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
        usb_buffer_free(dev->udev, length, buffer, dma);
#else
        usb_free_coherent(dev->udev, length, buffer, dma);
#endif

            return -EFAULT;
        }
    }

    // Free allocated memory
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
    usb_buffer_free(dev->udev, length, buffer, dma);
#else
    usb_free_coherent(dev->udev, length, buffer, dma);
#endif

    // Return successfully
    vcrc->length = result;
    return result;
}



int qusb_ioctl(struct file *filp, unsigned int cmd, void __user *arg, BYTE compat) {
    struct qusb_dev *dev;
    QUSB_DRIVER_VERSION driver_version;
    QUSB_GET_STRING_DESCRIPTOR string_desc;
    QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL vendor_or_class_req_ctrl;
    struct usb_device_descriptor device_descriptor;
    ULONG timeout;
    int result;

    result = 0;

    // Check type
    if ((_IOC_TYPE(cmd) != QUSB_IOCTL_INDEX)) {
        QUSB_PRINTK(("Invalid command type.\n"));
    }

    // Verify arg address
    if (_IOC_DIR(cmd) & _IOC_READ) {
        result = !access_ok(arg, _IOC_SIZE(cmd));
    }
    else if ( _IOC_DIR( cmd ) & _IOC_WRITE ) {
        result = !access_ok(arg, _IOC_SIZE(cmd));
    }
    if (result) {
        QUSB_PRINTK(("Invalid argument\n"));
        return -EFAULT;
    }

    // Validate the device
    dev = filp->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        return -ENODEV;
    }

    // Interpret commands
    switch (cmd) {
        case IOCTL_QUSB_GET_DEVICE_DESCRIPTOR:
            //QUSB_PRINTK(("IOCTL_QUSB_GET_DEVICE_DESCRIPTOR\n"));
            
            result = qusb_GetDeviceDescriptor(filp, &device_descriptor);
            if (result < 0) {
                QUSB_PRINTK(("qusb_GetDeviceDescriptor failed with error: %i\n", -result));
                return result;
            }

            // Send data to user
            result = copy_to_user(( void __user *)arg, &device_descriptor, sizeof(struct usb_device_descriptor));
            if (result != 0) {
                QUSB_PRINTK(("copy_to_user failed with %i bytes not copied\n", result));
                return result;
            }

            break;

        case IOCTL_QUSB_GET_STRING_DESCRIPTOR:
            //QUSB_PRINTK(("IOCTL_QUSB_GET_STRING_DESCRIPTOR\n"));

            // Interpret arg
            result = copy_from_user(&string_desc, arg, sizeof(QUSB_GET_STRING_DESCRIPTOR));
            if (result != 0) {
                QUSB_PRINTK(("copy_to_user failed with %i bytes not copied\n", result));
                return result;
            }

            // Get string descriptor
            result = qusb_GetStringDescriptor(filp, string_desc.Index, string_desc.Data, string_desc.Length);
            if (result < 0) {
                return result;
            }
            QUSB_PRINTK(("Read String Desc #%i: %s\n", string_desc.Index, string_desc.Data));

            // Store bytes read
            string_desc.Length = result;

            // Send data to user
            result = copy_to_user(arg, &string_desc, sizeof(QUSB_GET_STRING_DESCRIPTOR));
            if (result != 0) {
                QUSB_PRINTK(("copy_to_user failed with %i bytes not copied\n", result));
                return result;
            }

            break;

        case IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST:
            //QUSB_PRINTK(("IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST\n"));

            // Interpret arg
            result = copy_from_user(&vendor_or_class_req_ctrl, arg, sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL));
            if (result != 0) {
                QUSB_PRINTK(("copy_from_user failed with %i bytes not copied\n", result));
                return result;
            }

            result = qusb_VendorRequest(filp, &vendor_or_class_req_ctrl, compat);
            if (result < 0) {
                QUSB_PRINTK(("IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST failed (Error=%i)\n", -result));
                return result;
            }

            // Send data to user
            result = copy_to_user(( void __user *)arg, &vendor_or_class_req_ctrl, sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL));
            if (result != 0) {
                QUSB_PRINTK(("copy_to_user failed with %i bytes not copied\n", result));
                return result;
            }

            break;

        case IOCTL_QUSB_GET_DRIVER_VERSION:
            //QUSB_PRINTK(("IOCTL_QUSB_GET_DRIVER_VERSION\n"));
            
            driver_version.MajorVersion = QUSB_DRIVER_MAJOR_VERSION;
            driver_version.MinorVersion = QUSB_DRIVER_MINOR_VERSION;
            driver_version.BuildVersion = QUSB_DRIVER_BUILD_VERSION;
            driver_version.QusbDriverNum = 0;

            // Send data to user
            result = copy_to_user(arg, &driver_version, sizeof(QUSB_DRIVER_VERSION));
            if (result != 0) {
                QUSB_PRINTK(("copy_to_user failed with error: %i\n", result));
                return result;
            }

            break;

        case IOCTL_QUSB_SET_TIMEOUT:
            //QUSB_PRINTK(("IOCTL_QUSB_SET_TIMEOUT\n"));

            // Interpret arg
            result = copy_from_user(&timeout , arg, sizeof(unsigned long));
            if (result != 0) {
                QUSB_PRINTK(("copy_from_user failed with %i bytes not copied\n", result));
                return result;
            }

            // Set timeout
            dev->timeout = timeout;
            
            break;

        default:
            QUSB_PRINTK(("Invalid command - %d..\n", cmd));
            return -EINVAL;
    }

    // Return successfully
    return 0;
}
