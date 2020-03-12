"""
============================================================================
 Title       : BitTwiddler.py
 Description : General N-Bit Editing wx User Control (max of 16 bits)
 Notes       : Currently only tested for 8-bits
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

#wx imports
import wx

#Python imports
import sys



class BitTwiddler(wx.Control):
    def __init__(self, parent, id=-1, pos=wx.DefaultPosition, size=wx.DefaultSize, style=wx.NO_BORDER, validator=wx.DefaultValidator, name="BitTwiddler", **kwargs):
        """BitTwiddler Constructor"""
        wx.Control.__init__(self, parent, id, pos, wx.Size(-1, -1), style, validator, name)

        # Extract keyword arguments
        self._changing = False
        self.bits = int(kwargs['bits'])
        self.value = int(kwargs['value'])
        self.ToolTips = kwargs['tooltips']
        self.showlabel = bool(kwargs['showlabel'])
        self.bitspacing = int(kwargs['bitspacing'])
        self.label = str(kwargs['label'])
        self.tagint = int(kwargs['tagint'])
        
        # Set up the default checkbox spacing
        if sys.platform == 'win32':
            self.bitspacing = 3
        elif sys.platform == 'linux':
            self.bitspacing = 0
        elif sys.platform == 'darwin':
            self.bitspacing = 0
        else:
            self.bitspacing = 3

        # Create sizers
        self.szfgMain = wx.FlexGridSizer(cols=2, hgap=4, rows=1, vgap=0)
        self.szfgBT = wx.FlexGridSizer(cols=self.bits, hgap=self.bitspacing, rows=1, vgap=0)
        self.szfgMain.AddSizer(self.szfgBT, 0, border=0, flag=wx.ALIGN_RIGHT | wx.ALIGN_CENTER_VERTICAL)

        # Create label
        self.stLabel = wx.StaticText(id=-1, label=self.label, name='stLabel', parent=self, pos=wx.Point(0, 0), size=wx.Size(-1, -1), style=0)
        if self.showlabel:
            self.szfgMain.SetCols(3)
            self.szfgMain.InsertWindow(0, self.stLabel, 0, border=0, flag=wx.GROW | wx.ALIGN_LEFT | wx.ALIGN_CENTER_VERTICAL)
            self.szfgMain.AddGrowableCol(0, 1)

        # Add the checkboxes
        self.cbBits = {}
        for k in xrange(self.bits):
            self.InsertCheckbox()

        # Create textbox
        self.tbValue = wx.TextCtrl(id=-1, name='tbValue', parent=self, pos=wx.Point(0, 36), size=wx.Size(35, 18), style=wx.TE_READONLY, value='0x00')
        hexDigits = len('%x' % ((2 ** self.bits) - 1))
        hexStr = ('0x%.' + str(hexDigits) + 'X') % 0xAA
        if sys.platform == 'win32':
            extra = (5, 0)
        elif sys.platform.startswith('linux'):
            extra = (9, 0)
        else:
            extra = (0, 0)
        self.tbValue.SetMinSize((self.tbValue.GetSize() - self.tbValue.GetClientSize()) + self.tbValue.GetTextExtent(hexStr) + extra)
        self.szfgMain.AddWindow(self.tbValue, 0, border=0, flag=wx.ALIGN_LEFT)
        self.SetValue(0)

        # Set tooltips
        for k,v in self.cbBits.iteritems():
            tip = self.ToolTips[k].split(':')
            if len(tip) == 1:
                tip = tip[0].strip()
            else:
                tip = ':'.join(tip[1:]).strip()
            v.SetToolTip(wx.ToolTip(tip))
            
        # Allow a forced size
        self.forcedSize = (-1, -1)

        # Bind events
        self.Bind(wx.EVT_CHECKBOX, self.OnCheckBox)
        self.Bind(wx.EVT_SIZE, self.OnSize)

        # Size and layout
        self.SetSizerAndFit(self.szfgMain)
        self.Layout()
        
        
        
    def GetBestSize(self):
        """
        Calculate the minimum size required to show the label (if enabled),
        all the checkboxes, and the textbox.
        """
        # Get the size of a checkbox
        if len(self.cbBits) > 0:
            cbW, cbH = self.cbBits[0].GetBestSize()
        else:
            cbW, cbH = 0, 0
            
        # Get the size of the label
        if self.showlabel:
            stW, stH = self.stLabel.GetBestSize()
        else:
            stW, stH = 0, 0
            
        # Get the size of the textbox
        tbW, tbH = self.tbValue.GetSize()
        
        # Calculate our best size based on the size of the label, each checkbox,
        # the checkbox spacing, and the textbox.
        nBits = len(self.cbBits)
        best = wx.Size((nBits * cbW) + ((nBits - 1) * self.bitspacing) + stW + tbW + ((self.szfgMain.GetHGap() - 1) * self.szfgMain.GetCols()), max(stH, tbH, cbH))
          
        # Remember and return our best size
        #self.CacheBestSize(best)
        return best
        
        
        
    def InsertCheckbox(self):
        """Internal function: Add a checkbox"""
        #Create the new checkbox
        bit = len(self.cbBits)
        cbBit = wx.CheckBox(id=wx.NewId(), label='', name='cbBit%i' % bit, parent=self, style=0)
        self.Bind(wx.EVT_CHECKBOX, self.OnCheckBox, cbBit)
        (cbW, cbH) = cbBit.GetSize()
        cbBit.SetPosition(wx.Point((cbW + self.bitspacing) * (self.bits - bit - 1)))
        
        # Move the current bit checkboxes over
        for k,v in self.cbBits.iteritems():
            v.SetPosition(wx.Point((cbW + self.bitspacing) * (self.bits - k - 1), 0))

        # Set the value of the checkbox
        if ((2 ** bit) & self.value):
            cbBit.SetValue(True)
        else:
            cbBit.SetValue(False)
            
        # Add the checkbox and update our layout
        self.cbBits[bit] = cbBit
        self.szfgBT.Insert(0, cbBit, proportion=0, border=0, flag=0)
        self.szfgBT.SetCols(self.bits)
        self.szfgBT.Layout()
        
        
        
    def RemoveCheckbox(self):
        """Internal function: Remove a checkbox"""
        # Move the current bit checkboxes over
        for k,v in self.cbBits.iteritems():
            v.SetPosition(wx.Point((self.bits - k - 1) * 17, 0))

        # Remove the checkbox and update our layout
        bit = len(self.cbBits) - 1
        if bit >= 0:
            self.szfgBT.Remove(self.cbBits[bit])
            self.szfgBT.SetCols(self.bits)
            self.szfgBT.Layout()
            del self.cbBits[bit]



    def OnCheckBox(self, event):
        """Internal function: Handle the checkbox event"""
        if self._changing:
            return

        # Retrieve our current value and set it to ensure the checkboxes match
        # the textbox
        self._changing = True
        value = self.GetValue()
        self.SetValue(value)

        # Change the valkue and ownership of the checkbox event
        event.SetInt(value)
        event.SetId(self.GetId())
        event.SetEventObject(self)

        # Send this altered event to anyone listening
        self.GetEventHandler().ProcessEvent(event)
        self._changing = False
        


    def OnSize(self, event):
        """Internal function: Handle a resizing event"""
        msx, msy = self.GetMinSize()
        bsx, bsy = self.GetBestSize()
        self.SetSize((max(msx, bsx), max(msy, bsy)))
        self.Layout()



    def GetBits(self):
        """Return the number of bits"""
        return self.bits



    def SetBits(self, value):
        """Set the number of bits"""
        # Remember our old bit size and cap the number of bits
        oldBits = self.bits
        self.bits = int(value)
        if self.bits <= 0:
            self.bits = 1
        if self.bits > 16:
            self.bits = 16

        # If we are adding bits
        if oldBits < self.bits:
            # Increase the size of the tooltips array
            self.ToolTips.extend(["",]*(self.bits-oldBits))

            # Create additional checkboxes
            for k in xrange(self.bits - oldBits):
                self.InsertCheckbox()
        # If we are removing bits
        elif oldBits > self.bits:
            #Decrease the size of the tooltips array
            self.ToolTips = self.ToolTips[:self.bits]

            # Remove checkboxes
            for k in xrange(oldBits - self.bits):
                self.RemoveCheckbox()

        # Update our size and layout
        self.SetSizerAndFit(self.szfgMain)
        self.Layout()
        
        
        
    def GetValue(self):
        """
        Get the value of the control.  Currently, any BitTwiddler control with
        more bits than can fit in an int will be truncated.
        """
        # Calculate our value from the actual checkboxes
        value = 0
        for k,v in self.cbBits.iteritems():
            if v.GetValue():
                value += (2 ** k)

        # Remember and return this value
        self.value = int(value)
        return self.value



    def SetValue(self, value):
        """
        Set the value of the control.  Currently, if the BitTwiddler control
        contains more bits than can fit in an int the control will not 
        accurately reflect the value set by this method
        """
        # Mask off unused bits in 'value'
        self.value = int(value) & ((2 ** self.bits) - 1)
        
        # Set the checkboxes to reflect our new value
        for k,v in self.cbBits.iteritems():
            if (2 ** k) & self.value:
                v.SetValue(True)
            else:
                v.SetValue(False)
        
        # Set the textbox to match our value
        hexDigits = len('%x' % ((2 ** self.bits) - 1))
        hexStr = ('0x%.' + str(hexDigits) + 'X') % self.value
        self.tbValue.SetValue(hexStr)



    def GetToolTips(self):
        """
        Return a list of the tooltips associated with each bit of the
        BitTwiddler control.  The 0th element of the array corresponds to the
        0-th bit, 1st to 1st, and so on.
        """
        return self.ToolTips
    
    
    
    def SetToolTips(self, value):
        """
        Set the tooltips associated with each bit of the BitTwiddler control.
        The 0th element of the array corresponds to the 0-th bit, 1st to 1st, 
        and so on.
        """
        self.ToolTips = value

        if len(self.ToolTips) < self.bits:
            # Increase the size of the tooltips array
            self.ToolTips.extend(["",]*(self.bits-len(self.ToolTips)))
        else:
            # Decrease the size of the tooltips array
            self.ToolTips = self.ToolTips[:self.bits]

        for k,v in self.cbBits.iteritems():
            tip = self.ToolTips[k].split(':')[0].strip()
            v.SetToolTip(wx.ToolTip(tip))

        # Set tooltips
        for k,v in self.cbBits.iteritems():
            tip = self.ToolTips[k].split(':')
            if len(tip) == 1:
                tip = tip[0].strip()
            else:
                tip = ':'.join(tip[1:]).strip()
            v.SetToolTip(wx.ToolTip(tip))



    def GetShowLabel(self):
        """Return a bool indicating if the label should be visible"""
        return self.showlabel
    
    
    
    def SetShowLabel(self, value):
        """Set whether the label should be visible"""
        # Remember our old value and set our new one
        oldshowlabel = self.showlabel
        self.showlabel = bool(value)
        self.stLabel.Show(self.showlabel)

        # If our visibility changed
        if oldshowlabel != self.showlabel:
            if self.showlabel:
                # Add the label to our sizer
                self.szfgMain.SetCols(3)
                self.szfgMain.InsertWindow(0, self.stLabel, 0, border=0, flag=wx.GROW | wx.ALIGN_LEFT | wx.ALIGN_CENTER_VERTICAL)
                self.szfgMain.AddGrowableCol(0, 1)
            else:
                # Remove the label from our sizer
                self.szfgMain.SetCols(2)
                self.szfgMain.Remove(self.stLabel)
                self.szfgMain.RemoveGrowableCol(0)

        #Update our size and layout
        self.Layout()
        
        
        
    def GetBitSpacing(self):
        """Return the spacing between neighboring bit checkboxes"""
        return self.bitspacing
        
        
        
    def SetBitSpacing(self, value):
        """Set the spacing between neighboring bit checkboxes"""
        # Set the new bit spacing
        self.bitspacing = int(value)
        if self.bitspacing < 0:
            self.bitspacing = 0
        self.szfgBT.SetHGap(self.bitspacing)
        
        # Update our layout
        self.Layout()



    def GetLabel(self):
        """Return the label text"""
        return self.Label



    def SetLabel(self, value):
        """Set the label text"""
        # Update the label text
        self.label = str(value).strip("'")
        self.stLabel.SetLabel(self.label)
        
        # Update our layout
        self.Layout()
        
        
        
    def GetStyle(self):
        """Get the wx control style"""
        return self.GetWindowStyle()



    def SetStyle(self, value):
        """Set the wx control style"""
        # Set our window style
        self.SetWindowStyle(int(value))
        
        # Update our control and layout.  Note that some style values cannot be 
        # changed after a control has been created.
        self.Refresh()
        self.Layout()
    
    
    
    def GetTagInt(self):
        """Get the user int value"""
        return self.tagint
        
        
        
    def SetTagInt(self, value):
        """Set the user int value"""
        self.tagint = value
