VERSION 5.00
Begin VB.Form frmAbout 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About"
   ClientHeight    =   2310
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   4215
   ClipControls    =   0   'False
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1594.403
   ScaleMode       =   0  'User
   ScaleWidth      =   3958.103
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Image Image1 
      Height          =   1245
      Left            =   120
      Picture         =   "frmAbout.frx":0442
      Top             =   120
      Width           =   4005
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
      Height          =   240
      Left            =   120
      TabIndex        =   2
      Top             =   1980
      Width           =   2865
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
      Top             =   1500
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
      Top             =   1740
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
' User: Blake Henry  Date: 1/20/04    Time: 2:32p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbPwmEncode
' Initial Check-in
' 
' *****************  Version 1  *****************
' User: Blake Henry  Date: 8/08/03    Time: 6:36p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbPwmDecoder
' Initial checkin
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

Private Sub Form_Load()
    Me.Caption = "About " & App.Title
    'Me.Icon = Display.Icon
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = App.Title
    lblDisclaimer.Caption = App.LegalCopyright
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub


