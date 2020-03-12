/* ========================================================================
	Title        : 
	Description  : 
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
using System.Runtime.InteropServices;
using BitwiseSystems;
using System.Drawing.Imaging;
using System.Threading;

namespace QuickUsbBulkAsyncImagingCs
{
    public partial class Form1 : Form
    {
        [DllImport("Kernel32.dll", EntryPoint = "RtlMoveMemory", SetLastError = false)]
        static extern void MoveMemory(IntPtr dest, IntPtr src, int size);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool QueryPerformanceFrequency(out long frequency);


        // Frame buffer info
        const int NumBuffers = 1;
        const int FrameWidth = 1280;
        const int FrameHeight = 800;
        const int FrameByteSize = FrameWidth * FrameHeight;
        const int LOOPS = 2; // Must be at least 2

        // Variables
        Bitmap image;
        QuickUsb qusb;
        string[] nameList;
        bool qResult;
        QuickUsb.Error qusbError;
        List<IntPtr> BufferArray = new List<IntPtr>();
        IntPtr[] BulkStream = new IntPtr[NumBuffers];
        QuickUsb.BulkStreamCompletionRoutine cbDelegate;
        bool aquiring = false;

        long tStart, tEnd, freq;
        double tElapsed;

        public Form1()
        {
            InitializeComponent();

            cbDelegate = new QuickUsb.BulkStreamCompletionRoutine(CompletionRoutine);

            pbFrame.Size = new Size(FrameWidth, FrameHeight);
            int width = (this.Width - this.ClientSize.Width) + pbFrame.Width + 6;
            int height = (this.Height - this.ClientSize.Height) + pbFrame.Height + statusStrip1.Height + 6;
            this.Size = new Size(width, height);

            image = new Bitmap(FrameWidth, FrameHeight, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
            ColorPalette palette = image.Palette;
            for (int k = 0; k < palette.Entries.Length; ++k)
            {
                palette.Entries[k] = Color.FromArgb(255, k, k, k);
            }
            image.Palette = palette;

            // Set the initial image (black)
            pbFrame.Image = image;
        }

        void StartAcquisition()
        {
            // Query connected modules
            nameList = QuickUsb.FindModules();
            if (nameList.Length == 0)
            {
                return;
            }

            // Open the first module
            qusb = new QuickUsb(nameList[0]);
            qusb.Open(nameList[0]);

            // Allocate buffers
            for (int k = 0; k < NumBuffers; ++k)
            {
                BufferArray.Add(Marshal.AllocHGlobal(FrameByteSize));
                BulkStream[k] = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(QuickUsb.BulkStream)));
            }

            QueryPerformanceFrequency(out freq);
            QueryPerformanceCounter(out tStart);

            aquiring = true;
            timer1.Enabled = true;

            // Issue a new transaction
            qResult = qusb.ReadBulkDataAsync(
                BufferArray[0],
                FrameByteSize,
                BulkStream[0],
                cbDelegate,
                IntPtr.Zero);

            if (!qResult)
            {
                toolStripStatusLabel1.Text = "Error: Unable to issue data read";
                qusb.Close();
                return;
            }

        }

        void StopAcquisition()
        {
            // Close the module
            qResult = qusb.Close();

            // Free buffers
            for (int k = 0; k < NumBuffers; ++k)
            {
                Marshal.FreeHGlobal(BufferArray[k]);
                Marshal.FreeHGlobal(BulkStream[k]);
            }

            aquiring = false;
            timer1.Enabled = false;
        }

        void CompletionRoutine(QuickUsb.BulkStream bs)
        {
            // Check for errors
            if (bs.Error == 0)
            {
                /*// Calculate FPS
                QueryPerformanceCounter(out tEnd);
                tElapsed = (double)(tEnd - tStart) / (double)(freq);
                tStart = tEnd;
                toolStripStatusLabel1.Text = String.Format("FPS: {0:0.0}", 1.0 / tElapsed);

                // Blit data to screen
                BitmapData bData = image.LockBits(new Rectangle(0, 0, image.Width, image.Height), ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                MoveMemory(bData.Scan0, bs.Buffer, FrameByteSize);
                image.UnlockBits(bData);
                pbFrame.Invalidate();*/
            }
        }


        // QuickUSB Asynchronous Imaging Example
        void Acquire()
        {
            int k = 0;

            if (!aquiring)
            {
                return;
            }

            // Wait for the transaction to complete (The completion routine will be called)
            do
            {
                qResult = qusb.BulkWait(BulkStream[k], false);
                if (!qResult)
                {
                    qusbError = qusb.LastError();
                    if (qusbError == QuickUsb.Error.Timeout)
                    {
                        // While we wait for this request to complete, we could be performing some
                        // background tasks
                        timer1.Enabled = false;
                        Application.DoEvents();
                        timer1.Enabled = true;
                    }
                }
            } while (!qResult && qusb.LastError() == QuickUsb.Error.Timeout);

            if (!qResult)
            {
                qusbError = qusb.LastError();
                return;
            }
            // Calculate FPS
            QueryPerformanceCounter(out tEnd);
            tElapsed = (double)(tEnd - tStart) / (double)(freq);
            tStart = tEnd;
            toolStripStatusLabel1.Text = String.Format("FPS: {0:0.0}", 1.0 / tElapsed);

            // Blit data to screen
            BitmapData bData = image.LockBits(new Rectangle(0, 0, image.Width, image.Height), ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
            MoveMemory(bData.Scan0, BufferArray[0], FrameByteSize);
            image.UnlockBits(bData);
            pbFrame.Invalidate();

            // Issue a new transaction
            qResult = qusb.ReadBulkDataAsync(
                BufferArray[0],
                FrameByteSize,
                BulkStream[0],
                cbDelegate,
                IntPtr.Zero);

            if (!qResult)
            {
                toolStripStatusLabel1.Text = "Error: Unable to issue data read";
                qusb.Close();
                return;
            }

        }

        private void pbFrame_Click(object sender, EventArgs e)
        {
            if (!aquiring)
            {
                StartAcquisition();
            }
            else
            {
                StopAcquisition();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Acquire();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (aquiring)
            {
                StopAcquisition();
            }
        }
    }
}
