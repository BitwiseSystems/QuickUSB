VERSION 5.00
Begin VB.Form frmAbout 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About"
   ClientHeight    =   2790
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   4215
   ClipControls    =   0   'False
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1925.707
   ScaleMode       =   0  'User
   ScaleWidth      =   3958.103
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Image Image1 
      Height          =   1200
      Left            =   138
      Picture         =   "frmAbout.frx":0442
      Top             =   120
      Width           =   3870
   End
   Begin VB.Label lblDriverVersion 
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
      TabIndex        =   4
      Top             =   2460
      Width           =   2865
   End
   Begin VB.Label lblDllVersion 
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
      TabIndex        =   3
      Top             =   2220
      Width           =   2865
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
' (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real,
' Suite E, Goleta, CA  93117.
'
' Bitwise Systems
' 6489 Calle Real, Suite E
' Goleta, CA  93117
' Voice: (805) 683-6469
' Fax  : (805) 683-4833
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: frmAbout.frm $
' 
' *****************  Version 10  *****************
' User: Bob Jacobson Date: 2/13/06    Time: 2:20p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Correct icon in help/about display.
' Add better tool tips to the default page.
' Bump version to 2.11.0
'
' *****************  Version 8  *****************
' User: Blake Henry  Date: 1/13/06    Time: 7:02p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbCustomizer
' Reworked GUI
'
' *****************  Version 7  *****************
' User: Bob Jacobson Date: 12/20/05   Time: 4:36p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Change the Company address and Fax number.
'
' *****************  Version 6  *****************
' User: Bob Jacobson Date: 12/15/05   Time: 12:10p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Change the logo in the HelpAbout form.
' Move all HelpAbout form code to frmAbout.frm.
'
' *****************  Version 5  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 11:47a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 4  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 9:24a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Merged the features from all the different versions (timeout, about,
' etc.)
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
   Dim dllMajor As Integer
   Dim dllMinor As Integer
   Dim dllRevision As Integer
   Dim driverMajor As Integer
   Dim driverMinor As Integer
   Dim driverRevision As Integer
   
   Me.Caption = "About " & App.Title
   lblTitle.Caption = App.Title
   lblVersion.Caption = "App Version " & App.major & "." & App.minor & "." & App.revision
   lblDisclaimer.Caption = App.LegalCopyright
   
   On Error GoTo skipError
   
   ' Get the DLL version
   QuickUsbGetDllVersion dllMajor, dllMinor, dllRevision
               
   ' Get the driver version
   QuickUsbGetDriverVersion driverMajor, driverMinor, driverRevision
    
   lblDllVersion.Caption = "DLL Version " & dllMajor & "." & dllMinor & "." & dllRevision
   lblDriverVersion.Caption = "Driver Version " & driverMajor & "." & driverMinor & "." & driverRevision
   Exit Sub
   
skipError:
   Me.Height = 2750
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub

