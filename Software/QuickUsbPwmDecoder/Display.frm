VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomctl.ocx"
Begin VB.Form Display 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB PWM Decoder"
   ClientHeight    =   2370
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   5340
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2370
   ScaleWidth      =   5340
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "INT0"
      Height          =   1035
      Left            =   120
      TabIndex        =   2
      Top             =   900
      Width           =   2535
      Begin VB.TextBox WidthTextBox 
         Height          =   315
         Left            =   1080
         TabIndex        =   4
         Top             =   240
         Width           =   795
      End
      Begin VB.TextBox PeriodTextBox 
         Height          =   315
         Left            =   1080
         TabIndex        =   3
         Top             =   600
         Width           =   795
      End
      Begin VB.Label Label4 
         Caption         =   "secs"
         Height          =   195
         Left            =   1980
         TabIndex        =   8
         Top             =   660
         Width           =   375
      End
      Begin VB.Label Label3 
         Caption         =   "secs"
         Height          =   195
         Left            =   1980
         TabIndex        =   7
         Top             =   300
         Width           =   375
      End
      Begin VB.Label Label1 
         Caption         =   "Width"
         Height          =   195
         Left            =   180
         TabIndex        =   6
         Top             =   300
         Width           =   795
      End
      Begin VB.Label Label2 
         Caption         =   "Period"
         Height          =   195
         Left            =   180
         TabIndex        =   5
         Top             =   660
         Width           =   795
      End
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   330
      Left            =   0
      TabIndex        =   0
      Top             =   2040
      Width           =   5340
      _ExtentX        =   9419
      _ExtentY        =   582
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin VB.Timer Timer 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   4620
      Top             =   60
   End
   Begin QuickUsbPwmDecoder.QuickUsbCtl QuickUsbCtl 
      Height          =   735
      Left            =   180
      TabIndex        =   1
      Top             =   120
      Width           =   4995
      _ExtentX        =   8811
      _ExtentY        =   1296
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Connected       =   -1  'True
   End
   Begin VB.Menu helpMenu 
      Caption         =   "&Help"
      Begin VB.Menu helpAboutMenu 
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
' Description  : QuickUSB PWM Decoder Form
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
' 5385 Holllister Ave, Suite 215
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-4833
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: Display.frm $
' 
' *****************  Version 1  *****************
' User: Blake Henry  Date: 8/08/03    Time: 6:36p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbPwmDecoder
' Initial checkin
'
'=============================================================================
Option Explicit


Dim devName As String
Dim stringDescriptor As String


Private Sub helpAboutMenu_Click()
   frmAbout.Show vbModal, Me
End Sub



Private Sub QuickUsbCtl_ConnectionChanged(ByVal Connected As Boolean)
   Dim bVal As Byte
   Dim wVal As Integer
   
   If Connected Then
      ' Initialize the module
      QuickUsbCtl.ReadPortDir 0, bVal
      QuickUsbCtl.WritePortDir 0, bVal And &HFE

      ' Set TCON to turn on TIMER0 and select edge sensitive for Timer0 and 1
      QuickUsbCtl.WriteTimerReg TIMER_REG_TCON, &HFD

      ' Set TMOD to turn on GATE0 and turn off M1
      QuickUsbCtl.WriteTimerReg TIMER_REG_TMOD, &H19

      ' Set CKCON to 0
      QuickUsbCtl.WriteTimerReg TIMER_REG_CKCON, 0

      ' Turn on the INT0 pin
      QuickUsbCtl.ReadTimerReg TIMER_REG_PORTACFG, wVal
      QuickUsbCtl.WriteTimerReg TIMER_REG_PORTACFG, wVal Or 1

      ' Enable INT0 interrupts
      QuickUsbCtl.ReadTimerReg TIMER_REG_IE, wVal
      QuickUsbCtl.WriteTimerReg TIMER_REG_IE, wVal Or 1

      ' Open for business
      Timer.Enabled = True
      WidthTextBox.Enabled = True
      PeriodTextBox.Enabled = True
   Else
      Timer.Enabled = False
      WidthTextBox.Enabled = False
      PeriodTextBox.Enabled = False
   End If
End Sub



Private Sub Timer_Timer()
   Dim hDevice As Long
   Dim result As Long
   Dim value(0 To 3) As Integer
   Dim secsPerCount As Single
   
   ' Read the ISR registers
   QuickUsbCtl.ReadTimerIsrRegs value
   
   ' Update the Text boxes
   secsPerCount = 12 / 48000000#
   WidthTextBox = Format(secsPerCount * IIf(value(0) < 0, 65536 + CLng(value(0)), value(0)), "Scientific")
   PeriodTextBox = Format(secsPerCount * IIf(value(1) < 0, 65536 + CLng(value(1)), value(0)), "Scientific")
'   WidthTextBox = "&h" & asHex(IIf(value(0) < 0, 65536 + CLng(value(0)), value(0)), 4)
'   PeriodTextBox = "&h" & asHex(IIf(value(1) < 0, 65536 + CLng(value(1)), value(1)), 4)
End Sub



Private Sub ReadTimerReg(index As Integer, ByRef value As Integer)
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      Exit Sub
   End If
      
   result = QuickUsbReadTimerReg(hDevice, index, value)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction resul
   If (result = 0) Then
      Exit Sub
   End If
End Sub



Private Sub WriteTimerReg(index As Integer, value As Integer)
   Dim hDevice As Long
   Dim result As Long
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      Exit Sub
   End If
         
   ' Actually write the text out the timer register
   result = QuickUsbWriteTimerReg(hDevice, index, value)
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      Exit Sub
   End If
End Sub

