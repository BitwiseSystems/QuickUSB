#Boa:FramePanel:panelData
"""
============================================================================
 Title       : panelData.py
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
import os

# Bitwise Systems imports
from QuickUsb import *

# Generated Boa Constructor wx IDs
[wxID_PANELDATA, wxID_PANELDATABDATAREAD, wxID_PANELDATABDATAREADBROWSE, 
 wxID_PANELDATABDATATIMEOUTSET, wxID_PANELDATABDATAWRITE, 
 wxID_PANELDATABDATAWRITEBROWSE, wxID_PANELDATACBDATAREADAUTOINCADR, 
 wxID_PANELDATACBDATAREADUSEASYNC, wxID_PANELDATACBDATAREADUSEGPIFADR, 
 wxID_PANELDATACBDATAWORDWIDE, wxID_PANELDATACBDATAWRITEAUTOINCADR, 
 wxID_PANELDATACBDATAWRITELENGTH, wxID_PANELDATACBDATAWRITEUSEASYNC, 
 wxID_PANELDATACBDATAWRITEUSEGPIFADR, wxID_PANELDATASBDATAREAD, 
 wxID_PANELDATASBDATASETUP, wxID_PANELDATASBDATAWRITE, 
 wxID_PANELDATASTDATAREADADDRESS, wxID_PANELDATASTDATAREADBUFFERSIZE, 
 wxID_PANELDATASTDATAREADFILENAME, wxID_PANELDATASTDATAREADLENGTH, 
 wxID_PANELDATASTDATAREADNUMBUFFERS, wxID_PANELDATASTDATATIMEOUT, 
 wxID_PANELDATASTDATAWRITEBUFFERSIZE, wxID_PANELDATASTDATAWRITEFILENAME, 
 wxID_PANELDATASTDATAWRITELENGTH, wxID_PANELDATASTDATAWRITENUMBUFFERS, 
 wxID_PANELDATASTDATAWRITESTARTADDRESS, wxID_PANELDATATBDATAREADBUFFERSIZE, 
 wxID_PANELDATATBDATAREADFILENAME, wxID_PANELDATATBDATAREADLENGTH, 
 wxID_PANELDATATBDATAREADNUMBUFFERS, wxID_PANELDATATBDATAREADSTARTADDRESS, 
 wxID_PANELDATATBDATATIMEOUT, wxID_PANELDATATBDATAWRITEBUFFERSIZE, 
 wxID_PANELDATATBDATAWRITEFILENAME, wxID_PANELDATATBDATAWRITELENGTH, 
 wxID_PANELDATATBDATAWRITENUMBUFFERS, wxID_PANELDATATBDATAWRITESTARTADDRESS, 
] = [wx.NewId() for _init_ctrls in range(39)]

class panelData(wx.Panel):
    def _init_coll_szbDataWriteNumBuffers_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataWriteNumBuffers, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataWriteNumBuffers, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szfgDataWriteLengthFile_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataWriteLength, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSizer(self.szbReadLength, 0, border=0,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.bDataWrite, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.stDataWriteFileName, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataWriteFileName, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.bDataWriteBrowse, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szfgDataReadLengthFile_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(1)

    def _init_coll_szbDataWriteLeft_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbDataWriteAutoIncAdr, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSizer(self.szbDataWriteAddress, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szbDataSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataTimeout, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataTimeout, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.GROW | wx.ALL)
        parent.AddWindow(self.bDataTimeoutSet, 0, border=4,
              flag=wx.GROW | wx.ALL)

    def _init_coll_szbDataReadNumBuffers_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataReadBufferSize, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataReadBufferSize, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szbDataReadAddress_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataReadAddress, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tbDataReadStartAddress, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szbReadLength_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.tbDataWriteLength, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbDataWriteLength, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szbDataReadLeft_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbDataReadAutoIncAdr, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddSizer(self.szbDataReadAddress, 0, border=0, flag=wx.GROW)

    def _init_coll_szbDataWriteRight_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbDataWriteBufferSize, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szbDataWriteNumBuffers, 0, border=0,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL)

    def _init_coll_szfgDataWriteMain_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(0)
        parent.AddGrowableCol(1)

    def _init_coll_szsbDataSetup_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbDataWordwide, 0, border=4,
              flag=wx.GROW | wx.ALL)
        parent.AddSizer(self.szbDataSetup, 0, border=0, flag=wx.GROW)

    def _init_coll_szfgDataReadMain_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbDataReadUseGpifAdr, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.cbDataReadUseAsync, 0, border=4, flag=wx.ALL)
        parent.AddSizer(self.szbDataReadLeft, 0, border=20,
              flag=wx.LEFT | wx.GROW)
        parent.AddSizer(self.szbDataReadRight, 0, border=20,
              flag=wx.LEFT | wx.GROW)

    def _init_coll_szbDataWriteAddress_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataWriteStartAddress, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataWriteStartAddress, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szsbDataWrite_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szfgDataWriteLengthFile, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szfgDataWriteMain, 0, border=0, flag=wx.GROW)

    def _init_coll_szbDataReadRight_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbDataReadBufferSize, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szbDataReadNumBuffers, 0, border=0,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL)

    def _init_coll_szsbDataRead_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szfgDataReadLengthFile, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szfgDataReadMain, 0, border=0, flag=wx.GROW)

    def _init_coll_szfgDataReadLengthFile_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataReadLength, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataReadLength, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.bDataRead, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddWindow(self.stDataReadFileName, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tbDataReadFileName, 0, border=4,
              flag=wx.ALIGN_CENTER_HORIZONTAL | wx.GROW | wx.ALL)
        parent.AddWindow(self.bDataReadBrowse, 0, border=4,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szfgDataReadMain_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(0)
        parent.AddGrowableCol(1)

    def _init_coll_szfgDataWriteLengthFile_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(1)

    def _init_coll_szbDataWriteBufferSize_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataWriteBufferSize, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.tbDataWriteBufferSize, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)

    def _init_coll_szfgDataWriteMain_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.cbDataWriteUseGpifAdr, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddWindow(self.cbDataWriteUseAsync, 0, border=4,
              flag=wx.ALIGN_CENTER_VERTICAL | wx.ALL)
        parent.AddSizer(self.szbDataWriteLeft, 0, border=20,
              flag=wx.LEFT | wx.GROW)
        parent.AddSizer(self.szbDataWriteRight, 0, border=20,
              flag=wx.LEFT | wx.GROW)

    def _init_coll_bszDataMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbDataSetup, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbDataRead, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbDataWrite, 0, border=0, flag=wx.GROW)

    def _init_coll_szbDataReadBufferSize_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stDataReadNumBuffers, 0, border=4,
              flag=wx.ALL | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.tbDataReadNumBuffers, 0, border=4,
              flag=wx.GROW | wx.ALL | wx.ALIGN_CENTER_VERTICAL)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszDataMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbDataSetup = wx.StaticBoxSizer(box=self.sbDataSetup,
              orient=wx.VERTICAL)

        self.szsbDataRead = wx.StaticBoxSizer(box=self.sbDataRead,
              orient=wx.VERTICAL)

        self.szsbDataWrite = wx.StaticBoxSizer(box=self.sbDataWrite,
              orient=wx.VERTICAL)

        self.szbDataSetup = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szfgDataReadLengthFile = wx.FlexGridSizer(cols=3, hgap=4, rows=2,
              vgap=0)

        self.szbDataReadAddress = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbDataReadLeft = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbDataReadRight = wx.BoxSizer(orient=wx.VERTICAL)

        self.szfgDataReadMain = wx.FlexGridSizer(cols=2, hgap=0, rows=2, vgap=0)
        self.szfgDataReadMain.SetMinSize(wx.Size(-1, -1))

        self.szbDataReadNumBuffers = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbDataReadBufferSize = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szfgDataWriteLengthFile = wx.FlexGridSizer(cols=3, hgap=4, rows=2,
              vgap=0)

        self.szbDataWriteAddress = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbDataWriteLeft = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbDataWriteRight = wx.BoxSizer(orient=wx.VERTICAL)

        self.szfgDataWriteMain = wx.FlexGridSizer(cols=2, hgap=0, rows=2,
              vgap=0)
        self.szfgDataWriteMain.SetMinSize(wx.Size(50, 79))

        self.szbDataWriteNumBuffers = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbDataWriteBufferSize = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbReadLength = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszDataMain_Items(self.bszDataMain)
        self._init_coll_szsbDataSetup_Items(self.szsbDataSetup)
        self._init_coll_szsbDataRead_Items(self.szsbDataRead)
        self._init_coll_szsbDataWrite_Items(self.szsbDataWrite)
        self._init_coll_szbDataSetup_Items(self.szbDataSetup)
        self._init_coll_szfgDataReadLengthFile_Items(self.szfgDataReadLengthFile)
        self._init_coll_szfgDataReadLengthFile_Growables(self.szfgDataReadLengthFile)
        self._init_coll_szbDataReadAddress_Items(self.szbDataReadAddress)
        self._init_coll_szbDataReadLeft_Items(self.szbDataReadLeft)
        self._init_coll_szbDataReadRight_Items(self.szbDataReadRight)
        self._init_coll_szfgDataReadMain_Items(self.szfgDataReadMain)
        self._init_coll_szfgDataReadMain_Growables(self.szfgDataReadMain)
        self._init_coll_szbDataReadNumBuffers_Items(self.szbDataReadNumBuffers)
        self._init_coll_szbDataReadBufferSize_Items(self.szbDataReadBufferSize)
        self._init_coll_szfgDataWriteLengthFile_Growables(self.szfgDataWriteLengthFile)
        self._init_coll_szfgDataWriteLengthFile_Items(self.szfgDataWriteLengthFile)
        self._init_coll_szbDataWriteAddress_Items(self.szbDataWriteAddress)
        self._init_coll_szbDataWriteLeft_Items(self.szbDataWriteLeft)
        self._init_coll_szbDataWriteRight_Items(self.szbDataWriteRight)
        self._init_coll_szfgDataWriteMain_Items(self.szfgDataWriteMain)
        self._init_coll_szfgDataWriteMain_Growables(self.szfgDataWriteMain)
        self._init_coll_szbDataWriteNumBuffers_Items(self.szbDataWriteNumBuffers)
        self._init_coll_szbDataWriteBufferSize_Items(self.szbDataWriteBufferSize)
        self._init_coll_szbReadLength_Items(self.szbReadLength)

        self.SetSizer(self.bszDataMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELDATA, name='panelData',
              parent=prnt, pos=wx.Point(746, 262), size=wx.Size(583, 473),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(567, 435))

        self.sbDataSetup = wx.StaticBox(id=wxID_PANELDATASBDATASETUP,
              label='General', name='sbDataSetup', parent=self, pos=wx.Point(0,
              0), size=wx.Size(567, 74), style=0)

        self.sbDataRead = wx.StaticBox(id=wxID_PANELDATASBDATAREAD,
              label='Read', name='sbDataRead', parent=self, pos=wx.Point(0, 74),
              size=wx.Size(567, 163), style=0)

        self.sbDataWrite = wx.StaticBox(id=wxID_PANELDATASBDATAWRITE,
              label='Write', name='sbDataWrite', parent=self, pos=wx.Point(0,
              237), size=wx.Size(567, 163), style=0)

        self.bDataReadBrowse = wx.Button(id=wxID_PANELDATABDATAREADBROWSE,
              label='Browse...', name='bDataReadBrowse', parent=self,
              pos=wx.Point(483, 126), size=wx.Size(75, 23), style=0)
        self.bDataReadBrowse.SetMinSize(wx.Size(75, 23))
        self.bDataReadBrowse.Bind(wx.EVT_BUTTON, self.OnBDataReadBrowseButton,
              id=wxID_PANELDATABDATAREADBROWSE)

        self.stDataReadLength = wx.StaticText(id=wxID_PANELDATASTDATAREADLENGTH,
              label='Data Length (Bytes):', name='stDataReadLength',
              parent=self, pos=wx.Point(9, 100), size=wx.Size(101, 13),
              style=0)
        self.stDataReadLength.SetToolTipString('')

        self.cbDataWriteUseAsync = wx.CheckBox(id=wxID_PANELDATACBDATAWRITEUSEASYNC,
              label='Use asynchronous transfers', name='cbDataWriteUseAsync',
              parent=self, pos=wx.Point(293, 320), size=wx.Size(256, 13),
              style=0)
        self.cbDataWriteUseAsync.SetValue(False)
        self.cbDataWriteUseAsync.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAWRITEUSEASYNC)

        self.tbDataReadStartAddress = wx.TextCtrl(id=wxID_PANELDATATBDATAREADSTARTADDRESS,
              name='tbDataReadStartAddress', parent=self, pos=wx.Point(112,
              199), size=wx.Size(48, 21), style=0, value='')

        self.stDataReadNumBuffers = wx.StaticText(id=wxID_PANELDATASTDATAREADNUMBUFFERS,
              label='Number of Buffers:', name='stDataReadNumBuffers',
              parent=self, pos=wx.Point(322, 182), size=wx.Size(92, 13),
              style=0)

        self.tbDataWriteFileName = wx.TextCtrl(id=wxID_PANELDATATBDATAWRITEFILENAME,
              name='tbDataWriteFileName', parent=self, pos=wx.Point(122, 289),
              size=wx.Size(349, 23), style=0, value='')
        self.tbDataWriteFileName.SetMinSize(wx.Size(50, 23))
        self.tbDataWriteFileName.Bind(wx.EVT_TEXT, self.OnBrowseText,
              id=wxID_PANELDATATBDATAWRITEFILENAME)

        self.stDataWriteFileName = wx.StaticText(id=wxID_PANELDATASTDATAWRITEFILENAME,
              label='Input File Name:', name='stDataWriteFileName',
              parent=self, pos=wx.Point(9, 294), size=wx.Size(87, 13), style=0)

        self.cbDataWriteAutoIncAdr = wx.CheckBox(id=wxID_PANELDATACBDATAWRITEAUTOINCADR,
              label='Auto-increment address', name='cbDataWriteAutoIncAdr',
              parent=self, pos=wx.Point(29, 341), size=wx.Size(248, 13),
              style=0)
        self.cbDataWriteAutoIncAdr.SetValue(True)
        self.cbDataWriteAutoIncAdr.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAWRITEAUTOINCADR)

        self.stDataWriteNumBuffers = wx.StaticText(id=wxID_PANELDATASTDATAWRITENUMBUFFERS,
              label='Number of Buffers:', name='stDataWriteNumBuffers',
              parent=self, pos=wx.Point(313, 374), size=wx.Size(95, 13),
              style=0)

        self.tbDataReadFileName = wx.TextCtrl(id=wxID_PANELDATATBDATAREADFILENAME,
              name='tbDataReadFileName', parent=self, pos=wx.Point(122, 126),
              size=wx.Size(349, 23), style=0, value='')
        self.tbDataReadFileName.SetMinSize(wx.Size(50, 23))
        self.tbDataReadFileName.Bind(wx.EVT_TEXT, self.OnBrowseText,
              id=wxID_PANELDATATBDATAREADFILENAME)

        self.bDataWrite = wx.Button(id=wxID_PANELDATABDATAWRITE, label='Write',
              name='bDataWrite', parent=self, pos=wx.Point(483, 258),
              size=wx.Size(75, 23), style=0)
        self.bDataWrite.Bind(wx.EVT_BUTTON, self.OnBDataWriteButton,
              id=wxID_PANELDATABDATAWRITE)

        self.tbDataReadBufferSize = wx.TextCtrl(id=wxID_PANELDATATBDATAREADBUFFERSIZE,
              name='tbDataReadBufferSize', parent=self, pos=wx.Point(425, 207),
              size=wx.Size(48, 21), style=0, value='')

        self.tbDataWriteLength = wx.TextCtrl(id=wxID_PANELDATATBDATAWRITELENGTH,
              name='tbDataWriteLength', parent=self, pos=wx.Point(122, 259),
              size=wx.Size(74, 21), style=0, value='')
        self.tbDataWriteLength.SetToolTipString('')

        self.cbDataWriteUseGpifAdr = wx.CheckBox(id=wxID_PANELDATACBDATAWRITEUSEGPIFADR,
              label='Use GPIF[8..0] address bus', name='cbDataWriteUseGpifAdr',
              parent=self, pos=wx.Point(9, 320), size=wx.Size(256, 13),
              style=0)
        self.cbDataWriteUseGpifAdr.SetValue(False)
        self.cbDataWriteUseGpifAdr.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAWRITEUSEGPIFADR)

        self.bDataWriteBrowse = wx.Button(id=wxID_PANELDATABDATAWRITEBROWSE,
              label='Browse...', name='bDataWriteBrowse', parent=self,
              pos=wx.Point(483, 289), size=wx.Size(75, 23), style=0)
        self.bDataWriteBrowse.Bind(wx.EVT_BUTTON, self.OnBDataWriteBrowseButton,
              id=wxID_PANELDATABDATAWRITEBROWSE)

        self.tbDataWriteNumBuffers = wx.TextCtrl(id=wxID_PANELDATATBDATAWRITENUMBUFFERS,
              name='tbDataWriteNumBuffers', parent=self, pos=wx.Point(416, 370),
              size=wx.Size(48, 21), style=0, value='')

        self.stDataWriteStartAddress = wx.StaticText(id=wxID_PANELDATASTDATAWRITESTARTADDRESS,
              label='Start Address:', name='stDataWriteStartAddress',
              parent=self, pos=wx.Point(33, 370), size=wx.Size(70, 13),
              style=0)

        self.cbDataWordwide = wx.CheckBox(id=wxID_PANELDATACBDATAWORDWIDE,
              label='Word-wide mode (0=8-bit data bus, 1=16-bit data bus)',
              name='cbDataWordwide', parent=self, pos=wx.Point(9, 21),
              size=wx.Size(549, 13), style=0)
        self.cbDataWordwide.SetValue(False)
        self.cbDataWordwide.SetToolTipString('')
        self.cbDataWordwide.Bind(wx.EVT_CHECKBOX, self.OnCbWordwideCheckbox,
              id=wxID_PANELDATACBDATAWORDWIDE)

        self.stDataWriteBufferSize = wx.StaticText(id=wxID_PANELDATASTDATAWRITEBUFFERSIZE,
              label='Buffer Size (Bytes):', name='stDataWriteBufferSize',
              parent=self, pos=wx.Point(313, 345), size=wx.Size(94, 13),
              style=0)

        self.bDataTimeoutSet = wx.Button(id=wxID_PANELDATABDATATIMEOUTSET,
              label='Set', name='bDataTimeoutSet', parent=self,
              pos=wx.Point(200, 42), size=wx.Size(40, 23), style=0)
        self.bDataTimeoutSet.SetToolTipString('')
        self.bDataTimeoutSet.Bind(wx.EVT_BUTTON, self.OnBDataTimeoutSetButton,
              id=wxID_PANELDATABDATATIMEOUTSET)

        self.bDataRead = wx.Button(id=wxID_PANELDATABDATAREAD, label='Read',
              name='bDataRead', parent=self, pos=wx.Point(483, 95),
              size=wx.Size(75, 23), style=0)
        self.bDataRead.Bind(wx.EVT_BUTTON, self.OnBDataReadButton,
              id=wxID_PANELDATABDATAREAD)

        self.tbDataWriteStartAddress = wx.TextCtrl(id=wxID_PANELDATATBDATAWRITESTARTADDRESS,
              name='tbDataWriteStartAddress', parent=self, pos=wx.Point(111,
              366), size=wx.Size(48, 21), style=0, value='')

        self.stDataWriteLength = wx.StaticText(id=wxID_PANELDATASTDATAWRITELENGTH,
              label='Data Length (Bytes):', name='stDataWriteLength',
              parent=self, pos=wx.Point(9, 263), size=wx.Size(101, 13),
              style=0)
        self.stDataWriteLength.SetToolTipString('')

        self.tbDataWriteBufferSize = wx.TextCtrl(id=wxID_PANELDATATBDATAWRITEBUFFERSIZE,
              name='tbDataWriteBufferSize', parent=self, pos=wx.Point(415, 341),
              size=wx.Size(48, 21), style=0, value='')

        self.cbDataReadAutoIncAdr = wx.CheckBox(id=wxID_PANELDATACBDATAREADAUTOINCADR,
              label='Auto-increment address', name='cbDataReadAutoIncAdr',
              parent=self, pos=wx.Point(29, 178), size=wx.Size(265, 13),
              style=0)
        self.cbDataReadAutoIncAdr.SetValue(True)
        self.cbDataReadAutoIncAdr.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAREADAUTOINCADR)

        self.tbDataReadLength = wx.TextCtrl(id=wxID_PANELDATATBDATAREADLENGTH,
              name='tbDataReadLength', parent=self, pos=wx.Point(122, 95),
              size=wx.Size(74, 21), style=0, value='')
        self.tbDataReadLength.SetToolTipString('')

        self.cbDataReadUseAsync = wx.CheckBox(id=wxID_PANELDATACBDATAREADUSEASYNC,
              label='Use asynchronous transfers', name='cbDataReadUseAsync',
              parent=self, pos=wx.Point(302, 157), size=wx.Size(256, 13),
              style=0)
        self.cbDataReadUseAsync.SetValue(False)
        self.cbDataReadUseAsync.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAREADUSEASYNC)

        self.cbDataReadUseGpifAdr = wx.CheckBox(id=wxID_PANELDATACBDATAREADUSEGPIFADR,
              label='Use GPIF[8..0] address bus', name='cbDataReadUseGpifAdr',
              parent=self, pos=wx.Point(9, 157), size=wx.Size(256, 13),
              style=0)
        self.cbDataReadUseGpifAdr.SetValue(False)
        self.cbDataReadUseGpifAdr.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAREADUSEGPIFADR)

        self.stDataReadAddress = wx.StaticText(id=wxID_PANELDATASTDATAREADADDRESS,
              label='Start Address:', name='stDataReadAddress', parent=self,
              pos=wx.Point(29, 203), size=wx.Size(75, 13), style=0)

        self.stDataTimeout = wx.StaticText(id=wxID_PANELDATASTDATATIMEOUT,
              label='Timeout (Milliseconds):', name='stDataTimeout',
              parent=self, pos=wx.Point(9, 47), size=wx.Size(111, 13), style=0)
        self.stDataTimeout.SetToolTipString('')

        self.tbDataTimeout = wx.TextCtrl(id=wxID_PANELDATATBDATATIMEOUT,
              name='tbDataTimeout', parent=self, pos=wx.Point(128, 42),
              size=wx.Size(64, 23), style=0, value='1000')
        self.tbDataTimeout.SetToolTipString('')

        self.tbDataReadNumBuffers = wx.TextCtrl(id=wxID_PANELDATATBDATAREADNUMBUFFERS,
              name='tbDataReadNumBuffers', parent=self, pos=wx.Point(422, 178),
              size=wx.Size(48, 21), style=0, value='')

        self.stDataReadFileName = wx.StaticText(id=wxID_PANELDATASTDATAREADFILENAME,
              label='Output File Name:', name='stDataReadFileName', parent=self,
              pos=wx.Point(9, 131), size=wx.Size(87, 13), style=0)

        self.stDataReadBufferSize = wx.StaticText(id=wxID_PANELDATASTDATAREADBUFFERSIZE,
              label='Buffer Size (Bytes):', name='stDataReadBufferSize',
              parent=self, pos=wx.Point(322, 211), size=wx.Size(95, 13),
              style=0)

        self.cbDataWriteLength = wx.CheckBox(id=wxID_PANELDATACBDATAWRITELENGTH,
              label='Use entire file length', name='cbDataWriteLength',
              parent=self, pos=wx.Point(204, 258), size=wx.Size(116, 23),
              style=0)
        self.cbDataWriteLength.SetValue(True)
        self.cbDataWriteLength.SetToolTipString('')
        self.cbDataWriteLength.Bind(wx.EVT_CHECKBOX, self.OnCheckbox,
              id=wxID_PANELDATACBDATAWRITELENGTH)

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)
        
        # Hide async GUI as it is currently unimplemented
        self.cbDataReadUseAsync.Hide()
        self.tbDataReadBufferSize.Hide()
        self.tbDataReadNumBuffers.Hide()
        self.stDataReadBufferSize.Hide()
        self.stDataReadNumBuffers.Hide()
        
        self.cbDataWriteUseAsync.Hide()
        self.tbDataWriteBufferSize.Hide()
        self.tbDataWriteNumBuffers.Hide()
        self.stDataWriteBufferSize.Hide()
        self.stDataWriteNumBuffers.Hide()



    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        self.cbDataReadAutoIncAdr.Enable(ena and self.cbDataReadUseGpifAdr.GetValue())
        self.tbDataReadStartAddress.Enable(ena and self.cbDataReadUseGpifAdr.GetValue() and self.cbDataReadAutoIncAdr.GetValue())
        
        self.cbDataWriteAutoIncAdr.Enable(ena and self.cbDataWriteUseGpifAdr.GetValue())
        self.tbDataWriteStartAddress.Enable(ena and self.cbDataWriteUseGpifAdr.GetValue() and self.cbDataWriteAutoIncAdr.GetValue())
        
        self.tbDataReadBufferSize.Enable(ena and self.cbDataReadUseAsync.GetValue())
        self.tbDataReadNumBuffers.Enable(ena and self.cbDataReadUseAsync.GetValue())
        
        self.tbDataWriteBufferSize.Enable(ena and self.cbDataWriteUseAsync.GetValue())
        self.tbDataWriteNumBuffers.Enable(ena and self.cbDataWriteUseAsync.GetValue())
        
        self.tbDataWriteLength.Enable(ena and not self.cbDataWriteLength.GetValue())
        
        self.bDataRead.Enable(ena and self.tbDataReadFileName.GetValue() != "")
        self.bDataWrite.Enable(ena and os.path.isfile(self.tbDataWriteFileName.GetValue()))
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our 
        controls"""
        if not qusb:
            return
        
        # Read current value
        (ok, value) = qusb.ReadSetting(Setting.WordWide)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        self.cbDataWordwide.SetValue(value & 0x0001)
        
        self.UpdateCtrlsEnable(True)
        
        
        
    def PeriodicUpdateCtrls(self, qusb):
        """
        Called fy frameMain when we are to refresh our controls with values
        read off the QuickUSB Module
        """
        pass
    
    
    
    def LoadConf(self, conf):
        """
        Called by frameMain when we are to load our configuration settings
        """
        # Use GPIF Address Checkbox
        if 'DataReadUseGpifAdr' in conf:
            self.cbDataReadUseGpifAdr.SetValue(bool(conf['DataReadUseGpifAdr']))
        else:
            self.cbDataReadUseGpifAdr.SetValue(True)
        if 'DataWriteUseGpifAdr' in conf:
            self.cbDataWriteUseGpifAdr.SetValue(bool(conf['DataWriteUseGpifAdr']))
        else:
            self.cbDataWriteUseGpifAdr.SetValue(True)
        
        # Auto-Inc Address Checkbox
        if 'DataReadAutoIncAdr' in conf:
            self.cbDataReadAutoIncAdr.SetValue(bool(conf['DataReadAutoIncAdr']))
        else:
            self.cbDataReadAutoIncAdr.SetValue(True)
        if 'DataWriteAutoIncAdr' in conf:
            self.cbDataWriteAutoIncAdr.SetValue(bool(conf['DataWriteAutoIncAdr']))
        else:
            self.cbDataWriteAutoIncAdr.SetValue(True)
            
        # Start Address Textbox
        if 'DataReadStartAddress' in conf:
            self.tbDataReadStartAddress.SetValue(conf['DataReadStartAddress'])
        else:
            self.tbDataReadStartAddress.SetValue("0x0000")
        if 'DataWriteStartAddress' in conf:
            self.tbDataWriteStartAddress.SetValue(conf['DataWriteStartAddress'])
        else:
            self.tbDataWriteStartAddress.SetValue("0x0000")
        
        # Timeout Textbox
        if 'DataTimeout' in conf:
            self.tbDataTimeout.SetValue(conf['DataTimeout'])
        else:
            self.tbDataTimeout.SetValue("1000")

        # Length Textbox
        if 'DataReadLength' in conf:
            self.tbDataReadLength.SetValue(conf['DataReadLength'])
        else:
            self.tbDataReadLength.SetValue("1024")
        if 'DataWriteLength' in conf:
            self.tbDataWriteLength.SetValue(conf['DataWriteLength'])
        else:
            self.tbDataWriteLength.SetValue("1024")

        # File Name Textbox
        if 'DataReadFileName' in conf:
            self.tbDataReadFileName.SetValue(conf['DataReadFileName'])
        else:
            self.tbDataReadFileName.SetValue("")
        if 'DataWriteFileName' in conf:
            self.tbDataWriteFileName.SetValue(conf['DataWriteFileName'])
        else:
            self.tbDataWriteFileName.SetValue("")
        
        # Use File Length Checkbox
        if 'DataWriteLengthUseFileLength' in conf:
            self.cbDataWriteLength.SetValue(bool(conf['DataWriteLengthUseFileLength']))
        else:
            self.cbDataWriteLength.SetValue(True)

        # Use Async Checkbox -- NOT YET IMPLEMENTED
        #if 'DataReadUseAsync' in conf:
        #    self.cbDataReadUseAsync.SetValue(bool(conf['DataReadUseAsync']))
        #else:
        #    self.cbDataReadUseAsync.SetValue(False)
        #if 'DataWriteUseAsync' in conf:
        #    self.cbDataWriteUseAsync.SetValue(bool(conf['DataWriteUseAsync']))
        #else:
        #    self.cbDataWriteUseAsync.SetValue(False)
        
        # Buffer Size Textbox -- NOT YET IMPLEMENTED
        #if 'DataReadBufferSize' in conf:
        #    self.tbDataReadBufferSize.SetValue(conf['DataReadBufferSize'])
        #else:
        #    self.tbDataReadBufferSize.SetValue("0x0000")
        #if 'DataWriteBufferSize' in conf:
        #    self.tbDataWriteBufferSize.SetValue(conf['DataWriteBufferSize'])
        #else:
        #    self.tbDataWriteBufferSize.SetValue("0x0000")
        
        # Number of Buffers Textbox -- NOT YET IMPLEMENTED
        #if 'DataReadNumBuffers' in conf:
        #    self.tbDataReadNumBuffers.SetValue(conf['DataReadNumBuffers'])
        #else:
        #    self.tbDataReadNumBuffers.SetValue("3")
        #if 'DataWriteNumBuffers' in conf:
        #    self.tbDataWriteNumBuffers.SetValue(conf['DataWriteNumBuffers'])
        #else:
        #    self.tbDataWriteNumBuffers.SetValue("3")



    def SaveConf(self):
        """
        Called by frameMain when we are to save our configuration settings
        """
        return {'DataReadUseGpifAdr':self.cbDataReadUseGpifAdr.GetValue(),
                'DataWriteUseGpifAdr':self.cbDataWriteUseGpifAdr.GetValue(),
                'DataReadAutoIncAdr':self.cbDataReadAutoIncAdr.GetValue(),
                'DataWriteAutoIncAdr':self.cbDataWriteAutoIncAdr.GetValue(),
                'DataReadStartAddress':self.tbDataReadStartAddress.GetValue(),
                'DataWriteStartAddress':self.tbDataWriteStartAddress.GetValue(),
                'DataTimeout':self.tbDataTimeout.GetValue(),
                'DataReadLength':self.tbDataReadLength.GetValue(),
                'DataWriteLength':self.tbDataWriteLength.GetValue(),
                'DataReadFileName':self.tbDataReadFileName.GetValue(),
                'DataWriteFileName':self.tbDataWriteFileName.GetValue(),
                'DataWriteLengthUseFileLength':self.cbDataWriteLength.GetValue()}



    def OnBDataReadBrowseButton(self, event):
        """
        Event handle for the data read file browse button
        """
        # Create the dialog and select the first file filter
        dlg = wx.FileDialog(self, 'Save', '.', '', 'Binary Files (*.bin)|*.bin|All Files (*.*)|*.*', wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT)
        dlg.SetFilterIndex(0)
        
        # Show the dialog
        try:
            if dlg.ShowModal() == wx.ID_OK:
                # Place the file path in the appropriate textbox
                fileName = dlg.GetPath()
                self.tbDataReadFileName.SetValue(fileName)
                
                # Scroll cursor to end
                self.tbDataReadFileName.SetInsertionPointEnd()
        finally:
            dlg.Destroy()



    def OnBDataWriteBrowseButton(self, event):
        """
        Event handle for the data write file browse button
        """
        # Create the dialog and select the first file filter
        dlg = wx.FileDialog(self, 'Open', '.', '', 'Binary Files (*.bin)|*.bin|All Files (*.*)|*.*', wx.FD_OPEN)
        dlg.SetFilterIndex(0)
        
        # Show the dialog
        try:
            if dlg.ShowModal() == wx.ID_OK:
                # Place the file path in the appropriate textbox
                fileName = dlg.GetPath()
                self.tbDataWriteFileName.SetValue(fileName)
                
                # Scroll cursor to end
                self.tbDataWriteFileName.SetInsertionPointEnd()
        finally:
            dlg.Destroy()



    def OnCbWordwideCheckbox(self, event):
        """
        Event handle for the wordwide checkbox
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        # Read current value
        (ok, valueWordWide) = qusb.ReadSetting(Setting.WordWide)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update values
        if self.cbDataWordwide.GetValue():
            valueWordWide |= 0x0001
        else:
            valueWordWide &= ~0x0001
        
        # Write values
        (ok,) = qusb.WriteSetting(Setting.WordWide, valueWordWide)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Wait a little before we perform our next read
        time.sleep(0.01)
        
        # Read back values to ensure our state reflects our status
        self.UpdateCtrls(qusb)



    def OnBDataTimeoutSetButton(self, event):
        """
        Event handler for the set timeout button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Attempt to parse the timeout value
        try:
            timeout = self.TheFrame.GetInt(self.tbDataTimeout.GetValue())
        except ValueError:
            wx.MessageBox("Please provide a valid integer for the timeout", "Error")
            return
        
        # Set the timeout
        (ok,) = qusb.SetTimeout(timeout)
        if not ok:
            self.TheFrame.SetStatus("SetTimeout() failed with error: " + str(Error(qusb.LastError())))
            return

        self.TheFrame.SetStatus("Successfully set timeout")


    def OnCheckbox(self, event):
        """
        Event handler for many of the checkboxes on this panel
        """
        # All of the checkboxes on this panel simply enable/disable R/W
        # functionality and need only to enable/disable other controls
        self.UpdateCtrlsEnable(True)



    def OnBrowseText(self, event):
        """
        Event handler for browse textbox
        """
        # If the user is editing the text for the file name, update enable/
        # diable controls as a valid file path enables/disables R/W
        self.UpdateCtrlsEnable(True)
        
        
        
    def OnBDataReadButton(self, event):
        """
        Event handler for the data read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        # Validate all fields contain valid information to perform the
        # requested operation
        if not self.ValidateFields(True):
            return

        # Setup the GPIF Address Bus
        self.SetupGPIF(qusb, True)

        # Grab the setup info
        wordwide = self.cbDataWordwide.GetValue()
        bytes = self.TheFrame.GetInt(self.tbDataReadLength.GetValue())
        words = bytes / 2

        # Perfom the read
        #if self.cbDataReadUseAsync.GetValue():
        # Perform an asynchronous read transaction
        #    pass

        # Perform an synchronous read transaction

        # Create the data buffers
        if wordwide:
            buffer = CreateWordBuffer(words)
        else:
            buffer = CreateByteBuffer(bytes)

        # Perform the transaction
        if wordwide:
            (ok, wordsRead) = qusb.ReadDataWords(buffer, words)
            if not ok:
                self.TheFrame.SetStatus("ReadDataWords() failed with error: " + str(Error(qusb.LastError())))
                return
        else:
            (ok, bytesRead) = qusb.ReadData(buffer, bytes)
            if not ok:
                self.TheFrame.SetStatus("ReadData() failed with error: " + str(Error(qusb.LastError())))
                return
                
        # Open the file and write the data
        fout = open(self.tbDataReadFileName.GetValue(), 'wb')
        fout.write(buffer)
        fout.close()

        self.TheFrame.SetStatus("Successfully read data from GPIF")
                


    def OnBDataWriteButton(self, event):
        """
        Event handler for the data read button
        """
        # Ensure a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # Validate all fields contain valid information to perform the
        # requested operation
        if not self.ValidateFields(False):
            return
        
        # Setup the GPIF Address Bus
        self.SetupGPIF(qusb, False)
        
        # Grab the setup info
        wordwide = self.cbDataWordwide.GetValue()
        bytes = self.TheFrame.GetInt(self.tbDataWriteLength.GetValue())
        words = bytes / 2
        
        # Perfom the write
        #if self.cbDataReadUseAsync.GetValue():
        #    # Perform an asynchronous write transaction
        #    pass
        #else:
            
        # Perform an synchronous write transaction
        
        # Open the file and read the data
        fin = open(self.tbDataWriteFileName.GetValue(), 'rb')
        data = fin.read()
        fin.close()
            
        # Create the data buffers
        bytes = self.TheFrame.GetInt(self.tbDataReadLength.GetValue())
        words = bytes / 2
        if wordwide:
            buffer = CreateWordBuffer(words)
            for x in xrange(words):
                buffer[x] = (ord(data[x*2 + 1]) << 8) | ord(data[x*2 + 0])
        else:
            buffer = CreateByteBuffer(bytes, [ord(x) for x in list(data)])
        
        # Perform the transaction
        if wordwide:
            (ok,) = qusb.WriteDataWords(buffer, words)
            if not ok:
                self.TheFrame.SetStatus("WriteDataWords() failed with error: " + str(Error(qusb.LastError())))
                return
        else:
            (ok,) = qusb.WriteData(buffer, bytes)
            if not ok:
                self.TheFrame.SetStatus("WriteData() failed with error: " + str(Error(qusb.LastError())))
                return

        self.TheFrame.SetStatus("Successfully wrote data to GPIF")

            
    
    def ValidateFields(self, read):
        """
        Helper function to ensure that all fields for a read or a write are
        properly filled out.  If they are not an error message is displayed
        describing the error.
        """
        if read:
            # Check the start address
            if self.cbDataReadUseGpifAdr.GetValue() and self.cbDataReadAutoIncAdr.GetValue():
                try:
                    self.TheFrame.GetInt(self.tbDataReadStartAddress.GetValue())
                except:
                    wx.MessageBox("Please provide a valid integer or hex value for the read address", "Error")
                    return False
                
            # Check the number of bytes
            try:
                self.TheFrame.GetInt(self.tbDataReadLength.GetValue())
            except ValueError:
                wx.MessageBox("Please provide a valid integer or hex value for the number of bytes to read", "Error")
                return False
            
            # Ensure that a file has been specified
            if not self.tbDataReadFileName.GetValue():
                wx.MessageBox("Please specify a file to save the read data to", "Error")
                return False
        else:
            # Check the start address
            if self.cbDataReadUseGpifAdr.GetValue() and self.cbDataReadAutoIncAdr.GetValue():
                try:
                    self.TheFrame.GetInt(self.tbDataReadStartAddress.GetValue())
                except:
                    wx.MessageBox("Please provide a valid integer or hex value for the write address", "Error")
                    return False
                
            # Check the number of bytes
            if not self.cbDataWriteLength.GetValue():
                try:
                    self.TheFrame.GetInt(self.tbDataWriteLength.GetValue())
                except ValueError:
                    wx.MessageBox("Please provide a valid integer or hex value for the number of bytes to write", "Error")
                    return False
            
            # Check that the file is valid
            if not os.path.isfile(self.tbDataWriteFileName.GetValue()):
                wx.MessageBox("The file '%s' does not exist.  Please specify a valid file." % self.tbDataWriteFileName.GetValue(), "Error")
                return False
        
        # Everything checked out okay
        return True


        
    def SetupGPIF(self, qusb, read):
        """
        Helper function to setup the GPIF on the module based on the value of
        all the controls on this panel
        """
        # Read current value
        (ok, value) = qusb.ReadSetting(Setting.DataAddress)
        if not ok:
            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
            return
        
        # Update values
        if read:
            # Set GPIF to on/off
            if self.cbDataReadUseGpifAdr.GetValue():
                value &= ~0x4000
            else:
                value |= 0x4000
                
            # Set GPIF auto-inc to on/off
            if self.cbDataReadAutoIncAdr.GetValue():
                value &= ~0x8000
            else:
                value |= 0x8000
            
            # Set the GPIF address
            value = (value & ~0x001f) | self.TheFrame.GetInt(self.tbDataReadStartAddress.GetValue())
        else:
            # Set GPIF to on/off
            if self.cbDataWriteUseGpifAdr.GetValue():
                value &= ~0x4000
            else:
                value |= 0x4000
                
            # Set GPIF auto-inc to on/off
            if self.cbDataWriteAutoIncAdr.GetValue():
                value &= ~0x8000
            else:
                value |= 0x8000
            
            # Set the GPIF address
            value = (value & ~0x001f) | self.TheFrame.GetInt(self.tbDataWriteStartAddress.GetValue())
        
        # Write values
        (ok,) = qusb.WriteSetting(Setting.DataAddress, value)
        if not ok:
            self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
            return
