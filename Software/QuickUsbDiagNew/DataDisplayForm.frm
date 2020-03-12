VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Begin VB.Form OutputForm 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Output"
   ClientHeight    =   5580
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   4620
   Icon            =   "DataDisplayForm.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5580
   ScaleWidth      =   4620
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin RichTextLib.RichTextBox OutputTextBox 
      Height          =   5355
      Left            =   60
      TabIndex        =   0
      ToolTipText     =   "Displays hex dump of transferred data"
      Top             =   120
      Width           =   4455
      _ExtentX        =   7858
      _ExtentY        =   9446
      _Version        =   393217
      ScrollBars      =   2
      DisableNoScroll =   -1  'True
      TextRTF         =   $"DataDisplayForm.frx":0442
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Lucida Console"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
End
Attribute VB_Name = "OutputForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'=============================================================================
' Title        : OutputForm.frm
' Description  : QuickUSB Output Form
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
' $History: DataDisplayForm.frm $
' 
' *****************  Version 3  *****************
' User: Bob Jacobson Date: 12/20/05   Time: 4:36p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Change the Company address and Fax number.
' 
' *****************  Version 2  *****************
' User: Blake Henry  Date: 7/03/03    Time: 5:07p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Truncate data display for data longer than 4k
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/17/03    Time: 8:47a
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
'=============================================================================
Option Explicit

Dim MaxDataLength As Long


Private Sub Form_Initialize()
   MaxDataLength = 4096
End Sub


Private Sub Form_Resize()
   OutputTextBox.Width = ScaleWidth - 100
   OutputTextBox.Height = ScaleHeight - 100
End Sub


Public Sub Clear()
   OutputTextBox.Text = ""
End Sub


Public Sub ShowByteData(theData() As Byte)
   Dim col As Long
   Dim maxCol As Long
   Dim count As Long
   Dim start As Long
   Dim displayString As String
   
   start = OutputTextBox.SelStart
   OutputTextBox.Text = ""
   col = 0
   maxCol = 15
   For count = 0 To Min(UBound(theData, 1), MaxDataLength) - 1
      If (col = 0) Then
         displayString = displayString & asHex(count, 4) & ": "
      End If
      
      displayString = displayString & asHex(theData(count), 2)
      
      If (col = maxCol) Then
         displayString = displayString & vbCrLf
      End If
      If (col < maxCol) Then
         col = col + 1
      Else
         col = 0
      End If
   Next
   OutputTextBox.Text = displayString
   OutputTextBox.SelStart = start
End Sub


Public Sub ShowByteDataComparison(theDataA() As Byte, theDataB() As Byte)
   Dim count As Long
   Dim start As Long
   Dim displayString As String
   
   start = OutputTextBox.SelStart
   OutputTextBox.Text = ""
   For count = 0 To Min(UBound(theDataA, 1), MaxDataLength) - 1
      displayString = displayString & asHex(count, 4) & ": " & asHex(theDataA(count), 2) & " - " & asHex(theDataB(count), 2) & vbCrLf
   Next
   OutputTextBox.Text = displayString
   OutputTextBox.SelStart = start
End Sub


