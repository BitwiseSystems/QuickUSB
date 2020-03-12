#include <iostream>
#include <fstream>
#include <sstream>

#include "QuickUSB.h"
#include "PlatformIndependentTimer.h"

using namespace std;

#define BYTES
#define KB * 1024
#define MB * 1024 * 1024

QULONG totalBytes;
ofstream fout;

// Test parameters
const int TestTime = 5; // Seconds
const QULONG MinNumBuffers = 4;
const QULONG MaxNumBuffers = 4;
const float MaxProcessingRate = 100.0f; // MS/MB
const QULONG MaxRequestSize = (1 MB);

PCRITICAL_SECTION lock;

#define LOG(x) cout << x; fout << x;

QBOOL test_sync(QHANDLE hDevice, QBOOL read, PQBYTE buffer, QULONG requestSize, float processingRate, PlatformIndependentTimer &timer) {
    QRESULT ok;
    QULONG lastError;
    QULONG length;

    // Run test for a certain period of time to get an accurate measurement
    do {
        // Read data
        length = requestSize;
        if (read) {
            ok = QuickUsbReadData(hDevice, buffer, &length);
        } else {
            ok = QuickUsbWriteData(hDevice, buffer, length);
        }
        if (!ok) {
            ok = QuickUsbGetLastError(&lastError);
            LOG("QuickUsbWriteData failed with error: " << lastError << endl);
        } else {
            PlatformIndependentTimer processingTimer;

            // Keep track of the number of bytes transferred
            totalBytes += length;

            // The processing rate is in MS / MB, so we must calculate how long to process data
            float time = (processingRate * (float)length) / (1 MB); // In MS

            // Process data
            processingTimer.Start();
            do {
                // Do nothing
            } while ((processingTimer.GetElapsedTimeInSeconds() * 1000.0) < time);
        }
    } while (timer.GetElapsedTimeInSeconds() < TestTime);

    return 1;
}

QVOIDRETURN async_callback(PQBULKSTREAM BulkStream) {
    // Keep track of the total number of bytes transferred

    if (BulkStream->Error == 0) {
        EnterCriticalSection(lock);
        totalBytes += BulkStream->BytesTransferred; // Needs protection when multithreading
        LeaveCriticalSection(lock);
        
        if (BulkStream->BytesRequested != BulkStream->BytesTransferred) {
            LOG("Incorrect number of bytes transferred!" << endl);
        }
    }

    // Tag parameter stores processing rate
    if (BulkStream->Tag != NULL) {
        PlatformIndependentTimer processingTimer;

        // The processing rate is in MS / MB, so we must calculate how long to process data
        float processingRate = *(float *)BulkStream->Tag;
        float time = (processingRate * (float)BulkStream->BytesTransferred) / (1 MB); // In MS

        // Process data
        processingTimer.Start();
        do {
            // Do nothing
        } while ((processingTimer.GetElapsedTimeInSeconds() * 1000.0) < time);
    }
}

QBOOL test_async(QHANDLE hDevice, QBOOL read, PQBULKSTREAM bulkstreams, PQBYTE *buffers, QULONG numBuffers, QULONG requestSize, float processingRate, PlatformIndependentTimer &timer) {
    QRESULT ok;
    QULONG lastError;
    QULONG k, issuedTrans, curTrans;

    // Issue initial transactions
    issuedTrans = 0;
    for (k = 0; k < numBuffers; ++k) {
        // Issue transaction
        if (read) {
            ok = QuickUsbReadBulkDataAsync(hDevice, buffers[k], requestSize, &bulkstreams[k], &async_callback, &processingRate);
        } else {
            ok = QuickUsbWriteBulkDataAsync(hDevice, buffers[k], requestSize, &bulkstreams[k], &async_callback, &processingRate);
        }
        if (!ok) {
            ok = QuickUsbGetLastError(&lastError);
            LOG("QuickUsb(Read|Write)BulkData failed with error: " << lastError << endl);
        } else {
            ++issuedTrans;
        }
    }

    // Run test for a certain period of time to get an accurate measurement
    curTrans = 0;
    do {
        // Wait for the next transaction to complete
        ok = QuickUsbBulkWait(hDevice, &bulkstreams[curTrans], 0);
        if (!ok) {
            ok = QuickUsbGetLastError(&lastError);
            LOG("QuickUsbBulkWait failed with error: " << lastError << endl);
            return 0;
        } else {
            --issuedTrans;

            // Check that the transaction complete successfully
            if (bulkstreams[curTrans].Error != 0) {
                LOG("Request failed with error: " << lastError << endl);
                return 0;
            }

            // Re-issue the data request
            if (timer.GetElapsedTimeInSeconds() < TestTime) {
                if (read) {
                    ok = QuickUsbReadBulkDataAsync(hDevice, buffers[curTrans], requestSize, &bulkstreams[curTrans], &async_callback, &processingRate);
                } else {
                    ok = QuickUsbReadBulkDataAsync(hDevice, buffers[curTrans], requestSize, &bulkstreams[curTrans], &async_callback, &processingRate);
                }
                if (!ok) {
                    ok = QuickUsbGetLastError(&lastError);
                    LOG("QuickUsb(Read|Write)BulkDataAsync failed with error: " << lastError << endl);
                    return 0;
                } else {
                    ++issuedTrans;
                }
            }
        }

        // Move to the next transaction
        if (++curTrans >= numBuffers) {
            curTrans = 0;
        }
    } while (issuedTrans > 0);

    // Be absolutely sure that all transactions have completed
    ok = QuickUsbBulkWait(hDevice, 0, 0);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsbBulkWait failed with error: " << lastError << endl);
        return 0;
    }

    return 1;
}

QBOOL test_streaming(QHANDLE hDevice, QBOOL read, PQBULKSTREAM bulkstreams, PQBYTE *buffers, QULONG numBuffers, QULONG requestSize, float processingRate, QWORD numThreads, QWORD concurrency, PlatformIndependentTimer &timer) {
    QRESULT ok;
    QULONG lastError;
    QBYTE streamID;

    // Start stream
    if (read) {
        ok = QuickUsbReadBulkDataStartStream(hDevice, buffers, numBuffers, requestSize, async_callback, &processingRate, &streamID, numThreads, concurrency);
    } else {
        ok = QuickUsbWriteBulkDataStartStream(hDevice, buffers, numBuffers, requestSize, async_callback, &processingRate, &streamID, numThreads, concurrency);
    }
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsb(Read|Write)BulkDataStartStream failed with error: " << lastError << endl);
        return 0;
    }

    timer.Start();

    // Run test for a certain period of time to get an accurate measurement
    do {
        if (numThreads == 0) {
            // Perform stream processing here
            ok = QuickUsbProcessStream(hDevice, streamID, 0);
            if (!ok) {
                ok = QuickUsbGetLastError(&lastError);

                // Ignore timeout errors here as they just mean no requests
                // we're processed, not that a request timed out
                if (lastError != QUICKUSB_ERROR_TIMEOUT) {
                    LOG("QuickUsbProcessStream failed with error: " << lastError << endl);
                    return 0;
                }
            }
        } else {
            // For multi-threaded apps, yield our thread so that the worker threads may do their work
            SleepMS(0);
        }
    } while (timer.GetElapsedTimeInSeconds() < TestTime);

    // Stop the stream
    ok = QuickUsbStopStream(hDevice, streamID, 0);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsbStopStream failed with error: " << lastError << endl);
        return 0;
    }

    return 1;
}

enum TestType { Synchronous, Asynchronous, Streaming };

int RunTest(TestType test, QBOOL read, QWORD NumThreads, QWORD Concurrency) {
    QCHAR nameList[1024];
    QHANDLE hDevice;
    QRESULT ok;
    QULONG lastError;
    PlatformIndependentTimer timer;
    float processingRate;
    PQBYTE *buffers;
    PQBYTE buffer;
    QULONG requestSize;
    PQBULKSTREAM bulkstream;
    QBOOL TestOk;
    PQCHAR model;
    time_t rawtime;
    struct tm *timeinfo;

    // Get a list of the connected QuickUSB devices
    ok = QuickUsbFindModules(nameList, 1024);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        cout << "QuickUsbFindModules failed with error: " << lastError << endl;
        return 1;
    }

    // Open the device
    ok = QuickUsbOpen(&hDevice, nameList);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsbOpen failed with error: " << lastError << endl);
        return 1;
    }

    // Read the QuickUSB Module's IO Model
    QCHAR str[128];
    ok = QuickUsbGetStringDescriptor(hDevice, QUICKUSB_MODEL, str, 128);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsbGetStringDescriptor failed with error: " << lastError << endl);
        return 1;
    }

    // Close the device
    ok = QuickUsbClose(hDevice);
    if (!ok) {
        ok = QuickUsbGetLastError(&lastError);
        LOG("QuickUsbClose failed with error: " << lastError << endl);
        return 1;
    }

    // Extract only the IO Model name
    int length = (int)strlen(str);
    for (int k = 0; k < length; ++k) {
        if (str[k] == '(') {
            model = &str[k + 1];
        }
        if (str[k] == ')') {
            str[k] = '\0';
        }
        if (str[k] == '/') {
            for (int j = k; str[j] != '\0'; ++j) {
                str[j] = str[j + 1];
            }
        }
    }

#if defined(__linux__)
    string path = "/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbThroughputTest/Logs/";
    path += "Linux-";
#endif
#if defined(__APPLE__)
    string path = "/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbThroughputTest/Logs/";
    path += "Mac-";
#endif
#if defined(_WIN32)
    string path = "C:\\Projects\\Bitwise\\QuickUSB\\Library\\Software\\QuickUsbThroughputTest\\Logs\\";
    path += "Windows-";
#endif
    path += model;
    if (test == Synchronous) {
        path += " Synchronous ";
    } else if (test == Asynchronous) {
        path += " Asynchronous ";
    } else if (test == Streaming) {
        path += " Streaming ";
    }
    if (read) {
        path += "Read";
    } else {
        path += "Write";
    }
    path += ".txt";
    // Open the output file
    fout.open(path.c_str());

    // Write out file header
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    LOG(" Throughput Test - " << asctime(timeinfo) << endl);
    LOG("--------------------------------------------------------------------------------" << endl);
    LOG("File: " << path << endl);
    LOG("Program Compiled As: " << ((sizeof(PQBYTE) == 8) ? "x64" : "x86") << endl);
    LOG("IO Model: " << model << endl);
    if (test != Synchronous) {
        LOG("NumThreads: " << NumThreads << endl);
        LOG("Concurrency: " << Concurrency << endl);
    }
    LOG(endl);

    // Set up number formatting
    cout.setf(ios::fixed);
    cout.precision(4);

    fout.setf(ios::fixed);
    fout.precision(4);

    switch (test) {
        case Synchronous:
            // Output info about test
            LOG("Packet Size (Bytes)\t");
            for (processingRate = 0.0f; processingRate <= MaxProcessingRate; ) {
                LOG("\'" << processingRate << " MS/MB");
                if (processingRate != MaxProcessingRate) {
                    LOG("\t");
                }

                // Be smart about choosing processing times
                if (processingRate < 50.0f) {
                    processingRate += 10.0f;
                } else {
                    processingRate += 25.0f;
                }
            }
            LOG(endl);

            // Loop over large packet sizes
            for (requestSize = (8 BYTES); requestSize <= MaxRequestSize; requestSize *= 2) {
                // Loop over processing times
                for (processingRate = 0.0f; processingRate <= MaxProcessingRate; ) {
                    // Open the device
                    ok = QuickUsbOpen(&hDevice, nameList);
                    if (!ok) {
                        ok = QuickUsbGetLastError(&lastError);
                        LOG("QuickUsbOpen failed with error: " << lastError << endl);
                        return 1;
                    }

                    // Allocate memory
                    buffer = new QBYTE[requestSize];

                    // Begin timer
                    totalBytes = 0;
                    timer.Start();

                    // Perform the test
                    TestOk = test_sync(hDevice, read, buffer, requestSize, processingRate, timer);

                    // Stop timer
                    timer.Stop();

                    if (TestOk) {
                        // Record throughput
                        double throughput = (totalBytes / timer.GetElapsedTimeInSeconds());
                        if (processingRate == 0.0f) {
                            LOG(requestSize << "\t");
                        }
                        LOG(throughput / (1 MB));

                        // Note any issues with accuracy
                        if (processingRate > 0.0f) {
                            float processingTime = (processingRate * requestSize) / (1 MB); // In MS
                            if (processingTime < PlatformIndependentTimer::Accuracy) {
                                LOG("*"); // '*' means timer inaccuracies may be affecting this measurement
                            }
                        }
                        if (processingRate != MaxProcessingRate) {
                            LOG("\t");
                        } else {
                            LOG(endl);
                        }
                    }

                    // Free memory
                    delete [] buffer;

                    // Close the device
                    ok = QuickUsbClose(hDevice);
                    if (!ok) {
                        ok = QuickUsbGetLastError(&lastError);
                        LOG("QuickUsbClose failed with error: " << lastError << endl);
                        return 1;
                    }

                    // If there we're errors, stop the test
                    if (!TestOk) {
                        break;
                    }

                    // Be smart about choosing processing times
                    if (processingRate < 50.0f) {
                        processingRate += 10.0f;
                    } else {
                        processingRate += 25.0f;
                    }
                }

                if (!TestOk) {
                    break;
                }
            }

            break;

        case Asynchronous:
            // Loop over processing times
            for (processingRate = 0.0f; processingRate <= MaxProcessingRate; ) {
                // Output info about test
                LOG("Processing Rate: " << processingRate << " MS/MB" << endl);

                // Loop over the number of buffers
                QULONG numBuffers;
                LOG("Request Size (Bytes)\t");
                for (numBuffers = MinNumBuffers; numBuffers <= MaxNumBuffers; numBuffers *= 2) {
                    LOG("\'" << numBuffers << " Buffers");
                    if (numBuffers != MaxNumBuffers) {
                        LOG("\t");
                    }
                }
                LOG(endl);

                // Loop over large packet sizes
                for (requestSize = (8 BYTES); requestSize <= MaxRequestSize; requestSize *= 2) {
                    for (numBuffers = MinNumBuffers; numBuffers <= MaxNumBuffers; numBuffers *= 2) {
                        // Open the device
                        ok = QuickUsbOpen(&hDevice, nameList);
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbOpen failed with error: " << lastError << endl);
                            return 1;
                        }

                        // Set the timeout to a high value because a large number of buffers combined with a 
                        // large processing time will lead to timeouts (32 Requests * (100 MS / MB) * 16 MB) = 51.2 s
                        ok = QuickUsbSetTimeout(hDevice, 100 * 1000); // 100 s
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbSetTimeout failed with error: " << lastError << endl);
                            return 1;
                        }

                        // Set the number of async threads
                        ok = QuickUsbSetNumAsyncThreads(hDevice, NumThreads, Concurrency);
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbSetNumAsyncThreads failed with error: " << lastError << endl);
                            return 1;
                        }

                        // Allocate memory
                        buffers = new PQBYTE[numBuffers];
                        for (QULONG j = 0; j < numBuffers; ++j) {
                            ok = QuickUsbAllocateDataBuffer(&buffers[j], requestSize);
                            if (!ok) {
                                ok = QuickUsbGetLastError(&lastError);
                                LOG("QuickUsbAllocateDataaBuffer failed with error: " << lastError << endl);
                                return 1;
                            }
                        }
                        bulkstream = new QBULKSTREAM[numBuffers];

                        // Begin timer
                        totalBytes = 0;
                        timer.Start();

                        // Perform the test
                        TestOk = test_async(hDevice, read, bulkstream, buffers, numBuffers, requestSize, processingRate, timer);

                        // Stop timer
                        timer.Stop();

                        if (TestOk) {
                            // Record throughput
                            double throughput = (totalBytes / timer.GetElapsedTimeInSeconds());
                            if (numBuffers == MinNumBuffers) {
                                LOG(requestSize << "\t");
                            }
                            LOG(throughput / (1 MB));
                            // Note any issues with accuracy
                            if (processingRate > 0.0f) {
                                float processingTime = (processingRate * requestSize) / (1 MB); // In MS
                                if (processingTime < PlatformIndependentTimer::Accuracy) {
                                    LOG("*"); // '*' means timer inaccuracies may be affecting this measurement
                                }
                            }
                            if (numBuffers != MaxNumBuffers) {
                                LOG("\t");
                            } else {
                                LOG(endl);
                            }
                        }

                        // Free memory
                        for (QULONG j = 0; j < numBuffers; ++j) {
                            ok = QuickUsbFreeDataBuffer(&buffers[j]);
                            if (!ok) {
                                ok = QuickUsbGetLastError(&lastError);
                                LOG("QuickUsbFreeDataBuffer failed with error: " << lastError << endl);
                                return 1;
                            }
                        }
                        delete [] buffers;
                        delete [] bulkstream;

                        // Close the device
                        ok = QuickUsbClose(hDevice);
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbClose failed with error: " << lastError << endl);
                            return 1;
                        }

                        // If there we're errors, stop the test
                        if (!TestOk) {
                            break;
                        }
                    }

                    if (!TestOk) {
                        break;
                    }
                }

                // Be smart about choosing processing times
                if (processingRate < 50.0f) {
                    processingRate += 10.0f;
                } else {
                    processingRate += 25.0f;
                }

                if (!TestOk) {
                    break;
                }
            }
            break;

        case Streaming:
            // Loop over processing times
            for (processingRate = 0.0f; processingRate <= MaxProcessingRate; ) {
                // Output info about test
                LOG("Processing Rate: " << processingRate << " MS/MB" << endl);

                // Loop over the number of buffers
                QULONG numBuffers;
                LOG("Request Size (Bytes)\t");
                for (numBuffers = MinNumBuffers; numBuffers <= MaxNumBuffers; numBuffers *= 2) {
                    LOG("\'" << numBuffers << " Buffers");
                    if (numBuffers != MaxNumBuffers) {
                        LOG("\t");
                    }
                }
                LOG(endl);

                // Loop over large packet sizes
                for (requestSize = (8 BYTES); requestSize <= MaxRequestSize; requestSize *= 2) {
                    for (numBuffers = MinNumBuffers; numBuffers <= MaxNumBuffers; numBuffers *= 2) {
                        // Wait a little before each test
                        SleepMS(250);

                        // Open the device
                        ok = QuickUsbOpen(&hDevice, nameList);
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbOpen failed with error: " << lastError << endl);
                            return 1;
                        }

                        // Set the timeout to a high value because a large number of buffers combined with a 
                        // large processing time will lead to timeouts (32 Requests * (100 MS / MB) * 16 MB) = 51.2 s
                        ok = QuickUsbSetTimeout(hDevice, 100 * 1000); // 100 s
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbSetTimeout failed with error: " << lastError << endl);
                            return 1;
                        }

                        // Allocate memory
                        buffers = new PQBYTE[numBuffers];
                        for (QULONG j = 0; j < numBuffers; ++j) {
                            ok = QuickUsbAllocateDataBuffer(&buffers[j], requestSize);
                            if (!ok) {
                                ok = QuickUsbGetLastError(&lastError);
                                LOG("QuickUsbAllocateDataaBuffer failed with error: " << lastError << endl);
                                return 1;
                            }
                        }
                        bulkstream = new QBULKSTREAM[numBuffers];

                        // Begin timer
                        totalBytes = 0;
                        timer.Start();

                        // Perform the test
                        TestOk = test_streaming(hDevice, read, bulkstream, buffers, numBuffers, requestSize, processingRate, NumThreads, Concurrency, timer);

                        // Stop timer
                        timer.Stop();

                        if (TestOk) {
                            // Record throughput
                            double throughput = (totalBytes / timer.GetElapsedTimeInSeconds());
                            if (numBuffers == MinNumBuffers) {
                                LOG(requestSize << "\t");
                            }
                            LOG(throughput / (1 MB));
                            // Note any issues with accuracy
                            if (processingRate > 0.0f) {
                                float processingTime = (processingRate * requestSize) / (1 MB); // In MS
                                if (processingTime < PlatformIndependentTimer::Accuracy) {
                                    LOG("*"); // '*' means timer inaccuracies may be affecting this measurement
                                }
                            }
                            if (numBuffers != MaxNumBuffers) {
                                LOG("\t");
                            } else {
                                LOG(endl);
                            }
                        }

                        // Free memory
                        for (QULONG j = 0; j < numBuffers; ++j) {
                            ok = QuickUsbFreeDataBuffer(&buffers[j]);
                            if (!ok) {
                                ok = QuickUsbGetLastError(&lastError);
                                LOG("QuickUsbFreeDataBuffer failed with error: " << lastError << endl);
                                return 1;
                            }
                        }
                        delete [] buffers;
                        delete [] bulkstream;

                        // Close the device
                        ok = QuickUsbClose(hDevice);
                        if (!ok) {
                            ok = QuickUsbGetLastError(&lastError);
                            LOG("QuickUsbClose failed with error: " << lastError << endl);
                            return 1;
                        }

                        // If there we're errors, stop the test
                        if (!TestOk) {
                            break;
                        }
                    }

                    if (!TestOk) {
                        break;
                    }
                }

                // Be smart about choosing processing times
                if (processingRate < 50.0f) {
                    processingRate += 10.0f;
                } else {
                    processingRate += 25.0f;
                }

                if (!TestOk) {
                    break;
                }
            }
            break;
    }

    // Close the file
    fout.close();

    // Return successfully
    return 0;
}

int main(int argc, char *argv[]) {
    lock = (PCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(lock);

    // Test reads
    //RunTest(Synchronous, 1, 0, 0);  // No threads
    //RunTest(Asynchronous, 1, 0, 0); // No threads
    //RunTest(Streaming, 1, 0, 0);    // No threads

    // Test writes
    //RunTest(Synchronous, 0, 0, 0);  // No threads
    //RunTest(Asynchronous, 0, 0, 0); // No threads
    //RunTest(Streaming, 0, 0, 0);    // No threads

    RunTest(Streaming, 1, 4, 2);    // No threads

    free(lock);
    // Return successfully
    return 0;
}