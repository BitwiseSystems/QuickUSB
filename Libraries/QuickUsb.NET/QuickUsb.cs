/* ========================================================================
	Title        : QuickUSB.cs
	Description  : 
	Notes        : None
	History      :

	Copyright © 2012 Bitwise Systems.  All rights reserved.
	This software contains confidential information and trade secrets of
	Bitwise Systems and is protected by United States and international
	copyright laws.  Use, disclosure, or reproduction is prohibited
	without the prior express written permission of Bitwise Systems,
	except as agreed in the QuickUSB license agreement.

	Use, duplication or disclosure by the U.S. Government is subject to
	restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
	(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle
	Real, Suite E, Goleta, CA  93117.

	Bitwise Systems
	6489 Calle Real, Suite E
	Santa Barbara, CA  93117
	Voice: (805) 683-6469
	Fax  : (805) 683-4833
	Web  : www.bitwisesys.com
	email: support@bitwisesys.com
   ======================================================================== */
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace BitwiseSystems
{
    /// <summary>
    /// QuickUSB Object Class
    /// </summary>
	public partial class QuickUsb
    {
        //---------------------------------------------------------------------
        #region Instance Variables
        //---------------------------------------------------------------------

		private int m_globalOpen = 0;
        private int m_localOpen = 0;
        private IntPtr m_hDevice = IntPtr.Zero;
        private string m_devname = null;
        private Error m_lastError = QuickUsb.Error.ERROR_NO_ERROR;
        private uint m_lastDriverError = 0;
        private int m_LegacyTransCount = 0;

        // Keep track of managed QuickUsb objects
        private static Dictionary<IntPtr, QuickUsb> QuickUsbObjects = new Dictionary<IntPtr, QuickUsb>();

        // Keep track of managed BulkStream objects to allow automatic internal pinning
        private static Dictionary<BulkStream, GCHandle> PinnedAsyncBulkStreamObjects = new Dictionary<BulkStream, GCHandle>();

        // Keep track of managed byte[] and ushort data objects to allow automatic internal pinning for asynchronous requests
        private static Dictionary<byte[], GCHandle> PinnedAsyncBulkDataByteObjects = new Dictionary<byte[], GCHandle>();
        private static Dictionary<ushort[], GCHandle> PinnedAsyncBulkDataUShortObjects = new Dictionary<ushort[], GCHandle>();
        private static Dictionary<IntPtr, byte[]> PinnedAsyncBulkDataBytePtrs = new Dictionary<IntPtr, byte[]>();
        private static Dictionary<IntPtr, ushort[]> PinnedAsyncBulkDataUShortPtrs = new Dictionary<IntPtr, ushort[]>();

        // Keep track of managed BulkStream objects to allow automatic internal pinning for streaming requests
        private static Dictionary<byte, InternalStreamPinningData> PinnedStreamingObjects = new Dictionary<byte, InternalStreamPinningData>();
        private static Dictionary<IntPtr, byte[]> PinnedStreamingBulkDataBytePtrs = new Dictionary<IntPtr, byte[]>();
        private static Dictionary<IntPtr, ushort[]> PinnedStreamingBulkDataUShortPtrs = new Dictionary<IntPtr, ushort[]>();

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Constants, Enums, and Structures
        //---------------------------------------------------------------------

        /// <summary>
        /// Path to QuickUSB.dll file
        /// </summary>
		public const string Path = "QuickUSB.dll";

        /// <summary>
        /// Constant time value representing infinite time
        /// </summary>
        public const uint InfiniteTime = 0xFFFFFFFF;

        /// <summary>
        /// QuickUSB Ports Enumeration
        /// </summary>
		public enum Port : ushort
		{
            /// <summary>
            /// Port A index
            /// </summary>
			PORT_A = 0,
            
            /// <summary>
            /// Port A index
            /// </summary>
			A = 0,

            /// <summary>
            /// Port B index
            /// </summary>
			PORT_B = 1,

            /// <summary>
            /// Port B index
            /// </summary>
			B = 1,

            /// <summary>
            /// Port C index
            /// </summary>
			PORT_C = 2,

            /// <summary>
            /// Port C index
            /// </summary>
			C = 2,

            /// <summary>
            /// Port D index
            /// </summary>
			PORT_D = 3,

            /// <summary>
            /// Port D index
            /// </summary>
			D = 3,

            /// <summary>
            /// Port E index
            /// </summary>
			PORT_E = 4,

            /// <summary>
            /// Port E index
            /// </summary>
			E = 4,
		}

		/// <summary>
		/// QuickUSB String Descriptor Enumeration
		/// </summary>
		public enum StringDescriptor : byte 
		{
            /// <summary>
            /// Make string descriptor index
            /// </summary>
			MAKE = (byte) 1,

            /// <summary>
            /// Make string descriptor index
            /// </summary>
			Make = (byte)1,

            /// <summary>
            /// Model string descriptor index
            /// </summary>
			MODEL = (byte) 2,

            /// <summary>
            /// Model string descriptor index
            /// </summary>
			Model = (byte)2,

            /// <summary>
            /// Serial string descriptor index
            /// </summary>
			SERIAL = (byte)3,

            /// <summary>
            /// Serial string descriptor index
            /// </summary>
			Serial = (byte)3,
		};

        /// <summary>
        /// QuickUSB Settings Enumeration
        /// </summary>
		public enum Setting : ushort
		{
            /// <summary>
            /// Ep26Config setting index
            /// </summary>
			SETTING_EP26CONFIG = 0,

            /// <summary>
            /// Ep26Config setting index
            /// </summary>
			Ep26Config = 0,

            /// <summary>
            /// WordWide setting index
            /// </summary>
			SETTING_WORDWIDE = 1,

            /// <summary>
            /// WordWide setting index
            /// </summary>
			WordWide = 1,

            /// <summary>
            /// DataAddress setting index
            /// </summary>
			SETTING_DATAADDRESS = 2,

            /// <summary>
            /// DataAddress setting index
            /// </summary>
			DataAddress = 2,

            /// <summary>
            /// FifoConfig setting index
            /// </summary>
			SETTING_FIFO_CONFIG = 3,

            /// <summary>
            /// FifoConfig setting index
            /// </summary>
			FifoConfig = 3,

            /// <summary>
            /// FpgaType setting index
            /// </summary>
			SETTING_FPGATYPE = 4,

            /// <summary>
            /// FpgaType setting index
            /// </summary>
			FpgaType = 4,

            /// <summary>
            /// CpuConfig setting index
            /// </summary>
			SETTING_CPUCONFIG = 5,

            /// <summary>
            /// CpuConfig setting index
            /// </summary>
			CpuConfig = 5,

            /// <summary>
            /// SpiConfig setting index
            /// </summary>
			SETTING_SPICONFIG = 6,

            /// <summary>
            /// SpiConfig setting index
            /// </summary>
			SpiConfig = 6,

            /// <summary>
            /// SlaveFifoFlags setting index
            /// </summary>
			SETTING_SLAVEFIFOFLAGS = 7,

            /// <summary>
            /// SlaveFifoFlags setting index
            /// </summary>
			SlaveFifoFlags = 7,

            /// <summary>
            /// I2Ctl setting index
            /// </summary>
			SETTING_I2CTL = 8,

            /// <summary>
            /// I2Ctl setting index
            /// </summary>
			I2Ctl = 8,

            /// <summary>
            /// PortA setting index
            /// </summary>
			SETTING_PORTA = 9,

            /// <summary>
            /// PortA setting index
            /// </summary>
			PortA = 9,

            /// <summary>
            /// PortB setting index
            /// </summary>
			SETTING_PORTB = 10,

            /// <summary>
            /// PortB setting index
            /// </summary>
			PortB = 10,

            /// <summary>
            /// PortC setting index
            /// </summary>
			SETTING_PORTC = 11,

            /// <summary>
            /// PortC setting index
            /// </summary>
			PortC = 11,

            /// <summary>
            /// PortD setting index
            /// </summary>
			SETTING_PORTD = 12,

            /// <summary>
            /// PortD setting index
            /// </summary>
			PortD = 12,

            /// <summary>
            /// PortE setting index
            /// </summary>
			SETTING_PORTE = 13,

            /// <summary>
            /// PortE setting index
            /// </summary>
			PortE = 13,

            /// <summary>
            /// PortACCfg setting index
            /// </summary>
			SETTING_PORTACCFG = 14,

            /// <summary>
            /// PortACCfg setting index
            /// </summary>
			PortACCfg = 14,

            /// <summary>
            /// PinFlags setting index
            /// </summary>
			SETTING_PINFLAGS = 15,

            /// <summary>
            /// PinFlags setting index
            /// </summary>
			PinFlags = 15,

            /// <summary>
            /// VersionBuild setting index
            /// </summary>
			SETTING_VERSIONBUILD = 16,

            /// <summary>
            /// VersionBuild setting index
            /// </summary>
			VersionBuild = 16,

            /// <summary>
            /// VersionSpeed setting index
            /// </summary>
			SETTING_VERSIONSPEED = 17,

            /// <summary>
            /// VersionSpeed setting index
            /// </summary>
            VersionSpeed = 17,

            /// <summary>
            /// TimeoutHigh setting index
            /// </summary>
            SETTING_TIMEOUT_HIGH = 18,

            /// <summary>
            /// TimeoutHigh setting index
            /// </summary>
            TimeoutHigh = 18,

            /// <summary>
            /// TimeoutLow setting index
            /// </summary>
            SETTING_TIMEOUT_LOW = 19,

            /// <summary>
            /// TimeoutLow setting index
            /// </summary>
            TimeoutLow = 19
		}

        /// <summary>
        /// QuickUSB FPGA Configuration Type Enumeration
        /// </summary>
		public enum FpgaConfType : byte
		{
            /// <summary>
            /// AlteraPS configuration type
            /// </summary>
			FPGATYPE_ALTERAPS = 0,

            /// <summary>
            /// AlteraPS configuration type
            /// </summary>
			AlteraPS = 0,

            /// <summary>
            /// Xilinx configuration type
            /// </summary>
			FPGATYPE_XILINXSS = 1,

            /// <summary>
            /// Xilinx configuration type
            /// </summary>
			XilinxSS = 1,
		}

        /// <summary>
        /// EPCS ID Enumeration
        /// </summary>
        public enum EpcsId : ushort
        {
            /// <summary>
            /// An unknown EPCS device
            /// </summary>
            EpcsUnknown = 0x00,

            /// <summary>
            /// An EPCS1 device
            /// </summary>
            Epcs1 = 0x10,

            /// <summary>
            /// An EPCS4 device
            /// </summary>
            Epcs4 = 0x12,

            /// <summary>
            /// An EPCS16 device
            /// </summary>
            Epcs16 = 0x14,

            /// <summary>
            /// An EPCS64 device
            /// </summary>
            Epcs64 = 0x16,
        }

        /// <summary>
        /// EPCS Flags Enumeration
        /// </summary>
        public enum EpcsFlags : uint
        {
            /// <summary>
            /// Default operation
            /// </summary>
            None = 0x00,

            /// <summary>
            /// Ignore the nCE line
            /// </summary>
            IgnoreNCE = 0x01,

            /// <summary>
            /// An EPCS1 device
            /// </summary>
            SkipErase = 0x02,
        }

        /// <summary>
        /// EPCS Size Enumeration
        /// </summary>
        public enum EpcsSize : uint
        {
            /// <summary>
            /// EPCS1 Size (in bytes)
            /// </summary>
            Epcs1 = (1048576 / 8),

            /// <summary>
            /// EPCS1 Size (in bytes)
            /// </summary>
            QUICKUSB_EPCS1_SIZE = Epcs1,

            /// <summary>
            /// EPCS4 Size (in bytes)
            /// </summary>
            Epcs4 = (Epcs1 * 4),

            /// <summary>
            /// EPCS4 Size (in bytes)
            /// </summary>
            QUICKUSB_EPCS4_SIZE = Epcs4,

            /// <summary>
            /// EPCS16 Size (in bytes)
            /// </summary>
            Epcs16 = (Epcs1 * 16),

            /// <summary>
            /// EPCS16 Size (in bytes)
            /// </summary>
            QUICKUSB_EPCS16_SIZE = Epcs16,

            /// <summary>
            /// EPCS64 Size (in bytes)
            /// </summary>
            Epcs64 = (Epcs1 * 64),

            /// <summary>
            /// EPCS64 Size (in bytes)
            /// </summary>
            QUICKUSB_EPCS64_SIZE = Epcs64
        }
        
        /// <summary>
        /// Programming Firmware Enumeration
        /// </summary>
        public enum Programming : uint
        {
            /// <summary>
            /// Program a new serial number.  Bitwise or (|) this together 
            /// with the serial number to program a new serial.  The serial 
            /// number must be between 1 and 65535.
            /// </summary>
            ProgramSerial = 0x10000000,

            /// <summary>
            /// Preserve any customizations performed by the QuickUSB 
            /// Customizer
            /// </summary>
            PreserveCustomization = 0x00010000,

            /// <summary>
            /// Preserve defaults (they are not preserved if the IO Model 
            /// changes)
            /// </summary>
            PreserveDefaults = 0x00020000,

            /// <summary>
            /// Preserve the GPIF
            /// </summary>
            PreserveGpif = 0x00040000,

            /// <summary>
            /// Preserve the serial
            /// </summary>
            PreserveSerial = 0x00080000,
        }

        /// <summary>
        /// Data Flags Enumeration
        /// </summary>
        public enum DataFlags : uint
        {
            /// <summary>
            /// Perform a normal data transfer
            /// </summary>
            None = 0x00,

            /// <summary>
            /// Process this data request in a different request queue from 
            /// normal data operations.  This flag will cause this request to 
            /// be processed out-of-order from other pending requests, except 
            /// those already issued out-of-order.
            /// </summary>
            OutOfOrder = 0x01,
        }

        /// <summary>
        /// QuickUSB Error Enumeration
        /// </summary>
		public enum Error : uint
		{
            /// <summary>
            /// No error
            /// </summary>
			ERROR_NO_ERROR = 0,

            /// <summary>
            /// No error
            /// </summary>
			NoError = 0,

            /// <summary>
            /// Out of memory error
            /// </summary>
			ERROR_OUT_OF_MEMORY = 1,

            /// <summary>
            /// Out of memory error
            /// </summary>
			OutOfMemory = 1,

            /// <summary>
            /// Cannot open module
            /// </summary>
			ERROR_CANNOT_OPEN_MODULE = 2,

            /// <summary>
            /// Cannot open module
            /// </summary>
			CannotOpenModule = 2,
            
            /// <summary>
            /// Cannot find specified QuickUSB device
            /// </summary>
			ERROR_CANNOT_FIND_DEVICE = 3,
            
            /// <summary>
            /// Cannot find specified QuickUSB device
            /// </summary>
			CannotFindDevice = 3,

            /// <summary>
            /// IOCTL failed
            /// </summary>
			ERROR_IOCTL_FAILED = 4,
            
            /// <summary>
            /// IOCTL failed
            /// </summary>
			IoctlFailed = 4,

            /// <summary>
            /// Invalid parameter
            /// </summary>
			ERROR_INVALID_PARAMETER = 5,

            /// <summary>
            /// Invalid parameter
            /// </summary>
			InvalidParameter = 5,

            /// <summary>
            /// Operation timed out
            /// </summary>
			ERROR_TIMEOUT = 6,

            /// <summary>
            /// Operation timed out
            /// </summary>
			Timeout = 6,

            /// <summary>
            /// Function not supported
            /// </summary>
			ERROR_FUNCTION_NOT_SUPPORTED = 7,

            /// <summary>
            /// Function not supported
            /// </summary>
			FunctionNotSupported = 7,
            
            /// <summary>
            /// I2C bus error
            /// </summary>
			ERROR_I2C_BUS_ERROR = 8,

            /// <summary>
            /// I2C bus error
            /// </summary>
			I2cBusError = 8,

            /// <summary>
            /// No ACK received from I2C device
            /// </summary>
			ERROR_I2C_NO_ACK = 9,

            /// <summary>
            /// No ACK received from I2C device
            /// </summary>
			I2cNoAck = 9,

            /// <summary>
            /// An I2C slave device is holding SCL low
            /// </summary>
			ERROR_I2C_SLAVE_WAIT = 10,

            /// <summary>
            /// An I2C slave device is holding SCL low
            /// </summary>
			I2cSlaveWait = 10,

            /// <summary>
            /// Timeout on the I2C bus
            /// </summary>
			ERROR_I2C_TIMEOUT = 11,

            /// <summary>
            /// Timeout on the I2C bus
            /// </summary>
			I2cTimeout = 11,

            /// <summary>
            /// Unknown driver
            /// </summary>
			QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE = 12,

            /// <summary>
            /// Unknown driver
            /// </summary>
            UnknownDriverType = 12,

            /// <summary>
            /// The QuickUSB device has already been opened
            /// </summary>
			QUICKUSB_ERROR_ALREADY_OPENED = 13,

            /// <summary>
            /// The QuickUSB device has already been opened
            /// </summary>
            AlreadyOpened = 13,

            /// <summary>
            /// The QuickUSB device has already been closed or was never opened
            /// </summary>
			QUICKUSB_ERROR_CANNOT_CLOSE_MODULE = 14,
            
            /// <summary>
            /// The QuickUSB device has already been closed or was never opened
            /// </summary>
            CannotCloseModule = 14,

            /// <summary>
            /// Unable to initialize the FPGA configuration process
            /// </summary>
			QUICKUSB_ERROR_FPGA_INIT_FAILED = 15,

            /// <summary>
            /// Unable to initialize the FPGA configuration process
            /// </summary>
            FpgaInitFailed = 15,

            /// <summary>
            /// The requested amount of data was not a multiple of 512 bytes
            /// </summary>
			QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_512 = 16,

            /// <summary>
            /// The requested amount of data was not a multiple of 512 bytes
            /// </summary>
            PacketNotMultiple512 = 16,

            /// <summary>
            /// The requested amount of data was not a multiple of 64 bytes
            /// </summary>
			QUICKUSB_ERROR_PACKET_NOT_MULTIPLE_64 = 17,

            /// <summary>
            /// The requested amount of data was not a multiple of 64 bytes
            /// </summary>
            PacketNotMultiple64 = 17,
            
            /// <summary>
            /// Unknown system error
            /// </summary>
			QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR = 18,

            /// <summary>
            /// Unknown system error
            /// </summary>
            UnknownSystemError = 18,

            /// <summary>
            /// Operation was aborted
            /// </summary>
			QUICKUSB_ERROR_ABORTED = 19,

            /// <summary>
            /// Operation was aborted
            /// </summary>
            Aborted = 19,

            /// <summary>
            /// Function has been deprecated
            /// </summary>
			QUICKUSB_ERROR_DEPRECATED = 20,

            /// <summary>
            /// Function has been deprecated
            /// </summary>
            Deprecated = 20,

            /// <summary>
            /// Invalid serial number
            /// </summary>
			QUICKUSB_ERROR_INVALID_SERIAL = 21,

            /// <summary>
            /// Invalid serial number
            /// </summary>
            InvalidSerial = 21,

            /// <summary>
            /// Cannot open file
            /// </summary>
			QUICKUSB_ERROR_CANNOT_OPEN_FILE = 22,

            /// <summary>
            /// Cannot open file
            /// </summary>
            CannotOpenFile = 22,

            /// <summary>
            /// Firmware verify failed
            /// </summary>
			QUICKUSB_ERROR_VERIFY_FAILED = 23,

            /// <summary>
            /// Firmware verify failed
            /// </summary>
            VerifyFailed = 23,

            /// <summary>
            /// Firmware error
            /// </summary>
			QUICKUSB_ERROR_FIRMWARE_ERROR = 24,

            /// <summary>
            /// Firmware error
            /// </summary>
            FirmwareError = 24,

            /// <summary>
            /// Operation already completed
            /// </summary>
			QUICKUSB_ERROR_ALREADY_COMPLETED = 25,

            /// <summary>
            /// Operation already completed
            /// </summary>
            AlreadyCompleted = 25,

            /// <summary>
            /// Operation not yet completed
            /// </summary>
			QUICKUSB_ERROR_NOT_COMPLETED = 26,

            /// <summary>
            /// Operation not yet completed
            /// </summary>
            NotCompleted = 26,
            
            /// <summary>
            /// FPGA configuration failed
            /// </summary>
			QUICKUSB_ERROR_FPGA_CONFIG_FAILED = 27,

            /// <summary>
            /// FPGA configuration failed
            /// </summary>
            FpgaConfigFailed = 27,

            /// <summary>
            /// Invalid operation
            /// </summary>
			QUICKUSB_ERROR_INVALID_OPERATION = 28,

            /// <summary>
            /// Invalid operation
            /// </summary>
            InvalidOperation = 28,
                
            /// <summary>
            /// Too many outstanding async requests have been issued
            /// </summary>
			QUICKUSB_ERROR_TOO_MANY_REQUESTS = 29,

            /// <summary>
            /// Too many outstanding async requests have been issued
            /// </summary>
            TooManyRequests = 29,

            /// <summary>
            /// An EPCS device was not found
            /// </summary>
            QUICKUSB_ERROR_EPCS_NOT_FOUND = 30,

            /// <summary>
            /// An EPCS device was not found
            /// </summary>
            EpcsNotFound = 30,

            /// <summary>
            /// The EPCS device is not large enough to fit all of the specified data
            /// </summary>
            QUICKUSB_ERROR_EPCS_TOO_SMALL = 31,

            /// <summary>
            /// The EPCS device is not large enough to fit all of the specified data
            /// </summary>
            EpcsTooSmall = 31,

            /// <summary>
            /// The API is currently not streaming data
            /// </summary>
            QUICKUSB_ERROR_NOT_STREAMING = 32,

            /// <summary>
            /// The API is currently not streaming data
            /// </summary>
            NotStreaming = 32,

            /// <summary>
            /// The specified data buffer is not aligned
            /// </summary>
            QUICKUSB_ERROR_BUFFER_NOT_ALIGNED = 33,

            /// <summary>
            /// The specified data buffer is not aligned
            /// </summary>
            BufferNotAligned = 33,

            /// <summary>
            /// An internal error has occured
            /// </summary>
            QUICKUSB_ERROR_INTERNAL_ERROR = 34,

            /// <summary>
            /// An internal error has occured
            /// </summary>
            InternalError = 34,

            /// <summary>
            /// The operation cannot be performed because the device is in the processes of closing
            /// </summary>
            QUICKUSB_ERROR_DEVICE_IS_CLOSING = 35,

            /// <summary>
            /// The operation cannot be performed because the device is in the processes of closing
            /// </summary>
            DeviceIsClosing = 35,

            /// <summary>
            /// The operation cannot be performed because it will corrupt the device
            /// </summary>
            QUICKUSB_ERROR_PROTECTION = 36,

            /// <summary>
            /// The operation cannot be performed because it will corrupt the device
            /// </summary>
            Protection = 36,

            /// <summary>
            /// Additional data is being requested
            /// </summary>
            QUICKUSB_ERROR_NEED_DATA = 37,

            /// <summary>
            /// Additional data is being requested
            /// </summary>
            NeedData = 37,

            /// <summary>
            /// The file was not found
            /// </summary>
            QUICKUSB_ERROR_FILE_NOT_FOUND = 38,

            /// <summary>
            /// The file was not found
            /// </summary>
            FileNotFound = 38,

            /// <summary>
            /// The file already exists
            /// </summary>
            QUICKUSB_ERROR_FILE_ALREADY_EXISTS = 39,

            /// <summary>
            /// The file already exists
            /// </summary>
            FileAlreadyExists = 39,

            /// <summary>
            /// There was a file read/write error
            /// </summary>
            QUICKUSB_ERROR_FILE_RW = 40,

            /// <summary>
            /// There was a file read/write error
            /// </summary>
            FileRW = 40,

            /// <summary>
            /// The end of the file has been reached
            /// </summary>
            QUICKUSB_ERROR_FILE_EOF = 41,

            /// <summary>
            /// The end of the file has been reached
            /// </summary>
            EndOfFile = 41,

            /// <summary>
            /// An invalid file name was specified
            /// </summary>
            QUICKUSB_ERROR_FILE_NAME = 42,

            /// <summary>
            /// An invalid file name was specified
            /// </summary>
            FileName = 42,

            /// <summary>
            /// Access was denied (permissions error)
            /// </summary>
            QUICKUSB_ERROR_ACCESS_DENIED = 43,

            /// <summary>
            /// Access was denied (permissions error)
            /// </summary>
            AccessDenied = 43,

            /// <summary>
            /// Sharing violation (permissions error)
            /// </summary>
            QUICKUSB_ERROR_SHARING_VIOLATION = 44,

            /// <summary>
            /// Sharing violation (permissions error)
            /// </summary>
            SharingViolation = 44
		}

        /// <summary>
        /// QuickUSB Open Flags Enumeration
        /// </summary>
        public enum OpenFlag : ushort
        {
            /// <summary>
            /// Open the device normally (the device may be in use by another applciation)
            /// </summary>
            Normal = 0,

            /// <summary>
            /// Open the device normally (the device may be in use by another applciation)
            /// </summary>
            QUICKUSB_OPEN_NORMAL = 0,

            /// <summary>
            /// Open the device only if it is closed (i.e. not in use by another application)
            /// </summary>
            OpenIfClosed = 1,

            /// <summary>
            /// Open the device only if it is closed (i.e. not in use by another application)
            /// </summary>
            QUICKUSB_OPEN_IF_CLOSED = 1,
        }

        /// <summary>
        /// QuickUSB Stream Flags Enumeration
        /// </summary>
        public enum StreamFlag : uint
        {
            /// <summary>
            /// For reads streams, overwrite the specified file if it already exists
            /// </summary>
            CreateAlways = 1,

            /// <summary>
            /// For reads streams, overwrite the specified file if it already exists
            /// </summary>
            QUICKUSB_STREAM_CREATE_ALWAYS = 1,

            /// <summary>
            /// For write streams, automatically seek to the beginning of the file 
            /// once the end of file has been reached and continue to stream
            /// </summary>
            LoopAtEof = 2,

            /// <summary>
            /// For write streams, automatically seek to the beginning of the file 
            /// once the end of file has been reached and continue to stream
            /// </summary>
            QUICKUSB_STREAM_LOOP_AT_EOF = 2,
        }

        /// <summary>
        /// QuickUSB Stream Status Enumeration
        /// </summary>
        public enum StreamStatus : uint
        {
            /// <summary>
            /// The stream is in an unknown state
            /// </summary>
            Unknown = 0,

            /// <summary>
            /// The stream is in an unknown state
            /// </summary>
            QUICKUSB_STREAM_STATUS_UNKNOWN = 0,

            /// <summary>
            /// The stream is currently running
            /// </summary>
            Running = 1,

            /// <summary>
            /// The stream is currently running
            /// </summary>
            QUICKUSB_STREAM_STATUS_RUNNING = 1,
            
            /// <summary>
            /// The stream has stopped running
            /// </summary>
            Stopped = 2,

            /// <summary>
            /// The stream has stopped running
            /// </summary>
            QUICKUSB_STREAM_STATUS_STOPPED = 2,
            
            /// <summary>
            /// The stream has been paused and is currently not running
            /// </summary>
            Paused = 3,

            /// <summary>
            /// The stream has been paused and is currently not running
            /// </summary>
            QUICKUSB_STREAM_STATUS_PAUSED = 3
        }

        /// <summary>
        /// QuickUSB Statistics Unit Enumeration
        /// </summary>
        public enum StatUnit : uint
        {
            /// <summary>
            /// Report statistic in bytes
            /// </summary>
            Bytes = 0x0001,

            /// <summary>
            /// Report statistic in bytes
            /// </summary>
            QUICKUSB_STAT_UNIT_BYTES = 0x0001,

            /// <summary>
            /// Report statistic in kilobytes
            /// </summary>
            KB = 0x0002,

            /// <summary>
            /// Report statistic in kilobytes
            /// </summary>
            QUICKUSB_STAT_UNIT_KB = 0x0002,

            /// <summary>
            /// Report statistic in megabytes
            /// </summary>
            MB = 0x0004,

            /// <summary>
            /// 
            /// </summary>
            QUICKUSB_STAT_UNIT_MB = 0x0004,

            /// <summary>
            /// Report statistic in megabytes
            /// </summary>
            GB = 0x0008,

            /// <summary>
            /// Report statistic in gigabytes
            /// </summary>
            QUICKUSB_STAT_UNIT_GB = 0x0008,

            /// <summary>
            /// Report statistic in (unit) / nanoseconds
            /// </summary>
            PerNS = 0x0010,

            /// <summary>
            /// Report statistic in (unit) / nanoseconds
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_NS = 0x0010,

            /// <summary>
            /// Report statistic in (unit) / microseconds
            /// </summary>
            PerUS = 0x0020,

            /// <summary>
            /// Report statistic in (unit) / microseconds
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_US = 0x0020,

            /// <summary>
            /// Report statistic in (unit) / milliseconds
            /// </summary>
            PerMS = 0x0040,

            /// <summary>
            /// Report statistic in (unit) / milliseconds
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_MS = 0x0040,

            /// <summary>
            /// Report statistic in (unit) / seconds
            /// </summary>
            PerSec = 0x0080,

            /// <summary>
            /// Report statistic in (unit) / seconds
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_SEC = 0x0080,

            /// <summary>
            /// Report statistic in (unit) / minutes
            /// </summary>
            PerMin = 0x0100,

            /// <summary>
            /// Report statistic in (unit) / minutes
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_MIN = 0x0100,

            /// <summary>
            /// Report statistic in (unit) / hours
            /// </summary>
            PerHour = 0x0200,

            /// <summary>
            /// Report statistic in (unit) / hours
            /// </summary>
            QUICKUSB_STAT_UNIT_PER_HOUR = 0x0200,

            /// <summary>
            /// Report statistic in bytes / second
            /// </summary>
            BytesPerSec = StatUnit.Bytes | StatUnit.PerSec,

            /// <summary>
            /// Report statistic in bytes / second
            /// </summary>
            QUICKUSB_STAT_UNIT_BYTES_PER_SEC = StatUnit.Bytes | StatUnit.PerSec,

            /// <summary>
            /// Report statistic in kilobytes / second
            /// </summary>
            KBPerSec = StatUnit.KB | StatUnit.PerSec,

            /// <summary>
            /// Report statistic in kilobytes / second
            /// </summary>
            QUICKUSB_STAT_UNIT_KB_PER_SEC = StatUnit.KB | StatUnit.PerSec,

            /// <summary>
            /// Report statistic in megabytes / second
            /// </summary>
            MBPerSec = StatUnit.MB | StatUnit.PerSec,

            /// <summary>
            /// Report statistic in megabytes / second
            /// </summary>
            QUICKUSB_STAT_UNIT_MB_PER_SEC = StatUnit.MB | StatUnit.PerSec,
        }

        /// <summary>
        /// QuickUSB Statistics Enumeration
        /// </summary>
        public enum Stat : uint
        {
            /// <summary>
            /// Indicates all statistics.  Typically sent as a parameter to
            /// QuickUsb.ResetStatistic to intialize all statistics before beginning
            /// to read/write data.
            /// </summary>
            All = 0,

            /// <summary>
            /// Indicates all statistics.  Typically sent as a parameter to
            /// QuickUsb.ResetStatistic to intialize all statistics before beginning
            /// to read/write data.
            /// </summary>
            QUICKUSB_STAT_ALL = 0,

            /// <summary>
            /// Report the throughput of data reads (typically in MB/s)
            /// </summary>
            ReadThroughput = 1,

            /// <summary>
            /// Report the throughput of data reads (typically in MB/s)
            /// </summary>
            QUICKUSB_STAT_READ_THROUGHPUT = 1,

            /// <summary>
            /// Report the throughput of data writes (typically in MB/s)
            /// </summary>
            WriteThroughput = 2,

            /// <summary>
            /// Report the throughput of data writes (typically in MB/s)
            /// </summary>
            QUICKUSB_STAT_WRITE_THROUGHPUT = 2,

            /// <summary>
            /// Report the total throughput of both reads and writes (equal to the
            /// sum of read throughput and write throughput and typically in MB/s)
            /// </summary>
            TotalThroughput = 3,

            /// <summary>
            /// Report the total throughput of both reads and writes (equal to the
            /// sum of read throughput and write throughput and typically in MB/s)
            /// </summary>
            QUICKUSB_STAT_TOTAL_THROUGHPUT = 3,

            /// <summary>
            /// Report the total amount of data read (typically in MB's)
            /// </summary>
            TotalDataRead = 4,

            /// <summary>
            /// Report the total amount of data read (typically in MB's)
            /// </summary>
            QUICKUSB_STAT_TOTAL_DATA_READ = 4,

            /// <summary>
            /// Report the total amount of data written (typically in MB's)
            /// </summary>
            TotalDataWritten = 5,

            /// <summary>
            /// Report the total amount of data written (typically in MB's)
            /// </summary>
            QUICKUSB_STAT_TOTAL_DATA_WRITTEN = 5,
        }

        /// <summary>
        /// QuickUSB BulkStream Structure
        /// </summary>
        [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi, Pack=4)]
        public class BulkStream
        {
            /// <summary>
            /// Reserved
            /// </summary>
            public System.Threading.NativeOverlapped InternalData;

            /// <summary>
            /// Handle to QuickUSB device
            /// </summary>
            public IntPtr Handle;

            /// <summary>
            /// Data buffer
            /// </summary>
            public IntPtr Buffer;

            /// <summary>
            /// Completion routine
            /// </summary>
            //public QuickUsb.BulkStreamCompletionRoutine CompletionRoutine;
            public IntPtr CompletionRoutine;

            /// <summary>
            /// User data
            /// </summary>
            public IntPtr Tag;

            /// <summary>
            /// Reserved
            /// </summary>
            public IntPtr InternalReserved;

            /// <summary>
            /// The number of bytes requested
            /// </summary>
            public uint BytesRequested;

            /// <summary>
            /// The number of bytes actually transferred
            /// </summary>
            public uint BytesTransferred;

            /// <summary>
            /// The error associated with this request
            /// </summary>
            public Error Error;

            /// <summary>
            /// Reserved
            /// </summary>
            public uint InternalStatus;

            /// <summary>
            /// The ID of the stream associated with this request
            /// </summary>
            public uint StreamID;

            /// <summary>
            /// The ID of this request
            /// </summary>
            public uint RequestID;

            /// <summary>
            /// Reserved
            /// </summary>
            public uint Reserved;

            /// <summary>
            /// Reserved
            /// </summary>
            public ushort StreamType;

            /// <summary>
            /// This size of this structure
            /// </summary>
            public byte StructSize;

            /// <summary>
            /// Reserved
            /// </summary>
            public byte CompletionType;
        }

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Properties
        //---------------------------------------------------------------------

        /// <summary>
        /// A read-only property containing the QuickUSB handle to the device, if opened
        /// </summary>
        public IntPtr Handle
        {
            get
            {
                return m_hDevice;
            }
        }

		/// <summary>
		/// A read-only property which indicates if the module is currently opened by the API
		/// </summary>
		public bool IsOpened
		{
			get
			{
				return (m_globalOpen != 0) || (m_localOpen != 0);
			}
		}

        /// <summary>
        /// The name of the QuickUSB device
        /// </summary>
		public string Name
		{
			get
			{
				return m_devname;
			}

			set
			{
				m_devname = value;
			}
		}

        /// <summary>
        /// The make of the QuickUSB device (as returned by QuickUsb.GetStringDescriptor)
        /// </summary>
		public string Make
		{
			get
			{
				return GetStringDescriptor(QuickUsb.StringDescriptor.Make);
			}
		}

        /// <summary>
        /// The model of the QuickUSB device (as returned by QuickUsb.GetStringDescriptor)
        /// </summary>
		public string Model
		{
			get
			{
				return GetStringDescriptor(QuickUsb.StringDescriptor.Model);
			}
		}

        /// <summary>
        /// The serial number of the QuickUSB device (as returned by QuickUsb.GetStringDescriptor)
        /// </summary>
		public string Serial
		{
			get
			{
				return GetStringDescriptor(QuickUsb.StringDescriptor.Serial);
			}
		}

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region DLL Imports
        //---------------------------------------------------------------------

        static class NativeMethods
        {
            /// <summary>
            /// Return extended error information for the last API call that 
            /// returned a value of FALSE (0).
            /// </summary>
            /// <param name="error">A PLONG pointing to a variable in which to 
            /// place the error code</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on 
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetLastError(out uint error);

            /// <summary>
            /// Return extended driver error information for the last API call 
            /// that returned a value of FALSE (0).
            /// </summary>
            /// <param name="error">A PLONG pointing to a variable in which to 
            /// place the error code</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on 
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetLastDriverError(out uint error);

            /// <summary>
            /// Build a list of all QuickUSB modules connected to the host
            /// </summary>
            /// <param name="nameList">A PCHAR that points to a buffer in which to
            /// store a of QuickUSB module names found by the library. Device names
            /// are of the form 'QUSB-XXX' where XXX is the device address (0-126)
            /// in decimal. 'nameList' must be large enough to contain all the 
            /// device names + 1 character</param>
            /// <param name="length">A LONG containing the length of the nameList 
            /// buffer in CHARs</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbFindModules(byte[] nameList, uint length);

            /// <summary>
            /// Open a QuickUSB device for use by the library
            /// </summary>
            /// <param name="hDevice">A PHANDLE that points to a HANDLE in which to
            /// place the new device ID. If successful, hDevice will contain the 
            /// new HANDLE.</param>
            /// <param name="deviceName">A PCHAR that points to a null-terminated 
            /// CHAR array containing the name of the device. Device names are of 
            /// the form 'QUSB-XXX' where XXX is the device address (0-126) in 
            /// decimal. The device name should be parsed from the response from 
            /// QuickUsbFindModules. For example, if two modules are connected, 
            /// devName should contain 'QUSB-0' to select the first module, and 
            /// devName should contain 'QUSB-1' to select the second module.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbOpen(out IntPtr hDevice, String deviceName);

            /// <summary>
            /// Open a QuickUSB device for use by the library
            /// </summary>
            /// <param name="hDevice">A PHANDLE that points to a HANDLE in which to
            /// place the new device ID. If successful, hDevice will contain the 
            /// new HANDLE.</param>
            /// <param name="deviceName">A PCHAR that points to a null-terminated 
            /// CHAR array containing the name of the device. Device names are of 
            /// the form 'QUSB-XXX' where XXX is the device address (0-126) in 
            /// decimal. The device name should be parsed from the response from 
            /// QuickUsbFindModules. For example, if two modules are connected, 
            /// devName should contain 'QUSB-0' to select the first module, and 
            /// devName should contain 'QUSB-1' to select the second module.</param>
            /// <param name="flags"></param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbOpenEx(out IntPtr hDevice, String deviceName, ushort flags);

            /// <summary>
            /// Close a QuickUSB device
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbClose(IntPtr hDevice);

            /// <summary>
            /// Returns the string descriptor for the selected QuickUSB module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="index">The BYTE string descriptor index</param>
            /// <param name="buffer">A PCHAR that points to a buffer in which to 
            /// place the string descriptor. The buffer should be at least 128 
            /// bytes long.</param>
            /// <param name="length">A WORD that contains the length of the buffer 
            /// in bytes</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetStringDescriptor(IntPtr hDevice, StringDescriptor index, IntPtr buffer, ushort length);

            /// <summary>
            /// Read QuickUSB module settings
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the setting address 
            /// (number)</param>
            /// <param name="setting">A PWORD pointing to a variable in which to 
            /// place the value of the setting if successful</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadSetting(IntPtr hDevice, Setting address, out ushort setting);

            /// <summary>
            /// Write QuickUSB module settings
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the setting address 
            /// (number)</param>
            /// <param name="setting">A WORD containing the new setting 
            /// value</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteSetting(IntPtr hDevice, Setting address, ushort setting);

            /// <summary>
            /// Start the process of FPGA configuration. If the FPGA is in the 
            /// process of being configured, the process will restart. If the FPGA 
            /// is already configured, it will be reset and reconfigured.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbStartFpgaConfiguration(IntPtr hDevice);

            /// <summary>
            /// Sends FPGA configuration data to the FPGA using the QuickUSB FPGA 
            /// configuration port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A PBYTE pointing to a BYTE buffer containing the 
            /// FPGA configuration data</param>
            /// <param name="length">A ULONG containing the length of the data in
            /// bytes</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteFpgaData(IntPtr hDevice, IntPtr data, uint length);

            /// <summary>
            /// Check to see if the FPGA is configured
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="isConfigured">A PWORD pointing to a WORD in which to 
            /// write the configuration status of the FPGA connected to the QuickUSB 
            /// FPGA configuration port. 1 = the FPGA is configured 
            /// (CONF_DONE = '1'), 0 = the FPGA is not configured 
            /// (CONF_DONE = '0').</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbIsFpgaConfigured(IntPtr hDevice, out ushort isConfigured);

            /// <summary>
            /// Configure an FPGA with a binary configuration file (RBF for Altera
            /// and BIN for Xilinx).  You do not need to Call 
            /// QuickUsbStartFpgaConfiguration before calling this function.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="filePath">Path to the FPGA configuration file</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbConfigureFpga(IntPtr hDevice, [MarshalAs(UnmanagedType.LPStr)] string filePath);

            /// <summary>
            /// Read a block of command values from the high-speed parallel port 
            /// using the QuickUSB module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the address. This address 
            /// is the starting value of the HSPP address bus. If address bit 14 
            /// is set (1), then the address bus will not be driven. If address bit
            /// 15 is set (1), then the address will not be incremented after each 
            /// read.</param>
            /// <param name="data">A pointer to a buffer in which to place data read
            /// from the high-speed parallel port</param>
            /// <param name="length">A PWORD pointing to a WORD containing the number
            /// of bytes to read from the high-speed parallel port on input and the
            /// number of bytes read on return</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadCommand(IntPtr hDevice, ushort address, IntPtr data, ref ushort length);

            /// <summary>
            /// Write a block of command values to the high-speed parallel port
            /// using the QuickUSB module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the address. This address
            /// is the starting value of the HSPP address bus. If address bit 14
            /// is set (1), then the address bus will not be driven. If address bit
            /// 15 is set (1), then the address will not be incremented after each
            /// write.</param>
            /// <param name="data">A pointer to a buffer containing the data to
            /// write to the high-speed parallel port</param>
            /// <param name="length">A WORD containing the number of bytes to write
            /// to the high-speed parallel port</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteCommand(IntPtr hDevice, ushort address, IntPtr data, ushort length);

            /// <summary>
            /// Read a block of data values from the high-speed parallel port
            /// using the QuickUSB module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a buffer in which to place data
            /// values read from the HSPP</param>
            /// <param name="length">A PULONG to a ULONG containing the number of
            /// bytes to read from the HSPP. Additionally, length is overwritten
            /// with the number of bytes actually read.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadData(IntPtr hDevice, IntPtr data, ref uint length);

            /// <summary>
            /// Read a block of data values from the high-speed parallel port
            /// using the QuickUSB module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a buffer in which to place data
            /// values read from the HSPP</param>
            /// <param name="length">A PULONG to a ULONG containing the number of
            /// bytes to read from the HSPP. Additionally, length is overwritten
            /// with the number of bytes actually read.</param>
            /// <param name="flags">Extra flags</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadDataEx(IntPtr hDevice, IntPtr data, ref uint length, DataFlags flags);

            /// <summary>
            /// Write a block of data values to the high-speed parallel port
            /// using the QuickUSB module.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a block of data values to write to
            /// the HSPP</param>
            /// <param name="length">A ULONG containing the number of bytes to
            /// write to the HSPP</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteData(IntPtr hDevice, IntPtr data, uint length);

            /// <summary>
            /// Write a block of data values to the high-speed parallel port
            /// using the QuickUSB module.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a block of data values to write to
            /// the HSPP</param>
            /// <param name="length">A ULONG containing the number of bytes to
            /// write to the HSPP</param>
            /// <param name="flags">Extra flags</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteDataEx(IntPtr hDevice, IntPtr data, ref uint length, DataFlags flags);

            /// <summary>
            /// Read the data direction of each data port bit for the specified
            /// port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A BYTE containing the port address. Ports are
            /// addressed 0 to 4 corresponding to port A-E.</param>
            /// <param name="data">A PBYTE to a BYTE in which to place the data
            /// direction bit values. Each bit in data corresponds to data bits of
            /// the specified port. A data direction bit value of 0=input and
            /// 1=output (i.e. 0x03 means that bits 0 and 1 are outputs and bits
            /// 2-7 are inputs).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadPortDir(IntPtr hDevice, Port address, out byte data);

            /// <summary>
            /// Set the data direction of each data port bit for the specified port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A BYTE containing the port address. Ports are
            /// addressed 0 to 4 corresponding to port A-E.</param>
            /// <param name="data">A byte that contains the data direction bit
            /// values. Each bit in data corresponds to data bits of the specified
            /// port. A data direction bit value of 0=input and 1=output.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWritePortDir(IntPtr hDevice, Port address, byte data);

            /// <summary>
            /// Read a series of bytes from the specified port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A BYTE containing the port address. Ports are
            /// addressed 0 to 4 corresponding to port A-E.</param>
            /// <param name="data">A pointer to an array of bytes in which to place
            /// the data. This buffer must be at least „length? bytes long.</param>
            /// <param name="length">A pointer to the number of bytes to read from
            /// the port. The bytes are read sequentially. The maximum length is
            /// 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadPort(IntPtr hDevice, Port address, IntPtr data, ref ushort length);

            /// <summary>
            /// Write a series of bytes to the specified port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A BYTE containing the port address. Ports are
            /// addressed 0 to 4 corresponding to port A-E.</param>
            /// <param name="data">A pointer to an array of bytes to send out the
            /// port. This buffer must be at least „length? bytes long.</param>
            /// <param name="length">A pointer to the number of bytes to write to
            /// the port. The bytes are written sequentially. The maximum length is
            /// 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWritePort(IntPtr hDevice, Port address, IntPtr data, ushort length);

            /// <summary>
            /// Set the baud rate for both serial ports. Baud rates are
            /// programmable from 4800 to 230k baud. This function sets the baud
            /// rate of both serial ports. It is not possible to set the baud rate
            /// of each serial port independently.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="baudRate">An unsigned long integer (32-bits)
            /// containing the baud rate in bits per second</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbSetRS232BaudRate(IntPtr hDevice, uint baudRate);

            /// <summary>
            /// Read the number of characters waiting in the receive buffer
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The serial port number. Serial port 0 
            /// (P1) = 0, serial port 1 (P2) = 1.</param>
            /// <param name="length">A pointer to the number of characters to read. 
            /// Set to the number of characters actually read on return.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetNumRS232(IntPtr hDevice, byte portNum, out ushort length);

            /// <summary>
            /// Flush the RS232 port transmit and receive buffers
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The serial port number. Serial port 0
            /// (P1) = 0, serial port 1 (P2) = 1.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbFlushRS232(IntPtr hDevice, byte portNum);

            /// <summary>
            /// Read a block of characters from the interrupt receive buffer of the
            /// specified QuickUSB serial port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The serial port number. Serial port 0
            /// (P1) = 0, serial port 1 (P2) = 1.</param>
            /// <param name="data">A pointer to a buffer in which to place the data.
            /// The buffer must be at least 128 bytes long.</param>
            /// <param name="length">A pointer to the number of characters to read.
            /// Set to the number of characters actually read on return.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadRS232(IntPtr hDevice, byte portNum, IntPtr data, ref ushort length);

            /// <summary>
            /// Write a block of characters to the specified QuickUSB serial port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The serial port number. Serial port 0
            /// (P1) = 0, serial port 1 (P2) = 1.</param>
            /// <param name="data">A pointer to a buffer containing the data</param>
            /// <param name="length">The number of characters to write</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteRS232(IntPtr hDevice, byte portNum, IntPtr data, ushort length);

            /// <summary>
            /// Read a block of bytes from the specified SPI slave port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The SPI device address (nSS line) to read
            /// from</param>
            /// <param name="data">A pointer to a buffer in which to place the
            /// received data</param>
            /// <param name="length">A pointer to the number of bytes to read. The
            /// maximum length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadSpi(IntPtr hDevice, byte portNum, IntPtr data, ref ushort length);

            /// <summary>
            /// Write a block of bytes to the specified SPI slave port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The SPI device address (nSS line) to write
            /// to</param>
            /// <param name="data">A pointer to the data to send</param>
            /// <param name="length">The number of bytes to send. The maximum
            /// length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteSpi(IntPtr hDevice, byte portNum, IntPtr data, ushort length);

            /// <summary>
            /// Simultaneously write and read a block of data to and from the
            /// specified SPI slave port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="portNum">The SPI device address (nSS line) to write to
            /// and read from</param>
            /// <param name="data">A pointer to the buffer that contains the data
            /// to send and in which to place the received data</param>
            /// <param name="length">The number of bytes to send and receive. The
            /// maximum length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteReadSpi(IntPtr hDevice, byte portNum, IntPtr data, ushort length);

            /// <summary>
            /// Read for the I2C port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">The device address</param>
            /// <param name="data">A pointer to a buffer in which to place the
            /// data</param>
            /// <param name="length">The length of the data buffer in bytes. The
            /// maximum length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadI2C(IntPtr hDevice, ushort address, IntPtr data, ref ushort length);

            /// <summary>
            /// Write to the I2C port
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">The device address</param>
            /// <param name="data">A pointer to the data to send</param>
            /// <param name="length">The length of the data buffer in bytes. The
            /// maximum length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteI2C(IntPtr hDevice, ushort address, IntPtr data, ushort length);

            /// <summary>
            /// Perform a cached write to the I2C port.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">The device address</param>
            /// <param name="data">A pointer to the data to send</param>
            /// <param name="length">The length of the data buffer in bytes. The
            /// maximum length is 64 bytes (Full/High Speed) or 1024 bytes (Super Speed).</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbCachedWriteI2C(IntPtr hDevice, ushort address, IntPtr data, ushort length);

            /// <summary>
            /// Set the timeout for QuickUSB read data transfers
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="timeOut">A LONG that specifies the new timeout value
            /// in milliseconds</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbSetTimeout(IntPtr hDevice, uint timeOut);

            /// <summary>
            /// Determine the version of the QuickUSB driver
            /// </summary>
            /// <param name="major">A PWORD that points to a variable in which to
            /// place the major version number</param>
            /// <param name="minor">A PWORD that points to a variable in which to
            /// place the minor version number</param>
            /// <param name="build">A PWORD that points to a variable in which to
            /// place the build number</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetDriverVersion(out ushort major, out ushort minor, out ushort build);

            /// <summary>
            /// Determine the version of the QuickUSB DLL (QuickUSB.dll)
            /// </summary>
            /// <param name="major">A PWORD that points to a variable in which to
            /// place the major version number</param>
            /// <param name="minor">A PWORD that points to a variable in which to
            /// place the minor version number</param>
            /// <param name="build">A PWORD that points to a variable in which to
            /// place the build number</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetDllVersion(out ushort major, out ushort minor, out ushort build);

            /// <summary>
            /// Determine the version of the QuickUSB Firmware is currently in the QuickUSB Module
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="major">A PWORD that points to a variable in which to
            /// place the major version number</param>
            /// <param name="minor">A PWORD that points to a variable in which to
            /// place the minor version number</param>
            /// <param name="build">A PWORD that points to a variable in which to
            /// place the build number</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetFirmwareVersion(IntPtr hDevice, out ushort major, out ushort minor, out ushort build);

            /// <summary>
            /// Write a block of data values to the high-speed parallel port using
            /// an asynchronous function call
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a block of data values to write to
            /// the HSPP</param>
            /// <param name="length">A ULONG containing the number of bytes to
            /// write to the HSPP</param>
            /// <param name="transaction">A PBYTE to a BYTE in which to place the
            /// transaction identifier required by QuickUsb.AsyncWait</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteDataAsync(IntPtr hDevice, IntPtr data, uint length, out byte transaction);

            /// <summary>
            /// Read a block of data values from the high-speed parallel port
            /// using an asynchronous function call
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="data">A pointer to a buffer in which to place data
            /// read from the HSPP</param>
            /// <param name="length">A PULONG containing the number of bytes to
            /// read from the HSPP</param>
            /// <param name="transaction">A PBYTE to a BYTE in which to place the
            /// transaction identifier required by QuickUsbAsyncWait</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadDataAsync(IntPtr hDevice, IntPtr data, ref uint length, out byte transaction);

            /// <summary>
            /// Wait for an asynchronous transfer to complete
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="length">An PULONG that returns the number of bytes
            /// that were transferred as a result of the asynchronous function
            /// call. If the asynchronous function call is still pending, "length"
            /// will be set to 0. This function must be called until „length? is
            /// non-zero otherwise, the driver will not release its internal
            /// buffers, thus causing a memory leak and an eventual system crash
            /// (Blue Screen of Death). If the asynchronous function call has
            /// completed, the number of bytes requested will be stored in 'length'
            /// and all internal buffers will be released.</param>
            /// <param name="transaction">A BYTE transaction identifier returned by
            /// QuickUsbReadDataAsync or QuickUsbWriteDataAsync</param>
            /// <param name="immediate">A BYTE value. If non-zero, the driver will
            /// not wait the default timeout value for the transaction to complete.
            /// If zero, the driver will wait the default timeout period for the
            /// transaction to complete.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbAsyncWait(IntPtr hDevice, out uint length, byte transaction, byte immediate);

            /// <summary>
            /// Read QuickUSB module defaults. The defaults are non-volatile and
            /// are read into the settings table on power up.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the default address
            /// (number)</param>
            /// <param name="setting">A PWORD pointing to a variable in which to
            /// place the value of the default if successful</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadDefault(IntPtr hDevice, Setting address, out ushort setting);

            /// <summary>
            /// Write QuickUSB module defaults. The defaults are non-volatile and
            /// are read into the settings table on power up.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD containing the default address 
            /// (number)</param>
            /// <param name="setting">A WORD containing the new default
            /// value</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteDefault(IntPtr hDevice, Setting address, ushort setting);

            /// <summary>
            /// Read a block of bytes from memory
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD indicating the byte offset into memory
            /// where the read should begin</param>
            /// <param name="data">A pointer to a buffer in which to place the
            /// received data</param>
            /// <param name="bytes">A WORD indicating the number of bytes to
            /// read</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadStorage(IntPtr hDevice, ushort address, IntPtr data, ushort bytes);

            /// <summary>
            /// Write a block of bytes from memory
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="address">A WORD indicating the byte offset into
            /// memory where the write should begin</param>
            /// <param name="data">A pointer to the data buffer to write to
            /// memory</param>
            /// <param name="bytes">A WORD indicating the number of bytes to
            /// write</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteStorage(IntPtr hDevice, ushort address, IntPtr data, ushort bytes);

            /// <summary>
            /// Allocate a data buffer 'bytes' long that will work on for 
            /// asynchronous and streaming data requests
            /// </summary>
            /// <param name="bytes">The number of bytes to allocate</param>
            /// <param name="buffer">The data buffer for use with the 
            /// asynchronous and streaming data APIs</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbAllocateDataBuffer(uint bytes, IntPtr buffer);

            /// <summary>
            /// Free a data buffer previously allocated with QuickUsbAllocateDataBuffer()
            /// </summary>
            /// <param name="buffer">The buffer to free</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbFreeDataBuffer(IntPtr buffer);

            /// <summary>
            /// Issue an asynchronous bulk data read request.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffer">The data buffer.  This buffer must remain 
            /// valid until the request completes.</param>
            /// <param name="bytes">The number of bytes to read.</param>
            /// <param name="bulkStream">A pointer to a user-allocated QBULKSTREAM 
            /// variable used to store all the information about the request.  
            /// This data must remain valid until the request completes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// to call when the request completes.  If zero (0) or NULL is 
            /// specified, then no completion routine is called at the completion 
            /// of the transaction and QuickUsbBulkWait must be called to determine
            /// when the transaction has completed.  When multithreading, this 
            /// routine executes from another thread and multiple threads may 
            /// execute this routine at the same time.  When single-threaded (not 
            /// multithreading), the QuickUsbBulkWait must be called to determine 
            /// when the transaction has completed, which will then execute the 
            /// completion routine.</param>
            /// <param name="tag">A user pointer passed on to the completion 
            /// routine used to associate user information with the request.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadBulkDataAsync(IntPtr hDevice, IntPtr buffer, uint bytes, IntPtr bulkStream, BulkStreamCompletionRoutine cRoutine, IntPtr tag);

            /// <summary>
            /// Issue an asynchronous bulk data write request.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffer">The data buffer.  This buffer must remain 
            /// valid until the request completes.</param>
            /// <param name="bytes">The number of bytes to write.</param>
            /// <param name="bulkStream">A pointer to a user-allocated QBULKSTREAM 
            /// variable used to store all the information about the request.  
            /// This data must remain valid until the request completes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// to call when the request completes.  If zero (0) or NULL is 
            /// specified, then no completion routine is called at the completion 
            /// of the transaction and QuickUsbBulkWait must be called to determine
            /// when the transaction has completed.  When multithreading, this 
            /// routine executes from another thread and multiple threads may 
            /// execute this routine at the same time.  When single-threaded (not 
            /// multithreading), the QuickUsbBulkWait must be called to determine 
            /// when the transaction has completed, which will then execute the 
            /// completion routine.</param>
            /// <param name="tag">A user pointer passed on to the completion 
            /// routine used to associate user information with the request.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteBulkDataAsync(IntPtr hDevice, IntPtr buffer, uint bytes, IntPtr bulkStream, BulkStreamCompletionRoutine cRoutine, IntPtr tag);

            /// <summary>
            /// Determine if an asynchronous transaction has completed, wait for an asynchronous transaction to complete, or wait for all pending asynchronous transactions to complete.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="bulkStream">A pointer to a user-allocated QBULKSTREAM 
            /// variable that has been associated with a data transaction from a 
            /// call to QuickUsbReadBulkDataAsync or QuickUsbWriteBulkDataAsync.  
            /// If zero (0) or NULL is specified, then this function will wait 
            /// until ALL pending requests complete and execute their completion 
            /// routines.</param>
            /// <param name="immediate">If false or zero, this function will wait 
            /// for the specified request to complete and execute its completion 
            /// routine.  If true or non-zero, this function will return 
            /// immediately and indicate whether the specified transaction has 
            /// completed (or all transactions if bulkStream is zero or NULL) by 
            /// either returning true (1) or by returning zero (0) with a call to 
            /// QuickUsbGetLastError indicating QUICKUSB_ERROR_NOT_COMPLETED.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbBulkWait(IntPtr hDevice, IntPtr bulkStream, byte immediate);

            /// <summary>
            /// Abort in-flight asynchronous requests without having to wait for them 
            /// to timeout.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="bulkStream">A pointer to a user-allocated QBULKSTREAM 
            /// variable that has been associated with a data transaction from a 
            /// call to QuickUsbReadBulkDataAsync or QuickUsbWriteBulkDataAsync.  
            /// Pass zero (0) to abort all pending asynchronous IO requests.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbBulkAbort(IntPtr hDevice, IntPtr bulkStream);

            /// <summary>
            /// Set the number of threads and the thread concurrency for the 
            /// Asynchronous Data API.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="numThreads">A word indicating the number of threads 
            /// to allocate for the Asynchronous API.  If numThreads is zero, no 
            /// threads are created and the Asynchronous API will be single-
            /// threaded.  If numThreads is greater than zero, the Asynchronous API
            /// is multithreaded—the indicated number of threads will be created 
            /// and dedicated to asynchronously processing completion routines.  
            /// When multithreading, the recommended number of threads to maximize 
            /// multithreading performance is equal to the number of 
            /// processors/cores in the system times two.</param>
            /// <param name="concurrency">A word indicating the number of threads 
            /// that are allowed to execute completion routines simultaneously.  
            /// Specify zero if numThreads is zero.  When multithreading, the 
            /// number of threads allowed to execute completion routines 
            /// concurrently will be limited by the value of ‘concurrency’, which 
            /// must be at least one.  The recommended concurrency to maximize 
            /// multithreading performance is equal to the total number of 
            /// processors/cores in the system.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbSetNumAsyncThreads(IntPtr hDevice, ushort numThreads, ushort concurrency);

            /// <summary>
            /// Get the number of threads and the thread concurrency for the 
            /// Asynchronous Data API.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="numThreads">A pointed to a word indicating the number 
            /// of threads allocated for the Asynchronous API.</param>
            /// <param name="concurrency">A pointer to a word indicating the number
            /// of threads that are allowed to execute completion routines 
            /// simultaneously.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetNumAsyncThreads(IntPtr hDevice, out ushort numThreads, out ushort concurrency);

            /// <summary>
            /// Start streaming data into the computer from the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffers">An array of pointers to the user allocated 
            /// buffers used to store the read data.  These buffers must remain 
            /// valid until the read stream has been stopped.  If this parameter is
            /// zero (0) or NULL then the API will automatically allocate and 
            /// manage the memory for the data buffers.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// that is called each time a buffer is filled with data.  This 
            /// routine executes from another thread and multiple threads may 
            /// execute this routine at the same time.</param>
            /// <param name="tag">A pointer containing user-specific data that is 
            /// passed into the completion routine.  This parameter may optionally 
            /// be zero (0) or NULL.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="numThreads">The number of threads to allocate for this 
            /// stream.  If numThreads is zero, no threads are created and the 
            /// stream will be single-threaded.  If numThreads is greater than zero, 
            /// the stream is multithreaded—the indicated number of threads will be 
            /// created and dedicated to asynchronously processing completion 
            /// routines.  When multithreading, the recommended number of threads to 
            /// maximize multithreading performance is equal to the number of 
            /// processors/cores in the system times two.</param>
            /// <param name="concurrency">The number of threads that are 
            /// allowed to execute completion routines simultaneously.  Specify zero 
            /// if numThreads is zero.  When multithreading, the number of threads 
            /// allowed to execute completion routines concurrently will be limited 
            /// by the value of ‘concurrency’, which must be at least one.  The 
            /// recommended concurrency to maximize multithreading performance is 
            /// equal to the total number of processors/cores in the system.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadBulkDataStartStream(IntPtr hDevice, IntPtr buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency);

            /// <summary>
            /// Start streaming data into the computer from the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffers">An array of pointers to the user allocated 
            /// buffers used to store the read data.  These buffers must remain 
            /// valid until the read stream has been stopped.  If this parameter is
            /// zero (0) or NULL then the API will automatically allocate and 
            /// manage the memory for the data buffers.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// that is called each time a buffer is filled with data.  This 
            /// routine executes from another thread and multiple threads may 
            /// execute this routine at the same time.</param>
            /// <param name="tag">A pointer containing user-specific data that is 
            /// passed into the completion routine.  This parameter may optionally 
            /// be zero (0) or NULL.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="numThreads">The number of threads to allocate for this 
            /// stream.  If numThreads is zero, no threads are created and the 
            /// stream will be single-threaded.  If numThreads is greater than zero, 
            /// the stream is multithreaded—the indicated number of threads will be 
            /// created and dedicated to asynchronously processing completion 
            /// routines.  When multithreading, the recommended number of threads to 
            /// maximize multithreading performance is equal to the number of 
            /// processors/cores in the system times two.</param>
            /// <param name="concurrency">The number of threads that are 
            /// allowed to execute completion routines simultaneously.  Specify zero 
            /// if numThreads is zero.  When multithreading, the number of threads 
            /// allowed to execute completion routines concurrently will be limited 
            /// by the value of ‘concurrency’, which must be at least one.  The 
            /// recommended concurrency to maximize multithreading performance is 
            /// equal to the total number of processors/cores in the system.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadBulkDataStartStream(IntPtr hDevice, IntPtr[] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency);

            /// <summary>
            /// Start streaming data to a disk file from the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="filePath">Path to the disk file to create.  If the
            /// file already exists, it will be overwritten.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="maxTransfers">The maximum number of data transfers to
            /// successfully complete.  Once the maximum is reached, the stream is
            /// automatically paused.  Each successful transfer equates to a single
            /// data buffer filling with 'bufferSize' bytes of data being written to
            /// disk.  Specifying a value of zero indicates the stream should not 
            /// automatically pause and instead continuously run until explicitly
            /// paused or stopped.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="flags">Additional flags controlling the operation of
            /// this function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbReadBulkDataStartStreamToFile(IntPtr hDevice, [MarshalAs(UnmanagedType.LPStr)] string filePath, uint numBuffers, uint bufferSize, uint maxTransfers, out byte streamID, StreamFlag flags);

            /// <summary>
            /// Start streaming data from the computer to the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffers">An array of pointers to the user allocated 
            /// buffers used to store the read data.  These buffers must remain 
            /// valid until the read stream has been stopped.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// that is called each time a buffer has been written to the device.  
            /// The buffer must be refilled with new data to write from within the 
            /// completion routine.  This routine executes from another thread and 
            /// multiple threads may execute this routine at the same time.</param>
            /// <param name="tag">A pointer containing user-specific data that is 
            /// passed into the completion routine.  tag - A pointer containing 
            /// user-specific data that is passed into the completion routine.  
            /// This parameter may optionally be zero (0) or NULL.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="numThreads">The number of threads to allocate for this 
            /// stream.  If numThreads is zero, no threads are created and the 
            /// stream will be single-threaded.  If numThreads is greater than zero, 
            /// the stream is multithreaded—the indicated number of threads will be 
            /// created and dedicated to asynchronously processing completion 
            /// routines.  When multithreading, the recommended number of threads to 
            /// maximize multithreading performance is equal to the number of 
            /// processors/cores in the system times two.</param>
            /// <param name="concurrency">The number of threads that are 
            /// allowed to execute completion routines simultaneously.  Specify zero 
            /// if numThreads is zero.  When multithreading, the number of threads 
            /// allowed to execute completion routines concurrently will be limited 
            /// by the value of ‘concurrency’, which must be at least one.  The 
            /// recommended concurrency to maximize multithreading performance is 
            /// equal to the total number of processors/cores in the system.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteBulkDataStartStream(IntPtr hDevice, IntPtr buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency);

            /// <summary>
            /// Start streaming data from the computer to the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="filePath">Path to the disk file to read.  The file 
            /// must exist or the function call will fail.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="maxTransfers">The maximum number of data transfers to
            /// successfully complete.  Once the maximum is reached, the stream is
            /// automatically paused.  Each successful transfer equates to a single
            /// data buffer filling with 'bufferSize' bytes of data being read from
            /// disk and written out QuickUSB.  Specifying a value of zero indicates 
            /// the stream should not automatically pause and instead continuously 
            /// run until the entire contents of the file have been written out, or
            /// until the stream has been explicitly paused or stopped.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="flags">Additional flags controlling the operation of
            /// this function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteBulkDataStartStreamFromFile(IntPtr hDevice, [MarshalAs(UnmanagedType.LPStr)] string filePath, uint numBuffers, uint bufferSize, uint maxTransfers, out byte streamID, StreamFlag flags);

            /// <summary>
            /// Start streaming data from the computer to the QuickUSB device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="buffers">An array of pointers to the user allocated 
            /// buffers used to store the read data.  These buffers must remain 
            /// valid until the read stream has been stopped.</param>
            /// <param name="numBuffers">The number of data buffers to use.</param>
            /// <param name="bufferSize">The size of each buffer, in bytes.</param>
            /// <param name="cRoutine">The completion routine (callback function) 
            /// that is called each time a buffer has been written to the device.  
            /// The buffer must be refilled with new data to write from within the 
            /// completion routine.  This routine executes from another thread and 
            /// multiple threads may execute this routine at the same time.</param>
            /// <param name="tag">A pointer containing user-specific data that is 
            /// passed into the completion routine.  tag - A pointer containing 
            /// user-specific data that is passed into the completion routine.  
            /// This parameter may optionally be zero (0) or NULL.</param>
            /// <param name="streamID">The ID of the stream.</param>
            /// <param name="numThreads">The number of threads to allocate for this 
            /// stream.  If numThreads is zero, no threads are created and the 
            /// stream will be single-threaded.  If numThreads is greater than zero, 
            /// the stream is multithreaded—the indicated number of threads will be 
            /// created and dedicated to asynchronously processing completion 
            /// routines.  When multithreading, the recommended number of threads to 
            /// maximize multithreading performance is equal to the number of 
            /// processors/cores in the system times two.</param>
            /// <param name="concurrency">The number of threads that are 
            /// allowed to execute completion routines simultaneously.  Specify zero 
            /// if numThreads is zero.  When multithreading, the number of threads 
            /// allowed to execute completion routines concurrently will be limited 
            /// by the value of ‘concurrency’, which must be at least one.  The 
            /// recommended concurrency to maximize multithreading performance is 
            /// equal to the total number of processors/cores in the system.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteBulkDataStartStream(IntPtr hDevice, IntPtr[] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency);

            /// <summary>
            /// Stop a previously started read or write bulk data stream.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="streamID">The stream ID, as returned from a call to
            /// QuickUsbReadBulkDataStartStream or 
            /// QuickUsbWriteBulkDataStartStream.</param>
            /// <param name="immediate">If zero (or false), this function will 
            /// block until the stream has completely stopped and any remaining 
            /// completion routines have executed.  If non-zero (or true), the 
            /// stream will be marked to shutdown and will return immediately 
            /// without waiting for any remaining completion routines to execute.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbStopStream(IntPtr hDevice, byte streamID, bool immediate);

            /// <summary>
            /// For single-threaded streams, this function is called to process and
            /// re-issue completed requests.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="streamID">The ID of the stream to process, as returned 
            /// from a call to QuickUsbReadBulkDataStartStream or 
            /// QuickUsbWriteBulkDataStartStream.</param>
            /// <param name="milliseconds">The number of milliseconds the caller is 
            /// willing to wait until a data request on the indicated stream has 
            /// completed and is processed.  If no data request on the indicated 
            /// stream has completed within the specified amount of time, the 
            /// function will return unsuccessfully and a call to 
            /// QuickUsbGetLastError will indicate the error 
            /// QUICKUSB_ERROR_TIMEOUT.  If milliseconds is zero, the function will
            /// process any requests that are completed or timeout immediately.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbProcessStream(IntPtr hDevice, byte streamID, uint milliseconds);

            /// <summary>
            /// Pause the automatic re-issuing of data requests on a stream.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="streamID">streamID: 	The ID of the stream to 
            /// process, as returned from a call to 
            /// QuickUsbReadBulkDataStartStream or 
            /// QuickUsbWriteBulkDataStartStream.</param>
            /// <param name="immediate">If zero (or false), this function will 
            /// block until the stream has paused and all in-process requests have 
            /// completed.  If non-zero (or true), the stream will be marked to 
            /// enter the pause state and will return immediately without waiting 
            /// for any in-process requests to complete.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbPauseStream(IntPtr hDevice, byte streamID, bool immediate);

            /// <summary>
            /// Resume the automatic re-issuing of data requests on a stream.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="streamID">streamID: 	The ID of the stream to 
            /// process, as returned from a call to 
            /// QuickUsbReadBulkDataStartStream or 
            /// QuickUsbWriteBulkDataStartStream.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbResumeStream(IntPtr hDevice, byte streamID);

            /// <summary>
            /// Get the current status of the indicated stream
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="streamID">streamID: 	The ID of the stream to 
            /// process, as returned from a call to 
            /// QuickUsbReadBulkDataStartStream or 
            /// QuickUsbWriteBulkDataStartStream.</param>
            /// <param name="status">The status of the stream</param>
            /// <param name="error">The last error reported by the stream</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            /// <returns></returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetStreamStatus(IntPtr hDevice, byte streamID, out StreamStatus status, out Error error);

            /// <summary>
            /// Write QuickUSB firmware to a device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="fileName">The path and file name of the QuickUSB 
            /// firmware (QUSB) file.</param>
            /// <param name="options">Specify QUICKUSB_PRESERVE_CUSTOMIZATION to 
            /// preserve customizations present in the current firmware, 
            /// QUICKUSB_PRESERVE_DEFAULTS to preserve default settings present in
            /// the current firmware, QUICKUSB_PRESERVE_GPIF to preserve GPIF 
            /// customizations, and/or QUICKUSB_PRESERVE_SERIAL to preserve the 
            /// serial number.  To specify a new serial number, pass 
            /// QUICKUSB_PROGRAM_SERIAL bitwise or’ed (|) together with the new 
            /// serial number (must be between 1 and 65535).</param>
            /// <param name="callback">A pointer to a QPROGRESS_CALLBACK function 
            /// which is called during the programming process to report the 
            /// percentage completed.  This may be used to perform background 
            /// processes and GUI updates while the programming process takes 
            /// place.</param>
            /// <param name="tag">A PQVOID variable to store user data that is 
            /// passed onto the callback function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbWriteFirmware(IntPtr hDevice, [MarshalAs(UnmanagedType.LPStr)] string fileName, uint options, ProgressDelegate callback, IntPtr tag);

            /// <summary>
            /// Verify that the firmware contained in the specified device matches 
            /// that of the supplied firmware file.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="fileName">The path and file name of the QuickUSB 
            /// firmware (QUSB) file.</param>
            /// <param name="callback">A pointer to a QPROGRESS_CALLBACK function 
            /// which is called during the verify process to report the percentage 
            /// completed.  This may be used to perform background processes and 
            /// GUI updates while the verify process takes place.</param>
            /// <param name="tag">A PQVOID variable to store user data that is 
            /// passed onto the callback function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbVerifyFirmware(IntPtr hDevice, [MarshalAs(UnmanagedType.LPStr)] string fileName, ProgressDelegate callback, IntPtr tag);

            /// <summary>
            /// Identify the EPCS device connected to the SPI bus on the specified 
            /// nSS line.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="nSS">The nSS SPI line that the EPCS device is
            /// connected to (0-9). </param>
            /// <param name="epcsId">A pointer to QWORD used to store the ID of the
            /// EPCS found.  This will be one of QUICKUSB_EPCS1_ID (0x10), 
            /// QUICKUSB_EPCS4_ID (0x12), QUICKUSB_EPCS16_ID (0x14), 
            /// QUICKUSB_EPCS64_ID (0x16), or QUICKUSB_EPCS_ID_UNKNOWN (0x00).</param>
            /// <param name="epcsByteSize">A pointer to a QULONG used to store the
            /// size of the EPCS device, in bytes.</param>
            /// <param name="flags">Additional flags controlling the operation of 
            /// this function.  Specify EpcsFlags.IgnoreNCE to prevent the 
            /// function from automatically asserting the nCE line of the FPGA at 
            /// the beginning of the function are de-asserting it at the end of the 
            /// function.</param>
            /// <returns></returns>
            [DllImport(Path)]
            public static extern int QuickUsbIdentifyEpcs(IntPtr hDevice, byte nSS, out EpcsId epcsId, out uint epcsByteSize, uint flags);

            /// <summary>
            /// Write the specified FPGA data file to the EPCS device.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="nSS">The nSS SPI line that the EPCS device is
            /// connected to (0-9). </param>
            /// <param name="filePath">The path and file name of the FPGA data 
            /// file.</param>
            /// <param name="flags">Additional flags controlling the operation of 
            /// this function.  Specify EpcsFlags.IgnoreNCE to prevent the 
            /// function from automatically asserting the nCE line of the FPGA at 
            /// the beginning of the function are de-asserting it at the end of the
            /// function.</param>
            /// <param name="callback">A pointer to a QPROGRESS_CALLBACK function 
            /// which is called during the configuring process to report the 
            /// percentage completed.  This may be used to perform background 
            /// processes and GUI updates while the configure process takes place.</param>
            /// <param name="tag">A PQVOID variable to store user data that is 
            /// passed onto the callback function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbConfigureEpcs(IntPtr hDevice, byte nSS, [MarshalAs(UnmanagedType.LPStr)] string filePath, uint flags, ProgressDelegate callback, IntPtr tag);

            /// <summary>
            /// Verify that the data contained in the EPCS device matches that of 
            /// the specified FPGA data file.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="nSS">The nSS SPI line that the EPCS device is
            /// connected to (0-9). </param>
            /// <param name="filePath">The path and file name of the FPGA data 
            /// file.</param>
            /// <param name="flags">Additional flags controlling the operation of 
            /// this function.  Specify EpcsFlags.IgnoreNCE to prevent the 
            /// function from automatically asserting the nCE line of the FPGA at 
            /// the beginning of the function are de-asserting it at the end of the
            /// function.</param>
            /// <param name="callback">A pointer to a QPROGRESS_CALLBACK function 
            /// which is called during the verify process to report the percentage 
            /// completed.  This may be used to perform background processes and 
            /// GUI updates while the verify process takes place.</param>
            /// <param name="tag">A PQVOID variable to store user data that is 
            /// passed onto the callback function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbVerifyEpcs(IntPtr hDevice, byte nSS, [MarshalAs(UnmanagedType.LPStr)] string filePath, uint flags, ProgressDelegate callback, IntPtr tag);

            /// <summary>
            /// Perform a bulk erase action of the EPCS device to erase the entire 
            /// device to 0xFF.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="nSS">The nSS SPI line that the EPCS device is
            /// connected to (0-9). </param>
            /// <param name="flags">Additional flags controlling the operation of 
            /// this function.  Specify EpcsFlags.IgnoreNCE to prevent the 
            /// function from automatically asserting the nCE line of the FPGA at 
            /// the beginning of the function are de-asserting it at the end of the
            /// function.</param>
            /// <param name="callback">A pointer to a QPROGRESS_CALLBACK function 
            /// which is called during the erase process to report the percentage 
            /// completed.  This may be used to perform background processes and 
            /// GUI updates while the erase process takes place.</param>
            /// <param name="tag">A PQVOID variable to store user data that is 
            /// passed onto the callback function.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbEraseEpcs(IntPtr hDevice, byte nSS, uint flags, ProgressDelegate callback, IntPtr tag);

            /// <summary>
            /// Reset a given statistic.  Reseting a statistic resets any associate
            /// byte counts and time indicaters used by the statistic.
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="statistic">The statistic to reset.  Specify Stat.All 
            /// to reset all statistics.  You should reset all statistics before 
            /// first attempting to read and statistics from QuickUSB.</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbResetStatistic(IntPtr hDevice, Stat statistic);

            /// <summary>
            /// Retreive a statistic in the specified unit
            /// </summary>
            /// <param name="hDevice">A HANDLE that was returned from a call to 
            /// QuickUsbOpen or QuickUsbOpenEx</param>
            /// <param name="statistic">The statistic retrieve</param>
            /// <param name="unit">The unit to report the statistic in</param>
            /// <param name="value">The value of the reported statistic</param>
            /// <param name="flags">Additional flags controlling the operation of 
            /// this function</param>
            /// <returns>A LONG that is either non-zero on success or zero (0) on
            /// failure</returns>
            [DllImport(Path)]
            public static extern int QuickUsbGetStatistic(IntPtr hDevice, Stat statistic, StatUnit unit, out float value, uint flags);
        }

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Constructor
        //---------------------------------------------------------------------

        /// <summary>
        /// Default constructor
        /// </summary>
        public QuickUsb()
        {
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="devname">The name of the QuickUSB device as returned
        /// by a call to QuickUsbFindModules</param>
        public QuickUsb(String devname)
		{
			m_devname = devname;
		}

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Static Methods
        //---------------------------------------------------------------------

        /// <summary>
        /// Get the last API error
        /// </summary>
        /// <returns>The last error reported by the QuickUSB API</returns>
		public static Error GetLastError() 
		{
			uint errcode = (uint) QuickUsb.Error.ERROR_NO_ERROR;
			
			NativeMethods.QuickUsbGetLastError(out errcode);

			return (Error)errcode;
		}

        /// <summary>
        /// Get the last driver error
        /// </summary>
        /// <returns>The last driver error reported by the QuickUSB API</returns>
        public static uint GetLastDriverError()
        {
            uint errcode;

            NativeMethods.QuickUsbGetLastDriverError(out errcode);

            return errcode;
        }

        /// <summary>
        /// Build a list of all connected QuickUSB devices
        /// </summary>
        /// <returns>A string containing a NULL ('\0' or CHR(0)) delimited list
        /// of QuickUSB module names found by the library. The final entry is 
        /// designated by two consecutive NULL characters. For example, after 
        /// executing this function with one module connected, nameList will 
        /// contain "QUSB-0\0\0". If there are two devices plugged in, 
        /// nameList will contain "QUSB-0\0QUSB-1\0\0".</returns>
		public static string[] FindModules()
		{
			int result;
			int start = 0;
			byte[] devlist = new byte[4096];
			String devname;
			List<string> moduleList = new List<string>();
			ASCIIEncoding convert = new ASCIIEncoding();

            // Go find all the modules in the system
            result = QuickUsb.NativeMethods.QuickUsbFindModules(devlist, (uint)devlist.Length);

			// Parse the module name out of the device list
			while (devlist[start] != '\0')
			{
				devname = convert.GetString(devlist, start, 7);
				moduleList.Add(devname.TrimEnd('\0'));
				start += devname.Length;
			}
			return moduleList.ToArray();
		}

        /// <summary>
        /// Build a list of all connected QuickUSB devices
        /// </summary>
        /// <returns>A string containing a NULL ('\0' or CHR(0)) delimited list
        /// of QuickUSB module names found by the library. The final entry is 
        /// designated by two consecutive NULL characters. For example, after 
        /// executing this function with one module connected, nameList will 
        /// contain "QUSB-0\0\0". If there are two devices plugged in, 
        /// nameList will contain "QUSB-0\0QUSB-1\0\0".</returns>
        internal static string[] FindModules(ref byte[] devlist)
        {
            int result;
            int start = 0;
            String devname;
            List<string> moduleList = new List<string>();
            ASCIIEncoding convert = new ASCIIEncoding();

            // Go find all the modules in the system
            result = QuickUsb.NativeMethods.QuickUsbFindModules(devlist, (uint)devlist.Length);

            // Parse the module name out of the device list
            while (devlist[start] != '\0')
            {
                devname = convert.GetString(devlist, start, 7);
                moduleList.Add(devname.TrimEnd('\0'));
                start += devname.Length;
            }
            return moduleList.ToArray();
        }

        /// <summary>
        /// Determine the version of the QuickUSB Driver
        /// </summary>
        /// <param name="majorVersion">The major version number</param>
        /// <param name="minorVersion">The minor version number</param>
        /// <param name="buildVersion">The build version number</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public static bool GetDriverVersion(out ushort majorVersion, out ushort minorVersion, out ushort buildVersion)
		{
            if (NativeMethods.QuickUsbGetDriverVersion(out majorVersion, out minorVersion, out buildVersion) == 0)
			{
				return false;
			}
			return true;
		}
        
        /// <summary>
        /// Determine the version of the QuickUSB Driver
        /// </summary>
        /// <param name="version">Variable to hold the version</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public static bool GetDriverVersion(out Version version)
		{
			version = null;

			ushort majorVersion, minorVersion, buildVersion;
            if (NativeMethods.QuickUsbGetDriverVersion(out majorVersion, out minorVersion, out buildVersion) == 0)
            {
				return false;
			}

			// Use the native version object
			version = new Version(majorVersion, minorVersion, buildVersion);

			return true;
		}

        /// <summary>
        /// Determine the version of the QuickUSB DLL
        /// </summary>
        /// <param name="majorVersion">The major version number</param>
        /// <param name="minorVersion">The minor version number</param>
        /// <param name="buildVersion">The build version number</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public static bool GetDllVersion(out ushort majorVersion, out ushort minorVersion, out ushort buildVersion)
		{
            if (NativeMethods.QuickUsbGetDllVersion(out majorVersion, out minorVersion, out buildVersion) == 0)
            {
				return false;
			}
			return true;
        }
        
        /// <summary>
        /// Determine the version of the QuickUSB DLL
        /// </summary>
        /// <param name="version">Variable to hold the version</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public static bool GetDllVersion(out Version version)
		{
			version = null;

			ushort majorVersion, minorVersion, buildVersion;
            if (NativeMethods.QuickUsbGetDllVersion(out majorVersion, out minorVersion, out buildVersion) == 0)
			{
				return false;
			}

			// Use the native version object
			version = new Version(majorVersion, minorVersion, buildVersion);

			return true;
		}

        /// <summary>
        /// Helper function to retrieve a reference to the managed data buffer associated
        /// with the specified IntPtr.  This is typically used from within a completion
        /// routine to convert a BulkStream.Buffer IntPtr object back to a managed object
        /// when data buffers are automatically being pinned.
        /// </summary>
        /// <param name="buffer">The IntPtr data buffer (typically the BulkStream.Buffer
        /// member)</param>
        /// <returns>A reference to the managed byte[] data buffer or null if the IntPtr
        /// is not associated with any automatically pinned data buffers.</returns>
        public static byte[] RetrieveManagedByteDataBuffer(IntPtr buffer)
        {
            // Retreive the managed data pointer
            if (PinnedAsyncBulkDataBytePtrs.ContainsKey(buffer))
            {
                return PinnedAsyncBulkDataBytePtrs[buffer];
            }
            if (PinnedStreamingBulkDataBytePtrs.ContainsKey(buffer))
            {
                return PinnedStreamingBulkDataBytePtrs[buffer];
            }

            return null;
        }

        /// <summary>
        /// Helper function to retrieve a reference to the managed data buffer associated
        /// with the specified IntPtr.  This is typically used from within a completion
        /// routine to convert a BulkStream.Buffer IntPtr object back to a managed object
        /// when data buffers are automatically being pinned.
        /// </summary>
        /// <param name="buffer">The IntPtr data buffer (typically the BulkStream.Buffer
        /// member)</param>
        /// <returns>A reference to the managed ushort[] data buffer or null if the IntPtr
        /// is not associated with any automatically pinned data buffers.</returns>
        public static ushort[] RetrieveManagedUShortDataBuffer(IntPtr buffer)
        {
            // Retreive the managed data pointer
            if (PinnedAsyncBulkDataUShortPtrs.ContainsKey(buffer))
            {
                return PinnedAsyncBulkDataUShortPtrs[buffer];
            }
            if (PinnedStreamingBulkDataUShortPtrs.ContainsKey(buffer))
            {
                return PinnedStreamingBulkDataUShortPtrs[buffer];
            }

            return null;
        }

        /// <summary>
        /// Helper function to retreive a reference to the managed QuickUsb object associated
        /// with the specified IntPtr handle.  This is typically used from within a completion
        /// routine to covert a BulkStream.Handle IntPtr object back to a managed object.
        /// </summary>
        /// <param name="hDevice">The IntPtr handle (typically the BulkStream.Handle member)</param>
        /// <returns></returns>
        public static QuickUsb RetreiveManagedQuickUsbObject(IntPtr hDevice)
        {
            if (QuickUsbObjects.ContainsKey(hDevice))
            {
                return QuickUsbObjects[hDevice];
            }

            return null;
        }

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Delegates
        //---------------------------------------------------------------------

        /// <summary>
        /// 
        /// </summary>
        /// <param name="value"></param>
		public delegate void PercentCompleteDelegate(int value);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bulkStream"></param>
        public delegate void BulkStreamCompletionRoutine([MarshalAs(UnmanagedType.LPStruct)] [In, Out] BulkStream bulkStream);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="hDevice"></param>
        /// <param name="percentComplete"></param>
        /// <param name="tag"></param>
		public delegate void ProgressDelegate(IntPtr hDevice, ushort percentComplete, IntPtr tag);

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Private and Protected Methods
        //---------------------------------------------------------------------

        /// <summary>
        /// For internal use only
        /// </summary>
        protected struct InternalStreamPinningData
        {
            /// <summary>
            /// For internal use only
            /// </summary>
            public GCHandle hPinnedDataArray;

            /// <summary>
            /// For internal use only
            /// </summary>
            public GCHandle[] hDataBuffers;
        }

        /// <summary>
        /// Internal function used to open a device if it is not already opened
        /// (with reference counting).
        /// </summary>
        /// <returns></returns>
		protected bool OpenPolitely()
		{
			// Disregard if already globally opened
			if (m_globalOpen != 0)
			{
				return true;
			}

			// This method of reference counting is extremely important.  If nested calls
			// to this function are made we need to make sure we know when to close
			// the QuickUSB (of course, only if locally opened).
			++m_localOpen;

			// Open the QuickUSB is it isn't already opened
			if (m_localOpen == 1)
            {
                if (QuickUsb.NativeMethods.QuickUsbOpen(out m_hDevice, m_devname.TrimEnd('\0')) == 0)
                {
                    --m_localOpen;
                    m_lastError = GetLastError();
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                // Store an entry in the module list
                if (!QuickUsbObjects.ContainsKey(m_hDevice))
                {
                    QuickUsbObjects.Add(m_hDevice, this);
                }
                else
                {
                    QuickUsbObjects[m_hDevice] = this;
                }
			}
			return true;
		}

        /// <summary>
        /// Internal function used to close a device if it is not already 
        /// opened (with reference counting).
        /// </summary>
		protected void ClosePolitely()
		{
			// Close the QuickUSB if we opened it locally
			if (m_localOpen > 0)
			{
                // Only close the module when our reference count indicates this 
                // is the last ClosePolitely call.
                if (m_localOpen == 1)
                {
				    // We don't want lastError to be overwritten, so any errors
                    NativeMethods.QuickUsbClose(m_hDevice);
                }

                // We must decrement our reference count after the call to Close
				--m_localOpen;
			}
		}

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        protected IntPtr AcquireInternalPin(byte[] data)
        {
            // We must pin the data object so that it is not moved by the garbage collector
            GCHandle hPinnedDataObject;
            if (!PinnedAsyncBulkDataByteObjects.ContainsKey(data))
            {
                // We must hold a reference to the pinned object until we're done with it
                hPinnedDataObject = GCHandle.Alloc(data, GCHandleType.Pinned);
                PinnedAsyncBulkDataByteObjects[data] = hPinnedDataObject;
                PinnedAsyncBulkDataBytePtrs[hPinnedDataObject.AddrOfPinnedObject()] = data;
            }
            else
            {
                hPinnedDataObject = PinnedAsyncBulkDataByteObjects[data];
            }

            return hPinnedDataObject.AddrOfPinnedObject();
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        protected IntPtr AcquireInternalPin(ushort[] data)
        {
            // We must pin the data object so that it is not moved by the garbage collector
            GCHandle hPinnedDataObject;
            if (!PinnedAsyncBulkDataUShortObjects.ContainsKey(data))
            {
                // We must hold a reference to the pinned object until we're done with it
                hPinnedDataObject = GCHandle.Alloc(data, GCHandleType.Pinned);
                PinnedAsyncBulkDataUShortObjects[data] = hPinnedDataObject;
                PinnedAsyncBulkDataUShortPtrs[hPinnedDataObject.AddrOfPinnedObject()] = data;
            }
            else
            {
                hPinnedDataObject = PinnedAsyncBulkDataUShortObjects[data];
            }

            return hPinnedDataObject.AddrOfPinnedObject();
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="bulkStream"></param>
        /// <returns></returns>
        protected IntPtr AcquireInternalPin(BulkStream bulkStream)
        {
            // We must pin the BulkStream object so that it is not moved by the garbage collector
            GCHandle hPinnedBulkStreamObject;
            if (!PinnedAsyncBulkStreamObjects.ContainsKey(bulkStream))
            {
                // We must hold a reference to the pinned object until we're done with it
                hPinnedBulkStreamObject = GCHandle.Alloc(bulkStream, GCHandleType.Pinned);
                PinnedAsyncBulkStreamObjects[bulkStream] = hPinnedBulkStreamObject;
            }
            else
            {
                hPinnedBulkStreamObject = PinnedAsyncBulkStreamObjects[bulkStream];
            }

            return hPinnedBulkStreamObject.AddrOfPinnedObject();
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="data"></param>
        /// <param name="numBuffers"></param>
        /// <returns></returns>
        protected InternalStreamPinningData AcquireInternalPin(byte[][] data, uint numBuffers)
        {
            // Automatically pin data buffers
            InternalStreamPinningData pinData = new InternalStreamPinningData();
            IntPtr[] hPinnedDataArray = new IntPtr[numBuffers];
            pinData.hDataBuffers = new GCHandle[numBuffers];
            for (int k = 0; k < numBuffers; ++k)
            {
                pinData.hDataBuffers[k] = GCHandle.Alloc(data[k], GCHandleType.Pinned);
                hPinnedDataArray[k] = pinData.hDataBuffers[k].AddrOfPinnedObject();
                PinnedStreamingBulkDataBytePtrs.Add(hPinnedDataArray[k], data[k]);
            }
            pinData.hPinnedDataArray = GCHandle.Alloc(hPinnedDataArray, GCHandleType.Pinned);

            return pinData;
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="data"></param>
        /// <param name="numBuffers"></param>
        /// <returns></returns>
        protected InternalStreamPinningData AcquireInternalPin(ushort[][] data, uint numBuffers)
        {
            // Automatically pin data buffers
            InternalStreamPinningData pinData = new InternalStreamPinningData();
            IntPtr[] hPinnedDataArray = new IntPtr[numBuffers];
            pinData.hDataBuffers = new GCHandle[numBuffers];
            for (int k = 0; k < numBuffers; ++k)
            {
                pinData.hDataBuffers[k] = GCHandle.Alloc(data[k], GCHandleType.Pinned);
                hPinnedDataArray[k] = pinData.hDataBuffers[k].AddrOfPinnedObject();
                PinnedStreamingBulkDataUShortPtrs.Add(hPinnedDataArray[k], data[k]);
            }
            pinData.hPinnedDataArray = GCHandle.Alloc(hPinnedDataArray, GCHandleType.Pinned);

            return pinData;
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="bulkStream"></param>
        protected void ReleaseInternalPin(BulkStream bulkStream)
        {
            // Release the pin on the BulkStream object
            if (PinnedAsyncBulkStreamObjects.ContainsKey(bulkStream))
            {
                PinnedAsyncBulkStreamObjects[bulkStream].Free();
                PinnedAsyncBulkStreamObjects.Remove(bulkStream);
            }
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="pinnedData"></param>
        protected void ReleaseInternalPin(IntPtr pinnedData)
        {
            // Release the pin on the data object
            if (PinnedAsyncBulkDataBytePtrs.ContainsKey(pinnedData))
            {
                // Retreive the managed data pointer
                byte[] data = PinnedAsyncBulkDataBytePtrs[pinnedData];

                // Release the pin
                PinnedAsyncBulkDataByteObjects[data].Free();

                // Remove the objects from the dictionaries
                PinnedAsyncBulkDataByteObjects.Remove(data);
                PinnedAsyncBulkDataBytePtrs.Remove(pinnedData);
            }
            else if (PinnedAsyncBulkDataUShortPtrs.ContainsKey(pinnedData))
            {
                // Retreive the managed data pointer
                ushort[] data = PinnedAsyncBulkDataUShortPtrs[pinnedData];

                // Release the pin
                PinnedAsyncBulkDataUShortObjects[data].Free();

                // Remove the objects from the dictionaries
                PinnedAsyncBulkDataUShortObjects.Remove(data);
                PinnedAsyncBulkDataBytePtrs.Remove(pinnedData);
            }
        }
        
        /// <summary>
        /// For internal use only
        /// </summary>
        /// <param name="pinData"></param>
        protected void ReleaseInternalPin(InternalStreamPinningData pinData)
        {
            if (pinData.hDataBuffers != null)
            {
                for (int k = 0; k < pinData.hDataBuffers.Length; ++k)
                {
                    if (PinnedStreamingBulkDataBytePtrs.ContainsKey(pinData.hDataBuffers[k].AddrOfPinnedObject()))
                    {
                        PinnedStreamingBulkDataBytePtrs.Remove(pinData.hDataBuffers[k].AddrOfPinnedObject());
                    }
                    else if (PinnedStreamingBulkDataUShortPtrs.ContainsKey(pinData.hDataBuffers[k].AddrOfPinnedObject()))
                    {
                        PinnedStreamingBulkDataUShortPtrs.Remove(pinData.hDataBuffers[k].AddrOfPinnedObject());
                    }
                    else
                    {
                        throw new Exception("QuickUSB Internal Error");
                    }

                    pinData.hDataBuffers[k].Free();
                }
                pinData.hPinnedDataArray.Free();

                pinData.hDataBuffers = null;
            }
        }

        /// <summary>
        /// For internal use only
        /// </summary>
        protected void ReleaseAllInternalPins()
        {
            // Release pins on managed byte[] objects
            foreach (KeyValuePair<IntPtr, byte[]> kv in new List<KeyValuePair<IntPtr, byte[]>>(PinnedAsyncBulkDataBytePtrs))
            {
                ReleaseInternalPin(kv.Key);
            }
            Debug.Assert(PinnedAsyncBulkDataBytePtrs.Count == 0);
            Debug.Assert(PinnedAsyncBulkDataByteObjects.Count == 0);
            PinnedAsyncBulkDataBytePtrs.Clear();
            PinnedAsyncBulkDataByteObjects.Clear();
            
            // Release pins on managed ushort[] objects
            foreach (KeyValuePair<IntPtr, ushort[]> kv in new List<KeyValuePair<IntPtr, ushort[]>>(PinnedAsyncBulkDataUShortPtrs))
            {
                ReleaseInternalPin(kv.Key);
            }
            Debug.Assert(PinnedAsyncBulkDataUShortPtrs.Count == 0);
            Debug.Assert(PinnedAsyncBulkDataUShortObjects.Count == 0);
            PinnedAsyncBulkDataUShortPtrs.Clear();
            PinnedAsyncBulkDataUShortObjects.Clear();

            // Release pins on managed BulkStream objects
            foreach (KeyValuePair<BulkStream, GCHandle> kv in new List<KeyValuePair<BulkStream, GCHandle>>(PinnedAsyncBulkStreamObjects))
            {
                ReleaseInternalPin(kv.Key);
            }

            // Release pins on managed streaming objects
            foreach (KeyValuePair<byte, InternalStreamPinningData> kv in new List<KeyValuePair<byte, InternalStreamPinningData>>(PinnedStreamingObjects))
            {
                ReleaseInternalPin(kv.Value);
            }
            Debug.Assert(PinnedStreamingObjects.Count == 0);
            Debug.Assert(PinnedStreamingBulkDataBytePtrs.Count == 0);
            Debug.Assert(PinnedStreamingBulkDataUShortPtrs.Count == 0);
            PinnedStreamingObjects.Clear();
            PinnedStreamingBulkDataBytePtrs.Clear();
            PinnedStreamingBulkDataUShortPtrs.Clear();
        }

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        #region Public Methods
        //---------------------------------------------------------------------

        /// <summary>
        /// Open a QuickUSB device for use by the library
        /// </summary>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool Open()
        {
            if (QuickUsb.NativeMethods.QuickUsbOpen(out m_hDevice, m_devname.TrimEnd('\0')) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }
            
            // Store an entry in the module list
            if (!QuickUsbObjects.ContainsKey(m_hDevice))
            {
                QuickUsbObjects.Add(m_hDevice, this);
            }
            else
            {
                QuickUsbObjects[m_hDevice] = this;
            }

            // This is a global open
            ++m_globalOpen;

            return true;
        }
        
        /// <summary>
        /// Open a QuickUSB device for use by the library
        /// </summary>
        /// <param name="devname">The name of the QuickUSB device</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool Open(string devname)
		{
			m_devname = devname.TrimEnd('\0');

			if (QuickUsb.NativeMethods.QuickUsbOpen(out m_hDevice, devname) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				return false;
			}

            // Store an entry in the module list
            if (!QuickUsbObjects.ContainsKey(m_hDevice))
            {
                QuickUsbObjects.Add(m_hDevice, this);
            }
            else
            {
                QuickUsbObjects[m_hDevice] = this;
            }

			// This is a global open
			++m_globalOpen;

			return true;
		}

        /// <summary>
        /// Open a QuickUSB device for use by the library
        /// </summary>
        /// <param name="devname">The name of the QuickUSB device</param>
        /// <param name="flags">Additional flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool OpenEx(string devname, OpenFlag flags)
        {
            m_devname = devname.TrimEnd('\0');

            if (QuickUsb.NativeMethods.QuickUsbOpenEx(out m_hDevice, devname, (ushort)flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Store an entry in the module list
            if (!QuickUsbObjects.ContainsKey(m_hDevice))
            {
                QuickUsbObjects.Add(m_hDevice, this);
            }
            else
            {
                QuickUsbObjects[m_hDevice] = this;
            }

            // If this is not a local open, it must be a global open
            if (m_localOpen == 0)
            {
                ++m_globalOpen;
            }
            return true;
        }

        /// <summary>
        /// Open a QuickUSB device for use by the library
        /// </summary>
        /// <param name="flags">Additional flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool OpenEx(OpenFlag flags)
        {
            if (QuickUsb.NativeMethods.QuickUsbOpenEx(out m_hDevice, m_devname.TrimEnd('\0'), (ushort)flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Store an entry in the module list
            if (!QuickUsbObjects.ContainsKey(m_hDevice))
            {
                QuickUsbObjects.Add(m_hDevice, this);
            }
            else
            {
                QuickUsbObjects[m_hDevice] = this;
            }

            // If this is not a local open, it must be a global open
            if (m_localOpen == 0)
            {
                ++m_globalOpen;
            }
            return true;
        }

        /// <summary>
        /// Close a QuickUSB device
        /// </summary>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool Close()
		{
			if (NativeMethods.QuickUsbClose(m_hDevice) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				return false;
			}

			// Decrement our open counter
            if (m_globalOpen > 0)
            {
                --m_globalOpen;
            }

            // When we're completely done with the device we must be sure to
            // free any pinned objects or memory leaks will occur
            if ((m_localOpen == 0) && (m_globalOpen == 0))
            {
                // Store an entry in the module list
                QuickUsbObjects.Remove(m_hDevice);

                // Release all pins
                ReleaseAllInternalPins();
            }

			return true;
		}

        /// <summary>
        /// Returns the string descriptor for the selected QuickUSB module
        /// </summary>
        /// <param name="stringIndex">The string index to retrieve</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public string GetStringDescriptor(StringDescriptor stringIndex)
		{
			int result;
            string desc;

			if (!OpenPolitely())
			{
				return null;
			}

            IntPtr pData = Marshal.AllocHGlobal(128);
			result = QuickUsb.NativeMethods.QuickUsbGetStringDescriptor(m_hDevice, stringIndex, pData, 128);
			if (result == 0)
            {
                Marshal.FreeHGlobal(pData);
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return null;
			}

            desc = Marshal.PtrToStringAnsi(pData);

            Marshal.FreeHGlobal(pData);

			ClosePolitely();
			return desc;
		}

        /// <summary>
        /// Return extended error information for the last method call that
        /// returned a value of false
        /// </summary>
        /// <returns>The extended error information.</returns>
		public Error LastError()
		{
			return m_lastError;
		}

        /// <summary>
        /// Return extended driver error information from the last method call
        /// that returned a value of false
        /// </summary>
        /// <returns>The extended error information.</returns>
        public uint LastDriverError()
        {
            return m_lastDriverError;
        }
        
        /// <summary>
        /// Determine the version of the QuickUSB Firmware is currently in the
        /// QuickUSB Module
        /// </summary>
        /// <param name="majorVersion">The major version number</param>
        /// <param name="minorVersion">The minor version number</param>
        /// <param name="buildVersion">The build version number</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool GetFirmwareVersion(out ushort majorVersion, out ushort minorVersion, out ushort buildVersion)
		{
			if (!OpenPolitely())
			{
                majorVersion = 0;
                minorVersion = 0;
                buildVersion = 0;
				return false;
			}

			if (NativeMethods.QuickUsbGetFirmwareVersion(m_hDevice, out majorVersion, out minorVersion, out buildVersion) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Determine the version of the QuickUSB Firmware is currently in the
        /// QuickUSB Module
        /// </summary>
        /// <param name="version">Variable to hold the version</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool GetFirmwareVersion(out Version version)
		{
			version = null;

			if (!OpenPolitely())
			{
				return false;
			}

			ushort majorVersion, minorVersion, buildVersion;
			if (NativeMethods.QuickUsbGetFirmwareVersion(m_hDevice, out majorVersion, out minorVersion, out buildVersion) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			// Use the native version object
			version = new Version(majorVersion, minorVersion, buildVersion);

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read QuickUSB module settings
        /// </summary>
        /// <param name="address">The setting address</param>
        /// <param name="value">A variable in which to place the value of the
        /// setting</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadSetting(Setting address, out ushort value)
		{
			value = 0;

			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbReadSetting(m_hDevice, address, out value) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write QuickUSB module settings
        /// </summary>
        /// <param name="address">The setting address</param>
        /// <param name="value">The setting value</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteSetting(Setting address, ushort value)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbWriteSetting(m_hDevice, address, value) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read QuickUSB module defaults. The defaults are non-volatile and
        /// are read into the settings table on power up.
        /// </summary>
        /// <param name="address">The default setting address</param>
        /// <param name="value">The read default value</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadDefault(Setting address, out ushort value)
		{
			value = 0;

			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbReadDefault(m_hDevice, address, out value) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write QuickUSB module defaults. The defaults are non-volatile and
        /// are read into the settings table on power up.
        /// </summary>
        /// <param name="address">The default setting address</param>
        /// <param name="value">The value to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteDefault(Setting address, ushort value)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbWriteDefault(m_hDevice, address, value) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Start the process of FPGA configuration. If the FPGA is in the
        /// process of being configured, the process will restart. If the FPGA
        /// is already configured, it will be reset and reconfigured.
        /// </summary>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool StartFpgaConfiguration()
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbStartFpgaConfiguration(m_hDevice) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Sends FPGA configuration data to the FPGA using the QuickUSB FPGA
        /// configuration port
        /// </summary>
        /// <param name="fpgadata">The FPGA byte data to write</param>
        /// <param name="datalength">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteFpgaData(byte[] fpgadata, uint datalength)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(fpgadata, GCHandleType.Pinned);
			if (NativeMethods.QuickUsbWriteFpgaData(m_hDevice, hData.AddrOfPinnedObject(), datalength) == 0)
			{
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Check to see if the FPGA is configured
        /// </summary>
        /// <param name="isConfigured">Outputs true if the FPGA is configured, false otherwise</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool IsFpgaConfigured(out bool isConfigured)
		{
			isConfigured = false;
			ushort conf;

			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbIsFpgaConfigured(m_hDevice, out conf) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			isConfigured = (conf != 0);

			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Configure an FPGA with an Altera RBF or Xilinx BIN data file
        /// </summary>
        /// <param name="filePath">The path to the data file</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ConfigureFpga(string filePath)
        {
            if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbConfigureFpga(m_hDevice, filePath) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
        }

        /// <summary>
        /// Configure an FPGA with an Altera RBF or Xilinx BIN data file
        /// </summary>
        /// <param name="path">The path to the data file.  If the file does 
        /// not exist a standard Windows File Browser will be displayed to 
        /// prompt the user for the location of the data file.</param>
        /// <param name="updateProgress">A callback delegate indicating the 
        /// progress of the FPGA configuration</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool UploadFpga(String path, PercentCompleteDelegate updateProgress)
		{
            const int maxControlTransferSize = 1024; // If were not running in Super-Speed, then transfers will automatically be broken into 64-byte chunks
			long index, blocksize, length, bytesremaining;
            byte[] buffer = new byte[maxControlTransferSize];

			if (updateProgress != null)
			{
				updateProgress(0);
			}

            // If the path isn't valid, give the user a chance to pick a file
            if (!System.IO.File.Exists(path))
            {
                OpenFileDialog d = new OpenFileDialog();
                d.Filter = "RBF Files|*.rbf|Bin Files|*.bin";
                if (d.ShowDialog() == DialogResult.OK)
                {
                    path = d.FileName;
                }
                else
                {
                    return false;
                }
            }

            // Open the FPGA file and get the length
			FileStream fs;
			try
			{
				fs = new FileStream(path, FileMode.Open, FileAccess.Read);
			}
			catch
			{
				return false;
			}
			length = fs.Length;

			// Read the file block by block and write to the FPGA
			index = 0;
			bytesremaining = length;
			do
			{
                blocksize = (bytesremaining > maxControlTransferSize) ? maxControlTransferSize : bytesremaining;
				fs.Read(buffer, 0, (int) blocksize);
				if (!WriteFpgaData(buffer, (uint)blocksize))
				{
					m_lastError = GetLastError();
					m_lastDriverError = GetLastDriverError();
					fs.Close();
					return false;
				}

				index += blocksize;
				bytesremaining -= blocksize;
				if (updateProgress != null)
				{
					updateProgress((int)((index * 100) / length));
				}

				Application.DoEvents();
			} while ((length - index) > 0);

			// Close the FPGA file
			fs.Close();

			return true;
		}

        /// <summary>
        /// Write a block of command values to the high-speed parallel port
        /// using the QuickUSB module
        /// </summary>
        /// <param name="address">This address is the starting value of the 
        /// HSPP address bus. If address bit 14 is set (1), then the address 
        /// bus will not be driven. If address bit 15 is set (1), then the 
        /// address will not be incremented after each write.</param>
        /// <param name="data">The buffer containing the data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteCommand(ushort address, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
			if (NativeMethods.QuickUsbWriteCommand(m_hDevice, address, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Write a block of command values to the high-speed parallel port
        /// using the QuickUSB module
        /// </summary>
        /// <param name="address">This address is the starting value of the 
        /// HSPP address bus. If address bit 14 is set (1), then the address 
        /// bus will not be driven. If address bit 15 is set (1), then the 
        /// address will not be incremented after each write.</param>
        /// <param name="data">The buffer containing the data to write</param>
        /// <param name="length">The number of ushorts to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteCommand(ushort address, ushort[] data, ushort length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2; // Convert to number of bytes
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteCommand(m_hDevice, address, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="address">This address is the starting value of the 
        /// HSPP address bus. If address bit 14 is set (1), then the address 
        /// bus will not be driven. If address bit 15 is set (1), then the 
        /// address will not be incremented after each read.</param>
        /// <param name="data">The buffer containing the read data</param>
        /// <param name="length">The number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadCommand(ushort address, byte[] data, ref ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadCommand(m_hDevice, address, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Read a block of command values from the high-speed parallel port
        /// using the QuickUSB module
        /// </summary>
        /// <param name="address">This address is the starting value of the 
        /// HSPP address bus. If address bit 14 is set (1), then the address 
        /// bus will not be driven. If address bit 15 is set (1), then the 
        /// address will not be incremented after each read.</param>
        /// <param name="data">The buffer containing the read data</param>
        /// <param name="length">The number of ushorts to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadCommand(ushort address, ushort[] data, ref ushort length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2; // Convert to number of bytes
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadCommand(m_hDevice, address, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                length /= 2; // Convert to number of ushorts
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }
            length /= 2; // Convert to number of ushorts

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteData(IntPtr data, uint length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbWriteData(m_hDevice, data, length) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteDataEx(IntPtr data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbWriteDataEx(m_hDevice, data, ref length, flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }
        
        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteData(byte[] data, uint length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteData(m_hDevice, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteDataEx(byte[] data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteDataEx(m_hDevice, hData.AddrOfPinnedObject(), ref length, flags) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }
        
        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of words to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteData(ushort[] data, uint length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2;
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteData(m_hDevice, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of words to write</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteDataEx(ushort[] data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2;
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteDataEx(m_hDevice, hData.AddrOfPinnedObject(), ref length, flags) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadData(IntPtr data, ref uint length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbReadData(m_hDevice, data, ref length) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of bytes to read</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadDataEx(IntPtr data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbReadDataEx(m_hDevice, data, ref length, flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadData(byte[] data, ref uint length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadData(m_hDevice, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of bytes to read</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadDataEx(byte[] data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadDataEx(m_hDevice, hData.AddrOfPinnedObject(), ref length, flags) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }
        
        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of words to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadData(ushort[] data, ref uint length)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2;
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadData(m_hDevice, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }
            length /= 2;

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// the QuickUSB module
        /// </summary>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of words to read</param>
        /// <param name="flags">Extra flags</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadDataEx(ushort[] data, ref uint length, DataFlags flags)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            length *= 2;
            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadDataEx(m_hDevice, hData.AddrOfPinnedObject(), ref length, flags) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }
            length /= 2;

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write a block of data values to the high-speed parallel port using
        /// an asynchronous function call
        /// </summary>
        /// <param name="data">The data to write.  This data must be pinned 
        /// (unable to be reloacted by the Garbage Collector) and must not be 
        /// freed until the request has fully completed as indicated by a call
        /// to QuickUsb.AsyncWait.</param>
        /// <param name="length">The number of bytes to write</param>
        /// <param name="transaction">The transaction ID of the request.  This 
        /// ID is used with the QuickUsb.AsyncWait function to determine when 
        /// the request has completed.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteDataAsync(IntPtr data, uint length, out byte transaction)
		{
			transaction = 0;

			if (!OpenPolitely())
			{
				return false;
			}

            if (NativeMethods.QuickUsbWriteDataAsync(m_hDevice, data, length, out transaction) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            // Keep track of the number of outstanding requests so we know when to politely close
            ++m_LegacyTransCount;

            // Don't politely close module now that we're performing async requests
			return true;
		}

        /// <summary>
        /// Read a block of data values from the high-speed parallel port using
        /// an asynchronous function call
        /// </summary>
        /// <param name="data">The read data.  This data must be pinned 
        /// (unable to be reloacted by the Garbage Collector) and must not be 
        /// freed until the request has fully completed as indicated by a call
        /// to QuickUsb.AsyncWait.</param>
        /// <param name="length">The number of bytes to read</param>
        /// <param name="transaction">The transaction ID of the request.  This 
        /// ID is used with the QuickUsb.AsyncWait function to determine when 
        /// the request has completed.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadDataAsync(IntPtr data, ref uint length, out byte transaction)
		{
			transaction = 0;

			if (!OpenPolitely())
			{
				ClosePolitely();
				return false;
			}

            if (NativeMethods.QuickUsbReadDataAsync(m_hDevice, data, ref length, out transaction) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            // Keep track of the number of outstanding requests so we know when to politely close
            ++m_LegacyTransCount;

            // Don't politely close module now that we're performing async requests
			return true;
		}

        /// <summary>
        /// Wait for an asynchronous transfer to complete
        /// </summary>
        /// <param name="bytecount">The number of bytes written/read for the 
        /// transactions.  This function must be called until bytecount is 
        /// not zero or until the transaction times out.</param>
        /// <param name="transaction">The transaction ID, as returned by a 
        /// call to QuickUsb.WriteDataAsync or QuickUsb.ReadDataAsync.</param>
        /// <param name="immediate">If true, the function will check if the 
        /// request has completed and return immediately.  If false, the 
        /// function will block until the request either completes or times 
        /// out.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool AsyncWait(out uint bytecount, byte transaction, byte immediate)
		{
			bytecount = 0;

            // Don't politely open as we must already be open for this call

			if (NativeMethods.QuickUsbAsyncWait(m_hDevice, out bytecount, transaction, immediate) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
                if (immediate == 0)
                {
                    ClosePolitely();
                }
				return false;
			}

            // Keep track of the number of outstanding requests so we know when to politely close
            if (bytecount != 0)
            {
                --m_LegacyTransCount;
            }

            // Politely close
            if (immediate == 0)
            {
                ClosePolitely();
            }
			return true;
		}

        /// <summary>
        /// Wait for an asynchronous transfer to complete
        /// </summary>
        /// <param name="bytecount">The number of bytes written/read for the 
        /// transactions.  This function must be called until bytecount is 
        /// not zero or until the transaction times out.</param>
        /// <param name="transaction">The transaction ID, as returned by a 
        /// call to QuickUsb.WriteDataAsync or QuickUsb.ReadDataAsync.</param>
        /// <param name="immediate">If true, the function will check if the 
        /// request has completed and return immediately.  If false, the 
        /// function will block until the request either completes or times 
        /// out.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool AsyncWait(out uint bytecount, byte transaction, bool immediate)
        {
            // We take a 'bool' for the immediate parameter instead of a 'byte'
            return AsyncWait(out bytecount, transaction, (byte)(immediate ? 1 : 0));
        }

        /// <summary>
        /// Read the data direction of each data port bit for the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data direction bit values. Each bit in data 
        /// corresponds to data bits of the specified port. A data direction 
        /// bit value of 0=input and 1=output (i.e. 0x03 means that bits 0 
        /// and 1 are outputs and bits 2-7 are inputs).</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadPortDir(Port address, out byte data)
		{
			data = 0;

			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbReadPortDir(m_hDevice, address, out data) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Set the data direction of each data port bit for the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data direction bit values. Each bit in data 
        /// corresponds to data bits of the specified port. A data direction 
        /// bit value of 0=input and 1=output (i.e. 0x03 means that bits 0 
        /// and 1 are outputs and bits 2-7 are inputs).</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WritePortDir(Port address, byte data)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbWritePortDir(m_hDevice, address, data) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Read a byte from the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data byte read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadPort(Port address, out byte data)
		{
            byte[] dataarray = new byte[1];
            ushort length = 1;

			if (!OpenPolitely())
			{
                data = 0;
				return false;
			}

            GCHandle hData = GCHandle.Alloc(dataarray, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadPort(m_hDevice, address, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                data = 0;
				return false;
			}

            data = dataarray[0];

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read a series of bytes from the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data read from the port, byte-by-byte</param>
        /// <param name="length">The number of bytes to read.  Also, outputs 
        /// the number of bytes actually read.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadPort(Port address, byte[] data, ref ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadPort(m_hDevice, address, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Read a byte from the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data byte to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WritePort(Port address, byte data)
        {
            byte[] dataarray = new byte[1];

            if (!OpenPolitely())
            {
                return false;
            }

            dataarray[0] = data;
            GCHandle hData = GCHandle.Alloc(dataarray, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWritePort(m_hDevice, address, hData.AddrOfPinnedObject(), 1) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Read a series of bytes from the specified port
        /// </summary>
        /// <param name="address">The port address. Ports are addressed 0 to 4
        /// corresponding to port A-E.</param>
        /// <param name="data">The data to write to the port, byte-by-byte</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WritePort(Port address, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWritePort(m_hDevice, address, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}


        /// <summary>
        /// Set the baud rate for both serial ports. Baud rates are
        /// programmable from 4800 to 230k baud. This function sets the baud
        /// rate of both serial ports. It is not possible to set the baud rate
        /// of each serial port independently.
        /// </summary>
        /// <param name="baudrate">The baud rate</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool SetRS232BaudRate(uint baudrate)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbSetRS232BaudRate(m_hDevice, baudrate) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read a block of characters from the interrupt receive buffer of the
        /// specified QuickUSB serial port
        /// </summary>
        /// <param name="portNum">The serial port number. Serial port 0 
        /// (P1) = 0, serial port 1 (P2) = 1.</param>
        /// <param name="data">The buffer to place the read data. The buffer 
        /// must be at least 128 bytes long.</param>
        /// <param name="length">The number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadRS232(byte portNum, byte[] data, ref ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadRS232(m_hDevice, portNum, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write a block of characters to the specified QuickUSB serial port
        /// </summary>
        /// <param name="portNum">The serial port number. Serial port 0 
        /// (P1) = 0, serial port 1 (P2) = 1.</param>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteRS232(byte portNum, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteRS232(m_hDevice, portNum, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Flush the RS232 port transmit and receive buffers
        /// </summary>
        /// <param name="portNum">The serial port number. Serial port 0 
        /// (P1) = 0, serial port 1 (P2) = 1.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool FlushRS232(byte portNum)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbFlushRS232(m_hDevice, portNum) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read the number of characters waiting in the receive buffer
        /// </summary>
        /// <param name="portNum">The serial port number. Serial port 0 
        /// (P1) = 0, serial port 1 (P2) = 1.</param>
        /// <param name="length">The number of characters that may be read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool GetNumRS232(byte portNum, out ushort length)
		{
			length = 0;

			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbGetNumRS232(m_hDevice, portNum, out length) == 0)
            {
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read the I2C port
        /// </summary>
        /// <param name="address">The device address</param>
        /// <param name="data">The read data</param>
        /// <param name="length">the number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadI2C(ushort address, byte[] data, ref ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadI2C(m_hDevice, address, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}
        
        /// <summary>
        /// Read the I2C port by first writing a register index, then 
        /// performing a read
        /// </summary>
        /// <param name="address">The device address</param>
        /// <param name="regaddr">The register index</param>
        /// <param name="data">The read data</param>
        /// <param name="length">The number of words to read.  Also, outputs 
        /// the number of bytes actually read.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadI2C(ushort address, byte regaddr, ushort[] data, ref ushort length)
        {
            ushort writelen = 1;
            ushort readlen = (ushort)(length * 2);
            byte[] bytedata = new byte[readlen];

            if (!OpenPolitely())
            {
                return false;
            }

            // Cache the register address in the firmware
            bytedata[0] = regaddr;
            GCHandle hData = GCHandle.Alloc(bytedata, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteI2C(m_hDevice, (ushort)(address | (writelen << 8)), hData.AddrOfPinnedObject(), writelen) == 0)
            {
                hData.Free();
                ClosePolitely();
                return false;
            }
            hData.Free();

            hData = GCHandle.Alloc(bytedata, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadI2C(m_hDevice, address, hData.AddrOfPinnedObject(), ref readlen) == 0)
            {
                hData.Free();
                ClosePolitely();
                return false;
            }

            // Copy the data back
            for (byte i = 0; i < length; i++)
            {
                data[i] = (ushort)((bytedata[(i * 2) + 0] << 8) + (bytedata[(i * 2) + 1]));
            }

            hData.Free();
            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write the I2C port
        /// </summary>
        /// <param name="address">The device address</param>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteI2C(ushort address, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
			if (NativeMethods.QuickUsbWriteI2C(m_hDevice, address, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write the I2C port by first writing a register index, then by 
        /// writing the data
        /// </summary>
        /// <param name="address">The device address</param>
        /// <param name="regaddr">The register index</param>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of words to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteI2C(ushort address, byte regaddr, ushort[] data, ushort length)
        {
            ushort bytelen = (ushort)((length * 2) + 1);
            byte[] bytedata = new byte[bytelen];

            if (!OpenPolitely())
            {
                return false;
            }

            // Set the address and the data
            bytedata[0] = regaddr;
            for (byte i = 0; i < length; i++)
            {
                bytedata[(i * 2) + 1] = (byte)(data[i] >> 8);
                bytedata[(i * 2) + 2] = (byte)(data[i] & 0xFF);
            }

            GCHandle hData = GCHandle.Alloc(bytedata, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteI2C(m_hDevice, address, hData.AddrOfPinnedObject(), bytelen) == 0)
            {
                hData.Free();
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            hData.Free();
            ClosePolitely();
            return true;
        }
        
        /// <summary>
        /// Perform a cached-write (repeated-start) on the I2C port
        /// </summary>
        /// <param name="address">The device address</param>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool CachedWriteI2C(ushort address, byte[] data, ushort length)
        {
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbCachedWriteI2C(m_hDevice, address, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
        }

        /// <summary>
        /// Read a block of bytes from memory
        /// </summary>
        /// <param name="address">The byte offset into memory where the read 
        /// should begin</param>
        /// <param name="data">The data read</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadStorage(ushort address, byte[] data, ushort bytes)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadStorage(m_hDevice, address, hData.AddrOfPinnedObject(), bytes) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write a block of bytes to memory
        /// </summary>
        /// <param name="address">The byte offset into memory where the write 
        /// should begin</param>
        /// <param name="data">The data to write</param>
        /// <param name="bytes">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteStorage(ushort address, byte[] data, ushort bytes)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteStorage(m_hDevice, address, hData.AddrOfPinnedObject(), bytes) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Set the timeout for QuickUSB read data transfers
        /// </summary>
        /// <param name="timeOut">The timeout value, in milliseconds</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool SetTimeout(uint timeOut)
		{
			if (!OpenPolitely())
			{
				return false;
			}

			if (NativeMethods.QuickUsbSetTimeout(m_hDevice, timeOut) == 0)
			{
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Read a block of bytes from the specified SPI slave port
        /// </summary>
        /// <param name="portNum">The SPI device address (nSS line) to read 
        /// from</param>
        /// <param name="data">The data to read</param>
        /// <param name="length">The number of bytes to read.  Also, outputs 
        /// the number of bytes actually read.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool ReadSPI(byte portNum, byte[] data, ref ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbReadSpi(m_hDevice, portNum, hData.AddrOfPinnedObject(), ref length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Write a block of bytes to the specified SPI slave port
        /// </summary>
        /// <param name="portNum">The SPI device address (nSS line) to read 
        /// from</param>
        /// <param name="data">The data to write</param>
        /// <param name="length">The number of bytes to write</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteSPI(byte portNum, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteSpi(m_hDevice, portNum, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
		}

        /// <summary>
        /// Simultaneously write and read a block of data to and from the
        /// specified SPI slave port
        /// </summary>
        /// <param name="portNum">The SPI device address (nSS line) to read 
        /// from</param>
        /// <param name="data">The data to write.  The buffer is overwritten 
        /// with the read data.</param>
        /// <param name="length">The number of bytes to write/read</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
		public bool WriteReadSPI(byte portNum, byte[] data, ushort length)
		{
			if (!OpenPolitely())
			{
				return false;
			}

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);
            if (NativeMethods.QuickUsbWriteReadSpi(m_hDevice, portNum, hData.AddrOfPinnedObject(), length) == 0)
            {
                hData.Free();
				m_lastError = GetLastError();
				m_lastDriverError = GetLastDriverError();
				ClosePolitely();
				return false;
			}

            hData.Free();
			ClosePolitely();
			return true;
        }

        /// <summary>
        /// Perform an asynchronous read with the option of having a callback 
        /// routine.  The function automatically handles pinning and unpinning
        /// the bulkStream object but not the data.
        /// </summary>
        /// <param name="data">The data to read.  This buffer must remain 
        /// valid until the request completes.</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataAsync(IntPtr data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (bulkStream == null)
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // We must pin the BulkStream object so that it is not moved by the garbage collector
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbReadBulkDataAsync(m_hDevice, data, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Perform an asynchronous read with the option of having a callback 
        /// routine.  The function automatically handles pinning and unpinning
        /// the bulkStream object and data.
        /// </summary>
        /// <param name="data">The data to read</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataAsync(byte[] data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (bulkStream == null)
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Get the pointers to the pinned data and bulkstream objects
            IntPtr pinnedData = AcquireInternalPin(data);
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbReadBulkDataAsync(m_hDevice, pinnedData, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                ReleaseInternalPin(pinnedData);
                ReleaseInternalPin(bulkStream);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Perform an asynchronous read with the option of having a callback 
        /// routine.  The function automatically handles pinning and unpinning
        /// the bulkStream object and data.
        /// </summary>
        /// <param name="data">The data to read</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataAsync(ushort[] data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (bulkStream == null)
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Get the pointers to the pinned data and bulkstream objects
            IntPtr pinnedData = AcquireInternalPin(data);
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbReadBulkDataAsync(m_hDevice, pinnedData, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                ReleaseInternalPin(pinnedData);
                ReleaseInternalPin(bulkStream);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Perform an asynchronous write with the option of having a callback
        /// routine  The function automatically handles pinning and unpinning
        /// the bulkStream object but not the data.
        /// </summary>
        /// <param name="data">The data to write.  This buffer must remain 
        /// valid until the request completes.</param>
        /// <param name="bytes">The number of bytes to write</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataAsync(IntPtr data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            // We must pin the BulkStream object so that it is not moved by the garbage collector
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbWriteBulkDataAsync(m_hDevice, data, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Perform an asynchronous write with the option of having a callback 
        /// routine.  The function automatically handles pinning and unpinning
        /// the bulkStream object and data.
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataAsync(byte[] data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (bulkStream == null)
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Get the pointers to the pinned data and bulkstream objects
            IntPtr pinnedData = AcquireInternalPin(data);
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbWriteBulkDataAsync(m_hDevice, pinnedData, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                ReleaseInternalPin(pinnedData);
                ReleaseInternalPin(bulkStream);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Perform an asynchronous write with the option of having a callback 
        /// routine.  The function automatically handles pinning and unpinning
        /// the bulkStream object and data.
        /// </summary>
        /// <param name="data">The data to write</param>
        /// <param name="bytes">The number of bytes to read</param>
        /// <param name="bulkStream">A QuickUsb.BulkStream object</param>
        /// <param name="completion">(Optional) The completion routine to call 
        /// when the request completes.  To not use, specifiy IntPtr.Zero.</param>
        /// <param name="tag">(Optional) A user pointer passed on to the 
        /// completion rountine used to associate user information with the 
        /// request.  To not use, specifiy IntPtr.Zero.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataAsync(ushort[] data, uint bytes, BulkStream bulkStream, BulkStreamCompletionRoutine completion, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (bulkStream == null)
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Get the pointers to the pinned data and bulkstream objects
            IntPtr pinnedData = AcquireInternalPin(data);
            IntPtr pinnedBulkstream = AcquireInternalPin(bulkStream);

            if (NativeMethods.QuickUsbWriteBulkDataAsync(m_hDevice, pinnedData, bytes, pinnedBulkstream, completion, tag) == 0)
            {
                ReleaseInternalPin(pinnedData);
                ReleaseInternalPin(bulkStream);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're performing async requests
            return true;
        }

        /// <summary>
        /// Check if a asynchronous transaction (made with either 
        /// QuickUsb.WriteBulkDataAsync or QuickUsb.ReadBulkDataAsync) has 
        /// completed, or wait for the transaction to complete
        /// </summary>
        /// <param name="bulkStream">The QuickUsbBulkStream object passed into
        /// the corresponding QuickUsb.WriteBulkDataAsync or 
        /// QuickUsb.ReadBulkDataAsync function call.  This object must not be
        /// able to be relocated by the Garbage Collector and must exist until
        /// the request completes.</param>
        /// <param name="immediate">If false, the function will block until the 
        /// request completes and executes its completion routine (if any).  
        /// If true, the function will check if the indicated request has 
        /// already completed and executed its associated completion routine 
        /// (if any) and return immediately.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool BulkWait(BulkStream bulkStream, bool immediate)
        {
            // Don't politely open as we must already be open for this call

            if (bulkStream != null)
            {
                // Verify that the BulkStream object is in our internal list
                if (!PinnedAsyncBulkStreamObjects.ContainsKey(bulkStream))
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                if (NativeMethods.QuickUsbBulkWait(m_hDevice, PinnedAsyncBulkStreamObjects[bulkStream].AddrOfPinnedObject(), (byte)(immediate ? 1 : 0)) == 0)
                {
                    m_lastError = GetLastError();
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                // If we're done with this bulkStream object then release our pin
                if (!immediate)
                {
                    ReleaseInternalPin(bulkStream);
                }
            }
            else
            {
                if (NativeMethods.QuickUsbBulkWait(m_hDevice, IntPtr.Zero, (byte)(immediate ? 1 : 0)) == 0)
                {
                    m_lastError = GetLastError();
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                // All requests have been completed so we may release all pins
                ReleaseAllInternalPins();
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Abort in-flight asynchronous requests without having to wait for them 
        /// to timeout.
        /// </summary>
        /// <param name="bulkStream">The QuickUsbBulkStream object passed into
        /// the corresponding QuickUsb.WriteBulkDataAsync or 
        /// QuickUsb.ReadBulkDataAsync function call.  This object must not be
        /// able to be relocated by the Garbage Collector and must exist until
        /// the request completes.  Pass zero (0) to abort all pending 
        /// asynchronous IO requests.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool BulkAbort(BulkStream bulkStream)
        {
            // Don't politely open as we must already be open for this call

            if (bulkStream != null)
            {
                // Verify that the BulkStream object is in our internal list
                if (!PinnedAsyncBulkStreamObjects.ContainsKey(bulkStream))
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                if (NativeMethods.QuickUsbBulkAbort(m_hDevice, PinnedAsyncBulkStreamObjects[bulkStream].AddrOfPinnedObject()) == 0)
                {
                    m_lastError = GetLastError();
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }
            }
            else
            {
                if (NativeMethods.QuickUsbBulkAbort(m_hDevice, IntPtr.Zero) == 0)
                {
                    m_lastError = GetLastError();
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Set the number of threads and the thread concurrency for the 
        /// Asynchronous Data API.
        /// </summary>
        /// <param name="numThreads">A word indicating the number of threads 
        /// to allocate for the Asynchronous API.  If numThreads is zero, no 
        /// threads are created and the Asynchronous API will be 
        /// single-threaded.  If numThreads is greater than zero, the 
        /// Asynchronous API is multithreaded—the indicated number of threads 
        /// will be created and dedicated to asynchronously processing 
        /// completion routines.  When multithreading, the recommended number 
        /// of threads to maximize multithreading performance is equal to the 
        /// number of processors/cores in the system times two.</param>
        /// <param name="concurrency">A word indicating the number of threads 
        /// that are allowed to execute completion routines simultaneously.  
        /// Specify zero if numThreads is zero.  When multithreading, the 
        /// number of threads allowed to execute completion routines 
        /// concurrently will be limited by the value of ‘concurrency’, which 
        /// must be at least one.  The recommended concurrency to maximize 
        /// multithreading performance is equal to the total number of 
        /// processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool SetNumAsyncThreads(ushort numThreads, ushort concurrency)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbSetNumAsyncThreads(m_hDevice, numThreads, concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Get the number of threads and the thread concurrency for the 
        /// Asynchronous Data API.
        /// </summary>
        /// <param name="numThreads">A pointed to a word indicating the number 
        /// of threads allocated for the Asynchronous API.</param>
        /// <param name="concurrency">A pointer to a word indicating the 
        /// number of threads that are allowed to execute completion routines 
        /// simultaneously.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool GetNumAsyncThreads(out ushort numThreads, out ushort concurrency)
        {
            if (!OpenPolitely())
            {
                numThreads = 0;
                concurrency = 0;
                return false;
            }

            if (NativeMethods.QuickUsbGetNumAsyncThreads(m_hDevice, out numThreads, out concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Start streaming data into the computer from the QuickUSB device.
        /// </summary>
        /// <param name="buffers">(Optional) An array of pointers to the user allocated
        /// buffers used to store the read data.  These buffers must 
        /// remain valid until the read stream has been stopped.  If this
        /// parameter is null then the API will allocate and managed the 
        /// memory for the data buffers.  </param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        /// called each time a buffer is filled with data.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataStartStream(IntPtr buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbReadBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data into the computer from the QuickUSB device.
        /// </summary>
        /// <param name="buffers">(Optional) An array of pointers to the user allocated
        /// buffers used to store the read data.  These buffers must 
        /// remain valid until the read stream has been stopped.  If this
        /// parameter is null then the API will allocate and managed the 
        /// memory for the data buffers.  </param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        /// called each time a buffer is filled with data.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataStartStream(IntPtr[] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbReadBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data into the computer from the QuickUSB device.  The 
        /// function automatically handles pinning and unpinning the data buffers.
        /// </summary>
        /// <param name="buffers">(Optional) An array of pointers to the user allocated
        /// buffers used to store the read data.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        /// called each time a buffer is filled with data.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataStartStream(byte[][] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            streamID = 0;
            if (!OpenPolitely())
            {
                return false;
            }

            // Verify that each buffer is allocated and is the appropriate size
            if (buffers != null)
            {
                if (buffers.Length < numBuffers)
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                for (int k = 0; k < numBuffers; ++k)
                {
                    if ((buffers[k] == null) || (buffers[k].Length < bufferSize))
                    {
                        m_lastError = Error.InvalidParameter;
                        m_lastDriverError = GetLastDriverError();
                        return false;
                    }
                }
            }

            InternalStreamPinningData pinData = AcquireInternalPin(buffers, numBuffers);

            if (NativeMethods.QuickUsbReadBulkDataStartStream(m_hDevice, pinData.hPinnedDataArray.AddrOfPinnedObject(), numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                ReleaseInternalPin(pinData);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            PinnedStreamingObjects.Add(streamID, pinData);

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data into the computer from the QuickUSB device.  The 
        /// function automatically handles pinning and unpinning the data buffers.
        /// </summary>
        /// <param name="buffers">(Optional) An array of pointers to the user allocated
        /// buffers used to store the read data.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        /// called each time a buffer is filled with data.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataStartStream(ushort[][] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            streamID = 0;
            if (!OpenPolitely())
            {
                return false;
            }

            // Verify that each buffer is allocated and is the appropriate size
            if (buffers != null)
            {
                if (buffers.Length < numBuffers)
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }

                for (int k = 0; k < numBuffers; ++k)
                {
                    if ((buffers[k] == null) || ((buffers[k].Length * 2) < bufferSize))
                    {
                        m_lastError = Error.InvalidParameter;
                        m_lastDriverError = GetLastDriverError();
                        return false;
                    }
                }
            }

            InternalStreamPinningData pinData = AcquireInternalPin(buffers, numBuffers);

            if (NativeMethods.QuickUsbReadBulkDataStartStream(m_hDevice, pinData.hPinnedDataArray.AddrOfPinnedObject(), numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                ReleaseInternalPin(pinData);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            PinnedStreamingObjects.Add(streamID, pinData);

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data to a disk file from the QuickUSB device.
        /// </summary>
        /// <param name="filePath">Path to the disk file to create.  If the
        /// file already exists, it will be overwritten.</param>
        /// <param name="numBuffers">The number of data buffers to use.</param>
        /// <param name="bufferSize">The size of each buffer, in bytes.</param>
        /// <param name="maxTransfers">The maximum number of data transfers to
        /// successfully complete.  Once the maximum is reached, the stream is
        /// automatically paused.  Each successful transfer equates to a single
        /// data buffer filling with 'bufferSize' bytes of data being written to
        /// disk.  Specifying a value of zero indicates the stream should not 
        /// automatically pause and instead continuously run until explicitly
        /// paused or stopped.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="flags">Additional flags controlling the operation of
        /// this function.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ReadBulkDataStartStreamToFile(string filePath, uint numBuffers, uint bufferSize, uint maxTransfers, out byte streamID, StreamFlag flags)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbReadBulkDataStartStreamToFile(m_hDevice, filePath, numBuffers, bufferSize, maxTransfers, out streamID, flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data from the computer to the QuickUSB device.
        /// </summary>
        /// <param name="buffers">An array of pointers to the user allocated buffers  
        /// holding the data to write.  These buffers must remain valid 
        /// until the read stream has been stopped.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        ///  called each time a buffer has been written.  This buffer must
        ///  be refilled with the new data to write from within the 
        ///  completion routine.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataStartStream(IntPtr buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbWriteBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data from the computer to the QuickUSB device.
        /// </summary>
        /// <param name="buffers">An array of pointers to the user allocated buffers  
        /// holding the data to write.  These buffers must remain valid 
        /// until the read stream has been stopped.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        ///  called each time a buffer has been written.  This buffer must
        ///  be refilled with the new data to write from within the 
        ///  completion routine.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataStartStream(IntPtr[] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbWriteBulkDataStartStream(m_hDevice, buffers, numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data from computer into the QuickUSB device.  The 
        /// function automatically handles pinning and unpinning the data buffers.
        /// </summary>
        /// <param name="buffers">An array of pointers to the user allocated buffers  
        /// holding the data to write.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        ///  called each time a buffer has been written.  This buffer must
        ///  be refilled with the new data to write from within the 
        ///  completion routine.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataStartStream(byte[][] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            streamID = 0;
            if (!OpenPolitely())
            {
                return false;
            }

            // Verify that each buffer is allocated and is the appropriate size
            if ((buffers == null) || (buffers.Length < numBuffers))
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            for (int k = 0; k < numBuffers; ++k)
            {
                if ((buffers[k] == null) || ((buffers[k].Length * 2) < bufferSize))
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }
            }

            InternalStreamPinningData pinData = AcquireInternalPin(buffers, numBuffers);

            if (NativeMethods.QuickUsbWriteBulkDataStartStream(m_hDevice, pinData.hPinnedDataArray.AddrOfPinnedObject(), numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                ReleaseInternalPin(pinData);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            PinnedStreamingObjects.Add(streamID, pinData);

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data into the computer from the QuickUSB device.  The 
        /// function automatically handles pinning and unpinning the data buffers.
        /// </summary>
        /// <param name="buffers">An array of pointers to the user allocated buffers  
        /// holding the data to write.</param>
        /// <param name="numBuffers">The number of buffers.</param>
        /// <param name="bufferSize">The byte size of each buffer.</param>
        /// <param name="cRoutine">The completion routine (callback function) that is
        ///  called each time a buffer has been written.  This buffer must
        ///  be refilled with the new data to write from within the 
        ///  completion routine.</param>
        /// <param name="tag">(Optional) A pointer containing user-specific data that is
        /// passed into the completion routine.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="numThreads">The number of threads to allocate for this 
        /// stream.  If numThreads is zero, no threads are created and the 
        /// stream will be single-threaded.  If numThreads is greater than zero, 
        /// the stream is multithreaded—the indicated number of threads will be 
        /// created and dedicated to asynchronously processing completion 
        /// routines.  When multithreading, the recommended number of threads to 
        /// maximize multithreading performance is equal to the number of 
        /// processors/cores in the system times two.</param>
        /// <param name="concurrency">The number of threads that are 
        /// allowed to execute completion routines simultaneously.  Specify zero 
        /// if numThreads is zero.  When multithreading, the number of threads 
        /// allowed to execute completion routines concurrently will be limited 
        /// by the value of ‘concurrency’, which must be at least one.  The 
        /// recommended concurrency to maximize multithreading performance is 
        /// equal to the total number of processors/cores in the system.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataStartStream(ushort[][] buffers, uint numBuffers, uint bufferSize, BulkStreamCompletionRoutine cRoutine, IntPtr tag, out byte streamID, ushort numThreads, ushort concurrency)
        {
            streamID = 0;
            if (!OpenPolitely())
            {
                return false;
            }

            // Verify that each buffer is allocated and is the appropriate size
            if ((buffers == null) || (buffers.Length < numBuffers))
            {
                m_lastError = Error.InvalidParameter;
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            for (int k = 0; k < numBuffers; ++k)
            {
                if ((buffers[k] == null) || ((buffers[k].Length * 2) < bufferSize))
                {
                    m_lastError = Error.InvalidParameter;
                    m_lastDriverError = GetLastDriverError();
                    return false;
                }
            }

            InternalStreamPinningData pinData = AcquireInternalPin(buffers, numBuffers);

            if (NativeMethods.QuickUsbWriteBulkDataStartStream(m_hDevice, pinData.hPinnedDataArray.AddrOfPinnedObject(), numBuffers, bufferSize, cRoutine, tag, out streamID, numThreads, concurrency) == 0)
            {
                ReleaseInternalPin(pinData);

                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            PinnedStreamingObjects.Add(streamID, pinData);

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Start streaming data from the computer to the QuickUSB device.
        /// </summary>
        /// <param name="filePath">Path to the disk file to read.  The file 
        /// must exist or the function call will fail.</param>
        /// <param name="numBuffers">The number of data buffers to use.</param>
        /// <param name="bufferSize">The size of each buffer, in bytes.</param>
        /// <param name="maxTransfers">The maximum number of data transfers to
        /// successfully complete.  Once the maximum is reached, the stream is
        /// automatically paused.  Each successful transfer equates to a single
        /// data buffer filling with 'bufferSize' bytes of data being read from
        /// disk and written out QuickUSB.  Specifying a value of zero indicates 
        /// the stream should not automatically pause and instead continuously 
        /// run until the entire contents of the file have been written out, or
        /// until the stream has been explicitly paused or stopped.</param>
        /// <param name="streamID">The ID of the stream.</param>
        /// <param name="flags">Additional flags controlling the operation of
        /// this function.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteBulkDataStartStreamFromFile(string filePath, uint numBuffers, uint bufferSize, uint maxTransfers, out byte streamID, StreamFlag flags)
        {
            if (!OpenPolitely())
            {
                streamID = 0;
                return false;
            }

            if (NativeMethods.QuickUsbWriteBulkDataStartStreamFromFile(m_hDevice, filePath, numBuffers, bufferSize, maxTransfers, out streamID, flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            // Don't politely close module now that we're streaming
            return true;
        }

        /// <summary>
        /// Stop a previously started read or write data stream.
        /// </summary>
        /// <param name="streamID">The ID of the stream to process, as returned
        /// from a call to QuickUsbReadBulkDataStartStream or 
        /// QuickUsbWriteBulkDataStartStream.</param>
        /// <param name="immediate">If false, this function will block until the stream has
        /// completely stopped and any remaining callbacks have been
        /// called.  If true, the stream will be marked to shutdown and
        /// will return without blocking.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool StopStream(byte streamID, bool immediate)
        {
            // Don't politely open as we must already be open for this call

            if (NativeMethods.QuickUsbStopStream(m_hDevice, streamID, immediate) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                if (!immediate)
                {
                    ClosePolitely();
                }
                return false;
            }
            
            // We're done streaming if we are not returning immediately, so we can politely close the module again
            if (!immediate)
            {
                // Check if the data buffers were automatically pinned
                if (PinnedStreamingObjects.ContainsKey(streamID))
                {
                    ReleaseInternalPin(PinnedStreamingObjects[streamID]);
                    PinnedStreamingObjects.Remove(streamID);
                }

                ClosePolitely();
            }
            return true;
        }

        /// <summary>
        /// For single-threaded streams, this function is called to process and
        /// re-issue completed requests.
        /// </summary>
        /// <param name="streamID">The ID of the stream to process, as returned
        /// from a call to QuickUsbReadBulkDataStartStream or 
        /// QuickUsbWriteBulkDataStartStream.</param>
        /// <param name="milliseconds">The number of milliseconds the caller is
        /// willing to wait until a data request on the indicated stream has 
        /// completed and is processed.  If no data request on the indicated 
        /// stream has completed within the specified amount of time, the 
        /// function will return unsuccessfully and a call to 
        /// QuickUsbGetLastError will indicate the error 
        /// QUICKUSB_ERROR_TIMEOUT.  If milliseconds is zero, the function will
        /// process any requests that are completed or timeout immediately.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ProcessStream(byte streamID, uint milliseconds)
        {
            // Don't politely open as we must already be open for this call

            if (NativeMethods.QuickUsbProcessStream(m_hDevice, streamID, milliseconds) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Don't politely close module because we're still streaming
            return true;
        }

        /// <summary>
        /// Pause the automatic re-issuing of data requests on a stream.
        /// </summary>
        /// <param name="streamID">The ID of the stream to process, as returned
        /// from a call to QuickUsbReadBulkDataStartStream or 
        /// QuickUsbWriteBulkDataStartStream.</param>
        /// <param name="immediate">If zero (or false), this function will 
        /// block until the stream has paused and all in-process requests have 
        /// completed.  If non-zero (or true), the stream will be marked to 
        /// enter the pause state and will return immediately without waiting 
        /// for any in-process requests to complete.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool PauseStream(byte streamID, bool immediate)
        {
            // Don't politely open as we must already be open for this call

            if (NativeMethods.QuickUsbPauseStream(m_hDevice, streamID, immediate) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Don't politely close module because we're still streaming
            return true;
        }

        /// <summary>
        /// Resume the automatic re-issuing of data requests on a stream.
        /// </summary>
        /// <param name="streamID">The ID of the stream to process, as returned
        /// from a call to QuickUsbReadBulkDataStartStream or 
        /// QuickUsbWriteBulkDataStartStream.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ResumeStream(byte streamID)
        {
            // Don't politely open as we must already be open for this call

            if (NativeMethods.QuickUsbResumeStream(m_hDevice, streamID) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Don't politely close module because we're still streaming
            return true;
        }

        /// <summary>
        /// Get the current status of a stream
        /// </summary>
        /// <param name="streamID">The ID of the stream to process, as returned
        /// from a call to QuickUsbReadBulkDataStartStream or 
        /// QuickUsbWriteBulkDataStartStream.</param>
        /// <param name="status">The status of the stream</param>
        /// <param name="error">The last error (if any) reported by the stream</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool GetStreamStatus(byte streamID, out StreamStatus status, out Error error)
        {
            // Don't politely open as we must already be open for this call

            if (NativeMethods.QuickUsbGetStreamStatus(m_hDevice, streamID, out status, out error) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                return false;
            }

            // Don't politely close module because we're still streaming
            return true;
        }

        /// <summary>
        /// Write QuickUSB firmware to a device
        /// </summary>
        /// <param name="fileName">The path and file name of the QuickUSB 
        /// firmware (QUSB) file</param>
        /// <param name="options">Specify QuickUsb.PreserveCustomization to 
        /// preserve customizations present in the current firmware, 
        /// QuickUsb.PreserveDefaults to preserve default settings present 
        /// in the current firmware, QuickUsb.PreserveGpif to preserve GPIF 
        /// customizations, and/or QuickUsb.PreserveSerial to preserve the 
        /// serial number.  To specify a new serial number, pass 
        /// QuickUsb.ProgramSerial bitwise or'ed (|) together with the new 
        /// serial number (must be between 1 and 65535).</param>
        /// <param name="callback">(Optional) A function to call during the 
        /// programming process to report the percentage completed.  This 
        /// may be used to perform background processes and GUI updates 
        /// while the programming process takes place.</param>
        /// <param name="tag">(Optional) A pointer to user data that is 
        /// passed onto the callback function.  Specify IntPtr.Zero to 
        /// ignore.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool WriteFirmware(string fileName, Programming options, ProgressDelegate callback, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbWriteFirmware(m_hDevice, fileName, (uint)options, callback, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Verify that the firmware contained in the specified device matches
        /// that of the supplied firmware file
        /// </summary>
        /// <param name="fileName">The path and file name of the QuickUSB 
        /// firmware (QUSB) file</param>
        /// <param name="callback">(Optional) A callback function to call 
        /// during the verify process to report the percentage completed.  
        /// This may be used to perform background processes and GUI updates 
        /// while the verify process takes place.</param>
        /// <param name="tag">(Optional) A pointer to user data that is 
        /// passed onto the callback function.  Specify IntPtr.Zero to 
        /// ignore.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool VerifyFirmware(string fileName, ProgressDelegate callback, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbVerifyFirmware(m_hDevice, fileName, callback, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Identify the EPCS device connected to the SPI bus on the specified
        /// nSS line
        /// </summary>
        /// <param name="nSS">The nSS SPI line that the EPCS device is 
        /// connected to (0-9)</param>
        /// <param name="epcsId">The ID of the EPCS device</param>
        /// <param name="epcsByteSize">Outputs the ID of the EPCS device 
        /// found.  This will be one of EpcsId.Epcs1 (0x10), EpcsId.Epcs4 
        /// (0x12), EpcsId.Epcs16 (0x14), EpcsId.Epcs64 (0x16), or 
        /// EpcsId.Unknown (0x00).</param>
        /// <param name="flags">Additional flags.  Specify 
        /// EpcsFlags.IgnoreNCE (0x0001) to prevent the function from 
        /// automatically asserting the nCE line of the FPGA at the 
        /// beginning of the function are re-asserting is at the end of 
        /// the function.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool IdentifyEpcs(byte nSS, out EpcsId epcsId, out uint epcsByteSize, EpcsFlags flags)
        {
            if (!OpenPolitely())
            {
                epcsId = EpcsId.EpcsUnknown;
                epcsByteSize = 0;
                return false;
            }

            if (NativeMethods.QuickUsbIdentifyEpcs(m_hDevice, nSS, out epcsId, out epcsByteSize, (uint)flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Write the specified FPGA data file to the EPCS device
        /// </summary>
        /// <param name="nSS">The nSS SPI line that the EPCS device is 
        /// connected to (0-9)</param>
        /// <param name="filePath">The path and file name of the FPGA data file</param>
        /// <param name="flags">Additional flags.  Specify 
        /// EpcsFlags.IgnoreNCE (0x0001) to prevent the function from 
        /// automatically asserting the nCE line of the FPGA at the 
        /// beginning of the function are re-asserting is at the end of 
        /// the function.  Specify EpcsFlags.SkipErase (0x0002) to 
        /// prevent the bulk erase step prior to writing the device.</param>
        /// <param name="callback">(Optional) A callback function to call 
        /// during the verify process to report the percentage completed.  
        /// This may be used to perform background processes and GUI updates 
        /// while the verify process takes place.</param>
        /// <param name="tag">(Optional) A pointer to user data that is 
        /// passed onto the callback function.  Specify IntPtr.Zero to 
        /// ignore.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ConfigureEpcs(byte nSS, string filePath, EpcsFlags flags, ProgressDelegate callback, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbConfigureEpcs(m_hDevice, nSS, filePath, (uint)flags, callback, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Verify that the data contained in the EPCS device matches that of 
        /// the specified FPGA data file
        /// </summary>
        /// <param name="nSS">The nSS SPI line that the EPCS device is 
        /// connected to (0-9)</param>
        /// <param name="filePath">The path and file name of the FPGA data file</param>
        /// <param name="flags">Additional flags.  Specify 
        /// EpcsFlags.IgnoreNCE (0x0001) to prevent the function from 
        /// automatically asserting the nCE line of the FPGA at the 
        /// beginning of the function are re-asserting is at the end of 
        /// the function.</param>
        /// <param name="callback">(Optional) A callback function to call 
        /// during the verify process to report the percentage completed.  
        /// This may be used to perform background processes and GUI updates 
        /// while the verify process takes place.</param>
        /// <param name="tag">(Optional) A pointer to user data that is 
        /// passed onto the callback function.  Specify IntPtr.Zero to 
        /// ignore.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool VerifyEpcs(byte nSS, string filePath, EpcsFlags flags, ProgressDelegate callback, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbVerifyEpcs(m_hDevice, nSS, filePath, (uint)flags, callback, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Perform the bulk erase action of the EPCS device to erase the
        /// entire device to 0xFF
        /// </summary>
        /// <param name="nSS">The nSS SPI line that the EPCS device is 
        /// connected to (0-9)</param>
        /// <param name="flags">Additional flags controlling the operation of 
        /// this function.  Specify EpcsFlags.IgnoreNCE (0x0001) to 
        /// prevent the function from automatically asserting the nCE line of 
        /// the FPGA at the beginning of the function are re-asserting is at 
        /// the end of the function.</param>
        /// <param name="callback">(Optional) A callback function to call 
        /// during the verify process to report the percentage completed.  
        /// This may be used to perform background processes and GUI updates 
        /// while the verify process takes place.</param>
        /// <param name="tag">(Optional) A pointer to user data that is 
        /// passed onto the callback function.  Specify IntPtr.Zero to 
        /// ignore.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool EraseEpcs(byte nSS, EpcsFlags flags, ProgressDelegate callback, IntPtr tag)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbEraseEpcs(m_hDevice, nSS, (uint)flags, callback, tag) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Reset a given statistic.  Reseting a statistic resets any associated
        /// byte counts and time indicaters used by the statistic.
        /// </summary>
        /// <param name="statistic">The statistic to reset.  Specify Stat.All 
        /// to reset all statistics.  You should reset all statistics before 
        /// first attempting to read and statistics from QuickUSB.</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool ResetStatistic(Stat statistic)
        {
            if (!OpenPolitely())
            {
                return false;
            }

            if (NativeMethods.QuickUsbResetStatistic(m_hDevice, statistic) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        /// <summary>
        /// Retreive a statistic in the specified unit
        /// </summary>
        /// <param name="statistic">The statistic to retrieve</param>
        /// <param name="unit">The unit to report the statistic in</param>
        /// <param name="value">The value of the reported statistic</param>
        /// <param name="flags">Additional flags controlling the operation of 
        /// this function</param>
        /// <returns>True on success or false on failure.  Get extended error
        /// information with QuickUsb.LastError().</returns>
        public bool GetStatistic(Stat statistic, StatUnit unit, out float value, uint flags)
        {
            if (!OpenPolitely())
            {
                value = 0.0f;
                return false;
            }

            if (NativeMethods.QuickUsbGetStatistic(m_hDevice, statistic, unit, out value, flags) == 0)
            {
                m_lastError = GetLastError();
                m_lastDriverError = GetLastDriverError();
                ClosePolitely();
                return false;
            }

            ClosePolitely();
            return true;
        }

        //---------------------------------------------------------------------
        #endregion
        //---------------------------------------------------------------------
	}
}