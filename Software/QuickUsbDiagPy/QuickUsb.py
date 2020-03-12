"""
============================================================================
 Title       : QuickUSB.py
 Description : QuickUSB Python API Interface
 Notes       :
 History     :

 Copyright (c) 2012 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real,
 Suite E, Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

============================================================================
"""
import ctypes, sys, platform
from ctypes.util import find_library



############################
# QuickUSB Type Defintions #
############################
#QuickUSB Type Defintions
QBYTE = ctypes.c_ubyte
PQBYTE = ctypes.POINTER(QBYTE)
QBOOL = ctypes.c_ubyte
PQBOOL = ctypes.POINTER(QBOOL)
QCHAR = ctypes.c_char
PQCHAR = ctypes.POINTER(QCHAR)
QBOOL = ctypes.c_ubyte
PQBOOL = ctypes.POINTER(QBOOL)
QWORD = ctypes.c_ushort
PQWORD = ctypes.POINTER(QWORD)
QLONG = ctypes.c_long
PQLONG = ctypes.POINTER(QLONG)
QULONG = ctypes.c_ulong
PQULONG = ctypes.POINTER(QULONG)
QFLOAT = ctypes.c_float
PQFLOAT = ctypes.POINTER(QFLOAT)
QHANDLE = QULONG
PQHANDLE = ctypes.POINTER(QHANDLE)
QRESULT = QLONG
PQVOID = ctypes.c_voidp



#########################
# QuickUSB Enumerations #
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
    FpgaInitFailed = 15
    PacketNotMultiple512 = 16
    PacketNotMultiple64 = 17
    UnknownSystemError = 18
    Aborted = 19
    Deprecated = 20
    InvalidSerial =  21
    CannotOpenFile = 22
    VerifyFailed = 23
    FirmwareError = 24
    AlreadyCompleted = 25
    NotCompleted = 26
    FpgaConfigFailed = 27
    InvalidOperation = 28
    TooManyRequests = 29
    EpcsNotFound = 30
    EpcsTooSmall = 31
    NotStreaming = 32
    BufferNotAligned = 33
    InternalError = 34
    DeviceIsClosing = 35
    Protection = 36
    NeedData = 37
    FileNotFound = 38
    FileAlreadyExists = 39
    FileRW = 40
    FileEOF = 41
    FileName = 42
    AccessDenied = 43
    SharingViolation = 44

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class EpcsId(int):
    """QuickUSB EPCS ID Enumeration"""
    EpcsUnknown = 0x00
    Epcs1 = 0x10
    Epcs4 = 0x12
    Epcs16 = 0x14
    Epcs64 = 0x16

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class EpcsSize(int):
    """QuickUSB EPCS Size Enumeration"""
    Epcs1 = (1048576 / 8)
    Epcs4 = (Epcs1 * 4)
    Epcs16 = (Epcs1 * 16)
    Epcs64 = (Epcs1 * 64)

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class EpcsFlags(int):
    """QuickUSB EPCS Flags Enumeration"""
    IgnoreNCE = 0x0001
    SkipErase = 0x0002

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class OpenFlags(int):
    """QuickUSB Open Flags Enumeration"""
    Normal = 0x0000
    OpenIfClosed = 0x0001

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class Programming(int):
    """QuickUSB Programming Firmware Enumeration"""
    ProgramSerial = 0x10000000
    PreserveCustomization = 0x00010000
    PreserveDefaults = 0x00020000
    PreserveGpif = 0x00040000
    PreserveSerial = 0x00080000

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class Stat(int):
    """QuickUSB Statistics Enumeration"""
    All = 0
    ReadThroughput = 1
    WriteThroughput = 2
    TotalThroughput = 3
    TotalDataRead = 4
    TotalDataWritten = 5

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class StatUnit(int):
    """QuickUSB Statistics Unit Enumeration"""
    Bytes = 0x0001
    KB = 0x0002
    MB = 0x0004
    GB = 0x0008
    
    NS = 0x0010
    US = 0x0020
    MS = 0x0040
    Sec = 0x0080
    Min = 0x0100
    Hour = 0x0200
    
    BytesPerSec = (Bytes | Sec)
    KBPerSec = (KB | Sec)
    MBPerSec = (MB | Sec)

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class StreamFlag(int):
    """QuickUSB Stream Flag Enumeration"""
    MemoryFile = 1
    CreateAlways = 2

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



class StreamStatus(int):
    """QuickUSB Stream Status Enumeration"""
    Unknown = 0
    Running = 1
    Stopped = 2
    Paused = 3

    def __str__(self):
        keys = [k for k,v in self.__class__.__dict__.iteritems() if v == int(self)]
        if len(keys) > 0:
            return "%s.%s" % (self.__class__.__name__, keys[0])
        else:
            return "%s.%s" % (self.__class__.__name__, str(int(self)))



#######################
# QuickUSB Structures #
#######################
(bits, linkage) = platform.architecture()
if sys.platform == 'win32':
    if bits == '64bit':
        bytes = 32
    else:
        bytes = 20
    InternalData = ("InternalData", ctypes.c_byte * bytes)
elif sys.platform == 'darwin':
    InternalData = ("InternalData", PQVOID)
else:
    InternalData = ("InternalData", ctypes.c_byte * 64)
        
class QBULKSTREAM(ctypes.Structure):
    _fields_ = [InternalData,
                ("Handle", QHANDLE),
                ("Buffer", PQBYTE),
                ("CompletionRoutine", PQVOID),
                ("Tag", PQVOID),
                ("InternalReserved", PQVOID),
                ("BytesRequested", QULONG),
                ("BytesTransferred", QULONG),
                ("Error", QULONG),
                ("InternalStatus", QULONG),
                ("StreamID", QULONG),
                ("RequestID", QULONG),
                ("Reserved", QULONG),
                ("StreamType", QWORD),
                ("StructSize", QBYTE),
                ("CompletionType", QBYTE)]
PQBULKSTREAM = ctypes.POINTER(QBULKSTREAM)



################################
# QuickUSB Callback Defintions #
################################
if sys.platform == "win32":
    PQBULKSTREAM_COMPLETION_ROUTINE = ctypes.WINFUNCTYPE(None, PQBULKSTREAM)
    PQPROGRESS_CALLBACK = ctypes.WINFUNCTYPE(None, QHANDLE, QWORD, PQVOID)
else:
    PQBULKSTREAM_COMPLETION_ROUTINE = ctypes.CFUNCTYPE(None, PQBULKSTREAM)
    PQPROGRESS_CALLBACK = ctypes.CFUNCTYPE(None, QHANDLE, QWORD, PQVOID)



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
    


#QuickUSB ctypes error checking handler
def QuickUsbErrorCheck(result, func, args):
    """ctypes error checking function for the QuickUSB Library functions"""
    if not result:
        if func != QuickUsbLibrary.QuickUsbGetLastError:
            raise QuickUsbError(QuickUsb.GetLastError())
    return args



###########################################
# QuickUSB External Function Declarations #
###########################################
#Load the QuickUSB Library
if sys.platform == "win32":     #Both x86 and x64
    try:
        QuickUsbLibrary = ctypes.WinDLL("QuickUSB.dll")
    except:
        try:
            QuickUsbLibrary = ctypes.WinDLL(find_library("QuickUSB")) # Automatically adds '.dll' suffix
        except:
            raise OSError, "Unable to load QuickUSB Library"
elif 'linux' in sys.platform:   #Linux if uname -s contains 'linux'
    try:
        QuickUsbLibrary = ctypes.CDLL("libquickusb.so")
    except:
        try:
            QuickUsbLibrary = ctypes.CDLL(find_library("quickusb")) #Automatically adds 'lib' prefix and '.so' suffix
        except:
            raise OSError, "Unable to load QuickUSB Library"
elif sys.platform == 'darwin':  #Mac OS X
    try:
        QuickUsbLibrary = ctypes.CDLL("libquickusb.dylib")
    except:
        try:
            QuickUsbLibrary = ctypes.CDLL(find_library("quickusb")) #Automatically adds 'lib' prefix and '.dylib' suffix
        except:
            raise OSError, "Unable to load QuickUSB Library"
else:
    raise OSError, "Unsupported platform:", sys.platform



#QRETURN QuickUsbGetLastError(QULONG *error);
try:
    QuickUsbLibrary.QuickUsbGetLastError.argtypes = [PQULONG]
    QuickUsbLibrary.QuickUsbGetLastError.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetLastError = None

#QRETURN QuickUsbFindModules(QCHAR *nameList, QULONG length);
try:
    QuickUsbLibrary.QuickUsbFindModules.argtypes = [PQCHAR, QULONG]
    QuickUsbLibrary.QuickUsbFindModules.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbFindModules = None

#QRETURN QuickUsbLibrary.QuickUsbOpen(QHANDLE *hDevice, QCHAR *deviceName);
try:
    QuickUsbLibrary.QuickUsbOpen.argtypes = [PQHANDLE, PQCHAR]
    QuickUsbLibrary.QuickUsbOpen.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbOpen = None

#QRETURN QuickUsbLibrary.QuickUsbOpenEx(QHANDLE *hDevice, QCHAR *deviceName, QWORD flags);
try:
    QuickUsbLibrary.QuickUsbOpenEx.argtypes = [PQHANDLE, PQCHAR, QWORD]
    QuickUsbLibrary.QuickUsbOpenEx.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbOpenEx = None

#QRETURN QuickUsbLibrary.QuickUsbClose(QHANDLE hDevice);
try:
    QuickUsbLibrary.QuickUsbClose.argtypes = [QHANDLE]
    QuickUsbLibrary.QuickUsbClose.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbClose = None

#QRETURN QuickUsbLibrary.QuickUsbGetStringDescriptor(QHANDLE hDevice, QBYTE index, QCHAR *buffer, QWORD length);
try:
    QuickUsbLibrary.QuickUsbGetStringDescriptor.argtypes = [QHANDLE, QBYTE, PQCHAR, QWORD]
    QuickUsbLibrary.QuickUsbGetStringDescriptor.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetStringDescriptor = None

#QRETURN QuickUsbLibrary.QuickUsbReadSetting(QHANDLE hDevice, QWORD address,  QWORD *value);
try:
    QuickUsbLibrary.QuickUsbReadSetting.argtypes = [QHANDLE, QWORD, PQWORD]
    QuickUsbLibrary.QuickUsbReadSetting.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadSetting = None

#QRETURN QuickUsbLibrary.QuickUsbWriteSetting(QHANDLE hDevice, QWORD address,  QWORD value);
try:
    QuickUsbLibrary.QuickUsbWriteSetting.argtypes = [QHANDLE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbWriteSetting.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteSetting = None

#QRETURN QuickUsbLibrary.QuickUsbStartFpgaConfiguration(QHANDLE hDevice);
try:
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration.argtypes = [QHANDLE]
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbStartFpgaConfiguration = None

#QRETURN QuickUsbLibrary.QuickUsbWriteFpgaData(QHANDLE hDevice, QBYTE *fpgadata, QULONG datalength);
try:
    QuickUsbLibrary.QuickUsbWriteFpgaData.argtypes = [QHANDLE, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbWriteFpgaData.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteFpgaData = None

#QRETURN QuickUsbLibrary.QuickUsbIsFpgaConfigured(QHANDLE hDevice, QWORD *isConfigured);
try:
    QuickUsbLibrary.QuickUsbIsFpgaConfigured.argtypes = [QHANDLE, PQWORD]
    QuickUsbLibrary.QuickUsbIsFpgaConfigured.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbIsFpgaConfigured = None

#QUSBLIB QRETURN QuickUsbConfigureFpga(QHANDLE hDevice, PCQCHAR filePath);
try:
    QuickUsbLibrary.QuickUsbConfigureFpga.argtypes = [QHANDLE, PQCHAR]
    QuickUsbLibrary.QuickUsbConfigureFpga.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbConfigureFpga = None

#QRETURN QuickUsbLibrary.QuickUsbReadCommand(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadCommand.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadCommand.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadCommand = None

#QRETURN QuickUsbLibrary.QuickUsbWriteCommand(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteCommand.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteCommand.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteCommand = None

#QRETURN QuickUsbLibrary.QuickUsbReadData(QHANDLE hDevice, QBYTE *data, QULONG *length);
try:
    QuickUsbLibrary.QuickUsbReadData.argtypes = [QHANDLE, PQBYTE, PQULONG]
    QuickUsbLibrary.QuickUsbReadData.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadData = None

#QRETURN QuickUsbLibrary.QuickUsbWriteData(QHANDLE hDevice, QBYTE *data, QULONG length);
try:
    QuickUsbLibrary.QuickUsbWriteData.argtypes = [QHANDLE, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbWriteData.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteData = None

#QRETURN QuickUsbLibrary.QuickUsbReadPortDir(QHANDLE hDevice, QWORD address, QBYTE *data);
try:
    QuickUsbLibrary.QuickUsbReadPortDir.argtypes = [QHANDLE, QWORD, PQBYTE]
    QuickUsbLibrary.QuickUsbReadPortDir.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadPortDir = None

#QRETURN QuickUsbLibrary.QuickUsbWritePortDir(QHANDLE hDevice, QWORD address, QBYTE data);
try:
    QuickUsbLibrary.QuickUsbWritePortDir.argtypes = [QHANDLE, QWORD, QBYTE]
    QuickUsbLibrary.QuickUsbWritePortDir.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWritePortDir = None

#QRETURN QuickUsbLibrary.QuickUsbReadPort(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadPort.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadPort.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadPort = None

#QRETURN QuickUsbLibrary.QuickUsbWritePort(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWritePort.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWritePort.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWritePort = None

#QRETURN QuickUsbLibrary.QuickUsbSetRS232BaudRate(QHANDLE hDevice, QULONG baudRate);
try:
    QuickUsbLibrary.QuickUsbSetRS232BaudRate.argtypes = [QHANDLE, QULONG]
    QuickUsbLibrary.QuickUsbSetRS232BaudRate.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbSetRS232BaudRate = None

#QRETURN QuickUsbLibrary.QuickUsbGetNumRS232(QHANDLE hDevice, QBYTE portNum, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbGetNumRS232.argtypes = [QHANDLE, QBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbGetNumRS232.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetNumRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbFlushRS232(QHANDLE hDevice, QBYTE portNum);
try:
    QuickUsbLibrary.QuickUsbFlushRS232.argtypes = [QHANDLE, QBYTE]
    QuickUsbLibrary.QuickUsbFlushRS232.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbFlushRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbReadRS232(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadRS232.argtypes = [QHANDLE, QBYTE, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadRS232.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbWriteRS232(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteRS232.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteRS232.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteRS232 = None

#QRETURN QuickUsbLibrary.QuickUsbReadSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadSpi.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadSpi = None

#QRETURN QuickUsbLibrary.QuickUsbWriteSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteSpi.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteSpi = None

#QRETURN QuickUsbLibrary.QuickUsbWriteReadSpi(QHANDLE hDevice, QBYTE portNum, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteReadSpi.argtypes = [QHANDLE, QBYTE, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteReadSpi.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteReadSpi = None

#QRETURN QuickUsbLibrary.QuickUsbReadI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD *length);
try:
    QuickUsbLibrary.QuickUsbReadI2C.argtypes = [QHANDLE, QWORD, PQBYTE, PQWORD]
    QuickUsbLibrary.QuickUsbReadI2C.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadI2C = None

#QRETURN QuickUsbLibrary.QuickUsbWriteI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbWriteI2C.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteI2C.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteI2C = None

#QRETURN QuickUsbLibrary.QuickUsbCachedWriteI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length);
try:
    QuickUsbLibrary.QuickUsbCachedWriteI2C.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbCachedWriteI2C.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbCachedWriteI2C = None

#QRETURN QuickUsbLibrary.QuickUsbSetTimeout(QHANDLE hDevice, QULONG timeOut);
try:
    QuickUsbLibrary.QuickUsbSetTimeout.argtypes = [QHANDLE, QULONG]
    QuickUsbLibrary.QuickUsbSetTimeout.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbSetTimeout = None

#QRETURN QuickUsbLibrary.QuickUsbGetDriverVersion(QWORD *MajorDriverVersion, QWORD *MinorDriverVersion, QWORD *BuildDriverVersion);
try:
    QuickUsbLibrary.QuickUsbGetDriverVersion.argtypes = [PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetDriverVersion.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetDriverVersion = None

#QRETURN QuickUsbLibrary.QuickUsbGetDllVersion(QWORD *MajorDllVersion, QWORD *MinorDllVersion, QWORD *BuildDllVersion);
try:
    QuickUsbLibrary.QuickUsbGetDllVersion.argtypes = [PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetDllVersion.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetDllVersion = None

#QRETURN QuickUsbLibrary.QuickUsbGetFirmwareVersion(QHANDLE hDevice, QWORD *MajorFirmwareVersion, QWORD *MinorFirmwareVersion, QWORD *BuildFirmwareVersion);
try:
    QuickUsbLibrary.QuickUsbGetFirmwareVersion.argtypes = [QHANDLE, PQWORD, PQWORD, PQWORD]
    QuickUsbLibrary.QuickUsbGetFirmwareVersion.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetFirmwareVersion = None

#QRETURN QuickUsbLibrary.QuickUsbWriteDataAsync(QHANDLE hDevice, QBYTE *data, QULONG length, QBYTE *transaction);
try:
    QuickUsbLibrary.QuickUsbWriteDataAsync.argtypes = [QHANDLE, PQBYTE, QULONG, PQBYTE]
    QuickUsbLibrary.QuickUsbWriteDataAsync.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteDataAsync = None

#QRETURN QuickUsbLibrary.QuickUsbReadDataAsync(QHANDLE hDevice, QBYTE *data, QULONG *length, QBYTE *transaction);
try:
    QuickUsbLibrary.QuickUsbReadDataAsync.argtypes = [QHANDLE, PQBYTE, PQULONG, PQBYTE]
    QuickUsbLibrary.QuickUsbReadDataAsync.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadDataAsync = None

#QRETURN QuickUsbLibrary.QuickUsbAsyncWait(QHANDLE hDevice, QULONG *bytecount, QBYTE transaction, QBYTE immediate);
try:
    QuickUsbLibrary.QuickUsbAsyncWait.argtypes = [QHANDLE, PQULONG, QBYTE, QBYTE]
    QuickUsbLibrary.QuickUsbAsyncWait.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbAsyncWait = None

#QRETURN QuickUsbLibrary.QuickUsbReadDefault(QHANDLE hDevice, QWORD address, QWORD *data);
try:
    QuickUsbLibrary.QuickUsbReadDefault.argtypes = [QHANDLE, QWORD, PQWORD]
    QuickUsbLibrary.QuickUsbReadDefault.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadDefault = None

#QRETURN QuickUsbLibrary.QuickUsbWriteDefault(QHANDLE hDevice, QWORD address, QWORD data);
try:
    QuickUsbLibrary.QuickUsbWriteDefault.argtypes = [QHANDLE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbWriteDefault.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteDefault = None

#QRETURN QuickUsbLibrary.QuickUsbReadStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes);
try:
    QuickUsbLibrary.QuickUsbReadStorage.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbReadStorage.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadStorage = None

#QRETURN QuickUsbLibrary.QuickUsbWriteStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes);
try:
    QuickUsbLibrary.QuickUsbWriteStorage.argtypes = [QHANDLE, QWORD, PQBYTE, QWORD]
    QuickUsbLibrary.QuickUsbWriteStorage.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteStorage = None

#QUSBLIB QRETURN QuickUsbReadBulkDataAsync(QHANDLE hDevice, PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbReadBulkDataAsync.argtypes = [QHANDLE, PQBYTE, QULONG, PQBULKSTREAM, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID]
    QuickUsbLibrary.QuickUsbReadBulkDataAsync.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadBulkDataAsync = None

#QUSBLIB QRETURN QuickUsbWriteBulkDataAsync(QHANDLE hDevice, PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbWriteBulkDataAsync.argtypes = [QHANDLE, PQBYTE, QULONG, PQBULKSTREAM, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID]
    QuickUsbLibrary.QuickUsbWriteBulkDataAsync.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteBulkDataAsync = None

#QUSBLIB QRETURN QuickUsbBulkWait(QHANDLE hDevice, PQBULKSTREAM bulkStream, QBYTE immediate);
try:
    QuickUsbLibrary.QuickUsbBulkWait.argtypes = [QHANDLE, PQBULKSTREAM, QBYTE]
    QuickUsbLibrary.QuickUsbBulkWait.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbBulkWait = None

#QUSBLIB QRETURN QuickUsbReadBulkDataStartStream(QHANDLE hDevice, PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
try:
    QuickUsbLibrary.QuickUsbReadBulkDataStartStream.argtypes = [QHANDLE, ctypes.POINTER(PQBYTE), QULONG, QULONG, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID, PQBYTE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbReadBulkDataStartStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadBulkDataStartStream = None

#QUSBLIB QRETURN QuickUsbWriteBulkDataStartStream(QHANDLE hDevice, PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
try:
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStream.argtypes = [QHANDLE, ctypes.POINTER(PQBYTE), QULONG, QULONG, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID, PQBYTE, QWORD, QWORD]
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStream = None

#QUSBLIB QRETURN QuickUsbReadBulkDataStartStreamToFile(QHANDLE hDevice, PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags);
try:
    QuickUsbLibrary.QuickUsbReadBulkDataStartStreamToFile.argtypes = [QHANDLE, PQCHAR, QULONG, QULONG, QULONG, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbReadBulkDataStartStreamToFile.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbReadBulkDataStartStreamToFile = None

#QUSBLIB QRETURN QuickUsbWriteBulkDataStartStreamFromFile(QHANDLE hDevice, PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags);
try:
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStreamFromFile.argtypes = [QHANDLE, PQCHAR, QULONG, QULONG, QULONG, PQBYTE, QULONG]
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStreamFromFile.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteBulkDataStartStreamFromFile = None

#QUSBLIB QRETURN QuickUsbStopStream(QHANDLE hDevice, QBYTE streamID, QBOOL wait);
try:
    QuickUsbLibrary.QuickUsbStopStream.argtypes = [QHANDLE, QBYTE, QBOOL]
    QuickUsbLibrary.QuickUsbStopStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbStopStream = None

#QUSBLIB QRETURN QuickUsbProcessStream(QHANDLE hDevice, QBYTE streamID, QULONG milliseconds);
try:
    QuickUsbLibrary.QuickUsbProcessStream.argtypes = [QHANDLE, QBYTE, QULONG]
    QuickUsbLibrary.QuickUsbProcessStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbProcessStream = None

#QUSBLIB QRETURN QuickUsbPauseStream(QHANDLE hDevice, QBYTE streamID, QBOOL immediate);
try:
    QuickUsbLibrary.QuickUsbPauseStream.argtypes = [QHANDLE, QBYTE, QBOOL]
    QuickUsbLibrary.QuickUsbPauseStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbPauseStream = None

#QUSBLIB QRETURN QuickUsbResumeStream(QHANDLE hDevice, QBYTE streamID);
try:
    QuickUsbLibrary.QuickUsbResumeStream.argtypes = [QHANDLE, QBYTE]
    QuickUsbLibrary.QuickUsbResumeStream.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbResumeStream = None

#QUSBLIB QRETURN QuickUsbGetStreamStatus(QHANDLE hDevice, QBYTE streamID, PQULONG status, PQULONG error);
try:
    QuickUsbLibrary.QuickUsbGetStreamStatus.argtypes = [QHANDLE, QBYTE, PQULONG, PQULONG]
    QuickUsbLibrary.QuickUsbGetStreamStatus.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbGetStreamStatus = None

#QUSBLIB QRETURN QuickUsbWriteFirmware(QHANDLE hDevice, PCQCHAR fileName, QULONG options, PQPROGRESS_CALLBACK callback, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbWriteFirmware.argtypes = [QHANDLE, PQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
    QuickUsbLibrary.QuickUsbWriteFirmware.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbWriteFirmware = None

#QUSBLIB QRETURN QuickUsbVerifyFirmware(QHANDLE hDevice, PCQCHAR filename, PQPROGRESS_CALLBACK callback, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbVerifyFirmware.argtypes = [QHANDLE, PQCHAR, PQPROGRESS_CALLBACK, PQVOID]
    QuickUsbLibrary.QuickUsbVerifyFirmware.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbVerifyFirmware = None

#QUSBLIB QRETURN QuickUsbIdentifyEpcs(QHANDLE hDevice, QBYTE nSS, PQWORD epcsId, PQULONG epcsSize, QULONG flags);
try:
    QuickUsbLibrary.QuickUsbIdentifyEpcs.argtypes = [QHANDLE, QBYTE, PQWORD, PQULONG, QULONG]
    QuickUsbLibrary.QuickUsbIdentifyEpcs.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbIdentifyEpcs = None

#QUSBLIB QRETURN QuickUsbConfigureEpcs(QHANDLE hDevice, QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbConfigureEpcs.argtypes = [QHANDLE, QBYTE, PQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
    QuickUsbLibrary.QuickUsbConfigureEpcs.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbConfigureEpcs = None

#QUSBLIB QRETURN QuickUsbVerifyEpcs(QHANDLE hDevice, QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbVerifyEpcs.argtypes = [QHANDLE, QBYTE, PQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
    QuickUsbLibrary.QuickUsbVerifyEpcs.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbVerifyEpcs = None

#QUSBLIB QRETURN QuickUsbEraseEpcs(QHANDLE hDevice, QBYTE nSS, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
try:
    QuickUsbLibrary.QuickUsbEraseEpcs.argtypes = [QHANDLE, QBYTE, QULONG, PQPROGRESS_CALLBACK, PQVOID]
    QuickUsbLibrary.QuickUsbEraseEpcs.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbEraseEpcs = None

#QUSBLIB QRETURN QuickUsbResetStatistic(QHANDLE hDevice, QULONG statistic);
try:
    QuickUsbLibrary.QuickUsbResetStatistic.argtypes = [QHANDLE, QULONG]
    QuickUsbLibrary.QuickUsbResetStatistic.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbResetStatistic = None

#QUSBLIB QRETURN QuickUsbGetStatistic(QHANDLE hDevice, QULONG statistic, QULONG unit, PQFLOAT value, QULONG flags);
try:
    QuickUsbLibrary.QuickUsbResetStatistic.argtypes = [QHANDLE, QULONG, QULONG, PQFLOAT, QULONG]
    QuickUsbLibrary.QuickUsbResetStatistic.restype = QRESULT
except AttributeError:
    QuickUsbLibrary.QuickUsbResetStatistic = None



####################################
# QuickUSB Helper Functions        #
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
    
    

def CreateByteBufferArray(arrays, size, data=None):
    """
    Helper function to create a ctypes-style array of byte buffers needeed as
    an argument to any of the QuickUSB Library functions which require a
    ctypes-style array of byte buffers (such as
    QuickUsb.ReadBulkDataStartStream and QuickUsb.WriteBulkDataStartStream).

    Keyword Arguments:
        arrays -- the number of arrays
        size   -- the size of the buffer in bytes
        data   -- an array of data arrays to initialize each buffer

    Returns:
        ctypes-style byte buffer with 'size' elements
    """
    # Create array of pointers to buffers
    ptr = (PQBYTE * arrays)()

    # Create each buffer
    for k in xrange(arrays):
        if data:
            if isinstance(data[k], str):
                data = bytearray(data)
            ptr[k] = (QBYTE * size)(data[k])
        else:
            ptr[k] = (QBYTE * size)()

    return ptr



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
            err = qusb.LastError()
            QuickUsb.QuickUsbLogFunc('%s() exited with error #%i (%s)' % (func.func_name, int(err), str(err)))



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
    QuickUsbLogFunc = None
    __throwExceptions = True

    #------------------------------#
    # Static Class Methods         #
    #------------------------------#
    def FindModules(cls):
        """
        Static class method to find all QuickUSB modules connected to the host.

        Keyword Arguments:
            None
            
        Returns:
            A tuple of the form (ok, nameList).
                ok       -- Indicates that the function call succeeded or
                            failed
                nameList -- A list of strings, where each element in the 
                            list is the name of a QuickUSB Module of the
                            form 'QUSB-XXX', where 'XXX' is a non-zero
                            padded integer.
        """
        nameList = ctypes.create_string_buffer(1024)

        #QRETURN <= [PQCHAR, QULONG]
        result = QuickUsbLibrary.QuickUsbFindModules(nameList, 1024)

        if not result:
            lastError = QuickUsb.GetLastError()
            return (result, [])
        
        nameList = nameList[:].split('\x00\x00')[0]
        nameList = nameList[:].strip('\x00')
        if len(nameList) == 0:
            return (result, [])
        return (result, nameList.split('\x00'))
    FindModules = classmethod(FindModules)



    def GetLastError(cls):
        """
        Static class method to retrieve the last QuickUSB API error code.
        
        Designed to only be used with static class methods such as
        FindModules(), GetDriverVersion(), and GetDllVersion().  Otherwise,
        use QuickUsb.LastError().
        
        Keyword Arguments:
            None
            
        Returns:
            An integer holding the last error code set by the last failed API
            function call.  To retrieve a string representation of the error
            use the following code:
                print str(QuickUsb.Error(QuickUsb.GetLastError()))
        """
        lastError = QULONG()
        QuickUsbLibrary.QuickUsbGetLastError(ctypes.byref(lastError))
        return lastError.value
    GetLastError = classmethod(GetLastError)



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
    GetDriverVersion = classmethod(GetDriverVersion)



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
    GetDllVersion = classmethod(GetDllVersion)



    def GetThrowExceptions(self):
        """
        Determine if QuickUSB exceptions are to be thrown on errors.
        
        Keyword Arguments:
            None
            
        Returns:
            A bool indicating is exceptions are to be thrown.
        """
        return self.__dict__['__throwExceptions']
    GetThrowExceptions = classmethod(GetThrowExceptions)



    def SetThrowExceptions(self, value):
        """
        Enable/Disable Python exceptions for QuickUSB errors.  By default, API
        errors are reported through return values indicating the success or
        failure of an API call.  This behavior may be changed such that Python
        exceptions are raised when API errors occur.
        
        Keyword Arguments:
            value -- Boolean value
            
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
    SetThrowExceptions = classmethod(SetThrowExceptions)



    #------------------------------#
    # Properties                   #
    #------------------------------#
    def get_IsOpened(self):
        """
        Determine if the module has been opened.
        """
        return ((self.globalOpen > 0) or (self.localOpen > 0))
    IsOpened = property(get_IsOpened, None, doc="Determine if the module has been opened.")



    def get_Name(self):
        """
        Get the QuickUSB Module's name.
        """
        return self.devName



    def set_Name(self, value):
        """
        Set the QuickUSB Module's name.
        """
        self.devName = value
    Name = property(get_Name, set_Name, doc="Get or set the QuickUSB Module's name")



    def get_Make(self):
        """
        Get the QuickUSB Module's make string descriptor.
        """
        return self.GetStringDescriptor(StringDescriptor.Make)
    Make = property(get_Make, None, doc="Get the QuickUSB Module's make string descriptor.")



    def get_Model(self):
        """
        Get the QuickUSB Module's model string descriptor.
        """
        return self.GetStringDescriptor(StringDescriptor.Model)
    Model = property(get_Model, None, doc="Get the QuickUSB Module's model string descriptor.")



    def get_Serial(self):
        """
        Get the QuickUSB Module's serial string descriptor.
        """
        return self.GetStringDescriptor(StringDescriptor.Serial)
    Serial = property(get_Serial, None, doc="Get the QuickUSB Module's serial string descriptor.")



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
            self.hDevice = QHANDLE(0)
            try:
                ok = QuickUsbLibrary.QuickUsbOpen(ctypes.byref(self.hDevice), ctypes.create_string_buffer(self.devName))
            except:
                self.localOpen -= 1
                raise

            if not ok:
                self.lastError = self.GetLastError()
            
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

        # Close the QuickUSB if we opened it locally
        if (self.localOpen > 0):
            if (self.localOpen == 1):
                ok = QuickUsbLibrary.QuickUsbClose(self.hDevice)
            self.localOpen -= 1



    #------------------------------#
    # Public Class Methods         #
    #------------------------------#
    def LastError(self):
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
                print str(QuickUsb.Error(QuickUsbInstance.LastError()))
        """

        return self.lastError



    def Open(self, devname=""):
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
        if (devname != ""):
            self.devName = devname
        result = QuickUsbLibrary.QuickUsbOpen(ctypes.byref(self.hDevice), ctypes.create_string_buffer(self.devName))
        if not result:
            self.lastError = self.GetLastError()
        else:
            self.globalOpen += 1

        # Log the result
        #if self.globalOpen != 0:
            #QuickUsbLog(self, QuickUsb.Open, result)
        #else:
            #QuickUsbLog(self, QuickUsb._OpenPolitely, result)

        return (result,)



    def OpenEx(self, devname="", flags=0):
        """
        Open a QuickUSB device for use by the library.
        
        Keyword Arguments:
            flags -- Flags to control the open.  Valid values are:
                0x0000 - Normal (Same operation as Open())
                0x0001 - Fail if module is already opened
            
        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        #QRETURN <= [PQHANDLE, PQCHAR]
        self.hDevice = QHANDLE(0)
        if (devname != ""):
            self.devName = devname
        result = QuickUsbLibrary.QuickUsbOpenEx(ctypes.byref(self.hDevice), ctypes.create_string_buffer(self.devName), QWORD(flags))
        if not result:
            self.lastError = self.GetLastError()
        else:
            self.globalOpen += 1

        # Log the result
        #if self.globalOpen != 0:
            #QuickUsbLog(self, QuickUsb.OpenEx, result)
        #else:
            #QuickUsbLog(self, QuickUsb._OpenPolitely, result)

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
            self.lastError = self.GetLastError()
        else:
            # Decrement our open counter
            if self.globalOpen > 0:
                self.globalOpen -= 1

        # Log the result
        #if self.globalOpen != 0:
            #QuickUsbLog(self, QuickUsb.Close, result)
        #else:
            #QuickUsbLog(self, QuickUsb._ClosePolitely, result)

        return (result,)



    def GetStringDescriptor(self, stringIndex):
        """
        Returns the string descriptor for the selected QuickUSB module.
        
        Keyword Arguments:
            stringIndex -- The string index to return.  Valid string indices
                           are defined in the QuickUsb.StringDescriptor class.
            
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.GetStringDescriptor, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadSetting, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteSetting, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.StartFpgaConfiguration, result)

        self._ClosePolitely()
        return (result,)



    def WriteFpgaData(self, c_fpgadata, length):
        """
        Sends FPGA configuration data to the FPGA using the QuickUSB FPGA 
        configuration port.
        
        Keyword Arguments:
            c_fpgadata -- A ctypes-style byte array containing the FPGA data to
                          write.  The buffer may be created with a call to
                          QuickUsb.CreateByteBuffer().  If instead an int,
                          tuple, or list is passed in a ctypes-style byte array
                          will automatically be created to wrap the python type
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteFpgaData, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.IsFpgaConfigured, result)

        self._ClosePolitely()
        return (result, configured.value)



    def ConfigureFpga(self, filePath):
        """
        Configure an FPGA with the specified data file.

        Keyword Arguments:
            filePath -- Path tot he FPGA data file.
            
        Returns:
            A tuple of the form (ok,).
                ok    -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PCQCHAR]
        result = QuickUsbLibrary.QuickUsbConfigureFpga(self.hDevice, ctypes.create_string_buffer(filePath))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ConfigureFpga, result)

        self._ClosePolitely()
        return (result,)



    def ReadCommand(self, address, c_data, length):
        """
        Read a block of command values from the high-speed parallel port using
        the QuickUSB module.
        
        Keyword Arguments:
            address -- The address used for the command read
            c_data  -- A ctypes-style byte array that is filled with the read
                       data after the function successfully executes.  The
                       buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadCommand, result)

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
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteCommand, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadData, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadDataWords, result)

        self._ClosePolitely()
        return (result, c_bytes.value / 2)



    def WriteData(self, c_data, bytes):
        """
        Write a block of data bytes to the high-speed parallel port using the 
        QuickUSB module.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteData, result)

        self._ClosePolitely()
        return (result,)



    def WriteDataWords(self, c_array, words):
        """
        Write a block of data words to the high-speed parallel port using the
        QuickUSB module.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteDataWords, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadPortDir, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WritePortDir, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadPort, result)

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
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WritePort, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.SetRS232BaudRate, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.GetNumRS232, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.FlushRS232, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadRS232, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteRS232(self, portNum, c_data, length):
        """
        Write a block of characters to the specified QuickUSB serial port.
        
        Keyword Arguments:
            portNum -- The RS232 port number, either 0 or 1
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteRS232, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadSpi, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteSpi(self, portNum, c_data, length):
        """
        Write a block of bytes to the specified SPI slave port.
        
        Keyword Arguments:
            portNum -- The SPI port number between 0 and 9, inclusive
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteSpi, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteReadSpi, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadI2C, result)

        self._ClosePolitely()
        return (result, bytes.value)



    def WriteI2C(self, address, c_data, length):
        """
        Write a block of bytes to the specified I2C address.
        
        Keyword Arguments:
            address -- The I2C address
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteI2C, result)

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
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.CachedWriteI2C, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.SetTimeout, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.GetFirmwareVersion, result)

        self._ClosePolitely()
        return (result, major.value, minor.value, rev.value)



    def WriteDataAsync(self, c_data, length):
        """
        Write a block of data values to the high-speed parallel port using an 
        asynchronous function call.
        
        Keyword Arguments:
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteDataAsync, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadDataAsync, result)

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
                bytes -- The number of bytes transferred.  If the transaction
                         is still pending this will be zero.  This function
                         must be called until the transaction completes and
                         this value is reported as non-zero.
        """
        if not self._OpenPolitely():
            return (0, 0)

        bytes = QULONG()

        #QRETURN <= [QHANDLE, PQULONG, QBYTE, QBYTE]
        result = QuickUsbLibrary.QuickUsbAsyncWait(self.hDevice, ctypes.byref(bytes), QBYTE(transaction), QBYTE(immediate))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.AsyncWait, result)

        if not immediate:
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadDefault, result)

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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteDefault, result)

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
                       QuickUsb.CreateByteBuffer().
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadStorage, result)

        self._ClosePolitely()
        return (result,)



    def WriteStorage(self, address, c_data, bytes):
        """
        Write a block of bytes to memory.
        
        Keyword Arguments:
            address -- The EEPROM address
            c_data  -- A ctypes-style byte array containing the data to write. 
                       The buffer may be created with a call to 
                       QuickUsb.CreateByteBuffer().  If instead an int, tuple,
                       or list is passed in a ctypes-style byte array will
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
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteStorage, result)

        self._ClosePolitely()
        return (result,)



    def ReadBulkDataAsync(self, c_data, bytes, cRoutine, tag):
        """
        Issue an asynchronous data read.
        
        Keyword Arguments:
            c_data     -- A ctypes-style byte array containing the data to
                          write.  The buffer may be created with a call to
                          QuickUsb.CreateByteBuffer().  If instead a tuple or
                          list is passed in a ctypes-style byte array will 
                          automatically be created to wrap the python type 
                          (which will incur a small performace penalty).  This
                          buffer must remain valid until the request
                          completes.
            bytes      -- The number of bytes to read
            cRoutine   -- (Optional) The completion routine to call when the
                          request completes.
            tag        -- (Optional) A user pointer passed on to the
                          completion rountine used to associate user
                          information with the request.

        Returns:
            A tuple of the form (ok, bulkStream).
                ok         -- Indicates that the function call succeeded or failed
                bulkStream -- A QBULKSTREAM structure that is filled out with
                              essential information about the data request and is
                              used to track the request.
        """
        if not self._OpenPolitely():
            return (0, 0)

        # Allow for c_data to simply be a tuple or list
        val = c_data
        if type(val) == tuple or type(val) == list:
            c_data = CreateByteBuffer(length, val)

        #QRETURN <= [QHANDLE, PQBYTE, QULONG, PQBULKSTREAM, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID]
        c_bulkStream = QBULKSTREAM()
        result = QuickUsbLibrary.QuickUsbReadBulkDataAsync(self.hDevice, c_data, QULONG(bytes), ctypes.pointer(c_bulkStream), PQBULKSTREAM_COMPLETION_ROUTINE(cRoutine), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ReadBulkDataAsync, result)

        self._ClosePolitely()
        return (result, c_bulkStream)



    def WriteBulkDataAsync(self, c_data, bytes, cRoutine, tag):
        """
        Issue an asynchronous data write.

        Keyword Arguments:
            c_data     -- A ctypes-style byte array containing the data to
                          write.  The buffer may be created with a call to
                          QuickUsb.CreateByteBuffer().  If instead a tuple or
                          list is passed in a ctypes-style byte array will 
                          automatically be created to wrap the python type 
                          (which will incur a small performace penalty).  This
                          buffer must remain valid until the request
                          completes.
            bytes      -- The number of bytes to write
            cRoutine   -- (Optional) The completion routine to call when the
                          request completes.
            tag        -- (Optional) A user pointer passed on to the
                          completion rountine used to associate user
                          information with the request.

        Returns:
            A tuple of the form (ok, bulkStream).
                ok         -- Indicates that the function call succeeded or failed
                bulkStream -- A QBULKSTREAM structure that is filled out with
                              essential information about the data request and is
                              used to track the request.
        """
        if not self._OpenPolitely():
            return (0, 0)

        #QRETURN <= [QHANDLE, PQBYTE, QULONG, PQBULKSTREAM, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID]
        c_bulkStream = QBULKSTREAM()
        result = QuickUsbLibrary.QuickUsbWriteBulkDataAsync(self.hDevice, c_data, QULONG(bytes), ctypes.pointer(c_bulkStream), PQBULKSTREAM_COMPLETION_ROUTINE(cRoutine), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteBulkDataAsync, result)

        self._ClosePolitely()
        return (result, c_bulkStream)



    def BulkWait(self, bulkStream, immediate):
        """
        Have the current thread block until the asynchronous request has
        completed and executed its completion routine.

        Keyword Arguments:
            bulkStream -- A QBULKSTREAM structure that is filled out with
                          essential information about the data request and is
                          used to track the request.  This structure is 
                          returned from ReadBulkDataAsync or
                          WriteBulkDataAsync after an asynchrnous request has
                          been successfully issued.
            immediate  -- A boolean value indicating whether a call to this
                          function should return immediately (not block) or
                          wait until either the indicated transaction has
                          completed or the default timeout period has elapsed.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PQBULKSTREAM]
        result = QuickUsbLibrary.QuickUsbBulkWait(self.hDevice, bulkStream, immediate)
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.BulkWait, result)

        self._ClosePolitely()
        return (result,)



    def WriteFirmware(self, fileName, options, callback, tag):
        """
        Write QuickUSB firmware to a device.

        Keyword Arguments:
            fileName -- The path and file name of the QuickUSB firmware (QUSB)
                        file.
            options  -- Specify Programming.PreserveCustomization to preserve
                        customizations present in the current firmware, 
                        Programming.PreserveDefaults to preserve default
                        settings present in the current firmware, 
                        Programming.PreserveDefaults to preserve GPIF
                        customizations, and/or Programming.PreserveSerial to
                        preserve the serial number.  To specify a new serial 
                        number, pass Programming.ProgramSerial bitwise
                        or'ed (|) together with the new serial number (must be
                         between 1 and 65535).
            callback -- A pointer to a QPROGRESS_CALLBACK function which is 
                        called during the programming process to report the 
                        percentage completed.  This may be used to perform 
                        background processes and GUI updates while the 
                        programming process takes place.
            tag      -- A PQVOID variable to store user data that is passed
                        onto the callback function.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PCQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
        result = QuickUsbLibrary.QuickUsbWriteFirmware(self.hDevice, ctypes.create_string_buffer(fileName), QULONG(options), PQPROGRESS_CALLBACK(callback), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.WriteFirmware, result)

        self._ClosePolitely()
        return (result,)



    def VerifyFirmware(self, fileName, callback, tag):
        """
        Verify that the firmware contained in the specified device matches
        that of the supplied firmware file.

        Keyword Arguments:
            fileName -- The path and file name of the QuickUSB firmware (QUSB)
                        file.
            callback -- A pointer to a QPROGRESS_CALLBACK function which is
                        called during the verify process to report the
                        percentage completed.  This may be used to perform 
                        background processes and GUI updates while the 
                        verify process takes place.
            tag      -- A PQVOID variable to store user data that is passed 
                        onto the callback function.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PCQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
        result = QuickUsbLibrary.QuickUsbVerifyFirmware(self.hDevice, ctypes.create_string_buffer(fileName), PQPROGRESS_CALLBACK(callback), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.VerifyFirmware, result)

        self._ClosePolitely()
        return (result,)



    def IdentifyEpcs(self, nSS, flags):
        """
        Identify the EPCS device connected to the SPI bus on the specified nSS
        line.

        Keyword Arguments:
            nSS      -- The nSS SPI line that the EPCS device is connected to
                        (0-9).
            epcsSize -- A pointer to QWORD used to store the ID of the EPCS
                        found.  This will be one of EpcsId.Epcs1 (0x10),
                        EpcsId.Epcs4 (0x12), EpcsId.Epcs16 (0x14),
                        EpcsId.Epcs64 (0x16), or EpcsId.Unknown (0x00).
            flags    -- Additional flags controlling the operation of this 
                        function.  Specify QUICKUSB_EPCS_IGNORE_NCE (0x0001) 
                        to prevent the function from automatically asserting 
                        the nCE line of the FPGA at the beginning of the
                        function are re-asserting is at the end of the
                        function.

        Returns:
            A tuple of the form (ok,).
                ok       -- Indicates that the function call succeeded or
                            failed
                epcsId   -- The ID of the EPCS device
                epcsSize -- The size of the EPCS device, in bytes
        """
        if not self._OpenPolitely():
            return (0, 0, 0)

        #QRETURN <= [QHANDLE, QBYTE, PQWORD, PQULONG, QULONG]
        c_epcsId = QWORD()
        c_epcsSize = QULONG()
        result = QuickUsbLibrary.QuickUsbIdentifyEpcs(self.hDevice, QBYTE(nSS), ctypes.pointer(c_epcsId), ctypes.pointer(c_epcsSize), QULONG(flags))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.IdentifyEpcs, result)

        self._ClosePolitely()
        return (result, c_epcsId.value, c_epcsSize.value)



    def VerifyEpcs(self, nSS, filePath, flags, callback, tag):
        """
        Verify that the data contained in the EPCS device matches that of the 
        specified FPGA data file.

        Keyword Arguments:
            nSS      -- The nSS SPI line that the EPCS device is connected to
                        (0-9).
            filePath -- The path and file name of the FPGA data file.
            flags    -- Additional flags controlling the operation of this 
                        function.  Specify QUICKUSB_EPCS_IGNORE_NCE to prevent
                        the function from automatically asserting the nCE
                        line of the FPGA at the beginning of the function are 
                        re-asserting is at the end of the function.
            callback -- A pointer to a QPROGRESS_CALLBACK function which is 
                        called during the verify process to report the 
                        percentage completed.  This may be used to perform 
                        background processes and GUI updates while the verify
                        process takes place.
            tag      -- A PQVOID variable to store user data that is passed 
                        onto the callback function.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QBYTE, PCQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
        result = QuickUsbLibrary.QuickUsbVerifyEpcs(self.hDevice, QBYTE(nSS), ctypes.create_string_buffer(filePath), QULONG(flags), PQPROGRESS_CALLBACK(callback), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.VerifyEpcs, result)

        self._ClosePolitely()
        return (result,)



    def EraseEpcs(self, nSS, flags, callback, tag):
        """
        Perform the bulk erase action of the EPCS device to erase the entire
        device to 0xFF.

        Keyword Arguments:
            nSS      -- The nSS SPI line that the EPCS device is connected to
                        (0-9).
            flags    -- Additional flags controlling the operation of this 
                        function.  Specify QUICKUSB_EPCS_IGNORE_NCE to prevent
                        the function from automatically asserting the nCE
                        line of the FPGA at the beginning of the function are 
                        re-asserting is at the end of the function.
            callback -- A pointer to a QPROGRESS_CALLBACK function which is
                        called during the erase process to report the
                        percentage completed.  This may be used to perform 
                        background processes and GUI updates while the erase
                        process takes place.
            tag      -- A PQVOID variable to store user data that is passed 
                        onto the callback function.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QBYTE, QULONG, PQPROGRESS_CALLBACK, PQVOID]
        result = QuickUsbLibrary.QuickUsbEraseEpcs(self.hDevice, QBYTE(nSS), QULONG(flags), PQPROGRESS_CALLBACK(callback), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.EraseEpcs, result)

        self._ClosePolitely()
        return (result,)



    def ConfigureEpcs(self, nSS, filePath, flags, callback, tag):
        """
        Write the specified FPGA data file to the EPCS device.

        Keyword Arguments:
            nSS      -- The nSS SPI line that the EPCS device is connected to
                        (0-9).
            filePath -- The path and file name of the FPGA data file.
            flags    -- Additional flags controlling the operation of this 
                        function.  Specify QUICKUSB_EPCS_IGNORE_NCE to prevent
                        the function from automatically asserting the nCE
                        line of the FPGA at the beginning of the function are
                        re-asserting is at the end of the function.  Specify 
                        QUICKUSB_EPCS_SKIP_ERASE (0x0002) to prevent the bulk 
                        erase step prior to writing the device.
            callback -- A pointer to a QPROGRESS_CALLBACK function which is
                        called during the configuring process to report the
                        percentage completed.  This may be used to perform 
                        background processes and GUI updates while the 
                        configuring process takes place.
            tag      -- A PQVOID variable to store user data that is passed 
                        onto the callback function.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QBYTE, PCQCHAR, QULONG, PQPROGRESS_CALLBACK, PQVOID]
        result = QuickUsbLibrary.QuickUsbConfigureEpcs(self.hDevice, QBYTE(nSS), ctypes.create_string_buffer(filePath), QULONG(flags), PQPROGRESS_CALLBACK(callback), PQVOID(tag))
        if not result:
            self.lastError = self.GetLastError()

        # Log the result
        #QuickUsbLog(self, QuickUsb.ConfigureEpcs, result)

        self._ClosePolitely()
        return (result,)



    def ReadBulkDataStartStream(self, c_dataArr, numBuffers, bufferSize, cRoutine, tag, numThreads, concurrency):
        """
        Start streaming data into the computer from the QuickUSB device.

        Keyword Arguments:
            c_dataArr  -- (Optional) An ctypes array of buffers (create with a 
                          call to QuickUsb.CreateByteBufferArray) to the user
                          allocated buffers used to store the read data.  
                          These buffers must remain valid until the read
                          stream has been stopped.  If this parameter is None,
                          then the API will allocate and managed the memory
                          for the data buffers.
            numBuffers -- The number of buffers.
            bufferSize -- The byte size of each buffer.
            cRoutine   -- The completion routine (callback function) that is
                          called each time a buffer is filled with data.
            tag        -- (Optional) A pointer containing user-specific data
                          that is passed into the completion routine.
            numThreads -- The number of threads to allocate for this stream.
                          If numThreads is zero, no threads are created and 
                          the stream will be single-threaded.  If numThreads 
                          is greater than zero, the stream is multithreaded.
                          The indicated number of threads will be created and 
                          dedicated to asynchronously processing completion 
                          routines.  When multithreading, the recommended 
                          number of threads to maximize multithreading 
                          performance is equal to the number of 
                          processors/cores in the system times two.
            concurrency-- The number of threads that are allowed to execute
                          completion routines simultaneously.  Specify zero if
                          numThreads is zero.  When multithreading, the number
                          of threads allowed to execute completion routines 
                          concurrently will be limited by the value of 
                          'concurrency', which must be at least one.  The
                          recommended concurrency to maximize multithreading 
                          performance isequal to the total number of
                          processors/cores in the system.

        Returns:
            A tuple of the form (ok, streamID).
                ok       -- Indicates that the function call succeeded or failed
                streamID -- The ID of the stream
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PQBYTE[], QULONG, QULONG, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID, PQBYTE, QWORD, QWORD]
        self.__StreamCR = PQBULKSTREAM_COMPLETION_ROUTINE(cRoutine) # This reference must be held on to until the stream is stopped
        streamID = QBYTE()
        result = QuickUsbLibrary.QuickUsbReadBulkDataStartStream(self.hDevice, c_dataArr, QULONG(numBuffers), QULONG(bufferSize), self.__StreamCR, PQVOID(tag), ctypes.byref(streamID), QWORD(numThreads), QWORD(concurrency))
        if not result:
            self.lastError = self.GetLastError()

            # Log the result
            #QuickUsbLog(self, QuickUsb.ReadBulkDataStartStream, result)

            self._ClosePolitely()

            return (result, 0)

        # Don't politely close module now that we're streaming
        return (result, streamID.value)



    def WriteBulkDataStartStream(self, c_dataArr, numBuffers, bufferSize, cRoutine, tag, numThreads, concurrency):
        """
        Start streaming data from the computer to the QuickUSB device.

        Keyword Arguments:
            c_dataArr  -- (Optional) An ctypes array of buffers (create with a
                          call to QuickUsb.CreateByteBufferArray) to the user
                          allocated buffers holding the data to write.  These
                          buffers must remain valid until the read stream has 
                          been stopped.
            numBuffers -- The number of buffers.
            bufferSize -- The byte size of each buffer.
            cRoutine   -- The completion routine (callback function) that is
                          called each time a buffer has been written.  This 
                          buffer must be refilled with the new data to write 
                          from within the completion routine.
            tag        -- (Optional) A pointer containing user-specific data
                          that is passed into the completion routine.
            numThreads -- The number of threads to allocate for this stream.
                          If numThreads is zero, no threads are created and 
                          the stream will be single-threaded.  If numThreads 
                          is greater than zero, the stream is multithreaded.
                          The indicated number of threads will be created and 
                          dedicated to asynchronously processing completion 
                          routines.  When multithreading, the recommended 
                          number of threads to maximize multithreading 
                          performance is equal to the number of 
                          processors/cores in the system times two.
            concurrency-- The number of threads that are allowed to execute
                          completion routines simultaneously.  Specify zero if
                          numThreads is zero.  When multithreading, the number
                          of threads allowed to execute completion routines 
                          concurrently will be limited by the value of 
                          'concurrency', which must be at least one.  The
                          recommended concurrency to maximize multithreading 
                          performance isequal to the total number of 
                          processors/cores in the system.

        Returns:
            A tuple of the form (ok,).
                ok       -- Indicates that the function call succeeded or failed
                streamID -- The ID of the stream
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, PQBYTE[], QULONG, QULONG, PQBULKSTREAM_COMPLETION_ROUTINE, PQVOID]
        streamID = QBYTE()
        self.__StreamCR = PQBULKSTREAM_COMPLETION_ROUTINE(cRoutine) # This reference must be held on to until the stream is stopped
        result = QuickUsbLibrary.QuickUsbWriteBulkDataStartStream(self.hDevice, c_dataArr, QULONG(numBuffers), QULONG(bufferSize), self.__StreamCR, PQVOID(tag), ctypes.byref(streamID), QWORD(numThreads), QWORD(concurrency))
        if not result:
            self.lastError = self.GetLastError()

            # Log the result
            #QuickUsbLog(self, QuickUsb.WriteBulkDataStartStream, result)

            self._ClosePolitely()

            return (result, 0)

        # Don't politely close module now that we're streaming
        return (result, streamID.value)



    def ReadBulkDataStartStreamToFile(self, path, numBuffers, bufferSize, maxTransfers, flags):
        """
        Start streaming data into the computer to a file from the QuickUSB 
        device.

        Keyword Arguments:
            path         -- Path to the file
            numBuffers   -- The number of buffers.
            bufferSize   -- The byte size of each buffer.
            maxTransfers -- (Optional) The maximum number of transfers to 
                            perform before automatically pausing the stream
            flags        -- (Optional) Additional flags.  Pass 
                            StreamFlag.MemoryFile to use a memory file instead
                            of a disk file.

        Returns:
            A tuple of the form (ok, streamID).
                ok       -- Indicates that the function call succeeded or failed
                streamID -- The ID of the stream
        """
        if not self._OpenPolitely():
            return (0, 0)

        #QRETURN <= [QHANDLE, PQCHAR, QULONG, QULONG, QULONG, PQBYTE, QULONG]
        streamID = QBYTE()
        result = QuickUsbLibrary.QuickUsbReadBulkDataStartStreamToFile(self.hDevice, ctypes.create_string_buffer(path), QULONG(numBuffers), QULONG(bufferSize), QULONG(maxTransfers), ctypes.byref(streamID), QULOING(flags))
        if not result:
            self.lastError = self.GetLastError()

            # Log the result
            #QuickUsbLog(self, QuickUsb.ReadBulkDataStartStreamToFile, result)

            self._ClosePolitely()

            return (result, 0)

        # Don't politely close module now that we're streaming
        return (result, streamID.value)



    def WriteBulkDataStartStreamFromFile(self, path, numBuffers, bufferSize, maxTransfers, flags):
        """
        Start streaming data from a file on the computer to the QuickUSB
        device.

        Keyword Arguments:
            path         -- Path to the file
            numBuffers   -- The number of buffers.
            bufferSize   -- The byte size of each buffer.
            maxTransfers -- (Optional) The maximum number of transfers to 
                            perform before automatically pausing the stream
            flags        -- (Optional) Additional flags.  Pass 
                            StreamFlag.MemoryFile to use a memory file instead
                            of a disk file.

        Returns:
            A tuple of the form (ok, streamID).
                ok       -- Indicates that the function call succeeded or failed
                streamID -- The ID of the stream
        """
        if not self._OpenPolitely():
            return (0, 0)

        #QRETURN <= [QHANDLE, PQCHAR, QULONG, QULONG, QULONG, PQBYTE, QULONG]
        streamID = QBYTE()
        result = QuickUsbLibrary.QuickUsbWriteBulkDataStartStreamFromFile(self.hDevice, ctypes.create_string_buffer(path), QULONG(numBuffers), QULONG(bufferSize), QULONG(maxTransfers), ctypes.byref(streamID), QULOING(flags))
        if not result:
            self.lastError = self.GetLastError()

            # Log the result
            #QuickUsbLog(self, QuickUsb.WriteBulkDataStartStreamFromFile, result)

            self._ClosePolitely()

            return (result, 0)

        # Don't politely close module now that we're streaming
        return (result, streamID.value)



    def StopStream(self, streamID, immediate):
        """
        Stop a previously started read or write data stream.

        Keyword Arguments:
            streamID  -- The ID of the stream.
            immediate -- If False, this function will block until the stream
                         has completely stopped and any remaining callbacks 
                         have been called.  If true, the stream will be marked
                         to shutdown and will return without blocking.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        # Don't politely open as we must already be open for this call

        #QRETURN <= [QHANDLE, QBYTE, QBOOL]
        result = QuickUsbLibrary.QuickUsbStopStream(self.hDevice, QBYTE(streamID), QBOOL(immediate))
        if not result:
            self.lastError = self.GetLastError()
        
        self.__StreamCR = None

        # Log the result
        #QuickUsbLog(self, QuickUsb.StopStream, result)

        # We're done streaming so we can politely close the module again
        if not immediate:
            self._ClosePolitely()
        return (result,)



    def ProcessStream(self, streamID, milliseconds):
        """
        For single-threaded streams, this function is called to process and
        re-issue completed requests.

        Keyword Arguments:
            streamID     -- The ID of the stream.
            milliseconds -- The number of milliseconds the caller is willing 
                            to wait until a data request on the indicated 
                            stream has completed and is processed.  If no 
                            data request on the indicated stream has 
                            completed within the specified amount of time, 
                            the function will return unsuccessfully and a 
                            call to QuickUsbGetLastError will indicate the 
                            error QUICKUSB_ERROR_TIMEOUT.  If milliseconds 
                            is zero, the function will process any requests 
                            that are completed or timeout immediately.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        # Don't politely open as we must already be open for this call

        #QRETURN <= [QHANDLE, QBYTE, QULONG]
        result = QuickUsbLibrary.QuickUsbProcessStream(self.hDevice, QBYTE(streamID), QULONG(milliseconds))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.StopStream, result)

        # Don't politely close module now that we're streaming
        return (result,)



    def PauseStream(self, streamID, immediate):
        """
        Pause the automatic re-issuing of data requests on a stream.

        Keyword Arguments:
            streamID  -- The ID of the stream.
            immediate -- If zero (or false), this function will block until 
                         the stream has paused and all in-process requests 
                         have completed.  If non-zero (or true), the stream 
                         will be marked to enter the pause state and will 
                         return immediately without waiting for any in-process
                         requests to complete.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        # Don't politely open as we must already be open for this call

        #QRETURN <= [QHANDLE, PQULONG, PQULONG]
        result = QuickUsbLibrary.QuickUsbPauseStream(self.hDevice, QBYTE(streamID), QBOOL(immediate))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.StopStream, result)

        # Don't politely close module now that we're streaming
        return (result,)



    def ResumeStream(self, streamID):
        """
        Resume the automatic re-issuing of data requests on a stream.

        Keyword Arguments:
            streamID  -- The ID of the stream.

        Returns:
            A tuple of the form (ok,).
                ok -- Indicates that the function call succeeded or failed
        """
        # Don't politely open as we must already be open for this call

        #QRETURN <= [QHANDLE, QBYTE]
        result = QuickUsbLibrary.QuickUsbResumeStream(self.hDevice, QBYTE(streamID))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.StopStream, result)

        # Don't politely close module now that we're streaming
        return (result,)



    def GetStreamStatus(self, streamID):
        """
        Retrieve the current status of the specified stream

        Keyword Arguments:
            streamID  -- The ID of the stream.

        Returns:
            A tuple of the form (ok, status, error).
                ok     -- Indicates that the function call succeeded or failed
                status -- The current status of the stream
                error  -- The last status reported by the stream
        """
        # Don't politely open as we must already be open for this call

        #QRETURN <= [QHANDLE, QBYTE]
        status = QULONG()
        streamErr = QULONG()
        result = QuickUsbLibrary.QuickUsbGetStreamStatus(self.hDevice, QBYTE(streamID), ctypes.byref(status), ctypes.byref(streamErr))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.GetStreamStatus, result)

        # Don't politely close module now that we're streaming
        return (result, status.value, streamErr.value)



    def ResetStatistic(self, statistic):
        """
        Reset the specified statistic.  All statistics should be reset prior
        to their use.

        Keyword Arguments:
            statistic  -- The statistic to reset.  Valid statistics are
                          defined in the Stat enumeration.

        Returns:
            A tuple of the form (ok,).
                ok     -- Indicates that the function call succeeded or failed
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QULONG]
        result = QuickUsbLibrary.QuickUsbResetStatistic(self.hDevice, QULONG(statistic))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.ResetStatistic, result)

        self._ClosePolitely()
        return (result, status.value, streamErr.value)



    def GetStatistic(self, statistic, unit, flags):
        """
        Retrieve the specified statistic in the indicated unit

        Keyword Arguments:
            statistic  -- The statistic to retrieve.  Valid statistics are
                          defined in the Stat enumeration.
            unit       -- The unit of the statistic should be reported in.
                          Define units using the StatUnit enumeration.
            flags      -- Additional flags

        Returns:
            A tuple of the form (ok,).
                ok     -- Indicates that the function call succeeded or failed
                value  -- The value of the requested statistic
        """
        if not self._OpenPolitely():
            return (0,)

        #QRETURN <= [QHANDLE, QULONG, QULONG, PQULONG, QULONG]
        value = QFLOAT()
        result = QuickUsbLibrary.QuickUsbGetStatistic(self.hDevice, QULONG(statistic), QULONG(unit), ctypes.byref(value), QULONG(flags))
        if not result:
            self.lastError = self.GetLastError()
        
        # Log the result
        #QuickUsbLog(self, QuickUsb.GetStatistic, result)

        self._ClosePolitely()
        return (result, value.value)



#Test Program
if __name__ == '__main__':
    (ok, major, minor, rev) = QuickUsb.GetDllVersion()
    print "DLL Version:", (major, minor, rev)
    print

    print "Finding Modules"
    (ok, nameList) = QuickUsb.FindModules()
    if not ok:
        errCode = QuickUsb.GetLastError()
        print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))

    print "Found Modules:", nameList
    print

    if len(nameList) > 0:
        qusb = QuickUsb(nameList[0])

        print "Opening:", qusb.Name
        (ok,) = qusb.Open()
        if not ok:
            errCode = QuickUsb.LastError()
            print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))

        (ok, major, minor, rev) = QuickUsb.GetDriverVersion()
        print "\tDriver Version:", (major, minor, rev)

        (ok, major, minor, rev) = qusb.GetFirmwareVersion()
        print "\tFirmware Version:", (major, minor, rev)

        #Print string descriptors
        for k in range(1,4):
            sd = StringDescriptor(k)
            (ok, string) = qusb.GetStringDescriptor(sd)
            print "\t%s: %s" % (str(sd), string)

        #Read PA
        buffer = CreateByteBuffer(10)
        (ok, bytes) = qusb.ReadPort(Port.A, buffer, len(buffer))
        print "\tReading Port A:", buffer[:]
        
        #Write data
        data = CreateByteBuffer(512)
        qusb.WriteData(data, 512)

        #Close the module
        print "Closing:", qusb.Name
        (ok,) = qusb.Close()
        if not ok:
            errCode = QuickUsb.LastError()
            print "\t-> Failed with code %i - %s" % (errCode, Error(errCode))
