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

    //////////////
    // Base API //
    //////////////
    static QRETURN FindModules(PQCHAR nameList, QULONG length);
    static QRETURN GetDriverVersion(PQWORD MajorDriverVersion, PQWORD MinorDriverVersion, PQWORD BuildDriverVersion);
    static QRETURN GetDllVersion(PQWORD MajorDllVersion, PQWORD MinorDllVersion, PQWORD BuildDllVersion);
    static QULONG GetLastError();

    QRETURN Open();
    QRETURN OpenEx(PQCHAR deviceName, QWORD flags);
    QRETURN Close();
    QRETURN GetStringDescriptor(QBYTE index, PQCHAR buffer, QWORD length);
    QRETURN SetTimeout(QULONG timeOut);
    QRETURN GetFirmwareVersion(PQWORD MajorFirmwareVersion, PQWORD MinorFirmwareVersion, PQWORD BuildFirmwareVersion);
    QULONG  LastError();

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
    QRETURN WriteData(PQBYTE data, QULONG length);
    QRETURN WriteDataAsync(PQBYTE data, QULONG length, PQBYTE transaction);
    QRETURN ReadDataAsync(PQBYTE data, PQULONG length, PQBYTE transaction);
    QRETURN AsyncWait(PQULONG bytecount, QBYTE transaction, QBYTE immediate);

    //////////////
    // GPIO API //
    //////////////
    QRETURN ReadPortDir(QWORD address, PQBYTE data);
    QRETURN WritePortDir(QWORD address, QBYTE data);
    QRETURN ReadPort(QWORD address, PQBYTE data, PQWORD length);
    QRETURN WritePort(QWORD address, PQBYTE data, QWORD length);

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
    QRETURN ReadBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
    QRETURN WriteBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag);
    QRETURN BulkWait(PQBULKSTREAM bulkStream, QBYTE immediate);
    QRETURN SetNumAsyncThreads(QWORD numThreads, QWORD concurrency);
    QRETURN GetNumAsyncThreads(PQWORD numThreads, PQWORD concurrency);

    ///////////////////
    // Streaming API //
    ///////////////////
    QRETURN ReadBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
    QRETURN WriteBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency);
    QRETURN StopStream(QBYTE streamID, QBOOL wait);
    QRETURN ProcessStream(QBYTE streamID, QULONG milliseconds);
    QRETURN PauseStream(QBYTE streamID, QBOOL immediate);
    QRETURN ResumeStream(QBYTE streamID);

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
};
