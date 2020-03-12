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
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using BitwiseSystems;
using System.Reflection;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;
using System.Diagnostics;

namespace QuickUsbDiagCs
{
	public partial class QuickUsbDiag : Form
    {
        enum LastAction { None, ReadCommand, WriteCommand, ReadData, WriteData, WriteDataCount, CountPortA, ReadPortA, ReadPortB, ReadPortC, ReadPortD, ReadPortE, ReadRS232, WriteRS232, GetRS232Num, ReadSpi, WriteSpi, WriteReadSpi, ReadI2C, WriteI2C };

		#region Instance Variables

        // Variables
        bool skipSettingAccess;
        byte readStreamID = 255, writeStreamID = 255;
        bool reportReadStatistics = false;
        bool reportWriteStatistics = false;
        ushort writeStreamCount;
        QuickUsb.BulkStreamCompletionRoutine streamCompletionRoutine = new QuickUsb.BulkStreamCompletionRoutine(streamCompletionRoutineFunc);
        bool disableUsbSpeedCheckChanged = false;

		// Status message info
		Queue<string> statusMsgs = new Queue<string>();
		int msgTime = 0;
		const int MESG_WAIT_TIME = 1500; // 1.0 seconds

        // To allow for automatic action repeating, we must keep track of the last action
        LastAction lastAction = LastAction.None;

		#endregion

		#region DLL Imports and Properties

        // Convenience function to access currently selected module
		public BitwiseSystems.QuickUsb Module		  
		{
			get
			{
				return quickUsbWinForms.SelectedModule;
			}
		}

        // The directory that the executing assembly is located in
        static public string AssemblyDirectory
        {
            get
            {
                string codeBase = System.Reflection.Assembly.GetExecutingAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        // The directory that the winhex.exe program is located in
        static public string WinHexPath
        {
            get
            {
                string asmPath = AssemblyDirectory;
                string rootPath = asmPath;

                // First, check if winhex.exe if in the same directory as the assembly
                if (File.Exists(Path.Combine(asmPath, "winhex.exe")))
                {
                    return Path.Combine(asmPath, "winhex.exe");
                }

                // Look for a winhex directory, traveling up directories until we find one
                string[] matches;
                while (true)
                {
                    matches = Directory.GetDirectories(rootPath, "winhex");
                    if (matches.Length == 0)
                    {
                        // Back up a directory
                        rootPath = Path.GetFullPath(Path.Combine(rootPath, ".."));

                        // Check if we've backed up to the root drive
                        if (rootPath == Directory.GetDirectoryRoot(asmPath))
                        {
                            // Check for winhex in the QuickUSB Install directory
                            string installDir = Environment.GetEnvironmentVariable("QUICKUSB_DIR");
                            if ((installDir != null) && File.Exists(Path.Combine(Path.Combine(Path.Combine(installDir, "Tools"), "winhex"), "winhex.exe")))
                            {
                                return Path.Combine(Path.Combine(Path.Combine(installDir, "Tools"), "winhex"), "winhex.exe");
                            }

                            return null;
                        }
                    }
                    else
                    {
                        // We've found the a winhex directory
                        rootPath = Path.Combine(Path.Combine(rootPath, "winhex"), "winhex.exe");

                        // Verify that winhex.exe is in the winhex folder
                        if (!File.Exists(rootPath))
                        {
                            // winhex.exe is not in the winhex folder!
                            return null;
                        }

                        break;
                    }
                }

                return rootPath;
            }
        }

		#endregion

		#region Public Methods

		// Contructor
		public QuickUsbDiag()
		{
            InitializeComponent();

            // Set application title
            Version v = Assembly.GetExecutingAssembly().GetName().Version;
            this.Text = AboutBox.AssemblyTitle + String.Format(" v{0}.{1}.{2}", v.Major, v.Minor, v.Build);

            // Set up other controls
            tbFpgaFile.SelectionStart = tbFpgaFile.Text.Length;
            tbFpgaFile.ScrollToCaret();
            cbSpiReadPort.SelectedIndex = 0;
            cbSpiWritePort.SelectedIndex = 0;
            cbEpcsPort.SelectedIndex = 0;
            cbRS232Port.SelectedIndex = 0;
            cbRS232Baud.SelectedIndex = 5;
		}

        // FPGA programming progress callback used to update the progress bar
		public void UpdateFpgaProgressBar(int value)
		{
			if (value > 0)
			{
				pbProgress.Visible = true;
				pbProgress.Value = value;
			}
			else
			{
				pbProgress.Visible = false;
			}
			ssStatus.Refresh();
		}

        // Function to queue a message to display on the status bar
		public void ShowStatusMessage(string message)
		{
			statusMsgs.Enqueue(message);
		}

		#endregion

		#region Event Handlers

        private void QuickUsbDiag_Load(object sender, EventArgs e)
        {
            // Load the setting tooltips from the embedded resource file and associate
            // them with the settings and defaults BitTwiddlers. This must be done after
            // the controls are created.
            SetToolTips();
        }

        // Timer event to control the message displayed in the status bar, handle statistics
        // updates, and monitor the status of data streams
        private void statusTimer_Tick(object sender, EventArgs e)
        {
            bool success;
            bool writePaused = false;
            bool readPaused = false;

            if (Module == null)
            {
                readStreamID = 255;
                writeStreamID = 255;
                reportReadStatistics = false;
                reportWriteStatistics = false;

                // Update enabled controls
                gbStreamingGeneral.Enabled = true;
                foreach (Control ctrl in gbStreamingRead.Controls)
                {
                    ctrl.Enabled = (ctrl != bReadStreamStop);
                }
                bReadStreamStart.Text = "Start";

                // Update enabled controls
                gbStreamingGeneral.Enabled = true;
                foreach (Control ctrl in gbStreamingWrite.Controls)
                {
                    ctrl.Enabled = (ctrl != bWriteStreamStop);
                }
                bWriteStreamStart.Text = "Start";
            } 
            else 
            {
                // Check on the status of the read stream
                if (readStreamID != 255)
                {
                    QuickUsb.StreamStatus status;
                    QuickUsb.Error error;

                    success = Module.GetStreamStatus(readStreamID, out status, out error);
                    if (!success)
                    {
                        ShowStatusMessage(String.Format("QuickUsb.GetStreamStatus failed with error: {0}", error.ToString()));
                        //return; // Don't return or the message will never be displayed
                    }
                    else
                    {
                        readPaused = (status == QuickUsb.StreamStatus.Paused);

                        // Report any stream errors
                        if (error != QuickUsb.Error.NoError)
                        {
                            ShowStatusMessage(String.Format("Read stream reported error: {0}", error.ToString()));
                            bReadStreamStop_Click(sender, e);
                        } else if ((status != QuickUsb.StreamStatus.Running) && !readPaused)
                        {
                            bReadStreamStop_Click(sender, e);
                            ShowStatusMessage(String.Format("Read stream reported status: {0}", status.ToString()));
                        }
                    }
                }

                // Check on the status of the write stream
                if (writeStreamID != 255)
                {
                    QuickUsb.StreamStatus status;
                    QuickUsb.Error error;

                    success = Module.GetStreamStatus(writeStreamID, out status, out error);
                    if (!success)
                    {
                        ShowStatusMessage(String.Format("QuickUsb.GetStreamStatus failed with error: {0}", error.ToString()));
                        //return; // Don't return or the message will never be displayed
                    }
                    else
                    {
                        writePaused = (status == QuickUsb.StreamStatus.Paused);

                        // Report any stream errors
                        if (error != QuickUsb.Error.NoError)
                        {
                            ShowStatusMessage(String.Format("Write stream reported error: {0}", error.ToString()));
                            bWriteStreamStop_Click(sender, e);
                        }
                        else if ((status != QuickUsb.StreamStatus.Running) && !writePaused)
                        {
                            bWriteStreamStop_Click(sender, e);
                            ShowStatusMessage(String.Format("Write stream reported status: {0}", status.ToString()));
                        }
                    }
                }

                // Update read statistics
                if (reportReadStatistics && !readPaused)
                {
                    float throughput, megabytes;

                    // Read statistics
                    success = Module.GetStatistic(QuickUsb.Stat.ReadThroughput, QuickUsb.StatUnit.MBPerSec, out throughput, 0);
                    success = Module.GetStatistic(QuickUsb.Stat.TotalDataRead, QuickUsb.StatUnit.MB, out megabytes, 0);

                    // Only update throughput when the stream is running
                    if (readStreamID != 255)
                    {
                        labelReadStreamRate.Text = String.Format("{0:0.0} MB/s", throughput);
                    }

                    if (megabytes > 1024.0)
                    {
                        labelReadStreamCount.Text = String.Format("{0:0.00} GB", megabytes / 1024.0);
                    }
                    else
                    {
                        labelReadStreamCount.Text = String.Format("{0:0.00} MB", megabytes);
                    }

                    // If we're not streaming, stop reporting statistics
                    reportReadStatistics = (readStreamID != 255);
                }

                // Update write statistics
                if (reportWriteStatistics && !writePaused)
                {
                    float throughput, megabytes;

                    // Read statistics
                    success = Module.GetStatistic(QuickUsb.Stat.WriteThroughput, QuickUsb.StatUnit.MBPerSec, out throughput, 0);
                    success = Module.GetStatistic(QuickUsb.Stat.TotalDataWritten, QuickUsb.StatUnit.MB, out megabytes, 0);
                    
                    // Only update throughput when the stream is running
                    if (writeStreamID != 255)
                    {
                        labelWriteStreamRate.Text = String.Format("{0:0.0} MB/s", throughput);
                    }

                    if (megabytes > 1024.0)
                    {
                        labelWriteStreamCount.Text = String.Format("{0:0.00} GB", megabytes / 1024.0);
                    }
                    else
                    {
                        labelWriteStreamCount.Text = String.Format("{0:0.00} MB", megabytes);
                    }

                    // If we're not streaming, stop reporting statistics
                    reportWriteStatistics = (writeStreamID != 255);
                }
            }

            // Get the time since the last message was displayed
            int time = Environment.TickCount - msgTime;

            // If time is up or a meesage is not being displayed...
            if (toolStripStatusLabel.Text == "" || (time > MESG_WAIT_TIME) || statusMsgs.Count > 0)
            {
                // Check if there is a message waiting to be displayed
                if (statusMsgs.Count != 0)
                {
                    if (toolStripStatusLabel.Text == statusMsgs.Peek())
                    {
                        // This causes repeated messages to flash away to clearly
                        // show that the message has been repeated
                        toolStripStatusLabel.Text = "";
                    }
                    else
                    {
                        // Displayed the next message and reset the time counter
                        toolStripStatusLabel.Text = statusMsgs.Dequeue();
                        msgTime = Environment.TickCount;
                    }
                }
                else
                {
                    // Check if the message time has elapsed
                    if (time > MESG_WAIT_TIME)
                    {
                        // There are no messages to display so just clear the status bar
                        toolStripStatusLabel.Text = "";
                    }
                }
            }
        }

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
            // Exit the application
			this.Close();
		}

		private void QuickUsbDiag_FormClosing(object sender, FormClosingEventArgs e)
		{
            // Save application settings
            Properties.Settings.Default.Save();
		}

		private void quickUSBFAQToolStripMenuItem_Click(object sender, EventArgs e)
		{
            // Launch default browser to QuickUSB FAQ web address
			System.Diagnostics.Process proc = new System.Diagnostics.Process();
			proc.StartInfo.FileName = "iexplore";
			proc.StartInfo.Arguments = "http://www.quickusb.com/index.php?main_page=page&id=5";
			proc.Start();
		}

		private void quickUSBSupportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Launch default browser to QuickUSB Support web address
			System.Diagnostics.Process proc = new System.Diagnostics.Process();
			proc.StartInfo.FileName = "iexplore";
			proc.StartInfo.Arguments = "http://www.quickusb.com/support/my_view_page.php";
			proc.Start();
		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
            // Show about dialog
			BitwiseSystems.AboutBox about = new BitwiseSystems.AboutBox();
			about.ShowDialog();
		}

        private void quickUsbWinForms_SelectionChanged(object sender, EventArgs e)
        {
            // Update the control values on the selected tab
            UpdateTabCtrl(tabCtrl.SelectedTab);

            // Update the '.Enabled' property of the controls on the selected tab
            UpdateTabCtrlEnable(tabCtrl.SelectedTab);
        }

        private void quickUsbWinForms_ModuleConnected(object sender, QuickUsbWinForms.ModuleEventArgs e)
        {
            // If no module is selected, then select the recently connected module
            if ((quickUsbWinForms.Modules.Length > 0) && (Module == null))
            {
                quickUsbWinForms.SelectedModule = e.qusb;
            }
        }

        private void tabCtrl_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Update the control values on the selected tab
            UpdateTabCtrl(tabCtrl.SelectedTab);

            // Update the '.Enabled' property of the controls on the selected tab
            UpdateTabCtrlEnable(tabCtrl.SelectedTab);

            
            // Set the location of the settings/defaults helper labels.  This prevents misalignment of
            // the labels when the system is using non-standard fonts.
            switch (tabCtrl.SelectedTab.Text)
            {
                case "Settings":
                    labelSetting15_1.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[15].Location.X, labelSetting15_1.Location.Y);
                    labelSetting15_2.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[15].Location.X, labelSetting15_1.Location.Y);
                    labelSetting8_1.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[8].Location.X, labelSetting15_1.Location.Y);
                    labelSetting8_2.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[8].Location.X, labelSetting15_1.Location.Y);
                    labelSetting7_1.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[7].Location.X, labelSetting15_1.Location.Y);
                    labelSetting7_2.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[7].Location.X, labelSetting15_1.Location.Y);
                    labelSetting0_1.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[0].Location.X, labelSetting15_1.Location.Y);
                    labelSetting0_2.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[0].Location.X, labelSetting15_1.Location.Y);
                    labelSettingMSB.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[12].Location.X, labelSetting15_1.Location.Y);
                    labelSettingLSB.Location = new Point(epcfgBitTwiddler.Location.X + epcfgBitTwiddler.CheckBoxes[4].Location.X, labelSetting15_1.Location.Y);
                    break;

                case "Defaults":
                    labelDefault15_1.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[15].Location.X, labelSetting15_1.Location.Y);
                    labelDefault15_2.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[15].Location.X, labelSetting15_1.Location.Y);
                    labelDefault8_1.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[8].Location.X, labelSetting15_1.Location.Y);
                    labelDefault8_2.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[8].Location.X, labelSetting15_1.Location.Y);
                    labelDefault7_1.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[7].Location.X, labelSetting15_1.Location.Y);
                    labelDefault7_2.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[7].Location.X, labelSetting15_1.Location.Y);
                    labelDefault0_1.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[0].Location.X, labelSetting15_1.Location.Y);
                    labelDefault0_2.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[0].Location.X, labelSetting15_1.Location.Y);
                    labelDefaultMSB.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[12].Location.X, labelSetting15_1.Location.Y);
                    labelDefaultLSB.Location = new Point(epcfgDBitTwiddler.Location.X + epcfgDBitTwiddler.CheckBoxes[4].Location.X, labelSetting15_1.Location.Y);
                    break;
            }
        }

        private void rbUsbSpeed_CheckedChanged(object sender, EventArgs e)
        {
            ushort value, cpuConfig;
            byte speed, version;

            // Make sure the device is still connected and selected
            if (Module == null || disableUsbSpeedCheckChanged)
            {
                return;
            }

            // Get the QuickUSB VesionSpeed Info
            if (Module.ReadSetting(QuickUsb.Setting.VersionSpeed, out value))
            {
                version = (byte)((value / 0xff) & 0xff);
                speed = (byte)(value & 0xff);

                // Read old setting
                if (!Module.ReadSetting(QuickUsb.Setting.CpuConfig, out cpuConfig))
                {
                    return;
                }

                // Decode the VersionSpeed info into a meaningful string
                if ((version >= 0x00) && (version <= 04))
                {
                    // QUSB2 Device
                    if (rbFullSpeed.Checked)
                    {
                        cpuConfig &= 0x7FFF;
                    }
                    else
                    {
                        cpuConfig |= 0x8000;
                    }
                }
                else if (version == 0x10)
                {
                    // QUSB3 Device
                    if (rbSuperSpeed.Checked)
                    {
                        cpuConfig |= 0xC000;
                    }
                    else if (rbHighSpeed.Checked)
                    {
                        cpuConfig = (ushort)((cpuConfig & 0x3FFF) | 0x8000);
                    }
                    else if (rbFullSpeed.Checked)
                    {
                        cpuConfig &= 0x3FFF;
                    }
                }
                else
                {
                    // Unknown Device
                    return;
                }

                //quickUsbWinForms.ScanInterval = 50;

                QuickUsb qusb = Module;
                quickUsbWinForms.SelectedModule = null;

                // Write new setting
                bool success = qusb.WriteSetting(QuickUsb.Setting.CpuConfig, cpuConfig);
                if (!success)
                {
                    quickUsbWinForms.ScanInterval = 500;
                    ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", qusb.LastError().ToString()));
                    return;
                }

                // Wait a little while for the device to re-enumerate
                int sTime = Environment.TickCount;
                do
                {
                    Application.DoEvents();
                } while ((Module == null) && (Environment.TickCount - sTime) < 500);

                // If there is not a selected module, select one
                if ((Module == null) && (quickUsbWinForms.Modules.Length > 0))
                {
                    quickUsbWinForms.SelectedModule = quickUsbWinForms.Modules[0];
                }

                //quickUsbWinForms.ScanInterval = 500;
                // Force a re-scan for devices
                //quickUsbWinForms.FlushDevices();
                /*do
                {
                    Application.DoEvents();
                } while (Module == null);*/

                //UpdateTabCtrl(tabGeneral);
            }
        }

        private void isFpgaConfiguredButton_Click(object sender, EventArgs e)
        {
            bool configured;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check if the FPGA is configured (This is only accurate if the module is actually connected to a FPGA)
            if (Module.IsFpgaConfigured(out configured))
            {
                if (configured)
                {
                    ShowStatusMessage("FPGA is configured");
                }
                else
                {
                    ShowStatusMessage("FPGA is not configured");
                }
            }
            else
            {
                ShowStatusMessage("QuickUsb.IsFpgaConfgured failed with error: " + Module.LastError().ToString());
            }
        }

        private void browseFpgaFileButton_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbFpgaFile.Text;
            openFileDialog.Filter = "Altera RBF Files (*.rbf)|*.rbf|Xilinx BIN Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbFpgaFile.Text = openFileDialog.FileName;
                tbFpgaFile.SelectionStart = tbFpgaFile.Text.Length;
                tbFpgaFile.ScrollToCaret();
            }
        }

        private void uploadFpgaButton_Click(object sender, EventArgs e)
        {
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // See which Radiobutton is checked and set the configuration type
            if (alteraPsRadioButton.Checked)
            {
                success = Module.WriteSetting(QuickUsb.Setting.FpgaType, (ushort)QuickUsb.FpgaConfType.FPGATYPE_ALTERAPS);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                }
            }
            else if (xilinxSsRadioButton.Checked)
            {
                success = Module.WriteSetting(QuickUsb.Setting.FpgaType, (ushort)QuickUsb.FpgaConfType.FPGATYPE_XILINXSS);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                }
            }
            else
            {
                ShowStatusMessage("Cannot Configure FPGA: Invalid configuration type");
                return;
            }

            // Check to see if we can reset the FPGA
            // If not, the FPGA probably isn't on or connected up right
            success = Module.StartFpgaConfiguration();
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.StartFpgaConfiguration failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Disable form controls to prevent breaking stuff
            quickUsbWinForms.Enabled = false;
            gbGeneralGeneral.Enabled = false;
            gbGeneralFpga.Enabled = false;

            // Configure the FPGA
            ShowStatusMessage("Configuring FPGA...please wait");
            success = Module.UploadFpga(tbFpgaFile.Text, Program.QuickUsbDiag.UpdateFpgaProgressBar);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.UploadFpga failed with error: {0}", Module.LastError().ToString()));
            }

            // Re-enable form controls
            quickUsbWinForms.Enabled = true;
            gbGeneralGeneral.Enabled = true;
            gbGeneralFpga.Enabled = true;

            // Check to see if the FPGA is configured
            isFpgaConfiguredButton_Click(sender, e);
        }

        private void fpgaFileTextBox_TextChanged(object sender, EventArgs e)
        {
            // Only enable the upload button if a valid file is specified and a module is selected
            uploadFpgaButton.Enabled = (Module != null) && (File.Exists(tbFpgaFile.Text));
        }

        private void bCmdRead_Click(object sender, EventArgs e)
        {
            ushort address, length;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            cmdReadData.Text = "";

            // Parse the address field
            try
            {
                address = QuickUsbDiag.ParseNumberUInt16(tbCmdReadAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = QuickUsbDiag.ParseNumberUInt16(tbCmdReadBytes.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create data array to hold read values
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform command read: Not enough data.");
                return;
            }

            // Set whether the address bus should be used and auto-incremented
            if (!cbCmdUseAddr.Checked)
            {
                address |= (1 << 14);
            }
            if (!cbCmdIncAddr.Checked)
            {
                address |= (1 << 15);
            }

            // Perform a command read
            success = Module.ReadCommand(address, data, ref length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadCommand failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully read command bus");
            }

            // Format and display read data
            for (int k = 0; k < length; ++k)
            {
                cmdReadData.Text += "0x" + Convert.ToString(data[k], 16).PadLeft(2, '0').ToUpper() + " ";
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadCommand;
        }

        private void bCmdWrite_Click(object sender, EventArgs e)
        {
            ushort address, length;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Parse address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbCmdWriteAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse length field
            try
            {
                length = (ushort)QuickUsbDiag.ParseNumberUInt16(tbCmdWriteBytes.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create data array and fill with specified data
            data = new byte[length];
            textData = cmdWriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if (length == 0 || textData.Length < length)
            {
                ShowStatusMessage("Unable to perform command write: Not enough data.");
                return;
            }
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Set whether the address bus should be used and auto-incremented
            if (!cbCmdUseAddr.Checked)
            {
                address |= (1 << 14);
            }
            if (!cbCmdIncAddr.Checked)
            {
                address |= (1 << 15);
            }

            // Perform the command write
            success = Module.WriteCommand(address, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteCommand failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote command bus");
            }
            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteCommand;
        }

        private void cbCmdAddr_CheckedChanged(object sender, EventArgs e)
        {
            cbCmdIncAddr.Enabled = cbCmdUseAddr.Checked;
            tbCmdReadAddress.Enabled = cbCmdUseAddr.Checked;
            tbCmdWriteAddress.Enabled = cbCmdUseAddr.Checked;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check if we should skip reading the settings and updating the checkbox values
            if (skipSettingAccess)
            {
                return;
            }

            // Read current setting
            ushort value;
            bool success = Module.ReadSetting(QuickUsb.Setting.DataAddress, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
            }

            // Update setting
            if (cbCmdUseAddr.Checked)
            {
                value &= 0xBFFF; // ~0x4000
            }
            else
            {
                value |= 0x4000;
            }
            if (cbCmdIncAddr.Checked)
            {
                value &= 0x7FFF; // ~0x8000
            }
            else
            {
                value |= 0x8000;
            }

            // Write new setting
            success = Module.WriteSetting(QuickUsb.Setting.DataAddress, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote Setting.DataAddress");
            }
        }

        private void cbCmdWordWide_CheckedChanged(object sender, EventArgs e)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check if we should skip reading the settings and updating the checkbox values
            if (skipSettingAccess)
            {
                return;
            }

            // Read current setting
            ushort value;
            bool success = Module.ReadSetting(QuickUsb.Setting.WordWide, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update setting
            if (cbCmdWordWide.Checked)
            {
                value |= 0x0001;
            }
            else
            {
                value &= 0xFFFE; // ~0x0001
            }

            // Write new setting
            success = Module.WriteSetting(QuickUsb.Setting.WordWide, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote Setting.WordWide");
            }
        }

        private void btSetting_ValueChanged(object sender, uint value)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Write new value to setting
            bool success = Module.WriteSetting((QuickUsb.Setting)((BitTwiddler)sender).TagInt, (ushort)((BitTwiddler)sender).Value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Wait 10 ms
            Thread.Sleep(10);

            // Read back setting
            ushort setting;
            success = Module.ReadSetting((QuickUsb.Setting)((BitTwiddler)sender).TagInt, out setting);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));

                // Don't return as we will need to read back the default to ensure the BitTwiddler checkboxes are set correctly
            }

            // Set BitTwiddler to the read value
            ((BitTwiddler)sender).Value = setting;
        }

        private void btDefault_ValueChanged(object sender, uint value)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Write new value to setting
            bool success = Module.WriteDefault((QuickUsb.Setting)((BitTwiddler)sender).TagInt, (ushort)((BitTwiddler)sender).Value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteDefault failed with error: {0}", Module.LastError().ToString()));

                // Don't return as we will need to read back the default to ensure the BitTwiddler checkboxes are set correctly
            }

            // Wait 10 ms
            Thread.Sleep(10);

            // Read back default
            ushort setting;
            success = Module.ReadDefault((QuickUsb.Setting)((BitTwiddler)sender).TagInt, out setting);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadDefault failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Set BitTwiddler to the read value
            ((BitTwiddler)sender).Value = setting;
        }

        private void portDir_ValueChanged(object sender, uint value)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Write new value to port
            bool success = Module.WritePortDir((QuickUsb.Port)((BitTwiddler)sender).TagInt, (byte)((BitTwiddler)sender).Value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WritePortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote port direction");
            }
        }

        private void portVal_ValueChanged(object sender, uint value)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Write new value to port
            bool success = Module.WritePort((QuickUsb.Port)((BitTwiddler)sender).TagInt, (byte)((BitTwiddler)sender).Value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WritePort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote port");
            }
        }

        private void cbRS232Baud_SelectedIndexChanged(object sender, EventArgs e)
        {
            uint baud;

            // Ensure that a module is selected
            if (Module == null)
            {
                return;
            }

            // Parse the baud field
            try
            {
                baud = (uint)QuickUsbDiag.ParseNumberInt32(cbRS232Baud.Text);
            }
            catch (FormatException)
            {
                return;
            }

            bool success = Module.SetRS232BaudRate(baud);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.SetRS232BaudRate failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully set baud rate");
            }
        }

        private void bRS232Read_Click(object sender, EventArgs e)
        {
            ushort length;
            byte port;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            tbRS232ReadData.Text = "";

            // Get the RS232 port number
            port = (byte)cbRS232Port.SelectedIndex;

            // Get the number of bytes in the receive buffer
            success = Module.GetNumRS232(port, out length);
            if (!success)
            {
                ShowStatusMessage(String.Format("Unable to get number of RS232 bytes in read buffer: {0}", Module.LastError().ToString()));
                return;
            }

            // Create the data buffer
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform RS232 read: Not enough data.");
                return;
            }

            // Perform the RS232 read
            success = Module.ReadRS232(port, data, ref length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadRS232 failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully read RS232 port");
            }

            // Format and display the read data
            for (int k = 0; k < length; ++k)
            {
                tbRS232ReadData.Text += "0x" + Convert.ToString(data[k], 16).PadLeft(2, '0').ToUpper() + " ";
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadRS232;
        }

        private void bRS232Write_Click(object sender, EventArgs e)
        {
            ushort length;
            byte port;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Get the RS232 port number
            port = (byte)cbRS232Port.SelectedIndex;

            // Retrieve the data to write from the textbox
            textData = rs232WriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            length = (ushort)textData.Length;
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform RS232 write: Not enough data.");
                return;
            }

            // Parse the data
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Perform the data write
            success = Module.WriteRS232(port, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteRS232 failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote RS232 port");
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteRS232;
        }

        private void bRS232Flush_Click(object sender, EventArgs e)
        {
            byte port;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Get the RS232 port number
            port = (byte)cbRS232Port.SelectedIndex;

            // Perform the buffer flush
            success = Module.FlushRS232(port);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.FlushRS232 failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully flushed RS232 port");
            }
        }

        private void bRS232Get_Click(object sender, EventArgs e)
        {
            byte port;
            ushort length;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Get the RS232 port number
            port = (byte)cbRS232Port.SelectedIndex;

            // Retrieve the receive buffer count
            success = Module.GetNumRS232(port, out length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.GetNumRS232 failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully read RS232 buffer size");
            }

            // Display the number of characters in the receive buffer
            tbRS232Chars.Text = length.ToString();

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.GetRS232Num;
        }

        private void cbSpiEndian_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the endianness bit
                if (cbSpiEndian.Checked)
                {
                    value |= 0x01;
                }
                else
                {
                    value &= 0xFFFE;
                }

                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void cbSpiCPOL_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the clock polarity bit
                if (cbSpiCPOL.Checked)
                {
                    value |= 0x02;
                }
                else
                {
                    value &= 0xFFFD;
                }
                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void cbSpiCPHA_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the clock phase bit
                if (cbSpiCPHA.Checked)
                {
                    value |= 0x04;
                }
                else
                {
                    value &= 0xFFFB;
                }
                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void cbSpiPORT_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the SPI Port bit
                if (cbSpiPORT.Checked)
                {
                    value |= 0x08;
                }
                else
                {
                    value &= 0xFFF7;
                }
                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void cbSpiNCEPIN_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the NCE Pin bit
                if (cbSpiNCEPIN.Checked)
                {
                    value |= 0x10;
                }
                else
                {
                    value &= 0xFFEF;
                }
                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void cbSpiMISOPIN_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the current SPI setting
            if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
            {
                // Change the MISO Pin bit
                if (cbSpiMISOPIN.Checked)
                {
                    value |= 0x20;
                }
                else
                {
                    value &= 0xFFDF;
                }
                // Write the new SPI setting
                bool success = Module.WriteSetting(QuickUsb.Setting.SpiConfig, value);
                if (!success)
                {
                    ShowStatusMessage(String.Format("Module.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
                else
                {
                    ShowStatusMessage("Succesfully wrote Setting.SpiConfig");
                }
            }
        }

        private void bSpiRead_Click(object sender, EventArgs e)
        {
            ushort length;
            byte port;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            tbSpiReadData.Text = "";

            // Get the SPI port number
            port = (byte)cbSpiReadPort.SelectedIndex;

            // Parse the length field
            try
            {
                length = (ushort)QuickUsbDiag.ParseNumberUInt16(tbSpiBytes.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create the data array
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform SPI read: Not enough data.");
                return;
            }

            // Perform the SPI read
            success = Module.ReadSPI(port, data, ref length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSPI failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully read SPI port");
            }

            // Format and display the read data
            tbSpiReadData.Text = "";
            for (int k = 0; k < length; ++k)
            {
                tbSpiReadData.Text += "0x" + Convert.ToString(data[k], 16).PadLeft(2, '0').ToUpper() + " ";
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadSpi;
        }

        private void bSpiWrite_Click(object sender, EventArgs e)
        {
            ushort length;
            byte port;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Get the SPI port number
            port = (byte)cbSpiReadPort.SelectedIndex;

            // Get the data to write
            textData = tbSpiWriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            length = (ushort)textData.Length;
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform SPI write: Not enough data.");
                return;
            }

            // Parse the data to write
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Perform the SPI write
            success = Module.WriteSPI(port, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSPI failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote SPI port");
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteSpi;
        }

        private void bWriteReadSpi_Click(object sender, EventArgs e)
        {
            ushort length;
            byte port;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Get the SPI port number
            port = (byte)cbSpiReadPort.SelectedIndex;

            // Get the data to write
            textData = tbSpiWriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            length = (ushort)textData.Length;
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform SPI write: Not enough data.");
                return;
            }

            // Parse the data to write
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Perform the SPI WriteRead
            success = Module.WriteReadSPI(port, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteReadSPI failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote/read SPI port");
            }

            // Format and display the read data
            tbSpiReadData.Text = "";
            for (int k = 0; k < length; ++k)
            {
                tbSpiReadData.Text += "0x" + Convert.ToString(data[k], 16).PadLeft(2, '0').ToUpper() + " ";
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteReadSpi;
        }

        private void bI2cRead_Click(object sender, EventArgs e)
        {
            ushort address, length;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            tbI2cReadData.Text = "";

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbI2cReadAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = (ushort)QuickUsbDiag.ParseNumberUInt16(tbI2cBytes.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create the data buffer
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform I2C read: Not enough data.");
                return;
            }

            // Perform the I2C read
            success = Module.ReadI2C(address, data, ref length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadI2C failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully read I2C port");
            }

            // Format and display the read data
            for (int k = 0; k < length; ++k)
            {
                tbI2cReadData.Text += "0x" + Convert.ToString(data[k], 16).PadLeft(2, '0').ToUpper() + " ";
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadI2C;
        }

        private void bI2cWrite_Click(object sender, EventArgs e)
        {
            ushort address, length;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbI2cWriteAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create the data buffer
            textData = tbI2cWriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            length = (ushort)textData.Length;
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform I2C write: Not enough data.");
                return;
            }

            // Parse the data to write
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Perform the data write
            success = Module.WriteI2C(address, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteI2C failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote I2C port");
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteI2C;
        }

        private void bI2cCachedWrite_Click(object sender, EventArgs e)
        {
            ushort address, length;
            string[] textData;
            byte[] data;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbI2cWriteAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create the data buffer
            textData = tbI2cWriteData.Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            length = (ushort)textData.Length;
            data = new byte[length];
            if (length == 0)
            {
                ShowStatusMessage("Unable to perform I2C write: Not enough data.");
                return;
            }

            // Parse the data to write
            for (int k = 0; k < length; ++k)
            {
                try
                {
                    data[k] = (byte)QuickUsbDiag.ParseNumberInt32(textData[k]);
                }
                catch (FormatException)
                {
                    MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // Perform the cached data write
            success = Module.CachedWriteI2C(address, data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.CachedWriteI2C failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully cached write to I2C port");
            }

            // We cannot repeat this action so explicitly don't allow it
            lastAction = LastAction.None;
        }

        private void cbI2cBusSpeed_SelectedIndexChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the I2C setting
            bool success = Module.ReadSetting(QuickUsb.Setting.I2Ctl, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Change the I2C bus speed bit
            if (cbI2cBusSpeed.SelectedIndex == 0)
            {
                value &= 0xFFFE;
            }
            else
            {
                value |= 0x01;
            }

            // Write the new setting
            success = Module.WriteSetting(QuickUsb.Setting.I2Ctl, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote Setting.I2Ctl");
            }
        }

        private void cbDataAddr_CheckedChanged(object sender, EventArgs e)
        {
            cbDataIncAddr.Enabled = cbDataUseAddr.Checked;
            tbDataReadAddress.Enabled = cbDataUseAddr.Checked;
            tbDataWriteAddress.Enabled = cbDataUseAddr.Checked;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Return if we should not read settings and update checkboxes
            if (skipSettingAccess)
            {
                return;
            }

            // Read current setting
            ushort value;
            bool success = Module.ReadSetting(QuickUsb.Setting.DataAddress, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update setting
            if (cbDataUseAddr.Checked)
            {
                value &= 0xBFFF; // ~0x4000
            }
            else
            {
                value |= 0x4000;
            }
            if (cbDataIncAddr.Checked)
            {
                value &= 0x7FFF; // ~0x8000
            }
            else
            {
                value |= 0x8000;
            }

            // Write new setting
            success = Module.WriteSetting(QuickUsb.Setting.DataAddress, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote Setting.DataAddress");
            }
        }

        private void cbDataWordwide_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Return if we should not read settings and update checkboxes
            if (skipSettingAccess)
            {
                return;
            }

            // Read current setting
            bool success = Module.ReadSetting(QuickUsb.Setting.WordWide, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update setting
            if (cbDataWordwide.Checked)
            {
                value |= 0x0001;
            }
            else
            {
                value &= 0xFFFE; // ~0x0001
            }

            // Write new setting
            success = Module.WriteSetting(QuickUsb.Setting.WordWide, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully wrote Setting.WordWide");
            }
        }

        private void bSetTimeout_Click(object sender, EventArgs e)
        {
            uint timeout;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Parse the timeout field
            try
            {
                timeout = (uint)QuickUsbDiag.ParseNumberInt32(tbTimeout.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter valid data", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Set the timeout
            bool success = Module.SetTimeout(timeout);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.SetTimeout failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Succesfully set timeout");
            }
        }

        private void bBrowseRead_Click(object sender, EventArgs e)
        {
            // Set the directory
            saveFileDialog.InitialDirectory = tbOutputFileName.Text;
            saveFileDialog.Filter = "Raw Binary Data Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbOutputFileName.Text = saveFileDialog.FileName;
                tbOutputFileName.SelectionStart = tbOutputFileName.Text.Length;
                tbOutputFileName.ScrollToCaret();
            }
        }

        private void bBrowseWrite_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbInputFileName.Text;
            openFileDialog.Filter = "Raw Binary Data Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbInputFileName.Text = openFileDialog.FileName;
                tbInputFileName.SelectionStart = tbInputFileName.Text.Length;
                tbInputFileName.ScrollToCaret();
            }
        }

        private void bDataRead_Click(object sender, EventArgs e)
        {
            ushort address, oldAddress;
            uint length;
            bool success;
            byte[] data;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Ensure that a valid file has been specified
            if (tbOutputFileName.Text == "")
            {
                if (lastAction == LastAction.ReadData)
                {
                    lastAction = LastAction.None;
                }
                MessageBox.Show("Please enter a valid output file path", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbDataReadAddress.Text);
            }
            catch (FormatException)
            {
                if (lastAction == LastAction.ReadData)
                {
                    lastAction = LastAction.None;
                }
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = (uint)QuickUsbDiag.ParseNumberInt32(tbDataReadBytes.Text);
            }
            catch (FormatException)
            {
                if (lastAction == LastAction.ReadData)
                {
                    lastAction = LastAction.None;
                }
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
            // Read old address setting
            success = Module.ReadSetting(QuickUsb.Setting.DataAddress, out oldAddress);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Write address
            if (!cbDataUseAddr.Checked)
            {
                address |= 0x4000;
            }
            if (!cbDataIncAddr.Checked)
            {
                address |= 0x8000;
            }
            address |= (ushort)(oldAddress & 0x3E00); // Don't overwrite any of the reserved bits if they are set
            success = Module.WriteSetting(QuickUsb.Setting.DataAddress, address);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Create data array
            data = new byte[length];

            // Clear the read statistics
            success = Module.ResetStatistic(QuickUsb.Stat.ReadThroughput);
            if (!success) {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Perform data transfer
            if (!cbDataOutOfOrder.Checked)
            {
                success = Module.ReadData(data, ref length);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ReadData failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
            }
            else
            {
                success = Module.ReadDataEx(data, ref length, QuickUsb.DataFlags.OutOfOrder);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ReadData failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
            }

            // Read the throughput statistic
            float datarate;
            success = Module.GetStatistic(QuickUsb.Stat.ReadThroughput, QuickUsb.StatUnit.BytesPerSec, out datarate, 0);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.GetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Try to write data to file
            int tries = 3;
            do
            {
                success = true;
                try
                {
                    File.WriteAllBytes(tbOutputFileName.Text, data);
                }
                catch
                {
                    success = false;
                    --tries;
                    Thread.Sleep(50);
                }
            } while (!success && (tries > 0));

            if (success)
            {
                ShowStatusMessage(String.Format("Successfully read {0} bytes ({1}/s)", length, GetFriendlyDataSize(datarate)));
            }
            else
            {
                ShowStatusMessage("Unable to write file");
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadData;
        }

        private void bDataWrite_Click(object sender, EventArgs e)
        {
            ushort address, oldAddress;
            uint length;
            bool success;
            byte[] data;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Ensure a valid file has been specified
            if (!File.Exists(tbInputFileName.Text))
            {
                if (lastAction == LastAction.WriteData)
                {
                    lastAction = LastAction.None;
                }
                MessageBox.Show("Please enter a valid input file path", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbDataWriteAddress.Text);
            }
            catch (FormatException)
            {
                if (lastAction == LastAction.WriteData)
                {
                    lastAction = LastAction.None;
                }
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // See if we should write the entire file, or just part of it
            if (cbUseFileLength.Checked)
            {
                // Read all the data from the file
                try
                {
                    data = File.ReadAllBytes(tbInputFileName.Text);
                    length = (uint)data.Length;
                }
                catch
                {
                    ShowStatusMessage("Unable to read file");
                    return;
                }
            }
            else
            {
                // Parse the length field
                try
                {
                    length = (uint)QuickUsbDiag.ParseNumberInt32(tbDataWriteBytes.Text);
                }
                catch (FormatException)
                {
                    if (lastAction == LastAction.WriteData)
                    {
                        lastAction = LastAction.None;
                    }
                    MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                // Create data buffer
                data = new byte[length];
                try
                {
                    FileStream fs = File.OpenRead(tbInputFileName.Text);
                    try
                    {
                        length = (uint)fs.Read(data, 0, Convert.ToInt32(length));
                    }
                    catch
                    {
                        ShowStatusMessage("Unable to open file");
                    }
                    finally
                    {
                        fs.Close();
                    }
                }
                catch
                {
                    ShowStatusMessage("Unable to open file");
                }
            }

            // Read old address setting
            success = Module.ReadSetting(QuickUsb.Setting.DataAddress, out oldAddress);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Write address
            if (!cbDataUseAddr.Checked)
            {
                address |= 0x4000;
            }
            if (!cbDataIncAddr.Checked)
            {
                address |= 0x8000;
            }
            address |= (ushort)(oldAddress & 0x3E00); // Don't overwrite any of the reserved bits if they are set
            success = Module.WriteSetting(QuickUsb.Setting.DataAddress, address);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Clear the read statistics
            success = Module.ResetStatistic(QuickUsb.Stat.WriteThroughput);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Perform and time the data transfer
            if (!cbDataOutOfOrder.Checked)
            {
                success = Module.WriteData(data, length);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.WriteData failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
            }
            else
            {
                success = Module.WriteDataEx(data, ref length, QuickUsb.DataFlags.OutOfOrder);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.WriteData failed with error: {0}", Module.LastError().ToString()));
                    return;
                }
            }

            // Read the throughput statistic
            float datarate;
            success = Module.GetStatistic(QuickUsb.Stat.WriteThroughput, QuickUsb.StatUnit.BytesPerSec, out datarate, 0);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.GetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Report throughput
            ShowStatusMessage(String.Format("Successfully wrote {0} bytes ({1}/s)", length, GetFriendlyDataSize(datarate)));

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.WriteData;
        }

        private void bDataReadEdit_Click(object sender, EventArgs e)
        {
            string path = WinHexPath;

            if (path == null)
            {
                // Center message box on parent window and display message
                MessageBox.Show("Unable to locate 'winhex.exe'", "Error");
                return;
            }

            Process.Start(path, "\"" + tbOutputFileName.Text + "\"");
        }

        private void bDataCount_Click(object sender, EventArgs e)
        {
            ushort address;
            uint length;
            bool success;
            byte[] data;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Parse the address field
            try
            {
                address = (ushort)QuickUsbDiag.ParseNumberUInt16(tbDataWriteAddress.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the address", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = (uint)QuickUsbDiag.ParseNumberInt32(tbDataWriteBytes.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Create data buffer
            data = new byte[length];
            if (cbDataWordwide.Checked)
            {
                for (int k = 0; k < (data.Length / 2); ++k)
                {
                    data[k * 2] = (byte)(k & 0xff);
                    data[(k * 2) + 1] = (byte)((k >> 8) & 0xff);
                }
            }
            else
            {
                for (int k = 0; k < data.Length; ++k)
                {
                    data[k] = (byte)k;
                }
            }

            // Write address
            if (!cbDataUseAddr.Checked)
            {
                address |= 0x4000;
            }
            if (!cbDataIncAddr.Checked)
            {
                address |= 0x8000;
            }
            success = Module.WriteSetting(QuickUsb.Setting.DataAddress, address);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Clear the read statistics
            success = Module.ResetStatistic(QuickUsb.Stat.WriteThroughput);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Perform and time the data transfer
            success = Module.WriteData(data, length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteData failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Read the throughput statistic
            float datarate;
            success = Module.GetStatistic(QuickUsb.Stat.WriteThroughput, QuickUsb.StatUnit.BytesPerSec, out datarate, 0);
            if (!success) 
            {
                ShowStatusMessage(String.Format("QuickUsb.GetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Report throughput
            ShowStatusMessage(String.Format("Successfully wrote counting pattern of {0} bytes ({1}/s)", length, GetFriendlyDataSize(datarate)));
        }

        private void bDataWriteEdit_Click(object sender, EventArgs e)
        {
            string path = WinHexPath;

            if (path == null)
            {
                // Center message box on parent window and display message
                MessageBox.Show("Unable to locate 'winhex.exe'", "Error");
                return;
            }

            Process.Start(path, "\"" + tbInputFileName.Text + "\"");
        }

        private void cbI2cIgnoreAck_CheckedChanged(object sender, EventArgs e)
        {
            ushort value;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read current setting
            bool success = Module.ReadSetting(QuickUsb.Setting.I2Ctl, out value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update setting
            if (cbI2cIgnoreAck.Checked)
            {
                value |= 0x0080;
            }
            else
            {
                value &= 0xFF7F; // ~0x0080
            }

            // Write new setting
            success = Module.WriteSetting(QuickUsb.Setting.I2Ctl, value);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteSetting failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Successfully wrote Setting.I2Ctl");
            }
        }

        private void bReadPortA_Click(object sender, EventArgs e)
        {
            byte bvalue;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the port value
            bool success = Module.ReadPort(QuickUsb.Port.A, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortAVal.Value = bvalue;
            }

            // Read the port direction
            success = Module.ReadPortDir(QuickUsb.Port.A, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortADir.Value = bvalue;
            }

            ShowStatusMessage("Successfully read Port A");

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadPortA;
        }

        private void bReadPortB_Click(object sender, EventArgs e)
        {
            byte bvalue;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the port value
            bool success = Module.ReadPort(QuickUsb.Port.B, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortBVal.Value = bvalue;
            }

            // Read the port direction
            success = Module.ReadPortDir(QuickUsb.Port.B, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortBDir.Value = bvalue;
            }

            ShowStatusMessage("Successfully read Port B");

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadPortB;
        }

        private void bReadPortC_Click(object sender, EventArgs e)
        {
            byte bvalue;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the port value
            bool success = Module.ReadPort(QuickUsb.Port.C, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortCVal.Value = bvalue;
            }

            // Read the port direction
            success = Module.ReadPortDir(QuickUsb.Port.C, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortCDir.Value = bvalue;
            }

            ShowStatusMessage("Successfully read Port C");

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadPortC;
        }

        private void bReadPortD_Click(object sender, EventArgs e)
        {
            byte bvalue;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the port value
            bool success = Module.ReadPort(QuickUsb.Port.D, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortDVal.Value = bvalue;
            }

            // Read the port direction
            success = Module.ReadPortDir(QuickUsb.Port.D, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortDDir.Value = bvalue;
            }

            ShowStatusMessage("Successfully read Port D");

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadPortD;
        }

        private void bReadPortE_Click(object sender, EventArgs e)
        {
            byte bvalue;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Read the port value
            bool success = Module.ReadPort(QuickUsb.Port.E, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortEVal.Value = bvalue;
            }

            // Read the port direction
            success = Module.ReadPortDir(QuickUsb.Port.E, out bvalue);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadPortDir failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                btPortEDir.Value = bvalue;
            }

            ShowStatusMessage("Successfully read Port E");

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.ReadPortE;
        }

        private void bCountPortA_Click(object sender, EventArgs e)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Generate a counting sequence that starts and ends with zero
            byte[] data = new byte[64];
            for (int k = 0; k < (data.Length - 1); ++k)
            {
                data[k] = (byte)k;
            }
            data[data.Length - 1] = 0;

            // Write the count to Port A
            bool success = Module.WritePort(QuickUsb.Port.A, data, (ushort)data.Length);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WritePort failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Successfully wrote count pattern to Port A");
            }

            // Keep track of this successful action to allow for automatic repeating
            lastAction = LastAction.CountPortA;
        }

        private void bEpcsIdentify_Click(object sender, EventArgs e)
        {
            bool success;
            byte nSS;
            QuickUsb.EpcsId epcsId;
            uint byteSize;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            nSS = (byte)cbEpcsPort.SelectedIndex;

            success = Module.IdentifyEpcs(nSS, out epcsId, out byteSize, QuickUsb.EpcsFlags.None);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.IdentifyEpcs failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            else
            {
                ShowStatusMessage("Successfully identified EPCS device");
            }

            switch (epcsId)
            {
                case QuickUsb.EpcsId.Epcs1:
                    labelEpcsIdentify.Text = "EPCS1 Device found (" + (byteSize / 1024) + " KB)";
                    break;
                case QuickUsb.EpcsId.Epcs16:
                    labelEpcsIdentify.Text = "EPCS16 Device found (" + (byteSize / 1024) + " KB)";
                    break;
                case QuickUsb.EpcsId.Epcs4:
                    labelEpcsIdentify.Text = "EPCS4 Device found (" + (byteSize / 1024) + " KB)";
                    break;
                case QuickUsb.EpcsId.Epcs64:
                    labelEpcsIdentify.Text = "EPCS64 Device found (" + (byteSize / 1024) + " KB)";
                    break;
                case QuickUsb.EpcsId.EpcsUnknown:
                    labelEpcsIdentify.Text = "EPCS device not found";
                    break;
                default:
                    labelEpcsIdentify.Text = "Unknown EPCS device";
                    break;
            }
        }

        private void bEpcsBrowse_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbInputFileName.Text;
            openFileDialog.Filter = "Altera RBF Files (*.rbf)|*.rbf|Raw Binary Data Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbEpcsFilePath.Text = openFileDialog.FileName;
                tbEpcsFilePath.SelectionStart = tbEpcsFilePath.Text.Length;
                tbEpcsFilePath.ScrollToCaret();
            }
        }

        private void ProgressCallback(IntPtr hDevice, ushort percentComplete, IntPtr tag)
        {
            if (tag == IntPtr.Zero)
            {
                UpdateFpgaProgressBar(percentComplete);
            }

            Application.DoEvents();
        }

        private void bEpcsWrite_Click(object sender, EventArgs e)
        {
            bool success;
            byte nSS;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            nSS = (byte)cbEpcsPort.SelectedIndex;

            // Disable form controls to prevent breaking stuff
            quickUsbWinForms.Enabled = false;
            gbEpcsGeneral.Enabled = false;
            gbEpcsWriting.Enabled = false;

            // We will use a callback to update the progress bar in the status bar
            QuickUsb.ProgressDelegate cbDelegate = new QuickUsb.ProgressDelegate(ProgressCallback);

            // Perform the configuration
            success = Module.ConfigureEpcs(nSS, tbEpcsFilePath.Text, QuickUsb.EpcsFlags.None, cbDelegate, IntPtr.Zero);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ConfigureEpcs failed with error: {0}", Module.LastError().ToString()));
            }
            else
            {
                ShowStatusMessage("Successfully configured EPCS device");
            }

            // Re-enable form controls
            quickUsbWinForms.Enabled = true;
            gbEpcsGeneral.Enabled = true;
            gbEpcsWriting.Enabled = true;
        }

        private void bEpcsVerify_Click(object sender, EventArgs e)
        {
            bool success;
            byte nSS;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            nSS = (byte)cbEpcsPort.SelectedIndex;

            // Disable form controls to prevent breaking stuff
            quickUsbWinForms.Enabled = false;
            gbEpcsGeneral.Enabled = false;
            gbEpcsWriting.Enabled = false;

            // We will use a callback to update the progress bar in the status bar
            QuickUsb.ProgressDelegate cbDelegate = new QuickUsb.ProgressDelegate(ProgressCallback);

            // Perform the configuration
            success = Module.VerifyEpcs(nSS, tbEpcsFilePath.Text, QuickUsb.EpcsFlags.None, cbDelegate, IntPtr.Zero);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.VerifyEpcs failed with error: {0}", Module.LastError().ToString()));
            }
            else
            {
                ShowStatusMessage("EPCS verify ok");
            }

            // Re-enable form controls
            quickUsbWinForms.Enabled = true;
            gbEpcsGeneral.Enabled = true;
            gbEpcsWriting.Enabled = true;
        }

        private void bEpcsErase_Click(object sender, EventArgs e)
        {
            bool success;
            byte nSS;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            nSS = (byte)cbEpcsPort.SelectedIndex;

            // Disable form controls to prevent breaking stuff
            quickUsbWinForms.Enabled = false;
            gbEpcsGeneral.Enabled = false;
            gbEpcsWriting.Enabled = false;

            pbProgress.Style = ProgressBarStyle.Marquee;
            pbProgress.Visible = true;
            pbProgress.MarqueeAnimationSpeed = 10;

            // We will use a callback to update the progress bar in the status bar
            QuickUsb.ProgressDelegate cbDelegate = new QuickUsb.ProgressDelegate(ProgressCallback);

            // Perform the configuration
            success = Module.EraseEpcs(nSS, QuickUsb.EpcsFlags.None, cbDelegate, (IntPtr)1);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.EraseEpcs failed with error: {0}", Module.LastError().ToString()));
            }
            else
            {
                ShowStatusMessage("EPCS erased successfully");
            }

            pbProgress.Style = ProgressBarStyle.Continuous;
            pbProgress.Value = 100;

            // Re-enable form controls
            quickUsbWinForms.Enabled = true;
            gbEpcsGeneral.Enabled = true;
            gbEpcsWriting.Enabled = true;
        }

        private void cbRepeat_CheckedChanged(object sender, EventArgs e)
        {
            if (cbRepeat.Checked)
            {
                if (lastAction == LastAction.None)
                {
                    ShowStatusMessage("No prior action to repeat");
                    cbRepeat.Checked = false;
                    return;
                }

                ShowStatusMessage("Periodically repeating last action: " + lastAction.ToString());

                timerRepeat.Interval = (int)nRepeatInterval.Value;
                timerRepeat.Enabled = true;
            }
            else
            {
                timerRepeat.Enabled = false;
            }
        }

        private void timerRepeat_Tick(object sender, EventArgs e)
        {
            switch (lastAction)
            {
                default:
                case LastAction.None:
                    cbRepeat.Checked = false;
                    timerRepeat.Enabled = false;
                    break;

                case LastAction.ReadCommand:
                    bCmdRead_Click(sender, e);
                    break;

                case LastAction.WriteCommand:
                    bCmdWrite_Click(sender, e);
                    break;

                case LastAction.ReadData:
                    bDataRead_Click(sender, e);
                    break;

                case LastAction.WriteData:
                    bDataWrite_Click(sender, e);
                    break;

                case LastAction.WriteDataCount:
                    bDataCount_Click(sender, e);
                    break;

                case LastAction.CountPortA:
                    bCountPortA_Click(sender, e);
                    break;

                case LastAction.ReadPortA:
                    bReadPortA_Click(sender, e);
                    break;

                case LastAction.ReadPortB:
                    bReadPortB_Click(sender, e);
                    break;

                case LastAction.ReadPortC:
                    bReadPortC_Click(sender, e);
                    break;

                case LastAction.ReadPortD:
                    bReadPortD_Click(sender, e);
                    break;

                case LastAction.ReadPortE:
                    bReadPortE_Click(sender, e);
                    break;

                case LastAction.ReadRS232:
                    bRS232Read_Click(sender, e);
                    break;

                case LastAction.WriteRS232:
                    bRS232Write_Click(sender, e);
                    break;
                case LastAction.GetRS232Num:
                    bRS232Get_Click(sender, e);
                    break;

                case LastAction.ReadSpi:
                    bSpiRead_Click(sender, e);
                    break;

                case LastAction.WriteSpi:
                    bSpiWrite_Click(sender, e);
                    break;

                case LastAction.WriteReadSpi:
                    bWriteReadSpi_Click(sender, e);
                    break;

                case LastAction.ReadI2C:
                    bI2cRead_Click(sender, e);
                    break;

                case LastAction.WriteI2C:
                    bI2cWrite_Click(sender, e);
                    break;
            }
        }

		#endregion

		#region Private Methods

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

        private void SetToolTips()
        {
            string tooltipStr;

            // Read the tooltips from the embedded resource file
            using (Stream fs = Assembly.GetExecutingAssembly().GetManifestResourceStream("QuickUsbDiagCs.Embedded_Resources.tooltips.txt"))
            {
                byte[] tooltipData = new byte[fs.Length];
                fs.Read(tooltipData, 0, Convert.ToInt32(fs.Length));
                tooltipStr = System.Text.Encoding.ASCII.GetString(tooltipData);
            }

            // Allocate the tooltip string arrays
            string[][] tooltips = new string[18][]; // 18 settings
            for (int k = 0; k < tooltips.Length; ++k)
            {
                tooltips[k] = new string[16]; // 16-bits wide
            }

            // Loop through each line of the tooltips file
            int settingIndex = 0, bitIndex = 0, index;
            bool high = true;
            string[] tokens;
            string tip;
            foreach (string line in Regex.Split(tooltipStr, Environment.NewLine))
            {
                // Ignore blank lines
                if (line == "")
                {
                    continue;
                }

                // Check if we are on a new setting
                if (!line.StartsWith("\t"))
                {
                    // The first token is the setting index (the second is the setting name)
                    tokens = line.Split(' ');
                    settingIndex = Convert.ToInt32(tokens[0]);
                    bitIndex = 0;
                    high = line.Trim().EndsWith("H");
                }
                else
                {
                    // Allow endlines in the tooltips
                    tip = line.Trim().Replace("\\n", Environment.NewLine);

                    // Save the tooltip
                    if (high)
                    {
                        // Alter the bit number since the high byte is couting from 0
                        tokens = tip.Split(' ');
                        index = Convert.ToInt32(tokens[1].Substring(0, tokens[1].Length - 1)) + 8;
                        tokens[1] = index.ToString() + ':';
                        tip = String.Join(" ", tokens);

                        tooltips[settingIndex][8 + bitIndex++] = tip;
                    }
                    else
                    {
                        tooltips[settingIndex][bitIndex++] = tip;
                    }
                }
            }

            // Set the tooltips
            foreach (Control ctrl in gbSettings.Controls)
            {
                if (ctrl is BitTwiddler)
                {
                    ((BitTwiddler)ctrl).ToolTips = tooltips[((BitTwiddler)ctrl).TagInt];
                }
            }
            foreach (Control ctrl in gbDefaults.Controls)
            {
                if (ctrl is BitTwiddler)
                {
                    ((BitTwiddler)ctrl).ToolTips = tooltips[((BitTwiddler)ctrl).TagInt];
                }
            }
        }

        // Enable/Disable the controls on the indicated tab
        private void UpdateTabCtrlEnable(TabPage page)
        {
            bool ena = Module != null;

            switch (page.Text)
            {
                case "General":
                    gbGeneralGeneral.Enabled = ena;
                    gbGeneralFpga.Enabled = ena;
                    uploadFpgaButton.Enabled = ena && (File.Exists(tbFpgaFile.Text));
                    break;

                case "Command":
                    gbCommandGeneral.Enabled = ena;
                    gbCommandRead.Enabled = ena;
                    gbCommandWrite.Enabled = ena;
                    break;

                case "Data":
                    gbDataGeneral.Enabled = ena;
                    gbDataRead.Enabled = ena;
                    gbDataWrite.Enabled = ena;
                    break;

                case "Streaming":
                    gbStreamingGeneral.Enabled = ena;
                    gbStreamingRead.Enabled = ena;
                    gbStreamingWrite.Enabled = ena;
                    gbStreamingStatistics.Enabled = ena;

                    rbStreamRead_CheckedChanged(null, null);
                    rbStreamWrite_CheckedChanged(null, null);
                    break;

                case "Ports":
                    gbPortA.Enabled = ena;
                    gbPortB.Enabled = ena;
                    gbPortC.Enabled = ena;
                    gbPortD.Enabled = ena;
                    gbPortE.Enabled = ena;
                    break;

                case "RS232":
                    gbRS232General.Enabled = ena;
                    gbRS232Read.Enabled = ena;
                    gbRS232Write.Enabled = ena;
                    break;

                case "SPI":
                    gbSPIGeneral.Enabled = ena;
                    gbSPIRead.Enabled = ena;
                    gbSPIWrite.Enabled = ena;
                    break;

                case "I2C":
                    gbI2CGeneral.Enabled = ena;
                    gbI2CRead.Enabled = ena;
                    gbI2CWrite.Enabled = ena;
                    break;

                case "Settings":
                    gbSettings.Enabled = ena;
                    break;

                case "Defaults":
                    gbDefaults.Enabled = ena;
                    break;
            }
        }

        // Populate the controls on the indicated tab with proper values (typically read from the QuickUSB Module)
        private void UpdateTabCtrl(TabPage page)
        {
            ushort value;
            byte bvalue;

            if (Module == null)
            {
                return;
            }

            switch (page.Text)
            {
                case "General":
                    // Get the QuickUSB Model
                    tbDesciption.Text = Module.Model;
                    
                    // Get the QuickUSB Serial
                    tbSerialNumber.Text = Module.Serial;

                    // Set the FPGA combobox based on the FPGA setting
                    if (Module.ReadSetting(QuickUsb.Setting.FpgaType, out value))
                    {
                        alteraPsRadioButton.Checked = (value & 0x0001) == 0;
                        xilinxSsRadioButton.Checked = (value & 0x0001) != 0;
                    }

                    // Get the QuickUSB VesionSpeed Info
                    if (Module.ReadSetting(QuickUsb.Setting.VersionSpeed, out value))
                    {
                        byte version = (byte)((value / 0xff) & 0xff);
                        byte speed = (byte)(value & 0xff);

                        // Decode the VersionSpeed info into a meaningful string
                        if (version == 0x00)
                        {
                            tbHWVersion.Text = "CY7C68013 Rev A/B (EZ-USB FX2)";
                            rbSuperSpeed.Enabled = false;
                        }
                        else if (version == 0x01)
                        {
                            tbHWVersion.Text = "CY7C68013A Rev A (EZ-USB FX2LP)";
                            rbSuperSpeed.Enabled = false;
                        }
                        else if (version == 0x02)
                        {
                            tbHWVersion.Text = "CY7C68013 Rev C/D (EZ-USB FX2)";
                            rbSuperSpeed.Enabled = false;
                        }
                        else if (version == 0x04)
                        {
                            tbHWVersion.Text = "CY7C68013 Rev E (EZ-USB FX2)";
                            rbSuperSpeed.Enabled = false;
                        }
                        else if (version == 0x10)
                        {
                            tbHWVersion.Text = "CYUSB3014 Rev ?? (EZ-USB FX3)";
                            rbSuperSpeed.Enabled = true;
                        }
                        else
                        {
                            tbHWVersion.Text = "Unknown";
                            rbSuperSpeed.Enabled = true;
                        }

                        if (speed == 0x00)
                        {
                            lLabelDeviceSpeed.Text = "Full-Speed (12 Mbps)";
                        }
                        else if (speed == 0x80)
                        {
                            lLabelDeviceSpeed.Text = "High-Speed (480 Mbps)";
                        }
                        else if (speed == 0xC0)
                        {
                            lLabelDeviceSpeed.Text = "Super-Speed (5 Gbps)";
                        }
                        else
                        {
                            lLabelDeviceSpeed.Text = "Unknown";
                        }

                        // Read the requested device speed setting
                        if (Module.ReadSetting(QuickUsb.Setting.CpuConfig, out value))
                        {
                            disableUsbSpeedCheckChanged = true;

                            speed = (byte)((value / 0xff) & 0xff);

                            if (speed == 0x00)
                            {
                                rbFullSpeed.Checked = true;
                            }
                            else if (speed == 0x80)
                            {
                                rbHighSpeed.Checked = true;
                            }
                            else if (speed == 0xC0)
                            {
                                rbSuperSpeed.Checked = true;
                            }
                            else
                            {
                                // Unknown
                                rbFullSpeed.Checked = false;
                                rbHighSpeed.Checked = false;
                                rbSuperSpeed.Checked = false;
                            }

                            disableUsbSpeedCheckChanged = false;
                        }
                    }

                    break;

                case "Command":
                    skipSettingAccess = true;
                    // Read Data Address Setting
                    if (Module.ReadSetting(QuickUsb.Setting.DataAddress, out value))
                    {
                        // Update checkboxes
                        cbCmdUseAddr.Checked = ((value & 0x4000) == 0);
                        cbCmdIncAddr.Checked = ((value & 0x8000) == 0);
                    }
                    
                    // Read Word Wide Setting
                    if (Module.ReadSetting(QuickUsb.Setting.WordWide, out value))
                    {
                        // Update Checkbox
                        cbCmdWordWide.Checked = ((value & 0x0001) != 0);
                    }
                    skipSettingAccess = false;
                    break;

                case "Data":
                    skipSettingAccess = true;
                    // Read Data Address Setting
                    if (Module.ReadSetting(QuickUsb.Setting.DataAddress, out value))
                    {
                        // Update checkboxes
                        cbDataUseAddr.Checked = ((value & 0x4000) == 0);
                        cbDataIncAddr.Checked = ((value & 0x8000) == 0);
                    }

                    // Read Data Address Setting
                    if (Module.ReadSetting(QuickUsb.Setting.WordWide, out value))
                    {
                        // Update checkboxes
                        cbDataWordwide.Checked = ((value & 0x0001) != 0);
                    }

                    skipSettingAccess = false;
                    break;

                case "Streaming":
                    skipSettingAccess = true;
                    // Read Data Address Setting
                    if (Module.ReadSetting(QuickUsb.Setting.DataAddress, out value))
                    {
                        // Update checkboxes
                        cbStreamingUseAddr.Checked = ((value & 0x4000) == 0);
                        cbStreamingIncAddr.Checked = ((value & 0x8000) == 0);
                    }

                    // Read Data Address Setting
                    if (Module.ReadSetting(QuickUsb.Setting.WordWide, out value))
                    {
                        // Update checkboxes
                        cbStreamingWordwide.Checked = ((value & 0x0001) != 0);
                    }

                    skipSettingAccess = false;
                    break;

                case "Ports":
                    // Loop through all of the group boxes on the tab
                    foreach (Control groupBox in tabPorts.Controls)
                    {
                        if (groupBox is GroupBox)
                        {
                            // Loop over the BitTwiddler controls in the group box
                            foreach (Control ctrl in groupBox.Controls)
                            {
                                if (ctrl is BitTwiddler)
                                {
                                    // Determine if the BitTwiddler is the value or the direction
                                    if (((BitTwiddler)ctrl).Caption.StartsWith("Value"))
                                    {
                                        // Read the port value
                                        if (Module.ReadPort((QuickUsb.Port)((BitTwiddler)ctrl).TagInt, out bvalue))
                                        {
                                            ((BitTwiddler)ctrl).Value = bvalue;
                                        }
                                    }
                                    else if (((BitTwiddler)ctrl).Caption.StartsWith("Direction"))
                                    {
                                        // Read the port direction
                                        if (Module.ReadPortDir((QuickUsb.Port)((BitTwiddler)ctrl).TagInt, out bvalue))
                                        {
                                            ((BitTwiddler)ctrl).Value = bvalue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                case "RS232":
                    // Be nice and check the receive buffer
                    bRS232Get_Click(null, null);
                    break;

                case "SPI":
                    // Read SPI Settings
                    if (Module.ReadSetting(QuickUsb.Setting.SpiConfig, out value))
                    {
                        // Update checkboxes
                        cbSpiEndian.Checked = ((value & 0x0001) != 0);
                        cbSpiCPOL.Checked = ((value & 0x0002) != 0);
                        cbSpiCPHA.Checked = ((value & 0x0004) != 0);

                        cbSpiPORT.Checked = ((value & 0x0008) != 0);
                        cbSpiNCEPIN.Checked = ((value & 0x0010) != 0);
                        cbSpiMISOPIN.Checked = ((value & 0x0020) != 0);
                    }
                    break;

                case "I2C":
                    // Read I2C Settings
                    if (Module.ReadSetting(QuickUsb.Setting.I2Ctl, out value))
                    {
                        // Update checkboxes
                        cbI2cBusSpeed.SelectedIndex = (value & 0x0001);
                        cbI2cIgnoreAck.Checked = ((value & 0x0080) != 0);
                    }
                    break;

                case "Settings":
                    foreach (Control ctrl in gbSettings.Controls)
                    {
                        if (ctrl is BitTwiddler)
                        {
                            if (Module.ReadSetting((QuickUsb.Setting)((BitTwiddler)ctrl).TagInt, out value))
                            {
                                ((BitTwiddler)ctrl).Value = value;
                            }
                        }
                    }
                    break;

                case "Defaults":
                    foreach (Control ctrl in gbDefaults.Controls)
                    {
                        if (ctrl is BitTwiddler)
                        {
                            if (Module.ReadDefault((QuickUsb.Setting)((BitTwiddler)ctrl).TagInt, out value))
                            {
                                ((BitTwiddler)ctrl).Value = value;
                            }
                        }
                    }
                    break;
            }
        }

        #endregion

        #region Static Methods

        public static int ParseNumberInt32(string x)
		{
			int value, numbase = 10;
			if (x.ToLower().StartsWith("&h") || x.ToLower().StartsWith("0x"))
			{
				numbase = 16;
				x = x.Substring(2, x.Length - 2);
			}
			value = Convert.ToInt32(x, numbase);
			return value;
		}

        public static ushort ParseNumberUInt16(string x)
        {
            ushort value;
            int numbase = 10;

            if (x.ToLower().StartsWith("&h") || x.ToLower().StartsWith("0x"))
            {
                numbase = 16;
                x = x.Substring(2, x.Length - 2);
            }

            // Check if the string is a negative number
            if (x.StartsWith("-"))
            {
                return (ushort)Convert.ToInt16(x, 10);
            }

            try
            {
                // Try to convert the number
                value = Convert.ToUInt16(x, numbase);
            }
            catch
            {
                // If all else fails, try a hex convertion
                value = Convert.ToUInt16(x, 16);
            }

            return value;
        }

		#endregion

        private void tbStreamBufferSize_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (Char.IsDigit(e.KeyChar))
            {
                // Digits are OK
            }
            else if (e.KeyChar == '\b')
            {
                // Backspace key is OK
            }
            else if ((ModifierKeys & (Keys.Control | Keys.Alt)) != 0)
            {
                // Let the edit control handle control and alt key combinations
            }
            else
            {
                e.Handled = true;
            }

        }

        private void bReadStreamBrowse_Click(object sender, EventArgs e)
        {
            // Set the directory
            saveFileDialog.InitialDirectory = tbReadStreamFileName.Text;
            saveFileDialog.Filter = "Raw Binary Data Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbReadStreamFileName.Text = saveFileDialog.FileName;
                tbReadStreamFileName.SelectionStart = tbReadStreamFileName.Text.Length;
                tbReadStreamFileName.ScrollToCaret();
            }
        }

        private void bWriteStreamBrowse_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbWriteStreamFileName.Text;
            openFileDialog.Filter = "Raw Binary Data Files (*.bin)|*.bin|All Files (*.*)|*.*";

            // Display the FileOpen dialog
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Extract the selected filename
                tbWriteStreamFileName.Text = openFileDialog.FileName;
                tbWriteStreamFileName.SelectionStart = tbWriteStreamFileName.Text.Length;
                tbWriteStreamFileName.ScrollToCaret();
            }
        }

        private void bReadStreamEdit_Click(object sender, EventArgs e)
        {
            string path = WinHexPath;

            if (path == null)
            {
                // Center message box on parent window and display message
                MessageBox.Show("Unable to locate 'winhex.exe'", "Error");
                return;
            }

            Process.Start(path, "\"" + tbReadStreamFileName.Text + "\"");
        }

        private void bWriteStreamEdit_Click(object sender, EventArgs e)
        {
            string path = WinHexPath;

            if (path == null)
            {
                // Center message box on parent window and display message
                MessageBox.Show("Unable to locate 'winhex.exe'", "Error");
                return;
            }

            Process.Start(path, "\"" + tbWriteStreamFileName.Text + "\"");
        }

        private static void streamCompletionRoutineFunc(QuickUsb.BulkStream bulkStream)
        {
            if (bulkStream.Error != 0)
            {
                // Error occurred
            }
        }

        private void bReadStreamStart_Click(object sender, EventArgs e)
        {
            uint length;
            uint numBuffers;
            ushort numThreads;
            ushort threadConcurrency;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check if we are to start, pause, or resume
            if (bReadStreamStart.Text == "Pause")
            {
                // Pause the write stream
                success = Module.PauseStream(readStreamID, true);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.PauseStream failed with error: {0}", Module.LastError().ToString()));
                }
                else
                {
                    bReadStreamStart.Text = "Resume";
                }
                return;
            }
            else if (bReadStreamStart.Text == "Resume")
            {
                // When we resume we should reset the throughput statistic so that the pause time doesn't influence the reported data rate
                success = Module.ResetStatistic(QuickUsb.Stat.ReadThroughput);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                    return;
                }

                // Resume the read stream
                success = Module.ResumeStream(readStreamID);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ResumeStream failed with error: {0}", Module.LastError().ToString()));
                }
                else
                {
                    bReadStreamStart.Text = "Pause";
                }
                return;
            }

            // Ensure that a valid file has been specified
            if (rbReadStreamToDisk.Checked && tbReadStreamFileName.Text == "")
            {
                MessageBox.Show("Please enter a valid output file path", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = (uint)QuickUsbDiag.ParseNumberInt32(tbStreamBufferSize.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the number of buffers field
            try
            {
                numBuffers = (uint)QuickUsbDiag.ParseNumberInt32(tbStreamNumBuffers.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the number of threads field
            try
            {
                numThreads = (ushort)QuickUsbDiag.ParseNumberInt32(tbStreamNumThreads.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of threads", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of threads", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the thread concurrency field
            try
            {
                threadConcurrency = (ushort)QuickUsbDiag.ParseNumberInt32(tbStreamThreadConcurrency.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the thread concurrency", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the thread concurrency", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Determine where the file will be located
            QuickUsb.StreamFlag flags = 0;

            // Check if the file already exists
            if (rbReadStreamToDisk.Checked && File.Exists(tbReadStreamFileName.Text))
            {
                if (MessageBox.Show("The indicated file already exists.  Would you like to overwrite it?", "File Error", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    flags |= QuickUsb.StreamFlag.CreateAlways;
                }
            }

            // Intialize statistics
            success = Module.ResetStatistic(QuickUsb.Stat.TotalDataRead);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            success = Module.ResetStatistic(QuickUsb.Stat.ReadThroughput);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Begin data streaming
            if (rbReadStreamToDisk.Checked)
            {
                success = Module.ReadBulkDataStartStreamToFile(tbReadStreamFileName.Text, numBuffers, length, 0, out readStreamID, flags);
            }
            else
            {
                success = Module.ReadBulkDataStartStream(IntPtr.Zero, numBuffers, length, streamCompletionRoutine, IntPtr.Zero, out readStreamID, numThreads, threadConcurrency);
            }
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ReadData failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update enabled controls
            gbStreamingGeneral.Enabled = false;
            foreach (Control ctrl in gbStreamingRead.Controls)
            {
                ctrl.Enabled = ((ctrl == bReadStreamStop) || (ctrl == bReadStreamStart));
            }
            bReadStreamStart.Text = "Pause";

            // Start reporting statistics
            reportReadStatistics = true;
        }

        private void bReadStreamStop_Click(object sender, EventArgs e)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check that a stream is running
            if (readStreamID == 255)
            {
                ShowStatusMessage("No read stream is currently running");
                return;
            }

            // Stop the stream
            bool success = Module.StopStream(readStreamID, false);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.StopStream failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Do not track the stream ID anymore (this will cause statistic updates to halt on the next update)
            readStreamID = 255;

            // Update enabled controls
            gbStreamingGeneral.Enabled = true;
            foreach (Control ctrl in gbStreamingRead.Controls)
            {
                ctrl.Enabled = (ctrl != bReadStreamStop);
            }
            lReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
            tbReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
            lWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;
            tbWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;

            bReadStreamStart.Text = "Start";
        }

        private void bWriteStreamStart_Click(object sender, EventArgs e)
        {
            uint length;
            uint numBuffers;
            ushort numThreads;
            ushort threadConcurrency;
            bool success;

            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check if we are to start, pause, or resume
            if (bWriteStreamStart.Text == "Pause")
            {
                // Pause the write stream
                success = Module.PauseStream(writeStreamID, true);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.PauseStream failed with error: {0}", Module.LastError().ToString()));
                }
                else
                {
                    bWriteStreamStart.Text = "Resume";
                }
                return;
            }
            else if (bWriteStreamStart.Text == "Resume")
            {
                // When we resume we should reset the throughput statistic so that the pause time doesn't influence the reported data rate
                success = Module.ResetStatistic(QuickUsb.Stat.WriteThroughput);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                    return;
                }

                // Resume the write stream
                success = Module.ResumeStream(writeStreamID);
                if (!success)
                {
                    ShowStatusMessage(String.Format("QuickUsb.ResumeStream failed with error: {0}", Module.LastError().ToString()));
                }
                else
                {
                    bWriteStreamStart.Text = "Pause";
                }
                return;
            }

            // Ensure that a valid file has been specified
            if (rbWriteStreamFromDisk.Checked && tbWriteStreamFileName.Text == "")
            {
                MessageBox.Show("Please enter a valid output file path", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the length field
            try
            {
                length = (uint)QuickUsbDiag.ParseNumberInt32(tbStreamBufferSize.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the number of buffers field
            try
            {
                numBuffers = (uint)QuickUsbDiag.ParseNumberInt32(tbStreamNumBuffers.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of bytes", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the number of threads field
            try
            {
                numThreads = (ushort)QuickUsbDiag.ParseNumberInt32(tbStreamNumThreads.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the number of thwrites", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the number of thwrites", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Parse the thread concurrency field
            try
            {
                threadConcurrency = (ushort)QuickUsbDiag.ParseNumberInt32(tbStreamThreadConcurrency.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid value for the thwrite concurrency", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (OverflowException)
            {
                MessageBox.Show("Please enter a valid value for the thwrite concurrency", "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Determine where the file will be located
            QuickUsb.StreamFlag flags = 0;

            // Intialize statistics
            success = Module.ResetStatistic(QuickUsb.Stat.TotalDataWritten);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }
            success = Module.ResetStatistic(QuickUsb.Stat.WriteThroughput);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.ResetStatistic failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Create data buffers
            ushort[][] writeStreamBuffers = new ushort[numBuffers][];
            for (int k = 0; k < numBuffers; ++k)
            {
                writeStreamBuffers[k] = new ushort[length / 2]; // 'length' is in bytes

                // Fill each buffer with data
                for (int j = 0; j < (length / 2); ++j)
                {
                    writeStreamBuffers[k][j] = writeStreamCount++;
                }
            }

            // Begin data streaming
            if (rbWriteStreamFromDisk.Checked)
            {
                success = Module.WriteBulkDataStartStreamFromFile(tbWriteStreamFileName.Text, numBuffers, length, 0, out writeStreamID, flags);
            }
            else
            {
                success = Module.WriteBulkDataStartStream(writeStreamBuffers, numBuffers, length, streamCompletionRoutine, IntPtr.Zero, out writeStreamID, numThreads, threadConcurrency);
            }
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.WriteBulkDataStartStreamFromFile failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Update enabled controls
            gbStreamingGeneral.Enabled = false;
            foreach (Control ctrl in gbStreamingWrite.Controls)
            {
                ctrl.Enabled = ((ctrl == bWriteStreamStart) || (ctrl == bWriteStreamStop));
            }
            bWriteStreamStart.Text = "Pause";

            // Start reporting statistics
            reportWriteStatistics = true;
        }

        private void bWriteStreamStop_Click(object sender, EventArgs e)
        {
            // Make sure the device is still connected and selected
            if (Module == null)
            {
                return;
            }

            // Check that a stream is running
            if (writeStreamID == 255)
            {
                ShowStatusMessage("No write stream is currently running");
                return;
            }

            // Stop the stream
            bool success = Module.StopStream(writeStreamID, false);
            if (!success)
            {
                ShowStatusMessage(String.Format("QuickUsb.StopStream failed with error: {0}", Module.LastError().ToString()));
                return;
            }

            // Do not track the stream ID anymore (this will cause statistic updates to halt on the next update)
            writeStreamID = 255;

            // Update enabled controls
            gbStreamingGeneral.Enabled = true;
            foreach (Control ctrl in gbStreamingWrite.Controls)
            {
                ctrl.Enabled = (ctrl != bWriteStreamStop);
            }
            lReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
            tbReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
            lWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;
            tbWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;

            bWriteStreamStart.Text = "Start";
        }

        private void rbStreamRead_CheckedChanged(object sender, EventArgs e)
        {
            lReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
            tbReadStreamFileName.Enabled = rbReadStreamToDisk.Checked;
        }

        private void rbStreamWrite_CheckedChanged(object sender, EventArgs e)
        {
            lWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;
            tbWriteStreamFileName.Enabled = rbWriteStreamFromDisk.Checked;
        }

        private void QuickUsbDiag_Shown(object sender, EventArgs e)
        {
            splitContainer.SplitterDistance = 120;
        }

        private void continuallyScanForNewremovedDevicesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            continuallyScanForNewremovedDevicesToolStripMenuItem.Checked = !continuallyScanForNewremovedDevicesToolStripMenuItem.Checked;
            quickUsbWinForms.ScanEnabled = continuallyScanForNewremovedDevicesToolStripMenuItem.Checked;
        }
    }
}
