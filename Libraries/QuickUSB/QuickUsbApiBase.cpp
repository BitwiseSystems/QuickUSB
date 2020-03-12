/*=============================================================================
 Title        : QuickUsbApiBase.cpp
 Description  : QuickUSB QUSB2 Module Base API
 Notes        : None
 History      :

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

=============================================================================*/
#include "QuickUsbUtil.h"


typedef struct _USB_DEVICE_DESCRIPTOR {
  UCHAR  bLength;
  UCHAR  bDescriptorType;
  USHORT bcdUSB;
  UCHAR  bDeviceClass;
  UCHAR  bDeviceSubClass;
  UCHAR  bDeviceProtocol;
  UCHAR  bMaxPacketSize0;
  USHORT idVendor;
  USHORT idProduct;
  USHORT bcdDevice;
  UCHAR  iManufacturer;
  UCHAR  iProduct;
  UCHAR  iSerialNumber;
  UCHAR  bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - offset into array of _QUSB_DEVICE_INFO structures
 Output  :  
 Returns :  Non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
#if DRIVER==WindowsDriver
QRETURN QuickUsbGetDeviceDesc(QHANDLE hDevice, PUSB_DEVICE_DESCRIPTOR buffer, QULONG bufferSize) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReceived;
    QBOOL result;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters - None to verify
    if (bufferSize < sizeof(USB_DEVICE_DESCRIPTOR)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    result = QuickUsbIoctl(hDevice, hDevHandle,
        IOCTL_QUSB_GET_DEVICE_DESCRIPTOR,
        NULL,
        0,
        (PVOID) buffer,
        bufferSize,
        &bytesReceived);
    if (!result) {
        lastError = TranslateSystemError(GetLastError());
        return(FALSE);
    }

    return(TRUE);
}
#endif


/*---------------------------------------------------------------------------
 Purpose :  Used by QuickUsbFindModules to test device on USB Bus for use with
            QUSB legacy Driver
 Input   :  deviceName - QUSB-xxx  The xxx value is the device number
 Output  :  hDevice - handle to QUSB device
 Notes   :  This function is currently not being used for MacOSX.
 ---------------------------------------------------------------------------*/
int TestQusbOpen(QHANDLE *hDevice, char *deviceName) {
    char devicePath[128];
    QDEVHANDLE handle;

#if DRIVER==WindowsDriver
    // Build the complete device name
#ifdef USE_SAFE_FUNCTIONS
    sprintf_s(devicePath, 128, "\\\\.\\%s", deviceName);
#else
     sprintf(devicePath, "\\\\.\\%s", deviceName);
#endif

    // Open the file handle
    handle = CreateFile((LPCSTR) devicePath,
        GENERIC_READ | GENERIC_WRITE,
        0,             //  No module sharing
        NULL,
        OPEN_EXISTING,
        0,
        NULL
        );

    // Check the returned handle
    if (handle == INVALID_HANDLE_VALUE) {
        return (FALSE);
    }

    // Return the handle
    *hDevice = handle;
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    UCHAR index;
    
    // Build the complete device name
    sprintf(devicePath, "/dev/%s", deviceName);
    
    // Check to see if the file is already opened
    for (index = 0; index < MAX_DEVICENUM; index++) {
        if (strcmp(QusbDeviceInfo[index].devName, deviceName) == 0) {
            if (QusbDeviceInfo[index].hDevHandle == INVALID_HANDLE_VALUE) {
                break;
            }
            else {
                *hDevice = QusbDeviceInfo[index].hDevHandle;
                return(TRUE);
            }
        }
    }

    // Open the file descriptor (handle)
    handle = open(devicePath, O_RDWR);
    if (handle == INVALID_HANDLE_VALUE) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }

    // Return the handle
    *hDevice = handle;
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    strcpy(devicePath, "invalid");
    handle = NULL;
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif // DRIVER==MacOSXDriver

    return (TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To identify device type.
 Input   :  hDevice - offset into array of _QUSB_DEVICE_INFO structures
 Output  :  pType - Device type
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
int QuickUsbGetDeviceType(QHANDLE hDevice, QusbDriverType *pType) {
    UCHAR index = (UCHAR)hDevice;

    // BUGFIX: Check that the device handle does not exceed the QusbDeviceInfo
    // array bounds so we don't access invalid memory.  Before if a invalid
    // handle was passed in that exceeded the array bounds then the function
    // call would potentially crash.  This big existed in v2.14.2 and earlier.
    if (index > MAX_DEVICENUM) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    if (hDevice == INVALID_HANDLE_VALUE) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    if (QusbDeviceInfo[index].devType == QUSB_DRIVER) {
        *pType = QUSB_DRIVER;
        return(TRUE);
    }

    *pType = OTHER_DRIVER;
    lastError = QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE;
    return(FALSE);
}



/*---------------------------------------------------------------------------
 Purpose :  To return a valid QUSB device handle.
 Input   :  hDevice - offset into array of _QUSB_DEVICE_INFO structures
 Output  :  hDevHandle - Handle to device interface
            Return non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
int QuickUsbGetDeviceHandle(QHANDLE hDevice, QDEVHANDLE *hDevHandle) {
    UCHAR index;
    index = (UCHAR)hDevice;

    // BUGFIX: Check that the device handle does not exceed the QusbDeviceInfo
    // array bounds so we don't access invalid memory.  Before if a invalid
    // handle was passed in that exceeded the array bounds then the function
    // call would potentially crash.  This big existed in v2.14.2 and earlier.
    if (index > MAX_DEVICENUM) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    if (hDevice != INVALID_HANDLE_VALUE) {
        *hDevHandle = QusbDeviceInfo[index].hDevHandle;
        if (*hDevHandle == INVALID_QHANDLE_VALUE) {
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        lastError = 0;
        lastDriverError = 0;
        return(TRUE);
    }

    lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
    return(FALSE);
}



/*---------------------------------------------------------------------------
 Purpose :  To verify the driver type, Q handle, and safely get the device
            handle.  This function should be the first function called from
            every API function.
 Input   :  hDevice - offset into array of _QUSB_DEVICE_INFO structures
 Output  :  hDevHandle - Handle to device interface
 Returns :  Non-zero on success, zero on failure.
 Notes   :  In the event of an error, lastError is set
 ---------------------------------------------------------------------------*/
QRESULT QuickUsbRetrieveDeviceHandle(QHANDLE hDevice, QDEVHANDLE *hDevHandle) {
    QusbDriverType type;

    // Get the driver type
    if (!QuickUsbGetDeviceType(hDevice, &type)) { // Sets lastError on failure
        return(FALSE);
    }

    // Verify the driver type
    if (type != QUSB_DRIVER) {
        lastError = QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE;
        return(FALSE);
    }

    // Convert the generic handle to the device specific handle
    if (!QuickUsbGetDeviceHandle(hDevice, hDevHandle)) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;         
        return(FALSE);
    }

    return(TRUE);
}



QRETURN QuickUsbOpen(QHANDLE *hDevice, QCHAR *deviceName) {
/*---------------------------------------------------------------------------
 Purpose :  Open a QuickUSB device for use by the library.
 Input   :  deviceName - A valid QuickUSB device name
 Output  :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            Returns non-zero on success, zero on failure.
 Notes   :
 ---------------------------------------------------------------------------*/
    return QuickUsbOpenEx(hDevice, deviceName, QUICKUSB_OPEN_NORMAL);
}



QUSBLIB QRETURN QuickUsbOpenEx(QHANDLE *hDevice, QCHAR *deviceName, QWORD flags) {
/*---------------------------------------------------------------------------
 Purpose :  Open a QuickUSB device for use by the library.
 Input   :  deviceName - A valid QuickUSB device name
            flags - Optional flags
 Output  :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            Returns non-zero on success, zero on failure.
 Notes   :
 ---------------------------------------------------------------------------*/
#if (DRIVER==WindowsDriver) || (DRIVER==LinuxDriver)
    QCHAR devicePath[MAX_DEVICEPATH];
#endif
    QCHAR devList[256];
    QDEVHANDLE hDevHandle;
    UCHAR index;

    QBOOL found;

    // Test for a null devicename
    if (deviceName == NULL || strncmp(deviceName, "QUSB-", 5) != 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // If FindModules has yet to be called, call it as it must be
    // called before this function executes or it will fail
    if (!initialized)
    {
        // The QuickUsbUtil.dll file needs to automatically allow for opening of
        // unprogrammed modules (to allow the programmer and customizer to work 
        // properly)
#if defined(QUICKUSB_UTILITY_DLL)
        devList[0] = '*';
        devList[1] = '.';
        devList[2] = '*';
        devList[3] = 0;
#endif

        // BUGFIX: QuickUsbFindModules must be called before QuickUsbOpen.
        // This behavior was undocumented causing some QuickUsbOpen calls
        // to fail.  This bug existed in v2.14.0 and earlier.
        if (!QuickUsbFindModules(devList, 256))
        {
            // BUGFIX: When this function fails, DO NOT return an error as the 
            // QuickUsbProgrammer will not longer work on unprogrammed modules.  
            // This bug existed in v2.14.1 and v2.14.2 of the DLLs.
            //
            // return(FALSE); <-- DON'T DO THIS
        }
        initialized = TRUE;
    }

    // Find device name from the QUSB_DEVICE_INFO array and access the QusbDeviceType
    // to determine which driver is called to open the module interface.
    found = FALSE;
    for (index = 0; index < MAX_DEVICENUM; index++) {
        if (!strcmp(deviceName, (const char *) QusbDeviceInfo[index].devName)) {
            found = TRUE;
            break;
        }
    }

    // Check if the device was found
    if (!found) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return (FALSE);
    }

    // Handle an undefined driver type   
    if (QusbDeviceInfo[index].devType != QUSB_DRIVER) {
        lastError = QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE;
        return (FALSE);
    }

#if (DRIVER==MacOSXDriver) || (DRIVER==LinuxDriver)
    if (!QusbDeviceRefCount) {
        int hSharedMemory;

        // Get access to shared memory
        hSharedMemory = shm_open("/QuickUSB.DeviceRefCount", O_CREAT | O_RDWR, 0666);
        if (hSharedMemory < 0) {
            lastError = TranslateSystemError(errno);
            return(FALSE);
        }

        // Resize the shared memory
        if (ftruncate(hSharedMemory, sizeof(QULONG) * MAX_DEVICENUM) < 0) {
            // On Mac, ftruncate throws a EINVAL error at us if we try to resize
            // shared memory that was shm_open'ed by another process, so we'll
            // just ignore it
            if (errno != EINVAL) {
                lastError = TranslateSystemError(errno);
                return(FALSE);
            }
        }

        // Map the shared memory into our processes address space
        QusbDeviceRefCount = (PQULONG) mmap(NULL, sizeof(QULONG) * MAX_DEVICENUM, PROT_READ | PROT_WRITE, MAP_SHARED, hSharedMemory, 0);
        if (QusbDeviceRefCount == MAP_FAILED) {
            QusbDeviceRefCount = NULL;
            lastError = TranslateSystemError(errno);
            return(FALSE);
        }

        // Close the handle to the shared memory
        close(hSharedMemory);
        hSharedMemory = 0;
    }
#endif

    // BUGFIX: Check to make sure the device is not already opened and return an
    // error if it is already open.  This behavior was incorrectly documented but
    // never implemented and therefore existed in v2.14.2 and earlier.  Now
    // opening a closed or open device is default behavior that may be altered
    // with a flag.
    if (QusbDeviceRefCount[index] > 0) {
        if (flags & QUICKUSB_OPEN_IF_CLOSED) {
            // The device is opened by at least one process, but we were instructed
            // only to successfully perform an open if the module was not already
            // opened
            lastError = QUICKUSB_ERROR_ALREADY_OPENED;
            return(FALSE);
        } else {
            // The device is already open by at least one process and we're 
            // allowed to open a device multiple times
            if (QusbDeviceInfo[index].deviceOpen > 0) {
                // The device is already opened by this process so we already have
                // a valid handle to the device
                ++(QusbDeviceInfo[index].deviceOpen);
                *hDevice = (QHANDLE) index;
                return(TRUE);
            } else {
                // The device has been opened by another process but not by this 
                // process so we must open it to get a valid device handle and
                // fill out the QusbDeviceInfo entry for the device.
            }
        }
    } else {
        // The device has not yet been opened by any processes so we must open it 
        // to get a valid device handle and fill out the QusbDeviceInfo entry for 
        // the device.
    }

#if DRIVER==WindowsDriver
    // Build the complete device name
#ifdef USE_SAFE_FUNCTIONS
    sprintf_s(devicePath, MAX_DEVICEPATH, "\\\\.\\%s", deviceName);
#else
    sprintf(devicePath, "\\\\.\\%s", deviceName);
#endif

    // Open the file handle
    hDevHandle = CreateFile((LPCSTR) devicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
#ifdef IMPLEMENT_OVERLAPPED
        FILE_FLAG_OVERLAPPED,
#else
        0,
#endif
        NULL
        );
#endif

#if DRIVER==LinuxDriver
    // Build the complete device name
    sprintf(devicePath, "/dev/%s", deviceName);

    // Open the file descriptor (handle)    
    hDevHandle = open(devicePath, O_RDWR);
#endif
    
#if (DRIVER==WindowsDriver) || (DRIVER==LinuxDriver)
    // Check the returned handle
    if (hDevHandle == INVALID_HANDLE_VALUE) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
#endif
    
#if DRIVER==MacOSXDriver
    {
    //IOUSBDeviceInterface245 **module;
        
    mach_port_t masterPort;
    CFMutableDictionaryRef matchDict;
    CFNumberRef numberRef;
    SInt32 vid = BITWISE_VID, pid = QUSB2_PID;
    io_iterator_t itr, itritr;
    io_service_t usbDev, usbInterface;
    IOCFPlugInInterface **ioDev = NULL, **ioIntf = NULL;
    IOUSBDeviceInterface245 **dev = NULL;
    IOUSBConfigurationDescriptorPtr confDesc;
    IOUSBFindInterfaceRequest interfaceRequest;
    SInt32 score;
    UInt8 numConf, numIntf;
    kern_return_t err;
    UInt8 configuration;
    UCHAR i, secondIndex;
    
    UCHAR numPipes;
    UCHAR direction, number, transferType, interval;
    QWORD maxPacketSize;
    QULONG k;
    USBDeviceAddress devAddress;

    // Obtain master port
    err = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (err || !masterPort) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Obtain matching dictionary
    matchDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchDict) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Get reference number for vendor id
    numberRef = CFNumberCreate(kCFAllocatorDefault,
                               kCFNumberSInt32Type,
                               &vid);
    if (!numberRef) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Add vendor id in dictionary
    CFDictionaryAddValue(matchDict,
                         CFSTR(kUSBVendorID),
                         numberRef);
    CFRelease(numberRef);
    
    // Get reference number for product id
    numberRef = CFNumberCreate(kCFAllocatorDefault,
                               kCFNumberSInt32Type,
                               &pid);
    if (!numberRef) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Add product id in dictionary
    CFDictionaryAddValue(matchDict,
                         CFSTR(kUSBProductID),
                         numberRef);
    CFRelease(numberRef);
    
    // Get services matching the dictionary
    err = IOServiceGetMatchingServices(masterPort,
                                       matchDict,
                                       &itr);
    if (err != kIOReturnSuccess) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Iterate through devices
    while ((usbDev = IOIteratorNext(itr))) { // Free IOObjectRelease(usbDev) when done with usbDev
        err = IOCreatePlugInInterfaceForService(usbDev,
                                                kIOUSBDeviceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &ioDev,
                                                &score);
        
        // Check that the plug-in interface was created successfully
        if (err || !ioDev) {
            err = IOObjectRelease(usbDev);
            continue;
        }
        
        // We don't need the device object anymore
        err = IOObjectRelease(usbDev);
        if (err != kIOReturnSuccess) {
            continue;
        }
        
        // Get device interface
        err = (*ioDev)->QueryInterface(ioDev,
                                       CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID245),
                                       (LPVOID *)&dev);
        
        // We are now done with the device plug-in interface
        (*ioDev)->Release(ioDev); //IODestroyPlugInInterface(ioDev);
        if ((err != kIOReturnSuccess) || !dev) {
            continue;
        }
        
        // Get the bus address of this USB device
        err = (*dev)->GetDeviceAddress(dev, &devAddress);
        if (err != kIOReturnSuccess) {
            (*dev)->Release(dev);
            continue;
        }
        
        // Check if this is the USB device we are looking for
        if (QusbDeviceInfo[index].devAddress == devAddress) {
            // NOTE: At this point the device interface is still open
            break;
        }
        
        // Free the device interface and continue searching
        (*dev)->Release(dev);
    }
        
    // Release the iterator object
    IOObjectRelease(itr);
        
    // Check that we found the USB device we were looking for
    if (QusbDeviceInfo[index].devAddress != devAddress) {
        (*dev)->Release(dev);
            
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // This device should have already been discovered and placed in the
    // device table, so find its entry.
    secondIndex = MAX_DEVICENUM;
    for (k = 0; k < MAX_DEVICENUM; ++k) {
        if (QusbDeviceInfo[k].devAddress == devAddress) {
            secondIndex = k;
            break;
        }
    }
    
    // The device was not found
    if (secondIndex >= MAX_DEVICENUM) {
        (*dev)->Release(dev);
        
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Check that the index we have and the index that device was found
    // under are the same
    if (index != secondIndex) {
        // This may point to an internal error with how devices are entered
        // into the device table by QuickUsbFindModues().
        (*dev)->Release(dev);
        
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Open device
    err = (*dev)->USBDeviceOpen(dev);
    if (err != kIOReturnSuccess) {
        (*dev)->Release(dev);
        
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Get number of configurations
    err = (*dev)->GetNumberOfConfigurations(dev, &numConf);
    if ((err != kIOReturnSuccess) || !numConf) {
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Get configurations (The device does not need to be open to call this function)
    err = (*dev)->GetConfigurationDescriptorPtr(dev, 0, &confDesc);
    if (err != kIOReturnSuccess) {
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Set configuration (The device does not need to be open to call this function)
    err= (*dev)->GetConfiguration(dev, &configuration);
    if (err != kIOReturnSuccess) {
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // File interface request
    interfaceRequest.bInterfaceClass = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
    interfaceRequest.bAlternateSetting = kIOUSBFindInterfaceDontCare;
    
    // Get iterator for interfaces
    err = (*dev)->CreateInterfaceIterator(dev, &interfaceRequest, &itritr);
    if (err != kIOReturnSuccess) {
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return(FALSE);
    }
    
    // Get number of interfaces
    numIntf = 0;
    while ((usbInterface = IOIteratorNext(itritr))) { // Free IOObjectRelease(usbInterface) when done with usbInterface
        numIntf++;
        
        // We don't need the interface object after intermediate plug-in is created
        if (IOObjectRelease(usbInterface) != kIOReturnSuccess) {
            continue;
        }
        
    }
    
    // Configuration did not match, set configuration
    // Note: It is important not to set the configuration each time
    //       FindModules is called if the configuration value does not change.
    // Note: The configuration value MUST be set to get the interfaces.
    //       When switching between High-Speed and Full-Speed mode, the
    //       interfaces are closed, so SetConfiguration MUST be called to
    //       create the new interfaces.
    if ((numIntf != confDesc->bNumInterfaces) || (configuration != confDesc->bConfigurationValue)) {
        err= (*dev)->SetConfiguration(dev, confDesc->bConfigurationValue);
        if (err) {
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
    }
    
    // Reset iterator
    IOIteratorReset(itritr);
    
    // Iterate through interfaces
    // Note: A single device may have more than one interface.
    while ((usbInterface = IOIteratorNext(itritr))) { // Free IOObjectRelease(usbInterface) when done with usbInterface
        // Get IO interface
        err = IOCreatePlugInInterfaceForService(usbInterface,
                                                kIOUSBInterfaceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &ioIntf,
                                                &score);
        
        // We don't need the interface object after intermediate plug-in is created
        if (IOObjectRelease(usbInterface) != kIOReturnSuccess) {
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        if ((err != kIOReturnSuccess) || !ioIntf) {
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        // Get interface
        err = (*ioIntf)->QueryInterface(ioIntf,
                                        CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID245),
                                        (LPVOID *)&hDevHandle);
        if ((err != kIOReturnSuccess) || !hDevHandle) {
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        // Open interface
        err = (*hDevHandle)->USBInterfaceOpen(hDevHandle);
        if (err != kIOReturnSuccess) {
            (*hDevHandle)->Release(hDevHandle);
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        // Get number of endpoints
        err = (*hDevHandle)->GetNumEndpoints(hDevHandle, &numPipes);
        if (err || !numPipes) {
            (*hDevHandle)->Release(hDevHandle);
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            
            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return(FALSE);
        }
        
        // Get pipe properties and store for later use
        for (i = 1; i <= numPipes; i++) {
            err = (*hDevHandle)->GetPipeProperties(hDevHandle,
                                             i,
                                             &direction,
                                             &number,
                                             &transferType,
                                             &maxPacketSize,
                                             &interval);
            if (err != kIOReturnSuccess) {
                (*hDevHandle)->Release(hDevHandle);
                (*dev)->USBDeviceClose(dev);
                (*dev)->Release(dev);
                
                lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
                return(FALSE);
            }
            if ((transferType == kUSBBulk) && (direction == kUSBOut)) {
                QusbDeviceInfo[index].outPipe = i;
            }
            if ((transferType == kUSBBulk) && (direction == kUSBIn)) {
                QusbDeviceInfo[index].inPipe = i;
            }
            QusbDeviceInfo[index].maxPacketSize = maxPacketSize;
        }
        
        // Store interface and device type
        QusbDeviceInfo[index].hDevHandle = hDevHandle;
        QusbDeviceInfo[index].module = dev;
        QusbDeviceInfo[index].devType = QUSB_DRIVER;
        QusbDeviceInfo[index].devQusb = usbDev;
        QusbDeviceInfo[index].devInterface = usbInterface;
        QusbDeviceInfo[index].deviceOpen = 0;
        QusbDeviceInfo[index].requestQ = new std::queue<PQBULKSTREAM>();
    }
    
    IOObjectRelease(itritr);

    // Init mutex
    QusbDeviceInfo[index].lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(QusbDeviceInfo[index].lock, NULL);
        
    }
#endif // DRIVER==MacOSXDriver
    
    
    // Keep a reference count of the number of times this device is open
    ++(QusbDeviceInfo[index].deviceOpen);   // # of times device has been opened by this process
    ++(QusbDeviceRefCount[index]);          // # of processes that have opened this device

    // Store the handle and return the offset into the QUSB_DEVICE_INFO array
    QusbDeviceInfo[index].hDevHandle = hDevHandle;
    QusbDeviceInfo[index].devType = QUSB_DRIVER;
    QusbDeviceInfo[index].timeout = QUSB_DEFAULT_TIMEOUT;
    
    *hDevice = (QHANDLE) index;
    
    // Return successfully
    lastError = QUICKUSB_ERROR_NO_ERROR;
    lastDriverError = QUICKUSB_ERROR_NO_ERROR;
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To close a handle to a QuickUsb device.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbClose(QHANDLE hDevice) {
    QDEVHANDLE hDevHandle;
#ifdef IMPLEMENT_ASYNC
    QULONG k;
#endif
#if DRIVER==MacOSXDriver
    kern_return_t err;
    IOUSBDeviceInterface245 **module;
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        // If the user tries to close a device that is not present (or perhaps is
        // no longer present) then we will gracefully return without error.
        return(TRUE);
    }

    // BUGFIX: Check that the device has been opened by this process before trying to 
    // close it.  This bug existed in v2.14.2 and earlier.
    if (QusbDeviceRefCount[(QULONG) hDevice] == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_CLOSE_MODULE;
        return(FALSE);
    }
    if (QusbDeviceInfo[(QULONG) hDevice].deviceOpen == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_CLOSE_MODULE;
        return(FALSE);
    }

    // Sanity check the device handle
    if (QusbDeviceInfo[(QULONG) hDevice].hDevHandle == INVALID_QHANDLE_VALUE) {
        // This likely indicates an internal issue with reference counting
        lastError = QUICKUSB_ERROR_INTERNAL_ERROR; 
        return(FALSE);
    }

    // We have a reference counting mechanism.  Only close if our count reaches zero.
    --(QusbDeviceInfo[(QULONG) hDevice].deviceOpen);    // # of times device has been opened by this process
    if (QusbDeviceInfo[(QULONG) hDevice].deviceOpen == 0) {
        // The device is no longer opened by this process so we must free resources
        // and close the handle
        --(QusbDeviceRefCount[(QULONG) hDevice]);       // # of processes that have opened this device
    } else {
        // The device is still opened by this process so we should not free 
        // resources or close the handle yet
        return(TRUE);
    }

#ifdef IMPLEMENT_ASYNC
    // Stop all streams
    for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
        // Check if the stream is active
        if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
            if (!QuickUsbStopStream(hDevice, (QBYTE)k, FALSE)) { // Sets lastError on failure
                // If we are unable to stop stream, we must make sure that our reference counts
                // still remain accurate
                if (QusbDeviceInfo[(QULONG) hDevice].deviceOpen == 0) {
                    ++(QusbDeviceRefCount[(QULONG) hDevice]);       // # of processes that have opened this device
                }
                ++(QusbDeviceInfo[(QULONG) hDevice].deviceOpen);    // # of times device has been opened by this process
#if defined(_DEBUG)
                // This is a bad failure case and indicative of an internal problem
                // with shutting down streams, so we'll assert on debug builds for
                // easier debugging.
                assert(0); 
#endif
                // We want to overwrite the error set by 'QuickUsbProcessSTStream' so that we
                // catch the fact that this is an internal error.
                lastError = QUICKUSB_ERROR_INTERNAL_ERROR;
                return(FALSE);
            }
        }
    }
#endif

#if DRIVER==WindowsDriver
    // Close the file handle
    if (!CloseHandle(hDevHandle)) {
        // We do not need to worry about errors with closing the handle.  Doing could
        // introduce problems with gracefully closing when a device is no longer present.
    }
#endif

#if DRIVER==LinuxDriver
    // Close file descriptor
    close(QusbDeviceInfo[hDevice].hDevHandle);
#endif

#if DRIVER==MacOSXDriver
    // Free the request queue
    if (QusbDeviceInfo[hDevice].requestQ != NULL) {
        delete QusbDeviceInfo[hDevice].requestQ;
        QusbDeviceInfo[hDevice].requestQ = NULL;
    }
    
    // Close interface
    // Note: Since the interface is created and opened in QuickUsbOpen,
    //       the interface MUST be closed AND released here.
    if (QusbDeviceInfo[hDevice].hDevHandle != NULL) {
        err = (*hDevHandle)->USBInterfaceClose(hDevHandle);
        if (err != kIOReturnSuccess) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
        (*hDevHandle)->Release(hDevHandle);
        QusbDeviceInfo[hDevice].hDevHandle = NULL;
    }
    
    // Close device
    if (QusbDeviceInfo[hDevice].module != NULL) {
        module = QusbDeviceInfo[hDevice].module;
        (*module)->USBDeviceClose(module);
        (*module)->Release(module);
        QusbDeviceInfo[hDevice].module = NULL;
    }
    
    if (QusbDeviceInfo[hDevice].lock != NULL) {
        pthread_mutex_destroy(QusbDeviceInfo[hDevice].lock);
        free(QusbDeviceInfo[hDevice].lock);
        QusbDeviceInfo[hDevice].lock = NULL;
    }
#endif

#if (DRIVER==MacOSXDriver) || (DRIVER==LinuxDriver)
    // Unmap shared memory
    // NOTE: Shared memory is automatically unmapped when the process exits
    //if (munmap(QusbDeviceInfo[index].QusbDeviceRefCount, sizeof(QULONG) * MAX_DEVICENUM) < 0) {
    //    lastError = TranslateSystemError(errno);
    //    return(FALSE);
    //}
    //QusbDeviceInfo[index].QusbDeviceRefCount = NULL;

    // Unlink shared memory
    // NOTE: Shared memory is automatically unlinked when the process exits
    //if (shm_unlink("/QuickUSB.DeviceRefCount") < 0) {
    //    lastError = TranslateSystemError(errno);
    //    return(FALSE);
    //}
#endif

    // Update the device info
    QusbDeviceInfo[(QULONG) hDevice].hDevHandle = INVALID_QHANDLE_VALUE;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Returns the string descriptor for the selected QuickUSB module.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            index - QuickUsb string descriptor index. See QuickUsb.h
            length - Length to read
 Output  :  buffer - Buffer to store string descriptor
            length - Length of string descriptor
            Returns non-zero on success, zero on failure.
 Notes   :  This function returns a NULL terminated string in the buffer.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetStringDescriptor(QHANDLE hDevice, QBYTE index, QCHAR *buffer, QWORD length) {
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    unsigned char i, ulength;
    QULONG bytesReceived;
    QUSB_GET_STRING_DESCRIPTOR_IN indesc;
    QUSB_GET_STRING_DESCRIPTOR_OUT outdesc;

    indesc.Index = index;
    indesc.LanguageId = 0;
    bytesReceived = length;

    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    result = QuickUsbIoctl(hDevice, hDevHandle,
        IOCTL_QUSB_GET_STRING_DESCRIPTOR,
        &indesc,
        (QWORD)sizeof(QUSB_GET_STRING_DESCRIPTOR_IN),
        &outdesc,
        (QWORD)sizeof(QUSB_GET_STRING_DESCRIPTOR_OUT),
        &bytesReceived);

    if (!result) {
        return(FALSE);
    }

    // Convert the UNICODE string descriptor to ASCII
    ulength = (outdesc.bLength - 2) / 2;

    // Only return up to the user's string length
    if (ulength > (length - 1)) {
        ulength = length - 1;
    }

    for (i = 0; i < ulength; i++) {
        buffer[i] = ((PQCHAR)outdesc.bString)[i * 2];
    }
    buffer[i] = '\0';
    }
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    {
    QUSB_GET_STRING_DESCRIPTOR string_desc;
    ULONG ulength;
    UCHAR i;
    int ioResult;

    // Fill string descriptor
    string_desc.Index = index;
    string_desc.LanguageID = 0x0409;
    string_desc.Length = 128;
    string_desc.Data[0] = '\0';
    
    // Send string descriptor request
    ioResult = ioctl(hDevHandle, IOCTL_QUSB_GET_STRING_DESCRIPTOR, &string_desc);
    if (ioResult < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }
    
    // Convert the UNICODE string descriptor to ASCII
    ulength = (string_desc.Length - 2) / 2;

    // Only return up to the user's string length
    if (ulength > (length - 1)) {
        ulength = length - 1;
    }

    for (i = 0; i < ulength; i++) {
        buffer[i] = ((PQCHAR)string_desc.Data)[i * 2 + 2];
    }
    buffer[i] = '\0';
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    QBOOL result;
    IOUSBDevRequest request;
    kern_return_t err;
    
    UCHAR data[MAX_STRINGDESCRIPTOR];
    ULONG ulength;
    UCHAR i;
    
    result = TRUE;
    
    // Build the vendor request
    request.bmRequestType = USBmakebmRequestType(kUSBIn, kUSBStandard, kUSBDevice);
    request.bRequest = kUSBRqGetDescriptor;
    request.wValue = (kUSBStringDesc<<8)|index;
    request.wIndex = 0;
    request.wLength = MAX_STRINGDESCRIPTOR;
    request.pData = data;
    
    // Send control request to get string descriptor
    err = (*hDevHandle)->ControlRequest(hDevHandle,
            0,
            &request);
    
    // If there was an error set the last error code
    if (err != kIOReturnSuccess) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorURB;
        return(FALSE);
    }

    // Convert UNICODE to ASCII
    ulength = (request.wLenDone - 2) / 2;
    if (ulength > (MAX_STRINGDESCRIPTOR - 1)) {
        ulength = MAX_STRINGDESCRIPTOR - 1;
    }
    for (i = 0; i < ulength; i++) {
        buffer[i] = data[i*2 + 2];
    }
    buffer[i] = '\0';
    
    return result;
    }
#endif // DRIVER==MacOSXDriver

    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Build a list of all QuickUSB modules connected to the host.
 Input   :  length - The length of the nameList buffer
 Output  :  nameList - Buffer containing module names
            Returns non-zero on success, zero on failure.
 Notes   :  devName is limited to 9 characters long.
            If successful, nameList will contain a NULL delimited list of
                QuickUSB module names found by the library.
            This routine will only find devices that are closed.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbFindModules(QCHAR *nameList, QULONG length) {
    QCHAR devName[64];
    PQCHAR namePtr; 
    UCHAR index;
    UCHAR hDevice;
    QDEVHANDLE hDevHandle;
#if (DRIVER==WindowsDriver) || (DRIVER==LinuxDriver)
    QCHAR strDescriptor1[MAX_STRINGDESCRIPTOR];
    QCHAR strDescriptor6[MAX_STRINGDESCRIPTOR];
#endif
    
#if DRIVER==WindowsDriver
    QULONG bytesReceived;
    WDF_USB_PIPE_INFORMATION pipeInfo;
    QBYTE pipeIndex;

    // Check the length of the buffer
    if (length < 10) {
        return(FALSE);
    }

    DriverVersion.DeviceFound = 0;

    // Check to see that devices that are already in namelist are still open

    // Search for a suitable device
    hDevice = 0;
    namePtr = nameList;
    for (index=0; index < MAX_DEVICENUM; index++) {
#ifdef USE_SAFE_FUNCTIONS
        sprintf_s(devName, 64, "%s-%1d", QUICKUSB_DEVICE_BASENAME, index);
#else
        sprintf(devName, "%s-%1d", QUICKUSB_DEVICE_BASENAME, index);
#endif
        if (TestQusbOpen(&hDevHandle, devName))   {  // Get device handle from devName
            // This is tricky!  Since we don't know if this device is open or not
            // we're going to put this device in an extra entry at the end of the QusbDeviceInfo table.
            // If the device looks good, then we'll put it in the correct place in the table.
#ifdef USE_SAFE_FUNCTIONS
            strcpy_s((char *)QusbDeviceInfo[MAX_DEVICENUM-1].devName, 9, devName);
#else
            strcpy((char *)QusbDeviceInfo[MAX_DEVICENUM-1].devName, devName);
#endif
            QusbDeviceInfo[MAX_DEVICENUM-1].hDevHandle = hDevHandle;
            QusbDeviceInfo[MAX_DEVICENUM-1].devType = QUSB_DRIVER;
            QusbDeviceInfo[MAX_DEVICENUM-1].timeout = QUSB_DEFAULT_TIMEOUT;

            // Read the module string descriptor and look for QUICKUSB_DEVICE_STRING in it.
            // If it's found or the user passed *.* in the nameList, then add the device to the nameList.
            // Note: *.* gets overwritten by the first module we find, so it won't work for the second module, but that's not a problem.
            QuickUsbGetStringDescriptor((QHANDLE) (MAX_DEVICENUM-1), 
                QUICKUSB_MAKE, 
                (QCHAR *) strDescriptor1, 
                MAX_STRINGDESCRIPTOR);
            QuickUsbGetStringDescriptor((QHANDLE) (MAX_DEVICENUM-1), 
                QUICKUSB_MAKE_PERM, 
                (QCHAR *) strDescriptor6, 
                MAX_STRINGDESCRIPTOR);
            if (((strcmp(strDescriptor1, QUICKUSB_DEVICE_STRING) == 0) ||
                (strcmp(strDescriptor6, QUICKUSB_DEVICE_STRING) == 0) ||
                (strstr(nameList, "*.*") != NULL)) &&
                (strlen(devName) < (size_t)(length - (namePtr-nameList)))) {
#ifdef USE_SAFE_FUNCTIONS
                    sprintf_s(namePtr, 64, "%s", devName);
#else
                    sprintf(namePtr, "%s", devName);
#endif
                    namePtr += strlen(devName) + 1;

                    // Now's a good time to go read out that device version number information.
                    if (!DriverVersion.DeviceFound) {
                        // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
                        QuickUsbIoctl((QHANDLE)(MAX_DEVICENUM-1), hDevHandle,
                            IOCTL_QUSB_GET_DRIVER_VERSION,
                            NULL,
                            0,
                            (PVOID) &DriverVersion,
                            (QWORD)sizeof(QUSB_DRIVER_VERSION),
                            &bytesReceived);
                        DriverVersion.DeviceFound = TRUE;
                    }

                    // Is this device already in the table?
                    if (strcmp((const char *) QusbDeviceInfo[index].devName, devName) != 0) {
                        // The device is okay to put in the nameList so let's populate the table entry.
                        memset(&QusbDeviceInfo[index], 0, sizeof(QUSB_DEVICE_INFO));
#ifdef USE_SAFE_FUNCTIONS
                        strcpy_s((char *) QusbDeviceInfo[index].devName, 9, devName);
#else
                        strcpy((char *) QusbDeviceInfo[index].devName, devName);
#endif
                        QusbDeviceInfo[index].hDevHandle = INVALID_HANDLE_VALUE;
                        QusbDeviceInfo[index].devType = QUSB_DRIVER;
                        QusbDeviceInfo[index].timeout = QUSB_DEFAULT_TIMEOUT;

                        // Get the endpoint maximum packet sizes
                        pipeIndex = 0;
                        if (!QuickUsbIoctl((QHANDLE)(MAX_DEVICENUM-1), hDevHandle, IOCTL_QUSB_GET_PIPE_INFO, &pipeIndex, sizeof(pipeIndex), (PVOID) &pipeInfo, (QWORD)sizeof(WDF_USB_PIPE_INFORMATION), &bytesReceived)) {
                            QusbDeviceInfo[index].maxControlPacketSize = 64; // On error, default to 64 to support Full, High, and Super Speed USB
                        } else {
                            // Retrieve the max control EP packet size based off the bulk EP size
                            if (pipeInfo.MaximumPacketSize == 1024) {
                                QusbDeviceInfo[index].maxControlPacketSize = 512;  // SuperSpeed USB
                            } else {
                                QusbDeviceInfo[index].maxControlPacketSize = 64; // All others default to 64 to support Full, High, and Super Speed USB
                            }
                        }
                    }
                    hDevice++;
            }

            // We must ALWAYS close the handle we received from TestQusbOpen even if a user
            // program already had the QuickUSB device open to prevent the driver from not
            // being able to reuse this QuickUSB module number in case it becomes disconnected.
            CloseHandle(hDevHandle);
        }
        else {
            if ((strcmp((const char *) QusbDeviceInfo[index].devName, devName) == 0) && 
                (QusbDeviceInfo[index].devType == QUSB_DRIVER)) {
                    // Remove the device from the table
                    if (QusbDeviceInfo[index].hDevHandle != INVALID_HANDLE_VALUE) {
                        CloseHandle(QusbDeviceInfo[index].hDevHandle);
                    }
                    memset(&QusbDeviceInfo[index], 0, sizeof(QUSB_DEVICE_INFO));
                    QusbDeviceInfo[index].hDevHandle = INVALID_HANDLE_VALUE;
            }
        }
    } //end for

    // Add a terminating null as the final character in the nameList
    *namePtr = '\0';

    // Clear out the last entry
    memset(&QusbDeviceInfo[MAX_DEVICENUM-1], 0, sizeof(QUSB_DEVICE_INFO));
    QusbDeviceInfo[MAX_DEVICENUM-1].hDevHandle = INVALID_HANDLE_VALUE;

    // Return true if the list is not empty
    if (hDevice == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    int ioResult;

    // Check the length of the buffer
    if (nameList == 0 || length < 10) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    DriverVersion.DeviceFound = 0;

    // Search for a suitable device.
    hDevice = 0;
    namePtr = nameList;
    for (index=0; index < MAX_DEVICEPATH-1; index++) {
        sprintf(devName, "%s-%1d", QUICKUSB_DEVICE_BASENAME, index);
        if (TestQusbOpen(&hDevHandle, devName))   {  // Get device handle from devName
            // This is tricky!  Since we don't know if this device is open or not
            // we're going to put this device in an extra entry at the end of the QusbDeviceInfo table.
            // If the device looks good, then we'll put it in the correct place in the table.
            strcpy((char *)QusbDeviceInfo[MAX_DEVICENUM-1].devName, devName);
            QusbDeviceInfo[MAX_DEVICENUM-1].hDevHandle = hDevHandle;
            QusbDeviceInfo[MAX_DEVICENUM-1].devType = QUSB_DRIVER;

            // Read the module string descriptor and look for QUICKUSB_DEVICE_STRING in it.
            // If it's found or the user passed *.* in the nameList, then add the device to the nameList.
            // Note: *.* gets overwritten by the first module we find, so it won't work for the second module, but that's not a problem.
            QuickUsbGetStringDescriptor(MAX_DEVICENUM-1, 
                    QUICKUSB_MAKE, 
                    (QCHAR *) strDescriptor1, 
                    MAX_STRINGDESCRIPTOR);
            QuickUsbGetStringDescriptor(MAX_DEVICENUM-1,
                    QUICKUSB_MAKE_PERM,
                    (QCHAR *) strDescriptor6, // Not a valid strDescriptor6. See QuickUsbGetStringDescriptor().
                    MAX_STRINGDESCRIPTOR);
            if (((strcmp(strDescriptor1, QUICKUSB_DEVICE_STRING) == 0) ||
                    (strcmp(strDescriptor6, QUICKUSB_DEVICE_STRING) == 0) ||
                    (strstr(nameList, "*.*") != NULL)) &&
                    (strlen(devName) < (size_t)(length - (namePtr-nameList)))) {
                // Now's a good time to go read out that device version number information.
                sprintf(namePtr, "%s", devName);
                namePtr += strlen(devName) + 1;
                if (!DriverVersion.DeviceFound) {
                    ioResult = ioctl(hDevHandle, IOCTL_QUSB_GET_DRIVER_VERSION, &DriverVersion);
                    if (ioResult < 0) {
                        lastError = TranslateSystemError(errno);
                        return(FALSE);
                    }
                    DriverVersion.DeviceFound = TRUE;
                }
                // Is this device already in the table?
                if (strcmp((const char *) QusbDeviceInfo[index].devName, devName) != 0) {
                    // The device is okay to put in the nameList so let's populate the table entry.
                    strcpy((char *)QusbDeviceInfo[index].devName, devName);
                    QusbDeviceInfo[index].hDevHandle = INVALID_HANDLE_VALUE;
                    QusbDeviceInfo[index].devType = QUSB_DRIVER;
                    QusbDeviceInfo[index].maxControlPacketSize = 64;
                }
                hDevice++;
            }

            // If the device is not open, TestQusbOpen returns a valid handle for temporary usage. Once FindModules
            // is complete, this handle MUST be closed. If the device is open and the stored handle is valid,
            // TestQusbOpen returns the stored handle. Once FindModules is complete, this handle MUST NOT be closed
            // as it will be closed by QuickUsbClose.
            // NOTE: In the old implementation, TestQusbOpen would get a new handle even if a valid handle already
            //       exists. Once the new handle is closed, the valid handle would become invalid. This would cause
            //       unpredictable behavior. The new implementation of TestQusbOpen returns the valid handle if it
            //       exists, but if it does not exist, a new handle is returned. Now closing the handle depends on
            //       if the device has already been opened. If the device is open, the handle remains the same. If
            //       the device is not open, the handle closes safely.
            if (QusbDeviceInfo[index].deviceOpen == 0) {
                if (hDevHandle != INVALID_HANDLE_VALUE) {
                    close(hDevHandle);
                    hDevHandle = INVALID_HANDLE_VALUE;
                    QusbDeviceInfo[index].hDevHandle = INVALID_HANDLE_VALUE;
                }
            }
        }
        else {
            if ((strcmp((const char *) QusbDeviceInfo[index].devName, devName) == 0) && 
                    (QusbDeviceInfo[index].devType == QUSB_DRIVER)) {
                if (QusbDeviceInfo[index].hDevHandle != INVALID_HANDLE_VALUE) {
                    close(QusbDeviceInfo[index].hDevHandle);
                    QusbDeviceInfo[index].hDevHandle = INVALID_HANDLE_VALUE;
                }
                QusbDeviceInfo[index].devName[0] = 0;
                QusbDeviceInfo[index].deviceOpen = 0;
                //QusbDeviceInfo[index].iRefCount = 0;
                QusbDeviceInfo[index].defaultOffset = 0;
            }
        }
    } //end for

    // Add a terminating null as the final character in the nameList
    *namePtr = '\0';

    // Clear out the last entry
    strcpy((char *) QusbDeviceInfo[MAX_DEVICENUM-1].devName, "");
    QusbDeviceInfo[MAX_DEVICENUM-1].hDevHandle = INVALID_HANDLE_VALUE;
    QusbDeviceInfo[MAX_DEVICENUM-1].devType = OTHER_DRIVER;

    // Return true if the list is not empty
    if (hDevice == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
#endif // DRIVER==LinuxDriver
    
#if DRIVER==MacOSXDriver
    mach_port_t masterPort;
    CFMutableDictionaryRef matchDict;
    CFNumberRef numberRef;
    SInt32 vid, pid;
    io_iterator_t itr, itritr;
    io_service_t usbDev;
    IOCFPlugInInterface **ioDev;
    IOUSBDeviceInterface245 **dev;
    IOUSBInterfaceInterface245 **intf;
    SInt32 score;
    kern_return_t err;
    QULONG k;
    USBDeviceAddress devAddress;
    
    itr = 0;
    itritr = 0;
    ioDev = NULL;
    dev = NULL;
    intf = NULL;
    
    vid = BITWISE_VID;
    pid = QUSB2_PID;
    
    namePtr = nameList;
    hDevice = 0;
    hDevHandle = 0;
    
    // Obtain master port
    err = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (err || !masterPort) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Obtain matching dictionary
    matchDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchDict) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Get reference number for vendor id
    numberRef = CFNumberCreate(kCFAllocatorDefault,
                               kCFNumberSInt32Type,
                               &vid);
    if (!numberRef) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Add vendor id in dictionary
    CFDictionaryAddValue(matchDict,
                         CFSTR(kUSBVendorID),
                         numberRef);
    CFRelease(numberRef);
    
    // Get reference number for product id
    numberRef = CFNumberCreate(kCFAllocatorDefault,
                               kCFNumberSInt32Type,
                               &pid);
    if (!numberRef) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Add product id in dictionary
    CFDictionaryAddValue(matchDict,
                         CFSTR(kUSBProductID),
                         numberRef);
    CFRelease(numberRef);
    
    // Get services matching the dictionary
    err = IOServiceGetMatchingServices(masterPort,
                                       matchDict,
                                       &itr);
    if (err != kIOReturnSuccess) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
    
    // Iterate through devices
    nameList[0] = '\0';
    nameList[1] = '\0';
    while ((usbDev = IOIteratorNext(itr))) { // Free IOObjectRelease(usbDev) when done with usbDev
        err = IOCreatePlugInInterfaceForService(usbDev,
                                                kIOUSBDeviceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &ioDev,
                                                &score);
        
        // Check that the plug-in interface was created successfully
        if (err || !ioDev) {
            err = IOObjectRelease(usbDev);
            return(FALSE);
        }
        
        // We don't need the device object anymore
        err = IOObjectRelease(usbDev);
        if (err != kIOReturnSuccess) {
            return(FALSE);
        }
        
        // Get device interface
        err = (*ioDev)->QueryInterface(ioDev,
                                       CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID245),
                                       (LPVOID *)&dev);
        
        // We are now done with the device plug-in interface
        (*ioDev)->Release(ioDev); //IODestroyPlugInInterface(ioDev);
        if ((err != kIOReturnSuccess) || !dev) {
            continue;
        }
        
        // Get the bus address of this USB device
        err = (*dev)->GetDeviceAddress(dev, &devAddress);
        if (err != kIOReturnSuccess) {
            (*dev)->Release(dev);
            continue;
        }
        
        // Check if this device already has an entry in the device table
        index = MAX_DEVICENUM;
        for (k = 0; k < MAX_DEVICENUM; ++k) {
            if (QusbDeviceInfo[k].devAddress == devAddress) {
                index = k;
                break;
            }
        }
        
        // Add the new device to the device table
        if (index >= MAX_DEVICENUM) {
            // Locate a free entry in the device table
            for (k = 0; k < MAX_DEVICENUM; ++k) {
                if (QusbDeviceInfo[k].module == NULL) {
                    index = k;
                    break;
                }
            }
            
            // Check that we don't have too many devices in the table
            if (index >= MAX_DEVICENUM) {
                (*dev)->Release(dev);
                continue;
            }
        }
        
        // Free the device interface
        (*dev)->Release(dev);
        
        memset(&QusbDeviceInfo[index], 0, sizeof(QUSB_DEVICE_INFO));
        QusbDeviceInfo[index].module = dev; // This pointer will be invalid but is used to mark the entry as used
        QusbDeviceInfo[index].devAddress = devAddress;
        QusbDeviceInfo[index].devType = QUSB_DRIVER;
        QusbDeviceInfo[index].maxControlPacketSize = 64;
        
        // Create device name
        sprintf((char *)devName, "%s-%1d", QUICKUSB_DEVICE_BASENAME, index);
        
        // Add device name to nameList
        sprintf((char *)namePtr, "%s", devName);
        namePtr += strlen((const char *)devName)+1;
        
        // Add device to device table
        strcpy((char *)QusbDeviceInfo[index].devName, (const char *)devName);
    }
    
    // Clean up
    IOObjectRelease(itr);
    
    // Add a terminating null as the final character in the nameList
    *namePtr = '\0';
    
    //if (index == 0) {
    if (nameList[0] == '\0') {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
        return(FALSE);
    }
#endif // DRIVER==MacOSXDriver
    
    // Return successfully
    initialized = TRUE;
    lastError = QUICKUSB_ERROR_NO_ERROR;
    lastDriverError = QUICKUSB_ERROR_NO_ERROR;
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To set the timeout for API calls 
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            timeOut - Timeout value
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  The timeout is to the nearest millisecond.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbSetTimeout(QHANDLE hDevice, QULONG timeOut) {
    QDEVHANDLE hDevHandle;
    QWORD major, minor, build;
#if DRIVER==WindowsDriver
    QULONG bytesReceived = 0;
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (timeOut == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // NOTE: All timeout values are set on a per-module basis

    /////////////////////////////
    // Set the library timeout //
    /////////////////////////////
    QusbDeviceInfo[(QULONG) hDevice].timeout = timeOut;

    ////////////////////////////
    // Set the driver timeout //
    ////////////////////////////
#if DRIVER==WindowsDriver
    // This requires v2.15.0 of the QuickUSB driver or later.  The
    // call will fail if the driver doesn't have a set timeout IOCTL, so don't bother
    // checking for an error;
    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    if (!QuickUsbIoctl(hDevice, hDevHandle, IOCTL_QUSB_SET_TIMEOUT, (PVOID) &timeOut, (QWORD)sizeof(QULONG), NULL, 0, &bytesReceived)) { // Sets lastError on failure
        return(FALSE);
    }
#endif

#if DRIVER==LinuxDriver
    if (ioctl(hDevHandle, IOCTL_QUSB_SET_TIMEOUT, (PVOID)&timeOut) < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }
#endif //DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    QusbDeviceInfo[hDevice].timeout = timeOut;
#endif // DRIVER==MacOSXDriver

    //////////////////////////////
    // Set the firmware timeout //
    //////////////////////////////
    // This requires v2.15.0 of the QuickUSB firmware or later.
    if (!QuickUsbGetFirmwareVersion(hDevice, &major, &minor, &build)) { // Sets lastError on failure
        return(FALSE);
    }
    if (IsAtLeastVersion(2, 15, 0, major, minor, build)) {
        if (!QuickUsbWriteSetting(hDevice, SETTING_TIMEOUT_HIGH, (timeOut >> 16) & 0x0000ffff)) { // Sets lastError on failure
            return(FALSE);
        }

        if (!QuickUsbWriteSetting(hDevice, SETTING_TIMEOUT_LOW, timeOut & 0x0000ffff)) { // Sets lastError on failure
            return(FALSE);
        }
    }

    // Return successfully
    return(TRUE);
}


/*---------------------------------------------------------------------------
 Purpose :  To get the timeout for API calls 
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            timeOut - Timeout value
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  The timeout is to the nearest millisecond.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetTimeout(QHANDLE hDevice, PQULONG timeOut) {
#if DRIVER==WindowsDriver
    QDEVHANDLE hDevHandle;
    BOOL result;
    QULONG bytesReceived = 0;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (timeOut == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Get the driver timeout
    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    result = QuickUsbIoctl(hDevice, hDevHandle,
        IOCTL_QUSB_GET_TIMEOUT,
        NULL,
        0,
        timeOut,
        (QWORD)sizeof(QULONG),
        &bytesReceived
        );

    if (result == FALSE) {
        return(result);
    }

    // Return successfully
    return(TRUE);
#endif // DRIVER==WindowsDriver

    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
}


/*---------------------------------------------------------------------------
 Purpose :  To get the driver version.
 Input   :  None.
 Output  :  MajorDriverVersion - Major driver version
            MinorDriverVersion - Minor driver version
            BuildDriverVersion - Build driver version
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetDriverVersion(USHORT *MajorDriverVersion, USHORT *MinorDriverVersion, USHORT *BuildDriverVersion) {
#if (DRIVER==WindowsDriver) || (DRIVER==LinuxDriver)
    char str[256];

    // Iterate the bus
    // NOTE: FindModules only needs to be called once to get the driver version.
    if (!initialized) {
        QuickUsbFindModules(str, 256);
    }

    *MajorDriverVersion = DriverVersion.MajorVersion;
    *MinorDriverVersion = DriverVersion.MinorVersion;
    *BuildDriverVersion = DriverVersion.BuildVersion;
    if (!DriverVersion.DeviceFound) {
        return(FALSE);
    }
#endif
    
#if (DRIVER==MacOSXDriver)
    *MajorDriverVersion = QUSB_DRIVER_MAJOR_VERSION;
    *MinorDriverVersion = QUSB_DLL_MINOR_VERSION;
    *BuildDriverVersion = QUSB_DLL_BUILD_VERSION;
#endif
    
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To get the dll version.
 Input   :  None.
 Output  :  MajorDllVersion - Major Dll version
            MinorDllVersion - Minor Dll version
            BuildDllVersion - Build Dll version
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetDllVersion(USHORT *MajorDllVersion, USHORT *MinorDllVersion, USHORT *BuildDllVersion) {
    *MajorDllVersion = QUSB_DLL_MAJOR_VERSION;
    *MinorDllVersion = QUSB_DLL_MINOR_VERSION;
    *BuildDllVersion = QUSB_DLL_BUILD_VERSION;

    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To get the firmware version.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  MajorVersion - Major firmware version
            MinorVersion - Minor firmware version
            BuildVersion - Build firmware version
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetFirmwareVersion(QHANDLE hDevice, PQWORD MajorVersion, PQWORD MinorVersion, PQWORD BuildVersion) {
    QDEVHANDLE hDevHandle;
    QWORD buildVer = 0;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    __usb_Dev_Descriptor__ UsbDeviceDescriptor;
    QULONG bytesReceived;

    // Query the device for interface information
    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    result = QuickUsbIoctl(hDevice, hDevHandle,
       IOCTL_QUSB_GET_DEVICE_DESCRIPTOR,
       &UsbDeviceDescriptor,
       (QWORD)sizeof(UsbDeviceDescriptor),
       &UsbDeviceDescriptor,
       (QWORD)sizeof(UsbDeviceDescriptor),
       &bytesReceived
       );

    if (result == FALSE) {
        return(FALSE);
    }

    // Get the build version.  This requires firmware 2.11rc9 or later.
    if (!QuickUsbReadSetting(hDevice, SETTING_VERSIONBUILD, &buildVer))
    {
        return(FALSE);
    }

    *MajorVersion = (((UsbDeviceDescriptor.bcdDevice & 0xF000) >> 12) * 10) +
        ((UsbDeviceDescriptor.bcdDevice & 0x0F00) >> 8);
    *MinorVersion = (((UsbDeviceDescriptor.bcdDevice & 0x00F0) >> 4) * 10) +
        (UsbDeviceDescriptor.bcdDevice & 0x000F);
    *BuildVersion = buildVer;

    return(TRUE);
    }
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    {
    struct usb_device_descriptor UsbDeviceDescriptor;
    int ioResult;

    // Query the device for interface information
    ioResult = ioctl(hDevHandle, IOCTL_QUSB_GET_DEVICE_DESCRIPTOR, &UsbDeviceDescriptor);
    
    // If there was an error, display the error status box
    if (ioResult < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }
    
    // Get the build version.  This requires firmware 2.11rc9 or later.
    if (!QuickUsbReadSetting(hDevice, SETTING_VERSIONBUILD, &buildVer)) {
        return(FALSE);
    }
    
    *MajorVersion = (((UsbDeviceDescriptor.bcdDevice & 0xF000) >> 12) * 10) +
    ((UsbDeviceDescriptor.bcdDevice & 0x0F00) >> 8);
    *MinorVersion = (((UsbDeviceDescriptor.bcdDevice & 0x00F0) >> 4) * 10) +
    (UsbDeviceDescriptor.bcdDevice & 0x000F);
    *BuildVersion = buildVer;
    
    return(TRUE);
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    QBOOL result;
    IOUSBDeviceDescriptor deviceDescriptor;
    IOUSBDevRequest request;
    kern_return_t err;
    
    QWORD bcdDevice;
    
    result = TRUE;
    buildVer = 0;
    bcdDevice = 0;
    
    // Build the vendor request
    request.bmRequestType = USBmakebmRequestType(kUSBIn, kUSBStandard, kUSBDevice);
    request.bRequest = kUSBRqGetDescriptor;
    request.wIndex = 0;
    request.wValue = (kUSBDeviceDesc<<8);
    request.wLength = sizeof(IOUSBDeviceDescriptor);
    request.pData = &deviceDescriptor;
    
    // Query the device for interface information
    err = (*hDevHandle)->ControlRequest(hDevHandle,
            0,
            &request);
    
    // If there was an error, display the error status box
    if (err != kIOReturnSuccess) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorURB;
        return(FALSE);
    }
    
    // Get the build version.  This requires firmware 2.11rc9 or later.
    if (!QuickUsbReadSetting(hDevice, SETTING_VERSIONBUILD, &buildVer)) {
        return(FALSE);
    }
    
    // Swap bytes
    bcdDevice = CFSwapInt16LittleToHost(deviceDescriptor.bcdDevice);
    
    *MajorVersion = (((bcdDevice & 0xF000) >> 12) * 10) +
    ((bcdDevice & 0x0F00) >> 8);
    *MinorVersion = (((bcdDevice & 0x00F0) >> 4) * 10) +
    (bcdDevice & 0x000F);
    *BuildVersion = buildVer;
    
    return(TRUE);
    }
#endif // DRIVER==MacOSXDriver
}



/*---------------------------------------------------------------------------
 Purpose :  To obtain the last reported error from an API function call
 Input   :  None.
 Output  :  error - Last error value
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetLastError(PQULONG error) {
    // BUGFIX: Check to make sure that the OpenDeviceOffset index actually indexes
    // an open module.  Without this check if QuickUsbFindModules fails with the
    // incorrect QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE when no modules are connected
    // instead of QUICKUSB_ERROR_CANNOT_FIND_DEVICE.  This bug existed in v2.14.2
    // and earlier.  Also, this code is never used so it's commented out.
    /*if (QusbDeviceInfo[OpenDeviceOffset].deviceOpen) {
        QuickUsbGetError((QHANDLE) OpenDeviceOffset, &GetError);
        UsbError = GetError.UsbError;
        DriverError = GetError.QusbDriverError;
        DriverState = GetError.DriverState;
    }*/

    *error = lastError;
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To obtain the last reported error from the driver
 Input   :  None.
 Output  :  error - Last error value
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetLastDriverError(PQULONG error) {
    // BUGFIX: Check to make sure that the OpenDeviceOffset index actually indexes
    // an open module.  Without this check if QuickUsbFindModules fails with the
    // incorrect QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE when no modules are connected
    // instead of QUICKUSB_ERROR_CANNOT_FIND_DEVICE.  This bug existed in v2.14.2
    // and earlier.  Also, this code is never used so it's commented out.
    /*if (QusbDeviceInfo[OpenDeviceOffset].deviceOpen) {
        QuickUsbGetError((QHANDLE) OpenDeviceOffset, &GetError);
        UsbError = GetError.UsbError;
        DriverError = GetError.QusbDriverError;
        DriverState = GetError.DriverState;
    }*/

    *error = lastDriverError;
    return(TRUE);
}
