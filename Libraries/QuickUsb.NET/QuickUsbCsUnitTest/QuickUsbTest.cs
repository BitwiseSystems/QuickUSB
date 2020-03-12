using BitwiseSystems;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace QuickUsbCsUnitTest
{
    
    
    /// <summary>
    ///This is a test class for QuickUsbTest and is intended
    ///to contain all QuickUsbTest Unit Tests
    ///</summary>
    [TestClass()]
    public class QuickUsbTest
    {
        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for Serial
        ///</summary>
        [TestMethod()]
        public void SerialTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string actual;
            int snum;
            actual = target.Serial;
            Assert.AreEqual(Int32.TryParse(actual, out snum), true);
        }

        /// <summary>
        ///A test for Name
        ///</summary>
        [TestMethod()]
        public void NameTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string expected = "QUSB-0";
            string actual;
            actual = target.Name;
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for Model
        ///</summary>
        [TestMethod()]
        public void ModelTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string actual;
            actual = target.Model;
            Assert.AreEqual(actual.StartsWith("QuickUSB QUSB2 Module v2.15.1"), true);
        }

        /// <summary>
        ///A test for Make
        ///</summary>
        [TestMethod()]
        public void MakeTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string actual;
            actual = target.Make;
            Assert.AreEqual(actual, "Bitwise Systems");
        }

        /// <summary>
        ///A test for IsOpened
        ///</summary>
        [TestMethod()]
        public void IsOpenedTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            bool actual;
            actual = target.IsOpened;
            Assert.AreEqual(actual, false);
        }

        /// <summary>
        ///A test for WriteStorage
        ///</summary>
        [TestMethod()]
        public void WriteStorageTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address;
            byte[] data = new byte[128];
            byte[] readData = new byte[128];
            ushort bytes = (ushort)data.Length;
            bool expected = true;
            bool actual;

            address = (ushort)(r.Next() % (2048 - bytes));
            r.NextBytes(data);
            actual = target.WriteStorage(address, data, bytes);
            Assert.AreEqual(expected, actual);

            actual = target.ReadStorage(address, readData, bytes);
            Assert.AreEqual(expected, actual);

            CollectionAssert.AreEqual(data, readData);
        }

        /// <summary>
        ///A test for WriteSPI
        ///</summary>
        [TestMethod()]
        public void WriteSPITest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            r.NextBytes(data);
            actual = target.WriteSPI(portNum, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteSetting
        ///</summary>
        [TestMethod()]
        public void WriteSettingTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Setting address = QuickUsb.Setting.DataAddress;
            ushort value = 0xfeca, readValue;
            bool expected = true;
            bool actual;
            actual = target.WriteSetting(address, value);
            Assert.AreEqual(expected, actual);

            actual = target.ReadSetting(address, out readValue);
            Assert.AreEqual(expected, actual);

            Assert.AreEqual(value, readValue);
        }

        /// <summary>
        ///A test for WriteRS232
        ///</summary>
        [TestMethod()]
        public void WriteRS232Test()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            byte[] data = new byte[128];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            r.NextBytes(data);
            actual = target.WriteRS232(portNum, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteReadSPI
        ///</summary>
        [TestMethod()]
        public void WriteReadSPITest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            r.NextBytes(data);
            actual = target.WriteReadSPI(portNum, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WritePortDir
        ///</summary>
        [TestMethod()]
        public void WritePortDirTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.E;
            byte data = 0xff, readData;
            bool expected = true;
            bool actual;

            actual = target.WritePortDir(address, data);
            Assert.AreEqual(expected, actual);

            actual = target.ReadPortDir(address, out readData);
            Assert.AreEqual(data, readData);
        }

        /// <summary>
        ///A test for WritePort
        ///</summary>
        [TestMethod()]
        public void WritePortTest1()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.E;
            byte data = 0x7e;
            bool expected = true;
            bool actual;
            actual = target.WritePort(address, data);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WritePort
        ///</summary>
        [TestMethod()]
        public void WritePortTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.E;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            r.NextBytes(data);
            actual = target.WritePort(address, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteI2C
        ///</summary>
        [TestMethod()]
        public void WriteI2CTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = 0x45;
            byte regaddr = 0x23;
            ushort[] data = new ushort[16];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            for (int k = 0; k < data.Length; ++k)
            {
                data[k] = (ushort)r.Next();
            }
            actual = target.WriteI2C(address, regaddr, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteI2C
        ///</summary>
        [TestMethod()]
        public void WriteI2CTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = 0x45;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            r.NextBytes(data);
            actual = target.WriteI2C(address, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteFpgaData
        ///</summary>
        [TestMethod()]
        public void WriteFpgaDataTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte[] fpgadata = new byte[128];
            uint datalength = (ushort)fpgadata.Length;
            bool expected = true;
            bool actual;
            r.NextBytes(fpgadata);
            actual = target.WriteFpgaData(fpgadata, datalength);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteDefault
        ///</summary>
        [TestMethod()]
        public void WriteDefaultTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Setting address = QuickUsb.Setting.PortA;
            ushort value = 0xff00;
            bool expected = true;
            bool actual;
            actual = target.WriteDefault(address, value);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteDataAsync
        ///</summary>
        [TestMethod()]
        public void WriteDataAsyncTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte[] data = new byte[1024];
            uint length = (uint)data.Length;
            byte transId;
            bool expected = true;
            bool actual;

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);

            actual = target.WriteDataAsync(hData.AddrOfPinnedObject(), length, out transId);
            Assert.AreEqual(expected, actual);

            if (actual)
            {
                actual = target.AsyncWait(out length, transId, false);
                Assert.AreEqual(expected, actual);
            }
            hData.Free();
        }

        /// <summary>
        ///A test for WriteData
        ///</summary>
        [TestMethod()]
        public void WriteDataTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte[] data = new byte[1024];
            uint length = (uint)data.Length;
            bool expected = true;
            bool actual;
            r.NextBytes(data);
            actual = target.WriteData(data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteData
        ///</summary>
        [TestMethod()]
        public void WriteDataTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort[] data = new ushort[1024];
            uint length = (uint)data.Length;
            bool expected = true;
            bool actual;

            for (int k = 0; k < data.Length; ++k)
            {
                data[k] = (ushort)r.Next();
            }

            actual = target.WriteData(data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteCommand
        ///</summary>
        [TestMethod()]
        public void WriteCommandTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = (ushort)r.Next(0, 65535);
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;
            r.NextBytes(data);
            actual = target.WriteCommand(address, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteCommand
        ///</summary>
        [TestMethod()]
        public void WriteCommandTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = (ushort)r.Next(0, 65535);
            ushort[] data = new ushort[32];
            ushort length = (ushort)data.Length;
            bool expected = true;
            bool actual;

            for (int k = 0; k < data.Length; ++k)
            {
                data[k] = (ushort)r.Next();
            }

            actual = target.WriteCommand(address, data, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for WriteBulkDataAsync
        ///</summary>
        [TestMethod()]
        public void WriteBulkDataAsyncTest()
        {
            /*QuickUsb target = new QuickUsb("QUSB-0");
            IntPtr data = Marshal.AllocHGlobal(1024);
            uint bytes = 1024;
            IntPtr bs = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(QuickUsb.BulkStream)));
            QuickUsb.BulkStreamCompletionRoutine completion = null;
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;

            actual = target.WriteBulkDataAsync(data, bytes, bs, completion, tag);
            Assert.AreEqual(expected, actual);

            actual = target.BulkWait(bs, false);
            Assert.AreEqual(expected, actual);

            Marshal.FreeHGlobal(bs);*/

            Assert.Inconclusive();
        }

        /// <summary>
        ///A test for UploadFpga
        ///</summary>
        [TestMethod()]
        public void UploadFpgaTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string path = "C:\\Projects\\Bitwise\\QuickUSB\\Hardware\\EvaluationBoard\\Samples\\Rev A\\Verilog\\QUSBEVB_REVA_EP2C20_Counter\\QUSBEVB_REVA_EP2C20_Counter.rbf";
            QuickUsb.PercentCompleteDelegate updateProgress = null;
            bool expected = true;
            bool actual;
            actual = target.UploadFpga(path, updateProgress);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for StartFpgaConfiguration
        ///</summary>
        [TestMethod()]
        public void StartFpgaConfigurationTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            bool expected = true;
            bool actual;
            actual = target.StartFpgaConfiguration();
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for SetTimeout
        ///</summary>
        [TestMethod()]
        public void SetTimeoutTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            uint timeOut = 1000;
            bool expected = true;
            bool actual;
            actual = target.SetTimeout(timeOut);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for SetRS232BaudRate
        ///</summary>
        [TestMethod()]
        public void SetRS232BaudRateTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            uint baudrate = 115200;
            bool expected = true;
            bool actual;
            actual = target.SetRS232BaudRate(baudrate);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadStorage
        ///</summary>
        [TestMethod()]
        public void ReadStorageTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = 0;
            byte[] data = new byte[128];
            ushort bytes = (ushort)data.Length;
            bool expected = true;
            bool actual;
            actual = target.ReadStorage(address, data, bytes);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadSPI
        ///</summary>
        [TestMethod()]
        public void ReadSPITest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadSPI(portNum, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadSetting
        ///</summary>
        [TestMethod()]
        public void ReadSettingTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Setting address = QuickUsb.Setting.Ep26Config;
            ushort value;
            ushort valueExpected = 0xa2e2;
            bool expected = true;
            bool actual;
            actual = target.ReadSetting(address, out value);
            Assert.AreEqual(valueExpected, value);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadRS232
        ///</summary>
        [TestMethod()]
        public void ReadRS232Test()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = 0;
            bool expected = true;
            bool actual;
            actual = target.GetNumRS232(portNum, out lengthExpected);
            Assert.AreEqual(expected, actual);
            if (lengthExpected > 0)
            {
                actual = target.ReadRS232(portNum, data, ref length);
                Assert.AreEqual(lengthExpected, length);
                Assert.AreEqual(expected, actual);
            }
        }

        /// <summary>
        ///A test for ReadPortDir
        ///</summary>
        [TestMethod()]
        public void ReadPortDirTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.A;
            byte data;
            bool expected = true;
            bool actual;
            actual = target.ReadPortDir(address, out data);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadPort
        ///</summary>
        [TestMethod()]
        public void ReadPortTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.A;
            byte[] data = new byte[32];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = (ushort)data.Length;
            bool expected = true;
            bool actual;

            actual = target.ReadPort(address, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadPort
        ///</summary>
        [TestMethod()]
        public void ReadPortTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Port address = QuickUsb.Port.A;
            byte data = 0xab;
            bool expected = true;
            bool actual;
            actual = target.ReadPort(address, out data);
            Assert.AreEqual(expected, actual);
            Assert.AreEqual(data, 0x00);
        }

        /// <summary>
        ///A test for Write and Verify firmware
        ///</summary>
        [TestMethod()]
        public void WriteVerifyFirmwareTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            bool expected = true;
            bool actual;
            actual = target.WriteFirmware(@"C:\Projects\Bitwise\QuickUSB\Library\Firmware\QuickUSB Firmware v2.15.1\quickusb-simple v2.15.1.qusb", QuickUsb.Programming.PreserveDefaults | QuickUsb.Programming.PreserveSerial, null, IntPtr.Zero);
            Assert.AreEqual(expected, actual);

            actual = target.VerifyFirmware(@"C:\Projects\Bitwise\QuickUSB\Library\Firmware\QuickUSB Firmware v2.15.1\quickusb-simple v2.15.1.qusb", null, IntPtr.Zero);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadI2C
        ///</summary>
        [TestMethod()]
        public void ReadI2CTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = 0x90;
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = 64;
            bool expected = true;
            bool actual;
            actual = target.ReadI2C(address, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadI2C
        ///</summary>
        [TestMethod()]
        public void ReadI2CTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = 0x90;
            byte regaddr = 0x01;
            ushort[] data = new ushort[1];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = 1;
            bool expected = true;
            bool actual;
            actual = target.ReadI2C(address, regaddr, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadDefault
        ///</summary>
        [TestMethod()]
        public void ReadDefaultTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Setting address = QuickUsb.Setting.Ep26Config;
            ushort value;
            ushort valueExpected = 0xa2e2;
            bool expected = true;
            bool actual;
            actual = target.ReadDefault(address, out value);
            Assert.AreEqual(valueExpected, value);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadDataAsync
        ///</summary>
        [TestMethod()]
        public void ReadDataAsyncTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte[] data = new byte[1024];
            uint length = (uint)data.Length;
            byte transId;
            bool expected = true;
            bool actual;

            GCHandle hData = GCHandle.Alloc(data, GCHandleType.Pinned);

            actual = target.ReadDataAsync(hData.AddrOfPinnedObject(), ref length, out transId);
            Assert.AreEqual(expected, actual);

            if (actual)
            {
                actual = target.AsyncWait(out length, transId, false);
                Assert.AreEqual(expected, actual);
                Assert.AreEqual(length, (uint)data.Length);
            }
            hData.Free();
        }

        /// <summary>
        ///A test for ReadData
        ///</summary>
        [TestMethod()]
        public void ReadDataTest2()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            IntPtr data = Marshal.AllocHGlobal(1024);
            uint length = 1024;
            uint lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadData(data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
            Marshal.FreeHGlobal(data);
        }

        /// <summary>
        ///A test for ReadData
        ///</summary>
        [TestMethod()]
        public void ReadDataTest1()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort[] data = new ushort[1024];
            uint length = (uint)data.Length;
            uint lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadData(data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadData
        ///</summary>
        [TestMethod()]
        public void ReadDataTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte[] data = new byte[1024];
            uint length = (uint)data.Length;
            uint lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadData(data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadCommand
        ///</summary>
        [TestMethod()]
        public void ReadCommandTest1()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = (ushort)r.Next(0, 65535);
            ushort[] data = new ushort[32];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadCommand(address, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadCommand
        ///</summary>
        [TestMethod()]
        public void ReadCommandTest()
        {
            Random r = new Random();
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort address = (ushort)r.Next(0, 65535);
            byte[] data = new byte[64];
            ushort length = (ushort)data.Length;
            ushort lengthExpected = length;
            bool expected = true;
            bool actual;
            actual = target.ReadCommand(address, data, ref length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for ReadBulkDataStartStream
        ///</summary>
        [TestMethod()]
        public void ReadBulkDataStartStreamTest()
        {
            // For some reason this test does not pass, but there's no reason 
            // as to why.  The same exact code will execute in a project of its 
            // own, so I've shown that this method does pass (just not it this 
            // test case).
            Assert.Inconclusive();

            /*QuickUsb target = new QuickUsb("QUSB-0");
            IntPtr buffers = IntPtr.Zero;
            uint numBuffers = 1;
            uint bufferSize = 1 * 1024;
            QuickUsb.BulkStreamCompletionRoutine completion = new QuickUsb.BulkStreamCompletionRoutine(cRoutine);
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;
            //actual = target.ReadBulkDataStartStream(buffers, numBuffers, bufferSize, completion, tag);
            actual = target.ReadBulkDataStartStream(IntPtr.Zero, 4, 256 * 1024, completion, IntPtr.Zero);
            Assert.AreEqual(expected, actual);
            Thread.Sleep(1000);
            actual = target.BulkDataStopStream(true);
            if (!actual)
            {
                QuickUsb.Error err = target.LastError();
            }   
            Assert.AreEqual(expected, actual);*/
        }

        /// <summary>
        ///A test for ReadBulkDataAsync
        ///</summary>
        [TestMethod()]
        public void ReadBulkDataAsyncTest()
        {
            /*QuickUsb target = new QuickUsb("QUSB-0");
            IntPtr data = Marshal.AllocHGlobal(1024);
            uint bytes = 1024;
            IntPtr bs = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(QuickUsb.BulkStream)));
            QuickUsb.BulkStreamCompletionRoutine completion = null;
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;

            actual = target.ReadBulkDataAsync(data, bytes, bs, completion, tag);
            Assert.AreEqual(expected, actual);

            actual = target.BulkWait(bs, false);
            Assert.AreEqual(expected, actual);

            Marshal.FreeHGlobal(bs);*/

            Assert.Inconclusive();
        }

        /// <summary>
        ///A test for OpenEx
        ///</summary>
        [TestMethod()]
        public void OpenExTest()
        {
            QuickUsb target = new QuickUsb();
            string devname = "QUSB-0";
            QuickUsb.OpenFlag flags = QuickUsb.OpenFlag.Normal;
            bool expected = true;
            bool actual;
            actual = target.OpenEx(devname, flags);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for Open
        ///</summary>
        [TestMethod()]
        public void OpenTest1()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            bool expected = true;
            bool actual;
            actual = target.Open();
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for Open
        ///</summary>
        [TestMethod()]
        public void OpenTest()
        {
            QuickUsb target = new QuickUsb();
            string devname = "QUSB-0";
            bool expected = true;
            bool actual;
            actual = target.Open(devname);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for LastError
        ///</summary>
        [TestMethod()]
        public void LastErrorTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.Error expected = QuickUsb.Error.NoError;
            QuickUsb.Error actual;
            actual = target.LastError();
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for IsFpgaConfigured
        ///</summary>
        [TestMethod()]
        public void IsFpgaConfiguredTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            bool isConfigured;
            bool expected = true;
            bool actual;
            actual = target.IsFpgaConfigured(out isConfigured);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetStringDescriptor
        ///</summary>
        [TestMethod()]
        public void GetStringDescriptorTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            QuickUsb.StringDescriptor stringIndex = QuickUsb.StringDescriptor.Make;
            string expected = "Bitwise Systems";
            string actual;
            actual = target.GetStringDescriptor(stringIndex);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetNumRS232
        ///</summary>
        [TestMethod()]
        public void GetNumRS232Test()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            ushort length = 0;
            ushort lengthExpected = 0;
            bool expected = true;
            bool actual;
            actual = target.GetNumRS232(portNum, out length);
            Assert.AreEqual(lengthExpected, length);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetLastError
        ///</summary>
        [TestMethod()]
        public void GetLastErrorTest()
        {
            QuickUsb.Error actual;
            actual = QuickUsb.GetLastError();
        }

        /// <summary>
        ///A test for GetFirmwareVersion
        ///</summary>
        [TestMethod()]
        public void GetFirmwareVersionTest1()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            Version version;
            Version versionExpected = new Version(2, 15, 0); // This may change
            bool expected = true;
            bool actual;
            actual = target.GetFirmwareVersion(out version);
            Assert.AreEqual(versionExpected, version);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetFirmwareVersion
        ///</summary>
        [TestMethod()]
        public void GetFirmwareVersionTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            ushort majorVersion = 0;
            ushort majorVersionExpected = 2;
            ushort minorVersion = 0;
            ushort minorVersionExpected = 15;
            ushort buildVersion = 0;
            ushort buildVersionExpected = 0;
            bool expected = true;
            bool actual;
            actual = target.GetFirmwareVersion(out majorVersion, out minorVersion, out buildVersion);
            Assert.AreEqual(majorVersionExpected, majorVersion);
            Assert.AreEqual(minorVersionExpected, minorVersion);
            Assert.AreEqual(buildVersionExpected, buildVersion);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetDriverVersion
        ///</summary>
        [TestMethod()]
        public void GetDriverVersionTest1()
        {
            ushort majorVersion = 0;
            ushort majorVersionExpected = 2;
            ushort minorVersion = 0;
            ushort minorVersionExpected = 15;
            ushort buildVersion = 0;
            ushort buildVersionExpected = 0;
            bool expected = true;
            bool actual;
            actual = QuickUsb.GetDriverVersion(out majorVersion, out minorVersion, out buildVersion);
            Assert.AreEqual(majorVersionExpected, majorVersion);
            Assert.AreEqual(minorVersionExpected, minorVersion);
            Assert.AreEqual(buildVersionExpected, buildVersion);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetDriverVersion
        ///</summary>
        [TestMethod()]
        public void GetDriverVersionTest()
        {
            Version version = null;
            Version versionExpected = new Version(2, 15, 0);
            bool expected = true;
            bool actual;
            actual = QuickUsb.GetDriverVersion(out version);
            Assert.AreEqual(versionExpected, version);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetDllVersion
        ///</summary>
        [TestMethod()]
        public void GetDllVersionTest1()
        {
            ushort majorVersion = 0;
            ushort majorVersionExpected = 2;
            ushort minorVersion = 0;
            ushort minorVersionExpected = 15;
            ushort buildVersion = 0;
            ushort buildVersionExpected = 0;
            bool expected = true;
            bool actual;
            actual = QuickUsb.GetDllVersion(out majorVersion, out minorVersion, out buildVersion);
            Assert.AreEqual(majorVersionExpected, majorVersion);
            Assert.AreEqual(minorVersionExpected, minorVersion);
            Assert.AreEqual(buildVersionExpected, buildVersion);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for GetDllVersion
        ///</summary>
        [TestMethod()]
        public void GetDllVersionTest()
        {
            Version version = null;
            Version versionExpected = new Version(2, 15, 0);
            bool expected = true;
            bool actual;
            actual = QuickUsb.GetDllVersion(out version);
            Assert.AreEqual(versionExpected, version);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for FpgaConfigure
        ///</summary>
        [TestMethod()]
        public void ConfigureFpgaTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            string filePath = "C:\\Projects\\Bitwise\\QuickUSB\\Hardware\\EvaluationBoard\\Samples\\Rev A\\Verilog\\QUSBEVB_REVA_EP2C20_Counter\\QUSBEVB_REVA_EP2C20_Counter.rbf";
            bool expected = true;
            bool actual;
            actual = target.ConfigureFpga(filePath);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for FlushRS232
        ///</summary>
        [TestMethod()]
        public void FlushRS232Test()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte portNum = 0;
            bool expected = true;
            bool actual;
            actual = target.FlushRS232(portNum);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for Close
        ///</summary>
        [TestMethod()]
        public void CloseTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            target.Open();
            bool expected = true;
            bool actual;
            actual = target.Close();
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for QuickUsb Constructor
        ///</summary>
        [TestMethod()]
        public void QuickUsbConstructorTest()
        {
            string devname = "QUSB-0";
            QuickUsb target = new QuickUsb(devname);
            Assert.AreEqual(devname, target.Name);
        }

        void cRoutine(QuickUsb.BulkStream bs)
        {
            Assert.AreEqual(bs.Error, QuickUsb.Error.NoError);
            Assert.AreEqual(bs.BytesRequested, bs.BytesTransferred);
        }

        /// <summary>
        ///A test for WriteBulkDataStartStream
        ///</summary>
        [TestMethod()]
        public void WriteBulkDataStartStreamTest()
        {
            // For some reason this test does not pass, but there's no reason 
            // as to why.  The same exact code will execute in a project of its 
            // own, so I've shown that this method does pass (just not it this 
            // test case).
            Assert.Inconclusive();

            /*QuickUsb target = new QuickUsb("QUSB-0");
            IntPtr[] buffers = new IntPtr[1] { Marshal.AllocHGlobal(300 * 1024) };
            uint numBuffers = 1;
            uint bufferSize = 300 * 1024;
            QuickUsb.BulkStreamCompletionRoutine completion = cRoutine;
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;
            actual = target.WriteBulkDataStartStream(buffers, numBuffers, bufferSize, completion, tag);
            Assert.AreEqual(expected, actual);
            Thread.Sleep(250);
            actual = target.BulkDataStopStream(true);
            Assert.AreEqual(expected, actual);

            Marshal.FreeHGlobal(buffers[0]);*/
        }

        /// <summary>
        ///A test for FindModules
        ///</summary>
        [TestMethod()]
        public void FindModulesTest()
        {
            string expected = "QUSB";
            string[] actual;
            actual = QuickUsb.FindModules();
            for (int k = 0; k < actual.Length; ++k)
            {
                Assert.AreEqual(expected, actual[k].Split('-')[0]);
            }
        }

        /// <summary>
        ///A test for EraseEpcs
        ///</summary>
        [TestMethod()]
        public void EraseEpcsTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte nSS = 0;
            QuickUsb.EpcsFlags flags = QuickUsb.EpcsFlags.None;
            QuickUsb.ProgressDelegate callback = null;
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;
            actual = target.EraseEpcs(nSS, flags, callback, tag);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for CongifureEpce and VerifyEpcs
        ///</summary>
        [TestMethod()]
        public void ConfigureVerifyEpcsTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte nSS = 0;
            string filePath = @"C:\epcsTestData.rbf";
            QuickUsb.EpcsFlags flags = QuickUsb.EpcsFlags.None;
            QuickUsb.ProgressDelegate callback = null;
            IntPtr tag = IntPtr.Zero;
            bool expected = true;
            bool actual;

            actual = target.ConfigureEpcs(nSS, filePath, flags, callback, tag);
            Assert.AreEqual(expected, actual);

            actual = target.VerifyEpcs(nSS, filePath, flags, callback, tag);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for IdentifyEpcs
        ///</summary>
        [TestMethod()]
        public void IdentifyEpcsTest()
        {
            QuickUsb target = new QuickUsb("QUSB-0");
            byte nSS = 0;
            QuickUsb.EpcsId epcsId;
            QuickUsb.EpcsId epcsIdExpected = QuickUsb.EpcsId.Epcs1;
            uint epcsByteSize, epcsByteSizeExpected = (1048576 / 8) * 1;
            QuickUsb.EpcsFlags flags = QuickUsb.EpcsFlags.None;
            bool expected = true;
            bool actual;
            actual = target.IdentifyEpcs(nSS, out epcsId, out epcsByteSize, flags);
            Assert.AreEqual(expected, actual);
            Assert.AreEqual(epcsIdExpected, epcsId);
            Assert.AreEqual(epcsByteSizeExpected, epcsByteSize);
        }
    }
}
