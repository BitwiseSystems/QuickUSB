#Boa:Frame:frameMain

import wx
import time
import sys
import ctypes
import QuickUsb
from wx.lib.anchors import LayoutAnchors

def create(parent):
    return frameMain(parent)

[wxID_FRAMEMAIN, wxID_FRAMEMAINBSTART, wxID_FRAMEMAINBSTOP,
 wxID_FRAMEMAINCBPACKETSIZEUNIT, wxID_FRAMEMAINGTHROUGHPUT, 
 wxID_FRAMEMAINPANEL1, wxID_FRAMEMAINPANEL2, wxID_FRAMEMAINPANEL3, 
 wxID_FRAMEMAINRBASYNC, wxID_FRAMEMAINRBREAD, wxID_FRAMEMAINRBSYNC, 
 wxID_FRAMEMAINRBWRITE, wxID_FRAMEMAINSBCONFIGURATION, 
 wxID_FRAMEMAINSBTHROUGHPUT, wxID_FRAMEMAINSCPACKETSIZE, 
 wxID_FRAMEMAINSTPACKETSIZE, wxID_FRAMEMAINSTTHROUGHPUT, 
] = [wx.NewId() for _init_ctrls in range(17)]

[wxID_FRAMEMAINTIMER1] = [wx.NewId() for _init_utils in range(1)]

class frameMain(wx.Frame):
    def _init_coll_szgConfigurationB_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbConfigurationRadioA, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szbConfigurationRadioB, 0, border=0, flag=wx.GROW)

    def _init_coll_szbConfigurationA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stPacketSize, 0, border=0, flag=0)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.scPacketSize, 2, border=0,
              flag=wx.RIGHT | wx.LEFT)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.cbPacketSizeUnit, 3, border=0,
              flag=wx.RIGHT | wx.LEFT)

    def _init_coll_szbConfigurationRadioB_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.panel3, 0, border=0, flag=wx.GROW)

    def _init_coll_szbMain_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szsbConfiguration, 0, border=0, flag=wx.GROW)
        parent.AddSizer(self.szsbThroughputMeter, 0, border=0, flag=wx.GROW)

    def _init_coll_szbThroughputRadioA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.rbSync, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.rbAsync, 0, border=0, flag=wx.GROW)

    def _init_coll_szbConfigurationRadioA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.panel2, 0, border=0, flag=wx.GROW)

    def _init_coll_szsbThroughputMeter_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.gThroughput, 0, border=0, flag=wx.GROW)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddSizer(self.szfgThroughputMeterA, 0, border=0, flag=wx.GROW)

    def _init_coll_szbThroughputRadioB_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.rbRead, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.rbWrite, 0, border=0, flag=wx.GROW)

    def _init_coll_szfgThroughputMeterA_Growables(self, parent):
        # generated method, don't edit

        parent.AddGrowableCol(0)

    def _init_coll_szsbConfiguration_Items(self, parent):
        # generated method, don't edit

        parent.AddSizer(self.szbConfigurationA, 0, border=0, flag=wx.GROW)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddSizer(self.szgConfigurationB, 0, border=0, flag=wx.GROW)

    def _init_coll_szfgThroughputMeterA_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.stThroughput, 0, border=0,
              flag=wx.GROW | wx.ALIGN_CENTER_VERTICAL)
        parent.AddWindow(self.bStart, 0, border=0, flag=0)
        parent.AddSpacer(wx.Size(8, 8), border=0, flag=0)
        parent.AddWindow(self.bStop, 0, border=0, flag=0)

    def _init_utils(self):
        # generated method, don't edit
        self.timer1 = wx.Timer(id=wxID_FRAMEMAINTIMER1, owner=self)
        self.Bind(wx.EVT_TIMER, self.OnTimer1Timer, id=wxID_FRAMEMAINTIMER1)

    def _init_sizers(self):
        # generated method, don't edit
        self.szbMain = wx.BoxSizer(orient=wx.VERTICAL)

        self.szsbConfiguration = wx.StaticBoxSizer(box=self.sbConfiguration,
              orient=wx.VERTICAL)

        self.szsbThroughputMeter = wx.StaticBoxSizer(box=self.sbThroughput,
              orient=wx.VERTICAL)

        self.szbConfigurationA = wx.BoxSizer(orient=wx.HORIZONTAL)

        self.szbConfigurationRadioA = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbConfigurationRadioB = wx.BoxSizer(orient=wx.VERTICAL)

        self.szgConfigurationB = wx.GridSizer(cols=0, hgap=0, rows=1, vgap=0)

        self.szfgThroughputMeterA = wx.FlexGridSizer(cols=0, hgap=0, rows=1,
              vgap=0)
        self.szfgThroughputMeterA.SetFlexibleDirection(wx.BOTH)

        self.szbThroughputRadioA = wx.BoxSizer(orient=wx.VERTICAL)

        self.szbThroughputRadioB = wx.BoxSizer(orient=wx.VERTICAL)

        self._init_coll_szbMain_Items(self.szbMain)
        self._init_coll_szsbConfiguration_Items(self.szsbConfiguration)
        self._init_coll_szsbThroughputMeter_Items(self.szsbThroughputMeter)
        self._init_coll_szbConfigurationA_Items(self.szbConfigurationA)
        self._init_coll_szbConfigurationRadioA_Items(self.szbConfigurationRadioA)
        self._init_coll_szbConfigurationRadioB_Items(self.szbConfigurationRadioB)
        self._init_coll_szgConfigurationB_Items(self.szgConfigurationB)
        self._init_coll_szfgThroughputMeterA_Items(self.szfgThroughputMeterA)
        self._init_coll_szfgThroughputMeterA_Growables(self.szfgThroughputMeterA)
        self._init_coll_szbThroughputRadioA_Items(self.szbThroughputRadioA)
        self._init_coll_szbThroughputRadioB_Items(self.szbThroughputRadioB)

        self.panel1.SetSizer(self.szbMain)
        self.panel2.SetSizer(self.szbThroughputRadioA)
        self.panel3.SetSizer(self.szbThroughputRadioB)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Frame.__init__(self, id=wxID_FRAMEMAIN, name='frameMain',
              parent=prnt, pos=wx.Point(557, 303), size=wx.Size(339, 196),
              style=wx.DEFAULT_FRAME_STYLE,
              title='QuickUSB Throughput Test v2.15.0')
        self._init_utils()
        self.SetClientSize(wx.Size(323, 158))
        self.Bind(wx.EVT_CLOSE, self.OnFrameMainClose)

        self.panel1 = wx.Panel(id=wxID_FRAMEMAINPANEL1, name='panel1',
              parent=self, pos=wx.Point(0, 0), size=wx.Size(323, 158),
              style=wx.TAB_TRAVERSAL)

        self.sbConfiguration = wx.StaticBox(id=wxID_FRAMEMAINSBCONFIGURATION,
              label='Configuration', name='sbConfiguration', parent=self.panel1,
              pos=wx.Point(0, 0), size=wx.Size(323, 77), style=0)

        self.sbThroughput = wx.StaticBox(id=wxID_FRAMEMAINSBTHROUGHPUT,
              label='Throughput', name='sbThroughput', parent=self.panel1,
              pos=wx.Point(0, 77), size=wx.Size(323, 81), style=0)

        self.stPacketSize = wx.StaticText(id=wxID_FRAMEMAINSTPACKETSIZE,
              label='Packet Size:', name='stPacketSize', parent=self.panel1,
              pos=wx.Point(5, 17), size=wx.Size(58, 13), style=0)

        self.scPacketSize = wx.SpinCtrl(id=wxID_FRAMEMAINSCPACKETSIZE,
              initial=64, max=16777216, min=1, name='scPacketSize',
              parent=self.panel1, pos=wx.Point(71, 17), size=wx.Size(95, 21),
              style=wx.SP_ARROW_KEYS)
        self.scPacketSize.SetMinSize(wx.Size(500, 21))
        self.scPacketSize.SetHelpText('')
        self.scPacketSize.Bind(wx.EVT_SPIN, self.OnScPacketSizeSpin,
              id=wxID_FRAMEMAINSCPACKETSIZE)
        self.scPacketSize.Bind(wx.EVT_TEXT, self.OnScPacketSizeText,
              id=wxID_FRAMEMAINSCPACKETSIZE)

        self.cbPacketSizeUnit = wx.ComboBox(choices=['Bytes', 'Kilobytes',
              'Megabytes'], id=wxID_FRAMEMAINCBPACKETSIZEUNIT,
              name='cbPacketSizeUnit', parent=self.panel1, pos=wx.Point(174,
              17), size=wx.Size(143, 21), style=wx.CB_SORT | wx.CB_READONLY,
              value='sdf\nsdf')
        self.cbPacketSizeUnit.SetLabel('')
        self.cbPacketSizeUnit.Bind(wx.EVT_COMBOBOX,
              self.OnCbPacketSizeUnitCombobox,
              id=wxID_FRAMEMAINCBPACKETSIZEUNIT)

        self.gThroughput = wx.Gauge(id=wxID_FRAMEMAINGTHROUGHPUT,
              name='gThroughput', parent=self.panel1, pos=wx.Point(5, 94),
              range=35, size=wx.Size(313, 28), style=wx.GA_HORIZONTAL)

        self.stThroughput = wx.StaticText(id=wxID_FRAMEMAINSTTHROUGHPUT,
              label='Throughput: 0.00 MB/s', name='stThroughput',
              parent=self.panel1, pos=wx.Point(5, 130), size=wx.Size(155, 23),
              style=0)

        self.bStart = wx.Button(id=wxID_FRAMEMAINBSTART, label='Start',
              name='bStart', parent=self.panel1, pos=wx.Point(160, 130),
              size=wx.Size(75, 23), style=0)
        self.bStart.Bind(wx.EVT_BUTTON, self.OnBStartButton,
              id=wxID_FRAMEMAINBSTART)

        self.bStop = wx.Button(id=wxID_FRAMEMAINBSTOP, label='Stop',
              name='bStop', parent=self.panel1, pos=wx.Point(243, 130),
              size=wx.Size(75, 23), style=0)
        self.bStop.Enable(False)
        self.bStop.Bind(wx.EVT_BUTTON, self.OnBStopButton,
              id=wxID_FRAMEMAINBSTOP)

        self.panel2 = wx.Panel(id=wxID_FRAMEMAINPANEL2, name='panel2',
              parent=self.panel1, pos=wx.Point(5, 46), size=wx.Size(156, 26),
              style=wx.TAB_TRAVERSAL)

        self.panel3 = wx.Panel(id=wxID_FRAMEMAINPANEL3, name='panel3',
              parent=self.panel1, pos=wx.Point(161, 46), size=wx.Size(156, 26),
              style=wx.TAB_TRAVERSAL)

        self.rbSync = wx.RadioButton(id=wxID_FRAMEMAINRBSYNC,
              label='Synchronous', name='rbSync', parent=self.panel2,
              pos=wx.Point(0, 0), size=wx.Size(156, 13), style=0)
        self.rbSync.SetValue(True)

        self.rbAsync = wx.RadioButton(id=wxID_FRAMEMAINRBASYNC,
              label='Asynchrounous', name='rbAsync', parent=self.panel2,
              pos=wx.Point(0, 13), size=wx.Size(156, 13), style=0)
        self.rbAsync.SetValue(True)
        self.rbAsync.Enable(False)

        self.rbRead = wx.RadioButton(id=wxID_FRAMEMAINRBREAD, label='Read',
              name='rbRead', parent=self.panel3, pos=wx.Point(0, 0),
              size=wx.Size(156, 13), style=0)
        self.rbRead.SetValue(True)
        self.rbRead.Bind(wx.EVT_RADIOBUTTON, self.OnRbReadRadiobutton,
              id=wxID_FRAMEMAINRBREAD)

        self.rbWrite = wx.RadioButton(id=wxID_FRAMEMAINRBWRITE, label='Write',
              name='rbWrite', parent=self.panel3, pos=wx.Point(0, 13),
              size=wx.Size(156, 13), style=0)
        self.rbWrite.SetValue(True)
        self.rbWrite.Bind(wx.EVT_RADIOBUTTON, self.OnRbWriteRadiobutton,
              id=wxID_FRAMEMAINRBWRITE)

        self._init_sizers()

    def __init__(self, parent):
        self._init_ctrls(parent)
        
        self.thread = None
        self.quit = False
        self.throughput = 0.0
        self.testPacketSize = 0
        self.testRead = True
        
        self.cbPacketSizeUnit.SetSelection(1)
        self.rbSync.SetValue(True)
        self.rbRead.SetValue(True)

    def ClampPacketSize(self):
        if (self.scPacketSize.GetValue() > 16) and (self.cbPacketSizeUnit.GetSelection() == 2):
            self.scPacketSize.SetValue(16)
        if (self.scPacketSize.GetValue() > (16 * 1024)) and (self.cbPacketSizeUnit.GetSelection() == 1):
            self.scPacketSize.SetValue(16 * 1024)
        self.testPacketSize = int(self.scPacketSize.GetValue()) * (1024**self.cbPacketSizeUnit.GetSelection())

    """def PerformTest(self):
        modules = QuickUsb.QuickUsb.FindModules()
        if len(modules) == 0:
            return
        qusb = QuickUsb.QuickUsb(modules[0])
        qusb.Open(modules[0])
        
        # Variables
        transBytes = 0;
        samples = [0.0,]*10
        sampleIndex = 0
        startTime = 0.0
        stopTime = 0.0
        duration = 0.0
        
        dataSize = 0
        data = []
        size = 0
        
        # Perform the test
        #while not self.quit:
        for kk in range(100):
            if dataSize != self.testPacketSize:
                dataSize = self.testPacketSize
            
            #Start Timing
            transBytes = 0
            #startTime = time.time()
            startTime = time.time()
            
            # Perform R/W
            size = int(dataSize)
            if self.testRead:
                try:
                    data = qusb.ReadData(size)
                except QuickUsb.QuickUsbException:
                    wx.MessageBox("QuickUsb.ReadData(%d) failed with exception %s!" % (size, str(QuickUsb.Error(QuickUsb.QuickUsb.GetLastError()))), "QuickUsb Exception", wx.OK, self)
            else:
                try:
                    qusb.WriteData(data, size)
                except QuickUsb.QuickUsbException:
                    wx.MessageBox("QuickUsb.WriteData(data, %d) failed with exception %s!" % (size, str(QuickUsb.Error(QuickUsb.QuickUsb.GetLastError()))), "QuickUsb Exception", wx.OK, self)
            transBytes += int(size)
            
            # Calculate the throughput
            #stopTime = time.time()
            stopTime = time.time()
            duration = stopTime - startTime
            if duration != 0.0:
                self.throughput = float(transBytes) / (1024.0 * 1024.0 * duration)
                #samples[sampleIndex] = float(transBytes) / (1024.0 * 1024.0 * duration)
                #sampleIndex += 1
                #if sampleIndex >= len(samples):
                #    sampleIndex = 0
                #self.throughput = 0.0
                #for k in range(len(samples)):
                #    self.throughput += samples[k]
                #self.throughput /= float(len(samples))
                msg = "Throughput: %.2f MB/s" % self.throughput
                print msg
                
                # Allow GUI to update
                self.stThroughput.Label = msg
                self.gThroughput.Value = min(35, int(self.throughput))
            else:
                self.throughput = 0.0
            
            self.TheApp.ProcessPendingEvents()
            #wx.CallAfter(wx.App.ProcessPendingEvents, self.TheApp)
        
        self.bStart.Enable(True)
        self.bStop.Enable(False)
        
        # Close the module
        qusb.Close()"""

    def OnScPacketSizeSpin(self, event):
        self.ClampPacketSize()
        
    def OnScPacketSizeText(self, event):
        self.ClampPacketSize()

    def OnCbPacketSizeUnitCombobox(self, event):
        self.ClampPacketSize()

    def OnRbReadRadiobutton(self, event):
        self.testRead = self.rbRead.GetValue()

    def OnRbWriteRadiobutton(self, event):
        self.testRead = self.rbRead.GetValue()

    def OnBStartButton(self, event):
        try:
            (ok, modules) = QuickUsb.QuickUsb.FindModules()
        except QuickUsb.QuickUsbException:
            return
        if len(modules) == 0:
            return
        
        self.bStart.Enable(False)
        self.bStop.Enable(True)
        
        self.testRead = self.rbRead.GetValue()
        self.testPacketSize = 0

        #self.PerformTest()
        #self.thread = threading.Thread(target=self.PerformTest, name='QuickUsb Throughput Test Thread')
        #self.thread.start()
        
        self.qusb = QuickUsb.QuickUsb(modules[0])
        print modules
        try:
            (ok,) = self.qusb.Open()
        except QuickUsb.QuickUsbException:
            self.bStart.Enable(True)
            self.bStop.Enable(False)
            return
        self.fptr = open("/dev/qusb/qusb-0",'r')
        self.timer1.Start(10, True)

    def OnBStopButton(self, event):
        self.quit = True
        
        #while self.thread.isAlive():
        #   time.sleep(0.1)
        #self.thread.join()
        
        self.bStart.Enable(True)
        self.bStop.Enable(False)
        
        self.timer1.Stop()
        if self.qusb:
            self.qusb.Close()
        self.qusb = None

        self.fptr.close()
        self.fptr = None

    def OnUpdateTimer(self, event):
        #self.stThroughput.Label = "XXXXXXXXXXXX: %.2f MB/s" % self.throughput
        #self.gThroughput.Value = min(35, int(self.throughput))
        pass

    def OnFrameMainClose(self, event):
        if self.quit == False:
            self.quit = True
            
            #while self.thread.isAlive():
            #    time.sleep(0.1)
        self.timer1.Stop()
        event.Skip()

    def OnTimer1Timer(self, event):
        dataSize = int(self.scPacketSize.GetValue()) * (1024**self.cbPacketSizeUnit.GetSelection())
        if self.testPacketSize != dataSize:
            self.testPacketSize = dataSize
            self.array = QuickUsb.CreateByteBuffer(self.testPacketSize)
            
        #Start Timing
        transBytes = 0
        startTime = self.GetTime()
        
        # Perform R/W
        if self.testRead:
            try:
                #(ok, bytesRead) = self.qusb.ReadData(self.array, self.testPacketSize)
                self.array = fptr.read(self.testPacketSize)
                bytesRead = len(self.array)
            except QuickUsb.QuickUsbException:
                wx.MessageBox("QuickUsb.ReadData(%d) failed with exception %s!" % (self.testPacketSize, str(QuickUsb.Error(QuickUsb.QuickUsb.GetLastError()))), "QuickUsb Exception", wx.OK, self)
                return
            transBytes += bytesRead
        else:
            try:
                #(ok,) = self.qusb.WriteData(self.array, self.testPacketSize)
                fptr.write(self.array.raw)
            except QuickUsb.QuickUsbException:
                wx.MessageBox("QuickUsb.WriteData(data, %d) failed with exception %s!" % (self.testPacketSize, str(QuickUsb.Error(QuickUsb.QuickUsb.GetLastError()))), "QuickUsb Exception", wx.OK, self)
                return
            transBytes += self.testPacketSize
        
        # Calculate the throughput
        stopTime = self.GetTime()
        duration = stopTime - startTime
        if duration != 0.0:
            self.throughput = float(transBytes) / (1024.0 * 1024.0 * duration)

            msg = "Throughput: %.2f MB/s" % self.throughput
            print msg
            
            # Allow GUI to update
            self.stThroughput.Label = msg
            self.gThroughput.Value = min(35, int(self.throughput))
        else:
            self.throughput = 0.0
            
        self.timer1.Start(10, True)
        
    def GetTime(self):
        if 'win' in sys.platform:
            return time.clock() #Very accurate in Windows
        return time.time() #1ms resolution in windows, 1us in unix
