#Boa:FramePanel:panelRS232
"""
============================================================================
 Title       : panelRS232.py
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

# Bitwise Systems imports
from QuickUsb import *

# Generated Boa Constructor wx IDs
[wxID_PANELRS232, wxID_PANELRS232BGETCHARS, wxID_PANELRS232BRS232FLUSH, 
 wxID_PANELRS232BRS232READ, wxID_PANELRS232BRS232WRITE, 
 wxID_PANELRS232CBRS232BAUDRATE, wxID_PANELRS232CBRS232PORT, 
 wxID_PANELRS232SBRS232READ, wxID_PANELRS232SBRS232SETUP, 
 wxID_PANELRS232SBRS232WRITE, wxID_PANELRS232STATICTEXT1, 
 wxID_PANELRS232STATICTEXT2, wxID_PANELRS232STATICTEXT3, 
 wxID_PANELRS232TBRS232CHARS, wxID_PANELRS232TBRS232READDATA, 
 wxID_PANELRS232TBRS232WRITEDATA, 
] = [wx.NewId() for _init_ctrls in range(16)]

class panelRS232(wx.Panel):
    def _init_coll_szsbRs232Read_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbRs232Read, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.tbRs232ReadData, 0, border=4,
              flag=wx.ALL | wx.GROW)
        parent.AddWindow(self.bRs232Read, 0, border=4,
              flag=wx.ALL | wx.ALIGN_RIGHT)

    def _init_coll_bszRs232Main_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbRs232Setup, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbRs232Read, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbRs232Write, 0, border=0, flag=wx.GROW)

    def _init_coll_szbRs232Read_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText3, 0, border=4,
              flag=wx.RIGHT | wx.LEFT | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tbRs232Chars, 0, border=4,
              flag=wx.RIGHT | wx.LEFT | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.bGetChars, 0, border=4, flag=wx.RIGHT | wx.LEFT)

    def _init_coll_szsbRs232Write_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.tbRs232WriteData, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.bRs232Write, 0, border=4,
              flag=wx.ALIGN_RIGHT | wx.ALL)

    def _init_coll_szsbRs232Setup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText1, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbRs232Port, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.staticText2, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbRs232BaudRate, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.bRs232Flush, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszRs232Main = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbRs232Setup = wx.StaticBoxSizer(box=self.sbRs232Setup,
              orient=wx.HORIZONTAL)

        self.szsbRs232Read = wx.StaticBoxSizer(box=self.sbRs232Read,
              orient=wx.VERTICAL)

        self.szsbRs232Write = wx.StaticBoxSizer(box=self.sbRs232Write,
              orient=wx.VERTICAL)

        self.szbRs232Read = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszRs232Main_Items(self.bszRs232Main)
        self._init_coll_szsbRs232Setup_Items(self.szsbRs232Setup)
        self._init_coll_szsbRs232Read_Items(self.szsbRs232Read)
        self._init_coll_szsbRs232Write_Items(self.szsbRs232Write)
        self._init_coll_szbRs232Read_Items(self.szbRs232Read)

        self.SetSizer(self.bszRs232Main)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELRS232, name='panelRS232',
              parent=prnt, pos=wx.Point(533, 295), size=wx.Size(501, 397),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(485, 359))

        self.sbRs232Setup = wx.StaticBox(id=wxID_PANELRS232SBRS232SETUP,
              label='General', name='sbRs232Setup', parent=self, pos=wx.Point(0,
              0), size=wx.Size(485, 53), style=0)

        self.sbRs232Read = wx.StaticBox(id=wxID_PANELRS232SBRS232READ,
              label='Read', name='sbRs232Read', parent=self, pos=wx.Point(0,
              53), size=wx.Size(485, 161), style=0)

        self.sbRs232Write = wx.StaticBox(id=wxID_PANELRS232SBRS232WRITE,
              label='Write', name='sbRs232Write', parent=self, pos=wx.Point(0,
              214), size=wx.Size(485, 130), style=0)

        self.bRs232Write = wx.Button(id=wxID_PANELRS232BRS232WRITE,
              label='Write', name='bRs232Write', parent=self, pos=wx.Point(401,
              312), size=wx.Size(75, 23), style=0)
        self.bRs232Write.SetToolTipString('')
        self.bRs232Write.Bind(wx.EVT_BUTTON, self.OnBRs232WriteButton,
              id=wxID_PANELRS232BRS232WRITE)

        self.tbRs232WriteData = wx.TextCtrl(id=wxID_PANELRS232TBRS232WRITEDATA,
              name='tbRs232WriteData', parent=self, pos=wx.Point(9, 235),
              size=wx.Size(467, 69), style=wx.TE_MULTILINE, value='')
        self.tbRs232WriteData.SetToolTipString('')

        self.staticText2 = wx.StaticText(id=wxID_PANELRS232STATICTEXT2,
              label='Baud Rate:', name='staticText2', parent=self,
              pos=wx.Point(128, 25), size=wx.Size(54, 14), style=0)

        self.staticText3 = wx.StaticText(id=wxID_PANELRS232STATICTEXT3,
              label='Chars in Read Buffer:', name='staticText3', parent=self,
              pos=wx.Point(13, 77), size=wx.Size(111, 16), style=0)

        self.staticText1 = wx.StaticText(id=wxID_PANELRS232STATICTEXT1,
              label='Port:', name='staticText1', parent=self, pos=wx.Point(9,
              25), size=wx.Size(23, 14), style=0)
        self.staticText1.SetToolTipString('')

        self.bRs232Flush = wx.Button(id=wxID_PANELRS232BRS232FLUSH,
              label='Flush Buffer', name='bRs232Flush', parent=self,
              pos=wx.Point(336, 21), size=wx.Size(75, 23), style=0)
        self.bRs232Flush.SetToolTipString('')
        self.bRs232Flush.Bind(wx.EVT_BUTTON, self.OnBRs232FlushButton,
              id=wxID_PANELRS232BRS232FLUSH)

        self.bRs232Read = wx.Button(id=wxID_PANELRS232BRS232READ, label='Read',
              name='bRs232Read', parent=self, pos=wx.Point(401, 182),
              size=wx.Size(75, 23), style=0)
        self.bRs232Read.SetToolTipString('')
        self.bRs232Read.Bind(wx.EVT_BUTTON, self.OnBRs232ReadButton,
              id=wxID_PANELRS232BRS232READ)

        self.cbRs232Port = wx.Choice(choices=['0', '1'],
              id=wxID_PANELRS232CBRS232PORT, name='cbRs232Port', parent=self,
              pos=wx.Point(40, 21), size=wx.Size(72, 21), style=0)
        self.cbRs232Port.SetLabel('')
        self.cbRs232Port.SetToolTipString('')

        self.cbRs232BaudRate = wx.Choice(choices=['4800', '9600', '19200',
              '38400', '57600', '115200', '230400'],
              id=wxID_PANELRS232CBRS232BAUDRATE, name='cbRs232BaudRate',
              parent=self, pos=wx.Point(190, 21), size=wx.Size(130, 21),
              style=0)
        self.cbRs232BaudRate.SetToolTipString('')

        self.tbRs232Chars = wx.TextCtrl(id=wxID_PANELRS232TBRS232CHARS,
              name='tbRs232Chars', parent=self, pos=wx.Point(132, 75),
              size=wx.Size(48, 21), style=wx.TE_READONLY, value='')
        self.tbRs232Chars.SetToolTipString('')

        self.tbRs232ReadData = wx.TextCtrl(id=wxID_PANELRS232TBRS232READDATA,
              name='tbRs232ReadData', parent=self, pos=wx.Point(9, 105),
              size=wx.Size(467, 69), style=wx.TE_READONLY | wx.TE_MULTILINE,
              value='')
        self.tbRs232ReadData.SetToolTipString('')

        self.bGetChars = wx.Button(id=wxID_PANELRS232BGETCHARS, label='Get Num',
              name='bGetChars', parent=self, pos=wx.Point(188, 74),
              size=wx.Size(56, 23), style=0)
        self.bGetChars.Bind(wx.EVT_BUTTON, self.OnBGetCharsButton,
              id=wxID_PANELRS232BGETCHARS)

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
        Called by frameMain when we need to update the values within our 
        controls
        """
        self.GetNum(qusb)
        
        
        
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
        if 'Rs232Port' in conf:
            self.cbRs232Port.SetSelection(int(conf['Rs232Port']))
        else:
            self.cbRs232Port.SetSelection(0)
        if 'Rs232BaudRate' in conf:
            self.cbRs232BaudRate.SetSelection(int(conf['Rs232BaudRate']))
        else:
            self.cbRs232BaudRate.SetSelection(6)
            
            
            
    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        return {'Rs232Port':self.cbRs232Port.GetSelection(),
                'Rs232BaudRate':self.cbRs232BaudRate.GetSelection()}



    def OnBRs232WriteButton(self, event):
        """
        Event handler for the RS232 write button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        buf = self.TheFrame.ParseDataBox(self.tbRs232WriteData)
        if not buf:
            wx.MessageBox("Unable to parse the data in the write text box!  Please ensure that the data is a series of space deliminated bytes in decimal or in hex, such as:\n\'0x01 2 8 &h12\'.", "Parsing Error")
            return
        
        (ok,) = qusb.WriteRS232(self.cbRs232Port.GetSelection(), buf, len(buf))
        if not ok:
            self.TheFrame.SetStatus("WriteRS232() failed with error: " + str(Error(qusb.LastError())))
            return
        
        self.TheFrame.SetStatus("Successfully wrote data to RS232")



    def OnBRs232FlushButton(self, event):
        """
        Event handler for the Rs232 flush buffer button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Flush the buffer
        (ok,) = qusb.FlushRS232(self.cbRs232Port.GetSelection())   
        if not ok:
            self.TheFrame.SetStatus("FlushRS232() failed with error: " + str(Error(qusb.LastError())))
            return

        self.TheFrame.SetStatus("Successfully flushed RS232 buffer")
        


    def OnBRs232ReadButton(self, event):
        """
        Event handler for the Rs232 read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Get the number of available characters to read
        (ok, num) = qusb.GetNumRS232(self.cbRs232Port.GetSelection())
        if not ok:
            self.TheFrame.SetStatus("GetNumRS232() failed with error: " + str(Error(qusb.LastError())))
            return
        buf = CreateByteBuffer(num)
        
        # Read the data
        (ok, bytes) = qusb.ReadRS232(self.cbRs232Port.GetSelection(), buf, num)
        if not ok:
            self.TheFrame.SetStatus("ReadRS232() failed with error: " + str(Error(qusb.LastError())))
            self.tbRs232ReadData.SetValue("")
            return
        
        # Update the data textbox
        self.TheFrame.PlaceDataBox(self.tbRs232ReadData, buf, bytes)
        
        # Update the num chars textbox
        (ok, num) = qusb.GetNumRS232(self.cbRs232Port.GetSelection())
        if not ok:
            self.TheFrame.SetStatus("GetNumRS232() failed with error: " + str(Error(qusb.LastError())))
            return
        
        self.tbRs232Chars.SetValue(str(num))
        self.TheFrame.SetStatus("Successfully read data from RS232")
        
        
        
    def OnBGetCharsButton(self, event):
        """
        Event handler for the get number of char button
        """
        qusb = self.TheFrame.GetSelectedModule()
        self.GetNum(qusb)
        
        self.TheFrame.SetStatus("Successfully read number of characters in RS232 buffer")
    
    
    
    def GetNum(self, qusb):
        """
        Helper function to retrieve the number of characters waiting in the
        RS232 data buffer
        """
        # Ensure a valid module is given
        if not qusb:
            return
        
        # Get the number of chars
        (ok, num) = qusb.GetNumRS232(self.cbRs232Port.GetSelection())
        if not ok:
            self.TheFrame.SetStatus("GetNumRS232() failed with error: " + str(Error(qusb.LastError())))
            return
    
        # Update the num chars textbox
        self.tbRs232Chars.SetValue(str(num))
