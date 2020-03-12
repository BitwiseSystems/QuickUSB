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
import time, sys
from QuickUsb import *

# The unit tests in this file are not intended to exercise the QuickUSB API, but
# test that the C++ Class wrapper properly wraps every QuickUSB API call.

def progress(hDevice, percent, tag):
    #print percent, "%\r",
    pass


def StreamCR(pBulkStream):
    if not pBulkStream:
        print "Stream Error"
        return

    #Since we are using ctypes, we must dereference the pointer to the BulkStream struture here
    bs = pBulkStream[0]

    if (bs.Error or (bs.BytesRequested != bs.BytesTransferred)):
        print "Stream request failed with error: ", bs.Error, "("+str(bs.BytesTransferred), "of", str(bs.BytesRequested)+" bytes)"
    #else:
    #    print "Stream request OK ", "("+str(bs.BytesTransferred), "of", str(bs.BytesRequested)+" bytes)"

if len(sys.argv) == 1:
    test = ("epcs")#("base", "data", "streaming", "fpga", "epcs", "rs232", "i2c", "storage", "command", "spi", "firmware", "stat")
else:
    test = sys.argv[1:]

if "base" in test:
    # Test FindModules
    (ok, nameList) = QuickUsb.FindModules()
    if (not ok):
        print "***FindModules() failed with error: ", str(Error(QuickUsb.GetLastError()))
        sys.exit(1)
    else:
        print "FindModules OK"

    qusb = QuickUsb(nameList[0])

    # Test Open
    (ok, ) = qusb.Open()
    if (not ok):
        print "***Open() failed with error: ", str(Error(QuickUsb.GetLastError()))
    else:
        print "Open OK"

    # Test GetLastError
    if (QuickUsb.GetLastError()):
        print "***GetLastError() failed with error: ", str(Error(QuickUsb.GetLastError()))
    else:
        print "GetLastError OK"

    qusb = QuickUsb(nameList[0])

    # Test GetLastDriverError
    if (QuickUsb.GetLastDriverError()):
        print "***GetLastDriverError() failed with error: ", str(Error(QuickUsb.GetLastDriverError()))
    else:
        print "GetLastDriverError OK"

    # Test Close
    (ok, ) = qusb.Close()
    if (not ok):
        print "***Close() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "Close OK"
    
    # Test OpenEx
    (ok, ) = qusb.OpenEx(nameList[0], 0)
    if (not ok):
        print "***OpenEx() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "OpenEx OK"

    # Test GetStringDescriptor
    (ok, string) = qusb.GetStringDescriptor(0)
    if (not ok):
        print "***GetStringDescriptor() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "GetStringDescriptor OK"

    # Test SetTimeout
    (ok, ) = qusb.SetTimeout(1000)
    if (not ok):
        print "***SetTimeout() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "SetTimeout OK"
    
    # Test GetDllVersion
    (ok, major, minor, rev) = QuickUsb.GetDllVersion()
    if (not ok):
        print "***GetDllVersion() failed with error: ", QuickUsb.GetLastError() 
    else:
        print "GetDllVersion OK"
    
    # Test GetDriverVersion
    (ok, major, minor, rev) = QuickUsb.GetDriverVersion()
    if (not ok):
        print "***GetDriverVersion() failed with error: ", QuickUsb.GetLastError() 
    else:
        print "GetDriverVersion OK"
    
    # Test GetFirmwareVersion
    (ok, major, minor, rev) = qusb.GetFirmwareVersion()
    if (not ok):
        print "***GetFirmwareVersion() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "GetFirmwareVersion OK"
    
    # Test ReadPortDir
    (ok, byte) = qusb.ReadPortDir(Port.A)
    if (not ok):
        print "***ReadPortDir() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadPortDir OK"
    byte |= 0x80
    
    # Test WritePortDir
    (ok, ) = qusb.WritePortDir(Port.A, byte)
    if (not ok):
        print "***WritePortDir() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WritePortDir OK"
    
    # Test ReadPort
    c_data = CreateByteBuffer(1)
    (ok, value) = qusb.ReadPort(Port.A, c_data, 1)
    if (not ok):
        print "***ReadPort() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadPort OK"
    
    # Test WritePort
    (ok, ) = qusb.WritePort(Port.A, byte, 1)
    if (not ok):
        print "***WritePort() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WritePort OK"
    
    # Test ReadSetting
    (ok, value) = qusb.ReadSetting(Setting.PortA)
    if (not ok):
        print "***ReadSetting() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadSetting OK"
    value |= 0x80
    
    # Test WriteSetting
    (ok, ) = qusb.WriteSetting(Setting.PortA, value)
    if (not ok):
        print "***WriteSetting() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteSetting OK"
    
    # Test ReadDefault
    (ok, value) = qusb.ReadDefault(Setting.PortA)
    if (not ok):
        print "***ReadDefault() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadDefault OK"
    
    # Test WriteDefault
    (ok, ) = qusb.WriteDefault(Setting.PortA, value)
    if (not ok):
        print "***WriteDefault() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteDefault OK"
    
    # Test StartFpgaConfiguration
    (ok, ) = qusb.StartFpgaConfiguration()
    if (not ok):
        print "***StartFpgaConfiguration() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "StartFpgaConfiguration OK"
else:
    # Test FindModules
    (ok, nameList) = QuickUsb.FindModules()
    if (not ok):
        print "***FindModules() failed with error: ", str(Error(QuickUsb.GetLastError()))
        sys.exit(1)
    else:
        print "FindModules OK"

    qusb = QuickUsb(nameList[0])

    # Test Open
    (ok, ) = qusb.Open()
    if (not ok):
        print "***Open() failed with error: ", str(Error(QuickUsb.GetLastError()))
    else:
        print "Open OK"

if "fpga" in test:
    # Test WriteFpgaData
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteFpgaData(c_data, 64)
    if (not ok):
        print "***WriteFpgaData() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteFpgaData OK"
    
    # Test IsFpgaConfigured
    (ok, conf) = qusb.IsFpgaConfigured()
    if (not ok):
        print "***IsFpgaConfigured() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "IsFpgaConfigured OK"

    # Test FpgaConfigure
    (ok, ) = qusb.ConfigureFpga("fpga.rbf")
    if (not ok):
        print "***FpgaConfigure() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "FpgaConfigure OK"

if "command" in test:
    # Test ReadCommand
    c_data = CreateByteBuffer(64)
    (ok, data) = qusb.ReadCommand(0, c_data, 64)
    if (not ok):
        print "***ReadCommand() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadCommand OK"
    
    # Test WriteCommand
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteCommand(0, c_data, 64)
    if (not ok):
        print "***WriteCommand() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteCommand OK"

if "rs232" in test:
    # Test SetRS232BaudRate
    (ok, ) = qusb.SetRS232BaudRate(192000)
    if (not ok):
        print "***SetRS232BaudRate() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "SetRS232BaudRate OK"
    
    # Test GetNumRS232
    (ok, value) = qusb.GetNumRS232(0)
    if (not ok):
        print "***GetNumRS232() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "GetNumRS232 OK"
    
    # Test ReadRS232
    if (value > 0):
        (ok, data) = qusb.ReadRS232(1, value)
    if (not ok):
            print "***ReadRS232() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadRS232 OK"
    
    # Test FlushRS232
    (ok, ) = qusb.FlushRS232(0)
    if (not ok):
        print "***FlushRS232() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "FlushRS232 OK"
    
    # Test WriteRS232
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteRS232(0, c_data, 64)
    if (not ok):
        print "***WriteRS232() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteRS232 OK"

if "spi" in test:
    # Test ReadSpi
    c_data = CreateByteBuffer(64)
    (ok, data) = qusb.ReadSpi(0, c_data, 64)
    if (not ok):
        print "***ReadSpi() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadSpi OK"
    
    # Test WriteSpi
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteSpi(0, c_data, 64)
    if (not ok):
        print "***WriteSpi() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteSpi OK"
    
    # Test WriteReadSpi
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteReadSpi(0, c_data, 64)
    if (not ok):
        print "***WriteReadSpi() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteReadSpi OK"

if "i2c" in test:
    # Test ReadI2C
    c_data = CreateByteBuffer(64)
    (ok, data) = qusb.ReadI2C(0x60, c_data, 64)
    if (not ok):
        print "***ReadI2C() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadI2C OK"
    
    # Test CachedWriteI2C
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.CachedWriteI2C(0x60, c_data, 64)
    if (not ok):
        print "***CachedWriteI2C() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "CachedWriteI2C OK"
    
    # Test WriteI2C
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteI2C(0x60, c_data, 64)
    if (not ok):
        print "***WriteI2C() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteI2C OK"

if "storage" in test:
    # Test WriteStorage
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.WriteStorage(0, c_data, 64)
    if (not ok):
        print "***WriteStorage() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteStorage OK"
    
    # Test ReadStorage
    c_data = CreateByteBuffer(64)
    (ok, ) = qusb.ReadStorage(0, c_data, 64)
    if (not ok):
        print "***ReadStorage() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadStorage OK"

if "epcs" in test:
    # Test IdentifyEpcs
    (ok, id, size) = qusb.IdentifyEpcs(0, EpcsFlags.IgnoreNCE)
    if (not ok):
        print "***IdentifyEpcs() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "IdentifyEpcs OK"
    
    if (id != EpcsId.EpcsUnknown):
        # Test ConfigureEpcs
        (ok, ) = qusb.ConfigureEpcs(0, "epcs.dat", 0, progress, None)
        if (not ok):
            print "***ConfigureEpcs() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "ConfigureEpcs OK"
    
        # Test VerifyEpcs
        (ok, ) = qusb.VerifyEpcs(0, "epcs.dat", 0, progress, None)
        if (not ok):
            print "***VerifyEpcs() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "VerifyEpcs OK"
    
        # Test EraseEpcs
        (ok, ) = qusb.EraseEpcs(0, 0, progress, None)
        if (not ok):
            print "***EraseEpcs() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "EraseEpcs OK"
    else:
    	print "***Cannot test ConfigureEpcs(): No EPCS device found"
    	print "***Cannot test VerifyEpcs(): No EPCS device found"
    	print "***Cannot test EraseEpcs(): No EPCS device found"

if "data" in test:
    # Test ReadData
    length = 1024
    c_data = CreateByteBuffer(length)
    (ok, bytesRead) = qusb.ReadData(c_data, length)
    if (not ok):
        print "***ReadData() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "ReadData OK"

    # Test ReadDataEx
    length = 1024
    c_data = CreateByteBuffer(length)
    (ok, bytesRead) = qusb.ReadDataEx(c_data, length, DataFlags.OutOfOrder)
    if (not ok):
        print "***ReadDataEx() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "ReadDataEx OK"

    # Test WriteData
    length = 1024
    c_data = CreateByteBuffer(length)
    (ok, ) = qusb.WriteData(c_data, length)
    if (not ok):
        print "***WriteData() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteData OK"

    # Test WriteDataEx
    length = 1024
    c_data = CreateByteBuffer(length)
    (ok, ) = qusb.WriteDataEx(c_data, length, DataFlags.OutOfOrder)
    if (not ok):
        print "***WriteDataEx() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteDataEx OK"

    # Test GetNumAsyncThreads
    (ok, numThreads, concurrency) = qusb.GetNumAsyncThreads()
    if (not ok):
        print "***GetNumAsyncThreads() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "GetNumAsyncThreads OK"

    # Test SetNumAsyncThreads
    (ok,) = qusb.SetNumAsyncThreads(0, 0)
    if (not ok):
        print "***SetNumAsyncThreads() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "SetNumAsyncThreads OK"

    # Test ReadDataAsync - Deprecated
    size = 1024
    c_data = CreateByteBuffer(size)
    (ok, transId) = qusb.ReadDataAsync(c_data, size)
    if (not ok):
        print "***ReadDataAsync() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadDataAsync OK"
    
        # Test AsyncWait - Deprecated
        (ok, size) = qusb.AsyncWait(transId, False)
        if (not ok or size != 1024):
            print "***AsyncWait() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "AsyncWait OK"
    
    # Test WriteDataAsync - Deprecated
    c_data = CreateByteBuffer(1024)
    (ok, transId) = qusb.WriteDataAsync(c_data, 1024)
    if (not ok):
        print "***WriteDataAsync() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "WriteDataAsync OK"
    
        # Test AsyncWait - Deprecated
        (ok, size) = qusb.AsyncWait(transId, False)
        if (not ok or size != 1024):
            print "***AsyncWait() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "AsyncWait OK"

    # Test AllocateDataBuffer
    (ok, c_data) = qusb.AllocateDataBuffer(1024)
    if (not ok):
        print "***AllocateDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "AllocateDataBuffer OK"

    # Test AllocateDataBuffer
    (ok, c_data1) = qusb.AllocateDataBuffer(1024)
    if (not ok):
        print "***AllocateDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "AllocateDataBuffer OK"

    # Test ReadBulkDataAsync
    (ok, bs) = qusb.ReadBulkDataAsync(c_data, 1024, 0, 0)
    if (not ok):
        print "***ReadBulkDataAsync() failed with error: ", str(Error(qusb.LastError())) 
    else:
        print "ReadBulkDataAsync OK"

        # Test BulkAbort
        (ok,) = qusb.BulkAbort(None)
        if (not ok):
            print "***BulkAbort() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "BulkAbort OK"
    
        # Test BulkWait
        (ok, ) = qusb.BulkWait(bs, False)
        if (not ok):
            print "***BulkWait() failed with error: ", str(Error(qusb.LastError())) 
        else:
            print "BulkWait OK"

    # Test WriteBulkDataAsync
    (ok, bs) = qusb.WriteBulkDataAsync(c_data, 1024, StreamCR, 0)
    if (not ok):
        print "***WriteBulkDataAsync() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteBulkDataAsync OK"

        # Test WriteBulkDataAsync
        (ok, bs1) = qusb.WriteBulkDataAsync(c_data1, 1024, StreamCR, 0)
        if (not ok):
            print "***WriteBulkDataAsync() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "WriteBulkDataAsync OK"

            # Test BulkWait
            (ok, ) = qusb.BulkWait(bs, False)
            if (not ok):
                print "***BulkWait() failed with error: ", str(Error(qusb.LastError()))
            else:
                print "BulkWait OK"
                if (bs.Error or (bs.BytesRequested != bs.BytesTransferred)):
                    print "WriteBulkDataAsync request failed with error: ", bs.Error, bs.BytesRequested, bs.BytesTransferred
    
            # Test BulkWait
            (ok, ) = qusb.BulkWait(bs1, False)
            if (not ok):
                print "***BulkWait() failed with error: ", str(Error(qusb.LastError())), qusb.LastDriverError()
                print bs1.Buffer, bs1.BytesRequested, bs1.BytesTransferred, bs1.CompletionRoutine, bs1.CompletionType, bs1.Error
            else:
                print "BulkWait OK"
                if (bs1.Error or (bs1.BytesRequested != bs1.BytesTransferred)):
                    print "WriteBulkDataAsync request failed with error: ", bs1.Error, bs.BytesRequested, bs.BytesTransferred

    # Test FreeDataBuffer
    (ok,) = qusb.FreeDataBuffer(c_data)
    if (not ok):
        print "***FreeDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "FreeDataBuffer OK"

    # Test FreeDataBuffer
    (ok,) = qusb.FreeDataBuffer(c_data1)
    if (not ok):
        print "***FreeDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "FreeDataBuffer OK"

if "firmware" in test:
    # Test WriteFirmware
    if sys.platform.lower().startswith("linux") or sys.platform.lower().startswith("darwin"):
    	path = "/Projects/Bitwise/QuickUSB/Library/Firmware/QuickUSB Firmware v2.15.1/quickusb-simple v2.15.1.qusb"
    else:
    	path = "C:\\Projects\\Bitwise\\QuickUSB\\Library\\Firmware\\QuickUSB Firmware v2.15.1\\quickusb-simple v2.15.1.qusb"
    (ok, ) = qusb.WriteFirmware(path, 0, progress, None)
    if (not ok):
        print "***WriteFirmware() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteFirmware OK"
    
    # Test VerifyFirmware
    (ok, ) = qusb.VerifyFirmware(path, progress, None)
    if (not ok):
        print "***VerifyFirmware() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "VerifyFirmware OK"

if "streaming" in test:
    # Test ReadBulkDataStartStream
    (ok, streamID) = qusb.ReadBulkDataStartStream(None, 2, 16 * 1024, StreamCR, None, 1, 1)
    if (not ok):
        print "***ReadBulkDataStartStream() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "ReadBulkDataStartStream OK"

        # Stream data for a little while
        time.sleep(0.5)

        # Pause the stream
        (ok, ) = qusb.PauseStream(streamID, False)
        if (not ok):
            print "***PauseStream() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "PauseStream OK"

        # Wait a little
        time.sleep(0.5)

        # Resume the stream
        (ok, ) = qusb.ResumeStream(streamID)
        if (not ok):
            print "***ResumeStream() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "ResumeStream OK"

        # Stream data for a little while
        time.sleep(0.5)

        # Test BulkAbort
        (ok,) = qusb.BulkAbort(None)
        if (not ok):
            print "***BulkAbort() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "BulkAbort OK"
    
        # Test StopStream
        (ok, ) = qusb.StopStream(streamID, False)
        if (not ok):
            print "***StopStream() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "StopStream OK"
    
        time.sleep(0.1)
    
    # Test WriteBulkDataStartStream
    bufArray = (PQBYTE * 4)()
    allocOk = True
    for k in xrange(4):
        # Test AllocateDataBuffer
        (ok, c_data) = qusb.AllocateDataBuffer(1024)
        if (not ok):
            allocOk = False
        else:
            bufArray[k] = c_data
            
    if (not allocOk):
        print "***AllocateDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "AllocateDataBuffer-Array OK"

    
    (ok, streamID) = qusb.WriteBulkDataStartStream(bufArray, 4, 1024, StreamCR, None, 0, 0)
    if (not ok):
        print "***WriteBulkDataStartStream() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteBulkDataStartStream OK"

        # Stream data for a little while
        stime = time.time()
        processOk = True
        statusOk = True
        while (time.time() - stime) < 2.0:
            #This is a single-threaded stream test, so we must perform processing
            (ok,) = qusb.ProcessStream(streamID, 50)
            if ((not ok) and (qusb.LastError() != Error.Timeout)):
                processOk = False
                break

            (ok, status, error) = qusb.GetStreamStatus(streamID)
            if ((not ok) or (error != Error.NoError)):
                statusOk = False
                break

        # Test StopStream
        (ok, ) = qusb.StopStream(streamID, False)
        if (not ok):
            print "***StopStream() failed with error: ", str(Error(qusb.LastError()))
        else:
               print "StopStream OK"

        # Report if ProcessStream failed
        if (not processOk):
            print "***ProcessStream() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "ProcessStream OK"

        # Report if GetStreamStatus failed
        if (not statusOk):
            print "GetStreamStatus() failed with error: ", str(Error(qusb.LasError()))
        else:
            print "GetStreamStatus OK"

    # Test FreeDataBuffer
    freeOk = True
    for k in xrange(4):
        (ok,) = qusb.FreeDataBuffer(bufArray[k])
        if (not ok):
            freeOk = False
    if (not freeOk):
        print "***FreeDataBuffer() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "FreeDataBuffer-Array OK"

    # Test ReadBulkDataStartStreamToFile
    (ok, streamID) = qusb.ReadBulkDataStartStreamToFile("test.bin", 4, 1024, 100, StreamFlag.CreateAlways)
    if (not ok):
        print "ReadBulkDataStartStreamToFile() failed with error: ", str(Error(qusb.LasError()))
    else:
        print "ReadBulkDataStartStreamToFile OK"

        # Wait for the stream to finish
        statusOk = True
        while True:
            (ok, status, error) = qusb.GetStreamStatus(streamID)
            if (error != Error.NoError):
                statusOk = False
                break
                
            # Check if the stream has stopped running
            if (status != StreamStatus.Running):
                break;

        # Test StopStream
        (ok, ) = qusb.StopStream(streamID, False)
        if (not ok):
            print "***StopStream() failed with error: ", str(Error(qusb.LastError()))
        else:
               print "StopStream OK"

        # Report if GetStreamStatus failed
        if (not statusOk):
            print "GetStreamStatus() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "GetStreamStatus OK"

    # Test WriteBulkDataStartStreamFromFile
    (ok, streamID) = qusb.WriteBulkDataStartStreamFromFile("test.bin", 4, 1024, 100, StreamFlag.CreateAlways)
    if (not ok):
        print "WriteBulkDataStartStreamFromFile() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "WriteBulkDataStartStreamFromFile OK"

        # Wait for the stream to finish
        statusOk = True
        while True:
            (ok, status, error) = qusb.GetStreamStatus(streamID)
            if (error != Error.NoError):
                statusOk = False
                break
                
            # Check if the stream has stopped running
            if (status != StreamStatus.Running):
                break;

        # Test StopStream
        (ok, ) = qusb.StopStream(streamID, False)
        if (not ok):
            print "***StopStream() failed with error: ", str(Error(qusb.LastError()))
        else:
               print "StopStream OK"

        # Report if GetStreamStatus failed
        if ((not statusOk) and (error != Error.FileEOF)):
            print "GetStreamStatus() failed with error: ", str(Error(qusb.LastError()))
        else:
            print "GetStreamStatus OK"


if "stat" in test:
    # Test QuickUsbResetStatistic
    (ok, ) = qusb.ResetStatistic(Stat.All)
    if (not ok):
        print "***ResetStatistic() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "ResetStatistic OK"

    # Test QuickUsbGetStatistic
    (ok, value) = qusb.GetStatistic(Stat.TotalThroughput, StatUnit.MBPerSec, 0)
    if (not ok):
        print "***GetStatistic() failed with error: ", str(Error(qusb.LastError()))
    else:
        print "GetStatistic OK"

# Test Close
(ok, ) = qusb.Close()
if (not ok):
    print "***Close() failed with error: ", str(Error(qusb.LastError())) 
else:
    print "Close OK"
