"""
============================================================================
 Title       : QuickUsbCtrl.plug-in.py
 Description : Boa Contructor plug-in wrapper for the wx QuickUSB Control
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

# Python imports
import imp
import os
import sys

# wx imports
import wx

# Boa Contructor imports
import Plugins
from Utils import _

from Companions.EventCollections import *

from PropEdit.PropertyEditors import *
from PropEdit.Enumerations import *

# QuickUsbCtrl Object
try:
    import QuickUsbCtrl
except ImportError:
    if hasattr(sys, 'frozen'):
        application_path = os.path.dirname(sys.executable)
    elif __file__:
        application_path = os.path.dirname(__file__)
    else:
        application_path = os.path.dirname(sys.argv[0])
    QuickUsbCtrl = imp.load_source('QuickUsbCtrl', os.path.join(application_path, 'Plug-ins', 'QuickUsbCtrl.py'))

# Register QuickUsbCtrl Events
EventCategories['QuickUsbEvent'] = ('QuickUsbCtrl.EVT_QUSB_CONNECTED_EVENT', 'QuickUsbCtrl.EVT_QUSB_DISCONNECTED_EVENT')
commandCategories.append('QuickUsbEvent')

# Boa Design Time Control (DTC) object for QuickUsbCtrl
class QuickUsbCtrlDTC(ListViewDTC):
    def __init__(self, name, designer, parent, ctrlClass):
        """Constructor"""
        ListViewDTC.__init__(self, name, designer, parent, ctrlClass)
        self.scanOnLoad = True
        self.scanEnabled = True
        self.scanInterval = 500

        self.editors.update({'ScanOnLoad' : BoolPropEdit,
                             'ScanEnabled': BoolPropEdit,
                             'ScanInterval': IntConstrPropEdit})



    def properties(self):
        """"""
        props = ListViewDTC.properties(self)
        return props



    def constructor(self):
        """"""
        return {'Position': 'pos', 'Size': 'size', 'Style': 'style', 'Name': 'name',
                'ScanOnLoad':'scanonload', 'ScanEnabled':'scanenabled', 'ScanInterval':'scaninterval'}



    def designTimeSource(self, position = 'wx.DefaultPosition', size = 'wx.DefaultSize'):
        """"""
        return {'pos': position,
                'size': size,
                'style': 'wx.LC_REPORT | wx.LC_SORT_ASCENDING | wx.LC_SINGLE_SEL',
                'name': `self.name`,
                'scanonload': `self.scanOnLoad`,
                'scanenabled': `self.scanEnabled`,
                'scaninterval': `self.scanInterval`}



    def events(self):
        return WindowDTC.events(self) + ['QuickUsbEvent']



    def writeImports(self):
        """"""
        return 'import QuickUsbCtrl'


# Create a whole tab of the Boa palette for Bitwise Systems
Plugins.registerPalettePage('Bitwise Systems', _('Bitwise Systems'))

# Register the QuickUsbCtrl to appear on the Bitwise Systems palette
Plugins.registerComponent('Bitwise Systems', QuickUsbCtrl.QuickUsbCtrl, 'BitwiseSystems.QuickUsbCtrl', QuickUsbCtrlDTC)
