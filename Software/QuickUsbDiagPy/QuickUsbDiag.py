#!/usr/bin/python
#Boa:App:BoaApp
"""
============================================================================
 Title       : QuickUsbDiag.py
 Description : QuickUSB Diagnotics Application
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

# wx imports
import wx
"""
============================================================================
 Title       : QuickUsbDiag.py
 Description : The QuickUSB Diagnostics Program
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

# GUI imports
import frameMain

# Application information
AppTitle = 'QuickUSB Diagnostics (Python)'
AppVersion = 'v2.15.2'
AppCompany = 'Bitwise Systems'

# Boa Contructor modules
modules ={u'frameAbout': [0, '', u'frameAbout.py'],
 u'frameMain': [0, '', u'frameMain.py'],
 u'panelCommand': [0, '', u'panelCommand.py'],
 u'panelData': [0, '', u'panelData.py'],
 u'panelGeneral': [0, '', u'panelGeneral.py'],
 u'panelI2C': [0, '', u'panelI2C.py'],
 u'panelPorts': [0, '', u'panelPorts.py'],
 u'panelRS232': [0, '', u'panelRS232.py'],
 u'panelSPI': [0, '', u'panelSPI.py'],
 u'panelSettings': [0, '', u'panelSettings.py']}



# Main wx Application
class BoaApp(wx.App):
    def OnInit(self):
        self.main = frameMain.create(None, AppTitle, AppVersion)
        self.main.App = self
        self.main.Show()
        self.SetTopWindow(self.main)
        return True



# Main function
def main():
    application = BoaApp(0)
    application.MainLoop()



# If we're being executed and not imported
if __name__ == '__main__':
    main()
