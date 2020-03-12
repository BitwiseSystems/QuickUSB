#Boa:Frame:frameMain
"""
============================================================================
 Title       : frameMain.py
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
import wx.lib.filebrowsebutton
from wx.lib.anchors import LayoutAnchors



# Python imports
import os
import sys
import time
import webbrowser



# Bitwise Systems imports
from QuickUsb import *
import QuickUsbCtrl



# GUI imports
from panelGeneral import panelGeneral
from panelCommand import panelCommand
from panelData import panelData
from panelPorts import panelPorts
from panelI2C import panelI2C
from panelSPI import panelSPI
from panelRS232 import panelRS232 
from panelSettings import panelSettings
import frameAbout



# Have QuickUSB not throw exceptions and instead simply return the usual bool
# status indicator.  Errors then may be retrieved through GetLastError().
QuickUsb.SetThrowExceptions(False)



# Create a null device to redirect stdout (Used when calling os.write)
class DevNull:
    def write(self, s):
        pass



# Generated Boa Contructor create() method
def create(parent, *args):
    return frameMain(parent, *args)

# Generated Boa Constructor wx IDs
[wxID_FRAMEMAIN, wxID_FRAMEMAINGPROGRESS, wxID_FRAMEMAINLVDEVICE, 
 wxID_FRAMEMAINNBMAIN, wxID_FRAMEMAINPCOMMAND, wxID_FRAMEMAINPDATA, 
 wxID_FRAMEMAINPDEFAULTS, wxID_FRAMEMAINPGENERAL, wxID_FRAMEMAINPI2C, 
 wxID_FRAMEMAINPMAIN, wxID_FRAMEMAINPPORTS, wxID_FRAMEMAINPRS232, 
 wxID_FRAMEMAINPSETTINGS, wxID_FRAMEMAINPSPI, wxID_FRAMEMAINSTATUSBAR, 
] = [wx.NewId() for _init_ctrls in range(15)]

[wxID_FRAMEMAINMENUHELPITEM_ABOUT, wxID_FRAMEMAINMENUHELPITEM_FAQ, 
 wxID_FRAMEMAINMENUHELPITEM_QUSBUSERGUIDE, wxID_FRAMEMAINMENUHELPITEM_SUPPORT, 
 wxID_FRAMEMAINMENUHELPITEM_USERGUIDE, wxID_FRAMEMAINMENUHELPITEM_WEBSITE, 
] = [wx.NewId() for _init_coll_menuHelp_Items in range(6)]

[wxID_FRAMEMAINTSTATUS, wxID_FRAMEMAINTUPDATE, 
] = [wx.NewId() for _init_utils in range(2)]



[wxID_FRAMEMAINMENUEDITITEM_PERIODICUPDATE, 
 wxID_FRAMEMAINMENUEDITMENU_RESETALLFIELDS, 
] = [wx.NewId() for _init_coll_menuEdit_Items in range(2)]

[wxID_FRAMEMAINMENUFILEITEM_EXIT] = [wx.NewId() for _init_coll_menuFile_Items in range(1)]

class frameMain(wx.Frame):
    # Customer class definitions needed by Boa Constructor to allow us to place
    # our panels in separate files
    _custom_classes = {'wx.Panel': ['panelGeneral',
                                    'panelCommand', 
                                    'panelData', 
                                    'panelPorts', 
                                    'panelI2C', 
                                    'panelSPI', 
                                    'panelRS232',
                                    'panelSettings',
                                    'panelDefaults']}



    def _init_coll_szbMain_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.lvDevice, 1, border=0, flag=wx.GROW)
        parent.AddWindow(self.nbMain, 0, border=0, flag=wx.GROW)
        parent.AddWindow(self.gProgress, 0, border=0, flag=wx.GROW)

    def _init_coll_menuEdit_Items(self, parent):
        # generated method, don't edit

        parent.Append(help='', id=wxID_FRAMEMAINMENUEDITITEM_PERIODICUPDATE,
              kind=wx.ITEM_CHECK,
              text='Periodically refresh settings and values')
        parent.AppendSeparator()
        parent.Append(help='', id=wxID_FRAMEMAINMENUEDITMENU_RESETALLFIELDS,
              kind=wx.ITEM_NORMAL, text='Reset all fields')
        self.Bind(wx.EVT_MENU, self.OnMenuEditItem_periodicupdateMenu,
              id=wxID_FRAMEMAINMENUEDITITEM_PERIODICUPDATE)
        self.Bind(wx.EVT_MENU, self.OnMenuEditItems1Menu,
              id=wxID_FRAMEMAINMENUEDITMENU_RESETALLFIELDS)

    def _init_coll_menuHelp_Items(self, parent):
        # generated method, don't edit

        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_USERGUIDE,
              kind=wx.ITEM_NORMAL, text='QuickUSB Diagnostics User Guide...')
        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_QUSBUSERGUIDE,
              kind=wx.ITEM_NORMAL, text='QuickUSB User Guide...')
        parent.AppendSeparator()
        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_WEBSITE,
              kind=wx.ITEM_NORMAL, text='QuickUSB Website...')
        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_FAQ,
              kind=wx.ITEM_NORMAL, text='QuickUSB FAQ...')
        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_SUPPORT,
              kind=wx.ITEM_NORMAL, text='QuickUSB Customer Support...')
        parent.AppendSeparator()
        parent.Append(help='', id=wxID_FRAMEMAINMENUHELPITEM_ABOUT,
              kind=wx.ITEM_NORMAL, text='&About QuickUSB Diagnostics')
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_aboutMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_ABOUT)
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_userguideMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_USERGUIDE)
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_websiteMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_WEBSITE)
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_faqMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_FAQ)
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_supportMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_SUPPORT)
        self.Bind(wx.EVT_MENU, self.OnMenuHelpItem_qusbuserguideMenu,
              id=wxID_FRAMEMAINMENUHELPITEM_QUSBUSERGUIDE)

    def _init_coll_menuFile_Items(self, parent):
        # generated method, don't edit

        parent.Append(help='', id=wxID_FRAMEMAINMENUFILEITEM_EXIT,
              kind=wx.ITEM_NORMAL, text='E&xit')
        self.Bind(wx.EVT_MENU, self.OnMenuFileItemsExitMenu,
              id=wxID_FRAMEMAINMENUFILEITEM_EXIT)

    def _init_coll_mnubarMain_Menus(self, parent):
        # generated method, don't edit

        parent.Append(menu=self.menuFile, title='&File')
        parent.Append(menu=self.menuEdit, title='&Edit')
        parent.Append(menu=self.menuHelp, title='&Help')

    def _init_coll_nbMain_Pages(self, parent):
        # generated method, don't edit

        parent.AddPage(imageId=-1, page=self.pGeneral, select=True,
              text='General')
        parent.AddPage(imageId=-1, page=self.pCommand, select=False,
              text='Command')
        parent.AddPage(imageId=-1, page=self.pData, select=False, text='Data')
        parent.AddPage(imageId=-1, page=self.pPorts, select=False, text='Ports')
        parent.AddPage(imageId=-1, page=self.pRS232, select=False, text='RS232')
        parent.AddPage(imageId=-1, page=self.pSPI, select=False, text='SPI')
        parent.AddPage(imageId=-1, page=self.pI2C, select=False, text='I2C')
        parent.AddPage(imageId=-1, page=self.pSettings, select=False,
              text='Settings')
        parent.AddPage(imageId=-1, page=self.pDefaults, select=False,
              text='Defaults')

    def _init_sizers(self):
        # generated method, don't edit
        self.szbMain = wx.BoxSizer(orient=wx.VERTICAL)

        self._init_coll_szbMain_Items(self.szbMain)

        self.pMain.SetSizer(self.szbMain)

    def _init_utils(self):
        # generated method, don't edit
        self.mnubarMain = wx.MenuBar()

        self.menuFile = wx.Menu(title='')

        self.menuHelp = wx.Menu(title='')

        self.tStatus = wx.Timer(id=wxID_FRAMEMAINTSTATUS, owner=self)
        self.Bind(wx.EVT_TIMER, self.OnTStatusTimer, id=wxID_FRAMEMAINTSTATUS)

        self.tUpdate = wx.Timer(id=wxID_FRAMEMAINTUPDATE, owner=self)
        self.Bind(wx.EVT_TIMER, self.OnTUpdateTimer, id=wxID_FRAMEMAINTUPDATE)

        self.menuEdit = wx.Menu(title='')

        self._init_coll_mnubarMain_Menus(self.mnubarMain)
        self._init_coll_menuFile_Items(self.menuFile)
        self._init_coll_menuHelp_Items(self.menuHelp)
        self._init_coll_menuEdit_Items(self.menuEdit)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Frame.__init__(self, id=wxID_FRAMEMAIN, name='frameMain',
              parent=prnt, pos=wx.Point(701, 324), size=wx.Size(491, 268),
              style=wx.DEFAULT_FRAME_STYLE, title='Set in code!')
        self._init_utils()
        self.SetMenuBar(self.mnubarMain)
        self.SetStatusBarPane(0)
        self.SetClientSize(wx.Size(475, 230))
        self.SetMinSize(wx.Size(100, 100))
        self.SetHelpText('')
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnFrameMainEraseBackground)
        self.Bind(wx.EVT_CLOSE, self.OnFrameMainClose)

        self.pMain = wx.Panel(id=wxID_FRAMEMAINPMAIN, name='pMain', parent=self,
              pos=wx.Point(0, 0), size=wx.Size(475, 187),
              style=wx.TAB_TRAVERSAL)

        self.nbMain = wx.Notebook(id=wxID_FRAMEMAINNBMAIN, name='nbMain',
              parent=self.pMain, pos=wx.Point(0, 79), size=wx.Size(475, 100),
              style=0)
        self.nbMain.SetToolTipString('')
        self.nbMain.SetMinSize(wx.Size(100, 100))
        self.nbMain.Enable(True)
        self.nbMain.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED,
              self.OnNbMainNotebookPageChanged, id=wxID_FRAMEMAINNBMAIN)

        self.lvDevice = QuickUsbCtrl.QuickUsbCtrl(id=wxID_FRAMEMAINLVDEVICE,
              name='lvDevice', parent=self.pMain, pos=wx.Point(0, 0),
              scanenabled=True, scaninterval=500, scanonload=True,
              size=wx.Size(475, 79),
              style=wx.LC_REPORT | wx.LC_SORT_ASCENDING | wx.LC_SINGLE_SEL)
        self.lvDevice.SetToolTipString('')
        self.lvDevice.SetMinSize(wx.Size(565, 100))
        self.lvDevice.Bind(wx.EVT_LIST_ITEM_SELECTED,
              self.OnLvDeviceListItemSelected, id=wxID_FRAMEMAINLVDEVICE)
        self.lvDevice.Bind(wx.EVT_LIST_ITEM_DESELECTED,
              self.OnLvDeviceListItemSelected, id=wxID_FRAMEMAINLVDEVICE)
        self.lvDevice.Bind(QuickUsbCtrl.EVT_QUSB_CONNECTED_EVENT,
              self.OnLvDeviceConnect)
        self.lvDevice.Bind(QuickUsbCtrl.EVT_QUSB_DISCONNECTED_EVENT,
              self.OnLvDeviceDisconnect)

        self.pGeneral = panelGeneral(id=wxID_FRAMEMAINPGENERAL, name='pGeneral',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pCommand = panelCommand(id=wxID_FRAMEMAINPCOMMAND, name='pCommand',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pData = panelData(id=wxID_FRAMEMAINPDATA, name='pData',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pPorts = panelPorts(id=wxID_FRAMEMAINPPORTS, name='pPorts',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pRS232 = panelRS232(id=wxID_FRAMEMAINPRS232, name='pRS232',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pSPI = panelSPI(id=wxID_FRAMEMAINPSPI, name='pSPI',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pI2C = panelI2C(id=wxID_FRAMEMAINPI2C, name='pI2C',
              parent=self.nbMain, pos=wx.Point(0, 0), size=wx.Size(467, 74),
              style=wx.TAB_TRAVERSAL)

        self.pSettings = panelSettings(id=wxID_FRAMEMAINPSETTINGS,
              name='pSettings', parent=self.nbMain, pos=wx.Point(0, 0),
              size=wx.Size(467, 74), style=wx.TAB_TRAVERSAL)

        self.pDefaults = panelSettings(id=wxID_FRAMEMAINPDEFAULTS,
              name='pDefaults', parent=self.nbMain, pos=wx.Point(0, 0),
              size=wx.Size(467, 74), style=wx.TAB_TRAVERSAL)

        self.statusBar = wx.StatusBar(id=wxID_FRAMEMAINSTATUSBAR,
              name='statusBar', parent=self, style=0)
        self.SetStatusBar(self.statusBar)

        self.gProgress = wx.Gauge(id=wxID_FRAMEMAINGPROGRESS, name='gProgress',
              parent=self.pMain, pos=wx.Point(0, 179), range=100,
              size=wx.Size(475, 8), style=wx.GA_HORIZONTAL)

        self._init_coll_nbMain_Pages(self.nbMain)

        self._init_sizers()

    def __init__(self, parent, AppTitle, AppVersion):
#-------------------------------------------------------------------------------
        """Constructor"""
        # Set a state variable to indicate that we are being constructed
        self.loading = True
        
        # Enable QuickUsb call logging
        #QuickUsb.QuickUsbLogFunc = self.Log

        # When a value is written to a setting, default, port, etc. the 
        # value may be immediately read back so that the form matches the
        # hardware
        self._performReadBack = True
        
        # Populate ourself with our Boa-generated GUI
        self._init_ctrls(parent)
        self.Log("Loading QuickUsbDiag")
        
        # The code for the settings and defaults panel both exist in the
        # panelSetting.py file, as they are nearly identical.  We simply need
        # to tell our defaults panel that it is a defaults panel.
        self.pDefaults.UseAsDefaults()
        
        # Set the application title
        self.SetTitle('%s %s' % (AppTitle, AppVersion))
        
        # Set the application icon
        icopath = None
        if os.path.isfile(u'QuickUSB.ico'):
            icopath = u'QuickUSB.ico'
        elif os.path.isfile(os.path.join(os.path.dirname(sys.argv[0]), u'QuickUSB.ico')):
            icopath = os.path.join(os.path.dirname(sys.argv[0]), u'QuickUSB.ico')
        elif "_MEIPASS2" in os.environ:
            # Extract about image from executable
            icopath = os.path.join(os.environ["_MEIPASS2"], u'QuickUSB.ico')

        if icopath:
            self.SetIcon(wx.Icon(icopath, wx.BITMAP_TYPE_ICO))
              
        # Hold a list of all the panels in our notebook control
        self._panels = [self.pGeneral,
                        self.pCommand,
                        self.pData,
                        self.pPorts,
                        self.pRS232,
                        self.pSPI,
                        self.pI2C,
                        self.pSettings,
                        self.pDefaults]
        
        # Inform all of the panels that we are their father
        for panel in self._panels:
            panel.TheFrame = self
        
        # Set the status message to indicate that we are loading
        self.statusMsg = ['Loading...please wait']
        
        # Initialize the status message timer
        self.statusMsgTime = self.GetTime()
        self.tStatus.Start(250, False)
        
        # We are done loading
        self.loading = False
        
        # Go ahead and Enable/Disable all the controls on all the panels in the
        # notebook control depending on if a module is connected or not
        self.UpdateAllTabCtrlsEnables()
        
        # Load our saved configuration
        self.LoadConf()
        
        # Have every control set its minimum size to its best size so
        # that controls such as buttons and comboboxes will be properly
        # sized on all platforms
        for panel in self._panels:
            try:
                panel.SetControlMinSizes()
                continue
            except AttributeError:
                pass
            self.RecursivelySetMinSize(panel.GetSizer())
        self.nbMain.SetMinSize(self.nbMain.GetBestSize())
        
        # Figure our what page is currently selected and populate all the
        # controls on it with valid information from the selected module
        pageNum = self.nbMain.GetSelection()
        pageName = self.nbMain.GetPageText(pageNum)
        self.UpdateTabCtrl(pageNum, pageName)

        # Set the frame size.  To do this we must figure out how much space
        # our main size minimally requires, then set our client size.
        minSize = self.szbMain.GetMinSize()
        self.SetClientSize(minSize)
        self.SetMinSize(self.GetSize())



    def GetMenuItem(self, id):
        """
        Given the wx ID of a menu item, return the wx.MenuItem instance of that
        menu
        """
        # Iterate over all menus
        for k in xrange(self.mnubarMain.GetMenuCount()):
            menu = self.mnubarMain.GetMenu(k)
            
            # Iterate over all menu items on each menu
            for menuItem in menu.GetMenuItems():
                # Check if the IDs match
                if menuItem.GetId() == id:
                    return menuItem
        return None



    def RecursivelySetMinSize(self, parent):
        """
        Recursively iterate through the supplied parent object (which is a sizer
        or a window) and set the minimum size of every child control to its best
        size
        """
        # Iterate over all child objects
        for child in parent.GetChildren():
            # Check if the object is a sizer
            if child.IsSizer():
                # We can't set the minimum size of a sizer (because a sizer is
                # sized based on its child windows), so just recursively
                # iterate through all children of the sizer itself
                self.RecursivelySetMinSize(child.GetSizer())
            elif child.IsWindow():
                # If the child is a window (aka control), set its minimum size
                child.GetWindow().SetMinSize(child.GetWindow().GetBestSize())



    def RecursivelyEnable(self, parent, ena):
        """
        Recursively iterate through the supplied parent object (which is a sizer
        or a window) and enable/disable the every child control
        """
        # Iterate over all child objects
        for child in parent.GetChildren():
            # Check if the object is a sizer
            if child.IsSizer():
                # We can't enable/disable sizers, so just recursively iterate 
                # through all children of the sizer itself
                self.RecursivelyEnable(child.GetSizer(), ena)
            elif child.IsWindow():
                # If the child is a window (aka control), enable/disable it
                child.GetWindow().Enable(ena)



    def UpdateAllTabCtrlsEnables(self):
        """
        Enable/disable all the controls on all of the panels in our notebook
        control
        """
        # As this is potentially a slow process, set the mouse cursor to busy
        # and suspend updates of the notebook
        wx.BeginBusyCursor()
        self.nbMain.Freeze()
        
        # Iterate over all pages in the notebook
        for k in xrange(len(self._panels)):
            # Enable/disable the controls on the panel
            self.UpdateTabCtrlEnable(k, self.nbMain.GetPageText(k))

        # Now allow updates to the notebook and restore the mouse cursor
        self.nbMain.Thaw()
        wx.EndBusyCursor()



    def UpdateTabCtrlEnable(self, pageNum, pageName):
        """
        Enable/disable all the controls on a panel in our notebook control
        """
        # Ignore the log page
        if pageName == 'Log':
            return
        
        # Get the selected QuickUSB module
        qusb = self.lvDevice.SelectedModule()
        
        # If there is a selected module, we enable and otherwise disable
        ena = (qusb != None)
        
        # Recursively perform the enable/disable
        self.RecursivelyEnable(self._panels[pageNum].GetSizer(), ena)
        
        # Some panels may have particular controls which are always disabled or
        # only enabled/diabled under certain circumstances, so have those panels
        # perform there enabling/disabling
        try:
            self._panels[pageNum].UpdateCtrlsEnable(ena)
        except:
            raise "Panel", pageName, "must implement the UpdateCtrlsEnable() function"



    def UpdateTabCtrl(self, pageNum, pageName):
        """
        Update the indicated panel in the notebook with values from the 
        selected QuickUSB module
        """
        # Ignore the log page
        if pageName == 'Log':
            return
        
        # Don't do anything if we're loading
        if self.loading:
            return
        
        # Get the selected QuickUSB Module, if there is one
        qusb = self.lvDevice.SelectedModule()
        if not qusb:
            return
        
        # As this is potentially a slow process, set the mouse cursor to busy
        # and suspend updates of the panel
        wx.BeginBusyCursor()
        self._panels[pageNum].Freeze()
        
        # Have panel update control values
        try:
            self._panels[pageNum].UpdateCtrls(qusb)
        finally:
            # Now allow updates to the notebook and restore the mouse cursor.
            # This must be under 'finally' so that the cursor gets restore even
            # if an exception is thrown
            self._panels[pageNum].Thaw()
            wx.EndBusyCursor()



    def ParseDataBox(self, tbData):
        """
        Many of the panels in our notebook contain textboxes meant to hold user
        entered data which needs to be parsed.  This method will parse the
        text box and return the data in a standard array
        """
        # First, convert the text to lowercase and get all the hex values into
        # a single format
        text = tbData.GetValue().lower().replace('&h','0x')
        
        # Next, break the text up based on whitespace
        tokens = text.split()
        
        # Now, create a buffer to hold all of the data.  Each token corresponds
        # to one byte
        numTokens = len(tokens)
        buffer = CreateByteBuffer(numTokens)
        
        # Iterate over each token
        for k in xrange(numTokens):
            # Try to convert the string token to an integer value
            try:
                value = self.GetInt(tokens[k])
            except ValueError:
                return None
            
            # Make sure the integer is a valid value for a byte
            if value > 255:
                return None
            
            # Store the data
            buffer[k] = value
        
        # Return the data
        return buffer
    
    
    
    def GetInt(self, s):
        """
        Many of the panels in our notebook need to convert some value in a
        text box or other control to an integer value.  This method will parse
        a string and return an integer with for strings such as:
            '12'
            '0x3e5'
            '&h3e5'
        """
        # Get all the hex values into a single format
        text = s.replace('&h','0x')
        
        # See if the number is a normal int
        try:
            # Return the value
            return int(s)
        except ValueError:
            # Try to return the hex value of the string
            return int(s, 16)
    
    
    
    def PlaceDataBox(self, tbData, buffer, bytes):
        """
        Many of the panels in our notebook need to put data into a textbox.  
        This method provides an easy way to place data in a text box in a 
        nice, standard, format
        """
        data = ""
        for k in xrange(bytes):
            data += '0x%.2x ' % buffer[k]
        data = data.rstrip()
        tbData.SetValue(data)



    def SetConfiguration(self, confDict):
        """
        Store information that needs to be remebered between execution of this
        application, such as window position, window size, the last directory,
        entered data, etc.
        """
        cfg = wx.Config("QuickUsbDiagPy", "Bitwise Systems")
        for k, v in confDict.iteritems():
            cfg.Write(k, str(v))
        
        cfg.Write('Main.PeriodicUpdate', str(self.GetMenuItem(wxID_FRAMEMAINMENUEDITITEM_PERIODICUPDATE).IsChecked()))
        
        
    def GetConfiguration(self):
        """
        Load information that was stored the last time this application was
        executed.
        """
        result = {}
        cfg = wx.Config("QuickUsbDiagPy", "Bitwise Systems")

        # Get the first entry
        (more, key, index) = cfg.GetFirstEntry()
        while True:
            # Get the value and save the key-value pair into the dictionary
            value = cfg.Read(key)
            if value:
                result[key] = value
            
            # Check if there are more entries
            if more:
                (more, key, index) = cfg.GetNextEntry(index)
            else:
                break
        
        # Enable the update timer.  This is the timer that periodically updates
        # the information on the displayed notebook page with data from the
        # selected module.
        if 'Main.PeriodicUpdate' in result:
            if result['Main.PeriodicUpdate'] == 'True':
                ena = True
            else:
                ena = False
        else:
            ena = True
        self.GetMenuItem(wxID_FRAMEMAINMENUEDITITEM_PERIODICUPDATE).Check(ena)
        if ena:
            self.tUpdate.Start(1500)
        else:
            self.tUpdate.Stop()

        # Return the dictionary
        return result



    def LoadConf(self):
        """
        Load our saved configuration data, then pass it onto each panel
        """
        conf = self.GetConfiguration()
        for panel in self._panels:
            panel.LoadConf(conf)
            
            
            
    def SaveConf(self):
        """
        Get the information that needs to be saved from each panel, then save it
        """
        conf = {}
        for panel in self._panels:
            conf.update(panel.SaveConf())
        self.SetConfiguration(conf)



    def GetTime(self):
        """
        Get the current time (in seconds) as a float as accurately as we can
        """
        if sys.platform.startswith('win'):
            return time.clock() # Very accurate in Windows
        return time.time() # 1ms resolution in windows, 1us in unix
    
    
    
    def SetStatus(self, msg):
        """
        Add a message to be displayed in the status bar
        """
        self.statusMsg.append(msg)



    def GetSelectedModule(self):
        """
        Return the currently selected module
        """
        return self.lvDevice.SelectedModule()
    
    
    
    def Log(self, string):
        time = self.GetTime()
        msg = '%.3f: %s\n' % (time, string)
        try:
            self.tbLog.AppendText(msg)
        except:
            pass



    def LaunchDoc(self, doc):
        if sys.platform == 'win32':
            programFiles = os.environ.get("PROGRAMFILES")
            programFilesx86 = os.environ.get("PROGRAMFILES(x86)")
            searchPath = (sys.argv[0],
                          os.path.curdir,
                          os.path.join(programFiles, "Bitwise Systems", "QuickUsb", "Doc"),
                          os.path.join(programFilesx86, "Bitwise Systems", "QuickUsb", "Doc"))
            openCmd = (os.startfile,)
        elif sys.platform == 'darwin':
            searchPath = (sys.argv[0],
                          os.path.curdir,
                          os.path.join('/','usr','share','doc'))
            openCmd = ('open',)
        elif sys.platform.startswith('linux'):
            searchPath = (sys.argv[0],
                          os.path.curdir,
                          os.path.join('/','usr','share','doc'))
            openCmd = ('open', 'xdg-open', 'gnome-open')
        else:
            wx.MessageBox("Unable to open '%s' -- Unsupported platform." % doc, "Error")
        
        # Loop through all of the paths where documentation may reside
        found = False
        launched = False
        for docPath in searchPath:
            # See if the document exists here
            filePath = os.path.join(docPath, doc)
            if os.path.exists(filePath):
                # Keeping track if we were actually able to open the file
                # allows us to give better error information
                found = True

                # Loop through all the different ways to launch the file
                for cmd in openCmd:
                    # If the cmd is a callable function
                    if callable(cmd):
                        # Try to call the method
                        try:
                            cmd(filePath)
                        except:
                            pass
                        else:
                            launched = True
                            break
                    else:
                        # Try to launch the file, but redirect stdout and 
                        # stderr so the user doesn't see our failed attemps
                        sys.stdout = DevNull()
                        sys.stderr = DevNull()
                        result = os.system("%s \"%s\"" % (cmd, filePath))
                        sys.stdout = sys.__stdout__
                        sys.stderr = sys.__stderr__
                        if result == 0:
                            launched = True
                            break
            else:
                # If the file was not found step up the path tree looking for a 
                # 'Doc' folder until we reach the root dir
                parentDir = docPath
                while True:
                    nextParentDir = os.path.abspath(os.path.join(parentDir, os.path.pardir))
                    if nextParentDir == parentDir:
                        break
                    parentDir = nextParentDir
                    docPath = os.path.join(parentDir, "Doc")

                    # See if the document exists here
                    filePath = os.path.join(docPath, doc)
                    if os.path.exists(filePath):
                        # Keeping track if we were actually able to open the file
                        # allows us to give better error information
                        found = True

                        # Loop through all the different ways to launch the file
                        for cmd in openCmd:
                            # If the cmd is a callable function
                            if callable(cmd):
                                # Try to call the method
                                try:
                                    cmd(filePath)
                                except:
                                    pass
                                else:
                                    launched = True
                                    break
                            else:
                                # Try to launch the file, but redirect stdout and 
                                # stderr so the user doesn't see our failed attemps
                                sys.stdout = DevNull()
                                sys.stderr = DevNull()
                                result = os.system("%s \"%s\"" % (cmd, filePath))
                                sys.stdout = sys.__stdout__
                                sys.stderr = sys.__stderr__
                                if result == 0:
                                    launched = True
                                    break

                        if launched:
                            break
            if launched:
                break

        if not found:
            wx.MessageBox("Unable to open locate '%s'." % doc, "Error")
        elif not launched:
            wx.MessageBox("Unable to open '%s'." % doc, "Error")



    def OnTUpdateTimer(self, event):
        """
        This timer handles updating controls on the currently visible notebook
        page with the latest QuickUSB values.  Usual values to update are
        settings and port values.
        """
        # Get the selected module
        qusb = self.lvDevice.SelectedModule()
        
        # Update the currently visible panel
        try:
            self.nbMain.GetCurrentPage().PeriodicUpdateCtrls(qusb)
        except AttributeError:
            pass



    def OnTStatusTimer(self, event):
        """
        This timer updates the status bar with status messages in our status
        message queue so that messages do not flash by or do not hang around
        forever
        """
        # Get the current time
        now = self.GetTime()
        
        # Get the next status message
        if len(self.statusMsg) > 0:
            msg = self.statusMsg[0]
        else:
            msg = ""

        # Check is the message is already displayed
        if (msg != "") and (msg == self.statusBar.GetStatusText()):
            # Calculate the timeout for the message.  The number of seconds for
            # a timeout depends on the number of messages in our list that need
            # to be displayed.
            waitTime = 2.0
            numMsgs = len(self.statusMsg)
            if numMsgs > 1:
                waitTime = 1.0
                if numMsgs > 3:
                    # Don't allow the messages to pile up
                    self.statusMsg = self.statusMsg[:3]
                    msg = self.statusMsg[0]
            # Actually see if this message has timed out
            if (now - self.statusMsgTime) > waitTime:
                # Remove the message from the list
                del self.statusMsg[0]
                
                # Set the next message
                if len(self.statusMsg) > 0:
                    msg = self.statusMsg[0]
                    self.statusMsgTime = self.GetTime()
        else:
            # Set the next message
            self.statusMsgTime = self.GetTime()
        self.statusBar.SetStatusText(msg)



    def OnLvDeviceConnect(self, event):
        """
        Event handler for the connection of a QuickUSB Module
        """



    def OnLvDeviceDisconnect(self, event):
        """
        Event handler for the removal of a QuickUSB Module
        """
        # On some platforms the listview select event does not get sent
        # when an item is removed, so just make sure that we handler that
        # here
        self.OnLvDeviceListItemSelected(event)



    def OnLvDeviceListItemSelected(self, event):
        """
        Update the currently visible panel when a module is selected or
        deselected
        """
        # Don't bother doing anything if we are still loading
        if self.loading:
            return
        
        # Update the all the tab controls enables (visible or not)
        self.UpdateAllTabCtrlsEnables()

        # Update just the visible panel with data from the QuickUSB module
        pageNum = self.nbMain.GetSelection()
        self.UpdateTabCtrl(pageNum, self.nbMain.GetPageText(pageNum))
        
        
        
    def OnNbMainNotebookPageChanged(self, event):
        """
        Update the visible panel with data from the QuickUSB module when the
        notebook page is changed
        """
        event.Skip() # Neccessary to draw tab
        
        # Get the current notebook page
        pageNum = event.GetSelection()
        if pageNum != -1:
            # Update the page
            self.UpdateTabCtrl(pageNum, self.nbMain.GetPageText(pageNum))
    
    
    
    def OnMenuFileItemsExitMenu(self, event):
        """
        Quit the applcation
        """
        self.Close()
        
        
        
    def OnMenuHelpItem_aboutMenu(self, event):
        """
        Display the about dialog
        """
        dlg = frameAbout.create(self)
        dlg.ShowModal()
        dlg.Destroy()



    def OnMenuHelpItem_qusbuserguideMenu(self, event):
        """
        Open the QuickUSB User Guide
        """
        self.LaunchDoc("QuickUSB User Guide.pdf")



    def OnMenuHelpItem_userguideMenu(self, event):
        """
        Open the QuickUSB Diagnostics User Guide
        """
        self.LaunchDoc("QuickUSB Diagnostics User Guide.pdf")



    def OnMenuHelpItem_websiteMenu(self, event):
        """
        Navigate to the QuickUSB Website
        """
        webbrowser.open("http://www.quickusb.com")



    def OnMenuHelpItem_faqMenu(self, event):
        """
        Navigate to the QuickUSB FAQ Website
        """
        webbrowser.open("http://www.quickusb.com/index.php?main_page=page&id=5")



    def OnMenuHelpItem_supportMenu(self, event):
        """
        Navigate to the QuickUSB Support Website
        """
        webbrowser.open("http://www.quickusb.com/support")



    def OnFrameMainClose(self, event):
        """
        Event handler for the application close event
        """
        # Save our state
        self.SaveConf()
        
        # Allow this event to continue to propagate (otherwise we would be
        # canceling the close event)
        event.Skip()



    # Reduce flicker during resize events
    def OnFrameMainEraseBackground(self, event):
        # Intentionally do nothing
        pass



    def OnMenuEditItem_periodicupdateMenu(self, event):
        """
        Enable/Diable the periodic update timer
        """
        if event.Checked():
            if not self.tUpdate.IsRunning():
                self.tUpdate.Start(1000)
        else:
            if self.tUpdate.IsRunning():
                self.tUpdate.Stop()



    def OnMenuEditItems1Menu(self, event):
        # Delete the entries in the registry/file
        cfg = wx.Config("QuickUsbDiagPy", "Bitwise Systems")
        cfg.DeleteAll()
        cfg.Flush()
        
        # Update the controls
        self.LoadConf()
