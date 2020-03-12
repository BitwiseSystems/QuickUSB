namespace QuickUsbXilinxProg
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
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.button1 = new System.Windows.Forms.Button();
            this.quickUsbWinForms1 = new BitwiseSystems.QuickUsbWinForms();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 242);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(440, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(324, 192);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(73, 25);
            this.button1.TabIndex = 4;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // quickUsbWinForms1
            // 
            this.quickUsbWinForms1.AutoSize = true;
            this.quickUsbWinForms1.Location = new System.Drawing.Point(25, 13);
            this.quickUsbWinForms1.Name = "quickUsbWinForms1";
            this.quickUsbWinForms1.Size = new System.Drawing.Size(387, 107);
            this.quickUsbWinForms1.TabIndex = 5;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(440, 264);
            this.Controls.Add(this.quickUsbWinForms1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.statusStrip1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.Button button1;
        private BitwiseSystems.QuickUsbWinForms quickUsbWinForms1;
    }
}

