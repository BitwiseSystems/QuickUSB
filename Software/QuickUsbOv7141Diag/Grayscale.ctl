VERSION 5.00
Begin VB.UserControl Grayscale 
   ClientHeight    =   3285
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3960
   ClipControls    =   0   'False
   ScaleHeight     =   219
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   264
End
Attribute VB_Name = "Grayscale"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'=============================================================================
' Title        : Grayscale.ctl
' Description  : Grayscale Control
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : None.
' History      :
'   Date       Name        Description
'  5/5/2002   bhenry      Initial coding
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


Const OVERFLOW As Byte = 255
Const UNDERFLOW As Byte = 254

Const BM As Integer = &H4D42       ' BM
Const BI_RGB As Long = 0
Const DIB_RGB_COLORS = 0
Const SRCCOPY = &HCC0020

Dim m_ShowOutOfBounds As Boolean

'Property Variables:
Public MaxValue As Integer
Public MinValue As Integer
Public Random As Boolean
Public Pattern As Boolean
Public Invert As Boolean
'Public Colors As Integer

Private Type BITMAPINFOHEADER
   biSize As Long
   biWidth As Long
   biHeight As Long
   biPlanes As Integer
   biBitCount As Integer
   biCompression As Long
   biSizeImage As Long
   biXPelsPerMeter As Long
   biYPelsPerMeter As Long
   biClrUsed As Long
   biClrImportant As Long
End Type

Private Type RGBQUAD
   rgbBlue As Byte
   rgbGreen As Byte
   rgbRed As Byte
   rgbReserved As Byte
End Type

Private Type BITMAPINFO256
   bmiHeader As BITMAPINFOHEADER
   bmiColors(256) As RGBQUAD
End Type

Private Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Private Declare Function CreateCompatibleBitmap Lib "gdi32" (ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Private Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Private Declare Function SetDIBits Lib "gdi32" (ByVal hdc As Long, ByVal hBitmap As Long, ByVal nStartScan As Long, ByVal nNumScans As Long, lpBits As Any, lpBI As BITMAPINFO256, ByVal wUsage As Long) As Long
Private Declare Function StretchBlt Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long
Private Declare Sub CopyMemory Lib "Kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)


Dim m_Rows As Long
Dim m_Columns As Long
Dim scanLinePad As Long
Dim grayscaleDC As Long
Dim theBitmapHandle As Long
Dim theOldBitmapHandle As Long
Dim theBitmap() As Byte
Dim theData() As Integer
Dim theBitmapInfo As BITMAPINFO256


'Event Declarations:
Event Click() 'MappingInfo=UserControl,UserControl,-1,Click
Attribute Click.VB_Description = "Occurs when the user presses and then releases a mouse button over an object."
Event DblClick() 'MappingInfo=UserControl,UserControl,-1,DblClick
Attribute DblClick.VB_Description = "Occurs when the user presses and releases a mouse button and then presses and releases it again over an object."
Event MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseDown
Attribute MouseDown.VB_Description = "Occurs when the user presses the mouse button while an object has the focus."
Event MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseMove
Attribute MouseMove.VB_Description = "Occurs when the user moves the mouse."
Event MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseUp
Attribute MouseUp.VB_Description = "Occurs when the user releases the mouse button while an object has the focus."

'Default Property Values:
Const m_def_ShowOutOfBounds = 0



'Initialize Properties for User Control
Private Sub UserControl_Initialize()
   Dim count As Integer
   Dim colorsPerStep As Single
   
   ' Set some defaults
   MinValue = 0
   MaxValue = 16384
   m_Rows = 128
   m_Columns = 164
   
   grayscaleDC = CreateCompatibleDC(0)
   
   ' Initialize the normal bitmap information stuff
   theBitmapInfo.bmiHeader.biSize = 40
   theBitmapInfo.bmiHeader.biPlanes = 1
   theBitmapInfo.bmiHeader.biBitCount = 8
   theBitmapInfo.bmiHeader.biCompression = BI_RGB
   
   BuildPalette
End Sub



Private Sub UserControl_Resize()
  ResizeBitmap
End Sub



Private Sub BuildPalette()
   Dim count As Integer
   Dim colorsPerStep As Single

   If (m_ShowOutOfBounds) Then
      ' Build the color table
      colorsPerStep = 255 / (UNDERFLOW - 1)
      For count = 0 To UNDERFLOW - 1
         theBitmapInfo.bmiColors(count).rgbRed = count * colorsPerStep
         theBitmapInfo.bmiColors(count).rgbGreen = count * colorsPerStep
         theBitmapInfo.bmiColors(count).rgbBlue = count * colorsPerStep
      Next count
      
      ' Define underflow (128) as Blue
      theBitmapInfo.bmiColors(UNDERFLOW).rgbRed = 0
      theBitmapInfo.bmiColors(UNDERFLOW).rgbGreen = 0
      theBitmapInfo.bmiColors(UNDERFLOW).rgbBlue = 255
      
      ' Define overflow (129) as Red
      theBitmapInfo.bmiColors(OVERFLOW).rgbRed = 255
      theBitmapInfo.bmiColors(OVERFLOW).rgbGreen = 0
      theBitmapInfo.bmiColors(OVERFLOW).rgbBlue = 0
   Else
      ' Build the color table
      colorsPerStep = 1
      For count = 0 To 255
         theBitmapInfo.bmiColors(count).rgbRed = count * colorsPerStep
         theBitmapInfo.bmiColors(count).rgbGreen = count * colorsPerStep
         theBitmapInfo.bmiColors(count).rgbBlue = count * colorsPerStep
      Next count
   End If
   
   ' Resize the bitmap
   'Call ResizeBitmap
End Sub



Private Sub ResizeBitmap()
   Dim row As Long
   Dim col As Long
   
   ' Generate a new palette
   BuildPalette
   
   ' Calculate the number of scanline columns
   If ((m_Columns Mod 4) = 0) Then
      scanLinePad = 0
   Else
      scanLinePad = 4 - (m_Columns Mod 4)
   End If
   
   ' Resize the arrays
   ReDim theData(m_Columns * m_Rows)
   ReDim theBitmap((m_Columns + scanLinePad) * m_Rows)
   
   ' Create the DIB section
   'theBitmapHandle = CreateDIBitmap(hDC, theBitmapInfo.bmiHeader, CBM_INIT, theBitmap(0), theBitmapInfo, DIB_RGB_COLORS)
   theBitmapHandle = CreateCompatibleBitmap(hdc, m_Columns, m_Rows)
   
   ' Select the DIB section into the current DC
   theOldBitmapHandle = SelectObject(grayscaleDC, theBitmapHandle)
   
   ' Delete the old bitmap
   Call DeleteObject(theOldBitmapHandle)
   
   ' Update the BITMAPINFOHEADER to reflect the new bitmap size
   theBitmapInfo.bmiHeader.biWidth = m_Columns
   If (Invert) Then
      theBitmapInfo.bmiHeader.biHeight = m_Rows
   Else
      theBitmapInfo.bmiHeader.biHeight = -m_Rows
   End If
   
   ' Copy the data into the DIB section
   Call Refresh
End Sub



Public Sub SetRandomData()
   Dim count As Long
   
   ' Display random number data
      For count = 0 To m_Rows * m_Columns
         theData(count) = CInt((Rnd * ((MaxValue + 2) - MinValue)) + (MinValue - 1))
      Next count
End Sub



Sub SetTestPattern()
Attribute SetTestPattern.VB_Description = "Forces a complete repaint of a object."
   Dim count As Long
   Dim row As Long
   Dim column As Long
   Dim halfMax As Integer
   
   ' Put test pattern into the data array
   halfMax = (MaxValue - MinValue) / 2
   For count = 0 To m_Rows * m_Columns
      row = count \ m_Columns
      column = count Mod m_Columns
      If (row >= 5 And row <= (m_Rows - 5) And column >= 5 And column <= 15) Then
         If (row = 5 Or row = (m_Rows - 5) Or column = 5 Or column = 15) Then
            theData(count) = CInt(MaxValue)
         Else
            theData(count) = CInt((row * ((MaxValue - MinValue) / (m_Rows - 5))) + MinValue)
         End If
      Else
         theData(count) = CInt((column * ((MaxValue - MinValue) / m_Columns)) + MinValue)
      End If
   Next count

End Sub



Public Sub Refresh()
   Dim count As Long
   Dim destCount As Long
   Dim colorsPerVolt As Single
   
   If (Random = True) Then
      SetRandomData
   End If
   
   ' Display a test pattern
   If (Pattern = True) Then
      SetTestPattern
   End If
   
   ' Scale and bin the data to colortable entries
   colorsPerVolt = (UNDERFLOW - 1) / (MaxValue - MinValue)
   count = 0
   destCount = 0
   Do
      ' Mark out of bounds
      If (theData(count) < MinValue) Then
         If (m_ShowOutOfBounds) Then
            theBitmap(destCount) = UNDERFLOW
         Else
            theBitmap(destCount) = 0
         End If
      ElseIf (theData(count) > MaxValue) Then
         If (m_ShowOutOfBounds) Then
            theBitmap(destCount) = OVERFLOW
         Else
            theBitmap(destCount) = UNDERFLOW - 1
         End If
      Else
         theBitmap(destCount) = CInt((theData(count) - MinValue) * colorsPerVolt)
      End If
      
      ' Increment the counters
      count = count + 1
      destCount = destCount + 1
      
      ' Add line padding if needed
      If (count Mod m_Columns = 0) Then
         destCount = destCount + scanLinePad
      End If
            
   Loop Until (count >= m_Rows * m_Columns)
   
   ' Put the bitmap data into the DIB bitmap area
   Call SetDIBits(hdc, theBitmapHandle, 0, m_Rows, theBitmap(0), theBitmapInfo, DIB_RGB_COLORS)
   
   ' Update the display
   Call UserControl_Paint
End Sub



Private Sub UserControl_Paint()
   Call StretchBlt(hdc, 0, 0, ScaleWidth, ScaleHeight, grayscaleDC, 0, 0, m_Columns, m_Rows, SRCCOPY)
End Sub



Public Property Get Pixel(x As Single, y As Single) As Variant
   Pixel = theBitmap(x + (y * m_Columns))
End Property



Public Property Get Data(x As Single, y As Single) As Variant
   Data = theData(x + (y * m_Columns))
End Property



Public Property Get Bitmap(x As Single, y As Single) As Variant
   Bitmap = theBitmap(x + (y * m_Columns))
End Property



Public Property Get rows() As Long
   rows = m_Rows
End Property



Public Property Let rows(ByVal New_Rows As Long)
   m_Rows = New_Rows
   PropertyChanged "Rows"
   ResizeBitmap
End Property



Public Property Get Columns() As Long
   Columns = m_Columns
End Property



Public Property Let Columns(ByVal New_Columns As Long)
   m_Columns = New_Columns
   PropertyChanged "Columns"
   ResizeBitmap
End Property



Public Property Get Enabled() As Boolean
   Enabled = UserControl.Enabled
End Property



Public Property Let Enabled(ByVal New_Enabled As Boolean)
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
   UserControl.Enabled() = New_Enabled
   PropertyChanged "Enabled"
End Property



Private Sub UserControl_Click()
   RaiseEvent Click
End Sub



Private Sub UserControl_DblClick()
   RaiseEvent DblClick
End Sub



Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
   RaiseEvent MouseDown(Button, Shift, Int(x * m_Columns / ScaleWidth), Int(y * m_Rows / ScaleHeight))
End Sub



Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
   RaiseEvent MouseMove(Button, Shift, Int(x * m_Columns / ScaleWidth), Int(y * m_Rows / ScaleHeight))
End Sub



Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
   RaiseEvent MouseUp(Button, Shift, Int(x * m_Columns / ScaleWidth), Int(y * m_Rows / ScaleHeight))
End Sub



Public Property Get hwnd() As Long
Attribute hwnd.VB_Description = "Returns a handle (from Microsoft Windows) to an object's window."
   hwnd = UserControl.hwnd
End Property



Public Sub SetData(ByRef theNewData() As Integer)
   Call CopyMemory(theData(0), theNewData(0), m_Rows * m_Columns * 2)
   Call Refresh
End Sub



Public Sub SetBitmap(ByRef newBitmap() As Byte)
   Call CopyMemory(theBitmap(0), newBitmap(0), m_Rows * m_Columns)
   
   ' Put the bitmap data into the DIB bitmap area
   Call SetDIBits(hdc, theBitmapHandle, 0, m_Rows, theBitmap(0), theBitmapInfo, DIB_RGB_COLORS)
   
   ' Update the display
   Call UserControl_Paint
End Sub



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get ShowOutOfBounds() As Boolean
   ShowOutOfBounds = m_ShowOutOfBounds
End Property



Public Property Let ShowOutOfBounds(ByVal New_ShowOutOfBounds As Boolean)
   m_ShowOutOfBounds = New_ShowOutOfBounds
   PropertyChanged "ShowOutOfBounds"
End Property



'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
   UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
   m_Rows = PropBag.ReadProperty("Rows", ScaleHeight)
   m_Columns = PropBag.ReadProperty("Columns", ScaleWidth)
   MinValue = PropBag.ReadProperty("MinValue", 0)
   MaxValue = PropBag.ReadProperty("MaxValue", 16384)
   Random = PropBag.ReadProperty("Random", False)
   Pattern = PropBag.ReadProperty("Pattern", False)
   Invert = PropBag.ReadProperty("Invert", False)
   m_ShowOutOfBounds = PropBag.ReadProperty("ShowOutOfBounds", m_def_ShowOutOfBounds)
   ResizeBitmap
End Sub



'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
   Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
   Call PropBag.WriteProperty("Rows", m_Rows, ScaleHeight)
   Call PropBag.WriteProperty("Columns", m_Columns, ScaleWidth)
   Call PropBag.WriteProperty("MinValue", MinValue, 0)
   Call PropBag.WriteProperty("MaxValue", MaxValue, 16384)
   Call PropBag.WriteProperty("Random", Random, False)
   Call PropBag.WriteProperty("Pattern", Pattern, False)
   Call PropBag.WriteProperty("Invert", Invert, False)
   Call PropBag.WriteProperty("ShowOutOfBounds", m_ShowOutOfBounds, m_def_ShowOutOfBounds)
End Sub



'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
   m_ShowOutOfBounds = m_def_ShowOutOfBounds
   BuildPalette
End Sub

