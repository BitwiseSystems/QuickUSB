#!/usr/bin/python
#Boa:App:BoaApp
"""
============================================================================
 Title       : QuickUsbDiag.py
 Description : QuickUSB Diagnotics Application
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
