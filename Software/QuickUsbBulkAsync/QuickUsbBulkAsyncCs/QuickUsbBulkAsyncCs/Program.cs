/* ========================================================================
	Title        : Program.cs
	Description  : QuickUSB Asynchronous Example
	Notes        : 
	History      :

	Copyright © 2012 Bitwise Systems.  All rights reserved.
	This software contains confidential information and trade secrets of
	Bitwise Systems and is protected by United States and international
	copyright laws.  Use, disclosure, or reproduction is prohibited
	without the prior express written permission of Bitwise Systems,
	except as agreed in the QuickUSB license agreement.

	Use, duplication or disclosure by the U.S. Government is subject to
	restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
	(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle
	Real, Suite E, Goleta, CA  93117.

	Bitwise Systems
	6489 Calle Real, Suite E
	Santa Barbara, CA  93117
	Voice: (805) 683-6469
	Fax  : (805) 683-4833
	Web  : www.bitwisesys.com
	email: support@bitwisesys.com
   ======================================================================== */
using System;
using System.Collections.Generic;
using System.Text;
using BitwiseSystems;
using System.Runtime.InteropServices;
using System.Threading;

namespace QuickUsbBulkAsyncCs
{
    class Program
    {
        // DLL imports of the Windows API Performance Counters used to get extremely
        // accurate time measurements
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceFrequency(out long frequency);

        // Static variables
        static public QuickUsb qusb;
        static public int TotalBytes;
        static public int TransCount;
        static public int RefCount;
        static public int ErrorCount;

        // User information associated with each BulkStream transaction (the 'tag').  
        // This information must contain only native data types as we must pin a
        // managed reference to pass it to unmanaged QuickUSB land.
        [StructLayout(LayoutKind.Sequential)]
        class BulkStreamTag {
            public int Id;
        };

        ///////////////////////////////////////
        // The BulkStream completion routine //
        ///////////////////////////////////////
        static void CompletionRoutine(QuickUsb.BulkStream BulkStream)
        {
            // Check for general errors
            if (BulkStream == null)
            {
                Console.WriteLine("BulkStream Error: {0}", QuickUsb.GetLastError());
                return;
            }

            // Note: If we needed to reteive a reference to the managed QuickUsb object 
            // associated with this BulkStream object because, for example, the 'qusb'
            // variable was inaccessible to this function, we would do the following:
            //QuickUsb qusb = QuickUsb.RetreiveManagedQuickUsbObject(BulkStream.Handle);

            // Retrieve our user data (the 'tag' in the async function call)
            GCHandle hTag = GCHandle.FromIntPtr(BulkStream.Tag);
            BulkStreamTag tag = (BulkStreamTag)hTag.Target;

            // Check for errors
            if (BulkStream.Error != 0) {
                Console.WriteLine("--> CR: ERROR! Request #{0} failed with error {1} ({2} of {3} bytes)", tag.Id, BulkStream.Error, BulkStream.BytesTransferred, BulkStream.BytesRequested);
                ++ErrorCount;
            } else {
                //Console.WriteLine("--> CR: Completed Request #{0} for {1} bytes on thread {2}", tag.Id, BulkStream.BytesTransferred, Thread.CurrentThread.ManagedThreadId);

                //
                // Now that this request has completed we may process the data
                //

                // Retreive a reference to the managed data buffer from the BulkStream.Buffer
                //byte[] buffer = QuickUsb.RetrieveManagedByteDataBuffer(BulkStream.Buffer);
                ushort[] buffer = QuickUsb.RetrieveManagedUShortDataBuffer(BulkStream.Buffer);
                if (buffer == null)
                {
                    Console.WriteLine(String.Format("{0}: ERROR!  Unable to retrieve managed data buffer", DateTime.Now));
                }

                // TODO: Process data
                //Thread.Sleep(50); // Simulate data processing (to force use of more threads)
            }

            // Keep a running count of bytes transferred
            TotalBytes += (int)BulkStream.BytesTransferred;

            // Keep track of the number of oustanding requests
            --RefCount;

            hTag.Free();
        }

        ///////////////////////////////////
        // QuickUSB Asynchronous Example //
        ///////////////////////////////////
        static void Main(string[] args)
        {
            // The number of data buffers to use.  An asynchronous data request is issued
            // for each data buffer, then the main thread waits for each request to complete
            // then issues a new asynchrnonous request using the same data buffer as the
            // completed request.  To maximize throughout, the number of buffers should be
            // at least equal to the number of threads (see the SETTING_THREADS setting).
            const int NumBuffers = 8;

            // The byte size of the buffers.  To maximize performance the buffer size should
            // be as large as possible and a power of 2.
            const int BufferByteSize = 512 * 1024; // 512 KB

            // The number of times to issue a request for each buffer.  
            const int LOOPS = 50;

            // Variables
            int k, j, Id = 0;
            string[] nameList;
            bool qResult;
            QuickUsb.Error qusbError;

            // Variables to track time and performace
            long tStart, tEnd, freq;
            double tElapsed;

            // The data buffers, BulkStream objects, and tag objects.  Because these objects 
            // are passed to and used by unmanaged code in QuickUSB land, they must be handled
            // with care.  They cannot be relocated by the garbage collector and must either
            // be pinned or allocated in unmanaged memory.
            ushort[][] BufferArray = new ushort[NumBuffers][];
            QuickUsb.BulkStream[] BulkStream = new QuickUsb.BulkStream[NumBuffers];
            BulkStreamTag[] tag = new BulkStreamTag[NumBuffers];

            // We must keep an active reference to the delegate so it is not garbage collected.
            // This reference must minimally stay alive until all pending asynchronous requests
            // have completed.
            QuickUsb.BulkStreamCompletionRoutine cbDelegate = new QuickUsb.BulkStreamCompletionRoutine(CompletionRoutine);
            
            // Query connected modules
            nameList = QuickUsb.FindModules();
            if (nameList.Length == 0) {
                Console.WriteLine("No modules found.");
                return;
            }

            // Open the first module
            qusb = new QuickUsb(nameList[0]);
            qusb.Open(nameList[0]);

            // Allocate buffers
            for (k = 0; k < NumBuffers; ++k)
            {
                // Allocate the data buffers in unmanaged memory
                BufferArray[k] = new ushort[BufferByteSize / 2];

                // Allocate the BulkStream objects
                BulkStream[k] = new QuickUsb.BulkStream();

                // Create the tag object as normal.  We will later get a pinned reference to this
                // object to use in issuing the request, that we will later be able to use to
                // reconstruct a reference to the managed tag object in the completion routine.
                // We can do this because the tag data is never accessed or modified in unmanaged 
                // land and only ever used in managed land.
                tag[k] = new BulkStreamTag();
            }

            // Start throughput timer
            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            Console.WriteLine("Acquiring data...please wait");

            // Aquire
            for (j = 0; j < (LOOPS + 1); ++j) {
                for (k = 0; k < NumBuffers; ++k) {
                    // If this is not the first loop, wait for the last transaction to complete
                    if (j != 0) {
                        // Wait for the transaction to complete.  Once this function returns successfully
                        // the completion routine has already been executed and the transaction is
                        // entirely complete.
                        qResult = qusb.BulkWait(BulkStream[k], false);
                        if (!qResult) {
                            qusbError = qusb.LastError();
                            Console.WriteLine("Request failed (QuickUSB Error: {0})", qusb.LastError());
                        }
                        else
                        {
                            // Now that this request has completed we may process the data here or in the
                            // completion routine, though it is better to perform all processing in the
                            // completion routine as they can be multi-threaded, allowing the main thread
                            // to simply issue and re-issue data requests.
                        }
                    }

                    // If this is not the last loop, issue a new transaction
                    if (j != LOOPS) {
                        //Console.WriteLine("Issuing Request #{0}", (Id + 1));

                        // Issue a new transaction
                        tag[k].Id = ++Id;
                        qResult = qusb.ReadBulkDataAsync(
                            BufferArray[k],
                            BufferByteSize, 
                            BulkStream[k], 
                            cbDelegate,
                            GCHandle.ToIntPtr(GCHandle.Alloc(tag[k])));

                        if (!qResult)
                        {
                            Console.WriteLine(String.Format("QuickUSB Error: {0}", qusb.LastError()));
                            qusb.Close();
                            return;
                        }
                        else
                        {
                            ++RefCount;
                            ++TransCount;
                        }
                    }
                }
            }

            // Stop the throughput timer
            QueryPerformanceCounter(out tEnd);
            tElapsed = (double)(tEnd - tStart) / (double)(freq);
            Console.WriteLine();
            
            Console.WriteLine("Time elapsed: {0:0.000} s", tElapsed);
            Console.WriteLine("Total bytes transferred: {0:0.000} MB", ((float)TotalBytes / (1024.0 * 1024.0)));
            Console.WriteLine("Data rate: {0:0.000} MS/s", ((TotalBytes / (1024.0 * 1024.0)) / tElapsed));
            Console.WriteLine();

            // Close the module
            qResult = qusb.Close();
            if (!qResult)
            {
                Console.WriteLine("QuickUSB Error: {0}", qusb.LastError());
                return;
            }

            // Report any leaks or errors
            Console.WriteLine("{0} transaction(s) issued in total", TransCount);
            Console.WriteLine("{0} transaction(s) failed", ErrorCount);
            Console.WriteLine("{0} transaction(s) are still outstanding", RefCount);

            return;
        }
    }
}
