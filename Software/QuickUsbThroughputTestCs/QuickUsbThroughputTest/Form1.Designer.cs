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
			this.packetSizeUnit = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.bStart = new System.Windows.Forms.Button();
			this.bStop = new System.Windows.Forms.Button();
			this.packetSize = new System.Windows.Forms.NumericUpDown();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.radioButton4 = new System.Windows.Forms.RadioButton();
			this.radioButton3 = new System.Windows.Forms.RadioButton();
			this.radioButton2 = new System.Windows.Forms.RadioButton();
			this.radioButton1 = new System.Windows.Forms.RadioButton();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.lthroughput = new System.Windows.Forms.Label();
			this.progressBar = new System.Windows.Forms.ProgressBar();
			this.panel1 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.packetSize)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// packetSizeUnit
			// 
			this.packetSizeUnit.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.packetSizeUnit.FormattingEnabled = true;
			this.packetSizeUnit.Items.AddRange(new object[] {
            "Bytes",
            "Kilobytes",
            "Megabytes"});
			this.packetSizeUnit.Location = new System.Drawing.Point(208, 18);
			this.packetSizeUnit.Name = "packetSizeUnit";
			this.packetSizeUnit.Size = new System.Drawing.Size(121, 21);
			this.packetSizeUnit.TabIndex = 0;
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
			this.bStart.Location = new System.Drawing.Point(162, 48);
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
			this.bStop.Location = new System.Drawing.Point(243, 48);
			this.bStop.Name = "bStop";
			this.bStop.Size = new System.Drawing.Size(75, 23);
			this.bStop.TabIndex = 3;
			this.bStop.Text = "Stop";
			this.bStop.UseVisualStyleBackColor = true;
			this.bStop.Click += new System.EventHandler(this.bStop_Click);
			// 
			// packetSize
			// 
			this.packetSize.Location = new System.Drawing.Point(79, 19);
			this.packetSize.Name = "packetSize";
			this.packetSize.Size = new System.Drawing.Size(120, 20);
			this.packetSize.TabIndex = 4;
			this.packetSize.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.panel2);
			this.groupBox1.Controls.Add(this.panel1);
			this.groupBox1.Controls.Add(this.packetSize);
			this.groupBox1.Controls.Add(this.packetSizeUnit);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(344, 97);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Configuration";
			// 
			// radioButton4
			// 
			this.radioButton4.AutoSize = true;
			this.radioButton4.Location = new System.Drawing.Point(4, 26);
			this.radioButton4.Name = "radioButton4";
			this.radioButton4.Size = new System.Drawing.Size(50, 17);
			this.radioButton4.TabIndex = 9;
			this.radioButton4.Text = "Write";
			this.radioButton4.UseVisualStyleBackColor = true;
			// 
			// radioButton3
			// 
			this.radioButton3.AutoSize = true;
			this.radioButton3.Checked = true;
			this.radioButton3.Location = new System.Drawing.Point(4, 3);
			this.radioButton3.Name = "radioButton3";
			this.radioButton3.Size = new System.Drawing.Size(51, 17);
			this.radioButton3.TabIndex = 8;
			this.radioButton3.TabStop = true;
			this.radioButton3.Text = "Read";
			this.radioButton3.UseVisualStyleBackColor = true;
			// 
			// radioButton2
			// 
			this.radioButton2.AutoSize = true;
			this.radioButton2.Enabled = false;
			this.radioButton2.Location = new System.Drawing.Point(3, 26);
			this.radioButton2.Name = "radioButton2";
			this.radioButton2.Size = new System.Drawing.Size(92, 17);
			this.radioButton2.TabIndex = 7;
			this.radioButton2.Text = "Asynchronous";
			this.radioButton2.UseVisualStyleBackColor = true;
			// 
			// radioButton1
			// 
			this.radioButton1.AutoSize = true;
			this.radioButton1.Checked = true;
			this.radioButton1.Location = new System.Drawing.Point(3, 3);
			this.radioButton1.Name = "radioButton1";
			this.radioButton1.Size = new System.Drawing.Size(87, 17);
			this.radioButton1.TabIndex = 6;
			this.radioButton1.TabStop = true;
			this.radioButton1.Text = "Synchronous";
			this.radioButton1.UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.lthroughput);
			this.groupBox2.Controls.Add(this.progressBar);
			this.groupBox2.Controls.Add(this.bStart);
			this.groupBox2.Controls.Add(this.bStop);
			this.groupBox2.Location = new System.Drawing.Point(11, 115);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(345, 88);
			this.groupBox2.TabIndex = 6;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Throughput Meter";
			// 
			// lthroughput
			// 
			this.lthroughput.AutoSize = true;
			this.lthroughput.Location = new System.Drawing.Point(6, 53);
			this.lthroughput.Name = "lthroughput";
			this.lthroughput.Size = new System.Drawing.Size(68, 13);
			this.lthroughput.TabIndex = 7;
			this.lthroughput.Text = "Throughput: ";
			// 
			// progressBar
			// 
			this.progressBar.Location = new System.Drawing.Point(6, 19);
			this.progressBar.Maximum = 30;
			this.progressBar.Name = "progressBar";
			this.progressBar.Size = new System.Drawing.Size(312, 23);
			this.progressBar.Step = 1;
			this.progressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
			this.progressBar.TabIndex = 4;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.radioButton1);
			this.panel1.Controls.Add(this.radioButton2);
			this.panel1.Location = new System.Drawing.Point(9, 45);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(157, 46);
			this.panel1.TabIndex = 10;
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.radioButton3);
			this.panel2.Controls.Add(this.radioButton4);
			this.panel2.Location = new System.Drawing.Point(172, 45);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(157, 46);
			this.panel2.TabIndex = 11;
			// 
			// mainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(368, 213);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.Name = "mainForm";
			this.Text = "QuickUSB Throughput Test";
			this.Load += new System.EventHandler(this.mainForm_Load);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
			((System.ComponentModel.ISupportInitialize)(this.packetSize)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.panel2.ResumeLayout(false);
			this.panel2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ComboBox packetSizeUnit;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button bStart;
		private System.Windows.Forms.Button bStop;
		private System.Windows.Forms.NumericUpDown packetSize;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton radioButton4;
		private System.Windows.Forms.RadioButton radioButton3;
		private System.Windows.Forms.RadioButton radioButton2;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label lthroughput;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Panel panel1;
	}
}

