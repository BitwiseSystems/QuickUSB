VERSION 5.00
Begin VB.UserControl QuickUsbCtl 
   ClientHeight    =   765
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4995
   PropertyPages   =   "QuickUsbCtl.ctx":0000
   ScaleHeight     =   765
   ScaleWidth      =   4995
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      GetStringDescriptor = ""
      Exit Function
   End If
   
   ' Read the string descriptor
   str = Space(80)
   result = QuickUsbGetStringDescriptor(hDevice, 2, str, Len(str))
   If (result = 0) Then
      QuickUsbClose hDevice
      MsgBox "Cannot read string descriptor", vbCritical + vbOKOnly, Name
      GetStringDescriptor = ""
      Exit Function
   End If
      
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   GetStringDescriptor = str
End Function



Public Function WriteSetting(ByVal address As Integer, ByVal value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      WriteSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      WriteSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbWriteSetting(hDevice, address, value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write setting", vbCritical + vbOKOnly, Name
      WriteSetting = 0
      Exit Function
   End If
End Function



Public Function ReadSetting(ByVal address As Integer, ByRef value As Integer) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      ReadSetting = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      ReadSetting = 0
      Exit Function
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, address, value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot read setting", vbCritical + vbOKOnly, Name
      ReadSetting = 0
      Exit Function
   End If
End Function



Public Function ReadCommand(ByVal address As Integer, Data() As Integer, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      ReadCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      ReadCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadCommand(hDevice, address, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot read command", vbCritical + vbOKOnly, Name
      ReadCommand = 0
      Exit Function
   End If
End Function



Public Function WriteCommand(ByVal address As Integer, Data() As Integer, ByVal startIndex As Integer, ByVal numBytes As Integer) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      WriteCommand = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      WriteCommand = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteCommand(hDevice, address, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write command", vbCritical + vbOKOnly, Name
      WriteCommand = 0
      Exit Function
   End If
End Function



Public Function ReadData(Data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      ReadData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      ReadData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbReadData(hDevice, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot read data", vbCritical + vbOKOnly, Name
      ReadData = 0
      Exit Function
   End If
End Function



Public Function WriteData(Data() As Byte, ByVal startIndex As Long, ByVal numBytes As Long) As Long
   Dim index As Long
   Dim result As Long
   Dim hDevice As Long
          
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      WriteData = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      WriteData = 0
      Exit Function
   End If
   
   ' Write the data
   result = QuickUsbWriteData(hDevice, Data(startIndex), numBytes)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write data", vbCritical + vbOKOnly, Name
      WriteData = 0
      Exit Function
   End If
End Function



Public Function SetRS232BaudRate(ByVal baudRate As Long) As Long
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName, vbCritical + vbOKOnly, Name
      SetRS232BaudRate = False
      Exit Function
   End If
   
   result = QuickUsbSetRS232BaudRate(hDevice, baudRate)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot set RS232 baud rate", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      FlushRS232 = False
      Exit Function
   End If
   
   ' Read length characters from the RS232 port
   result = QuickUsbFlushRS232(hDevice, portNum)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot flush RS232", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      GetNumRS232 = False
      Exit Function
   End If
   
   result = QuickUsbGetNumRS232(hDevice, portNum, length)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot get number of RS232 bytes outstanding", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      ReadRS232String = ""
      Exit Function
   End If

   ' If the user didn't define the max length, just read what's in the buffer
   If IsMissing(maxLen) Then
      ' Find out how many characters are waiting for us
      result = QuickUsbGetNumRS232(hDevice, portNum, length)
      If (result = 0) Then
         QuickUsbClose hDevice
         MsgBox "Cannot read number of RS232 characters waiting", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot read string from RS232", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      WriteRS232String = False
      Exit Function
   End If
      
   result = QuickUsbWriteRS232String(hDevice, portNum, str, Len(str))
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write string to RS232", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      ReadRS232 = ""
      Exit Function
   End If

   ' If the user didn't define the max length, just read what's in the buffer
   If IsMissing(maxLen) Then
      ' Find out how many characters are waiting for us
      result = QuickUsbGetNumRS232(hDevice, portNum, length)
      If (result = 0) Then
         QuickUsbClose hDevice
         MsgBox "Cannot read number of RS232 bytes waiting", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot read bytes from RS232", vbCritical + vbOKOnly, Name
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
      MsgBox "Cannot open Device:" & m_devName, vbCritical + vbOKOnly, Name
      WriteRS232 = False
      Exit Function
   End If
      
   result = QuickUsbWriteRS232(hDevice, portNum, Data(0), UBound(Data) + (1 - LBound(Data)))
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write string to RS232", vbCritical + vbOKOnly, Name
      WriteRS232 = False
      Exit Function
   End If
   
   WriteRS232 = True
End Function



Public Function UploadFPGA(fileName As String) As Boolean
   Dim index As Long
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
      MsgBox "Cannot open Device:" & devName, , "Upload FPGA"
      UploadFPGA = False
      Exit Function
   End If
   
   ' Open the input file for reading
   inputFile = FreeFile
   Open fileName For Binary As inputFile
   fileLength = LOF(inputFile)
   
   ' Start configuring the FPGA
   result = QuickUsbStartFpgaConfiguration(hDevice)
   If (result = 0) Then
      MsgBox "Cannot start FPGA configuration", , "Upload FPGA"
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
   Loop Until index >= fileLength
      
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
      MsgBox "Cannot open Device:" & devName, , "Upload FPGA"
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
Public Function About() As Variant
   frmAbout.Show vbModal
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function ReadTimerReg(ByVal index As Integer, ByRef value As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      ReadTimerReg = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
      
   result = QuickUsbReadTimerReg(hDevice, index, value)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function WriteTimerReg(ByVal index As Integer, ByVal value As Integer) As Variant
   Dim hDevice As Long
   Dim result As Long
      
   ' Check for a selected device
   If (m_devName = "") Then
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
      WriteTimerReg = 0
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
         
   ' Actually write the text out the timer register
   result = QuickUsbWriteTimerReg(hDevice, index, value)
   
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
      MsgBox "Please select a QuickUSB module", vbCritical + vbOKOnly, Name
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
Public Function WritePort(ByVal portNum As Integer, ByVal value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
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
   result = QuickUsbOpen(hDevice, devName)
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
   result = QuickUsbOpen(hDevice, devName)
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
   result = QuickUsbOpen(hDevice, devName)
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

