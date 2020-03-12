#Boa:Dialog:frameAbout
"""
============================================================================
 Title       : frameAbout.py
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
import wx.lib.hyperlink

# Bitwise Systems imports
from QuickUsb import *

# GUI imports
import QuickUsbDiag

# Python imports
import os
import sys

logoPath = 'QuickUsbLogo.bmp'
if "_MEIPASS2" in os.environ:
    logoPath = os.path.join(os.environ["_MEIPASS2"], 'QuickUsbLogo.bmp')
else:
    if not os.path.exists(logoPath):
        logoPath = os.path.join(os.path.dirname(sys.argv[0]), logoPath)

def create(parent):
    return frameAbout(parent)



[wxID_FRAMEABOUT, wxID_FRAMEABOUTBUTTONOK, wxID_FRAMEABOUTLABELCOPYRIGHT, 
 wxID_FRAMEABOUTLABELDLLVERSION, wxID_FRAMEABOUTLABELDRIVERVERSION, 
 wxID_FRAMEABOUTLABELTITLE, wxID_FRAMEABOUTQUSBIMAGE, 
] = [wx.NewId() for _init_ctrls in range(7)]



class frameAbout(wx.Dialog):
    def _init_coll_szbMain_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.qusbImage, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.labelTitle, 0, border=0,
              flag=wx.GROW | wx.ALIGN_CENTER_HORIZONTAL)
        parent.AddWindow(self.labelCopyright, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.labelDllVersion, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.labelDriverVersion, 0, border=0, flag=wx.GROW)
        parent.AddSpacer(wx.Size(8,12), border=0, flag=0)
        parent.AddWindow(self.buttonOK, 0, border=0,
              flag=wx.ALIGN_CENTER_HORIZONTAL)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)

    def _init_sizers(self):
        # generated method, don't edit
        self.szbMain = wx.BoxSizer(orient=wx.VERTICAL)

        self._init_coll_szbMain_Items(self.szbMain)

        self.SetSizer(self.szbMain)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Dialog.__init__(self, id=wxID_FRAMEABOUT, name='QuickUsbDiagAbout',
              parent=prnt, pos=wx.Point(573, 298), size=wx.Size(316, 243),
              style=wx.DEFAULT_DIALOG_STYLE | wx.CLOSE_BOX,
              title='About QuickUSB Diag')
        self.SetClientSize(wx.Size(310, 215))
        self.SetBackgroundColour(wx.Colour(255, 255, 255))

        self.labelTitle = wx.StaticText(id=wxID_FRAMEABOUTLABELTITLE,
              label='QuickUSB Diag', name='labelTitle', parent=self,
              pos=wx.Point(0, 101), size=wx.Size(310, 13),
              style=wx.ALIGN_CENTRE)
        self.labelTitle.SetBackgroundColour(wx.Colour(255, 255, 255))
        self.labelTitle.SetMinSize(wx.Size(-1, -1))

        self.labelCopyright = wx.StaticText(id=wxID_FRAMEABOUTLABELCOPYRIGHT,
              label='Copyright (c) 2012 Bitwise Systems',
              name='labelCopyright', parent=self, pos=wx.Point(0, 114),
              size=wx.Size(310, 13), style=wx.ALIGN_CENTRE)
        self.labelCopyright.SetBackgroundColour(wx.Colour(255, 255, 255))
        self.labelCopyright.SetMinSize(wx.Size(-1, -1))

        self.labelDllVersion = wx.StaticText(id=wxID_FRAMEABOUTLABELDLLVERSION,
              label='DLL Version', name='labelDllVersion', parent=self,
              pos=wx.Point(0, 127), size=wx.Size(310, 13),
              style=wx.ALIGN_CENTRE)
        self.labelDllVersion.SetBackgroundColour(wx.Colour(255, 255, 255))
        self.labelDllVersion.SetMinSize(wx.Size(-1, -1))

        self.labelDriverVersion = wx.StaticText(id=wxID_FRAMEABOUTLABELDRIVERVERSION,
              label='Driver Version', name='labelDriverVersion', parent=self,
              pos=wx.Point(0, 140), size=wx.Size(310, 13),
              style=wx.ALIGN_CENTRE)
        self.labelDriverVersion.SetBackgroundColour(wx.Colour(255, 255, 255))
        self.labelDriverVersion.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        self.labelDriverVersion.SetMinSize(wx.Size(-1, -1))

        self.qusbImage = wx.StaticBitmap(bitmap=wx.Bitmap(logoPath,
              wx.BITMAP_TYPE_BMP), id=wxID_FRAMEABOUTQUSBIMAGE,
              name='qusbImage', parent=self, pos=wx.Point(4, 4),
              size=wx.Size(294, 93), style=0)
        self.qusbImage.SetBackgroundColour(wx.Colour(255, 255, 255))
        self.qusbImage.SetMinSize(wx.Size(294, 93))

        self.buttonOK = wx.Button(id=wxID_FRAMEABOUTBUTTONOK, label='OK',
              name='buttonOK', parent=self, pos=wx.Point(115, 165),
              size=wx.Size(80, 23), style=0)
        self.buttonOK.SetDefault()
        self.buttonOK.Bind(wx.EVT_BUTTON, self.OnButtonOKButton,
              id=wxID_FRAMEABOUTBUTTONOK)

        self._init_sizers()

    def __init__(self, parent):
        self._init_ctrls(parent)
        
        minSize = self.GetSizer().GetMinSize()
        self.SetClientSize(minSize)
        self.SetMinSize(self.GetSize())
        
        text = "%s %s" % (QuickUsbDiag.AppTitle, QuickUsbDiag.AppVersion)
        self.labelTitle.SetLabel(text)

        (ok, major, minor, rev) = QuickUsb.GetDllVersion()
        text = "DLL Version v%i.%i.%i" % (major, minor, rev)
        self.labelDllVersion.SetLabel(text)
        
        (ok, major, minor, rev) = QuickUsb.GetDriverVersion()
        text = "Driver Version v%i.%i.%i" % (major, minor, rev)
        self.labelDriverVersion.SetLabel(text)
        


    def OnButtonOKButton(self, event):
        self.Close()
