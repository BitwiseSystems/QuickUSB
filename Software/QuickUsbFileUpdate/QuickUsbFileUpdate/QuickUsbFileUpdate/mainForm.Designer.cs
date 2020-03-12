namespace QuickUsbFileUpdate
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.bLinkDirectory = new System.Windows.Forms.Button();
            this.bAddLink = new System.Windows.Forms.Button();
            this.bRemoveLink = new System.Windows.Forms.Button();
            this.bAddSource = new System.Windows.Forms.Button();
            this.bScanForLinks = new System.Windows.Forms.Button();
            this.bRemoveSource = new System.Windows.Forms.Button();
            this.tvFiles = new System.Windows.Forms.TreeView();
            this.bSave = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.bBrowseQuickUSBProjectDir = new System.Windows.Forms.Button();
            this.bBrowseSetupFileDir = new System.Windows.Forms.Button();
            this.tbSetupFileDir = new System.Windows.Forms.TextBox();
            this.tbQuickUSBProjectDir = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.nCheckForErrors = new System.Windows.Forms.Button();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.bPerformFileUpdate = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.bLinkDirectory);
            this.groupBox1.Controls.Add(this.bAddLink);
            this.groupBox1.Controls.Add(this.bRemoveLink);
            this.groupBox1.Controls.Add(this.bAddSource);
            this.groupBox1.Controls.Add(this.bScanForLinks);
            this.groupBox1.Controls.Add(this.bRemoveSource);
            this.groupBox1.Controls.Add(this.tvFiles);
            this.groupBox1.Location = new System.Drawing.Point(12, 95);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(717, 197);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Source Files";
            // 
            // bLinkDirectory
            // 
            this.bLinkDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bLinkDirectory.Location = new System.Drawing.Point(340, 168);
            this.bLinkDirectory.Name = "bLinkDirectory";
            this.bLinkDirectory.Size = new System.Drawing.Size(104, 23);
            this.bLinkDirectory.TabIndex = 7;
            this.bLinkDirectory.Text = "Link Directories";
            this.bLinkDirectory.UseVisualStyleBackColor = true;
            this.bLinkDirectory.Click += new System.EventHandler(this.bLinkDirectory_Click);
            // 
            // bAddLink
            // 
            this.bAddLink.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bAddLink.Enabled = false;
            this.bAddLink.Location = new System.Drawing.Point(6, 168);
            this.bAddLink.Name = "bAddLink";
            this.bAddLink.Size = new System.Drawing.Size(82, 23);
            this.bAddLink.TabIndex = 5;
            this.bAddLink.Text = "Add Link";
            this.bAddLink.UseVisualStyleBackColor = true;
            this.bAddLink.Click += new System.EventHandler(this.bAddLink_Click);
            // 
            // bRemoveLink
            // 
            this.bRemoveLink.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bRemoveLink.Enabled = false;
            this.bRemoveLink.Location = new System.Drawing.Point(94, 168);
            this.bRemoveLink.Name = "bRemoveLink";
            this.bRemoveLink.Size = new System.Drawing.Size(82, 23);
            this.bRemoveLink.TabIndex = 4;
            this.bRemoveLink.Text = "Remove Link";
            this.bRemoveLink.UseVisualStyleBackColor = true;
            this.bRemoveLink.Click += new System.EventHandler(this.bRemoveLink_Click);
            // 
            // bAddSource
            // 
            this.bAddSource.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bAddSource.Location = new System.Drawing.Point(511, 168);
            this.bAddSource.Name = "bAddSource";
            this.bAddSource.Size = new System.Drawing.Size(97, 23);
            this.bAddSource.TabIndex = 2;
            this.bAddSource.Text = "Add Source";
            this.bAddSource.UseVisualStyleBackColor = true;
            this.bAddSource.Click += new System.EventHandler(this.bAddSource_Click);
            // 
            // bScanForLinks
            // 
            this.bScanForLinks.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bScanForLinks.Enabled = false;
            this.bScanForLinks.Location = new System.Drawing.Point(182, 168);
            this.bScanForLinks.Name = "bScanForLinks";
            this.bScanForLinks.Size = new System.Drawing.Size(94, 23);
            this.bScanForLinks.TabIndex = 3;
            this.bScanForLinks.Text = "Scan for links";
            this.bScanForLinks.UseVisualStyleBackColor = true;
            this.bScanForLinks.Click += new System.EventHandler(this.bScanForLinks_Click);
            // 
            // bRemoveSource
            // 
            this.bRemoveSource.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bRemoveSource.Enabled = false;
            this.bRemoveSource.Location = new System.Drawing.Point(614, 168);
            this.bRemoveSource.Name = "bRemoveSource";
            this.bRemoveSource.Size = new System.Drawing.Size(97, 23);
            this.bRemoveSource.TabIndex = 1;
            this.bRemoveSource.Text = "Remove Source";
            this.bRemoveSource.UseVisualStyleBackColor = true;
            this.bRemoveSource.Click += new System.EventHandler(this.bRemoveSource_Click);
            // 
            // tvFiles
            // 
            this.tvFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tvFiles.Location = new System.Drawing.Point(6, 19);
            this.tvFiles.Name = "tvFiles";
            this.tvFiles.Size = new System.Drawing.Size(705, 143);
            this.tvFiles.TabIndex = 0;
            this.tvFiles.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvFiles_AfterSelect);
            // 
            // bSave
            // 
            this.bSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bSave.Location = new System.Drawing.Point(636, 19);
            this.bSave.Name = "bSave";
            this.bSave.Size = new System.Drawing.Size(75, 23);
            this.bSave.TabIndex = 2;
            this.bSave.Text = "Save";
            this.bSave.UseVisualStyleBackColor = true;
            this.bSave.Click += new System.EventHandler(this.bSave_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.bBrowseQuickUSBProjectDir);
            this.groupBox2.Controls.Add(this.bBrowseSetupFileDir);
            this.groupBox2.Controls.Add(this.tbSetupFileDir);
            this.groupBox2.Controls.Add(this.tbQuickUSBProjectDir);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(717, 77);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "General";
            // 
            // bBrowseQuickUSBProjectDir
            // 
            this.bBrowseQuickUSBProjectDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bBrowseQuickUSBProjectDir.Location = new System.Drawing.Point(636, 43);
            this.bBrowseQuickUSBProjectDir.Name = "bBrowseQuickUSBProjectDir";
            this.bBrowseQuickUSBProjectDir.Size = new System.Drawing.Size(75, 23);
            this.bBrowseQuickUSBProjectDir.TabIndex = 5;
            this.bBrowseQuickUSBProjectDir.Text = "Browse...";
            this.bBrowseQuickUSBProjectDir.UseVisualStyleBackColor = true;
            this.bBrowseQuickUSBProjectDir.Click += new System.EventHandler(this.bBrowseQuickUSBProjectDir_Click);
            // 
            // bBrowseSetupFileDir
            // 
            this.bBrowseSetupFileDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bBrowseSetupFileDir.Location = new System.Drawing.Point(636, 16);
            this.bBrowseSetupFileDir.Name = "bBrowseSetupFileDir";
            this.bBrowseSetupFileDir.Size = new System.Drawing.Size(75, 23);
            this.bBrowseSetupFileDir.TabIndex = 4;
            this.bBrowseSetupFileDir.Text = "Browse...";
            this.bBrowseSetupFileDir.UseVisualStyleBackColor = true;
            this.bBrowseSetupFileDir.Click += new System.EventHandler(this.bBrowseSetupFileDir_Click);
            // 
            // tbSetupFileDir
            // 
            this.tbSetupFileDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSetupFileDir.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbFileUpdate.Properties.Settings.Default, "SetupFileDirectory", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbSetupFileDir.Location = new System.Drawing.Point(153, 19);
            this.tbSetupFileDir.Name = "tbSetupFileDir";
            this.tbSetupFileDir.Size = new System.Drawing.Size(477, 20);
            this.tbSetupFileDir.TabIndex = 3;
            this.tbSetupFileDir.Text = global::QuickUsbFileUpdate.Properties.Settings.Default.SetupFileDirectory;
            // 
            // tbQuickUSBProjectDir
            // 
            this.tbQuickUSBProjectDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbQuickUSBProjectDir.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbFileUpdate.Properties.Settings.Default, "QuickUSBProjectDirectory", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbQuickUSBProjectDir.Location = new System.Drawing.Point(153, 45);
            this.tbQuickUSBProjectDir.Name = "tbQuickUSBProjectDir";
            this.tbQuickUSBProjectDir.Size = new System.Drawing.Size(477, 20);
            this.tbQuickUSBProjectDir.TabIndex = 2;
            this.tbQuickUSBProjectDir.Text = global::QuickUsbFileUpdate.Properties.Settings.Default.QuickUSBProjectDirectory;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(141, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "QuickUSB Project Directory:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(102, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Setup File Directory:";
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog1";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.nCheckForErrors);
            this.groupBox3.Controls.Add(this.bSave);
            this.groupBox3.Controls.Add(this.tbLog);
            this.groupBox3.Controls.Add(this.bPerformFileUpdate);
            this.groupBox3.Location = new System.Drawing.Point(12, 298);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(717, 228);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Actions";
            // 
            // nCheckForErrors
            // 
            this.nCheckForErrors.Location = new System.Drawing.Point(138, 19);
            this.nCheckForErrors.Name = "nCheckForErrors";
            this.nCheckForErrors.Size = new System.Drawing.Size(117, 23);
            this.nCheckForErrors.TabIndex = 6;
            this.nCheckForErrors.Text = "Check for Errors";
            this.nCheckForErrors.UseVisualStyleBackColor = true;
            this.nCheckForErrors.Click += new System.EventHandler(this.bCheckForErrors_Click);
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(6, 48);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbLog.Size = new System.Drawing.Size(705, 174);
            this.tbLog.TabIndex = 1;
            this.tbLog.WordWrap = false;
            // 
            // bPerformFileUpdate
            // 
            this.bPerformFileUpdate.Location = new System.Drawing.Point(9, 19);
            this.bPerformFileUpdate.Name = "bPerformFileUpdate";
            this.bPerformFileUpdate.Size = new System.Drawing.Size(123, 23);
            this.bPerformFileUpdate.TabIndex = 0;
            this.bPerformFileUpdate.Text = "Perform File Update";
            this.bPerformFileUpdate.UseVisualStyleBackColor = true;
            this.bPerformFileUpdate.Click += new System.EventHandler(this.bPerformFileUpdate_Click);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(741, 539);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "mainForm";
            this.Text = "QuickUsbFileUpdate";
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TreeView tvFiles;
        private System.Windows.Forms.Button bRemoveSource;
        private System.Windows.Forms.Button bAddSource;
        private System.Windows.Forms.Button bScanForLinks;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button bBrowseQuickUSBProjectDir;
        private System.Windows.Forms.Button bBrowseSetupFileDir;
        private System.Windows.Forms.TextBox tbSetupFileDir;
        private System.Windows.Forms.TextBox tbQuickUSBProjectDir;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.Button bAddLink;
        private System.Windows.Forms.Button bRemoveLink;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button bPerformFileUpdate;
        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.Button bSave;
        private System.Windows.Forms.Button nCheckForErrors;
        private System.Windows.Forms.Button bLinkDirectory;
    }
}

