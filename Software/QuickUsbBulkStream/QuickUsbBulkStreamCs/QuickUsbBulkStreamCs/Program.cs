/* ========================================================================
	Title        : Program.cs
	Description  : QuickUSB Bulk Streaming Example
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
using System.Diagnostics;

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

        // Class variables
        static public long TotalBytes, TransCount;
        static public int ErrorCount;
        static long freq;
        static double tElapsed;
        static long tStart, tEnd;

        // Keep track of which threads are servicing the completion routine
        static Dictionary<int, int> ThreadIds = new Dictionary<int, int>();

        ///////////////////////////////////////
        // The BulkStream completion routine //
        ///////////////////////////////////////
        static void CompletionRoutine(QuickUsb.BulkStream BulkStream)
        {
            // Check for general errors
            if (BulkStream == null)
            {
                Console.WriteLine("Bulkstream error: {0}", QuickUsb.GetLastError());
                return;
            }

            // Note: If we needed to reteive a reference to the managed QuickUsb object 
            // associated with this BulkStream object because, for example, the 'qusb'
            // variable was inaccessible to this function, we would do the following:
            //QuickUsb qusb = QuickUsb.RetreiveManagedQuickUsbObject(BulkStream.Handle);

            // Check for errors
            if (BulkStream.Error != 0) {
                Console.WriteLine("--> ERROR! (" + DateTime.Now.ToString() + ") Request failed with error {0} ({1} of {2} bytes)", BulkStream.Error, BulkStream.BytesTransferred, BulkStream.BytesRequested);
                ++ErrorCount;
            } else {
                //Console.WriteLine("--> CR: Completed Request for {0} bytes", BulkStream.BytesTransferred);
            }

            // Keep a running count of bytes transferred
            TotalBytes += (int)BulkStream.BytesTransferred;
            ++TransCount;

            //
            // Now that this request has completed we may process the data
            //

            // Retreive a reference to the managed data buffer from the BulkStream.Buffer
            //byte[] buffer = QuickUsb.RetrieveManagedByteDataBuffer(BulkStream.Buffer);
            ushort[] buffer = QuickUsb.RetrieveManagedUShortDataBuffer(BulkStream.Buffer);
            if (buffer == null)
            {
                Console.WriteLine("ERROR!  Unable to retrieve managed data buffer");
            }

            //Thread.Sleep(50); // Simulate data processing (to force the use of more threads)

            // Keep track of the thread that is executing this completion routine
            int id = Thread.CurrentThread.ManagedThreadId;
            if (!ThreadIds.ContainsKey(id))
            {
                if (id == 0)
                {
                    return;
                }
                ThreadIds[id] = 1;
            }
            else
            {
                ++ThreadIds[id];
            }
        }



        /////////////////////////////////////
        // QuickUSB Bulk Streaming Example //
        /////////////////////////////////////
        static void Main(string[] args)
        {
            // The number of data buffers to use.  An asynchronous data request is issued
            // for each data buffer, then the main thread waits for each request to complete
            // then issues a new asynchrnonous request using the same data buffer as the
            // completed request.  To maximize throughout, the number of buffers should be
            // at least equal to the number of threads (see the SETTING_THREADS setting).
            uint NumBuffers = 4;

            // The byte size of the buffers.  To maximize performance the buffer size should
            // be as large as possible and a power of 2.
            uint BufferByteSize = 512 * 1024; // 512 KB

            // The number of threads to process the data stream.  If NumThreads > 0, then
            // you are multithreading and must make sure to process access to shared data
            // from within completion routines.  Note that all threads may not be used.  The
            // more processing a completion routine does the more likely another thread will
            // be used.
            ushort NumThreads = 4;

            // The number of processing threads allowed to execute completion routines
            // concurrenty.  If NumThreads > 0, you are multithreading and 
            // ThreadConcurrency must be at least 1.
            ushort ThreadConcurrency = 2;

            // The Streaming API (for data reads) can use either data buffers allocated
            // internally by the QuickUSB API or data buffers allocate by the user.  This
            // example demonstrates how to do either.
            bool UserAllocatedBuffers = true;

            // Variables
            QuickUsb qusb;
            int k;
            string[] nameList;
            bool qResult;
            QuickUsb.Error qusbError;
            byte streamID;

            // The data buffers, BulkStream objects, and tag objects.  Because these objects 
            // are passed to and used by unmanaged code in QuickUSB land, they must be handled
            // with care.  They cannot be relocated by the garbage collector and must either
            // be pinned or allocated in unmanaged memory.  Refer to the QuickUsbBulkAsyncCs
            // sample on how to properly use user 'tag' data.
            ushort[][] BufferArray = new ushort[NumBuffers][];

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
            string name = nameList[0];
            Console.WriteLine("Using Device: " + name);
            qusb = new QuickUsb(name);
            qResult = qusb.Open(name);
            if (!qResult)
            {
                qusb.Close();
                return;
            }
            
            // Display the ID of the currently executing thread (the main thread)
            Console.WriteLine("Number of processors: " + Environment.ProcessorCount.ToString());
            Console.WriteLine("Main thread id: " + Thread.CurrentThread.ManagedThreadId.ToString());
            Console.WriteLine();

            // Allocate buffers
            if (UserAllocatedBuffers)
            {
                for (k = 0; k < NumBuffers; ++k)
                {
                    // Allocate the data buffers in unmanaged memory
                    BufferArray[k] = new ushort[BufferByteSize / 2];
                }
            }

            // Start throughput timer
            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            // Start the data stream
            Console.WriteLine("Acquiring data.  Press any key to stop...");
            if (UserAllocatedBuffers)
            {
                // Start streaming data to our user allocated data buffers
                qResult = qusb.ReadBulkDataStartStream(BufferArray, NumBuffers, BufferByteSize, cbDelegate, IntPtr.Zero, out streamID, NumThreads, ThreadConcurrency);
            }
            else
            {
                // Start streaming data to data buffers that will be allocated interally by the Streaming API
                qResult = qusb.ReadBulkDataStartStream(IntPtr.Zero, NumBuffers, BufferByteSize, cbDelegate, IntPtr.Zero, out streamID, NumThreads, ThreadConcurrency);
            }
            if (!qResult)
            {
                qusbError = qusb.LastError();
                Console.WriteLine("ReadBulkDataStartStream failed with error {0}", qusbError);
            }
            else
            {
                // Wait will data gets acquired and processed in the background
                while (!Console.KeyAvailable)
                {
                    //---------------------------------------------------------
                    // Performing some background tasks
                    //---------------------------------------------------------

                    // If we are not multithreading the stream in this example, we must
                    // allow time for processing requests.  Processing requests simply
                    // means checking if a request has completed, execute its completion
                    // routine, and re-issue the requests.  QuickUsb.ProcessStream will
                    // return when either any request has completed or the specified time
                    // has elapsed.
                    if (NumThreads == 0)
                    {
                        qResult = qusb.ProcessStream(streamID, 50);
                        if (!qResult)
                        {
                            qusbError = qusb.LastError();
                            Console.WriteLine("ProcessStream failed with error {0}", qusbError);
                            break;
                        }
                    }
                    else
                    {
                        // Our background threads are processing the stream so we will
                        // just not hog the processor
                        Thread.Sleep(50);
                    }
                }

                // Stop the data stream
                Console.WriteLine("Stopping data aquisition");
                qResult = qusb.StopStream(streamID, false);
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    Console.WriteLine("BulkDataStopStream failed with error {0}", qusbError);
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

            // Report any leaks or errors
            Console.WriteLine("{0} transaction(s) failed", ErrorCount);
            Console.WriteLine();
            foreach (KeyValuePair<int, int> kv in ThreadIds)
            {
                Console.WriteLine("Thread #{0} used for {1} completion routine callbacks", kv.Key, kv.Value);
            }

            // Close the module
            qResult = qusb.Close();
            if (!qResult)
            {
                Console.WriteLine("QuickUSB Error: {0}", qusb.LastError());
                return;
            }

            return;
        }
    }
}
