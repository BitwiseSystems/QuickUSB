namespace QuickUsbBulkStreamWinCs
{
    partial class mainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.dataRate = new System.Windows.Forms.Label();
            this.labelInTransCount = new System.Windows.Forms.Label();
            this.labelOutTransCount = new System.Windows.Forms.Label();
            this.labelErrorCount = new System.Windows.Forms.Label();
            this.gbConfig = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cbGlobalOopen = new System.Windows.Forms.CheckBox();
            this.nTimeout = new System.Windows.Forms.NumericUpDown();
            this.cbIsOpened = new System.Windows.Forms.CheckBox();
            this.cbScanInBackground = new System.Windows.Forms.CheckBox();
            this.cbUserAlloc = new System.Windows.Forms.CheckBox();
            this.cbTestType = new System.Windows.Forms.ComboBox();
            this.lConcurrency = new System.Windows.Forms.Label();
            this.lNumThreads = new System.Windows.Forms.Label();
            this.lBufferSize = new System.Windows.Forms.Label();
            this.nConcurrency = new System.Windows.Forms.NumericUpDown();
            this.nNumThreads = new System.Windows.Forms.NumericUpDown();
            this.nBufferSize = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.lNumBuffers = new System.Windows.Forms.Label();
            this.nNumBuffers = new System.Windows.Forms.NumericUpDown();
            this.bRunTest = new System.Windows.Forms.CheckBox();
            this.bPowerCycleFpga = new System.Windows.Forms.Button();
            this.gbLog = new System.Windows.Forms.GroupBox();
            this.bClearLog = new System.Windows.Forms.Button();
            this.gbStats = new System.Windows.Forms.GroupBox();
            this.lFailures = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.bUpdateCount = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lBERTCount = new System.Windows.Forms.Label();
            this.lDataRead = new System.Windows.Forms.Label();
            this.lDataWritten = new System.Windows.Forms.Label();
            this.lDataRate = new System.Windows.Forms.Label();
            this.lWriteTrans = new System.Windows.Forms.Label();
            this.lReadTrans = new System.Windows.Forms.Label();
            this.pbProgressBar = new System.Windows.Forms.ProgressBar();
            this.bProgramFpga = new System.Windows.Forms.Button();
            this.qusbControl = new BitwiseSystems.QuickUsbWinForms();
            this.suiteTimer = new System.Windows.Forms.Timer(this.components);
            this.bPauseStream = new System.Windows.Forms.Button();
            this.bResumeStream = new System.Windows.Forms.Button();
            this.bRunTestSuite = new System.Windows.Forms.CheckBox();
            this.bCopyLog = new System.Windows.Forms.Button();
            this.btConfig = new BitwiseSystems.BitTwiddler();
            this.gbConfig.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nTimeout)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nConcurrency)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nNumThreads)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nBufferSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nNumBuffers)).BeginInit();
            this.gbLog.SuspendLayout();
            this.gbStats.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbLog.Location = new System.Drawing.Point(3, 16);
            this.tbLog.Margin = new System.Windows.Forms.Padding(0);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.ShortcutsEnabled = false;
            this.tbLog.Size = new System.Drawing.Size(658, 144);
            this.tbLog.TabIndex = 0;
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Interval = 500;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // dataRate
            // 
            this.dataRate.AutoSize = true;
            this.dataRate.Location = new System.Drawing.Point(6, 67);
            this.dataRate.Name = "dataRate";
            this.dataRate.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.dataRate.Size = new System.Drawing.Size(59, 17);
            this.dataRate.TabIndex = 4;
            this.dataRate.Text = "Data Rate:";
            // 
            // labelInTransCount
            // 
            this.labelInTransCount.AutoSize = true;
            this.labelInTransCount.Location = new System.Drawing.Point(6, 84);
            this.labelInTransCount.Name = "labelInTransCount";
            this.labelInTransCount.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.labelInTransCount.Size = new System.Drawing.Size(100, 17);
            this.labelInTransCount.TabIndex = 5;
            this.labelInTransCount.Text = "Read Transactions:";
            // 
            // labelOutTransCount
            // 
            this.labelOutTransCount.AutoSize = true;
            this.labelOutTransCount.Location = new System.Drawing.Point(6, 101);
            this.labelOutTransCount.Name = "labelOutTransCount";
            this.labelOutTransCount.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.labelOutTransCount.Size = new System.Drawing.Size(99, 17);
            this.labelOutTransCount.TabIndex = 6;
            this.labelOutTransCount.Text = "Write Transactions:";
            // 
            // labelErrorCount
            // 
            this.labelErrorCount.AutoSize = true;
            this.labelErrorCount.Location = new System.Drawing.Point(6, 16);
            this.labelErrorCount.Name = "labelErrorCount";
            this.labelErrorCount.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.labelErrorCount.Size = new System.Drawing.Size(70, 17);
            this.labelErrorCount.TabIndex = 7;
            this.labelErrorCount.Text = "BERT Count:";
            // 
            // gbConfig
            // 
            this.gbConfig.Controls.Add(this.label1);
            this.gbConfig.Controls.Add(this.cbGlobalOopen);
            this.gbConfig.Controls.Add(this.nTimeout);
            this.gbConfig.Controls.Add(this.cbIsOpened);
            this.gbConfig.Controls.Add(this.btConfig);
            this.gbConfig.Controls.Add(this.cbScanInBackground);
            this.gbConfig.Controls.Add(this.cbUserAlloc);
            this.gbConfig.Controls.Add(this.cbTestType);
            this.gbConfig.Controls.Add(this.lConcurrency);
            this.gbConfig.Controls.Add(this.lNumThreads);
            this.gbConfig.Controls.Add(this.lBufferSize);
            this.gbConfig.Controls.Add(this.nConcurrency);
            this.gbConfig.Controls.Add(this.nNumThreads);
            this.gbConfig.Controls.Add(this.nBufferSize);
            this.gbConfig.Controls.Add(this.label3);
            this.gbConfig.Controls.Add(this.lNumBuffers);
            this.gbConfig.Controls.Add(this.nNumBuffers);
            this.gbConfig.Location = new System.Drawing.Point(0, 86);
            this.gbConfig.Name = "gbConfig";
            this.gbConfig.Size = new System.Drawing.Size(356, 203);
            this.gbConfig.TabIndex = 8;
            this.gbConfig.TabStop = false;
            this.gbConfig.Text = "Configuration";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 152);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Timeout:";
            // 
            // cbGlobalOopen
            // 
            this.cbGlobalOopen.AutoSize = true;
            this.cbGlobalOopen.Location = new System.Drawing.Point(219, 101);
            this.cbGlobalOopen.Name = "cbGlobalOopen";
            this.cbGlobalOopen.Size = new System.Drawing.Size(85, 17);
            this.cbGlobalOopen.TabIndex = 19;
            this.cbGlobalOopen.Text = "Global Open";
            this.cbGlobalOopen.UseVisualStyleBackColor = true;
            this.cbGlobalOopen.CheckedChanged += new System.EventHandler(this.cbGlobalOopen_CheckedChanged);
            // 
            // nTimeout
            // 
            this.nTimeout.Location = new System.Drawing.Point(88, 150);
            this.nTimeout.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.nTimeout.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nTimeout.Name = "nTimeout";
            this.nTimeout.Size = new System.Drawing.Size(73, 20);
            this.nTimeout.TabIndex = 12;
            this.nTimeout.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nTimeout.ValueChanged += new System.EventHandler(this.nTimeout_ValueChanged);
            // 
            // cbIsOpened
            // 
            this.cbIsOpened.AutoSize = true;
            this.cbIsOpened.Enabled = false;
            this.cbIsOpened.Location = new System.Drawing.Point(219, 122);
            this.cbIsOpened.Name = "cbIsOpened";
            this.cbIsOpened.Size = new System.Drawing.Size(72, 17);
            this.cbIsOpened.TabIndex = 18;
            this.cbIsOpened.Text = "IsOpened";
            this.cbIsOpened.UseVisualStyleBackColor = true;
            // 
            // cbScanInBackground
            // 
            this.cbScanInBackground.AutoSize = true;
            this.cbScanInBackground.Checked = true;
            this.cbScanInBackground.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScanInBackground.Location = new System.Drawing.Point(219, 78);
            this.cbScanInBackground.Name = "cbScanInBackground";
            this.cbScanInBackground.Size = new System.Drawing.Size(114, 17);
            this.cbScanInBackground.TabIndex = 18;
            this.cbScanInBackground.Text = "FindModules Scan";
            this.cbScanInBackground.UseVisualStyleBackColor = true;
            this.cbScanInBackground.CheckedChanged += new System.EventHandler(this.cbScanInBackground_CheckedChanged);
            // 
            // cbUserAlloc
            // 
            this.cbUserAlloc.AutoSize = true;
            this.cbUserAlloc.Checked = global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default.UserAllocBuffers;
            this.cbUserAlloc.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbUserAlloc.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default, "UserAllocBuffers", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbUserAlloc.Location = new System.Drawing.Point(167, 47);
            this.cbUserAlloc.Name = "cbUserAlloc";
            this.cbUserAlloc.Size = new System.Drawing.Size(95, 17);
            this.cbUserAlloc.TabIndex = 15;
            this.cbUserAlloc.Text = "User-Allocated";
            this.cbUserAlloc.UseVisualStyleBackColor = true;
            // 
            // cbTestType
            // 
            this.cbTestType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbTestType.FormattingEnabled = true;
            this.cbTestType.Items.AddRange(new object[] {
            "Synchronous Read",
            "Synchronous Write",
            "Synchronous Loopback",
            "Asynchronous Read",
            "Asynchronous Write",
            "Asynchronous Loopback",
            "Asynchronous Read (Legacy)",
            "Asynchronous Write (Legacy)",
            "Asynchronous Loopback (Legacy) NOT IMPLEMENTED",
            "Streaming Read",
            "Streaming Write",
            "Streaming Loopback"});
            this.cbTestType.Location = new System.Drawing.Point(88, 19);
            this.cbTestType.Name = "cbTestType";
            this.cbTestType.Size = new System.Drawing.Size(262, 21);
            this.cbTestType.TabIndex = 11;
            this.cbTestType.SelectedIndexChanged += new System.EventHandler(this.cbTestType_SelectedIndexChanged);
            // 
            // lConcurrency
            // 
            this.lConcurrency.AutoSize = true;
            this.lConcurrency.Location = new System.Drawing.Point(12, 126);
            this.lConcurrency.Name = "lConcurrency";
            this.lConcurrency.Size = new System.Drawing.Size(70, 13);
            this.lConcurrency.TabIndex = 14;
            this.lConcurrency.Text = "Concurrency:";
            // 
            // lNumThreads
            // 
            this.lNumThreads.AutoSize = true;
            this.lNumThreads.Location = new System.Drawing.Point(12, 100);
            this.lNumThreads.Name = "lNumThreads";
            this.lNumThreads.Size = new System.Drawing.Size(59, 13);
            this.lNumThreads.TabIndex = 14;
            this.lNumThreads.Text = "# Threads:";
            // 
            // lBufferSize
            // 
            this.lBufferSize.AutoSize = true;
            this.lBufferSize.Location = new System.Drawing.Point(12, 74);
            this.lBufferSize.Name = "lBufferSize";
            this.lBufferSize.Size = new System.Drawing.Size(61, 13);
            this.lBufferSize.TabIndex = 14;
            this.lBufferSize.Text = "Buffer Size:";
            // 
            // nConcurrency
            // 
            this.nConcurrency.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default, "Concurrency", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nConcurrency.Location = new System.Drawing.Point(88, 124);
            this.nConcurrency.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.nConcurrency.Name = "nConcurrency";
            this.nConcurrency.Size = new System.Drawing.Size(73, 20);
            this.nConcurrency.TabIndex = 13;
            this.nConcurrency.Value = global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default.Concurrency;
            // 
            // nNumThreads
            // 
            this.nNumThreads.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default, "NumThreads", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nNumThreads.Location = new System.Drawing.Point(88, 98);
            this.nNumThreads.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.nNumThreads.Name = "nNumThreads";
            this.nNumThreads.Size = new System.Drawing.Size(73, 20);
            this.nNumThreads.TabIndex = 13;
            this.nNumThreads.Value = global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default.NumThreads;
            // 
            // nBufferSize
            // 
            this.nBufferSize.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default, "BufferSize", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nBufferSize.Location = new System.Drawing.Point(88, 72);
            this.nBufferSize.Maximum = new decimal(new int[] {
            16777216,
            0,
            0,
            0});
            this.nBufferSize.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nBufferSize.Name = "nBufferSize";
            this.nBufferSize.Size = new System.Drawing.Size(73, 20);
            this.nBufferSize.TabIndex = 13;
            this.nBufferSize.Value = global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default.BufferSize;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Test Type:";
            // 
            // lNumBuffers
            // 
            this.lNumBuffers.AutoSize = true;
            this.lNumBuffers.Location = new System.Drawing.Point(12, 48);
            this.lNumBuffers.Name = "lNumBuffers";
            this.lNumBuffers.Size = new System.Drawing.Size(53, 13);
            this.lNumBuffers.TabIndex = 12;
            this.lNumBuffers.Text = "# Buffers:";
            // 
            // nNumBuffers
            // 
            this.nNumBuffers.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default, "NumBuffers", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nNumBuffers.Location = new System.Drawing.Point(88, 46);
            this.nNumBuffers.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nNumBuffers.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nNumBuffers.Name = "nNumBuffers";
            this.nNumBuffers.Size = new System.Drawing.Size(73, 20);
            this.nNumBuffers.TabIndex = 11;
            this.nNumBuffers.Value = global::QuickUsbBulkStreamLoopBackCs.Properties.Settings.Default.NumBuffers;
            // 
            // bRunTest
            // 
            this.bRunTest.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bRunTest.Appearance = System.Windows.Forms.Appearance.Button;
            this.bRunTest.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRunTest.Location = new System.Drawing.Point(563, 494);
            this.bRunTest.Name = "bRunTest";
            this.bRunTest.Size = new System.Drawing.Size(101, 23);
            this.bRunTest.TabIndex = 16;
            this.bRunTest.Text = "Run Test";
            this.bRunTest.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bRunTest.UseVisualStyleBackColor = true;
            this.bRunTest.CheckedChanged += new System.EventHandler(this.bRunTest_CheckedChanged);
            // 
            // bPowerCycleFpga
            // 
            this.bPowerCycleFpga.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bPowerCycleFpga.Location = new System.Drawing.Point(255, 494);
            this.bPowerCycleFpga.Name = "bPowerCycleFpga";
            this.bPowerCycleFpga.Size = new System.Drawing.Size(101, 23);
            this.bPowerCycleFpga.TabIndex = 11;
            this.bPowerCycleFpga.Text = "Power-Cyle FPGA";
            this.bPowerCycleFpga.UseVisualStyleBackColor = true;
            this.bPowerCycleFpga.Click += new System.EventHandler(this.bPowerCycleFpga_Click);
            // 
            // gbLog
            // 
            this.gbLog.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbLog.Controls.Add(this.bCopyLog);
            this.gbLog.Controls.Add(this.bClearLog);
            this.gbLog.Controls.Add(this.tbLog);
            this.gbLog.Location = new System.Drawing.Point(0, 295);
            this.gbLog.Name = "gbLog";
            this.gbLog.Size = new System.Drawing.Size(664, 193);
            this.gbLog.TabIndex = 9;
            this.gbLog.TabStop = false;
            this.gbLog.Text = "Log";
            // 
            // bClearLog
            // 
            this.bClearLog.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bClearLog.Location = new System.Drawing.Point(87, 164);
            this.bClearLog.Name = "bClearLog";
            this.bClearLog.Size = new System.Drawing.Size(75, 23);
            this.bClearLog.TabIndex = 11;
            this.bClearLog.Text = "Clear";
            this.bClearLog.UseVisualStyleBackColor = true;
            this.bClearLog.Click += new System.EventHandler(this.bClearLog_Click);
            // 
            // gbStats
            // 
            this.gbStats.Controls.Add(this.lFailures);
            this.gbStats.Controls.Add(this.label2);
            this.gbStats.Controls.Add(this.bUpdateCount);
            this.gbStats.Controls.Add(this.label6);
            this.gbStats.Controls.Add(this.labelInTransCount);
            this.gbStats.Controls.Add(this.label5);
            this.gbStats.Controls.Add(this.labelOutTransCount);
            this.gbStats.Controls.Add(this.lBERTCount);
            this.gbStats.Controls.Add(this.lDataRead);
            this.gbStats.Controls.Add(this.lDataWritten);
            this.gbStats.Controls.Add(this.lDataRate);
            this.gbStats.Controls.Add(this.lWriteTrans);
            this.gbStats.Controls.Add(this.lReadTrans);
            this.gbStats.Controls.Add(this.labelErrorCount);
            this.gbStats.Controls.Add(this.dataRate);
            this.gbStats.Location = new System.Drawing.Point(362, 86);
            this.gbStats.Name = "gbStats";
            this.gbStats.Size = new System.Drawing.Size(302, 139);
            this.gbStats.TabIndex = 10;
            this.gbStats.TabStop = false;
            this.gbStats.Text = "Statistics";
            // 
            // lFailures
            // 
            this.lFailures.AutoSize = true;
            this.lFailures.Location = new System.Drawing.Point(112, 118);
            this.lFailures.Name = "lFailures";
            this.lFailures.Size = new System.Drawing.Size(34, 13);
            this.lFailures.TabIndex = 22;
            this.lFailures.Text = "Value";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 118);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(70, 13);
            this.label2.TabIndex = 21;
            this.label2.Text = "Test Failures:";
            // 
            // bUpdateCount
            // 
            this.bUpdateCount.Location = new System.Drawing.Point(246, 11);
            this.bUpdateCount.Name = "bUpdateCount";
            this.bUpdateCount.Size = new System.Drawing.Size(53, 23);
            this.bUpdateCount.TabIndex = 18;
            this.bUpdateCount.Text = "Update";
            this.bUpdateCount.UseVisualStyleBackColor = true;
            this.bUpdateCount.Click += new System.EventHandler(this.bUpdateCount_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 33);
            this.label6.Name = "label6";
            this.label6.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.label6.Size = new System.Drawing.Size(62, 17);
            this.label6.TabIndex = 5;
            this.label6.Text = "Data Read:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 50);
            this.label5.Name = "label5";
            this.label5.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.label5.Size = new System.Drawing.Size(70, 17);
            this.label5.TabIndex = 6;
            this.label5.Text = "Data Written:";
            // 
            // lBERTCount
            // 
            this.lBERTCount.AutoSize = true;
            this.lBERTCount.Location = new System.Drawing.Point(112, 16);
            this.lBERTCount.Name = "lBERTCount";
            this.lBERTCount.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lBERTCount.Size = new System.Drawing.Size(34, 17);
            this.lBERTCount.TabIndex = 7;
            this.lBERTCount.Text = "Value";
            // 
            // lDataRead
            // 
            this.lDataRead.AutoSize = true;
            this.lDataRead.Location = new System.Drawing.Point(112, 33);
            this.lDataRead.Name = "lDataRead";
            this.lDataRead.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lDataRead.Size = new System.Drawing.Size(34, 17);
            this.lDataRead.TabIndex = 7;
            this.lDataRead.Text = "Value";
            // 
            // lDataWritten
            // 
            this.lDataWritten.AutoSize = true;
            this.lDataWritten.Location = new System.Drawing.Point(112, 50);
            this.lDataWritten.Name = "lDataWritten";
            this.lDataWritten.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lDataWritten.Size = new System.Drawing.Size(34, 17);
            this.lDataWritten.TabIndex = 7;
            this.lDataWritten.Text = "Value";
            // 
            // lDataRate
            // 
            this.lDataRate.AutoSize = true;
            this.lDataRate.Location = new System.Drawing.Point(112, 67);
            this.lDataRate.Name = "lDataRate";
            this.lDataRate.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lDataRate.Size = new System.Drawing.Size(34, 17);
            this.lDataRate.TabIndex = 7;
            this.lDataRate.Text = "Value";
            // 
            // lWriteTrans
            // 
            this.lWriteTrans.AutoSize = true;
            this.lWriteTrans.Location = new System.Drawing.Point(112, 101);
            this.lWriteTrans.Name = "lWriteTrans";
            this.lWriteTrans.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lWriteTrans.Size = new System.Drawing.Size(34, 17);
            this.lWriteTrans.TabIndex = 7;
            this.lWriteTrans.Text = "Value";
            // 
            // lReadTrans
            // 
            this.lReadTrans.AutoSize = true;
            this.lReadTrans.Location = new System.Drawing.Point(112, 84);
            this.lReadTrans.Name = "lReadTrans";
            this.lReadTrans.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            this.lReadTrans.Size = new System.Drawing.Size(34, 17);
            this.lReadTrans.TabIndex = 7;
            this.lReadTrans.Text = "Value";
            // 
            // pbProgressBar
            // 
            this.pbProgressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pbProgressBar.Location = new System.Drawing.Point(6, 494);
            this.pbProgressBar.Name = "pbProgressBar";
            this.pbProgressBar.Size = new System.Drawing.Size(243, 23);
            this.pbProgressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pbProgressBar.TabIndex = 17;
            // 
            // bProgramFpga
            // 
            this.bProgramFpga.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bProgramFpga.Location = new System.Drawing.Point(362, 494);
            this.bProgramFpga.Name = "bProgramFpga";
            this.bProgramFpga.Size = new System.Drawing.Size(96, 23);
            this.bProgramFpga.TabIndex = 18;
            this.bProgramFpga.Text = "Program FPGA";
            this.bProgramFpga.UseVisualStyleBackColor = true;
            this.bProgramFpga.Click += new System.EventHandler(this.bProgramFpga_Click);
            // 
            // qusbControl
            // 
            this.qusbControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.qusbControl.AutoScroll = true;
            this.qusbControl.AutoSelectFirstItem = true;
            this.qusbControl.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.qusbControl.Location = new System.Drawing.Point(0, 0);
            this.qusbControl.MinimumSize = new System.Drawing.Size(100, 40);
            this.qusbControl.Name = "qusbControl";
            this.qusbControl.ScanEnabled = true;
            this.qusbControl.ScanInterval = 500;
            this.qusbControl.ScanOnLoad = true;
            this.qusbControl.Scrollable = true;
            this.qusbControl.SelectedModule = null;
            this.qusbControl.SelectedModules = new BitwiseSystems.QuickUsb[0];
            this.qusbControl.ShowUnprogrammedDevices = false;
            this.qusbControl.Size = new System.Drawing.Size(664, 80);
            this.qusbControl.TabIndex = 3;
            this.qusbControl.View = System.Windows.Forms.View.Details;
            this.qusbControl.SelectionChanged += new BitwiseSystems.QuickUsbWinForms.SelectionChangedHandler(this.qusbControl_SelectionChanged);
            // 
            // suiteTimer
            // 
            this.suiteTimer.Tick += new System.EventHandler(this.suiteTimer_Tick);
            // 
            // bPauseStream
            // 
            this.bPauseStream.Location = new System.Drawing.Point(568, 238);
            this.bPauseStream.Name = "bPauseStream";
            this.bPauseStream.Size = new System.Drawing.Size(93, 23);
            this.bPauseStream.TabIndex = 21;
            this.bPauseStream.Text = "Pause Stream";
            this.bPauseStream.UseVisualStyleBackColor = true;
            this.bPauseStream.Click += new System.EventHandler(this.bPauseStream_Click);
            // 
            // bResumeStream
            // 
            this.bResumeStream.Location = new System.Drawing.Point(568, 266);
            this.bResumeStream.Name = "bResumeStream";
            this.bResumeStream.Size = new System.Drawing.Size(93, 23);
            this.bResumeStream.TabIndex = 23;
            this.bResumeStream.Text = "Resume Stream";
            this.bResumeStream.UseVisualStyleBackColor = true;
            this.bResumeStream.Click += new System.EventHandler(this.bResumeStream_Click);
            // 
            // bRunTestSuite
            // 
            this.bRunTestSuite.Appearance = System.Windows.Forms.Appearance.Button;
            this.bRunTestSuite.Location = new System.Drawing.Point(464, 494);
            this.bRunTestSuite.Name = "bRunTestSuite";
            this.bRunTestSuite.Size = new System.Drawing.Size(93, 24);
            this.bRunTestSuite.TabIndex = 21;
            this.bRunTestSuite.Text = "Run Test Suite";
            this.bRunTestSuite.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.bRunTestSuite.UseVisualStyleBackColor = true;
            this.bRunTestSuite.CheckedChanged += new System.EventHandler(this.bRunTestSuite_CheckedChanged);
            // 
            // bCopyLog
            // 
            this.bCopyLog.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bCopyLog.Location = new System.Drawing.Point(6, 164);
            this.bCopyLog.Name = "bCopyLog";
            this.bCopyLog.Size = new System.Drawing.Size(75, 23);
            this.bCopyLog.TabIndex = 12;
            this.bCopyLog.Text = "Copy";
            this.bCopyLog.UseVisualStyleBackColor = true;
            this.bCopyLog.Click += new System.EventHandler(this.bCopyLog_Click);
            // 
            // btConfig
            // 
            this.btConfig.AutoSize = true;
            this.btConfig.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.btConfig.Bits = ((byte)(16));
            this.btConfig.Caption = "Config";
            this.btConfig.CaptionWidth = 40;
            this.btConfig.Location = new System.Drawing.Point(11, 173);
            this.btConfig.Margin = new System.Windows.Forms.Padding(0);
            this.btConfig.Name = "btConfig";
            this.btConfig.Size = new System.Drawing.Size(324, 23);
            this.btConfig.TabIndex = 12;
            this.btConfig.TagInt = 0;
            this.btConfig.ToolTips = new string[] {
        "Bit 0: 0=Do not use OUT FIFO, 1=Use OUT FIFO",
        "Bit 1: 0=Do not use IN FIFO, 1=Use IN FIFO",
        "Bit 2: Reserved",
        "Bit 3: Reserved",
        "Bit 4: Reserved",
        "Bit 5: Reserved",
        "Bit 6: Reserved",
        "Bit 7: Reserved",
        "Bit 8: Reserved",
        "Bit 9: Reserved",
        "Bit 10: Reserved",
        "Bit 11: Reserved",
        "Bit 12: Reserved",
        "Bit 13: Reserved",
        "Bit 14: Reserved",
        "Bit 15: Reserved"};
            this.btConfig.Value = ((uint)(0u));
            this.btConfig.ValueChanged += new BitwiseSystems.ValueChangedHandler(this.btConfig_ValueChanged);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(665, 522);
            this.Controls.Add(this.bRunTestSuite);
            this.Controls.Add(this.bPauseStream);
            this.Controls.Add(this.bResumeStream);
            this.Controls.Add(this.bProgramFpga);
            this.Controls.Add(this.pbProgressBar);
            this.Controls.Add(this.bRunTest);
            this.Controls.Add(this.gbStats);
            this.Controls.Add(this.gbLog);
            this.Controls.Add(this.gbConfig);
            this.Controls.Add(this.qusbControl);
            this.Controls.Add(this.bPowerCycleFpga);
            this.Name = "mainForm";
            this.Text = "QuickUsbBulkStreamWinCs";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            this.gbConfig.ResumeLayout(false);
            this.gbConfig.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nTimeout)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nConcurrency)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nNumThreads)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nBufferSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nNumBuffers)).EndInit();
            this.gbLog.ResumeLayout(false);
            this.gbLog.PerformLayout();
            this.gbStats.ResumeLayout(false);
            this.gbStats.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox tbLog;
        private BitwiseSystems.QuickUsbWinForms qusbControl;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Label dataRate;
        private System.Windows.Forms.Label labelInTransCount;
        private System.Windows.Forms.Label labelOutTransCount;
        private System.Windows.Forms.Label labelErrorCount;
        private System.Windows.Forms.GroupBox gbConfig;
        private System.Windows.Forms.GroupBox gbLog;
        private System.Windows.Forms.GroupBox gbStats;
        private System.Windows.Forms.Label lBufferSize;
        private System.Windows.Forms.NumericUpDown nBufferSize;
        private System.Windows.Forms.Label lNumBuffers;
        private System.Windows.Forms.NumericUpDown nNumBuffers;
        private System.Windows.Forms.Button bPowerCycleFpga;
        private System.Windows.Forms.ComboBox cbTestType;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox bRunTest;
        private System.Windows.Forms.CheckBox cbUserAlloc;
        private System.Windows.Forms.Label lNumThreads;
        private System.Windows.Forms.NumericUpDown nNumThreads;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lBERTCount;
        private System.Windows.Forms.Label lDataRead;
        private System.Windows.Forms.Label lDataWritten;
        private System.Windows.Forms.Label lDataRate;
        private System.Windows.Forms.Label lWriteTrans;
        private System.Windows.Forms.Label lReadTrans;
        private System.Windows.Forms.Button bClearLog;
        private System.Windows.Forms.Label lConcurrency;
        private System.Windows.Forms.NumericUpDown nConcurrency;
        private System.Windows.Forms.ProgressBar pbProgressBar;
        private System.Windows.Forms.CheckBox cbScanInBackground;
        private System.Windows.Forms.Button bUpdateCount;
        private BitwiseSystems.BitTwiddler btConfig;
        private System.Windows.Forms.CheckBox cbIsOpened;
        private System.Windows.Forms.CheckBox cbGlobalOopen;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nTimeout;
        private System.Windows.Forms.Button bProgramFpga;
        private System.Windows.Forms.Timer suiteTimer;
        private System.Windows.Forms.Label lFailures;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button bPauseStream;
        private System.Windows.Forms.Button bResumeStream;
        private System.Windows.Forms.CheckBox bRunTestSuite;
        private System.Windows.Forms.Button bCopyLog;
    }
}

