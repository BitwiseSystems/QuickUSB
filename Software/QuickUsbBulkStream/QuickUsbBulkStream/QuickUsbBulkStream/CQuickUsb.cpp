/*=============================================================================
 Title        : CQuickUSB.cpp
 Description  : QuickUSB Class
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
#include "CQuickUsb.h"

////////////////////////////////////
// Constructors and Deconstructor //
////////////////////////////////////
CQuickUsb::CQuickUsb() :
    m_globalOpen(0), 
    m_localOpen(0),
    m_hDevice(0),
    m_devName(0) ,
    m_lastError(QUICKUSB_ERROR_NO_ERROR) {
}



CQuickUsb::CQuickUsb(PCQCHAR devName) : 
    m_globalOpen(0), 
    m_localOpen(0),
    m_hDevice(0),
    m_devName(0),
    m_lastError(QUICKUSB_ERROR_NO_ERROR) {

#if defined(_WIN32)
    size_t size = strlen(devName) + 1;
    m_devName = new QCHAR[size];
    strcpy_s(m_devName, size, devName);
#else
    int size = strlen(devName) + 1;
    m_devName = new QCHAR[size];
    strcpy(m_devName, devName);
#endif
}



CQuickUsb::~CQuickUsb() {
    if (m_devName) {
        delete [] m_devName;
    }
}



////////////////
// Properties //
////////////////
QBOOL CQuickUsb::IsOpened() {
    return (m_globalOpen != 0) || (m_localOpen != 0);
}



PQCHAR CQuickUsb::GetName() {
    return m_devName;
}



PQCHAR CQuickUsb::GetDeviceName() {
    return m_devName;
}



void CQuickUsb::SetName(PCQCHAR name) {
    if (m_devName) {
        delete [] m_devName;
    }

#if defined(_WIN32)
    size_t size = strlen(name) + 1;
    m_devName = new QCHAR[size];
    strcpy_s(m_devName, size, name);
#else
    int size = strlen(name) + 1;
    m_devName = new QCHAR[size];
    strcpy(m_devName, name);
#endif
}



void CQuickUsb::SetDeviceName(PCQCHAR name) {
    if (m_devName) {
        delete [] m_devName;
    }

#if defined(_WIN32)
    size_t size = strlen(name) + 1;
    m_devName = new QCHAR[size];
    strcpy_s(m_devName, size, name);
#else
    int size = strlen(name) + 1;
    m_devName = new QCHAR[size];
    strcpy(m_devName, name);
#endif
}



//////////////
// Base API //
//////////////
QRETURN CQuickUsb::FindModules(PQCHAR nameList, QULONG length) {
    if (QuickUsbFindModules(nameList, length) == 0) {
        return false;
    }

    return true;
}



QRETURN CQuickUsb::Open() {
    if (QuickUsbOpen(&m_hDevice, m_devName) == 0) {
        QuickUsbGetLastError(&m_lastError);
        return false;
    }

    // This is a global open
    ++m_globalOpen;

    return true;
}



QRETURN CQuickUsb::Open(PQCHAR deviceName) {
    SetName(deviceName);
    return Open();
}



QRETURN CQuickUsb::OpenEx(QWORD flags) {
    if (QuickUsbOpenEx(&m_hDevice, m_devName, flags) == 0) {
        QuickUsbGetLastError(&m_lastError);
        return false;
    }

    // This is a global open
    ++m_globalOpen;

    return true;
}



QRETURN CQuickUsb::OpenEx(PQCHAR deviceName, QWORD flags) {
    SetName(deviceName);
    return OpenEx(flags);
}



QRETURN CQuickUsb::Close() {
    if (QuickUsbClose(m_hDevice) == 0) {
        QuickUsbGetLastError(&m_lastError);
        return false;
    }

    if (m_globalOpen > 0) {
        --m_globalOpen;
    }

    return true;
}



QRETURN CQuickUsb::GetStringDescriptor(QBYTE index, PQCHAR buffer, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbGetStringDescriptor(m_hDevice, index, buffer, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::SetTimeout(QULONG timeOut) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbSetTimeout(m_hDevice, timeOut) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::GetDriverVersion(PQWORD MajorDriverVersion, PQWORD MinorDriverVersion, PQWORD BuildDriverVersion) {
    if (QuickUsbGetDriverVersion(MajorDriverVersion, MinorDriverVersion, BuildDriverVersion) == 0) {
        return false;
    }
    return true;
}



QRETURN CQuickUsb::GetDllVersion(PQWORD MajorDllVersion, PQWORD MinorDllVersion, PQWORD BuildDllVersion) {
    if (QuickUsbGetDllVersion(MajorDllVersion, MinorDllVersion, BuildDllVersion) == 0) {
        return false;
    }
    return true;
}



QRETURN CQuickUsb::GetFirmwareVersion(PQWORD MajorFirmwareVersion, PQWORD MinorFirmwareVersion, PQWORD BuildFirmwareVersion) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbGetFirmwareVersion(m_hDevice, MajorFirmwareVersion, MinorFirmwareVersion, BuildFirmwareVersion) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QULONG CQuickUsb::GetLastError() {
    QULONG lastError;

    QuickUsbGetLastError(&lastError);
    return lastError;
}



QULONG CQuickUsb::LastError() {
    return m_lastError;
}



//////////////////
// Settings API //
//////////////////
QRETURN CQuickUsb::ReadSetting(QWORD address, PQWORD value) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadSetting(m_hDevice, address, value) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteSetting(QWORD address, QWORD value) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteSetting(m_hDevice, address, value) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ReadDefault(QWORD address, PQWORD data) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadDefault(m_hDevice, address, data) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteDefault(QWORD address, QWORD data) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteDefault(m_hDevice, address, data) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



////////////////////////////
// FPGA Configuration API //
////////////////////////////
QRETURN CQuickUsb::StartFpgaConfiguration() {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbStartFpgaConfiguration(m_hDevice) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteFpgaData(PQBYTE fpgadata, QULONG datalength) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteFpgaData(m_hDevice, fpgadata, datalength) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::IsFpgaConfigured(PQWORD isConfigured) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbIsFpgaConfigured(m_hDevice, isConfigured) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ConfigureFpga(PCQCHAR filePath) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbConfigureFpga(m_hDevice, filePath) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



//////////////
// HSPP API //
//////////////
QRETURN CQuickUsb::ReadCommand(QWORD address, PQBYTE data, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadCommand(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteCommand(QWORD address, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteCommand(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ReadData(PQBYTE data, PQULONG length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadData(m_hDevice, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteData(PQBYTE data, QULONG length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteData(m_hDevice, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteDataAsync(PQBYTE data, QULONG length, PQBYTE transaction) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteDataAsync(m_hDevice, data, length, transaction) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're performing async requests
    return true;
}



QRETURN CQuickUsb::ReadDataAsync(PQBYTE data, PQULONG length, PQBYTE transaction) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadDataAsync(m_hDevice, data, length, transaction) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're performing async requests
    return true;
}



QRETURN CQuickUsb::AsyncWait(PQULONG bytecount, QBYTE transaction, QBYTE immediate) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbAsyncWait(m_hDevice, bytecount, transaction, immediate) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // We don't know when it's safe to close, so just don't politely close
    return true;
}



//////////////
// GPIO API //
//////////////
QRETURN CQuickUsb::ReadPortDir(QWORD address, PQBYTE data) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadPortDir(m_hDevice, address, data) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WritePortDir(QWORD address, QBYTE data) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWritePortDir(m_hDevice, address, data) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ReadPort(QWORD address, PQBYTE data, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadPort(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ReadPort(QWORD address, PQBYTE value) {
    QWORD length = 1;
    
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadPort(m_hDevice, address, value, &length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WritePort(QWORD address, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWritePort(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WritePort(QWORD address, QBYTE value) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWritePort(m_hDevice, address, &value, 1) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



///////////////
// RS232 API //
///////////////
QRETURN CQuickUsb::SetRS232BaudRate(QULONG baudRate) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbSetRS232BaudRate(m_hDevice, baudRate) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::GetNumRS232(QBYTE portNum, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbGetNumRS232(m_hDevice, portNum, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::FlushRS232(QBYTE portNum) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbFlushRS232(m_hDevice, portNum) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ReadRS232(QBYTE portNum, PQBYTE data, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadRS232(m_hDevice, portNum, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteRS232(QBYTE portNum, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteRS232(m_hDevice, portNum, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



/////////////
// SPI API //
/////////////
QRETURN CQuickUsb::ReadSpi(QBYTE portNum, PQBYTE data, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadSpi(m_hDevice, portNum, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteSpi(QBYTE portNum, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteSpi(m_hDevice, portNum, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteReadSpi(QBYTE portNum, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteReadSpi(m_hDevice, portNum, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



/////////////
// I2C API //
/////////////
QRETURN CQuickUsb::ReadI2C(QWORD address, PQBYTE data, PQWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadI2C(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteI2C(QWORD address, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteI2C(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::CachedWriteI2C(QWORD address, PQBYTE data, QWORD length) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbCachedWriteI2C(m_hDevice, address, data, length) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



/////////////////
// Storage API //
/////////////////
QRETURN CQuickUsb::ReadStorage(QWORD address, PQBYTE data, QWORD bytes) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadStorage(m_hDevice, address, data, bytes) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::WriteStorage(QWORD address, PQBYTE data, QWORD bytes) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteStorage(m_hDevice, address, data, bytes) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



////////////////////////////////////
// Asynchronous Bulk Transfer API //
////////////////////////////////////
QRETURN CQuickUsb::ReadBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID  tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadBulkDataAsync(m_hDevice, buffer, bytes, bulkStream, cRoutine, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're performing async requests
    return true;
}



QRETURN CQuickUsb::WriteBulkDataAsync(PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID  tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteBulkDataAsync(m_hDevice, buffer, bytes, bulkStream, cRoutine, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're performing async requests
    return true;
}



QRETURN CQuickUsb::BulkWait(PQBULKSTREAM bulkStream, QBYTE immediate) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbBulkWait(m_hDevice, bulkStream, immediate) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // We're done performing the async request so we can politely close the module again
    ClosePolitely();
    return true;
}

QRETURN CQuickUsb::SetNumAsyncThreads(QWORD numThreads, QWORD concurrency) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbSetNumAsyncThreads(m_hDevice, numThreads, concurrency) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}

QRETURN CQuickUsb::GetNumAsyncThreads(PQWORD numThreads, PQWORD concurrency) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbGetNumAsyncThreads(m_hDevice, numThreads, concurrency) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



///////////////////
// Streaming API //
///////////////////
QRETURN CQuickUsb::ReadBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbReadBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, streamID, numThreads, concurrency) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're streaming
    return true;
}



QRETURN CQuickUsb::WriteBulkDataStartStream(PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, streamID, numThreads, concurrency) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module now that we're streaming
    return true;
}



QRETURN CQuickUsb::StopStream(QBYTE streamID, QBOOL wait) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbStopStream(m_hDevice, streamID, wait) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // We're done streaming so we can politely close the module again
    ClosePolitely();
    return true;
}

QRETURN CQuickUsb::ProcessStream(QBYTE streamID, QULONG milliseconds) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbProcessStream(m_hDevice, streamID, milliseconds) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module because we're still streaming
    return true;
}

QRETURN CQuickUsb::PauseStream(QBYTE streamID, QBOOL immediate) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbPauseStream(m_hDevice, streamID, immediate) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module because we're still streaming
    return true;
}

QRETURN CQuickUsb::ResumeStream(QBYTE streamID) {
    // Don't politely open as we must already be open for this call

    if (QuickUsbResumeStream(m_hDevice, streamID) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    // Don't politely close module because we're still streaming
    return true;
}



/////////////////////
// Programming API //
/////////////////////
QRETURN CQuickUsb::WriteFirmware(PCQCHAR fileName, QULONG options, PQPROGRESS_CALLBACK callback, PQVOID tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbWriteFirmware(m_hDevice, fileName, options, callback, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::VerifyFirmware(PCQCHAR filename, PQPROGRESS_CALLBACK callback, PQVOID tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbVerifyFirmware(m_hDevice, filename, callback, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



//////////////
// EPCS API //
//////////////
QRETURN CQuickUsb::IdentifyEpcs(QBYTE nSS, PQWORD epcsId, PQULONG epcsByteSize, QULONG flags) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbIdentifyEpcs(m_hDevice, nSS, epcsId, epcsByteSize, flags) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::ConfigureEpcs(QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbConfigureEpcs(m_hDevice, nSS, filePath, flags, callback, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::VerifyEpcs(QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbVerifyEpcs(m_hDevice, nSS, filePath, flags, callback, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



QRETURN CQuickUsb::EraseEpcs(QBYTE nSS, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
    if (!OpenPolitely()) {
        return false;
    }

    if (QuickUsbEraseEpcs(m_hDevice, nSS, flags, callback, tag) == 0) {
        QuickUsbGetLastError(&m_lastError);
        ClosePolitely();
        return false;
    }

    ClosePolitely();
    return true;
}



/////////////////////////////////////
// Protected methods and variables //
/////////////////////////////////////
QBOOL CQuickUsb::OpenPolitely() {
    // Disregard if already globally opened
    if (m_globalOpen != 0) {
        return true;
    }

    // This method of reference counting is extremely important.  If nested calls
    // to this function are made we need to make sure we know when to close
    // the QuickUSB (of course, only if locally opened).
    ++m_localOpen;

    // Open the QuickUSB is it isn't already opened
    if (m_localOpen == 1) {
        if (QuickUsbOpen(&m_hDevice, m_devName) == 0) {
            --m_localOpen;
            QuickUsbGetLastError(&m_lastError);
            return false;
        }
    }
    return true;
}



void CQuickUsb::ClosePolitely() {
    // Close the QuickUSB if we opened it locally
    if (m_localOpen > 0) {
        // Only close the module when our reference count indicates this is the
        // last ClosePolitely call.
        if (m_localOpen == 1) {
            // We don't want lastError to be overwritten, so ignore any errors
            QuickUsbClose(m_hDevice);
        }

        // We must decrement our reference count after the call to Close
        --m_localOpen;
    }
}
