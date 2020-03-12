Module QuickUsb
   '=============================================================================
   ' Constants
   '=============================================================================
   Public Const QUICKUSB_ERROR_NO_ERROR As Short = 0
   Public Const QUICKUSB_ERROR_OUT_OF_MEMORY As Short = 1
   Public Const QUICKUSB_ERROR_CANNOT_OPEN_MODULE As Short = 2
   Public Const QUICKUSB_ERROR_CANNOT_FIND_DEVICE As Short = 3
   Public Const QUICKUSB_ERROR_IOCTL_FAILED As Short = 4
   Public Const QUICKUSB_ERROR_INVALID_PARAMETER As Short = 5
   Public Const QUICKUSB_ERROR_TIMEOUT As Short = 6
   Public Const QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED As Short = 7

   '=============================================================================
   ' QuickUSB Setting Functions
   '=============================================================================
   Public Const SETTING_WORDWIDE As Short = 1
   Public Const SETTING_DATAADDRESS As Short = 2
   Public Const SETTING_FIFOCONFIG As Short = 3
   Public Const SETTING_FPGATYPE As Short = 4
   Public Const SETTING_CPUCONFIG As Short = 5
   Public Const SETTING_SPIENDIAN As Short = 6
   Public Const SETTING_SLAVEFIFOFLAGS As Short = 7

   '=============================================================================
   ' General purpose routines
   '=============================================================================
    Public Declare Ansi Function QuickUsbGetLastError Lib "QuickUsb.dll" (ByRef errorCode As Integer) As Integer
    Public Declare Ansi Function QuickUsbFindModules Lib "QuickUsb.dll" (ByVal nameList() As Byte, ByRef length As Integer) As Integer
   Public Declare Ansi Function QuickUsbOpen Lib "QuickUsb" (ByRef hDevice As Integer, ByVal devName As String) As Integer
   Public Declare Ansi Function QuickUsbClose Lib "QuickUsb" (ByVal hDevice As Integer) As Integer
   Public Declare Ansi Function QuickUsbGetStringDescriptor Lib "QuickUsb" (ByVal hDevice As Integer, ByVal Index As Byte, ByVal theString As String, ByVal length As Short) As Integer

   '=============================================================================
   ' FPGA Configuration Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbStartFpgaConfiguration Lib "QuickUsb" (ByVal hDevice As Integer) As Integer
   Public Declare Ansi Function QuickUsbWriteFpgaData Lib "QuickUsb" (ByVal hDevice As Integer, ByVal Data() As Byte, ByVal length As Integer) As Integer
   Public Declare Ansi Function QuickUsbIsFpgaConfigured Lib "QuickUsb" (ByVal hDevice As Integer, ByRef isConfigured As Short) As Integer

   Public Declare Ansi Function QuickUsbReadSetting Lib "QuickUsb" (ByVal hDevice As Integer, ByVal settingNum As Short, ByRef theValue As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteSetting Lib "QuickUsb" (ByVal hDevice As Integer, ByVal settingNum As Short, ByVal theValue As Short) As Integer
   Public Declare Ansi Function QuickUsbSetResetState Lib "QuickUsb" (ByVal hDevice As Integer, ByVal state As Byte) As Integer

   '=============================================================================
   ' QuickUSB High-Speed Parallel (HS) Port Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbReadCommand Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteCommand Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByVal length As Short) As Integer
   Public Declare Ansi Function QuickUsbReadCommandShort Lib "QuickUsb" Alias "QuickUsbReadCommand" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Short, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteCommandShort Lib "QuickUsb" Alias "QuickUsbWriteCommand" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Short, ByVal length As Short) As Integer
   Public Declare Ansi Function QuickUsbReadData Lib "QuickUsb" (ByVal hDevice As Integer, ByVal Data() As Byte, ByRef length As Integer) As Integer
   Public Declare Ansi Function QuickUsbWriteData Lib "QuickUsb" (ByVal hDevice As Integer, ByVal Data() As Byte, ByVal length As Integer) As Integer
   Public Declare Ansi Function QuickUsbReadDataShort Lib "QuickUsb" Alias "QuickUsbReadData" (ByVal hDevice As Integer, ByVal Data() As Short, ByRef length As Integer) As Integer
   Public Declare Ansi Function QuickUsbWriteDataShort Lib "QuickUsb" Alias "QuickUsbReadData" (ByVal hDevice As Integer, ByVal Data() As Short, ByVal length As Integer) As Integer
   Public Declare Ansi Function QuickUsbReadDataString Lib "QuickUsb" Alias "QuickUsbReadData" (ByVal hDevice As Integer, ByVal str As String, ByRef length As Integer) As Integer
   Public Declare Ansi Function QuickUsbWriteDataString Lib "QuickUsb" Alias "QuickUsbReadData" (ByVal hDevice As Integer, ByVal str As String, ByVal length As Integer) As Integer

   '=============================================================================
   ' QuickUSB General Purpose Port Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbReadPortDir Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByRef Data As Byte) As Integer
   Public Declare Ansi Function QuickUsbWritePortDir Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data As Byte) As Integer
   Public Declare Ansi Function QuickUsbReadPort Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWritePort Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByVal length As Short) As Integer

   '=============================================================================
   ' QuickUSB RS-232 Port Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbSetRS232BaudRate Lib "QuickUsb" (ByVal hDevice As Integer, ByVal baudRate As Integer) As Integer
   Public Declare Ansi Function QuickUsbGetNumRS232 Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbFlushRS232 Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte) As Integer
   Public Declare Ansi Function QuickUsbReadRS232 Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal Data() As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteRS232 Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal Data() As Byte, ByVal length As Short) As Integer
   Public Declare Ansi Function QuickUsbReadRS232String Lib "QuickUsb" Alias "QuickUsbReadRS232" (ByVal hDevice As Integer, ByVal portNum As Byte, ByRef str As String, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteRS232String Lib "QuickUsb" Alias "QuickUsbWriteRS232" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal str As String, ByVal length As Short) As Integer

   '=============================================================================
   ' QuickUSB SPI Port Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbReadSpi Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal Data() As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteSpi Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal Data() As Byte, ByVal length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteReadSpi Lib "QuickUsb" (ByVal hDevice As Integer, ByVal portNum As Byte, ByVal Data() As Byte, ByVal length As Short) As Integer

   '=============================================================================
   ' QuickUSB I2C Port Functions
   '=============================================================================
   Public Declare Ansi Function QuickUsbReadI2C Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByRef length As Short) As Integer
   Public Declare Ansi Function QuickUsbWriteI2C Lib "QuickUsb" (ByVal hDevice As Integer, ByVal address As Short, ByVal Data() As Byte, ByVal length As Short) As Integer


   Public Function Min(ByVal X As Object, ByVal Y As Object) As Object
      If (X < Y) Then
         Min = X
      Else
         Min = Y
      End If
   End Function


   Public Function Max(ByVal X As Object, ByVal Y As Object) As Object
      If (X > Y) Then
         Max = X
      Else
         Max = Y
      End If
   End Function

End Module
