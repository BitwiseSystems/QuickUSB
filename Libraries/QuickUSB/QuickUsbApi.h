/*=============================================================================
 Title        : QuickUsbApi.h
 Description  : QuickUSB QUSB2 Module Base API
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
#ifndef __QUICKUSB_API_HEADER__
#define __QUICKUSB_API_HEADER__

#include "QuickUSB.h"

// See which driver we are compiling for
#define WindowsDriver 1
#define LinuxDriver   2
#define MacOSXDriver  3

#if defined _WIN32
    #define DRIVER WindowsDriver
    #define QUICKUSB_LIBRARY
#elif defined __linux__
    #define DRIVER LinuxDriver
#elif defined __APPLE__
    #define DRIVER MacOSXDriver
#else
    #error Invalid operating system or driver detected
#endif



#if DRIVER==WindowsDriver
    #include <windows.h>
    #include <winbase.h>
    #include <winioctl.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <lmaccess.h>
    #include <process.h>
    #include <assert.h>
    #include <time.h>
#endif

#if DRIVER==LinuxDriver
    #include <errno.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <time.h>
    #include <sys/time.h>
    #include <pthread.h>
    #include <fcntl.h>
    #include <sys/syscall.h>
    #include <semaphore.h>
#ifndef __USE_XOPEN
    #define __USE_XOPEN
#endif
    #include <unistd.h>
    #include "usb.h"

    #define QUSB_MAX_KMALLOC_SIZE (128*1024)

    typedef int             BOOL;
    typedef int             BOOLEAN;
    typedef unsigned char   BYTE, CHAR, UCHAR;
    typedef unsigned char * PUCHAR;
    typedef signed long     LONG;
    typedef unsigned long   ULONG, DWORD;
    typedef unsigned short  USHORT, WCHAR, WORD;
    typedef void *          PVOID;

    #define FALSE                 0
    #define TRUE                  1

    // Linux usbdevfs has a limit of one page size per read/write. 4096 is
    // the most portable maximum we can do for now
    #define QUSB_MAX_BULK_TRANSFER_BLOCK_SIZE   (4 * 1024)
    //#define MAX_WRITE_BLOCKSIZE   4096
    //#define MAX_READ_BLOCKSIZE    4096
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    // C and system headers
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/fcntl.h>
    #include <sys/mman.h>
    #include <sys/syscall.h>
    #include <semaphore.h>

    // IOKit headers
    #include <CoreFoundation/CoreFoundation.h>
    #include <IOKit/IOCFPlugIn.h>
    #include <IOKit/usb/IOUSBLib.h>
    #include <IOKit/usb/USB.h>

    // True/False values
    #define TRUE                   1
    #define FALSE                  0

    typedef unsigned char BYTE;
    typedef unsigned char CHAR;
    typedef unsigned char UCHAR;
    typedef unsigned char * PUCHAR;
    typedef signed long LONG;
    typedef unsigned long BOOLEAN;
    typedef unsigned short USHORT, WCHAR, WORD;
    typedef void * PVOID;

    void QuickUsbLoad() __attribute__ ((constructor));
    void QuickUsbUnload() __attribute__ ((destructor));
#endif

// Flags used in QIOSTREAM.info
#define QUSB_INFO_NONE              0x00000000
#define QUSB_INFO_INTERNAL_ALLOC    0x00000001
#define QUSB_INFO_MEMORY_FILE	    0x00000002
#define QUSB_INFO_LOOP_AT_EOF       0x00000004
#define QUSB_INFO_ASYNC             0x00010000
#define QUSB_INFO_STREAMING_READ    0x00020000
#define QUSB_INFO_STREAMING_WRITE   0x00040000
#define QUSB_INFO_STREAMING_STOP    0x00080000
#define QUSB_INFO_STREAMING_PAUSED  0x00100000
#define QUSB_INFO_STREAMING_IPAUSED 0x00200000

// Flags used in the QBULKSTREAM.InternalStatus
#define QUSB_BULKSTREAM_MAGIC_MASK   0xFF000000
#define QUSB_BULKSTREAM_MAGIC        0xBF000000
#define QUSB_BULKSTREAM_IO_COMPLETE  0x00000001
#define QUSB_BULKSTREAM_ABORTED      0x00000002
#define QUSB_BULKSTREAM_IN_CR        0x00000004
#define QUSB_BULKSTREAM_EVENT_OK     0x00000008
#define QUSB_BULKSTREAM_AUTO_REISSUE 0x00000010
#define QUSB_BULKSTREAM_CR_COMPLETE  0x00000020

// Flags used in QBULKSTREAM.StreamType
// Use bits: 31-6 reserved
//           7-5 for stream type (0=bulk, All others currently unused)
//           4 for IN/_OUT
//           3-0 for EP #
#define QUSB_STREAMTYPE_BULK 0x20
#define QUSB_STREAMTYPE_IN   0x10
#define QUSB_STREAMTYPE_OUT  0x00

#define QUSB_STREAMTYPE_BULK_OUT_EP2    (QUSB_STREAMTYPE_BULK | QUSB_STREAMTYPE_OUT | 0x02)
#define QUSB_STREAMTYPE_BULK_IN_EP6     (QUSB_STREAMTYPE_BULK | QUSB_STREAMTYPE_IN  | 0x06)

// Thread commands
#define QUSB_THREAD_CMD_RUN          0x0000E001
#define QUSB_THREAD_CMD_SHUTDOWN     0x0000E002

#if DRIVER==MacOSXDriver || DRIVER==LinuxDriver
#define INVALID_HANDLE_VALUE  ((QHANDLE)(-1))
#endif
#define INVALID_QHANDLE_VALUE ((QDEVHANDLE)-1)

// Limits
#define QUSB_MAX_BULK_TRANSFER_SIZE  (16 * 1024 * 1024)
#define QUSB_MAX_BULK_BLOCK_SIZE     (256 * 1024)
#define MAX_TRANSFER_SIZE             262144    // Maximum bulk transfer size for USBD calls
#define MAX_CNTL_XFER_SIZE 64


#ifndef max
    #define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

//
// Vendor specific request code for Anchor Upload/Download
//
// This one is implemented in the core
//
#define ANCHOR_LOAD_INTERNAL  0xA0

//
// These commands are not implemented in the core.  Requires firmware
//
#define ANCHOR_LOAD_EXTERNAL  0xA3
#define ANCHOR_ISFX2          0xAC

//
// This is the highest internal RAM address for the AN2131Q
//
#define MAX_INTERNAL_ADDRESS  0x1B3F

#define INTERNAL_RAM(address) ((address <= MAX_INTERNAL_ADDRESS) ? 1 : 0)
//
// EZ-USB Control and Status Register.  Bit 0 controls 8051 reset
//
#define CPUCS_REG_EZUSB    0x7F92
#define CPUCS_REG_FX2      0xE600


////
// Driver error status, returned from IOCTL_QUSB_GET_LAST_ERROR.  This
// replaces NTSTATUS codes which are undefined in the DLL.  These values
// are stored in the DEVICE CONTEXT DriverStatus member.
//
enum
{
    DriverErrorNone               = 0x0000,
    DriverErrorInvalidParameter   = 0x0001,
    DriverErrorMemory             = 0x0002,
    DriverErrorURB                = 0x0003,    // Deprecated
    DriverErrorTimeout            = 0x0004,
    DriverErrorBulk               = 0x0005,    // Deprecated
    DriverErrorAsync              = 0x0006,    // Deprecated
    DriverErrorPipe               = 0x0007,    // Deprecated
    DriverErrorBusy               = 0x0008,    // Deprecated
    DriverErrorFailure            = 0x0009,

    // NOTE: All new error codes should be defined such that the driver error
    // code matches the library error code
    DriverErrorDeprecated         = 20,
};



#define IMPLEMENT_OVERLAPPED
#define QUSB_DEFAULT_TIMEOUT        1000   // Milliseconds
#define QUSB_TIGHT_LOOP_WAIT        25     // Milliseconds
#define QUSB_TIGHT_LOOP_MIN_TIMEOUT 100    // Milliseconds
#define QUSB_MAX_PENDING_REQUESTS   256

#define CYPRESS_VID           0x04B4
#define CY7C68013_PID         0x8613

#define BITWISE_VID           0x0FBB
#define QUSB2_PID             0x0001

#define FIRMWARE_LOAD             0xA0
#define QUICKUSB_EEPROM           0xA9
#define QUICKUSB_DEVICE_BASENAME  "QUSB"
#define QUICKUSB_DEVICE_STRING    "Bitwise Systems"
#define MAX_DEVICENUM             128
#define MAX_DEVICEPATH            128
#define MAX_STRINGDESCRIPTOR      128

#if DRIVER==WindowsDriver
#endif // DRIVER==WindowsDriver

// QuickUSB String Descriptor Indices
#define QUICKUSB_SERIAL_PERM  0x05
#define QUICKUSB_MAKE_PERM    0x06

// QuickUSB vendor specific requests
#define QUICKUSB_SETTING      0xB0
#define QUICKUSB_FPGA         0xB1
#define QUICKUSB_COMMAND      0xB2
#define QUICKUSB_PORT         0xB3
#define QUICKUSB_RS232        0xB4
#define QUICKUSB_I2C          0xB5
#define QUICKUSB_SPI          0xB6
#define QUICKUSB_READDATALEN  0xB7
#define QUICKUSB_TIMER        0xB8
#define QUICKUSB_JTAG         0xB9
#define QUICKUSB_SCRIPT       0xBF
#define QUICKUSB_RESET        0xC0
#define QUICKUSB_TIMEOUT      0xC1


// QuickUSB FPGA commands (wIndex values for QUICKUSB_FPGA)
#define FPGA_START            0
#define FPGA_WRITE            1
#define FPGA_NSTATUS          2
#define FPGA_ISCONFIG         3

// QuickUSB RS-232 Commands 
// (wIndex values for QUICKUSB_RS232, wAddr has the portNum)
#define PORT_GETSET_DIR       0
#define PORT_READWRITE        1

// QuickUSB RS-232 Commands (wIndex values for QUICKUSB_RS232)
#define RS232_SETGET_BAUD     0
#define RS232_SETGET_NUM      1
#define RS232_READWRITE       2

// QuickUSB I2C Commands (wIndex values for QUICKUSB_I2C)
#define I2C_READWRITE         0

// QuickUSB SPI Commands (wIndex values for QUICKUSB_SPI)
#define SPI_SETPINS           0
#define SPI_READWRITE         1
#define SPI_WRITEANDREAD      2

// QuickUSB JTAG Commands (wIndex values for QUICKUSB_FPGA)
#define JTAG_DIRECT           0
#define JTAG_CLKRAW           1
#define JTAG_CLKMULT          2
#define JTAG_RECIRC           3
#define JTAG_FLUSH            4

// QuickUSB Storage Constants
#define STORAGE_SIZE        2048
#define STORAGE_PAGE_SIZE   64
#define STORAGE_BASE_ADDR   0x3800
#define STORAGE_I2C_ADDR    0x51

// QuickUSB RESET Commands (wIndex values for QUICKUSB_RESET)
#define RESET_ALL             0
#define RESET_CPU             1

#define RESET_FIFO_EP2        2
#define RESET_FIFO_EP4        3
#define RESET_FIFO_EP6        4
#define RESET_FIFO_EP8        5

#define RESET_ABORT_GPIF      6

#define QUICKUSB_CYCLE_PARENT_PORT 0x0001 //IOCTL_QUSB_CYCLE
#define QUICKUSB_RESET_PARENT_PORT 0x0002 //IOCTL_QUSB_RESET
#define QUICKUSB_ABORT_PIPE        0x0004 //IOCTL_QUSB_ABORTPIPE
#define QUICKUSB_RESET_PIPE        0x0008 //IOCTL_QUSB_RESETPIPE
#define QUICKUSB_RESET_FIFO_EP     0x0010
#define QUICKUSB_ABORT_GPIF        0x0020

typedef enum _WDF_USB_PIPE_TYPE { 
  WdfUsbPipeTypeInvalid       = 0,
  WdfUsbPipeTypeControl       = 1,
  WdfUsbPipeTypeIsochronous   = 2,
  WdfUsbPipeTypeBulk          = 3,
  WdfUsbPipeTypeInterrupt     = 4 
} WDF_USB_PIPE_TYPE;

typedef struct _WDF_USB_PIPE_INFORMATION {
  ULONG             Size;
  ULONG             MaximumPacketSize;
  UCHAR             EndpointAddress;
  UCHAR             Interval;
  UCHAR             SettingIndex;
  WDF_USB_PIPE_TYPE PipeType;
  ULONG             MaximumTransferSize;
} WDF_USB_PIPE_INFORMATION, *PWDF_USB_PIPE_INFORMATION;

#endif //__QUICKUSB_API_HEADER__
