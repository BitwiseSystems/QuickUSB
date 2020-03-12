using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using BitwiseSystems;

namespace QuickUsbThroughputTest
{
	public partial class mainForm : Form
	{
		[DllImport("Kernel32.dll")]
		private static extern bool QueryPerformanceCounter(
			out long lpPerformanceCount);

		[DllImport("Kernel32.dll")]
		private static extern bool QueryPerformanceFrequency(
			out long lpFrequency);

        [StructLayout(LayoutKind.Sequential)]
        class AsyncBulkStreamTag
        {
            public IntPtr Pointer;
            public byte shutdown;
        };

		bool quit = false;
		static private long freq;
		Thread testThread;
        QuickUsb qusb;

        long totalBytes;
        long startTime;
		int testPackSize;
		bool testRead;
        int streamBytes;
        byte streamID;
        QuickUsb.BulkStreamCompletionRoutine AsyncCompletionRoutineDelegate;
        QuickUsb.BulkStreamCompletionRoutine StreamCompletionRoutineDelegate;

		public mainForm()
		{
			if (QueryPerformanceFrequency(out freq) == false)
			{
				throw new Win32Exception();
			}

            InitializeComponent();
            AsyncCompletionRoutineDelegate = new QuickUsb.BulkStreamCompletionRoutine(AsyncCompletionRoutine);
            StreamCompletionRoutineDelegate = new QuickUsb.BulkStreamCompletionRoutine(StreamCompletionRoutine);
		}

		private void mainForm_Load(object sender, EventArgs e)
		{
			cbPacketSizeUnit.SelectedIndex = 1;
            rbAcqType_CheckedChanged(sender, e);
		}

		private void bStart_Click(object sender, EventArgs e)
		{
			string[] modules = QuickUsb.FindModules();
			if (modules.Length == 0)
			{
				return;
			}
			bStart.Enabled = false;
			bStop.Enabled = true;
			quit = false;

			testRead = rbRead.Checked;
			testPackSize = (int)nPacketSize.Value * (int)Math.Pow((double)1024, (double)cbPacketSizeUnit.SelectedIndex);
			updateTimer.Enabled = true;

            rbSync.Enabled = false;
            rbStreaming.Enabled = false;
            rbAsync.Enabled = false;
            rbAsyncLegacy.Enabled = false;

			testThread = new Thread(performTest);
            totalBytes = 0;
			testThread.Start();
		}

        private void AsyncCompletionRoutine(QuickUsb.BulkStream bulkStream)
        {
            // Retrieve our user data
            GCHandle hTag = GCHandle.FromIntPtr(bulkStream.Tag);
            AsyncBulkStreamTag tag = (AsyncBulkStreamTag)hTag.Target;

            // Check for errors
            if (bulkStream.Error != 0)
            {
                //Console.WriteLine("--> CR: ERROR! Request #{0} failed with error {1} ({2} of {3} bytes)", tag.Id, BulkStream.Error, BulkStream.BytesTransferred, BulkStream.BytesRequested);
            }
            else
            {
                lock (testThread)
                {
                    streamBytes += (int)bulkStream.BytesTransferred;
                    totalBytes += (int)bulkStream.BytesTransferred;
                }

                if (tag.shutdown == 0)
                {
                    if (!qusb.ReadBulkDataAsync(bulkStream.Buffer, bulkStream.BytesRequested, bulkStream, AsyncCompletionRoutine, bulkStream.Tag))
                    {
                    }
                }
                //Console.WriteLine("--> CR: Completed Request #{0} for {1} bytes", tag.Id, BulkStream.BytesTransferred);
            }
        }

        private void StreamCompletionRoutine(QuickUsb.BulkStream bulkStream)
        {
            lock (testThread)
            {
                streamBytes += (int)bulkStream.BytesTransferred;
                totalBytes += (int)bulkStream.BytesTransferred;
            }
        }

		private void performTest()
		{
			// Open the module
			string[] modules = QuickUsb.FindModules();
			if (modules.Length == 0)
			{
				return;
			}
			qusb = new QuickUsb(modules[0]);
            qusb.Open(modules[0]);

			int k, transBytes = 0;
            bool transOk;

            // Async Variables
            QuickUsb.BulkStream[] pBulkStream = null;
            ushort[][] pDataBuffer = null;
            AsyncBulkStreamTag[] asyncTag = null;
            GCHandle[] hAsyncTag = null;

            // Legacy Async Variables
            int issuedTrans = 0, nextToComplete = 0;
            IntPtr[] bufferArray = null;
            byte[] transId = null;

            // Streaming Variables
            bool streaming = false;

			int dataSize = 0;
            int numBuffers = 0;
			byte[] data = null;
			uint size, bytesTransferred;
			QuickUsb.Error qusbError;

			// Start timing
			QueryPerformanceCounter(out startTime);

			// Perform the test
			do
			{
				// Check for modification to configuration parameters
				if (dataSize != testPackSize || numBuffers != (int)nNumBuffers.Value)
				{
                    dataSize = testPackSize;
                    numBuffers = (int)nNumBuffers.Value;

                    if (rbSync.Checked)
                    {
                        data = new byte[dataSize];
                    }

                    if (rbAsync.Checked)
                    {
                        // Wait for all issued transactions to complete
                        while (issuedTrans > 0)
                        {
                            if (!qusb.BulkWait(pBulkStream[nextToComplete], false))
                            {
                            }
                            else
                            {
                                transOk = true;
                                --issuedTrans;
                                if (++nextToComplete == pBulkStream.Length)
                                {
                                    nextToComplete = 0;
                                }
                            }
                        }

                        // Free memories
                        if (hAsyncTag != null)
                        {
                            for (k = 0; k < hAsyncTag.Length; ++k)
                            {
                                hAsyncTag[k].Free();
                            }
                        }

                        // Allocate memories
                        pDataBuffer = new ushort[numBuffers][];
                        pBulkStream = new QuickUsb.BulkStream[numBuffers];
                        asyncTag = new AsyncBulkStreamTag[numBuffers];
                        hAsyncTag = new GCHandle[numBuffers];
                        for (k = 0; k < numBuffers; ++k)
                        {
                            pBulkStream[k] = new QuickUsb.BulkStream();
                            pDataBuffer[k] = new ushort[dataSize / 2];
                            asyncTag[k] = new AsyncBulkStreamTag();
                            hAsyncTag[k] = GCHandle.Alloc(asyncTag[k], GCHandleType.Pinned);
                            asyncTag[k].shutdown = 0;
                        }

                        // Reset varaibles
                        issuedTrans = 0;
                        nextToComplete = 0;

                        // Issue new transactions
                        while (issuedTrans != numBuffers)
                        {
                            if (!qusb.ReadBulkDataAsync(pDataBuffer[nextToComplete], (uint)dataSize, pBulkStream[nextToComplete], AsyncCompletionRoutineDelegate, GCHandle.ToIntPtr(hAsyncTag[nextToComplete])))
                            {
                            }
                            else
                            {
                                ++issuedTrans;
                                if (++nextToComplete == numBuffers)
                                {
                                    nextToComplete = 0;
                                }
                            }
                        }
                    }

                    if (rbAsyncLegacy.Checked)
                    {
                        // Wait for all issued transactions to complete
                        while (issuedTrans > 0)
                        {
                            if (!qusb.AsyncWait(out bytesTransferred, transId[nextToComplete], false))
                            {
                            }
                            else
                            {
                                transOk = true;
                                --issuedTrans;
                                if (++nextToComplete == transId.Length)
                                {
                                    nextToComplete = 0;
                                }
                            }
                        }

                        // Free old memories
                        if (bufferArray != null)
                        {
                            for (k = 0; k < bufferArray.Length; ++k)
                            {
                                Marshal.FreeHGlobal(bufferArray[k]);
                            }
                        }

                        // Allocate memories
                        transId = new byte[numBuffers];
                        bufferArray = new IntPtr[numBuffers];
                        for (k = 0; k < bufferArray.Length; ++k)
                        {
                            bufferArray[k] = Marshal.AllocHGlobal(dataSize);
                        }

                        // Reset varaibles
                        issuedTrans = 0;
                        nextToComplete = 0;
                    }

                    if (rbStreaming.Checked)
                    {
                        // Stop the stream if one is running
                        if (streaming)
                        {
                            if (!qusb.StopStream(streamID, false))
                            {
                            }
                        }

                        // Restart the stream with new parameters
                        if (!qusb.ReadBulkDataStartStream(IntPtr.Zero, (uint)numBuffers, (uint)dataSize, StreamCompletionRoutineDelegate, IntPtr.Zero, out streamID, 4, 2))
                        {
                        }
                    }
				}

				// Perform the data R/W
				size = (uint)dataSize;
                transOk = false;
                if (testRead)
                {
                    // Synchronous data read
                    if (rbSync.Checked)
                    {
                        if (!qusb.ReadData(data, ref size))
                        {
                            qusbError = qusb.LastError();
                            // Error
                            MessageBox.Show(String.Format("QuickUsbReadData() failed! - {0}", qusbError.ToString()), "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            quit = true;
                            transBytes = 0;
                            size = 0;
                        }
                        else if ((int)size != dataSize)
                        {
                            // Error	
                            MessageBox.Show(String.Format("QuickUsbReadData() did not read correct amount of data ({0} of {1} bytes)!", size, dataSize), "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            quit = true;
                            transBytes = 0;
                            size = 0;
                        }
                        else
                        {
                            transOk = true;
                            totalBytes += size;
                        }
                    }

                    // Asynchronous data read
                    else if (rbAsync.Checked)
                    {
                        transOk = true;
                        lock (testThread)
                        {
                            size = (uint)streamBytes;
                            streamBytes = 0;
                        }
                    }

                    // Legacy asynchronous data read
                    else if (rbAsyncLegacy.Checked)
                    {
                        // First wait on the next transaction to complete
                        bytesTransferred = 0;
                        if (issuedTrans > 0)
                        {
                            if (!qusb.AsyncWait(out bytesTransferred, transId[nextToComplete], false))
                            {
                                qusbError = qusb.LastError();
                                // Error
                                MessageBox.Show("Error", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                quit = true;
                                transBytes = 0;
                                size = 0;
                            }
                            else
                            {
                                transOk = true;
                                --issuedTrans;
                            }
                        }

                        // Issue new transactions
                        while (issuedTrans != numBuffers && !quit)
                        {
                            if (!qusb.ReadDataAsync(bufferArray[nextToComplete], ref size, out transId[nextToComplete]))
                            {
                                qusbError = qusb.LastError();
                                // Error
                                MessageBox.Show("Error", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                quit = true;
                                transBytes = 0;
                                size = 0;
                            }
                            else
                            {
                                ++issuedTrans;
                                if (++nextToComplete == numBuffers)
                                {
                                    nextToComplete = 0;
                                }
                            }
                        }

                        if (transOk)
                        {
                            size = bytesTransferred;
                        }
                    }

                    // Streaming data read
                    else if (rbStreaming.Checked)
                    {
                        transOk = true;
                        lock (testThread)
                        {
                            size = (uint)streamBytes;
                            streamBytes = 0;
                        }

                        // Nothing to do here
                        Thread.Sleep(250);
                    }
                }
                else if (rbSync.Checked)
				{
                    // Sychronous data write
					if (!qusb.WriteData(data, size))
					{
						qusbError = qusb.LastError();
						// Error
						MessageBox.Show("QuickUsbWriteData() failed!", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
						quit = true;
						transBytes = 0;
						size = 0;
                    }
                    else
                    {
                        transOk = true;
                        totalBytes += size;
                    }
				}

                if (transOk)
                {
                    transBytes += (int)size;
                }
			} while (!quit);

            if (rbAsync.Checked)
            {
                // Wait for all issued transactions to complete
                for (k = 0; k < pBulkStream.Length; ++k)
                {
                    asyncTag[k].shutdown = 1;
                }

                // Wait for pending transactions to complete
                qusb.BulkWait(null, false);

                /*while (issuedTrans > 0)
                {
                    if (!qusb.BulkWait(pBulkStream[nextToComplete], false))
                    {
                    }
                    else
                    {
                        transOk = true;
                        --issuedTrans;
                        if (++nextToComplete == numBuffers)
                        {
                            nextToComplete = 0;
                        }
                    }
                }*/

                // Free memory
                for (k = 0; k < pBulkStream.Length; ++k)
                {
                    hAsyncTag[k].Free();
                }
            }

            if (rbAsyncLegacy.Checked)
            {
                // Wait for all issued transactions to complete
                /*while (issuedTrans > 0)
                {
                    qusb.AsyncWait(out size, transId[nextToComplete], false);

                    --issuedTrans;
                    if (++nextToComplete == numBuffers)
                    {
                        nextToComplete = 0;
                    }
                }*/
            }

            if (rbStreaming.Checked)
            {
                // Stop the stream if one is running
                if (streaming)
                {
                    if (!qusb.StopStream(streamID, false))
                    {
                        qusbError = qusb.LastError();
                        // Error
                        MessageBox.Show("Error", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }

			qusb.Close();

            if (rbAsyncLegacy.Checked)
            {
                // Free memory
                for (k = 0; k < bufferArray.Length; ++k)
                {
                    Marshal.FreeHGlobal(bufferArray[k]);
                }
            }
		}

        private double MeasureThroughput(int count)
		{
			// Open the module
			string[] modules = QuickUsb.FindModules();
			if (modules.Length == 0)
			{
				return 0.0f;
			}
			QuickUsb qusb = new QuickUsb(modules[0]);

			// Perform throughput test
			int transBytes;
			long startTime, stopTime;
			double duration;
            double throughput;

			int dataSize = 0;
			byte[] data = null;
			uint size;
			QuickUsb.Error qusbError;

			// Check for modification to configuration parameters
			if (dataSize != testPackSize)
			{
				dataSize = testPackSize;
				data = new byte[dataSize];
			}

			// Start timing
			transBytes = 0;
			QueryPerformanceCounter(out startTime);

			// Perform the test
			for (int k = 0; k < count; ++k)
			{
				// Perform the data R/W
				size = (uint)dataSize;
				if (rbRead.Checked)
				{
					if (!qusb.ReadData(data, ref size))
					{
						// Error
						MessageBox.Show("QuickUsbReadData() failed!", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
						transBytes = 0;
						size = 0;
						return 0.0f;
					}
					else if ((int)size != dataSize)
					{
						// Error	
                        MessageBox.Show(String.Format("QuickUsbReadData() did not read correct amount of data ({0} of {1} bytes)!", size, dataSize), "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
						transBytes = 0;
						size = 0;
						return 0.0f;
					}
				}
				else
				{
					if (!qusb.WriteData(data, size))
					{
						qusbError = qusb.LastError();
						// Error
						MessageBox.Show("QuickUsbWriteData() failed!", "QuickUSB Throughput Test", MessageBoxButtons.OK, MessageBoxIcon.Error);
						transBytes = 0;
						size = 0;
						return 0.0f;
					}
				}
				transBytes += (int)size;
			}

			// Calculate the throughput
			QueryPerformanceCounter(out stopTime);
			duration = (double)(stopTime - startTime) / (double)freq;

			qusb.Close();

            throughput = ((double)(transBytes) / (1024.0f * 1024.0f * (double)duration));

			return throughput;

		}

		private void bStop_Click(object sender, EventArgs e)
		{
			// Kill the test thread, if it's running
			quit = true;
			if (testThread != null)
			{
				while (!testThread.IsAlive)
					;
				testThread.Join();
			}
			testThread = null;
			bStop.Enabled = false;
			bStart.Enabled = true;
            updateTimer.Enabled = false;

            rbSync.Enabled = true;
            rbStreaming.Enabled = true;
            rbAsync.Enabled = true;
            rbAsyncLegacy.Enabled = true;

		}

		private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			// Kill the test thread, if it's running
			quit = true;
			if (testThread != null)
			{
				testThread.Join();
			}
			updateTimer.Enabled = false;
			testThread = null;
		}

		private void updateTimer_Tick(object sender, EventArgs e)
        {
            long stopTime;
            double duration;

            // Calculate the throughput
            QueryPerformanceCounter(out stopTime);
            duration = (double)(stopTime - startTime) / (double)freq;
            double throughput = ((double)(totalBytes) / (1024.0f * 1024.0f * (double)duration));

			lThroughput.Text = String.Format("Throughput: {0:0.00} MB/s", throughput);
            progressBar.Value = Math.Min(progressBar.Maximum, (int)throughput);
			if (quit)
			{
				if (testThread != null)
				{
					testThread.Join();
				}
				bStop.Enabled = false;
				bStart.Enabled = true;
				updateTimer.Enabled = false;
			}
		}

		private void packetSize_ValueChanged(object sender, EventArgs e)
		{
			if (nPacketSize.Value > 16 && cbPacketSizeUnit.SelectedIndex == 2)
			{
				nPacketSize.Value = 16;
			}
			testPackSize = (int)nPacketSize.Value * (int)Math.Pow((double)1024, (double)cbPacketSizeUnit.SelectedIndex);
		}

		private void packetSizeUnit_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (nPacketSize.Value > 16 && cbPacketSizeUnit.SelectedIndex == 2)
			{
				nPacketSize.Value = 16;
			}
			testPackSize = (int)nPacketSize.Value * (int)Math.Pow((double)1024, (double)cbPacketSizeUnit.SelectedIndex);
		}

		private void rbRead_CheckedChanged(object sender, EventArgs e)
		{
			testRead = rbRead.Checked;
		}

		private void rbWrite_CheckedChanged(object sender, EventArgs e)
		{
			testRead = rbRead.Checked;
		}

		private void bMeasure_Click(object sender, EventArgs e)
		{
            double throughput = MeasureThroughput(1000);
			MessageBox.Show(String.Format("Throughput: {0:0.00} MB/s", throughput));
		}

        private void rbAcqType_CheckedChanged(object sender, EventArgs e)
        {
            nNumBuffers.Enabled = !rbSync.Checked;        
        }
	}
}
