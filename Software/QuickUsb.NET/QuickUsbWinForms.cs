using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.ComponentModel;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace BitwiseSystems
{
    /// <summary>
    /// QuickUSB Control Class
    /// </summary>
    public partial class QuickUsbWinForms : UserControl
    {
        [StructLayout(LayoutKind.Sequential, Size = 18), Serializable]
        internal struct USB_DEVICE_DESCRIPTOR
        {
            public byte bLength;
            public byte bDescriptorType;
            public ushort bcdUSB;
            public byte bDeviceClass;
            public byte bDeviceSubClass;
            public byte bDeviceProtocol;
            public byte bMaxPacketSize0;
            public ushort idVendor;
            public ushort idProduct;
            public ushort bcdDevice;
            public byte iManufacturer;
            public byte iProduct;
            public byte iSerialNumber;
            public byte bNumConfigurations;
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="hDevice"></param>
        /// <param name="deviceName"></param>
        /// <returns></returns>
        [DllImport("QuickUsbUtil.dll", EntryPoint = "QuickUsbOpen")]
        public static extern int QuickUsbOpenUtil(out IntPtr hDevice, String deviceName);

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="hDevice"></param>
        /// <returns></returns>
        [DllImport("QuickUsbUtil.dll", EntryPoint = "QuickUsbClose")]
        public static extern int QuickUsbCloseUtil(IntPtr hDevice);

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="hDevice"></param>
        /// <param name="data"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        [DllImport("QuickUsbUtil.dll")]
        internal static extern int QuickUsbGetDeviceDesc(IntPtr hDevice, ref USB_DEVICE_DESCRIPTOR data, uint length);

        /// <summary>
        /// Enumeration used to specify column headers
        /// </summary>
        public enum ColumnDataType
        {
            /// <summary>
            /// The name of the QuickUSB device
            /// </summary>
            Name,

            /// <summary>
            /// The make of the QuickUSB device, as returned from 
            /// QuickUsb.GetStringDescriptor for StringDescriptor.Make
            /// </summary>
            Make,

            /// <summary>
            /// The read-only make of the QuickUSB device.  This is always set
            /// to 'Bitwise Systems'
            /// </summary>
            PrivateMake,


            /// <summary>
            /// The make of the QuickUSB device, as returned from 
            /// QuickUsb.GetStringDescriptor for StringDescriptor.Model
            /// </summary>
            Model,

            /// <summary>
            /// The private model of the QuickUSB device.  This is typically
            /// set to a string describing the firmware, such as 'QuickUSB 
            /// QUSB2 Module v2.15.1 (FIFO Handshake)'
            /// </summary>
            PrivateModel,

            /// <summary>
            /// The make of the QuickUSB device, as returned from 
            /// QuickUsb.GetStringDescriptor for StringDescriptor.Serial
            /// </summary>
            Serial,

            /// <summary>
            /// The private serial number of the QuickUSB device
            /// </summary>
            PrivateSerial,

            /// <summary>
            /// The version of firmware in the QuickUSB device
            /// </summary>
            FirmwareVersion,

            /// <summary>
            /// The driver version
            /// </summary>
            DriverVersion,

            /// <summary>
            /// The version of the DLLs currently in use
            /// </summary>
            LibraryVersion,

            /// <summary>
            /// The USB Vendor ID of the device.  Unless the device has been 
            /// customized, this will be '0x0FBB'.
            /// </summary>
            VID,

            /// <summary>
            /// The USB Product ID of the device.  Unless the device has been
            /// customized, this will be '0x0001'.
            /// </summary>
            PID,

            /// <summary>
            /// The name of the firmware IO Model, such as 'Simple I/O'
            /// </summary>
            IOModel
        }

        /// <summary>
        /// Insert a new column of data for the detail view
        /// </summary>
        /// <param name="title">The column header text</param>
        /// <param name="type">The data to place in the column</param>
        /// <param name="index">The index to insert this column before.  
        /// Use -1 to append the column at then end of all columns.</param>
        public void InsertColumnData(string title, ColumnDataType type, int index)
        {
            if (index == -1)
            {
                index = columnData.Count;
            }

            deviceListView.Columns.Insert(index, title);
            columnSortOrder.Insert(index, SortOrder.Ascending);
            ColumnData cd = new ColumnData();
            cd.title = title;
            cd.type = type;
            columnData.Insert(index, cd);
        }

        /// <summary>
        /// Remove the column by the column header text
        /// </summary>
        /// <param name="title">The column header text</param>
        public void RemoveColumnData(string title)
        {
            for (int k = 0; k < columnData.Count; ++k)
            {
                if (columnData[k].title == title)
                {
                    columnData.RemoveAt(k);
                    columnSortOrder.RemoveAt(k);
                    deviceListView.Columns.RemoveAt(k);
                    break;
                }
            }
        }

        /// <summary>
        /// Remove all column headers and data
        /// </summary>
        public void RemoveAllColumnData()
        {
            while (columnData.Count > 0)
            {
                columnData.RemoveAt(0);
                columnSortOrder.RemoveAt(0);
                deviceListView.Columns.RemoveAt(0);
            }
        }

        #region Instance variables

        struct ColumnData
        {
            public string title;
            public ColumnDataType type;
        }

        bool scanEnabled = true, scanOnLoad = true, loading, showUnprogrammedDevices, performFlush = false, autoSelect = true;
        private int previouslySortedColumn = -1;
        private List<SortOrder> columnSortOrder = new List<SortOrder>();
        private List<ColumnData> columnData = new List<ColumnData>();

        class qusbListInfo
        {
            public QuickUsb qusb;
            public ListViewItem lvi;
            public bool visible;
            public bool attached;
        }

        ListViewGroup ModuleGroup = new ListViewGroup("QuickUSB Modules", HorizontalAlignment.Left);
        Dictionary<string, qusbListInfo> QuickUsbDict = new Dictionary<string, qusbListInfo>();
        Dictionary<string, qusbListInfo> QuickUsbSelDict = new Dictionary<string, qusbListInfo>();

        #endregion

        #region Delegates, events, and properties

        /// <summary>
        /// The module connected event handler delegate
        /// </summary>
        /// <param name="sender">The issuing object</param>
        /// <param name="e">The event args</param>
        public delegate void ConnectedHandler(object sender, ModuleEventArgs e);

        /// <summary>
        /// The module disconnected event handler delegate
        /// </summary>
        /// <param name="sender">The issuing object</param>
        /// <param name="e">The event args</param>
        public delegate void DisconnectedHandler(object sender, ModuleEventArgs e);

        /// <summary>
        /// The selection changed event handler delegate
        /// </summary>
        /// <param name="sender">The issuing object</param>
        /// <param name="e">The event args</param>
        public delegate void SelectionChangedHandler(object sender, EventArgs e);

        /// <summary>
        /// The event that gets fired when a new QuickUSB is discovered by the control
        /// </summary>
        [Category("Action")]
        [Description("Occurs when a QuickUSB module is connected to the host")]
        public event ConnectedHandler ModuleConnected;

        /// <summary>
        /// The event that gets fired when a QuickUSB in the control is removed from the host
        /// </summary>
        [Category("Action")]
        [Description("Occurs when a QuickUSB module is disconnected from the host")]
        public event DisconnectedHandler ModuleDisconnected;

        /// <summary>
        /// The event that gets fired when the currently selected QuickUSB has changed
        /// </summary>
        [Category("Action")]
        [Description("Occurs when the selected QuickUSB module has changed")]
        public event SelectionChangedHandler SelectionChanged;

        /// <summary>
        /// A property holding a list of all connected QuickUSB modules
        /// </summary>
        public QuickUsb[] Modules
        {
            get
            {
                QuickUsb[] qmuxList = new QuickUsb[deviceListView.Items.Count];
                for (int k = 0; k < qmuxList.Length; ++k)
                {
                    qmuxList[k] = deviceListView.Items[k].Tag as QuickUsb;
                }
                return qmuxList;
            }
        }

        /// <summary>
        /// A property holding a list of all connect QuickUSB modules by their name
        /// </summary>
        public string[] ModuleNames
        {
            get
            {
                string[] arr = new string[QuickUsbDict.Keys.Count];
                QuickUsbDict.Keys.CopyTo(arr, 0);
                return arr;
            }
        }

        /// <summary>
        /// A property holding a dictionary of all QuickUSB names (key) to QuickUSB objects (value)
        /// </summary>
        public Dictionary<string, QuickUsb> ModuleNamesDict
        {
            get
            {
                var dict = new Dictionary<string, QuickUsb>();
                foreach (var kv in QuickUsbDict)
                {
                    dict.Add(kv.Key, kv.Value.qusb);
                }
                return dict;
            }
        }

        /// <summary>
        /// A property holding the currently selected QuickUSB object.  If more than one QuickUSB is selected, this is the first QuickUSB in the list.
        /// </summary>
        public QuickUsb SelectedModule
        {
            get
            {
                if (deviceListView.SelectedItems.Count == 0)
                {
                    return null;
                }

                return deviceListView.SelectedItems[0].Tag as QuickUsb;
            }
            set
            {
                // Make sure we only throw one SelectionChanged event
                selectionTimer.Enabled = false;

                deviceListView.SelectedItems.Clear();

                // Find the QuickUSB
                if (value != null)
                {
                    foreach (var kv in QuickUsbDict)
                    {
                        if (kv.Value.qusb == value)
                        {
                            kv.Value.lvi.Selected = true;
                        }
                    }
                }

                // Throw the SelecitonChanged event
                selectionTimer.Enabled = true;
                selectionTimer.Start();
            }
        }

        /// <summary>
        /// A property holding a list of all the currently selected QuickUSB objects
        /// </summary>
        public QuickUsb[] SelectedModules
        {
            get
            {
                QuickUsb[] qmuxList = new QuickUsb[deviceListView.SelectedItems.Count];
                for (int k = 0; k < qmuxList.Length; ++k)
                {
                    qmuxList[k] = deviceListView.SelectedItems[k].Tag as QuickUsb;
                }
                return qmuxList;
            }
            set
            {
                // Make sure we only throw one SelectionChanged event
                selectionTimer.Enabled = false;

                deviceListView.SelectedItems.Clear();

                // Find each QuickUSB and select it
                if (value != null)
                {
                    foreach (QuickUsb qusb in value)
                    {
                        foreach (var kv in QuickUsbDict)
                        {
                            if (kv.Value.qusb == qusb)
                            {
                                kv.Value.lvi.Selected = true;
                            }
                        }
                    }
                }

                // Throw the SelecitonChanged event
                selectionTimer.Enabled = true;
                selectionTimer.Start();
            }
        }

        /// <summary>
        /// Enable the control to include listing unprogrammed QuickUSB devices.  Note that you
        /// will not be able to communicate with these devices are they are not yet licensed
        /// and do not contain valid QuickUSB firmware.
        /// </summary>
        [Category("Behavior")]
        [Description("Enable scanning of new QuickUSB modules.")]
        public bool ShowUnprogrammedDevices
        {
            get
            {
                return showUnprogrammedDevices;
            }

            set
            {
                showUnprogrammedDevices = value;
            }
        }

        /// <summary>
        /// Enable the scan timer which issues a scan for connected/disconnected QuickUSB modules
        /// </summary>
        [Category("Behavior")]
        [Description("Enable scanning of new QuickUSB modules.")]
        public bool ScanEnabled
        {
            get
            {
                return scanEnabled;
            }
            set
            {
                scanEnabled = value;
                scanTimer.Enabled = scanEnabled;
            }
        }

        /// <summary>
        /// The the timer interval for the scan timer  which issues a scan for connected/disconnected QuickUSB modules
        /// </summary>
        [Category("Behavior")]
        [Description("Sets the time interval (in milliseconds) between successive scans for new QuickUSB modules.")]
        public int ScanInterval
        {
            get
            {
                return scanTimer.Interval;
            }
            set
            {
                scanTimer.Interval = value;
            }
        }

        /// <summary>
        /// Indicates whether the control should scan for connected modules on control creation
        /// </summary>
        [Category("Behavior")]
        [Description("Determines if the control should scan for QuickUSB modules when theh form initially loads.")]
        public bool ScanOnLoad
        {
            get
            {
                return scanOnLoad;
            }
            set
            {
                scanOnLoad = value;
            }
        }

        /// <summary>
        /// Determines if the QuickUSB control has a scrollbar if the QuickUSB icons do not fit in the client area of the control
        /// </summary>
        [Category("Behavior")]
        [Description("Indicates whether the control will display scrollbars if it contains more items than can fit in the client area.")]
        public bool Scrollable
        {
            get
            {
                return deviceListView.Scrollable;
            }
            set
            {
                deviceListView.Scrollable = value;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        [Category("Behavior")]
        [Description("Automatically select the first module that appears in the control.")]
        public bool AutoSelectFirstItem
        {
            get
            {
                return autoSelect;
            }
            set
            {
                autoSelect = value;
            }
        }

        /// <summary>
        /// Get or set the current view (see the listview control) of the QuickUSB control.  Only three views are supported: Small icons, large icons, and detail.
        /// </summary>
        [Category("Appearance")]
        [Description("Select one of the three support view styles modules can be shown in.")]
        public View View
        {
            get
            {
                return deviceListView.View;
            }
            set
            {
                if (value == View.LargeIcon || value == View.SmallIcon || value == View.Details)
                {
                    deviceListView.View = value;

                    // Have the listview auto resize the columns
                    deviceListView.ArrangeIcons();
                    AutoResizeByContentAndHeader();
                }
                else
                {
                    throw new NotSupportedException("The QuickUSB control only supports the following three view: Large Icon, Small Icon, and Details");
                }
            }
        }

        #endregion

        #region Public methods

        /// <summary>
        /// QuickUSB Windows Form Control
        /// </summary>
        public QuickUsbWinForms()
        {
            InitializeComponent();

            // Add the modules group
            deviceListView.Groups.Add(ModuleGroup);

            // Add the module columns
            InsertColumnData("Name", ColumnDataType.Name, 0);
            InsertColumnData("Serial", ColumnDataType.Serial, 1);
            InsertColumnData("Firmware Version", ColumnDataType.FirmwareVersion, 2);
            InsertColumnData("Firmware Model", ColumnDataType.Model, 3);

            // Emulate a click on the first column to default sort by its contents
            deviceListView_ColumnClick(null, new ColumnClickEventArgs(0));
        }

        /// <summary>
        /// Remove all devices in the control and perform a full rescan for 
        /// all devices
        /// </summary>
        public void FlushDevices()
        {
            performFlush = true;
            RescanForModules();
        }

        /// <summary>
        /// Force a scan for QuickUsb modules.  This is automatically called by the timer.
        /// </summary>
        public void RescanForModules()
        {
            QuickUsb qusb;
            qusbListInfo listInfo;
            List<string> leaveList = new List<string>();
            List<string> enterList = new List<string>();
            string[] arr;

            // Pause the timer while we process
            scanTimer.Stop();

            // Check the ScanOnLoad and ScanEnabled properties and listen to them
            if (loading)
            {
                if (!ScanOnLoad)
                {
                    return;
                }
            }
            else
            {
                if (!ScanEnabled)
                {
                    return;
                }
            }

            // Find all attached modules
            string[] attachedModules;
            if (!performFlush)
            {
                if (showUnprogrammedDevices)
                {
                    byte[] buffer = new byte[4096];
                    buffer[0] = (byte)'*';
                    buffer[1] = (byte)'.';
                    buffer[2] = (byte)'*';
                    attachedModules = QuickUsb.FindModules(ref buffer);
                }
                else
                {
                    attachedModules = QuickUsb.FindModules();
                }
            }
            else
            {
                attachedModules = new string[0];
                performFlush = false;
            }

            // Check for lost modules
            arr = new string[QuickUsbDict.Count];
            QuickUsbDict.Keys.CopyTo(arr, 0);
            for (int k = 0; k < arr.Length; ++k)
            {
                string devName = arr[k];

                if (QuickUsbDict[devName].attached && !ArrayContains(attachedModules, devName))
                {
                    // Hold a reference to the QuickUsb
                    qusb = QuickUsbDict[devName].qusb;

                    // Remove the module from the listview
                    deviceListView.Items.Remove(QuickUsbDict[devName].lvi);

                    // Remove the module from our list
                    QuickUsbDict.Remove(devName);
                    AutoResizeByContentAndHeader();

                    // Send the ModuleDisconnected event
                    if (ModuleDisconnected != null)
                    {
                        ModuleDisconnected(this, new ModuleEventArgs(qusb));
                    }
                }
            }

            // Check for new modules
            arr = new string[QuickUsbDict.Count];
            QuickUsbDict.Keys.CopyTo(arr, 0);
            bool alreadyAdded;
            foreach (string devName in attachedModules)
            {
                if (!ArrayContains<string>(arr, devName))
                {
                    // Create a new qusbListInfo item
                    listInfo = CreateQLVI(new QuickUsb(devName), null, out alreadyAdded);
                    listInfo.attached = true;
                    listInfo.lvi.ImageIndex = 0;

                    // Add the new QuickUsb to the list and listview
                    if (!alreadyAdded)
                    {
                        if (listInfo.visible)
                        {
                            deviceListView.Items.Add(listInfo.lvi);
                        }
                        QuickUsbDict.Add(devName, listInfo);
                        AutoResizeByContentAndHeader();
                    }

                    // Send the ModuleConnected event
                    if (ModuleConnected != null)
                    {
                        ModuleConnected(this, new ModuleEventArgs(listInfo.qusb));
                    }

                    // If this is the first module connected, select it
                    if (autoSelect && deviceListView.Items.Count == 1)
                    {
                        deviceListView.Items[0].Selected = true;
                    }
                }
            }

            // Restart the timer
            scanTimer.Start();
        }

        /// <summary>
        /// You may prevent QuickUsb modules connected to the host from being shown in the control.  This method will mark a hidden QuickUsb as now visible.
        /// </summary>
        /// <param name="qusb">The QuickUsb to show</param>
        public void ShowModule(QuickUsb qusb)
        {
            QuickUsbDict[qusb.Name].visible = true;
            deviceListView.Items.Add(QuickUsbDict[qusb.Name].lvi);
        }

        /// <summary>
        /// You may prevent QuickUsb modules connected to the host from being shown in the control.  This method will mark a visible QuickUsb as now hidden.
        /// </summary>
        /// <param name="qusb">The QuickUsb to hide</param>
        public void HideModule(QuickUsb qusb)
        {
            QuickUsbDict[qusb.Name].visible = false;
            deviceListView.Items.Remove(QuickUsbDict[qusb.Name].lvi);
        }

        /// <summary>
        /// You may prevent QuickUsb modules connected to the host from being shown in the control.  This method will marks all modules as visible.
        /// </summary>
        public void ShowAllModules()
        {
            foreach (var kv in QuickUsbDict)
            {
                if (kv.Value.visible == false)
                {
                    kv.Value.visible = true;
                    deviceListView.Items.Add(kv.Value.lvi);
                }
            }
        }

        /// <summary>
        /// You may prevent QuickUsb modules connected to the host from being shown in the control.  This method will marks all modules as hidden.
        /// </summary>
        public void HideAllModules()
        {
            foreach (var kv in QuickUsbDict)
            {
                if (kv.Value.visible == true)
                {
                    kv.Value.visible = false;
                    deviceListView.Items.Remove(kv.Value.lvi);
                }
            }
        }

        /// <summary>
        /// Check to see if a particular QuickUsb is visible on the control
        /// </summary>
        /// <param name="qusb">The QuickUsb to check</param>
        /// <returns></returns>
        public bool IsModuleVisible(QuickUsb qusb)
        {
            return QuickUsbDict[qusb.Name].visible;
        }

        #endregion

        #region Private methods

        static bool ArrayContains<T>(T[] arr, T value) where T : IComparable<T>
        {
            for (int k = 0; k < arr.Length; ++k)
            {
                if (arr[k].CompareTo(value) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        private qusbListInfo CreateQLVI(QuickUsb qusb, string devName, out bool alreadyAdded)
		{
			// The name is only passed in when it can't be extract from the info parameter (probably bc the module is not connected)
			if (qusb == null)
			{
				alreadyAdded = false;
				return null;
			}
			if (qusb.Name == null)
			{
				if (devName == null)
				{
					throw new Exception("Unable to create item from QuickUSB - Too little information");
				}
			}
			else
			{
				if (devName != null && devName != qusb.Name)
				{
					throw new Exception("Unable to create item from QuickUSB - Conflicting module information");
				}
				devName = qusb.Name;
			}

			// First, see if this module has already been added
			string[] arr;
			arr = new string[QuickUsbDict.Count];
			QuickUsbDict.Keys.CopyTo(arr, 0);
			for (int k = 0; k < arr.Length; ++k)
			{
				if (QuickUsbDict[arr[k]].qusb.Name == devName)
				{
					alreadyAdded = true;
					return QuickUsbDict[arr[k]];
				}
			}

			// Create a new qusbListInfo item
			var listInfo = new qusbListInfo();
			listInfo.qusb = qusb;
			listInfo.attached = false;

            // Variables
			string newSerial;
			newSerial = listInfo.qusb.Serial;
			Version v;

            listInfo.lvi = new ListViewItem(new string[columnData.Count], 0, ModuleGroup);

            //  Read device descriptor
            USB_DEVICE_DESCRIPTOR desc = new USB_DEVICE_DESCRIPTOR();
            IntPtr hDevice;
            int result;
            result = QuickUsbOpenUtil(out hDevice, listInfo.qusb.Name);
            if (result != 0)
            {
                try
                {
                    result = QuickUsbGetDeviceDesc(listInfo.qusb.Handle, ref desc, 18);
                }
                catch
                {
                    // User is using an older DLL, so we won't be able to read descriptor information
                }
                result = QuickUsbCloseUtil(hDevice);
            }

            // Add data to columns
            for (int k = 0; k < columnData.Count; ++k)
            {
                switch (columnData[k].type)
	            {
		            case ColumnDataType.Name:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, listInfo.qusb.Name);
                        break;

                    case ColumnDataType.Make:
                        if (showUnprogrammedDevices && listInfo.qusb.Make == null && desc.idVendor == 0x04b4 && desc.idProduct == 0x8613)
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, "Bitwise Systems");
                        }
                        else
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, listInfo.qusb.Make);
                        }
                        break;

                    case ColumnDataType.Model:
                        if (showUnprogrammedDevices && listInfo.qusb.Make == null)
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, "Unprogrammed");
                        }
                        else
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, listInfo.qusb.Model);
                        }
                        break;

                    case ColumnDataType.Serial:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, listInfo.qusb.Serial);
                        break;

                    case ColumnDataType.PrivateModel:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, qusb.GetStringDescriptor((QuickUsb.StringDescriptor)4));
                        break;

                    case ColumnDataType.IOModel:
                        string ioModel = qusb.GetStringDescriptor((QuickUsb.StringDescriptor)4);

                        // The IO Model is in parenthesis in the private model string.  If there are no parentheses, then it's the simple IO model
                        if (ioModel != null)
                        {
                            int start = ioModel.IndexOf('(');
                            int end = ioModel.IndexOf(')');
                            if (start == -1 || end == -1)
                            {
                                ioModel = "Simple I/O";
                            }
                            else
                            {
                                ioModel = ioModel.Substring(start + 1, (end - start - 1));
                            }
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, ioModel);
                        }
                        break;

                    case ColumnDataType.PrivateSerial:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, qusb.GetStringDescriptor((QuickUsb.StringDescriptor)5));
                        break;

                    case ColumnDataType.PrivateMake:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, qusb.GetStringDescriptor((QuickUsb.StringDescriptor)6));
                        break;

                    case ColumnDataType.FirmwareVersion:
                        // Make sure that device contains firmware before trying to communicate with it
                        if (listInfo.qusb.Make != null)
                        {
                            listInfo.qusb.GetFirmwareVersion(out v);
                            if (v != null)
                            {
                                listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, String.Format("{0}.{1}.{2}", v.Major, v.Minor, v.Build));
                            }
                        }
                        break;

                    case ColumnDataType.DriverVersion:
                        QuickUsb.GetDriverVersion(out v);
                        if (v != null)
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, String.Format("{0}.{1}.{2}", v.Major, v.Minor, v.Build));
                        }
                        break;

                    case ColumnDataType.LibraryVersion:
                        QuickUsb.GetDllVersion(out v);
                        if (v != null)
                        {
                            listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, String.Format("{0}.{1}.{2}", v.Major, v.Minor, v.Build));
                        }
                        break;

                    case ColumnDataType.VID:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, "0x" + desc.idVendor.ToString("X4"));
                        break;

                    case ColumnDataType.PID:
                        listInfo.lvi.SubItems[k] = new ListViewItem.ListViewSubItem(listInfo.lvi, "0x" + desc.idProduct.ToString("X4"));
                        break;
	            }
            }

			listInfo.visible = true;
			listInfo.lvi.Name = listInfo.qusb.Make;
            listInfo.lvi.ToolTipText = "Name: " + devName + Environment.NewLine +
                                       "Serial: " + newSerial + Environment.NewLine;
			listInfo.lvi.Tag = listInfo.qusb; // NOTE: Tag of LVI contains QuickUSB object (Very convenient)
			listInfo.lvi.ImageIndex = 1;	// Default to unattached icon

			alreadyAdded = false;
			return listInfo;
		}

        private bool isModuleInControl(QuickUsb qusb, out ListViewItem foundItem)
        {
            // Loop through all the items in the listview
            foreach (ListViewItem item in deviceListView.Items)
            {
                if (qusb.Name == qusb.Name)
                {
                    foundItem = item;
                    return true;
                }
            }

            // Not found
            foundItem = null;
            return false;
        }

        private void deviceListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Restart timer
            selectionTimer.Stop();
            selectionTimer.Start();
        }

        private void QuickUsbWinForms_Load(object sender, EventArgs e)
        {
            selectionTimer.Stop();
            selectionTimer.Enabled = true;

            if (scanOnLoad)
            {
                loading = true;
                RescanForModules();
                loading = false;
            }
        }

        private void deviceListView_EnabledChanged(object sender, EventArgs e)
        {
            if (Enabled == false)
            {
                scanTimer.Enabled = false;
            }
            else
            {
                scanTimer.Enabled = scanEnabled;
            }
        }

        private void scanTimer_Tick(object sender, EventArgs e)
        {
            RescanForModules();
        }

        private void smallIconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deviceListView.View = View.SmallIcon;

            // Have the listview auto resize the columns
            deviceListView.ArrangeIcons();
            AutoResizeByContentAndHeader();
        }

        private void largeIconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deviceListView.View = View.LargeIcon;

            // Have the listview auto resize the columns
            deviceListView.ArrangeIcons();
            AutoResizeByContentAndHeader();
        }

        private void detailToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deviceListView.View = View.Details;

            // Have the listview auto resize the columns
            deviceListView.ArrangeIcons();
            AutoResizeByContentAndHeader();
        }

        private void scrollableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deviceListView.Scrollable = !deviceListView.Scrollable;
        }

        private void AutoResizeByContentAndHeader()
        {
            foreach (ColumnHeader column in deviceListView.Columns)
            {
                column.AutoResize(ColumnHeaderAutoResizeStyle.ColumnContent);
                int contentAutoWidth = column.Width;
                column.AutoResize(ColumnHeaderAutoResizeStyle.HeaderSize);
                if (contentAutoWidth > column.Width)
                {
                    column.AutoResize(ColumnHeaderAutoResizeStyle.ColumnContent);
                }
            }
        }

        private void deviceListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            // Determine whether the column is the same as the last column clicked.
            if (e.Column != previouslySortedColumn)
            {
                previouslySortedColumn = e.Column;
            }
            else
            {
                // Determine what the last sort order was and change it.
                if (columnSortOrder[e.Column] == SortOrder.Ascending)
                {
                    columnSortOrder[e.Column] = SortOrder.Descending;
                }
                else
                {
                    columnSortOrder[e.Column] = SortOrder.Ascending;
                }
            }
            deviceListView.Sorting = columnSortOrder[e.Column];

            // Set the sort column to the new column.
            for (int k = 0; k < deviceListView.Columns.Count; ++k)
            {
                if (k == e.Column)
                {
                    SetSortIcons(k, deviceListView.Sorting);
                }
                else
                {
                    SetSortIcons(k, SortOrder.None);
                }
            }

            // Set the ListViewItemSorter property to a new ListViewItemComparer object.
            deviceListView.ListViewItemSorter = new ListViewItemComparer(e.Column, deviceListView.Sorting);

            // Call the sort method to manually sort.
            deviceListView.Sort();
        }

        private void SetSortIcons(int column, SortOrder order)
        {
            IntPtr hHeader = SendMessage(deviceListView.Handle, LVM_GETHEADER, IntPtr.Zero, IntPtr.Zero);
            IntPtr newColumn = new IntPtr(column);
            HDITEM hdItem;
            IntPtr rtn;

            hdItem = new HDITEM();
            hdItem.mask = HDI_FORMAT;
            rtn = SendMessageITEM(hHeader, HDM_GETITEM, newColumn, ref hdItem);
            switch (order)
            {
                case SortOrder.None:
                    hdItem.fmt &= ~HDF_SORTDOWN & ~HDF_SORTUP;
                    break;
                case SortOrder.Ascending:
                    hdItem.fmt &= ~HDF_SORTDOWN;
                    hdItem.fmt |= HDF_SORTUP;
                    break;
                case SortOrder.Descending:
                    hdItem.fmt &= ~HDF_SORTUP;
                    hdItem.fmt |= HDF_SORTDOWN;
                    break;
            }
            rtn = SendMessageITEM(hHeader, HDM_SETITEM, newColumn, ref hdItem);
        }

        private void contextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            // Set enabled state for context sensitive menu items
            propertiesToolStripMenuItem.Enabled = (deviceListView.SelectedItems.Count == 1);

            // Set the proper view submenu checked items
            smallIconsToolStripMenuItem.Checked = (deviceListView.View == View.SmallIcon);
            largeIconsToolStripMenuItem.Checked = (deviceListView.View == View.LargeIcon);
            detailToolStripMenuItem.Checked = (deviceListView.View == View.Details);
            scrollableToolStripMenuItem.Checked = deviceListView.Scrollable;
        }

        private void selectionTimer_Tick(object sender, EventArgs e)
        {
            selectionTimer.Stop();

            if (SelectionChanged != null)
            {
                SelectionChanged(this, new EventArgs());
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct HDITEM
        {
            public Int32 mask;
            public Int32 cxy;
            [MarshalAs(UnmanagedType.LPTStr)]
            public String pszText;
            public IntPtr hbm;
            public Int32 cchTextMax;
            public Int32 fmt;
            public Int32 lParam;
            public Int32 iImage;
            public Int32 iOrder;
        };

        // Parameters for ListView-Headers
        private const Int32 HDI_FORMAT = 0x0004;
        private const Int32 HDF_LEFT = 0x0000;
        private const Int32 HDF_STRING = 0x4000;
        private const Int32 HDF_SORTUP = 0x0400;
        private const Int32 HDF_SORTDOWN = 0x0200;
        private const Int32 LVM_GETHEADER = 0x1000 + 31;  // LVM_FIRST + 31
        private const Int32 HDM_GETITEM = 0x1200 + 11;  // HDM_FIRST + 11
        private const Int32 HDM_SETITEM = 0x1200 + 12;  // HDM_FIRST + 12

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessageITEM(IntPtr Handle, Int32 msg, IntPtr wParam, ref HDITEM lParam);

        #endregion

        #region EventArg classes

        /// <summary>
        /// The module event arguments
        /// </summary>
        public class ModuleEventArgs : EventArgs
        {
            /// <summary>
            /// The QuickUsb object the event refers to
            /// </summary>
            public QuickUsb qusb;

            /// <summary>
            /// Default constructor
            /// </summary>
            /// <param name="qusb">The disconnected QuickUsb object</param>
            public ModuleEventArgs(QuickUsb qusb)
            {
                this.qusb = qusb;
            }
        }

        #endregion

    }

    // Implements the manual sorting of items by columns.
    class ListViewItemComparer : IComparer
    {
        private int col;
        private SortOrder order;
        public ListViewItemComparer()
        {
            col = 0;
            order = SortOrder.Ascending;
        }
        public ListViewItemComparer(int column, SortOrder order)
        {
            col = column;
            this.order = order;
        }
        public int Compare(object x, object y)
        {
            int returnVal = -1;
            returnVal = String.Compare(((ListViewItem)x).SubItems[col].Text, ((ListViewItem)y).SubItems[col].Text);
            // Determine whether the sort order is descending.
            if (order == SortOrder.Descending)
            {
                // Invert the value returned by String.Compare.
                returnVal *= -1;
            }
            return returnVal;
        }
    }
}
