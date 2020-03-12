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
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbQuickUSBProjectDir = new System.Windows.Forms.TextBox();
            this.tbSetupFileDir = new System.Windows.Forms.TextBox();
            this.bBrowseSetupFileDir = new System.Windows.Forms.Button();
            this.bBrwoseQuickUSBProjectDir = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.treeView1);
            this.groupBox1.Location = new System.Drawing.Point(12, 95);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(717, 197);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Source Files";
            // 
            // treeView1
            // 
            this.treeView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeView1.Location = new System.Drawing.Point(6, 19);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(705, 143);
            this.treeView1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(636, 168);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Remove";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.Location = new System.Drawing.Point(555, 168);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Add";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button3.Location = new System.Drawing.Point(6, 168);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(94, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "Scan for links";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.bBrwoseQuickUSBProjectDir);
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
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(102, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Setup File Directory:";
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
            // tbQuickUSBProjectDir
            // 
            this.tbQuickUSBProjectDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbQuickUSBProjectDir.Location = new System.Drawing.Point(153, 45);
            this.tbQuickUSBProjectDir.Name = "tbQuickUSBProjectDir";
            this.tbQuickUSBProjectDir.Size = new System.Drawing.Size(477, 20);
            this.tbQuickUSBProjectDir.TabIndex = 2;
            // 
            // tbSetupFileDir
            // 
            this.tbSetupFileDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSetupFileDir.Location = new System.Drawing.Point(153, 19);
            this.tbSetupFileDir.Name = "tbSetupFileDir";
            this.tbSetupFileDir.Size = new System.Drawing.Size(477, 20);
            this.tbSetupFileDir.TabIndex = 3;
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
            // bBrwoseQuickUSBProjectDir
            // 
            this.bBrwoseQuickUSBProjectDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bBrwoseQuickUSBProjectDir.Location = new System.Drawing.Point(636, 43);
            this.bBrwoseQuickUSBProjectDir.Name = "bBrwoseQuickUSBProjectDir";
            this.bBrwoseQuickUSBProjectDir.Size = new System.Drawing.Size(75, 23);
            this.bBrwoseQuickUSBProjectDir.TabIndex = 5;
            this.bBrwoseQuickUSBProjectDir.Text = "Browse...";
            this.bBrwoseQuickUSBProjectDir.UseVisualStyleBackColor = true;
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog1";
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(741, 623);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "mainForm";
            this.Text = "QuickUsbFileUpdate";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button bBrwoseQuickUSBProjectDir;
        private System.Windows.Forms.Button bBrowseSetupFileDir;
        private System.Windows.Forms.TextBox tbSetupFileDir;
        private System.Windows.Forms.TextBox tbQuickUSBProjectDir;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}

