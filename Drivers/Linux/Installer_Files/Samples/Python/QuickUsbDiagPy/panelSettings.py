#Boa:FramePanel:panelSettings
"""
============================================================================
 Title       : panelSettings.py
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

#Python imports
import time

# Bitwise Systems imports
from QuickUsb import *
from BitTwiddler import *

# Generated Boa Constructor wx IDs
[wxID_PANELSETTINGS, wxID_PANELSETTINGSBTSCPUCONFIGH, 
 wxID_PANELSETTINGSBTSCPUCONFIGL, wxID_PANELSETTINGSBTSDATAADDRESSH, 
 wxID_PANELSETTINGSBTSDATAADDRESSL, wxID_PANELSETTINGSBTSEP26CONFIGH, 
 wxID_PANELSETTINGSBTSEP26CONFIGL, wxID_PANELSETTINGSBTSFIFOCONFIGH, 
 wxID_PANELSETTINGSBTSFIFOCONFIGL, wxID_PANELSETTINGSBTSFPGATYPEH, 
 wxID_PANELSETTINGSBTSFPGATYPEL, wxID_PANELSETTINGSBTSI2CTLH, 
 wxID_PANELSETTINGSBTSI2CTLL, wxID_PANELSETTINGSBTSPINFLAGSH, 
 wxID_PANELSETTINGSBTSPINFLAGSL, wxID_PANELSETTINGSBTSPORTACCFGH, 
 wxID_PANELSETTINGSBTSPORTACCFGL, wxID_PANELSETTINGSBTSPORTAH, 
 wxID_PANELSETTINGSBTSPORTAL, wxID_PANELSETTINGSBTSPORTBH, 
 wxID_PANELSETTINGSBTSPORTBL, wxID_PANELSETTINGSBTSPORTCH, 
 wxID_PANELSETTINGSBTSPORTCL, wxID_PANELSETTINGSBTSPORTDH, 
 wxID_PANELSETTINGSBTSPORTDL, wxID_PANELSETTINGSBTSPORTEH, 
 wxID_PANELSETTINGSBTSPORTEL, wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSH, 
 wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSL, wxID_PANELSETTINGSBTSSPICONFIGH, 
 wxID_PANELSETTINGSBTSSPICONFIGL, wxID_PANELSETTINGSBTSVERSIONBUILDH, 
 wxID_PANELSETTINGSBTSVERSIONBUILDL, wxID_PANELSETTINGSBTSVERSIONSPEEDH, 
 wxID_PANELSETTINGSBTSVERSIONSPEEDL, wxID_PANELSETTINGSBTSWORDWIDEH, 
 wxID_PANELSETTINGSBTSWORDWIDEL, wxID_PANELSETTINGSPSETTINGS, 
 wxID_PANELSETTINGSSBSETTINGS, 
] = [wx.NewId() for _init_ctrls in range(39)]

class panelSettings(wx.Panel):
    def _init_coll_szsbSettings_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.pSettings, 0, border=0, flag=wx.GROW)

    def _init_coll_bszSettingsMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbSettings, 0, border=0, flag=wx.GROW)

    def _init_coll_szbSettings_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szgSettings, 1, border=4, flag=wx.GROW | wx.ALL)

    def _init_coll_szgSettings_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.btsEP26CONFIGH, 0, border=0, flag=0)
        parent.AddWindow(self.btsEP26CONFIGL, 0, border=0, flag=0)
        parent.AddWindow(self.btsWORDWIDEH, 0, border=0, flag=0)
        parent.AddWindow(self.btsWORDWIDEL, 0, border=0, flag=0)
        parent.AddWindow(self.btsDATAADDRESSH, 0, border=0, flag=0)
        parent.AddWindow(self.btsDATAADDRESSL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsFIFOCONFIGH, 0, border=0, flag=0)
        parent.AddWindow(self.btsFIFOCONFIGL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsFPGATYPEH, 0, border=0, flag=0)
        parent.AddWindow(self.btsFPGATYPEL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsCPUCONFIGH, 0, border=0, flag=0)
        parent.AddWindow(self.btsCPUCONFIGL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsSPICONFIGH, 0, border=0, flag=0)
        parent.AddWindow(self.btsSPICONFIGL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsSLAVEFIFOFLAGSH, 0, border=0, flag=0)
        parent.AddWindow(self.btsSLAVEFIFOFLAGSL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsI2CTLH, 0, border=0, flag=0)
        parent.AddWindow(self.btsI2CTLL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTAH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTAL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTBH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTBL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTCH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTCL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTDH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTDL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTEH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTEL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPORTACCFGH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPORTACCFGL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsPINFLAGSH, 0, border=0, flag=0)
        parent.AddWindow(self.btsPINFLAGSL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsVERSIONBUILDH, 0, border=0, flag=0)
        parent.AddWindow(self.btsVERSIONBUILDL, 0, border=0,
              flag=0)
        parent.AddWindow(self.btsVERSIONSPEEDH, 0, border=0, flag=0)
        parent.AddWindow(self.btsVERSIONSPEEDL, 0, border=0,
              flag=0)

    def _init_sizers(self):
        # generated method, don't edit
        self.bszSettingsMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbSettings = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szsbSettings = wx.StaticBoxSizer(box=self.sbSettings,
              orient=wx.HORIZONTAL)

        self.szgSettings = wx.GridSizer(cols=2, hgap=16, rows=18, vgap=2)

        self._init_coll_bszSettingsMain_Items(self.bszSettingsMain)
        self._init_coll_szbSettings_Items(self.szbSettings)
        self._init_coll_szsbSettings_Items(self.szsbSettings)
        self._init_coll_szgSettings_Items(self.szgSettings)

        self.SetSizer(self.bszSettingsMain)
        self.pSettings.SetSizer(self.szbSettings)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Panel.__init__(self, id=wxID_PANELSETTINGS, name='panelSettings',
              parent=prnt, pos=wx.Point(654, 293), size=wx.Size(591, 407),
              style=wx.TAB_TRAVERSAL)
        self.SetClientSize(wx.Size(575, 369))

        self.sbSettings = wx.StaticBox(id=wxID_PANELSETTINGSSBSETTINGS,
              label='Settings', name='sbSettings', parent=self, pos=wx.Point(0,
              0), size=wx.Size(575, 388), style=0)

        self.pSettings = wx.Panel(id=wxID_PANELSETTINGSPSETTINGS,
              name='pSettings', parent=self, pos=wx.Point(5, 17),
              size=wx.Size(512, 366), style=wx.TAB_TRAVERSAL)

        self.btsPORTDL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTDL, label='IOD', name='btsPORTDL',
              parent=self.pSettings, pos=wx.Point(264, 244), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=12,
              tooltips=['Bit 0: PD[0] Value', 'Bit 1: PD[1] Value',
              'Bit 2: PD[2] Value', 'Bit 3: PD[3] Value', 'Bit 4: PD[4] Value',
              'Bit 5: PD[5] Value', 'Bit 6: PD[6] Value', 'Bit 7: PD[7] Value'],
              value=0)
        self.btsPORTDL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTDL)

        self.btsPORTDH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTDH, label='OED', name='btsPORTDH',
              parent=self.pSettings, pos=wx.Point(4, 244), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=12,
              tooltips=['Bit 0: PD[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PD[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PD[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PD[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PD[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PD[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PD[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PD[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btsPORTDH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTDH)

        self.btsWORDWIDEH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSWORDWIDEH, label='WORDWIDE',
              name='btsWORDWIDEH', parent=self.pSettings, pos=wx.Point(4, 24),
              showlabel=True, size=wx.Size(227, 18), style=wx.NO_BORDER,
              tagint=1, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsWORDWIDEH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSWORDWIDEH)

        self.btsWORDWIDEL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSWORDWIDEL, label='WORDWIDE',
              name='btsWORDWIDEL', parent=self.pSettings, pos=wx.Point(281, 24),
              showlabel=True, size=wx.Size(227, 18), style=wx.NO_BORDER,
              tagint=1,
              tooltips=['Bit 0: WORDWIDE Data Width\n    0 - 8-bits\n    1 - 16-bits',
              'Bit 1: Reserved', 'Bit 2: Reserved', 'Bit 3: Reserved',
              'Bit 4: Reserved', 'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: Reserved'], value=0)
        self.btsWORDWIDEL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSWORDWIDEL)

        self.btsPINFLAGSL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPINFLAGSL, label='PINFLAGSCD',
              name='btsPINFLAGSL', parent=self.pSettings, pos=wx.Point(264,
              304), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=15, tooltips=['Bit 0: FLAGC[0]', 'Bit 1: FLAGC[1]',
              'Bit 2: FLAGC[2]', 'Bit 3: FLAGC[3]', 'Bit 4: FLAGD[0]',
              'Bit 5: FLAGD[1]', 'Bit 6: FLAGD[2]', 'Bit 7: FLAGD[3]'],
              value=0)
        self.btsPINFLAGSL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPINFLAGSL)

        self.btsPINFLAGSH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPINFLAGSH, label='PINFLAGSAB',
              name='btsPINFLAGSH', parent=self.pSettings, pos=wx.Point(4, 304),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=15, tooltips=['Bit 0: FLAGA[0]', 'Bit 1: FLAGA[1]',
              'Bit 2: FLAGA[2]', 'Bit 3: FLAGA[3]', 'Bit 4: FLAGB[0]',
              'Bit 5: FLAGB[1]', 'Bit 6: FLAGB[2]', 'Bit 7: FLAGB[3]'],
              value=0)
        self.btsPINFLAGSH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPINFLAGSH)

        self.btsSPICONFIGH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSSPICONFIGH, label='PORTECFG',
              name='btsSPICONFIGH', parent=self.pSettings, pos=wx.Point(4, 124),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=6, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: GPIFA[8] - Enable GPIF Address Pins\n    0 - Configure PE[7] as GPIO\n    1 - Configure PE[7] as GPIFADR[8] output'],
              value=0)
        self.btsSPICONFIGH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSSPICONFIGH)

        self.btsSPICONFIGL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSSPICONFIGL, label='SPICONFIG',
              name='btsSPICONFIGL', parent=self.pSettings, pos=wx.Point(264,
              124), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=6,
              tooltips=['Bit 0: SPIENDIAN - SPI Bit Endianness\n    0 - LSBit to MSBit\n    1 - MSBit to LSBit',
              'Bit 1: SPICPOL - SPI Clock Polarity\n    0 - Normal\n    1 - Inverted',
              'Bit 2: SPICPHA - SPI Clock Phase\n    0 - Sample Then Clock\n    1 - Clock Then Sample',
              'Bit 3: SPIPORT - Select Port for SPI/FPGA communication\n    0 - Use Port E\n    1 - Use Port A',
              'Bit 4: NCEPIN - Select the pin for the nCE line\n    0 - nCE is on pin 2 of SPIPORT\n    1 - nCE is on pin 7 of SPIPORT',
              'Bit 5: MISOPIN - Select the port for the MISO line\n    0 - MISO is on pin 5 of SPIPORT\n    1 - MISO is on pin 2 of SPIPORT',
              'Bit 6: Reserved', 'Bit 7: Reserved'], value=0)
        self.btsSPICONFIGL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSSPICONFIGL)

        self.btsSLAVEFIFOFLAGSH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSH, label='SlaveFIFO',
              name='btsSLAVEFIFOFLAGSH', parent=self.pSettings, pos=wx.Point(4,
              144), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=7, tooltips=['Bit 0: EP6 Full Flag',
              'Bit 1: EP6 Empty Flag', 'Bit 2: Reserved',
              'Bit 3: RDY[0] - Pin Status', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsSLAVEFIFOFLAGSH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSH)

        self.btsSLAVEFIFOFLAGSL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSL, label='SlaveFIFO',
              name='btsSLAVEFIFOFLAGSL', parent=self.pSettings,
              pos=wx.Point(264, 144), showlabel=True, size=wx.Size(244, 18),
              style=wx.NO_BORDER, tagint=7, tooltips=['Bit 0: EP2 Full Flag',
              'Bit 1: EP2 Empty Flag', 'Bit 2: Reserved',
              'Bit 3: RDY[1] Pin Status', 'Bit 4: Reserved', 'Bit 5: Reserved',
              'Bit 6: Reserved', 'Bit 7: Reserved'], value=0)
        self.btsSLAVEFIFOFLAGSL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSSLAVEFIFOFLAGSL)

        self.btsCPUCONFIGL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSCPUCONFIGL, label='CPUCONFIG',
              name='btsCPUCONFIGL', parent=self.pSettings, pos=wx.Point(264,
              104), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=5, tooltips=['Bit 0: Reserved',
              'Bit 1: CLKOE - CLKOUT Output Enable\n    0 - Tri-state the CLKOUT Pin\n    1 - Drive the CLKOUT Pin',
              'Bit 2: CLKINV - Invert CLKOUT Pin\n    0 - Nomal\n    1 - Invert',
              'Bit 3: CLKSPD[0] - CPU Clock Speed\n    00 - 12 MHz\n    01 - 24 MHz\n    10 - 48 MHz\n    11=Reserved',
              'Bit 4: CLKSPD[1] - CPU Clock Speed\n    00 - 12 MHz\n    01 - 24 MHz\n    10 - 48 MHz\n    11=Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsCPUCONFIGL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSCPUCONFIGL)

        self.btsCPUCONFIGH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSCPUCONFIGH, label='BusSpeed',
              name='btsCPUCONFIGH', parent=self.pSettings, pos=wx.Point(4, 104),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=5, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: USB Bus Speed\n    0 - Force Full-Speed (12 Mbps)\n    1 - Allow High-Speed (480 Mbps)'],
              value=0)
        self.btsCPUCONFIGH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSCPUCONFIGH)

        self.btsPORTACCFGH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTACCFGH, label='PORTACFG',
              name='btsPORTACCFGH', parent=self.pSettings, pos=wx.Point(4, 284),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=14,
              tooltips=['Bit 0: INT0 - Interrupt 0 Alternate Configuration\n    0 - PA[0] not configured as interrupt\n    1 - PA[0] configured as interrupt input\nNOTE: INT0 is currently not used',
              'Bit 1: INT1 - Interrupt 1 Alternate Configuration\n    0 - PA[1] not configured as interrupt\n    1 - PA[1] configured as interrupt input\nNOTE: INT1 is currently not used',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved',
              'Bit 6: SLCS - Slave FIFO Chip Select Alternate Configuration\n    0 - PA[7] not configured as SLCS input in Slave Mode\n    1 - PA[7] confiured as SLCS input in Slave Mode\nNote: If both bit 15 (FLAGD) and bit 14 (SLCS) are set,\nPA[7] will be configured to give the FLAGD status',
              'Bit 7: FLAGD - Flag D Alternate Configuration\n    0 - PA[7] does not give FLAGD status in Slave Mode\n    1 - PA[7] gives FLAGD status when in Slave Mode\nNote: If both bit 15 (FLAGD) and bit 14 (SLCS) are set,\nPA[7] will be configured to give the FLAGD status'],
              value=0)
        self.btsPORTACCFGH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTACCFGH)

        self.btsPORTACCFGL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTACCFGL, label='PORTCCFG',
              name='btsPORTACCFGL', parent=self.pSettings, pos=wx.Point(264,
              284), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=14,
              tooltips=['Bit 0: GPIFA[0] - Enable GPIF Address Pins\n    0 - Configure PC[0] as GPIO\n    1 - Configure PC[0] as GPIFADR[7] output',
              'Bit 1: GPIFA[1] - Enable GPIF Address Pins\n    0 - Configure PC[1] as GPIO\n    1 - Configure PC[1] as GPIFADR[7] output',
              'Bit 2: GPIFA[2] - Enable GPIF Address Pins\n    0 - Configure PC[2] as GPIO\n    1 - Configure PC[2] as GPIFADR[7] output',
              'Bit 3: GPIFA[3] - Enable GPIF Address Pins\n    0 - Configure PC[3] as GPIO\n    1 - Configure PC[3] as GPIFADR[7] output',
              'Bit 4: GPIFA[4] - Enable GPIF Address Pins\n    0 - Configure PC[4] as GPIO\n    1 - Configure PC[4] as GPIFADR[7] output',
              'Bit 5: GPIFA[5] - Enable GPIF Address Pins\n    0 - Configure PC[5] as GPIO\n    1 - Configure PC[5] as GPIFADR[7] output',
              'Bit 6: GPIFA[6] - Enable GPIF Address Pins\n    0 - Configure PC[6] as GPIO\n    1 - Configure PC[6] as GPIFADR[7] output',
              'Bit 7: GPIFA[7] - Enable GPIF Address Pins\n    0 - Configure PC[7] as GPIO\n    1 - Configure PC[7] as GPIFADR[7] output'],
              value=0)
        self.btsPORTACCFGL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTACCFGL)

        self.btsPORTAH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTAH, label='OEA', name='btsPORTAH',
              parent=self.pSettings, pos=wx.Point(4, 184), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=9,
              tooltips=['Bit 0: PA[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PA[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PA[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PA[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PA[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PA[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PA[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PA[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btsPORTAH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTAH)

        self.btsPORTAL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTAL, label='IOA', name='btsPORTAL',
              parent=self.pSettings, pos=wx.Point(264, 184), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=9,
              tooltips=['Bit 0: PA[0] Value', 'Bit 1: PA[1] Value',
              'Bit 2: PA[2] Value', 'Bit 3: PA[3] Value', 'Bit 4: PA[4] Value',
              'Bit 5: PA[5] Value', 'Bit 6: PA[6] Value', 'Bit 7: PA[7] Value'],
              value=0)
        self.btsPORTAL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTAL)

        self.btsPORTCH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTCH, label='OEC', name='btsPORTCH',
              parent=self.pSettings, pos=wx.Point(4, 224), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=11,
              tooltips=['Bit 0: PC[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PC[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PC[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PC[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PC[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PC[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PC[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PC[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btsPORTCH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTCH)

        self.btsPORTEL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTEL, label='IOE', name='btsPORTEL',
              parent=self.pSettings, pos=wx.Point(264, 264), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=13,
              tooltips=['Bit 0: PE[0] Value', 'Bit 1: PE[1] Value',
              'Bit 2: PE[2] Value', 'Bit 3: PE[3] Value', 'Bit 4: PE[4] Value',
              'Bit 5: PE[5] Value', 'Bit 6: PE[6] Value', 'Bit 7: PE[7] Value'],
              value=0)
        self.btsPORTEL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTEL)

        self.btsPORTCL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTCL, label='IOC', name='btsPORTCL',
              parent=self.pSettings, pos=wx.Point(264, 224), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=11,
              tooltips=['Bit 0: PC[0] Value', 'Bit 1: PC[1] Value',
              'Bit 2: PC[2] Value', 'Bit 3: PC[3] Value', 'Bit 4: PC[4] Value',
              'Bit 5: PC[5] Value', 'Bit 6: PC[6] Value', 'Bit 7: PC[7] Value'],
              value=0)
        self.btsPORTCL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTCL)

        self.btsPORTEH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTEH, label='OEE', name='btsPORTEH',
              parent=self.pSettings, pos=wx.Point(4, 264), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=13,
              tooltips=['Bit 0: PE[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PE[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PE[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PE[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PE[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PE[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PE[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PE[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btsPORTEH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTEH)

        self.btsVERSIONSPEEDL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSVERSIONSPEEDL, label='VERSIONSPEED',
              name='btsVERSIONSPEEDL', parent=self.pSettings, pos=wx.Point(264,
              344), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=17, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: USB Bus Speed\n    0 - Full-Speed (12 Mbps)\n    1 - High-Speed (480 Mbps)'],
              value=0)
        self.btsVERSIONSPEEDL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSVERSIONSPEEDL)
        self.btsVERSIONSPEEDL.SetMinSize((-1, -1))

        self.btsDATAADDRESSH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSDATAADDRESSH, label='DataAddress',
              name='btsDATAADDRESSH', parent=self.pSettings, pos=wx.Point(4,
              44), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=2, tooltips=['Bit 0: GPIFADR[8]', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved',
              'Bit 6: Disable Address Bus\n    0 - Enable address bus\n    1 - Disable address bus',
              "Bit 7: Disable Auto-Increment of Address Bus\n    0 - Auto-increment address bus\n    1 - Don't auto-increment address bus"],
              value=0)
        self.btsDATAADDRESSH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSDATAADDRESSH)

        self.btsVERSIONSPEEDH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSVERSIONSPEEDH, label='VERSIONSPEED',
              name='btsVERSIONSPEEDH', parent=self.pSettings, pos=wx.Point(4,
              344), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=17,
              tooltips=['Bit 0: HARDWAREREV[0]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 1: HARDWAREREV[1]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 2: HARDWAREREV[2]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 3: HARDWAREREV[3]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 4: HARDWAREREV[4]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 5: HARDWAREREV[5]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 6: HARDWAREREV[6]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E',
              'Bit 7: HARDWAREREV[7]\n    00000000 - CYC68013 Rev A/B\n    00000001 - CYC68013A Rev A\n    00000010 - CYC68013 Rev C/D\n    00000100 - CYC68013 Rev E'],
              value=0)
        self.btsVERSIONSPEEDH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSVERSIONSPEEDH)
        self.btsVERSIONSPEEDH.SetMinSize((-1, -1))

        self.btsDATAADDRESSL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSDATAADDRESSL, label='DataAddress',
              name='btsDATAADDRESSL', parent=self.pSettings, pos=wx.Point(264,
              44), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=2, tooltips=['Bit 0: GPIFADR[0] - GPIF Adddress Bit 0',
              'Bit 1: GPIFADR[1] - GPIF Adddress Bit 1',
              'Bit 2: GPIFADR[2] - GPIF Adddress Bit 2',
              'Bit 3: GPIFADR[3] - GPIF Adddress Bit 3',
              'Bit 4: GPIFADR[4] - GPIF Adddress Bit 4',
              'Bit 5: GPIFADR[5] - GPIF Adddress Bit 5',
              'Bit 6: GPIFADR[6] - GPIF Adddress Bit 6',
              'Bit 7: GPIFADR[7] - GPIF Adddress Bit 7'], value=0)
        self.btsDATAADDRESSL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSDATAADDRESSL)

        self.btsFIFOCONFIGH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSFIFOCONFIGH, label='FifoPinPolar',
              name='btsFIFOCONFIGH', parent=self.pSettings, pos=wx.Point(4, 64),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=3,
              tooltips=['Bit 0: FF - FIFO Full Flag Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 1: EF - FIFO Empty Flag Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 2: SLWR - FIFO Write Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 3: SLRD - FIFO Read Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 4: SLOE - FIFO Output Enable Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 5: PKTEND - FIFO Packet End Polarity\n    0 - Active low\n    1 - Active high',
              'Bit 6: Reserved', 'Bit 7: Reserved'], value=0)
        self.btsFIFOCONFIGH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSFIFOCONFIGH)

        self.btsFIFOCONFIGL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSFIFOCONFIGL, label='IFCONFIG',
              name='btsFIFOCONFIGL', parent=self.pSettings, pos=wx.Point(264,
              64), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=3,
              tooltips=['Bit 0: IFCFG[0] - HSPP Configuration Bit 0\n    00 - IO ports\n    01 - Reserved\n    10 - GPIF master mode\n    11 - Slave FIFO mode',
              'Bit 1: IFCFG[1] - HSPP Configuration Bit 1\n    00 - IO ports\n    01 - Reserved\n    10 - GPIF master mode\n    11 - Slave FIFO mode',
              'Bit 2: Reserved',
              'Bit 3: ASYNC - GPIF Clock Mode Select\n    0 - Synchronous\n    1 - Asynchronous',
              'Bit 4: IFCLKPOL - IFCLK Polarity Select\n    0 - Normal\n    1 - Inverted',
              'Bit 5: IFCLKOE - IFCLK Output Enable\n    0 - Tri-state the IFCLK pin\n    1 - Drive the IFCLK pin',
              'Bit 6: 3048MHZ - IFCLK Speed Select\n    0 - 30 MHz\n    1 - 48 MHz',
              'Bit 7: IFCLKSRC - IFCLK Source\n    0 - External\n    1 - Internal'],
              value=0)
        self.btsFIFOCONFIGL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSFIFOCONFIGL)

        self.btsI2CTLH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSI2CTLH, label='I2CTL', name='btsI2CTLH',
              parent=self.pSettings, pos=wx.Point(4, 164), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=8,
              tooltips=['Bit 0: Last I2C Read Status Bit 0\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 1: Last I2C Read Status Bit 1\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 2: Last I2C Read Status Bit 2\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 3: Last I2C Read Status Bit 3\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 4: Last I2C Read Status Bit 4\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 5: Last I2C Read Status Bit 5\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 6: Last I2C Read Status Bit 6\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout',
              'Bit 7: Last I2C Read Status Bit 7\n    00000110 - Bus error\n    00000111 - No ACK\n    00001000 - Normal completion\n    00001010 - Slave wait\n    00001011 - Timeout'],
              value=0)
        self.btsI2CTLH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSI2CTLH)

        self.btsFPGATYPEL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSFPGATYPEL, label='FPGATYPE',
              name='btsFPGATYPEL', parent=self.pSettings, pos=wx.Point(264, 84),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=4,
              tooltips=['Bit 0: FPGATYPE\n    0 - Altera (Passive Serial)\n    1 - Xilinx (Slave Serial)',
              'Bit 1: Reserved', 'Bit 2: Reserved', 'Bit 3: Reserved',
              'Bit 4: Reserved', 'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: Reserved'], value=0)
        self.btsFPGATYPEL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSFPGATYPEL)

        self.btsEP26CONFIGH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSEP26CONFIGH, label='EP2CFG',
              name='btsEP26CONFIGH', parent=self.pSettings, pos=wx.Point(4, 4),
              showlabel=True, size=wx.Size(206, 18), style=wx.NO_BORDER,
              tagint=0,
              tooltips=['Bit 0: EP2 Buffering[0]\n    00 - Quad\n    01 - Invalid\n    10 - Double\n    11 - Triple',
              'Bit 1: EP2 Buffering[1]\n    00 - Quad\n    01 - Invalid\n    10 - Double\n    11 - Triple',
              'Bit 2: Reserved',
              'Bit 3: EP2 Buffer Size\n    0 - 512 bytes\n    1 - 1024 bytes',
              'Bit 4: EP2 Type[0]\n    00 - Invalid\n    01 - Isochronous\n    10 - Bulk\n    11 - Interrupt',
              'Bit 5: EP2 Type[0]\n    00 - Invalid\n    01 - Isochronous\n    10 - Bulk\n    11 - Interrupt',
              'Bit 6: EP2 Direction\n    0 - Output\n    1 - Input',
              'Bit 7: EP2 Valid\n    0 - Not activated\n    1 - Activated'],
              value=0)
        self.btsEP26CONFIGH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSEP26CONFIGH)

        self.btsVERSIONBUILDL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSVERSIONBUILDL, label='VERSIONBUILD',
              name='btsVERSIONBUILDL', parent=self.pSettings, pos=wx.Point(264,
              324), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=16, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsVERSIONBUILDL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSVERSIONBUILDL)

        self.btsI2CTLL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSI2CTLL, label='I2CTL', name='btsI2CTLL',
              parent=self.pSettings, pos=wx.Point(264, 164), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=8,
              tooltips=['Bit 0: I2C Bus Clock Speed\n    0 - Appoximately 100 kHz\n    1 - Appoximately 400 kHz',
              'Bit 1: Reserved', 'Bit 2: Reserved', 'Bit 3: Reserved',
              'Bit 4: Reserved', 'Bit 5: Reserved', 'Bit 6: Reserved',
              'Bit 7: IgnoreACK\n    0 - Handle ACK for normal I2C traffic\n    1 - Process I2C traffic regardless of ACK'],
              value=0)
        self.btsI2CTLL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSI2CTLL)

        self.btsFPGATYPEH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSFPGATYPEH, label='FPGATYPE',
              name='btsFPGATYPEH', parent=self.pSettings, pos=wx.Point(4, 84),
              showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=4, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsFPGATYPEH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSFPGATYPEH)

        self.btsEP26CONFIGL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSEP26CONFIGL, label='EP6CFG',
              name='btsEP26CONFIGL', parent=self.pSettings, pos=wx.Point(302,
              4), showlabel=True, size=wx.Size(206, 18), style=wx.NO_BORDER,
              tagint=0,
              tooltips=['Bit 0: EP6 Buffering[0]\n    00 - Quad\n    01 - Invalid\n    10 - Double\n    11 - Triple',
              'Bit 1: EP6 Buffering[1]\n    00 - Quad\n    01 - Invalid\n    10 - Double\n    11 - Triple',
              'Bit 2: Reserved',
              'Bit 3: EP6 Buffer Size\n    0 - 512 bytes\n    1 - 1024 bytes',
              'Bit 4: EP6 Type[0]\n    00 - Invalid\n    01 - Isochronous\n    10 - Bulk\n    11 - Interrupt',
              'Bit 5: EP6 Type[0]\n    00 - Invalid\n    01 - Isochronous\n    10 - Bulk\n    11 - Interrupt',
              'Bit 6: EP6 Direction\n    0 - Output\n    1 - Input',
              'Bit 7: EP6 Valid\n    0 - Not activated\n    1 - Activated'],
              value=0)
        self.btsEP26CONFIGL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSEP26CONFIGL)

        self.btsVERSIONBUILDH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSVERSIONBUILDH, label='VERSIONBUILD',
              name='btsVERSIONBUILDH', parent=self.pSettings, pos=wx.Point(4,
              324), showlabel=True, size=wx.Size(244, 18), style=wx.NO_BORDER,
              tagint=16, tooltips=['Bit 0: Reserved', 'Bit 1: Reserved',
              'Bit 2: Reserved', 'Bit 3: Reserved', 'Bit 4: Reserved',
              'Bit 5: Reserved', 'Bit 6: Reserved', 'Bit 7: Reserved'],
              value=0)
        self.btsVERSIONBUILDH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSVERSIONBUILDH)

        self.btsPORTBL = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTBL, label='IOB', name='btsPORTBL',
              parent=self.pSettings, pos=wx.Point(264, 204), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=10,
              tooltips=['Bit 0: PB[0] Value', 'Bit 1: PB[1] Value',
              'Bit 2: PB[2] Value', 'Bit 3: PB[3] Value', 'Bit 4: PB[4] Value',
              'Bit 5: PB[5] Value', 'Bit 6: PB[6] Value', 'Bit 7: PB[7] Value'],
              value=0)
        self.btsPORTBL.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTBL)

        self.btsPORTBH = BitTwiddler(bits=8, bitspacing=4,
              id=wxID_PANELSETTINGSBTSPORTBH, label='OEB', name='btsPORTBH',
              parent=self.pSettings, pos=wx.Point(4, 204), showlabel=True,
              size=wx.Size(244, 18), style=wx.NO_BORDER, tagint=10,
              tooltips=['Bit 0: PB[0] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 1: PB[1] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 2: PB[2] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 3: PB[3] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 4: PB[4] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 5: PB[5] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 6: PB[6] Output Enable\n    0 - Input\n    1 - Output',
              'Bit 7: PB[7] Output Enable\n    0 - Input\n    1 - Output'],
              value=0)
        self.btsPORTBH.Bind(wx.EVT_CHECKBOX, self.OnBtsCheckbox,
              id=wxID_PANELSETTINGSBTSPORTBH)

        self._init_sizers()

    def __init__(self, parent, id, pos, size, style, name):
#-------------------------------------------------------------------------------
        self._init_ctrls(parent)
        
        # By default, this panel is used to access QuickUSB settings (instead of
        # defaults)
        self._useAsSettings = True



    def UseAsDefaults(self, defaults=True):
        """
        Method to set the use of this panel to defaults instead of settings
        """
        self._useAsSettings = not defaults
        self.sbSettings.SetLabel('Defaults')
 
 
 
    def SetControlMinSizes(self):
        """
        This method sets the minimum size of all the BitTwiddler controls on the
        panel based on the size of the largest BitTwiddler.  This ensures that
        all of the BT's line up correctly as they do not play nicely with sizers
        quite yet.
        """
        wH, wL = 0, 0
        h = 0
        for settingH, settingL, setting in self.GetSettings():
            wsH, hsH = settingH.GetSize()
            wsL, hsL = settingL.GetSize()
            wH = max(wH, wsH)
            wL = max(wL, wsL)
            h = max(h, hsH, hsL)
        
        for settingH, settingL, setting in self.GetSettings():
            settingH.SetMinSize((wH, h))
            settingL.SetMinSize((wL, h))
            self.pSettings.Layout()
            
            
            
    def GetSettings(self):
        """
        Return an iterable list of 3-element tuples of all the settings.  The
        list has the form:
            [(BitTwiddlerObjH, BitTwiddlerObjL, QuickUsb.Setting), ...]
        """
        settings = self.GetDefaults()
        settings.extend([(self.btsVERSIONBUILDH, self.btsVERSIONBUILDL, Setting.VersionBuild),
                         (self.btsVERSIONSPEEDH, self.btsVERSIONSPEEDL, Setting.VersionSpeed)])
        return settings
                    
    
    
    def GetDefaults(self):
        """
        Return an iterable list of 3-element tuples of all the defaults.  The
        list has the form:
            [(BitTwiddlerObjH, BitTwiddlerObjL, QuickUsb.Setting), ...]
        """
        return [(self.btsEP26CONFIGH, self.btsEP26CONFIGL, Setting.Ep26Config), 
                (self.btsWORDWIDEH, self.btsWORDWIDEL, Setting.WordWide),
                (self.btsDATAADDRESSH, self.btsDATAADDRESSL, Setting.DataAddress),
                (self.btsFIFOCONFIGH, self.btsFIFOCONFIGL, Setting.FifoConfig),
                (self.btsFPGATYPEH, self.btsFPGATYPEL, Setting.FpgaType),
                (self.btsCPUCONFIGH, self.btsCPUCONFIGL, Setting.CpuConfig),
                (self.btsSPICONFIGH, self.btsSPICONFIGL, Setting.SpiConfig),
                (self.btsSLAVEFIFOFLAGSH, self.btsSLAVEFIFOFLAGSL, Setting.SlaveFifoFlags),
                (self.btsI2CTLH, self.btsI2CTLL, Setting.I2Ctl),
                (self.btsPORTAH, self.btsPORTAL, Setting.PortA),
                (self.btsPORTBH, self.btsPORTBL, Setting.PortB),
                (self.btsPORTCH, self.btsPORTCL, Setting.PortC),
                (self.btsPORTDH, self.btsPORTDL, Setting.PortD),
                (self.btsPORTEH, self.btsPORTEL, Setting.PortE),
                (self.btsPORTACCFGH, self.btsPORTACCFGL, Setting.PortACCfg),
                (self.btsPINFLAGSH, self.btsPINFLAGSL, Setting.PinFlags)]
              
              
                
    def UpdateCtrlsEnable(self, ena):
        """
        Called by frameMain when we need to update which controls are enabled 
        and disabled
        """
        self.btsVERSIONBUILDH.Enable(ena and self._useAsSettings)
        self.btsVERSIONBUILDL.Enable(ena and self._useAsSettings)
        
        self.btsVERSIONSPEEDH.Enable(ena and self._useAsSettings)
        self.btsVERSIONSPEEDL.Enable(ena and self._useAsSettings)

        self.btsSLAVEFIFOFLAGSH.Enable(ena and self._useAsSettings)
        self.btsSLAVEFIFOFLAGSL.Enable(ena and self._useAsSettings)
        
        
        
    def UpdateCtrls(self, qusb):
        """
        Called by frameMain when we need to update the values within our 
        controls
        """
        if self._useAsSettings:
            self.TheFrame.SetStatus('Reading settings...')
            settings = self.GetSettings()
        else:
            self.TheFrame.SetStatus('Reading defaults...')
            settings = self.GetDefaults()
        
        # Iterate through all settings/defaults
        for valueCtrlH, valueCtrlL, setting in settings:
            if self._useAsSettings:
                # Read the setting
                (ok, value) = qusb.ReadSetting(setting)
                if not ok:
                    self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
                    return
            else:
                # Read the default
                (ok, value) = qusb.ReadDefault(setting)
                if not ok:
                    self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
                    return
            
            # Extract the MSB and LSB
            valueH = (value >> 8) & 0xff
            valueL = (value & 0xff)
            
            # Update the BitTwiddler controls
            valueCtrlH.SetValue(valueH)
            valueCtrlL.SetValue(valueL)
       
        
        
        
    def PeriodicUpdateCtrls(self, qusb):
        """
        Called fy frameMain when we are to refresh our controls with values
        read off the QuickUSB Module
        """
        if self._useAsSettings:
            self.UpdateCtrls(qusb)

    
    
    def LoadConf(self, conf):
        """Called by frameMain when we are to load our configuration settings"""
        pass
            
            
            
    def SaveConf(self):
        """Called by frameMain when we are to save our configuration settings"""
        return {}



    def OnBtsCheckbox(self, event):
        """
        Event handler for all of the BitTwiddler controls
        """
        # Ensure that a valid module is selected
        qusb = self.TheFrame.GetSelectedModule()
        if not qusb:
            return

        # The event object holds a reference to the actual BitTwiddler control
        # that was checked
        obj = event.GetEventObject()
        
        # The tag int holds the QuickUsb.Setting value for the BT, and the value
        # holds the new value of the BT object
        TagInt, Value = obj.GetTagInt(), obj.GetValue()
        
        # Locate the setting that was clicked
        if self._useAsSettings:
            settings = self.GetSettings()
        else:
            settings = self.GetDefaults()
            
        # Iterate over all settings/defaults to locate the BT that was clicked
        for valueCtrlH, valueCtrlL, setting in settings:
            # Check if this was our BT
            if (valueCtrlH is obj) or (valueCtrlL is obj):
                # Create the setting word from the MSB and LSB bytes
                valueH, valueL = valueCtrlH.GetValue(), valueCtrlL.GetValue()
                value = (valueH << 8) | valueL
                
                # Write the setting/default
                if self._useAsSettings:
                    (ok,) = qusb.WriteSetting(setting, value)
                    if not ok:
                        self.TheFrame.SetStatus("WriteSetting() failed with error: " + str(Error(qusb.LastError())))
                        return
                else:
                    (ok,) = qusb.WriteDefault(setting, value)
                    if not ok:
                        self.TheFrame.SetStatus("WriteDefault() failed with error: " + str(Error(qusb.LastError())))
                        return
                
                # If our global setting says we should perform reads after
                # writes
                if self.TheFrame._performReadBack:
                    # Wait a little before reading back the setting to make sure
                    # that is took
                    time.sleep(0.01)
                
                    # Read the setting back
                    if self._useAsSettings:
                        (ok, value) = qusb.ReadSetting(setting)
                        if not ok:
                            self.TheFrame.SetStatus("ReadSetting() failed with error: " + str(Error(qusb.LastError())))
                            return
                    else:
                        (ok, value) = qusb.ReadDefault(setting)
                        if not ok:
                            self.TheFrame.SetStatus("ReadDefault() failed with error: " + str(Error(qusb.LastError())))
                            return
                        
                    # Extract the MSB and LSB
                    valueH = (value >> 8) & 0xff
                    valueL = (value & 0xff)
                    
                    # Update the BitTwiddler controls
                    valueCtrlH.SetValue(valueH)
                    valueCtrlL.SetValue(valueL)
                    
                break
