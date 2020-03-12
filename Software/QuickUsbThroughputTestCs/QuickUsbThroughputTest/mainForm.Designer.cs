namespace QuickUsbThroughputTest
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
            this.cbPacketSizeUnit = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.bStart = new System.Windows.Forms.Button();
            this.bStop = new System.Windows.Forms.Button();
            this.nPacketSize = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.nNumBuffers = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.rbRead = new System.Windows.Forms.RadioButton();
            this.rbWrite = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.rbStreaming = new System.Windows.Forms.RadioButton();
            this.rbAsync = new System.Windows.Forms.RadioButton();
            this.rbSync = new System.Windows.Forms.RadioButton();
            this.rbAsyncLegacy = new System.Windows.Forms.RadioButton();
            this.gbThroughputMeter = new System.Windows.Forms.GroupBox();
            this.bMeasure = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.lThroughput = new System.Windows.Forms.Label();
            this.updateTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.nPacketSize)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nNumBuffers)).BeginInit();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.gbThroughputMeter.SuspendLayout();
            this.SuspendLayout();
            // 
            // cbPacketSizeUnit
            // 
            this.cbPacketSizeUnit.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPacketSizeUnit.FormattingEnabled = true;
            this.cbPacketSizeUnit.Items.AddRange(new object[] {
            "Bytes",
            "Kilobytes",
            "Megabytes"});
            this.cbPacketSizeUnit.Location = new System.Drawing.Point(208, 18);
            this.cbPacketSizeUnit.Name = "cbPacketSizeUnit";
            this.cbPacketSizeUnit.Size = new System.Drawing.Size(121, 21);
            this.cbPacketSizeUnit.TabIndex = 0;
            this.cbPacketSizeUnit.SelectedIndexChanged += new System.EventHandler(this.packetSizeUnit_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Packet Size:";
            // 
            // bStart
            // 
            this.bStart.Location = new System.Drawing.Point(174, 48);
            this.bStart.Name = "bStart";
            this.bStart.Size = new System.Drawing.Size(75, 23);
            this.bStart.TabIndex = 2;
            this.bStart.Text = "Start";
            this.bStart.UseVisualStyleBackColor = true;
            this.bStart.Click += new System.EventHandler(this.bStart_Click);
            // 
            // bStop
            // 
            this.bStop.Enabled = false;
            this.bStop.Location = new System.Drawing.Point(255, 48);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(75, 23);
            this.bStop.TabIndex = 3;
            this.bStop.Text = "Stop";
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.bStop_Click);
            // 
            // nPacketSize
            // 
            this.nPacketSize.Location = new System.Drawing.Point(79, 19);
            this.nPacketSize.Maximum = new decimal(new int[] {
            16777216,
            0,
            0,
            0});
            this.nPacketSize.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nPacketSize.Name = "nPacketSize";
            this.nPacketSize.Size = new System.Drawing.Size(120, 20);
            this.nPacketSize.TabIndex = 4;
            this.nPacketSize.Value = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.nPacketSize.ValueChanged += new System.EventHandler(this.packetSize_ValueChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nNumBuffers);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.panel2);
            this.groupBox1.Controls.Add(this.panel1);
            this.groupBox1.Controls.Add(this.nPacketSize);
            this.groupBox1.Controls.Add(this.cbPacketSizeUnit);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(344, 170);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Configuration";
            // 
            // nNumBuffers
            // 
            this.nNumBuffers.Location = new System.Drawing.Point(79, 45);
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
            this.nNumBuffers.Size = new System.Drawing.Size(120, 20);
            this.nNumBuffers.TabIndex = 13;
            this.nNumBuffers.Value = new decimal(new int[] {
            4,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(68, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Num Buffers:";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.rbRead);
            this.panel2.Controls.Add(this.rbWrite);
            this.panel2.Location = new System.Drawing.Point(174, 71);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(157, 46);
            this.panel2.TabIndex = 11;
            // 
            // rbRead
            // 
            this.rbRead.AutoSize = true;
            this.rbRead.Checked = true;
            this.rbRead.Location = new System.Drawing.Point(4, 3);
            this.rbRead.Name = "rbRead";
            this.rbRead.Size = new System.Drawing.Size(51, 17);
            this.rbRead.TabIndex = 8;
            this.rbRead.TabStop = true;
            this.rbRead.Text = "Read";
            this.rbRead.UseVisualStyleBackColor = true;
            this.rbRead.CheckedChanged += new System.EventHandler(this.rbRead_CheckedChanged);
            // 
            // rbWrite
            // 
            this.rbWrite.AutoSize = true;
            this.rbWrite.Location = new System.Drawing.Point(4, 26);
            this.rbWrite.Name = "rbWrite";
            this.rbWrite.Size = new System.Drawing.Size(50, 17);
            this.rbWrite.TabIndex = 9;
            this.rbWrite.Text = "Write";
            this.rbWrite.UseVisualStyleBackColor = true;
            this.rbWrite.CheckedChanged += new System.EventHandler(this.rbWrite_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.rbStreaming);
            this.panel1.Controls.Add(this.rbAsync);
            this.panel1.Controls.Add(this.rbSync);
            this.panel1.Controls.Add(this.rbAsyncLegacy);
            this.panel1.Location = new System.Drawing.Point(9, 71);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(157, 93);
            this.panel1.TabIndex = 10;
            // 
            // rbStreaming
            // 
            this.rbStreaming.AutoSize = true;
            this.rbStreaming.Location = new System.Drawing.Point(3, 72);
            this.rbStreaming.Name = "rbStreaming";
            this.rbStreaming.Size = new System.Drawing.Size(72, 17);
            this.rbStreaming.TabIndex = 9;
            this.rbStreaming.TabStop = true;
            this.rbStreaming.Text = "Streaming";
            this.rbStreaming.UseVisualStyleBackColor = true;
            this.rbStreaming.CheckedChanged += new System.EventHandler(this.rbAcqType_CheckedChanged);
            // 
            // rbAsync
            // 
            this.rbAsync.AutoSize = true;
            this.rbAsync.Location = new System.Drawing.Point(3, 26);
            this.rbAsync.Name = "rbAsync";
            this.rbAsync.Size = new System.Drawing.Size(92, 17);
            this.rbAsync.TabIndex = 8;
            this.rbAsync.TabStop = true;
            this.rbAsync.Text = "Asynchronous";
            this.rbAsync.UseVisualStyleBackColor = true;
            this.rbAsync.CheckedChanged += new System.EventHandler(this.rbAcqType_CheckedChanged);
            // 
            // rbSync
            // 
            this.rbSync.AutoSize = true;
            this.rbSync.Checked = true;
            this.rbSync.Location = new System.Drawing.Point(3, 3);
            this.rbSync.Name = "rbSync";
            this.rbSync.Size = new System.Drawing.Size(87, 17);
            this.rbSync.TabIndex = 6;
            this.rbSync.TabStop = true;
            this.rbSync.Text = "Synchronous";
            this.rbSync.UseVisualStyleBackColor = true;
            this.rbSync.CheckedChanged += new System.EventHandler(this.rbAcqType_CheckedChanged);
            // 
            // rbAsyncLegacy
            // 
            this.rbAsyncLegacy.AutoSize = true;
            this.rbAsyncLegacy.Location = new System.Drawing.Point(3, 49);
            this.rbAsyncLegacy.Name = "rbAsyncLegacy";
            this.rbAsyncLegacy.Size = new System.Drawing.Size(136, 17);
            this.rbAsyncLegacy.TabIndex = 7;
            this.rbAsyncLegacy.Text = "Asynchronous (Legacy)";
            this.rbAsyncLegacy.UseVisualStyleBackColor = true;
            this.rbAsyncLegacy.CheckedChanged += new System.EventHandler(this.rbAcqType_CheckedChanged);
            // 
            // gbThroughputMeter
            // 
            this.gbThroughputMeter.Controls.Add(this.bMeasure);
            this.gbThroughputMeter.Controls.Add(this.progressBar);
            this.gbThroughputMeter.Controls.Add(this.lThroughput);
            this.gbThroughputMeter.Controls.Add(this.bStart);
            this.gbThroughputMeter.Controls.Add(this.bStop);
            this.gbThroughputMeter.Location = new System.Drawing.Point(12, 188);
            this.gbThroughputMeter.Name = "gbThroughputMeter";
            this.gbThroughputMeter.Size = new System.Drawing.Size(345, 111);
            this.gbThroughputMeter.TabIndex = 6;
            this.gbThroughputMeter.TabStop = false;
            this.gbThroughputMeter.Text = "Throughput Meter";
            // 
            // bMeasure
            // 
            this.bMeasure.Location = new System.Drawing.Point(174, 77);
            this.bMeasure.Name = "bMeasure";
            this.bMeasure.Size = new System.Drawing.Size(155, 23);
            this.bMeasure.TabIndex = 9;
            this.bMeasure.Text = "Measure Throughput";
            this.bMeasure.UseVisualStyleBackColor = true;
            this.bMeasure.Click += new System.EventHandler(this.bMeasure_Click);
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(10, 19);
            this.progressBar.Maximum = 35;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(320, 23);
            this.progressBar.TabIndex = 8;
            // 
            // lThroughput
            // 
            this.lThroughput.AutoSize = true;
            this.lThroughput.Location = new System.Drawing.Point(7, 53);
            this.lThroughput.Name = "lThroughput";
            this.lThroughput.Size = new System.Drawing.Size(118, 13);
            this.lThroughput.TabIndex = 7;
            this.lThroughput.Text = "Throughput: 0.00 MB/s";
            // 
            // updateTimer
            // 
            this.updateTimer.Interval = 250;
            this.updateTimer.Tick += new System.EventHandler(this.updateTimer_Tick);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(371, 311);
            this.Controls.Add(this.gbThroughputMeter);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "mainForm";
            this.Text = "QuickUSB Throughput Test";
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.nPacketSize)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nNumBuffers)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.gbThroughputMeter.ResumeLayout(false);
            this.gbThroughputMeter.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ComboBox cbPacketSizeUnit;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button bStart;
		private System.Windows.Forms.Button bStop;
		private System.Windows.Forms.NumericUpDown nPacketSize;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton rbWrite;
		private System.Windows.Forms.RadioButton rbRead;
		private System.Windows.Forms.RadioButton rbAsyncLegacy;
		private System.Windows.Forms.RadioButton rbSync;
		private System.Windows.Forms.GroupBox gbThroughputMeter;
		private System.Windows.Forms.Label lThroughput;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Timer updateTimer;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.Button bMeasure;
        private System.Windows.Forms.RadioButton rbAsync;
        private System.Windows.Forms.NumericUpDown nNumBuffers;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RadioButton rbStreaming;
	}
}

