namespace QuickUsbCs_Demo
{
    partial class Form1
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
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.writeDataButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.readDataButton = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.label2 = new System.Windows.Forms.Label();
            this.modelTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.makeTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.serialTextBox = new System.Windows.Forms.TextBox();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(12, 28);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(99, 108);
            this.listBox1.TabIndex = 0;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // writeDataButton
            // 
            this.writeDataButton.Location = new System.Drawing.Point(320, 107);
            this.writeDataButton.Name = "writeDataButton";
            this.writeDataButton.Size = new System.Drawing.Size(129, 23);
            this.writeDataButton.TabIndex = 1;
            this.writeDataButton.Text = "QuickUsbWriteData";
            this.writeDataButton.UseVisualStyleBackColor = true;
            this.writeDataButton.Click += new System.EventHandler(this.writeDataButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Module Names";
            // 
            // readDataButton
            // 
            this.readDataButton.Location = new System.Drawing.Point(183, 107);
            this.readDataButton.Name = "readDataButton";
            this.readDataButton.Size = new System.Drawing.Size(129, 23);
            this.readDataButton.TabIndex = 3;
            this.readDataButton.Text = "QuickUsbReadData";
            this.readDataButton.UseVisualStyleBackColor = true;
            this.readDataButton.Click += new System.EventHandler(this.readDataButton_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 145);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(461, 22);
            this.statusStrip1.TabIndex = 4;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(117, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Model";
            // 
            // modelTextBox
            // 
            this.modelTextBox.Location = new System.Drawing.Point(183, 54);
            this.modelTextBox.Name = "modelTextBox";
            this.modelTextBox.Size = new System.Drawing.Size(266, 20);
            this.modelTextBox.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(117, 31);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Make";
            // 
            // makeTextBox
            // 
            this.makeTextBox.Location = new System.Drawing.Point(183, 28);
            this.makeTextBox.Name = "makeTextBox";
            this.makeTextBox.Size = new System.Drawing.Size(266, 20);
            this.makeTextBox.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(117, 84);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(33, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Serial";
            // 
            // serialTextBox
            // 
            this.serialTextBox.Location = new System.Drawing.Point(183, 81);
            this.serialTextBox.Name = "serialTextBox";
            this.serialTextBox.Size = new System.Drawing.Size(266, 20);
            this.serialTextBox.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(461, 167);
            this.Controls.Add(this.makeTextBox);
            this.Controls.Add(this.serialTextBox);
            this.Controls.Add(this.modelTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.readDataButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.writeDataButton);
            this.Controls.Add(this.listBox1);
            this.Name = "Form1";
            this.Text = "QuickUsbCs-Demo";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button writeDataButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button readDataButton;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox modelTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox makeTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox serialTextBox;

    }
}

