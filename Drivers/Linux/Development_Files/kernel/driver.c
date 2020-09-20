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



// QuickUSB SLA
MODULE_LICENSE("GPL");

inline void __user *compat_ptr(u32/*compat_uptr_t*/ uptr) {
    return (void __user *)(unsigned long)uptr;
}

// Globals
struct usb_device_id qusb_devtable [] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, qusb_devtable);

struct file_operations qusb_fops = {
    .owner = THIS_MODULE,
    .open = qusb_open,
    .release = qusb_release,


#if defined(HAVE_UNLOCKED_IOCTL)
    // Starting with the 2.6.11 kernels, we may specify unlocked_ioctl which will be used in place of ioctl and not use the BKL
    .unlocked_ioctl = qusb_unlocked_ioctl,
#else
    // For 2.6.10 kernels and earlier, we must specify ioctl, which will use the BKL
    .ioctl = qusb_locked_ioctl, // No longer defined in 2.6.36 kernels and later
#endif

#if defined(HAVE_COMPAT_IOCTL)
    // For kernels 2.6.11 and later we may specify a compat_ioctl which is called when a 32-bit process makes an ioctl call in a 64-bit kernel
    .compat_ioctl = qusb_compat_ioctl,
#endif

#if IMPLEMENT_ASYNC
    // By not specifying the .read and .write callbacks reads and writes
    // will be forwarded to the .aio_read and .aio_write methods with the
    // IOCBs marked as synchronous.
    .read_iter = qusb_read_iter,
    .write_iter = qusb_write_iter,
#else
    .read = qusb_read,
    .write = qusb_write,
#endif
    //.mmap = qusb_mmap,
};

struct usb_driver qusb_driver = {
    .name = DRIVER_NAME,
    .id_table = qusb_devtable,
    .probe = qusb_probe,
    .disconnect = qusb_disconnect,
};

struct usb_class_driver qusb_class = {
    .name = "QUSB-%d", // This creates the entry in /dev
    .fops = &qusb_fops,
    .minor_base = MINOR_BASE,
};

int num_qusb;



// Called by qusb_disconnect when the last QuickUSB device has been removed
// from the system
void qusb_delete(struct kref *reference) {
    struct qusb_dev *dev;

    QUSB_PRINTK(("qusb_delete()\n"));

    dev = container_of(reference, struct qusb_dev, ref);

    // Free any allocated URBs

    // Free allocated memory
    if (dev->buffer_ptr) {
        kfree(dev->buffer_ptr);
    }
    
    // Free the device
    usb_put_dev(dev->udev);
    kfree (dev);
}



// Called by open()
int qusb_open(struct inode *inode, struct file *filp) {
    struct qusb_dev *dev;
    struct usb_interface *interface;

    //QUSB_PRINTK(("qusb_open\n"));

    // Validate the device
    interface = usb_find_interface(&qusb_driver, iminor(inode));
    if (!interface) {
        QUSB_PRINTK(("Unable to get interface\n"));
        return -ENODEV;
    }
    
    dev = usb_get_intfdata(interface);
    if (!dev) {
        QUSB_PRINTK(("Invalid device\n"));
        return -ENODEV;
    }

    // Increment our usage counter for the device
    kref_get(&dev->ref);

#if IMPLEMENT_ASYNC
    // Lock the device to allow correctly handling errors in resumption
    mutex_lock(&dev->io_mutex);
#endif

    if (dev->open_count++) {
     /* //uncomment this block if you want exclusive open
        retval = -EBUSY;
        dev->open_count--;
        mutex_unlock(&dev->io_mutex);
        kref_put(&dev->kref, qusb_delete);
        return retval;*/
    }

    // Save device pointer
    filp->private_data = dev;
#if IMPLEMENT_ASYNC
    mutex_unlock(&dev->io_mutex);
#endif

    // Return successfully
    return 0;
}



// Called when the QuickUSB module is removed
int qusb_release(struct inode *inode, struct file *filp) {
    struct qusb_dev *dev;

    //QUSB_PRINTK(("qusb_release\n"));

    // Get QuickUSB device
    dev = filp->private_data;
    if (dev == NULL) {
        return -ENODEV;
    }

#if IMPLEMENT_ASYNC
    // Allow the device to be autosuspended
    mutex_lock(&dev->io_mutex);
    --dev->open_count;
    mutex_unlock(&dev->io_mutex);
#endif

    // Decrement our reference count of the device
    kref_put(&dev->ref, qusb_delete);

    return 0;
}



int qusb_probe(struct usb_interface *interface, const struct usb_device_id *device_id) {
    struct qusb_dev *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int result, i;

    QUSB_PRINTK(("qusb_probe\n"));

    // Allocate space for QuickUSB device
    dev = kzalloc(sizeof(struct qusb_dev), GFP_KERNEL);
    if (dev == NULL) {
        QUSB_PRINTK(("Out of memory\n"));
        return -ENOMEM;
    }

    // Initialize device fields
    kref_init(&dev->ref);
#if IMPLEMENT_ASYNC
    mutex_init(&dev->io_mutex);
    init_usb_anchor(&dev->pendingAsyncReadUrbs);
    init_usb_anchor(&dev->pendingAsyncWriteUrbs);
#endif
    dev->timeout = QUSB_DEFAULT_TIMEOUT;

    // Initialize QuickUSB device
    dev->udev = usb_get_dev(interface_to_usbdev(interface));
    dev->interface = interface;

    // Set up endpoints
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;

        // Check if there is no saved bulk in address and if the current endpoint is bulk in
        if (!dev->bulk_in_address && endpoint->bEndpointAddress == QUSB_EP_BULK_IN) {
            dev->bulk_in_address = endpoint->bEndpointAddress;
            dev->bulk_in_max_packet = endpoint->wMaxPacketSize;
        }
        // Check if there is no saved bulk out address and if the endpoint is bulk out
        if (!dev->bulk_out_address && endpoint->bEndpointAddress == QUSB_EP_BULK_OUT) {
            dev->bulk_out_address = endpoint->bEndpointAddress;
            dev->bulk_out_max_packet = endpoint->wMaxPacketSize;
        }
    }

    // Check if no bulk in or out addresses are found
    if (!(dev->bulk_in_address && dev->bulk_out_address)) {
        QUSB_PRINTK(("No bulk endpoints found\n"));
        kref_put(&dev->ref, qusb_delete);
        return -EINVAL;
    }

    // Allocate kernel space buffer
    dev->buffer_ptr = kmalloc(QUSB_INTERNAL_BUFFER_SIZE + PAGE_SIZE, GFP_KERNEL);
    if (dev->buffer_ptr == NULL) {
        QUSB_PRINTK(("Failed to allocate kernel buffer\n"));
        kref_put(&dev->ref, qusb_delete);
        return -ENOMEM;
    }
    dev->buffer_area = (unsigned char *)(((unsigned long)dev->buffer_ptr + PAGE_SIZE - 1) & PAGE_MASK);
    
    // Set QuickUSB device to interface
    usb_set_intfdata(interface, dev);

    // Register QuickUSB device
    result = usb_register_dev(interface, &qusb_class);
    if (result) {
        QUSB_PRINTK(("Unable to register device\n"));
        usb_set_intfdata(interface, NULL);
        kref_put(&dev->ref, qusb_delete);

        // dev->buffer_ptr is freed to be freed in qusb_delete
        return result;
    }

    // Keep track of the number of QuickUSB modules we're servicing
    QUSB_PRINTK(("Added QUSB-%i\n", num_qusb));
    ++num_qusb;

    // Return successfully
    return 0;
}



// Called when a module is disconnected from the computer.  If the last module
// was removed from the system, resources are freed by qusb_delete (which is
// automatically called when the dev->ref reference count reaches zero).
void qusb_disconnect(struct usb_interface *interface) {
    struct qusb_dev *dev;

    QUSB_PRINTK(("qusb_disconnect()\n"));

    // Unregisted the device
    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);
    usb_deregister_dev(interface, &qusb_class);

    // Prevent more IO from starting
#if IMPLEMENT_ASYNC
    mutex_lock(&dev->io_mutex);
        dev->interface = NULL;
    mutex_unlock(&dev->io_mutex);

    // Kill any outstanding URBS
    usb_kill_anchored_urbs(&dev->pendingAsyncReadUrbs);
    usb_kill_anchored_urbs(&dev->pendingAsyncWriteUrbs);
#else
    dev->interface = NULL;
#endif

    // Decrement our usage counter
    kref_put(&dev->ref, qusb_delete);
}



// Called when driver is loaded (insmod)
int __init qusb_init(void) {
    int result;

    QUSB_PRINTK(("qusb_init()\n"));

    // Initialize number of QuickUSB and driver error
    num_qusb = 0;

    // Register QuickUSB
    result = usb_register(&qusb_driver);
    if (result) {
        QUSB_PRINTK(("[%d] Failed to register driver.\n", result));
    }

    return result;
}
module_init(qusb_init);



// Called when driver is removed (rmmod)
void __exit qusb_exit(void) {
    // Unregister QuickUSB
    QUSB_PRINTK(("qusb_exit()\n"));
    usb_deregister(&qusb_driver);
}
module_exit(qusb_exit);


#if defined(HAVE_COMPAT_IOCTL)
long qusb_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    // This function is called whenever a 32-bit process calls ioctl() on a 64-bit system

    // Forward call to qusb_ioctl
    return qusb_ioctl(filp, cmd, compat_ptr(arg), 1);
}
#endif



// The main interface function to the library
#if defined(HAVE_UNLOCKED_IOCTL)

long qusb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    // Forward call on to qusb_ioctl
    return qusb_ioctl(filp, cmd, (void __user *)arg, 0);
}

#else

int qusb_locked_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
    // Forward call on to qusb_ioctl
    return qusb_ioctl(filp, cmd, (void __user *)arg, 0);
}

#endif

