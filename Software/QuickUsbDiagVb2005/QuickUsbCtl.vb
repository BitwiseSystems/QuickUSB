'=============================================================================
' Title        : QuickUsbCtl.vb
' Description  : QuickUSB Library Function Declarations
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
' History      :
'   Date       Name        Description
'  5/5/2002   bhenry      Initial coding
'
' Copyright © 2006 Bitwise Systems.  All rights reserved.
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
' 6469 Calle Real, Suite E
' Goleta, CA  93117
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: QuickUsbCtl.bas $
'
'=============================================================================
Imports System.Runtime.InteropServices

<ComClass(QuickUsbCtl.ClassId, QuickUsbCtl.InterfaceId, QuickUsbCtl.EventsId)> Public Class QuickUsbCtl
   Inherits System.Windows.Forms.UserControl

#Region "COM GUIDs"
   ' These  GUIDs provide the COM identity for this class 
   ' and its COM interfaces. If you change them, existing 
   ' clients will no longer be able to access the class.
   Public Const ClassId As String = "EB3A7DBD-F9FE-4F0E-B575-C253B6F8B9A6"
   Public Const InterfaceId As String = "6858F691-BC66-4151-A2BB-CEF5FD3AAEEA"
   Public Const EventsId As String = "93962C09-5BDA-4D40-BB00-D95082D21781"
#End Region

#Region " Windows Form Designer generated code "

   Public Sub New()
      MyBase.New()

      'This call is required by the Windows Form Designer.
      InitializeComponent()

      'Add any initialization after the InitializeComponent() call
      Initialize()
   End Sub

   'UserControl overrides dispose to clean up the component list.
   Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
      If disposing Then
         If Not (components Is Nothing) Then
            components.Dispose()
         End If
      End If
      MyBase.Dispose(disposing)
   End Sub

   'Required by the Windows Form Designer
   Private components As System.ComponentModel.IContainer

   'NOTE: The following procedure is required by the Windows Form Designer
   'It can be modified using the Windows Form Designer.  
   'Do not modify it using the code editor.
   Friend WithEvents ModuleComboBox As System.Windows.Forms.ComboBox
   Friend WithEvents ModuleStringDescriptorTextBox As System.Windows.Forms.TextBox
   Friend WithEvents ModuleScanTimer As System.Windows.Forms.Timer

   <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
      Me.components = New System.ComponentModel.Container
      Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(QuickUsbCtl))
      Me.ModuleComboBox = New System.Windows.Forms.ComboBox
      Me.ModuleStringDescriptorTextBox = New System.Windows.Forms.TextBox
      Me.ModuleScanTimer = New System.Windows.Forms.Timer(Me.components)
      Me.SuspendLayout()
      '
      'ModuleComboBox
      '
      Me.ModuleComboBox.AccessibleDescription = resources.GetString("ModuleComboBox.AccessibleDescription")
      Me.ModuleComboBox.AccessibleName = resources.GetString("ModuleComboBox.AccessibleName")
      Me.ModuleComboBox.Anchor = CType(resources.GetObject("ModuleComboBox.Anchor"), System.Windows.Forms.AnchorStyles)
      Me.ModuleComboBox.BackgroundImage = CType(resources.GetObject("ModuleComboBox.BackgroundImage"), System.Drawing.Image)
      Me.ModuleComboBox.Dock = CType(resources.GetObject("ModuleComboBox.Dock"), System.Windows.Forms.DockStyle)
      Me.ModuleComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
      Me.ModuleComboBox.Enabled = CType(resources.GetObject("ModuleComboBox.Enabled"), Boolean)
      Me.ModuleComboBox.Font = CType(resources.GetObject("ModuleComboBox.Font"), System.Drawing.Font)
      Me.ModuleComboBox.ImeMode = CType(resources.GetObject("ModuleComboBox.ImeMode"), System.Windows.Forms.ImeMode)
      Me.ModuleComboBox.IntegralHeight = CType(resources.GetObject("ModuleComboBox.IntegralHeight"), Boolean)
      Me.ModuleComboBox.ItemHeight = CType(resources.GetObject("ModuleComboBox.ItemHeight"), Integer)
      Me.ModuleComboBox.Location = CType(resources.GetObject("ModuleComboBox.Location"), System.Drawing.Point)
      Me.ModuleComboBox.MaxDropDownItems = CType(resources.GetObject("ModuleComboBox.MaxDropDownItems"), Integer)
      Me.ModuleComboBox.MaxLength = CType(resources.GetObject("ModuleComboBox.MaxLength"), Integer)
      Me.ModuleComboBox.Name = "ModuleComboBox"
      Me.ModuleComboBox.RightToLeft = CType(resources.GetObject("ModuleComboBox.RightToLeft"), System.Windows.Forms.RightToLeft)
      Me.ModuleComboBox.Size = CType(resources.GetObject("ModuleComboBox.Size"), System.Drawing.Size)
      Me.ModuleComboBox.TabIndex = CType(resources.GetObject("ModuleComboBox.TabIndex"), Integer)
      Me.ModuleComboBox.Text = resources.GetString("ModuleComboBox.Text")
      Me.ModuleComboBox.Visible = CType(resources.GetObject("ModuleComboBox.Visible"), Boolean)
      '
      'ModuleStringDescriptorTextBox
      '
      Me.ModuleStringDescriptorTextBox.AccessibleDescription = resources.GetString("ModuleStringDescriptorTextBox.AccessibleDescription")
      Me.ModuleStringDescriptorTextBox.AccessibleName = resources.GetString("ModuleStringDescriptorTextBox.AccessibleName")
      Me.ModuleStringDescriptorTextBox.Anchor = CType(resources.GetObject("ModuleStringDescriptorTextBox.Anchor"), System.Windows.Forms.AnchorStyles)
      Me.ModuleStringDescriptorTextBox.AutoSize = CType(resources.GetObject("ModuleStringDescriptorTextBox.AutoSize"), Boolean)
      Me.ModuleStringDescriptorTextBox.BackColor = System.Drawing.SystemColors.Control
      Me.ModuleStringDescriptorTextBox.BackgroundImage = CType(resources.GetObject("ModuleStringDescriptorTextBox.BackgroundImage"), System.Drawing.Image)
      Me.ModuleStringDescriptorTextBox.Dock = CType(resources.GetObject("ModuleStringDescriptorTextBox.Dock"), System.Windows.Forms.DockStyle)
      Me.ModuleStringDescriptorTextBox.Enabled = CType(resources.GetObject("ModuleStringDescriptorTextBox.Enabled"), Boolean)
      Me.ModuleStringDescriptorTextBox.Font = CType(resources.GetObject("ModuleStringDescriptorTextBox.Font"), System.Drawing.Font)
      Me.ModuleStringDescriptorTextBox.ImeMode = CType(resources.GetObject("ModuleStringDescriptorTextBox.ImeMode"), System.Windows.Forms.ImeMode)
      Me.ModuleStringDescriptorTextBox.Location = CType(resources.GetObject("ModuleStringDescriptorTextBox.Location"), System.Drawing.Point)
      Me.ModuleStringDescriptorTextBox.MaxLength = CType(resources.GetObject("ModuleStringDescriptorTextBox.MaxLength"), Integer)
      Me.ModuleStringDescriptorTextBox.Multiline = CType(resources.GetObject("ModuleStringDescriptorTextBox.Multiline"), Boolean)
      Me.ModuleStringDescriptorTextBox.Name = "ModuleStringDescriptorTextBox"
      Me.ModuleStringDescriptorTextBox.PasswordChar = CType(resources.GetObject("ModuleStringDescriptorTextBox.PasswordChar"), Char)
      Me.ModuleStringDescriptorTextBox.RightToLeft = CType(resources.GetObject("ModuleStringDescriptorTextBox.RightToLeft"), System.Windows.Forms.RightToLeft)
      Me.ModuleStringDescriptorTextBox.ScrollBars = CType(resources.GetObject("ModuleStringDescriptorTextBox.ScrollBars"), System.Windows.Forms.ScrollBars)
      Me.ModuleStringDescriptorTextBox.Size = CType(resources.GetObject("ModuleStringDescriptorTextBox.Size"), System.Drawing.Size)
      Me.ModuleStringDescriptorTextBox.TabIndex = CType(resources.GetObject("ModuleStringDescriptorTextBox.TabIndex"), Integer)
      Me.ModuleStringDescriptorTextBox.TabStop = False
      Me.ModuleStringDescriptorTextBox.Text = resources.GetString("ModuleStringDescriptorTextBox.Text")
      Me.ModuleStringDescriptorTextBox.TextAlign = CType(resources.GetObject("ModuleStringDescriptorTextBox.TextAlign"), System.Windows.Forms.HorizontalAlignment)
      Me.ModuleStringDescriptorTextBox.Visible = CType(resources.GetObject("ModuleStringDescriptorTextBox.Visible"), Boolean)
      Me.ModuleStringDescriptorTextBox.WordWrap = CType(resources.GetObject("ModuleStringDescriptorTextBox.WordWrap"), Boolean)
      '
      'ModuleScanTimer
      '
      Me.ModuleScanTimer.Interval = 1000
      '
      'QuickUsbCtl
      '
      Me.AccessibleDescription = resources.GetString("$this.AccessibleDescription")
      Me.AccessibleName = resources.GetString("$this.AccessibleName")
      Me.AutoScroll = CType(resources.GetObject("$this.AutoScroll"), Boolean)
      Me.AutoScrollMargin = CType(resources.GetObject("$this.AutoScrollMargin"), System.Drawing.Size)
      Me.AutoScrollMinSize = CType(resources.GetObject("$this.AutoScrollMinSize"), System.Drawing.Size)
      Me.BackgroundImage = CType(resources.GetObject("$this.BackgroundImage"), System.Drawing.Image)
      Me.Controls.Add(Me.ModuleStringDescriptorTextBox)
      Me.Controls.Add(Me.ModuleComboBox)
      Me.Enabled = CType(resources.GetObject("$this.Enabled"), Boolean)
      Me.Font = CType(resources.GetObject("$this.Font"), System.Drawing.Font)
      Me.ImeMode = CType(resources.GetObject("$this.ImeMode"), System.Windows.Forms.ImeMode)
      Me.Location = CType(resources.GetObject("$this.Location"), System.Drawing.Point)
      Me.Name = "QuickUsbCtl"
      Me.RightToLeft = CType(resources.GetObject("$this.RightToLeft"), System.Windows.Forms.RightToLeft)
      Me.Size = CType(resources.GetObject("$this.Size"), System.Drawing.Size)
      Me.ResumeLayout(False)

   End Sub

#End Region
   '=============================================================================
   ' Public Events
   '=============================================================================
   Public Event ConnectionChanged(ByVal Connected As Boolean)

   '=============================================================================
   ' Variables
   '=============================================================================
   Dim m_devName As String
   Dim m_Connected As Boolean
   Dim m_Enabled As Boolean


   Private Sub Initialize()
      ModuleScanTimer.Enabled = True
   End Sub


   Protected Overrides Sub OnResize(ByVal e As System.EventArgs)
      ModuleStringDescriptorTextBox.Width = Width - 5
   End Sub


   Private Sub UserControl_Show()
      UpdateModuleComboBox()
      ModuleComboBox.Refresh()
   End Sub



   Private Sub UserControl_Terminate()
      ModuleScanTimer.Enabled = False
   End Sub



   Public Property Connected() As Boolean
      Get
         UpdateModuleComboBox()
         Return m_Connected
      End Get
      Set(ByVal Value As Boolean)
         m_Connected = Value
      End Set
   End Property



   Public Property devName() As String
      Get
         Return m_devName
      End Get
      Set(ByVal Value As String)
         m_devName = Value
      End Set
   End Property



   Public Shared ReadOnly Property GetDevList() As String()
      Get
         Dim nameList(1024) As Byte
         Dim index As Short
         Dim modNum As Short
         Dim devName As String
         Dim devList(0) As String

         ' Call the DLL to get the name list
         QuickUsbFindModules(nameList, 1024)

         ' Parse out the module names and return an array of strings containing them
         index = 0
         modNum = 0
         Do Until nameList(index) = 0
            ' Extract the null terminated device name
            devName = ""
            Do
               devName = devName & Chr(nameList(index))
               index = index + 1
            Loop Until nameList(index) = 0

            ' Advance to the next name
            ReDim Preserve devList(modNum)
            devList(modNum) = devName
            modNum = modNum + 1
            index = index + 1
         Loop
         Return devList
      End Get
   End Property



   Public Shared ReadOnly Property LastError() As String
      Get
         Dim errCode As Integer
         Dim errStr As String

         ' Call the DLL to get the name list
         QuickUsbGetLastError(errCode)

         Select Case errCode
            Case 0
               errStr = ""
            Case 1
               errStr = "Out of memory"
            Case 2
               errStr = "Cannot open module"
            Case 3
               errStr = "Cannot find device"
            Case 4
               errStr = "IOCTL failed"
            Case 5
               errStr = "Invalid parameter"
            Case 6
               errStr = "Timeout"
            Case 7
               errStr = "Unsupported function"
            Case Else
               errStr = "Unknown error"
         End Select

         Return errStr
      End Get
   End Property



   Private Function UpdateModuleComboBox() As Integer
      ' Update the module combo box with the a list of all the connected modules
      Dim nameList(1024) As Byte
      Dim index As Short
      Dim modNum As Short
      Dim devName As String
      Dim devList() As String
      Static oldDevListLength As Short

      devList = GetDevList
      If (devList Is Nothing) Then
         ModuleComboBox.Items.Clear()
         ModuleStringDescriptorTextBox.Text = ""
         m_Connected = False
         RaiseEvent ConnectionChanged(m_Connected)
         Return 0
      End If

      ' If the list has items and it hasn't changed, just exit
      If (ModuleComboBox.Items.Count > 0 And devList.Length = oldDevListLength) Then
         Return ModuleComboBox.Items.Count
      End If

      ' Clear the list and add the modules to the listbox
      ModuleComboBox.Items.Clear()
      ModuleComboBox.Items.AddRange(devList)
      oldDevListLength = devList.Length
      ModuleComboBox.SelectedIndex = 0
      m_devName = devList(0)

      ' Enable all other controls
      On Error Resume Next
      For index = 0 To Controls.Count - 1
         Controls(index).Enabled = True
      Next

      ' Update the descriptor string
      UpdateStringDescriptorTextBox()

      ' Raise an event
      m_Connected = True
      RaiseEvent ConnectionChanged(m_Connected)

      ' Return the number of modules
      Return modNum
   End Function


   Private Sub UpdateStringDescriptorTextBox()
      Dim str As String

      str = GetStringDescriptor()
      If (str <> "") Then
         ModuleStringDescriptorTextBox.Text = str
      End If
   End Sub


   Private Sub ModuleScanTimer_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ModuleScanTimer.Tick
      UpdateModuleComboBox()
   End Sub


   Private Sub ModuleComboBox_Click()
      m_devName = ModuleComboBox.Items(ModuleComboBox.SelectedItem)
      If (ModuleComboBox.Enabled) Then
         UpdateStringDescriptorTextBox()
      End If
   End Sub


   Public Function ModuleCount() As Integer
      ModuleCount = ModuleComboBox.Items.Count
   End Function



   Public Sub SelectModule(ByVal moduleIndex As Integer)
      If (moduleIndex < ModuleComboBox.Items.Count) Then
         ModuleComboBox.SelectedItem = moduleIndex
         ModuleComboBox_Click()
      End If
   End Sub



   Public Function GetStringDescriptor() As String
      Dim hDevice As Integer
      Dim result As Integer
      Dim str As String

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return ""
      End If

      ' Read the string descriptor
      str = Space(255)
      result = QuickUsbGetStringDescriptor(hDevice, 2, str, Len(str))
      If (result = 0) Then
         QuickUsbClose(hDevice)
         Return ""
      End If

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return str
   End Function



   Public Function WriteSetting(ByVal address As Integer, ByVal Value As Integer) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Set the new setting value
      result = QuickUsbWriteSetting(hDevice, address, Value)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadSetting(ByVal address As Integer, ByRef Value As Integer) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
         Exit Function
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Set the new setting value
      result = QuickUsbReadSetting(hDevice, address, Value)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadCommand(ByVal address As Short, ByRef Data() As Byte, ByRef numBytes As Short) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      result = QuickUsbReadCommand(hDevice, address, Data, numBytes)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteCommand(ByVal address As Short, ByVal Data() As Byte, ByVal numBytes As Short) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
         Exit Function
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      result = QuickUsbWriteCommand(hDevice, address, Data, numBytes)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteCommandShort(ByVal address As Short, ByVal Data() As Short, ByVal numShorts As Short) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
         Exit Function
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      result = QuickUsbWriteCommandShort(hDevice, address, Data, numShorts * 2)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadData(ByRef Data() As Byte, ByRef numBytes As Integer) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
         Exit Function
      End If

      ' Write the data
      result = QuickUsbReadData(hDevice, Data, numBytes)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteData(ByVal Data() As Byte, ByVal numBytes As Integer) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      result = QuickUsbWriteData(hDevice, Data, numBytes)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadDataShort(ByRef Data() As Short, ByVal numShorts As Integer) As Integer
      Dim result As Integer
      Dim hDevice As Integer
      Dim numBytes As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      numBytes = numShorts * 2
      result = QuickUsbReadDataShort(hDevice, Data, numBytes)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteDataShort(ByVal Data() As Short, ByVal numInts As Integer) As Integer
      Dim result As Integer
      Dim hDevice As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Write the data
      result = QuickUsbWriteDataShort(hDevice, Data, numInts * 2)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
         Exit Function
      End If

      Return result
   End Function



   Public Function SetRS232BaudRate(ByVal baudRate As Integer) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbSetRS232BaudRate(hDevice, baudRate)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function FlushRS232(ByVal portNum As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Read length characters from the RS232 port
      result = QuickUsbFlushRS232(hDevice, portNum)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function GetNumRS232(ByVal portNum As Byte, ByRef length As Short) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbGetNumRS232(hDevice, portNum, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadRS232String(ByVal portNum As Byte) As String
      Dim hDevice As Integer
      Dim result As Integer
      Dim length As Short
      Dim str As String = ""

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbGetNumRS232(hDevice, portNum, length)

      ' Check for zero length
      If (length = 0) Then
         QuickUsbClose(hDevice)
         Return 0
      End If

      ' Stuff the string with nulls
      str.PadRight(length, ChrW(0))

      ' Read length characters from the RS232 port
      result = QuickUsbReadRS232String(hDevice, portNum, str, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function



   Public Function WriteRS232String(ByVal portNum As Byte, ByVal str As String) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbWriteRS232String(hDevice, portNum, str, Len(str))

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function



   Public Function ReadRS232(ByVal portNum As Byte, ByRef Data() As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer
      Dim length As Short
      Dim str As String = ""

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' If the user didn't define the max length, just read what's in the buffer
      ' Find out how many characters are waiting for us
      result = QuickUsbGetNumRS232(hDevice, portNum, length)
      If (result = 0) Then
         QuickUsbClose(hDevice)
         Return 0
      End If

      ' Check for zero length
      If (length = 0) Then
         QuickUsbClose(hDevice)
         Return 0
      End If

      ' Don't allow a length longer than the array
      If (length > UBound(Data) + (1 - LBound(Data))) Then
         length = UBound(Data) + (1 - LBound(Data))
      End If

      ' Stuff the string with nulls
      str.PadRight(length, ChrW(0))

      ' Read length characters from the RS232 port
      result = QuickUsbReadRS232(hDevice, portNum, Data, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteRS232(ByVal portNum As Byte, ByRef Data() As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbWriteRS232(hDevice, portNum, Data, UBound(Data) + (1 - LBound(Data)))

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function



   Public Function UploadFPGA(ByVal fileName As String, Optional ByVal status As StatusBar = Nothing) As Boolean
      Dim Index As Long
      Dim result As Long
      Dim length As Integer
      Dim isConfigured As Integer
      Dim fileLength As Long
      Dim blockLength As Integer
      Dim hDevice As Long
      Dim fpgaData() As Byte = {}
      Dim inputFile As Integer

      ' Allocate storage for data elements
      blockLength = 64

      ' Open the input file for reading
      inputFile = FreeFile()

      ' Check the configuration file length
      Try
         fileLength = FileLen(fileName)
      Catch
         status.Text = "Cannot open " + fileName
         Return False
      End Try

      ' Open the FPGA configuration file for reading
      Try
         FileOpen(inputFile, fileName, OpenMode.Binary)
      Catch
         status.Text = "Cannot open " + fileName
         Return False
      End Try

      ' Open the device
      result = QuickUsbOpen(hDevice, devName)
      If (result = 0) Then
         UploadFPGA = False
         Exit Function
      End If

      ' Start configuring the FPGA
      result = QuickUsbStartFpgaConfiguration(hDevice)
      If (result = 0) Then
         FileClose(inputFile)
         QuickUsbClose(hDevice)
         UploadFPGA = False
         Exit Function
      End If

      ' Send the FPGA configuration data one block at a time
      Index = 0
      Do
         ' Read the data
         length = Min(blockLength, fileLength - Index)

         Array.Resize(fpgaData, length)

         'Read the data from the file
         FileGet(inputFile, fpgaData, Index + 1)

         ' Write the data to the EEPROM
         result = QuickUsbWriteFpgaData(hDevice, fpgaData, length)
         If (result = 0) Then
            Exit Do
         End If

         ' Update the counters
         Index = Index + length

         If Not (status Is Nothing) Then
            status.Text = "Configuring FPGA from " & fileName & "..." & CInt((Index / fileLength) * 100) & "%"
            status.Update()
         End If
      Loop Until Index >= fileLength

      ' Close the file
      FileClose(inputFile)

      ' Start configuring the FPGA
      result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Free the memory
      ReDim fpgaData(0)

      ' Notify the user
      If (isConfigured = 0) Then
         If Not (status Is Nothing) Then
            status.Text = "FPGA configuration failed"
         End If
         UploadFPGA = False
      Else
         If Not (status Is Nothing) Then
            status.Text = "FPGA was successfully configured"
         End If
         UploadFPGA = True
      End If
      Exit Function

openError:
      UploadFPGA = False
   End Function



   Public Function IsFpgaConfigured() As Boolean
      Dim result As Integer
      Dim hDevice As Integer
      Dim isConfigured As Short

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Start configuring the FPGA
      result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      If (isConfigured = 0) Then
         Return False
      Else
         Return True
      End If
   End Function



   Public Function WritePort(ByVal portNum As Short, ByVal Value As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer
      Dim valArray(1) As Byte

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Read the port data
      valArray(0) = Value
      result = QuickUsbWritePort(hDevice, portNum, valArray, 1)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function



   Public Function ReadPort(ByVal portNum As Short, ByRef Value As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer
      Dim valArray(1) As Byte

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Read the port data
      result = QuickUsbReadPort(hDevice, portNum, valArray, 1)
      Value = valArray(0)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function



   Public Function WritePortDir(ByVal portNum As Integer, ByVal direction As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      ' Read the port data
      result = QuickUsbWritePortDir(hDevice, portNum, direction)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadPortDir(ByVal portNum As Integer, ByRef direction As Byte) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return result
      End If

      ' Read the port data
      result = QuickUsbReadPortDir(hDevice, portNum, direction)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadSPI(ByVal regAddr As Byte, ByRef Value() As Byte, ByRef length As Short) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbReadSpi(hDevice, regAddr, Value, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteSPI(ByVal regAddr As Byte, ByRef Value() As Byte, ByVal length As Short) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbWriteReadSpi(hDevice, regAddr, Value, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteReadSPI(ByVal regAddr As Byte, ByVal Value() As Byte, ByVal length As Short) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbWriteSpi(hDevice, regAddr, Value, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function ReadI2C(ByVal regAddr As Byte, ByRef Value() As Byte, ByRef length As Short) As Integer
      Dim hDevice As Integer
      Dim result As Integer

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbReadI2C(hDevice, regAddr, Value, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      Return result
   End Function



   Public Function WriteI2C(ByVal regAddr As Byte, ByRef Value() As Byte, ByVal length As Integer) As Object
      Dim hDevice As Long
      Dim result As Long

      ' Check for a selected device
      If (m_devName = "") Then
         Return 0
      End If

      ' Open the device
      result = QuickUsbOpen(hDevice, m_devName)
      If (result = 0) Then
         Return 0
      End If

      result = QuickUsbWriteI2C(hDevice, regAddr, Value, length)

      ' Close the device when you're done
      QuickUsbClose(hDevice)

      ' Check the transaction result
      If (result = 0) Then
         Return 0
      End If

      Return result
   End Function

End Class
