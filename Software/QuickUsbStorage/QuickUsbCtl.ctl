VERSION 5.00
Begin VB.UserControl QuickUsbCtl 
   ClientHeight    =   765
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   5310
   ScaleHeight     =   765
   ScaleWidth      =   5310
   ToolboxBitmap   =   "QuickUsbCtl.ctx":0000
   Begin VB.Timer ModuleScanTimer 
      Enabled         =   0   'False
      Interval        =   3000
      Left            =   4140
      Top             =   0
   End
   Begin VB.TextBox ModuleStringDescriptorTextBox 
      BackColor       =   &H8000000F&
      Height          =   315
      Left            =   1020
      Locked          =   -1  'True
      TabIndex        =   1
      Top             =   360
      Width           =   3915
   End
   Begin VB.ComboBox ModuleComboBox 
      Height          =   315
      Left            =   1020
      Style           =   2  'Dropdown List
      TabIndex        =   0
      ToolTipText     =   "Selects the active QuickUSB Module"
      Top             =   0
      Width           =   1215
   End
   Begin VB.Label Label10 
      Caption         =   "Description"
      Height          =   195
      Left            =   60
      TabIndex        =   3
      Top             =   420
      Width           =   855
   End
   Begin VB.Label Label5 
      Caption         =   "Module"
      Height          =   195
      Left            =   60
      TabIndex        =   2
      Top             =   60
      Width           =   555
   End
End
Attribute VB_Name = "QuickUsbCtl"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'=============================================================================
' Title        : QuickUSBCtl.ctl
' Description  : QuickUSB  User Control for Visual Basic 6
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
' History      :
'   Date       Name        Description
'  5/5/2002   bhenry      Initial coding
'
' Copyright © 2002,2003 Bitwise Systems, Inc.  All rights reserved.
' This software contains confidential information and trade secrets of
' Bitwise Systems and is protected by United States and international
' copyright laws.  Use, disclosure, or reproduction is prohibited without
' the prior express written permission of Bitwise Systems, except as agreed
' in the QuickUSB Plug-In Module license agreement.
'
' Use, duplication or disclosure by the U.S. Government is subject to
' restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
' (1998), and FAR 12.212, as applicable.
'
' Bitwise Systems
' 6489 Calle Real, Suite E
' Goleta, CA  93117
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: QuickUsbCtl.ctl $
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 9/27/08    Time: 4:28p
' Created in $/Projects/Bitwise/QuickUSB/Library/Software/QUICKUSBSTORAGE
' Demo of a really neat app that provides software access to the upper 2K
' of the code EEPROM.
' Survives across power cycles and firmware loads.
' Very neat.
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 4/13/07    Time: 5:45p
' Created in $/Projects/Raytheon/RVS/Gemini/LvdsMux/Software
'
' *****************  Version 2  *****************
' User: Blake Henry  Date: 11/26/06   Time: 12:15a
' Updated in $/Projects/OLIS/UDIO/Software/UdioStream
' Added first version of streaming to file
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 10/02/06   Time: 6:42p
' Created in $/Projects/OLIS/UDIO/Software/UdioStream
' Streaming Data Demo App
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 2/22/06    Time: 4:06p
' Created in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbOv7141Diag
'
' *****************  Version 2  *****************
' User: Blake Henry  Date: 8/25/05    Time: 6:36p
' Updated in $/Projects/OLIS/Software/UdioDiagVb
' Integration working version
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 7/09/05    Time: 9:46a
' Created in $/Projects/OLIS/Software/UDIODIAGVB
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 1/15/04    Time: 7:58a
' Created in $/Projects/Trex Enterprises/SWIB/Software
'=============================================================================
Option Explicit

'Property Variables:
Dim m_devName As String
Dim m_Connected As Boolean
Dim m_Enabled As Boolean
Private m_Font As Font

'Default Property Values:
Const m_def_Connected = False
Const m_def_Enabled = True

'Event Declarations:
Event ConnectionChanged(ByVal Connected As Boolean)
Attribute ConnectionChanged.VB_Description = "Indicates that the QuickUSB connection state changed and whether a module is connected or not"
Event IndexChanged(ByVal index As Integer)


Const EEPROM_STORAGE_BASE = &H3800     ' Reserve the top 2048 bytes for other uses
Const EEPROM_STORAGE_LENGTH = &O200
Const EEPROM_READ_ADDR = &H51
Const EEPROM_WRITE_ADDR = &H51
Const EEPROM_PAGE_LENGTH As Integer = 32


' Declare external functions
Private Declare Sub CopyMemory Lib "Kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)


Private Sub UserControl_Initialize()
'   UpdateModuleComboBox
End Sub


Private Function UpdateModuleComboBox() As Integer
   ' Update the module combo box with the a list of all the connected modules
   Dim result As Long
   Dim devCount As Integer
   Dim devList As String
   Dim ctlCount As Integer
   Dim devNameStr As String
   Dim startIndex As Integer
   Dim nullIndex As Integer
   Static oldDevList As String
   
   ' Get the list of modules
   devList = Space(128)
   result = QuickUsbFindModules(devList, 128)
   
   ' If we can't get the list or the list is empty
   ' then disable all the controls and exit
   If (result = 0 Or Mid(devList, 1, 1) = Chr(0)) Then
      ' Disable all other controls
      On Error Resume Next
      For ctlCount = 0 To Controls.count - 1
         Controls(ctlCount).Enabled = False
      Next
      
      ' Turn on the tabbed dialog, frame and rescan button
      oldDevList = ""
      ModuleComboBox.ListIndex = -1
      ModuleStringDescriptorTextBox.Text = ""
            
      ModuleScanTimer.Enabled = True
      UpdateModuleComboBox = 0
      m_Connected = False
      RaiseEvent ConnectionChanged(m_Connected)
      Exit Function
   End If
   
   ' If the list has items and it hasn't changed, just exit
   If (ModuleComboBox.ListCount > 0 And devList = oldDevList) Then
      UpdateModuleComboBox = ModuleComboBox.ListCount
      Exit Function
   End If
   
   ' Clear the list and add the modules to the listbox
   devCount = 0
   startIndex = 1
   ModuleComboBox.Clear
   Do While (Mid(devList, startIndex, 1) <> Chr(0))
      nullIndex = InStr(startIndex, devList, Chr(0), vbBinaryCompare)
      devNameStr = Mid(devList, startIndex, nullIndex - 1)
      ModuleComboBox.AddItem devNameStr
      startIndex = nullIndex + 1
      devCount = devCount + 1
   Loop
   oldDevList = devList
   ModuleComboBox.ListIndex = 0
   
   ' Enable all other controls
   On Error Resume Next
   For ctlCount = 0 To Controls.count - 1
      Controls(ctlCount).Enabled = True
   Next
   
   ' Update the descriptor string
   UpdateStringDescriptorTextBox
   
   ' Return the number of modules
   UpdateModuleComboBox = devCount
   
   ' Raise an event
   m_Connected = True
   RaiseEvent ConnectionChanged(m_Connected)
End Function


Private Sub UpdateStringDescriptorTextBox()
   Dim str As String
   Dim result As Long
   
   str = GetStringDescriptor()
   If (str <> "") Then
      ModuleStringDescriptorTextBox.Text = str
   End If
End Sub


Private Sub ModuleScanTimer_Timer()
   If (UserControl.Ambient.UserMode = True) Then
      UpdateModuleComboBox
   End If
End Sub



Private Sub ModuleComboBox_Click()
   m_devName = ModuleComboBox.List(ModuleComboBox.ListIndex)
   If (ModuleComboBox.Enabled) Then
      UpdateStringDescriptorTextBox
      RaiseEvent IndexChanged(ModuleComboBox.ListIndex)
   End If
End Sub
'
'
'Public Property Get devName() As String
'   devName = m_devName
'End Property


Public Function ModuleCount() As Integer
   ModuleCount = ModuleComboBox.ListCount
End Function



Public Function ModuleIndex() As Integer
   ModuleIndex = ModuleComboBox.ListIndex
End Function



Public Sub SelectModule(ModuleIndex As Integer)
   If (ModuleIndex < ModuleComboBox.ListCount) Then
      ModuleComboBox.ListIndex = ModuleIndex
      m_devName = ModuleComboBox.List(ModuleComboBox.ListIndex)
      UpdateStringDescriptorTextBox
   End If
End Sub



Public Function GetStringDescriptor() As String
   Dim hDevice As Long
   Dim result As Long
   Dim length As Byte
   Dim str As String
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      GetStringDescriptor = ""
      Exit Function
   End If
   
   ' Read the string descriptor
   str = Space(160)
   result = QuickUsbGetStringDescriptor(hDevice, 2, str, Len(str))
   If (result = 0) Then
      QuickUsbClose hDevice
      GetStringDescriptor = ""
      Exit Function
   End If
      
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   GetStringDescriptor = str
End Function



Public Function GetLastError() As Long
   GetLastError = GetLastError
End Function



Public Function WriteSetting(ByVal address As Integer, ByVal value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      WriteSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbWriteSetting(hDevice, address, value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteSetting = 0
   End If
End Function



Public Function ReadSetting(ByVal address As Integer, ByRef value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, address, value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadSetting = 0
      Exit Function
   End If
End Function



Public Function ReadCommand(ByVal address As Integer, data() As Byte, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadCommand(hDevice, address, data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadCommand = 0
      Exit Function
   End If
End Function



Public Function WriteCommand(ByVal address As Integer, data() As Byte, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteCommand(hDevice, address, data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteCommand = 0
      Exit Function
   End If
End Function



Public Function ReadData(data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      ReadData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadData(hDevice, data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadData = 0
      Exit Function
   End If
   
   ReadData = result
End Function



Public Function ReadDataInteger(data() As Integer, ByVal startIndex As Long, ByVal numInts As Long) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      ReadDataInteger = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadDataInteger = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadData(hDevice, data(startIndex), numInts * 2)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadDataInteger = 0
      Exit Function
   End If
   
   ReadDataInteger = result
End Function



Public Function ReadDataAsync(data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long, ByRef transaction As Byte) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      ReadDataAsync = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadDataAsync = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadDataAsync(hDevice, data(startIndex), numBytes, transaction)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadDataAsync = 0
      Exit Function
   End If
   
   ReadDataAsync = result
End Function



Public Function ReadDataIntegerAsync(data() As Integer, ByVal startIndex As Long, ByVal numInts As Long, ByRef transaction As Byte) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      ReadDataIntegerAsync = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadDataIntegerAsync = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadDataAsync(hDevice, data(startIndex), numInts * 2, transaction)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadDataIntegerAsync = 0
      Exit Function
   End If
   
   ReadDataIntegerAsync = result
End Function



Public Function WriteData(data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteData(hDevice, data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteData = 0
      Exit Function
   End If
   
   WriteData = result
End Function



Public Function WriteDataInteger(data() As Integer, ByVal startIndex As Long, ByVal numInts As Long) As Long
   Dim index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteDataInteger = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteDataInteger = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteData(hDevice, data(startIndex), numInts * 2)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteDataInteger = 0
      Exit Function
   End If
   
   WriteDataInteger = result
End Function



Public Function WriteDataAsync(data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long, ByRef transaction As Byte) As Long
   Dim index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteDataAsync = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteDataAsync = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteDataAsync(hDevice, data(startIndex), numBytes, transaction)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteDataAsync = 0
      Exit Function
   End If
   
   WriteDataAsync = result
End Function



Public Function WriteDataIntegerAsync(data() As Integer, ByVal startIndex As Long, ByVal numInts As Long, ByRef transaction As Byte) As Long
   Dim index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteDataIntegerAsync = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteDataIntegerAsync = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteDataAsync(hDevice, data(startIndex), numInts * 2, transaction)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteDataIntegerAsync = 0
      Exit Function
   End If
   
   WriteDataIntegerAsync = result
End Function



Public Function AsyncWait(ByRef length As Long, transaction As Byte, immediate As Byte) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      AsyncWait = False
      Exit Function
   End If
   
   result = QuickUsbAsyncWait(hDevice, length, transaction, immediate)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      AsyncWait = False
      Exit Function
   End If
   
   AsyncWait = True
End Function



Public Function SetRS232BaudRate(ByVal baudRate As Long) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      SetRS232BaudRate = False
      Exit Function
   End If
   
   result = QuickUsbSetRS232BaudRate(hDevice, baudRate)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      SetRS232BaudRate = False
      Exit Function
   End If
   
   SetRS232BaudRate = True
End Function



Public Function FlushRS232(ByVal portNum As Byte) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      FlushRS232 = False
      Exit Function
   End If
   
   ' Read length characters from the RS232 port
   result = QuickUsbFlushRS232(hDevice, portNum)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      FlushRS232 = False
      Exit Function
   End If
   
   FlushRS232 = True
End Function



Public Function GetNumRS232(ByVal portNum As Byte, ByRef length As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   Dim baudRate As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      GetNumRS232 = False
      Exit Function
   End If
   
   result = QuickUsbGetNumRS232(hDevice, portNum, length)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      GetNumRS232 = False
      Exit Function
   End If
   
   GetNumRS232 = True
End Function



Public Function ReadRS232String(ByVal portNum As Byte, Optional maxLen As Variant) As String
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim str As String
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadRS232String = ""
      Exit Function
   End If

   ' If the user didn't define the max length, just read what's in the buffer
   If IsMissing(maxLen) Then
      ' Find out how many characters are waiting for us
      result = QuickUsbGetNumRS232(hDevice, portNum, length)
      If (result = 0) Then
         QuickUsbClose hDevice
         ReadRS232String = ""
         Exit Function
      End If
   Else
      length = maxLen
   End If
   
   ' Check for zero length
   If (length = 0) Then
      QuickUsbClose hDevice
      ReadRS232String = ""
      Exit Function
   End If
   
   ' Stuff the string with nulls
   str = String(length, 0)
   
   ' Read length characters from the RS232 port
   result = QuickUsbReadRS232String(hDevice, portNum, str, length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
      
   ' Check the transaction result
   If (result = 0) Then
      ReadRS232String = ""
      Exit Function
   End If
   
   ReadRS232String = str
End Function



Public Function WriteRS232String(ByVal portNum As Byte, ByVal str As String) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteRS232String = False
      Exit Function
   End If
      
   result = QuickUsbWriteRS232String(hDevice, portNum, str, Len(str))
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      WriteRS232String = False
      Exit Function
   End If
   
   WriteRS232String = True
End Function



Public Function ReadRS232(ByVal portNum As Byte, ByRef data() As Byte, Optional maxLen As Variant) As Long
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim str As String
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      ReadRS232 = ""
      Exit Function
   End If

   ' If the user didn't define the max length, just read what's in the buffer
   If IsMissing(maxLen) Then
      ' Find out how many characters are waiting for us
      result = QuickUsbGetNumRS232(hDevice, portNum, length)
      If (result = 0) Then
         QuickUsbClose hDevice
         ReadRS232 = False
         Exit Function
      End If
   Else
      length = maxLen
   End If
   
   ' Check for zero length
   If (length = 0) Then
      QuickUsbClose hDevice
      ReadRS232 = False
      Exit Function
   End If
   
   ' Don't allow a length longer than the array
   If (length > UBound(data) + (1 - LBound(data))) Then
      length = UBound(data) + (1 - LBound(data))
   End If
   
   ' Stuff the string with nulls
   str = String(length, 0)
   
   ' Read length characters from the RS232 port
   result = QuickUsbReadRS232(hDevice, portNum, data(0), length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
      
   ' Check the transaction result
   If (result = 0) Then
      ReadRS232 = False
      Exit Function
   End If
   
   ReadRS232 = True
End Function



Public Function WriteRS232(ByVal portNum As Byte, ByRef data() As Byte) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteRS232 = False
      Exit Function
   End If
      
   result = QuickUsbWriteRS232(hDevice, portNum, data(0), UBound(data) + (1 - LBound(data)))
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      WriteRS232 = False
      Exit Function
   End If
   
   WriteRS232 = True
End Function



Public Function UploadFPGA(fileName As String, Optional status As StatusBar) As Boolean
   Dim index As Long
   Dim result As Long
   Dim length As Integer
   Dim isConfigured As Integer
   Dim fileLength As Long
   Dim blockLength As Integer
   Dim hDevice As Long
   Dim fpgaData() As Byte
   Dim inputFile As Integer
   Dim peDir As Byte
   Dim peVal As Byte
   
   ' Allocate storage for data elements
   blockLength = 64
   ReDim fpgaData(blockLength)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      UploadFPGA = False
      Exit Function
   End If
   
   ' Open the input file for reading
   inputFile = FreeFile
   On Error GoTo openError
   Open fileName For Binary As inputFile
   fileLength = LOF(inputFile)
   
   ' Start configuring the FPGA
   
   result = QuickUsbStartFpgaConfiguration(hDevice)
   If (result = 0) Then
      If Not IsMissing(status) Then
         status.Panels(1) = "Cannot start FPGA configuration"
      End If
      Close inputFile
      QuickUsbClose hDevice
      UploadFPGA = False
      Exit Function
   End If
   
   ' Send the FPGA configuration data one block at a time
   index = 0
   Do
      ' Read the data
      length = Min(blockLength, fileLength - index)
      
      'Read the data from the file
      Get inputFile, index + 1, fpgaData
            
      ' Write the data to the EEPROM
      result = QuickUsbWriteFpgaData(hDevice, fpgaData(0), length)
      If (result = 0) Then
         Exit Do
      End If
      
      ' Update the counters
      index = index + length
            
      ' Redraw the screen
      DoEvents
      
      If Not IsMissing(status) Then
         status.Panels(1) = "Configuring FPGA from " & fileName & "..." & CInt((index / fileLength) * 100) & "%"
      End If
   Loop Until index >= fileLength
      
   ' Close the file
   Close inputFile
   
   Sleep 100
   
   ' Start configuring the FPGA
   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Free the memory
   ReDim fpgaData(0)
   
   ' Notify the user
   If (isConfigured = 0) Then
      If Not IsMissing(status) Then
         status.Panels(1) = "FPGA configuration failed"
      End If
      UploadFPGA = False
   Else
      If Not IsMissing(status) Then
         status.Panels(1) = "FPGA was successfully configured"
      End If
      UploadFPGA = True
   End If
   Exit Function

openError:
   UploadFPGA = False
End Function



Public Function IsFpgaConfigured() As Boolean
   Dim result As Long
   Dim isConfigured As Integer
   Dim hDevice As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      IsFpgaConfigured = False
      Exit Function
   End If
   
   ' Start configuring the FPGA
   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   If (isConfigured = 0) Then
      IsFpgaConfigured = False
   Else
      IsFpgaConfigured = True
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=6,0,0,0
Public Property Get Font() As Font
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
   Set Font = m_Font
End Property

Public Property Set Font(ByVal New_Font As Font)
   Set m_Font = New_Font
   PropertyChanged "Font"
End Property

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
   Set m_Font = Ambient.Font
   m_Enabled = m_def_Enabled
   m_Connected = m_def_Connected
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
   Set m_Font = PropBag.ReadProperty("Font", Ambient.Font)
   m_Enabled = PropBag.ReadProperty("Enabled", m_def_Enabled)
   m_Connected = PropBag.ReadProperty("Connected", m_def_Connected)
End Sub

Private Sub UserControl_Resize()
   ModuleStringDescriptorTextBox.Width = Width - (ModuleStringDescriptorTextBox.Left + 10)
End Sub

Private Sub UserControl_Show()
   If Ambient.UserMode Then
      ModuleScanTimer.Enabled = True
   Else
      ModuleScanTimer.Enabled = False
   End If
   
   UpdateModuleComboBox
   DoEvents
   ModuleComboBox.Refresh
End Sub

Private Sub UserControl_Terminate()
   ModuleScanTimer.Enabled = False
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
   Call PropBag.WriteProperty("Font", m_Font, Ambient.Font)
   Call PropBag.WriteProperty("Enabled", m_Enabled, m_def_Enabled)
   Call PropBag.WriteProperty("Connected", m_Connected, m_def_Connected)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,True
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
   Enabled = m_Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
   m_Enabled = New_Enabled
   PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,1,1,False
Public Property Get Connected() As Boolean
   UpdateModuleComboBox
   Connected = m_Connected
End Property

Public Property Let Connected(ByVal New_Connected As Boolean)
   If Ambient.UserMode = False Then Err.Raise 387
   If Ambient.UserMode Then Err.Raise 382
   m_Connected = New_Connected
   PropertyChanged "Connected"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,1,1,
Public Property Get devName() As String
Attribute devName.VB_Description = "A read only string containing the QuickUSB device name"
   devName = m_devName
End Property

Public Property Let devName(ByVal New_devName As String)
   If Ambient.UserMode = False Then Err.Raise 387
   If Ambient.UserMode Then Err.Raise 382
   m_devName = New_devName
   PropertyChanged "devName"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function About() As Variant
   frmAbout.Show vbModal
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WritePort(ByVal portNum As Integer, ByVal value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbWritePort(hDevice, portNum, value, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadPort(ByVal portNum As Integer, ByRef value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbReadPort(hDevice, portNum, value, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WritePortDir(ByVal portNum As Integer, ByVal direction As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, portNum, direction)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadPortDir(ByVal portNum As Integer, ByRef direction As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, portNum, direction)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadSPI(ByVal regAddr As Byte, ByRef value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim data(1) As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   result = QuickUsbReadSpi(hDevice, regAddr, data(0), 1)
   value = data(0)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WriteSPI(ByVal regAddr As Byte, ByVal value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim data(1) As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   data(0) = value
   result = QuickUsbWriteSpi(hDevice, regAddr, data(0), 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function



Public Function ReadI2C(ByVal addr As Integer, ByRef data() As Byte, ByVal startIndex As Integer, ByRef length As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   result = QuickUsbReadI2C(hDevice, addr, data(startIndex), length)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function



Public Function WriteI2C(ByVal addr As Integer, ByRef data() As Byte, ByVal startIndex As Integer, ByRef length As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   result = QuickUsbWriteI2C(hDevice, addr, data(startIndex), length)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function



Public Function WriteStorageLong(ByVal longAddress As Integer, ByRef data() As Long, ByVal startIndex As Integer, ByRef numLongsToWrite As Integer) As Integer
   Dim payload() As Byte
   Dim memAddr As Integer
   Dim result As Integer
   Dim value As Integer
   Dim page As Integer
   Dim numBytes As Integer
   Dim numLongs As Integer
   Dim numLongsWritten As Integer
   
   numLongsWritten = 0
   Do
      ' Create an array to hold the data we write to the I2C
      numLongs = Min(numLongsToWrite - numLongsWritten, EEPROM_PAGE_LENGTH \ 4)
      numBytes = 2 + (numLongs * 4)
      ReDim payload(numBytes - 1)
      
      ' Build the payload packet
      ' Dest address in first two bytes (MSB, LSB)
      ' followed by the payload data
      memAddr = EEPROM_STORAGE_BASE + ((longAddress + numLongsWritten) * 4)
      payload(0) = memAddr \ &H100
      payload(1) = memAddr And &HFF
      CopyMemory payload(2), data(startIndex + numLongsWritten), numBytes - 2
      
      ' Actually write data to the EEPROM
      ' Note: You need to send the address header for each write operation
      ' Just be sure to increment the dest address for each write
      Debug.Print "Writing " & numLongs & " longs to " & Hex(memAddr \ 4)
      result = WriteI2C(EEPROM_WRITE_ADDR, payload, 0, numBytes)
      
      ' Read until the write has completed
      Do
         ' Tell the EEPROM the address we want to start reading at
         result = WriteI2C(EEPROM_READ_ADDR, payload, 0, 2)
         ReadSetting SETTING_I2CTL, value
      Loop While value \ &H100 = 7
      
      ' Increment the counter
      numLongsWritten = numLongsWritten + numLongs
      
   Loop While numLongsWritten < numLongsToWrite
   Debug.Print "Wrote " & numLongsWritten & " out of "; numLongsToWrite
   
   WriteStorageLong = result
End Function



Public Function ReadStorageLong(ByVal longAddress As Integer, ByRef data() As Long, ByVal startIndex As Integer, ByRef numLongsToRead As Integer) As Integer
   Dim payload() As Byte
   Dim memAddr As Integer
   Dim result As Integer
   Dim value As Integer
   Dim numBytes As Integer
   Dim numLongs As Integer
   Dim numLongsRead As Integer
   
   numLongsRead = 0
   Do
      ' Create an array to hold the data
      numLongs = Min(numLongsToRead - numLongsRead, EEPROM_PAGE_LENGTH \ 4)
      numBytes = numLongs * 4
      ReDim payload(numBytes - 1)
      
      ' Build the payload packet
      memAddr = EEPROM_STORAGE_BASE + ((longAddress + numLongsRead) * 4)
      payload(0) = memAddr \ &H100
      payload(1) = memAddr And &HFF
      
      ' Set the starting address of the read
      Debug.Print "Reading " & numLongs & " longs from " & Hex(memAddr \ 4)
      result = WriteI2C(EEPROM_WRITE_ADDR, payload, 0, 2)
      
      ' Read the data from the EEPROM
      ' Note: The chip will continuously stream data out as long as we keep reading
      result = ReadI2C(EEPROM_READ_ADDR, payload, 0, numBytes)
      CopyMemory data(startIndex + numLongsRead), payload(0), numBytes
      
      ' Increment the counter
      numLongsRead = numLongsRead + numLongs
      
   Loop While numLongsRead < numLongsToRead
   Debug.Print "Read " & numLongsRead & " out of "; numLongsToRead
   
   ReadStorageLong = result
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Refresh
Public Sub Refresh()
Attribute Refresh.VB_Description = "Forces a complete repaint of a object."
   UserControl.Refresh
End Sub

