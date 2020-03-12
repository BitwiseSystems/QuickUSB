namespace BitwiseSystems
{
    partial class AboutBox
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutBox));
            this.logoPictureBox = new System.Windows.Forms.PictureBox();
            this.labelProductName = new System.Windows.Forms.Label();
            this.labelCopyright = new System.Windows.Forms.Label();
            this.labelDllVersion = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.labelAssemblyVersion = new System.Windows.Forms.Label();
            this.labelDriverVersion = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // logoPictureBox
            // 
            this.logoPictureBox.BackColor = System.Drawing.Color.White;
            this.logoPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.logoPictureBox.Image = ((System.Drawing.Image)(resources.GetObject("logoPictureBox.Image")));
            this.logoPictureBox.Location = new System.Drawing.Point(12, 12);
            this.logoPictureBox.Name = "logoPictureBox";
            this.logoPictureBox.Padding = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.logoPictureBox.Size = new System.Drawing.Size(300, 131);
            this.logoPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.logoPictureBox.TabIndex = 12;
            this.logoPictureBox.TabStop = false;
            // 
            // labelProductName
            // 
            this.labelProductName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelProductName.AutoSize = true;
            this.labelProductName.Location = new System.Drawing.Point(6, 3);
            this.labelProductName.Margin = new System.Windows.Forms.Padding(6, 0, 3, 0);
            this.labelProductName.MaximumSize = new System.Drawing.Size(0, 17);
            this.labelProductName.Name = "labelProductName";
            this.labelProductName.Size = new System.Drawing.Size(291, 13);
            this.labelProductName.TabIndex = 19;
            this.labelProductName.Text = "Product Name";
            this.labelProductName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelCopyright
            // 
            this.labelCopyright.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelCopyright.AutoSize = true;
            this.labelCopyright.Location = new System.Drawing.Point(6, 22);
            this.labelCopyright.Margin = new System.Windows.Forms.Padding(6, 0, 3, 0);
            this.labelCopyright.MaximumSize = new System.Drawing.Size(0, 17);
            this.labelCopyright.Name = "labelCopyright";
            this.labelCopyright.Size = new System.Drawing.Size(291, 13);
            this.labelCopyright.TabIndex = 21;
            this.labelCopyright.Text = "Copyright";
            this.labelCopyright.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelDllVersion
            // 
            this.labelDllVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelDllVersion.AutoSize = true;
            this.labelDllVersion.Location = new System.Drawing.Point(6, 60);
            this.labelDllVersion.Margin = new System.Windows.Forms.Padding(6, 0, 3, 0);
            this.labelDllVersion.MaximumSize = new System.Drawing.Size(0, 17);
            this.labelDllVersion.Name = "labelDllVersion";
            this.labelDllVersion.Size = new System.Drawing.Size(291, 13);
            this.labelDllVersion.TabIndex = 21;
            this.labelDllVersion.Text = "DLL Version";
            this.labelDllVersion.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.labelDriverVersion, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.labelProductName, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.labelDllVersion, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.labelAssemblyVersion, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.labelCopyright, 0, 1);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 155);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(300, 98);
            this.tableLayoutPanel1.TabIndex = 25;
            // 
            // labelAssemblyVersion
            // 
            this.labelAssemblyVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelAssemblyVersion.AutoSize = true;
            this.labelAssemblyVersion.Location = new System.Drawing.Point(6, 41);
            this.labelAssemblyVersion.Margin = new System.Windows.Forms.Padding(6, 0, 3, 0);
            this.labelAssemblyVersion.MaximumSize = new System.Drawing.Size(0, 17);
            this.labelAssemblyVersion.Name = "labelAssemblyVersion";
            this.labelAssemblyVersion.Size = new System.Drawing.Size(291, 13);
            this.labelAssemblyVersion.TabIndex = 21;
            this.labelAssemblyVersion.Text = "Assembly Version";
            this.labelAssemblyVersion.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelDriverVersion
            // 
            this.labelDriverVersion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.labelDriverVersion.AutoSize = true;
            this.labelDriverVersion.Location = new System.Drawing.Point(6, 80);
            this.labelDriverVersion.Margin = new System.Windows.Forms.Padding(6, 0, 3, 0);
            this.labelDriverVersion.MaximumSize = new System.Drawing.Size(0, 17);
            this.labelDriverVersion.Name = "labelDriverVersion";
            this.labelDriverVersion.Size = new System.Drawing.Size(291, 13);
            this.labelDriverVersion.TabIndex = 26;
            this.labelDriverVersion.Text = "Driver Version";
            this.labelDriverVersion.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // AboutBox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(324, 267);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.logoPictureBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutBox";
            this.Padding = new System.Windows.Forms.Padding(9);
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "AboutBox";
            ((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.PictureBox logoPictureBox;
		private System.Windows.Forms.Label labelProductName;
        private System.Windows.Forms.Label labelCopyright;
		private System.Windows.Forms.Label labelDllVersion;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label labelAssemblyVersion;
        private System.Windows.Forms.Label labelDriverVersion;
    }
}
