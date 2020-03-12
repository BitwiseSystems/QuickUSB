#Boa:FramePanel:panelCommand
"""
============================================================================
 Title       : panelCommand.py
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

# Python imports
import time

# Bitwise Systems imports
from QuickUsb import *

# Generated Boa Constructor wx IDs
[wxID_PANELCOMMAND, wxID_PANELCOMMANDBCMDREAD, wxID_PANELCOMMANDBCMDWRITE, 
 wxID_PANELCOMMANDCBAUTOINCADDRESS, wxID_PANELCOMMANDCBUSEADDRESS, 
 wxID_PANELCOMMANDCBWORDWIDE, wxID_PANELCOMMANDSBCMDREAD, 
 wxID_PANELCOMMANDSBCMDSETUP, wxID_PANELCOMMANDSBCMDWRITE, 
 wxID_PANELCOMMANDSTATICTEXT4, wxID_PANELCOMMANDSTATICTEXT5, 
 wxID_PANELCOMMANDSTATICTEXT6, wxID_PANELCOMMANDTBCMDBYTES, 
 wxID_PANELCOMMANDTBCMDREADADDRESS, wxID_PANELCOMMANDTBCMDREADDATA, 
 wxID_PANELCOMMANDTBCMDWRITEADDRESS, wxID_PANELCOMMANDTBCMDWRITEDATA, 
] = [wx.NewId() for _init_ctrls in range(17)]

class panelCommand(wx.Panel):
    def _init_coll_szsbCmdSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbUseAddress, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.cbAutoIncAddress, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.cbWordWide, 0, border=4, flag=wx.GROW | wx.ALL)

    def _init_coll_bszCmdMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbCmdSetup, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbCmdRead, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbCmdWrite, 0, border=0, flag=wx.GROW)

    def _init_coll_szbCmdWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText6, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbCmdWriteAddress, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL | wx.GROW)

    def _init_coll_szsbCmdWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbCmdWrite, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.tbCmdWriteData, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.bCmdWrite, 0, border=4,
              flag=wx.ALL | wx.ALIGN_RIGHT)

    def _init_coll_szbCmdRead_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText4, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbCmdReadAddress, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.staticText5, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbCmdBytes, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_CENTER_VERTICAL)

    def _init_coll_szsbCmdRead_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbCmdRead, 0, border=0, flag=0)
        parent.AddWindow(self.tbCmdReadData, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.bCmdRead, 0, border=4,
              flag=wx.ALL | wx.ALIGN_RIGHT)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszCmdMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbCmdSetup = wx.StaticBoxSizer(box=self.sbCmdSetup,
              orient=wx.VERTICAL)

        self.szsbCmdRead = wx.StaticBoxSizer(box=self.sbCmdRead,
              orient=wx.VERTICAL)

        self.szsbCmdWrite = wx.StaticBoxSizer(box=self.sbCmdWrite,
              orient=wx.VERTICAL)

        self.szbCmdRead = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbCmdWrite = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszCmdMain_Items(self.bszCmdMain)
        self._init_coll_szsbCmdSetup_Items(self.szsbCmdSetup)
        self._init_coll_szsbCmdRead_Items(self.szsbCmdRead)
        self._init_coll_szsbCmdWrite_Items(self.szsbCmdWrite)
        self._init_coll_szbCmdRead_Items(self.szbCmdRead)
        self._init_coll_szbCmdWrite_Items(self.szbCmdWrite)

        self.SetSizer(self.bszCmdMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELCOMMAND, name='panelCmd',
              parent=prnt, pos=wx.Point(876, 371), size=wx.Size(548, 453),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(532, 415))

        self.sbCmdRead = wx.StaticBox(id=wxID_PANELCOMMANDSBCMDREAD,
              label='Read', name='sbCmdRead', parent=self, pos=wx.Point(0, 85),
              size=wx.Size(532, 161), style=0)

        self.sbCmdWrite = wx.StaticBox(id=wxID_PANELCOMMANDSBCMDWRITE,
              label='Write', name='sbCmdWrite', parent=self, pos=wx.Point(0,
              246), size=wx.Size(532, 159), style=0)

        self.sbCmdSetup = wx.StaticBox(id=wxID_PANELCOMMANDSBCMDSETUP,
              label='General', name='sbCmdSetup', parent=self, pos=wx.Point(0,
              0), size=wx.Size(532, 85), style=0)

        self.tbCmdReadData = wx.TextCtrl(id=wxID_PANELCOMMANDTBCMDREADDATA,
              name='tbCmdReadData', parent=self, pos=wx.Point(9, 137),
              size=wx.Size(514, 69), style=wx.TE_READONLY | wx.TE_MULTILINE,
              value='')
        self.tbCmdReadData.SetToolTipString('')

        self.tbCmdWriteData = wx.TextCtrl(id=wxID_PANELCOMMANDTBCMDWRITEDATA,
              name='tbCmdWriteData', parent=self, pos=wx.Point(9, 296),
              size=wx.Size(514, 69), style=wx.TE_MULTILINE, value='')
        self.tbCmdWriteData.SetToolTipString('')

        self.cbWordWide = wx.CheckBox(id=wxID_PANELCOMMANDCBWORDWIDE,
              label='Word-wide mode (0=8-bit mode data bus, 1=16-bit data bus)',
              name='cbWordWide', parent=self, pos=wx.Point(9, 63),
              size=wx.Size(514, 13), style=0)
        self.cbWordWide.SetValue(False)
        self.cbWordWide.Bind(wx.EVT_CHECKBOX, self.OnCmdCheckbox,
              id=wxID_PANELCOMMANDCBWORDWIDE)

        self.staticText6 = wx.StaticText(id=wxID_PANELCOMMANDSTATICTEXT6,
              label='Address:', name='staticText6', parent=self, pos=wx.Point(9,
              270), size=wx.Size(47, 14), style=0)
        self.staticText6.SetToolTipString('')

        self.staticText4 = wx.StaticText(id=wxID_PANELCOMMANDSTATICTEXT4,
              label='Address:', name='staticText4', parent=self, pos=wx.Point(9,
              110), size=wx.Size(47, 14), style=0)
        self.staticText4.SetToolTipString('')

        self.staticText5 = wx.StaticText(id=wxID_PANELCOMMANDSTATICTEXT5,
              label='Bytes:', name='staticText5', parent=self, pos=wx.Point(136,
              109), size=wx.Size(32, 16), style=0)
        self.staticText5.SetToolTipString('')

        self.cbUseAddress = wx.CheckBox(id=wxID_PANELCOMMANDCBUSEADDRESS,
              label='Use GPIF[8..0] address bus', name='cbUseAddress',
              parent=self, pos=wx.Point(9, 21), size=wx.Size(514, 13), style=0)
        self.cbUseAddress.SetValue(True)
        self.cbUseAddress.Bind(wx.EVT_CHECKBOX, self.OnCmdCheckbox,
              id=wxID_PANELCOMMANDCBUSEADDRESS)

        self.bCmdWrite = wx.Button(id=wxID_PANELCOMMANDBCMDWRITE, label='Write',
              name='bCmdWrite', parent=self, pos=wx.Point(448, 373),
              size=wx.Size(75, 23), style=0)
        self.bCmdWrite.SetToolTipString('')
        self.bCmdWrite.Bind(wx.EVT_BUTTON, self.OnBCmdWriteButton,
              id=wxID_PANELCOMMANDBCMDWRITE)

        self.bCmdRead = wx.Button(id=wxID_PANELCOMMANDBCMDREAD, label='Read',
              name='bCmdRead', parent=self, pos=wx.Point(448, 214),
              size=wx.Size(75, 23), style=0)
        self.bCmdRead.SetToolTipString('')
        self.bCmdRead.Bind(wx.EVT_BUTTON, self.OnBCmdReadButton,
              id=wxID_PANELCOMMANDBCMDREAD)

        self.tbCmdBytes = wx.TextCtrl(id=wxID_PANELCOMMANDTBCMDBYTES,
              name='tbCmdBytes', parent=self, pos=wx.Point(176, 106),
              size=wx.Size(56, 23), style=0, value='')
        self.tbCmdBytes.SetToolTipString('')

        self.cbAutoIncAddress = wx.CheckBox(id=wxID_PANELCOMMANDCBAUTOINCADDRESS,
              label='Auto-increment address', name='cbAutoIncAddress',
              parent=self, pos=wx.Point(9, 42), size=wx.Size(514, 13), style=0)
        self.cbAutoIncAddress.SetValue(False)
        self.cbAutoIncAddress.Bind(wx.EVT_CHECKBOX, self.OnCmdCheckbox,
              id=wxID_PANELCOMMANDCBAUTOINCADDRESS)

        self.tbCmdReadAddress = wx.TextCtrl(id=wxID_PANELCOMMANDTBCMDREADADDRESS,
              name='tbCmdReadAddress', parent=self, pos=wx.Point(64, 106),
              size=wx.Size(56, 23), style=0, value='')

        self.tbCmdWriteAddress = wx.TextCtrl(id=wxID_PANELCOMMANDTBCMDWRITEADDRESS,
              name='tbCmdWriteAddress', parent=self, pos=wx.Point(64, 267),
              size=wx.Size(56, 21), style=0, value='')

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)



    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        self.cbAutoIncAddress.Enable(ena and self.cbUseAddress.GetValue())
        self.tbCmdReadAddress.Enable(ena and self.cbUseAddress.GetValue())
        self.tbCmdWriteAddress.Enable(ena and self.cbUseAddress.GetValue())
        
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our 
        controls
        """
        # Read Data Address Setting
        (ok, value) = qusb.ReadSetting(Setting.DataAddress)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update checkboxes
        self.cbUseAddress.SetValue((value & 0x4000) == 0)
        self.cbAutoIncAddress.SetValue((value & 0x8000) == 0)
        
        # Read Word Wide Setting
        (ok, value) = qusb.ReadSetting(Setting.WordWide)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update Checkbox
        self.cbWordWide.SetValue((value & 0x0001) != 0)
        
        # Update which controls are enabled
        self.UpdateCtrlsEnable(True)
        
        
        
    def PeriodicUpdateCtrls(self, qusb):
        """
        Called fy frameMain when we are to refresh our controls with values
        read off the QuickUSB Module
        """
        self.UpdateCtrls(qusb)
    
        
        
    def LoadConf(self, conf):
        """
        Called by frameMain when we are to load our configuration settings
        """
        # Address Textbox
        if 'CmdReadAddress' in conf:
            self.tbCmdReadAddress.SetValue(conf['CmdReadAddress'])
        else:
            self.tbCmdReadAddress.SetValue("0x01")
        if 'CmdWriteAddress' in conf:
            self.tbCmdWriteAddress.SetValue(conf['CmdWriteAddress'])
        else:
            self.tbCmdWriteAddress.SetValue("0x01")
            
        # Number of Bytes Textbox
        if 'CmdBytes' in conf:
            self.tbCmdBytes.SetValue(conf['CmdBytes'])
        else:
            self.tbCmdBytes.SetValue("2")
            
        # Data textbox
        if 'CmdWriteData' in conf:
            self.tbCmdWriteData.SetValue(conf['CmdWriteData'])
        else:
            self.tbCmdWriteData.SetValue("0x00 0x01 0x02")
    
    
            
    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        return {'CmdReadAddress':self.tbCmdReadAddress.GetValue(),
                'CmdWriteAddress':self.tbCmdWriteAddress.GetValue(),
                'CmdBytes':self.tbCmdBytes.GetValue(),
                'CmdWriteData':self.tbCmdWriteData.GetValue()}



    def OnCmdCheckbox(self, event):
        """
        Event handler for the all the checkboxes
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Update which controls are enabled
        self.UpdateCtrlsEnable(True)
        
        # Read current values
        (ok, valueDataAddress) = qusb.ReadSetting(Setting.DataAddress)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        (ok, valueWordWide) = qusb.ReadSetting(Setting.WordWide)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update values
        if self.cbUseAddress.GetValue():
            valueDataAddress &= ~0x4000
        else:
            valueDataAddress |= 0x4000
        if self.cbAutoIncAddress.GetValue():
            valueDataAddress &= ~0x8000
        else:
            valueDataAddress |= 0x8000
        if self.cbWordWide.GetValue():
            valueWordWide |= 0x0001
        else:
            valueWordWide &= ~0x0001
        
        # Write values
        (ok,) = qusb.WriteSetting(Setting.DataAddress, valueDataAddress)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        (ok,) = qusb.WriteSetting(Setting.WordWide, valueWordWide)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Wait a little before we perform our next read
        time.sleep(0.01)
        
        # Read back values to ensure our state reflects our status
        if self.TheFrame._performReadBack:
            self.UpdateCtrls(qusb)



    def OnBCmdWriteButton(self, event):
        """
        Event handler for the write button
        """
        # Ensure a valid module is selected

        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        buf = self.TheFrame.ParseDataBox(self.tbCmdWriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return

        try:
            address = self.TheFrame.GetInt(self.tbCmdWriteAddress.GetValue())
        except ValueError:
            wx.MessageBox("The write address must be a valid integer!", "Integer Format Error")
            return

        (ok,) = qusb.WriteCommand(address, buf, len(buf))
        if not ok:
            self.TheFrame.SetStatus("WriteCommand() failed with error: " + str(Error(qusb.LastError())))
            return
        
        self.TheFrame.SetStatus("Successfully wrote command data")



    def OnBCmdReadButton(self, event):
        """
        Event handler for the read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        try:
            bytes = self.TheFrame.GetInt(self.tbCmdBytes.GetValue())
        except ValueError:
            wx.MessageBox("The number of bytes to read in the read bytes text box must be a valid integer!", "Integer Format Error")
            return
        
        try:
            address = self.TheFrame.GetInt(self.tbCmdReadAddress.GetValue())
        except ValueError:
            wx.MessageBox("The read address must be a valid integer!", "Integer Format Error")
            return
        
        buf = CreateByteBuffer(bytes)
        (ok, bytes) = qusb.ReadCommand(address, buf, bytes)
        if not ok:
            self.TheFrame.SetStatus("ReadCommand() failed with error: " + str(Error(qusb.LastError())))
            self.tbCmdReadData.SetValue("")
            return

        self.TheFrame.PlaceDataBox(self.tbCmdReadData, buf, bytes)
        self.TheFrame.SetStatus("Successfully read command data")
