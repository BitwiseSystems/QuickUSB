VERSION 5.00
Begin VB.UserControl QuickUsbCtl 
   ClientHeight    =   375
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   7350
   PropertyPages   =   "QuickUsbCtl.ctx":0000
   ScaleHeight     =   375
   ScaleWidth      =   7350
   ToolboxBitmap   =   "QuickUsbCtl.ctx":0013
   Begin VB.Timer ModuleScanTimer 
      Enabled         =   0   'False
      Interval        =   3000
      Left            =   4140
      Top             =   0
   End
   Begin VB.TextBox ModuleStringDescriptorTextBox 
      BackColor       =   &H8000000F&
      Height          =   315
      Left            =   3360
      Locked          =   -1  'True
      TabIndex        =   1
      Top             =   0
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
      Left            =   2400
      TabIndex        =   3
      Top             =   60
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
' Title        : QuickUSBCtl.bas
' Description  : QuickUSB ActiveX User Control for QuickUSB
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
' (1998), and FAR 12.212, as applicable.  Bitwise Systems, 697 Via Miguel
' Santa Barbara, CA  93111.
'
' Bitwise Systems
' 5385 Hollister Ave, Suite 215
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: $
'=============================================================================
Option Explicit


Public StringDescriptor As String

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


Private Sub UserControl_Initialize()
'   UpdateModuleComboBox
   ModuleScanTimer.Enabled = True
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
   devList = "*.*" & Space(128)
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
   If (ModuleComboBox.ListCount > 0 And devList = oldDevList And StringDescriptor <> "") Then
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
   If Mid(StringDescriptor, 1, 1) <> Chr(0) Then
      m_Connected = True
   End If
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
   UpdateModuleComboBox
End Sub



Private Sub ModuleComboBox_Click()
   m_devName = ModuleComboBox.List(ModuleComboBox.ListIndex)
   If (ModuleComboBox.Enabled) Then
      UpdateStringDescriptorTextBox
   End If
End Sub
'
'
'Public Property Get devName() As String
'   devName = m_devName
'End Property


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
   str = Space(80)
   result = QuickUsbGetStringDescriptor(hDevice, 2, str, Len(str))
   If (result = 0) Then
      QuickUsbClose hDevice
      StringDescriptor = ""
      GetStringDescriptor = ""
      Exit Function
   End If
      
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   StringDescriptor = str
   GetStringDescriptor = str
End Function



Public Function WriteSetting(ByVal address As Integer, ByVal Value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      WriteSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbWriteSetting(hDevice, address, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteSetting = 0
      Exit Function
   End If
End Function



Public Function ReadSetting(ByVal address As Integer, ByRef Value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, address, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadSetting = 0
      Exit Function
   End If
End Function



Public Function ReadCommand(ByVal address As Integer, Data() As Integer, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadCommand(hDevice, address, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadCommand = 0
      Exit Function
   End If
End Function



Public Function WriteCommand(ByVal address As Integer, Data() As Integer, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteCommand(hDevice, address, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteCommand = 0
      Exit Function
   End If
End Function



Public Function ReadData(Data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      ReadData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadData(hDevice, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadData = 0
      Exit Function
   End If
   
   ReadData = result
End Function



Public Function WriteData(Data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim Index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      WriteData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteData(hDevice, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteData = 0
      Exit Function
   End If
   
   WriteData = result
End Function



Public Function SetRS232BaudRate(ByVal baudRate As Long) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
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



Public Function ReadRS232(ByVal portNum As Byte, ByRef Data() As Byte, Optional maxLen As Variant) As Long
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
   If (length > UBound(Data) + (1 - LBound(Data))) Then
      length = UBound(Data) + (1 - LBound(Data))
   End If
   
   ' Stuff the string with nulls
   str = String(length, 0)
   
   ' Read length characters from the RS232 port
   result = QuickUsbReadRS232(hDevice, portNum, Data(0), length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
      
   ' Check the transaction result
   If (result = 0) Then
      ReadRS232 = False
      Exit Function
   End If
   
   ReadRS232 = True
End Function



Public Function WriteRS232(ByVal portNum As Byte, ByRef Data() As Byte) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      WriteRS232 = False
      Exit Function
   End If
      
   result = QuickUsbWriteRS232(hDevice, portNum, Data(0), UBound(Data) + (1 - LBound(Data)))
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      WriteRS232 = False
      Exit Function
   End If
   
   WriteRS232 = True
End Function



Public Function UploadFPGA(filename As String) As Boolean
   Dim Index As Long
   Dim result As Long
   Dim length As Integer
   Dim isConfigured As Integer
   Dim fileLength As Long
   Dim blockLength As Integer
   Dim hDevice As Long
   Dim fpgaData() As Byte
   Dim inputFile As Integer
   
   ' Allocate storage for data elements
   blockLength = 64
   ReDim fpgaData(blockLength)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      UploadFPGA = False
      Exit Function
   End If
   
   ' Open the input file for reading
   inputFile = FreeFile
   Open filename For Binary As inputFile
   fileLength = LOF(inputFile)
   
   ' Start configuring the FPGA
   result = QuickUsbStartFpgaConfiguration(hDevice)
   If (result = 0) Then
      Close inputFile
      QuickUsbClose hDevice
      UploadFPGA = False
      Exit Function
   End If
   
   ' Send the FPGA configuration data one block at a time
   Index = 0
   Do
      ' Read the data
      length = Min(blockLength, fileLength - Index)
      
      'Read the data from the file
      Get inputFile, Index + 1, fpgaData
            
      ' Write the data to the EEPROM
      result = QuickUsbWriteFpgaData(hDevice, fpgaData(0), length)
      If (result = 0) Then
         Exit Do
      End If
      
      ' Update the counters
      Index = Index + length
            
      ' Redraw the screen
      DoEvents
   Loop Until Index >= fileLength
      
   ' Close the file
   Close inputFile
   
   ' Start configuring the FPGA
   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Free the memory
   ReDim fpgaData(0)
   
   ' Notify the user
   If (isConfigured = 0) Then
      UploadFPGA = False
   Else
      UploadFPGA = True
   End If
End Function



Public Function IsFpgaConfigured() As Boolean
   Dim result As Long
   Dim isConfigured As Integer
   Dim hDevice As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
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
'   m_Connected = PropBag.ReadProperty("Connected", m_def_Connected)
End Sub

Private Sub UserControl_Resize()
   ModuleStringDescriptorTextBox.Width = UserControl.Width - (ModuleStringDescriptorTextBox.Left + 50)
End Sub

Private Sub UserControl_Show()
   UpdateModuleComboBox
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
Public Function ReadTimerReg(ByVal Index As Integer, ByRef Value As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadTimerReg = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
      
   result = QuickUsbReadTimerReg(hDevice, Index, Value)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WriteTimerReg(ByVal Index As Integer, ByVal Value As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
      
   ' Check for a selected device
   If (m_devName = "") Then
      WriteTimerReg = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
         
   ' Actually write the text out the timer register
   result = QuickUsbWriteTimerReg(hDevice, Index, Value)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadTimerIsrRegs(values() As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadTimerIsrRegs = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
      
   length = 2 * (UBound(values) + (1 - LBound(values)))
   result = QuickUsbReadTimerIsrRegs(hDevice, values(0), length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function


'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WritePort(ByVal portNum As Integer, ByVal Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      WritePort = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WritePort = False
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbWritePort(hDevice, portNum, Value, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WritePort = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadPort(ByVal portNum As Integer, ByRef Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadPort = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadPort = False
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbReadPort(hDevice, portNum, Value, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadPort = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WritePortDir(ByVal portNum As Integer, ByVal direction As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      WritePortDir = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WritePortDir = False
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, portNum, direction)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WritePortDir = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadPortDir(ByVal portNum As Integer, ByRef direction As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadPortDir = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadPortDir = False
      Exit Function
   End If
   
   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, portNum, direction)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadPortDir = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadSPI(ByVal regAddr As Byte, ByRef Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadSPI = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadSPI = False
      Exit Function
   End If
   
   result = QuickUsbReadSpi(hDevice, regAddr, Data(0), 1)
   Value = Data(0)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadSPI = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WriteSPI(ByVal regAddr As Byte, ByVal Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Check for a selected device
   If (m_devName = "") Then
      WriteSPI = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteSPI = False
      Exit Function
   End If
   
   Data(0) = Value
   result = QuickUsbWriteSpi(hDevice, regAddr, Data(0), 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteSPI = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadI2C(ByVal regAddr As Byte, ByRef Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Check for a selected device
   If (m_devName = "") Then
      ReadI2C = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ReadI2C = False
      Exit Function
   End If
   
   result = QuickUsbReadI2C(hDevice, regAddr, Data(0), 1)
   Value = Data(0)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ReadI2C = False
      Exit Function
   End If
End Function



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WriteI2C(ByVal regAddr As Byte, ByVal Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Check for a selected device
   If (m_devName = "") Then
      WriteI2C = False
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteI2C = False
      Exit Function
   End If
   
   Data(0) = Value
   result = QuickUsbWriteI2C(hDevice, regAddr, Data(0), 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      WriteI2C = False
      Exit Function
   End If
End Function


