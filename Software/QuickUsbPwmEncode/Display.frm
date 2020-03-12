VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Display 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB PWM Encoder"
   ClientHeight    =   3975
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   5475
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3975
   ScaleWidth      =   5475
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame3 
      Caption         =   "Duty Cycle"
      Height          =   1275
      Left            =   120
      TabIndex        =   5
      Top             =   2220
      Width           =   5235
      Begin MSComctlLib.Slider Channel0Slider 
         Height          =   315
         Left            =   120
         TabIndex        =   6
         Top             =   300
         Width           =   4935
         _ExtentX        =   8705
         _ExtentY        =   556
         _Version        =   393216
         LargeChange     =   10
         Max             =   100
         SelStart        =   50
         TickFrequency   =   5
         Value           =   50
      End
      Begin MSComctlLib.Slider Channel1Slider 
         Height          =   315
         Left            =   120
         TabIndex        =   7
         Top             =   780
         Width           =   4935
         _ExtentX        =   8705
         _ExtentY        =   556
         _Version        =   393216
         LargeChange     =   10
         Max             =   100
         SelStart        =   50
         TickFrequency   =   5
         Value           =   50
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "General"
      Height          =   1095
      Left            =   120
      TabIndex        =   3
      Top             =   60
      Width           =   5235
      Begin QuickUsbPwmEncoder.QuickUsbCtl QuickUsbCtl 
         Height          =   735
         Left            =   120
         TabIndex        =   4
         Top             =   240
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
   End
   Begin VB.Frame Frame2 
      Caption         =   "Frequency"
      Height          =   795
      Left            =   120
      TabIndex        =   1
      Top             =   1260
      Width           =   5235
      Begin MSComctlLib.Slider FrequencySlider 
         Height          =   315
         Left            =   120
         TabIndex        =   2
         Top             =   300
         Width           =   4935
         _ExtentX        =   8705
         _ExtentY        =   556
         _Version        =   393216
         LargeChange     =   10
         Min             =   1
         Max             =   200
         SelStart        =   50
         TickFrequency   =   10
         Value           =   50
      End
   End
   Begin MSComctlLib.StatusBar StatusBar 
      Align           =   2  'Align Bottom
      Height          =   330
      Left            =   0
      TabIndex        =   0
      Top             =   3645
      Width           =   5475
      _ExtentX        =   9657
      _ExtentY        =   582
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
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
' User: Blake Henry  Date: 1/20/04    Time: 2:32p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbPwmEncode
' Initial Check-in
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 8/08/03    Time: 6:36p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbPwmDecoder
' Initial checkin
'
'=============================================================================
Option Explicit


Const CLOCK_FREQ As Integer = 450      ' 12000 KHz (12.0 MHz)

Private Type PwmType
   dutyCycle As Integer
   lowVal As Integer
   highVal As Integer
   highCycles As Integer
   lowCycles As Integer
End Type


Dim devName As String
Dim stringDescriptor As String


Private Sub helpAboutMenu_Click()
   frmAbout.Show vbModal, Me
End Sub



Private Sub QuickUsbCtl_ConnectionChanged(ByVal Connected As Boolean)
   Dim bVal As Byte
   Dim wVal As Integer
   Dim lowVal As Integer
   Dim highVal As Integer
   
   If Connected Then
      ' Enable Port A bit 0 and 1 outputs
      QuickUsbCtl.ReadPortDir 0, bVal
      QuickUsbCtl.WritePortDir 0, bVal Or &H3

      ' Set TCON to turn on TIMER0 and select edge sensitive for Timer0 and 1
      QuickUsbCtl.WriteTimerReg TIMER_REG_TCON, &H50

      ' Set TMOD to put Timer0 and Timer0 in Mode 0
      QuickUsbCtl.WriteTimerReg TIMER_REG_TMOD, &H0

      ' Set CKCON to 0
      QuickUsbCtl.WriteTimerReg TIMER_REG_CKCON, &H18

      ' Enable TIMER0 and TIMER1 interrupts
      QuickUsbCtl.WriteTimerReg TIMER_REG_IE, &H8A
      
      QuickUsbCtl.ReadTimerReg TIMER_REG_PWM0LOW, lowVal
      QuickUsbCtl.ReadTimerReg TIMER_REG_PWM0HIGH, highVal
      ShowStatus "low = " & Hex(lowVal) & ", high = " & Hex(highVal)
   End If
End Sub




Private Sub FrequencySlider_Click()
   Channel0Slider_Change
   Channel1Slider_Change
End Sub



Private Sub FrequencySlider_Scroll()
   FrequencySlider_Click
End Sub



Private Sub UpdatePwmRegs()
   Dim frequency As Integer
   Dim totalCycles As Integer
   Dim pwm(2) As PwmType
   
   ' Calculate the number of cycles
   frequency = FrequencySlider.Value
   totalCycles = Max(CLOCK_FREQ / frequency, 2)
   
   pwm(0).dutyCycle = Channel0Slider.Value
   pwm(0).highCycles = Max(((totalCycles * pwm(0).dutyCycle) / 100), 1)
   pwm(0).lowCycles = totalCycles - pwm(0).highCycles
   pwm(0).highVal = 1 - pwm(0).highCycles
   pwm(0).lowVal = 1 - pwm(0).lowCycles
   
   pwm(1).dutyCycle = Channel1Slider.Value
   pwm(1).highCycles = Max(((totalCycles * pwm(1).dutyCycle) / 100), 1)
   pwm(1).lowCycles = totalCycles - pwm(1).highCycles
   pwm(1).highVal = &HFFFF - pwm(1).highCycles
   pwm(1).lowVal = &HFFFF - pwm(1).lowCycles
   
   QuickUsbCtl.WriteTimerReg TIMER_REG_PWM0LOW, pwm(0).lowVal
   QuickUsbCtl.WriteTimerReg TIMER_REG_PWM0HIGH, pwm(0).highVal
   QuickUsbCtl.WriteTimerReg TIMER_REG_PWM1LOW, pwm(1).lowVal
   QuickUsbCtl.WriteTimerReg TIMER_REG_PWM1HIGH, pwm(1).highVal
   
   'ShowStatus "PWM0 low: " & Hex(pwm(0).lowVal) & ", high:" & Hex(pwm(0).highVal) & "  PWM1 low: " & Hex(pwm(1).lowVal) & ", high:" & Hex(pwm(1).highVal)
   ShowStatus "Total: " & totalCycles & ", PWM0 low: " & Hex(pwm(0).lowCycles) & ", high:" & Hex(pwm(0).highCycles) & "  PWM1 low: " & Hex(pwm(1).lowCycles) & ", high:" & Hex(pwm(1).highCycles)

End Sub



Private Sub Channel0Slider_Change()
   UpdatePwmRegs
End Sub



Private Sub Channel0Slider_Scroll()
   UpdatePwmRegs
End Sub



Private Sub Channel1Slider_Change()
   UpdatePwmRegs
End Sub



Private Sub Channel1Slider_Scroll()
   UpdatePwmRegs
End Sub



Private Sub ShowStatus(message As String)
   StatusBar.SimpleText = message
End Sub
