#Boa:FramePanel:panelI2C
"""
============================================================================
 Title       : panelI2C.py
 Description : QuickUSB Python API Interface
 Notes       : 
 History     :

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

============================================================================
"""

# wx imports
import wx

# Bitwise Systems imports
from QuickUsb import *

# Generated Boa Constructor wx IDs
[wxID_PANELI2C, wxID_PANELI2CBI2CCACHEDWRITE, wxID_PANELI2CBI2CREAD, 
 wxID_PANELI2CBI2CWRITE, wxID_PANELI2CCBI2CBUSSPEED, 
 wxID_PANELI2CCBI2CIGNOREACK, wxID_PANELI2CSBI2CREAD, wxID_PANELI2CSBI2CSETUP, 
 wxID_PANELI2CSBI2CWRITE, wxID_PANELI2CSTATICTEXT10, wxID_PANELI2CSTATICTEXT7, 
 wxID_PANELI2CSTATICTEXT8, wxID_PANELI2CSTATICTEXT9, wxID_PANELI2CTBI2CBYTES, 
 wxID_PANELI2CTBI2CREADADDRESS, wxID_PANELI2CTBI2CREADDATA, 
 wxID_PANELI2CTBI2CWRITEADDRESS, wxID_PANELI2CTBI2CWRITEDATA, 
] = [wx.NewId() for _init_ctrls in range(18)]

class panelI2C(wx.Panel):
    def _init_coll_szsbI2cRead_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbI2cRead, 0, border=0, flag=0)
        parent.AddWindow(self.tbI2cReadData, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.bI2cRead, 0, border=4,
              flag=wx.ALL | wx.ALIGN_RIGHT)

    def _init_coll_bszI2cMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbI2cSetup, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbI2cRead, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbI2cWrite, 0, border=0, flag=wx.GROW)

    def _init_coll_szbI2cWriteButtons_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.bI2cWrite, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.bI2cCachedWrite, 0, border=4, flag=wx.ALL)

    def _init_coll_szbI2cWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText10, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbI2cWriteAddress, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_CENTER_VERTICAL)

    def _init_coll_szsbI2cSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbI2cSetup, 0, border=0, flag=0)
        parent.AddWindow(self.cbI2cIgnoreAck, 0, border=4,
              flag=wx.GROW | wx.ALL)

    def _init_coll_szsbI2cWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbI2cWrite, 0, border=0, flag=0)
        parent.AddWindow(self.tbI2cWriteData, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddSizer(self.szbI2cWriteButtons, 0, border=0,
              flag=wx.ALIGN_RIGHT)

    def _init_coll_szbI2cSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText7, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbI2cBusSpeed, 0, border=4, flag=wx.GROW | wx.ALL)

    def _init_coll_szbI2cRead_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText8, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbI2cReadAddress, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.staticText9, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbI2cBytes, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszI2cMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbI2cSetup = wx.StaticBoxSizer(box=self.sbI2cSetup,
              orient=wx.VERTICAL)

        self.szsbI2cRead = wx.StaticBoxSizer(box=self.sbI2cRead,
              orient=wx.VERTICAL)

        self.szsbI2cWrite = wx.StaticBoxSizer(box=self.sbI2cWrite,
              orient=wx.VERTICAL)

        self.szbI2cSetup = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbI2cRead = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbI2cWriteButtons = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbI2cWrite = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszI2cMain_Items(self.bszI2cMain)
        self._init_coll_szsbI2cSetup_Items(self.szsbI2cSetup)
        self._init_coll_szsbI2cRead_Items(self.szsbI2cRead)
        self._init_coll_szsbI2cWrite_Items(self.szsbI2cWrite)
        self._init_coll_szbI2cSetup_Items(self.szbI2cSetup)
        self._init_coll_szbI2cRead_Items(self.szbI2cRead)
        self._init_coll_szbI2cWriteButtons_Items(self.szbI2cWriteButtons)
        self._init_coll_szbI2cWrite_Items(self.szbI2cWrite)

        self.SetSizer(self.bszI2cMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELI2C, name='panelI2C', parent=prnt,
              pos=wx.Point(692, 273), size=wx.Size(518, 431),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(502, 393))

        self.sbI2cRead = wx.StaticBox(id=wxID_PANELI2CSBI2CREAD, label='Read',
              name='sbI2cRead', parent=self, pos=wx.Point(0, 72),
              size=wx.Size(502, 161), style=0)
        self.sbI2cRead.SetToolTipString('')

        self.sbI2cSetup = wx.StaticBox(id=wxID_PANELI2CSBI2CSETUP,
              label='General', name='sbI2cSetup', parent=self, pos=wx.Point(0,
              0), size=wx.Size(502, 72), style=0)

        self.sbI2cWrite = wx.StaticBox(id=wxID_PANELI2CSBI2CWRITE,
              label='Write', name='sbI2cWrite', parent=self, pos=wx.Point(0,
              233), size=wx.Size(502, 161), style=0)
        self.sbI2cWrite.SetToolTipString('')

        self.tbI2cWriteData = wx.TextCtrl(id=wxID_PANELI2CTBI2CWRITEDATA,
              name='tbI2cWriteData', parent=self, pos=wx.Point(9, 285),
              size=wx.Size(484, 69), style=wx.TE_MULTILINE, value='')
        self.tbI2cWriteData.SetToolTipString('')

        self.staticText8 = wx.StaticText(id=wxID_PANELI2CSTATICTEXT8,
              label='Address:', name='staticText8', parent=self, pos=wx.Point(9,
              97), size=wx.Size(47, 14), style=0)
        self.staticText8.SetToolTipString('')

        self.staticText9 = wx.StaticText(id=wxID_PANELI2CSTATICTEXT9,
              label='Bytes:', name='staticText9', parent=self, pos=wx.Point(128,
              96), size=wx.Size(32, 16), style=0)
        self.staticText9.SetToolTipString('')

        self.staticText7 = wx.StaticText(id=wxID_PANELI2CSTATICTEXT7,
              label='Bus Speed:', name='staticText7', parent=self,
              pos=wx.Point(9, 25), size=wx.Size(54, 13), style=0)
        self.staticText7.SetToolTipString('')

        self.bI2cWrite = wx.Button(id=wxID_PANELI2CBI2CWRITE, label='Write',
              name='bI2cWrite', parent=self, pos=wx.Point(335, 362),
              size=wx.Size(75, 23), style=0)
        self.bI2cWrite.SetToolTipString('')
        self.bI2cWrite.Bind(wx.EVT_BUTTON, self.OnBI2cWriteButton,
              id=wxID_PANELI2CBI2CWRITE)

        self.tbI2cReadAddress = wx.TextCtrl(id=wxID_PANELI2CTBI2CREADADDRESS,
              name='tbI2cReadAddress', parent=self, pos=wx.Point(64, 93),
              size=wx.Size(48, 23), style=0, value='')
        self.tbI2cReadAddress.SetToolTipString('')

        self.bI2cRead = wx.Button(id=wxID_PANELI2CBI2CREAD, label='Read',
              name='bI2cRead', parent=self, pos=wx.Point(418, 201),
              size=wx.Size(75, 23), style=0)
        self.bI2cRead.SetToolTipString('')
        self.bI2cRead.Bind(wx.EVT_BUTTON, self.OnBI2cReadButton,
              id=wxID_PANELI2CBI2CREAD)

        self.staticText10 = wx.StaticText(id=wxID_PANELI2CSTATICTEXT10,
              label='Address:', name='staticText10', parent=self,
              pos=wx.Point(9, 258), size=wx.Size(47, 14), style=0)
        self.staticText10.SetToolTipString('')

        self.tbI2cWriteAddress = wx.TextCtrl(id=wxID_PANELI2CTBI2CWRITEADDRESS,
              name='tbI2cWriteAddress', parent=self, pos=wx.Point(64, 254),
              size=wx.Size(48, 23), style=0, value='')
        self.tbI2cWriteAddress.SetToolTipString('')

        self.tbI2cBytes = wx.TextCtrl(id=wxID_PANELI2CTBI2CBYTES,
              name='tbI2cBytes', parent=self, pos=wx.Point(168, 93),
              size=wx.Size(48, 23), style=0, value='')
        self.tbI2cBytes.SetToolTipString('')

        self.tbI2cReadData = wx.TextCtrl(id=wxID_PANELI2CTBI2CREADDATA,
              name='tbI2cReadData', parent=self, pos=wx.Point(9, 124),
              size=wx.Size(484, 69), style=wx.TE_READONLY | wx.TE_MULTILINE,
              value='')
        self.tbI2cReadData.SetToolTipString('')

        self.bI2cCachedWrite = wx.Button(id=wxID_PANELI2CBI2CCACHEDWRITE,
              label='Cached Write', name='bI2cCachedWrite', parent=self,
              pos=wx.Point(418, 362), size=wx.Size(75, 23), style=0)
        self.bI2cCachedWrite.SetToolTipString('')
        self.bI2cCachedWrite.Bind(wx.EVT_BUTTON, self.OnBI2cCachedWriteButton,
              id=wxID_PANELI2CBI2CCACHEDWRITE)

        self.cbI2cBusSpeed = wx.Choice(choices=['100 kHz', '400 kHz'],
              id=wxID_PANELI2CCBI2CBUSSPEED, name='cbI2cBusSpeed', parent=self,
              pos=wx.Point(71, 21), size=wx.Size(73, 21), style=0)
        self.cbI2cBusSpeed.SetLabel('')
        self.cbI2cBusSpeed.SetToolTipString('')
        self.cbI2cBusSpeed.Bind(wx.EVT_COMBOBOX, self.OnCbI2cBusSpeedCombobox,
              id=wxID_PANELI2CCBI2CBUSSPEED)

        self.cbI2cIgnoreAck = wx.CheckBox(id=wxID_PANELI2CCBI2CIGNOREACK,
              label='Ignore ACK (0=Handle ACK, 1=Process even if no ACK is supplied)',
              name='cbI2cIgnoreAck', parent=self, pos=wx.Point(9, 50),
              size=wx.Size(484, 13), style=0)
        self.cbI2cIgnoreAck.SetValue(False)
        self.cbI2cIgnoreAck.SetToolTipString('')
        self.cbI2cIgnoreAck.Bind(wx.EVT_CHECKBOX, self.OnCbI2cIgnoreAckCheckbox,
              id=wxID_PANELI2CCBI2CIGNOREACK)

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)



    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        # The QuickUsbCachedWrite() function only exists in v2.15.1+ of the 
        # QuickUSB Library, so disable the button if we can't call it
        self.bI2cCachedWrite.Enable(ena and (QuickUsbLibrary.QuickUsbCachedWriteI2C != None))
        
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our 
        controls
        """
        # Read the I2C setting register
        (ok, value) = qusb.ReadSetting(Setting.I2Ctl)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Set the bus speed and ignore ack checkboxes
        self.cbI2cBusSpeed.SetSelection(value & 0x0001)
        self.cbI2cIgnoreAck.SetValue((value & 0x0080) != 0)
        
        
        
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
        # I2C Address Textbox
        if 'I2cReadAddress' in conf:
            self.tbI2cReadAddress.SetValue(conf['I2cReadAddress'])
        else:
            self.tbI2cReadAddress.SetValue("0x01")
        if 'I2cWriteAddress' in conf:
            self.tbI2cWriteAddress.SetValue(conf['I2cWriteAddress'])
        else:
            self.tbI2cWriteAddress.SetValue("0x01")
            
        # Number of I2C Bytes Textbox
        if 'I2cBytes' in conf:
            self.tbI2cBytes.SetValue(conf['I2cBytes'])
        else:
            self.tbI2cBytes.SetValue("2")
        if 'I2cWriteData' in conf:
            self.tbI2cWriteData.SetValue(conf['I2cWriteData'])
        else:
            self.tbI2cWriteData.SetValue("0x00 0x01 0x02")
            
            
            
    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        return {'I2cReadAddress':self.tbI2cReadAddress.GetValue(),
                'I2cWriteAddress':self.tbI2cWriteAddress.GetValue(),
                'I2cBytes':self.tbI2cBytes.GetValue(),
                'I2cWriteData':self.tbI2cWriteData.GetValue()}



    def OnBI2cWriteButton(self, event):
        """
        Event handler for the I2C write button
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        buf = self.TheFrame.ParseDataBox(self.tbI2cWriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return
        
        try:
            address = self.TheFrame.GetInt(self.tbI2cWriteAddress.GetValue())
        except ValueError:
            wx.MessageBox("The write address must be a valid integer!", "Integer Format Error")
            return
        
        (ok,) = qusb.WriteI2C(address, buf, len(buf))
        if not ok:
            self.TheFrame.SetStatus("WriteI2C() failed with error: " + str(Error(qusb.LastError())))
            return

        self.TheFrame.SetStatus("Successfully wrote data to I2C")



    def OnBI2cReadButton(self, event):
        """
        Event handler for the I2C read button
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        try:
            bytes = self.TheFrame.GetInt(self.tbI2cBytes.GetValue())
        except ValueError:
            wx.MessageBox("The number of bytes to read in the read bytes text box must be a valid integer!", "Integer Format Error")
            return
        
        try:
            address = self.TheFrame.GetInt(self.tbI2cReadAddress.GetValue())
        except ValueError:
            wx.MessageBox("The read address must be a valid integer!", "Integer Format Error")
            return
        
        buf = CreateByteBuffer(bytes)
        (ok, bytes) = qusb.ReadI2C(address, buf, bytes)
        if not ok:
            self.TheFrame.SetStatus("ReadI2C() failed with error: " + str(Error(qusb.LastError())))
            self.tbI2cReadData.SetValue("")
            return

        self.TheFrame.PlaceDataBox(self.tbI2cReadData, buf, bytes)

        self.TheFrame.SetStatus("Successfully read data from I2C")
        


    def OnBI2cCachedWriteButton(self, event):
        """
        Event handler for the I2C cached write button
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        buf = self.TheFrame.ParseDataBox(self.tbI2cWriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return
        
        try:
            address = self.TheFrame.GetInt(self.tbI2cWriteAddress.GetValue())
        except ValueError:
            wx.MessageBox("The write address must be a valid integer!", "Integer Format Error")
            return
        
        (ok,) = qusb.CachedWriteI2C(address, buf, len(buf))
        if not ok:
            self.TheFrame.SetStatus("CachedWriteI2C() failed with error: " + str(Error(qusb.LastError())))
            return
        
        self.TheFrame.SetStatus("Successfully perform a I2C cached-write")



    def OnCbI2cBusSpeedCombobox(self, event):
        """
        Event handler for the I2C bus speed combobox
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Read setting
        (ok, value) = qusb.ReadSetting(Setting.I2Ctl)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Write over bit 0
        if self.cbI2cBusSpeed.GetSelection() == 0:
            value &= ~0x0001
        else:
            value |= 0x0001
            
        # Write setting
        (ok,) = qusb.WriteSetting(Setting.I2Ctl, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return



    def OnCbI2cIgnoreAckCheckbox(self, event):
        """
        Event handler for the I2C ignore ACK checkbox
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Read setting
        (ok, value) = qusb.ReadSetting(Setting.I2Ctl)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Write over bit 7
        if self.cbI2cIgnoreAck.GetValue():
            value |= 0x0080
        else:
            value &= ~0x0080
            
        # Write setting
        (ok,) = qusb.WriteSetting(Setting.I2Ctl, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
