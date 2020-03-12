import QuickUsb

WORDWIDE = 0

q = QuickUsb.QuickUsb("QUSB-0")
q.Open()


# QuickUsb Base API
print 'QuickUsb Base API'

print '\tGetStringDescriptor - Working'
r = q.GetStringDescriptor(1)
print '\t\tMake:',r
r = q.GetStringDescriptor(2)
print '\t\tModel:',r
r = q.GetStringDescriptor(3)
print '\t\tSerial:',r

print '\tGetDriverVersion - Working'
r = q.GetDriverVersion()
print '\t\tDriver version:',r

print '\tGetDllVersion - Working'
r = q.GetDllVersion()
print '\t\tDll version:',r

print '\tGetFirmwareVersion - Working'
r = q.GetFirmwareVersion()
print '\t\tFirmware version:',r

print '\tGetLastError - Driver returns invalid values'
#r = q.GetLastError()
#print '\t\tLast error:',r


# QuickUsb Settings
print 'QuickUsb Settings'

print '\tReadSetting - Working'
fifo_config = q.ReadSetting(QuickUsb.Setting.FifoConfig)
print '\t\tFifo configuration:',fifo_config
#data_address = q.ReadSetting(QuickUsb.Setting.DataAddress)
#print '\t\tData address:',data_address

print '\tWriteSetting - Working'
#t = fifo_config^0x2
#q.WriteSetting(QuickUsb.Setting.FifoConfig,t)
#fifo_config = q.ReadSetting(QuickUsb.Setting.FifoConfig)
#print '\t\tFifo configuration:',fifo_config
#q.WriteSetting(QuickUsb.Setting.WordWide,WORDWIDE)
#word_wide = q.ReadSetting(QuickUsb.Setting.WordWide)
#print '\t\tWord wide:',word_wide

print '\tReadDefault - Not tested'
#r = q.ReadDefault(QuickUsb.Setting.FifoConfig)
#print '\t\tDefault fifo configuration:',r

print '\tWriteDefault - Not tested'
#t = r+1
#q.WriteDefault(QuickUsb.Setting.FifoConfig,t)
#r = q.ReadDefault(QuickUsb.Setting.FifoConfig)
#print '\t\tDefault fifo configuration:',r
#t = r-1
#q.WriteDefault(QuickUsb.Setting.FifoConfig,t)
#r = q.ReadDefault(QuickUsb.Setting.FifoConfig)
#print '\t\tDefault fifo configuration:',r

"""
# FPGA Configuration
print 'FPGA Configuration'

print '\tStartFPGAConfiguration - Not fully tested'
t = [1<<7]
q.WritePortDir(0,t[0]) # Set port A bit 7 as output
r = q.ReadPortDir(0)
print '\t\tPort direction:',r # 1<<7 = 128
q.WritePort(0,t,1) # Set port A bit 7 to high to turn on FPGA power
r = q.ReadPort(0,1)
print '\t\tPort A:',r
q.StartFpgaConfiguration()

print '\tWriteFPGAData - Not fully tested'
fpgafile = open('QUSBEVB_REVA_EP2C20_Memory_Demo.rbf','rb')
fpgadata = fpgafile.readlines()
q.WriteFpgaData(fpgadata)
fpgafile.close()

print '\tIsFPGAConfigured - Not fully tested'
r = q.IsFpgaConfigured()
print '\t\tConfigured:',r"""

# High-Speed Parallel Port
print 'High-Speed Parallel Port'

print '\tReadCommand - Working'
#r = q.ReadCommand(0,2)
#print '\t\tCommand:',r

print '\tWriteCommand - Not tested'
#r = q.WriteCommand(0,0xFF,1)
#print '\t\tReturn:',r
#r = q.ReadCommand(0,1)
#print '\t\tCommand:',r

print '\tReadData - Not tested'
#r = q.ReadData(0,1)
#print '\t\tData:',r

print '\tWriteData - Working'
#data = []
#for i in range(0,2048):
#	data.append(i%256)
#	data.append(0xFF-i%256)
#q.WriteData(data,4096)
#r = q.ReadData(2)
#print '\t\tData:',r

print '\tReadDataAsync - Not tested'
#r = q.ReadDataAsync(0,1)
#print '\t\tData:',r

print '\tWriteDataAsync - Not tested'
#r = q.WriteDataAsync(0,0xFF,1)
#print '\t\tReturn:',r
#r = q.ReadDataAsync(0,1)
#print '\t\tData:',r


# General-Purpose I/O
print 'General-Purpose I/O'

print '\tReadPortDir - Working'
#r = q.ReadPortDir(0)
#print '\t\tPort direction:',r

print '\tWritePortDir - Working'
#if ( r == 0 ):
#	t = 1;
#if ( r == 1 ):
#	t = 0;
#q.WritePortDir(0, t)
#r = q.ReadPortDir(0)
#print '\t\tPort direction:',r

print '\tReadPort - Working'
#q.WritePortDir(0,0x00)
#r = q.ReadPort(0,1)
#print '\t\tPort data:',r

print '\tWritePort - Working'
#q.WritePortDir(1,0xFF)
#t = [0x61]
#q.WritePort(1,t,1)


# RS-232
print 'RS-232'

print '\tSetRS232BaudRate(4800) - Not fully tested'
#r = q.SetRS232BaudRate(4800)
#print '\t\tReturn:',r

print '\tGetNumRS232(0) - Not fully tested'
#r = q.GetNumRS232(0)
#print '\t\tNumber of characters:',r

print '\tFlushRS232(0) - Not fully tested'
#r = q.FlushRS232(0)
#print '\t\tReturn:',r

print '\tReadRS232(0) - Not tested'
#r = q.ReadRS232(0,1)
#print '\t\tData:',r

print '\tWriteRS232(0) - Not tested'
#r = q.WriteRS232(0,0xFF,1)
#print '\t\tReturn',r
#r = q.ReadRS232(0,1)
#print '\t\tData:'r


# I2C-Compatible Port
print 'I2C-Compatible Port'

print '\tReadI2C - Not fully tested'
#r = q.ReadI2C(0,1)
#print '\t\tData:',r

print '\tWriteI2C - Working'
#i2ctl = q.ReadSetting(QuickUsb.Setting.I2Ctl)
#print '\t\tI2Ctl:',i2ctl
#i2ctl = i2ctl|1<<7
#q.WriteSetting(QuickUsb.Setting.I2Ctl,i2ctl)
#i2ctl = q.ReadSetting(QuickUsb.Setting.I2Ctl)
#print '\t\tI2Ctl:',i2ctl
#t = [0x49,0x32,0x43] # I2c
#q.WriteI2C(0,t,3)


# SPI-Compatible Port
print 'SPI-Compatible Port'

print '\tReadSpi - Not fully tested'
#r = q.ReadSpi(0,1)
#print '\t\tData:',r

print '\tWriteSpi - Working'
#t = [0x53,0x50,0x49] # SPI
#q.WriteSpi(0,t,3)

print '\tWriteReadSpi - Not fully tested (read)'
#t = [0x53,0x50,0x49] # SPI
#q.WriteReadSpi(0,t,3)


# QuickUsb Storage
print 'QuickUsb Storage'

print '\tReadStorage - Not tested'
#r = q.ReadStorage(0,1)
#print '\t\tData:',r

print '\tWriteStorage - Not tested'
#r = q.WriteStorage(0,0xFF,1)
#print '\t\tReturn:',r
#r = q.ReadStorage(0,1)
#print '\t\tData:',r

q.Close()
