"""
============================================================================
 Title       : BitTwiddler.plug-in.py
 Description : The Boa Constructor interface for the BitTwiddler Control
 Notes       : 
 History     :

 Copyright (c) 2010 Bitwise Systems.  All rights reserved.
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

# wx imports
import wx

# Boa Contructor imports
import Plugins
from Utils import _

from Companions.EventCollections import *

from PropEdit.PropertyEditors import *
from PropEdit.Enumerations import *

EventCategories['BitTwiddlerEvent'] =  ('wx.EVT_CHECKBOX',)
commandCategories.append('BitTwiddlerEvent')

# BitTwiddler Object
try:
    import BitTwiddler
except ImportError:
    if hasattr(sys, 'frozen'):
        application_path = os.path.dirname(sys.executable)
    elif __file__:
        application_path = os.path.dirname(__file__)
    else:
        application_path = os.path.dirname(sys.argv[0])
    BitTwiddler = imp.load_source('BitTwiddler', os.path.join(application_path, 'Plug-ins', 'BitTwiddler.py'))



class BitTwiddlerDTC(WindowDTC):
    def __init__(self, name, designer, parent, ctrlClass):
        WindowDTC.__init__(self, name, designer, parent, ctrlClass)
        self.bits = 8
        self.value = 0
        self.tooltips = []
        self.showlabel = True
        self.bitspacing = 4
        self.label = name
        self.tagint = 0

        for k in xrange(self.bits):
            self.tooltips.append("Bit %i: " % k)

        self.editors = {'Bits' : IntConstrPropEdit,
                        'ToolTips': ChoicesConstrPropEdit,
                        'Value': IntConstrPropEdit,
                        'ShowLabel':BoolPropEdit,
                        'BitSpacing': IntConstrPropEdit,
                        'Label': StrConstrPropEdit,
                        'Style': StyleConstrPropEdit,
                        'TagInt': IntConstrPropEdit}



    def properties(self):
        props = WindowDTC.properties(self)
        return props



    def constructor(self):
        return {'Value': 'value', 'Position': 'pos', 'Size': 'size',
                'Style': 'style', 'Name': 'name', 'Bits': 'bits',
                'ToolTips': 'tooltips', 'ShowLabel':'showlabel',
                'BitSpacing':'bitspacing', 'Label':'label', 'TagInt':'tagint'}



    def designTimeSource(self, position = 'wx.DefaultPosition', size = 'wx.DefaultSize'):
        return {'value': `self.value`,
                'pos': position,
                'size': size,
                'style': 'wx.NO_BORDER',
                'name': `self.name`,
                'bits': `self.bits`,
                'tooltips': `self.tooltips`,
                'showlabel': `self.showlabel`,
                'bitspacing': `self.bitspacing`,
                'label': `self.label`,
                'tagint': `self.tagint`}



    #def designTimeControl(self, position, size, args = None):
    #    print "args=",args
    #    ctrl = CheckBoxDTC.designTimeControl(self, position, size, args)
    #    return ctrl



    def hideDesignTime(self):
        return WindowDTC.hideDesignTime(self) + \
              ['Selection', 'Title', 'DefaultStyle', 'ToolTipString']



    def events(self):
        return WindowDTC.events(self) + ['BitTwiddlerEvent']



    def defaultAction(self):
        insp = self.designer.inspector
        insp.pages.SetSelection(2)
        insp.events.doAddEvent('BitTwiddlerEvent', 'wx.EVT_CHECKBOX')



    def writeImports(self):
        return 'import BitTwiddler'



Plugins.registerPalettePage('Bitwise Systems', _('Bitwise Systems'))
Plugins.registerComponent('Bitwise Systems', BitTwiddler.BitTwiddler, 'BitwiseSystems.BitTwiddler', BitTwiddlerDTC)
