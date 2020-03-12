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
using System.Linq;

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

        // FPGA file for eval board
        const string FpgaFile = @"C:\Projects\Bitwise\QuickUSB\Hardware\EvaluationBoard\Samples\Rev A\Verilog\QUSBEVB_REVA_EP2C20_BERT\QUSBEVB_REVA_EP2C20_BERT.rbf";

        enum DataTestType { None, SyncRead, SyncWrite, SyncLoopback, AsyncRead, AsyncWrite, AsyncLoopback, LegacyAsyncRead, LegacyAsyncWrite,/*, LegacyAsyncLoopback*/ StreamingRead, StreamingWrite, StreamingLoopback };

        // Variables
        QuickUsb qusb;
        QuickUsb.Error qusbError;
        StringBuilder log = new StringBuilder();
        long BytesRead, BytesWritten;
        long freq, tStart, tEnd;
        int tTestStart, tLastUpdate;
        ulong inTransCount, outTransCount;
        ulong fpgaBertCount, softwareBertCount;
        byte OutStreamID, InStreamID;
        uint NumBuffers, BufferByteSize;
        bool UserAllocatedBuffers;
        ushort NumThreads, ThreadConcurrency;
        DataTestType TestType;
        bool RunningTest, RunningTestSuite, CompletionRoutineFailure;
        ushort NextInLFSR, NextOutLFSR;
        QuickUsb.PercentCompleteDelegate percentCompleteDelegate;
        ulong nextInRequestID, nextOutRequestID;
        int Failures = 0, NumTests;

        ushort[][] InBufferArray;
        ushort[][] OutBufferArray;
        GCHandle[] hInBufferArray;
        GCHandle[] hOutBufferArray;

        Queue<ushort[]> PendingBuffersToVerify = new Queue<ushort[]>();

        // We must keep an active reference to the delegate so it is not garbage collected.
        // This reference must minimally stay alive until all pending asynchronous requests
        // have completed.
        QuickUsb.BulkStreamCompletionRoutine callbackDelegate;

        public mainForm()
        {
            InitializeComponent();
            percentCompleteDelegate = new QuickUsb.PercentCompleteDelegate(FpgaUploadUpdate);

            // Retreive clock frequency
            QueryPerformanceFrequency(out freq);

            // We must keep an active reference to the delegate so it is not garbage collected
            callbackDelegate = new QuickUsb.BulkStreamCompletionRoutine(CompletionRoutine);

            // Update stats labels
            Test_UpdateStats();

            // Select the default test type
            cbTestType.SelectedItem = GetTestType(DataTestType.StreamingLoopback);
            nNumThreads.Value = 1;
        }

        void GetNextInLFSR(ushort[] data, int bytes)
        {
            for (int k = 0; k < bytes; ++k)
            {
                data[k] = NextInLFSR;
                NextInLFSR = GetNextLFSR(data[k]);
            }
        }

        void GetNextOutLFSR(ushort[] data, int bytes)
        {
            for (int k = 0; k < bytes; ++k)
            {
                data[k] = NextOutLFSR;
                NextOutLFSR = GetNextLFSR(data[k]);
            }
        }

        ushort GetNextLFSR(ushort prev)
        {
            int bit16 = (prev >> 15) & 1;
            int bit14 = (prev >> 13) & 1;
            int bit13 = (prev >> 12) & 1;
            int bit11 = (prev >> 10) & 1;

            ushort feedback = (ushort)(~(bit16 ^ bit14 ^ bit13 ^ bit11) & 1);
            return (ushort)((prev << 1) | feedback);
        }

        string GetTestType(DataTestType type)
        {
            switch (type)
            {
                case DataTestType.SyncRead:
                    return "Synchronous Read";
                case DataTestType.SyncWrite:
                    return "Synchronous Write";
                case DataTestType.SyncLoopback:
                    return "Synchronous Loopback";
                case DataTestType.AsyncRead:
                    return "Asynchronous Read";
                case DataTestType.AsyncWrite:
                    return "Asynchronous Write";
                case DataTestType.AsyncLoopback:
                    return "Asynchronous Loopback";
                case DataTestType.LegacyAsyncRead:
                    return "Asynchronous Read (Legacy)";
                case DataTestType.LegacyAsyncWrite:
                    return "Asynchronous Write (Legacy)";
                //case DataTestType.LegacyAsyncLoopback:
                //    return "Asynchronous Loopback (Legacy)";
                case DataTestType.StreamingRead:
                    return "Streaming Read";
                case DataTestType.StreamingWrite:
                    return "Streaming Write";
                case DataTestType.StreamingLoopback:
                    return "Streaming Loopback";
                default:
                    return null;
            }
        }

        DataTestType GetTestType(string type)
        {
            switch (type)
            {
                case "Synchronous Read":
                    return DataTestType.SyncRead;
                case "Synchronous Write":
                    return DataTestType.SyncWrite;
                case "Synchronous Loopback":
                    return DataTestType.SyncLoopback;
                case "Asynchronous Read":
                    return DataTestType.AsyncRead;
                case "Asynchronous Write":
                    return DataTestType.AsyncWrite;
                case "Asynchronous Loopback":
                    return DataTestType.AsyncLoopback;
                case "Asynchronous Read (Legacy)":
                    return DataTestType.LegacyAsyncRead;
                case "Asynchronous Write (Legacy)":
                    return DataTestType.LegacyAsyncWrite;
                //case "Asynchronous Loopback (Legacy)":
                //    return DataTestType.LegacyAsyncLoopback;
                case "Streaming Read":
                    return DataTestType.StreamingRead;
                case "Streaming Write":
                    return DataTestType.StreamingWrite;
                case "Streaming Loopback":
                    return DataTestType.StreamingLoopback;
                default:
                    return DataTestType.None;
            }
        }

        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (RunningTest)
            {
                MessageBox.Show("A test is currently in process.  To shutdown properly, first stop the test then close the application as normal.", "Shutdown Error");
                e.Cancel = true;
                return;
            }

            // Free GC handles
            if (hInBufferArray != null)
            {
                for (int k = 0; k < hInBufferArray.Length; ++k)
                {
                    hInBufferArray[k].Free();
                }
            }
            if (hOutBufferArray != null)
            {
                for (int k = 0; k < hOutBufferArray.Length; ++k)
                {
                    hOutBufferArray[k].Free();
                }
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            lock (log)
            {
                tbLog.AppendText(log.ToString());
                log = new StringBuilder();
            }

            // Read the config register
            if (qusb != null)
            {
                bool qResult;
                byte[] inData = new byte[2];
                ushort length = 2;
                qResult = qusb.ReadCommand(0, inData, ref length);
                if (!qResult)
                {
                    tbLog.AppendText(String.Format("QuickUsb.ReadCommand() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                }

                // If the read value is different, update the value to reflect it
                uint val = (uint)((inData[1] << 8) | inData[0]);
                if (val != btConfig.Value)
                {
                    btConfig.ValueChanged -= btConfig_ValueChanged;
                    btConfig.Value = val;
                    btConfig.ValueChanged += btConfig_ValueChanged;
                }
            }

            cbIsOpened.Checked = (qusb != null) && qusb.IsOpened;
        }

        private void qusbControl_SelectionChanged(object sender, EventArgs e)
        {
            UpdateControlEnables();

            // Have the progress bar indicate whether the FPGA is configured
            bool qResult;
            bool configured;
            qusb = qusbControl.SelectedModule;
            if (qusb != null)
            {
                qResult = qusb.IsFpgaConfigured(out configured);
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    tbLog.AppendText(String.Format("Unable to determine if FPGA is configured: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                }
                else
                {
                    pbProgressBar.Value = (configured ? 100 : 0);
                }
            }

        }

        private void cbTestType_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateControlEnables();
        }

        private void bPowerCycleFpga_Click(object sender, EventArgs e)
        {
            bool qResult;

            // Turn FPGA power off
            qResult = qusb.WritePortDir(QuickUsb.Port.A, 0x81);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePortDir() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return;
            }
            qResult = qusb.WritePort(QuickUsb.Port.A, 0x00);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePort() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return;
            }

            Thread.Sleep(100);

            // Turn FPGA power on and place FPGA in reset
            qResult = qusb.WritePortDir(QuickUsb.Port.A, 0x81);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePortDir() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return;
            }
            qResult = qusb.WritePort(QuickUsb.Port.A, 0x80);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePort() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return;
            }

            pbProgressBar.Value = 0;
        }

        ///////////////////////////////////////
        // The BulkStream completion routine //
        ///////////////////////////////////////
        void CompletionRoutine(QuickUsb.BulkStream BulkStream)
        {
            int time = Environment.TickCount - tTestStart;

            // Check for general errors
            if (BulkStream == null)
            {
                lock (log)
                {
                    log.AppendFormat("{0}: ERROR! BulkStream is null: {1} - {2}" + Environment.NewLine, time, QuickUsb.GetLastError(), QuickUsb.GetLastDriverError());
                }
                return;
            }

            string reqType = ((BulkStream.StreamType & 0x10) != 0) ? "IN" : "OUT";

            // Check for errors
            if (BulkStream.Error != 0)
            {
                lock (log)
                {
                    log.AppendFormat("{0}: ERROR! {1} Request failed: {2} - {3} ({4} of {5} bytes)" + Environment.NewLine, time, reqType, BulkStream.Error, QuickUsb.GetLastDriverError(), BulkStream.BytesTransferred, BulkStream.BytesRequested);

                    // If the request failed for any reason other than a timeout, we need to stop the test
                    if (BulkStream.Error != QuickUsb.Error.Timeout)
                    {
                        CompletionRoutineFailure = true;
                        ++Failures;
                    }
                    else
                    {
                        // A timeout error occurred on the stream.  By default, the stream shuts down after any
                        // error, unless we clear the error to allow the stream to continue.  Since timeouts are
                        // not fatal errors, we'll have the stream continue onwards.
                        BulkStream.Error = QuickUsb.Error.NoError;
                    }
                }

                // To prevent lockups, if a request fails we still must increment our request id counter so
                // that we may continue to process subsequent requests
                if (reqType == "IN")
                {
                    ++nextInRequestID; // TODO: Handle recovery cases
                }
                if (reqType == "OUT")
                {
                    ++nextOutRequestID; // TODO: Handle recovery cases
                }
            }
            else
            {
                if (reqType == "IN") // IN?
                {
                    // If we're multi-threading then it's possible that our completion routines are executing 
                    // out-of-order.  We need to make sure that we read the appropriate data in the correct 
                    // order, so if this routine is processing a request a little too early, then we need to 
                    // wait until other threads process requests and it becomes our turn.
                    if (NumThreads > 0)
                    {
                        int tStart = Environment.TickCount;
                        while ((nextInRequestID != BulkStream.RequestID) && (Environment.TickCount - tStart) < 5000)
                        {
                            // Yield processing to another thread.  This is better than sleeping a finite amount
                            // of time and better than an empty loop as we don't waste processing time we don't need;
                            Thread.Sleep(0);
                        }

                        // If (nextRequestID != BulkStream.RequestID) never becomes true, then it is an indication
                        // that there is some internal issue with keeping track of the correct request id to process.
                        if (nextInRequestID != BulkStream.RequestID)
                        {
                            lock (log)
                            {
                                log.AppendLine("Internal Error: Thread is deadlocked and unable to process packet because it is out of order");
                            }
                            return;
                        }

                        // If we're performing a streaming loopback
                        if (BulkStream.Tag == (IntPtr)1)
                        {
                            // Now that it is our turn, we must also make sure that some writes have occurred before our read
                            while ((PendingBuffersToVerify.Count <= 2) && RunningTest)
                            {
                                // Yield processing to another thread.  This is better than sleeping a finite amount
                                // of time and better than an empty loop as we don't waste processing time we don't need.
                                Thread.Sleep(0);
                            }
                        }
                    }
                    else if (nextInRequestID != BulkStream.RequestID)
                    {
                        lock (log)
                        {
                            log.AppendLine("Internal Error: Data packet received out of order during single-threaded execution");
                        }
                    }

                    if (UserAllocatedBuffers)
                    {
                        // We are using buffers that we have allocated and pinned in managed land but
                        // we get the buffers pinnned IntPtr address, so we need to find the managed
                        // buffer that the IntPtr refers to.
                        bool found = false;
                        int k = 0;
                        for (k = 0; k < NumBuffers; ++k)
                        {
                            if (BulkStream.Buffer == hInBufferArray[k].AddrOfPinnedObject())
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            // Internal problem
                            lock (log)
                            {
                                log.AppendFormat("{0}: ERROR! IN buffer not found." + Environment.NewLine, time);
                            }
                        }

                        // If we're performing a streaming loopback
                        if (BulkStream.Tag == (IntPtr)1)
                        {
                            // Ensure that data exists in our queue
                            if (PendingBuffersToVerify.Count == 0)
                            {
                                lock (log)
                                {
                                    log.AppendLine(string.Format("Internal Error: Completed read before a write ({0} - {1})", qusb.LastError(), qusb.LastDriverError()));
                                }
                            }

                            // Verify the read data
                            if (RunningTest && PendingBuffersToVerify.Count > 0)
                            {
                                softwareBertCount += CountBitErrors(InBufferArray[k], PendingBuffersToVerify.Dequeue());
                            }
                        }
                        else
                        {
                            // Get the next set of data to data to verify against
                            ushort[] buffer = new ushort[BulkStream.BytesTransferred / 2];
                            GetNextInLFSR(buffer, buffer.Length);

                            // Verify the read data
                            if (RunningTest)
                            {
                                softwareBertCount += CountBitErrors(InBufferArray[k], buffer);
                            }
                        }
                    }
                    else
                    {
                        // The read data buffer has been allocated internally by the QuickUSB API.  The
                        // easiest way to access its data is to cast the IntPtr to ushort*
                        unsafe
                        {
                            ushort* data = (ushort*)BulkStream.Buffer;

                            // If we're performing a streaming loopback
                            if (BulkStream.Tag == (IntPtr)1)
                            {
                                // Ensure that data exists in our queue
                                if (PendingBuffersToVerify.Count == 0)
                                {
                                    lock (log)
                                    {
                                        log.AppendLine(string.Format("Internal Error: Completed read before a write ({0} - {1})", qusb.LastError(), qusb.LastDriverError()));
                                    }
                                }

                                // Verify the read data
                                if (RunningTest && PendingBuffersToVerify.Count > 0)
                                {
                                    softwareBertCount += CountBitErrors(data, PendingBuffersToVerify.Dequeue());
                                }
                            }
                            else
                            {
                                // Get the next set of data to data to verify against
                                ushort[] buffer = new ushort[BulkStream.BytesTransferred / 2];
                                GetNextInLFSR(buffer, buffer.Length);

                                // Verify the read data
                                if (RunningTest)
                                {
                                    softwareBertCount += CountBitErrors(data, buffer);
                                }
                            }
                        }
                    }

                    ++nextInRequestID;
                    ++inTransCount;
                    BytesRead += BulkStream.BytesTransferred;

                    // If we're stopping a streaming test, then pause the stream here so that
                    // requests are not re-issued and don't cause shutdown problems
                    if (!RunningTest && (BulkStream.Tag == (IntPtr)1))
                    {
                        bool qResult = qusb.PauseStream(InStreamID, true);
                        if (!qResult)
                        {
                            tbLog.AppendText(String.Format("QuickUsb.PauseStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                    }
                }
                else // OUT request
                {
                    bool found = false;
                    for (int k = 0; k < NumBuffers; ++k)
                    {
                        if (BulkStream.Buffer == hOutBufferArray[k].AddrOfPinnedObject())
                        {
                            // If we're multi-threading then it's possible that our completion routines are executing 
                            // out-of-order.  We need to make sure that we write the appropriate data in the correct 
                            // order, so if this routine is processing a request a little too early, then we need to 
                            // wait until other threads process requests and it becomes our turn.
                            if (NumThreads > 0)
                            {
                                while (nextOutRequestID != BulkStream.RequestID)
                                {
                                    // Yield processing to another thread.  This is better than sleeping a finite amount
                                    // of time and better than an empty loop as we don't waste processing time we don't need
                                    Thread.Sleep(0);
                                }

                                // Now that it is our turn, we must also make sure that we don't try to perform too many
                                // writes before our reads have a chance to get processed
                                while ((PendingBuffersToVerify.Count > 2) && RunningTest)
                                {
                                    // Yield processing to another thread.  This is better than sleeping a finite amount
                                    // of time and better than an empty loop as we don't waste processing time we don't need;
                                    Thread.Sleep(0);
                                }
                            }
                            else if (nextOutRequestID != BulkStream.RequestID)
                            {
                                log.AppendLine("Internal Error: Data packet received out of order during single-threaded execution");
                            }

                                // If we're performing a streaming loopback
                            if (BulkStream.Tag == (IntPtr)1)
                            {
                                // We need to store this completed data buffer so we may verify against it later
                                // once we have read it back
                                ushort[] data = new ushort[OutBufferArray[k].Length];
                                OutBufferArray[k].CopyTo(data, 0);
                                PendingBuffersToVerify.Enqueue(data);
                            }

                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);
                            ++nextOutRequestID;

                            found = true;
                        }
                    }

                    if (!found)
                    {
                        // Internal problem
                        lock (log)
                        {
                            log.AppendFormat("{0}: ERROR! OUT buffer not found." + Environment.NewLine, time);
                        }
                    }

                    ++outTransCount;
                    BytesWritten += BulkStream.BytesTransferred;

                    // If we're stopping a streaming test, then pause the stream here so that
                    // requests are not re-issued and don't cause shutdown problems
                    if (!RunningTest && (BulkStream.Tag == (IntPtr)1))
                    {
                        bool qResult = qusb.PauseStream(OutStreamID, true);
                        if (!qResult)
                        {
                            tbLog.AppendText(String.Format("QuickUsb.PauseStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                    }
                }

                //lock (log)
                //{
                //    log.AppendFormat("{0}: Completed {1} Request for {2} bytes" + Environment.NewLine, time, reqType, BulkStream.BytesTransferred);
                //}
            }
        }

        string GetFriendlyDataSize(double x)
        {
            if (x < 1024.0)
            {
                return String.Format("{0:0} Bytes", x);
            }
            else if (x < (1024.0 * 1024.0))
            {
                double KB = x / (1024.0);
                return String.Format("{0:0.00} KB", KB);
            }
            else if (x < (1024.0 * 1024.0 * 1024.0))
            {
                double MB = x / (1024.0 * 1024.0);
                return String.Format("{0:0.00} MB", MB);
            }
            else
            {
                double GB = x / (1024.0 * 1024.0 * 1024.0);
                return String.Format("{0:0.00} GB", GB);
            }
        }

        void UpdateControlEnables()
        {
            bool moduleSelected = (qusbControl.SelectedModules.Length == 1);

            qusbControl.Enabled = !RunningTest;
            gbConfig.Enabled = moduleSelected && !RunningTest;
            gbLog.Enabled = moduleSelected;
            bPowerCycleFpga.Enabled = moduleSelected && !RunningTest;
            bProgramFpga.Enabled = moduleSelected && !RunningTest;
            bRunTest.Enabled = moduleSelected;
            cbScanInBackground.Enabled = moduleSelected;
            bUpdateCount.Enabled = moduleSelected;
            bRunTestSuite.Enabled = moduleSelected;

            DataTestType type = GetTestType((string)cbTestType.SelectedItem);
            bool streaming = (type == DataTestType.StreamingRead) || (type == DataTestType.StreamingWrite) || (type == DataTestType.StreamingLoopback);
            bool async = (type == DataTestType.AsyncRead) || (type == DataTestType.AsyncWrite) || (type == DataTestType.AsyncLoopback);
            bool legacyAsync = (type == DataTestType.LegacyAsyncRead) || (type == DataTestType.LegacyAsyncWrite) /*|| (type == DataTestType.LegacyAsyncLoopback)*/;
            bool sync = (type == DataTestType.SyncRead) || (type == DataTestType.SyncWrite) || (type == DataTestType.SyncLoopback);

            lNumThreads.Enabled = streaming || async;
            nNumThreads.Enabled = streaming || async;
            lConcurrency.Enabled = streaming || async;
            nConcurrency.Enabled = streaming || async;
            lNumBuffers.Enabled = !sync;
            nNumBuffers.Enabled = !sync;
            cbUserAlloc.Enabled = streaming || async;

            bPauseStream.Enabled = streaming && RunningTest;
            bResumeStream.Enabled = streaming && RunningTest;
        }

        void FpgaUploadUpdate(int percent)
        {
            // We must move one past the desrired rate to disable the progress bar animation
            if (percent == 100)
            {
                pbProgressBar.Value = percent;
                pbProgressBar.Value = percent - 1;
                pbProgressBar.Value = percent;
            }
            else
            {
                pbProgressBar.Value = percent + 1;
                pbProgressBar.Value = percent;
            }

            // Update the stats every once in a while
            if ((Environment.TickCount - tLastUpdate) > 100)
            {
                tLastUpdate = Environment.TickCount;
            }
        }

        bool Test_InitFpga()
        {
            bool qResult;
            bool configured;

            // Set initial timeout
            qusb.SetTimeout(1000);

            // Turn FPGA power on and place FPGA in reset
            qResult = qusb.WritePortDir(QuickUsb.Port.A, 0x81);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePortDir() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return false;
            }
            qResult = qusb.WritePort(QuickUsb.Port.A, 0x80);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePort() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return false;
            }

            // Check if the FPGA is configured
            qResult = qusb.IsFpgaConfigured(out configured);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("Unable to determine if FPGA is configured: {0}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return false;
            }
            else
            {
                pbProgressBar.Value = (configured ? 100 : 0);
            }

            // Configure the FPGA
            if (!configured)
            {
                tbLog.AppendText("Programming FPGA...");
                Application.DoEvents();

                // Allow some time for the FPGA to become fully powered
                Thread.Sleep(250);

                // Write the RBF file
                tLastUpdate = Environment.TickCount;
                qResult = qusb.UploadFpga(FpgaFile, FpgaUploadUpdate);
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    tbLog.AppendText(String.Format("\nUnable to configure FPGA: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                    return false;
                }
                else
                {
                    pbProgressBar.Value = 100;
                    tbLog.AppendText("Done" + Environment.NewLine);
                }

                // Allow FPGA ample time to boot up
                Thread.Sleep(500);
                Application.DoEvents();
            }

            // Fake a timer event to force a re-read of the config register
            timer_Tick(null, null);

            // Write control register (Allowed while FPGA is in reset)
            if ((TestType == DataTestType.SyncLoopback) || (TestType == DataTestType.AsyncLoopback) || /*(TestType == DataTestType.LegacyAsyncLoopback) ||*/ (TestType == DataTestType.StreamingLoopback))
            {
                btConfig.Value = (uint)(btConfig.Value | 0x07);
            }
            else
            {
                btConfig.Value = (uint)(btConfig.Value & ~0x04);
            }

            // Remove FPGA from reset
            qResult = qusb.WritePort(QuickUsb.Port.A, 0x81);
            if (!qResult)
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.WritePort() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return false;
            }

            // Wait a little for good measure
            Thread.Sleep(250);

            // Return successfully
            return true;
        }

        void Test_RunTestSuite()
        {
            //int TestCount = 1 * 1 * 1 * 1 * (Enum.GetNames(typeof(DataTestType)).Length - 1);
            //int TotalTestTime = 1 * 60 * 1000;

            int timePerTest = (int)(500);// TotalTestTime / (TestCount);
            suiteTimer.Interval = timePerTest;

            Failures = 0;
            NumTests = 0;
            RunningTestSuite = true;

            // Test
            foreach (int concurrency in new int[] { 0, 1, 2 })
            {
                foreach (int numThreads in new int[] { 0, 1, 4 })
                {
                    foreach (int bufferSize in new int[] { 512, 1024, 2000 }) // In bytes
                    {
                        foreach (int numBuffers in new int[] { 1, 2, 3, 4 })
                        {
                            foreach (DataTestType testType in Enum.GetValues(typeof(DataTestType)))
                            {
                                // Only continue if we're still running the tests
                                if (!RunningTestSuite)
                                {
                                    continue;
                                }

                                // Skip 'None'
                                if (testType == DataTestType.None)
                                {
                                    continue;
                                }

                                // Verify we don't try to set an incorrect number of threads/concurrency
                                if ((NumThreads > 0) && (concurrency == 0))
                                {
                                    continue;
                                }

                                // Skip redundant synchronous tests where the number of threads > 0
                                if ((testType == DataTestType.SyncRead) || (testType == DataTestType.SyncWrite) || (testType == DataTestType.SyncLoopback))
                                {
                                    if (NumThreads > 0)
                                    {
                                        continue;
                                    }
                                }

                                // Verify that we don't try to exceed the size of our FIFO buffers (4 kWords)
                                if ((bufferSize * numBuffers) > (2 * 4096))
                                {
                                    continue;
                                }

                                // Set test info
                                cbTestType.SelectedItem = GetTestType(testType);
                                nNumBuffers.Value = numBuffers;
                                nBufferSize.Value = bufferSize;
                                nNumThreads.Value = numThreads;
                                nConcurrency.Value = concurrency;

                                // Set up the timer that will end the test
                                suiteTimer.Start();

                                // Run the test
                                bRunTest.Checked = true;

                                // Update the form controls
                                Application.DoEvents();

                                //if (Failures > 0)
                                //{
                                //    return;
                                //}
                            }
                        }
                    }
                }
            }

            RunningTestSuite = false;
        }

        bool Test_ResetTestState()
        {
            // Make sure a module is selected
            if (qusb == null)
            {
                return false;
            }

            // Reset variables
            NextOutLFSR = 0x1234;
            NextInLFSR = 0xABCD;
            tStart = 0;
            tEnd = 0;
            BytesRead = 0;
            BytesWritten = 0;
            inTransCount = 0;
            outTransCount = 0;
            fpgaBertCount = 0;
            softwareBertCount = 0;
            nextInRequestID = 0;
            nextOutRequestID = 0;
            CompletionRoutineFailure = false;

            // Clear any lingering buffers
            PendingBuffersToVerify.Clear();

            // Set the timeout
            qusb.SetTimeout((uint)nTimeout.Value);

            // Grab test values
            NumBuffers = (uint)nNumBuffers.Value;
            BufferByteSize = (uint)nBufferSize.Value;
            NumThreads = (ushort)nNumThreads.Value;
            ThreadConcurrency = (ushort)nConcurrency.Value;
            TestType = GetTestType((string)cbTestType.SelectedItem);
            UserAllocatedBuffers = !(!cbUserAlloc.Checked && ((TestType == DataTestType.StreamingRead) || (TestType == DataTestType.StreamingLoopback)));

            // Allocate arrays
            if (UserAllocatedBuffers)
            {
                InBufferArray = new ushort[NumBuffers][];
                hInBufferArray = new GCHandle[NumBuffers];
            }
            else
            {
                InBufferArray = null;
                hInBufferArray = null;
            }
            OutBufferArray = new ushort[NumBuffers][];
            hOutBufferArray = new GCHandle[NumBuffers];

            // Allocate buffers
            for (int k = 0; k < NumBuffers; ++k)
            {
                if (UserAllocatedBuffers)
                {
                    InBufferArray[k] = new ushort[BufferByteSize / 2];
                }
                OutBufferArray[k] = new ushort[BufferByteSize / 2];

                // We must pass an array of the unmanaged buffers to the start stream 
                // function.  To do that we must also make sure that our managed array
                // is never moved by the garbage collector.
                if (UserAllocatedBuffers)
                {
                    hInBufferArray[k] = GCHandle.Alloc(InBufferArray[k], GCHandleType.Pinned);
                }
                hOutBufferArray[k] = GCHandle.Alloc(OutBufferArray[k], GCHandleType.Pinned);
            }

            ushort value;
            qusb.ReadSetting(QuickUsb.Setting.FifoConfig, out value);
            qusb.WriteSetting(QuickUsb.Setting.FifoConfig, value);
            // Return successfully
            return true;
        }

        bool Test_UpdateStats()
        {
            QueryPerformanceCounter(out tEnd);
            double tElapsed = (double)(tEnd - tStart) / (double)(freq);

            double overallRate;
            if (tElapsed == 0.0)
            {
                overallRate = 0.0;
            }
            else
            {
                overallRate = (double)(BytesRead + BytesWritten) / tElapsed;
            }

            // Update stats
            lBERTCount.Text = "0x" + (fpgaBertCount + softwareBertCount).ToString("X16");
            lDataRead.Text = GetFriendlyDataSize(BytesRead);
            lDataWritten.Text = GetFriendlyDataSize(BytesWritten);
            if (RunningTest)
            {
                lDataRate.Text = GetFriendlyDataSize(overallRate) + "/s";
            }
            lReadTrans.Text = inTransCount.ToString();
            lWriteTrans.Text = outTransCount.ToString();
            lFailures.Text = Failures.ToString() + " (Of " + (NumTests == 0 ? 1 : NumTests).ToString() + " Tests)";

            // Return successfully
            return true;
        }

        bool Test_ReadBert()
        {
            byte[] data = new byte[8]; // BERT is 8 bytes
            ushort numBytes = (ushort)data.Length;
            if (!qusb.ReadCommand(3, data, ref numBytes))
            {
                qusbError = qusb.LastError();
                tbLog.AppendText(String.Format("QuickUsb.ReadCommand() failed with error: {0} - {1}" + Environment.NewLine, qusbError, qusb.LastDriverError()));
                return false;
            }

            // Construct BERT count
            fpgaBertCount = ((ulong)data[7] << 56) | ((ulong)data[6] << 48) | ((ulong)data[5] << 40) | ((ulong)data[4] << 32) | ((ulong)data[3] << 24) | ((ulong)data[2] << 16) | ((ulong)data[1] << 8) | (ulong)data[0];

            return true;
        }

        void Test_Stop()
        {
            // Perform a final stats update
            Test_ReadBert();
            Test_LogStats();
            Test_UpdateStats();

            // Update the check button state
            RunningTest = true;
            bRunTest.Checked = false;
            RunningTest = false;
            bRunTest.Text = "Run Test";

            // Update control enables
            UpdateControlEnables();

            // Log that we've finished the test
            tbLog.AppendText(String.Format("*** Completed test {0} ***" + Environment.NewLine, GetTestType(TestType)));
            tbLog.AppendText(Environment.NewLine);
        }

        void Test_LogStats()
        {
            // Copy over the buffered log messages now so that they appear in the proper 
            // order.  We know there's a problem if messages are displayed after the 
            // "Completed test" message
            lock (log)
            {
                tbLog.AppendText(log.ToString());
                log = new StringBuilder();
            }

            QueryPerformanceCounter(out tEnd);
            double tElapsed = (double)(tEnd - tStart) / (double)(freq);

            lReadTrans.Text = inTransCount.ToString();
            lWriteTrans.Text = outTransCount.ToString();


            double overallRate;
            if (tElapsed == 0.0)
            {
                overallRate = 0.0;
            }
            else
            {
                overallRate = (double)(BytesRead + BytesWritten) / tElapsed;
            }

            // Log info
            tbLog.AppendText("End Time: " + DateTime.Now + Environment.NewLine);
            tbLog.AppendText("FPGA BERT Count:     0x" + fpgaBertCount.ToString("X16") + Environment.NewLine);
            tbLog.AppendText("Software BERT Count: 0x" + softwareBertCount.ToString("X16") + Environment.NewLine);
            tbLog.AppendText("Data Read:           " + GetFriendlyDataSize(BytesRead) + Environment.NewLine);
            tbLog.AppendText("Data Written:        " + GetFriendlyDataSize(BytesWritten) + Environment.NewLine);
            tbLog.AppendText("Average Data Rate:   " + GetFriendlyDataSize(overallRate) + "/s" + Environment.NewLine);
            tbLog.AppendText("Read Transactions:   " + inTransCount + Environment.NewLine);
            tbLog.AppendText("Write Transactions:  " + outTransCount + Environment.NewLine);

            // Check that, for Async tests, the number of threads is actually correct
            bool threadOk = true;
            if ((TestType == DataTestType.AsyncRead) || (TestType == DataTestType.AsyncWrite) || (TestType == DataTestType.AsyncLoopback))
            {
                ushort actualNumThreads, actualConcurrency;
                if (!qusb.GetNumAsyncThreads(out actualNumThreads, out actualConcurrency))
                {
                    tbLog.AppendText(String.Format("QuickUsb.GetNumAsyncThreads() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                }

                if ((actualNumThreads != NumThreads) || (actualConcurrency != ThreadConcurrency))
                {
                    threadOk = false;
                }
            }

            // Keep track of failed tests
            bool failed = (!CompletionRoutineFailure && threadOk && (fpgaBertCount != 0) || (softwareBertCount != 0) || (inTransCount == 0 && outTransCount == 0) || (BytesRead == 0 && BytesWritten == 0));
            if (failed)
            {
                ++Failures;
                tbLog.AppendText("Result: FAIL" + Environment.NewLine);
            }
            else
            {
                tbLog.AppendText("Result: OK" + Environment.NewLine);
            }
        }

        ulong CountBitErrors(ushort[] a, ushort[] b)
        {
            if (a.Length != b.Length)
            {
                throw new Exception("Buffer sizes do not match!");
            }

            ulong bert = 0;
            ushort bitErrors;
            int k, j;
            for (k = 0; k < a.Length; ++k)
            {
                bitErrors = (ushort)(a[k] ^ b[k]);
                if (bitErrors != 0)
                {
                    for (j = 0; j < 15; ++j)
                    {
                        bert += (ulong)(bitErrors & 1);
                        bitErrors >>= 1;
                    }
                }
            }

            return bert;
        }

        unsafe ulong CountBitErrors(ushort* a, ushort[] b)
        {
            ulong bert = 0;
            ushort bitErrors;
            int k, j;
            for (k = 0; k < b.Length; ++k)
            {
                bitErrors = (ushort)(a[k] ^ b[k]);
                if (bitErrors != 0)
                {
                    for (j = 0; j < 15; ++j)
                    {
                        bert += (ulong)(bitErrors & 1);
                        bitErrors >>= 1;
                    }
                }
            }

            return bert;
        }

        private void bRunTest_CheckedChanged(object sender, EventArgs e)
        {
            const int updateFreqMs = 100; // 100ms between updates
            bool qResult;
            ushort[] outBuffer;
            ushort[] inBuffer;
            ushort[] buffer;
            uint length;
            IntPtr[] outArr = null;
            IntPtr[] inArr = null;
            byte[] transID; // For legacy async
            QuickUsb.BulkStream[] outBulkStream; // For new async
            QuickUsb.BulkStream[] inBulkStream; // For new async
            int issuedInRequests, issuedOutRequests;
            int nextInRequest, nextOutRequest;

            // If we're already running a test, we just need to stop so we don't
            // recursively start tests
            if (RunningTest)
            {
                RunningTest = false;
                return;
            }
            
            if (bRunTest.Checked)
            {
                // Start a test
                ++NumTests;
                RunningTest = true;
                bRunTest.Text = "Stop Test";
                UpdateControlEnables();

                // Reset test state
                if (!Test_ResetTestState())
                {
                    Test_Stop();
                    return;
                }

                // Configure FPGA
                if (!Test_InitFpga())
                {
                    Test_Stop();
                    return;
                }

                // Update stats
                if (!Test_UpdateStats())
                {
                    Test_Stop();
                    return;
                }

                tbLog.AppendText(String.Format("*** Starting test {0} ***" + Environment.NewLine, GetTestType(TestType)));

                // Log information about the test setup
                tbLog.AppendText("Start Time: " + DateTime.Now + Environment.NewLine);
                tbLog.AppendText("NumBuffers = " + NumBuffers + Environment.NewLine);
                tbLog.AppendText("BufferByteSize = " + BufferByteSize + Environment.NewLine);
                tbLog.AppendText("NumThreads = " + NumThreads + Environment.NewLine);
                tbLog.AppendText("Concurrency = " + (int)nConcurrency.Value + Environment.NewLine);
                tbLog.AppendText("Config Register = 0x" + btConfig.Value.ToString("X2") + Environment.NewLine);
                if ((TestType == DataTestType.StreamingRead) || (TestType == DataTestType.StreamingLoopback))
                {
                    tbLog.AppendText("User allocated buffers = " + UserAllocatedBuffers.ToString() + Environment.NewLine);
                }

                // Start timing
                tLastUpdate = tTestStart = Environment.TickCount;
                QueryPerformanceCounter(out tStart);

                switch (TestType)
                {
                    /********************************************************/
                    case DataTestType.SyncRead:
                    /********************************************************/
                        // Create data buffer
                        inBuffer = new ushort[BufferByteSize / 2];
                        buffer = new ushort[BufferByteSize / 2];

                        // Loop until the test ends
                        do
                        {
                            // Perform the synchronous data read
                            length = (uint)inBuffer.Length;
                            qResult = qusb.ReadData(inBuffer, ref length);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.ReadData() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));

                                // If the request failed because it timed out, we'll continue our test
                                if (qusb.LastError() != QuickUsb.Error.Timeout)
                                {
                                    ++Failures;
                                    Test_Stop();
                                    return;
                                }
                            }
                            else
                            {
                                // Sanity check that we read the appropriate number of bytes
                                if (length != (BufferByteSize / 2))
                                {
                                    tbLog.AppendText(String.Format("QuickUsb.ReadData() Succeeded with incorrect length: {0} ({1} expected)" + Environment.NewLine, length, BufferByteSize / 2));
                                }

                                // Get the next set of data to verify against
                                GetNextInLFSR(buffer, buffer.Length);

                                // Verify the read data
                                softwareBertCount += CountBitErrors(inBuffer, buffer);

                                // Successfuly data transfer
                                ++inTransCount;
                                BytesRead += BufferByteSize;
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // Since this is a single-threaded test, we must explicitly perform app undates
                            Application.DoEvents();

                        } while (RunningTest);
                        break;

                    /********************************************************/
                    case DataTestType.SyncWrite:
                    /********************************************************/
                        // Create data buffer
                        outBuffer = new ushort[BufferByteSize / 2];

                        // Get the next set of data to write out
                        GetNextOutLFSR(outBuffer, outBuffer.Length);

                        // Loop until the test ends
                        do
                        {
                            // Perform the synchronous data write
                            qResult = qusb.WriteData(outBuffer, (uint)outBuffer.Length);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.WriteData() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));

                                // If the request failed because it timed out, we'll continue our test
                                if (qusb.LastError() != QuickUsb.Error.Timeout)
                                {
                                    ++Failures;
                                    Test_Stop();
                                    return;
                                }
                            }
                            else
                            {
                                // Get the next set of data to write out
                                GetNextOutLFSR(outBuffer, outBuffer.Length);

                                // Successfuly data transfer
                                ++outTransCount;
                                BytesWritten += BufferByteSize;
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // Since this is a single-threaded test, we must explicitly perform app undates
                            Application.DoEvents();

                        } while (RunningTest);
                        break;

                    /********************************************************/
                    case DataTestType.SyncLoopback:
                    /********************************************************/
                        // Create data buffer
                        inBuffer = new ushort[BufferByteSize / 2];
                        outBuffer = new ushort[BufferByteSize / 2];

                        // Get the next set of data to write out
                        GetNextOutLFSR(outBuffer, outBuffer.Length);

                        // Loop until the test ends
                        do
                        {
                            // Perform the synchronous data write
                            qResult = qusb.WriteData(outBuffer, (uint)outBuffer.Length);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.WriteData() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));

                                // If the request failed because it timed out, we'll continue our test
                                if (qusb.LastError() != QuickUsb.Error.Timeout)
                                {
                                    ++Failures;
                                    Test_Stop();
                                    return;
                                }
                            }
                            else
                            {
                                // Perform the synchronous data read
                                length = (uint)inBuffer.Length;
                                qResult = qusb.ReadData(inBuffer, ref length);
                                if (!qResult)
                                {
                                    tbLog.AppendText(String.Format("QuickUsb.ReadData() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));

                                    // If the request failed because it timed out, we'll continue our test
                                    if (qusb.LastError() != QuickUsb.Error.Timeout)
                                    {
                                        ++Failures;
                                        Test_Stop();
                                        return;
                                    }
                                }
                                else
                                {
                                    // Sanity check that we read the appropriate number of bytes
                                    if (length != (BufferByteSize / 2))
                                    {
                                        tbLog.AppendText(String.Format("QuickUsb.ReadData() Succeeded with incorrect length: {0} ({1} expected)" + Environment.NewLine, length, BufferByteSize / 2));
                                    }

                                    // Verify the read data
                                    softwareBertCount += CountBitErrors(inBuffer, outBuffer);

                                    // Successfuly read data transfer
                                    ++inTransCount;
                                    BytesRead += BufferByteSize;
                                }

                                // Get the next set of data to write out
                                GetNextOutLFSR(outBuffer, outBuffer.Length);

                                // Successfuly write data transfer
                                ++outTransCount;
                                BytesWritten += BufferByteSize;
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // Since this is a single-threaded test, we must explicitly perform app undates
                            Application.DoEvents();

                        } while (RunningTest);
                        break;

                    /********************************************************/
                    case DataTestType.AsyncRead:
                    /********************************************************/
                        buffer = new ushort[BufferByteSize / 2];

                        if (!qusb.OpenEx(QuickUsb.OpenFlag.Normal))
                        {
                            tbLog.AppendText(String.Format("QuickUsb.Open() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            ++Failures;
                            Test_Stop();
                            return;
                        }

                        // Set the number of threads
                        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency))
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.SetNumAsyncThreads() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }

                        // Issue initial set of read requests
                        inBulkStream = new QuickUsb.BulkStream[NumBuffers];
                        issuedInRequests = 0;
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            // Allocate BulkStream object
                            inBulkStream[k] = new QuickUsb.BulkStream();

                            length = BufferByteSize;
                            qResult = qusb.ReadBulkDataAsync(hInBufferArray[k].AddrOfPinnedObject(), length, inBulkStream[k], null, IntPtr.Zero);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedInRequests = 0;
                                ++Failures;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                ++issuedInRequests;
                            }
                        }

                        // Loop until the test ends
                        nextInRequest = 0;
                        while (RunningTest || (issuedInRequests > 0))
                        {
                            // Wait for a request to complete
                            qResult = qusb.BulkWait(inBulkStream[nextInRequest], false);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.BulkWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedInRequests = 0;
                                ++Failures;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                // Check if the request succeeded
                                if (inBulkStream[nextInRequest].Error != 0)
                                {
                                    tbLog.AppendText(String.Format("Request failed with error: {0} - {1}" + Environment.NewLine, inBulkStream[nextInRequest].Error));
                                    ++Failures;
                                    RunningTest = false;
                                    break;
                                }

                                // Sanity check that we wrote the proper number of bytes
                                if (inBulkStream[nextInRequest].BytesRequested != inBulkStream[nextInRequest].BytesTransferred)
                                {
                                    tbLog.AppendText(String.Format("Read incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, inBulkStream[nextInRequest].BytesTransferred, inBulkStream[nextInRequest].BytesRequested));
                                    ++Failures;
                                }

                                // Get the next set of data to verify against
                                GetNextInLFSR(buffer, buffer.Length);

                                // Verify the read data
                                softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], buffer);

                                // Successfuly data transfer
                                ++inTransCount;
                                BytesRead += inBulkStream[nextInRequest].BytesTransferred;
                                --issuedInRequests;

                                // Re-issue the data request
                                if (RunningTest)
                                {
                                    length = BufferByteSize;
                                    qResult = qusb.ReadBulkDataAsync(hInBufferArray[nextInRequest].AddrOfPinnedObject(), length, inBulkStream[nextInRequest], null, IntPtr.Zero);
                                    if (!qResult)
                                    {
                                        tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        // TODO: Wait on issued transactions
                                        issuedInRequests = 0;
                                        Test_Stop();
                                        if (!qusb.Close())
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        }
                                        return;
                                    }
                                    else
                                    {
                                        ++issuedInRequests;
                                    }
                                }

                                // Move to next buffer to wait on
                                if (++nextInRequest >= NumBuffers)
                                {
                                    nextInRequest = 0;
                                }
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_UpdateStats();

                                // Since this is a single-threaded test, we must explicitly perform app undates
                                Application.DoEvents();

                                tLastUpdate = Environment.TickCount;
                            }
                        }

                        Test_Stop();
                        if (!qusb.Close())
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        return;

                    /********************************************************/
                    case DataTestType.AsyncWrite:
                    /********************************************************/
                        if (!qusb.OpenEx(QuickUsb.OpenFlag.Normal))
                        {
                            tbLog.AppendText(String.Format("QuickUsb.Open() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            ++Failures;
                            Test_Stop();
                            return;
                        }

                        buffer = new ushort[BufferByteSize / 2];

                        // Set the number of threads
                        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency))
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.SetNumAsyncThreads() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }

                        // Issue initial set of write requests
                        outBulkStream = new QuickUsb.BulkStream[NumBuffers];
                        issuedOutRequests = 0;
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);

                            // Allocate BulkStream object
                            outBulkStream[k] = new QuickUsb.BulkStream();

                            length = BufferByteSize;
                            qResult = qusb.WriteBulkDataAsync(hOutBufferArray[k].AddrOfPinnedObject(), length, outBulkStream[k], null, IntPtr.Zero);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedOutRequests = 0;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                ++issuedOutRequests;
                            }
                        }

                        // Loop until the test ends
                        nextOutRequest = 0;
                        while (RunningTest || (issuedOutRequests > 0))
                        {
                            // Wait for a request to complete
                            qResult = qusb.BulkWait(outBulkStream[nextOutRequest], false);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.BulkWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedOutRequests = 0;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                // Check if the request succeeded
                                if (outBulkStream[nextOutRequest].Error != 0)
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("Request failed with error: {0} - {1}" + Environment.NewLine, outBulkStream[nextOutRequest].Error));
                                    RunningTest = false;
                                    break;
                                }

                                // Sanity check that we wrote the proper number of bytes
                                if (outBulkStream[nextOutRequest].BytesRequested != outBulkStream[nextOutRequest].BytesTransferred)
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("Wrote incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, outBulkStream[nextOutRequest].BytesTransferred, outBulkStream[nextOutRequest].BytesRequested));
                                }

                                // Successfuly data transfer
                                ++outTransCount;
                                BytesWritten += outBulkStream[nextOutRequest].BytesTransferred;
                                --issuedOutRequests;

                                // Re-issue the data request
                                if (RunningTest)
                                {
                                    // Get the next set of data to verify against
                                    GetNextOutLFSR(OutBufferArray[nextOutRequest], OutBufferArray[nextOutRequest].Length);

                                    length = BufferByteSize;
                                    qResult = qusb.WriteBulkDataAsync(hOutBufferArray[nextOutRequest].AddrOfPinnedObject(), length, outBulkStream[nextOutRequest], null, IntPtr.Zero);
                                    if (!qResult)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        // TODO: Wait on issued transactions
                                        issuedOutRequests = 0;
                                        Test_Stop();
                                        if (!qusb.Close())
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        }
                                        return;
                                    }
                                    else
                                    {
                                        ++issuedOutRequests;
                                    }
                                }

                                // Move to next buffer to wait on
                                if (++nextOutRequest >= NumBuffers)
                                {
                                    nextOutRequest = 0;
                                }
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();

                                // Since this is a single-threaded test, we must explicitly perform app undates
                                Application.DoEvents();

                                tLastUpdate = Environment.TickCount;
                            }
                        }
                        Test_Stop();
                        if (!qusb.Close())
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        return;

                    /********************************************************/
                    case DataTestType.AsyncLoopback:
                    /********************************************************/
                        if (!qusb.OpenEx(QuickUsb.OpenFlag.Normal))
                        {
                            tbLog.AppendText(String.Format("QuickUsb.Open() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            ++Failures;
                            Test_Stop();
                            return;
                        }

                        // Set the number of threads
                        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency))
                        {
                            tbLog.AppendText(String.Format("QuickUsb.SetNumAsyncThreads() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError()));
                            ++Failures;
                            Test_Stop();
                            if (!qusb.Close())
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            }
                            return;
                        }

                        outBulkStream = new QuickUsb.BulkStream[NumBuffers];
                        inBulkStream = new QuickUsb.BulkStream[NumBuffers];
                        issuedOutRequests = 0;
                        issuedInRequests = 0;
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            //
                            // Issue initial write requests
                            //

                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);

                            // Allocate BulkStream object
                            outBulkStream[k] = new QuickUsb.BulkStream();

                            length = BufferByteSize;
                            qResult = qusb.WriteBulkDataAsync(hOutBufferArray[k].AddrOfPinnedObject(), length, outBulkStream[k], null, IntPtr.Zero);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedOutRequests = 0;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                ++issuedOutRequests;
                            }

                            //
                            // Issue initial read requests
                            //

                            // Allocate BulkStream object
                            inBulkStream[k] = new QuickUsb.BulkStream();

                            length = BufferByteSize;
                            qResult = qusb.ReadBulkDataAsync(hInBufferArray[k].AddrOfPinnedObject(), length, inBulkStream[k], null, IntPtr.Zero);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions and free memory
                                issuedInRequests = 0;
                                Test_Stop();
                                if (!qusb.Close())
                                {
                                    ++Failures;
                                    tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                }
                                return;
                            }
                            else
                            {
                                ++issuedInRequests;
                            }
                        }

                        // Loop until the test ends
                        nextOutRequest = 0;
                        nextInRequest = 0;
                        while (RunningTest || (issuedInRequests > 0) || (issuedOutRequests > 0))
                        {
                            //
                            // Check if the next OUT request has completed
                            //
                            if (issuedOutRequests > 0)
                            {
                                qResult = qusb.BulkWait(outBulkStream[nextOutRequest], true);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.NotCompleted)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.BulkWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError()));
                                        // TODO: Wait on issued transactions and free memory
                                        issuedOutRequests = 0;
                                        Test_Stop();
                                        if (!qusb.Close())
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        }
                                        return;
                                    }
                                }
                                else
                                {
                                    // Check if the request failed
                                    if (outBulkStream[nextOutRequest].Error == QuickUsb.Error.NoError)
                                    {
                                        // Sanity check that we wrote the proper number of bytes
                                        if (outBulkStream[nextOutRequest].BytesRequested != outBulkStream[nextOutRequest].BytesTransferred)
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("Wrote incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, outBulkStream[nextOutRequest].BytesTransferred, outBulkStream[nextOutRequest].BytesRequested));
                                        }

                                        // Successfuly data transfer
                                        BytesWritten += outBulkStream[nextOutRequest].BytesTransferred;

                                        // We need to store this completed data buffer so we may verify against it later
                                        // once we have read it back
                                        ushort[] data = new ushort[OutBufferArray[nextOutRequest].Length];
                                        OutBufferArray[nextOutRequest].CopyTo(data, 0);
                                        PendingBuffersToVerify.Enqueue(data);
                                    }
                                    else
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("Bulk Write Request failed with error: {0} - {1}" + Environment.NewLine, outBulkStream[nextOutRequest].Error));

                                        // For all non-timeout errors, stop the test
                                        if (outBulkStream[nextOutRequest].Error != QuickUsb.Error.Timeout)
                                        {
                                            // TODO: Wait on issued transactions and free memory
                                            issuedOutRequests = 0;
                                            Test_Stop();
                                            if (!qusb.Close())
                                            {
                                                ++Failures;
                                                tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                            }
                                            return;
                                        }
                                    }
                                    ++outTransCount;
                                    --issuedOutRequests;

                                    // Re-issue the data request
                                    if (RunningTest)
                                    {
                                        // Get the next set of data to verify against
                                        GetNextOutLFSR(OutBufferArray[nextOutRequest], OutBufferArray[nextOutRequest].Length);

                                        length = BufferByteSize;
                                        qResult = qusb.WriteBulkDataAsync(hOutBufferArray[nextOutRequest].AddrOfPinnedObject(), length, outBulkStream[nextOutRequest], null, IntPtr.Zero);
                                        if (!qResult)
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                            // TODO: Wait on issued transactions
                                            issuedOutRequests = 0;
                                            Test_Stop();
                                            return;
                                        }
                                        else
                                        {
                                            ++issuedOutRequests;
                                        }
                                    }

                                    // Move to next buffer to wait on
                                    if (++nextOutRequest >= NumBuffers)
                                    {
                                        nextOutRequest = 0;
                                    }
                                }
                            }

                            //
                            // Check if the next IN request has completed
                            //
                            if ((issuedInRequests > 0) && (PendingBuffersToVerify.Count > 0))
                            {
                                qResult = qusb.BulkWait(inBulkStream[nextInRequest], false);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.NotCompleted)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.BulkWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        // TODO: Wait on issued transactions and free memory
                                        issuedInRequests = 0;
                                        Test_Stop();
                                        return;
                                    }
                                }
                                else
                                {
                                    // Check if the request failed
                                    if (inBulkStream[nextInRequest].Error == QuickUsb.Error.NoError)
                                    {
                                        // Sanity check that we read the proper number of bytes
                                        if (inBulkStream[nextInRequest].BytesRequested != inBulkStream[nextInRequest].BytesTransferred)
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("Read incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, inBulkStream[nextInRequest].BytesTransferred, inBulkStream[nextInRequest].BytesRequested));
                                        }

                                        // Verify the read data
                                        softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], PendingBuffersToVerify.Dequeue());

                                        // Successfuly data transfer
                                        BytesRead += inBulkStream[nextInRequest].BytesTransferred;
                                    }
                                    else
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("Bulk Read Request failed with error: {0} - {1}" + Environment.NewLine, inBulkStream[nextInRequest].Error));

                                        // For all non-timeout errors, stop the test
                                        if (inBulkStream[nextInRequest].Error != QuickUsb.Error.Timeout)
                                        {
                                            // TODO: Wait on issued transactions and free memory
                                            issuedOutRequests = 0;
                                            Test_Stop();
                                            if (!qusb.Close())
                                            {
                                                ++Failures;
                                                tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                            }
                                            return;
                                        }
                                    }
                                    ++inTransCount;
                                    --issuedInRequests;

                                    // Re-issue the data request.  We also need to make sure that for every write we issue a read
                                    // so that all data is drained from the FPGA FIFOs.
                                    if (RunningTest || (outTransCount > inTransCount))
                                    {
                                        length = BufferByteSize;
                                        qResult = qusb.ReadBulkDataAsync(hInBufferArray[nextInRequest].AddrOfPinnedObject(), length, inBulkStream[nextInRequest], null, IntPtr.Zero);
                                        if (!qResult)
                                        {
                                            ++Failures;
                                            tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                            // TODO: Wait on issued transactions
                                            issuedInRequests = 0;
                                            Test_Stop();
                                            if (!qusb.Close())
                                            {
                                                ++Failures;
                                                tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                            }
                                            return;
                                        }
                                        else
                                        {
                                            ++issuedInRequests;
                                        }
                                    }

                                    // Move to next buffer to wait on
                                    if (++nextInRequest >= NumBuffers)
                                    {
                                        nextInRequest = 0;
                                    }
                                }
                            }
                        
                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();

                                // Since this is a single-threaded test, we must explicitly perform app undates
                                Application.DoEvents();

                                tLastUpdate = Environment.TickCount;
                            }
                        }
                        Test_Stop();
                        if (!qusb.Close())
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.Close() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        return;

                    /********************************************************/
                    case DataTestType.LegacyAsyncRead:
                    /********************************************************/
                        buffer = new ushort[BufferByteSize / 2];

                        // Issue initial set of read requests
                        transID = new byte[NumBuffers];
                        issuedInRequests = 0;
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            length = BufferByteSize;
                            qResult = qusb.ReadDataAsync(hInBufferArray[k].AddrOfPinnedObject(), ref length, out transID[k]);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.ReadDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions
                                issuedInRequests = 0;
                                Test_Stop();
                                return;
                            }
                            else
                            {
                                ++issuedInRequests;
                            }
                        }

                        // Loop until the test ends
                        nextInRequest = 0;
                        while (RunningTest || (issuedInRequests > 0))
                        {
                            // Wait for a request to complete
                            qResult = qusb.AsyncWait(out length, transID[nextInRequest], false);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.AsyncWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions
                                issuedInRequests = 0;
                                Test_Stop();
                                return;
                            }
                            else if (length != 0) // Check if the transaction has completed
                            {
                                // Sanity check that we wrote the proper number of bytes
                                if (length != BufferByteSize)
                                {
                                    tbLog.AppendText(String.Format("Read incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, length, BufferByteSize));
                                }

                                // Get the next set of data to verify against
                                GetNextInLFSR(buffer, buffer.Length);

                                // Verify the read data
                                softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], buffer);

                                // Successfuly data transfer
                                ++inTransCount;
                                BytesRead += length;
                                --issuedInRequests;

                                // Re-issue the data request
                                if (RunningTest)
                                {
                                    length = BufferByteSize;
                                    qResult = qusb.ReadDataAsync(hInBufferArray[nextInRequest].AddrOfPinnedObject(), ref length, out transID[nextInRequest]);
                                    if (!qResult)
                                    {
                                        tbLog.AppendText(String.Format("QuickUsb.ReadDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        // TODO: Wait on issued transactions
                                        issuedInRequests = 0;
                                        Test_Stop();
                                        return;
                                    }
                                    else
                                    {
                                        ++issuedInRequests;
                                    }
                                }

                                // Move to next buffer to wait on
                                if (++nextInRequest >= NumBuffers)
                                {
                                    nextInRequest = 0;
                                }
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_UpdateStats();

                                // Since this is a single-threaded test, we must explicitly perform app undates
                                Application.DoEvents();

                                tLastUpdate = Environment.TickCount;
                            }
                        }
                        break;

                    /********************************************************/
                    case DataTestType.LegacyAsyncWrite:
                    /********************************************************/
                        buffer = new ushort[BufferByteSize / 2];

                        // Issue initial set of read requests
                        transID = new byte[NumBuffers];
                        issuedOutRequests = 0;
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);

                            length = BufferByteSize;
                            qResult = qusb.WriteDataAsync(hOutBufferArray[k].AddrOfPinnedObject(), length, out transID[k]);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.WriteDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions
                                issuedOutRequests = 0;
                                Test_Stop();
                                return;
                            }
                            else
                            {
                                ++issuedOutRequests;
                            }
                        }

                        // Loop until the test ends
                        nextOutRequest = 0;
                        while (RunningTest || (issuedOutRequests > 0))
                        {
                            // Wait for a request to complete
                            qResult = qusb.AsyncWait(out length, transID[nextOutRequest], false);
                            if (!qResult)
                            {
                                tbLog.AppendText(String.Format("QuickUsb.AsyncWait() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                // TODO: Wait on issued transactions
                                issuedOutRequests = 0;
                                Test_Stop();
                                return;
                            }
                            else if (length != 0) // Check if the transaction has completed
                            {
                                // Sanity check that we wrote the proper number of bytes
                                if (length != BufferByteSize)
                                {
                                    tbLog.AppendText(String.Format("Wrote incorrect number of bytes: {0} (Expected {1})" + Environment.NewLine, length, BufferByteSize));
                                }

                                // Successfuly data transfer
                                ++outTransCount;
                                BytesWritten += length;
                                --issuedOutRequests;

                                // Re-issue the data request
                                if (RunningTest)
                                {
                                    // Get the next set of data to write out
                                    GetNextOutLFSR(OutBufferArray[nextOutRequest], OutBufferArray[nextOutRequest].Length);

                                    length = BufferByteSize;
                                    qResult = qusb.WriteDataAsync(hOutBufferArray[nextOutRequest].AddrOfPinnedObject(), length, out transID[nextOutRequest]);
                                    if (!qResult)
                                    {
                                        tbLog.AppendText(String.Format("QuickUsb.WriteDataAsync() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        // TODO: Wait on issued transactions
                                        issuedOutRequests = 0;
                                        Test_Stop();
                                        return;
                                    }
                                    else
                                    {
                                        ++issuedOutRequests;
                                    }
                                }

                                // Move to next buffer to wait on
                                if (++nextOutRequest >= NumBuffers)
                                {
                                    nextOutRequest = 0;
                                }
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();

                                // Since this is a single-threaded test, we must explicitly perform app undates
                                Application.DoEvents();

                                tLastUpdate = Environment.TickCount;
                            }
                        }
                        break;

                    /********************************************************/
                    case DataTestType.StreamingRead:
                    /********************************************************/
                        // Create the buffer array
                        if (UserAllocatedBuffers)
                        {
                            inArr = new IntPtr[NumBuffers];
                            for (int k = 0; k < NumBuffers; ++k)
                            {
                                inArr[k] = hInBufferArray[k].AddrOfPinnedObject();
                            }

                            // Start data stream using user-allocated managed buffers (they must be pinned so that the
                            // garbage collector doesn't move them)
                            qResult = qusb.ReadBulkDataStartStream(inArr, NumBuffers, (uint)BufferByteSize, callbackDelegate, IntPtr.Zero, out InStreamID, NumThreads, ThreadConcurrency);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                Test_Stop();
                                return;
                            }
                        }
                        else
                        {
                            // Start data stream
                            qResult = qusb.ReadBulkDataStartStream(IntPtr.Zero, NumBuffers, (uint)BufferByteSize, callbackDelegate, IntPtr.Zero, out InStreamID, NumThreads, ThreadConcurrency);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                Test_Stop();
                                return;
                            }
                        }

                        // Loop until the test ends
                        do
                        {
                            if (NumThreads == 0)
                            {
                                // For single-threaded streams, we must process the streams on this main thread 
                                // explicitly.  The ProcessStream function will block this thread until either a
                                // data request has completed or until the timeout has elapsed.  A timeout 
                                // indicates that the stream simply did not process any data requests in the given
                                // amount of time, not that a data request timed out.
                                qResult = qusb.ProcessStream(InStreamID, 50);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.Timeout)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.ProcessStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                // For multi-threaded streams, the worker threads will automatically process streams so we 
                                // don't have to worry about it.  We just want to be sure to not hog the processor.
                                Thread.Sleep(50);
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // If a transaction failed in the completion routine, we need to stop the test
                            if (CompletionRoutineFailure)
                            {
                                break;
                            }

                            // Perform GUI updates
                            Application.DoEvents();
                        } while (RunningTest);

                        // Stop the stream
                        tbLog.AppendText("Stopping Stream...");
                        qResult = qusb.StopStream(InStreamID, false); // Block until stream stops
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.StopStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        else
                        {
                            tbLog.AppendText("Streaming successfully stopped" + Environment.NewLine);
                        }
                        break;

                    /********************************************************/
                    case DataTestType.StreamingWrite:
                    /********************************************************/
                        // Create the buffer array
                        outArr = new IntPtr[NumBuffers];
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            outArr[k] = hOutBufferArray[k].AddrOfPinnedObject();

                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);
                        }

                        // Start data stream
                        qResult = qusb.WriteBulkDataStartStream(outArr, (uint)outArr.Length, (uint)BufferByteSize, callbackDelegate, IntPtr.Zero, out OutStreamID, NumThreads, ThreadConcurrency);
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            Test_Stop();
                            return;
                        }

                        // Loop until the test ends
                        do
                        {
                            if (NumThreads == 0)
                            {
                                // For single-threaded streams, we must process the streams on this main thread 
                                // explicitly.  The ProcessStream function will block this thread until either a
                                // data request has completed or until the timeout has elapsed.  A timeout 
                                // indicates that the stream simply did not process any data requests in the given
                                // amount of time, not that a data request timed out.
                                qResult = qusb.ProcessStream(OutStreamID, 50);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.Timeout)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.ProcessStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        RunningTest = false;
                                    }
                                }
                            }
                            else
                            {
                                // For multi-threaded streams, the worker threads will automatically process streams so we 
                                // don't have to worry about it (although we could if we wanted to, but then the true number
                                // of threads be be one higher that NumThreads).  For now, just don't hog the processor.
                                Thread.Sleep(50);
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_ReadBert();
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // If a transaction failed in the completion routine, we need to stop the test
                            if (CompletionRoutineFailure)
                            {
                                RunningTest = false;
                            }

                            // Perform GUI updates
                            Application.DoEvents();
                        } while (RunningTest);

                        // Stop the stream
                        tbLog.AppendText("Stopping Stream...");
                        qResult = qusb.StopStream(OutStreamID, false); // Block until stream stops
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.StopStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        else
                        {
                            tbLog.AppendText("Streaming successfully stopped" + Environment.NewLine);
                        }
                        break;

                    /********************************************************/
                    case DataTestType.StreamingLoopback:
                    /********************************************************/
                        // Create the write buffer array
                        outArr = new IntPtr[NumBuffers];
                        for (int k = 0; k < NumBuffers; ++k)
                        {
                            outArr[k] = hOutBufferArray[k].AddrOfPinnedObject();

                            // Get the next set of data to write out
                            GetNextOutLFSR(OutBufferArray[k], OutBufferArray[k].Length);
                        }

                        // Create the read buffer array
                        if (UserAllocatedBuffers)
                        {
                            inArr = new IntPtr[NumBuffers];
                            for (int k = 0; k < NumBuffers; ++k)
                            {
                                inArr[k] = hInBufferArray[k].AddrOfPinnedObject();
                            }
                        }

                        // Start write data stream
                        qResult = qusb.WriteBulkDataStartStream(outArr, NumBuffers, BufferByteSize, callbackDelegate, (IntPtr)1, out OutStreamID, NumThreads, ThreadConcurrency);
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.WriteBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                            Test_Stop();
                            return;
                        }

                        // Start read data stream
                        if (UserAllocatedBuffers)
                        {
                            qResult = qusb.ReadBulkDataStartStream(inArr, NumBuffers, BufferByteSize, callbackDelegate, (IntPtr)1, out InStreamID, NumThreads, ThreadConcurrency);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                Test_Stop();
                                return;
                            }
                        }
                        else
                        {
                            qResult = qusb.ReadBulkDataStartStream(IntPtr.Zero, NumBuffers, BufferByteSize, callbackDelegate, (IntPtr)1, out InStreamID, NumThreads, ThreadConcurrency);
                            if (!qResult)
                            {
                                ++Failures;
                                tbLog.AppendText(String.Format("QuickUsb.ReadBulkDataStartStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                Test_Stop();
                                return;
                            }
                        }

                        // Loop until the test ends
                        do
                        {
                            if (NumThreads == 0)
                            {
                                // For single-threaded streams, we must process the streams on this main thread 
                                // explicitly.  The ProcessStream function will block this thread until either a
                                // data request has completed or until the timeout has elapsed.  A timeout 
                                // indicates that the stream simply did not process any data requests in the given
                                // amount of time, not that a data request timed out.
                                qResult = qusb.ProcessStream(OutStreamID, 50);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.Timeout)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.ProcessStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        RunningTest = false;
                                    }
                                }
                                qResult = qusb.ProcessStream(InStreamID, 50);
                                if (!qResult)
                                {
                                    if (qusb.LastError() != QuickUsb.Error.Timeout)
                                    {
                                        ++Failures;
                                        tbLog.AppendText(String.Format("QuickUsb.ProcessStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                                        RunningTest = false;
                                    }
                                }
                            }
                            else
                            {
                                // For multi-threaded streams, the worker threads will automatically process streams so we 
                                // don't have to worry about it (although we could if we wanted to, but then the true number
                                // of threads be be one higher that NumThreads).  For now, just don't hog the processor.
                                Thread.Sleep(50);
                            }

                            // Update the stats every once in a while
                            if ((Environment.TickCount - tLastUpdate) > updateFreqMs)
                            {
                                Test_UpdateStats();
                                tLastUpdate = Environment.TickCount;
                            }

                            // If a transaction failed in the completion routine, we need to stop the test
                            if (CompletionRoutineFailure)
                            {
                                RunningTest = false;
                            }

                            // Perform GUI updates
                            Application.DoEvents();
                        } while (RunningTest);

                        // Wait for the write stream to fully shutdown
                        tbLog.AppendText("Stopping Write Stream...");
                        qResult = qusb.StopStream(OutStreamID, false); // Block until stream stops
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.StopStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        else
                        {
                            tbLog.AppendText("Write Streaming successfully stopped" + Environment.NewLine);
                        }

                        // Wait for the read stream to fully shutdown
                        tbLog.AppendText("Stopping Read Stream...");
                        qResult = qusb.StopStream(InStreamID, false); // Block until stream stops
                        if (!qResult)
                        {
                            ++Failures;
                            tbLog.AppendText(String.Format("QuickUsb.StopStream() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
                        }
                        else
                        {
                            tbLog.AppendText("Read Streaming successfully stopped" + Environment.NewLine);
                        }

                        break;
                }
            }

            // Stop the test
            Test_Stop();
        }

        private void cbScanInBackground_CheckedChanged(object sender, EventArgs e)
        {
            qusbControl.ScanEnabled = cbScanInBackground.Checked;
        }

        private void bUpdateCount_Click(object sender, EventArgs e)
        {
            Test_ReadBert();
            Test_UpdateStats();
        }

        private void bClearLog_Click(object sender, EventArgs e)
        {
            tbLog.Clear();
        }

        private void btConfig_ValueChanged(object sender, uint value)
        {
            bool qResult;

            // Write the new value
            qResult = qusb.WriteCommand(0, new byte[] { (byte)(btConfig.Value & 0xff), (byte)(btConfig.Value >> 8) }, 2);
            if (!qResult)
            {
                tbLog.AppendText(String.Format("QuickUsb.WriteCommand() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
            }

            // Read back the value
            byte[] inData = new byte[2];
            ushort length = 2;
            qResult = qusb.ReadCommand(0, inData, ref length);
            if (!qResult)
            {
                tbLog.AppendText(String.Format("QuickUsb.ReadCommand() failed with error: {0} - {1}" + Environment.NewLine, qusb.LastError(), qusb.LastDriverError()));
            }

            // If the read value is different, update the value to reflect it
            uint val = (uint)((inData[1] << 8) | inData[0]);
            if (val != btConfig.Value)
            {
                btConfig.Value = val;
            }

            Thread.Sleep(100);
        }

        private void cbGlobalOopen_CheckedChanged(object sender, EventArgs e)
        {
            if (qusb == null)
            {
                return;
            }

            if (cbGlobalOopen.Checked)
            {
                if (!qusb.OpenEx(QuickUsb.OpenFlag.OpenIfClosed))
                {
                    MessageBox.Show("QuickUsb.OpenEx() failed with error: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }
            else
            {
                if (!qusb.Close())
                {
                    MessageBox.Show("QuickUsb.Close() failed with error: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }
        }

        private void nTimeout_ValueChanged(object sender, EventArgs e)
        {
            if (qusb == null)
            {
                return;
            }

            qusb.SetTimeout((uint)nTimeout.Value);
        }

        private void bProgramFpga_Click(object sender, EventArgs e)
        {
            Test_InitFpga();
        }

        private void bRunTestSuite_Click(object sender, EventArgs e)
        {
            Test_RunTestSuite();
        }

        private void suiteTimer_Tick(object sender, EventArgs e)
        {
            bRunTest.Checked = false;
            suiteTimer.Stop();
        }

        private void bPauseStream_Click(object sender, EventArgs e)
        {
            DataTestType type = GetTestType((string)cbTestType.SelectedItem);

            if (type == DataTestType.StreamingWrite)
            {
                if (!qusb.PauseStream(OutStreamID, false))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }

            if (type == DataTestType.StreamingRead)
            {
                if (!qusb.PauseStream(InStreamID, false))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }

            if (type == DataTestType.StreamingLoopback)
            {
                // Pause the streams without blocking
                if (!qusb.PauseStream(OutStreamID, true))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
                if (!qusb.PauseStream(InStreamID, true))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }

                // Wait for the streams to fully pause
                if (!qusb.PauseStream(OutStreamID, false))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
                if (!qusb.PauseStream(InStreamID, false))
                {
                    log.AppendLine("Failed to pause stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }
        }

        private void bResumeStream_Click(object sender, EventArgs e)
        {
            DataTestType type = GetTestType((string)cbTestType.SelectedItem);

            if ((type == DataTestType.StreamingWrite) || (type == DataTestType.StreamingLoopback))
            {
                if (!qusb.ResumeStream(OutStreamID))
                {
                    log.AppendLine("Failed to resume stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }

            if ((type == DataTestType.StreamingRead) || (type == DataTestType.StreamingLoopback))
            {
                if (!qusb.ResumeStream(InStreamID))
                {
                    log.AppendLine("Failed to resume stream: " + qusb.LastError() + " - " + qusb.LastDriverError());
                }
            }
        }

        private void bRunTestSuite_CheckedChanged(object sender, EventArgs e)
        {
            if (bRunTestSuite.Checked)
            {
                bRunTestSuite.Text = "Stop Test Suite";
                Test_RunTestSuite();
            }
            else
            {
                bRunTestSuite.Text = "Run Test Suite";
                RunningTestSuite = false;
            }
        }

        private void bCopyLog_Click(object sender, EventArgs e)
        {
            if (tbLog.Text != null)
            {
                Clipboard.SetText(tbLog.Text);
            }
        }
    }
}
