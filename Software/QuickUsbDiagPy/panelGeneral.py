#Boa:FramePanel:panelGeneral
"""
============================================================================
 Title       : panelGeneral.py
 Description : 
 Notes       : 
 History     :

 Copyright (c) 2012 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real,
 Suite E, Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

============================================================================
"""

# wx imports
import wx
from wx.lib.anchors import LayoutAnchors



# Python imports
import os



# Bitwise Systems imports
from QuickUsb import *



# Generated Boa Constructor wx IDs
[wxID_PANELGENERAL, wxID_PANELGENERALBFPGABROWSE, 
 wxID_PANELGENERALBFPGAUPLOAD, wxID_PANELGENERALBISCONFIGURED, 
 wxID_PANELGENERALCBFPGATYPE, wxID_PANELGENERALRBHIGHSPEED, 
 wxID_PANELGENERALRBLOWSPEED, wxID_PANELGENERALSBFPGA, 
 wxID_PANELGENERALSBGENERAL, wxID_PANELGENERALSTDESCRIPTION, 
 wxID_PANELGENERALSTFPGAFILE, wxID_PANELGENERALSTFPGATYPE, 
 wxID_PANELGENERALSTHWVERSION, wxID_PANELGENERALSTSERIALNUMBER, 
 wxID_PANELGENERALSTUSBSPEED, wxID_PANELGENERALTBFPGAFILENAME, 
 wxID_PANELGENERALTCDESCRIPTION, wxID_PANELGENERALTCHWVERSION, 
 wxID_PANELGENERALTCSERIALNUMBER, 
] = [wx.NewId() for _init_ctrls in range(19)]



class panelGeneral(wx.Panel):
    def _init_coll_szfgFpgaMain_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stFpgaFile, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddSizer(self.szbFpgaFileName, 0, border=0,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.GROW)
        parent.AddWindow(self.bFpgaBrowse, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_RIGHT | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.stFpgaType, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddSizer(self.szbFpga, 0, border=0, flag=0)
        parent.AddWindow(self.bFpgaUpload, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szbFpgaFileName_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.tbFpgaFileName, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL | wx.GROW)

    def _init_coll_szfgGeneral_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(1)

    def _init_coll_szfgGeneral_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stSerialNumber, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tcSerialNumber, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.stDescription, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tcDescription, 0, border=4, flag=wx.ALL | wx.GROW)
        parent.AddWindow(self.stHwVersion, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tcHwVersion, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.stUsbSpeed, 0, border=4, flag=wx.ALL)
        parent.AddSizer(self.szbUsbSpeed, 0, border=4, flag=wx.ALL | wx.BOTTOM)

    def _init_coll_szsbFpga_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szfgFpgaMain, 1, border=0, flag=wx.GROW)

    def _init_coll_szfgFpgaMain_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(1)

    def _init_coll_szbFpga_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbFpgaType, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.bIsConfigured, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szsbGeneral_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szfgGeneral, 0, border=0, flag=wx.GROW)

    def _init_coll_szbUsbSpeed_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.rbLowSpeed, 0, border=4,
              flag=wx.RIGHT | wx.LEFT | wx.BOTTOM | wx.GROW)
        parent.AddSpacer(wx.Size(4, 4), border=0, flag=0)
        parent.AddWindow(self.rbHighSpeed, 0, border=4, flag=wx.ALL | wx.GROW)

    def _init_coll_bszGeneralMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbGeneral, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbFpga, 0, border=0, flag=wx.GROW)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszGeneralMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbGeneral = wx.StaticBoxSizer(box=self.sbGeneral,
              orient=wx.VERTICAL)

        self.szsbFpga = wx.StaticBoxSizer(box=self.sbFpga, orient=wx.VERTICAL)

        self.szfgGeneral = wx.FlexGridSizer(cols=2, hgap=8, rows=1, vgap=4)

        self.szbUsbSpeed = wx.BoxSizer(orient=wx.VERTICAL)

        self.szfgFpgaMain = wx.FlexGridSizer(cols=3, hgap=8, rows=2, vgap=4)

        self.szbFpgaFileName = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbFpga = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszGeneralMain_Items(self.bszGeneralMain)
        self._init_coll_szsbGeneral_Items(self.szsbGeneral)
        self._init_coll_szsbFpga_Items(self.szsbFpga)
        self._init_coll_szfgGeneral_Items(self.szfgGeneral)
        self._init_coll_szfgGeneral_Growables(self.szfgGeneral)
        self._init_coll_szbUsbSpeed_Items(self.szbUsbSpeed)
        self._init_coll_szfgFpgaMain_Items(self.szfgFpgaMain)
        self._init_coll_szfgFpgaMain_Growables(self.szfgFpgaMain)
        self._init_coll_szbFpgaFileName_Items(self.szbFpgaFileName)
        self._init_coll_szbFpga_Items(self.szbFpga)

        self.SetSizer(self.bszGeneralMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELGENERAL, name='panelGeneral',
              parent=prnt, pos=wx.Point(567, 455), size=wx.Size(673, 307),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(657, 269))

        self.sbGeneral = wx.StaticBox(id=wxID_PANELGENERALSBGENERAL,
              label='General', name='sbGeneral', parent=self, pos=wx.Point(0,
              0), size=wx.Size(657, 171), style=0)
        self.sbGeneral.SetToolTipString('')

        self.sbFpga = wx.StaticBox(id=wxID_PANELGENERALSBFPGA, label='FPGA',
              name='sbFpga', parent=self, pos=wx.Point(0, 171),
              size=wx.Size(657, 89), style=0)
        self.sbFpga.SetToolTipString('')

        self.rbLowSpeed = wx.RadioButton(id=wxID_PANELGENERALRBLOWSPEED,
              label='Low-Speed (12 Mbps)', name='rbLowSpeed', parent=self,
              pos=wx.Point(99, 120), size=wx.Size(131, 13), style=0)
        self.rbLowSpeed.SetValue(True)
        self.rbLowSpeed.SetToolTipString('')
        self.rbLowSpeed.SetMinSize(wx.Size(-1, -1))
        self.rbLowSpeed.Bind(wx.EVT_RADIOBUTTON, self.OnRbUsbSpeedRadiobutton,
              id=wxID_PANELGENERALRBLOWSPEED)

        self.stDescription = wx.StaticText(id=wxID_PANELGENERALSTDESCRIPTION,
              label='Description:', name='stDescription', parent=self,
              pos=wx.Point(9, 58), size=wx.Size(57, 13), style=0)
        self.stDescription.SetMinSize(wx.Size(-1, -1))
        self.stDescription.SetToolTipString('')

        self.tcDescription = wx.TextCtrl(id=wxID_PANELGENERALTCDESCRIPTION,
              name='tcDescription', parent=self, pos=wx.Point(95, 54),
              size=wx.Size(553, 21), style=wx.TE_READONLY, value='')
        self.tcDescription.SetToolTipString('')

        self.tbFpgaFileName = wx.TextCtrl(id=wxID_PANELGENERALTBFPGAFILENAME,
              name='tbFpgaFileName', parent=self, pos=wx.Point(82, 192),
              size=wx.Size(503, 21), style=0, value='')
        self.tbFpgaFileName.SetConstraints(LayoutAnchors(self.tbFpgaFileName,
              True, False, True, False))
        self.tbFpgaFileName.SetMinSize(wx.Size(-1, -1))
        self.tbFpgaFileName.SetToolTipString('')
        self.tbFpgaFileName.Bind(wx.EVT_TEXT, self.OnTbFpgaFileNameText,
              id=wxID_PANELGENERALTBFPGAFILENAME)

        self.tcSerialNumber = wx.TextCtrl(id=wxID_PANELGENERALTCSERIALNUMBER,
              name='tcSerialNumber', parent=self, pos=wx.Point(95, 21),
              size=wx.Size(100, 21), style=wx.TE_READONLY, value='')
        self.tcSerialNumber.SetToolTipString('')

        self.stFpgaFile = wx.StaticText(id=wxID_PANELGENERALSTFPGAFILE,
              label='FPGA File:', name='stFpgaFile', parent=self,
              pos=wx.Point(9, 197), size=wx.Size(49, 13), style=0)
        self.stFpgaFile.SetMinSize(wx.Size(-1, -1))
        self.stFpgaFile.SetToolTipString('')

        self.stSerialNumber = wx.StaticText(id=wxID_PANELGENERALSTSERIALNUMBER,
              label='Serial Number:', name='stSerialNumber', parent=self,
              pos=wx.Point(9, 25), size=wx.Size(70, 13), style=0)
        self.stSerialNumber.SetMinSize(wx.Size(-1, -1))
        self.stSerialNumber.SetToolTipString('')

        self.bFpgaBrowse = wx.Button(id=wxID_PANELGENERALBFPGABROWSE,
              label='Browse...', name='bFpgaBrowse', parent=self,
              pos=wx.Point(601, 192), size=wx.Size(47, 24), style=0)
        self.bFpgaBrowse.SetToolTipString('')
        self.bFpgaBrowse.Bind(wx.EVT_BUTTON, self.OnBFpgaBrowseButton,
              id=wxID_PANELGENERALBFPGABROWSE)

        self.rbHighSpeed = wx.RadioButton(id=wxID_PANELGENERALRBHIGHSPEED,
              label='High-Speed (480 Mbps)', name='rbHighSpeed', parent=self,
              pos=wx.Point(99, 145), size=wx.Size(131, 13), style=0)
        self.rbHighSpeed.SetValue(True)
        self.rbHighSpeed.SetToolTipString('')
        self.rbHighSpeed.SetMinSize(wx.Size(-1, -1))
        self.rbHighSpeed.Bind(wx.EVT_RADIOBUTTON, self.OnRbUsbSpeedRadiobutton,
              id=wxID_PANELGENERALRBHIGHSPEED)

        self.bFpgaUpload = wx.Button(id=wxID_PANELGENERALBFPGAUPLOAD,
              label='Upload', name='bFpgaUpload', parent=self, pos=wx.Point(601,
              228), size=wx.Size(47, 23), style=0)
        self.bFpgaUpload.Bind(wx.EVT_BUTTON, self.OnBFpgaUploadButton,
              id=wxID_PANELGENERALBFPGAUPLOAD)

        self.stFpgaType = wx.StaticText(id=wxID_PANELGENERALSTFPGATYPE,
              label='FPGA Type:', name='stFpgaType', parent=self,
              pos=wx.Point(9, 233), size=wx.Size(57, 13), style=0)
        self.stFpgaType.SetMinSize(wx.Size(-1, -1))
        self.stFpgaType.SetToolTipString('')

        self.tcHwVersion = wx.TextCtrl(id=wxID_PANELGENERALTCHWVERSION,
              name='tcHwVersion', parent=self, pos=wx.Point(95, 87),
              size=wx.Size(553, 21), style=wx.TE_READONLY, value='')
        self.tcHwVersion.SetToolTipString('')

        self.cbFpgaType = wx.Choice(choices=["Altera Passive Serial",
              "Xilinx Slave Serial"], id=wxID_PANELGENERALCBFPGATYPE,
              name='cbFpgaType', parent=self, pos=wx.Point(82, 229),
              size=wx.Size(54, 21))
        self.cbFpgaType.SetToolTipString('')
        self.cbFpgaType.Bind(wx.EVT_COMBOBOX, self.OnCbFpgaTypeCombobox,
              id=wxID_PANELGENERALCBFPGATYPE)

        self.stUsbSpeed = wx.StaticText(id=wxID_PANELGENERALSTUSBSPEED,
              label='USB Speed:', name='stUsbSpeed', parent=self,
              pos=wx.Point(9, 120), size=wx.Size(56, 13), style=0)
        self.stUsbSpeed.SetMinSize(wx.Size(-1, -1))
        self.stUsbSpeed.SetToolTipString('')

        self.stHwVersion = wx.StaticText(id=wxID_PANELGENERALSTHWVERSION,
              label='HW Version:', name='stHwVersion', parent=self,
              pos=wx.Point(9, 91), size=wx.Size(59, 13), style=0)
        self.stHwVersion.SetMinSize(wx.Size(-1, -1))
        self.stHwVersion.SetToolTipString('')

        self.bIsConfigured = wx.Button(id=wxID_PANELGENERALBISCONFIGURED,
              label='Is Configured?', name='bIsConfigured', parent=self,
              pos=wx.Point(144, 228), size=wx.Size(92, 23), style=0)
        self.bIsConfigured.Bind(wx.EVT_BUTTON, self.OnBIsConfiguredButton,
              id=wxID_PANELGENERALBISCONFIGURED)

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        """Constructor"""
        # Populate ourself with our Boa-generated GUI
        self._init_ctrls(parent)



    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        # Only unable the upload button if a valid file has been specified
        self.bFpgaUpload.Enable(ena and os.path.isfile(self.tbFpgaFileName.GetValue()))
        
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our 
        controls
        """
        # Get the QuickUSB Model
        (ok, Model) = qusb.Model
        if not ok:
            self.TheFrame.SetStatus("Model() failed with error: " + str(Error(qusb.LastError())))
            return
        self.tcDescription.SetValue(Model)
        
        # Get the QuickUSB Serial
        (ok, Serial) = qusb.Serial
        if not ok:
            self.TheFrame.SetStatus("Serial() failed with error: " + str(Error(qusb.LastError())))
            return
        self.tcSerialNumber.SetValue(Serial)
        
        # Get the QuickUSB VesionSpeed Info
        (ok, VersionSpeed) = qusb.ReadSetting(Setting.VersionSpeed)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        version = (VersionSpeed / 0xff) & 0xff
        
        # Decode the VersionSpeed info into a meaningful string
        if version == 0x00:
            VersionString = "CY7C68013 Rev A/B (EZ-USB FX2)"
        elif version == 0x01:
            VersionString = "CY7C68013A Rev A (EZ-USB FX2LP)"
        elif version == 0x02:
            VersionString = "CY7C68013 Rev C/D (EZ-USB FX2)"
        elif version == 0x04:
            VersionString = "CY7C68013 Rev E (EZ-USB FX2)"
        else:
            VersionString = "Unknown"
        self.tcHwVersion.SetValue(VersionString)
        
        # Get the USB operating speed speed
        (ok, speed) = qusb.ReadSetting(Setting.CpuConfig)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        if speed & 0x8000:
            self.rbHighSpeed.SetValue(True)
        else:
            self.rbLowSpeed.SetValue(True)
        
        # Set the FPGA combobox based on the FPGA setting
        (ok, value) = qusb.ReadSetting(Setting.FpgaType)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        self.cbFpgaType.SetSelection(value & 0x0001)



    def LoadConf(self, conf):
        """
        Called by frameMain when we are to load our configuration settings
        """
        if 'FpgaFileName' in conf:
            self.tbFpgaFileName.SetValue(conf['FpgaFileName'])
            self.tbFpgaFileName.SetInsertionPointEnd()
        else:
            self.tbFpgaFileName.SetValue("")
            
            
            
    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        conf = {'FpgaFileName':self.tbFpgaFileName.GetValue()}
        return conf



    def OnRbUsbSpeedRadiobutton(self, event):
        """
        Event handler for USB Speed selection.  Changing the USB Speed with
        cause the QuickUSB to re-enumerate.
        """
        # Ensure a module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Read old setting
        (ok, value) = qusb.ReadSetting(Setting.CpuConfig)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Change setting
        if self.rbHighSpeed.GetValue():
            value = value | 0x8000
        elif self.rbLowSpeed.GetValue():
            value = value & ~0x8000
        
        # Write new setting
        (ok,) = qusb.WriteSetting(Setting.CpuConfig, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        
        
    def OnTbFpgaFileNameText(self, event):
        """
        Event handler for FPGA file name textbox
        """
        # If any text in the FPGA file textbox changes, enable the upload
        # button if we now have a valid path
        self.bFpgaUpload.Enable(os.path.isfile(self.tbFpgaFileName.GetValue()))



    def OnBFpgaBrowseButton(self, event):
        """
        Event handler for FPGA browse button
        """
        # Create the File open dialog
        dlg = wx.FileDialog(self, 'Open', '.', '', 'Altera RBF Files (*.rbf)|*.rbf|Xilinx BIN Files (*.bin)|*.bin|All Files (*.*)|*.*', wx.OPEN)
        
        # Set the default file filter selection to match the FPGA type
        dlg.SetFilterIndex(self.cbFpgaType.GetSelection())
        try:
            # Show the dialog
            if dlg.ShowModal() == wx.ID_OK:
                # If the user accepted, get the indicated file path
                fileName = dlg.GetPath()
                
                # Set the new file path
                self.tbFpgaFileName.SetValue(fileName)
                
                # Scroll to the end of the file path textbox so that if we have
                # a really long path we can at least see the filename
                self.tbFpgaFileName.SetInsertionPointEnd()
        finally:
            # Always destroy the dialog when we are done
            dlg.Destroy()



    def OnBFpgaUploadButton(self, event):
        """
        Event handler for the FPGA upload button
        """
        # Ensure a module is selected and we have a valid file
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb or not os.path.isfile(self.tbFpgaFileName.GetValue()):
            return
        
        # Initialize variables
        ok = True
        configured = False
        
        # Disable the upload button so that it is not clicked while we are
        # programming
        self.bFpgaUpload.Enable(False)
        
        # Open FPGA file
        try:
            fin = open(self.tbFpgaFileName.GetValue(), 'rb')
        except:
            ok = False
        
        # If we couldn't open the file, quit
        if not ok:
            return
        
        # Determine file length
        start = fin.tell()
        fin.seek(0, 2)
        end = fin.tell()
        fin.seek(0, 0)
        bytes = end - start
        
        # Setup the progress bar
        self.TheFrame.gProgress.SetRange(bytes)
        self.TheFrame.gProgress.SetValue(0)
        
        # As this is potentially a slow process, set the mouse cursor to busy
        wx.BeginBusyCursor()
        
        # We are going to write the FPGA data in 64 byte chucks (the maximum
        # allowed as documented in the QuickUSB User Guide).  We first need to
        # create a data buffer to hold the data.
        buffer = CreateByteBuffer(64)
        
        # Start the configuration
        (ok,) = qusb.StartFpgaConfiguration()
        if not ok:
            self.TheFrame.SetStatus("StartFpgaConfiguration() failed with error: " + str(Error(qusb.LastError())))

            # Display a message to the user
            wx.MessageBox("Unable to start FPGA configuration.  Please ensure that the FPGA is both powered and connected to the QuickUSB Module.", "FPGA Error")

            # Re-enable the upload button
            self.bFpgaUpload.Enable()

            # Close the file
            fin.close(); fin = None
        
            # We must restore the cursor
            wx.EndBusyCursor()
            return

        # Loop through all the data in the file
        for k in xrange(0, bytes, 64):
            # Read 64 bytes of data from the file
            data = fin.read(64)
            
            # File reads always return python arrays, but we need to use our own
            # buffer so we must copy the data over
            for j in xrange(len(data)):
                buffer[j] = ord(data[j])
                
            # Now, write the 64 bytes to the FPGA
            (ok,) = qusb.WriteFpgaData(buffer, len(data))
            if not ok:
                self.TheFrame.SetStatus("WriteFpgaData() failed with error: " + str(Error(qusb.LastError())))
                
                # Re-enable the upload button
                self.bFpgaUpload.Enable()

                # Close the file
                fin.close(); fin = None
            
                # We must restore the cursor
                wx.EndBusyCursor()
                return
            
            # Update the progress bar
            self.TheFrame.gProgress.SetValue(k + len(data))
            
            # This is a slow process, so we should process GUI messages
            wx.Yield()

        # Close the file
        fin.close(); fin = None
        
        # We must restore the cursor
        wx.EndBusyCursor()
        
        # Check if FPGA was successfully configured
        (ok, configured) = qusb.IsFpgaConfigured()
        if not ok:
            self.TheFrame.SetStatus("IsFpgaConfigured() failed with error: " + str(Error(qusb.LastError())))
            return
        
        if not configured:
            self.TheFrame.gProgress.SetValue(0)
            self.TheFrame.SetStatus("FPGA configuration failed!")
        else:
            self.TheFrame.SetStatus("FPGA configuration succeeded!")
            
        # Re-enable the upload button
        self.bFpgaUpload.Enable()

        # Now that the programming is complete, sound the bell!
        wx.Bell()
        
        # Reset the progress bar, but wait a little first so the user can
        # really see that it hit 100%
        wx.CallLater(2000, self.TheFrame.gProgress.SetValue, 0)



    def OnCbFpgaTypeCombobox(self, event):
        """
        Event handler for the FPGA type combobox
        """
        1/0
        return
        # Ensure a module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        # Read setting so we don't change any other bits than the ones we
        # want to
        (ok, value) = qusb.ReadSetting(Setting.FpgaType)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Change setting
        if self.cbFpgaType.GetSelection() == 0:
            value = value & ~0x0001
        elif self.cbFpgaType.GetSelection() == 1:
            value = value | 0x0001
        
        # Write new setting
        (ok,) = qusb.WriteSetting(Setting.FpgaType, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return



    def OnBIsConfiguredButton(self, event):
        # Ensure a module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        # Check if FPGA was successfully configured
        (ok, configured) = qusb.IsFpgaConfigured()
        if not ok:
            self.TheFrame.SetStatus("IsFpgaConfigured() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Report our status
        if ok and configured:
            self.TheFrame.SetStatus("FPGA is configured")
        else:
            self.TheFrame.SetStatus("FPGA is not configured")
