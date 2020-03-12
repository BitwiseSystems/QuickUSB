"""
============================================================================
 Title       : QuickUsbCtrl.py
 Description : The QuickUSB ListView Control Object
 Notes       : Currently only supports the wx.LC_REPORT style
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
import wx.lib.newevent

# Python imports
import sys
import imp

# Import QuickUsb
try:
    from QuickUsb import *
except ImportError:
    QuickUsb = None

# Create QuickUsbCtrl Events
QuickUsbConnectedEvent, EVT_QUSB_CONNECTED_EVENT = wx.lib.newevent.NewEvent()
QuickUsbDisconnectedEvent, EVT_QUSB_DISCONNECTED_EVENT = wx.lib.newevent.NewEvent()

# Class to hold information about each item in our listview
class qusbListInfo:
    def __init(self):
        self.qusb = None
        self.lvi = None
        self.attached = False



# QuickUsbCtrl Object
class QuickUsbCtrl(wx.ListView):
    """
    The QuickUSB ListView Control Object
    Currently only supports the wx.LC_REPORT style
    """
    def __init__(self, parent, id=-1, pos=wx.DefaultPosition, size=wx.DefaultSize, style=wx.LC_REPORT | wx.LC_SORT_ASCENDING | wx.LC_SINGLE_SEL, validator=wx.DefaultValidator, name="BitTwiddler", **kwargs):
        """Constructor"""
        wx.ListView.__init__(self, parent, id, pos, size, style, validator, name)

        # Extract keyword arguments
        self.scanOnLoad = bool(kwargs['scanonload'])
        self.scanEnabled = bool(kwargs['scanenabled'])
        self.scanInterval = int(kwargs['scaninterval'])
        
        # Intialize variables
        self.QuickUsbDict = {}
        self.QuickUsbSelDict = {}
        self.loading = True

        # Bind events


        # Create wx.LC_REPORT style headers
        self.InsertColumn(0, "Name")
        self.InsertColumn(1, "Serial")
        self.InsertColumn(2, "Firmware Version")
        self.InsertColumn(3, "Firmware Model")

        # Perform initial scan
        if self.scanOnLoad:
            self.RescanForModules()
            if self.GetItemCount() > 0:
                self.Select(0)
            else:
                self.AutoResizeByContentAndHeader()
        self.loading = False

        # Setup scan timer
        self.tScan = wx.Timer(owner=self)
        self.Bind(wx.EVT_TIMER, self.OnTScanTimer, self.tScan)
        self.tScan.Start(self.scanInterval, True)

    
    
    def GetScanEnabled(self):
        """
        Return whether or not if the controls automatically scans for newly
        added or removed QuickUSB modules
        """
        return self.scanEnabled
        
    
    
    def SetScanEnabled(self, value):
        """
        Set whether or not the controls automatically scans for newly added or 
        removed QuickUSB modules
        """
        self.scanEnabled = bool(value)
        
        
        
    def GetScanOnLoad(self):
        """
        Return whether or not the control performs a scan for connected QuickUSB
        modules when it is created
        """
        return self.scanOnLoad
        

    
    def SetScanOnLoad(self, value):
        """
        Set whether or not the control performs a scan for connected QuickUSB
        modules when it is created
        """
        self.scanOnLoad = bool(value)
        
        
    
    def GetScanInterval(self):
        """
        Return the interval (in milliseconds) indicating how often the control
        should scan for newly added or removed QuickUSB modules
        """
        return self.scanInterval
        
        
    
    def SetScanInterval(self, value):
        """
        Set the interval (in milliseconds) indicating how often the control
        should scan for newly added or removed QuickUSB modules
        """
        self.scanInterval = int(value)



    def Modules(self):
        """
        Return a list of QuickUSB objects for all QuickUSB modules currently
        connected and seen by the control
        """
        return [v.qusb for k,v in self.QuickUsbDict.iteritems()]



    def ModuleNames(self):
        """
        Return a list of QuickUSB names for all QuickUSB modules currently
        connected and seen by the control
        """
        return self.QuickUsbDict.keys()



    def ModuleNamesDict(self):
        """
        Return a dictionary of the form {QuickUsbName:QuickUsbObject, ...} of 
        all QuickUSB modules currently connected and seen by the control
        """
        moduleDict = {}
        for k,v in self.QuickUsbDict.iteritems():
            moduleDict[k] = v.qusb
        return moduleDict



    def SelectedModule(self):
        """
        Return the selected QuickUSB object.  If more than one module is
        selected, this function will return the first.
        """
        count = self.GetSelectedItemCount()
        if count > 0:
            devName = self.GetItem(self.GetFirstSelected(), 0).GetText()
            return self.QuickUsbDict[devName].qusb
        return None


    def SelectedModules(self):
        """
        Return a list of all selected QuickUSB objects
        """
        moduleList = []

        count = self.GetSelectedItemCount()
        if count > 0:
            index = self.GetFirstSelected()
            moduleList.append(self.GetItem(index).qusb)
            for k in xrange(count - 1):
                index = self.GetNextSelected(index)
                moduleList.append(self.GetItem(index).qusb)
        return moduleList



    def RescanForModules(self):
        """
        Perform a scan of all modules connected to the host and update the
        control with newly added and removed QuickUSB modules
        """
        # Check the ScanOnLoad and ScanEnabled properties and listen to them
        if not self.scanEnabled:
            return False
        if self.loading:
            if not self.scanOnLoad:
                return
            
        # Find all attached modules
        try:
            (ok, attachedModules) = QuickUsb.FindModules()
        except:
            attachedModules = []

        # Check for lost modules
        arr = self.QuickUsbDict.keys()
        for k in xrange(len(arr)):
            devName = arr[k]
            
            if self.QuickUsbDict[devName].attached and not devName in attachedModules:
                # Hold a reference to the QuickUSB module
                qusb = self.QuickUsbDict[devName].qusb
                
                # Remove the module from the listview
                self.DeleteItem(self.FindItem(0, devName))
                
                # Remove the module from our list
                del self.QuickUsbDict[devName]

                # Send the ModuleDisconnected event
                event = QuickUsbDisconnectedEvent(ModuleName=devName)
                event.SetEventObject(self)
                event.SetId(self.GetId())
                wx.PostEvent(self, event)

                self.AutoResizeByContentAndHeader()

        # Check for new modules
        arr = self.QuickUsbDict.keys()
        alreadyAdded = False
        for devName in attachedModules:
            if devName not in arr:
                # Create a new qusbListInfo item
                listInfo = qusbListInfo()
                listInfo.attached = True
                listInfo.qusb = QuickUsb(devName)
                listInfo.lvi = wx.ListItem()
                listInfo.lvi.SetText(devName)

                # Add the new QuickUSB to the list and listview
                if not alreadyAdded:
                    self.InsertItem(listInfo.lvi)
                    itemId = self.FindItem(0, devName)
                    (ok, Model) = listInfo.qusb.Model
                    (ok, Serial) = listInfo.qusb.Serial
                    (ok, major, minor, rev) = listInfo.qusb.GetFirmwareVersion()
                    self.SetStringItem(itemId, 1, Serial)
                    self.SetStringItem(itemId, 2, "%i.%i.%i" % (major, minor, rev))
                    self.SetStringItem(itemId, 3, Model)
                    self.QuickUsbDict[devName] = listInfo

                # Send the ModuleConnected event
                event = QuickUsbConnectedEvent(Module=listInfo.qusb, ModuleName=devName)
                event.SetEventObject(self)
                event.SetId(self.GetId())
                wx.PostEvent(self, event)

                self.AutoResizeByContentAndHeader()
                if self.GetItemCount() == 1:
                    self.Select(0)



    def AutoResizeByContentAndHeader(self):
        """
        Set the column header widths to be large enoguh to display both the
        header column label and all of the content under than header
        """
        dc = wx.ScreenDC()
        if not dc.Ok() or not self.GetFont().Ok():
            return
        
        # Get listview column header widths and heights
        for k in xrange(self.GetColumnCount()):
            # Measure the column text
            column = self.GetColumn(k)
            font = column.GetFont()
            if not font.Ok() and sys.platform != 'win32':
                font = self.GetFont()
                if not font.Ok():
                    break
            (colSizeW, colSizeH) = dc.GetTextExtent(column.GetText())

            # Find the largest listview entries for each column
            for j in xrange(self.GetItemCount()):
                item = self.GetItem(j, k)
                font = item.GetFont()
                if not font.Ok() and sys.platform != 'win32':
                    font = self.GetFont()
                    if not font.Ok():
                        break
                dc.SetFont(font)
                (itemSizeW, itemSizeH) = dc.GetTextExtent(item.GetText())
                if itemSizeW > colSizeW:
                    colSizeW = itemSizeW
                
            # Now set the column size
            self.SetColumnWidth(k, colSizeW + 8)



    def OnTScanTimer(self, event):
        """
        Event handler for the scan timer, which handles scanning for newly
        added and remved QuickUSB
        """
        self.RescanForModules()

        # Restart the timer
        self.tScan.Start(500, True)
