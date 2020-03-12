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
            BitwiseSystems.QuickUsb quickUsb1 = new BitwiseSystems.QuickUsb();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.bStartStreaming = new System.Windows.Forms.Button();
            this.bStopStreaming = new System.Windows.Forms.Button();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.dataRate = new System.Windows.Forms.Label();
            this.qusbControl = new BitwiseSystems.QuickUsbWinForms();
            this.SuspendLayout();
            // 
            // tbLog
            // 
            this.tbLog.Location = new System.Drawing.Point(0, 83);
            this.tbLog.Margin = new System.Windows.Forms.Padding(0);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.ShortcutsEnabled = false;
            this.tbLog.Size = new System.Drawing.Size(513, 156);
            this.tbLog.TabIndex = 0;
            // 
            // bStartStreaming
            // 
            this.bStartStreaming.Location = new System.Drawing.Point(315, 242);
            this.bStartStreaming.Name = "bStartStreaming";
            this.bStartStreaming.Size = new System.Drawing.Size(96, 23);
            this.bStartStreaming.TabIndex = 1;
            this.bStartStreaming.Text = "Start Streaming";
            this.bStartStreaming.UseVisualStyleBackColor = true;
            this.bStartStreaming.Click += new System.EventHandler(this.bStartStreaming_Click);
            // 
            // bStopStreaming
            // 
            this.bStopStreaming.Enabled = false;
            this.bStopStreaming.Location = new System.Drawing.Point(417, 242);
            this.bStopStreaming.Name = "bStopStreaming";
            this.bStopStreaming.Size = new System.Drawing.Size(96, 23);
            this.bStopStreaming.TabIndex = 2;
            this.bStopStreaming.Text = "Stop Streaming";
            this.bStopStreaming.UseVisualStyleBackColor = true;
            this.bStopStreaming.Click += new System.EventHandler(this.bStopStreaming_Click);
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
            this.qusbControl.AutoScroll = true;
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
            this.qusbControl.Size = new System.Drawing.Size(513, 80);
            this.qusbControl.TabIndex = 3;
            this.qusbControl.View = System.Windows.Forms.View.Details;
            this.qusbControl.SelectionChanged += new BitwiseSystems.QuickUsbWinForms.SelectionChangedHandler(this.qusbControl_SelectionChanged);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(514, 269);
            this.Controls.Add(this.dataRate);
            this.Controls.Add(this.qusbControl);
            this.Controls.Add(this.bStopStreaming);
            this.Controls.Add(this.bStartStreaming);
            this.Controls.Add(this.tbLog);
            this.Name = "mainForm";
            this.Text = "QuickUsbBulkStreamWinCs";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.Button bStartStreaming;
        private System.Windows.Forms.Button bStopStreaming;
        private BitwiseSystems.QuickUsbWinForms qusbControl;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Label dataRate;
    }
}

