namespace QuickUsbBulkAsyncWinCs
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
            BitwiseSystems.QuickUsb quickUsb1 = new BitwiseSystems.QuickUsb();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.bIssueRead = new System.Windows.Forms.Button();
            this.bIssueWrite = new System.Windows.Forms.Button();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.dataRate = new System.Windows.Forms.Label();
            this.qusbControl = new BitwiseSystems.QuickUsbWinForms();
            this.SuspendLayout();
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(0, 83);
            this.tbLog.Margin = new System.Windows.Forms.Padding(0);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.ShortcutsEnabled = false;
            this.tbLog.Size = new System.Drawing.Size(540, 156);
            this.tbLog.TabIndex = 0;
            // 
            // bIssueRead
            // 
            this.bIssueRead.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bIssueRead.Location = new System.Drawing.Point(300, 242);
            this.bIssueRead.Name = "bIssueRead";
            this.bIssueRead.Size = new System.Drawing.Size(117, 23);
            this.bIssueRead.TabIndex = 1;
            this.bIssueRead.Text = "Issue Async Reads";
            this.bIssueRead.UseVisualStyleBackColor = true;
            this.bIssueRead.Click += new System.EventHandler(this.bIssueRead_Click);
            // 
            // bIssueWrite
            // 
            this.bIssueWrite.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bIssueWrite.Enabled = false;
            this.bIssueWrite.Location = new System.Drawing.Point(423, 242);
            this.bIssueWrite.Name = "bIssueWrite";
            this.bIssueWrite.Size = new System.Drawing.Size(117, 23);
            this.bIssueWrite.TabIndex = 2;
            this.bIssueWrite.Text = "Issue Async Writes";
            this.bIssueWrite.UseVisualStyleBackColor = true;
            this.bIssueWrite.Click += new System.EventHandler(this.bIssueWrite_Click);
            // 
            // timer
            // 
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // dataRate
            // 
            this.dataRate.AutoSize = true;
            this.dataRate.Location = new System.Drawing.Point(12, 247);
            this.dataRate.Name = "dataRate";
            this.dataRate.Size = new System.Drawing.Size(59, 13);
            this.dataRate.TabIndex = 4;
            this.dataRate.Text = "Data Rate:";
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
            quickUsb1.Name = "QUSB-0";
            this.qusbControl.SelectedModule = quickUsb1;
            this.qusbControl.SelectedModules = new BitwiseSystems.QuickUsb[] {
        quickUsb1};
            this.qusbControl.ShowUnprogrammedDevices = false;
            this.qusbControl.Size = new System.Drawing.Size(540, 80);
            this.qusbControl.TabIndex = 3;
            this.qusbControl.View = System.Windows.Forms.View.Details;
            this.qusbControl.SelectionChanged += new BitwiseSystems.QuickUsbWinForms.SelectionChangedHandler(this.qusbControl_SelectionChanged);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(541, 269);
            this.Controls.Add(this.dataRate);
            this.Controls.Add(this.qusbControl);
            this.Controls.Add(this.bIssueWrite);
            this.Controls.Add(this.bIssueRead);
            this.Controls.Add(this.tbLog);
            this.Name = "mainForm";
            this.Text = "QuickUsbBulkAsyncWinCs";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.Button bIssueRead;
        private System.Windows.Forms.Button bIssueWrite;
        private BitwiseSystems.QuickUsbWinForms qusbControl;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Label dataRate;
    }
}

