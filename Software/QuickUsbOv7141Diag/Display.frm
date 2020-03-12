VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Display 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB OV7141 Diagnostics"
   ClientHeight    =   9000
   ClientLeft      =   150
   ClientTop       =   750
   ClientWidth     =   12810
   ClipControls    =   0   'False
   BeginProperty Font 
      Name            =   "Microsoft Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Display.frx":0000
   LinkTopic       =   "QuickUsb"
   MaxButton       =   0   'False
   ScaleHeight     =   600
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   854
   StartUpPosition =   3  'Windows Default
   WhatsThisHelp   =   -1  'True
   Begin VB.Timer GrabTimer 
      Interval        =   30
      Left            =   9780
      Top             =   7560
   End
   Begin VB.Timer StatusTimer 
      Interval        =   1000
      Left            =   10680
      Top             =   7560
   End
   Begin VB.Frame GeneralFrame 
      Caption         =   "General"
      Height          =   1275
      Left            =   60
      TabIndex        =   24
      Top             =   0
      Width           =   8715
      Begin QuickUsbOV7141Diag.QuickUsbCtl QuickUsbCtl 
         Height          =   675
         Left            =   120
         TabIndex        =   25
         Top             =   300
         Width           =   8355
         _ExtentX        =   14737
         _ExtentY        =   1191
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Connected       =   -1  'True
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Video"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   675
      Left            =   9780
      TabIndex        =   18
      Top             =   1920
      Width           =   2955
      Begin VB.CheckBox RecordCheckBox 
         Caption         =   "Record"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   1620
         Style           =   1  'Graphical
         TabIndex        =   20
         Top             =   240
         Width           =   975
      End
      Begin VB.CheckBox AcquireCheckBox 
         Caption         =   "Acquire"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   19
         Top             =   240
         Width           =   975
      End
   End
   Begin VB.Frame SpeedFrame 
      Caption         =   "Clock Speed"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   555
      Index           =   0
      Left            =   9780
      TabIndex        =   21
      Top             =   1320
      Width           =   2955
      Begin VB.OptionButton SpeedOptionButton 
         Caption         =   "12MHz"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Index           =   0
         Left            =   420
         TabIndex        =   23
         Top             =   240
         Width           =   795
      End
      Begin VB.OptionButton SpeedOptionButton 
         Caption         =   "24MHz"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Index           =   1
         Left            =   1680
         TabIndex        =   22
         Top             =   240
         Value           =   -1  'True
         Width           =   795
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "SCCB Registers"
      Height          =   4935
      Left            =   9780
      TabIndex        =   1
      Top             =   2580
      Width           =   2955
      Begin VB.Frame Frame5 
         Caption         =   "Brightness, Contrast && EV"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1515
         Left            =   180
         TabIndex        =   9
         Top             =   1320
         Width           =   2595
         Begin MSComctlLib.Slider BrightnessSlider 
            Height          =   315
            Left            =   60
            TabIndex        =   10
            Top             =   300
            Width           =   2475
            _ExtentX        =   4366
            _ExtentY        =   556
            _Version        =   393216
            LargeChange     =   16
            Max             =   255
            SelStart        =   128
            TickFrequency   =   16
            Value           =   128
         End
         Begin MSComctlLib.Slider ContrastSlider 
            Height          =   315
            Left            =   60
            TabIndex        =   11
            Top             =   660
            Width           =   2475
            _ExtentX        =   4366
            _ExtentY        =   556
            _Version        =   393216
            LargeChange     =   16
            Max             =   255
            SelStart        =   128
            TickFrequency   =   16
            Value           =   128
         End
         Begin MSComctlLib.Slider EvSlider 
            Height          =   315
            Left            =   60
            TabIndex        =   12
            Top             =   1020
            Width           =   2475
            _ExtentX        =   4366
            _ExtentY        =   556
            _Version        =   393216
            LargeChange     =   16
            Max             =   255
            SelStart        =   65
            TickFrequency   =   16
            Value           =   65
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "Image Orientation"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   675
         Left            =   180
         TabIndex        =   6
         Top             =   600
         Width           =   2595
         Begin VB.CheckBox RevertCheckBox 
            Caption         =   "Revert"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1440
            TabIndex        =   8
            Top             =   300
            Width           =   795
         End
         Begin VB.CheckBox InvertCheckBox 
            Caption         =   "Invert"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   300
            TabIndex        =   7
            Top             =   300
            Width           =   795
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "Register Explorer"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1935
         Left            =   180
         TabIndex        =   3
         Top             =   2880
         Width           =   2595
         Begin MSComctlLib.Slider WriteValueSlider 
            Height          =   315
            Left            =   60
            TabIndex        =   14
            ToolTipText     =   "Brightness"
            Top             =   1440
            Width           =   2475
            _ExtentX        =   4366
            _ExtentY        =   556
            _Version        =   393216
            Max             =   255
            TickFrequency   =   16
         End
         Begin QuickUsbOV7141Diag.BitCheckBox BitCheckBox1 
            Height          =   315
            Left            =   180
            TabIndex        =   16
            Top             =   1080
            Width           =   2235
            _ExtentX        =   3942
            _ExtentY        =   556
            Caption         =   ""
         End
         Begin VB.CommandButton ReadRegButton 
            Caption         =   "Read"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   315
            Left            =   1740
            TabIndex        =   15
            Top             =   300
            Width           =   675
         End
         Begin VB.ComboBox WriteRegComboBox 
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   315
            ItemData        =   "Display.frx":030A
            Left            =   180
            List            =   "Display.frx":037E
            TabIndex        =   13
            Top             =   300
            Width           =   1455
         End
         Begin VB.CommandButton WriteRegButton 
            Caption         =   "Write"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   315
            Left            =   1740
            TabIndex        =   5
            Top             =   660
            Width           =   675
         End
         Begin VB.TextBox WriteRegValueTextBox 
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   285
            Left            =   180
            TabIndex        =   4
            Top             =   660
            Width           =   1455
         End
      End
      Begin VB.CommandButton ResetSccbButton 
         Caption         =   "Reset All SCCB Registers"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   180
         TabIndex        =   2
         Top             =   240
         Width           =   2595
      End
   End
   Begin MSComDlg.CommonDialog CommonDialog 
      Left            =   10200
      Top             =   7560
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin QuickUsbOV7141Diag.Grayscale Grayscale 
      Height          =   7200
      Left            =   60
      TabIndex        =   17
      Top             =   1380
      Width           =   9600
      _ExtentX        =   16933
      _ExtentY        =   12700
      MaxValue        =   255
   End
   Begin MSComctlLib.StatusBar StatusBar 
      Align           =   2  'Align Bottom
      Height          =   330
      Left            =   0
      TabIndex        =   0
      Top             =   8670
      Width           =   12810
      _ExtentX        =   22595
      _ExtentY        =   582
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   2
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   19976
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Key             =   "frameRate"
         EndProperty
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   1200
      Left            =   8880
      Picture         =   "Display.frx":055C
      Top             =   60
      Width           =   3870
   End
   Begin VB.Menu FileMenu 
      Caption         =   "&File"
      Begin VB.Menu fileLoadSettingsMenu 
         Caption         =   "&Load Settings"
      End
      Begin VB.Menu fileSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu FileExitMenu 
         Caption         =   "&Exit"
         Shortcut        =   ^X
      End
   End
   Begin VB.Menu HelpMenu 
      Caption         =   "&Help"
      Begin VB.Menu HelpAboutMenu 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "Display"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'=============================================================================
' Title        : Display.frm
' Description  : FIBOCam Control Panel Form
' Notes        : None
' History      :
'   Date       Name        Description
'  9/15/2003   bhenry      Initial coding
'
' Copyright © 2002 Bitwise Systems  All rights reserved.
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
' 5385 Hollister Ave, Suite 215
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'=============================================================================
Option Explicit


Const OV7141_WRITE_ADDR As Byte = &H42
Const OV7141_READ_ADDR As Byte = &H43
Const SIO_C As Byte = 1
Const SIO_D As Byte = 2

Const NUM_BUFFERS = 4

Dim portVal As Byte
Dim portDir As Byte
Dim commandLength As Integer
Dim devName As String
Dim stringDescriptor As String
Dim fp As Long
Dim sp As Long
Dim frameNum As Long
Dim startTime As Long
Dim dataLength As Long
Dim videoBuffer0(0 To 307199) As Byte
Dim videoBuffer1(0 To 307199) As Byte
Dim videoBuffer2(0 To 307199) As Byte
Dim videoBuffer3(0 To 307199) As Byte
Dim bufferUsed(0 To 3) As Boolean
Dim transaction(0 To 3) As Byte
Dim writeBuffer As Byte
Dim readBuffer As Byte
Dim settingsFileName As String
Dim videoFileName As String


Private Sub Form_Load()
   Dim count As Integer
   
   ' Go to Real-time priority
   SetPriorityClass GetCurrentProcess(), REALTIME_PRIORITY_CLASS
   
   ' Get the datalength from the text box
   dataLength = (Grayscale.rows * Grayscale.Columns)
   
   ' Allocate storage for writeDataLength elements
   For count = 0 To NUM_BUFFERS - 1
      bufferUsed(count) = False
   Next
   
   videoFileName = "QuickUsbOv7141.avi"
End Sub



Private Sub LoadConfigFile(fileName As String)
   Dim address As Byte
   Dim register As Byte
   Dim Value As Byte
   Dim inputFile As Integer
   Dim Index As Integer
   Dim lineStr As String
   Dim scanStr As String
   
   ShowStatus "Loading configuration from " & fileName
   
   ' Read the settings file and program the device
   inputFile = FreeFile
   Open fileName For Input As inputFile
   
   Do While Not EOF(inputFile)
      ' Read in a line
      Line Input #inputFile, lineStr
      
      ' Skip comment marks and parse the string
      Index = 1
      If (Mid(lineStr, 1, 1) <> ";") Then
         ' Get the address
         scanStr = Mid(lineStr, Index, 2)
         address = Val("&H" & scanStr)
         
         ' Get the register
         Index = Index + 3
         scanStr = Mid(lineStr, Index, 2)
         register = Val("&H" & scanStr)
         
         ' Get the register
         Index = Index + 3
         scanStr = Mid(lineStr, Index, 2)
         Value = Val("&H" & scanStr)
         
         ' Update the screen
         ShowStatus "Setting register " & register & " to " & Value
         Sleep 10
         
         ' Now write the register
         WriteSccbReg register, Value
         
         ' Let the background tasks complete
         DoEvents
      End If
   Loop
   
   ' Close the file when we're done
   Close inputFile
   
   ' Refresh the currently selected register
   ReadRegButton_Click
End Sub



Private Sub fileLoadSettingsMenu_Click()
   Dim address As Byte
   Dim register As Byte
   Dim Value As Byte
   Dim inputFile As Integer
   Dim Index As Integer
   Dim lineStr As String
   Dim scanStr As String
   
   ' Display the open file dialog
   CommonDialog.fileName = settingsFileName
   CommonDialog.Filter = "Camera Settings File (*.txt)|*.txt"
   CommonDialog.ShowOpen
   If (CommonDialog.fileName <> "") Then
      settingsFileName = CommonDialog.fileName
   Else
      Exit Sub
   End If

   LoadConfigFile settingsFileName
End Sub



Public Function pow2(exponent As Byte) As Byte
   Dim table2 As Variant
      
   table2 = Array(&H1, &H2, &H4, &H8, &H10, &H20, &H40, &H80)
   pow2 = table2(exponent)
End Function




Private Sub ConfigureModule()
   ' Set the clock speed and output enables
   SpeedOptionButton_Click 1
   
   ' Reset list box to 1
   WriteRegComboBox.ListIndex = 0
   
   ' Load the default chip configuration
   LoadConfigFile "OV7141.txt"
End Sub



Private Sub AcquireCheckBox_Click()
   If (AcquireCheckBox.Value = vbChecked) Then
      GrabTimer.Enabled = True
   Else
      GrabTimer.Enabled = False
   End If
End Sub



Private Sub RecordCheckBox_Click()
   If (RecordCheckBox.Value = vbChecked) Then
      CommonDialog.fileName = videoFileName
      CommonDialog.Filter = "Video Files (*.avi)|*.avi"
      CommonDialog.ShowSave
      If (CommonDialog.fileName <> "") Then
         videoFileName = CommonDialog.fileName
      End If
      
      CreateAVI256 fp, sp, videoFileName, 480, 640, 30
      frameNum = 1
   Else
      CloseAVI256 fp, sp
   End If
End Sub



Private Sub FileExitMenu_Click()
   End
End Sub



Private Sub QuickUsbCtl_ConnectionChanged(ByVal Connected As Boolean)
   Dim ctl As control
   
   If (Connected) Then
      ' Enable all the controls
      On Error Resume Next
      For Each ctl In Display.Controls
         ctl.Enabled = True
      Next
      
      ShowStatus ""
      ' Configure for the Omnivision chip
      ConfigureModule
      
   Else
      ' Disable all the controls
      On Error Resume Next
      For Each ctl In Display.Controls
         If TypeName(ctl) <> "Menu" Then
            ctl.Enabled = False
         End If
      Next
      fileLoadSettingsMenu.Enabled = False
      ShowStatus "Cannot find any a QuickUSB OV7141 camera module"
   End If
End Sub



Private Sub Grayscale_Click()
   GrabFrame
   DisplayFrame
End Sub



Private Sub Grayscale_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
   ShowStatus "Bitmap(" & x & "," & y & ") = &H" & asHex(Grayscale.Bitmap(x, y), 2) & " (" & Grayscale.Bitmap(x, y) & ")"
End Sub



Private Sub HelpAboutMenu_Click()
   frmAbout.Show vbModal
End Sub



Private Sub GrabTimer_Timer()
   If (AcquireCheckBox.Value = vbChecked) Then
      GrabFrame
      DisplayFrame
   End If
End Sub



Private Sub GrabFrame()
   Dim theTime As Long
   Static lastframeTime As Long
   Dim result As Integer
   Dim grabIndex As Byte
   
   ' Look for a valid index
   Do Until bufferUsed(grabIndex) = False
      If grabIndex < NUM_BUFFERS - 1 Then
         grabIndex = grabIndex + 1
      Else
         Return
      End If
   Loop
   
   ' Start to read the frameffer
   theTime = timeGetTime
   Select Case grabIndex
   Case 0
      result = QuickUsbCtl.ReadDataAsync(videoBuffer0, 0, dataLength, transaction(0))
   Case 1
      result = QuickUsbCtl.ReadDataAsync(videoBuffer1, 0, dataLength, transaction(1))
   Case 2
      result = QuickUsbCtl.ReadDataAsync(videoBuffer2, 0, dataLength, transaction(2))
   Case 3
      result = QuickUsbCtl.ReadDataAsync(videoBuffer3, 0, dataLength, transaction(3))
   End Select
   
   If (result = 0) Then
      ShowStatus "Timeout"
      AcquireCheckBox.Value = vbUnchecked
   End If
   
   bufferUsed(grabIndex) = True
   
   ' Write to the AVI file
   If (RecordCheckBox.Value = vbChecked) Then
      Select Case grabIndex
      Case 0
         WriteAVIFrame256 sp, frameNum, videoBuffer0
      Case 1
         WriteAVIFrame256 sp, frameNum, videoBuffer1
      Case 2
         WriteAVIFrame256 sp, frameNum, videoBuffer2
      Case 3
         WriteAVIFrame256 sp, frameNum, videoBuffer3
      End Select
      frameNum = frameNum + 1
   End If

   ' Write the framerate to the screen
   ShowStatus Format(1000 / (theTime - lastframeTime), "0.0") & " Hz", 2
   lastframeTime = theTime
End Sub



Private Sub DisplayFrame()
   Dim result As Integer
   Dim displayIndex As Byte
   
   For displayIndex = 0 To NUM_BUFFERS - 1
      If bufferUsed(displayIndex) = True Then
         result = QuickUsbCtl.AsyncWait(dataLength, transaction(displayIndex), 0)
         Select Case displayIndex
         Case 0
            Grayscale.SetBitmap videoBuffer0
            bufferUsed(0) = False
         Case 1
            Grayscale.SetBitmap videoBuffer1
            bufferUsed(1) = False
         Case 2
            Grayscale.SetBitmap videoBuffer2
            bufferUsed(2) = False
         Case 3
            Grayscale.SetBitmap videoBuffer3
            bufferUsed(3) = False
         End Select
      End If
   Next
End Sub



Private Sub ReadSccbReg(ByVal regAddr As Byte, ByRef Value As Byte)
   QuickUsbCtl.ReadSPI regAddr + 128, Value
End Sub



Private Sub WriteSccbReg(ByVal regAddr As Byte, ByRef Value As Byte)
   QuickUsbCtl.WriteSPI regAddr + 128, Value
End Sub



Private Sub SpeedOptionButton_Click(Index As Integer)
   Dim Value As Integer
   
   QuickUsbCtl.ReadSetting SETTING_CPUCONFIG, Value
   If (Index = 0) Then
      QuickUsbCtl.WriteSetting SETTING_CPUCONFIG, (Value And Not &H18)            ' 12MHz
   Else
      QuickUsbCtl.WriteSetting SETTING_CPUCONFIG, (Value And Not &H18) Or 8       ' 24Mhz
   End If
End Sub



Private Sub ResetSccbButton_Click()
   ' Reset the chip
   WriteSccbReg &H12, &H80
   WriteSccbReg &H12, &H14

   ' Set the default invert/revert setting
   InvertCheckBox.Value = vbUnchecked
   RevertCheckBox.Value = vbUnchecked
   
   ' Set the default brightness and contrast
   BrightnessSlider.Value = &H80
   ContrastSlider.Value = &H80
   EvSlider.Value = &H41
   
   ' Update the read data
   ReadRegButton_Click
End Sub



Private Sub InvertCheckBox_Click()
   If (InvertCheckBox.Value = vbChecked) Then
      WriteSccbReg &H75, &H82
   Else
      WriteSccbReg &H75, &H2
   End If
End Sub


Private Sub RevertCheckBox_Click()
   If (RevertCheckBox.Value = vbChecked) Then
      WriteSccbReg &H12, &H54
   Else
      WriteSccbReg &H12, &H14
   End If
End Sub



Private Sub BrightnessSlider_Click()
   WriteSccbReg &H6, BrightnessSlider.Value
End Sub


Private Sub BrightnessSlider_Scroll()
   BrightnessSlider_Click
End Sub


Private Sub ContrastSlider_Click()
   WriteSccbReg &H0, ContrastSlider.Value
End Sub


Private Sub ContrastSlider_Scroll()
   ContrastSlider_Click
End Sub


Private Sub EvSlider_Click()
   WriteSccbReg &H10, EvSlider.Value
End Sub


Private Sub EvSlider_Scroll()
   EvSlider_Click
End Sub


Private Sub ReadRegButton_Click()
   WriteRegComboBox_Click
End Sub



Private Sub WriteRegButton_Click()
   Dim regAddr As Byte
   
   ' Get the register address
   If (InStr(1, WriteRegComboBox.Text, "-") <> 0) Then
      regAddr = Val(Left(WriteRegComboBox.Text, 4))
   Else
      regAddr = Val(WriteRegComboBox.Text)
   End If
   
   WriteSccbReg regAddr, Val(WriteRegValueTextBox)
End Sub



Private Sub WriteRegComboBox_Change()
   WriteRegComboBox_Click
End Sub


Private Sub WriteRegComboBox_Click()
   Dim itemText As String
   Dim regAddr As Byte
   Dim Value As Byte
   
   ' Get the register address
   If (InStr(1, WriteRegComboBox.Text, "-") <> 0) Then
      regAddr = Val(Left(WriteRegComboBox.Text, 4))
   Else
      regAddr = Val(WriteRegComboBox.Text)
   End If
   
   ' Read the register first
   ReadSccbReg regAddr, Value
   
   ' Update the WriteValueSlider
   WriteValueSlider.Value = Value
   
   ' Update the bitcheckbox
   BitCheckBox1.Value = Value
   
   ' Put the value in the value box
   WriteRegValueTextBox = "&H" & asHex(Value, 2)
   
   WriteValueSlider.Refresh
End Sub


Private Sub BitCheckBox1_Click()
   WriteRegValueTextBox = "&H" & asHex(BitCheckBox1.Value, 2)
   WriteValueSlider.Value = BitCheckBox1.Value
   WriteRegButton_Click
End Sub


Private Sub WriteValueSlider_Click()
   WriteRegValueTextBox = "&H" & asHex(WriteValueSlider.Value, 2)
   WriteRegButton_Click
   ReadRegButton_Click
End Sub



Private Sub WriteValueSlider_Scroll()
   WriteValueSlider_Click
End Sub


Public Sub ShowStatus(status As String, Optional Index As Integer = 1)
   StatusBar.Panels(Index) = status
   StatusTimer.Enabled = True
End Sub



Private Sub StatusTimer_Timer()
   ShowStatus ""
   StatusTimer.Enabled = False
End Sub

