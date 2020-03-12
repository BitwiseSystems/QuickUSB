#!/usr/bin/env python
#Boa:PyApp:main

import wx

import frameMain

modules ={u'frameMain': [0, 'Main frame of Application', u'frameMain.py']}

class BoaApp(wx.App):
    def OnInit(self):
        self.main = frameMain.create(None)
        self.main.TheApp = self
        self.main.Show()
        self.SetTopWindow(self.main)
        return True

def main():
    application = BoaApp(0)
    application.MainLoop()

if __name__ == '__main__':
    main()