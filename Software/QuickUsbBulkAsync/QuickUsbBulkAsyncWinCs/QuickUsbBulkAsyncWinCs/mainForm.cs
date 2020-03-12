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

namespace QuickUsbBulkAsyncWinCs
{
    public partial class mainForm : Form
    {
        // DLL imports of the Windows API Performance Counters used to get extremely
        // accurate time measurements
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceFrequency(out long frequency);

        // The number of asynchronous requests to issue each time the read or write 
        // button is clicked.
        const int NumRequests = 8;

        // The byte size of the buffers.  To maximize performance the buffer size should
        // be as large as possible and a power of 2.
        const int BufferByteSize = 512 * 1024; // 512 KB

        // The number of threads to process the completion routines.  If NumThreads > 0, 
        // then you are multithreading and must make sure to process access to shared data
        // from within completion routines.  Note that all threads may not be used.  The
        // more processing a completion routine does the more likely another thread will
        // be used.
        ushort NumThreads = 1;

        // The number of processing threads allowed to execute completion routines
        // concurrenty.  If NumThreads > 0, you are multithreading and 
        // ThreadConcurrency must be at least 1.
        ushort ThreadConcurrency = 1;

        // Variables
        QuickUsb qusb;
        QuickUsb.Error qusbError;
        StringBuilder log = new StringBuilder();
        double TotalKiloBytes, InstKiloBytes;
        int ErrorCount;
        long freq, tStart, tEnd, tLast;
        int nextRequestToProcess, numPendingRequests;
        double overallRate;
        double instRate;

        // The data buffers, BulkStream objects, and tag objects.  Because these objects 
        // are passed to and used by unmanaged code in QuickUSB land, they must be handled
        // with care.  They cannot be relocated by the garbage collector and must either
        // be pinned or allocated in unmanaged memory.  Refer to the QuickUsbBulkAsyncCs
        // sample on how to properly use user 'tag' data.
        ushort[][] BufferArray = new ushort[NumRequests][];
        QuickUsb.BulkStream[] bulkStream = new QuickUsb.BulkStream[NumRequests];

        // We must keep an active reference to the delegate so it is not garbage collected.
        // This reference must minimally stay alive until all pending asynchronous requests
        // have completed.
        QuickUsb.BulkStreamCompletionRoutine cbDelegate;

        public mainForm()
        {
            InitializeComponent();

            // We must keep an active reference to the delegate so it is not garbage collected
            cbDelegate = new QuickUsb.BulkStreamCompletionRoutine(CompletionRoutine);

            for (int k = 0; k < NumRequests; ++k)
            {
                // Allocate the BulkStream objects
                bulkStream[k] = new QuickUsb.BulkStream();

                // Allocate the data objects
                BufferArray[k] = new ushort[BufferByteSize / 2];
            }
        }

        private void qusbControl_SelectionChanged(object sender, EventArgs e)
        {
            // If we were streaming, we need to stop
            if (numPendingRequests != 0)
            {
                // TODO: Wait for pendning requests to complete
            }

            // If a module if no longer selected
            if (qusbControl.SelectedModules.Length == 0)
            {
                bIssueRead.Enabled = false;
                bIssueWrite.Enabled = false;
            }
            else
            {
                bIssueRead.Enabled = true;
                bIssueWrite.Enabled = true;
            }
        }

        ////////////////////////////
        // The completion routine //
        ////////////////////////////
        void CompletionRoutine(QuickUsb.BulkStream BulkStream)
        {
            // Check for general errors
            if (BulkStream == null)
            {
                lock (log)
                {
                    log.AppendLine(String.Format("{0}: ERROR!  BulkStream error: {1}\n", DateTime.Now, QuickUsb.GetLastError()));
                }
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
                    log.AppendLine(String.Format("{0}: ERROR!  Request failed with error {1} ({2} of {3} bytes)\n", DateTime.Now, BulkStream.Error, BulkStream.BytesTransferred, BulkStream.BytesRequested));
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

            // Keep track of outstanding requests and the nuext request that will complete
            ++nextRequestToProcess;
            --numPendingRequests;

            // Time data rates
            QueryPerformanceCounter(out tEnd);
            double tElapsed = (double)(tEnd - tStart) / (double)(freq);
            double tElapsedInst = (double)(tEnd - tLast) / (double)(freq);

            overallRate = (TotalKiloBytes / 1024.0) / tElapsed;
            instRate = (numPendingRequests == 0) ? 0 : ((InstKiloBytes / 1024.0) / tElapsedInst);
            InstKiloBytes = 0;
            tLast = tEnd;

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

        private void bIssueRead_Click(object sender, EventArgs e)
        {
            bool qResult;
            qusb = qusbControl.SelectedModule;

            tbLog.AppendText(String.Format("********** Issuing {0} async reads **********\n", NumRequests));

            // Open the module
            qResult = qusb.Open();
            if (!qResult)
            {
                tbLog.AppendText(String.Format("Unable to open module\n", qusbError));
                return;
            }

            // Reset buttons
            bIssueRead.Enabled = false;
            bIssueWrite.Enabled = false;

            // Reset variables
            TotalKiloBytes = 0;
            ErrorCount = 0;
            numPendingRequests = 0;
            nextRequestToProcess = 0;

            // Set num threads and thread concurrency
            qResult = qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("SetNumAsyncThreads failed with error {0}\n", qusbError));

                bIssueRead.Enabled = false;
                bIssueWrite.Enabled = false;

                qusb.Close();
                return;
            }

            // Start throughput timer
            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            // Start streaming data to our user allocated data buffers
            for (int k = 0; k < NumRequests; ++k)
            {
                qResult = qusb.ReadBulkDataAsync(BufferArray[k], BufferByteSize, bulkStream[k], cbDelegate, IntPtr.Zero);

                // Check for an error issuing the request
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    tbLog.AppendText(String.Format("ReadBulkDataAsync failed with error {0}\n", qusbError));
                }
                else
                {
                    ++numPendingRequests;
                }
            }

            // Start timer
            timer.Start();
        }

        private void bIssueWrite_Click(object sender, EventArgs e)
        {
            bool qResult;
            qusb = qusbControl.SelectedModule;

            tbLog.AppendText(String.Format("********** Issuing {0} async writes **********\n", NumRequests));

            // Open the module
            qResult = qusb.Open();
            if (!qResult)
            {
                tbLog.AppendText(String.Format("Unable to open module\n", qusbError));
                return;
            }

            // Reset buttons
            bIssueRead.Enabled = false;
            bIssueWrite.Enabled = true;

            // Reset variables
            TotalKiloBytes = 0;
            ErrorCount = 0;
            numPendingRequests = 0;
            nextRequestToProcess = 0;

            // Set num threads and thread concurrency
            qResult = qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("SetNumAsyncThreads failed with error {0}\n", qusbError));

                bIssueRead.Enabled = false;
                bIssueWrite.Enabled = false;

                qusb.Close();
                return;
            }

            // Start throughput timer
            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            // Start streaming data to our user allocated data buffers
            for (int k = 0; k < NumRequests; ++k)
            {
                qResult = qusb.WriteBulkDataAsync(BufferArray[k], BufferByteSize, bulkStream[k], cbDelegate, IntPtr.Zero);

                // Check for an error issuing the request
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    tbLog.AppendText(String.Format("WriteBulkDataAsync failed with error {0}\n", qusbError));
                }
                else
                {
                    ++numPendingRequests;
                }
            }

            // Start timer
            timer.Start();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            if (numPendingRequests != 0)
            {
                // If we are not multithreading the stream in this example, we must
                // allow time for processing requests.  Processing requests simply
                // means checking if a request has completed, execute its completion
                // routine, and re-issue the requests.  QuickUsb.ProcessStream will
                // return when either any request has completed or the specified time
                // has elapsed.
                if (NumThreads == 0)
                {
                    // Check if the next request has completed
                    bool qResult = qusb.BulkWait(bulkStream[nextRequestToProcess], true); // Don't block
                    if (!qResult)
                    {
                        qusbError = qusb.LastError();
                        if (qusbError != QuickUsb.Error.NotCompleted)
                        {
                            Console.WriteLine("ProcessStream failed with error {0}", qusbError);
                        }
                    }
                }
            }
            else
            {
                // All the requests have completed
                bIssueRead.Enabled = true;
                bIssueWrite.Enabled = true;
                timer.Stop();

                // Close the module now that we're done with it
                qusb.Close();
            }

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
