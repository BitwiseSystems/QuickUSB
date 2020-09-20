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



//
// Perform a synchronous bulk data request
//
ssize_t qusb_sync(struct file *file, char __user *buf, size_t count, BOOL read, BOOL sendReadLength) {
    int retval, bytes, totalBytes;
    int offset;
    struct qusb_dev *dev;
    size_t blocksize;
    ULONG* pReadlen = kmalloc(sizeof(ULONG), GFP_KERNEL);
    if (!pReadlen)
    {
        QUSB_PRINTK(("Not enough memory.\n"));
        return -ENOMEM;
    }

    //QUSB_PRINTK(("qusb_sync() for %li bytes\n", count));

    // Validate the device
    dev = file->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        kfree(pReadlen);
        return -ENODEV;
    }

#if IMPLEMENT_ASYNC
    // Do not allow concurrent transactions to be issued
    /*retval = mutex_lock_interruptible(&dev->io_mutex);
    if (retval < 0) {
        return retval;
    }*/
    mutex_lock(&dev->io_mutex);
#endif

    // Check if qusb_disconnect was called
    if (dev->interface == NULL) {
#if IMPLEMENT_ASYNC
        mutex_unlock(&dev->io_mutex);
#endif
        kfree(pReadlen);
        return -ENODEV;
    }

    // For reads, send a control packet of the total number of bytes to read
    if (read && sendReadLength) {
        *pReadlen = (ULONG) count;
        retval = usb_control_msg(
            dev->udev,                     // Device
            usb_sndctrlpipe(dev->udev, 0), // Pipe
            0xB7,                          // Request
            USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,   // Request type
            0,                             // Value
            0,                             // Index
            pReadlen,                      // Data
            sizeof(ULONG),                 // Size
            dev->timeout);                 // Timeout

        // usb_control_msg returns 0 on success and a negative error value on failure
        if (retval < 0) {
            QUSB_PRINTK(("readlen failed with error: %i\n", -retval));
#if IMPLEMENT_ASYNC
            mutex_unlock(&dev->io_mutex);
#endif
            kfree(pReadlen);
            return retval;
        }
        QUSB_PRINTK(("Sent length control packet for %li bytes\n", (unsigned long)*pReadlen));
    }

    kfree(pReadlen);

    // Break the read into transfers no larger than QUSB_INTERNAL_BUFFER_SIZE in size
    offset = 0;
    totalBytes = 0;
    do {
        blocksize = (size_t) min(QUSB_INTERNAL_BUFFER_SIZE, (int)count - (int)offset);

        if (!read) {
            // copy_from_user returns the number of bytes that could not be copied
            if (copy_from_user(dev->buffer_area, buf + offset, blocksize)) {
                QUSB_PRINTK(("copy failed\n"));
#if IMPLEMENT_ASYNC
                mutex_unlock(&dev->io_mutex);
#endif
                return -EFAULT;
            }
        }

        // Submit bulk request
        retval = usb_bulk_msg(
            dev->udev,                   // Device
            read ? usb_rcvbulkpipe(dev->udev, dev->bulk_in_address) : usb_sndbulkpipe(dev->udev,
                dev->bulk_out_address),  // Pipe
            dev->buffer_area,            // Read buffer
            blocksize,                   // Bytes to read
            &bytes,                      // Bytes read
            dev->timeout);               // Timeout

        // usb_bulk_msg returns 0 on success and a negative error value on failure
        if (retval < 0) {
            QUSB_PRINTK(("usb_bulk_msg() failed with error: %i\n", -retval));

            /*if (retval == -ETIMEDOUT) {
                QUSB_PRINTK(("Timeout: Reseting endpoint\n"));
                usb_reset_endpoint(dev->udev, read ? dev->bulk_in_address : dev->bulk_out_address);
                usb_clear_halt(dev->udev, read ? usb_rcvbulkpipe(dev->udev, dev->bulk_in_address) : usb_sndbulkpipe(dev->udev, dev->bulk_out_address));
            }*/

#if IMPLEMENT_ASYNC
            mutex_unlock(&dev->io_mutex);
#endif
            return retval;
        }

        if (read) {
            // copy_to_user returns the number of bytes that could not be copied
            if (copy_to_user(buf + offset, dev->buffer_area, bytes)) {
                QUSB_PRINTK(("copy failed\n"));
#if IMPLEMENT_ASYNC
                mutex_unlock(&dev->io_mutex);
#endif
                return -EFAULT;
            }
        }

        // Keep track of the number of bytes read
        totalBytes += bytes;
        offset += blocksize;
    } while (offset < count);
    
    QUSB_PRINTK(("Successfully read %i bytes\n", (int)totalBytes));

#if IMPLEMENT_ASYNC
    // Release the IO hold so subsequent transactions may be issued
    mutex_unlock(&dev->io_mutex);
#endif

    // Return the number of bytes read
    return totalBytes;
}



#if IMPLEMENT_ASYNC==0

extern ssize_t qusb_read(struct file *filp, char __user *buf, size_t count, loff_t *pos) {
    return qusb_sync(filp, buf, count, TRUE, TRUE);
}



extern ssize_t qusb_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos) {
    return qusb_sync(filp, (char __user *)buf, count, FALSE, FALSE);
}

#endif



#if IMPLEMENT_ASYNC

ssize_t qusb_read_iter(struct kiocb *iocb, struct iov_iter *vec)
{
    int k;

    //QUSB_PRINTK(("qusb_aio_read() (%li requests)\n", count));

    // Currently, only 'count=1' is supported
    for (k = 0; k < vec->count; ++k) {
        // Check for zero-length requests
        if (vec->iov[k].iov_len == 0) {
            return 0;
        }

        // Check for 'special' commands
        if ((vec->iov[k].iov_base == NULL)) {
            if (vec->iov[k].iov_len == QUSB_THREAD_CMD_RUN) {
                QUSB_PRINTK(("Run command\n"));
                return vec->iov[k].iov_len;
            } else if (vec->iov[k].iov_len == QUSB_THREAD_CMD_SHUTDOWN) {
                QUSB_PRINTK(("Shutdown command\n"));
                return vec->iov[k].iov_len;
            } else {
                QUSB_PRINTK(("Unknown command: 0x%x\n", (int)vec->iov[k].iov_len));
                return -EINVAL;
            }
        }

        // Is this is a synchronous IOCB
        if (is_sync_kiocb(iocb)) {
            // Synchronous IOCBs must complete their operation immediately and
            // return their result
            return qusb_sync(iocb->ki_filp, vec->iov[k].iov_base, vec->iov[k].iov_len, true, true);
        } else {
            // Asynchronous requests may either complete their operation immediately
            // and return their result (as in the synchronous case), or issue
            // asynchronous requests and return EIOCBQUEUED.  If EIOCBQUEUED is
            // returned, then aio_complete must be called on the IOCB when
            // the asynchronous requests have completed.
            return qusb_async(iocb, vec->iov[k].iov_base, vec->iov[k].iov_len, true);
        }
    }

    return -EINVAL;
}

ssize_t qusb_write_iter(struct kiocb *iocb, struct iov_iter *vec) 
{
    int k;

    //QUSB_PRINTK(("qusb_aio_write() (%li requests)\n", count));

    // Currently, only 'count=1' is supported
    for (k = 0; k < vec->count; ++k) {
        // Check for zero-length requests
        if (vec->iov[k].iov_len == 0) {
            return 0;
        }

        // Check for 'special' commands
        if ((vec->iov[k].iov_base == NULL)) {
            if (vec->iov[k].iov_len == QUSB_THREAD_CMD_RUN) {
                QUSB_PRINTK(("Run command\n"));
                return vec->iov[k].iov_len;
            } else if (vec->iov[k].iov_len == QUSB_THREAD_CMD_SHUTDOWN) {
                QUSB_PRINTK(("Shutdown command\n"));
                return vec->iov[k].iov_len;
            } else {
                QUSB_PRINTK(("Unknown command: 0x%x\n", (int)vec->iov[k].iov_len));
                return -EINVAL;
            }
        }

        // Is this is a synchronous IOCB
        if (is_sync_kiocb(iocb)) {
            // Synchronous IOCBs must complete their operation immediately and
            // return their result
            return qusb_sync(iocb->ki_filp, vec->iov[k].iov_base, vec->iov[k].iov_len, false, false);
        } else {
            // Asynchronous requests may either complete their operation immediately
            // and return their result (as in the synchronous case), or issue
            // asynchronous requests and return EIOCBQUEUED.  If EIOCBQUEUED is
            // returned, then aio_complete must be called on the IOCB when
            // the asynchronous requests have completed.
            return qusb_async(iocb, vec->iov[k].iov_base, vec->iov[k].iov_len, false);
        }
    }

    return -EINVAL;
}

/*int qusb_aio_cancel(struct kiocb *iocb, struct io_event *event) {
    struct async_request_context *req = (struct async_request_context*) iocb->ki_user_data;
    QUSB_PRINTK(("qusb_aio_cancel()\n"));

    if (req != NULL) {
        QUSB_PRINTK(("--> Cancelling request\n"));
        qusb_sg_cancel(&req->io);
    }
    return 0;
}*/

static void async_request_timeout(struct timer_list *t){
    struct async_request_context *req = (struct async_request_context*) from_timer(req, t, timer);
    QUSB_PRINTK(("Async request has timed out and will be unlinked\n"));

    qusb_sg_timedout(&req->io);
}



//
// Perform an asynchronous bulk data request
//
ssize_t qusb_async(struct kiocb *iocb, char __user *buf, size_t count, BOOL read) {
    struct qusb_dev *dev;
    int failure, retval;
    int k;
    int transSize;
    int temp;
    int pageOffset;
    char __user *addr = buf;
    BOOL aligned;
    int totalBytes = 0;
    ULONG* pReadlen = kmalloc(sizeof(ULONG), GFP_KERNEL);
    size_t unalignedCount = 0;
    struct async_request_context *req;
    unsigned long expire;

    if (!pReadlen)
    {
        QUSB_PRINTK(("Not enough memory.\n"));
        return -ENOMEM;
    }
    
    QUSB_PRINTK(("qusb_async() %s for %li bytes (0x%p)\n", (read ? "read" : "write"), (long)count, iocb));

    // Validate the device
    dev = iocb->ki_filp->private_data;
    if ((dev == NULL) || (dev->udev == NULL)) {
        QUSB_PRINTK(("Invalid device.\n"));
        kfree(pReadlen);
        return -ENODEV;
    }

    // If the user buffer is not aligned on 512 byte boundaries then we cannot
    // issue a DMA transation with that buffer.
    unalignedCount = (long unsigned int)addr & (512 - 1);
    if (unalignedCount != 0) {
        QUSB_PRINTK(("User buffer not 512-byte aligned (At 0x%p)\n", addr));
        kfree(pReadlen);
        return -EINVAL;
    }

    // Allocate asynchronous request object
    req = (struct async_request_context *) kzalloc(sizeof(struct async_request_context), GFP_KERNEL);
    if (!req) {
        kfree(pReadlen);
        return -ENOMEM;
    }

    // Initialize the async request fields
    req->dev = dev;
    req->iocb = iocb;
    //req->iocb->ki_user_data = (__u64) req;
    //req->iocb->ki_cancel = qusb_aio_cancel; // TODO: Implement abort on Linux
    req->read = read;

    // Figure out how many pages the user buffer is occupying
    req->numPages = 1;
    pageOffset = ((long unsigned int)addr - ((long unsigned int)addr & PAGE_MASK));
    transSize = min((long unsigned int)PAGE_SIZE - (long unsigned int)pageOffset, (long unsigned int)count); // First page transaction size
    temp = (count - transSize); // Length of transaction less the first page
    req->numPages += (temp >> PAGE_SHIFT);
    if ((temp & PAGE_MASK) != temp) {
        ++req->numPages;
    }

    // Lock the user pages
    req->pages = (struct page **) kmalloc(sizeof(struct page *) * req->numPages, GFP_KERNEL);
    if (!req->pages) {
        kfree(req);
        kfree(pReadlen);
        return -ENOMEM;
    }
    down_read(&current->mm->mmap_sem);
    req->lockedPages = get_user_pages_remote(current, current->mm, (unsigned long)addr, req->numPages, 0, req->pages, NULL, NULL);
    up_read(&current->mm->mmap_sem);
    if (req->lockedPages != req->numPages) {
        QUSB_PRINTK(("Not all pages locked.\n"));
        
        // Release the locked user pages
        for (k = 0; k < req->numPages; ++k) {
            put_page(req->pages[k]);
        }

        kfree(req->pages);
        kfree(req);
        kfree(pReadlen);
        return -EFAULT;
    }
    //QUSB_PRINTK(("Locked %i user pages\n", req->numPages));

    req->sgl = (struct scatterlist *) kmalloc(sizeof(struct scatterlist) * req->numPages, GFP_KERNEL);
    if (!req->sgl) {
        // Release the locked user pages
        for (k = 0; k < req->numPages; ++k) {
            put_page(req->pages[k]);
        }

        kfree(req->pages);
        kfree(req);
        kfree(pReadlen);
        return -ENOMEM;
    }

    failure = 0;
    for (k = 0; k < req->numPages; ++k) {
        // Check for a NULL page address
        if (req->pages[k] == NULL) {
            QUSB_PRINTK(("NULL page pointer found.\n"));
            failure = -EINVAL;
        }

        // We are performing Direct IO, so the user space buffer must be page aligned
        pageOffset = ((long unsigned int)addr - ((long unsigned int)addr & PAGE_MASK));
        transSize = min((long unsigned int)PAGE_SIZE - (long unsigned int)pageOffset, (long unsigned int)(count - totalBytes));
        aligned = (pageOffset == 0);

        //QUSB_PRINTK(("Page=%i, Offset=%i, Aligned=%i, TransSize=%i\n", k, pageOffset, aligned, transSize));

        req->sgl[k].page_link = (unsigned long)req->pages[k];
        req->sgl[k].offset = pageOffset;
        req->sgl[k].length = transSize;

        // Move to next page
        addr += transSize;
        totalBytes += transSize;
    }

    // Do not allow concurrent transactions to be issued
    /*retval = mutex_lock_interruptible(&dev->io_mutex);
    if (retval < 0) {
        // Release the locked user pages
        for (k = 0; k < req->numPages; ++k) {
            page_cache_release(req->pages[k]);
        }

        kfree(req->pages);
        kfree(req->sgl);
        kfree(req);
        return retval;
    }*/
    mutex_lock(&dev->io_mutex);

    // Check if qusb_disconnect was called
    if (dev->interface == NULL) {
        mutex_unlock(&dev->io_mutex);
        
        // Release the locked user pages
        for (k = 0; k < req->numPages; ++k) {
            put_page(req->pages[k]);
        }

        kfree(req->pages);
        kfree(req->sgl);
        kfree(req);
        kfree(pReadlen);
        return -ENODEV;
    }

    // Handle internal request serialization
    expire = msecs_to_jiffies(dev->timeout);
    if (read) {
        // To perform a read, there must be no pending writes
        retval = usb_wait_anchor_empty_timeout(&dev->pendingAsyncWriteUrbs, dev->timeout);
        if (retval == 0) {
            QUSB_PRINTK(("usb_wait_anchor_empty_timeout timed out\n"));
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                put_page(req->pages[k]);
            }

            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            kfree(pReadlen);
            return -ETIMEDOUT;
        }
    } else {
        // To perform a write, there must be no pending reads
        retval = usb_wait_anchor_empty_timeout(&dev->pendingAsyncReadUrbs, dev->timeout);
        if (retval == 0) {
            QUSB_PRINTK(("usb_wait_anchor_empty_timeout timed out\n"));
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                put_page(req->pages[k]);
            }

            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            kfree(pReadlen);
            return -ETIMEDOUT;
        }
    }

    // For reads, send the control packet telling firmware how many bytes we want to read
    if (read) {
        *pReadlen = (count - unalignedCount);

        // Allocate the URB
        /*req->ctrlRequest = usb_alloc_urb(0, GFP_KERNEL);
        if (!req->ctrlRequest) {
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                page_cache_release(req->pages[k]);
            }

            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            return -ENODEV;
        }

        // Allocate the control setup packet
        req->setup_packet = (struct usb_ctrlrequest *) kmalloc(sizeof(struct usb_ctrlrequest), GFP_KERNEL);
        if (!req->setup_packet) {
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                page_cache_release(req->pages[k]);
            }

            usb_free_urb(req->ctrlRequest);
            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            return -ENOMEM;
        }

        // Populate the setup control packet
        req->setup_packet->bRequestType = USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE; // 64
        req->setup_packet->bRequest = 0xB7;
        req->setup_packet->wValue = cpu_to_le16(0);
        req->setup_packet->wIndex = cpu_to_le16(0);
        req->setup_packet->wLength = cpu_to_le16(sizeof(ULONG));

        // Populate the URB
        usb_fill_control_urb(req->ctrlRequest, dev->udev, usb_sndctrlpipe(dev->udev, 0), (unsigned char *) req->setup_packet, &readlen, sizeof(ULONG), internal_control_completion, req);

        // Anchor the URB
        usb_anchor_urb(req->ctrlRequest, &dev->pendingUrbs);

        init_completion(&req->ctrlDone);
        
        // Submit the URB
        retval = usb_submit_urb(req->ctrlRequest, GFP_KERNEL);
        if (retval < 0) {
            QUSB_PRINTK(("readlen failed with error %i (%li bytes)\n", -retval, (unsigned long)readlen));
            usb_unanchor_urb(req->ctrlRequest);
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                page_cache_release(req->pages[k]);
            }

            kfree(req->setup_packet);
            usb_free_urb(req->ctrlRequest);
            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            return retval;
        }

        // Wait for the control packet to complete
        expire = msecs_to_jiffies(dev->timeout);
        if (!wait_for_completion_timeout(&req->ctrlDone, expire)) {
            usb_kill_urb(req->ctrlRequest);

            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                page_cache_release(req->pages[k]);
            }
            
            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);

            return -ETIMEDOUT;
        }*/

        retval = usb_control_msg(
            dev->udev,                     // Device
            usb_sndctrlpipe(dev->udev, 0), // Pipe
            0xB7,                          // Request
            USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,   // Request type
            0,                             // Value
            0,                             // Index
            pReadlen,                      // Data
            sizeof(ULONG),                 // Size
            dev->timeout);                 // Timeout

        // usb_control_msg returns 0 on success and a negative error value on failure
        if (retval < 0) {
            QUSB_PRINTK(("readlen failed with error: %i\n", -retval));
            mutex_unlock(&dev->io_mutex);

            // Release the locked user pages
            for (k = 0; k < req->numPages; ++k) {
                put_page(req->pages[k]);
            }

            //kfree(req->setup_packet);
            //usb_free_urb(req->ctrlRequest);
            kfree(req->pages);
            kfree(req->sgl);
            kfree(req);
            kfree(pReadlen);
            return retval;
        }
        //QUSB_PRINTK(("Sent length control packet for %li bytes\n", (unsigned long)readlen));
    }

    kfree(pReadlen);

    // Initialize scatter/gather list, URBs, etc.
    //QUSB_PRINTK(("Issuing USB scatter-gather operation\n"));
    retval = qusb_sg_init(&req->io, dev->udev, read ? usb_rcvbulkpipe(dev->udev, dev->bulk_in_address) : usb_sndbulkpipe(dev->udev, dev->bulk_out_address), 0, req->sgl, req->numPages, 0, 0, req);
    if (retval != 0) {
        mutex_unlock(&dev->io_mutex);

        // Release the locked user pages
        for (k = 0; k < req->numPages; ++k) {
            put_page(req->pages[k]);
        }

        kfree(req->pages);
        kfree(req->sgl);
        kfree(req);
        return retval;
    }

    // Increase the pending request counts
    if (read) {
        ++(dev->pendingReads);
    } else {
        ++(dev->pendingWrites);
    }

    // If the buffer was not aligned, then part of the transaction was done synchronous and
    // needs to be reflected in the request so that the proper number of bytes is reported
    // when the request completes
    //req->io.bytes = unalignedCount;
    //QUSB_PRINTK(("Unaligned count: %i\n", (unsigned long)unalignedCount));

    // Initialize the timeout timer for the request
    timer_setup(&req->timer, async_request_timeout, 0);
    mod_timer(&req->timer, jiffies + (HZ * dev->timeout) / 1000);
    // Submit the scatter/gather URBs asynchronously
    qusb_sg_submit_async(&req->io, (read ? &dev->pendingAsyncReadUrbs : &dev->pendingAsyncWriteUrbs));
    QUSB_PRINTK(("Asynchronous %s request issued for %i bytes\n", (read ? "read" : "write"), (int)(totalBytes - unalignedCount)));

    // Release the IO hold so subsequent transactions may be issued
    mutex_unlock(&dev->io_mutex);

    // All req->numPages locked user pages in req->pages are to be release in the request completion handler
    // reg and req->sgl to be freed in the request completion handler
        
    // The asynchronous request has been issued and added to the IOCB queue
    return -EIOCBQUEUED;
}



/*static void internal_control_completion(struct urb* urb) {
    //QUSB_PRINTK(("Async control URB: %i\n", urb->status));
    struct async_request_context* req = (struct async_request_context*) urb->context;

    if (urb->status != 0) {
        // Error
        QUSB_PRINTK(("Async control URB failed with error: %i\n", urb->status));
    } else {
        // Success
        QUSB_PRINTK(("Async control URB completed successfully\n"));
    }

    // Free control packet memories
    kfree(req->setup_packet);
    usb_free_urb(req->ctrlRequest);

    // Complete the control request
    complete(&req->ctrlDone);
}*/

#endif
