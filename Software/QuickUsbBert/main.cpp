#include <iostream>
#include <stdio.h>
#include <queue>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <limits>
#include <errno.h>
#include <fcntl.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
#endif

#include "CQuickUsb.h"
#include "PlatformIndependentTimer.h"

using namespace std;

#if defined(__linux__)
    #define gettid() (int)syscall(SYS_gettid)
    #include <linux/version.h>
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25))
        #define IMPLEMENT_ASYNC
    #endif
#endif
#if defined(__APPLE__)
    #define gettid() (int)pthread_mach_thread_np(pthread_self())
#endif

#define LOG(x) cout << x; fout << x;
#define LOG_ERR(x) ++GeneralFailures; cout << x; fout << x;

//const char *FpgaFile = "/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbBert/QUSBEVB_REVA_EP2C20_BERT.rbf";
const char *FpgaFile = "QUSBEVB_REVA_EP2C20_BERT.rbf";

enum DataTestType { Start=0, SyncRead=1, SyncWrite=2, SyncLoopback=3, AsyncRead=4, AsyncWrite=5, AsyncLoopback=6, LegacyAsyncRead=7, LegacyAsyncWrite=8, 
    StreamingRead=9, StreamingWrite=10, StreamingLoopback=11, End=12};

enum StreamingDataTestType { Memory=0, DiskFile=1/*, MemoryFile=2 */};

// Global variables
int NumBuffers = 1;
QULONG BufferByteSize = 1 * 1024;
int NumThreads = 1;
int ThreadConcurrency = 1;
DataTestType TestType = StreamingWrite;
StreamingDataTestType StreamTestType = Memory;
bool UserAllocatedBuffers = true;
double UpdateFreq = 3.0; //numeric_limits<double>::max(); // Seconds
double TestTime = 5.0; // Seconds
ofstream fout;
QWORD NextOutLFSR = 0x1234;
QWORD NextInLFSR = 0xABCD;

bool RunningTest;
double tElapsed;
int NumTests;
int Failures, GeneralFailures;
unsigned long BytesRead, BytesWritten;
int inTransCount, outTransCount;
unsigned long long fpgaBertCount, softwareBertCount;
int CompletionRoutineFailure = false;

#ifdef IMPLEMENT_ASYNC
PQWORD *InBufferArray = NULL;
PQWORD *OutBufferArray = NULL;
PCQCHAR InFileName = "inTest.bin";
PCQCHAR OutFileName = "outTest.bin";
QBYTE InStreamID;
QBYTE OutStreamID;
QULONG nextInRequestID;
QULONG nextOutRequestID;
queue<PQWORD> PendingBuffersToVerify;
map<int, int> threadRequestCount;
#endif

// Function prototypes
bool Test_InitFpga(CQuickUsb &qusb);
bool Test_ReadBert(CQuickUsb &qusb);
bool Test_ResetTestState(CQuickUsb &qusb);
bool Test_DisplayStats(CQuickUsb &qusb);
void Test_Stop(CQuickUsb &qusb, bool showStats);
void Test_LogStats(CQuickUsb &qusb);
void Test_Run(CQuickUsb &qusb);
void Test_RunTestSuite(CQuickUsb &qusb);

#ifdef IMPLEMENT_ASYNC
QVOIDRETURN AsyncCompletionRoutine(PQBULKSTREAM BulkStream);
QVOIDRETURN CompletionRoutine(PQBULKSTREAM BulkStream);
#endif

QRESULT WriteFpgaConfigRegister(CQuickUsb &qusb, QWORD config);

void GetNextInLFSR(QWORD* data, int bytes);
void GetNextOutLFSR(QWORD* data, int bytes);
QWORD GetNextLFSR(QWORD prev);

unsigned long long CountBitErrors(QWORD* a, QWORD* b, int length);
void GetFriendlyDataSize(char* str, int length, double x);

PlatformIndependentTimer timer;


// ************
// *** Main ***
// ************
int main(int agrc, char **argv) {
    QCHAR nameList[128];
    QCHAR str[128], *model;
    QRESULT ok;
    CQuickUsb qusb;
    const char IOModel[] = "Simple I/O";
    
    
    // Search for connected QuickUSB devices
    ok = CQuickUsb::FindModules(nameList, 1024);
    if (!ok) {
        LOG_ERR("Unable to locate any QuickUSB Devices" << endl);
        return 1;
    } else {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        string path = "/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbBert/Logs/";
#if defined(__linux__)
        path += "Linux-";
#endif
#if defined(__APPLE__)
        path += "Mac-";
#endif
#if defined(_WIN32)
        path += "Windows-";
#endif
        path += asctime(timeinfo);
        path.erase(path.end()-1);
        path += ".txt";
        fout.open(path.c_str());
        
        LOG("Using QuickUSB device: " << nameList << endl);
        qusb.SetName(nameList);
    }

    QWORD driverMajor, driverMinor, driverBuild;
    QWORD firmwareMajor, firmwareMinor, firmwareBuild;
    QWORD libraryMajor, libraryMinor, libraryBuild;

    ok = qusb.Open();
    ok = qusb.GetDriverVersion(&driverMajor, &driverMinor, &driverBuild);
    ok = qusb.GetFirmwareVersion(&firmwareMajor, &firmwareMinor, &firmwareBuild);
    ok = qusb.GetDllVersion(&libraryMajor, &libraryMinor, &libraryBuild);

    if (((driverMajor != firmwareMajor) || (driverMajor != libraryMajor)) || 
        ((driverMinor != firmwareMinor) || (driverMinor != libraryMinor)) || 
        ((driverBuild != firmwareBuild) || (driverBuild != libraryBuild)))
    {
        // Error: Versions do not match!
        LOG("Library/Driver/Firmware version mismatch!" << endl);
    }
    qusb.Close();

    // Verify the device has the correct IO Model for these tests
    ok = qusb.GetStringDescriptor(2, str, 128);
    if (!ok) {
        LOG_ERR("Unable to retrieve string descritors" << endl);
        return 1;
    }
    *strrchr(str, ')') = '\0';
    model = strrchr(str, '(') + 1;
    if (strcmp(IOModel, model) != 0) {
        LOG("QuickUSB Module does not contain the correct firmware IO Model.  Please reprogram with " << IOModel << "." << endl);
        LOG("Current IO Model: " << model << endl);
        //return 1;
    }

    //Test_Run(qusb);
    Test_RunTestSuite(qusb);
    
    fout.close();

    cout << "Done...Press enter to continue" << endl;
    cin.get();
    return 0;
}



bool Test_InitFpga(CQuickUsb &qusb) {
    QWORD configured;
    QRESULT ok;

    // Set initial timeout
    ok = qusb.SetTimeout(1000);

    // Turn FPGA power on and place FPGA in reset
    ok = qusb.WritePortDir(CQuickUsb::PortA, 0x81);
    if (!ok) {
        LOG_ERR("QuickUsb.WritePortDir() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        return false;
    }
    ok = qusb.WritePort(CQuickUsb::PortA, 0x80);
    if (!ok) {
        LOG_ERR("QuickUsb.WritePort() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        return false;
    }

    // Check if the FPGA is configured
    ok = qusb.IsFpgaConfigured(&configured);
    if (!ok) {
        LOG_ERR("Unable to determine if FPGA is configured: " << qusb.LastError() << endl);
        return false;
    }

    // Configure the FPGA
    if (!configured) {
        LOG("Programming FPGA..." << endl);

        // Allow some time for the FPGA to become fully powered
        SleepMS(250);

        // Write the RBF file
        ok = qusb.ConfigureFpga(FpgaFile);
        if (!ok) {
            LOG_ERR("\nUnable to configure FPGA: " << qusb.LastError() << endl);
            return false;
        } else {
            LOG("Done" << endl);
        }

        // Allow FPGA ample time to boot up
        SleepMS(250);
    }

    // Write control register (Allowed while FPGA is in reset)
    if ((TestType == SyncLoopback) || (TestType == AsyncLoopback) || /*(TestType == LegacyAsyncLoopback) ||*/ (TestType == StreamingLoopback)) {
        WriteFpgaConfigRegister(qusb, 0x00 | 0x07);
    } else {
        WriteFpgaConfigRegister(qusb, 0x00 & ~0x04);
    }

    // Remove FPGA from reset
    ok = qusb.WritePort(CQuickUsb::PortA, 0x81);
    if (!ok) {
        LOG_ERR("QuickUsb.WritePort() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        return false;
    }

    // Wait a little for good measure
    SleepMS(100);

    // Return successfully
    return true;
}



bool Test_ReadBert(CQuickUsb &qusb)
{
    QBYTE data[8]; // BERT is 8 bytes
    QWORD numBytes = 8;
    
    if (!qusb.ReadCommand(3, data, &numBytes)) {
        LOG_ERR("QuickUsb.ReadCommand() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        return false;
    }

    // Construct BERT count
    fpgaBertCount = ((unsigned long long)data[7] << 56) | ((unsigned long long)data[6] << 48) |
                    ((unsigned long long)data[5] << 40) | ((unsigned long long)data[4] << 32) |
                    ((unsigned long long)data[3] << 24) | ((unsigned long long)data[2] << 16) |
                    ((unsigned long long)data[1] << 8)  | ((unsigned long long)data[0] << 0);

    return true;
}



bool Test_ResetTestState(CQuickUsb &qusb) {
#ifdef IMPLEMENT_ASYNC
    QBOOL allocBuffers;
    PQWORD data;
#endif
    
    // Reset variables
    BytesRead = 0;
    BytesWritten = 0;
    inTransCount = 0;
    outTransCount = 0;
    fpgaBertCount = 0;
    softwareBertCount = 0;
    GeneralFailures = 0;

#ifdef IMPLEMENT_ASYNC
    nextInRequestID = 0;
    nextOutRequestID = 0;
    //CompletionRoutineFailure = false;

    // Clear any lingering buffers
    while (PendingBuffersToVerify.size() > 0) {
        data = PendingBuffersToVerify.front();
        PendingBuffersToVerify.pop();
        delete[] data;
    }

    // Set the timeout
    //ok = qusb.SetTimeout(1000);

    // Grab test values
    allocBuffers = !(!UserAllocatedBuffers && ((TestType == StreamingRead) || (TestType == StreamingLoopback)));

    // Allocate arrays
    if (allocBuffers) {
        InBufferArray = new PQWORD[NumBuffers];
        memset(InBufferArray, 0, sizeof(PQWORD) * NumBuffers);

        for (int k = 0; k < NumBuffers; ++k) {
            CQuickUsb::AllocateDataBuffer((PQBYTE *)&InBufferArray[k], BufferByteSize);
            memset(InBufferArray[k], 0, BufferByteSize);
        }
    } else {
        InBufferArray = NULL;
    }

    OutBufferArray = new PQWORD[NumBuffers];
    for (int k = 0; k < NumBuffers; ++k) {
        CQuickUsb::AllocateDataBuffer((PQBYTE *)&OutBufferArray[k], BufferByteSize);
        memset(OutBufferArray[k], 0, BufferByteSize);
    }

#endif // IMPLEMENT_ASYNC
  
    // Return successfully
    return true;
}

bool Test_DisplayStats(CQuickUsb &qusb) {
    QCHAR str[128];
    tElapsed = timer.GetElapsedTimeInSeconds();
    
    double overallRate;
    if (tElapsed == 0.0) {
        overallRate = 0.0;
    } else {
        overallRate = (double)(BytesRead + BytesWritten) / tElapsed;
    }

    // Read the FPGA BERT
    Test_ReadBert(qusb);

    // Update stats
    LOG("FPGA BERT Count:     " << fpgaBertCount << endl);
    LOG("Software BERT Count: " << softwareBertCount << endl);
    GetFriendlyDataSize(str, 128, BytesRead);
    LOG("Data Read: " << str << endl);
    GetFriendlyDataSize(str, 128, BytesWritten);
    LOG("Data Written: " << str << endl);
    GetFriendlyDataSize(str, 128, overallRate);
    LOG("Data Rate: " << str << "/s" << endl);
    LOG("Read Transactions: " << inTransCount << endl);
    LOG("Write Transactions: " << outTransCount << endl);
    LOG(endl);

    // Return successfully
    return true;
}



void Test_Stop(CQuickUsb &qusb, bool showStats) {
    // Perform a final stats update
    if (showStats) {
        Test_DisplayStats(qusb);
        Test_LogStats(qusb);
    }

    RunningTest = false;

    // Log that we've finished the test
    LOG("*** Completed test ***" << endl);
    LOG(endl);
}



void Test_RunTestSuite(CQuickUsb &qusb) {
    double msPerTest = 5000; // 5 seconds
    
    int concurrency, numThreads, bufferSize, numBuffers;
    int testType;
    PQWORD data;
    bool stop = false;
    
    for (concurrency = 0; !stop && concurrency <= 2; ++concurrency) {
        for (numThreads = 0; !stop && numThreads <= 4; ++numThreads) {
            for (bufferSize = 512; !stop && bufferSize <= 2048 ; bufferSize *= 2) {
                for (numBuffers = 1; !stop && numBuffers <= 4; ++numBuffers) {
                    for (testType = (int)Start + 1; !stop && testType != (int)End; ++testType) {
                        // Verify we don't exceed the size of our FIFO buffers
                        if (((DataTestType)testType == AsyncLoopback) || ((DataTestType)testType == StreamingLoopback)) {
                            if ((bufferSize * numBuffers) > (2 * 4096)) {
                                continue;
                            }
                        }
                        
                        // Verify we dont try to set an incorrect number of threads/concurrency
                        if ((numThreads > 0) && (concurrency == 0)) {
                            continue;
                        }
                        
                        // Skip redundant synchronous tests where the number of threads > 0
                        if (((DataTestType)testType == SyncRead) || ((DataTestType)testType == SyncWrite) || ((DataTestType)testType == SyncLoopback)) {
                            if (numThreads > 0) {
                                continue;
                            }
                        }
                        
                        // Skip tests where only the concurrency is set but the number of threads is zero
                        if ((numThreads == 0) && (concurrency > 0)) {
                            continue;
                        }
                        
#if defined(__linux__) || defined(__APPLE__)
                        // Don't do legacy tests on Linux and Mac
                        if (((DataTestType)testType == LegacyAsyncRead) || ((DataTestType)testType == LegacyAsyncWrite)) {
                            continue;
                        }
#endif
                        
#if defined(__APPLE__)
                        // Multithreaded loopbacks are not supported on Mac, so skip them
                        if ((numThreads > 0) && (((DataTestType)testType == StreamingLoopback) || ((DataTestType)testType == AsyncLoopback))) {
                            continue;
                        }
#endif
                        
                        // Setup test parameters
                        NumBuffers = numBuffers;
                        BufferByteSize = bufferSize;
                        NumThreads = numThreads;
                        ThreadConcurrency = concurrency;
                        TestType = (DataTestType)testType;
                        UserAllocatedBuffers = true;
                        TestTime = msPerTest / 1000.0; // Seconds
                        
                        // Reset test state
                        NextOutLFSR = 0x1234;
                        NextInLFSR = 0xABCD;
                        BytesRead = 0;
                        BytesWritten = 0;
                        inTransCount = 0;
                        outTransCount = 0;
                        fpgaBertCount = 0;
                        softwareBertCount = 0;
                        GeneralFailures = 0;
                        CompletionRoutineFailure = false;
#ifdef IMPLEMENT_ASYNC
                        nextInRequestID = 0;
                        nextOutRequestID = 0;
                        
                        // Clear any lingering buffers
                        while (PendingBuffersToVerify.size() > 0) {
                            data = PendingBuffersToVerify.front();
                            PendingBuffersToVerify.pop();
                            delete[] data;
                        }
                        
                        map<int, int> emptyMap;
                        swap(threadRequestCount, emptyMap);
#endif
                        
                        // Run the test
                        Test_Run(qusb);
                        
                        // If a test has failed, stop
                        if (Failures > 0) {
                            stop = true;
                        }
                    }
                }
            }
        }
    }
}



void Test_LogStats(CQuickUsb &qusb) {
    // Check that, for Async tests, the number of threads is actually correct
    bool threadOk = true;
    if ((TestType == AsyncRead) || (TestType == AsyncWrite) || (TestType == AsyncLoopback)) {
        QWORD actualNumThreads, actualConcurrency;
        if (!qusb.GetNumAsyncThreads(&actualNumThreads, &actualConcurrency)) {
            LOG_ERR("QuickUsb.GetNumAsyncThreads() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        }

        if ((actualNumThreads != NumThreads) || (actualConcurrency != ThreadConcurrency)) {
            threadOk = false;
            LOG_ERR("ERROR: Number of threads and/or thread concurrency not the proper value for this test" << endl);
        }
    }

#ifdef IMPLEMENT_ASYNC
    // Display the request completion counts for each threaded
#if defined(__APPLE__) || defined(__linux__)
    bool showedMain = false;
    for (std::map<int, int>::iterator it = threadRequestCount.begin(); it != threadRequestCount.end(); ++it) {
        if (it->first == gettid()) {
            LOG("Thread " << it->first << " completed " << it->second << " requests (Main Thread)" << endl);
            showedMain = true;
        } else {
            LOG("Thread " << it->first << " completed " << it->second << " requests (Helper Thread)" << endl);
        }
    }
    if (!showedMain) {
        LOG("Thread " << gettid() << " completed " << 0 << " requests (Main Thread)" << endl);
    }
#endif
#endif
    
    // Keep track of failed tests
    bool failed = (GeneralFailures != 0) || (!threadOk || (fpgaBertCount != 0) || (softwareBertCount != 0) || (inTransCount == 0 && outTransCount == 0) || (BytesRead == 0 && BytesWritten == 0));
    if (failed) {
        ++Failures;
        LOG("Result: FAIL" << endl);
    } else {
        LOG("Result: OK" << endl);
    }
    LOG("Test Failures: " << Failures << " (of " << NumTests << " tests)" << endl);
}


#ifdef IMPLEMENT_ASYNC

QVOIDRETURN AsyncCompletionRoutine(PQBULKSTREAM BulkStream) {
    // Check for general errors
    if (!BulkStream) {
        return;
    }

    // Retrieve our user data
    PQULONG tag = (PQULONG)BulkStream->Tag;
    QULONG requestID = *tag;
    delete tag;
    tag = NULL;

    // Check for errors
    if (BulkStream->Error) {
        LOG_ERR("--> CR: ERROR! Request #" << requestID << "(" << BulkStream->RequestID << ") failed with error " << BulkStream->Error << " (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl);
    } else {
        // Check that the correct number of bytes were transferred
        if (BulkStream->BytesRequested != BulkStream->BytesTransferred) {
            LOG_ERR("Incorrect number of bytes transferred: " << BulkStream->BytesTransferred << " (Expected " << BulkStream->BytesRequested << ")" << endl);
        }

        // Check that the RequestID is correct
        if (requestID != BulkStream->RequestID) {
            LOG_ERR("Request ID is incorrect: " << BulkStream->RequestID << "(Expected " << requestID << ")" << endl);
        }

        //LOG("--> CR: Completed request #" << requestID << "(" << BulkStream->RequestID << ") for " << BulkStream->BytesTransferred << " bytes" << endl);
    }

    // Keep track of how many requests each thread is processing
#if defined(__APPLE__) || defined(__linux__)
    if (threadRequestCount.find(gettid()) == threadRequestCount.end()) {
        threadRequestCount[gettid()] =  1;
    } else {
        ++threadRequestCount[gettid()];
    }
#endif

    //
    // Now that this request has completed we may process the data
    //
}

///////////////////////////////////////
// The BulkStream completion routine //
///////////////////////////////////////
QVOIDRETURN CompletionRoutine(PQBULKSTREAM BulkStream) {
    // Check for general errors
    if (BulkStream == NULL) {
        LOG_ERR("ERROR! BulkStream is null - {1}" << CQuickUsb::GetLastError() << endl);
        return;
    }

    // Keep track of how many requests each thread is processing
#if defined(__APPLE__) || defined(__linux__)
    if (threadRequestCount.find(gettid()) == threadRequestCount.end()) {
        threadRequestCount[gettid()] =  1;
    } else {
        ++threadRequestCount[gettid()];
    }
#endif

    // Check for errors
    if (BulkStream->Error != 0) {
        LOG_ERR("ERROR! " << (((BulkStream->StreamType & 0x10) != 0) ? "IN" : "OUT") << " Request failed - " << BulkStream->Error << " (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl);

        // If the request failed for any reason other than a timeout, we need to stop the test
        //if (BulkStream->Error != CQuickUsb::ErrorTimeout) {
            CompletionRoutineFailure = true;
        //} else {
            // A timeout error occurred on the stream.  By default, the stream shuts down after any
            // error, unless we clear the error to allow the stream to continue.  Since timeouts are
            // not fatal errors, we'll have the stream continue onwards.
        //    BulkStream->Error = CQuickUsb::ErrorNoError;
        //}

        // To prevent lockups, if a request fails we still must increment our request id counter so
        // that we may continue to process subsequent requests
        if ((BulkStream->StreamType & 0x10) != 0) { // IN
            ++nextInRequestID;
        } else { // OUT
            ++nextOutRequestID;
        }

        return;
    } /*else {
        if ((BulkStream->StreamType & 0x10) != 0) { // IN
            LOG("IN  Request #" << BulkStream->RequestID << " completed (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl);
        } else { // OUT
            LOG("OUT Request #" << BulkStream->RequestID << " completed (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl);
        }
    }*/
    
    if ((BulkStream->StreamType & 0x10) != 0) { // IN?
        // If we're multi-threading then it's possible that our completion routines are executing
        // out-of-order.  We need to make sure that we read the appropriate data in the correct
        // order, so if this routine is processing a request a little too early, then we need to
        // wait until other threads process requests and it becomes our turn.
        if (NumThreads > 0) {
            //int tStart = Environment.TickCount;
            while ((nextInRequestID != BulkStream->RequestID)) {// && (Environment.TickCount - tStart) < 5000) {
                // Yield processing to another thread.  This is better than sleeping a finite amount
                // of time and better than an empty loop as we don't waste processing time we don't need;
                SleepMS(0);
            }

            // If (nextRequestID != BulkStream->RequestID) never becomes true, then it is an indication
            // that there is some internal issue with keeping track of the correct request id to process.
            if (nextInRequestID != BulkStream->RequestID) {
                LOG_ERR("Internal Error: Thread is deadlocked and unable to process packet because it is out of order" << endl);
                return;
            }

            // If we're performing a streaming loopback
            if (BulkStream->Tag == (PQVOID)1) {
                // Now that it is our turn, we must also make sure that some writes have occurred before our read
                while ((PendingBuffersToVerify.size() <= 2) && RunningTest) {
                    // Yield processing to another thread.  This is better than sleeping a finite amount
                    // of time and better than an empty loop as we don't waste processing time we don't need.
                    SleepMS(0);
                }
            }
        } else if (nextInRequestID != BulkStream->RequestID) {
            LOG_ERR("Internal Error: Data packet received out of order during single-threaded execution" << endl);
        }

        if (UserAllocatedBuffers) {
            // If we're performing a streaming loopback
            if (BulkStream->Tag == (PQVOID)1) {
                PQWORD data;

                // Ensure that data exists in our queue
                if ((NumThreads == 0) && (PendingBuffersToVerify.size() == 0)) {
                    LOG_ERR("Internal Error: Completed read before a write" << endl);
                } else {
                    while (PendingBuffersToVerify.size() == 0) 
                        ;
                        
                    // Verify the read data
                    data = PendingBuffersToVerify.front();
                    PendingBuffersToVerify.pop();
                    if (/*RunningTest &&*/ PendingBuffersToVerify.size() > 0) {
                        softwareBertCount += CountBitErrors((PQWORD)BulkStream->Buffer, data, BulkStream->BytesTransferred / 2);
                    }
                    delete[] data;
                    data = NULL;
                }
            } else {
                // Get the next set of data to data to verify against
                PQWORD buffer = new QWORD[BulkStream->BytesTransferred / 2];
                GetNextInLFSR(buffer, BulkStream->BytesTransferred / 2);

                // Verify the read data
                if (RunningTest) {
                    softwareBertCount += CountBitErrors((PQWORD)BulkStream->Buffer, buffer, BulkStream->BytesTransferred / 2);
                }

                delete[] buffer;
                buffer = NULL;
            }
        } else {
            // The read data buffer has been allocated internally by the QuickUSB API.  The
            // easiest way to access its data is to cast the IntPtr to QWORD*

            // If we're performing a streaming loopback
            if (BulkStream->Tag == (PQVOID)1) {
                // Ensure that data exists in our queue
                if ((NumThreads == 0) && (PendingBuffersToVerify.size() == 0)) {
                    LOG_ERR("Internal Error: Completed read before a write" << endl);
                } else {
                    while (PendingBuffersToVerify.size() == 0) 
                        ;
                        
                    // Verify the read data
                    if (/*RunningTest &&*/ PendingBuffersToVerify.size() > 0) {
                        PQWORD data = PendingBuffersToVerify.front();
                        PendingBuffersToVerify.pop();
                        softwareBertCount += CountBitErrors((PQWORD)BulkStream->Buffer, data, BulkStream->BytesTransferred / 2);
                        delete[] data;
                        data = NULL;
                    }
                }
            } else {
                // Get the next set of data to data to verify against
                PQWORD buffer = new QWORD[BulkStream->BytesTransferred / 2];
                GetNextInLFSR(buffer, BulkStream->BytesTransferred / 2);

                // Verify the read data
                if (RunningTest) {
                    softwareBertCount += CountBitErrors((PQWORD)BulkStream->Buffer, buffer, BulkStream->BytesTransferred / 2);
                }

                delete[] buffer;
                buffer = NULL;
            }
        }

        ++nextInRequestID;
        ++inTransCount;
        BytesRead += BulkStream->BytesTransferred;

        // If we're stopping a streaming test, then pause the stream here so that
        // requests are not re-issued and don't cause shutdown problems
        if (!RunningTest && (BulkStream->Tag == (PQVOID)1)) {
            // Immediate must be true or we'll deadlock
            QRESULT qResult = QuickUsbPauseStream(BulkStream->Handle, InStreamID, true); 
            if (!qResult) {
                QULONG err;
                QuickUsbGetLastError(&err);
                LOG_ERR("QuickUsb.PauseStream() failed with error: " << err << endl);
            }
        }
    } else { // OUT request
        // If we're multi-threading then it's possible that our completion routines are executing
        // out-of-order.  We need to make sure that we write the appropriate data in the correct
        // order, so if this routine is processing a request a little too early, then we need to
        // wait until other threads process requests and it becomes our turn.
        if (NumThreads > 0) {
            //int tStart = Environment.TickCount;
            while ((nextOutRequestID != BulkStream->RequestID)) {// && (Environment.TickCount - tStart) < 5000) {
                // Yield processing to another thread.  This is better than sleeping a finite amount
                // of time and better than an empty loop as we don't waste processing time we don't need;
                SleepMS(0);
            }

            // If (nextRequestID != BulkStream->RequestID) never becomes true, then it is an indication
            // that there is some internal issue with keeping track of the correct request id to process.
            if (nextOutRequestID != BulkStream->RequestID) {
                LOG_ERR("Internal Error: Thread is deadlocked and unable to process packet because it is out of order" << endl);
                return;
            }

            // Now that it is our turn, we must also make sure that we don't try to perform too many
            // writes before our reads have a chance to get processed
            while ((PendingBuffersToVerify.size() > 2) && RunningTest) {
                // Yield processing to another thread.  This is better than sleeping a finite amount
                // of time and better than an empty loop as we don't waste processing time we don't need;
                SleepMS(0);
            }
        } else if (nextOutRequestID != BulkStream->RequestID) {
            LOG_ERR("Internal Error: Data packet received out of order during single-threaded execution" << endl);
        }

        // If we're performing a streaming loopback
        if (BulkStream->Tag == (PQVOID)1) {
            // We need to store this completed data buffer so we may verify against it later
            // once we have read it back
            PQWORD data = new QWORD[BufferByteSize / 2];
            memcpy(data, BulkStream->Buffer, BufferByteSize);
            PendingBuffersToVerify.push(data);
        }

        // Get the next set of data to write out
        GetNextOutLFSR((PQWORD)BulkStream->Buffer, BufferByteSize / 2);
        ++nextOutRequestID;

        ++outTransCount;
        BytesWritten += BulkStream->BytesTransferred;

        // If we're stopping a streaming test, then pause the stream here so that
        // requests are not re-issued and don't cause shutdown problems
        if (!RunningTest && (BulkStream->Tag == (PQVOID)1)) {
            //QRESULT qResult = qusb.PauseStream(OutStreamID, true);
            QRESULT qResult = QuickUsbPauseStream(BulkStream->Handle, OutStreamID, true);
            if (!qResult)  {
                QULONG err;
                QuickUsbGetLastError(&err);
                LOG_ERR("QuickUsb.PauseStream() failed with error: " << err << endl);
            }
        }
    }

    //LOG("Completed " << (((BulkStream->StreamType & 0x10) != 0) ? "IN" : "OUT") << " Request #" << BulkStream->RequestID << " for " << BulkStream->BytesTransferred << " bytes" << endl);
}

#endif


void Test_Run(CQuickUsb &qusb) {
    QRESULT qResult;
    PQWORD outBuffer = NULL;
    PQWORD inBuffer = NULL;
    PQWORD *inBufferArray = NULL;
    PQWORD buffer = NULL;
    QULONG length;
#ifdef IMPLEMENT_ASYNC
    PQBULKSTREAM outBulkStream = NULL; // For new async
    PQBULKSTREAM inBulkStream = NULL;  // For new async
    int issuedInRequests, issuedOutRequests;
    int nextInRequest, nextOutRequest;
    PQBYTE transID = NULL;
#endif
    PlatformIndependentTimer updateTimer;
    QBOOL opened = false;
    QULONG requestID;
    FILE *hFile;
    QFLOAT statValue;
    QULONG flags;
    //QMEMFILEHANDLE hMemFile;
    PQWORD memFilePtr;
    QULONG memFileSize;

    // Start a test
    ++NumTests;
    RunningTest = true;

    // Reset test state
    if (!Test_ResetTestState(qusb)) {
        ++Failures;
        Test_Stop(qusb, false);
        return;
    }

    // Configure FPGA
    if (!Test_InitFpga(qusb)) {
        ++Failures;
        Test_Stop(qusb, false);
        return;
    }

    LOG("*** Starting test " << (int)TestType << " ***"  << endl);
    
    // Update stats
    /*if (!Test_DisplayStats(qusb)) {
        ++Failures;
        Test_Stop(qusb, false);
        return;
    }*/

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Log information about the test setup
    LOG("Start Time: " << asctime(timeinfo) << endl);
    LOG("NumBuffers = " << NumBuffers << endl);
    LOG("BufferByteSize = " << BufferByteSize << endl);
    LOG("NumThreads = " << NumThreads << endl);
    LOG("Concurrency = " << ThreadConcurrency << endl);
    //LOG("Config Register = 0x" + btConfig.Value.ToString("X2") << endl);
    if ((TestType == StreamingRead) || (TestType == StreamingLoopback)) {
        LOG("User allocated buffers = " << UserAllocatedBuffers << endl);
    }
    LOG(endl);

    // Start timing
    timer.Start();
    updateTimer.Start();

    switch (TestType) {
    case Start:
        break;

    case End:
        break;

    /********************************************************/
    case SyncRead:
    /********************************************************/
        // Create data buffer
        inBuffer = new QWORD[BufferByteSize / 2];
        memset(inBuffer, 0, BufferByteSize);
        buffer = new QWORD[BufferByteSize / 2];
        memset(buffer, 0, BufferByteSize);

        // Loop until the test ends
        do {
            // Perform the synchronous data read
            length = (unsigned int) BufferByteSize;
            qResult = qusb.ReadData((PQBYTE)inBuffer, &length);
            if (!qResult) {
                LOG_ERR("QuickUsb.ReadData() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            } else {
                // Sanity check that we read the appropriate number of bytes
                if (length != BufferByteSize) {
                    LOG_ERR("QuickUsb.ReadData() Succeeded with incorrect length: " << length << ' ' << BufferByteSize << "expected)" << endl);
                }

                // Get the next set of data to verify against
                GetNextInLFSR(buffer, BufferByteSize / 2);

                // Verify the read data
                softwareBertCount += CountBitErrors(inBuffer, buffer, BufferByteSize / 2);

                // Successfuly data transfer
                ++inTransCount;
                BytesRead += BufferByteSize;
            }

            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        } while (RunningTest);

        break;


        
    /********************************************************/
    case SyncWrite:
    /********************************************************/
        // Create data buffer
        outBuffer = new QWORD[BufferByteSize / 2];
        memset(outBuffer, 0, BufferByteSize);

        // Get the next set of data to write out
        GetNextOutLFSR(outBuffer, BufferByteSize / 2);

        // Loop until the test ends
        do {
            // Perform the synchronous data write
            qResult = qusb.WriteData((PQBYTE)outBuffer, (unsigned int) BufferByteSize);
            if (!qResult) {
                LOG_ERR("QuickUsb.WriteData() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            } else {
                // Get the next set of data to write out
                GetNextOutLFSR(outBuffer, BufferByteSize / 2);

                // Successfuly data transfer
                ++outTransCount;
                BytesWritten += BufferByteSize;
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        } while (RunningTest);
        
        break;


        
    /********************************************************/
    case SyncLoopback:
    /********************************************************/
        // Create data buffer
        inBuffer = new QWORD[BufferByteSize / 2];
        memset(inBuffer, 0, BufferByteSize);
        outBuffer = new QWORD[BufferByteSize / 2];
        memset(outBuffer, 0, BufferByteSize);

        // Get the next set of data to write out
        GetNextOutLFSR(outBuffer, BufferByteSize / 2);

        // Loop until the test ends
        do {
            // Perform the synchronous data write
            qResult = qusb.WriteData((PQBYTE)outBuffer, (unsigned int) BufferByteSize);
            if (!qResult) {
                LOG_ERR("QuickUsb.WriteData() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            } else {
                // Perform the synchronous data read
                length = (unsigned int) BufferByteSize;
                qResult = qusb.ReadData((PQBYTE)inBuffer, &length);
                if (!qResult) {
                    LOG_ERR("QuickUsb.ReadData() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    break;
                } else {
                    // Check that the correct number of bytes were transferred
                    if (length != BufferByteSize) {
                        LOG_ERR("QuickUsb.ReadData() succeeded with incorrect length: " << length << ' ' << BufferByteSize << "expected)" << endl);
                        break;
                    }

                    // Verify the read data
                    softwareBertCount += CountBitErrors(inBuffer, outBuffer, BufferByteSize / 2);

                    // Successfuly read data transfer
                    ++inTransCount;
                    BytesRead += BufferByteSize;
                }

                // Get the next set of data to write out
                GetNextOutLFSR(outBuffer, BufferByteSize / 2);

                // Successfuly write data transfer
                ++outTransCount;
                BytesWritten += BufferByteSize;
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        } while (RunningTest);
        
        break;


        
#ifdef IMPLEMENT_ASYNC
    /********************************************************/
    case AsyncRead:
    /********************************************************/
        buffer = new QWORD[BufferByteSize / 2];
        memset(buffer, 0, BufferByteSize);

        if (!qusb.Open()) {
            LOG_ERR("QuickUsb.Open() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }
        opened = true;

        // Set the number of threads
        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency)) {
            LOG_ERR("QuickUsb.SetNumAsyncThreads() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }

        // Issue initial set of read requests
        inBulkStream = new QBULKSTREAM[NumBuffers];
        memset(inBulkStream, 0, sizeof(QBULKSTREAM) * NumBuffers);
        issuedInRequests = 0;
        requestID = 0;
        for (int k = 0; k < NumBuffers; ++k) {
            PQULONG tag = new QULONG;
            *tag = requestID++;
            length = BufferByteSize;
            memset(InBufferArray[k], 0, BufferByteSize / 2);
            qResult = qusb.ReadBulkDataAsync((PQBYTE)InBufferArray[k], length, &inBulkStream[k], &AsyncCompletionRoutine, tag);
            if (!qResult) {
                LOG_ERR("QuickUsb.ReadBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                RunningTest = false;
                issuedInRequests = 0;
                break;
            }
            ++issuedInRequests;
        }

        // Loop until the test ends
        nextInRequest = 0;
        while (RunningTest || (issuedInRequests > 0)) {
            // Wait for a request to complete
            qResult = qusb.BulkWait(&inBulkStream[nextInRequest], false);
            if (!qResult) {
                LOG_ERR("QuickUsb.BulkWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            } else {
                // Check that the request succeeded
                if (inBulkStream[nextInRequest].Error != CQuickUsb::ErrorNoError) {
                    LOG_ERR("Request failed with error: " << inBulkStream[nextInRequest].Error << endl);
                    break;
                }

                // Check that the correct number of bytes were transferred
                if (inBulkStream[nextInRequest].BytesRequested != inBulkStream[nextInRequest].BytesTransferred) {
                    LOG_ERR("Read incorrect number of bytes: " << inBulkStream[nextInRequest].BytesTransferred << " (Expected " << inBulkStream[nextInRequest].BytesRequested << ")" << endl);
                    break;
                }

                // Get the next set of data to verify against
                GetNextInLFSR(buffer, BufferByteSize / 2);

                // Verify the read data
                softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], buffer, BufferByteSize / 2);

                // Successfuly data transfer
                ++inTransCount;
                BytesRead += inBulkStream[nextInRequest].BytesTransferred;
                --issuedInRequests;

                // Re-issue the data request
                if (RunningTest) {
                    PQULONG tag = new QULONG;
                    *tag = requestID++;
                    length = BufferByteSize;
                    memset(InBufferArray[nextInRequest], 0, BufferByteSize / 2);
                    qResult = qusb.ReadBulkDataAsync((PQBYTE)InBufferArray[nextInRequest], length, &inBulkStream[nextInRequest], &AsyncCompletionRoutine, tag);
                    if (!qResult) {
                        LOG_ERR("QuickUsb.ReadBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                    ++issuedInRequests;
                }

                // Move to next buffer to wait on
                if (++nextInRequest >= NumBuffers) {
                    nextInRequest = 0;
                }
            }

            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        }

        break;

        

    /********************************************************/
    case AsyncWrite:
    /********************************************************/
        if (!qusb.Open()) {
            LOG_ERR("QuickUsb.Open() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }
        opened = true;

        buffer = new QWORD[BufferByteSize / 2];
        memset(buffer, 0, BufferByteSize);

        // Set the number of threads
        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency)) {
            LOG_ERR("QuickUsb.SetNumAsyncThreads() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }

        // Issue initial set of write requests
        outBulkStream = new QBULKSTREAM[NumBuffers];
        memset(outBulkStream, 0, sizeof(QBULKSTREAM) * NumBuffers);
        issuedOutRequests = 0;
        requestID = 0;
        for (int k = 0; k < NumBuffers; ++k) {
            // Get the next set of data to write out
            GetNextOutLFSR(OutBufferArray[k], BufferByteSize / 2);

            PQULONG tag = new QULONG;
            *tag = requestID++;
            length = BufferByteSize;
            qResult = qusb.WriteBulkDataAsync((PQBYTE)OutBufferArray[k], length, &outBulkStream[k], &AsyncCompletionRoutine, tag);
            if (!qResult) {
                LOG_ERR("QuickUsb.WriteBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                RunningTest = false;
                issuedOutRequests = 0;
                break;
            }
            ++issuedOutRequests;
        }

        // Loop until the test ends
        nextOutRequest = 0;
        while (RunningTest || (issuedOutRequests > 0)) {
            // Wait for a request to complete
            qResult = qusb.BulkWait(&outBulkStream[nextOutRequest], false);
            if (!qResult) {
                LOG_ERR("QuickUsb.BulkWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            } else {
                // Check that the request succeeded
                if (outBulkStream[nextOutRequest].Error != CQuickUsb::ErrorNoError) {
                    LOG_ERR("Request failed with error: " << outBulkStream[nextOutRequest].Error << endl);
                    break;
                }

                // Check that the correct number of bytes were transferred
                if (outBulkStream[nextOutRequest].BytesRequested != outBulkStream[nextOutRequest].BytesTransferred) {
                    LOG_ERR("Wrote incorrect number of bytes: " << outBulkStream[nextOutRequest].BytesTransferred << " (Expected " << outBulkStream[nextOutRequest].BytesRequested << ")" << endl);
                    break;
                }

                // Successful data transfer
                ++outTransCount;
                BytesWritten += outBulkStream[nextOutRequest].BytesTransferred;
                --issuedOutRequests;

                // Re-issue the data request
                if (RunningTest) {
                    // Get the next set of data to verify against
                    GetNextOutLFSR(OutBufferArray[nextOutRequest], BufferByteSize / 2);

                    PQULONG tag = new QULONG;
                    *tag = requestID++;
                    length = BufferByteSize;
                    qResult = qusb.WriteBulkDataAsync((PQBYTE)OutBufferArray[nextOutRequest], length, &outBulkStream[nextOutRequest], &AsyncCompletionRoutine, tag);
                    if (!qResult) {
                        LOG_ERR("QuickUsb.WriteBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                    ++issuedOutRequests;
                }

                // Move to next buffer to wait on
                if (++nextOutRequest >= NumBuffers) {
                    nextOutRequest = 0;
                }
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        }

        break;

        

    /********************************************************/
    case AsyncLoopback:
    /********************************************************/
        if (!qusb.Open()) {
            LOG_ERR("QuickUsb.Open() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }
        opened = true;
        
        // Set the number of threads
        if (!qusb.SetNumAsyncThreads(NumThreads, ThreadConcurrency)) {
            LOG_ERR("QuickUsb.SetNumAsyncThreads() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }
        
        outBulkStream = new QBULKSTREAM[NumBuffers];
        memset(outBulkStream, 0, sizeof(QBULKSTREAM) * NumBuffers);
        inBulkStream = new QBULKSTREAM[NumBuffers];
        memset(inBulkStream, 0, sizeof(QBULKSTREAM) * NumBuffers);
        issuedOutRequests = 0;
        issuedInRequests = 0;
        requestID = 0;
        requestID = 0;
        for (int k = 0; k < NumBuffers; ++k) {
            //
            // Issue initial write requests
            //

            // Get the next set of data to write out
            GetNextOutLFSR(OutBufferArray[k], BufferByteSize / 2);

            PQULONG tag = new QULONG;
            *tag = requestID++;
            length = BufferByteSize;
            qResult = qusb.WriteBulkDataAsync((PQBYTE)OutBufferArray[k], length, &outBulkStream[k], &AsyncCompletionRoutine, tag);
            if (!qResult) {
                LOG_ERR("QuickUsb.WriteBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                RunningTest = false;
                issuedInRequests = 0;
                issuedOutRequests = 0;
                break;
            }

            ++issuedOutRequests;

            //
            // Issue initial read requests
            //
            tag = new QULONG;
            *tag = requestID++;
            length = BufferByteSize;
            qResult = qusb.ReadBulkDataAsync((PQBYTE)InBufferArray[k], length, &inBulkStream[k], &AsyncCompletionRoutine, tag);
            if (!qResult) {
                LOG_ERR("QuickUsb.ReadBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                RunningTest = false;
                issuedInRequests = 0;
                issuedOutRequests = 0;
                break;
            }

            ++issuedInRequests;
        }

        // Loop until the test ends
        nextOutRequest = 0;
        nextInRequest = 0;
        while (RunningTest || (issuedInRequests > 0) || (issuedOutRequests > 0)) {
            //
            // Check if the next OUT request has completed
            //
            if (issuedOutRequests > 0) {
                qResult = qusb.BulkWait(&outBulkStream[nextOutRequest], false);
                if (!qResult) {
                    if (qusb.LastError() != CQuickUsb::ErrorNotCompleted) {
                        LOG_ERR("QuickUsb.BulkWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                } else {
                    // Check that the request succeeded
                    if (outBulkStream[nextOutRequest].Error == CQuickUsb::ErrorNoError) {
                        PQWORD data;

                        // Check that the correct number of bytes were transferred
                        if (outBulkStream[nextOutRequest].BytesRequested != outBulkStream[nextOutRequest].BytesTransferred) {
                            LOG_ERR("Wrote incorrect number of bytes: " << outBulkStream[nextOutRequest].BytesTransferred << " (Expected " << outBulkStream[nextOutRequest].BytesRequested << ")" << endl);
                            break;
                        }

                        // Successfuly data transfer
                        BytesWritten += outBulkStream[nextOutRequest].BytesTransferred;

                        // We need to store this completed data buffer so we may verify against it later
                        // once we have read it back
                        data = new QWORD[BufferByteSize / 2];
                        memcpy(data, OutBufferArray[nextOutRequest], BufferByteSize);
                        PendingBuffersToVerify.push(data);
                    } else {
                        LOG_ERR("Bulk Write Request failed with error: " << outBulkStream[nextOutRequest].Error << endl);
                        break;
                    }
                    ++outTransCount;
                    --issuedOutRequests;

                    // Re-issue the data request
                    if (RunningTest) {
                        // Get the next set of data to verify against
                        GetNextOutLFSR(OutBufferArray[nextOutRequest], BufferByteSize / 2);

                        PQULONG tag = new QULONG;
                        *tag = requestID++;
                        length = BufferByteSize;
                        qResult = qusb.WriteBulkDataAsync((PQBYTE)OutBufferArray[nextOutRequest], length, &outBulkStream[nextOutRequest], &AsyncCompletionRoutine, tag);
                        if (!qResult) {
                            LOG_ERR("QuickUsb.WriteBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                            break;
                        }
                        ++issuedOutRequests;
                    }

                    // Move to next buffer to wait on
                    if (++nextOutRequest >= NumBuffers) {
                        nextOutRequest = 0;
                    }
                }
            }

            //
            // Check if the next IN request has completed
            //
            if ((issuedInRequests > 0)  && (PendingBuffersToVerify.size() > 0)) {
                qResult = qusb.BulkWait(&inBulkStream[nextInRequest], false);
                if (!qResult) {
                    if (qusb.LastError() != CQuickUsb::ErrorNotCompleted) {
                        LOG_ERR("QuickUsb.BulkWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                } else {
                    PQWORD data;

                    // Check that the request succeeded
                    if (inBulkStream[nextInRequest].Error == CQuickUsb::ErrorNoError) {
                        // Check that the correct number of bytes were transferred
                        if (inBulkStream[nextInRequest].BytesRequested != inBulkStream[nextInRequest].BytesTransferred) {
                            LOG_ERR("Read incorrect number of bytes: " << inBulkStream[nextInRequest].BytesTransferred << " (Expected " << inBulkStream[nextInRequest].BytesRequested << ")" << endl);
                            break;
                        }

                        // Verify the read data
                        data = PendingBuffersToVerify.front();
                        PendingBuffersToVerify.pop();
                        softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], data, BufferByteSize / 2);
                        delete[] data;
                        data = NULL;

                        // Successful data transfer
                        BytesRead += inBulkStream[nextInRequest].BytesTransferred;
                    } else {
                        LOG_ERR("Bulk Read Request failed with error: " << inBulkStream[nextInRequest].Error << endl);
                        break;
                    }
                    ++inTransCount;
                    --issuedInRequests;

                    // Re-issue the data request.  We also need to make sure that for every write we issue a read
                    // so that all data is drained from the FPGA FIFOs.
                    if (RunningTest || (outTransCount > inTransCount)) {
                        length = BufferByteSize;
                        PQULONG tag = new QULONG;
                        *tag = requestID++;
                        qResult = qusb.ReadBulkDataAsync((PQBYTE)InBufferArray[nextInRequest], length, &inBulkStream[nextInRequest], &AsyncCompletionRoutine, tag);
                        if (!qResult) {
                            LOG_ERR("QuickUsb.ReadBulkDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                            break;
                        }
                        ++issuedInRequests;
                    }

                    // Move to next buffer to wait on
                    if (++nextInRequest >= NumBuffers) {
                        nextInRequest = 0;
                    }
                }
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
        }

        break;

        

        /********************************************************/
        case LegacyAsyncRead:
        /********************************************************/
            // NOTE: Only supported on Windows
            
            buffer = new QWORD[BufferByteSize / 2];
            memset(buffer, 0, BufferByteSize);

            // Issue initial set of read requests
            transID = new QBYTE[NumBuffers];
            memset(transID, 0, sizeof(QBYTE) * NumBuffers);
            issuedInRequests = 0;
            for (int k = 0; k < NumBuffers; ++k) {
                length = BufferByteSize;
                qResult = qusb.ReadDataAsync((PQBYTE) InBufferArray[k], &length, &transID[k]);
                if (!qResult) {
                    LOG_ERR("QuickUsb.ReadDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    RunningTest = false;
                    issuedInRequests = 0;
                    break;
                }
                ++issuedInRequests;
            }

            // Loop until the test ends
            nextInRequest = 0;
            while (RunningTest || (issuedInRequests > 0)) {
                // Wait for a request to complete
                qResult = qusb.AsyncWait(&length, transID[nextInRequest], false);
                if (!qResult) {
                    LOG_ERR("QuickUsb.AsyncWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    break;
                }
                
                // Check that the transaction completed
                if (length == 0) {
                    LOG_ERR("Request not fully completed: " << length << " (Expected " << BufferByteSize << ")" << endl);
                    break;
                }

                // Check that the correct number of bytes were transferred
                if (length != BufferByteSize) {
                    LOG_ERR("Read incorrect number of bytes: " << length << " (Expected " << BufferByteSize << ")" << endl);
                    break;
                }

                // Get the next set of data to verify against
                GetNextInLFSR(buffer, BufferByteSize / 2);

                // Verify the read data
                softwareBertCount += CountBitErrors(InBufferArray[nextInRequest], buffer, BufferByteSize / 2);

                // Successfuly data transfer
                ++inTransCount;
                BytesRead += length;
                --issuedInRequests;

                // Re-issue the data request
                if (RunningTest) {
                    length = BufferByteSize;
                    qResult = qusb.ReadDataAsync((PQBYTE)InBufferArray[nextInRequest], &length, &transID[nextInRequest]);
                    if (!qResult) {
                        LOG_ERR("QuickUsb.ReadDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                    ++issuedInRequests;
                }

                // Move to next buffer to wait on
                if (++nextInRequest >= NumBuffers) {
                    nextInRequest = 0;
                }
                
                // Is it time to end the test
                if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                    RunningTest = false;
                } else {
                    // Update the stats every once in a while
                    tElapsed = updateTimer.GetElapsedTimeInSeconds();
                    if (tElapsed >= UpdateFreq) {
                        Test_DisplayStats(qusb);
                        updateTimer.Start();
                    }
                }
            }

            break;

            

        /********************************************************/
        case LegacyAsyncWrite:
        /********************************************************/
            // NOTE: Only supported on Windows
            
            buffer = new QWORD[BufferByteSize / 2];
            memset(buffer, 0, BufferByteSize);

            // Issue initial set of read requests
            transID = new QBYTE[NumBuffers];
            memset(transID, 0, sizeof(QBYTE) * NumBuffers);
            issuedOutRequests = 0;
            for (int k = 0; k < NumBuffers; ++k) {
                // Get the next set of data to write out
                GetNextOutLFSR(OutBufferArray[k], BufferByteSize / 2);

                length = BufferByteSize;
                qResult = qusb.WriteDataAsync((PQBYTE)OutBufferArray[k], length, &transID[k]);
                if (!qResult) {
                    LOG_ERR("QuickUsb.WriteDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    RunningTest = false;
                    issuedOutRequests = 0;
                    break;
                }
                ++issuedOutRequests;
            }

            // Loop until the test ends
            nextOutRequest = 0;
            while (RunningTest || (issuedOutRequests > 0)) {
                // Wait for a request to complete
                qResult = qusb.AsyncWait(&length, transID[nextOutRequest], false);
                if (!qResult) {
                    LOG_ERR("QuickUsb.AsyncWait() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    break;
                }
                
                // Check that the transaction completed
                if (length == 0) {
                    LOG_ERR("Request not fully completed: " << length << " (Expected " << BufferByteSize << ")" << endl);
                    break;
                }

                // Check that the correct number of bytes were transferred
                if (length != BufferByteSize) {
                    LOG_ERR("Wrote incorrect number of bytes: " << length << " (Expected " << BufferByteSize << ")" << endl);
                    break;
                }

                // Successfuly data transfer
                ++outTransCount;
                BytesWritten += length;
                --issuedOutRequests;

                // Re-issue the data request
                if (RunningTest) {
                    // Get the next set of data to write out
                    GetNextOutLFSR(OutBufferArray[nextOutRequest], BufferByteSize / 2);

                    length = BufferByteSize;
                    qResult = qusb.WriteDataAsync((PQBYTE)OutBufferArray[nextOutRequest], length, &transID[nextOutRequest]);
                    if (!qResult) {
                        LOG_ERR("QuickUsb.WriteDataAsync() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }

                    ++issuedOutRequests;
                }

                // Move to next buffer to wait on
                if (++nextOutRequest >= NumBuffers) {
                    nextOutRequest = 0;
                }
                
                // Is it time to end the test
                if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                    RunningTest = false;
                } else {
                    // Update the stats every once in a while
                    tElapsed = updateTimer.GetElapsedTimeInSeconds();
                    if (tElapsed >= UpdateFreq) {
                        Test_DisplayStats(qusb);
                        updateTimer.Start();
                    }
                }
            }

            break;


            
    /********************************************************/
    case StreamingRead:
    /********************************************************/
		inBufferArray = (UserAllocatedBuffers) ? (InBufferArray) : (NULL);

		if (StreamTestType == Memory) {
            // Stream to data buffers in memory
			qResult = qusb.ReadBulkDataStartStream((PQBYTE *) inBufferArray, NumBuffers, (QULONG)BufferByteSize, CompletionRoutine, NULL, &InStreamID, NumThreads, ThreadConcurrency);
			if (!qResult) {
				LOG_ERR("QuickUsb.ReadBulkDataStartStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
				break;
			}
		} else {
            memFileSize = (100 * 1024 * 1024); // 100 MB

            // To keep track of the number of bytes transferred, we will use the Statistics API
            qResult = qusb.ResetStatistic(CQuickUsb::StatAll);
            if (!qResult) {
				LOG_ERR("QuickUsb.ResetStatistic() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
				break;
            }

            // Stream to a file (either on disk or in system memory)
            flags = QUICKUSB_STREAM_CREATE_ALWAYS;   // Overwrite any pre-existing file
            //if (StreamTestType == MemoryFile) {
            //    flags |= QUICKUSB_STREAM_MEMORY_FILE;       // Write to a shared memory file
            //}
			qResult = qusb.ReadBulkDataStartStreamToFile(InFileName, NumBuffers, (QULONG)BufferByteSize, memFileSize / BufferByteSize, &InStreamID, flags);
			if (!qResult) {
				LOG_ERR("QuickUsb.ReadBulkDataStartStreamToFile() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
				break;
			}
		}

        // Loop until the test ends
        do {
            if (StreamTestType == Memory) {
                if (NumThreads == 0) {
                    // For single-threaded streams, we must process the streams on this main thread
                    // explicitly.  The ProcessStream function will block this thread until either a
                    // data request has completed or until the timeout has elapsed.  A timeout
                    // indicates that the stream simply did not process any data requests in the given
                    // amount of time, not that a data request timed out.
                    qResult = qusb.ProcessStream(InStreamID, 50);
                    if (!qResult && (qusb.LastError() != CQuickUsb::ErrorTimeout)) {
                        LOG_ERR("QuickUsb.ProcessStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                        break;
                    }
                } else {
                    // For multi-threaded streams, the worker threads will automatically process streams so we
                    // don't have to worry about it.  We just want to be sure to not hog the processor.
                    SleepMS(50);
                }
            } else {
                // For reading to files, we don't have to do anything but wait
                SleepMS(50);
            }
                
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
            
            // If a transaction failed in the completion routine, we need to stop the test
            if (CompletionRoutineFailure) {
                break;
            }
        } while (RunningTest);

        //if (StreamTestType == MemoryFile) {
            // Before we stop the stream we need to open the memory file or all references to it will be closed
            // and it will be cleaned up by the OS
            //qResult = CQuickUsb::OpenMemoryFile(InFileName, memFileSize, &hMemFile, (PQBYTE *)&memFilePtr, 0);
            //if (!qResult) {
            //    LOG_ERR("QuickUsb.OpenMemoryFile() failed with error: " << CQuickUsb::GetLastError() << endl);
            //    break;
            //}
        //}

        // Stop the stream
        LOG("Stopping Stream..." << endl);
        qResult = qusb.StopStream(InStreamID, false); // Block until stream stops
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        } else {
            LOG("Streaming successfully stopped" << endl);
        }

        // Get the number of bytes read
        qResult = qusb.GetStatistic(CQuickUsb::StatTotalDataRead, CQuickUsb::StatUnitBytes, &statValue, 0);
        if (!qResult) {
			LOG_ERR("QuickUsb.GetStatistic() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
			break;
        }
            
        // We must go back and read through the file to verify each bit is correct
        if (StreamTestType == DiskFile) {
            // Open the disk file
            hFile = fopen(InFileName, "rb");
            size_t bytesRead = 0;

            // Create data buffer
            inBuffer = new QWORD[BufferByteSize / 2];
            buffer = new QWORD[BufferByteSize / 2];

            // Loop until the end of the file
            BytesRead = 0;            
            do {
                // Clear memory to zero
                memset(inBuffer, 0, BufferByteSize);
                memset(buffer, 0, BufferByteSize);

                // Read a chunk of data from the file
                bytesRead = fread(inBuffer, sizeof(QBYTE), BufferByteSize, hFile);
                if ((bytesRead == 0) && (feof(hFile))) {
                    break;
                }

                // Verify that all the data was read
                if (bytesRead != BufferByteSize) {
                    LOG_ERR("Read incorrect number of bytes from disk file (" << bytesRead << " of " << BufferByteSize << ")" << endl);
                }
                BytesRead += bytesRead;
                ++inTransCount;

                // Verify the data, bit-by-bit
                GetNextInLFSR(buffer, BufferByteSize / 2);
                softwareBertCount += CountBitErrors(buffer, inBuffer, (bytesRead / 2));
            } while (true);

            // Close the file
            fclose(hFile);
            hFile = NULL;
            
            // Verify that the number of total bytes read matches up with the file size
            if ((QFLOAT)BytesRead != statValue) {
				LOG_ERR("File size does not match bytes read: " << ((int)BytesRead - (int)statValue) << endl);
            }
        }
        //else if (StreamTestType == MemoryFile) {
        //    QULONG wordCount;

        //    // Create data buffer
        //    buffer = new QWORD[BufferByteSize / 2];

        //    // Loop until the end of the file
        //    BytesRead = (QULONG) statValue;  
        //    wordCount = 0;
        //    do {
        //        // Clear memory to zero
        //        memset(buffer, 0, BufferByteSize);

        //        // Verify the data, bit-by-bit
        //        GetNextInLFSR(buffer, BufferByteSize / 2);
        //        softwareBertCount += CountBitErrors(buffer, memFilePtr + wordCount, (BufferByteSize / 2));
        //        
        //        // Keep count
        //        wordCount += BufferByteSize / 2;
        //        ++inTransCount;
        //    } while (wordCount < (BytesRead / 2));
        //    
        //     Close the memory file
        //    if (!CQuickUsb::CloseMemoryFile(&hMemFile, (PQBYTE *)&memFilePtr, memFileSize, 0)) {
        //        LOG_ERR("QuickUsb.CloseMemoryFile() failed with error: " << CQuickUsb::GetLastError() << endl);
        //    }
        //}
        break;


        
    /********************************************************/
    case StreamingWrite:
    /********************************************************/
        memFileSize = (100 * 1024 * 1024);
		if (StreamTestType == Memory) {
            // Fill output buffers with initial data
            for (int k = 0; k < NumBuffers; ++k) {
                // Get the next set of data to write out
                GetNextOutLFSR(OutBufferArray[k], BufferByteSize / 2);
            }

            // Start data stream
            qResult = qusb.WriteBulkDataStartStream((PQBYTE*) OutBufferArray, NumBuffers, (QULONG)BufferByteSize, CompletionRoutine, NULL, &OutStreamID, NumThreads, ThreadConcurrency);
            if (!qResult) {
                LOG_ERR("QuickUsb.WriteBulkDataStartStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            }
        } else if (StreamTestType == DiskFile) {
            // We must create a file containing data to write out (100 MB)
            hFile = fopen(OutFileName, "wb");
            buffer = new QWORD[memFileSize / 2];
            GetNextOutLFSR(buffer, memFileSize / 2);
            fwrite(buffer, sizeof(QWORD), memFileSize / 2, hFile);
            fclose(hFile);
            hFile = NULL;
            delete[] buffer;
            buffer = NULL;
        }
        //else if (StreamTestType == MemoryFile) {
        //     We must create a memory file containing data to write out (100 MB)
        //    qResult = CQuickUsb::OpenMemoryFile(OutFileName, memFileSize, &hMemFile, (PQBYTE *)&memFilePtr, CQuickUsb::CreateAlways);
        //    if (!qResult) {
        //        LOG_ERR("QuickUsb.OpenMemoryFile() failed with error: " << CQuickUsb::GetLastError() << endl);
        //        break;
        //    }

        //     Fill the buffers with the LFSR data
        //    GetNextOutLFSR(memFilePtr, memFileSize / 2);
        //}
        
        // To keep track of the number of bytes transferred, we will use the Statistics API
        qResult = qusb.ResetStatistic(CQuickUsb::StatAll);
        if (!qResult) {
			LOG_ERR("QuickUsb.ResetStatistic() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
			break;
        }

        // Stream from a file (either on disk or in system memory)
        //flags = 0;
        //if (StreamTestType == MemoryFile) {
        //    flags |= QUICKUSB_STREAM_MEMORY_FILE;       // Write to a shared memory file
        //}
		//qResult = qusb.WriteBulkDataStartStreamFromFile(OutFileName, NumBuffers, (QULONG)BufferByteSize, memFileSize / BufferByteSize, &OutStreamID, flags);
		//if (!qResult) {
		//	LOG_ERR("QuickUsb.WriteBulkDataStartStreamFromFile() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
		//	break;
		//}

        // Reset the timer so we don't measure the time it took to create the out file
        timer.Start();

        // Loop until the test ends
        do {
		    if (StreamTestType == Memory) {
                if (NumThreads == 0) {
                    // For single-threaded streams, we must process the streams on this main thread
                    // explicitly.  The ProcessStream function will block this thread until either a
                    // data request has completed or until the timeout has elapsed.  A timeout
                    // indicates that the stream simply did not process any data requests in the given
                    // amount of time, not that a data request timed out.
                    qResult = qusb.ProcessStream(OutStreamID, 50);
                    if (!qResult) {
                        if (qusb.LastError() != CQuickUsb::ErrorTimeout) {
                            LOG_ERR("QuickUsb.ProcessStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                            break;
                        }
                    }
                } else {
                    // For multi-threaded streams, the worker threads will automatically process streams so we
                    // don't have to worry about it (although we could if we wanted to, but then the true number
                    // of threads be be one higher than NumThreads).  For now, just don't hog the processor.
                    SleepMS(50);
                }
            } else {
                QULONG status, error;

                // For writing files, we should periodically check the stream status to see if we have reached
                // the end of the file
                qResult = qusb.GetStreamStatus(OutStreamID, &status, &error);
			    if (!qResult) {
				    LOG_ERR("QuickUsb.GetStreamStatus() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
				    break;
			    }
                if (error == CQuickUsb::ErrorFileEOF) {
                    LOG("End of file reached...Quitting test early" << endl);
                    break;
                }

                // Otherwise, don't hog the processor
                SleepMS(50);
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
            
            // If a transaction failed in the completion routine, we need to stop the test
            if (CompletionRoutineFailure) {
                RunningTest = false;
            }
        } while (RunningTest);

        // Stop the stream
        LOG("Stopping Stream..." << endl);
        qResult = qusb.StopStream(OutStreamID, false); // Block until stream stops
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        } else {
            LOG("Streaming successfully stopped" << endl);
        }

        // Read the total number of bytes written
        QFLOAT statValue;
        qResult = qusb.GetStatistic(CQuickUsb::StatTotalDataWritten, CQuickUsb::StatUnitBytes, &statValue, 0);
        if (!qResult) {
			LOG_ERR("QuickUsb.GetStatistic() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
			break;
        }
        BytesWritten = (QULONG) statValue;
            
        // We don't have any way to know how many writes were performed, so we'll calculate it
        outTransCount = BytesWritten / BufferByteSize;

        // Close the memory file
        //if (StreamTestType == MemoryFile) {
        //    if (!CQuickUsb::CloseMemoryFile(&hMemFile, (PQBYTE *)&memFilePtr, memFileSize, 0)) {
        //        LOG_ERR("QuickUsb.CloseMemoryFile() failed with error: " << CQuickUsb::GetLastError() << endl);
        //    }
        //}
        break;


        
    /********************************************************/
    case StreamingLoopback:
    /********************************************************/
        // Create the write buffer array
        for (int k = 0; k < NumBuffers; ++k) {
            // Get the next set of data to write out
            GetNextOutLFSR(OutBufferArray[k], BufferByteSize / 2);
        }

        // Start write data stream
        qResult = qusb.WriteBulkDataStartStream((PQBYTE*) OutBufferArray, NumBuffers, BufferByteSize, CompletionRoutine, (PQVOID)1, &OutStreamID, NumThreads, ThreadConcurrency);
        if (!qResult) {
            LOG_ERR("QuickUsb.WriteBulkDataStartStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
            break;
        }

        // Start read data stream
        if (UserAllocatedBuffers) {
            qResult = qusb.ReadBulkDataStartStream((PQBYTE*) InBufferArray, NumBuffers, BufferByteSize, CompletionRoutine, (PQVOID)1, &InStreamID, NumThreads, ThreadConcurrency);
            if (!qResult) {
                LOG_ERR("QuickUsb.ReadBulkDataStartStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            }
        } else {
            qResult = qusb.ReadBulkDataStartStream(NULL, NumBuffers, BufferByteSize, CompletionRoutine, (PQVOID)1, &InStreamID, NumThreads, ThreadConcurrency);
            if (!qResult) {
                LOG_ERR("QuickUsb.ReadBulkDataStartStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                break;
            }
        }

        // Loop until the test ends
        do {
            if (NumThreads == 0) {
                // For single-threaded streams, we must process the streams on this main thread
                // explicitly.  The ProcessStream function will block this thread until either a
                // data request has completed or until the timeout has elapsed.  A timeout
                // indicates that the stream simply did not process any data requests in the given
                // amount of time, not that a data request timed out.
                qResult = qusb.ProcessStream(OutStreamID, 50);
                if (!qResult && (qusb.LastError() != CQuickUsb::ErrorTimeout)) {
                    LOG_ERR("QuickUsb.ProcessStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    break;
                }
                qResult = qusb.ProcessStream(InStreamID, 50);
                if (!qResult && (qusb.LastError() != CQuickUsb::ErrorTimeout)) {
                    LOG_ERR("QuickUsb.ProcessStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
                    break;
                }
            } else {
                // For multi-threaded streams, the worker threads will automatically process streams so we
                // don't have to worry about it (although we could if we wanted to, but then the true number
                // of threads be be one higher that NumThreads).  For now, just don't hog the processor.
                SleepMS(50);
            }
            
            // Is it time to end the test
            if (timer.GetElapsedTimeInSeconds() >= TestTime) {
                RunningTest = false;
            } else {
                // Update the stats every once in a while
                tElapsed = updateTimer.GetElapsedTimeInSeconds();
                if (tElapsed >= UpdateFreq) {
                    Test_DisplayStats(qusb);
                    updateTimer.Start();
                }
            }
            
            // If a transaction failed in the completion routine, we need to stop the test
            if (CompletionRoutineFailure) {
                RunningTest = false;
            }
        } while (RunningTest);

        // Halt the write stream
        LOG("Stopping Write Stream..." << endl);
        qResult = qusb.StopStream(OutStreamID, true); // Return without blocking
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        }

        // Halt the read stream
        LOG("Stopping Read Stream..." << endl);
        qResult = qusb.StopStream(InStreamID, true); // Return without blocking
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        }

        // Wait for the write stream to fully shutdown
        LOG("Stopping Write Stream..." << endl);
        qResult = qusb.StopStream(OutStreamID, false); // Block until stream stops
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        } else {
            LOG("Write Streaming successfully stopped" << endl);
        }

        // Wait for the read stream to fully shutdown
        LOG("Stopping Read Stream..." << endl);
        qResult = qusb.StopStream(InStreamID, false); // Block until stream stops
        if (!qResult) {
            LOG_ERR("QuickUsb.StopStream() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        } else {
            LOG("Read Streaming successfully stopped" << endl);
        }

        break;
#endif // IMPLEMENT_ASYNC
    }

    // Stop the test
    Test_Stop(qusb, true);

    // If the device was opened for the test, close it
    if (opened) {
        if (!qusb.Close()) {
            LOG_ERR("QuickUsb.Close() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
        }
    }

    // Verify that the device is closed
    if (qusb.IsOpened()) {
        LOG_ERR("QuickUSB device is not closed" << endl);
    }
    
    // Free memory
    if (buffer) {
        delete[] buffer;
        buffer = NULL;
    }
    if (transID) {
        delete[] transID;
        transID = NULL;
    }
    if (outBuffer != NULL) {
        delete[] outBuffer;
        outBuffer = NULL;
    }
    if (inBuffer != NULL) {
        delete[] inBuffer;
        inBuffer = NULL;
    }
#ifdef IMPLEMENT_ASYNC
    if (inBulkStream != NULL) {
        delete[] inBulkStream;
        inBulkStream = NULL;
    }
    if (outBulkStream != NULL) {
        delete[] outBulkStream;
        outBulkStream = NULL;
    }
    if (InBufferArray != NULL) {
        for (int k = 0; k < NumBuffers; ++k) {
            if (InBufferArray[k] != NULL) {
                CQuickUsb::FreeDataBuffer((PQBYTE *)&InBufferArray[k]);            }
        }
        delete[] InBufferArray;
        InBufferArray = NULL;
    }
    if (OutBufferArray != NULL) {
        for (int k = 0; k < NumBuffers; ++k) {
            if (OutBufferArray[k] != NULL) {
                CQuickUsb::FreeDataBuffer((PQBYTE *)&OutBufferArray[k]);
            }
        }
        delete[] OutBufferArray;
        OutBufferArray = NULL;
    }
#endif
}



QRESULT WriteFpgaConfigRegister(CQuickUsb &qusb, QWORD config) {
    QRESULT ok;

    // Write the new value
    unsigned char outData[2] = { (unsigned char)(config & 0xff), (unsigned char)(config >> 8) };
    ok = qusb.WriteCommand(0, outData, 2);
    if (!ok) {
        LOG_ERR("QuickUsb.WriteCommand() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
    }
    
    SleepMS(50);

    // Read back the value
    unsigned char inData[2];
    QWORD length = 2;
    ok = qusb.ReadCommand(0, inData, &length);
    if (!ok) {
        LOG_ERR("QuickUsb.ReadCommand() failed with error: " << qusb.LastError() << " (" << qusb.LastDriverError() << ")" << endl);
    }

    SleepMS(50);

    return ok;
}



void GetNextInLFSR(QWORD* data, int words) {    
    for (int k = 0; k < words; ++k) {
        data[k] = NextInLFSR;
        NextInLFSR = GetNextLFSR(data[k]);
    }
}



void GetNextOutLFSR(QWORD* data, int words) {
    for (int k = 0; k < words; ++k) {
        data[k] = NextOutLFSR;
        NextOutLFSR = GetNextLFSR(data[k]);
    }
}



QWORD GetNextLFSR(QWORD prev) {
    int bit16 = (prev >> 15) & 1;
    int bit14 = (prev >> 13) & 1;
    int bit13 = (prev >> 12) & 1;
    int bit11 = (prev >> 10) & 1;

    QWORD feedback = (QWORD)(~(bit16 ^ bit14 ^ bit13 ^ bit11) & 1);
    return (QWORD)((prev << 1) | feedback);
}


unsigned long long CountBitErrors(QWORD* a, QWORD* b, int length) {
    unsigned long long bert = 0;
    QWORD bitErrors;
    int k, j;
    
    for (k = 0; k <length; ++k) {
        bitErrors = (QWORD)(a[k] ^ b[k]);
        //bitErrors = (QWORD)(a[k] ^ 0xFFFE);
        if (bitErrors != 0) {
            for (j = 0; j < 15; ++j) {
                bert += (unsigned long long)(bitErrors & 1);
                bitErrors >>= 1;
            }
        }
    }

    return bert;
}



void GetFriendlyDataSize(char* str, int length, double x) {
    if (x < 1024.0)     {
        sprintf(str, "%.2f Bytes", x);
    } else if (x < (1024.0 * 1024.0))     {
        double KB = x / (1024.0);
        sprintf(str, "%.2f KB", KB);
    } else if (x < (1024.0 * 1024.0 * 1024.0))     {
        double MB = x / (1024.0 * 1024.0);
        sprintf(str, "%.2f MB", MB);
    } else {
        double GB = x / (1024.0 * 1024.0 * 1024.0);
        sprintf(str, "%.2f GB", GB);
    }
}
