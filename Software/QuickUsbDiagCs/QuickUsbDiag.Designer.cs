namespace QuickUsbDiagCs
{
	partial class QuickUsbDiag
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(QuickUsbDiag));
            BitwiseSystems.QuickUsb quickUsb1 = new BitwiseSystems.QuickUsb();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.bDataWriteEdit = new System.Windows.Forms.Button();
            this.bDataCount = new System.Windows.Forms.Button();
            this.bDataWrite = new System.Windows.Forms.Button();
            this.bDataReadEdit = new System.Windows.Forms.Button();
            this.bDataRead = new System.Windows.Forms.Button();
            this.bWriteStreamStop = new System.Windows.Forms.Button();
            this.bWriteStreamEdit = new System.Windows.Forms.Button();
            this.bWriteStreamStart = new System.Windows.Forms.Button();
            this.bReadStreamStop = new System.Windows.Forms.Button();
            this.bReadStreamEdit = new System.Windows.Forms.Button();
            this.bReadStreamStart = new System.Windows.Forms.Button();
            this.BottomToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.TopToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.RightToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.LeftToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.ContentPanel = new System.Windows.Forms.ToolStripContentPanel();
            this.ssStatus = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.pbProgress = new System.Windows.Forms.ToolStripProgressBar();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.continuallyScanForNewremovedDevicesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.quickUSBSupportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.quickUSBFAQToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusTimer = new System.Windows.Forms.Timer(this.components);
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label27 = new System.Windows.Forms.Label();
            this.label62 = new System.Windows.Forms.Label();
            this.bSetTimeout = new System.Windows.Forms.Button();
            this.tbTimeout = new System.Windows.Forms.TextBox();
            this.nRepeatInterval = new System.Windows.Forms.NumericUpDown();
            this.cbRepeat = new System.Windows.Forms.CheckBox();
            this.tabCtrl = new System.Windows.Forms.TabControl();
            this.tabGeneral = new System.Windows.Forms.TabPage();
            this.gbGeneralFpga = new System.Windows.Forms.GroupBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.isFpgaConfiguredButton = new System.Windows.Forms.Button();
            this.browseFpgaFileButton = new System.Windows.Forms.Button();
            this.tbFpgaFile = new System.Windows.Forms.TextBox();
            this.uploadFpgaButton = new System.Windows.Forms.Button();
            this.fpgaButtonFlowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.alteraPsRadioButton = new System.Windows.Forms.RadioButton();
            this.xilinxSsRadioButton = new System.Windows.Forms.RadioButton();
            this.gbGeneralGeneral = new System.Windows.Forms.GroupBox();
            this.rbSuperSpeed = new System.Windows.Forms.RadioButton();
            this.rbHighSpeed = new System.Windows.Forms.RadioButton();
            this.rbFullSpeed = new System.Windows.Forms.RadioButton();
            this.tbHWVersion = new System.Windows.Forms.TextBox();
            this.tbDesciption = new System.Windows.Forms.TextBox();
            this.tbSerialNumber = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.tabCommand = new System.Windows.Forms.TabPage();
            this.gbCommandWrite = new System.Windows.Forms.GroupBox();
            this.tbCmdWriteAddress = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.bCmdWrite = new System.Windows.Forms.Button();
            this.tbCmdWriteBytes = new System.Windows.Forms.TextBox();
            this.cmdWriteData = new System.Windows.Forms.TextBox();
            this.gbCommandRead = new System.Windows.Forms.GroupBox();
            this.tbCmdReadAddress = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.tbCmdReadBytes = new System.Windows.Forms.TextBox();
            this.cmdReadData = new System.Windows.Forms.TextBox();
            this.bCmdRead = new System.Windows.Forms.Button();
            this.gbCommandGeneral = new System.Windows.Forms.GroupBox();
            this.cbCmdWordWide = new System.Windows.Forms.CheckBox();
            this.cbCmdUseAddr = new System.Windows.Forms.CheckBox();
            this.cbCmdIncAddr = new System.Windows.Forms.CheckBox();
            this.tabData = new System.Windows.Forms.TabPage();
            this.gbDataWrite = new System.Windows.Forms.GroupBox();
            this.cbUseFileLength = new System.Windows.Forms.CheckBox();
            this.tbDataWriteBytes = new System.Windows.Forms.TextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.tbInputFileName = new System.Windows.Forms.TextBox();
            this.bBrowseWrite = new System.Windows.Forms.Button();
            this.tbDataWriteAddress = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.gbDataRead = new System.Windows.Forms.GroupBox();
            this.tbDataReadBytes = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.tbOutputFileName = new System.Windows.Forms.TextBox();
            this.dataReadBrowse = new System.Windows.Forms.Button();
            this.tbDataReadAddress = new System.Windows.Forms.TextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.gbDataGeneral = new System.Windows.Forms.GroupBox();
            this.cbDataOutOfOrder = new System.Windows.Forms.CheckBox();
            this.cbDataIncAddr = new System.Windows.Forms.CheckBox();
            this.cbDataUseAddr = new System.Windows.Forms.CheckBox();
            this.cbDataWordwide = new System.Windows.Forms.CheckBox();
            this.tabStreaming = new System.Windows.Forms.TabPage();
            this.gbStreamingStatistics = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.labelWriteStreamCount = new System.Windows.Forms.Label();
            this.labelWriteStreamRate = new System.Windows.Forms.Label();
            this.labelReadStreamCount = new System.Windows.Forms.Label();
            this.labelReadStreamRate = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.gbStreamingWrite = new System.Windows.Forms.GroupBox();
            this.panel4 = new System.Windows.Forms.Panel();
            this.rbWriteStreamFromDisk = new System.Windows.Forms.RadioButton();
            this.rbWriteStreamFromPattern = new System.Windows.Forms.RadioButton();
            this.tbWriteStreamFileName = new System.Windows.Forms.TextBox();
            this.bWriteStreamBrowse = new System.Windows.Forms.Button();
            this.lWriteStreamFileName = new System.Windows.Forms.Label();
            this.gbStreamingGeneral = new System.Windows.Forms.GroupBox();
            this.tbStreamThreadConcurrency = new System.Windows.Forms.TextBox();
            this.label70 = new System.Windows.Forms.Label();
            this.tbStreamNumThreads = new System.Windows.Forms.TextBox();
            this.label69 = new System.Windows.Forms.Label();
            this.tbStreamNumBuffers = new System.Windows.Forms.TextBox();
            this.label68 = new System.Windows.Forms.Label();
            this.tbStreamBufferSize = new System.Windows.Forms.TextBox();
            this.label67 = new System.Windows.Forms.Label();
            this.cbStreamingIncAddr = new System.Windows.Forms.CheckBox();
            this.cbStreamingUseAddr = new System.Windows.Forms.CheckBox();
            this.cbStreamingWordwide = new System.Windows.Forms.CheckBox();
            this.gbStreamingRead = new System.Windows.Forms.GroupBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.rbReadStreamToDisk = new System.Windows.Forms.RadioButton();
            this.rbReadStreamDisgard = new System.Windows.Forms.RadioButton();
            this.tbReadStreamFileName = new System.Windows.Forms.TextBox();
            this.bReadStreamBrowse = new System.Windows.Forms.Button();
            this.lReadStreamFileName = new System.Windows.Forms.Label();
            this.tabPorts = new System.Windows.Forms.TabPage();
            this.gbPortE = new System.Windows.Forms.GroupBox();
            this.bReadPortE = new System.Windows.Forms.Button();
            this.gbPortD = new System.Windows.Forms.GroupBox();
            this.bReadPortD = new System.Windows.Forms.Button();
            this.gbPortC = new System.Windows.Forms.GroupBox();
            this.bReadPortC = new System.Windows.Forms.Button();
            this.gbPortB = new System.Windows.Forms.GroupBox();
            this.bReadPortB = new System.Windows.Forms.Button();
            this.gbPortA = new System.Windows.Forms.GroupBox();
            this.bCountPortA = new System.Windows.Forms.Button();
            this.bReadPortA = new System.Windows.Forms.Button();
            this.label29 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.tabRS232 = new System.Windows.Forms.TabPage();
            this.gbRS232Write = new System.Windows.Forms.GroupBox();
            this.rs232WriteData = new System.Windows.Forms.TextBox();
            this.rs232Write = new System.Windows.Forms.Button();
            this.gbRS232Read = new System.Windows.Forms.GroupBox();
            this.label30 = new System.Windows.Forms.Label();
            this.tbRS232Chars = new System.Windows.Forms.TextBox();
            this.rs232Get = new System.Windows.Forms.Button();
            this.tbRS232ReadData = new System.Windows.Forms.TextBox();
            this.rs232Read = new System.Windows.Forms.Button();
            this.gbRS232General = new System.Windows.Forms.GroupBox();
            this.cbRS232Port = new System.Windows.Forms.ComboBox();
            this.label32 = new System.Windows.Forms.Label();
            this.cbRS232Baud = new System.Windows.Forms.ComboBox();
            this.label31 = new System.Windows.Forms.Label();
            this.bRS232Flush = new System.Windows.Forms.Button();
            this.tabSPI = new System.Windows.Forms.TabPage();
            this.gbSPIWrite = new System.Windows.Forms.GroupBox();
            this.bWriteReadSpi = new System.Windows.Forms.Button();
            this.cbSpiWritePort = new System.Windows.Forms.ComboBox();
            this.label35 = new System.Windows.Forms.Label();
            this.tbSpiWriteData = new System.Windows.Forms.TextBox();
            this.spiWrite = new System.Windows.Forms.Button();
            this.gbSPIRead = new System.Windows.Forms.GroupBox();
            this.cbSpiReadPort = new System.Windows.Forms.ComboBox();
            this.label34 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.tbSpiBytes = new System.Windows.Forms.TextBox();
            this.spiRead = new System.Windows.Forms.Button();
            this.tbSpiReadData = new System.Windows.Forms.TextBox();
            this.gbSPIGeneral = new System.Windows.Forms.GroupBox();
            this.cbSpiPORT = new System.Windows.Forms.CheckBox();
            this.cbSpiNCEPIN = new System.Windows.Forms.CheckBox();
            this.cbSpiMISOPIN = new System.Windows.Forms.CheckBox();
            this.cbSpiEndian = new System.Windows.Forms.CheckBox();
            this.cbSpiCPOL = new System.Windows.Forms.CheckBox();
            this.cbSpiCPHA = new System.Windows.Forms.CheckBox();
            this.tabEPCS = new System.Windows.Forms.TabPage();
            this.gbEpcsWriting = new System.Windows.Forms.GroupBox();
            this.bEpcsErase = new System.Windows.Forms.Button();
            this.bEpcsVerify = new System.Windows.Forms.Button();
            this.tbEpcsFilePath = new System.Windows.Forms.TextBox();
            this.bEpcsBrowse = new System.Windows.Forms.Button();
            this.label60 = new System.Windows.Forms.Label();
            this.bEpcsWrite = new System.Windows.Forms.Button();
            this.gbEpcsGeneral = new System.Windows.Forms.GroupBox();
            this.labelEpcsIdentify = new System.Windows.Forms.Label();
            this.bEpcsIdentify = new System.Windows.Forms.Button();
            this.cbEpcsPort = new System.Windows.Forms.ComboBox();
            this.label61 = new System.Windows.Forms.Label();
            this.tabI2C = new System.Windows.Forms.TabPage();
            this.gbI2CWrite = new System.Windows.Forms.GroupBox();
            this.bI2cCachedWrite = new System.Windows.Forms.Button();
            this.label39 = new System.Windows.Forms.Label();
            this.tbI2cWriteAddress = new System.Windows.Forms.TextBox();
            this.tbI2cWriteData = new System.Windows.Forms.TextBox();
            this.i2cWrite = new System.Windows.Forms.Button();
            this.gbI2CRead = new System.Windows.Forms.GroupBox();
            this.tbI2cBytes = new System.Windows.Forms.TextBox();
            this.label38 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.tbI2cReadAddress = new System.Windows.Forms.TextBox();
            this.i2cRead = new System.Windows.Forms.Button();
            this.tbI2cReadData = new System.Windows.Forms.TextBox();
            this.gbI2CGeneral = new System.Windows.Forms.GroupBox();
            this.cbI2cIgnoreAck = new System.Windows.Forms.CheckBox();
            this.cbI2cBusSpeed = new System.Windows.Forms.ComboBox();
            this.label37 = new System.Windows.Forms.Label();
            this.tabSettings = new System.Windows.Forms.TabPage();
            this.gbSettings = new System.Windows.Forms.GroupBox();
            this.labelSetting8_2 = new System.Windows.Forms.Label();
            this.labelSetting7_2 = new System.Windows.Forms.Label();
            this.labelSetting0_2 = new System.Windows.Forms.Label();
            this.labelSetting15_2 = new System.Windows.Forms.Label();
            this.labelSettingLSB = new System.Windows.Forms.Label();
            this.labelSettingMSB = new System.Windows.Forms.Label();
            this.labelSetting8_1 = new System.Windows.Forms.Label();
            this.labelSetting7_1 = new System.Windows.Forms.Label();
            this.labelSetting0_1 = new System.Windows.Forms.Label();
            this.labelSetting15_1 = new System.Windows.Forms.Label();
            this.tabDefaults = new System.Windows.Forms.TabPage();
            this.gbDefaults = new System.Windows.Forms.GroupBox();
            this.labelDefault8_2 = new System.Windows.Forms.Label();
            this.labelDefault7_2 = new System.Windows.Forms.Label();
            this.labelDefault0_2 = new System.Windows.Forms.Label();
            this.labelDefault15_2 = new System.Windows.Forms.Label();
            this.labelDefaultLSB = new System.Windows.Forms.Label();
            this.labelDefaultMSB = new System.Windows.Forms.Label();
            this.labelDefault8_1 = new System.Windows.Forms.Label();
            this.labelDefault7_1 = new System.Windows.Forms.Label();
            this.labelDefault0_1 = new System.Windows.Forms.Label();
            this.labelDefault15_1 = new System.Windows.Forms.Label();
            this.groupBox24 = new System.Windows.Forms.GroupBox();
            this.label40 = new System.Windows.Forms.Label();
            this.label41 = new System.Windows.Forms.Label();
            this.label42 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label45 = new System.Windows.Forms.Label();
            this.label46 = new System.Windows.Forms.Label();
            this.label47 = new System.Windows.Forms.Label();
            this.label48 = new System.Windows.Forms.Label();
            this.label49 = new System.Windows.Forms.Label();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.timerRepeat = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lLabelDeviceSpeed = new System.Windows.Forms.Label();
            this.quickUsbWinForms = new BitwiseSystems.QuickUsbWinForms();
            this.btPortEDir = new QuickUsbDiagCs.BitTwiddler();
            this.btPortEVal = new QuickUsbDiagCs.BitTwiddler();
            this.btPortDDir = new QuickUsbDiagCs.BitTwiddler();
            this.btPortDVal = new QuickUsbDiagCs.BitTwiddler();
            this.btPortCDir = new QuickUsbDiagCs.BitTwiddler();
            this.btPortCVal = new QuickUsbDiagCs.BitTwiddler();
            this.btPortBDir = new QuickUsbDiagCs.BitTwiddler();
            this.btPortBVal = new QuickUsbDiagCs.BitTwiddler();
            this.btPortADir = new QuickUsbDiagCs.BitTwiddler();
            this.btPortAVal = new QuickUsbDiagCs.BitTwiddler();
            this.epcfgBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.wordWideBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.dataAddressBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.fpgaTypeBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.cpuConfigBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.spiConfigBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.slaveFifoBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.i2ctlBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.ioaBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.iobBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.iocBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.ifconfigBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.iodBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.pinFlagsBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.ioeBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.portAcfgBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.epcfgDBitTwiddler = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler26 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler27 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler28 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler29 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler30 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler31 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler32 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler33 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler34 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler35 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler36 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler37 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler38 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler39 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler40 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler9 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler10 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler11 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler12 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler13 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler14 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler15 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler16 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler17 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler18 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler19 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler20 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler21 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler22 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler23 = new QuickUsbDiagCs.BitTwiddler();
            this.bitTwiddler24 = new QuickUsbDiagCs.BitTwiddler();
            this.ssStatus.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nRepeatInterval)).BeginInit();
            this.tabCtrl.SuspendLayout();
            this.tabGeneral.SuspendLayout();
            this.gbGeneralFpga.SuspendLayout();
            this.fpgaButtonFlowLayoutPanel.SuspendLayout();
            this.gbGeneralGeneral.SuspendLayout();
            this.tabCommand.SuspendLayout();
            this.gbCommandWrite.SuspendLayout();
            this.gbCommandRead.SuspendLayout();
            this.gbCommandGeneral.SuspendLayout();
            this.tabData.SuspendLayout();
            this.gbDataWrite.SuspendLayout();
            this.gbDataRead.SuspendLayout();
            this.gbDataGeneral.SuspendLayout();
            this.tabStreaming.SuspendLayout();
            this.gbStreamingStatistics.SuspendLayout();
            this.gbStreamingWrite.SuspendLayout();
            this.panel4.SuspendLayout();
            this.gbStreamingGeneral.SuspendLayout();
            this.gbStreamingRead.SuspendLayout();
            this.panel3.SuspendLayout();
            this.tabPorts.SuspendLayout();
            this.gbPortE.SuspendLayout();
            this.gbPortD.SuspendLayout();
            this.gbPortC.SuspendLayout();
            this.gbPortB.SuspendLayout();
            this.gbPortA.SuspendLayout();
            this.tabRS232.SuspendLayout();
            this.gbRS232Write.SuspendLayout();
            this.gbRS232Read.SuspendLayout();
            this.gbRS232General.SuspendLayout();
            this.tabSPI.SuspendLayout();
            this.gbSPIWrite.SuspendLayout();
            this.gbSPIRead.SuspendLayout();
            this.gbSPIGeneral.SuspendLayout();
            this.tabEPCS.SuspendLayout();
            this.gbEpcsWriting.SuspendLayout();
            this.gbEpcsGeneral.SuspendLayout();
            this.tabI2C.SuspendLayout();
            this.gbI2CWrite.SuspendLayout();
            this.gbI2CRead.SuspendLayout();
            this.gbI2CGeneral.SuspendLayout();
            this.tabSettings.SuspendLayout();
            this.gbSettings.SuspendLayout();
            this.tabDefaults.SuspendLayout();
            this.gbDefaults.SuspendLayout();
            this.groupBox24.SuspendLayout();
            this.SuspendLayout();
            // 
            // bDataWriteEdit
            // 
            resources.ApplyResources(this.bDataWriteEdit, "bDataWriteEdit");
            this.bDataWriteEdit.Name = "bDataWriteEdit";
            this.toolTip.SetToolTip(this.bDataWriteEdit, resources.GetString("bDataWriteEdit.ToolTip"));
            this.bDataWriteEdit.UseVisualStyleBackColor = true;
            this.bDataWriteEdit.Click += new System.EventHandler(this.bDataWriteEdit_Click);
            // 
            // bDataCount
            // 
            resources.ApplyResources(this.bDataCount, "bDataCount");
            this.bDataCount.Name = "bDataCount";
            this.toolTip.SetToolTip(this.bDataCount, resources.GetString("bDataCount.ToolTip"));
            this.bDataCount.UseVisualStyleBackColor = true;
            this.bDataCount.Click += new System.EventHandler(this.bDataCount_Click);
            // 
            // bDataWrite
            // 
            resources.ApplyResources(this.bDataWrite, "bDataWrite");
            this.bDataWrite.Name = "bDataWrite";
            this.toolTip.SetToolTip(this.bDataWrite, resources.GetString("bDataWrite.ToolTip"));
            this.bDataWrite.UseVisualStyleBackColor = true;
            this.bDataWrite.Click += new System.EventHandler(this.bDataWrite_Click);
            // 
            // bDataReadEdit
            // 
            resources.ApplyResources(this.bDataReadEdit, "bDataReadEdit");
            this.bDataReadEdit.Name = "bDataReadEdit";
            this.toolTip.SetToolTip(this.bDataReadEdit, resources.GetString("bDataReadEdit.ToolTip"));
            this.bDataReadEdit.UseVisualStyleBackColor = true;
            this.bDataReadEdit.Click += new System.EventHandler(this.bDataReadEdit_Click);
            // 
            // bDataRead
            // 
            resources.ApplyResources(this.bDataRead, "bDataRead");
            this.bDataRead.Name = "bDataRead";
            this.toolTip.SetToolTip(this.bDataRead, resources.GetString("bDataRead.ToolTip"));
            this.bDataRead.UseVisualStyleBackColor = true;
            this.bDataRead.Click += new System.EventHandler(this.bDataRead_Click);
            // 
            // bWriteStreamStop
            // 
            resources.ApplyResources(this.bWriteStreamStop, "bWriteStreamStop");
            this.bWriteStreamStop.Name = "bWriteStreamStop";
            this.toolTip.SetToolTip(this.bWriteStreamStop, resources.GetString("bWriteStreamStop.ToolTip"));
            this.bWriteStreamStop.UseVisualStyleBackColor = true;
            this.bWriteStreamStop.Click += new System.EventHandler(this.bWriteStreamStop_Click);
            // 
            // bWriteStreamEdit
            // 
            resources.ApplyResources(this.bWriteStreamEdit, "bWriteStreamEdit");
            this.bWriteStreamEdit.Name = "bWriteStreamEdit";
            this.toolTip.SetToolTip(this.bWriteStreamEdit, resources.GetString("bWriteStreamEdit.ToolTip"));
            this.bWriteStreamEdit.UseVisualStyleBackColor = true;
            this.bWriteStreamEdit.Click += new System.EventHandler(this.bWriteStreamEdit_Click);
            // 
            // bWriteStreamStart
            // 
            resources.ApplyResources(this.bWriteStreamStart, "bWriteStreamStart");
            this.bWriteStreamStart.Name = "bWriteStreamStart";
            this.toolTip.SetToolTip(this.bWriteStreamStart, resources.GetString("bWriteStreamStart.ToolTip"));
            this.bWriteStreamStart.UseVisualStyleBackColor = true;
            this.bWriteStreamStart.Click += new System.EventHandler(this.bWriteStreamStart_Click);
            // 
            // bReadStreamStop
            // 
            resources.ApplyResources(this.bReadStreamStop, "bReadStreamStop");
            this.bReadStreamStop.Name = "bReadStreamStop";
            this.toolTip.SetToolTip(this.bReadStreamStop, resources.GetString("bReadStreamStop.ToolTip"));
            this.bReadStreamStop.UseVisualStyleBackColor = true;
            this.bReadStreamStop.Click += new System.EventHandler(this.bReadStreamStop_Click);
            // 
            // bReadStreamEdit
            // 
            resources.ApplyResources(this.bReadStreamEdit, "bReadStreamEdit");
            this.bReadStreamEdit.Name = "bReadStreamEdit";
            this.toolTip.SetToolTip(this.bReadStreamEdit, resources.GetString("bReadStreamEdit.ToolTip"));
            this.bReadStreamEdit.UseVisualStyleBackColor = true;
            this.bReadStreamEdit.Click += new System.EventHandler(this.bReadStreamEdit_Click);
            // 
            // bReadStreamStart
            // 
            resources.ApplyResources(this.bReadStreamStart, "bReadStreamStart");
            this.bReadStreamStart.Name = "bReadStreamStart";
            this.toolTip.SetToolTip(this.bReadStreamStart, resources.GetString("bReadStreamStart.ToolTip"));
            this.bReadStreamStart.UseVisualStyleBackColor = true;
            this.bReadStreamStart.Click += new System.EventHandler(this.bReadStreamStart_Click);
            // 
            // BottomToolStripPanel
            // 
            resources.ApplyResources(this.BottomToolStripPanel, "BottomToolStripPanel");
            this.BottomToolStripPanel.Name = "BottomToolStripPanel";
            this.BottomToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.BottomToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            // 
            // TopToolStripPanel
            // 
            resources.ApplyResources(this.TopToolStripPanel, "TopToolStripPanel");
            this.TopToolStripPanel.Name = "TopToolStripPanel";
            this.TopToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.TopToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            // 
            // RightToolStripPanel
            // 
            resources.ApplyResources(this.RightToolStripPanel, "RightToolStripPanel");
            this.RightToolStripPanel.Name = "RightToolStripPanel";
            this.RightToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.RightToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            // 
            // LeftToolStripPanel
            // 
            resources.ApplyResources(this.LeftToolStripPanel, "LeftToolStripPanel");
            this.LeftToolStripPanel.Name = "LeftToolStripPanel";
            this.LeftToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.LeftToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            // 
            // ContentPanel
            // 
            resources.ApplyResources(this.ContentPanel, "ContentPanel");
            // 
            // ssStatus
            // 
            this.ssStatus.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel,
            this.pbProgress});
            resources.ApplyResources(this.ssStatus, "ssStatus");
            this.ssStatus.Name = "ssStatus";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            resources.ApplyResources(this.toolStripStatusLabel, "toolStripStatusLabel");
            this.toolStripStatusLabel.Spring = true;
            // 
            // pbProgress
            // 
            this.pbProgress.Name = "pbProgress";
            resources.ApplyResources(this.pbProgress, "pbProgress");
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.helpMenu});
            resources.ApplyResources(this.menuStrip, "menuStrip");
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            resources.ApplyResources(this.fileToolStripMenuItem, "fileToolStripMenuItem");
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            resources.ApplyResources(this.exitToolStripMenuItem, "exitToolStripMenuItem");
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.continuallyScanForNewremovedDevicesToolStripMenuItem});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            resources.ApplyResources(this.optionsToolStripMenuItem, "optionsToolStripMenuItem");
            // 
            // continuallyScanForNewremovedDevicesToolStripMenuItem
            // 
            this.continuallyScanForNewremovedDevicesToolStripMenuItem.Checked = true;
            this.continuallyScanForNewremovedDevicesToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.continuallyScanForNewremovedDevicesToolStripMenuItem.Name = "continuallyScanForNewremovedDevicesToolStripMenuItem";
            resources.ApplyResources(this.continuallyScanForNewremovedDevicesToolStripMenuItem, "continuallyScanForNewremovedDevicesToolStripMenuItem");
            this.continuallyScanForNewremovedDevicesToolStripMenuItem.Click += new System.EventHandler(this.continuallyScanForNewremovedDevicesToolStripMenuItem_Click);
            // 
            // helpMenu
            // 
            this.helpMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.quickUSBSupportToolStripMenuItem,
            this.quickUSBFAQToolStripMenuItem,
            this.toolStripSeparator8,
            this.aboutToolStripMenuItem});
            this.helpMenu.Name = "helpMenu";
            resources.ApplyResources(this.helpMenu, "helpMenu");
            // 
            // quickUSBSupportToolStripMenuItem
            // 
            this.quickUSBSupportToolStripMenuItem.Name = "quickUSBSupportToolStripMenuItem";
            resources.ApplyResources(this.quickUSBSupportToolStripMenuItem, "quickUSBSupportToolStripMenuItem");
            this.quickUSBSupportToolStripMenuItem.Click += new System.EventHandler(this.quickUSBSupportToolStripMenuItem_Click);
            // 
            // quickUSBFAQToolStripMenuItem
            // 
            this.quickUSBFAQToolStripMenuItem.Name = "quickUSBFAQToolStripMenuItem";
            resources.ApplyResources(this.quickUSBFAQToolStripMenuItem, "quickUSBFAQToolStripMenuItem");
            this.quickUSBFAQToolStripMenuItem.Click += new System.EventHandler(this.quickUSBFAQToolStripMenuItem_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            resources.ApplyResources(this.toolStripSeparator8, "toolStripSeparator8");
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            resources.ApplyResources(this.aboutToolStripMenuItem, "aboutToolStripMenuItem");
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // statusTimer
            // 
            this.statusTimer.Enabled = true;
            this.statusTimer.Tick += new System.EventHandler(this.statusTimer_Tick);
            // 
            // splitContainer
            // 
            resources.ApplyResources(this.splitContainer, "splitContainer");
            this.splitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer.Name = "splitContainer";
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.quickUsbWinForms);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.panel1);
            this.splitContainer.Panel2.Controls.Add(this.tabCtrl);
            // 
            // panel1
            // 
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Controls.Add(this.label27);
            this.panel1.Controls.Add(this.label62);
            this.panel1.Controls.Add(this.bSetTimeout);
            this.panel1.Controls.Add(this.tbTimeout);
            this.panel1.Controls.Add(this.nRepeatInterval);
            this.panel1.Controls.Add(this.cbRepeat);
            this.panel1.Name = "panel1";
            // 
            // label27
            // 
            resources.ApplyResources(this.label27, "label27");
            this.label27.Name = "label27";
            // 
            // label62
            // 
            resources.ApplyResources(this.label62, "label62");
            this.label62.Name = "label62";
            // 
            // bSetTimeout
            // 
            resources.ApplyResources(this.bSetTimeout, "bSetTimeout");
            this.bSetTimeout.Name = "bSetTimeout";
            this.bSetTimeout.UseVisualStyleBackColor = true;
            this.bSetTimeout.Click += new System.EventHandler(this.bSetTimeout_Click);
            // 
            // tbTimeout
            // 
            resources.ApplyResources(this.tbTimeout, "tbTimeout");
            this.tbTimeout.Name = "tbTimeout";
            // 
            // nRepeatInterval
            // 
            this.nRepeatInterval.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::QuickUsbDiagCs.Properties.Settings.Default, "RepeatInterval", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.nRepeatInterval, "nRepeatInterval");
            this.nRepeatInterval.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.nRepeatInterval.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nRepeatInterval.Name = "nRepeatInterval";
            this.nRepeatInterval.Value = global::QuickUsbDiagCs.Properties.Settings.Default.RepeatInterval;
            // 
            // cbRepeat
            // 
            resources.ApplyResources(this.cbRepeat, "cbRepeat");
            this.cbRepeat.Name = "cbRepeat";
            this.cbRepeat.UseVisualStyleBackColor = true;
            this.cbRepeat.CheckedChanged += new System.EventHandler(this.cbRepeat_CheckedChanged);
            // 
            // tabCtrl
            // 
            resources.ApplyResources(this.tabCtrl, "tabCtrl");
            this.tabCtrl.Controls.Add(this.tabGeneral);
            this.tabCtrl.Controls.Add(this.tabCommand);
            this.tabCtrl.Controls.Add(this.tabData);
            this.tabCtrl.Controls.Add(this.tabStreaming);
            this.tabCtrl.Controls.Add(this.tabPorts);
            this.tabCtrl.Controls.Add(this.tabRS232);
            this.tabCtrl.Controls.Add(this.tabSPI);
            this.tabCtrl.Controls.Add(this.tabEPCS);
            this.tabCtrl.Controls.Add(this.tabI2C);
            this.tabCtrl.Controls.Add(this.tabSettings);
            this.tabCtrl.Controls.Add(this.tabDefaults);
            this.tabCtrl.DataBindings.Add(new System.Windows.Forms.Binding("SelectedIndex", global::QuickUsbDiagCs.Properties.Settings.Default, "SelectedTabIndex", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tabCtrl.Name = "tabCtrl";
            this.tabCtrl.SelectedIndex = global::QuickUsbDiagCs.Properties.Settings.Default.SelectedTabIndex;
            this.tabCtrl.SelectedIndexChanged += new System.EventHandler(this.tabCtrl_SelectedIndexChanged);
            // 
            // tabGeneral
            // 
            this.tabGeneral.Controls.Add(this.gbGeneralFpga);
            this.tabGeneral.Controls.Add(this.gbGeneralGeneral);
            resources.ApplyResources(this.tabGeneral, "tabGeneral");
            this.tabGeneral.Name = "tabGeneral";
            this.tabGeneral.UseVisualStyleBackColor = true;
            // 
            // gbGeneralFpga
            // 
            resources.ApplyResources(this.gbGeneralFpga, "gbGeneralFpga");
            this.gbGeneralFpga.Controls.Add(this.label15);
            this.gbGeneralFpga.Controls.Add(this.label16);
            this.gbGeneralFpga.Controls.Add(this.isFpgaConfiguredButton);
            this.gbGeneralFpga.Controls.Add(this.browseFpgaFileButton);
            this.gbGeneralFpga.Controls.Add(this.tbFpgaFile);
            this.gbGeneralFpga.Controls.Add(this.uploadFpgaButton);
            this.gbGeneralFpga.Controls.Add(this.fpgaButtonFlowLayoutPanel);
            this.gbGeneralFpga.Name = "gbGeneralFpga";
            this.gbGeneralFpga.TabStop = false;
            // 
            // label15
            // 
            resources.ApplyResources(this.label15, "label15");
            this.label15.Name = "label15";
            // 
            // label16
            // 
            resources.ApplyResources(this.label16, "label16");
            this.label16.Name = "label16";
            // 
            // isFpgaConfiguredButton
            // 
            resources.ApplyResources(this.isFpgaConfiguredButton, "isFpgaConfiguredButton");
            this.isFpgaConfiguredButton.Name = "isFpgaConfiguredButton";
            this.isFpgaConfiguredButton.UseVisualStyleBackColor = true;
            this.isFpgaConfiguredButton.Click += new System.EventHandler(this.isFpgaConfiguredButton_Click);
            // 
            // browseFpgaFileButton
            // 
            resources.ApplyResources(this.browseFpgaFileButton, "browseFpgaFileButton");
            this.browseFpgaFileButton.Name = "browseFpgaFileButton";
            this.browseFpgaFileButton.UseVisualStyleBackColor = true;
            this.browseFpgaFileButton.Click += new System.EventHandler(this.browseFpgaFileButton_Click);
            // 
            // tbFpgaFile
            // 
            resources.ApplyResources(this.tbFpgaFile, "tbFpgaFile");
            this.tbFpgaFile.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "FpgaFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbFpgaFile.Name = "tbFpgaFile";
            this.tbFpgaFile.Text = global::QuickUsbDiagCs.Properties.Settings.Default.FpgaFilePath;
            this.tbFpgaFile.TextChanged += new System.EventHandler(this.fpgaFileTextBox_TextChanged);
            // 
            // uploadFpgaButton
            // 
            resources.ApplyResources(this.uploadFpgaButton, "uploadFpgaButton");
            this.uploadFpgaButton.Name = "uploadFpgaButton";
            this.uploadFpgaButton.UseVisualStyleBackColor = true;
            this.uploadFpgaButton.Click += new System.EventHandler(this.uploadFpgaButton_Click);
            // 
            // fpgaButtonFlowLayoutPanel
            // 
            this.fpgaButtonFlowLayoutPanel.Controls.Add(this.alteraPsRadioButton);
            this.fpgaButtonFlowLayoutPanel.Controls.Add(this.xilinxSsRadioButton);
            resources.ApplyResources(this.fpgaButtonFlowLayoutPanel, "fpgaButtonFlowLayoutPanel");
            this.fpgaButtonFlowLayoutPanel.Name = "fpgaButtonFlowLayoutPanel";
            // 
            // alteraPsRadioButton
            // 
            resources.ApplyResources(this.alteraPsRadioButton, "alteraPsRadioButton");
            this.alteraPsRadioButton.Checked = true;
            this.alteraPsRadioButton.Name = "alteraPsRadioButton";
            this.alteraPsRadioButton.TabStop = true;
            this.alteraPsRadioButton.UseVisualStyleBackColor = true;
            // 
            // xilinxSsRadioButton
            // 
            resources.ApplyResources(this.xilinxSsRadioButton, "xilinxSsRadioButton");
            this.xilinxSsRadioButton.Name = "xilinxSsRadioButton";
            this.xilinxSsRadioButton.UseVisualStyleBackColor = true;
            // 
            // gbGeneralGeneral
            // 
            resources.ApplyResources(this.gbGeneralGeneral, "gbGeneralGeneral");
            this.gbGeneralGeneral.Controls.Add(this.lLabelDeviceSpeed);
            this.gbGeneralGeneral.Controls.Add(this.label6);
            this.gbGeneralGeneral.Controls.Add(this.label1);
            this.gbGeneralGeneral.Controls.Add(this.rbSuperSpeed);
            this.gbGeneralGeneral.Controls.Add(this.rbHighSpeed);
            this.gbGeneralGeneral.Controls.Add(this.rbFullSpeed);
            this.gbGeneralGeneral.Controls.Add(this.tbHWVersion);
            this.gbGeneralGeneral.Controls.Add(this.tbDesciption);
            this.gbGeneralGeneral.Controls.Add(this.tbSerialNumber);
            this.gbGeneralGeneral.Controls.Add(this.label14);
            this.gbGeneralGeneral.Controls.Add(this.label13);
            this.gbGeneralGeneral.Controls.Add(this.label8);
            this.gbGeneralGeneral.Controls.Add(this.label7);
            this.gbGeneralGeneral.Name = "gbGeneralGeneral";
            this.gbGeneralGeneral.TabStop = false;
            // 
            // rbSuperSpeed
            // 
            resources.ApplyResources(this.rbSuperSpeed, "rbSuperSpeed");
            this.rbSuperSpeed.Name = "rbSuperSpeed";
            this.rbSuperSpeed.TabStop = true;
            this.rbSuperSpeed.UseVisualStyleBackColor = true;
            this.rbSuperSpeed.CheckedChanged += new System.EventHandler(this.rbUsbSpeed_CheckedChanged);
            // 
            // rbHighSpeed
            // 
            resources.ApplyResources(this.rbHighSpeed, "rbHighSpeed");
            this.rbHighSpeed.Name = "rbHighSpeed";
            this.rbHighSpeed.TabStop = true;
            this.rbHighSpeed.UseVisualStyleBackColor = true;
            this.rbHighSpeed.CheckedChanged += new System.EventHandler(this.rbUsbSpeed_CheckedChanged);
            // 
            // rbFullSpeed
            // 
            resources.ApplyResources(this.rbFullSpeed, "rbFullSpeed");
            this.rbFullSpeed.Name = "rbFullSpeed";
            this.rbFullSpeed.TabStop = true;
            this.rbFullSpeed.UseVisualStyleBackColor = true;
            this.rbFullSpeed.CheckedChanged += new System.EventHandler(this.rbUsbSpeed_CheckedChanged);
            // 
            // tbHWVersion
            // 
            resources.ApplyResources(this.tbHWVersion, "tbHWVersion");
            this.tbHWVersion.Name = "tbHWVersion";
            // 
            // tbDesciption
            // 
            resources.ApplyResources(this.tbDesciption, "tbDesciption");
            this.tbDesciption.Name = "tbDesciption";
            // 
            // tbSerialNumber
            // 
            resources.ApplyResources(this.tbSerialNumber, "tbSerialNumber");
            this.tbSerialNumber.Name = "tbSerialNumber";
            this.tbSerialNumber.ReadOnly = true;
            // 
            // label14
            // 
            resources.ApplyResources(this.label14, "label14");
            this.label14.Name = "label14";
            // 
            // label13
            // 
            resources.ApplyResources(this.label13, "label13");
            this.label13.Name = "label13";
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // tabCommand
            // 
            this.tabCommand.Controls.Add(this.gbCommandWrite);
            this.tabCommand.Controls.Add(this.gbCommandRead);
            this.tabCommand.Controls.Add(this.gbCommandGeneral);
            resources.ApplyResources(this.tabCommand, "tabCommand");
            this.tabCommand.Name = "tabCommand";
            this.tabCommand.UseVisualStyleBackColor = true;
            // 
            // gbCommandWrite
            // 
            resources.ApplyResources(this.gbCommandWrite, "gbCommandWrite");
            this.gbCommandWrite.Controls.Add(this.tbCmdWriteAddress);
            this.gbCommandWrite.Controls.Add(this.label19);
            this.gbCommandWrite.Controls.Add(this.label20);
            this.gbCommandWrite.Controls.Add(this.bCmdWrite);
            this.gbCommandWrite.Controls.Add(this.tbCmdWriteBytes);
            this.gbCommandWrite.Controls.Add(this.cmdWriteData);
            this.gbCommandWrite.Name = "gbCommandWrite";
            this.gbCommandWrite.TabStop = false;
            // 
            // tbCmdWriteAddress
            // 
            this.tbCmdWriteAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "CommandWriteAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbCmdWriteAddress, "tbCmdWriteAddress");
            this.tbCmdWriteAddress.Name = "tbCmdWriteAddress";
            this.tbCmdWriteAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.CommandWriteAddress;
            // 
            // label19
            // 
            resources.ApplyResources(this.label19, "label19");
            this.label19.Name = "label19";
            // 
            // label20
            // 
            resources.ApplyResources(this.label20, "label20");
            this.label20.Name = "label20";
            // 
            // bCmdWrite
            // 
            resources.ApplyResources(this.bCmdWrite, "bCmdWrite");
            this.bCmdWrite.Name = "bCmdWrite";
            this.bCmdWrite.UseVisualStyleBackColor = true;
            this.bCmdWrite.Click += new System.EventHandler(this.bCmdWrite_Click);
            // 
            // tbCmdWriteBytes
            // 
            this.tbCmdWriteBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "CommandWriteBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbCmdWriteBytes, "tbCmdWriteBytes");
            this.tbCmdWriteBytes.Name = "tbCmdWriteBytes";
            this.tbCmdWriteBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.CommandWriteBytes;
            // 
            // cmdWriteData
            // 
            resources.ApplyResources(this.cmdWriteData, "cmdWriteData");
            this.cmdWriteData.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "CommandWriteData", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cmdWriteData.Name = "cmdWriteData";
            this.cmdWriteData.Text = global::QuickUsbDiagCs.Properties.Settings.Default.CommandWriteData;
            // 
            // gbCommandRead
            // 
            resources.ApplyResources(this.gbCommandRead, "gbCommandRead");
            this.gbCommandRead.Controls.Add(this.tbCmdReadAddress);
            this.gbCommandRead.Controls.Add(this.label18);
            this.gbCommandRead.Controls.Add(this.label17);
            this.gbCommandRead.Controls.Add(this.tbCmdReadBytes);
            this.gbCommandRead.Controls.Add(this.cmdReadData);
            this.gbCommandRead.Controls.Add(this.bCmdRead);
            this.gbCommandRead.Name = "gbCommandRead";
            this.gbCommandRead.TabStop = false;
            // 
            // tbCmdReadAddress
            // 
            this.tbCmdReadAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "CommandReadAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbCmdReadAddress, "tbCmdReadAddress");
            this.tbCmdReadAddress.Name = "tbCmdReadAddress";
            this.tbCmdReadAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.CommandReadAddress;
            // 
            // label18
            // 
            resources.ApplyResources(this.label18, "label18");
            this.label18.Name = "label18";
            // 
            // label17
            // 
            resources.ApplyResources(this.label17, "label17");
            this.label17.Name = "label17";
            // 
            // tbCmdReadBytes
            // 
            this.tbCmdReadBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "CommandReadBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbCmdReadBytes, "tbCmdReadBytes");
            this.tbCmdReadBytes.Name = "tbCmdReadBytes";
            this.tbCmdReadBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.CommandReadBytes;
            // 
            // cmdReadData
            // 
            resources.ApplyResources(this.cmdReadData, "cmdReadData");
            this.cmdReadData.Name = "cmdReadData";
            this.cmdReadData.ReadOnly = true;
            // 
            // bCmdRead
            // 
            resources.ApplyResources(this.bCmdRead, "bCmdRead");
            this.bCmdRead.Name = "bCmdRead";
            this.bCmdRead.UseVisualStyleBackColor = true;
            this.bCmdRead.Click += new System.EventHandler(this.bCmdRead_Click);
            // 
            // gbCommandGeneral
            // 
            resources.ApplyResources(this.gbCommandGeneral, "gbCommandGeneral");
            this.gbCommandGeneral.Controls.Add(this.cbCmdWordWide);
            this.gbCommandGeneral.Controls.Add(this.cbCmdUseAddr);
            this.gbCommandGeneral.Controls.Add(this.cbCmdIncAddr);
            this.gbCommandGeneral.Name = "gbCommandGeneral";
            this.gbCommandGeneral.TabStop = false;
            // 
            // cbCmdWordWide
            // 
            resources.ApplyResources(this.cbCmdWordWide, "cbCmdWordWide");
            this.cbCmdWordWide.Name = "cbCmdWordWide";
            this.cbCmdWordWide.UseVisualStyleBackColor = true;
            this.cbCmdWordWide.CheckedChanged += new System.EventHandler(this.cbCmdWordWide_CheckedChanged);
            // 
            // cbCmdUseAddr
            // 
            resources.ApplyResources(this.cbCmdUseAddr, "cbCmdUseAddr");
            this.cbCmdUseAddr.Name = "cbCmdUseAddr";
            this.cbCmdUseAddr.UseVisualStyleBackColor = true;
            this.cbCmdUseAddr.CheckedChanged += new System.EventHandler(this.cbCmdAddr_CheckedChanged);
            // 
            // cbCmdIncAddr
            // 
            resources.ApplyResources(this.cbCmdIncAddr, "cbCmdIncAddr");
            this.cbCmdIncAddr.Name = "cbCmdIncAddr";
            this.cbCmdIncAddr.UseVisualStyleBackColor = true;
            this.cbCmdIncAddr.CheckedChanged += new System.EventHandler(this.cbCmdAddr_CheckedChanged);
            // 
            // tabData
            // 
            this.tabData.Controls.Add(this.gbDataWrite);
            this.tabData.Controls.Add(this.gbDataRead);
            this.tabData.Controls.Add(this.gbDataGeneral);
            resources.ApplyResources(this.tabData, "tabData");
            this.tabData.Name = "tabData";
            this.tabData.UseVisualStyleBackColor = true;
            // 
            // gbDataWrite
            // 
            resources.ApplyResources(this.gbDataWrite, "gbDataWrite");
            this.gbDataWrite.Controls.Add(this.bDataWriteEdit);
            this.gbDataWrite.Controls.Add(this.bDataCount);
            this.gbDataWrite.Controls.Add(this.cbUseFileLength);
            this.gbDataWrite.Controls.Add(this.bDataWrite);
            this.gbDataWrite.Controls.Add(this.tbDataWriteBytes);
            this.gbDataWrite.Controls.Add(this.label21);
            this.gbDataWrite.Controls.Add(this.tbInputFileName);
            this.gbDataWrite.Controls.Add(this.bBrowseWrite);
            this.gbDataWrite.Controls.Add(this.tbDataWriteAddress);
            this.gbDataWrite.Controls.Add(this.label25);
            this.gbDataWrite.Controls.Add(this.label26);
            this.gbDataWrite.Name = "gbDataWrite";
            this.gbDataWrite.TabStop = false;
            // 
            // cbUseFileLength
            // 
            resources.ApplyResources(this.cbUseFileLength, "cbUseFileLength");
            this.cbUseFileLength.Checked = global::QuickUsbDiagCs.Properties.Settings.Default.DataWriteEntireFile;
            this.cbUseFileLength.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbUseFileLength.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::QuickUsbDiagCs.Properties.Settings.Default, "DataWriteEntireFile", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbUseFileLength.Name = "cbUseFileLength";
            this.cbUseFileLength.UseVisualStyleBackColor = true;
            // 
            // tbDataWriteBytes
            // 
            this.tbDataWriteBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataWriteBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbDataWriteBytes, "tbDataWriteBytes");
            this.tbDataWriteBytes.Name = "tbDataWriteBytes";
            this.tbDataWriteBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataWriteBytes;
            this.tbDataWriteBytes.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label21
            // 
            resources.ApplyResources(this.label21, "label21");
            this.label21.Name = "label21";
            // 
            // tbInputFileName
            // 
            resources.ApplyResources(this.tbInputFileName, "tbInputFileName");
            this.tbInputFileName.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataWriteFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbInputFileName.Name = "tbInputFileName";
            this.tbInputFileName.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataWriteFilePath;
            // 
            // bBrowseWrite
            // 
            resources.ApplyResources(this.bBrowseWrite, "bBrowseWrite");
            this.bBrowseWrite.Name = "bBrowseWrite";
            this.bBrowseWrite.UseVisualStyleBackColor = true;
            this.bBrowseWrite.Click += new System.EventHandler(this.bBrowseWrite_Click);
            // 
            // tbDataWriteAddress
            // 
            this.tbDataWriteAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataWriteStartAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbDataWriteAddress, "tbDataWriteAddress");
            this.tbDataWriteAddress.Name = "tbDataWriteAddress";
            this.tbDataWriteAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataWriteStartAddress;
            // 
            // label25
            // 
            resources.ApplyResources(this.label25, "label25");
            this.label25.Name = "label25";
            // 
            // label26
            // 
            resources.ApplyResources(this.label26, "label26");
            this.label26.Name = "label26";
            // 
            // gbDataRead
            // 
            resources.ApplyResources(this.gbDataRead, "gbDataRead");
            this.gbDataRead.Controls.Add(this.bDataReadEdit);
            this.gbDataRead.Controls.Add(this.tbDataReadBytes);
            this.gbDataRead.Controls.Add(this.label24);
            this.gbDataRead.Controls.Add(this.tbOutputFileName);
            this.gbDataRead.Controls.Add(this.dataReadBrowse);
            this.gbDataRead.Controls.Add(this.tbDataReadAddress);
            this.gbDataRead.Controls.Add(this.label23);
            this.gbDataRead.Controls.Add(this.label22);
            this.gbDataRead.Controls.Add(this.bDataRead);
            this.gbDataRead.Name = "gbDataRead";
            this.gbDataRead.TabStop = false;
            // 
            // tbDataReadBytes
            // 
            this.tbDataReadBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataReadBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbDataReadBytes, "tbDataReadBytes");
            this.tbDataReadBytes.Name = "tbDataReadBytes";
            this.tbDataReadBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataReadBytes;
            this.tbDataReadBytes.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label24
            // 
            resources.ApplyResources(this.label24, "label24");
            this.label24.Name = "label24";
            // 
            // tbOutputFileName
            // 
            resources.ApplyResources(this.tbOutputFileName, "tbOutputFileName");
            this.tbOutputFileName.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataReadFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbOutputFileName.Name = "tbOutputFileName";
            this.tbOutputFileName.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataReadFilePath;
            // 
            // dataReadBrowse
            // 
            resources.ApplyResources(this.dataReadBrowse, "dataReadBrowse");
            this.dataReadBrowse.Name = "dataReadBrowse";
            this.dataReadBrowse.UseVisualStyleBackColor = true;
            this.dataReadBrowse.Click += new System.EventHandler(this.bBrowseRead_Click);
            // 
            // tbDataReadAddress
            // 
            this.tbDataReadAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataReadStartAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbDataReadAddress, "tbDataReadAddress");
            this.tbDataReadAddress.Name = "tbDataReadAddress";
            this.tbDataReadAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataReadStartAddress;
            // 
            // label23
            // 
            resources.ApplyResources(this.label23, "label23");
            this.label23.Name = "label23";
            // 
            // label22
            // 
            resources.ApplyResources(this.label22, "label22");
            this.label22.Name = "label22";
            // 
            // gbDataGeneral
            // 
            resources.ApplyResources(this.gbDataGeneral, "gbDataGeneral");
            this.gbDataGeneral.Controls.Add(this.cbDataOutOfOrder);
            this.gbDataGeneral.Controls.Add(this.cbDataIncAddr);
            this.gbDataGeneral.Controls.Add(this.cbDataUseAddr);
            this.gbDataGeneral.Controls.Add(this.cbDataWordwide);
            this.gbDataGeneral.Name = "gbDataGeneral";
            this.gbDataGeneral.TabStop = false;
            // 
            // cbDataOutOfOrder
            // 
            this.cbDataOutOfOrder.Checked = global::QuickUsbDiagCs.Properties.Settings.Default.DataOutOfOrder;
            this.cbDataOutOfOrder.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::QuickUsbDiagCs.Properties.Settings.Default, "DataOutOfOrder", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.cbDataOutOfOrder, "cbDataOutOfOrder");
            this.cbDataOutOfOrder.Name = "cbDataOutOfOrder";
            this.cbDataOutOfOrder.UseVisualStyleBackColor = true;
            // 
            // cbDataIncAddr
            // 
            resources.ApplyResources(this.cbDataIncAddr, "cbDataIncAddr");
            this.cbDataIncAddr.Name = "cbDataIncAddr";
            this.cbDataIncAddr.UseVisualStyleBackColor = true;
            this.cbDataIncAddr.CheckedChanged += new System.EventHandler(this.cbDataAddr_CheckedChanged);
            // 
            // cbDataUseAddr
            // 
            resources.ApplyResources(this.cbDataUseAddr, "cbDataUseAddr");
            this.cbDataUseAddr.Name = "cbDataUseAddr";
            this.cbDataUseAddr.UseVisualStyleBackColor = true;
            this.cbDataUseAddr.CheckedChanged += new System.EventHandler(this.cbDataAddr_CheckedChanged);
            // 
            // cbDataWordwide
            // 
            resources.ApplyResources(this.cbDataWordwide, "cbDataWordwide");
            this.cbDataWordwide.Name = "cbDataWordwide";
            this.cbDataWordwide.UseVisualStyleBackColor = true;
            this.cbDataWordwide.CheckedChanged += new System.EventHandler(this.cbDataWordwide_CheckedChanged);
            // 
            // tabStreaming
            // 
            this.tabStreaming.Controls.Add(this.gbStreamingStatistics);
            this.tabStreaming.Controls.Add(this.gbStreamingWrite);
            this.tabStreaming.Controls.Add(this.gbStreamingGeneral);
            this.tabStreaming.Controls.Add(this.gbStreamingRead);
            resources.ApplyResources(this.tabStreaming, "tabStreaming");
            this.tabStreaming.Name = "tabStreaming";
            this.tabStreaming.UseVisualStyleBackColor = true;
            // 
            // gbStreamingStatistics
            // 
            resources.ApplyResources(this.gbStreamingStatistics, "gbStreamingStatistics");
            this.gbStreamingStatistics.Controls.Add(this.label5);
            this.gbStreamingStatistics.Controls.Add(this.label4);
            this.gbStreamingStatistics.Controls.Add(this.labelWriteStreamCount);
            this.gbStreamingStatistics.Controls.Add(this.labelWriteStreamRate);
            this.gbStreamingStatistics.Controls.Add(this.labelReadStreamCount);
            this.gbStreamingStatistics.Controls.Add(this.labelReadStreamRate);
            this.gbStreamingStatistics.Controls.Add(this.label3);
            this.gbStreamingStatistics.Controls.Add(this.label2);
            this.gbStreamingStatistics.Name = "gbStreamingStatistics";
            this.gbStreamingStatistics.TabStop = false;
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // labelWriteStreamCount
            // 
            resources.ApplyResources(this.labelWriteStreamCount, "labelWriteStreamCount");
            this.labelWriteStreamCount.Name = "labelWriteStreamCount";
            // 
            // labelWriteStreamRate
            // 
            resources.ApplyResources(this.labelWriteStreamRate, "labelWriteStreamRate");
            this.labelWriteStreamRate.Name = "labelWriteStreamRate";
            // 
            // labelReadStreamCount
            // 
            resources.ApplyResources(this.labelReadStreamCount, "labelReadStreamCount");
            this.labelReadStreamCount.Name = "labelReadStreamCount";
            // 
            // labelReadStreamRate
            // 
            resources.ApplyResources(this.labelReadStreamRate, "labelReadStreamRate");
            this.labelReadStreamRate.Name = "labelReadStreamRate";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // gbStreamingWrite
            // 
            resources.ApplyResources(this.gbStreamingWrite, "gbStreamingWrite");
            this.gbStreamingWrite.Controls.Add(this.panel4);
            this.gbStreamingWrite.Controls.Add(this.bWriteStreamStop);
            this.gbStreamingWrite.Controls.Add(this.bWriteStreamEdit);
            this.gbStreamingWrite.Controls.Add(this.tbWriteStreamFileName);
            this.gbStreamingWrite.Controls.Add(this.bWriteStreamBrowse);
            this.gbStreamingWrite.Controls.Add(this.lWriteStreamFileName);
            this.gbStreamingWrite.Controls.Add(this.bWriteStreamStart);
            this.gbStreamingWrite.Name = "gbStreamingWrite";
            this.gbStreamingWrite.TabStop = false;
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.rbWriteStreamFromDisk);
            this.panel4.Controls.Add(this.rbWriteStreamFromPattern);
            resources.ApplyResources(this.panel4, "panel4");
            this.panel4.Name = "panel4";
            // 
            // rbWriteStreamFromDisk
            // 
            resources.ApplyResources(this.rbWriteStreamFromDisk, "rbWriteStreamFromDisk");
            this.rbWriteStreamFromDisk.Checked = true;
            this.rbWriteStreamFromDisk.Name = "rbWriteStreamFromDisk";
            this.rbWriteStreamFromDisk.TabStop = true;
            this.rbWriteStreamFromDisk.UseVisualStyleBackColor = true;
            this.rbWriteStreamFromDisk.CheckedChanged += new System.EventHandler(this.rbStreamWrite_CheckedChanged);
            // 
            // rbWriteStreamFromPattern
            // 
            resources.ApplyResources(this.rbWriteStreamFromPattern, "rbWriteStreamFromPattern");
            this.rbWriteStreamFromPattern.Name = "rbWriteStreamFromPattern";
            this.rbWriteStreamFromPattern.UseVisualStyleBackColor = true;
            this.rbWriteStreamFromPattern.CheckedChanged += new System.EventHandler(this.rbStreamWrite_CheckedChanged);
            // 
            // tbWriteStreamFileName
            // 
            resources.ApplyResources(this.tbWriteStreamFileName, "tbWriteStreamFileName");
            this.tbWriteStreamFileName.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataStreamWriteFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbWriteStreamFileName.Name = "tbWriteStreamFileName";
            this.tbWriteStreamFileName.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataStreamWriteFilePath;
            // 
            // bWriteStreamBrowse
            // 
            resources.ApplyResources(this.bWriteStreamBrowse, "bWriteStreamBrowse");
            this.bWriteStreamBrowse.Name = "bWriteStreamBrowse";
            this.bWriteStreamBrowse.UseVisualStyleBackColor = true;
            this.bWriteStreamBrowse.Click += new System.EventHandler(this.bWriteStreamBrowse_Click);
            // 
            // lWriteStreamFileName
            // 
            resources.ApplyResources(this.lWriteStreamFileName, "lWriteStreamFileName");
            this.lWriteStreamFileName.Name = "lWriteStreamFileName";
            // 
            // gbStreamingGeneral
            // 
            resources.ApplyResources(this.gbStreamingGeneral, "gbStreamingGeneral");
            this.gbStreamingGeneral.Controls.Add(this.tbStreamThreadConcurrency);
            this.gbStreamingGeneral.Controls.Add(this.label70);
            this.gbStreamingGeneral.Controls.Add(this.tbStreamNumThreads);
            this.gbStreamingGeneral.Controls.Add(this.label69);
            this.gbStreamingGeneral.Controls.Add(this.tbStreamNumBuffers);
            this.gbStreamingGeneral.Controls.Add(this.label68);
            this.gbStreamingGeneral.Controls.Add(this.tbStreamBufferSize);
            this.gbStreamingGeneral.Controls.Add(this.label67);
            this.gbStreamingGeneral.Controls.Add(this.cbStreamingIncAddr);
            this.gbStreamingGeneral.Controls.Add(this.cbStreamingUseAddr);
            this.gbStreamingGeneral.Controls.Add(this.cbStreamingWordwide);
            this.gbStreamingGeneral.Name = "gbStreamingGeneral";
            this.gbStreamingGeneral.TabStop = false;
            // 
            // tbStreamThreadConcurrency
            // 
            resources.ApplyResources(this.tbStreamThreadConcurrency, "tbStreamThreadConcurrency");
            this.tbStreamThreadConcurrency.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "StreamThreadConcurrency", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbStreamThreadConcurrency.Name = "tbStreamThreadConcurrency";
            this.tbStreamThreadConcurrency.Text = global::QuickUsbDiagCs.Properties.Settings.Default.StreamThreadConcurrency;
            this.tbStreamThreadConcurrency.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label70
            // 
            resources.ApplyResources(this.label70, "label70");
            this.label70.Name = "label70";
            // 
            // tbStreamNumThreads
            // 
            resources.ApplyResources(this.tbStreamNumThreads, "tbStreamNumThreads");
            this.tbStreamNumThreads.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "StreamNumThreads", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbStreamNumThreads.Name = "tbStreamNumThreads";
            this.tbStreamNumThreads.Text = global::QuickUsbDiagCs.Properties.Settings.Default.StreamNumThreads;
            this.tbStreamNumThreads.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label69
            // 
            resources.ApplyResources(this.label69, "label69");
            this.label69.Name = "label69";
            // 
            // tbStreamNumBuffers
            // 
            resources.ApplyResources(this.tbStreamNumBuffers, "tbStreamNumBuffers");
            this.tbStreamNumBuffers.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "StreamNumBuffers", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbStreamNumBuffers.Name = "tbStreamNumBuffers";
            this.tbStreamNumBuffers.Text = global::QuickUsbDiagCs.Properties.Settings.Default.StreamNumBuffers;
            this.tbStreamNumBuffers.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label68
            // 
            resources.ApplyResources(this.label68, "label68");
            this.label68.Name = "label68";
            // 
            // tbStreamBufferSize
            // 
            resources.ApplyResources(this.tbStreamBufferSize, "tbStreamBufferSize");
            this.tbStreamBufferSize.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "StreamBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbStreamBufferSize.Name = "tbStreamBufferSize";
            this.tbStreamBufferSize.Text = global::QuickUsbDiagCs.Properties.Settings.Default.StreamBytes;
            this.tbStreamBufferSize.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStreamBufferSize_KeyPress);
            // 
            // label67
            // 
            resources.ApplyResources(this.label67, "label67");
            this.label67.Name = "label67";
            // 
            // cbStreamingIncAddr
            // 
            resources.ApplyResources(this.cbStreamingIncAddr, "cbStreamingIncAddr");
            this.cbStreamingIncAddr.Name = "cbStreamingIncAddr";
            this.cbStreamingIncAddr.UseVisualStyleBackColor = true;
            // 
            // cbStreamingUseAddr
            // 
            resources.ApplyResources(this.cbStreamingUseAddr, "cbStreamingUseAddr");
            this.cbStreamingUseAddr.Name = "cbStreamingUseAddr";
            this.cbStreamingUseAddr.UseVisualStyleBackColor = true;
            // 
            // cbStreamingWordwide
            // 
            resources.ApplyResources(this.cbStreamingWordwide, "cbStreamingWordwide");
            this.cbStreamingWordwide.Name = "cbStreamingWordwide";
            this.cbStreamingWordwide.UseVisualStyleBackColor = true;
            // 
            // gbStreamingRead
            // 
            resources.ApplyResources(this.gbStreamingRead, "gbStreamingRead");
            this.gbStreamingRead.Controls.Add(this.panel3);
            this.gbStreamingRead.Controls.Add(this.bReadStreamStop);
            this.gbStreamingRead.Controls.Add(this.bReadStreamEdit);
            this.gbStreamingRead.Controls.Add(this.tbReadStreamFileName);
            this.gbStreamingRead.Controls.Add(this.bReadStreamBrowse);
            this.gbStreamingRead.Controls.Add(this.lReadStreamFileName);
            this.gbStreamingRead.Controls.Add(this.bReadStreamStart);
            this.gbStreamingRead.Name = "gbStreamingRead";
            this.gbStreamingRead.TabStop = false;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.rbReadStreamToDisk);
            this.panel3.Controls.Add(this.rbReadStreamDisgard);
            resources.ApplyResources(this.panel3, "panel3");
            this.panel3.Name = "panel3";
            // 
            // rbReadStreamToDisk
            // 
            resources.ApplyResources(this.rbReadStreamToDisk, "rbReadStreamToDisk");
            this.rbReadStreamToDisk.Checked = true;
            this.rbReadStreamToDisk.Name = "rbReadStreamToDisk";
            this.rbReadStreamToDisk.TabStop = true;
            this.rbReadStreamToDisk.UseVisualStyleBackColor = true;
            this.rbReadStreamToDisk.CheckedChanged += new System.EventHandler(this.rbStreamRead_CheckedChanged);
            // 
            // rbReadStreamDisgard
            // 
            resources.ApplyResources(this.rbReadStreamDisgard, "rbReadStreamDisgard");
            this.rbReadStreamDisgard.Name = "rbReadStreamDisgard";
            this.rbReadStreamDisgard.UseVisualStyleBackColor = true;
            this.rbReadStreamDisgard.CheckedChanged += new System.EventHandler(this.rbStreamRead_CheckedChanged);
            // 
            // tbReadStreamFileName
            // 
            resources.ApplyResources(this.tbReadStreamFileName, "tbReadStreamFileName");
            this.tbReadStreamFileName.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "DataStreamReadFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbReadStreamFileName.Name = "tbReadStreamFileName";
            this.tbReadStreamFileName.Text = global::QuickUsbDiagCs.Properties.Settings.Default.DataStreamReadFilePath;
            // 
            // bReadStreamBrowse
            // 
            resources.ApplyResources(this.bReadStreamBrowse, "bReadStreamBrowse");
            this.bReadStreamBrowse.Name = "bReadStreamBrowse";
            this.bReadStreamBrowse.UseVisualStyleBackColor = true;
            this.bReadStreamBrowse.Click += new System.EventHandler(this.bReadStreamBrowse_Click);
            // 
            // lReadStreamFileName
            // 
            resources.ApplyResources(this.lReadStreamFileName, "lReadStreamFileName");
            this.lReadStreamFileName.Name = "lReadStreamFileName";
            // 
            // tabPorts
            // 
            this.tabPorts.Controls.Add(this.gbPortE);
            this.tabPorts.Controls.Add(this.gbPortD);
            this.tabPorts.Controls.Add(this.gbPortC);
            this.tabPorts.Controls.Add(this.gbPortB);
            this.tabPorts.Controls.Add(this.gbPortA);
            resources.ApplyResources(this.tabPorts, "tabPorts");
            this.tabPorts.Name = "tabPorts";
            this.tabPorts.UseVisualStyleBackColor = true;
            // 
            // gbPortE
            // 
            resources.ApplyResources(this.gbPortE, "gbPortE");
            this.gbPortE.Controls.Add(this.bReadPortE);
            this.gbPortE.Controls.Add(this.btPortEDir);
            this.gbPortE.Controls.Add(this.btPortEVal);
            this.gbPortE.Name = "gbPortE";
            this.gbPortE.TabStop = false;
            // 
            // bReadPortE
            // 
            resources.ApplyResources(this.bReadPortE, "bReadPortE");
            this.bReadPortE.Name = "bReadPortE";
            this.bReadPortE.UseVisualStyleBackColor = true;
            this.bReadPortE.Click += new System.EventHandler(this.bReadPortE_Click);
            // 
            // gbPortD
            // 
            resources.ApplyResources(this.gbPortD, "gbPortD");
            this.gbPortD.Controls.Add(this.bReadPortD);
            this.gbPortD.Controls.Add(this.btPortDDir);
            this.gbPortD.Controls.Add(this.btPortDVal);
            this.gbPortD.Name = "gbPortD";
            this.gbPortD.TabStop = false;
            // 
            // bReadPortD
            // 
            resources.ApplyResources(this.bReadPortD, "bReadPortD");
            this.bReadPortD.Name = "bReadPortD";
            this.bReadPortD.UseVisualStyleBackColor = true;
            this.bReadPortD.Click += new System.EventHandler(this.bReadPortD_Click);
            // 
            // gbPortC
            // 
            resources.ApplyResources(this.gbPortC, "gbPortC");
            this.gbPortC.Controls.Add(this.bReadPortC);
            this.gbPortC.Controls.Add(this.btPortCDir);
            this.gbPortC.Controls.Add(this.btPortCVal);
            this.gbPortC.Name = "gbPortC";
            this.gbPortC.TabStop = false;
            // 
            // bReadPortC
            // 
            resources.ApplyResources(this.bReadPortC, "bReadPortC");
            this.bReadPortC.Name = "bReadPortC";
            this.bReadPortC.UseVisualStyleBackColor = true;
            this.bReadPortC.Click += new System.EventHandler(this.bReadPortC_Click);
            // 
            // gbPortB
            // 
            resources.ApplyResources(this.gbPortB, "gbPortB");
            this.gbPortB.Controls.Add(this.bReadPortB);
            this.gbPortB.Controls.Add(this.btPortBDir);
            this.gbPortB.Controls.Add(this.btPortBVal);
            this.gbPortB.Name = "gbPortB";
            this.gbPortB.TabStop = false;
            // 
            // bReadPortB
            // 
            resources.ApplyResources(this.bReadPortB, "bReadPortB");
            this.bReadPortB.Name = "bReadPortB";
            this.bReadPortB.UseVisualStyleBackColor = true;
            this.bReadPortB.Click += new System.EventHandler(this.bReadPortB_Click);
            // 
            // gbPortA
            // 
            resources.ApplyResources(this.gbPortA, "gbPortA");
            this.gbPortA.Controls.Add(this.bCountPortA);
            this.gbPortA.Controls.Add(this.bReadPortA);
            this.gbPortA.Controls.Add(this.label29);
            this.gbPortA.Controls.Add(this.label28);
            this.gbPortA.Controls.Add(this.btPortADir);
            this.gbPortA.Controls.Add(this.btPortAVal);
            this.gbPortA.Name = "gbPortA";
            this.gbPortA.TabStop = false;
            // 
            // bCountPortA
            // 
            resources.ApplyResources(this.bCountPortA, "bCountPortA");
            this.bCountPortA.Name = "bCountPortA";
            this.bCountPortA.UseVisualStyleBackColor = true;
            this.bCountPortA.Click += new System.EventHandler(this.bCountPortA_Click);
            // 
            // bReadPortA
            // 
            resources.ApplyResources(this.bReadPortA, "bReadPortA");
            this.bReadPortA.Name = "bReadPortA";
            this.bReadPortA.UseVisualStyleBackColor = true;
            this.bReadPortA.Click += new System.EventHandler(this.bReadPortA_Click);
            // 
            // label29
            // 
            resources.ApplyResources(this.label29, "label29");
            this.label29.Name = "label29";
            // 
            // label28
            // 
            resources.ApplyResources(this.label28, "label28");
            this.label28.Name = "label28";
            // 
            // tabRS232
            // 
            this.tabRS232.Controls.Add(this.gbRS232Write);
            this.tabRS232.Controls.Add(this.gbRS232Read);
            this.tabRS232.Controls.Add(this.gbRS232General);
            resources.ApplyResources(this.tabRS232, "tabRS232");
            this.tabRS232.Name = "tabRS232";
            this.tabRS232.UseVisualStyleBackColor = true;
            // 
            // gbRS232Write
            // 
            resources.ApplyResources(this.gbRS232Write, "gbRS232Write");
            this.gbRS232Write.Controls.Add(this.rs232WriteData);
            this.gbRS232Write.Controls.Add(this.rs232Write);
            this.gbRS232Write.Name = "gbRS232Write";
            this.gbRS232Write.TabStop = false;
            // 
            // rs232WriteData
            // 
            resources.ApplyResources(this.rs232WriteData, "rs232WriteData");
            this.rs232WriteData.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "RS232WriteData", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.rs232WriteData.Name = "rs232WriteData";
            this.rs232WriteData.Text = global::QuickUsbDiagCs.Properties.Settings.Default.RS232WriteData;
            // 
            // rs232Write
            // 
            resources.ApplyResources(this.rs232Write, "rs232Write");
            this.rs232Write.Name = "rs232Write";
            this.rs232Write.UseVisualStyleBackColor = true;
            this.rs232Write.Click += new System.EventHandler(this.bRS232Write_Click);
            // 
            // gbRS232Read
            // 
            resources.ApplyResources(this.gbRS232Read, "gbRS232Read");
            this.gbRS232Read.Controls.Add(this.label30);
            this.gbRS232Read.Controls.Add(this.tbRS232Chars);
            this.gbRS232Read.Controls.Add(this.rs232Get);
            this.gbRS232Read.Controls.Add(this.tbRS232ReadData);
            this.gbRS232Read.Controls.Add(this.rs232Read);
            this.gbRS232Read.Name = "gbRS232Read";
            this.gbRS232Read.TabStop = false;
            // 
            // label30
            // 
            resources.ApplyResources(this.label30, "label30");
            this.label30.Name = "label30";
            // 
            // tbRS232Chars
            // 
            resources.ApplyResources(this.tbRS232Chars, "tbRS232Chars");
            this.tbRS232Chars.Name = "tbRS232Chars";
            this.tbRS232Chars.ReadOnly = true;
            // 
            // rs232Get
            // 
            resources.ApplyResources(this.rs232Get, "rs232Get");
            this.rs232Get.Name = "rs232Get";
            this.rs232Get.UseVisualStyleBackColor = true;
            this.rs232Get.Click += new System.EventHandler(this.bRS232Get_Click);
            // 
            // tbRS232ReadData
            // 
            resources.ApplyResources(this.tbRS232ReadData, "tbRS232ReadData");
            this.tbRS232ReadData.Name = "tbRS232ReadData";
            this.tbRS232ReadData.ReadOnly = true;
            // 
            // rs232Read
            // 
            resources.ApplyResources(this.rs232Read, "rs232Read");
            this.rs232Read.Name = "rs232Read";
            this.rs232Read.UseVisualStyleBackColor = true;
            this.rs232Read.Click += new System.EventHandler(this.bRS232Read_Click);
            // 
            // gbRS232General
            // 
            resources.ApplyResources(this.gbRS232General, "gbRS232General");
            this.gbRS232General.Controls.Add(this.cbRS232Port);
            this.gbRS232General.Controls.Add(this.label32);
            this.gbRS232General.Controls.Add(this.cbRS232Baud);
            this.gbRS232General.Controls.Add(this.label31);
            this.gbRS232General.Controls.Add(this.bRS232Flush);
            this.gbRS232General.Name = "gbRS232General";
            this.gbRS232General.TabStop = false;
            // 
            // cbRS232Port
            // 
            this.cbRS232Port.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbRS232Port.FormattingEnabled = true;
            this.cbRS232Port.Items.AddRange(new object[] {
            resources.GetString("cbRS232Port.Items"),
            resources.GetString("cbRS232Port.Items1")});
            resources.ApplyResources(this.cbRS232Port, "cbRS232Port");
            this.cbRS232Port.Name = "cbRS232Port";
            // 
            // label32
            // 
            resources.ApplyResources(this.label32, "label32");
            this.label32.Name = "label32";
            // 
            // cbRS232Baud
            // 
            this.cbRS232Baud.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbRS232Baud.FormattingEnabled = true;
            this.cbRS232Baud.Items.AddRange(new object[] {
            resources.GetString("cbRS232Baud.Items"),
            resources.GetString("cbRS232Baud.Items1"),
            resources.GetString("cbRS232Baud.Items2"),
            resources.GetString("cbRS232Baud.Items3"),
            resources.GetString("cbRS232Baud.Items4"),
            resources.GetString("cbRS232Baud.Items5"),
            resources.GetString("cbRS232Baud.Items6")});
            resources.ApplyResources(this.cbRS232Baud, "cbRS232Baud");
            this.cbRS232Baud.Name = "cbRS232Baud";
            this.cbRS232Baud.SelectedIndexChanged += new System.EventHandler(this.cbRS232Baud_SelectedIndexChanged);
            // 
            // label31
            // 
            resources.ApplyResources(this.label31, "label31");
            this.label31.Name = "label31";
            // 
            // bRS232Flush
            // 
            resources.ApplyResources(this.bRS232Flush, "bRS232Flush");
            this.bRS232Flush.Name = "bRS232Flush";
            this.bRS232Flush.UseVisualStyleBackColor = true;
            this.bRS232Flush.Click += new System.EventHandler(this.bRS232Flush_Click);
            // 
            // tabSPI
            // 
            this.tabSPI.Controls.Add(this.gbSPIWrite);
            this.tabSPI.Controls.Add(this.gbSPIRead);
            this.tabSPI.Controls.Add(this.gbSPIGeneral);
            resources.ApplyResources(this.tabSPI, "tabSPI");
            this.tabSPI.Name = "tabSPI";
            this.tabSPI.UseVisualStyleBackColor = true;
            // 
            // gbSPIWrite
            // 
            resources.ApplyResources(this.gbSPIWrite, "gbSPIWrite");
            this.gbSPIWrite.Controls.Add(this.bWriteReadSpi);
            this.gbSPIWrite.Controls.Add(this.cbSpiWritePort);
            this.gbSPIWrite.Controls.Add(this.label35);
            this.gbSPIWrite.Controls.Add(this.tbSpiWriteData);
            this.gbSPIWrite.Controls.Add(this.spiWrite);
            this.gbSPIWrite.Name = "gbSPIWrite";
            this.gbSPIWrite.TabStop = false;
            // 
            // bWriteReadSpi
            // 
            resources.ApplyResources(this.bWriteReadSpi, "bWriteReadSpi");
            this.bWriteReadSpi.Name = "bWriteReadSpi";
            this.bWriteReadSpi.UseVisualStyleBackColor = true;
            this.bWriteReadSpi.Click += new System.EventHandler(this.bWriteReadSpi_Click);
            // 
            // cbSpiWritePort
            // 
            this.cbSpiWritePort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSpiWritePort.FormattingEnabled = true;
            this.cbSpiWritePort.Items.AddRange(new object[] {
            resources.GetString("cbSpiWritePort.Items"),
            resources.GetString("cbSpiWritePort.Items1"),
            resources.GetString("cbSpiWritePort.Items2"),
            resources.GetString("cbSpiWritePort.Items3"),
            resources.GetString("cbSpiWritePort.Items4"),
            resources.GetString("cbSpiWritePort.Items5"),
            resources.GetString("cbSpiWritePort.Items6"),
            resources.GetString("cbSpiWritePort.Items7"),
            resources.GetString("cbSpiWritePort.Items8"),
            resources.GetString("cbSpiWritePort.Items9")});
            resources.ApplyResources(this.cbSpiWritePort, "cbSpiWritePort");
            this.cbSpiWritePort.Name = "cbSpiWritePort";
            // 
            // label35
            // 
            resources.ApplyResources(this.label35, "label35");
            this.label35.Name = "label35";
            // 
            // tbSpiWriteData
            // 
            resources.ApplyResources(this.tbSpiWriteData, "tbSpiWriteData");
            this.tbSpiWriteData.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "SpiWriteData", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbSpiWriteData.Name = "tbSpiWriteData";
            this.tbSpiWriteData.Text = global::QuickUsbDiagCs.Properties.Settings.Default.SpiWriteData;
            // 
            // spiWrite
            // 
            resources.ApplyResources(this.spiWrite, "spiWrite");
            this.spiWrite.Name = "spiWrite";
            this.spiWrite.UseVisualStyleBackColor = true;
            this.spiWrite.Click += new System.EventHandler(this.bSpiWrite_Click);
            // 
            // gbSPIRead
            // 
            resources.ApplyResources(this.gbSPIRead, "gbSPIRead");
            this.gbSPIRead.Controls.Add(this.cbSpiReadPort);
            this.gbSPIRead.Controls.Add(this.label34);
            this.gbSPIRead.Controls.Add(this.label33);
            this.gbSPIRead.Controls.Add(this.tbSpiBytes);
            this.gbSPIRead.Controls.Add(this.spiRead);
            this.gbSPIRead.Controls.Add(this.tbSpiReadData);
            this.gbSPIRead.Name = "gbSPIRead";
            this.gbSPIRead.TabStop = false;
            // 
            // cbSpiReadPort
            // 
            this.cbSpiReadPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSpiReadPort.FormattingEnabled = true;
            this.cbSpiReadPort.Items.AddRange(new object[] {
            resources.GetString("cbSpiReadPort.Items"),
            resources.GetString("cbSpiReadPort.Items1"),
            resources.GetString("cbSpiReadPort.Items2"),
            resources.GetString("cbSpiReadPort.Items3"),
            resources.GetString("cbSpiReadPort.Items4"),
            resources.GetString("cbSpiReadPort.Items5"),
            resources.GetString("cbSpiReadPort.Items6"),
            resources.GetString("cbSpiReadPort.Items7"),
            resources.GetString("cbSpiReadPort.Items8"),
            resources.GetString("cbSpiReadPort.Items9")});
            resources.ApplyResources(this.cbSpiReadPort, "cbSpiReadPort");
            this.cbSpiReadPort.Name = "cbSpiReadPort";
            // 
            // label34
            // 
            resources.ApplyResources(this.label34, "label34");
            this.label34.Name = "label34";
            // 
            // label33
            // 
            resources.ApplyResources(this.label33, "label33");
            this.label33.Name = "label33";
            // 
            // tbSpiBytes
            // 
            this.tbSpiBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "SpiReadBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbSpiBytes, "tbSpiBytes");
            this.tbSpiBytes.Name = "tbSpiBytes";
            this.tbSpiBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.SpiReadBytes;
            // 
            // spiRead
            // 
            resources.ApplyResources(this.spiRead, "spiRead");
            this.spiRead.Name = "spiRead";
            this.spiRead.UseVisualStyleBackColor = true;
            this.spiRead.Click += new System.EventHandler(this.bSpiRead_Click);
            // 
            // tbSpiReadData
            // 
            resources.ApplyResources(this.tbSpiReadData, "tbSpiReadData");
            this.tbSpiReadData.Name = "tbSpiReadData";
            this.tbSpiReadData.ReadOnly = true;
            // 
            // gbSPIGeneral
            // 
            resources.ApplyResources(this.gbSPIGeneral, "gbSPIGeneral");
            this.gbSPIGeneral.Controls.Add(this.cbSpiPORT);
            this.gbSPIGeneral.Controls.Add(this.cbSpiNCEPIN);
            this.gbSPIGeneral.Controls.Add(this.cbSpiMISOPIN);
            this.gbSPIGeneral.Controls.Add(this.cbSpiEndian);
            this.gbSPIGeneral.Controls.Add(this.cbSpiCPOL);
            this.gbSPIGeneral.Controls.Add(this.cbSpiCPHA);
            this.gbSPIGeneral.Name = "gbSPIGeneral";
            this.gbSPIGeneral.TabStop = false;
            // 
            // cbSpiPORT
            // 
            resources.ApplyResources(this.cbSpiPORT, "cbSpiPORT");
            this.cbSpiPORT.Name = "cbSpiPORT";
            this.cbSpiPORT.UseVisualStyleBackColor = true;
            this.cbSpiPORT.CheckedChanged += new System.EventHandler(this.cbSpiPORT_CheckedChanged);
            // 
            // cbSpiNCEPIN
            // 
            resources.ApplyResources(this.cbSpiNCEPIN, "cbSpiNCEPIN");
            this.cbSpiNCEPIN.Name = "cbSpiNCEPIN";
            this.cbSpiNCEPIN.UseVisualStyleBackColor = true;
            this.cbSpiNCEPIN.CheckedChanged += new System.EventHandler(this.cbSpiNCEPIN_CheckedChanged);
            // 
            // cbSpiMISOPIN
            // 
            resources.ApplyResources(this.cbSpiMISOPIN, "cbSpiMISOPIN");
            this.cbSpiMISOPIN.Name = "cbSpiMISOPIN";
            this.cbSpiMISOPIN.UseVisualStyleBackColor = true;
            this.cbSpiMISOPIN.CheckedChanged += new System.EventHandler(this.cbSpiMISOPIN_CheckedChanged);
            // 
            // cbSpiEndian
            // 
            resources.ApplyResources(this.cbSpiEndian, "cbSpiEndian");
            this.cbSpiEndian.Name = "cbSpiEndian";
            this.cbSpiEndian.UseVisualStyleBackColor = true;
            this.cbSpiEndian.CheckedChanged += new System.EventHandler(this.cbSpiEndian_CheckedChanged);
            // 
            // cbSpiCPOL
            // 
            resources.ApplyResources(this.cbSpiCPOL, "cbSpiCPOL");
            this.cbSpiCPOL.Name = "cbSpiCPOL";
            this.cbSpiCPOL.UseVisualStyleBackColor = true;
            this.cbSpiCPOL.CheckedChanged += new System.EventHandler(this.cbSpiCPOL_CheckedChanged);
            // 
            // cbSpiCPHA
            // 
            resources.ApplyResources(this.cbSpiCPHA, "cbSpiCPHA");
            this.cbSpiCPHA.Name = "cbSpiCPHA";
            this.cbSpiCPHA.UseVisualStyleBackColor = true;
            this.cbSpiCPHA.CheckedChanged += new System.EventHandler(this.cbSpiCPHA_CheckedChanged);
            // 
            // tabEPCS
            // 
            this.tabEPCS.Controls.Add(this.gbEpcsWriting);
            this.tabEPCS.Controls.Add(this.gbEpcsGeneral);
            resources.ApplyResources(this.tabEPCS, "tabEPCS");
            this.tabEPCS.Name = "tabEPCS";
            this.tabEPCS.UseVisualStyleBackColor = true;
            // 
            // gbEpcsWriting
            // 
            resources.ApplyResources(this.gbEpcsWriting, "gbEpcsWriting");
            this.gbEpcsWriting.Controls.Add(this.bEpcsErase);
            this.gbEpcsWriting.Controls.Add(this.bEpcsVerify);
            this.gbEpcsWriting.Controls.Add(this.tbEpcsFilePath);
            this.gbEpcsWriting.Controls.Add(this.bEpcsBrowse);
            this.gbEpcsWriting.Controls.Add(this.label60);
            this.gbEpcsWriting.Controls.Add(this.bEpcsWrite);
            this.gbEpcsWriting.Name = "gbEpcsWriting";
            this.gbEpcsWriting.TabStop = false;
            // 
            // bEpcsErase
            // 
            resources.ApplyResources(this.bEpcsErase, "bEpcsErase");
            this.bEpcsErase.Name = "bEpcsErase";
            this.bEpcsErase.UseVisualStyleBackColor = true;
            this.bEpcsErase.Click += new System.EventHandler(this.bEpcsErase_Click);
            // 
            // bEpcsVerify
            // 
            resources.ApplyResources(this.bEpcsVerify, "bEpcsVerify");
            this.bEpcsVerify.Name = "bEpcsVerify";
            this.bEpcsVerify.UseVisualStyleBackColor = true;
            this.bEpcsVerify.Click += new System.EventHandler(this.bEpcsVerify_Click);
            // 
            // tbEpcsFilePath
            // 
            resources.ApplyResources(this.tbEpcsFilePath, "tbEpcsFilePath");
            this.tbEpcsFilePath.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "EpcsFilePath", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbEpcsFilePath.Name = "tbEpcsFilePath";
            this.tbEpcsFilePath.Text = global::QuickUsbDiagCs.Properties.Settings.Default.EpcsFilePath;
            // 
            // bEpcsBrowse
            // 
            resources.ApplyResources(this.bEpcsBrowse, "bEpcsBrowse");
            this.bEpcsBrowse.Name = "bEpcsBrowse";
            this.bEpcsBrowse.UseVisualStyleBackColor = true;
            this.bEpcsBrowse.Click += new System.EventHandler(this.bEpcsBrowse_Click);
            // 
            // label60
            // 
            resources.ApplyResources(this.label60, "label60");
            this.label60.Name = "label60";
            // 
            // bEpcsWrite
            // 
            resources.ApplyResources(this.bEpcsWrite, "bEpcsWrite");
            this.bEpcsWrite.Name = "bEpcsWrite";
            this.bEpcsWrite.UseVisualStyleBackColor = true;
            this.bEpcsWrite.Click += new System.EventHandler(this.bEpcsWrite_Click);
            // 
            // gbEpcsGeneral
            // 
            resources.ApplyResources(this.gbEpcsGeneral, "gbEpcsGeneral");
            this.gbEpcsGeneral.Controls.Add(this.labelEpcsIdentify);
            this.gbEpcsGeneral.Controls.Add(this.bEpcsIdentify);
            this.gbEpcsGeneral.Controls.Add(this.cbEpcsPort);
            this.gbEpcsGeneral.Controls.Add(this.label61);
            this.gbEpcsGeneral.Name = "gbEpcsGeneral";
            this.gbEpcsGeneral.TabStop = false;
            // 
            // labelEpcsIdentify
            // 
            resources.ApplyResources(this.labelEpcsIdentify, "labelEpcsIdentify");
            this.labelEpcsIdentify.Name = "labelEpcsIdentify";
            // 
            // bEpcsIdentify
            // 
            resources.ApplyResources(this.bEpcsIdentify, "bEpcsIdentify");
            this.bEpcsIdentify.Name = "bEpcsIdentify";
            this.bEpcsIdentify.UseVisualStyleBackColor = true;
            this.bEpcsIdentify.Click += new System.EventHandler(this.bEpcsIdentify_Click);
            // 
            // cbEpcsPort
            // 
            this.cbEpcsPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbEpcsPort.FormattingEnabled = true;
            this.cbEpcsPort.Items.AddRange(new object[] {
            resources.GetString("cbEpcsPort.Items"),
            resources.GetString("cbEpcsPort.Items1"),
            resources.GetString("cbEpcsPort.Items2"),
            resources.GetString("cbEpcsPort.Items3"),
            resources.GetString("cbEpcsPort.Items4"),
            resources.GetString("cbEpcsPort.Items5"),
            resources.GetString("cbEpcsPort.Items6"),
            resources.GetString("cbEpcsPort.Items7"),
            resources.GetString("cbEpcsPort.Items8"),
            resources.GetString("cbEpcsPort.Items9")});
            resources.ApplyResources(this.cbEpcsPort, "cbEpcsPort");
            this.cbEpcsPort.Name = "cbEpcsPort";
            // 
            // label61
            // 
            resources.ApplyResources(this.label61, "label61");
            this.label61.Name = "label61";
            // 
            // tabI2C
            // 
            this.tabI2C.Controls.Add(this.gbI2CWrite);
            this.tabI2C.Controls.Add(this.gbI2CRead);
            this.tabI2C.Controls.Add(this.gbI2CGeneral);
            resources.ApplyResources(this.tabI2C, "tabI2C");
            this.tabI2C.Name = "tabI2C";
            this.tabI2C.UseVisualStyleBackColor = true;
            // 
            // gbI2CWrite
            // 
            resources.ApplyResources(this.gbI2CWrite, "gbI2CWrite");
            this.gbI2CWrite.Controls.Add(this.bI2cCachedWrite);
            this.gbI2CWrite.Controls.Add(this.label39);
            this.gbI2CWrite.Controls.Add(this.tbI2cWriteAddress);
            this.gbI2CWrite.Controls.Add(this.tbI2cWriteData);
            this.gbI2CWrite.Controls.Add(this.i2cWrite);
            this.gbI2CWrite.Name = "gbI2CWrite";
            this.gbI2CWrite.TabStop = false;
            // 
            // bI2cCachedWrite
            // 
            resources.ApplyResources(this.bI2cCachedWrite, "bI2cCachedWrite");
            this.bI2cCachedWrite.Name = "bI2cCachedWrite";
            this.bI2cCachedWrite.UseVisualStyleBackColor = true;
            this.bI2cCachedWrite.Click += new System.EventHandler(this.bI2cCachedWrite_Click);
            // 
            // label39
            // 
            resources.ApplyResources(this.label39, "label39");
            this.label39.Name = "label39";
            // 
            // tbI2cWriteAddress
            // 
            this.tbI2cWriteAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "I2cWriteAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbI2cWriteAddress, "tbI2cWriteAddress");
            this.tbI2cWriteAddress.Name = "tbI2cWriteAddress";
            this.tbI2cWriteAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.I2cWriteAddress;
            // 
            // tbI2cWriteData
            // 
            resources.ApplyResources(this.tbI2cWriteData, "tbI2cWriteData");
            this.tbI2cWriteData.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "I2cWriteData", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbI2cWriteData.Name = "tbI2cWriteData";
            this.tbI2cWriteData.Text = global::QuickUsbDiagCs.Properties.Settings.Default.I2cWriteData;
            // 
            // i2cWrite
            // 
            resources.ApplyResources(this.i2cWrite, "i2cWrite");
            this.i2cWrite.Name = "i2cWrite";
            this.i2cWrite.UseVisualStyleBackColor = true;
            this.i2cWrite.Click += new System.EventHandler(this.bI2cWrite_Click);
            // 
            // gbI2CRead
            // 
            resources.ApplyResources(this.gbI2CRead, "gbI2CRead");
            this.gbI2CRead.Controls.Add(this.tbI2cBytes);
            this.gbI2CRead.Controls.Add(this.label38);
            this.gbI2CRead.Controls.Add(this.label36);
            this.gbI2CRead.Controls.Add(this.tbI2cReadAddress);
            this.gbI2CRead.Controls.Add(this.i2cRead);
            this.gbI2CRead.Controls.Add(this.tbI2cReadData);
            this.gbI2CRead.Name = "gbI2CRead";
            this.gbI2CRead.TabStop = false;
            // 
            // tbI2cBytes
            // 
            this.tbI2cBytes.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "I2cReadBytes", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbI2cBytes, "tbI2cBytes");
            this.tbI2cBytes.Name = "tbI2cBytes";
            this.tbI2cBytes.Text = global::QuickUsbDiagCs.Properties.Settings.Default.I2cReadBytes;
            // 
            // label38
            // 
            resources.ApplyResources(this.label38, "label38");
            this.label38.Name = "label38";
            // 
            // label36
            // 
            resources.ApplyResources(this.label36, "label36");
            this.label36.Name = "label36";
            // 
            // tbI2cReadAddress
            // 
            this.tbI2cReadAddress.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::QuickUsbDiagCs.Properties.Settings.Default, "I2cReadAddress", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            resources.ApplyResources(this.tbI2cReadAddress, "tbI2cReadAddress");
            this.tbI2cReadAddress.Name = "tbI2cReadAddress";
            this.tbI2cReadAddress.Text = global::QuickUsbDiagCs.Properties.Settings.Default.I2cReadAddress;
            // 
            // i2cRead
            // 
            resources.ApplyResources(this.i2cRead, "i2cRead");
            this.i2cRead.Name = "i2cRead";
            this.i2cRead.UseVisualStyleBackColor = true;
            this.i2cRead.Click += new System.EventHandler(this.bI2cRead_Click);
            // 
            // tbI2cReadData
            // 
            resources.ApplyResources(this.tbI2cReadData, "tbI2cReadData");
            this.tbI2cReadData.Name = "tbI2cReadData";
            this.tbI2cReadData.ReadOnly = true;
            // 
            // gbI2CGeneral
            // 
            resources.ApplyResources(this.gbI2CGeneral, "gbI2CGeneral");
            this.gbI2CGeneral.Controls.Add(this.cbI2cIgnoreAck);
            this.gbI2CGeneral.Controls.Add(this.cbI2cBusSpeed);
            this.gbI2CGeneral.Controls.Add(this.label37);
            this.gbI2CGeneral.Name = "gbI2CGeneral";
            this.gbI2CGeneral.TabStop = false;
            // 
            // cbI2cIgnoreAck
            // 
            resources.ApplyResources(this.cbI2cIgnoreAck, "cbI2cIgnoreAck");
            this.cbI2cIgnoreAck.Name = "cbI2cIgnoreAck";
            this.cbI2cIgnoreAck.UseVisualStyleBackColor = true;
            this.cbI2cIgnoreAck.CheckedChanged += new System.EventHandler(this.cbI2cIgnoreAck_CheckedChanged);
            // 
            // cbI2cBusSpeed
            // 
            this.cbI2cBusSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbI2cBusSpeed.FormattingEnabled = true;
            this.cbI2cBusSpeed.Items.AddRange(new object[] {
            resources.GetString("cbI2cBusSpeed.Items"),
            resources.GetString("cbI2cBusSpeed.Items1")});
            resources.ApplyResources(this.cbI2cBusSpeed, "cbI2cBusSpeed");
            this.cbI2cBusSpeed.Name = "cbI2cBusSpeed";
            this.cbI2cBusSpeed.SelectedIndexChanged += new System.EventHandler(this.cbI2cBusSpeed_SelectedIndexChanged);
            // 
            // label37
            // 
            resources.ApplyResources(this.label37, "label37");
            this.label37.Name = "label37";
            // 
            // tabSettings
            // 
            this.tabSettings.Controls.Add(this.gbSettings);
            resources.ApplyResources(this.tabSettings, "tabSettings");
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.UseVisualStyleBackColor = true;
            // 
            // gbSettings
            // 
            resources.ApplyResources(this.gbSettings, "gbSettings");
            this.gbSettings.Controls.Add(this.epcfgBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting8_2);
            this.gbSettings.Controls.Add(this.wordWideBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting7_2);
            this.gbSettings.Controls.Add(this.dataAddressBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting0_2);
            this.gbSettings.Controls.Add(this.fpgaTypeBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting15_2);
            this.gbSettings.Controls.Add(this.cpuConfigBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSettingLSB);
            this.gbSettings.Controls.Add(this.spiConfigBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSettingMSB);
            this.gbSettings.Controls.Add(this.slaveFifoBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting8_1);
            this.gbSettings.Controls.Add(this.i2ctlBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting7_1);
            this.gbSettings.Controls.Add(this.ioaBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting0_1);
            this.gbSettings.Controls.Add(this.iobBitTwiddler);
            this.gbSettings.Controls.Add(this.labelSetting15_1);
            this.gbSettings.Controls.Add(this.iocBitTwiddler);
            this.gbSettings.Controls.Add(this.ifconfigBitTwiddler);
            this.gbSettings.Controls.Add(this.iodBitTwiddler);
            this.gbSettings.Controls.Add(this.pinFlagsBitTwiddler);
            this.gbSettings.Controls.Add(this.ioeBitTwiddler);
            this.gbSettings.Controls.Add(this.portAcfgBitTwiddler);
            this.gbSettings.Name = "gbSettings";
            this.gbSettings.TabStop = false;
            // 
            // labelSetting8_2
            // 
            resources.ApplyResources(this.labelSetting8_2, "labelSetting8_2");
            this.labelSetting8_2.Name = "labelSetting8_2";
            // 
            // labelSetting7_2
            // 
            resources.ApplyResources(this.labelSetting7_2, "labelSetting7_2");
            this.labelSetting7_2.Name = "labelSetting7_2";
            // 
            // labelSetting0_2
            // 
            resources.ApplyResources(this.labelSetting0_2, "labelSetting0_2");
            this.labelSetting0_2.Name = "labelSetting0_2";
            // 
            // labelSetting15_2
            // 
            resources.ApplyResources(this.labelSetting15_2, "labelSetting15_2");
            this.labelSetting15_2.Name = "labelSetting15_2";
            // 
            // labelSettingLSB
            // 
            resources.ApplyResources(this.labelSettingLSB, "labelSettingLSB");
            this.labelSettingLSB.Name = "labelSettingLSB";
            // 
            // labelSettingMSB
            // 
            resources.ApplyResources(this.labelSettingMSB, "labelSettingMSB");
            this.labelSettingMSB.Name = "labelSettingMSB";
            // 
            // labelSetting8_1
            // 
            resources.ApplyResources(this.labelSetting8_1, "labelSetting8_1");
            this.labelSetting8_1.Name = "labelSetting8_1";
            // 
            // labelSetting7_1
            // 
            resources.ApplyResources(this.labelSetting7_1, "labelSetting7_1");
            this.labelSetting7_1.Name = "labelSetting7_1";
            // 
            // labelSetting0_1
            // 
            resources.ApplyResources(this.labelSetting0_1, "labelSetting0_1");
            this.labelSetting0_1.Name = "labelSetting0_1";
            // 
            // labelSetting15_1
            // 
            resources.ApplyResources(this.labelSetting15_1, "labelSetting15_1");
            this.labelSetting15_1.Name = "labelSetting15_1";
            // 
            // tabDefaults
            // 
            this.tabDefaults.Controls.Add(this.gbDefaults);
            resources.ApplyResources(this.tabDefaults, "tabDefaults");
            this.tabDefaults.Name = "tabDefaults";
            this.tabDefaults.UseVisualStyleBackColor = true;
            // 
            // gbDefaults
            // 
            resources.ApplyResources(this.gbDefaults, "gbDefaults");
            this.gbDefaults.Controls.Add(this.epcfgDBitTwiddler);
            this.gbDefaults.Controls.Add(this.labelDefault8_2);
            this.gbDefaults.Controls.Add(this.bitTwiddler26);
            this.gbDefaults.Controls.Add(this.labelDefault7_2);
            this.gbDefaults.Controls.Add(this.bitTwiddler27);
            this.gbDefaults.Controls.Add(this.labelDefault0_2);
            this.gbDefaults.Controls.Add(this.bitTwiddler28);
            this.gbDefaults.Controls.Add(this.labelDefault15_2);
            this.gbDefaults.Controls.Add(this.bitTwiddler29);
            this.gbDefaults.Controls.Add(this.labelDefaultLSB);
            this.gbDefaults.Controls.Add(this.bitTwiddler30);
            this.gbDefaults.Controls.Add(this.labelDefaultMSB);
            this.gbDefaults.Controls.Add(this.bitTwiddler31);
            this.gbDefaults.Controls.Add(this.labelDefault8_1);
            this.gbDefaults.Controls.Add(this.bitTwiddler32);
            this.gbDefaults.Controls.Add(this.labelDefault7_1);
            this.gbDefaults.Controls.Add(this.bitTwiddler33);
            this.gbDefaults.Controls.Add(this.labelDefault0_1);
            this.gbDefaults.Controls.Add(this.bitTwiddler34);
            this.gbDefaults.Controls.Add(this.labelDefault15_1);
            this.gbDefaults.Controls.Add(this.bitTwiddler35);
            this.gbDefaults.Controls.Add(this.bitTwiddler36);
            this.gbDefaults.Controls.Add(this.bitTwiddler37);
            this.gbDefaults.Controls.Add(this.bitTwiddler38);
            this.gbDefaults.Controls.Add(this.bitTwiddler39);
            this.gbDefaults.Controls.Add(this.bitTwiddler40);
            this.gbDefaults.Name = "gbDefaults";
            this.gbDefaults.TabStop = false;
            // 
            // labelDefault8_2
            // 
            resources.ApplyResources(this.labelDefault8_2, "labelDefault8_2");
            this.labelDefault8_2.Name = "labelDefault8_2";
            // 
            // labelDefault7_2
            // 
            resources.ApplyResources(this.labelDefault7_2, "labelDefault7_2");
            this.labelDefault7_2.Name = "labelDefault7_2";
            // 
            // labelDefault0_2
            // 
            resources.ApplyResources(this.labelDefault0_2, "labelDefault0_2");
            this.labelDefault0_2.Name = "labelDefault0_2";
            // 
            // labelDefault15_2
            // 
            resources.ApplyResources(this.labelDefault15_2, "labelDefault15_2");
            this.labelDefault15_2.Name = "labelDefault15_2";
            // 
            // labelDefaultLSB
            // 
            resources.ApplyResources(this.labelDefaultLSB, "labelDefaultLSB");
            this.labelDefaultLSB.Name = "labelDefaultLSB";
            // 
            // labelDefaultMSB
            // 
            resources.ApplyResources(this.labelDefaultMSB, "labelDefaultMSB");
            this.labelDefaultMSB.Name = "labelDefaultMSB";
            // 
            // labelDefault8_1
            // 
            resources.ApplyResources(this.labelDefault8_1, "labelDefault8_1");
            this.labelDefault8_1.Name = "labelDefault8_1";
            // 
            // labelDefault7_1
            // 
            resources.ApplyResources(this.labelDefault7_1, "labelDefault7_1");
            this.labelDefault7_1.Name = "labelDefault7_1";
            // 
            // labelDefault0_1
            // 
            resources.ApplyResources(this.labelDefault0_1, "labelDefault0_1");
            this.labelDefault0_1.Name = "labelDefault0_1";
            // 
            // labelDefault15_1
            // 
            resources.ApplyResources(this.labelDefault15_1, "labelDefault15_1");
            this.labelDefault15_1.Name = "labelDefault15_1";
            // 
            // groupBox24
            // 
            this.groupBox24.Controls.Add(this.bitTwiddler9);
            this.groupBox24.Controls.Add(this.label40);
            this.groupBox24.Controls.Add(this.bitTwiddler10);
            this.groupBox24.Controls.Add(this.label41);
            this.groupBox24.Controls.Add(this.bitTwiddler11);
            this.groupBox24.Controls.Add(this.label42);
            this.groupBox24.Controls.Add(this.bitTwiddler12);
            this.groupBox24.Controls.Add(this.label43);
            this.groupBox24.Controls.Add(this.bitTwiddler13);
            this.groupBox24.Controls.Add(this.label44);
            this.groupBox24.Controls.Add(this.bitTwiddler14);
            this.groupBox24.Controls.Add(this.label45);
            this.groupBox24.Controls.Add(this.bitTwiddler15);
            this.groupBox24.Controls.Add(this.label46);
            this.groupBox24.Controls.Add(this.bitTwiddler16);
            this.groupBox24.Controls.Add(this.label47);
            this.groupBox24.Controls.Add(this.bitTwiddler17);
            this.groupBox24.Controls.Add(this.label48);
            this.groupBox24.Controls.Add(this.bitTwiddler18);
            this.groupBox24.Controls.Add(this.label49);
            this.groupBox24.Controls.Add(this.bitTwiddler19);
            this.groupBox24.Controls.Add(this.bitTwiddler20);
            this.groupBox24.Controls.Add(this.bitTwiddler21);
            this.groupBox24.Controls.Add(this.bitTwiddler22);
            this.groupBox24.Controls.Add(this.bitTwiddler23);
            this.groupBox24.Controls.Add(this.bitTwiddler24);
            resources.ApplyResources(this.groupBox24, "groupBox24");
            this.groupBox24.Name = "groupBox24";
            this.groupBox24.TabStop = false;
            // 
            // label40
            // 
            resources.ApplyResources(this.label40, "label40");
            this.label40.Name = "label40";
            // 
            // label41
            // 
            resources.ApplyResources(this.label41, "label41");
            this.label41.Name = "label41";
            // 
            // label42
            // 
            resources.ApplyResources(this.label42, "label42");
            this.label42.Name = "label42";
            // 
            // label43
            // 
            resources.ApplyResources(this.label43, "label43");
            this.label43.Name = "label43";
            // 
            // label44
            // 
            resources.ApplyResources(this.label44, "label44");
            this.label44.Name = "label44";
            // 
            // label45
            // 
            resources.ApplyResources(this.label45, "label45");
            this.label45.Name = "label45";
            // 
            // label46
            // 
            resources.ApplyResources(this.label46, "label46");
            this.label46.Name = "label46";
            // 
            // label47
            // 
            resources.ApplyResources(this.label47, "label47");
            this.label47.Name = "label47";
            // 
            // label48
            // 
            resources.ApplyResources(this.label48, "label48");
            this.label48.Name = "label48";
            // 
            // label49
            // 
            resources.ApplyResources(this.label49, "label49");
            this.label49.Name = "label49";
            // 
            // timerRepeat
            // 
            this.timerRepeat.Tick += new System.EventHandler(this.timerRepeat_Tick);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.Name = "label6";
            // 
            // lLabelDeviceSpeed
            // 
            resources.ApplyResources(this.lLabelDeviceSpeed, "lLabelDeviceSpeed");
            this.lLabelDeviceSpeed.Name = "lLabelDeviceSpeed";
            // 
            // quickUsbWinForms
            // 
            resources.ApplyResources(this.quickUsbWinForms, "quickUsbWinForms");
            this.quickUsbWinForms.AutoSelectFirstItem = true;
            this.quickUsbWinForms.MinimumSize = new System.Drawing.Size(133, 49);
            this.quickUsbWinForms.Name = "quickUsbWinForms";
            this.quickUsbWinForms.ScanEnabled = true;
            this.quickUsbWinForms.ScanInterval = 500;
            this.quickUsbWinForms.ScanOnLoad = true;
            this.quickUsbWinForms.Scrollable = true;
            quickUsb1.Name = "QUSB-0";
            this.quickUsbWinForms.SelectedModule = quickUsb1;
            this.quickUsbWinForms.SelectedModules = new BitwiseSystems.QuickUsb[] {
        quickUsb1};
            this.quickUsbWinForms.ShowUnprogrammedDevices = false;
            this.quickUsbWinForms.View = System.Windows.Forms.View.Details;
            this.quickUsbWinForms.ModuleConnected += new BitwiseSystems.QuickUsbWinForms.ConnectedHandler(this.quickUsbWinForms_ModuleConnected);
            this.quickUsbWinForms.SelectionChanged += new BitwiseSystems.QuickUsbWinForms.SelectionChangedHandler(this.quickUsbWinForms_SelectionChanged);
            // 
            // btPortEDir
            // 
            resources.ApplyResources(this.btPortEDir, "btPortEDir");
            this.btPortEDir.Bits = ((byte)(8));
            this.btPortEDir.Caption = "Direction:";
            this.btPortEDir.CaptionWidth = 50;
            this.btPortEDir.Name = "btPortEDir";
            this.btPortEDir.Tag = "portADir";
            this.btPortEDir.TagInt = 4;
            this.btPortEDir.ToolTips = null;
            this.btPortEDir.Value = ((uint)(0u));
            this.btPortEDir.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portDir_ValueChanged);
            // 
            // btPortEVal
            // 
            resources.ApplyResources(this.btPortEVal, "btPortEVal");
            this.btPortEVal.Bits = ((byte)(8));
            this.btPortEVal.Caption = "Value:";
            this.btPortEVal.CaptionWidth = 50;
            this.btPortEVal.Name = "btPortEVal";
            this.btPortEVal.Tag = "portAVal";
            this.btPortEVal.TagInt = 4;
            this.btPortEVal.ToolTips = null;
            this.btPortEVal.Value = ((uint)(0u));
            this.btPortEVal.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portVal_ValueChanged);
            // 
            // btPortDDir
            // 
            resources.ApplyResources(this.btPortDDir, "btPortDDir");
            this.btPortDDir.Bits = ((byte)(8));
            this.btPortDDir.Caption = "Direction:";
            this.btPortDDir.CaptionWidth = 50;
            this.btPortDDir.Name = "btPortDDir";
            this.btPortDDir.Tag = "portADir";
            this.btPortDDir.TagInt = 3;
            this.btPortDDir.ToolTips = null;
            this.btPortDDir.Value = ((uint)(0u));
            this.btPortDDir.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portDir_ValueChanged);
            // 
            // btPortDVal
            // 
            resources.ApplyResources(this.btPortDVal, "btPortDVal");
            this.btPortDVal.Bits = ((byte)(8));
            this.btPortDVal.Caption = "Value:";
            this.btPortDVal.CaptionWidth = 50;
            this.btPortDVal.Name = "btPortDVal";
            this.btPortDVal.Tag = "portAVal";
            this.btPortDVal.TagInt = 3;
            this.btPortDVal.ToolTips = null;
            this.btPortDVal.Value = ((uint)(0u));
            this.btPortDVal.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portVal_ValueChanged);
            // 
            // btPortCDir
            // 
            resources.ApplyResources(this.btPortCDir, "btPortCDir");
            this.btPortCDir.Bits = ((byte)(8));
            this.btPortCDir.Caption = "Direction:";
            this.btPortCDir.CaptionWidth = 50;
            this.btPortCDir.Name = "btPortCDir";
            this.btPortCDir.Tag = "portADir";
            this.btPortCDir.TagInt = 2;
            this.btPortCDir.ToolTips = null;
            this.btPortCDir.Value = ((uint)(0u));
            this.btPortCDir.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portDir_ValueChanged);
            // 
            // btPortCVal
            // 
            resources.ApplyResources(this.btPortCVal, "btPortCVal");
            this.btPortCVal.Bits = ((byte)(8));
            this.btPortCVal.Caption = "Value:";
            this.btPortCVal.CaptionWidth = 50;
            this.btPortCVal.Name = "btPortCVal";
            this.btPortCVal.Tag = "portAVal";
            this.btPortCVal.TagInt = 2;
            this.btPortCVal.ToolTips = null;
            this.btPortCVal.Value = ((uint)(0u));
            this.btPortCVal.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portVal_ValueChanged);
            // 
            // btPortBDir
            // 
            resources.ApplyResources(this.btPortBDir, "btPortBDir");
            this.btPortBDir.Bits = ((byte)(8));
            this.btPortBDir.Caption = "Direction:";
            this.btPortBDir.CaptionWidth = 50;
            this.btPortBDir.Name = "btPortBDir";
            this.btPortBDir.Tag = "portADir";
            this.btPortBDir.TagInt = 1;
            this.btPortBDir.ToolTips = null;
            this.btPortBDir.Value = ((uint)(0u));
            this.btPortBDir.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portDir_ValueChanged);
            // 
            // btPortBVal
            // 
            resources.ApplyResources(this.btPortBVal, "btPortBVal");
            this.btPortBVal.Bits = ((byte)(8));
            this.btPortBVal.Caption = "Value:";
            this.btPortBVal.CaptionWidth = 50;
            this.btPortBVal.Name = "btPortBVal";
            this.btPortBVal.Tag = "portAVal";
            this.btPortBVal.TagInt = 1;
            this.btPortBVal.ToolTips = null;
            this.btPortBVal.Value = ((uint)(0u));
            this.btPortBVal.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portVal_ValueChanged);
            // 
            // btPortADir
            // 
            resources.ApplyResources(this.btPortADir, "btPortADir");
            this.btPortADir.Bits = ((byte)(8));
            this.btPortADir.Caption = "Direction:";
            this.btPortADir.CaptionWidth = 50;
            this.btPortADir.Name = "btPortADir";
            this.btPortADir.Tag = "portADir";
            this.btPortADir.TagInt = 0;
            this.btPortADir.ToolTips = null;
            this.btPortADir.Value = ((uint)(0u));
            this.btPortADir.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portDir_ValueChanged);
            // 
            // btPortAVal
            // 
            resources.ApplyResources(this.btPortAVal, "btPortAVal");
            this.btPortAVal.Bits = ((byte)(8));
            this.btPortAVal.Caption = "Value:";
            this.btPortAVal.CaptionWidth = 50;
            this.btPortAVal.Name = "btPortAVal";
            this.btPortAVal.Tag = "portAVal";
            this.btPortAVal.TagInt = 0;
            this.btPortAVal.ToolTips = null;
            this.btPortAVal.Value = ((uint)(0u));
            this.btPortAVal.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.portVal_ValueChanged);
            // 
            // epcfgBitTwiddler
            // 
            resources.ApplyResources(this.epcfgBitTwiddler, "epcfgBitTwiddler");
            this.epcfgBitTwiddler.Bits = ((byte)(16));
            this.epcfgBitTwiddler.Caption = "EP2CFG/EP6CFG";
            this.epcfgBitTwiddler.CaptionWidth = 175;
            this.epcfgBitTwiddler.Name = "epcfgBitTwiddler";
            this.epcfgBitTwiddler.TagInt = 0;
            this.epcfgBitTwiddler.ToolTips = null;
            this.epcfgBitTwiddler.Value = ((uint)(0u));
            this.epcfgBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // wordWideBitTwiddler
            // 
            resources.ApplyResources(this.wordWideBitTwiddler, "wordWideBitTwiddler");
            this.wordWideBitTwiddler.Bits = ((byte)(16));
            this.wordWideBitTwiddler.Caption = "WORDWIDE";
            this.wordWideBitTwiddler.CaptionWidth = 175;
            this.wordWideBitTwiddler.Name = "wordWideBitTwiddler";
            this.wordWideBitTwiddler.TagInt = 1;
            this.wordWideBitTwiddler.ToolTips = null;
            this.wordWideBitTwiddler.Value = ((uint)(0u));
            this.wordWideBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // dataAddressBitTwiddler
            // 
            resources.ApplyResources(this.dataAddressBitTwiddler, "dataAddressBitTwiddler");
            this.dataAddressBitTwiddler.Bits = ((byte)(16));
            this.dataAddressBitTwiddler.Caption = "DataAddress";
            this.dataAddressBitTwiddler.CaptionWidth = 175;
            this.dataAddressBitTwiddler.Name = "dataAddressBitTwiddler";
            this.dataAddressBitTwiddler.TagInt = 2;
            this.dataAddressBitTwiddler.ToolTips = null;
            this.dataAddressBitTwiddler.Value = ((uint)(0u));
            this.dataAddressBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // fpgaTypeBitTwiddler
            // 
            resources.ApplyResources(this.fpgaTypeBitTwiddler, "fpgaTypeBitTwiddler");
            this.fpgaTypeBitTwiddler.Bits = ((byte)(16));
            this.fpgaTypeBitTwiddler.Caption = "FPGATYPE";
            this.fpgaTypeBitTwiddler.CaptionWidth = 175;
            this.fpgaTypeBitTwiddler.Name = "fpgaTypeBitTwiddler";
            this.fpgaTypeBitTwiddler.TagInt = 4;
            this.fpgaTypeBitTwiddler.ToolTips = null;
            this.fpgaTypeBitTwiddler.Value = ((uint)(0u));
            this.fpgaTypeBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // cpuConfigBitTwiddler
            // 
            resources.ApplyResources(this.cpuConfigBitTwiddler, "cpuConfigBitTwiddler");
            this.cpuConfigBitTwiddler.Bits = ((byte)(16));
            this.cpuConfigBitTwiddler.Caption = "BusSpeed/CPUCONFIG";
            this.cpuConfigBitTwiddler.CaptionWidth = 175;
            this.cpuConfigBitTwiddler.Name = "cpuConfigBitTwiddler";
            this.cpuConfigBitTwiddler.TagInt = 5;
            this.cpuConfigBitTwiddler.ToolTips = null;
            this.cpuConfigBitTwiddler.Value = ((uint)(0u));
            this.cpuConfigBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // spiConfigBitTwiddler
            // 
            resources.ApplyResources(this.spiConfigBitTwiddler, "spiConfigBitTwiddler");
            this.spiConfigBitTwiddler.Bits = ((byte)(16));
            this.spiConfigBitTwiddler.Caption = "PORTECFG/SPICONFIG";
            this.spiConfigBitTwiddler.CaptionWidth = 175;
            this.spiConfigBitTwiddler.Name = "spiConfigBitTwiddler";
            this.spiConfigBitTwiddler.TagInt = 6;
            this.spiConfigBitTwiddler.ToolTips = null;
            this.spiConfigBitTwiddler.Value = ((uint)(0u));
            this.spiConfigBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // slaveFifoBitTwiddler
            // 
            resources.ApplyResources(this.slaveFifoBitTwiddler, "slaveFifoBitTwiddler");
            this.slaveFifoBitTwiddler.Bits = ((byte)(16));
            this.slaveFifoBitTwiddler.Caption = "SlaveFIFO";
            this.slaveFifoBitTwiddler.CaptionWidth = 175;
            this.slaveFifoBitTwiddler.Name = "slaveFifoBitTwiddler";
            this.slaveFifoBitTwiddler.TagInt = 7;
            this.slaveFifoBitTwiddler.ToolTips = null;
            this.slaveFifoBitTwiddler.Value = ((uint)(0u));
            this.slaveFifoBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // i2ctlBitTwiddler
            // 
            resources.ApplyResources(this.i2ctlBitTwiddler, "i2ctlBitTwiddler");
            this.i2ctlBitTwiddler.Bits = ((byte)(16));
            this.i2ctlBitTwiddler.Caption = "I2CTL";
            this.i2ctlBitTwiddler.CaptionWidth = 175;
            this.i2ctlBitTwiddler.Name = "i2ctlBitTwiddler";
            this.i2ctlBitTwiddler.TagInt = 8;
            this.i2ctlBitTwiddler.ToolTips = null;
            this.i2ctlBitTwiddler.Value = ((uint)(0u));
            this.i2ctlBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // ioaBitTwiddler
            // 
            resources.ApplyResources(this.ioaBitTwiddler, "ioaBitTwiddler");
            this.ioaBitTwiddler.Bits = ((byte)(16));
            this.ioaBitTwiddler.Caption = "OEA/IOA";
            this.ioaBitTwiddler.CaptionWidth = 175;
            this.ioaBitTwiddler.Name = "ioaBitTwiddler";
            this.ioaBitTwiddler.TagInt = 9;
            this.ioaBitTwiddler.ToolTips = null;
            this.ioaBitTwiddler.Value = ((uint)(0u));
            this.ioaBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // iobBitTwiddler
            // 
            resources.ApplyResources(this.iobBitTwiddler, "iobBitTwiddler");
            this.iobBitTwiddler.Bits = ((byte)(16));
            this.iobBitTwiddler.Caption = "OEB/IOB";
            this.iobBitTwiddler.CaptionWidth = 175;
            this.iobBitTwiddler.Name = "iobBitTwiddler";
            this.iobBitTwiddler.TagInt = 10;
            this.iobBitTwiddler.ToolTips = null;
            this.iobBitTwiddler.Value = ((uint)(0u));
            this.iobBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // iocBitTwiddler
            // 
            resources.ApplyResources(this.iocBitTwiddler, "iocBitTwiddler");
            this.iocBitTwiddler.Bits = ((byte)(16));
            this.iocBitTwiddler.Caption = "OEC/IOC";
            this.iocBitTwiddler.CaptionWidth = 175;
            this.iocBitTwiddler.Name = "iocBitTwiddler";
            this.iocBitTwiddler.TagInt = 11;
            this.iocBitTwiddler.ToolTips = null;
            this.iocBitTwiddler.Value = ((uint)(0u));
            this.iocBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // ifconfigBitTwiddler
            // 
            resources.ApplyResources(this.ifconfigBitTwiddler, "ifconfigBitTwiddler");
            this.ifconfigBitTwiddler.Bits = ((byte)(16));
            this.ifconfigBitTwiddler.Caption = "FIFOPolar/IFCONFIG";
            this.ifconfigBitTwiddler.CaptionWidth = 175;
            this.ifconfigBitTwiddler.Name = "ifconfigBitTwiddler";
            this.ifconfigBitTwiddler.TagInt = 3;
            this.ifconfigBitTwiddler.ToolTips = null;
            this.ifconfigBitTwiddler.Value = ((uint)(0u));
            this.ifconfigBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // iodBitTwiddler
            // 
            resources.ApplyResources(this.iodBitTwiddler, "iodBitTwiddler");
            this.iodBitTwiddler.Bits = ((byte)(16));
            this.iodBitTwiddler.Caption = "OED/IOD";
            this.iodBitTwiddler.CaptionWidth = 175;
            this.iodBitTwiddler.Name = "iodBitTwiddler";
            this.iodBitTwiddler.TagInt = 12;
            this.iodBitTwiddler.ToolTips = null;
            this.iodBitTwiddler.Value = ((uint)(0u));
            this.iodBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // pinFlagsBitTwiddler
            // 
            resources.ApplyResources(this.pinFlagsBitTwiddler, "pinFlagsBitTwiddler");
            this.pinFlagsBitTwiddler.Bits = ((byte)(16));
            this.pinFlagsBitTwiddler.Caption = "PINFLAGSAB/CD";
            this.pinFlagsBitTwiddler.CaptionWidth = 175;
            this.pinFlagsBitTwiddler.Name = "pinFlagsBitTwiddler";
            this.pinFlagsBitTwiddler.TagInt = 15;
            this.pinFlagsBitTwiddler.ToolTips = null;
            this.pinFlagsBitTwiddler.Value = ((uint)(0u));
            this.pinFlagsBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // ioeBitTwiddler
            // 
            resources.ApplyResources(this.ioeBitTwiddler, "ioeBitTwiddler");
            this.ioeBitTwiddler.Bits = ((byte)(16));
            this.ioeBitTwiddler.Caption = "OEE/IOE";
            this.ioeBitTwiddler.CaptionWidth = 175;
            this.ioeBitTwiddler.Name = "ioeBitTwiddler";
            this.ioeBitTwiddler.TagInt = 13;
            this.ioeBitTwiddler.ToolTips = null;
            this.ioeBitTwiddler.Value = ((uint)(0u));
            this.ioeBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // portAcfgBitTwiddler
            // 
            resources.ApplyResources(this.portAcfgBitTwiddler, "portAcfgBitTwiddler");
            this.portAcfgBitTwiddler.Bits = ((byte)(16));
            this.portAcfgBitTwiddler.Caption = "PORTACFG/PORTCCFG";
            this.portAcfgBitTwiddler.CaptionWidth = 175;
            this.portAcfgBitTwiddler.Name = "portAcfgBitTwiddler";
            this.portAcfgBitTwiddler.TagInt = 14;
            this.portAcfgBitTwiddler.ToolTips = null;
            this.portAcfgBitTwiddler.Value = ((uint)(0u));
            this.portAcfgBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btSetting_ValueChanged);
            // 
            // epcfgDBitTwiddler
            // 
            resources.ApplyResources(this.epcfgDBitTwiddler, "epcfgDBitTwiddler");
            this.epcfgDBitTwiddler.Bits = ((byte)(16));
            this.epcfgDBitTwiddler.Caption = "EP2CFG/EP6CFG";
            this.epcfgDBitTwiddler.CaptionWidth = 175;
            this.epcfgDBitTwiddler.Name = "epcfgDBitTwiddler";
            this.epcfgDBitTwiddler.TagInt = 0;
            this.epcfgDBitTwiddler.ToolTips = null;
            this.epcfgDBitTwiddler.Value = ((uint)(0u));
            this.epcfgDBitTwiddler.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler26
            // 
            resources.ApplyResources(this.bitTwiddler26, "bitTwiddler26");
            this.bitTwiddler26.Bits = ((byte)(16));
            this.bitTwiddler26.Caption = "WORDWIDE";
            this.bitTwiddler26.CaptionWidth = 175;
            this.bitTwiddler26.Name = "bitTwiddler26";
            this.bitTwiddler26.TagInt = 1;
            this.bitTwiddler26.ToolTips = null;
            this.bitTwiddler26.Value = ((uint)(0u));
            this.bitTwiddler26.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler27
            // 
            resources.ApplyResources(this.bitTwiddler27, "bitTwiddler27");
            this.bitTwiddler27.Bits = ((byte)(16));
            this.bitTwiddler27.Caption = "DataAddress";
            this.bitTwiddler27.CaptionWidth = 175;
            this.bitTwiddler27.Name = "bitTwiddler27";
            this.bitTwiddler27.TagInt = 2;
            this.bitTwiddler27.ToolTips = null;
            this.bitTwiddler27.Value = ((uint)(0u));
            this.bitTwiddler27.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler28
            // 
            resources.ApplyResources(this.bitTwiddler28, "bitTwiddler28");
            this.bitTwiddler28.Bits = ((byte)(16));
            this.bitTwiddler28.Caption = "FPGATYPE";
            this.bitTwiddler28.CaptionWidth = 175;
            this.bitTwiddler28.Name = "bitTwiddler28";
            this.bitTwiddler28.TagInt = 4;
            this.bitTwiddler28.ToolTips = null;
            this.bitTwiddler28.Value = ((uint)(0u));
            this.bitTwiddler28.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler29
            // 
            resources.ApplyResources(this.bitTwiddler29, "bitTwiddler29");
            this.bitTwiddler29.Bits = ((byte)(16));
            this.bitTwiddler29.Caption = "BusSpeed/CPUCONFIG";
            this.bitTwiddler29.CaptionWidth = 175;
            this.bitTwiddler29.Name = "bitTwiddler29";
            this.bitTwiddler29.TagInt = 5;
            this.bitTwiddler29.ToolTips = null;
            this.bitTwiddler29.Value = ((uint)(0u));
            this.bitTwiddler29.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler30
            // 
            resources.ApplyResources(this.bitTwiddler30, "bitTwiddler30");
            this.bitTwiddler30.Bits = ((byte)(16));
            this.bitTwiddler30.Caption = "PORTECFG/SPICONFIG";
            this.bitTwiddler30.CaptionWidth = 175;
            this.bitTwiddler30.Name = "bitTwiddler30";
            this.bitTwiddler30.TagInt = 6;
            this.bitTwiddler30.ToolTips = null;
            this.bitTwiddler30.Value = ((uint)(0u));
            this.bitTwiddler30.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler31
            // 
            resources.ApplyResources(this.bitTwiddler31, "bitTwiddler31");
            this.bitTwiddler31.Bits = ((byte)(16));
            this.bitTwiddler31.Caption = "SlaveFIFO";
            this.bitTwiddler31.CaptionWidth = 175;
            this.bitTwiddler31.Name = "bitTwiddler31";
            this.bitTwiddler31.TagInt = 7;
            this.bitTwiddler31.ToolTips = null;
            this.bitTwiddler31.Value = ((uint)(0u));
            this.bitTwiddler31.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler32
            // 
            resources.ApplyResources(this.bitTwiddler32, "bitTwiddler32");
            this.bitTwiddler32.Bits = ((byte)(16));
            this.bitTwiddler32.Caption = "I2CTL";
            this.bitTwiddler32.CaptionWidth = 175;
            this.bitTwiddler32.Name = "bitTwiddler32";
            this.bitTwiddler32.TagInt = 8;
            this.bitTwiddler32.ToolTips = null;
            this.bitTwiddler32.Value = ((uint)(0u));
            this.bitTwiddler32.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler33
            // 
            resources.ApplyResources(this.bitTwiddler33, "bitTwiddler33");
            this.bitTwiddler33.Bits = ((byte)(16));
            this.bitTwiddler33.Caption = "OEA/IOA";
            this.bitTwiddler33.CaptionWidth = 175;
            this.bitTwiddler33.Name = "bitTwiddler33";
            this.bitTwiddler33.TagInt = 9;
            this.bitTwiddler33.ToolTips = null;
            this.bitTwiddler33.Value = ((uint)(0u));
            this.bitTwiddler33.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler34
            // 
            resources.ApplyResources(this.bitTwiddler34, "bitTwiddler34");
            this.bitTwiddler34.Bits = ((byte)(16));
            this.bitTwiddler34.Caption = "OEB/IOB";
            this.bitTwiddler34.CaptionWidth = 175;
            this.bitTwiddler34.Name = "bitTwiddler34";
            this.bitTwiddler34.TagInt = 10;
            this.bitTwiddler34.ToolTips = null;
            this.bitTwiddler34.Value = ((uint)(0u));
            this.bitTwiddler34.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler35
            // 
            resources.ApplyResources(this.bitTwiddler35, "bitTwiddler35");
            this.bitTwiddler35.Bits = ((byte)(16));
            this.bitTwiddler35.Caption = "OEC/IOC";
            this.bitTwiddler35.CaptionWidth = 175;
            this.bitTwiddler35.Name = "bitTwiddler35";
            this.bitTwiddler35.TagInt = 11;
            this.bitTwiddler35.ToolTips = null;
            this.bitTwiddler35.Value = ((uint)(0u));
            this.bitTwiddler35.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler36
            // 
            resources.ApplyResources(this.bitTwiddler36, "bitTwiddler36");
            this.bitTwiddler36.Bits = ((byte)(16));
            this.bitTwiddler36.Caption = "FIFOPolar/IFCONFIG";
            this.bitTwiddler36.CaptionWidth = 175;
            this.bitTwiddler36.Name = "bitTwiddler36";
            this.bitTwiddler36.TagInt = 3;
            this.bitTwiddler36.ToolTips = null;
            this.bitTwiddler36.Value = ((uint)(0u));
            this.bitTwiddler36.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler37
            // 
            resources.ApplyResources(this.bitTwiddler37, "bitTwiddler37");
            this.bitTwiddler37.Bits = ((byte)(16));
            this.bitTwiddler37.Caption = "OED/IOD";
            this.bitTwiddler37.CaptionWidth = 175;
            this.bitTwiddler37.Name = "bitTwiddler37";
            this.bitTwiddler37.TagInt = 12;
            this.bitTwiddler37.ToolTips = null;
            this.bitTwiddler37.Value = ((uint)(0u));
            this.bitTwiddler37.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler38
            // 
            resources.ApplyResources(this.bitTwiddler38, "bitTwiddler38");
            this.bitTwiddler38.Bits = ((byte)(16));
            this.bitTwiddler38.Caption = "PINFLAGSAB/CD";
            this.bitTwiddler38.CaptionWidth = 175;
            this.bitTwiddler38.Name = "bitTwiddler38";
            this.bitTwiddler38.TagInt = 15;
            this.bitTwiddler38.ToolTips = null;
            this.bitTwiddler38.Value = ((uint)(0u));
            this.bitTwiddler38.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler39
            // 
            resources.ApplyResources(this.bitTwiddler39, "bitTwiddler39");
            this.bitTwiddler39.Bits = ((byte)(16));
            this.bitTwiddler39.Caption = "OEE/IOE";
            this.bitTwiddler39.CaptionWidth = 175;
            this.bitTwiddler39.Name = "bitTwiddler39";
            this.bitTwiddler39.TagInt = 13;
            this.bitTwiddler39.ToolTips = null;
            this.bitTwiddler39.Value = ((uint)(0u));
            this.bitTwiddler39.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler40
            // 
            resources.ApplyResources(this.bitTwiddler40, "bitTwiddler40");
            this.bitTwiddler40.Bits = ((byte)(16));
            this.bitTwiddler40.Caption = "PORTACFG/PORTCCFG";
            this.bitTwiddler40.CaptionWidth = 175;
            this.bitTwiddler40.Name = "bitTwiddler40";
            this.bitTwiddler40.TagInt = 14;
            this.bitTwiddler40.ToolTips = null;
            this.bitTwiddler40.Value = ((uint)(0u));
            this.bitTwiddler40.ValueChanged += new QuickUsbDiagCs.ValueChangedHandler(this.btDefault_ValueChanged);
            // 
            // bitTwiddler9
            // 
            resources.ApplyResources(this.bitTwiddler9, "bitTwiddler9");
            this.bitTwiddler9.Bits = ((byte)(16));
            this.bitTwiddler9.Caption = "EP2CFG/EP6CFG";
            this.bitTwiddler9.CaptionWidth = 130;
            this.bitTwiddler9.Name = "bitTwiddler9";
            this.bitTwiddler9.TagInt = 1;
            this.bitTwiddler9.ToolTips = null;
            this.bitTwiddler9.Value = ((uint)(0u));
            // 
            // bitTwiddler10
            // 
            resources.ApplyResources(this.bitTwiddler10, "bitTwiddler10");
            this.bitTwiddler10.Bits = ((byte)(16));
            this.bitTwiddler10.Caption = "WORDWIDE";
            this.bitTwiddler10.CaptionWidth = 130;
            this.bitTwiddler10.Name = "bitTwiddler10";
            this.bitTwiddler10.TagInt = 1;
            this.bitTwiddler10.ToolTips = null;
            this.bitTwiddler10.Value = ((uint)(0u));
            // 
            // bitTwiddler11
            // 
            resources.ApplyResources(this.bitTwiddler11, "bitTwiddler11");
            this.bitTwiddler11.Bits = ((byte)(16));
            this.bitTwiddler11.Caption = "DataAddress";
            this.bitTwiddler11.CaptionWidth = 130;
            this.bitTwiddler11.Name = "bitTwiddler11";
            this.bitTwiddler11.TagInt = 2;
            this.bitTwiddler11.ToolTips = null;
            this.bitTwiddler11.Value = ((uint)(0u));
            // 
            // bitTwiddler12
            // 
            resources.ApplyResources(this.bitTwiddler12, "bitTwiddler12");
            this.bitTwiddler12.Bits = ((byte)(16));
            this.bitTwiddler12.Caption = "FPGATYPE";
            this.bitTwiddler12.CaptionWidth = 130;
            this.bitTwiddler12.Name = "bitTwiddler12";
            this.bitTwiddler12.TagInt = 4;
            this.bitTwiddler12.ToolTips = null;
            this.bitTwiddler12.Value = ((uint)(0u));
            // 
            // bitTwiddler13
            // 
            resources.ApplyResources(this.bitTwiddler13, "bitTwiddler13");
            this.bitTwiddler13.Bits = ((byte)(16));
            this.bitTwiddler13.Caption = "BusSpeed/CPUCONFIG";
            this.bitTwiddler13.CaptionWidth = 130;
            this.bitTwiddler13.Name = "bitTwiddler13";
            this.bitTwiddler13.TagInt = 5;
            this.bitTwiddler13.ToolTips = null;
            this.bitTwiddler13.Value = ((uint)(0u));
            // 
            // bitTwiddler14
            // 
            resources.ApplyResources(this.bitTwiddler14, "bitTwiddler14");
            this.bitTwiddler14.Bits = ((byte)(16));
            this.bitTwiddler14.Caption = "PORTECFG/SPICONFIG";
            this.bitTwiddler14.CaptionWidth = 130;
            this.bitTwiddler14.Name = "bitTwiddler14";
            this.bitTwiddler14.TagInt = 6;
            this.bitTwiddler14.ToolTips = null;
            this.bitTwiddler14.Value = ((uint)(0u));
            // 
            // bitTwiddler15
            // 
            resources.ApplyResources(this.bitTwiddler15, "bitTwiddler15");
            this.bitTwiddler15.Bits = ((byte)(16));
            this.bitTwiddler15.Caption = "SlaveFIFO";
            this.bitTwiddler15.CaptionWidth = 130;
            this.bitTwiddler15.Name = "bitTwiddler15";
            this.bitTwiddler15.TagInt = 7;
            this.bitTwiddler15.ToolTips = null;
            this.bitTwiddler15.Value = ((uint)(0u));
            // 
            // bitTwiddler16
            // 
            resources.ApplyResources(this.bitTwiddler16, "bitTwiddler16");
            this.bitTwiddler16.Bits = ((byte)(16));
            this.bitTwiddler16.Caption = "I2OPT/I2CTL";
            this.bitTwiddler16.CaptionWidth = 130;
            this.bitTwiddler16.Name = "bitTwiddler16";
            this.bitTwiddler16.TagInt = 8;
            this.bitTwiddler16.ToolTips = null;
            this.bitTwiddler16.Value = ((uint)(0u));
            // 
            // bitTwiddler17
            // 
            resources.ApplyResources(this.bitTwiddler17, "bitTwiddler17");
            this.bitTwiddler17.Bits = ((byte)(16));
            this.bitTwiddler17.Caption = "OEA/IOA";
            this.bitTwiddler17.CaptionWidth = 130;
            this.bitTwiddler17.Name = "bitTwiddler17";
            this.bitTwiddler17.TagInt = 9;
            this.bitTwiddler17.ToolTips = null;
            this.bitTwiddler17.Value = ((uint)(0u));
            // 
            // bitTwiddler18
            // 
            resources.ApplyResources(this.bitTwiddler18, "bitTwiddler18");
            this.bitTwiddler18.Bits = ((byte)(16));
            this.bitTwiddler18.Caption = "OEB/IOB";
            this.bitTwiddler18.CaptionWidth = 130;
            this.bitTwiddler18.Name = "bitTwiddler18";
            this.bitTwiddler18.TagInt = 10;
            this.bitTwiddler18.ToolTips = null;
            this.bitTwiddler18.Value = ((uint)(0u));
            // 
            // bitTwiddler19
            // 
            resources.ApplyResources(this.bitTwiddler19, "bitTwiddler19");
            this.bitTwiddler19.Bits = ((byte)(16));
            this.bitTwiddler19.Caption = "OEC/IOC";
            this.bitTwiddler19.CaptionWidth = 130;
            this.bitTwiddler19.Name = "bitTwiddler19";
            this.bitTwiddler19.TagInt = 11;
            this.bitTwiddler19.ToolTips = null;
            this.bitTwiddler19.Value = ((uint)(0u));
            // 
            // bitTwiddler20
            // 
            resources.ApplyResources(this.bitTwiddler20, "bitTwiddler20");
            this.bitTwiddler20.Bits = ((byte)(16));
            this.bitTwiddler20.Caption = "FIFOPolar/IFCONFIG";
            this.bitTwiddler20.CaptionWidth = 130;
            this.bitTwiddler20.Name = "bitTwiddler20";
            this.bitTwiddler20.TagInt = 3;
            this.bitTwiddler20.ToolTips = null;
            this.bitTwiddler20.Value = ((uint)(0u));
            // 
            // bitTwiddler21
            // 
            resources.ApplyResources(this.bitTwiddler21, "bitTwiddler21");
            this.bitTwiddler21.Bits = ((byte)(16));
            this.bitTwiddler21.Caption = "OED/IOD";
            this.bitTwiddler21.CaptionWidth = 130;
            this.bitTwiddler21.Name = "bitTwiddler21";
            this.bitTwiddler21.TagInt = 12;
            this.bitTwiddler21.ToolTips = null;
            this.bitTwiddler21.Value = ((uint)(0u));
            // 
            // bitTwiddler22
            // 
            resources.ApplyResources(this.bitTwiddler22, "bitTwiddler22");
            this.bitTwiddler22.Bits = ((byte)(16));
            this.bitTwiddler22.Caption = "PINFLAGSAB/CD";
            this.bitTwiddler22.CaptionWidth = 130;
            this.bitTwiddler22.Name = "bitTwiddler22";
            this.bitTwiddler22.TagInt = 15;
            this.bitTwiddler22.ToolTips = null;
            this.bitTwiddler22.Value = ((uint)(0u));
            // 
            // bitTwiddler23
            // 
            resources.ApplyResources(this.bitTwiddler23, "bitTwiddler23");
            this.bitTwiddler23.Bits = ((byte)(16));
            this.bitTwiddler23.Caption = "OEE/IOE";
            this.bitTwiddler23.CaptionWidth = 130;
            this.bitTwiddler23.Name = "bitTwiddler23";
            this.bitTwiddler23.TagInt = 13;
            this.bitTwiddler23.ToolTips = null;
            this.bitTwiddler23.Value = ((uint)(0u));
            // 
            // bitTwiddler24
            // 
            resources.ApplyResources(this.bitTwiddler24, "bitTwiddler24");
            this.bitTwiddler24.Bits = ((byte)(16));
            this.bitTwiddler24.Caption = "PORTACFG/PORTCCFG";
            this.bitTwiddler24.CaptionWidth = 130;
            this.bitTwiddler24.Name = "bitTwiddler24";
            this.bitTwiddler24.TagInt = 14;
            this.bitTwiddler24.ToolTips = null;
            this.bitTwiddler24.Value = ((uint)(0u));
            // 
            // QuickUsbDiag
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.ssStatus);
            this.Controls.Add(this.menuStrip);
            this.DoubleBuffered = true;
            this.MainMenuStrip = this.menuStrip;
            this.Name = "QuickUsbDiag";
            this.Load += new System.EventHandler(this.QuickUsbDiag_Load);
            this.Shown += new System.EventHandler(this.QuickUsbDiag_Shown);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.QuickUsbDiag_FormClosing);
            this.ssStatus.ResumeLayout(false);
            this.ssStatus.PerformLayout();
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nRepeatInterval)).EndInit();
            this.tabCtrl.ResumeLayout(false);
            this.tabGeneral.ResumeLayout(false);
            this.gbGeneralFpga.ResumeLayout(false);
            this.gbGeneralFpga.PerformLayout();
            this.fpgaButtonFlowLayoutPanel.ResumeLayout(false);
            this.fpgaButtonFlowLayoutPanel.PerformLayout();
            this.gbGeneralGeneral.ResumeLayout(false);
            this.gbGeneralGeneral.PerformLayout();
            this.tabCommand.ResumeLayout(false);
            this.gbCommandWrite.ResumeLayout(false);
            this.gbCommandWrite.PerformLayout();
            this.gbCommandRead.ResumeLayout(false);
            this.gbCommandRead.PerformLayout();
            this.gbCommandGeneral.ResumeLayout(false);
            this.gbCommandGeneral.PerformLayout();
            this.tabData.ResumeLayout(false);
            this.gbDataWrite.ResumeLayout(false);
            this.gbDataWrite.PerformLayout();
            this.gbDataRead.ResumeLayout(false);
            this.gbDataRead.PerformLayout();
            this.gbDataGeneral.ResumeLayout(false);
            this.gbDataGeneral.PerformLayout();
            this.tabStreaming.ResumeLayout(false);
            this.gbStreamingStatistics.ResumeLayout(false);
            this.gbStreamingStatistics.PerformLayout();
            this.gbStreamingWrite.ResumeLayout(false);
            this.gbStreamingWrite.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.gbStreamingGeneral.ResumeLayout(false);
            this.gbStreamingGeneral.PerformLayout();
            this.gbStreamingRead.ResumeLayout(false);
            this.gbStreamingRead.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.tabPorts.ResumeLayout(false);
            this.gbPortE.ResumeLayout(false);
            this.gbPortE.PerformLayout();
            this.gbPortD.ResumeLayout(false);
            this.gbPortD.PerformLayout();
            this.gbPortC.ResumeLayout(false);
            this.gbPortC.PerformLayout();
            this.gbPortB.ResumeLayout(false);
            this.gbPortB.PerformLayout();
            this.gbPortA.ResumeLayout(false);
            this.gbPortA.PerformLayout();
            this.tabRS232.ResumeLayout(false);
            this.gbRS232Write.ResumeLayout(false);
            this.gbRS232Write.PerformLayout();
            this.gbRS232Read.ResumeLayout(false);
            this.gbRS232Read.PerformLayout();
            this.gbRS232General.ResumeLayout(false);
            this.gbRS232General.PerformLayout();
            this.tabSPI.ResumeLayout(false);
            this.gbSPIWrite.ResumeLayout(false);
            this.gbSPIWrite.PerformLayout();
            this.gbSPIRead.ResumeLayout(false);
            this.gbSPIRead.PerformLayout();
            this.gbSPIGeneral.ResumeLayout(false);
            this.gbSPIGeneral.PerformLayout();
            this.tabEPCS.ResumeLayout(false);
            this.gbEpcsWriting.ResumeLayout(false);
            this.gbEpcsWriting.PerformLayout();
            this.gbEpcsGeneral.ResumeLayout(false);
            this.gbEpcsGeneral.PerformLayout();
            this.tabI2C.ResumeLayout(false);
            this.gbI2CWrite.ResumeLayout(false);
            this.gbI2CWrite.PerformLayout();
            this.gbI2CRead.ResumeLayout(false);
            this.gbI2CRead.PerformLayout();
            this.gbI2CGeneral.ResumeLayout(false);
            this.gbI2CGeneral.PerformLayout();
            this.tabSettings.ResumeLayout(false);
            this.gbSettings.ResumeLayout(false);
            this.gbSettings.PerformLayout();
            this.tabDefaults.ResumeLayout(false);
            this.gbDefaults.ResumeLayout(false);
            this.gbDefaults.PerformLayout();
            this.groupBox24.ResumeLayout(false);
            this.groupBox24.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private System.Windows.Forms.ToolTip toolTip;
		private System.Windows.Forms.StatusStrip ssStatus;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
		private System.Windows.Forms.ToolStripProgressBar pbProgress;
        private System.Windows.Forms.MenuStrip menuStrip;
		private System.Windows.Forms.ToolStripMenuItem helpMenu;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ToolStripPanel BottomToolStripPanel;
		private System.Windows.Forms.ToolStripPanel TopToolStripPanel;
		private System.Windows.Forms.ToolStripPanel RightToolStripPanel;
		private System.Windows.Forms.ToolStripPanel LeftToolStripPanel;
		private System.Windows.Forms.ToolStripContentPanel ContentPanel;
		private System.Windows.Forms.Timer statusTimer;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem quickUSBFAQToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem quickUSBSupportToolStripMenuItem;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.TabControl tabCtrl;
		private System.Windows.Forms.TabPage tabSettings;
		private System.Windows.Forms.TabPage tabDefaults;
		internal BitwiseSystems.QuickUsbWinForms quickUsbWinForms;
		private System.Windows.Forms.SplitContainer splitContainer;
		private System.Windows.Forms.TabPage tabGeneral;
		private System.Windows.Forms.TabPage tabCommand;
		private System.Windows.Forms.TabPage tabData;
		private System.Windows.Forms.TabPage tabPorts;
        private System.Windows.Forms.TabPage tabRS232;
        private System.Windows.Forms.TabPage tabSPI;
        private System.Windows.Forms.TabPage tabI2C;
        private System.Windows.Forms.GroupBox gbGeneralGeneral;
        private System.Windows.Forms.RadioButton rbHighSpeed;
        private System.Windows.Forms.RadioButton rbFullSpeed;
        private System.Windows.Forms.TextBox tbHWVersion;
        private System.Windows.Forms.TextBox tbDesciption;
        private System.Windows.Forms.TextBox tbSerialNumber;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label labelSetting8_2;
        private System.Windows.Forms.Label labelSetting7_2;
        private System.Windows.Forms.Label labelSetting0_2;
        private System.Windows.Forms.Label labelSetting15_2;
        private System.Windows.Forms.Label labelSettingLSB;
        private System.Windows.Forms.Label labelSettingMSB;
        private System.Windows.Forms.Label labelSetting8_1;
        private System.Windows.Forms.Label labelSetting7_1;
        private System.Windows.Forms.Label labelSetting0_1;
        private System.Windows.Forms.Label labelSetting15_1;
        private BitTwiddler ifconfigBitTwiddler;
        private BitTwiddler pinFlagsBitTwiddler;
        private BitTwiddler portAcfgBitTwiddler;
        private BitTwiddler ioeBitTwiddler;
        private BitTwiddler iodBitTwiddler;
        private BitTwiddler iocBitTwiddler;
        private BitTwiddler iobBitTwiddler;
        private BitTwiddler ioaBitTwiddler;
        private BitTwiddler i2ctlBitTwiddler;
        private BitTwiddler slaveFifoBitTwiddler;
        private BitTwiddler spiConfigBitTwiddler;
        private BitTwiddler cpuConfigBitTwiddler;
        private BitTwiddler fpgaTypeBitTwiddler;
        private BitTwiddler dataAddressBitTwiddler;
        private BitTwiddler wordWideBitTwiddler;
        private BitTwiddler epcfgBitTwiddler;
        private System.Windows.Forms.GroupBox gbGeneralFpga;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Button isFpgaConfiguredButton;
        private System.Windows.Forms.Button browseFpgaFileButton;
        private System.Windows.Forms.TextBox tbFpgaFile;
        private System.Windows.Forms.Button uploadFpgaButton;
        private System.Windows.Forms.FlowLayoutPanel fpgaButtonFlowLayoutPanel;
        private System.Windows.Forms.RadioButton alteraPsRadioButton;
        private System.Windows.Forms.RadioButton xilinxSsRadioButton;
        private System.Windows.Forms.GroupBox gbCommandGeneral;
        private System.Windows.Forms.CheckBox cbCmdUseAddr;
        private System.Windows.Forms.CheckBox cbCmdIncAddr;
        private System.Windows.Forms.TextBox cmdWriteData;
        private System.Windows.Forms.TextBox cmdReadData;
        private System.Windows.Forms.Button bCmdWrite;
        private System.Windows.Forms.Button bCmdRead;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox tbCmdReadBytes;
        private System.Windows.Forms.TextBox tbCmdReadAddress;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.GroupBox gbCommandWrite;
        private System.Windows.Forms.TextBox tbCmdWriteAddress;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox tbCmdWriteBytes;
        private System.Windows.Forms.GroupBox gbCommandRead;
        private System.Windows.Forms.CheckBox cbCmdWordWide;
        private System.Windows.Forms.GroupBox gbDataRead;
        private System.Windows.Forms.TextBox tbDataReadBytes;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.GroupBox gbDataGeneral;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Button bSetTimeout;
        private System.Windows.Forms.TextBox tbTimeout;
        private System.Windows.Forms.CheckBox cbDataWordwide;
        private System.Windows.Forms.TextBox tbDataReadAddress;
        private System.Windows.Forms.CheckBox cbDataUseAddr;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Button bDataRead;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox tbOutputFileName;
        private System.Windows.Forms.Button dataReadBrowse;
        private System.Windows.Forms.GroupBox gbDataWrite;
        private System.Windows.Forms.CheckBox cbUseFileLength;
        private System.Windows.Forms.TextBox tbDataWriteBytes;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox tbInputFileName;
        private System.Windows.Forms.Button bBrowseWrite;
        private System.Windows.Forms.TextBox tbDataWriteAddress;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Button bDataWrite;
        private System.Windows.Forms.GroupBox gbPortA;
        private BitTwiddler btPortAVal;
        private BitTwiddler btPortADir;
        private System.Windows.Forms.GroupBox gbPortE;
        private BitTwiddler btPortEDir;
        private BitTwiddler btPortEVal;
        private System.Windows.Forms.GroupBox gbPortD;
        private BitTwiddler btPortDDir;
        private BitTwiddler btPortDVal;
        private System.Windows.Forms.GroupBox gbPortC;
        private BitTwiddler btPortCDir;
        private BitTwiddler btPortCVal;
        private System.Windows.Forms.GroupBox gbPortB;
        private BitTwiddler btPortBDir;
        private BitTwiddler btPortBVal;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.GroupBox gbRS232General;
        private System.Windows.Forms.ComboBox cbRS232Port;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.ComboBox cbRS232Baud;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Button rs232Get;
        private System.Windows.Forms.TextBox tbRS232Chars;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Button bRS232Flush;
        private System.Windows.Forms.TextBox rs232WriteData;
        private System.Windows.Forms.Button rs232Write;
        private System.Windows.Forms.Button rs232Read;
        private System.Windows.Forms.TextBox tbRS232ReadData;
        private System.Windows.Forms.GroupBox gbRS232Write;
        private System.Windows.Forms.GroupBox gbRS232Read;
        private System.Windows.Forms.GroupBox gbSPIRead;
        private System.Windows.Forms.ComboBox cbSpiReadPort;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TextBox tbSpiBytes;
        private System.Windows.Forms.GroupBox gbSPIGeneral;
        private System.Windows.Forms.CheckBox cbSpiEndian;
        private System.Windows.Forms.CheckBox cbSpiCPOL;
        private System.Windows.Forms.CheckBox cbSpiCPHA;
        private System.Windows.Forms.TextBox tbSpiWriteData;
        private System.Windows.Forms.Button spiWrite;
        private System.Windows.Forms.Button spiRead;
        private System.Windows.Forms.TextBox tbSpiReadData;
        private System.Windows.Forms.GroupBox gbSPIWrite;
        private System.Windows.Forms.ComboBox cbSpiWritePort;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.GroupBox gbI2CGeneral;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.TextBox tbI2cBytes;
        private System.Windows.Forms.TextBox tbI2cReadAddress;
        private System.Windows.Forms.ComboBox cbI2cBusSpeed;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.TextBox tbI2cWriteData;
        private System.Windows.Forms.Button i2cWrite;
        private System.Windows.Forms.Button i2cRead;
        private System.Windows.Forms.TextBox tbI2cReadData;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.GroupBox gbI2CWrite;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.TextBox tbI2cWriteAddress;
        private System.Windows.Forms.GroupBox gbI2CRead;
        private System.Windows.Forms.CheckBox cbI2cIgnoreAck;
        private System.Windows.Forms.GroupBox gbSettings;
        private System.Windows.Forms.GroupBox gbDefaults;
        private BitTwiddler epcfgDBitTwiddler;
        private System.Windows.Forms.Label labelDefault8_2;
        private BitTwiddler bitTwiddler26;
        private System.Windows.Forms.Label labelDefault7_2;
        private BitTwiddler bitTwiddler27;
        private System.Windows.Forms.Label labelDefault0_2;
        private BitTwiddler bitTwiddler28;
        private System.Windows.Forms.Label labelDefault15_2;
        private BitTwiddler bitTwiddler29;
        private System.Windows.Forms.Label labelDefaultLSB;
        private BitTwiddler bitTwiddler30;
        private System.Windows.Forms.Label labelDefaultMSB;
        private BitTwiddler bitTwiddler31;
        private System.Windows.Forms.Label labelDefault8_1;
        private BitTwiddler bitTwiddler32;
        private System.Windows.Forms.Label labelDefault7_1;
        private BitTwiddler bitTwiddler33;
        private System.Windows.Forms.Label labelDefault0_1;
        private BitTwiddler bitTwiddler34;
        private System.Windows.Forms.Label labelDefault15_1;
        private BitTwiddler bitTwiddler35;
        private BitTwiddler bitTwiddler36;
        private BitTwiddler bitTwiddler37;
        private BitTwiddler bitTwiddler38;
        private BitTwiddler bitTwiddler39;
        private BitTwiddler bitTwiddler40;
        private System.Windows.Forms.GroupBox groupBox24;
        private BitTwiddler bitTwiddler9;
        private System.Windows.Forms.Label label40;
        private BitTwiddler bitTwiddler10;
        private System.Windows.Forms.Label label41;
        private BitTwiddler bitTwiddler11;
        private System.Windows.Forms.Label label42;
        private BitTwiddler bitTwiddler12;
        private System.Windows.Forms.Label label43;
        private BitTwiddler bitTwiddler13;
        private System.Windows.Forms.Label label44;
        private BitTwiddler bitTwiddler14;
        private System.Windows.Forms.Label label45;
        private BitTwiddler bitTwiddler15;
        private System.Windows.Forms.Label label46;
        private BitTwiddler bitTwiddler16;
        private System.Windows.Forms.Label label47;
        private BitTwiddler bitTwiddler17;
        private System.Windows.Forms.Label label48;
        private BitTwiddler bitTwiddler18;
        private System.Windows.Forms.Label label49;
        private BitTwiddler bitTwiddler19;
        private BitTwiddler bitTwiddler20;
        private BitTwiddler bitTwiddler21;
        private BitTwiddler bitTwiddler22;
        private BitTwiddler bitTwiddler23;
        private BitTwiddler bitTwiddler24;
        private System.Windows.Forms.CheckBox cbDataIncAddr;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Button bReadPortE;
        private System.Windows.Forms.Button bReadPortD;
        private System.Windows.Forms.Button bReadPortC;
        private System.Windows.Forms.Button bReadPortB;
        private System.Windows.Forms.Button bReadPortA;
        private System.Windows.Forms.Button bCountPortA;
        private System.Windows.Forms.Button bWriteReadSpi;
        private System.Windows.Forms.Button bI2cCachedWrite;
        private System.Windows.Forms.TabPage tabEPCS;
        private System.Windows.Forms.GroupBox gbEpcsGeneral;
        private System.Windows.Forms.GroupBox gbEpcsWriting;
        private System.Windows.Forms.TextBox tbEpcsFilePath;
        private System.Windows.Forms.Button bEpcsBrowse;
        private System.Windows.Forms.Label label60;
        private System.Windows.Forms.Button bEpcsWrite;
        private System.Windows.Forms.Button bEpcsErase;
        private System.Windows.Forms.Button bEpcsVerify;
        private System.Windows.Forms.Button bEpcsIdentify;
        private System.Windows.Forms.ComboBox cbEpcsPort;
        private System.Windows.Forms.Label label61;
        private System.Windows.Forms.Label labelEpcsIdentify;
        private System.Windows.Forms.CheckBox cbSpiPORT;
        private System.Windows.Forms.CheckBox cbSpiNCEPIN;
        private System.Windows.Forms.CheckBox cbSpiMISOPIN;
        private System.Windows.Forms.Button bDataWriteEdit;
        private System.Windows.Forms.Button bDataCount;
        private System.Windows.Forms.Button bDataReadEdit;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label62;
        private System.Windows.Forms.NumericUpDown nRepeatInterval;
        private System.Windows.Forms.CheckBox cbRepeat;
        private System.Windows.Forms.Timer timerRepeat;
        private System.Windows.Forms.TabPage tabStreaming;
        private System.Windows.Forms.GroupBox gbStreamingGeneral;
        private System.Windows.Forms.TextBox tbStreamBufferSize;
        private System.Windows.Forms.Label label67;
        private System.Windows.Forms.CheckBox cbStreamingIncAddr;
        private System.Windows.Forms.CheckBox cbStreamingUseAddr;
        private System.Windows.Forms.CheckBox cbStreamingWordwide;
        private System.Windows.Forms.TextBox tbStreamThreadConcurrency;
        private System.Windows.Forms.Label label70;
        private System.Windows.Forms.TextBox tbStreamNumThreads;
        private System.Windows.Forms.Label label69;
        private System.Windows.Forms.TextBox tbStreamNumBuffers;
        private System.Windows.Forms.Label label68;
        private System.Windows.Forms.GroupBox gbStreamingRead;
        private System.Windows.Forms.RadioButton rbReadStreamDisgard;
        private System.Windows.Forms.RadioButton rbReadStreamToDisk;
        private System.Windows.Forms.Button bReadStreamEdit;
        private System.Windows.Forms.TextBox tbReadStreamFileName;
        private System.Windows.Forms.Button bReadStreamBrowse;
        private System.Windows.Forms.Label lReadStreamFileName;
        private System.Windows.Forms.Button bReadStreamStart;
        private System.Windows.Forms.Button bReadStreamStop;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.GroupBox gbStreamingStatistics;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox gbStreamingWrite;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.RadioButton rbWriteStreamFromDisk;
        private System.Windows.Forms.RadioButton rbWriteStreamFromPattern;
        private System.Windows.Forms.Button bWriteStreamStop;
        private System.Windows.Forms.Button bWriteStreamEdit;
        private System.Windows.Forms.TextBox tbWriteStreamFileName;
        private System.Windows.Forms.Button bWriteStreamBrowse;
        private System.Windows.Forms.Label lWriteStreamFileName;
        private System.Windows.Forms.Button bWriteStreamStart;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label labelWriteStreamCount;
        private System.Windows.Forms.Label labelWriteStreamRate;
        private System.Windows.Forms.Label labelReadStreamCount;
        private System.Windows.Forms.Label labelReadStreamRate;
        private System.Windows.Forms.CheckBox cbDataOutOfOrder;
        private System.Windows.Forms.RadioButton rbSuperSpeed;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem continuallyScanForNewremovedDevicesToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lLabelDeviceSpeed;
	}
}



