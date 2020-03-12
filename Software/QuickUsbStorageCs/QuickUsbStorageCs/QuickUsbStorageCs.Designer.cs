namespace QuickUsbStorageCs
{
	partial class QuickUsbDiagCs
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
            this.dgvEeprom = new System.Windows.Forms.DataGridView();
            this.bClearAll = new System.Windows.Forms.Button();
            this.bReadToFile = new System.Windows.Forms.Button();
            this.bWriteFromFile = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.qusbControl = new BitwiseSystems.QuickUsbWinForms();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.dgvEeprom)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // dgvEeprom
            // 
            this.dgvEeprom.AllowUserToAddRows = false;
            this.dgvEeprom.AllowUserToDeleteRows = false;
            this.dgvEeprom.AllowUserToResizeColumns = false;
            this.dgvEeprom.AllowUserToResizeRows = false;
            this.dgvEeprom.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvEeprom.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dgvEeprom.BackgroundColor = System.Drawing.SystemColors.Window;
            this.dgvEeprom.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.dgvEeprom.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvEeprom.Location = new System.Drawing.Point(6, 19);
            this.dgvEeprom.Name = "dgvEeprom";
            this.dgvEeprom.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.AutoSizeToAllHeaders;
            this.dgvEeprom.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dgvEeprom.Size = new System.Drawing.Size(445, 81);
            this.dgvEeprom.TabIndex = 1;
            this.dgvEeprom.CellValidating += new System.Windows.Forms.DataGridViewCellValidatingEventHandler(this.dgvEeprom_CellValidating);
            this.dgvEeprom.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvEeprom_CellEndEdit);
            // 
            // bClearAll
            // 
            this.bClearAll.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.bClearAll.Location = new System.Drawing.Point(189, 109);
            this.bClearAll.Name = "bClearAll";
            this.bClearAll.Size = new System.Drawing.Size(75, 23);
            this.bClearAll.TabIndex = 101;
            this.bClearAll.Text = "Clear All";
            this.bClearAll.UseVisualStyleBackColor = true;
            this.bClearAll.Click += new System.EventHandler(this.bClearAll_Click);
            // 
            // bReadToFile
            // 
            this.bReadToFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bReadToFile.Location = new System.Drawing.Point(6, 109);
            this.bReadToFile.Name = "bReadToFile";
            this.bReadToFile.Size = new System.Drawing.Size(176, 23);
            this.bReadToFile.TabIndex = 100;
            this.bReadToFile.Text = "Save EEPROM to file...";
            this.bReadToFile.UseVisualStyleBackColor = true;
            this.bReadToFile.Click += new System.EventHandler(this.bReadToFile_Click);
            // 
            // bWriteFromFile
            // 
            this.bWriteFromFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bWriteFromFile.Location = new System.Drawing.Point(271, 109);
            this.bWriteFromFile.Name = "bWriteFromFile";
            this.bWriteFromFile.Size = new System.Drawing.Size(176, 23);
            this.bWriteFromFile.TabIndex = 99;
            this.bWriteFromFile.Text = "Write file to EEPROM...";
            this.bWriteFromFile.UseVisualStyleBackColor = true;
            this.bWriteFromFile.Click += new System.EventHandler(this.bWriteFromFile_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.dgvEeprom);
            this.groupBox1.Controls.Add(this.bWriteFromFile);
            this.groupBox1.Controls.Add(this.bClearAll);
            this.groupBox1.Controls.Add(this.bReadToFile);
            this.groupBox1.Location = new System.Drawing.Point(1, 97);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(457, 137);
            this.groupBox1.TabIndex = 103;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "EEPROM Storage Navigator";
            // 
            // qusbControl
            // 
            this.qusbControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.qusbControl.AutoScroll = true;
            this.qusbControl.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.qusbControl.Location = new System.Drawing.Point(0, 0);
            this.qusbControl.MinimumSize = new System.Drawing.Size(100, 40);
            this.qusbControl.Name = "qusbControl";
            this.qusbControl.ScanEnabled = true;
            this.qusbControl.ScanInterval = 500;
            this.qusbControl.ScanOnLoad = true;
            this.qusbControl.Scrollable = true;
            this.qusbControl.Size = new System.Drawing.Size(459, 95);
            this.qusbControl.TabIndex = 104;
            this.qusbControl.View = System.Windows.Forms.View.Details;
            this.qusbControl.SelectionChanged += new BitwiseSystems.QuickUsbWinForms.SelectionChangedHandler(this.qusbControl_SelectionChanged);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 242);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(459, 22);
            this.statusStrip.TabIndex = 105;
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(0, 17);
            this.toolStripStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // statusTimer
            // 
            this.statusTimer.Enabled = true;
            this.statusTimer.Interval = 2500;
            this.statusTimer.Tick += new System.EventHandler(this.statusTimer_Tick);
            // 
            // QuickUsbDiagCs
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 264);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.qusbControl);
            this.Controls.Add(this.groupBox1);
            this.MinimumSize = new System.Drawing.Size(475, 300);
            this.Name = "QuickUsbDiagCs";
            this.Text = "QuickUsbStorageCs";
            ((System.ComponentModel.ISupportInitialize)(this.dgvEeprom)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.DataGridView dgvEeprom;
		private System.Windows.Forms.Button bClearAll;
		private System.Windows.Forms.Button bReadToFile;
		private System.Windows.Forms.Button bWriteFromFile;
		private System.Windows.Forms.GroupBox groupBox1;
		private BitwiseSystems.QuickUsbWinForms qusbControl;
		private System.Windows.Forms.StatusStrip statusStrip;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
		private System.Windows.Forms.Timer statusTimer;
	}
}

