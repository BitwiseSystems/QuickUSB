/*=============================================================================
 Title        : CQuickUSB.h
 Description  : QuickUSB Class Header File
 Notes        : None
 History      :

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

=============================================================================*/
#include "QuickUSB.h"
#include <string.h>


// QuickUSB Class Declaration
class CQuickUsb {
public:
    ////////////////////////////////////
    // Constructors and Deconstructor //
    ////////////////////////////////////
    CQuickUsb();
    CQuickUsb(PCQCHAR devName);
    ~CQuickUsb();

    ////////////////
    // Properties //
    ////////////////
    QBOOL IsOpened();
    PQCHAR GetName();
    PQCHAR GetDeviceName();
    void SetName(PCQCHAR name);
    void SetDeviceName(PCQCHAR name);

    //////////////////
    // Enumerations //
    //////////////////
    enum Port {
        PortA=PORT_A,
        PortB=PORT_B,
        PortC=PORT_C,
        PortD=PORT_D,
        PortE=PORT_E
    };
    
    enum StringDescriptor {
        StringDescriptorMake = QUICKUSB_MAKE,
        StringDescriptorModel = QUICKUSB_MODEL,
        StringDescriptorSerial = QUICKUSB_SERIAL
    };

    enum Setting {
        SettingEp26Config = SETTING_EP26CONFIG,
        SettingWordWide = SETTING_WORDWIDE,
        SettingDataAddress = SETTING_DATAADDRESS,
        SettingFifoConfig = SETTING_FIFO_CONFIG,
        SettingFpgaType = SETTING_FPGATYPE,
        SettingCpuConfig = SETTING_CPUCONFIG,
        SettingSpiConfig = SETTING_SPICONFIG,
        SettingSlaveFifoFlags = SETTING_SLAVEFIFOFLAGS,
        SettingI2Ctl = SETTING_I2CTL,
        SettingPortA = SETTING_PORTA,
        SettingPortB = SETTING_PORTB,
        SettingPortC = SETTING_PORTC,
        SettingPortD = SETTING_PORTD,
        SettingPortE = SETTING_PORTE,
        SettingPortACCfg = SETTING_PORTACCFG,
        SettingPinFlags = SETTING_PINFLAGS,
        SettingVersionBuild = SETTING_VERSIONBUILD,
        SettingVersionSpeed = SETTING_VERSIONSPEED,
        SettingTimeoutHigh = SETTING_TIMEOUT_HIGH,
        SettingTimeoutLow = SETTING_TIMEOUT_LOW
    };

    enum FpgaConfType {
        FpgaConfTypeAlteraPS = FPGATYPE_ALTERAPS,
        FpgaConfTypeXilinxSS = FPGATYPE_XILINXSS
    };

    enum Error {
        ErrorNoError = QUICKUSB_ERROR_NO_ERROR,
        ErrorOutOfMemory = QUICKUSB_ERROR_OUT_OF_MEMORY,
        ErrorCannotOpenModule = QUICKUSB_ERROR_CANNOT_OPEN_MODULE,
        ErrorCannotFindDevice = QUICKUSB_ERROR_CANNOT_FIND_DEVICE,
        ErrorIoctlFailed = QUICKUSB_ERROR_IOCTL_FAILED,
        ErrorInvalidParameter = QUICKUSB_ERROR_INVALID_PARAMETER,
        ErrorTimeout = QUICKUSB_ERROR_TIMEOUT,
        ErrorFunctionNotSupported = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED,
        ErrorI2cBusError = QUICKUSB_ERROR_I2C_BUS_ERROR,
        ErrorI2cNoAck = QUICKUSB_ERROR_I2C_NO_ACK,
        ErrorI2cSlaveWait = QUICKUSB_ERROR_I2C_SLAVE_WAIT,
        ErrorI2cTimeout = QUICKUSB_ERROR_I2C_TIMEOUT,
        ErrorUnknownDriverType = QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE,
        ErrorAlreadyOpened = QUICKUSB_ERROR_ALREADY_OPENED,
        ErrorCannotCloseModule = QUICKUSB_ERROR_CANNOT_CLOSE_MODULE,
        ErrorFpgaInitFailed = QUICKUSB_ERROR_FPGA_INIT_FAILED,
        ErrorPacketNotMultiple512 = QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_512,
        ErrorPacketNotMultiple64 = QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_64,
        ErrorUnknownSystemError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR,
        ErrorAborted = QUICKUSB_ERROR_ABORTED,
        ErrorDeprecated = QUICKUSB_ERROR_DEPRECATED,
        ErrorInvalidSerial =  QUICKUSB_ERROR_INVALID_SERIAL,
        ErrorCannotOpenFile = QUICKUSB_ERROR_CANNOT_OPEN_FILE,
        ErrorVerifyFailed = QUICKUSB_ERROR_VERIFY_FAILED,
        ErrorFirmwareError = QUICKUSB_ERROR_FIRMWARE_ERROR,
        ErrorAlreadyCompleted = QUICKUSB_ERROR_ALREADY_COMPLETED,
        ErrorNotCompleted = QUICKUSB_ERROR_NOT_COMPLETED,
        ErrorFpgaConfigFailed = QUICKUSB_ERROR_FPGA_CONFIG_FAILED,
        ErrorInvalidOperation = QUICKUSB_ERROR_INVALID_OPERATION,
        ErrorTooManyRequests = QUICKUSB_ERROR_TOO_MANY_REQUESTS,
        ErrorEpcsNotFound = QUICKUSB_ERROR_EPCS_NOT_FOUND,
        ErrorEpcsTooSmall = QUICKUSB_ERROR_EPCS_TOO_SMALL,
        ErrorNotStreaming = QUICKUSB_ERROR_NOT_STREAMING,
        ErrorBufferNotAligned = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED,
        ErrorInternalError = QUICKUSB_ERROR_INTERNAL_ERROR, 
        ErrorDeviceIsClosing = QUICKUSB_ERROR_DEVICE_IS_CLOSING, 
        ErrorProtection = QUICKUSB_ERROR_PROTECTION,
        ErrorNeedData = QUICKUSB_ERROR_NEED_DATA,
        ErrorFileNotFound = QUICKUSB_ERROR_FILE_NOT_FOUND,
        ErrorFileAlreadyExists = QUICKUSB_ERROR_FILE_ALREADY_EXISTS,
        ErrorFileRW = QUICKUSB_ERROR_FILE_RW,
        ErrorFileEOF = QUICKUSB_ERROR_FILE_EOF,
        ErrorFileName = QUICKUSB_ERROR_FILE_NAME,
        ErrorAccessDenied = QUICKUSB_ERROR_ACCESS_DENIED,
        ErrorSharingViolation = QUICKUSB_ERROR_SHARING_VIOLATION
    };

    enum EpcsId {
        EpcsIdUnknown = QUICKUSB_EPCS_ID_UNKNOWN,
        EpcsId1 = QUICKUSB_EPCS1_ID,
        EpcsId4 = QUICKUSB_EPCS4_ID,
        EpcsId16 = QUICKUSB_EPCS16_ID,
        EpcsId64 = QUICKUSB_EPCS64_ID
    };

    enum EpcsFlags {
        EpcsFlagIgnoreNCE = QUICKUSB_EPCS_IGNORE_NCE,
        EpcsFlagSkipErase = QUICKUSB_EPCS_SKIP_ERASE
    };

    enum EpcsSize {
        Epcs1Size = QUICKUSB_EPCS1_SIZE,
        Epcs4Size = QUICKUSB_EPCS4_SIZE,
        Epcs16Size = QUICKUSB_EPCS16_SIZE,
        Epcs64Size = QUICKUSB_EPCS64_SIZE,
    };
    
    enum Programming {
        ProgrammingProgramSerial = QUICKUSB_PROGRAM_SERIAL,
        ProgrammingPreserveCustomization = QUICKUSB_PRESERVE_CUSTOMIZATION,
        ProgrammingPreserveDefaults = QUICKUSB_PRESERVE_DEFAULTS,
        ProgrammingPreserveGpif = QUICKUSB_PRESERVE_GPIF,
        ProgrammingPreserveSerial = QUICKUSB_PRESERVE_SERIAL
    };

    enum OpenFlags {
        OpenFlagNormal = QUICKUSB_OPEN_NORMAL,
        OpenFlagOpenIfClosed = QUICKUSB_OPEN_IF_CLOSED
    };

    enum Stat {
        StatAll = QUICKUSB_STAT_ALL,
        StatReadThroughput = QUICKUSB_STAT_READ_THROUGHPUT,
        StatWriteThroughput = QUICKUSB_STAT_WRITE_THROUGHPUT,
        StatTotalThroughput = QUICKUSB_STAT_TOTAL_THROUGHPUT,
        StatTotalDataRead = QUICKUSB_STAT_TOTAL_DATA_READ,
        StatTotalDataWritten = QUICKUSB_STAT_TOTAL_DATA_WRITTEN
    };

    enum StatUnit {
        StatUnitBytes = QUICKUSB_STAT_UNIT_BYTES,
        StatUnitKB = QUICKUSB_STAT_UNIT_KB,
        StatUnitMB = QUICKUSB_STAT_UNIT_MB,
        StatUnitGB = QUICKUSB_STAT_UNIT_GB,

        StatUnitNS = QUICKUSB_STAT_UNIT_PER_NS,
        StatUnitUS = QUICKUSB_STAT_UNIT_PER_US,
        StatUnitMS = QUICKUSB_STAT_UNIT_PER_MS,
        StatUnitSec = QUICKUSB_STAT_UNIT_PER_SEC,
        StatUnitMin = QUICKUSB_STAT_UNIT_PER_MIN,
        StatUnitHour = QUICKUSB_STAT_UNIT_PER_HOUR,

        StatUnitBytesPerSec = QUICKUSB_STAT_UNIT_BYTES_PER_SEC,
        StatUnitKBPerSec = QUICKUSB_STAT_UNIT_KB_PER_SEC,
        StatUnitMBPerSec = QUICKUSB_STAT_UNIT_MB_PER_SEC
    };

    enum StreamFlag {
        StreamFlagCreateAlways = QUICKUSB_STREAM_CREATE_ALWAYS,
        StreamFlagLoopAtEof = QUICKUSB_STREAM_LOOP_AT_EOF
    };

    enum StreamStatus {
        StreamStatusUnknown = QUICKUSB_STREAM_STATUS_UNKNOWN,
        StreamStatusRunning = QUICKUSB_STREAM_STATUS_RUNNING,
        StreamStatusStopped = QUICKUSB_STREAM_STATUS_STOPPED,
        StreamStatusPaused = QUICKUSB_STREAM_STATUS_PAUSED
    };

    enum DataFlags {
        OutOfOrder = QUICKUSB_OUT_OF_ORDER
    };



    //////////////
    // Base API //
    //////////////
    static QRETURN FindModules(PQCHAR nameList, QULONG length);
    static QRETURN GetDriverVersion(PQWORD MajorDriverVersion, PQWORD MinorDriverVersion, PQWORD BuildDriverVersion);
    static QRETURN GetDllVersion(PQWORD MajorDllVersion, PQWORD MinorDllVersion, PQWORD BuildDllVersion);
    static QULONG GetLastError();
    static QULONG GetLastDriverError();

    QRETURN Open();
    QRETURN Open(PQCHAR deviceName);
    QRETURN OpenEx(QWORD flags);
    QRETURN OpenEx(PQCHAR deviceName, QWORD flags);
    QRETURN Close();
    QRETURN GetStringDescriptor(QBYTE index, PQCHAR buffer, QWORD length);
    QRETURN SetTimeout(QULONG timeOut);
    QRETURN GetFirmwareVersion(PQWORD MajorFirmwareVersion, PQWORD MinorFirmwareVersion, PQWORD BuildFirmwareVersion);
    QULONG  LastError();
    QULONG LastDriverError();

    //////////////////
    // Settings API //
    //////////////////
    QRETURN ReadSetting(QWORD address, PQWORD value);
    QRETURN WriteSetting(QWORD address, QWORD value);
    QRETURN ReadDefault(QWORD address, PQWORD data);
    QRETURN WriteDefault(QWORD address, QWORD data);

    ////////////////////////////
    // FPGA Configuration API //
    ////////////////////////////
    QRETURN StartFpgaConfiguration();
    QRETURN WriteFpgaData(PQBYTE fpgadata, QULONG datalength);
    QRETURN IsFpgaConfigured(PQWORD isConfigured);
    QRETURN ConfigureFpga(PCQCHAR filePath);

    //////////////
    // HSPP API //
    //////////////
    QRETURN ReadCommand(QWORD address, PQBYTE data, PQWORD length);
    QRETURN WriteCommand(QWORD address, PQBYTE data, QWORD length);
    QRETURN ReadData(PQBYTE data, PQULONG length);
    QRETURN ReadDataEx(PQBYTE data, PQULONG length, QULONG flags);
    QRETURN WriteData(PQBYTE data, QULONG length);
    QRETURN WriteDataEx(PQBYTE data, PQULONG length, QULONG flags);

    QRETURN WriteDataAsync(PQBYTE data, QULONG length, PQBYTE transaction);
    QRETURN ReadDataAsync(PQBYTE data, PQULONG length, PQBYTE transaction);
    QRETURN AsyncWait(PQULONG bytecount, QBYTE transaction, QBYTE immediate);

    //////////////
    // GPIO API //
    //////////////
    QRETURN ReadPortDir(QWORD address, PQBYTE data);
    QRETURN WritePortDir(QWORD address, QBYTE data);
    QRETURN ReadPort(QWORD address, PQBYTE data, PQWORD length);
    QRETURN ReadPort(QWORD address, PQBYTE value);
    QRETURN WritePort(QWORD address, PQBYTE data, QWORD length);
    QRETURN WritePort(QWORD address, QBYTE value);

    ///////////////
    // RS232 API //
    ///////////////
    QRETURN SetRS232BaudRate(QULONG baudRate);
    QRETURN GetNumRS232(QBYTE portNum, PQWORD length);
    QRETURN FlushRS232(QBYTE portNum);
    QRETURN ReadRS232(QBYTE portNum, PQBYTE data, PQWORD length);
    QRETURN WriteRS232(QBYTE portNum, PQBYTE data, QWORD length);

    /////////////
    // SPI API //
    /////////////
    QRETURN ReadSpi(QBYTE portNum, PQBYTE data, PQWORD length);
    QRETURN WriteSpi(QBYTE portNum, PQBYTE data, QWORD length);
    QRETURN WriteReadSpi(QBYTE portNum, PQBYTE data, QWORD length);

    /////////////
    // I2C API //
    /////////////
    QRETURN ReadI2C(QWORD address, PQBYTE data, PQWORD length);
    QRETURN WriteI2C(QWORD address, PQBYTE data, QWORD length);
    QRETURN CachedWriteI2C(QWORD address, PQBYTE data, QWORD length);

    /////////////////
    // Storage API //
    /////////////////
    QRETURN ReadStorage(QWORD address, PQBYTE data, QWORD bytes);
    QRETURN WriteStorage(QWORD address, PQBYTE data, QWORD bytes);

    ////////////////////////////////////
    // Asynchronous Bulk Transfer API //
    ////////////////////////////////////
    static QRETURN AllocateDataBuffer(PQBYTE *buffer, QULONG bytes);
    static QRETURN FreeDataBuffer(PQBYTE *buffer);
    QRETURN ReadBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
    QRETURN WriteBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
    QRETURN BulkWait(PQBULKSTREAM bulkStream, QBYTE immediate);
    QRETURN BulkAbort(PQBULKSTREAM bulkStream);
    QRETURN SetNumAsyncThreads(QWORD numThreads, QWORD concurrency);
    QRETURN GetNumAsyncThreads(PQWORD numThreads, PQWORD concurrency);

    ///////////////////
    // Streaming API //
    ///////////////////
    QRETURN ReadBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
    QRETURN WriteBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
    QRETURN ReadBulkDataStartStreamToFile(PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags);
    QRETURN WriteBulkDataStartStreamFromFile(PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags);
    QRETURN StopStream(QBYTE streamID, QBOOL immediate);
    QRETURN ProcessStream(QBYTE streamID, QULONG milliseconds);
    QRETURN PauseStream(QBYTE streamID, QBOOL immediate);
    QRETURN ResumeStream(QBYTE streamID);
    QRETURN GetStreamStatus(QBYTE streamID, PQULONG status, PQULONG error);

    /////////////////////
    // Programming API //
    /////////////////////
    QRETURN WriteFirmware(PCQCHAR fileName, QULONG options, PQPROGRESS_CALLBACK callback, PQVOID tag);
    QRETURN VerifyFirmware(PCQCHAR filename, PQPROGRESS_CALLBACK callback, PQVOID tag);

    //////////////
    // EPCS API //
    //////////////
    QRETURN IdentifyEpcs(QBYTE nSS, PQWORD epcsId, PQULONG epcsByteSize, QULONG flags);
    QRETURN ConfigureEpcs(QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
    QRETURN VerifyEpcs(QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
    QRETURN EraseEpcs(QBYTE nSS, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag);
    
    ////////////////////
    // Statistics API //
    ////////////////////
    QRETURN ResetStatistic(Stat statistic);
    QRETURN GetStatistic(Stat statistic, StatUnit unit, PQFLOAT value, QULONG flags);

    /////////////////////////////////////
    // Protected methods and variables //
    /////////////////////////////////////
protected:
    QBOOL OpenPolitely();
    void ClosePolitely();

    QULONG m_globalOpen;
    QULONG m_localOpen;
    QHANDLE m_hDevice;
    PQCHAR m_devName;
    QULONG m_lastError;
    QULONG m_lastDriverError;
};
