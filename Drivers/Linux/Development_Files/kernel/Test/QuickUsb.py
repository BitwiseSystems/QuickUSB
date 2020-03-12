"""
============================================================================
 Title       : QuickUSB.py
 Description : QuickUSB Python API Interface
 Notes       : 
 History     :

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

============================================================================
"""
import ctypes, sys
from ctypes.util import find_library



############################
# QuickUSB Type Defintions #
############################
#QuickUSB Type Defintions
QBYTE = ctypes.c_ubyte
PQBYTE = ctypes.POINTER(QBYTE)
QCHAR = ctypes.c_char
PQCHAR = ctypes.POINTER(QCHAR)
QWORD = ctypes.c_ushort
PQWORD = ctypes.POINTER(QWORD)
QLONG = ctypes.c_long
PQLONG = ctypes.POINTER(QLONG)
QULONG = ctypes.c_ulong
PQULONG = ctypes.POINTER(QULONG)
QHANDLE = QULONG
PQHANDLE = ctypes.POINTER(QHANDLE)
QRESULT = QLONG



#########################
# QuickUSB Enumerations    #
#########################
class Port(int):
    """QuickUSB Port Addresses Enumeration"""
    A = 0
    B = 1
    C = 2
    D = 3
    E = 4

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class StringDescriptor(int):
    """QuickUSB String Descriptor Enumeration"""
    Make = 1
    Model = 2
    Serial = 3

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class Setting(int):
    """QuickUSB Settings Enumeration"""
    Ep26Config = 0
    WordWide = 1
    DataAddress = 2
    FifoConfig = 3
    FpgaType = 4
    CpuConfig = 5
    SpiConfig = 6
    SlaveFifoFlags = 7
    I2Ctl = 8
    PortA = 9
    PortB = 10
    PortC = 11
    PortD = 12
    PortE = 13
    PortACCfg = 14
    PinFlags = 15
    VersionBuild = 16
    VersionSpeed = 17

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class FpgaConfType(int):
    """FPGA Configuration Type Enumeration"""
    AlteraPS = 0
    XilinxSS = 1
    Jtag = 127
        
    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class Error(int):
    """QuickUSB Error Codes Enumeration"""
    NoError = 0
    OutOfMemory = 1
    CannotOpenModule = 2
    CannotFindDevice = 3
    IoctlFailed = 4
    InvalidParameter = 5
    Timeout = 6
    FunctionNotSupported = 7
    I2cBusError = 8
    I2cNoAck = 9
    I2cSlaveWait = 10
    I2cTimeout = 11
    UnknownDriverType = 12
    AlreadyOpened = 13
    CannotCloseModule = 14

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



#######################
# QuickUSB Exceptions #
#######################
class QuickUsbError(Exception):
    """QuickUSB exception class"""
    def __init__(self, error):
        self._error = error

    def __str__(self):
        return "%i - %s" % (self._error, Error(self._error))

    def GetErrorCode(self):
        return self._error
    


#########################
# QuickUSB Call Logging #
#########################
def QuickUsbLog(qusb, func, result, **kwargs):
    """
    This method is called after every call to a QuickUSB function through the
    QuickUSB class.  If a logging function has been set with
    QuickUSB.SetLoggingFunc() then then specified function will be called with
    a single log string as a parameter for every QuickUSB function call.
    """
    if QuickUsb.QuickUsbLogFunc:
        if result:
            QuickUsb.QuickUsbLogFunc('%s() exited successfully' % func.func_name)
        else:
            err = qusb.GetLastError()
            QuickUsb.QuickUsbLogFunc('%s() exited with error #%i (%s)' % (func.func_name, int(err), str(err)))



#QuickUSB ctypes error checking handler
def QuickUsbErrorCheck(result, func, args):
    """ctypes error checking function for the QuickUSB Library functions"""
    if not result:
        if func != QuickUsbLibrary.QuickUsbGetLastError:
            raise QuickUsbError(QuickUsb._GetLastError())
    return args



###########################################
# QuickUSB External Function Declarations #
###########################################
#Load the QuickUSB Library
class empty:
    pass

if sys.platform == "win32":     #Both x86 and x64
    try:
        QuickUsbLibrary = ctypes.WinDLL(find_library("QuickUSB")) # Automatically adds '.dll' suffix
    except:
        print "OSError, Unable to load QuickUSB Library"
        QuickUsbLibrary = empty()
elif 'linux' in sys.platform:   #Linux if uname -s contains 'linux'
    QuickUsbLibrary = ctypes.CDLL(find_library("quickusb")) #Automatically adds 'lib' prefix and '.so' suffix
    try:
        QuickUsbLibrary = ctypes.CDLL(find_library("quickusb")) #Automatically adds 'lib' prefix and '.so' suffix
    except:
        print "OSError, Unable to load QuickUSB Library"
        QuickUsbLibrary = empty()
elif sys.platform == 'darwin':  #Mac OS X
    try:
        QuickUsbLibrary = ctypes.CDLL(find_library("quickusb")) #Automatically adds 'lib' prefix and '.dylib' suffix
    except:
        print "OSError, Unable to load QuickUSB Library"
        QuickUsbLibrary = empty()
else:
    print "OSError, Unsupported platform:", sys.platform
    QuickUsbLibrary = empty()



#QRETURN QuickUsbGetLastError(QULONG *error);
try:
    QuickUsbLibrary.QuickUsbGetLastError.argtypes = [PQULONG]
    QuickUsbLibrary.QuickUsbGetLastError.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetLastError.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetLastError = None

#QRETURN QuickUsbFindModules(QCHAR *nameList, QULONG length);
try:
    QuickUsbLibrary.QuickUsbFindModules.argtypes = [PQCHAR, QULONG]
    QuickUsbLibrary.QuickUsbFindModules.restype = QRESULT
    QuickUsbLibrary.QuickUsbFindModules.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbFindModules = None

#QRETURN QuickUsbLibrary.QuickUsbOpen(QHANDLE *hDevice, QCHAR *deviceName);
try:
    QuickUsbLibrary.QuickUsbOpen.argtypes = [PQHANDLE, PQCHAR]
    QuickUsbLibrary.QuickUsbOpen.restype = QRESULT
    QuickUsbLibrary.QuickUsbOpen.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbOpen = None

#QRETURN QuickUsbLibrary.QuickUsbOpenEx(QHANDLE *hDevice, QCHAR *deviceName, QWORD flags);
try:
    QuickUsbLibrary.QuickUsbOpenEx.argtypes = [PQHANDLE, PQCHAR, QWORD]
    QuickUsbLibrary.QuickUsbOpenEx.restype = QRESULT
    QuickUsbLibrary.QuickUsbOpenEx.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbOpenEx = None

#QRETURN QuickUsbLibrary.QuickUsbClose(QHANDLE hDevice);
try:
    QuickUsbLibrary.QuickUsbClose.argtypes = [QHANDLE]
    QuickUsbLibrary.QuickUsbClose.restype = QRESULT
    QuickUsbLibrary.QuickUsbClose.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbClose = None

#QRETURN QuickUsbLibrary.QuickUsbGetStringDescriptor(QHANDLE hDevice, QBYTE index, QCHAR *buffer, QWORD length);
try:
    QuickUsbLibrary.QuickUsbGetStringDescriptor.argtypes = [QHANDLE, QBYTE, PQCHAR, QWORD]
    QuickUsbLibrary.QuickUsbGetStringDescriptor.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetStringDescriptor.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetStringDescriptor = None

#QRETURN QuickUsbLibrary.QuickUsbReadSetting(QHANDLE hDevice, QWORD address,  QWORD *value);
try:
    QuickUsbLibrary.QuickUsbReadSetting.argtypes = [QHANDLE, QWORD, PQWORD]
    QuickUsbLibrary.QuickUsbReadSetting.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadSetting.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadSetting = None

#QRETURN QuickUsbLibrary.QuickUsbWriteSetting(QHANDLE hDevice, QWORD address,  QWORD value);
try:
    QuickUsbLibrary.QuickUsbWriteSetting.argtypes = [QHANDLE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbWriteSetting.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteSetting.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteSetting = None

#QRETURN QuickUsbLibrary.QuickUsbStartFpgaConfiguration(QHANDLE hDevice);
try:
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration.argtypes = [QHANDLE]
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration.restype = QRESULT
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration = None

#QRETURN QuickUsbLibrary.QuickUsbWriteFpgaData(QHANDLE hDevice, QBYTE *fpgadata, QULONG datalength);
try:
    QuickUsbLibrary.QuickUsbWriteFpgaData.argtypes = [QHANDLE, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbWriteFpgaData.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteFpgaData.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteFpgaData = None

#QRETURN QuickUsbLibrary.QuickUsbIsFpgaConfigured(QHANDLE hDevice, QWORD *isConfigured);
try:
    QuickUsbLibrary.QuickUsbIsFpgaConfigured.argtypes = [QHANDLE, PQWORD]
    QuickUsbLibrary.QuickUsbIsFpgaConfigured.restype = QRESULT
    QuickUsbLibrary.QuickUsbIsFpgaConfigured.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbIsFpgaConfigured = None

#QRETURN QuickUsbLibrary.QuickUsbReadCommand(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadCommand.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadCommand.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadCommand.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadCommand = None

#QRETURN QuickUsbLibrary.QuickUsbWriteCommand(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteCommand.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteCommand.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteCommand.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteCommand = None

#QRETURN QuickUsbLibrary.QuickUsbReadData(QHANDLE hDevice, QBYTE *data, QULONG *length);
try:
    QuickUsbLibrary.QuickUsbReadData.argtypes = [QHANDLE, PQBYTE, PQULONG]
    QuickUsbLibrary.QuickUsbReadData.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadData.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadData = None

#QRETURN QuickUsbLibrary.QuickUsbWriteData(QHANDLE hDevice, QBYTE *data, QULONG length);
try:
    QuickUsbLibrary.QuickUsbWriteData.argtypes = [QHANDLE, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbWriteData.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteData.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteData = None

#QRETURN QuickUsbLibrary.QuickUsbReadPortDir(QHANDLE hDevice, QWORD address, QBYTE *data);
try:
    QuickUsbLibrary.QuickUsbReadPortDir.argtypes = [QHANDLE, QWORD, PQBYTE]
    QuickUsbLibrary.QuickUsbReadPortDir.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadPortDir.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadPortDir = None

#QRETURN QuickUsbLibrary.QuickUsbWritePortDir(QHANDLE hDevice, QWORD address, QBYTE data);
try:
    QuickUsbLibrary.QuickUsbWritePortDir.argtypes = [QHANDLE, QWORD, QBYTE]
    QuickUsbLibrary.QuickUsbWritePortDir.restype = QRESULT
    QuickUsbLibrary.QuickUsbWritePortDir.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWritePortDir = None

#QRETURN QuickUsbLibrary.QuickUsbReadPort(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadPort.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadPort.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadPort.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadPort = None

#QRETURN QuickUsbLibrary.QuickUsbWritePort(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWritePort.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWritePort.restype = QRESULT
    QuickUsbLibrary.QuickUsbWritePort.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWritePort = None

#QRETURN QuickUsbLibrary.QuickUsbSetRS232BaudRate(QHANDLE hDevice, QULONG baudRate);
try:
    QuickUsbLibrary.QuickUsbSetRS232BaudRate.argtypes = [QHANDLE, QULONG]
    QuickUsbLibrary.QuickUsbSetRS232BaudRate.restype = QRESULT
    QuickUsbLibrary.QuickUsbSetRS232BaudRate.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbSetRS232BaudRate = None

#QRETURN QuickUsbLibrary.QuickUsbGetNumRS232(QHANDLE hDevice, QBYTE portNum, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbGetNumRS232.argtypes = [QHANDLE, QBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbGetNumRS232.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetNumRS232.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetNumRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbFlushRS232(QHANDLE hDevice, QBYTE portNum);
try:
    QuickUsbLibrary.QuickUsbFlushRS232.argtypes = [QHANDLE, QBYTE]
    QuickUsbLibrary.QuickUsbFlushRS232.restype = QRESULT
    QuickUsbLibrary.QuickUsbFlushRS232.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbFlushRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbReadRS232(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadRS232.argtypes = [QHANDLE, QBYTE, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadRS232.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadRS232.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbWriteRS232(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteRS232.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteRS232.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteRS232.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbReadSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadSpi.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadSpi.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadSpi = None

#QRETURN QuickUsbLibrary.QuickUsbWriteSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteSpi.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteSpi.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteSpi = None

#QRETURN QuickUsbLibrary.QuickUsbWriteReadSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteReadSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteReadSpi.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteReadSpi.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteReadSpi = None

#QRETURN QuickUsbLibrary.QuickUsbReadI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadI2C.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadI2C.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadI2C.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadI2C = None

#QRETURN QuickUsbLibrary.QuickUsbWriteI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteI2C.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteI2C.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteI2C.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteI2C = None

#QRETURN QuickUsbLibrary.QuickUsbCachedWriteI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbCachedWriteI2C.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbCachedWriteI2C.restype = QRESULT
    QuickUsbLibrary.QuickUsbCachedWriteI2C.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbCachedWriteI2C = None

#QRETURN QuickUsbLibrary.QuickUsbSetTimeout(QHANDLE hDevice, QULONG timeOut);
try:
    QuickUsbLibrary.QuickUsbSetTimeout.argtypes = [QHANDLE, QULONG]
    QuickUsbLibrary.QuickUsbSetTimeout.restype = QRESULT
    QuickUsbLibrary.QuickUsbSetTimeout.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbSetTimeout = None

#QRETURN QuickUsbLibrary.QuickUsbGetDriverVersion(QWORD *MajorDriverVersion, QWORD *MinorDriverVersion, QWORD *BuildDriverVersion);
try:
    QuickUsbLibrary.QuickUsbGetDriverVersion.argtypes = [PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetDriverVersion.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetDriverVersion.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetDriverVersion = None

#QRETURN QuickUsbLibrary.QuickUsbGetDllVersion(QWORD *MajorDllVersion, QWORD *MinorDllVersion, QWORD *BuildDllVersion);
try:
    QuickUsbLibrary.QuickUsbGetDllVersion.argtypes = [PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetDllVersion.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetDllVersion.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetDllVersion = None

#QRETURN QuickUsbLibrary.QuickUsbGetFirmwareVersion(QHANDLE hDevice, QWORD *MajorFirmwareVersion, QWORD *MinorFirmwareVersion, QWORD *BuildFirmwareVersion);
try:
    QuickUsbLibrary.QuickUsbGetFirmwareVersion.argtypes = [QHANDLE, PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetFirmwareVersion.restype = QRESULT
    QuickUsbLibrary.QuickUsbGetFirmwareVersion.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbGetFirmwareVersion = None

#QRETURN QuickUsbLibrary.QuickUsbWriteDataAsync(QHANDLE hDevice, QBYTE *data, QULONG length, QBYTE *transaction);
try:
    QuickUsbLibrary.QuickUsbWriteDataAsync.argtypes = [QHANDLE, PQBYTE, QULONG, PQBYTE]
    QuickUsbLibrary.QuickUsbWriteDataAsync.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteDataAsync.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteDataAsync = None

#QRETURN QuickUsbLibrary.QuickUsbReadDataAsync(QHANDLE hDevice, QBYTE *data, QULONG *length, QBYTE *transaction);
try:
    QuickUsbLibrary.QuickUsbReadDataAsync.argtypes = [QHANDLE, PQBYTE, PQULONG, PQBYTE]
    QuickUsbLibrary.QuickUsbReadDataAsync.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadDataAsync.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadDataAsync = None

#QRETURN QuickUsbLibrary.QuickUsbAsyncWait(QHANDLE hDevice, QULONG *bytecount, QBYTE transaction, QBYTE immediate);
try:
    QuickUsbLibrary.QuickUsbAsyncWait.argtypes = [QHANDLE, PQULONG, QBYTE, QBYTE]
    QuickUsbLibrary.QuickUsbAsyncWait.restype = QRESULT
    QuickUsbLibrary.QuickUsbAsyncWait.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbAsyncWait = None

#QRETURN QuickUsbLibrary.QuickUsbReadDefault(QHANDLE hDevice, QWORD address, QWORD *data);
try:
    QuickUsbLibrary.QuickUsbReadDefault.argtypes = [QHANDLE, QWORD, PQWORD]
    QuickUsbLibrary.QuickUsbReadDefault.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadDefault.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadDefault = None

#QRETURN QuickUsbLibrary.QuickUsbWriteDefault(QHANDLE hDevice, QWORD address, QWORD data);
try:
    QuickUsbLibrary.QuickUsbWriteDefault.argtypes = [QHANDLE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbWriteDefault.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteDefault.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteDefault = None

#QRETURN QuickUsbLibrary.QuickUsbReadStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes);
try:
    QuickUsbLibrary.QuickUsbReadStorage.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbReadStorage.restype = QRESULT
    QuickUsbLibrary.QuickUsbReadStorage.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbReadStorage = None

#QRETURN QuickUsbLibrary.QuickUsbWriteStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes);
try:
    QuickUsbLibrary.QuickUsbWriteStorage.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteStorage.restype = QRESULT
    QuickUsbLibrary.QuickUsbWriteStorage.errcheck = QuickUsbErrorCheck
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteStorage = None



####################################
# QuickUSB Helper Functions #
####################################
def CreateByteBuffer(size, data=None):
    """
    Helper function to create a ctypes-style buffer of bytes needed as an
    argument to any of the QuickUSB Library functions which require a ctypes-
    style byte array.
    
    Keyword Arguments:
        size -- the size of the buffer in bytes
        data -- an array of data to initialize the buffer
    
    Returns:
        ctypes-style byte buffer with 'size' elements
    """
    if data:
        if isinstance(data, str):
            data = bytearray(data)
        return (QBYTE * size)(*data)
    return (QBYTE * size)()



def CreateWordBuffer(size, data=None):
    """
    Helper function to create a ctypes-style buffer of words needed as an
    argument to any of the QuickUSB Library functions which require a ctypes-
    style word array.
    
    Keyword Arguments:
        size -- the size of the buffer in words
        data -- an array of data to initialize the buffer
    
    Returns:
        ctypes-style word buffer with 'size' elements
    """
    if data:
        return (QWORD * size)(*data)
    return (QWORD * size)()



def CastToCharBuffer(buffer):
    return memoryview(buffer).tobytes() #Require Python2.7+.  Might make a copy.



####################################
# QuickUSB Class Object Definition #
####################################
class QuickUsb(object):
    """
    The QuickUSB Class Object
    """
    #------------------------------#
    # Static Class Variables       #
    #------------------------------#
    __throwExceptions = True
    QuickUsbLogFunc = None

    #------------------------------#
    # Static Class Methods         #
    #------------------------------#
    @classmethod
    def FindModules(cls):
        """
        Static class method to find all QuickUSB modules connected to the host.
        
        Keyword Arguments:
            None
            
        Returns:
            A list of strings, where each element in the list is the name of a
            QuickUSB Module of the form 'QUSB-XXX', where 'XXX' is a non-zero
            padded integer.
        """
        nameList = ctypes.create_string_buffer(512)

        #QRETURN <= [PQCHAR, QULONG]
        result = QuickUsbLibrary.QuickUsbFindModules(nameList, 512)
            
        if not result:
            lastError = QuickUsb._GetLastError()
            return (result, [])
        
        nameList = nameList[:].strip('\x00')
        if len(nameList) == 0:
            return (result, [])
        return (result, nameList.split('\x00'))



    @classmethod
    def _GetLastError(cls):
        """
        Static class method to retrieve the last QuickUSB API error code.
        
        Designed to only be used with static class methods such as
        FindModules(), GetDriverVersion(), and GetDllVersion().
        
        Keyword Arguments:
            None
            
        Returns:
            An integer holding the last error code set by the last failed API
            function call.  To retrieve a string representation of the error
            use the following code:
                print str(QuickUsb.Error(QuickUsb._GetLastError()))
        """
        lastError = QULONG()
        QuickUsbLibrary.QuickUsbGetLastError(ctypes.byref(lastError))
        return lastError.value



    @classmethod
    def GetDriverVersion(cls):
        """
        Static class method to determine the version of the QuickUSB driver.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok, major, minor, rev).
                ok    -- Indicates that the function call succeeded or failed
                major -- The major version number
                minor -- The minor version number
                rev   -- The revision number
        """
        major = QWORD()
        minor = QWORD()
        rev = QWORD()

        #QRETURN <= [PQWORD, PQWORD, PQWORD]
        result = QuickUsbLibrary.QuickUsbGetDriverVersion(ctypes.byref(major), ctypes.byref(minor), ctypes.byref(rev))
        
        return (result, major.value, minor.value, rev.value)



    @classmethod
    def GetDllVersion(cls):
        """
        Static class method to determine the version of the QuickUSB library.
        
        On Windows this is the version of the QuickUSB.dll file and on Linux
        this is the version of the libquickusb.so file.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok, major, minor, rev).
                ok    -- Indicates that the function call succeeded or failed
                major -- The major version number
                minor -- The minor version number
                rev   -- The revision number
        """
        major = QWORD()
        minor = QWORD()
        rev = QWORD()

        #QRETURN <= [PQWORD, PQWORD, PQWORD]
        result = QuickUsbLibrary.QuickUsbGetDllVersion(ctypes.byref(major), ctypes.byref(minor), ctypes.byref(rev))

        return (result, major.value, minor.value, rev.value)



    @classmethod
    def GetThrowExceptions(self):
        """
        Determine if QuickUSB exceptions are to be thrown on errors.
        
        Keyword Arguments:
            None
            
        Returns:
            A bool indicating is exceptions are to be thrown.
        """
        return self.__dict__['__throwExceptions']



    @classmethod
    def SetThrowExceptions(self, value):
        """
        
        
        Keyword Arguments:
            None
            
        Returns:
            None
        """
        setattr(QuickUsb, '__throwExceptions', bool(value))

        # Update ctypes error checking function
        for (k, v) in QuickUsbLibrary.__dict__.iteritems():
            if not k.startswith("QuickUsb") or not QuickUsbLibrary.__dict__[k]:
                continue
            if self.__dict__['__throwExceptions']:
                QuickUsbLibrary.__dict__[k].errcheck = QuickUsbErrorCheck
            else:
                QuickUsbLibrary.__dict__[k].errcheck = lambda result, func, args: args



    #------------------------------#
    # Properties                   #
    #------------------------------#
    @property
    def IsOpened(self):
        """
        Determine if the module has been opened.
        
        Returns:
            None
        """
        return ((self.globalOpen > 0) or (self.localOpen > 0))



    @property
    def Name(self):
        """
        Get the QuickUSB Module's name.
            
        Returns:
            None
        """
        return self.devName



    @Name.setter
    def Name(self, value):
        """
        Set the QuickUSB Module's name.
            
        Returns:
            None
        """
        self.devName = value



    @property
    def Make(self):
        """
        Get the QuickUSB Module's make string descriptor.
            
        Returns:
            None
        """
        return self.GetStringDescriptor(StringDescriptor.Make)



    @property
    def Model(self):
        """
        Get the QuickUSB Module's model string descriptor.
            
        Returns:
            None
        """
        return self.GetStringDescriptor(StringDescriptor.Model)



    @property
    def Serial(self):
        """
        Get the QuickUSB Module's serial string descriptor.
        
        Returns:
            None
        """
        return self.GetStringDescriptor(StringDescriptor.Serial)



    #------------------------------#
    # Class Constructor            #
    #------------------------------#
    def __init__(self, devName=None):
        self.localOpen = 0
        self.globalOpen = 0
        self.hDevice = 255
        self.devName = devName
        self.lastError = Error.NoError



    #------------------------------#
    # Protected Class Methods      #
    #------------------------------#
    def _OpenPolitely(self):
        """
        Internal function used to call Open() on a module when a function has
        been called and the module has not been explicitly opened by the user.
        
        Keyword Arguments:
            devName -- The QuickUSB device name, as returned by FindModules()
            
        Returns:
            True if the module was open or has been opened, False otherwise.
        """
        # Disregard if already globally opened
        if self.globalOpen != 0:
            return True

        # This method of reference counting is extremely important.  If nested calls
        # to this function are made we need to make sure we know when to close
        # the QuickUSB (of course, only if locally opened).
        self.localOpen += 1

        # Open the QuickUSB is it isn't already opened
        if self.localOpen == 1:
            try:
                ok = self.Open()
            except:
                self.localOpen -= 1
                raise
            
            if not ok:
                self.localOpen -= 1
                return False
            
        return True



    def _ClosePolitely(self):
        """
        Internal function used to call Close() on a module that was previously
        automatically opened from a call to __OpenPolitely().
        
        Keyword Arguments:
            None
            
        Returns:
            None
        """
        # Disregard if already globally opened
        if self.globalOpen != 0:
            return

        # Close the QuickUSB if we opened it locally
        self.localOpen -= 1
        if (self.localOpen == 0):
            (ok,) = self.Close()



    #------------------------------#
    # Public Class Methods         #
    #------------------------------#
    def GetLastError(self):
        """
        Retrieve the last QuickUSB API error code.
        
        The error codes are stored on a per-module-instance basis as a class
        member variable.
        
        Keyword Arguments:
            None
            
        Returns:
            An integer holding the last error code set by the last failed API
            function call.  To retrieve a string representation of the error
            use the following code:
                print str(QuickUsb.Error(QuickUsbInstance.GetLastError()))
        """

        return self.lastError



    def Open(self):
        """
        Open a QuickUSB device for use by the library.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        #QRETURN <= [PQHANDLE, PQCHAR]
        self.hDevice = QHANDLE(0)
        result = QuickUsbLibrary.QuickUsbOpen(ctypes.byref(self.hDevice), ctypes.create_string_buffer(self.devName))
        if not result:
            self.lastError = self._GetLastError()
        else:
            # If this was not a local open, it must be a global open
            if self.localOpen == 0:
                self.globalOpen += 1
        
        # Log the result
        if self.globalOpen != 0:
            QuickUsbLog(self, QuickUsb.Open, result)
        else:
            QuickUsbLog(self, QuickUsb._OpenPolitely, result)

        return (result,)



    def OpenEx(self, flags):
        """
        Open a QuickUSB device for use by the library.
        
        Keyword Arguments:
            flags -- Flags to control the open.  Valid values are
                0x0000 - Normal (Same operation as Open())
                0x0001 - Fail if module is already opened
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        #QRETURN <= [PQHANDLE, PQCHAR]
        self.hDevice = QHANDLE(0)
        result = QuickUsbLibrary.QuickUsbOpenEx(ctypes.byref(self.hDevice), ctypes.create_string_buffer(self.devName), QWORD(flags))
        if not result:
            self.lastError = self._GetLastError()
        else:
            # If this was not a local open, it must be a global open
            if self.localOpen == 0:
                self.globalOpen += 1
        
        # Log the result
        if self.globalOpen != 0:
            QuickUsbLog(self, QuickUsb.OpenEx, result)
        else:
            QuickUsbLog(self, QuickUsb._OpenPolitely, result)

        return (result,)



    def Close(self):
        """
        Close a QuickUSB device.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        #QRETURN <= [QHANDLE]
        result = QuickUsbLibrary.QuickUsbClose(self.hDevice)
        
        if not result:
            self.lastError = self._GetLastError()
        else:
            # If this was not a local close, it must be a global close
            if self.localOpen == 0:
                self.globalOpen += 1
            self.globalOpen = 0
        
        # Log the result
        if self.globalOpen != 0:
            QuickUsbLog(self, QuickUsb.Close, result)
        else:
            QuickUsbLog(self, QuickUsb._ClosePolitely, result)

        return (result,)



    def GetStringDescriptor(self, stringIndex):
        """
        Returns the string descriptor for the selected QuickUSB module.
        
        Keyword Arguments:
            stringIndex -- The string index to return.  Valid string indices are
                           defined in the QuickUsb.StringDescriptor class.
            
        Returns:
            A tuple of the form (ok, string).
                ok     -- indicates that the function call succeeded or failed
                string -- the string descriptor
        """
        if not self._OpenPolitely():
            return (0, "")
        
        index = QBYTE(stringIndex)
        string = ctypes.create_string_buffer(512)

        #QRETURN <= [QHANDLE, QBYTE, PQCHAR, QWORD]
        result = QuickUsbLibrary.QuickUsbGetStringDescriptor(self.hDevice, index, string, QWORD(512))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.GetStringDescriptor, result)

        string = string[:].split('\0')[0]
        string = string[:].split('\0')[0]
        
        self._ClosePolitely()
        return (result, string)



    def ReadSetting(self, address):
        """
        Read QuickUSB module settings.
        
        Keyword Arguments:
            address -- The address of the setting to retrieve.  Valid setting
                       addresses are defined in the QuickUsb.Setting class.
            
        Returns:
            A tuple of the form (ok, value).
                ok    -- indicates that the function call succeeded or failed
                value -- the word value of the setting
        """
        if not self._OpenPolitely():
            return (0, 0)

        value = QWORD()

        #QRETURN <= [QHANDLE, QWORD, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadSetting(self.hDevice, QWORD(address), ctypes.byref(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadSetting, result)

        self._ClosePolitely()
        return (result, value.value)



    def WriteSetting(self, address, value):
        """
        Write QuickUSB module settings.
        
        Keyword Arguments:
            address -- The address of the setting to retrieve.  Valid setting
                       addresses are defined in the QuickUsb.Setting class.
            value   -- The word value of the setting.
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= = [QHANDLE, QWORD, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteSetting(self.hDevice, QWORD(address), QWORD(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteSetting, result)

        self._ClosePolitely()
        return (result,)



    def StartFpgaConfiguration(self):
        """
        Start the process of FPGA configuration. If the FPGA is in the process
        of being configured, the process will restart. If the FPGA is already 
        configured, it will be reset and reconfigured.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE]
        result = QuickUsbLibrary.QuickUsbStartFpgaConfiguration(self.hDevice)
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.StartFpgaConfiguration, result)

        self._ClosePolitely()
        return (result,)



    def WriteFpgaData(self, c_fpgadata, length):
        """
        Sends FPGA configuration data to the FPGA using the QuickUSB FPGA 
        configuration port.
        
        Keyword Arguments:
            c_fpgadata -- A ctypes-style byte array containing the FPGA data to
                          write.  The buffer may be created with a call to 
                          QuickUsb.CreateByteArray().  If instead an int, tuple,
                          or list is passed in a ctypes-style byte array will
                          automatically be created to wrap the python type 
                          (which will incur a small performace penalty).
            length     -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_fpgadata
        if type(val) == int:
            c_fpgadata = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_fpgadata = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, PQBYTE, QULONG]
        result = QuickUsbLibrary.QuickUsbWriteFpgaData(self.hDevice, c_fpgadata, QULONG(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteFpgaData, result)

        self._ClosePolitely()
        return (result,)



    def IsFpgaConfigured(self):
        """
        Check to see if the FPGA is configured.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok, value).
                ok    -- Indicates that the function call succeeded or failed
                value -- Holds a value of 1 if the FPGA has been configured and
                         0 otherwise.
        """
        if not self._OpenPolitely():
            return (0, 0)

        configured = QWORD()

        #QRETURN <= [QHANDLE, PQWORD]
        result = QuickUsbLibrary.QuickUsbIsFpgaConfigured(self.hDevice, ctypes.byref(configured))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.IsFpgaConfigured, result)

        self._ClosePolitely()
        return (result, configured.value)



    def ReadCommand(self, address, c_data, length):
        """
        Read a block of command values from the high-speed parallel port using
        the QuickUSB module.
        
        Keyword Arguments:
            address -- The address used for the command read
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QWORD(length)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadCommand(self.hDevice, QWORD(address), c_data, ctypes.byref(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadCommand, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteCommand(self, address, c_data, length):
        """
        Write a block of command values to the high-speed parallel port using 
        the QuickUSB module.
        
        Keyword Arguments:
            address -- The address used for the command write
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteCommand(self.hDevice, QWORD(address), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteCommand, result)

        self._ClosePolitely()
        return (result,)



    def ReadData(self, c_array, bytes):
        """
        Read a block of data bytes from the high-speed parallel port using the
        QuickUSB module.
        
        Keyword Arguments:
            c_array -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            bytes   -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        c_bytes = QULONG(bytes)

        #QRETURN <= [QHANDLE, PQBYTE, PQULONG]
        result = QuickUsbLibrary.QuickUsbReadData(self.hDevice, c_array, ctypes.byref(c_bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadData, result)

        self._ClosePolitely()
        return (result, c_bytes.value)



    def ReadDataWords(self, c_array, words):
        """
        Read a block of data words from the high-speed parallel port using the
        QuickUSB module.
        
        Keyword Arguments:
            c_array -- A ctypes-style word array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            words   -- The number of words to read
            
        Returns:
            A tuple of the form (ok, words).
                ok    -- Indicates that the function call succeeded or failed
                words -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        c_bytes = QULONG(words * 2)

        #QRETURN <= [QHANDLE, PQBYTE, PQULONG]
        c_word_array = ctypes.cast(c_array, ctypes.POINTER(QBYTE))
        result = QuickUsbLibrary.QuickUsbReadData(self.hDevice, c_word_array, ctypes.byref(c_bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadDataWords, result)

        self._ClosePolitely()
        return (result, c_bytes.value / 2)



    def WriteData(self, c_data, bytes):
        """
        Write a block of data bytes to the high-speed parallel port using the 
        QuickUSB module.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            bytes   -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PQBYTE, QULONG]
        result = QuickUsbLibrary.QuickUsbWriteData(self.hDevice, c_data, QULONG(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteData, result)

        self._ClosePolitely()
        return (result,)



    def WriteDataWords(self, c_array, words):
        """
        Write a block of data words to the high-speed parallel port using the
        QuickUSB module.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            words   -- The number of words to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        c_bytes = QULONG(words * 2)

        #QRETURN <= [QHANDLE, PQBYTE, QULONG]
        c_byte_array = ctypes.cast(c_array, ctypes.POINTER(QBYTE))
        result = QuickUsbLibrary.QuickUsbWriteData(self.hDevice, c_byte_array, c_bytes)
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteDataWords, result)

        self._ClosePolitely()
        return (result,)



    def ReadPortDir(self, address):
        """
        Read the data direction of each data port bit for the specified port.
        
        Keyword Arguments:
            address -- The port address.  Valid port addresses are defined in
                       QuickUsb.Port.
            
        Returns:
            A tuple of the form (ok, value).
                ok    -- Indicates that the function call succeeded or failed
                value -- The value of the port direction.  Each bit corresponds
                         to the respective port bit with a 0 indicating input
                         and 1 indicating output.
        """
        if not self._OpenPolitely():
            return (0, 0)

        value = QBYTE()

        #QRETURN <= [QHANDLE, QWORD, PQBYTE]
        result = QuickUsbLibrary.QuickUsbReadPortDir(self.hDevice, QWORD(address), ctypes.byref(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadPortDir, result)

        self._ClosePolitely()
        return (result, value.value)



    def WritePortDir(self, address, value):
        """
        Set the data direction of each data port bit for the specified port.
        
        Keyword Arguments:
            address -- The port address.  Valid port addresses are defined in
                       QuickUsb.Port.
            value   -- The value of the port direction.  Each bit corresponds
                       to the respective port bit with a 0 indicating input and
                       1 indicating output.
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QWORD, QBYTE]
        result = QuickUsbLibrary.QuickUsbWritePortDir(self.hDevice, QWORD(address), QBYTE(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WritePortDir, result)

        self._ClosePolitely()
        return (result,)



    def ReadPort(self, address, c_data, length):
        """
        Read a series of bytes from the specified port.
        
        Keyword Arguments:
            address -- The port address.  Valid port addresses are defined in
                       QuickUsb.Port.
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QWORD(length)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadPort(self.hDevice, QWORD(address), c_data, ctypes.byref(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadPort, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WritePort(self, address, c_data, length):
        """
        Write a series of bytes to the specified port.
        
        Keyword Arguments:
            address -- The port address.  Valid port addresses are defined in
                       QuickUsb.Port.
            c_data --  A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)
        
        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWritePort(self.hDevice, QWORD(address),  c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WritePort, result)

        self._ClosePolitely()
        return (result,)



    def SetRS232BaudRate(self, baudRate):
        """
        Set the baud rate for both serial ports. Baud rates are programmable
        from 4.8k to 230k baud. This function sets the baud rate of both serial 
        ports. It is not possible to set the baud rate of each serial port 
        independently.
        
        Keyword Arguments:
            baudRate -- The baud rate
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QULONG]
        result = QuickUsbLibrary.QuickUsbSetRS232BaudRate(self.hDevice, QULONG(baudRate))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.SetRS232BaudRate, result)

        self._ClosePolitely()
        return (result,)



    def GetNumRS232(self, portNum):
        """
        Read the number of characters waiting in the receive buffer.
        
        Keyword Arguments:
            portNum -- The RS232 port number, either 0 or 1
            
        Returns:
            A tuple of the form (ok, num).
                ok  -- Indicates that the function call succeeded or failed
                num -- The number of bytes in the read buffer
        """
        if not self._OpenPolitely():
            return (0, 0)

        num = QWORD()

        #QRETURN <= [QHANDLE, QBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbGetNumRS232(self.hDevice, QBYTE(portNum), ctypes.byref(num))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.GetNumRS232, result)

        self._ClosePolitely()
        return (result, num.value)



    def FlushRS232(self, portNum):
        """
        Flush the RS232 port transmit and receive buffers.
        
        Keyword Arguments:
            portNum -- The RS232 port number, either 0 or 1
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QBYTE]
        result = QuickUsbLibrary.QuickUsbFlushRS232(self.hDevice, QBYTE(portNum))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.FlushRS232, result)

        self._ClosePolitely()
        return (result,)



    def ReadRS232(self, portNum, c_data, length):
        """
        Read a block of characters from the interrupt receive buffer of the 
        specified QuickUSB serial port.
        
        Keyword Arguments:
            portNum -- The RS232 port number, either 0 or 1
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QWORD(length)

        #QRETURN <= [QHANDLE, QBYTE, PQBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadRS232(self.hDevice, QBYTE(portNum), c_data, ctypes.byref(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadRS232, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteRS232(self, portNum, c_data, length):
        """
        Write a block of characters to the specified QuickUSB serial port.
        
        Keyword Arguments:
            portNum -- The RS232 port number, either 0 or 1
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QBYTE, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteRS232(self.hDevice, QBYTE(portNum), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteRS232, result)

        self._ClosePolitely()
        return (result,)



    def ReadSpi(self, portNum, c_data, length):
        """
        Read a block of bytes from the specified SPI slave port.
        
        Keyword Arguments:
            portNum -- The SPI port number between 0 and 9, inclusive
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QWORD(length)

        #QRETURN <= [QHANDLE, QBYTE, PQBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadSpi(self.hDevice, QBYTE(portNum), c_data, ctypes.byref(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadSpi, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteSpi(self, portNum, c_data, length):
        """
        Write a block of bytes to the specified SPI slave port.
        
        Keyword Arguments:
            portNum -- The SPI port number between 0 and 9, inclusive
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QBYTE, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteSpi(self.hDevice, QBYTE(portNum), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteSpi, result)

        self._ClosePolitely()
        return (result,)



    def WriteReadSpi(self, portNum, c_data, length):
        """
        Simultaneously write and read a block of data to and from the specified 
        SPI slave port.
        
        Keyword Arguments:
            portNum -- The SPI port number between 0 and 9, inclusive
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QBYTE, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteReadSpi(self.hDevice, QBYTE(portNum), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteReadSpi, result)

        self._ClosePolitely()
        return (result,)



    def ReadI2C(self, address, c_data, length):
        """
        Read a block of bytes from the specified I2C address.
        
        Keyword Arguments:
            address -- The I2C address
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes actually read
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QWORD(length)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadI2C(self.hDevice, QWORD(address), c_data, ctypes.byref(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadI2C, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteI2C(self, address, c_data, length):
        """
        Write a block of bytes to the specified I2C address.
        
        Keyword Arguments:
            address -- The I2C address
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteI2C(self.hDevice, QWORD(address), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteI2C, result)

        self._ClosePolitely()
        return (result,)



    def CachedWriteI2C(self, address, c_data, length):
        """
        Write a block of bytes to the specified I2C address.  The actual write 
        will not be performed until    a subsequent ReadI2C() has been issued.
        The write will then first occur, following by a repeated start
        condition, followed by the read.
        
        Keyword Arguments:
            address -- The I2C address
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbCachedWriteI2C(self.hDevice, QWORD(address), c_data, QWORD(length))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.CachedWriteI2C, result)

        self._ClosePolitely()
        return (result,)



    def SetTimeout(self, timeOut):
        """
        Set the timeout for QuickUSB read data transfers.
        
        Keyword Arguments:
            timeOut -- The time out in milliseconds
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QULONG]
        result = QuickUsbLibrary.QuickUsbSetTimeout(self.hDevice, QULONG(timeOut))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.SetTimeout, result)

        self._ClosePolitely()
        return (result,)



    def GetFirmwareVersion(self):
        """
        Determine the version of the QuickUSB Firmware is currently in the 
        QuickUSB Module.
        
        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok, major, minor, rev).
                ok    -- Indicates that the function call succeeded or failed
                major -- The major version number
                minor -- The minor version number
                rev   -- The revision number
        """
        if not self._OpenPolitely():
            return (0, 0, 0, 0)

        #QRETURN <= [QHANDLE, PQWORD, PQWORD, PQWORD]
        major = QWORD()
        minor = QWORD()
        rev = QWORD()

        result = QuickUsbLibrary.QuickUsbGetFirmwareVersion(self.hDevice, ctypes.byref(major), ctypes.byref(minor), ctypes.byref(rev))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.GetFirmwareVersion, result)

        self._ClosePolitely()
        return (result, major.value, minor.value, rev.value)



    def WriteDataAsync(self, c_data, length):
        """
        Write a block of data values to the high-speed parallel port using an 
        asynchronous function call.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok, transId).
                ok      -- Indicates that the function call succeeded or failed
                transId -- The transaction ID used with AsyncWait()
        """
        if not self._OpenPolitely():
            return (0, 0)

        transId = QBYTE()

        #QRETURN <= [QHANDLE, PQBYTE, QULONG, PQBYTE]
        result = QuickUsbLibrary.QuickUsbWriteDataAsync(self.hDevice, c_data, QULONG(length), ctypes.byref(transId))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteDataAsync, result)

        self._ClosePolitely()
        return (result, transId.value)



    def ReadDataAsync(self, c_data, length):
        """
        Read a block of data values from the high-speed parallel port using an 
        asynchronous function call.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            length  -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok, transId).
                ok -- Indicates that the function call succeeded or failed
                transId -- The transaction ID used with AsyncWait()
        """
        if not self._OpenPolitely():
            return (0, 0)

        transId = QBYTE()
        bytes = QULONG(length)

        #QRETURN <= [QHANDLE, PQBYTE, PQULONG, PQBYTE]
        result = QuickUsbLibrary.QuickUsbReadDataAsync(self.hDevice, c_data, ctypes.byref(bytes), ctypes.byref(transId))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadDataAsync, result)

        self._ClosePolitely()
        return (result, transId.value)



    def AsyncWait(self, transaction, immediate):
        """
        Wait for an asynchronous transfer to complete.
        
        Keyword Arguments:
            transaction -- The transaction ID used with AsyncWait()
            immediate   -- 0 to wait until transaction either completes or times
                           out and non-zero to return immediately
            
        Returns:
            A tuple of the form (ok, bytes).
                ok    -- Indicates that the function call succeeded or failed
                bytes -- The number of bytes transferred.  If the transaction is
                         still pending this will be zero.  This function must
                         be called until the transaction completes and this 
                         value is reported as non-zero.
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QULONG()

        #QRETURN <= [QHANDLE, PQULONG, QBYTE, QBYTE]
        result = QuickUsbLibrary.QuickUsbAsyncWait(self.hDevice, ctypes.byref(bytes), QBYTE(transaction), QBYTE(immediate))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.AsyncWait, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def ReadDefault(self, setting):
        """
        Read QuickUSB module defaults.
        
        The defaults are non-volatile and are read into the settings table on 
        power up.
        
        Keyword Arguments:
            setting -- The address of the default setting to retrieve.  Valid
                       setting addresses are defined in the QuickUsb.Setting
                       class. 
            
        Returns:
            A tuple of the form (ok, value).
                ok    -- Indicates that the function call succeeded or failed
                value -- The default setting value
        """
        if not self._OpenPolitely():
            return (0, 0)

        value = QWORD()

        #QRETURN <= [QHANDLE, QWORD, PQWORD]
        result = QuickUsbLibrary.QuickUsbReadDefault(self.hDevice, QWORD(setting), ctypes.byref(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadDefault, result)

        self._ClosePolitely()
        return (result, value.value)



    def WriteDefault(self, setting, value):
        """
        Write QuickUSB module defaults.
        
        The defaults are non-volatile and are read into the settings table on 
        power up.
        
        Keyword Arguments:
            setting -- The address of the default setting to retrieve.  Valid
                       setting addresses are defined in the QuickUsb.Setting
                       class.
            value   -- The value of the default to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QWORD, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteDefault(self.hDevice, QWORD(setting), QWORD(value))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteDefault, result)

        self._ClosePolitely()
        return (result,)



    def ReadStorage(self, address, c_data, bytes):
        """
        Read a block of bytes from EEPROM memory (2KB available).
        
        Keyword Arguments:
            address -- The EEPROM address
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteArray().
            bytes   -- The number of bytes to read
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbReadStorage(self.hDevice, QWORD(address), c_data, QWORD(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.ReadStorage, result)

        self._ClosePolitely()
        return (result,)



    def WriteStorage(self, address, c_data, bytes):
        """
        Write a block of bytes to memory.
        
        Keyword Arguments:
            address -- The EEPROM address
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteArray().  If instead an int, tuple, or
                       list is passed in a ctypes-style byte array will
                       automatically be created to wrap the python type (which
                       will incur a small performace penalty).
            bytes   -- The number of bytes to write
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        # Allow for c_data to simply be an int, tuple, or list
        val = c_data
        if type(val) == int:
            c_data = CreateByteBuffer(length, (val,))
        elif type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, QWORD, PQBYTE, QWORD]
        result = QuickUsbLibrary.QuickUsbWriteStorage(self.hDevice, QWORD(address), c_data, QWORD(bytes))
        if not result:
            self.lastError = self._GetLastError()

        # Log the result
        QuickUsbLog(self, QuickUsb.WriteStorage, result)

        self._ClosePolitely()
        return (result,)



#Test Program
if __name__ == '__main__':
    print "Finding Modules"
    (ok, nameList) = QuickUsb.FindModules()
    if not ok:
        errCode = QuickUsb._GetLastError()
        print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))

    #try:
    #    (result, nameList) = QuickUsb.FindModules()
    #except (QuickUsbError):
    #    nameList = []

    (ok, major, minor, rev) = QuickUsb.GetDllVersion()
    print "DLL Version:", (major, minor, rev)
    print
    print "Found Modules:", nameList

    if len(nameList) > 0:
        qusb = QuickUsb(nameList[0])

        print "Opening:", qusb.Name
        (ok,) = qusb.Open()
        if not ok:
            errCode = QuickUsb.GetLastError()
            print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))

        (ok, major, minor, rev) = QuickUsb.GetDriverVersion()
        print "Driver Version:", (major, minor, rev)
        
        (ok, major, minor, rev) = qusb.GetFirmwareVersion()
        print "Firmware Version:", (major, minor, rev)

        #Print string descriptors
        for k in range(1,4):
            sd = StringDescriptor(k)
            (ok, string) = qusb.GetStringDescriptor(sd)
            print "\t%s: %s" % (str(sd), string)

        #Read PA
        buffer = CreateByteBuffer(10)
        (ok, bytes) = qusb.ReadPort(Port.A, buffer, len(buffer))
        print "Reading Port A:", buffer[:]

        #Close the module
        print "Closing:", qusb.Name
        (ok,) = qusb.Close()
        if not ok:
            errCode = QuickUsb.GetLastError()
            print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))
