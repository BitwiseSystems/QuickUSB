namespace QuickUsbDiagCs
{
	partial class BitTwiddler
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.bitTwiddlerToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // bitTwiddlerToolTip
            // 
            this.bitTwiddlerToolTip.AutoPopDelay = 5000;
            this.bitTwiddlerToolTip.InitialDelay = 10;
            this.bitTwiddlerToolTip.ReshowDelay = 10;
            this.bitTwiddlerToolTip.ShowAlways = true;
            // 
            // BitTwiddler
            // 
            this.DoubleBuffered = true;
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "BitTwiddler";
            this.Size = new System.Drawing.Size(170, 18);
            this.Load += new System.EventHandler(this.BitTwiddler_Load);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ToolTip bitTwiddlerToolTip;
	}
}
