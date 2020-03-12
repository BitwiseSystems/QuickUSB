#Boa:FramePanel:panelSPI
"""
============================================================================
 Title       : panelSPI.py
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
[wxID_PANELSPI, wxID_PANELSPIBSPIREAD, wxID_PANELSPIBSPIWRITE, 
 wxID_PANELSPIBSPIWRITEREAD, wxID_PANELSPICBCLKPHASE, 
 wxID_PANELSPICBCLKPOLARITY, wxID_PANELSPICBENDIAN, wxID_PANELSPICBSPIREAD, 
 wxID_PANELSPICBSPIWRITE, wxID_PANELSPISBSPIREAD, wxID_PANELSPISBSPISETUP, 
 wxID_PANELSPISBSPIWRITE, wxID_PANELSPISTATICTEXT4, wxID_PANELSPISTATICTEXT5, 
 wxID_PANELSPISTATICTEXT6, wxID_PANELSPITBSPIBYTES, 
 wxID_PANELSPITBSPIREADDATA, wxID_PANELSPITBSPIWRITEDATA, 
] = [wx.NewId() for _init_ctrls in range(18)]

class panelSPI(wx.Panel):
    def _init_coll_szsbSpiSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbEndian, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.cbClkPolarity, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.cbClkPhase, 0, border=4, flag=wx.GROW | wx.ALL)

    def _init_coll_szbSpiRead_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText4, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbSpiRead, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.staticText5, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbSpiBytes, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_CENTER_VERTICAL)

    def _init_coll_szbSpiWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText6, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbSpiWrite, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL | wx.GROW)

    def _init_coll_szsbSpiRead_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbSpiRead, 0, border=0, flag=0)
        parent.AddWindow(self.tbSpiReadData, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.bSpiRead, 0, border=4,
              flag=wx.ALL | wx.ALIGN_RIGHT)

    def _init_coll_szbSpiWriteButtons_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.bSpiWrite, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.bSpiWriteRead, 0, border=4, flag=wx.ALL)

    def _init_coll_bszSpiMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbSpiSetup, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbSpiRead, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbSpiWrite, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbSpiWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbSpiWrite, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.tbSpiWriteData, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddSizer(self.szbSpiWriteButtons, 0, border=0,
              flag=wx.ALIGN_RIGHT)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszSpiMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbSpiSetup = wx.StaticBoxSizer(box=self.sbSpiSetup,
              orient=wx.VERTICAL)

        self.szsbSpiRead = wx.StaticBoxSizer(box=self.sbSpiRead,
              orient=wx.VERTICAL)

        self.szsbSpiWrite = wx.StaticBoxSizer(box=self.sbSpiWrite,
              orient=wx.VERTICAL)

        self.szbSpiRead = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbSpiWrite = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbSpiWriteButtons = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszSpiMain_Items(self.bszSpiMain)
        self._init_coll_szsbSpiSetup_Items(self.szsbSpiSetup)
        self._init_coll_szsbSpiRead_Items(self.szsbSpiRead)
        self._init_coll_szsbSpiWrite_Items(self.szsbSpiWrite)
        self._init_coll_szbSpiRead_Items(self.szbSpiRead)
        self._init_coll_szbSpiWrite_Items(self.szbSpiWrite)
        self._init_coll_szbSpiWriteButtons_Items(self.szbSpiWriteButtons)

        self.SetSizer(self.bszSpiMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELSPI, name='panelSPI', parent=prnt,
              pos=wx.Point(567, 364), size=wx.Size(497, 464),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(481, 426))

        self.sbSpiSetup = wx.StaticBox(id=wxID_PANELSPISBSPISETUP,
              label='General', name='sbSpiSetup', parent=self, pos=wx.Point(0,
              0), size=wx.Size(481, 85), style=0)
        self.sbSpiSetup.SetToolTipString('')

        self.sbSpiRead = wx.StaticBox(id=wxID_PANELSPISBSPIREAD, label='Read',
              name='sbSpiRead', parent=self, pos=wx.Point(0, 85),
              size=wx.Size(481, 161), style=0)
        self.sbSpiRead.SetToolTipString('')

        self.sbSpiWrite = wx.StaticBox(id=wxID_PANELSPISBSPIWRITE,
              label='Write', name='sbSpiWrite', parent=self, pos=wx.Point(0,
              246), size=wx.Size(481, 159), style=0)
        self.sbSpiWrite.SetToolTipString('')

        self.tbSpiReadData = wx.TextCtrl(id=wxID_PANELSPITBSPIREADDATA,
              name='tbSpiReadData', parent=self, pos=wx.Point(9, 137),
              size=wx.Size(463, 69), style=wx.TE_READONLY | wx.TE_MULTILINE,
              value='')
        self.tbSpiReadData.SetToolTipString('')

        self.cbSpiWrite = wx.Choice(choices=['0', '1', '2', '3', '4', '5', '6',
              '7', '8', '9'], id=wxID_PANELSPICBSPIWRITE, name='cbSpiWrite',
              parent=self, pos=wx.Point(40, 267), size=wx.Size(56, 21),
              style=0)
        self.cbSpiWrite.SetLabel('')
        self.cbSpiWrite.SetToolTipString('')

        self.tbSpiBytes = wx.TextCtrl(id=wxID_PANELSPITBSPIBYTES,
              name='tbSpiBytes', parent=self, pos=wx.Point(152, 106),
              size=wx.Size(49, 23), style=0, value='')
        self.tbSpiBytes.SetToolTipString('')

        self.tbSpiWriteData = wx.TextCtrl(id=wxID_PANELSPITBSPIWRITEDATA,
              name='tbSpiWriteData', parent=self, pos=wx.Point(9, 296),
              size=wx.Size(463, 69), style=wx.TE_MULTILINE, value='')
        self.tbSpiWriteData.SetToolTipString('')

        self.staticText6 = wx.StaticText(id=wxID_PANELSPISTATICTEXT6,
              label='Port:', name='staticText6', parent=self, pos=wx.Point(9,
              270), size=wx.Size(23, 14), style=0)
        self.staticText6.SetToolTipString('')

        self.staticText4 = wx.StaticText(id=wxID_PANELSPISTATICTEXT4,
              label='Port:', name='staticText4', parent=self, pos=wx.Point(9,
              110), size=wx.Size(23, 14), style=0)
        self.staticText4.SetToolTipString('')

        self.staticText5 = wx.StaticText(id=wxID_PANELSPISTATICTEXT5,
              label='Bytes:', name='staticText5', parent=self, pos=wx.Point(112,
              109), size=wx.Size(32, 16), style=0)
        self.staticText5.SetToolTipString('')

        self.bSpiWrite = wx.Button(id=wxID_PANELSPIBSPIWRITE, label='Write',
              name='bSpiWrite', parent=self, pos=wx.Point(314, 373),
              size=wx.Size(75, 23), style=0)
        self.bSpiWrite.SetToolTipString('')
        self.bSpiWrite.Bind(wx.EVT_BUTTON, self.OnBSpiWriteButton,
              id=wxID_PANELSPIBSPIWRITE)

        self.bSpiWriteRead = wx.Button(id=wxID_PANELSPIBSPIWRITEREAD,
              label='Write Read', name='bSpiWriteRead', parent=self,
              pos=wx.Point(397, 373), size=wx.Size(75, 23), style=0)
        self.bSpiWriteRead.SetToolTipString('')
        self.bSpiWriteRead.Bind(wx.EVT_BUTTON, self.OnBSpiWriteReadButton,
              id=wxID_PANELSPIBSPIWRITEREAD)

        self.bSpiRead = wx.Button(id=wxID_PANELSPIBSPIREAD, label='Read',
              name='bSpiRead', parent=self, pos=wx.Point(397, 214),
              size=wx.Size(75, 23), style=0)
        self.bSpiRead.SetToolTipString('')
        self.bSpiRead.Bind(wx.EVT_BUTTON, self.OnBSpiReadButton,
              id=wxID_PANELSPIBSPIREAD)

        self.cbEndian = wx.CheckBox(id=wxID_PANELSPICBENDIAN,
              label='Little/Big endian (0=LSBit to MSBit, 1=MSBit to LSBit)',
              name='cbEndian', parent=self, pos=wx.Point(9, 21),
              size=wx.Size(463, 13), style=0)
        self.cbEndian.SetValue(True)
        self.cbEndian.Bind(wx.EVT_CHECKBOX, self.OnSpiCheckbox,
              id=wxID_PANELSPICBENDIAN)

        self.cbClkPolarity = wx.CheckBox(id=wxID_PANELSPICBCLKPOLARITY,
              label='CPOL (0=Normal clock, 1=Inverted clock)',
              name='cbClkPolarity', parent=self, pos=wx.Point(9, 42),
              size=wx.Size(463, 13), style=0)
        self.cbClkPolarity.SetValue(True)
        self.cbClkPolarity.Bind(wx.EVT_CHECKBOX, self.OnSpiCheckbox,
              id=wxID_PANELSPICBCLKPOLARITY)

        self.cbClkPhase = wx.CheckBox(id=wxID_PANELSPICBCLKPHASE,
              label='CPHA (0=Sample then clock, 1=Clock then sample)',
              name='cbClkPhase', parent=self, pos=wx.Point(9, 63),
              size=wx.Size(463, 13), style=0)
        self.cbClkPhase.SetValue(True)
        self.cbClkPhase.Bind(wx.EVT_CHECKBOX, self.OnSpiCheckbox,
              id=wxID_PANELSPICBCLKPHASE)

        self.cbSpiRead = wx.Choice(choices=['0', '1', '2', '3', '4', '5', '6',
              '7', '8', '9'], id=wxID_PANELSPICBSPIREAD, name='cbSpiRead',
              parent=self, pos=wx.Point(40, 106), size=wx.Size(56, 21),
              style=0)
        self.cbSpiRead.SetLabel('')
        self.cbSpiRead.SetToolTipString('')

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)
        
        
        
    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled"""
        pass
        
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our controls
        """
        (ok, value) = qusb.ReadSetting(Setting.SpiConfig)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        self.cbEndian.SetValue((value & 0x0001) != 0)
        self.cbClkPolarity.SetValue((value & 0x0002) != 0)
        self.cbClkPhase.SetValue((value & 0x0004) != 0)
        
        
        
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
        # SPI R/W Checkbox
        if 'SpiWritePort' in conf:
            self.cbSpiWrite.SetSelection(int(conf['SpiWritePort']))
        else:
            self.cbSpiWrite.SetSelection(0)
        if 'SpiReadPort' in conf:
            self.cbSpiRead.SetSelection(int(conf['SpiReadPort']))
        else:
            self.cbSpiRead.SetSelection(0)
            
        # SPI Number of Bytes Textbox
        if 'SpiBytes' in conf:
            self.tbSpiBytes.SetValue(conf['SpiBytes'])
        else:
            self.tbSpiBytes.SetValue("1")
        
        # SPI Write Data Textbox
        if 'SpiWriteData' in conf:
            self.tbSpiWriteData.SetValue(conf['SpiWriteData'])
        else:
            self.tbSpiWriteData.SetValue("0x00 0x01 0x02")
            
            
            
    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        return {'SpiWritePort':self.cbSpiWrite.GetSelection(),
                'SpiReadPort':self.cbSpiRead.GetSelection(),
                'SpiBytes':self.tbSpiBytes.GetValue(),
                'SpiWriteData':self.tbSpiWriteData.GetValue()}



    def OnBSpiReadButton(self, event):
        """
        Event handler for the SPI read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Parse the number of bytes to read
        try:
            bytes = self.TheFrame.GetInt(self.tbSpiBytes.GetValue())
        except ValueError:
            wx.MessageBox("The number of bytes to read in the read bytes text box must be a valid integer!", "Integer Format Error")
            return
        
        # Perform the SPI read
        buf = CreateByteBuffer(bytes)
        (ok, bytes) = qusb.ReadSpi(self.cbSpiRead.GetSelection(), buf, bytes)
        if not ok:
            self.TheFrame.SetStatus("ReadSpi() failed with error: " + str(Error(qusb.LastError())))
            self.tbSpiReadData.SetValue("")
            return
        
        # Update the read data textbox
        self.TheFrame.PlaceDataBox(self.tbSpiReadData, buf, bytes)
        
        self.TheFrame.SetStatus("Successfully read data from SPI")
        
        
        
    def OnBSpiWriteButton(self, event):
        """
        Event handler for the SPI write button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Parse the data to write
        buf = self.TheFrame.ParseDataBox(self.tbSpiWriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return
        
        # Perfrom the SPI write
        (ok,) = qusb.WriteSpi(self.cbSpiWrite.GetSelection(), buf, len(buf))
        if not ok:
            self.TheFrame.SetStatus("WriteSpi() failed with error: " + str(Error(qusb.LastError())))
            return

        self.TheFrame.SetStatus("Successfully wrote data to SPI")
        


    def OnBSpiWriteReadButton(self, event):
        """
        Event handler for the SPI write-read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Parse the data to write
        buf = self.TheFrame.ParseDataBox(self.tbSpiWriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return
        
        # Parse the number of bytes to write/read
        try:
            bytes = self.TheFrame.GetInt(self.tbSpiBytes.GetValue())
        except ValueError:
            wx.MessageBox("The number of bytes to read in the read bytes text box must be a valid integer!", "Integer Format Error")
            return

        # Ensure that the number of read bytes and write bytes match up
        writeBytes = len(buf)
        if bytes != writeBytes:
            wx.MessageBox("The number of bytes to read indicated in the read bytes text box and the number of bytes to write in the write text box must be the same!", "Read Size and Write Size Mismatch")
            return
        
        # Perform the Write-Read
        (ok,) = qusb.WriteReadSpi(self.cbSpiWrite.GetSelection(), buf, bytes)
        if not ok:
            self.TheFrame.SetStatus("WriteReadSpi() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update the read data textbox with the data read back
        self.TheFrame.PlaceDataBox(self.tbSpiReadData, buf, bytes)
        self.TheFrame.SetStatus("Successfully issued SPI Write/Read command")



    def OnSpiCheckbox(self, event):
        """
        Event handler for the SPI checkboxes
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        #Read setting
        (ok, value) = qusb.ReadSetting(Setting.SpiConfig)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        #Write over bits 2:0
        value = (value & ~0x0007)
        if self.cbEndian.GetValue():
            value |= 0x0001
        if self.cbClkPolarity.GetValue():
            value |= 0x0002
        if self.cbClkPhase.GetValue():
            value |= 0x0004
            
        #Write setting
        (ok,) = qusb.WriteSetting(Setting.SpiConfig, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
