namespace BitwiseSystems
{
	partial class QuickUsbWinForms
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(QuickUsbWinForms));
            this.scanTimer = new System.Windows.Forms.Timer(this.components);
            this.imageListSmall = new System.Windows.Forms.ImageList(this.components);
            this.deviceListView = new System.Windows.Forms.ListView();
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.smallIconsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.largeIconsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.detailToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.scrollableToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.propertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageListLarge = new System.Windows.Forms.ImageList(this.components);
            this.selectionTimer = new System.Windows.Forms.Timer(this.components);
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // scanTimer
            // 
            this.scanTimer.Enabled = true;
            this.scanTimer.Interval = 500;
            this.scanTimer.Tick += new System.EventHandler(this.scanTimer_Tick);
            // 
            // imageListSmall
            // 
            this.imageListSmall.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListSmall.ImageStream")));
            this.imageListSmall.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListSmall.Images.SetKeyName(0, "QuickUsbCtl.bmp");
            // 
            // deviceListView
            // 
            resources.ApplyResources(this.deviceListView, "deviceListView");
            this.deviceListView.AllowColumnReorder = true;
            this.deviceListView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.deviceListView.ContextMenuStrip = this.contextMenuStrip;
            this.deviceListView.FullRowSelect = true;
            this.deviceListView.GridLines = true;
            this.deviceListView.HideSelection = false;
            this.deviceListView.LargeImageList = this.imageListLarge;
            this.deviceListView.Name = "deviceListView";
            this.deviceListView.ShowItemToolTips = true;
            this.deviceListView.SmallImageList = this.imageListSmall;
            this.deviceListView.UseCompatibleStateImageBehavior = false;
            this.deviceListView.EnabledChanged += new System.EventHandler(this.deviceListView_EnabledChanged);
            this.deviceListView.SelectedIndexChanged += new System.EventHandler(this.deviceListView_SelectedIndexChanged);
            this.deviceListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.deviceListView_ColumnClick);
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewToolStripMenuItem,
            this.toolStripSeparator1,
            this.propertiesToolStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip1";
            resources.ApplyResources(this.contextMenuStrip, "contextMenuStrip");
            this.contextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip_Opening);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.smallIconsToolStripMenuItem,
            this.largeIconsToolStripMenuItem,
            this.detailToolStripMenuItem,
            this.toolStripSeparator4,
            this.scrollableToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            resources.ApplyResources(this.viewToolStripMenuItem, "viewToolStripMenuItem");
            // 
            // smallIconsToolStripMenuItem
            // 
            this.smallIconsToolStripMenuItem.Name = "smallIconsToolStripMenuItem";
            resources.ApplyResources(this.smallIconsToolStripMenuItem, "smallIconsToolStripMenuItem");
            this.smallIconsToolStripMenuItem.Click += new System.EventHandler(this.smallIconsToolStripMenuItem_Click);
            // 
            // largeIconsToolStripMenuItem
            // 
            this.largeIconsToolStripMenuItem.Name = "largeIconsToolStripMenuItem";
            resources.ApplyResources(this.largeIconsToolStripMenuItem, "largeIconsToolStripMenuItem");
            this.largeIconsToolStripMenuItem.Click += new System.EventHandler(this.largeIconsToolStripMenuItem_Click);
            // 
            // detailToolStripMenuItem
            // 
            this.detailToolStripMenuItem.Name = "detailToolStripMenuItem";
            resources.ApplyResources(this.detailToolStripMenuItem, "detailToolStripMenuItem");
            this.detailToolStripMenuItem.Click += new System.EventHandler(this.detailToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            resources.ApplyResources(this.toolStripSeparator4, "toolStripSeparator4");
            // 
            // scrollableToolStripMenuItem
            // 
            this.scrollableToolStripMenuItem.Name = "scrollableToolStripMenuItem";
            resources.ApplyResources(this.scrollableToolStripMenuItem, "scrollableToolStripMenuItem");
            this.scrollableToolStripMenuItem.Click += new System.EventHandler(this.scrollableToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            resources.ApplyResources(this.toolStripSeparator1, "toolStripSeparator1");
            // 
            // propertiesToolStripMenuItem
            // 
            this.propertiesToolStripMenuItem.Name = "propertiesToolStripMenuItem";
            resources.ApplyResources(this.propertiesToolStripMenuItem, "propertiesToolStripMenuItem");
            // 
            // imageListLarge
            // 
            this.imageListLarge.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListLarge.ImageStream")));
            this.imageListLarge.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListLarge.Images.SetKeyName(0, "QuickUsb.NET.ico");
            // 
            // selectionTimer
            // 
            this.selectionTimer.Interval = 50;
            this.selectionTimer.Tick += new System.EventHandler(this.selectionTimer_Tick);
            // 
            // QuickUsbWinForms
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.deviceListView);
            this.DoubleBuffered = true;
            this.MinimumSize = new System.Drawing.Size(100, 40);
            this.Name = "QuickUsbWinForms";
            this.Load += new System.EventHandler(this.QuickUsbWinForms_Load);
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Timer scanTimer;
		private System.Windows.Forms.ImageList imageListSmall;
		private System.Windows.Forms.ListView deviceListView;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem smallIconsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem largeIconsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem detailToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
		private System.Windows.Forms.ToolStripMenuItem scrollableToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem propertiesToolStripMenuItem;
		private System.Windows.Forms.ImageList imageListLarge;
		private System.Windows.Forms.Timer selectionTimer;
	}
}
