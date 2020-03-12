/*=============================================================================
 Title        : QuickUSB.h
 Description  : QuickUSB QUSB2 Module API
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
#ifndef __QUICKUSB_LIBRARY_HEADER__
#define __QUICKUSB_LIBRARY_HEADER__

#if defined(_WIN32)
    #include <windows.h>
    #define IMPLEMENT_ASYNC 1
#endif

#if defined(__linux__)
    #include <linux/version.h>
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25))
        #define IMPLEMENT_ASYNC 
        #include <linux/aio_abi.h>
    #endif
#endif

#if defined(__APPLE__)
    // IOKit headers
    #include <CoreFoundation/CoreFoundation.h>
    #include <IOKit/IOCFPlugIn.h>
    #include <IOKit/usb/IOUSBLib.h>
    #include <IOKit/usb/USB.h>
    #include <queue>
    #define IMPLEMENT_ASYNC 1
#endif



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Constants
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// QuickUSB String Descriptor Indices
#define QUICKUSB_MAKE                           1
#define QUICKUSB_MODEL                          2
#define QUICKUSB_SERIAL                         3

// QuickUSB Port Addresses
#define PORT_A                                  0
#define PORT_B                                  1
#define PORT_C                                  2
#define PORT_D                                  3
#define PORT_E                                  4

// QuickUSB Settings
#define SETTING_EP26CONFIG                      0
#define SETTING_WORDWIDE                        1
#define SETTING_DATAADDRESS                     2
#define SETTING_FIFO_CONFIG                     3
#define SETTING_FPGATYPE                        4
#define SETTING_CPUCONFIG                       5
#define SETTING_SPICONFIG                       6
#define SETTING_SLAVEFIFOFLAGS                  7
#define SETTING_I2CTL                           8
#define SETTING_PORTA                           9
#define SETTING_PORTB                           10
#define SETTING_PORTC                           11
#define SETTING_PORTD                           12
#define SETTING_PORTE                           13
#define SETTING_PORTACCFG                       14
#define SETTING_PINFLAGS                        15
#define SETTING_VERSIONBUILD                    16
#define SETTING_VERSIONSPEED                    17
#define SETTING_TIMEOUT_HIGH                    18
#define SETTING_TIMEOUT_LOW                     19

// FPGA Configuration Type Settings
#define FPGATYPE_ALTERAPS                       0
#define FPGATYPE_XILINXSS                       1

// QuickUSB Error Codes
#define QUICKUSB_ERROR_NO_ERROR                 0
#define QUICKUSB_ERROR_OUT_OF_MEMORY            1
#define QUICKUSB_ERROR_CANNOT_OPEN_MODULE       2
#define QUICKUSB_ERROR_CANNOT_FIND_DEVICE       3
#define QUICKUSB_ERROR_IOCTL_FAILED             4
#define QUICKUSB_ERROR_INVALID_PARAMETER        5
#define QUICKUSB_ERROR_TIMEOUT                  6
#define QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED   7
#define QUICKUSB_ERROR_I2C_BUS_ERROR            8
#define QUICKUSB_ERROR_I2C_NO_ACK               9
#define QUICKUSB_ERROR_I2C_SLAVE_WAIT           10
#define QUICKUSB_ERROR_I2C_TIMEOUT              11
#define QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE      12
#define QUICKUSB_ERROR_ALREADY_OPENED           13
#define QUICKUSB_ERROR_CANNOT_CLOSE_MODULE      14
#define QUICKUSB_ERROR_FPGA_INIT_FAILED         15
#define QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_512  16
#define QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_64   17
#define QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR     18
#define QUICKUSB_ERROR_ABORTED                  19
#define QUICKUSB_ERROR_DEPRECATED               20
#define QUICKUSB_ERROR_INVALID_SERIAL           21
#define QUICKUSB_ERROR_CANNOT_OPEN_FILE         22
#define QUICKUSB_ERROR_VERIFY_FAILED            23
#define QUICKUSB_ERROR_FIRMWARE_ERROR           24
#define QUICKUSB_ERROR_ALREADY_COMPLETED        25
#define QUICKUSB_ERROR_NOT_COMPLETED            26
#define QUICKUSB_ERROR_FPGA_CONFIG_FAILED       27
#define QUICKUSB_ERROR_INVALID_OPERATION        28
#define QUICKUSB_ERROR_TOO_MANY_REQUESTS        29
#define QUICKUSB_ERROR_EPCS_NOT_FOUND           30
#define QUICKUSB_ERROR_EPCS_TOO_SMALL           31
#define QUICKUSB_ERROR_NOT_STREAMING            32
#define QUICKUSB_ERROR_BUFFER_NOT_ALIGNED       33
#define QUICKUSB_ERROR_INTERNAL_ERROR           34
#define QUICKUSB_ERROR_DEVICE_IS_CLOSING        35
#define QUICKUSB_ERROR_PROTECTION               36
#define QUICKUSB_ERROR_NEED_DATA                37
#define QUICKUSB_ERROR_FILE_NOT_FOUND           38
#define QUICKUSB_ERROR_FILE_ALREADY_EXISTS      39
#define QUICKUSB_ERROR_FILE_RW                  40
#define QUICKUSB_ERROR_FILE_EOF                 41

#define QUICKUSB_ERROR_DRIVER                   100
#define QUICKUSB_ERROR_DRIVER_URB               101
#define QUICKUSB_ERROR_DRIVER_BULK              102
#define QUICKUSB_ERROR_DRIVER_ASYNC             103
#define QUICKUSB_ERROR_DRIVER_PIPE              104
#define QUICKUSB_ERROR_DRIVER_BUSY              105

// QuickUSB Open Flags 
#define QUICKUSB_OPEN_NORMAL                    0x0000
#define QUICKUSB_OPEN_IF_CLOSED                 0x0001

// QuickUSB Programming API Flags
#define QUICKUSB_PROGRAM_SERIAL                 0x10000000
#define QUICKUSB_PRESERVE_CUSTOMIZATION         0x00010000
#define QUICKUSB_PRESERVE_DEFAULTS              0x00020000
#define QUICKUSB_PRESERVE_GPIF                  0x00040000
#define QUICKUSB_PRESERVE_SERIAL                0x00080000

// QuickUSB EPCS IDs
#define QUICKUSB_EPCS_ID_UNKNOWN                0x00
#define QUICKUSB_EPCS1_ID                       0x10
#define QUICKUSB_EPCS4_ID                       0x12
#define QUICKUSB_EPCS16_ID                      0x14
#define QUICKUSB_EPCS64_ID                      0x16

// QuickUSB EPCS Capacities (in bytes)
#define QUICKUSB_EPCS1_SIZE                     (1048576 / 8)
#define QUICKUSB_EPCS4_SIZE                     (QUICKUSB_EPCS1_SIZE * 4)
#define QUICKUSB_EPCS16_SIZE                    (QUICKUSB_EPCS1_SIZE * 16)
#define QUICKUSB_EPCS64_SIZE                    (QUICKUSB_EPCS1_SIZE * 64)

// EPCS flags
#define QUICKUSB_EPCS_IGNORE_NCE                0x0001
#define QUICKUSB_EPCS_SKIP_ERASE                0x0002

// Data flags
#define QUICKUSB_OUT_OF_ORDER                   0x0001

// Statistics
#define QUICKUSB_STAT_ALL                   0x0000
#define QUICKUSB_STAT_READ_THROUGHPUT       0x0001
#define QUICKUSB_STAT_WRITE_THROUGHPUT      0x0002
#define QUICKUSB_STAT_TOTAL_THROUGHPUT      0x0003
#define QUICKUSB_STAT_TOTAL_DATA_READ       0x0004
#define QUICKUSB_STAT_TOTAL_DATA_WRITTEN    0x0005

// Statistic units
#define QUICKUSB_STAT_UNIT_BYTES    0x0001
#define QUICKUSB_STAT_UNIT_KB       0x0002
#define QUICKUSB_STAT_UNIT_MB       0x0004
#define QUICKUSB_STAT_UNIT_GB       0x0008

#define QUICKUSB_STAT_UNIT_PER_NS   0x0010
#define QUICKUSB_STAT_UNIT_PER_US   0x0020
#define QUICKUSB_STAT_UNIT_PER_MS   0x0040
#define QUICKUSB_STAT_UNIT_PER_SEC  0x0080
#define QUICKUSB_STAT_UNIT_PER_MIN  0x0100
#define QUICKUSB_STAT_UNIT_PER_HOUR 0x0200

#define QUICKUSB_STAT_UNIT_BYTES_PER_SEC    (QUICKUSB_STAT_UNIT_BYTES   | QUICKUSB_STAT_UNIT_PER_SEC)
#define QUICKUSB_STAT_UNIT_KB_PER_SEC       (QUICKUSB_STAT_UNIT_KB      | QUICKUSB_STAT_UNIT_PER_SEC)
#define QUICKUSB_STAT_UNIT_MB_PER_SEC       (QUICKUSB_STAT_UNIT_MB      | QUICKUSB_STAT_UNIT_PER_SEC)

// Statistics flags
#define QUICKUSB_STAT_FLAGS_RESET   0x0001

// Streaming Status
#define QUICKUSB_STREAM_STATUS_UNKNOWN  0x0000
#define QUICKUSB_STREAM_STATUS_RUNNING  0x0001
#define QUICKUSB_STREAM_STATUS_STOPPED  0x0002
#define QUICKUSB_STREAM_STATUS_PAUSED   0x0003



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB Type Definitions
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define QUSBLIB               extern

#define QBOOL unsigned char
#define QBYTE unsigned char
#define QCHAR char
#define QWORD unsigned short
#define QFLOAT float
#define QSTREAMTYPE QWORD
#define QCOMPLETIONTYPE QBYTE

#if defined(_WIN32)
    #define QLONG signed long
    #define QULONG unsigned long
    #define QRESULT QLONG
    #define QHANDLE HANDLE
    #define QDEVHANDLE HANDLE
    #define QFILEHANDLE HANDLE
    #define QRETURN QRESULT WINAPI
    #define QVOIDRETURN void WINAPI
    #define QUICKUSB_INFINITE_TIME INFINITE
#endif

#if defined(__linux__)
    #define QLONG signed long
    #define QULONG unsigned long
    #define QRESULT QLONG
    #define QHANDLE signed short
    #define QDEVHANDLE signed short
    #define QFILEHANDLE FILE*
    #define QRETURN QRESULT
    #define QVOIDRETURN void
    #define QUICKUSB_INFINITE_TIME (~0)
#endif

#if defined(__APPLE__)
    #define QLONG int32_t
    #define QULONG UInt32
    #define QRESULT QLONG
    #define QHANDLE QLONG
    #define QDEVHANDLE IOUSBInterfaceInterface245 **
    #define QFILEHANDLE FILE*
    #define QRETURN QRESULT
    #define QVOIDRETURN void
    #define QUICKUSB_INFINITE_TIME (~0)
#endif

#define PQHANDLE QHANDLE*
#define PQDEVHANDLE QDEVHANDLE*
#define PQFILEHANDLE QFILEHANDLE*
#define PQVOID void*
#define PQBOOL QBOOL*
#define PQBYTE QBYTE*
#define PQCHAR QCHAR*
#define PCQCHAR const QCHAR*
#define PQWORD QWORD*
#define PQLONG QLONG*
#define PQULONG QULONG*
#define PQFLOAT QFLOAT*
#define PQSTREAMTYPE QSTREAMTYPE*
#define PQCOMPLETIONTYPE QCOMPLETIONTYPE*



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB Structure Definitions
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// Asynchronous Bulk Transfer Structure
// Packed to be x86 and x64 compatible
typedef struct QBULKSTREAM {
#ifdef IMPLEMENT_ASYNC
#if defined(_WIN32)
    OVERLAPPED      InternalData;       // 20 Bytes (x86) or 32 Bytes(x64)
#endif
#if defined(__linux__)
    struct iocb     InternalData;       // 64 Bytes (x86 and x64)
#endif
#if defined(__APPLE__)
    PQVOID          InternalData;       // 4 Bytes (x86) or 8 Bytes (x64)
#endif
    QHANDLE         Handle;             // Varies by platform
    PQBYTE          Buffer;             // 4 Bytes (x86) or 8 Bytes (x64)
    PQVOID          CompletionRoutine;  // 4 Bytes (x86) or 8 Bytes (x64)
    PQVOID          Tag;                // 4 Bytes (x86) or 8 Bytes (x64)
    PQVOID          InternalReserved;   // 4 Bytes (x86) or 8 Bytes (x64)

    QULONG          BytesRequested;     // 4 Bytes
    QULONG          BytesTransferred;   // 4 Bytes
    QULONG          Error;              // 4 Bytes
    QULONG          InternalStatus;     // 4 Bytes

    QULONG          StreamID;           // 4 Bytes
    QULONG          RequestID;          // 4 Bytes
    QULONG          Reserved;           // 4 Bytes

    QSTREAMTYPE     StreamType;         // 2 Bytes
    QBYTE           StructSize;         // 1 Byte
    QCOMPLETIONTYPE CompletionType;     // 1 Byte
#endif
} QBULKSTREAM, *PQBULKSTREAM;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB Callback and Completion Routine Definitions
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#if defined(_WIN32)
    typedef void (WINAPI *PQBULKSTREAM_COMPLETION_ROUTINE)(PQBULKSTREAM BulkStream);
    typedef void (WINAPI *PQPROGRESS_CALLBACK)(QHANDLE hDevice, QWORD percentComplete, PQVOID tag);
#else
    typedef void (*PQBULKSTREAM_COMPLETION_ROUTINE)(PQBULKSTREAM BulkStream);
    typedef void (*PQPROGRESS_CALLBACK)(QHANDLE hDevice, QWORD percentComplete, PQVOID tag);
#endif



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB API Function Declarations
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef __cplusplus
    extern "C" {
#endif

QUSBLIB QRETURN QuickUsbGetLastError(
    PQULONG error
);

QUSBLIB QRETURN QuickUsbGetLastDriverError(
    PQULONG error
);

QUSBLIB QRETURN QuickUsbFindModules(
    PQCHAR nameList,
    QULONG length);

QUSBLIB QRETURN QuickUsbOpen(
    PQHANDLE hDevice, 
    PQCHAR   deviceName
);

QUSBLIB QRETURN QuickUsbOpenEx(
    PQHANDLE hDevice, 
    PQCHAR   deviceName, 
    QWORD    flags
);

QUSBLIB QRETURN QuickUsbClose(
    QHANDLE hDevice
);

QUSBLIB QRETURN QuickUsbGetStringDescriptor(
    QHANDLE hDevice, 
    QBYTE   index,
    PQCHAR  buffer, 
    QWORD   length
);

QUSBLIB QRETURN QuickUsbReadSetting(
    QHANDLE hDevice, 
    QWORD   address, 
    PQWORD  value
);

QUSBLIB QRETURN QuickUsbWriteSetting(
    QHANDLE hDevice,
    QWORD   address, 
    QWORD   value
);

QUSBLIB QRETURN QuickUsbStartFpgaConfiguration(
    QHANDLE hDevice
);

QUSBLIB QRETURN QuickUsbWriteFpgaData(
    QHANDLE hDevice, 
    PQBYTE  fpgadata, 
    QULONG  datalength
);

QUSBLIB QRETURN QuickUsbIsFpgaConfigured(
    QHANDLE hDevice, 
    PQWORD  isConfigured
);

QUSBLIB QRETURN QuickUsbConfigureFpga(
    QHANDLE hDevice, 
    PCQCHAR filePath
);

QUSBLIB QRETURN QuickUsbReadCommand(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbWriteCommand(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    QWORD   length
);

QUSBLIB QRETURN QuickUsbReadData(
    QHANDLE hDevice, 
    PQBYTE  data, 
    PQULONG length
);

QUSBLIB QRETURN QuickUsbReadDataEx(
    QHANDLE hDevice, 
    PQBYTE  data, 
    PQULONG length,
    QULONG  flags
);

QUSBLIB QRETURN QuickUsbWriteData(
    QHANDLE hDevice, 
    PQBYTE  data, 
    QULONG  length
);

QUSBLIB QRETURN QuickUsbWriteDataEx(
    QHANDLE hDevice, 
    PQBYTE  data, 
    PQULONG length,
    QULONG  flags
);

QUSBLIB QRETURN QuickUsbReadPortDir(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data
);

QUSBLIB QRETURN QuickUsbWritePortDir(
    QHANDLE hDevice, 
    QWORD   address, 
    QBYTE   data
);

QUSBLIB QRETURN QuickUsbReadPort(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbWritePort(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    QWORD  length
);

QUSBLIB QRETURN QuickUsbSetRS232BaudRate(
    QHANDLE hDevice, 
    QULONG  baudRate
);

QUSBLIB QRETURN QuickUsbGetNumRS232(
    QHANDLE hDevice, 
    QBYTE   portNum, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbFlushRS232(
    QHANDLE hDevice, 
    QBYTE   portNum
);

QUSBLIB QRETURN QuickUsbReadRS232(
    QHANDLE hDevice, 
    QBYTE   portNum, 
    PQBYTE  data, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbWriteRS232(
    QHANDLE hDevice, 
    QBYTE   portNum, 
    PQBYTE  data, 
    QWORD   length
);

QUSBLIB QRETURN QuickUsbReadSpi(
    QHANDLE hDevice, 
    QBYTE   portNum,
    PQBYTE  data, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbWriteSpi(
    QHANDLE hDevice, 
    QBYTE   portNum,
    PQBYTE  data, 
    QWORD   length
);

QUSBLIB QRETURN QuickUsbWriteReadSpi(
    QHANDLE hDevice, 
    QBYTE   portNum, 
    PQBYTE  data, 
    QWORD   length
);

QUSBLIB QRETURN QuickUsbReadI2C(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    PQWORD  length
);

QUSBLIB QRETURN QuickUsbWriteI2C(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data,
    QWORD   length
);

QUSBLIB QRETURN QuickUsbCachedWriteI2C(
    QHANDLE hDevice,
    QWORD   address,
    PQBYTE  data,
    QWORD   length
);

QUSBLIB QRETURN QuickUsbSetTimeout(
    QHANDLE hDevice,
    QULONG  timeOut
);

QUSBLIB QRETURN QuickUsbGetDriverVersion(
    PQWORD MajorDriverVersion,
    PQWORD MinorDriverVersion,
    PQWORD BuildDriverVersion
);

QUSBLIB QRETURN QuickUsbGetDllVersion(
    PQWORD MajorDllVersion,
    PQWORD MinorDllVersion,
    PQWORD BuildDllVersion
);

QUSBLIB QRETURN QuickUsbGetFirmwareVersion(
    QHANDLE hDevice, 
    PQWORD  MajorFirmwareVersion,
    PQWORD  MinorFirmwareVersion,
    PQWORD  BuildFirmwareVersion
);

QUSBLIB QRETURN QuickUsbWriteDataAsync(
    QHANDLE hDevice, 
    PQBYTE  data, 
    QULONG  length, 
    PQBYTE  transaction
);

QUSBLIB QRETURN QuickUsbReadDataAsync(
    QHANDLE hDevice, 
    PQBYTE  data, 
    PQULONG length, 
    PQBYTE  transaction
);

QUSBLIB QRETURN QuickUsbAsyncWait(
    QHANDLE hDevice, 
    PQULONG bytecount,
    QBYTE   transaction,
    QBYTE   immediate
);

QUSBLIB QRETURN QuickUsbReadDefault(
    QHANDLE hDevice,
    QWORD   address, 
    PQWORD  data
);

QUSBLIB QRETURN QuickUsbWriteDefault(
    QHANDLE hDevice,
    QWORD   address, 
    QWORD   data
);

QUSBLIB QRETURN QuickUsbReadStorage(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data, 
    QWORD   bytes
);

QUSBLIB QRETURN QuickUsbWriteStorage(
    QHANDLE hDevice, 
    QWORD   address, 
    PQBYTE  data,
    QWORD   bytes
);

// Asynchronous Bulk Transfer API
QUSBLIB QRETURN QuickUsbAllocateDataBuffer(
    PQBYTE *buffer, 
    QULONG bytes
);

QUSBLIB QRETURN QuickUsbFreeDataBuffer(
    PQBYTE *buffer
);

QUSBLIB QRETURN QuickUsbReadBulkDataAsync(
    QHANDLE hDevice,
    PQBYTE  buffer,
    QULONG  bytes,
    PQBULKSTREAM bulkStream,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine,
    PQVOID  tag
);
        
QUSBLIB QRETURN QuickUsbWriteBulkDataAsync(
    QHANDLE hDevice,
    PQBYTE  buffer,
    QULONG  bytes,
    PQBULKSTREAM bulkStream,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine,
    PQVOID  tag
);

QUSBLIB QRETURN QuickUsbBulkWait(
    QHANDLE      hDevice,
    PQBULKSTREAM bulkStream,
    QBYTE        immediate
);

QUSBLIB QRETURN QuickUsbSetNumAsyncThreads(
    QHANDLE hDevice,
    QWORD  numThreads,
    QWORD  concurrency
);

QUSBLIB QRETURN QuickUsbGetNumAsyncThreads(
    QHANDLE hDevice,
    PQWORD numThreads,
    PQWORD concurrency
);

// Streaming API
QUSBLIB QRETURN QuickUsbReadBulkDataStartStream(
    QHANDLE hDevice,
    PQBYTE  buffers[],
    QULONG  numBuffers,
    QULONG  bufferSize,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine,
    PQVOID  tag,
    PQBYTE  streamID,
    QWORD   numThreads,
    QWORD   concurrency
);

QUSBLIB QRETURN QuickUsbWriteBulkDataStartStream(
    QHANDLE hDevice,
    PQBYTE  buffers[],
    QULONG  numBuffers,
    QULONG  bufferSize,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine,
    PQVOID  tag,
    PQBYTE  streamID,
    QWORD   numThreads,
    QWORD   concurrency
);

QUSBLIB QRETURN QuickUsbReadBulkDataStartStreamToFile(
    QHANDLE hDevice, 
    PCQCHAR path, 
    QULONG  numBuffers, 
    QULONG  bufferSize, 
    QULONG  maxTransfers,
    PQBYTE  streamID, 
    QULONG  flags
);

QUSBLIB QRETURN QuickUsbWriteBulkDataStartStreamFromFile(
    QHANDLE hDevice, 
    PCQCHAR path, 
    QULONG  numBuffers, 
    QULONG  bufferSize, 
    QULONG  maxTransfers,
    PQBYTE  streamID, 
    QULONG  flags
);

QUSBLIB QRETURN QuickUsbStopStream(
    QHANDLE hDevice,
    QBYTE   streamID,
    QBOOL   immediate
);

QUSBLIB QRETURN QuickUsbProcessStream(
    QHANDLE hDevice, 
    QBYTE   streamID,
    QULONG  milliseconds
);

QUSBLIB QRETURN QuickUsbPauseStream(
    QHANDLE hDevice,
    QBYTE   streamID,
    QBOOL   immediate
);

QUSBLIB QRETURN QuickUsbResumeStream(
    QHANDLE hDevice, 
    QBYTE   streamID
);

QUSBLIB QRETURN QuickUsbGetStreamStatus(
    QHANDLE hDevice, 
    QBYTE streamID, 
    PQULONG status, 
    PQULONG error
);



// Firmware Programming API
QUSBLIB QRETURN QuickUsbWriteFirmware(
    QHANDLE hDevice, 
    PCQCHAR fileName, 
    QULONG options, 
    PQPROGRESS_CALLBACK callback,
    PQVOID tag
);

QUSBLIB QRETURN QuickUsbVerifyFirmware(
    QHANDLE hDevice, 
    PCQCHAR filename, 
    PQPROGRESS_CALLBACK callback,
    PQVOID tag
);

// EPCS Programming API
QUSBLIB QRETURN QuickUsbIdentifyEpcs(
    QHANDLE hDevice, 
    QBYTE nSS, 
    PQWORD epcsId, 
    PQULONG ecpsByteSize,
    QULONG flags
);

QUSBLIB QRETURN QuickUsbConfigureEpcs(
    QHANDLE hDevice,
    QBYTE nSS, 
    PCQCHAR filePath,
    QULONG flags, 
    PQPROGRESS_CALLBACK callback,
    PQVOID tag
);

QUSBLIB QRETURN QuickUsbVerifyEpcs(
    QHANDLE hDevice, 
    QBYTE nSS, 
    PCQCHAR filePath,
    QULONG flags, 
    PQPROGRESS_CALLBACK callback,
    PQVOID tag
);

QUSBLIB QRETURN QuickUsbEraseEpcs(
    QHANDLE hDevice, 
    QBYTE nSS,
    QULONG flags, 
    PQPROGRESS_CALLBACK callback,
    PQVOID tag
);

// Statistics API
QUSBLIB QRETURN QuickUsbResetStatistic(
    QHANDLE hDevice, 
    QULONG statistic
);

QUSBLIB QRETURN QuickUsbGetStatistic(
    QHANDLE hDevice, 
    QULONG statistic, 
    QULONG unit, 
    PQFLOAT value, 
    QULONG flags
);



#ifdef __cplusplus
    }
#endif

#endif // __QUICKUSB_LIBRARY_HEADER__
