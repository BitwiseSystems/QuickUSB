VERSION 5.00
Begin VB.Form frmAbout 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About"
   ClientHeight    =   3210
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   4275
   ClipControls    =   0   'False
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   214
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   285
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Label WebSiteLabel 
      BackColor       =   &H00FFFFFF&
      Caption         =   "http://www.quickusb.com"
      ForeColor       =   &H00C00000&
      Height          =   195
      Left            =   1740
      TabIndex        =   5
      Top             =   1800
      Width           =   1875
   End
   Begin VB.Label Label2 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Learn more at "
      Height          =   195
      Left            =   720
      TabIndex        =   4
      Top             =   1800
      Width           =   1035
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
      Caption         =   "This software was written using the QuickUSB Library."
      Height          =   195
      Left            =   120
      TabIndex        =   3
      Top             =   1500
      Width           =   3975
   End
   Begin VB.Image Image1 
      Height          =   1155
      Left            =   120
      Picture         =   "frmAbout.frx":0442
      Top             =   120
      Width           =   4080
   End
   Begin VB.Label lblDisclaimer 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Warning: ..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   480
      Left            =   120
      TabIndex        =   2
      Top             =   2640
      Width           =   4005
   End
   Begin VB.Label lblTitle 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Application Title"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   240
      Left            =   120
      TabIndex        =   1
      Top             =   2160
      Width           =   2865
   End
   Begin VB.Label lblVersion 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Version"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Left            =   120
      TabIndex        =   0
      Top             =   2400
      Width           =   2865
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'=============================================================================
' Title        : frmAbout.frm
' Description  : QuickUSB About Box Form
' Notes        : None
' History      :
'   Date       Name        Description
'  3/30/2002   bhenry      Initial coding
'
' Copyright © 2002 Bitwise Systems, Inc.  All rights reserved.
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
' 697 Via Miguel
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: frmAbout.frm $
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/20/03    Time: 8:56a
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Changed QuickUSB Image back to TM version
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/17/03    Time: 8:47a
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
'=============================================================================
Option Explicit

Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long

Private Sub Form_Load()
    Me.Caption = "About " & App.Title
    'Me.Icon = Display.Icon
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = App.Title
    lblDisclaimer.Caption = App.LegalCopyright & vbCrLf & App.LegalTrademarks
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub


Private Sub WebSiteLabel_Click()
   ShellExecute frmAbout.hwnd, "open", "http://www.quickusb.com", vbNullString, vbNullString, 0
End Sub
