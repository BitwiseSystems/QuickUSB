using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using BitwiseSystems;
using System.Runtime.InteropServices;

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

		bool quit = false;
		private long freq;

		public mainForm()
		{
			if (QueryPerformanceFrequency(out freq) == false)
			{
				// high-performance counter not supported
				throw new Win32Exception();
			}

			InitializeComponent();
		}

		private void mainForm_Load(object sender, EventArgs e)
		{
			packetSizeUnit.SelectedIndex = 1;
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

			// Open the module
			QuickUsb qusb = new QuickUsb(modules[0]);

			// Perform throughput test
			int transBytes;
			double[] samples = new double[20];
			int sampleIndex = 0;
			long startTime, stopTime;
			double duration, throughput;

			int dataSize = 0;
			byte[] data = null;
			uint size;

			// Start timing
			transBytes = 0;
			QueryPerformanceCounter(out startTime);
			progressBar.Value = 0;

			// Perform the test
			do
			{
				// Check for modification to configuration parameters
				if (dataSize != (int)packetSize.Value * (int)Math.Pow((double)1024, (double)packetSizeUnit.SelectedIndex))
				{
					dataSize = (int)packetSize.Value * (int)Math.Pow((double)1024, (double)packetSizeUnit.SelectedIndex);
					data = new byte[dataSize];
				}

				size = (uint)dataSize;
				if (!qusb.ReadData(data, ref size))
				{
					// Error
				}
				if ((int)size != dataSize)
				{
					// Error
				}
				transBytes += (int)size;

				// Calculate the throughput
				QueryPerformanceCounter(out stopTime);
				duration = (double)(stopTime - startTime) / (double)freq;
				samples[sampleIndex++] = transBytes / (1024 * 1024 * duration);
				if (sampleIndex >= samples.Length)
				{
					sampleIndex = 0;
				}
				throughput = 0;
				for (int k = 0; k < samples.Length; ++k)
				{
					throughput += samples[k];
				}
				throughput /= samples.Length;
				lthroughput.Text = String.Format("Throughput: {0:0.0} MB/s", throughput);
				progressBar.Value = Math.Min(30, (int)throughput);

				// Restart timer
				transBytes = 0;
				QueryPerformanceCounter(out startTime);

				// Perform GUI operations
				Application.DoEvents();
			} while (!quit);

			qusb.Close();
		}

		private void bStop_Click(object sender, EventArgs e)
		{
			quit = true;
			bStop.Enabled = false;
			bStart.Enabled = true;
		}

		private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			quit = true;
		}
	}
}
