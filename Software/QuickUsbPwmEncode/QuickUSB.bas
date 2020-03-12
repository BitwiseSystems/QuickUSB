Attribute VB_Name = "QuickUSB"
'=============================================================================
' Title        : QuickUSB.bas
' Description  : QuickUSB Library Function Declarations
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
' History      :
'   Date       Name        Description
'  5/5/2002   bhenry      Initial coding
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
' $History: QuickUSB.bas $
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
' *****************  Version 5  *****************
' User: Blake Henry  Date: 8/07/03    Time: 7:12p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added timer support
'
' *****************  Version 4  *****************
' User: Blake Henry  Date: 6/24/03    Time: 11:51a
' Updated in $/Projects/Ultralink/Usb796/Software
' Added Align function to align data to a certain blocksize
'
' *****************  Version 3  *****************
' User: Blake Henry  Date: 6/20/03    Time: 7:38p
' Updated in $/Projects/Ultralink/Usb796/Software
' Massive changes
'
'=============================================================================
Option Explicit


'=============================================================================
' Variables
'=============================================================================
Dim devName As String


'=============================================================================
' Win32 Library functions useful for VB apps
'=============================================================================
Declare Sub Sleep Lib "Kernel32" (ByVal dwMilliseconds As Long)
Declare Sub CopyMemory Lib "Kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)


'=============================================================================
' General purpose routines
'=============================================================================
Declare Function QuickUsbFindModules Lib "QuickUsb" _
   (ByVal nameList As String, ByVal length As Long) As Long
Declare Function QuickUsbOpen Lib "QuickUsb" _
   (ByRef hDevice As Long, ByVal devName As String) As Long
Declare Function QuickUsbClose Lib "QuickUsb" (ByVal hDevice As Long) As Long
Declare Function QuickUsbGetStringDescriptor Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal Index As Byte, _
    ByVal theString As String, ByVal length As Integer) As Long


'=============================================================================
' FPGA Configuration Functions
'=============================================================================
Declare Function QuickUsbStartFpgaConfiguration Lib "QuickUsb" _
   (ByVal hDevice As Long) As Long
Declare Function QuickUsbWriteFpgaData Lib "QuickUsb" _
   (ByVal hDevice As Long, Data As Any, ByVal length As Long) As Long
Declare Function QuickUsbIsFpgaConfigured Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef isConfigured As Integer) As Long


'=============================================================================
' QuickUSB Setting Functions
'=============================================================================
Global Const SETTING_WORDWIDE = 1
' WORDWIDE is the address of the WORDWIDE QuickUSB setting.
' If the value of WORDWIDE is 1, high-speed parallel port transfers are
' one WORD (16-bits) wide.  If 0, the transfers are one BYTE (8-bits) wide.
' This setting affects the address bus as well since it will increment once
' per value.  If the value of WORDWIDE is 0, the upper 8 bits of the FD bus
' will be left unprogrammed so they may be used for other functions.

Global Const SETTING_DATAADDRESS = 2
' WORDWIDE is the address of the WORDWIDE QuickUSB setting.
' If the value of WORDWIDE is 1, high-speed parallel port transfers are
' one WORD (16-bits) wide.  If 0, the transfers are one BYTE (8-bits) wide.
' This setting affects the address bus as well since it will increment once
' per value.  If the value of WORDWIDE is 0, the upper 8 bits of the FD bus
' will be left unprogrammed so they may be used for other functions.

Global Const SETTING_FIFOCONFIG = 3
' Controls the FX2 IFCONFIG register.  Sets the external FIFO mode,
' clock speeds, output enables, IFCLK polarity and others.

Global Const SETTING_FPGATYPE = 4

Global Const SETTING_CPUCONFIG = 5

Global Const FIFOCONFIG_GPIF = &HFA
Global Const FIFOCONFIG_SLAVE = &H3

'=============================================================================
' Timer Constants
'=============================================================================
' Timer Registers
Global Const TIMER_REG_TCON = 0
Global Const TIMER_REG_TMOD = 1
Global Const TIMER_REG_T0 = 2
Global Const TIMER_REG_T1 = 3
Global Const TIMER_REG_CKCON = 4
Global Const TIMER_REG_IE = 5
Global Const TIMER_REG_T2CON = 6
Global Const TIMER_REG_RCAP2 = 7
Global Const TIMER_REG_T2 = 8
Global Const TIMER_REG_PORTACFG = 9
Global Const TIMER_REG_PORTECFG = 10
Global Const TIMER_REG_PWM0LOW = 11
Global Const TIMER_REG_PWM0HIGH = 12
Global Const TIMER_REG_PWM1LOW = 13
Global Const TIMER_REG_PWM1HIGH = 14


' Timer ISR registers (all these guys are read at the same time in an ISR)
Global Const TIMER_ISRREG_T0 = 0
Global Const TIMER_ISRREG_T1 = 1
Global Const TIMER_ISRREG_RCAP2 = 2
Global Const TIMER_ISRREG_T2 = 3


Declare Function QuickUsbReadSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByRef theValue As Integer) As Long
Declare Function QuickUsbWriteSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByVal theValue As Integer) As Long


'=============================================================================
' QuickUSB High-Speed Parallel (HS) Port Functions
'=============================================================================
Declare Function QuickUsbReadCommand Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWriteCommand Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long
Declare Function QuickUsbReadData Lib "QuickUsb" _
   (ByVal hDevice As Long, Data As Any, ByRef length As Long) As Long
Declare Function QuickUsbWriteData Lib "QuickUsb" _
   (ByVal hDevice As Long, Data As Any, ByVal length As Long) As Long
Declare Function QuickUsbReadDataString Lib "QuickUsb" Alias "QuickUsbReadData" _
   (ByVal hDevice As Long, ByVal str As String, ByRef length As Long) As Long
Declare Function QuickUsbWriteDataString Lib "QuickUsb" Alias "QuickUsbWriteData" _
   (ByVal hDevice As Long, ByVal str As String, ByVal length As Long) As Long


'=============================================================================
' QuickUSB General Purpose Port Functions
'=============================================================================
Declare Function QuickUsbReadPortDir Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   ByRef Data As Byte) As Long
Declare Function QuickUsbWritePortDir Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   ByVal Data As Byte) As Long
Declare Function QuickUsbReadPort Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWritePort Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long


'=============================================================================
' QuickUSB RS-232 Port Functions
'=============================================================================
Declare Function QuickUsbSetRS232BaudRate Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal baudRate As Long) As Long
Declare Function QuickUsbGetNumRS232 Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, ByRef length As Integer) As Long
Declare Function QuickUsbFlushRS232 Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte) As Long
Declare Function QuickUsbReadRS232 Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, _
   Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWriteRS232 Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, _
   Data As Any, ByVal length As Integer) As Long
Declare Function QuickUsbReadRS232String Lib "QuickUsb" Alias "QuickUsbReadRS232" _
   (ByVal hDevice As Long, ByVal portNum As Byte, _
   ByVal str As String, ByRef length As Integer) As Long
'-----------------------------------------------------------------------------
' Name    : QuickUsbReadRS232String
' Purpose : Read a string from the RS232 receive buffer.
' Params  :
'  hDevice - A HANDLE to an open QuickUSB module.
'  portNum - 0 or 1 for ports 0 and 1 respectively.
'  data - A 'filled' string in which to place the received data
'  length - The length of the passed 'filled' string
' Results : Returns a non-zero number on success, 0 otherwise.
' Notes   : The maximum size of the receive buffer is 16 bytes.  If the
'   buffer does not contain the number of characters requested, the routine
'   will wait until the characters become available.
'-----------------------------------------------------------------------------
Declare Function QuickUsbWriteRS232String Lib "QuickUsb" Alias "QuickUsbWriteRS232" _
   (ByVal hDevice As Long, ByVal portNum As Byte, _
   ByVal str As String, ByVal length As Integer) As Long
'-----------------------------------------------------------------------------
' Name    : QuickUsbWriteRS232String
' Purpose : Write a string to the RS232 transmit buffer.
' Params  :
'  hDevice - A HANDLE to an open QuickUSB module.
'  portNum - 0 or 1 for ports 0 and 1 respectively.
'  data - A byte array which contains the data to send
'  length - The length of the passed byte array
' Results : Returns a non-zero number on success, 0 otherwise.
' Notes   : The maximum size of the transmit buffer is 16 bytes.  If more
'  than 16 bytes are sent, the routine will wait until the last of the bytes
'  can be placed into the buffer.
'-----------------------------------------------------------------------------


'=============================================================================
' QuickUSB SPI Port Functions
'=============================================================================
Declare Function QuickUsbReadSpi Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWriteSpi Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, Data As Any, ByVal length As Integer) As Long
Declare Function QuickUsbWriteReadSpi Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal portNum As Byte, Data As Any, ByVal length As Integer) As Long


'=============================================================================
' QuickUSB I2C Port Functions
'=============================================================================
Declare Function QuickUsbReadI2C Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByRef length As Integer) As Long
Declare Function QuickUsbWriteI2C Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long

'=============================================================================
' QuickUSB Timer Functions
'=============================================================================
Declare Function QuickUsbReadTimerReg Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, ByRef Value As Integer) As Long
Declare Function QuickUsbWriteTimerReg Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, ByVal Value As Integer) As Long
Declare Function QuickUsbReadTimerIsrRegs Lib "QuickUsb" _
   (ByVal hDevice As Long, Data As Any, ByRef length As Integer) As Long


'=============================================================================
' General Purpose Functions
'=============================================================================
Public Function Min(X As Variant, Y As Variant) As Variant
   If (X < Y) Then
      Min = X
   Else
      Min = Y
   End If
End Function


Public Function Max(X As Variant, Y As Variant) As Variant
   If (X > Y) Then
      Max = X
   Else
      Max = Y
   End If
End Function


Public Function Align(length As Variant, blockSize As Variant) As Variant
   If (length Mod blockSize = 0) Then
      Align = length
   Else
      Align = length + (length Mod blockSize)
   End If
End Function



Public Function asHex(theData As Variant, length As Variant) As String
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



Function ByteArrayToString(theData() As Byte) As String
   Dim count As Integer
   Dim theString As String
   
   For count = 0 To UBound(theData) - 1
      theString = theString & "&h" & asHex(theData(count), 2) & " "
   Next
   ByteArrayToString = theString
End Function



Function StringToByteArray(theData() As Byte, theString As String) As Integer
   Dim dataIndex As Integer
   Dim startStrIndex As Long
   Dim endStrIndex As Integer
   Dim strLen As Integer
   
   ' Exit for an empty string
   If (Len(theString) = 0) Then
      StringToByteArray = 0
      Exit Function
   End If
   
   ' Allocate space for the data
   ReDim theData(Len(theString))         ' Guess that the data length is the maximum
   
   ' Parse text and put it into the array
   dataIndex = 0
   startStrIndex = 1
   strLen = Len(theString)
   Do While (startStrIndex <= strLen)
      ' Find the first non-space
      Do While (Mid(theString, startStrIndex, 1) = " " And startStrIndex <= strLen)
         startStrIndex = startStrIndex + 1
      Loop
      
      ' Find the first space
      endStrIndex = startStrIndex
      Do While (Mid(theString, endStrIndex, 1) <> " " And endStrIndex <= strLen)
         endStrIndex = endStrIndex + 1
      Loop
      
      ' Get the text
      theData(dataIndex) = CByte(Val(Mid(theString, startStrIndex, endStrIndex - startStrIndex)) And &HFF)
      dataIndex = dataIndex + 1
      startStrIndex = endStrIndex
   Loop
   
   ' Resize the data to the right length
   ReDim Preserve theData(dataIndex - 1)
   
   ' Return the actual length
   StringToByteArray = dataIndex
End Function



Function InitInpOut()
   ' Initialize the InpOut Module to find and use the first available
   ' QuickUSb module
   Dim hDevice As Long
   Dim result As Long
   Dim devList As String
   
   ' Get the list of modules
   devList = Space(128)
   result = QuickUsbFindModules(devList, 128)
   If (devList = "") Then
      MsgBox "Cannot find any QuickUSB Modules", , "InpOut:InitInpOut"
      InitInpOut = False
      Return
   End If
   
   ' Extract the first device name from the list
   devName = Mid(devList, 1, InStr(1, devList, Chr(0), vbBinaryCompare) - 1)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:InitInpOut"
      InitInpOut = False
      Exit Function
   End If
   
   ' Tell the device to run in byte mode only
   result = QuickUsbWriteSetting(hDevice, SETTING_WORDWIDE, 0)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the result
   If (result = 0) Then
      MsgBox "Cannot write WORDWIDE setting", , "InpOut:InitInpOut"
      InitInpOut = False
      Exit Function
   End If
   
   InitInpOut = True
End Function



Sub SetInpOutDevName(theDevName As String)
' Explicitly sets the device name of the QuickUsb module to use
   devName = theDevName
End Sub



Function Inp(ByVal address As Integer)
' Use this function just like the Qbasic Inp function
' address - The address to read from
   Dim result As Long
   Dim hDevice As Long
   Dim theData As Byte
   
   ' Check devName
   If (devName = "") Then
      MsgBox "Invalid device name", , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   ' Read the data
   result = QuickUsbReadCommand(hDevice, address, theData, 1)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Inp failed", , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   Inp = theData
End Function



Sub Out(ByVal address As Integer, ByVal theData As Integer)
   ' Use this function just like the Qbasic Out function
   ' address - The address to write to
   ' theData - the Byte to write
   Dim result As Long
   Dim hDevice As Long
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:Out"
      Exit Sub
   End If
   
   ' Write the data
   result = QuickUsbWriteCommand(hDevice, address, theData, 1)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Out failed", , "InpOut:Out"
      Exit Sub
   End If
   
End Sub



Sub InString(str As String, ByRef length As Long)
   ' Use this function to read a string from the QuickUSB module
   ' str - The string to read into
   ' length - the number of characters (bytes) to read
   ' Returns the number of bytes read in length
   Dim result As Long
   Dim hDevice As Long
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "QuickUSB: InString"
      Exit Sub
   End If
   
   ' Allocate and fill the string with NULLs
   str = String(length, Chr(0))
   
   ' Read the data
   result = QuickUsbReadDataString(hDevice, str, length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "InString failed", , "QuickUSB: InString"
      Exit Sub
   End If
End Sub



Sub OutString(str As String, ByVal length As Long)
   ' Use this function to write a string to the QuickUSB module
   ' str - The string to write to
   ' length - the number of characters (bytes) to write
   Dim result As Long
   Dim hDevice As Long
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "QuickUSB: OutString"
      Exit Sub
   End If
   
   ' Write the data
   result = QuickUsbWriteDataString(hDevice, str, length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "InString failed", , "QuickUSB: OutString"
      Exit Sub
   End If
End Sub



Public Sub WriteSetting(devName As String, settingNum As Integer, theValue As Integer)
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName
      Exit Sub
   End If
   
   result = QuickUsbWriteSetting(hDevice, settingNum, theValue)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write setting"
      Exit Sub
   End If
   
End Sub



Public Sub ReadSetting(devName As String, settingNum As Integer, ByRef theValue As Integer)
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open Device:" & devName
      Exit Sub
   End If
   
   result = QuickUsbReadSetting(hDevice, settingNum, theValue)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Cannot write setting"
      Exit Sub
   End If
   
End Sub




