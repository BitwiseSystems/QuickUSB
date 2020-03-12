#Boa:FramePanel:panelPorts
"""
============================================================================
 Title       : panelPorts.py
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
from BitTwiddler import *

# Python imports
import time

# Generated Boa Constructor wx IDs
[wxID_PANELPORTS, wxID_PANELPORTSBTDIRPORTA, wxID_PANELPORTSBTDIRPORTB, 
 wxID_PANELPORTSBTDIRPORTC, wxID_PANELPORTSBTDIRPORTD, 
 wxID_PANELPORTSBTDIRPORTE, wxID_PANELPORTSBTVALUEPORTA, 
 wxID_PANELPORTSBTVALUEPORTB, wxID_PANELPORTSBTVALUEPORTC, 
 wxID_PANELPORTSBTVALUEPORTD, wxID_PANELPORTSBTVALUEPORTE, 
 wxID_PANELPORTSPPORTA, wxID_PANELPORTSPPORTB, wxID_PANELPORTSPPORTC, 
 wxID_PANELPORTSPPORTD, wxID_PANELPORTSPPORTE, wxID_PANELPORTSSBPORTA, 
 wxID_PANELPORTSSBPORTB, wxID_PANELPORTSSBPORTC, wxID_PANELPORTSSBPORTD, 
 wxID_PANELPORTSSBPORTE, wxID_PANELPORTSSTATICTEXT1, 
 wxID_PANELPORTSSTATICTEXT2, 
] = [wx.NewId() for _init_ctrls in range(23)]

class panelPorts(wx.Panel):
    def _init_coll_szbPortBPanel_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbPortB, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortEPanel_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbPortE, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortC_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btDirPortC, 0, border=4, flag=wx.ALL | 4)
        parent.AddWindow(self.btValuePortC, 0, border=4, flag=wx.ALL)

    def _init_coll_szbPortA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btDirPortA, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.btValuePortA, 0, border=4, flag=wx.ALL)

    def _init_coll_szbPortAPanel_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbPortA, 0, border=0, flag=wx.GROW)
        parent.AddSpacer(wx.Size(16, 1), border=0, flag=0)
        parent.AddSizer(self.szbNotes, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortDPanel_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbPortD, 0, border=0, flag=wx.GROW)

    def _init_coll_bszPortsMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbPortA, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbPortB, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbPortC, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbPortD, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbPortE, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbPortD_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pPortD, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortCPanel_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbPortC, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbPortC_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pPortC, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbPortA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pPortA, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortD_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btDirPortD, 0, border=4, flag=wx.ALL | 4)
        parent.AddWindow(self.btValuePortD, 0, border=4, flag=wx.ALL)

    def _init_coll_szbPortE_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btDirPortE, 0, border=4, flag=wx.ALL | 4)
        parent.AddWindow(self.btValuePortE, 0, border=4, flag=wx.ALL)

    def _init_coll_szbNotes_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.staticText1, 0, border=4, flag=wx.GROW | wx.ALL)
        parent.AddSpacer(wx.Size(1, 4), border=0, flag=0)
        parent.AddWindow(self.staticText2, 0, border=4, flag=wx.GROW | wx.ALL)

    def _init_coll_szsbPortE_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pPortE, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbPortB_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pPortB, 0, border=0, flag=wx.GROW)

    def _init_coll_szbPortB_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btDirPortB, 0, border=4, flag=wx.ALL)
        parent.AddWindow(self.btValuePortB, 0, border=4, flag=wx.ALL)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszPortsMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbPortA = wx.StaticBoxSizer(box=self.sbPortA,
              orient=wx.HORIZONTAL)

        self.szsbPortB = wx.StaticBoxSizer(box=self.sbPortB,
              orient=wx.HORIZONTAL)

        self.szsbPortC = wx.StaticBoxSizer(box=self.sbPortC,
              orient=wx.HORIZONTAL)

        self.szsbPortD = wx.StaticBoxSizer(box=self.sbPortD,
              orient=wx.HORIZONTAL)

        self.szsbPortE = wx.StaticBoxSizer(box=self.sbPortE,
              orient=wx.HORIZONTAL)

        self.szbPortA = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbPortB = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbPortC = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbPortD = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbPortE = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbNotes = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbPortAPanel = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbPortBPanel = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbPortCPanel = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbPortDPanel = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbPortEPanel = wx.BoxSizer(orient=wx.HORIZONTAL)

        self._init_coll_bszPortsMain_Items(self.bszPortsMain)
        self._init_coll_szsbPortA_Items(self.szsbPortA)
        self._init_coll_szsbPortB_Items(self.szsbPortB)
        self._init_coll_szsbPortC_Items(self.szsbPortC)
        self._init_coll_szsbPortD_Items(self.szsbPortD)
        self._init_coll_szsbPortE_Items(self.szsbPortE)
        self._init_coll_szbPortA_Items(self.szbPortA)
        self._init_coll_szbPortB_Items(self.szbPortB)
        self._init_coll_szbPortC_Items(self.szbPortC)
        self._init_coll_szbPortD_Items(self.szbPortD)
        self._init_coll_szbPortE_Items(self.szbPortE)
        self._init_coll_szbNotes_Items(self.szbNotes)
        self._init_coll_szbPortAPanel_Items(self.szbPortAPanel)
        self._init_coll_szbPortBPanel_Items(self.szbPortBPanel)
        self._init_coll_szbPortCPanel_Items(self.szbPortCPanel)
        self._init_coll_szbPortDPanel_Items(self.szbPortDPanel)
        self._init_coll_szbPortEPanel_Items(self.szbPortEPanel)

        self.SetSizer(self.bszPortsMain)
        self.pPortE.SetSizer(self.szbPortEPanel)
        self.pPortD.SetSizer(self.szbPortDPanel)
        self.pPortA.SetSizer(self.szbPortAPanel)
        self.pPortC.SetSizer(self.szbPortCPanel)
        self.pPortB.SetSizer(self.szbPortBPanel)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELPORTS, name='panelPorts',
              parent=prnt, pos=wx.Point(654, 313), size=wx.Size(445, 432),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(429, 394))

        self.sbPortA = wx.StaticBox(id=wxID_PANELPORTSSBPORTA, label='Port A',
              name='sbPortA', parent=self, pos=wx.Point(0, 0), size=wx.Size(429,
              74), style=0)
        self.sbPortA.SetToolTipString('')

        self.sbPortB = wx.StaticBox(id=wxID_PANELPORTSSBPORTB, label='Port B',
              name='sbPortB', parent=self, pos=wx.Point(0, 74),
              size=wx.Size(429, 74), style=0)

        self.sbPortC = wx.StaticBox(id=wxID_PANELPORTSSBPORTC, label='Port C',
              name='sbPortC', parent=self, pos=wx.Point(0, 148),
              size=wx.Size(429, 74), style=0)

        self.sbPortD = wx.StaticBox(id=wxID_PANELPORTSSBPORTD, label='Port D',
              name='sbPortD', parent=self, pos=wx.Point(0, 222),
              size=wx.Size(429, 74), style=0)

        self.sbPortE = wx.StaticBox(id=wxID_PANELPORTSSBPORTE, label='Port E',
              name='sbPortE', parent=self, pos=wx.Point(0, 296),
              size=wx.Size(429, 74), style=0)

        self.pPortA = wx.Panel(id=wxID_PANELPORTSPPORTA, name='pPortA',
              parent=self, pos=wx.Point(5, 17), size=wx.Size(405, 52),
              style=wx.TAB_TRAVERSAL)

        self.pPortB = wx.Panel(id=wxID_PANELPORTSPPORTA, name='pPortB',
              parent=self, pos=wx.Point(5, 91), size=wx.Size(222, 52),
              style=wx.TAB_TRAVERSAL)

        self.pPortC = wx.Panel(id=wxID_PANELPORTSPPORTA, name='pPortC',
              parent=self, pos=wx.Point(5, 165), size=wx.Size(222, 52),
              style=wx.TAB_TRAVERSAL)

        self.pPortD = wx.Panel(id=wxID_PANELPORTSPPORTA, name='pPortD',
              parent=self, pos=wx.Point(5, 239), size=wx.Size(222, 52),
              style=wx.TAB_TRAVERSAL)

        self.pPortE = wx.Panel(id=wxID_PANELPORTSPPORTA, name='pPortE',
              parent=self, pos=wx.Point(5, 313), size=wx.Size(222, 52),
              style=wx.TAB_TRAVERSAL)

        self.btDirPortD = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTDIRPORTD, label='Direction:',
              name='btDirPortD', parent=self.pPortD, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(214, 18), style=wx.NO_BORDER,
              tagint=3,
              tooltips=['Bit 0: PD[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PD[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PD[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PD[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PD[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PD[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PD[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PD[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btDirPortD.SetToolTipString('')
        self.btDirPortD.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTDIRPORTD)

        self.btDirPortB = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTDIRPORTB, label='Direction:',
              name='btDirPortB', parent=self.pPortB, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(214, 18), style=wx.NO_BORDER,
              tagint=1,
              tooltips=['Bit 0: PB[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PB[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PB[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PB[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PB[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PB[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PB[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PB[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btDirPortB.SetToolTipString('')
        self.btDirPortB.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTDIRPORTB)

        self.btDirPortC = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTDIRPORTC, label='Direction:',
              name='btDirPortC', parent=self.pPortC, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(214, 18), style=wx.NO_BORDER,
              tagint=2,
              tooltips=['Bit 0: PC[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PC[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PC[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PC[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PC[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PC[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PC[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PC[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btDirPortC.SetToolTipString('')
        self.btDirPortC.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTDIRPORTC)

        self.btDirPortE = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTDIRPORTE, label='Direction:',
              name='btDirPortE', parent=self.pPortE, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(214, 18), style=wx.NO_BORDER,
              tagint=4,
              tooltips=['Bit 0: PE[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PE[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PE[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PE[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PE[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PE[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PE[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PE[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btDirPortE.SetToolTipString('')
        self.btDirPortE.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTDIRPORTE)

        self.btValuePortD = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTVALUEPORTD, label='Value:',
              name='btValuePortD', parent=self.pPortD, pos=wx.Point(4, 30),
              showlabel=True, size=wx.Size(199, 18), style=wx.NO_BORDER,
              tagint=3, tooltips=['Bit 0: PD[0] Value', 'Bit 1: PD[1] Value',
              'Bit 2: PD[2] Value', 'Bit 3: PD[3] Value', 'Bit 4: PD[4] Value',
              'Bit 5: PD[5] Value', 'Bit 6: PD[6] Value', 'Bit 7: PD[7] Value'],
              value=0)
        self.btValuePortD.SetToolTipString('')
        self.btValuePortD.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTVALUEPORTD)

        self.btValuePortE = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTVALUEPORTE, label='Value:',
              name='btValuePortE', parent=self.pPortE, pos=wx.Point(4, 30),
              showlabel=True, size=wx.Size(199, 18), style=wx.NO_BORDER,
              tagint=4, tooltips=['Bit 0: PE[0] Value', 'Bit 1: PE[1] Value',
              'Bit 2: PE[2] Value', 'Bit 3: PE[3] Value', 'Bit 4: PE[4] Value',
              'Bit 5: PE[5] Value', 'Bit 6: PE[6] Value', 'Bit 7: PE[7] Value'],
              value=0)
        self.btValuePortE.SetToolTipString('')
        self.btValuePortE.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTVALUEPORTE)

        self.btValuePortC = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTVALUEPORTC, label='Value:',
              name='btValuePortC', parent=self.pPortC, pos=wx.Point(4, 30),
              showlabel=True, size=wx.Size(199, 18), style=wx.NO_BORDER,
              tagint=2, tooltips=['Bit 0: PC[0] Value', 'Bit 1: PC[1] Value',
              'Bit 2: PC[2] Value', 'Bit 3: PC[3] Value', 'Bit 4: PC[4] Value',
              'Bit 5: PC[5] Value', 'Bit 6: PC[6] Value', 'Bit 7: PC[7] Value'],
              value=0)
        self.btValuePortC.SetToolTipString('')
        self.btValuePortC.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTVALUEPORTC)

        self.btValuePortB = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTVALUEPORTB, label='Value:',
              name='btValuePortB', parent=self.pPortB, pos=wx.Point(4, 30),
              showlabel=True, size=wx.Size(199, 18), style=wx.NO_BORDER,
              tagint=1, tooltips=['Bit 0: PB[0] Value', 'Bit 1: PB[1] Value',
              'Bit 2: PB[2] Value', 'Bit 3: PB[3] Value', 'Bit 4: PB[4] Value',
              'Bit 5: PB[5] Value', 'Bit 6: PB[6] Value', 'Bit 7: PB[7] Value'],
              value=0)
        self.btValuePortB.SetToolTipString('')
        self.btValuePortB.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTVALUEPORTB)

        self.btValuePortA = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTVALUEPORTA, label='Value:',
              name='btValuePortA', parent=self.pPortA, pos=wx.Point(4, 30),
              showlabel=True, size=wx.Size(199, 18), style=wx.NO_BORDER,
              tagint=0, tooltips=['Bit 0: PA[0] Value', 'Bit 1: PA[1] Value',
              'Bit 2: PA[2] Value', 'Bit 3: PA[3] Value', 'Bit 4: PA[4] Value',
              'Bit 5: PA[5] Value', 'Bit 6: PA[6] Value', 'Bit 7: PA[7] Value'],
              value=0)
        self.btValuePortA.SetToolTipString('')
        self.btValuePortA.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTVALUEPORTA)

        self.staticText1 = wx.StaticText(id=wxID_PANELPORTSSTATICTEXT1,
              label='Direction: 0=Input, 1=Output', name='staticText1',
              parent=self.pPortA, pos=wx.Point(242, 4), size=wx.Size(144, 13),
              style=0)
        self.staticText1.SetMinSize((-1, -1))

        self.staticText2 = wx.StaticText(id=wxID_PANELPORTSSTATICTEXT2,
              label='Value: 0=Logic low, 1=Logic high', name='staticText2',
              parent=self.pPortA, pos=wx.Point(242, 25), size=wx.Size(159, 13),
              style=0)
        self.staticText2.SetMinSize((-1, -1))

        self.btDirPortA = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELPORTSBTDIRPORTA, label='Direction:',
              name='btDirPortA', parent=self.pPortA, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(214, 18), style=wx.NO_BORDER,
              tagint=0,
              tooltips=['Bit 0: PA[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PA[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PA[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PA[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PA[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PA[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PA[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PA[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btDirPortA.SetToolTipString('')
        self.btDirPortA.Bind(wx.EVT_CHECKBOX, self.OnBtPortCheckbox,
              id=wxID_PANELPORTSBTDIRPORTA)

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)



    def SetControlMinSizes(self):
        for valueCtrl, dirCtrl, port in self.GetPorts():
            vW, vH = valueCtrl.GetBestSize()
            dW, dH = dirCtrl.GetBestSize()
            w = max(vW, dW)
            
            dirCtrl.SetMinSize((w, dH))
            valueCtrl.SetMinSize((w, vH))



    def GetPorts(self):
        return [(self.btValuePortA, self.btDirPortA, Port.A),
                (self.btValuePortB, self.btDirPortB, Port.B), 
                (self.btValuePortC, self.btDirPortC, Port.C), 
                (self.btValuePortD, self.btDirPortD, Port.D), 
                (self.btValuePortE, self.btDirPortE, Port.E)]
                 
                 
                           
    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        pass
        
        
        
    def UpdateCtrls(self, qusb):
        """Called by frameMain when we need to update the values within our 
        controls
        """
        data = CreateByteBuffer(1)
        for valueCtrl, dirCtrl, port in self.GetPorts():
            (ok, dir) = qusb.ReadPortDir(port)
            if not ok:
                self.TheFrame.SetStatus("ReadPortDir() failed with error: " + str(Error(qusb.LastError())))
                return
            
            dirCtrl.SetValue(dir)

            (ok, bytes) = qusb.ReadPort(port, data, 1)
            if not ok:
                self.TheFrame.SetStatus("ReadPort() failed with error: " + str(Error(qusb.LastError())))
                return
            
            valueCtrl.SetValue(data[0])

        
        
    def PeriodicUpdateCtrls(self, qusb):
        """
        Called by frameMain when we are to refresh our controls with values
        read off the QuickUSB Module
        """
        self.UpdateCtrls(qusb)
    
    
    
    def LoadConf(self, conf):
        """Called by frameMain when we are to load our configuration settings"""
        pass
            
            
            
    def SaveConf(self):
        """Called by frameMain when we are to save our configuration settings"""
        return {}



    def OnBtPortCheckbox(self, event):
        """
        Event handler for the BitTwiddler controls
        """
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return
        
        # The event object is a reference to the BitTwiddler object that was
        # actually clicked
        obj = event.GetEventObject()
        
        # The tag int is the integer index of the port the BitTwiddler is
        # controlling
        address = obj.GetTagInt()
        
        for valueCtrl, dirCtrl, port in self.GetPorts():
            if (dirCtrl is obj) or (valueCtrl is obj):
                dir, value = dirCtrl.GetValue(), valueCtrl.GetValue()

                # Write port direction
                if dirCtrl is obj:
                    (ok,) = qusb.WritePortDir(address, dir)
                    if not ok:
                        self.TheFrame.SetStatus("WritePortDir() failed with error: " + str(Error(qusb.LastError())))
                        return
                
                    if self.TheFrame._performReadBack:
                        # Wait a little to make sure the write took
                        time.sleep(0.01)
                        
                        # Read back port direction
                        (ok, newDir) = qusb.ReadPortDir(address)
                        if not ok:
                            self.TheFrame.SetStatus("ReadPortDir() failed with error: " + str(Error(qusb.LastError())))
                            return
                else:
                    # Create a ctypes-style byte buffer
                    buffer = CreateByteBuffer(1, (value,))

                    # Write port value
                    (ok,) = qusb.WritePort(address, buffer, 1)
                    if not ok:
                        self.TheFrame.SetStatus("WritePort() failed with error: " + str(Error(qusb.LastError())))
                        return

                    if self.TheFrame._performReadBack:
                        # Wait a little to make sure the write took
                        time.sleep(0.01)
                        
                        #Read back port value
                        (ok, bytes) = qusb.ReadPort(address, buffer, 1)
                        if not ok:
                            self.TheFrame.SetStatus("ReadPort() failed with error: " + str(Error(qusb.LastError())))
                            return
                        valueCtrl.SetValue(buffer[0])
                break
