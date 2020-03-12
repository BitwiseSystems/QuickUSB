Attribute VB_Name = "QuickUSBEEPROM"
'=============================================================================
' Title        : QuickUsbEeprom.frm
' Description  : QuickUSB Control Panel Form
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : None
' History      :
'   Date       Name        Description
'  3/30/2002   bhenry      Initial coding
'
' $History: QuickUSBEeprom.bas $
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/18/03    Time: 2:48p
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbProgrammer
'=============================================================================
Option Explicit

'=============================================================================
' User Defined Types
'=============================================================================
Public Const MAX_INTEL_HEX_RECORD_LENGTH = 16
Type INTEL_HEX_RECORD
   length As Byte
   address As Integer
   Type As Byte
   Data(16) As Byte
End Type


'=============================================================================
' EEPROM Read/Write routines
'=============================================================================
Declare Function QuickUsbSetResetState Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal resetBit As Byte) As Long
Declare Function QuickUsbWriteRamData Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long
Declare Function QuickUsbReadEepromData Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWriteEepromData Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long




Public Function WriteRamFromResource(devName As String, theType As String, id As Integer) As Boolean
   Dim Index As Long
   Dim result As Long
   Dim length As Integer
   Dim address As Integer
   Dim fileLength As Integer
   Dim blockLength As Integer
   Dim hDevice As Long
   Dim ramImage() As Byte
      
   ' Load the binary image
   ramImage = LoadResData(id, theType)
   
   ' Allocate storage for data elements
   blockLength = 64
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      WriteRamFromResource = False
      Exit Function
   End If
   
   ' Put the micro in reset
   result = QuickUsbSetResetState(hDevice, 1)
   If (result = 0) Then
      QuickUsbClose hDevice
      WriteRamFromResource = False
      Exit Function
   End If
   
   fileLength = UBound(ramImage) - 1
   
   Index = 0
   address = 0
   Do
      ' Read the data
      length = Min(blockLength, fileLength - Index)
      
      ' Write the data to the EEPROM
      result = QuickUsbWriteRamData(hDevice, address, ramImage(Index), length)
      If (result = 0) Then
         Exit Do
      End If
      
      ' Update the counters
      address = address + length
      Index = Index + length
            
      ' Redraw the screen
      DoEvents
   Loop Until Index >= fileLength
      
   ' Take the micro out of reset
   result = QuickUsbSetResetState(hDevice, 0)
   If (result = 0) Then
      QuickUsbClose hDevice
      WriteRamFromResource = False
      Exit Function
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   WriteRamFromResource = True
End Function



