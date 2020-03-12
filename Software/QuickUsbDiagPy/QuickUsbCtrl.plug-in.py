"""
============================================================================
 Title       : QuickUsbCtrl.plug-in.py
 Description : Boa Contructor plug-in wrapper for the wx QuickUSB Control
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
