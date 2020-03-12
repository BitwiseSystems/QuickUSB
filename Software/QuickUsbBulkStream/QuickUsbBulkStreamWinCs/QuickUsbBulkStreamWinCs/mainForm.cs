/* ========================================================================
	Title        : mainForm.cs
	Description  : QuickUSB Bulk Streaming Example with GUI
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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using BitwiseSystems;
using System.Runtime.InteropServices;
using System.Threading;

namespace QuickUsbBulkStreamWinCs
{
    public partial class mainForm : Form
    {
        // DLL imports of the Windows API Performance Counters used to get extremely
        // accurate time measurements
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceFrequency(out long frequency);

        // The number of data buffers to use.  An asynchronous data request is issued
        // for each data buffer, then the main thread waits for each request to complete
        // then issues a new asynchrnonous request using the same data buffer as the
        // completed request.  To maximize throughout, the number of buffers should be
        // at least equal to the number of threads (see the SETTING_THREADS setting).
        const int NumBuffers = 8;

        // The byte size of the buffers.  To maximize performance the buffer size should
        // be as large as possible and a power of 2.
        const int BufferByteSize = 512 * 1024; // 512 KB

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
        QuickUsb.Error qusbError;
        StringBuilder log = new StringBuilder();
        double TotalKiloBytes, InstKiloBytes;
        int ErrorCount;
        bool streaming = false;
        long freq, tStart, tEnd, tLast;
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
        QuickUsb.BulkStreamCompletionRoutine cbDelegate;

        public mainForm()
        {
            InitializeComponent();

            // We must keep an active reference to the delegate so it is not garbage collected
            cbDelegate = new QuickUsb.BulkStreamCompletionRoutine(CompletionRoutine);

            // Allocate buffers
            if (UserAllocatedBuffers)
            {
                for (int k = 0; k < NumBuffers; ++k)
                {
                    // Allocate the data buffers in unmanaged memory
                    BufferArray[k] = new ushort[BufferByteSize / 2];
                }
            }
        }

        private void qusbControl_SelectionChanged(object sender, EventArgs e)
        {
            // If we were streaming, we need to stop
            if (streaming)
            {
                bStopStreaming_Click(sender, e);
            }
            bStopStreaming.Enabled = false;

            // If a module if no longer selected
            if (qusbControl.SelectedModules.Length == 0)
            {
                bStartStreaming.Enabled = false;
            }
            else
            {
                bStartStreaming.Enabled = true;
            }
        }

        ///////////////////////////////////////
        // The BulkStream completion routine //
        ///////////////////////////////////////
        void CompletionRoutine(QuickUsb.BulkStream BulkStream)
        {
            // Check for general errors
            if (BulkStream == null)
            {
                log.AppendLine(String.Format("{0}: ERROR! BulkStream error: {1}\n", DateTime.Now, QuickUsb.GetLastError()));
                return;
            }

            // Note: If we needed to reteive a reference to the managed QuickUsb object 
            // associated with this BulkStream object because, for example, the 'qusb'
            // variable was inaccessible to this function, we would do the following:
            //QuickUsb qusb = QuickUsb.RetreiveManagedQuickUsbObject(BulkStream.Handle);

            // Check for errors
            if (BulkStream.Error != 0)
            {
                lock (log)
                {
                    log.AppendLine(String.Format("{0}: ERROR! Request failed with error {0} ({1} of {2} bytes)\n", DateTime.Now, BulkStream.BytesTransferred, BulkStream.BytesRequested));
                }
                ++ErrorCount;
            }
            else
            {
                lock (log)
                {
                    log.AppendLine(String.Format("{0}: Completed Request for {1} bytes", DateTime.Now, BulkStream.BytesTransferred));
                }
            }

            // Keep a running count of bytes transferred
            TotalKiloBytes += (double)(BulkStream.BytesTransferred) / 1024.0;
            InstKiloBytes += (double)(BulkStream.BytesTransferred) / 1024.0;

            //
            // Now that this request has completed we may process the data
            //

            // Retreive a reference to the managed data buffer from the BulkStream.Buffer
            //byte[] buffer = QuickUsb.RetrieveManagedByteDataBuffer(BulkStream.Buffer);
            ushort[] buffer = QuickUsb.RetrieveManagedUShortDataBuffer(BulkStream.Buffer);
            if (buffer == null)
            {
                lock (log)
                {
                    log.AppendLine(String.Format("{0}: ERROR!  Unable to retrieve managed data buffer", DateTime.Now));
                }
            }

            // TODO: Process data
            //Thread.Sleep(50); // Simulate data processing (to force use of more threads)
        }

        private void bStartStreaming_Click(object sender, EventArgs e)
        {
            bool qResult;
            qusb = qusbControl.SelectedModule;

            tbLog.AppendText(String.Format("********** Starting stream **********\n", qusbError));

            // Open the module
            qResult = qusb.Open();
            if (!qResult)
            {
                tbLog.AppendText(String.Format("Unable to open module\n", qusbError));
                return;
            }

            // Start the data stream
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

            // Check for an error starting the stream
            if (!qResult)
            {
                qusbError = qusb.LastError();
                qusb.Close();
                tbLog.AppendText(String.Format("ReadBulkDataStartStream failed with error {0}\n", qusbError));
                return;
            }

            // Reset buttons
            bStartStreaming.Enabled = false;
            bStopStreaming.Enabled = true;

            // Reset variables
            TotalKiloBytes = 0;
            ErrorCount = 0;
            streaming = true;

            // Start throughput timer
            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            // Start timer
            timer.Start();
        }

        private void bStopStreaming_Click(object sender, EventArgs e)
        {
            bool qResult;

            tbLog.AppendText(String.Format("********** Stopping stream **********\n", qusbError));

            // Stop the timer
            timer.Stop();

            // Stop the data stream
            qResult = qusb.StopStream(streamID, false);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("BulkDataStopStream failed with error {0}\n", qusbError));
            }

            // Close the module
            qResult = qusb.Close();
            if (!qResult)
            {
                Console.WriteLine("QuickUSB Error: {0}", qusb.LastError());
                return;
            }

            // Reset variables
            streaming = false;

            // Reset buttons
            bStartStreaming.Enabled = true;
            bStopStreaming.Enabled = false;
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            // If we are not multithreading the stream in this example, we must
            // allow time for processing requests.  Processing requests simply
            // means checking if a request has completed, execute its completion
            // routine, and re-issue the requests.  QuickUsb.ProcessStream will
            // return when either any request has completed or the specified time
            // has elapsed.
            if (NumThreads == 0)
            {
                bool qResult = qusb.ProcessStream(streamID, 50);
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    Console.WriteLine("ProcessStream failed with error {0}", qusbError);
                }
            }

            // Time data rates
            QueryPerformanceCounter(out tEnd);
            double tElapsed = (double)(tEnd - tStart) / (double)(freq);
            double tElapsedInst = (double)(tEnd - tLast) / (double)(freq);

            double overallRate = (TotalKiloBytes / 1024.0) / tElapsed;
            double instRate = (InstKiloBytes / 1024.0) / tElapsedInst;
            InstKiloBytes = 0;
            tLast = tEnd;

            // Report our data throughput
            dataRate.Text = String.Format("Data Rate: {0:0.0} MB/s Overall, {1:0.0} MB/s Instantaneously", overallRate, instRate);

            // Dump data to log
            lock (log)
            {
                tbLog.AppendText(log.ToString());
                log = new StringBuilder();
            }
        }
    }
}
