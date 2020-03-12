import QuickUsb
import time
#import sys
#if len(sys.argv) > 1 and sys.argv[1] == 'write':


q = QuickUsb.QuickUsb("QUSB-0")
q.Open()


print 'Loop back test'

# Check if FPGA is configured
print '\tIsFPGAConfigured - Working'
(ok, r) = q.IsFpgaConfigured()
print '\t\tConfigured:',r

if r == 0:
	# Set port and FPGA settings
	print '\tStartFPGAConfiguration - Working'
	q.WriteSetting(QuickUsb.Setting.PortA,0x8080) # Set port A bit 7 to output high to turn on FPGA power
	(ok, r) = q.ReadSetting(QuickUsb.Setting.PortA)
	print '\t\tPort A setting:',hex(r)
	mask = 0xFE
	(ok, fpga_type) = q.ReadSetting(QuickUsb.Setting.FpgaType)
	q.WriteSetting(QuickUsb.Setting.FpgaType,fpga_type&mask) # Set FPGA type to Altera Passive Serial
	(ok, fpga_type) = q.ReadSetting(QuickUsb.Setting.FpgaType)
	print '\t\tFpga type:',fpga_type
	
	# Wait for the FPGA to power up
	time.sleep(0.5)
	
	# Start FPGA configuration
	q.StartFpgaConfiguration()

	# Write data to FPGA
	print '\tWriteFPGAData - Working'
	fpgafile = open('QUSBEVB_REVA_EP2C20_FIFO.rbf','rb')
	fpgadata = fpgafile.read()
	q.WriteFpgaData([ord(x) for x in fpgadata], len(fpgadata))
	fpgafile.close()

	# Check if FPGA is configured correctly
	print '\tIsFPGAConfigured - Working'
	(ok, r) = q.IsFpgaConfigured()
	print '\t\tConfigured:',r
		
if r == 1:
	# Reset FPGA logic
	print '\tReset FPGA logic'
	q.WriteSetting(QuickUsb.Setting.PortA,0x8180) # Set port A bit 0 direction as output and value as 0
	q.WriteSetting(QuickUsb.Setting.PortA,0x8181) # Set port A bit 0 value as 1
	(ok, r) = q.ReadSetting(QuickUsb.Setting.PortA)
	print '\t\tPort A setting:',hex(r)

	# Set loop back mode
	print '\tSet loop back mode'
	data = QuickUsb.CreateByteBuffer(1)
	data[0] = 0x00
	q.WriteCommand(0,data,1)
	(ok, r) = q.ReadCommand(0,data,1)
	print '\t\tCommand:',r

	# Set fifo configuration and word wide
	print '\tWriteSetting - Working'
	(ok, fifo_config) = q.ReadSetting(QuickUsb.Setting.FifoConfig)
	t = fifo_config&0xFFFE
	q.WriteSetting(QuickUsb.Setting.FifoConfig,t)
	fifo_config = q.ReadSetting(QuickUsb.Setting.FifoConfig)
	print '\t\tFifo configuration:',fifo_config
	q.WriteSetting(QuickUsb.Setting.WordWide,1)
	(ok, word_wide) = q.ReadSetting(QuickUsb.Setting.WordWide)
	print '\t\tWord wide:',word_wide

	# Write data
	size = 4096 / 2
	print '\tWriteData - Working'
	data = QuickUsb.CreateWordBuffer(size)
	for i in xrange(size):
		data[i] = i
	(ok,) = q.WriteDataWords(data, size)

	# Read data
	print '\tReadData - Not working'
	rdata = []
	rdata = QuickUsb.CreateWordBuffer(size)
	(ok, bytes) = q.ReadDataWords(rdata, size)
	print rdata[:]

	# Compare read and write data
	print '\tTest data'
	for i in range(0,size):
		if data[i] != rdata[i]:
			print '\t\tInvalid data - Difference at %d'%i
			print '\t\t\t%d'%data[i]
			print '\t\t\t%d'%rdata[i]
			break

q.Close()

