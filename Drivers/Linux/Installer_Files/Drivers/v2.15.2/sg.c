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


#if IMPLEMENT_ASYNC

void qusb_sg_clean(struct usb_sg_request *io)
{
    if (io->urbs) {
        while (io->entries--)
            usb_free_urb(io->urbs [io->entries]);
        kfree(io->urbs);
        io->urbs = NULL;
    }
    io->dev = NULL;
}



void qusb_sg_complete(struct urb *urb)
{
    struct async_request_context *req = urb->context;
    struct usb_sg_request *io = &req->io;
    int status = urb->status;
    int k;

    spin_lock(&io->lock);

    // Stop the timeout timer
    del_timer(&req->timer);         //  NEW: Timeout

    /* In 2.5 we require hcds' endpoint queues not to progress after fault
    * reports, until the completion callback (this!) returns.  That lets
    * device driver code (like this routine) unlink queued urbs first,
    * if it needs to, since the HC won't work on them at all.  So it's
    * not possible for page N+1 to overwrite page N, and so on.
    *
    * That's only for "hard" faults; "soft" faults (unlinks) sometimes
    * complete before the HCD can get requests away from hardware,
    * though never during cleanup after a hard fault.
    */
    if (io->status
        && (io->status != -ECONNRESET
        || status != -ECONNRESET)
        && urb->actual_length) {
            dev_err(io->dev->bus->controller,
                "dev %s ep%d%s scatterlist error %d/%d\n",
                io->dev->devpath,
                usb_endpoint_num(&urb->ep->desc),
                usb_urb_dir_in(urb) ? "in" : "out",
                status, io->status);
            /* BUG (); */
    }

    if (io->status == 0 && status && status != -ECONNRESET) {
        int i, found, retval;

        io->status = status;

        /* the previous urbs, and this one, completed already.
        * unlink pending urbs so they won't rx/tx bad data.
        * careful: unlink can sometimes be synchronous...
        */
        spin_unlock(&io->lock);
        for (i = 0, found = 0; i < io->entries; i++) {
            if (!io->urbs [i] || !io->urbs [i]->dev)
                continue;
            if (found) {
                retval = usb_unlink_urb(io->urbs [i]);
                if (retval != -EINPROGRESS &&
                    retval != -ENODEV &&
                    retval != -EBUSY)
                    dev_err(&io->dev->dev,
                    "%s, unlink --> %d\n",
                    __func__, retval);
            } else if (urb == io->urbs [i])
                found = 1;
        }
        spin_lock(&io->lock);
    }
    urb->dev = NULL;

    /* on the last completion, signal usb_sg_wait() */
    io->bytes += urb->actual_length;
    io->count--;
    //QUSB_PRINTK(("COMPLETION ROUTINE\n"));
    if (!io->count) {
        //QUSB_PRINTK(("Asynchronous request complete (0x%p)\n", req->iocb));
        // Release the user pages
        for (k = 0; k < req->numPages; ++k) {
            // Mark page as dirty
            if (!PageReserved(req->pages[k])) {
                SetPageDirty(req->pages[k]);
            }

            // Unlock the page
            put_page(req->pages[k]);
        }

        // Signal that the operation has completed
        complete(&io->complete);

        // Complete the asynchronous request
        req->iocb->ki_complete(req->iocb, io->status, req->io.bytes);
        QUSB_PRINTK(("Asynchronous request complete (%i, %i, %p)\n", (int)io->status, (int)req->io.bytes, req->iocb));
        
        // Handle internal request serialization
        (req->read) ? --(req->dev->pendingReads) : --(req->dev->pendingWrites);
        
        // Clean up here since the scatter/gather URBs were asynchronously issued
        qusb_sg_clean(io);

        // Be careful to release the spinlock before freeing its memory
        spin_unlock(&io->lock);

        // Free memory
        kfree(req->pages);
        kfree(req->sgl);
        kfree(req);
        
        return;
    }

    spin_unlock(&io->lock);
}



int qusb_sg_init(struct usb_sg_request *io, struct usb_device *dev,
                unsigned pipe, unsigned period, struct scatterlist *sg,
                int nents, size_t length, gfp_t mem_flags, void *context)
{
    int k, result;
    
    result = usb_sg_init(io, dev, pipe, period, sg, nents, length, mem_flags);
    
    // Hijack the completion routine and context
    if (result == 0) {
        for (k = 0; k < io->entries; ++k) {
            io->urbs[k]->complete = qusb_sg_complete;
            io->urbs[k]->context = context;
        }
    }
    
    return result;
}



void qusb_sg_submit_async(struct usb_sg_request *io, struct usb_anchor *anchor)
{
    int i;
    int entries = io->entries;

    /* queue the urbs.  */
    spin_lock_irq(&io->lock);
    i = 0;
    while (i < entries && !io->status) {
        int retval;

        io->urbs[i]->dev = io->dev;
        usb_anchor_urb(io->urbs[i], anchor);            // NEW: ANCHORING
        retval = usb_submit_urb(io->urbs [i], GFP_ATOMIC);

        /* after we submit, let completions or cancelations fire;
        * we handshake using io->status.
        */
        spin_unlock_irq(&io->lock);
        switch (retval) {
            /* maybe we retrying will recover */
                case -ENXIO:    /* hc didn't queue this one */
                case -EAGAIN:
                case -ENOMEM:
                    io->urbs[i]->dev = NULL;
                    retval = 0;
                    yield();
                    break;

                    /* no error? continue immediately.
                    *
                    * NOTE: to work better with UHCI (4K I/O buffer may
                    * need 3K of TDs) it may be good to limit how many
                    * URBs are queued at once; N milliseconds?
                    */
                case 0:
                    ++i;
                    cpu_relax();
                    break;

                    /* fail any uncompleted urbs */
                default:
                    usb_unanchor_urb(io->urbs[i]);      // NEW: ANCHORING
                    io->urbs[i]->dev = NULL;
                    io->urbs[i]->status = retval;
                    dev_dbg(&io->dev->dev, "%s, submit --> %d\n",
                        __func__, retval);
                    qusb_sg_cancel(io);
        }
        spin_lock_irq(&io->lock);
        if (retval && (io->status == 0 || io->status == -ECONNRESET))
            io->status = retval;
    }
    io->count -= entries - i;
    if (io->count == 0)
        complete(&io->complete);
    spin_unlock_irq(&io->lock);

    // Do not wait for the requests to complete and do not clean ip
    // since the requests are not yet complete (do this in the completion
    // routine)
    //wait_for_completion(&io->complete);   <-- DON'T DO THIS!
    //qusb_sg_clean(io);                    <-- DON'T DO THIS!
}



void qusb_sg_async_wait(struct usb_sg_request *io)
{
    wait_for_completion(&io->complete);
}



/**
* usb_sg_cancel - stop scatter/gather i/o issued by usb_sg_wait()
* @io: request block, initialized with usb_sg_init()
*
* This stops a request after it has been started by usb_sg_wait().
* It can also prevents one initialized by usb_sg_init() from starting,
* so that call just frees resources allocated to the request.
*/
void qusb_sg_cancel(struct usb_sg_request *io)
{
    unsigned long flags;

    spin_lock_irqsave(&io->lock, flags);

    /* shut everything down, if it didn't already */
    if (!io->status) {
        int i;

        io->status = -ECONNRESET;
        spin_unlock(&io->lock);
        for (i = 0; i < io->entries; i++) {
            int retval;

            if (!io->urbs [i]->dev)
                continue;
            retval = usb_unlink_urb(io->urbs [i]);
            if (retval != -EINPROGRESS && retval != -EBUSY)
                dev_warn(&io->dev->dev, "%s, unlink --> %d\n",
                __func__, retval);
        }
        spin_lock(&io->lock);
    }
    spin_unlock_irqrestore(&io->lock, flags);
}



// Cancel due to timeout
void qusb_sg_timedout(struct usb_sg_request *io)
{
    unsigned long flags;

    spin_lock_irqsave(&io->lock, flags);

    /* shut everything down, if it didn't already */
    if (!io->status) {
        int i;

        io->status = -ETIMEDOUT;
        spin_unlock(&io->lock);
        for (i = 0; i < io->entries; i++) {
            int retval;

            if (!io->urbs [i]->dev)
                continue;
            retval = usb_unlink_urb(io->urbs [i]);
            if (retval != -EINPROGRESS && retval != -EBUSY)
                dev_warn(&io->dev->dev, "%s, unlink --> %d\n",
                __func__, retval);
        }
        spin_lock(&io->lock);
    }
    spin_unlock_irqrestore(&io->lock, flags);
}

#endif
