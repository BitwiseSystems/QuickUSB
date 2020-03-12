Attribute VB_Name = "QuickUSB"
'=============================================================================
' Title        : QuickUSB.bas
' Description  : QuickUSB Library Function Declarations
' Copyright    : Copyright (c) 2012 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
' History      :
'
' Copyright © 2012 Bitwise Systems, Inc.  All rights reserved.
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
' $History: QuickUSB.bas $
'
'=============================================================================
Option Explicit

'=============================================================================
' QuickUSB String Descriptor Indices
'=============================================================================
Global Const QUICKUSB_MAKE = 1
Global Const QUICKUSB_MODEL = 2
Global Const QUICKUSB_SERIAL = 3


'=============================================================================
' QuickUSB Port Addresses
'=============================================================================
Global Const PORT_A = 0
Global Const PORT_B = 1
Global Const PORT_C = 2
Global Const PORT_D = 3
Global Const PORT_E = 4


'=============================================================================
' QuickUSB Settings
'=============================================================================
Global Const SETTING_EP26CONFIG = 0
Global Const SETTING_WORDWIDE = 1
Global Const SETTING_DATAADDRESS = 2
Global Const SETTING_FIFO_CONFIG = 3
Global Const SETTING_FPGATYPE = 4
Global Const SETTING_CPUCONFIG = 5
Global Const SETTING_SPICONFIG = 6
Global Const SETTING_SLAVEFIFOFLAGS = 7
Global Const SETTING_I2CTL = 8
Global Const SETTING_PORTA = 9
Global Const SETTING_PORTB = 10
Global Const SETTING_PORTC = 11
Global Const SETTING_PORTD = 12
Global Const SETTING_PORTE = 13
Global Const SETTING_PORTACCFG = 14
Global Const SETTING_PINFLAGS = 15
Global Const SETTING_VERSIONBUILD = 16
Global Const SETTING_VERSIONSPEED = 17
Global Const SETTING_TIMEOUT_HIGH = 18
Global Const SETTING_TIMEOUT_LOW = 19
Global Const SETTING_THREADS = 20


'=============================================================================
' FPGA Configuration Type Settings
'=============================================================================
Global Const FPGATYPE_ALTERAPS = 0
Global Const FPGATYPE_XILINXSS = 1


'=============================================================================
' QuickUSB Error Codes
'=============================================================================
Global Const QUICKUSB_ERROR_NO_ERROR = 0
Global Const QUICKUSB_ERROR_OUT_OF_MEMORY = 1
Global Const QUICKUSB_ERROR_CANNOT_OPEN_MODULE = 2
Global Const QUICKUSB_ERROR_CANNOT_FIND_DEVICE = 3
Global Const QUICKUSB_ERROR_IOCTL_FAILED = 4
Global Const QUICKUSB_ERROR_INVALID_PARAMETER = 5
Global Const QUICKUSB_ERROR_TIMEOUT = 6
Global Const QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED = 7
Global Const QUICKUSB_ERROR_I2C_BUS_ERROR = 8
Global Const QUICKUSB_ERROR_I2C_NO_ACK = 9
Global Const QUICKUSB_ERROR_I2C_SLAVE_WAIT = 10
Global Const QUICKUSB_ERROR_I2C_TIMEOUT = 11
Global Const QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE = 12
Global Const QUICKUSB_ERROR_ALREADY_OPENED = 13
Global Const QUICKUSB_ERROR_CANNOT_CLOSE_MODULE = 14
Global Const QUICKUSB_ERROR_FPGA_INIT_FAILED = 15
Global Const QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_512 = 16
Global Const QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_64 = 17
Global Const QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR = 18
Global Const QUICKUSB_ERROR_ABORTED = 19
Global Const QUICKUSB_ERROR_DEPRECATED = 20
Global Const QUICKUSB_ERROR_INVALID_SERIAL = 21
Global Const QUICKUSB_ERROR_CANNOT_OPEN_FILE = 22
Global Const QUICKUSB_ERROR_VERIFY_FAILED = 23
Global Const QUICKUSB_ERROR_FIRMWARE_ERROR = 24
Global Const QUICKUSB_ERROR_ALREADY_COMPLETED = 25
Global Const QUICKUSB_ERROR_NOT_COMPLETED = 26
Global Const QUICKUSB_ERROR_FPGA_CONFIG_FAILED = 27
Global Const QUICKUSB_ERROR_INVALID_OPERATION = 28
Global Const QUICKUSB_ERROR_TOO_MANY_REQUESTS = 29
Global Const QUICKUSB_ERROR_EPCS_NOT_FOUND = 30
Global Const QUICKUSB_ERROR_EPCS_TOO_SMALL = 31
Global Const QUICKUSB_ERROR_NOT_STREAMING = 32


'=============================================================================
' QuickUSB Open Flags
'=============================================================================
Global Const QUICKUSB_OPEN_NORMAL = &H0
Global Const QUICKUSB_OPEN_IF_CLOSED = &H1


'=============================================================================
' QuickUSB Programming API Flags
'=============================================================================
Global Const QUICKUSB_PROGRAM_SERIAL = &H10000000
Global Const QUICKUSB_PRESERVE_CUSTOMIZATION = &H10000
Global Const QUICKUSB_PRESERVE_DEFAULTS = &H20000
Global Const QUICKUSB_PRESERVE_GPIF = &H40000
Global Const QUICKUSB_PRESERVE_SERIAL = &H80000


'=============================================================================
' QuickUSB EPCS IDs
'=============================================================================
Global Const QUICKUSB_EPCS_ID_UNKNOWN = &H0
Global Const QUICKUSB_EPCS1_ID = &H10
Global Const QUICKUSB_EPCS4_ID = &H12
Global Const QUICKUSB_EPCS16_ID = &H14
Global Const QUICKUSB_EPCS64_ID = &H16


'=============================================================================
' QuickUSB EPCS Capacities (in bytes)
'=============================================================================
Global Const QUICKUSB_EPCS1_SIZE = (1048576 / 8)
Global Const QUICKUSB_EPCS4_SIZE = (QUICKUSB_EPCS1_SIZE * 4)
Global Const QUICKUSB_EPCS16_SIZE = (QUICKUSB_EPCS1_SIZE * 16)
Global Const QUICKUSB_EPCS64_SIZE = (QUICKUSB_EPCS1_SIZE * 64)


'=============================================================================
' QuickUSB Type Definitions
'=============================================================================
Global Const QUICKUSB_EPCS_IGNORE_NCE = &H1
Global Const QUICKUSB_EPCS_SKIP_ERASE = &H2


'=============================================================================
' QuickUSB Chip ID Strings
'=============================================================================
Global Const QUICKUSB_CHIPID_STRING0 = "CY7C68013 Rev A/B (EZ-USB FX2)"
Global Const QUICKUSB_CHIPID_STRING1 = "CY7C68013A Rev A (EZ-USB FX2LP)"
Global Const QUICKUSB_CHIPID_STRING2 = "CY7C68013 Rev C/D (EZ-USB FX2)"
Global Const QUICKUSB_CHIPID_STRING4 = "CY7C68013 Rev E (EZ-USB FX2)"
Global Const QUICKUSB_SPEED_FULL = "Full Speed (12 Mbps)"
Global Const QUICKUSB_SPEED_HIGH = "High Speed (480 Mbps)"


'=============================================================================
' Win32 Library functions useful for VB apps
'=============================================================================
Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)
Declare Function GetTickCount Lib "kernel32" () As Long
Declare Function timeGetTime Lib "winmm" () As Long
Declare Function GetShortPathName Lib "kernel32" _
   Alias "GetShortPathNameA" (ByVal lpszLongPath As String, _
   ByVal lpszShortPath As String, ByVal cchBuffer As Long) As Long
   
'=============================================================================
' Base API
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
Declare Function QuickUsbConfigureFpga Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal theString As String) As Long


'=============================================================================
' Miscellaneous Constants
'=============================================================================
Global Const FIFOCONFIG_GPIF = &HFA
Global Const FIFOCONFIG_SLAVE = &H3


'=============================================================================
' QuickUSB Settings and Defaults Functions
'=============================================================================
Declare Function QuickUsbReadSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByRef theValue As Integer) As Long
Declare Function QuickUsbWriteSetting Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByVal theValue As Integer) As Long
Declare Function QuickUsbReadDefault Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal settingNum As Integer, _
   ByRef theValue As Integer) As Long
Declare Function QuickUsbWriteDefault Lib "QuickUsb" _
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
' QuickUSB General Purpose IO Functions
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
Declare Function QuickUsbCachedWriteI2C Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal address As Integer, _
   Data As Any, ByVal length As Integer) As Long


'=============================================================================
' QuickUSB Status Functions
'=============================================================================
Declare Function QuickUsbGetDllVersion Lib "QuickUsb" _
   (ByRef major As Integer, ByRef minor As Integer, ByRef revision As Integer) As Long
Declare Function QuickUsbGetDriverVersion Lib "QuickUsb" _
   (ByRef major As Integer, ByRef minor As Integer, ByRef revision As Integer) As Long
Declare Function QuickUsbGetFirmwareVersion Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef major As Integer, ByRef minor As Integer, ByRef revision As Integer) As Long
Declare Function QuickUsbSetTimeout Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal Timeout As Long) As Long


'=============================================================================
' Asynchronous I/O Functions
'=============================================================================
Declare Function QuickUsbReadDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByRef length As Long, _
   ByRef transaction As Byte) As Long
Declare Function QuickUsbWriteDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByVal length As Long, _
   ByRef transaction As Byte) As Long
Declare Function QuickUsbAsyncWait Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef length As Long, ByVal transaction As Byte, _
   ByVal immediate As Byte) As Long
Declare Function QuickUsbReadBulkDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByRef bytes As Long, _
   ByRef bulkstream As Any, ByVal cRoutine As Long, ByVal tag As Long) As Long
Declare Function QuickUsbWriteBulkDataAsync Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef Data As Any, ByRef bytes As Long, _
   ByRef bulkstream As Any, ByVal cRoutine As Long, ByVal tag As Long) As Long
Declare Function QuickUsbBulkWait Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef bulkstream As Any, _
   ByVal immediate As Byte) As Long


'=============================================================================
' Streaming Data Functions
'=============================================================================
Declare Function QuickUsbReadBulkDataStartStream Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef buffers As Any, ByVal numBuffers As Long, _
   ByVal bufferSize As Long, ByVal cRoutine As Long, ByVal tag As Long) As Long
Declare Function QuickUsbWriteBulkDataStartStream Lib "QuickUsb" _
   (ByVal hDevice As Long, ByRef buffers As Any, ByVal numBuffers As Long, _
   ByVal bufferSize As Long, ByVal cRoutine As Long, ByVal tag As Long) As Long
Declare Function QuickUsbBulkDataStopStream Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal wait As Boolean) As Long
   
   
'=============================================================================
' EPCS Programming API
'=============================================================================
Declare Function QuickUsbIdentifyEpcs Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal nSS As Byte, ByRef epcsId As Integer, _
   ByRef ecpsByteSize As Long, ByVal flags As Long) As Long
Declare Function QuickUsbConfigureEpcs Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal nSS As Byte, ByVal filePath As String, _
   ByVal flags As Long, ByVal callback As Long, ByVal tag As Long) As Long
Declare Function QuickUsbVerifyEpcs Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal nSS As Byte, ByVal filePath As String, _
   ByVal flags As Long, ByVal callback As Long, ByVal tag As Long) As Long
Declare Function QuickUsbEraseEpcs Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal nSS As Byte, ByVal flags As Long, _
   ByVal callback As Long, ByVal tag As Long) As Long


'=============================================================================
' Firmware Programming API
'=============================================================================
Declare Function QuickUsbWriteFirmware Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal fileName As String, ByVal Options As Long, _
   ByVal callback As Long, ByVal tag As Long) As Long
Declare Function QuickUsbVerifyFirmware Lib "QuickUsb" _
   (ByVal hDevice As Long, ByVal fileName As String, _
   ByVal callback As Long, ByVal tag As Long) As Long


'=============================================================================
' General Purpose Functions
'=============================================================================
Public Function Min(x As Variant, y As Variant) As Variant
   If (x < y) Then
      Min = x
   Else
      Min = y
   End If
End Function


Public Function Max(x As Variant, y As Variant) As Variant
   If (x > y) Then
      Max = x
   Else
      Max = y
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


' This function came from Microsoft Knowledge Base article Q175512
Public Function GetShortName(ByVal sLongFileName As String) As String
    Dim lRetVal As Long, sShortPathName As String, iLen As Integer
    'Set up buffer area for API function call return
    sShortPathName = Space(255)
    iLen = Len(sShortPathName)

    'Call the function
    lRetVal = GetShortPathName(sLongFileName, sShortPathName, iLen)
    'Strip away unwanted characters.
    GetShortName = Left(sShortPathName, lRetVal)
End Function

