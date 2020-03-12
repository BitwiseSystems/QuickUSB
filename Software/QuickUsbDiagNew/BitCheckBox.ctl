VERSION 5.00
Begin VB.UserControl BitCheckBox 
   ClientHeight    =   300
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3315
   ScaleHeight     =   300
   ScaleWidth      =   3315
   Begin VB.TextBox ValueTextBox 
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
      Left            =   2700
      TabIndex        =   8
      Text            =   "0"
      Top             =   0
      Width           =   555
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   7
      Left            =   1200
      TabIndex        =   0
      ToolTipText     =   "Bit 7"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   6
      Left            =   1380
      TabIndex        =   1
      ToolTipText     =   "Bit 6"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   5
      Left            =   1560
      TabIndex        =   2
      ToolTipText     =   "Bit 5"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   4
      Left            =   1740
      TabIndex        =   3
      ToolTipText     =   "Bit 4"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   3
      Left            =   1920
      TabIndex        =   4
      ToolTipText     =   "Bit 3"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   2
      Left            =   2100
      TabIndex        =   5
      ToolTipText     =   "Bit 2"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   1
      Left            =   2280
      TabIndex        =   6
      ToolTipText     =   "Bit 1"
      Top             =   0
      Width           =   195
   End
   Begin VB.CheckBox BitCheckBox 
      Height          =   255
      Index           =   0
      Left            =   2460
      TabIndex        =   7
      ToolTipText     =   "Bit 0"
      Top             =   0
      Width           =   195
   End
   Begin VB.Label TextLabel 
      Caption         =   "Label1"
      Height          =   195
      Left            =   60
      TabIndex        =   9
      Top             =   60
      Width           =   1080
   End
End
Attribute VB_Name = "BitCheckBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'=============================================================================
' Title        : BitCheckBox.ctl
' Description  : Bit Checkbox User Control
' Notes        : None
' History      :
'   Date       Name        Description
'  3/30/2002   bhenry      Initial coding
'
' Copyright © 2002,2003 Bitwise Systems, Inc.  All rights reserved.
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
' 5385 Hollister Ave, Suite 215
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-4833
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: BitCheckBox.ctl $
' 
' *****************  Version 12  *****************
' User: Blake Henry  Date: 1/16/06    Time: 6:39p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbCustomizer
' Changed the tab order
'
' *****************  Version 11  *****************
' User: Bob Jacobson Date: 8/30/05    Time: 11:22a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Blake's newer bitcheckbox control.
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 8/22/05    Time: 12:01p
' Created in $/Projects/OLIS/Software/UDIODIAGVB
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 4/07/05    Time: 7:08p
' Created in $/Projects/Trex/RIB/Software/RIBDIAGVB
'
' *****************  Version 2  *****************
' User: Blake Henry  Date: 1/20/04    Time: 2:54p
' Updated in $/Projects/Trex Enterprises/SWIB/Software
' SWIB v1.0
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 1/15/04    Time: 7:58a
' Created in $/Projects/Trex Enterprises/SWIB/Software
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 9/18/03    Time: 5:43p
' Created in $/Projects/Trex/QuickUsbPMC2
' Initial checkin
'
' *****************  Version 5  *****************
' User: Blake Henry  Date: 8/27/03    Time: 6:46p
' Updated in $/Projects/Ultralink/Usb796/Software
' The version shipped to Smith
'
' *****************  Version 4  *****************
' User: Blake Henry  Date: 6/24/03    Time: 11:05a
' Updated in $/Projects/Ultralink/Usb796/Software
' Added enable/disable
'
' *****************  Version 3  *****************
' User: Blake Henry  Date: 6/20/03    Time: 7:38p
' Updated in $/Projects/Ultralink/Usb796/Software
' Massive changes
'
' *****************  Version 2  *****************
' User: Blake Henry  Date: 6/17/03    Time: 4:41p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Updated comment setup in VSS to insert correct leading characters
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/17/03    Time: 8:47a
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
'=============================================================================
Option Explicit


'Default Property Values:
Const m_def_ToolTipTextBit0 = ""
Const m_def_ToolTipTextBit1 = ""
Const m_def_ToolTipTextBit2 = ""
Const m_def_ToolTipTextBit3 = ""
Const m_def_ToolTipTextBit4 = ""
Const m_def_ToolTipTextBit5 = ""
Const m_def_ToolTipTextBit6 = ""
Const m_def_ToolTipTextBit7 = ""
Const m_def_Enabled = 0
Const m_def_Value = 0

'Property Variables:
Dim m_ToolTipTextBit0 As String
Dim m_ToolTipTextBit1 As String
Dim m_ToolTipTextBit2 As String
Dim m_ToolTipTextBit3 As String
Dim m_ToolTipTextBit4 As String
Dim m_ToolTipTextBit5 As String
Dim m_ToolTipTextBit6 As String
Dim m_ToolTipTextBit7 As String
Dim m_Enabled As Boolean
'Dim m_ToolTipIndex As Byte
Dim m_Value As Byte
Dim settingValue As Boolean

'Event Declarations:
Public Event Click()
Public Event Change()


Private Sub UserControl_Initialize()
   Value = 0
   settingValue = False
End Sub


Private Function asHex(theData As Variant, length As Variant) As String
   Const digits As String = "0123456789ABCDEF"
   Dim pow2 As Variant
   Dim digit As Variant
   Dim tempStr As String
      
   pow2 = Array( _
      &H1, &H2, &H4, &H8, _
      &H10, &H20, &H40, &H80, _
      &H100, &H200, &H400, &H800, _
      &H1000, &H2000, &H4000, &H8000, _
      &H10000, &H20000, &H40000, &H80000, _
      &H100000, &H200000, &H400000, &H800000 _
   )
   For digit = length - 1 To 0 Step -1
      tempStr = tempStr & Mid(digits, ((theData \ pow2(digit * 4)) And &HF) + 1, 1)
   Next
   asHex = tempStr
End Function



Private Sub BitCheckBox_Click(Index As Integer)
   If Not settingValue Then
      ValueTextBox.Text = "&H" & asHex(Value, 2)
      RaiseEvent Click
      RaiseEvent Change
   End If
End Sub


Private Sub UserControl_Resize()
   Dim count As Byte
   
   ValueTextBox.Left = Width - (ValueTextBox.Width + 10)
   BitCheckBox(0).Left = ValueTextBox.Left - (BitCheckBox(0).Width + 40)
   For count = 1 To 7
      BitCheckBox(count).Left = BitCheckBox(count - 1).Left - (BitCheckBox(count).Width + 10)
      BitCheckBox(count).Caption = ""
   Next
   TextLabel.Width = Max(BitCheckBox(7).Left - (TextLabel.Left + 20), 0)
End Sub


Private Sub ValueTextBox_KeyDown(KeyCode As Integer, Shift As Integer)
   If (KeyCode = Asc(vbCr)) Then
      Value = CByte(Val(ValueTextBox.Text) And &HFF)
      RaiseEvent Change
   End If
End Sub


Private Sub ValueTextBox_LostFocus()
   Value = CByte(Val(ValueTextBox.Text) And &HFF)
End Sub


'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=TextLabel,TextLabel,-1,Caption
Public Property Get Caption() As String
Attribute Caption.VB_Description = "Returns/sets the text displayed in an object's title bar or below an object's icon."
   Caption = TextLabel.Caption
End Property


Public Property Let Caption(ByVal New_Caption As String)
   TextLabel.Caption() = New_Caption
   PropertyChanged "Caption"
End Property


'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)

   TextLabel.Caption = PropBag.ReadProperty("Caption", "TextLabel")
   m_Value = PropBag.ReadProperty("Value", m_def_Value)
   BitCheckBox(0).ToolTipText = PropBag.ReadProperty("ToolTipTextBit0", "Bit 0")
   BitCheckBox(1).ToolTipText = PropBag.ReadProperty("ToolTipTextBit1", "Bit 1")
   BitCheckBox(2).ToolTipText = PropBag.ReadProperty("ToolTipTextBit2", "Bit 2")
   BitCheckBox(3).ToolTipText = PropBag.ReadProperty("ToolTipTextBit3", "Bit 3")
   BitCheckBox(4).ToolTipText = PropBag.ReadProperty("ToolTipTextBit4", "Bit 4")
   BitCheckBox(5).ToolTipText = PropBag.ReadProperty("ToolTipTextBit5", "Bit 5")
   BitCheckBox(6).ToolTipText = PropBag.ReadProperty("ToolTipTextBit6", "Bit 6")
   BitCheckBox(7).ToolTipText = PropBag.ReadProperty("ToolTipTextBit7", "Bit 7")
   m_ToolTipTextBit0 = PropBag.ReadProperty("ToolTipTextBit0", m_def_ToolTipTextBit0)
   m_ToolTipTextBit1 = PropBag.ReadProperty("ToolTipTextBit1", m_def_ToolTipTextBit1)
   m_ToolTipTextBit2 = PropBag.ReadProperty("ToolTipTextBit2", m_def_ToolTipTextBit2)
   m_ToolTipTextBit3 = PropBag.ReadProperty("ToolTipTextBit3", m_def_ToolTipTextBit3)
   m_ToolTipTextBit4 = PropBag.ReadProperty("ToolTipTextBit4", m_def_ToolTipTextBit4)
   m_ToolTipTextBit5 = PropBag.ReadProperty("ToolTipTextBit5", m_def_ToolTipTextBit5)
   m_ToolTipTextBit6 = PropBag.ReadProperty("ToolTipTextBit6", m_def_ToolTipTextBit6)
   m_ToolTipTextBit7 = PropBag.ReadProperty("ToolTipTextBit7", m_def_ToolTipTextBit7)
   m_Enabled = PropBag.ReadProperty("Enabled", m_def_Enabled)
End Sub


'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
   Call PropBag.WriteProperty("Caption", TextLabel.Caption, "TextLabel")
   Call PropBag.WriteProperty("Value", m_Value, m_def_Value)
   Call PropBag.WriteProperty("ToolTipTextBit0", BitCheckBox(0).ToolTipText, "Bit 0")
   Call PropBag.WriteProperty("ToolTipTextBit1", BitCheckBox(1).ToolTipText, "Bit 1")
   Call PropBag.WriteProperty("ToolTipTextBit2", BitCheckBox(2).ToolTipText, "Bit 2")
   Call PropBag.WriteProperty("ToolTipTextBit3", BitCheckBox(3).ToolTipText, "Bit 3")
   Call PropBag.WriteProperty("ToolTipTextBit4", BitCheckBox(4).ToolTipText, "Bit 4")
   Call PropBag.WriteProperty("ToolTipTextBit5", BitCheckBox(5).ToolTipText, "Bit 5")
   Call PropBag.WriteProperty("ToolTipTextBit6", BitCheckBox(6).ToolTipText, "Bit 6")
   Call PropBag.WriteProperty("ToolTipTextBit7", BitCheckBox(7).ToolTipText, "Bit 7")
   Call PropBag.WriteProperty("ToolTipTextBit0", m_ToolTipTextBit0, m_def_ToolTipTextBit0)
   Call PropBag.WriteProperty("ToolTipTextBit1", m_ToolTipTextBit1, m_def_ToolTipTextBit1)
   Call PropBag.WriteProperty("ToolTipTextBit2", m_ToolTipTextBit2, m_def_ToolTipTextBit2)
   Call PropBag.WriteProperty("ToolTipTextBit3", m_ToolTipTextBit3, m_def_ToolTipTextBit3)
   Call PropBag.WriteProperty("ToolTipTextBit4", m_ToolTipTextBit4, m_def_ToolTipTextBit4)
   Call PropBag.WriteProperty("ToolTipTextBit5", m_ToolTipTextBit5, m_def_ToolTipTextBit5)
   Call PropBag.WriteProperty("ToolTipTextBit6", m_ToolTipTextBit6, m_def_ToolTipTextBit6)
   Call PropBag.WriteProperty("ToolTipTextBit7", m_ToolTipTextBit7, m_def_ToolTipTextBit7)
   Call PropBag.WriteProperty("Enabled", m_Enabled, m_def_Enabled)
End Sub



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=1,0,0,0
Public Property Get Value() As Byte
Attribute Value.VB_Description = "Returns the byte value of the bits"
   Dim mask As Byte
   Dim count As Byte
   
   m_Value = 0
   mask = 1
   For count = 0 To 7
      m_Value = m_Value Or IIf(BitCheckBox(count).Value = vbChecked, mask, 0)
      mask = IIf(count < 7, mask * 2, mask)
   Next
   Value = m_Value
End Property



Public Property Let Value(ByVal New_Value As Byte)
   Dim mask As Byte
   Dim count As Byte

   settingValue = True
   mask = 1
   m_Value = New_Value
   For count = 0 To 7
      BitCheckBox(count).Value = IIf((m_Value And mask) = mask, vbChecked, vbUnchecked)
      mask = IIf(count < 7, mask * 2, mask)
   Next
   settingValue = False
   ValueTextBox.Text = "&H" & asHex(m_Value, 2)
   PropertyChanged "Value"
End Property



'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
   m_Value = m_def_Value
   m_ToolTipTextBit0 = m_def_ToolTipTextBit0
   m_ToolTipTextBit1 = m_def_ToolTipTextBit1
   m_ToolTipTextBit2 = m_def_ToolTipTextBit2
   m_ToolTipTextBit3 = m_def_ToolTipTextBit3
   m_ToolTipTextBit4 = m_def_ToolTipTextBit4
   m_ToolTipTextBit5 = m_def_ToolTipTextBit5
   m_ToolTipTextBit6 = m_def_ToolTipTextBit6
   m_ToolTipTextBit7 = m_def_ToolTipTextBit7
   m_Enabled = m_def_Enabled
End Sub
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(0),BitCheckBox,0,ToolTipText
'Public Property Get ToolTipTextBit0() As String
'   ToolTipTextBit0 = BitCheckBox(0).ToolTipText
'End Property
'
'Public Property Let ToolTipTextBit0(ByVal New_ToolTipTextBit0 As String)
'   BitCheckBox(0).ToolTipText = New_ToolTipTextBit0
'   PropertyChanged "ToolTipTextBit0"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(1),BitCheckBox,1,ToolTipText
'Public Property Get ToolTipTextBit1() As String
'   ToolTipTextBit1 = BitCheckBox(1).ToolTipText
'End Property
'
'Public Property Let ToolTipTextBit1(ByVal New_ToolTipTextBit1 As String)
'   BitCheckBox(1).ToolTipText = New_ToolTipTextBit1
'   PropertyChanged "ToolTipTextBit1"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(2),BitCheckBox,2,ToolTipText
'Public Property Get ToolTipTextBit2() As String
'   ToolTipTextBit2 = BitCheckBox(2).ToolTipText
'End Property
'
'Public Property Let ToolTipTextBit2(ByVal New_ToolTipTextBit2 As String)
'   BitCheckBox(2).ToolTipText = New_ToolTipTextBit2
'   PropertyChanged "ToolTipTextBit2"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(3),BitCheckBox,3,ToolTipText
'Public Property Get ToolTipTextBit3() As String
'   ToolTipTextBit3 = BitCheckBox(3).ToolTipText
'End Property
'
'Public Property Let ToolTipTextBit3(ByVal New_ToolTipTextBit3 As String)
'   BitCheckBox(3).ToolTipText = New_ToolTipTextBit3
'   PropertyChanged "ToolTipTextBit3"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(4),BitCheckBox,4,ToolTipText
'Public Property Get ToolTipTextBit4() As String
'   ToolTipTextBit4 = BitCheckBox(4).ToolTipText
'End Property
'
'
'
'Public Property Let ToolTipTextBit4(ByVal New_ToolTipTextBit4 As String)
'   BitCheckBox(4).ToolTipText = New_ToolTipTextBit4
'   PropertyChanged "ToolTipTextBit4"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(5),BitCheckBox,5,ToolTipText
'Public Property Get ToolTipTextBit5() As String
'   ToolTipTextBit5 = BitCheckBox(5).ToolTipText
'End Property
'
'
'
'Public Property Let ToolTipTextBit5(ByVal New_ToolTipTextBit5 As String)
'   BitCheckBox(5).ToolTipText = New_ToolTipTextBit5
'   PropertyChanged "ToolTipTextBit5"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(6),BitCheckBox,6,ToolTipText
'Public Property Get ToolTipTextBit6() As String
'   ToolTipTextBit6 = BitCheckBox(6).ToolTipText
'End Property
'
'
'
'Public Property Let ToolTipTextBit6(ByVal New_ToolTipTextBit6 As String)
'   BitCheckBox(6).ToolTipText = New_ToolTipTextBit6
'   PropertyChanged "ToolTipTextBit6"
'End Property
'
'
'
''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
''MappingInfo=BitCheckBox(7),BitCheckBox,7,ToolTipText
'Public Property Get ToolTipTextBit7() As String
'   ToolTipTextBit7 = BitCheckBox(7).ToolTipText
'End Property
'
'
'
'Public Property Let ToolTipTextBit7(ByVal New_ToolTipTextBit7 As String)
'   BitCheckBox(7).ToolTipText = New_ToolTipTextBit7
'   PropertyChanged "ToolTipTextBit7"
'End Property
'
'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit0() As String
Attribute ToolTipTextBit0.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit0 = m_ToolTipTextBit0
End Property

Public Property Let ToolTipTextBit0(ByVal New_ToolTipTextBit0 As String)
   m_ToolTipTextBit0 = New_ToolTipTextBit0
   PropertyChanged "ToolTipTextBit0"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit1() As String
Attribute ToolTipTextBit1.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit1 = m_ToolTipTextBit1
End Property

Public Property Let ToolTipTextBit1(ByVal New_ToolTipTextBit1 As String)
   m_ToolTipTextBit1 = New_ToolTipTextBit1
   PropertyChanged "ToolTipTextBit1"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit2() As String
Attribute ToolTipTextBit2.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit2 = m_ToolTipTextBit2
End Property

Public Property Let ToolTipTextBit2(ByVal New_ToolTipTextBit2 As String)
   m_ToolTipTextBit2 = New_ToolTipTextBit2
   PropertyChanged "ToolTipTextBit2"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit3() As String
Attribute ToolTipTextBit3.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit3 = m_ToolTipTextBit3
End Property

Public Property Let ToolTipTextBit3(ByVal New_ToolTipTextBit3 As String)
   m_ToolTipTextBit3 = New_ToolTipTextBit3
   PropertyChanged "ToolTipTextBit3"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit4() As String
Attribute ToolTipTextBit4.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit4 = m_ToolTipTextBit4
End Property

Public Property Let ToolTipTextBit4(ByVal New_ToolTipTextBit4 As String)
   m_ToolTipTextBit4 = New_ToolTipTextBit4
   PropertyChanged "ToolTipTextBit4"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit5() As String
Attribute ToolTipTextBit5.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit5 = m_ToolTipTextBit5
End Property

Public Property Let ToolTipTextBit5(ByVal New_ToolTipTextBit5 As String)
   m_ToolTipTextBit5 = New_ToolTipTextBit5
   PropertyChanged "ToolTipTextBit5"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit6() As String
Attribute ToolTipTextBit6.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit6 = m_ToolTipTextBit6
End Property

Public Property Let ToolTipTextBit6(ByVal New_ToolTipTextBit6 As String)
   m_ToolTipTextBit6 = New_ToolTipTextBit6
   PropertyChanged "ToolTipTextBit6"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,
Public Property Get ToolTipTextBit7() As String
Attribute ToolTipTextBit7.VB_Description = "Returns/sets the text displayed when the mouse is paused over the control."
   ToolTipTextBit7 = m_ToolTipTextBit7
End Property

Public Property Let ToolTipTextBit7(ByVal New_ToolTipTextBit7 As String)
   m_ToolTipTextBit7 = New_ToolTipTextBit7
   PropertyChanged "ToolTipTextBit7"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
   Enabled = m_Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
   Dim control As control
   
   m_Enabled = New_Enabled
   For Each control In Controls
      control.Enabled = m_Enabled
   Next control
   PropertyChanged "Enabled"
End Property

