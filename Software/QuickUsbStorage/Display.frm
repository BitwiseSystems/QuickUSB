VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Display 
   Caption         =   "QuickUsb VB6 Application"
   ClientHeight    =   4200
   ClientLeft      =   225
   ClientTop       =   825
   ClientWidth     =   5415
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4200
   ScaleWidth      =   5415
   StartUpPosition =   3  'Windows Default
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   315
      Left            =   0
      TabIndex        =   6
      Top             =   3885
      Width           =   5415
      _ExtentX        =   9551
      _ExtentY        =   556
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin VB.Frame Frame1 
      Caption         =   "EEPROM Code"
      Height          =   2775
      Left            =   120
      TabIndex        =   1
      Top             =   900
      Width           =   5175
      Begin VB.CommandButton SetAllCommand 
         Caption         =   "Set Storage"
         Height          =   315
         Left            =   2940
         TabIndex        =   8
         Top             =   1320
         Width           =   1455
      End
      Begin VB.CommandButton ClearAllCommand 
         Caption         =   "Clear Storage"
         Height          =   315
         Left            =   2940
         TabIndex        =   7
         Top             =   900
         Width           =   1455
      End
      Begin VB.CommandButton WriteCommand 
         Caption         =   "Write"
         Height          =   315
         Left            =   4260
         TabIndex        =   5
         Top             =   360
         Width           =   675
      End
      Begin VB.TextBox ValueTextBox 
         Height          =   315
         Left            =   2940
         TabIndex        =   3
         Top             =   360
         Width           =   1215
      End
      Begin VB.ListBox List1 
         Height          =   2205
         Left            =   240
         TabIndex        =   2
         Top             =   360
         Width           =   1695
      End
      Begin VB.Label Label1 
         Caption         =   "Value"
         Height          =   195
         Left            =   2160
         TabIndex        =   4
         Top             =   420
         Width           =   675
      End
   End
   Begin QuickUsbStorageApp.QuickUsbCtl QuickUsbCtl1 
      Height          =   795
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   5175
      _extentx        =   9128
      _extenty        =   1402
      font            =   "Display.frx":0442
      connected       =   -1  'True
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "Display"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const STORAGE_LENGTH As Integer = 512
Dim buffer(STORAGE_LENGTH) As Long
Dim selIndex As Integer


Private Sub QuickUsbCtl1_ConnectionChanged(ByVal Connected As Boolean)
   If (Connected) Then
      DoEvents
      List1.Enabled = True
      UpdateListBox
   Else
      List1.Enabled = False
   End If
End Sub


Private Sub UpdateListBox()
   Dim count As Integer
   
   ' Read all the data
   QuickUsbCtl1.ReadStorageLong 0, buffer, 0, STORAGE_LENGTH
   
   List1.Clear
   For count = 0 To STORAGE_LENGTH - 1
      List1.AddItem Hex(count) & ": " & Hex(buffer(count))
   Next
End Sub


Private Sub List1_Click()
   selIndex = List1.ListIndex
   ValueTextBox.Text = Hex(buffer(selIndex))
End Sub


Private Sub List1_DblClick()
   UpdateListBox
End Sub


Private Sub WriteCommand_Click()
   Dim data(1) As Long
   
   data(0) = Val("&h" & ValueTextBox.Text)
   QuickUsbCtl1.WriteStorageLong selIndex, data, 0, 1
   
   UpdateListBox
End Sub


Private Sub ClearAllCommand_Click()
   Dim count As Integer
   
   For count = 0 To STORAGE_LENGTH
      buffer(count) = &HFFFFFFFF
   Next
   QuickUsbCtl1.WriteStorageLong 0, buffer, 0, STORAGE_LENGTH
   
   UpdateListBox
End Sub


Private Sub SetAllCommand_Click()
   Dim count As Integer
   
   For count = 0 To STORAGE_LENGTH
      buffer(count) = Val("&h" & ValueTextBox.Text)
   Next
   QuickUsbCtl1.WriteStorageLong 0, buffer, 0, STORAGE_LENGTH
   
   UpdateListBox
End Sub


Private Sub mnuHelpAbout_Click()
   frmAbout.Show vbModal
End Sub

