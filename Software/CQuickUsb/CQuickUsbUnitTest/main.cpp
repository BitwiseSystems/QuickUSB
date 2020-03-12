/*=============================================================================
 Title        : QuickUSB C++ Interface Unittest Program
 Description  : 
 Notes        : 
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
#include <iostream>
#include "../CQuickUsb.h"
#include <ctime>
using namespace std;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#if defined(__linux__)
    #define SleepMS(x) usleep((x) * 1000)
    #define FIRMWARE_FILE_PATH "/Projects/Bitwise/QuickUSB/Library/Firmware/quickusb-simple.qusb"
#endif
#if defined(__APPLE__)
    #define SleepMS(x) usleep((x) * 1000)
    #define FIRMWARE_FILE_PATH "/Projects/Bitwise/QuickUSB/Library/Firmware/quickusb-simple.qusb"
#endif
#if defined(_WIN32)
    #include <windows.h>
    #define SleepMS(x) SleepMS(x)
    #define FIRMWARE_FILE_PATH "C:\\Projects\\Bitwise\\QuickUSB\\Library\\Firmware\\quickusb-simple.qusb"
#endif
#define EPCS_FILE_PATH "epcs.bin"
#define FPGA_FILE_PATH "fpga.rbf"

// The unit tests in this file are not intended to exercise the QuickUSB API, but
// test that the C++ Class wrapper properly wraps every QuickUSB API call.

QVOIDRETURN progress(QHANDLE hDevice, QWORD percent, PQVOID tag) {
    cout << percent << "%\t\r";
    cout.flush();
}

QVOIDRETURN StreamCR(PQBULKSTREAM bs) {
    if (!bs) {
        cout << "Stream error: " << CQuickUsb::GetLastError() << endl;
        return;
    }
	if (bs->Error || (bs->BytesRequested != bs->BytesTransferred)) {
        cout << "*** Stream request #" << bs->RequestID << " failed with error: " << bs->Error << endl;
    }// else {
    //    cout << "Stream request ok" << endl;
    //}
}

int main(int argc, char *argv[]) {
    QCHAR nameList[2048];
    QCHAR str[128];
    QBYTE *buffer, *buffer1;
    QULONG length;
    QWORD major, minor, rev, conf, value;
    CQuickUsb *qusb;
    QBULKSTREAM bs, bs1;
    QBYTE byte, transId;
    QWORD id;
    QULONG size;
    PQBYTE bufArray[2];
    QBYTE streamID;
    QFLOAT stat;
    QWORD numThreads, concurrency;

    // Test QuickUsbAllocateDataBuffer
    if (!CQuickUsb::AllocateDataBuffer(&buffer, 1024)) {
        cout << "*** AllocateDataBuffer request failed with error: " << bs.Error << endl;
    } else {
        cout << "AllocateDataBuffer() OK" << endl;
    }

    // Test QuickUsbAllocateDataBuffer
    if (!CQuickUsb::AllocateDataBuffer(&buffer1, 1024)) {
        cout << "*** AllocateDataBuffer request failed with error: " << bs.Error << endl;
    } else {
        cout << "AllocateDataBuffer() OK" << endl;
    }

    // Test FindModules
    if (!CQuickUsb::FindModules(nameList, 2048)) {
        cout << "*** FindModules() failed with error: " << CQuickUsb::GetLastError() << endl;
        return 1;
    } else {
        cout << "FindModules() OK" << endl;
    }

    // Test GetLastError
    if (CQuickUsb::GetLastError()) {
        cout << "*** GetLastError() failed with error: " << CQuickUsb::GetLastError() << endl;
    } else {
        cout << "GetLastError() OK" << endl;
    }

    // Test GetLastDriverError
    if (CQuickUsb::GetLastDriverError()) {
        cout << "*** GetLastDriverError() failed with error: " << CQuickUsb::GetLastDriverError() << endl;
    } else {
        cout << "GetLastDriverError() OK" << endl;
    }

    qusb = new CQuickUsb(nameList);

    // Test Open
    if (!qusb->Open()) {
        cout << "*** Open() failed with error: " << CQuickUsb::GetLastError() << endl;
    } else {
        cout << "Open() OK" << endl;
    }

    // Test Close
    if (!qusb->Close()) {
        cout << "*** Close() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "Close() OK" << endl;
    }

    // Test OpenEx
    if (!qusb->OpenEx(nameList, 0)) {
        cout << "*** OpenEx() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "OpenEx() OK" << endl;
    }

    // Test LastError
    if (qusb->LastError()) {
        cout << "*** LastError() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "LastError() OK" << endl;
    }

    // Test LastDriverError
    if (qusb->LastDriverError()) {
        cout << "*** LastDriverError() failed with error: " << qusb->LastDriverError() << endl;
    } else {
        cout << "LastDriverError() OK" << endl;
    }

    // Test GetStringDescriptor
    if (!qusb->GetStringDescriptor(0, str, 128)) {
        cout << "*** GetStringDescriptor() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "GetStringDescriptor() OK" << endl;
    }

    // Test SetTimeout
    if (!qusb->SetTimeout(1000)) {
        cout << "*** SetTimeout() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "SetTimeout() OK" << endl;
    }

    // Test GetDllVersion
    if (!CQuickUsb::GetDllVersion(&major, &minor, &rev)) {
        cout << "*** GetDllVersion() failed with error: " << CQuickUsb::GetLastError() << endl;
    } else {
        cout << "GetDllVersion() OK" << endl;
    }

    // Test GetDriverVersion
    if (!CQuickUsb::GetDriverVersion(&major, &minor, &rev)) {
        cout << "*** GetDriverVersion() failed with error: " << CQuickUsb::GetLastError() << endl;
    } else {
        cout << "GetDriverVersion() OK" << endl;
    }

    // Test GetFirmwareVersion
    if (!qusb->GetFirmwareVersion(&major, &minor, &rev)) {
        cout << "*** GetFirmwareVersion() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "GetFirmwareVersion() OK" << endl;
    }

    // Test ReadPortDir
    if (!qusb->ReadPortDir(PORT_A, &byte)) {
        cout << "*** ReadPortDir() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadPortDir() OK" << endl;
    }

    // Test WritePortDir
    byte |= 0x80;
    if (!qusb->WritePortDir(PORT_A, byte)) {
        cout << "*** WritePortDir() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WritePortDir() OK" << endl;
    }

    // Test ReadPort
    value = 1;
    if (!qusb->ReadPort(PORT_A, &byte, &value)) {
        cout << "*** ReadPort() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadPort() OK" << endl;
    }

    // Test WritePort
    byte = value | 0x80;
    if (!qusb->WritePort(PORT_A, &byte, 1)) {
        cout << "*** WritePort() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WritePort() OK" << endl;
    }

    // Test ReadSetting
    if (!qusb->ReadSetting(SETTING_PORTA, &value)) {
        cout << "*** ReadSetting() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadSetting() OK" << endl;
    }

    // Test WriteSetting
    value |= 0x80;
    if (!qusb->WriteSetting(SETTING_PORTA, value)) {
        cout << "*** WriteSetting() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteSetting() OK" << endl;
    }

    // Test ReadDefault
    if (!qusb->ReadDefault(SETTING_PORTA, &value)) {
        cout << "*** ReadDefault() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadDefault() OK" << endl;
    }

    // Test WriteDefault
    if (!qusb->WriteDefault(SETTING_PORTA, value)) {
        cout << "*** WriteDefault() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteDefault() OK" << endl;
    }

    // Test StartFpgaConfiguration
    if (!qusb->StartFpgaConfiguration()) {
        cout << "*** StartFpgaConfiguration() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "StartFpgaConfiguration() OK" << endl;
    }

    // Test WriteFpgaData
    if (!qusb->WriteFpgaData(buffer, 64)) {
        cout << "*** WriteFpgaData() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteFpgaData() OK" << endl;
    }

    // Test FpgaConfigure
    if (!qusb->ConfigureFpga(FPGA_FILE_PATH)) {
        cout << "*** FpgaConfigure() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "FpgaConfigure() OK" << endl;
    }

    // Test IsFpgaConfigured
    if (!qusb->IsFpgaConfigured(&conf)) {
        cout << "*** IsFpgaConfigured() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "IsFpgaConfigured() OK" << endl;
    }

    // Test ReadCommand
    value = 64;
    if (!qusb->ReadCommand(0, buffer, &value)) {
        cout << "*** ReadCommand() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadCommand() OK" << endl;
    }

    // Test WriteCommand
    if (!qusb->WriteCommand(0, buffer, 64)) {
        cout << "*** WriteCommand() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteCommand() OK" << endl;
    }
	
    // Test SetRS232BaudRate
    if (!qusb->SetRS232BaudRate(192000)) {
        cout << "*** SetRS232BaudRate() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "SetRS232BaudRate() OK" << endl;
    }

    // Test GetNumRS232
    if (!qusb->GetNumRS232(0, &value)) {
        cout << "*** GetNumRS232() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "GetNumRS232() OK" << endl;
    }

    // Test ReadRS232
    if (value > 0) {
        if (!qusb->ReadRS232(1, buffer, &value)) {
            cout << "*** ReadRS232() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "ReadRS232() OK" << endl;
        }
    }

    // Test FlushRS232
    if (!qusb->FlushRS232(0)) {
        cout << "*** FlushRS232() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "FlushRS232() OK" << endl;
    }

    // Test WriteRS232
    if (!qusb->WriteRS232(0, buffer, 64)) {
        cout << "*** WriteRS232() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteRS232() OK" << endl;
    }

    // Test ReadSpi
    value = 64;
    if (!qusb->ReadSpi(0, buffer, &value)) {
        cout << "*** ReadSpi() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadSpi() OK" << endl;
    }

    // Test WriteSpi
    if (!qusb->WriteSpi(0, buffer, 64)) {
        cout << "*** WriteSpi() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteSpi() OK" << endl;
    }

    // Test WriteReadSpi
    if (!qusb->WriteReadSpi(0, buffer, 64)) {
        cout << "*** WriteReadSpi() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteReadSpi() OK" << endl;
    }

    // Set I2C to IgnoreACK
    if (!qusb->WriteSetting(SETTING_I2CTL, 0x80)) {
        cout << "*** WriteSetting() failed with error: " << qusb->LastError() << endl;
    }

    // Test ReadI2C
    value = 64;
    if (!qusb->ReadI2C(0x60, buffer, &value)) {
        cout << "*** ReadI2C() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadI2C() OK" << endl;
    }

    // Test CachedWriteI2C
    if (!qusb->CachedWriteI2C(0x60, buffer, 64)) {
        cout << "*** CachedWriteI2C() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "CachedWriteI2C() OK" << endl;
    }

    // Test WriteI2C
    if (!qusb->WriteI2C(0x60, buffer, 64)) {
        cout << "*** WriteI2C() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteI2C() OK" << endl;
    }

    // Test WriteStorage
    if (!qusb->WriteStorage(0, buffer, 64)) {
        cout << "*** WriteStorage() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteStorage() OK" << endl;
    }

    // Test ReadStorage
    if (!qusb->ReadStorage(0, buffer, 64)) {
        cout << "*** ReadStorage() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadStorage() OK" << endl;
    }

    // Test IdentifyEpcs
    if (!qusb->IdentifyEpcs(0, &id, &size, 0)) {
        cout << "*** IdentifyEpcs() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "IdentifyEpcs() OK (id=" << id << ", size=" << size << ")" << endl;
    }

    if (id != QUICKUSB_EPCS_ID_UNKNOWN) {
        // Test ConfigureEpcs
        if (!qusb->ConfigureEpcs(0, EPCS_FILE_PATH, 0, progress, NULL)) {
            cout << "*** ConfigureEpcs() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "ConfigureEpcs() OK" << endl;
        }

        // Test VerifyEpcs
        if (!qusb->VerifyEpcs(0, EPCS_FILE_PATH, 0, progress, NULL)) {
            cout << "*** VerifyEpcs() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "VerifyEpcs() OK" << endl;
        }

        // Test EraseEpcs
        if (!qusb->EraseEpcs(0, 0, progress, NULL)) {
            cout << "*** EraseEpcs() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "EraseEpcs() OK" << endl;
        }
    }

    // Test ReadData
    length = 1024;
    if (!qusb->ReadData(buffer, &length)) {
        cout << "*** ReadData() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadData() OK" << endl;
    }

    // Test ReadDataEx
    length = 1024;
    if (!qusb->ReadDataEx(buffer, &length, CQuickUsb::OutOfOrder)) {
        cout << "*** ReadDataEx() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadDataEx() OK" << endl;
    }

    // Test WriteData
    length = 1024;
    if (!qusb->WriteData(buffer, length)) {
        cout << "*** WriteData() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteData() OK" << endl;
    }

    // Test WriteDataEx
    length = 1024;
    if (!qusb->WriteDataEx(buffer, &length, CQuickUsb::OutOfOrder)) {
        cout << "*** WriteDataEx() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteDataEx() OK" << endl;
    }

    // Test SetNumAsyncThreads
    if (!qusb->SetNumAsyncThreads(2, 2)) {
        cout << "*** SetNumAsyncThreads() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "SetNumAsyncThreads() OK" << endl;
    }

    // Test GetNumAsyncThreads
    if (!qusb->GetNumAsyncThreads(&numThreads, &concurrency)) {
        cout << "*** GetNumAsyncThreads() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "GetNumAsyncThreads() OK" << endl;
    }   

    // Test ReadDataAsync - Deprecated
    size = 1024;
    if (!qusb->ReadDataAsync(buffer, &size, &transId)) {
        cout << "*** ReadDataAsync() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadDataAsync() OK" << endl;

        // Test AsyncWait - Deprecated
        if (!qusb->AsyncWait(&size, transId, FALSE) || size != 1024) {
            cout << "*** AsyncWait() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "AsyncWait() OK" << endl;
        }
    }

    // Test WriteDataAsync - Deprecated
    if (!qusb->WriteDataAsync(buffer, 1024, &transId)) {
        cout << "*** WriteDataAsync() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteDataAsync() OK" << endl;

        // Test AsyncWait - Deprecated
        if (!qusb->AsyncWait(&size, transId, FALSE) || size != 1024) {
            cout << "*** AsyncWait() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "AsyncWait() OK" << endl;
        }
    }

    // Test ReadBulkDataAsync
    if (!qusb->ReadBulkDataAsync(buffer, 1024, &bs, 0, 0)) {
        cout << "*** ReadBulkDataAsync() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadBulkDataAsync() OK" << endl;

        // Test BulkWait
        if (!qusb->BulkWait(&bs, FALSE)) {
            cout << "*** BulkWait() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "BulkWait() OK" << endl;
        }
    }

    // Test WriteBulkDataAsync
    if (!qusb->WriteBulkDataAsync(buffer, 1024, &bs, 0, 0)) {
        cout << "*** WriteBulkDataAsync() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteBulkDataAsync() OK" << endl;
        // Test WriteBulkDataAsync
        if (!qusb->WriteBulkDataAsync(buffer1, 1024, &bs1, 0, 0)) {
            cout << "*** WriteBulkDataAsync() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "WriteBulkDataAsync() OK" << endl;

            // Test BulkAbort
            if (!qusb->BulkAbort(NULL)) {
                cout << "*** BulkAbort request failed with error: " << qusb->LastError() << endl;
			} else {
                cout << "BulkAbort() OK" << endl;
            }

            // Test BulkWait
            if (!qusb->BulkWait(&bs, FALSE)) {
                cout << "*** BulkWait() failed with error: " << qusb->LastError() << endl;
            }
			if (bs.Error || (bs.BytesRequested != bs.BytesTransferred)) {
                cout << "*** BulkWait request failed with error: " << bs.Error << endl;
			} else {
                cout << "BulkWait() OK" << endl;
            }

            // Test BulkWait
            if (!qusb->BulkWait(&bs1, FALSE)) {
                cout << "*** BulkWait() failed with error: " << qusb->LastError() << endl;
            }
			if (bs1.Error || (bs1.BytesRequested != bs1.BytesTransferred)) {
				cout << "*** BulkWait request failed with error: " << bs1.Error << endl;
			} else {
                cout << "BulkWait() OK" << endl;
            }
        }
    }

    // Test WriteFirmware
    if (!qusb->WriteFirmware(FIRMWARE_FILE_PATH, CQuickUsb::ProgrammingProgramSerial | 1234, &progress, NULL)) {
        cout << "*** WriteFirmware() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteFirmware() OK" << endl;
    }

    // Test VerifyFirmware
    if (!qusb->VerifyFirmware(FIRMWARE_FILE_PATH, &progress, NULL)) {
        cout << "*** VerifyFirmware() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "VerifyFirmware() OK" << endl;
    }

    // Test ReadBulkDataStartStream
    if (!qusb->ReadBulkDataStartStream(NULL, 3, 300 * 1024, &StreamCR, NULL, &streamID, 1, 1)) {
        cout << "*** ReadBulkDataStartStream() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadBulkDataStartStream() OK" << endl;

        // Stream data for a little while
        SleepMS(500);

        // Pause the stream
        if (!qusb->PauseStream(streamID, false)) {
            cout << "*** PauseStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "PauseStream() OK" << endl;
        }

        // Wait a little while
        SleepMS(500);

        // Resume the stream
        if (!qusb->ResumeStream(streamID)) {
            cout << "*** ResumeStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "ResumeStream() OK" << endl;
        }

        // Wait a little while
        SleepMS(500);

        // Test BulkAbort
        if (!qusb->BulkAbort(NULL)) {
            cout << "*** BulkAbort request failed with error: " << qusb->LastError() << endl;
		} else {
            cout << "BulkAbort() OK" << endl;
        }

        // Test BulkDataStopStream
        if (!qusb->StopStream(streamID, FALSE)) {
            cout << "*** StopStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "StopStream() OK" << endl;
        }
    }

    // Test ReadBulkDataStartStreamToFile
    if (!qusb->ReadBulkDataStartStreamToFile("test.bin", 3, 300 * 1024, 100, &streamID, CQuickUsb::StreamFlagCreateAlways)) {
        cout << "*** ReadBulkDataStartStreamToFile() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ReadBulkDataStartStreamToFile() OK" << endl;

        // Stream data for a little while
        SleepMS(500);

        // Test BulkDataStopStream
        if (!qusb->StopStream(streamID, FALSE)) {
            cout << "*** StopStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "StopStream() OK" << endl;
        }
    }

    // Test WriteBulkDataStartStream
    bufArray[0] = buffer;
    if (!qusb->WriteBulkDataStartStream(bufArray, 1, 1024, &StreamCR, NULL, &streamID, 0, 0)) {
        cout << "*** WriteBulkDataStartStream() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteBulkDataStartStream() OK" << endl;

        // Stream data for a little while
        time_t stime = time(0);
        bool processOk = true;
        while ((time(0) - stime) < 2) {
            // This is a single-threaded test so we must process the stream
            if (!qusb->ProcessStream(streamID, 50)) {
                if (qusb->LastError() != CQuickUsb::ErrorTimeout) {
                    processOk = false;
                    cout << "*** ProcessStream() failed with error: " << qusb->LastError() << endl;
                    break;
                }
            }
        }

        // Test BulkAbort
        if (!qusb->BulkAbort(NULL)) {
            cout << "*** BulkAbort request failed with error: " << qusb->LastError() << endl;
	    } else {
            cout << "BulkAbort() OK" << endl;
        }

        // Test ProcessStream
        if (processOk) {
            cout << "ProcessStream() OK" << endl;
        }

        // Test WriteDataStopStream
        if (!qusb->StopStream(streamID, FALSE)) {
            cout << "*** WriteDataStopStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "StopStream() OK" << endl;
        }
    }

    // Test WriteBulkDataStartStream
    bufArray[0] = buffer;
    if (!qusb->WriteBulkDataStartStream(bufArray, 1, 1024, &StreamCR, NULL, &streamID, 0, 0)) {
        cout << "*** WriteBulkDataStartStream() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "WriteBulkDataStartStream() OK" << endl;

        // Stream data for a little while
        time_t stime = time(0);
        bool processOk = true;
        while ((time(0) - stime) < 2) {
            // This is a single-threaded test so we must process the stream
            if (!qusb->ProcessStream(streamID, 50)) {
                if (qusb->LastError() != CQuickUsb::ErrorTimeout) {
                    processOk = false;
                    cout << "*** ProcessStream() failed with error: " << qusb->LastError() << endl;
                    break;
                }
            }
        }

        // Test BulkAbort
        if (!qusb->BulkAbort(NULL)) {
            cout << "*** BulkAbort request failed with error: " << qusb->LastError() << endl;
	    } else {
            cout << "BulkAbort() OK" << endl;
        }

        // Test ProcessStream
        if (processOk) {
            cout << "ProcessStream() OK" << endl;
        }

        // Test WriteDataStopStream
        if (!qusb->StopStream(streamID, FALSE)) {
            cout << "*** WriteDataStopStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "StopStream() OK" << endl;
        }
    }

    // Test WriteBulkDataStartStreamFromFile
    bufArray[0] = buffer;
    if (!qusb->WriteBulkDataStartStreamFromFile("test.bin", 1, 1 * 1024, 100, &streamID, 0)) {
        cout << "*** WriteBulkDataStartStreamFromFile() failed with error: " << qusb->LastError() << endl;
    } else {
        QULONG status, streamError;
        QBOOL funcOK = TRUE;
        
        cout << "WriteBulkDataStartStreamFromFile() OK" << endl;

        do {
            if (!qusb->GetStreamStatus(streamID, &status, &streamError)) {
                cout << "*** GetStreamStatus() failed with error: " << qusb->LastError() << endl;
                funcOK = FALSE;
                break;
            }
        } while (status == QUICKUSB_STREAM_STATUS_RUNNING);
        
        if (funcOK) {
            cout << "GetStreamStatus() OK" << endl;
        }

        // Test WriteDataStopStream
        if (!qusb->StopStream(streamID, FALSE)) {
            cout << "*** StopStream() failed with error: " << qusb->LastError() << endl;
        } else {
            cout << "StopStream() OK" << endl;
        }
    }

    // Test ResetStatistic
    if (!qusb->ResetStatistic(CQuickUsb::StatAll)) {
        cout << "*** ResetStatistic() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "ResetStatistic() OK" << endl;
    }

    // Test GetStatistic
    if (!qusb->GetStatistic(CQuickUsb::StatTotalThroughput, CQuickUsb::StatUnitMBPerSec, &stat, 0)) {
        cout << "*** GetStatistic() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "GetStatistic() OK" << endl;
    }

    // Test Close
    if (!qusb->Close()) {
        cout << "*** Close() failed with error: " << qusb->LastError() << endl;
    } else {
        cout << "Close() OK" << endl;
    }

    // Test QuickUsbFreeDataBuffer
    if (!CQuickUsb::FreeDataBuffer(&buffer)) {
        cout << "*** FreeDataBuffer request failed with error: " << bs.Error << endl;
    } else {
        cout << "FreeDataBuffer() OK" << endl;
    }

    // Test QuickUsbFreeDataBuffer
    if (!CQuickUsb::FreeDataBuffer(&buffer1)) {
        cout << "*** FreeDataBuffer request failed with error: " << bs.Error << endl;
    } else {
        cout << "FreeDataBuffer() OK" << endl;
    }

    delete qusb;

    return 0;
}
