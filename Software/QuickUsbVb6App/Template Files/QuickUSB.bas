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
' *****************  Version 2  *****************
' User: Blake Henry  Date: 9/30/05    Time: 10:49a
' Updated in $/Projects/OLIS/Software/UdioDiagVb
'
' *****************  Version 18  *****************
' User: Bob Jacobson Date: 8/30/05    Time: 11:18a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Changes for setting defaults.
'
' *****************  Version 17  *****************
' User: Bob Jacobson Date: 8/04/05    Time: 9:27a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add async read and write capability
'
' *****************  Version 16  *****************
' User: Bob Jacobson Date: 8/01/05    Time: 3:21p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Async I/O fixes.
'
' *****************  Version 15  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 9:24a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Merged the features from all the different versions (timeout, about,
' etc.)
'
' *****************  Version 14  *****************
' User: Blake Henry  Date: 2/16/05    Time: 9:35a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added SETTING_FIFOCONFIG bit checkbox
' Added SETTING_SLAVEFIFOFLAGS bit checkboxes
'
' *****************  Version 13  *****************
' User: Blake Henry  Date: 2/04/05    Time: 2:16p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 9  *****************
' User: Blake Henry  Date: 9/03/03    Time: 9:50a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Removed antiquated InP/OutP functions
'
' *****************  Version 8  *****************
' User: Blake Henry  Date: 8/27/03    Time: 6:59p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Fixed headers
'
' *****************  Version 6  *****************
' User: Blake Henry  Date: 8/21/03    Time: 8:57a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added timer constants
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
' Constants
'=============================================================================
' Win32 Constants
Public Const REALTIME_PRIORITY_CLASS = &H100

' QuickUSB Constants
Global Const QUICKUSB_ERROR_NO_ERROR = 0
Global Const QUICKUSB_ERROR_OUT_OF_MEMORY = 1
Global Const QUICKUSB_ERROR_CANNOT_OPEN_MODULE = 2
Global Const QUICKUSB_ERROR_CANNOT_FIND_DEVICE = 3
Global Const QUICKUSB_ERROR_IOCTL_FAILED = 4
Global Const QUICKUSB_ERROR_INVALID_PARAMETER = 5
Global Const QUICKUSB_ERROR_TIMEOUT = 6
Global Const QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED = 7



'=============================================================================
' Variables
'=============================================================================
Dim devName As String


'=============================================================================
' Win32 Library functions useful for VB apps
'=============================================================================
Declare Sub Sleep Lib "Kernel32" (ByVal dwMilliseconds As Long)
Declare Sub CopyMemory Lib "Kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)
Declare Function GetTickCount Lib "Kernel32" () As Long
Declare Function SetPriorityClass Lib "Kernel32" (ByVal hProcess As Long, ByVal dwPriorityClass As Long) As Long
Declare Function GetCurrentProcess Lib "Kernel32" () As Long


'=============================================================================
' General purpose routines
'=============================================================================
Declare Function QuickUsbGetLastError Lib "QuickUsb" _
   (ByRef error As Long) As Long
Declare Function QuickUsbFindModules Lib "QuickUsb" _
   (ByVal nameList As String, ByVal length As Long) As Long
Declare Function QuickUsbFindModulesByte Lib "QuickUsb" Alias "QuickUsbFindModules" _
   (nameList As Any, ByVal length As Long) As Long
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
Global Const SETTING_EP26CONFIG = 0
Global Const SETTING_WORDWIDE = 1
Global Const SETTING_DATAADDRESS = 2
Global Const SETTING_FIFOCONFIG = 3
Global Const SETTING_FPGATYPE = 4
Global Const SETTING_CPUCONFIG = 5
Global Const SETTING_SPIENDIAN = 6
Global Const SETTING_SLAVEFIFOFLAGS = 7
Global Const SETTING_I2CTL = 8
Global Const SETTING_PORTA = 9
Global Const SETTING_PORTB = 10
Global Const SETTING_PORTC = 11
Global Const SETTING_PORTD = 12
Global Const SETTING_PORTE = 13
Global Const SETTING_PORTAECFG = 14
Global Const SETTING_PINFLAGS = 15


Global Const FIFOCONFIG_GPIF = &HFA
Global Const FIFOCONFIG_SLAVE = &H3

Declare Function QuickUsbReadSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByRef theValue As Integer) As Long
Declare Function QuickUsbWriteSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByVal theValue As Integer) As Long

Declare Function QuickUsbSetResetState Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal state As Byte) As Long


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
Declare Function QuickUsbWriteRS232String Lib "QuickUsb" Alias "QuickUsbWriteRS232" _
   (ByVal hDevice As Long, ByVal portNum As Byte, _
   ByVal str As String, ByVal length As Integer) As Long


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
Const TIMER_TCON As Integer = 0
Const TIMER_TMOD As Integer = 1
Const TIMER_T0 As Integer = 2
Const TIMER_T1 As Integer = 3
Const TIMER_CKCON As Integer = 4
Const TIMER_IE As Integer = 5
Const TIMER_T2CON As Integer = 6
Const TIMER_RCAP2 As Integer = 7
Const TIMER_T2 As Integer = 8
Const TIMER_PORTACFG As Integer = 9
Const TIMER_PORTECFG As Integer = 10

Declare Function QuickUsbReadTimerReg Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, ByRef Value As Integer) As Long
Declare Function QuickUsbWriteTimerReg Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, ByVal Value As Integer) As Long
Declare Function QuickUsbReadTimerIsrRegs Lib "QuickUsb" _
   (ByVal hDevice As Long, Data As Any, ByRef length As Integer) As Long

Declare Function QuickUsbGetDllVersion Lib "QuickUsb" _
   (ByRef major As Integer, ByRef minor As Integer, ByRef revision As Integer) As Long
Declare Function QuickUsbGetDriverVersion Lib "QuickUsb" _
   (ByRef major As Integer, ByRef minor As Integer, ByRef revision As Integer) As Long
   
Declare Function QuickUsbSetTimeout Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal Timeout As Long) As Long

'=============================================================================
' Asynchronous I/O Functions
'=============================================================================
Declare Function QuickUsbReadDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByRef length As Long, ByRef transaction As Byte) As Long
Declare Function QuickUsbWriteDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByVal length As Long, ByRef transaction As Byte) As Long
Declare Function QuickUsbAsyncWait Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef length As Long, ByVal transaction As Byte, ByVal immediate As Byte) As Long


Public Function QuickUsbErrorString(errorCode As Long) As String
   Select Case errorCode
   Case QUICKUSB_ERROR_NO_ERROR
      QuickUsbErrorString = "No Error"
   Case QUICKUSB_ERROR_OUT_OF_MEMORY
      QuickUsbErrorString = "Out of memory"
   Case QUICKUSB_ERROR_CANNOT_OPEN_MODULE
      QuickUsbErrorString = "Cannot open module"
   Case QUICKUSB_ERROR_CANNOT_FIND_DEVICE
      QuickUsbErrorString = "Cannot find module"
   Case QUICKUSB_ERROR_IOCTL_FAILED
      QuickUsbErrorString = "Call to QuickUSB driver failed"
   Case QUICKUSB_ERROR_INVALID_PARAMETER
      QuickUsbErrorString = "Invalid parameter"
   Case QUICKUSB_ERROR_TIMEOUT
      QuickUsbErrorString = "I/O timeout"
   Case QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED
      QuickUsbErrorString = "Function not supported"
   End Select
End Function


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
   Dim strlen As Integer
   
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
   strlen = Len(theString)
   Do While (startStrIndex <= strlen)
      ' Find the first non-space
      Do While (Mid(theString, startStrIndex, 1) = " " And startStrIndex <= strlen)
         startStrIndex = startStrIndex + 1
      Loop
      
      ' Find the first space
      endStrIndex = startStrIndex
      Do While (Mid(theString, endStrIndex, 1) <> " " And endStrIndex <= strlen)
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




