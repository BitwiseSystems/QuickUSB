#How to create shared libraries: http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html
#Where to place distributed files: http://proton.pathname.com/fhs/pub/fhs-2.3.html
VERSION=2.15.3
CC=gcc
RM=rm -f
INSTALL=install
LN=ln -s
AR=ar rc
STRIP=strip
CHMOD=chmod
LDCONFIG=/sbin/ldconfig
CFLAGS  = -x c -c -Wall -fPIC -fno-stack-protector -m$(ARCH) -D _GNU_SOURCE
LIBDIR = /usr/lib
DEBUG=0
ASYNC=1

ifeq ($(ASYNC), 1)
  LIBRARIES=-lc -lrt -laio
else
  LIBRARIES=-lc -lrt
endif

ifeq ($(ARCH),32)
  OUTDIR = x86/Release/Linux
else
  OUTDIR = x$(ARCH)/Release/Linux
endif

ifeq ($(DEBUG),1)
  CFLAGS += -g -O0
endif

SOURCES = QuickUsbApiBase.cpp \
          QuickUsbApiData.cpp \
          QuickUsbApiEpcs.cpp \
          QuickUsbApiFPGA.cpp \
          QuickUsbApiGPIO.cpp \
          QuickUsbApiIIC.cpp \
          QuickUsbApiProgram.cpp \
          QuickUsbApiRS232.cpp \
          QuickUsbApiSettings.cpp \
          QuickUsbApiSpi.cpp \
          QuickUsbApiStorage.cpp \
          QuickUsbApiStream.cpp \
          QuickUsbUtil.cpp \
          QuickUsbApiStatistics.cpp

OBJECTS=$(SOURCES:.cpp=.o)

SYM=-K QuickUsbGetLastError \
    -K QuickUsbGetLastDriverError \
    -K QuickUsbFindModules \
    -K QuickUsbOpen \
    -K QuickUsbOpenEx \
    -K QuickUsbClose \
    -K QuickUsbGetStringDescriptor \
    -K QuickUsbSetTimeout \
    -K QuickUsbGetDllVersion \
    -K QuickUsbGetDriverVersion \
    -K QuickUsbGetFirmwareVersion \
    -K QuickUsbStartFpgaConfiguration \
    -K QuickUsbWriteFpgaData \
    -K QuickUsbConfigureFpga \
    -K QuickUsbIsFpgaConfigured \
    -K QuickUsbReadSetting \
    -K QuickUsbWriteSetting \
    -K QuickUsbReadDefault \
    -K QuickUsbWriteDefault \
    -K QuickUsbReadCommand \
    -K QuickUsbWriteCommand \
    -K QuickUsbReadData \
    -K QuickUsbWriteData \
    -K QuickUsbReadDataAsync \
    -K QuickUsbWriteDataAsync \
    -K QuickUsbAsyncWait \
    -K QuickUsbReadBulkDataAsync \
    -K QuickUsbWriteBulkDataAsync \
    -K QuickUsbBulkWait \
    -K QuickUsbSetNumAsyncThreads \
    -K QuickUsbGetNumAsyncThreads \
    -K QuickUsbAllocateDataBuffer \
    -K QuickUsbFreeDataBuffer \
    -K QuickUsbReadBulkDataStartStream \
    -K QuickUsbWriteBulkDataStartStream \
    -K QuickUsbReadBulkDataStartStreamToFile \
    -K QuickUsbWriteBulkDataStartStreamFromFile \
    -K QuickUsbOpenMemoryFile \
    -K QuickUsbCloseMemoryFile \
    -K QuickUsbStopStream \
    -K QuickUsbProcessStream \
    -K QuickUsbPauseStream \
    -K QuickUsbResumeStream \
    -K QuickUsbReadPortDir \
    -K QuickUsbWritePortDir \
    -K QuickUsbReadPort \
    -K QuickUsbWritePort \
    -K QuickUsbSetRS232BaudRate \
    -K QuickUsbGetNumRS232 \
    -K QuickUsbFlushRS232 \
    -K QuickUsbReadRS232 \
    -K QuickUsbWriteRS232 \
    -K QuickUsbReadSpi \
    -K QuickUsbWriteSpi \
    -K QuickUsbWriteReadSpi \
    -K QuickUsbReadI2C \
    -K QuickUsbWriteI2C \
    -K QuickUsbCachedWriteI2C \
    -K QuickUsbWriteStorage \
    -K QuickUsbReadStorage \
    -K QuickUsbWriteFirmware \
    -K QuickUsbVerifyFirmware \
    -K QuickUsbIdentifyEpcs \
    -K QuickUsbVerifyEpcs \
    -K QuickUsbEraseEpcs \
    -K QuickUsbConfigureEpcs \
    -K QuickUsbSetResetState \
    -K QuickUsbWriteRamData \
    -K QuickUsbReadEepromData \
    -K QuickUsbWriteEepromData \
    -K QuickUsbGetDeviceDesc \
    -K QuickUsbResetStatistic \
    -K QuickUsbGetStatistic

ECHO = @echo -e
DISP_CC  =   "\033[32;1mCompiling \033[0m"
DISP_LN  =   "\033[34;1mLinking   \033[0m"
DISP_STRIP = "\033[35;1mExporting symbols\033[0m"
DISP_RM =    "\033[35;1mCleaning up\033[0m"

#make 
all: $(OBJECTS)
	$(ECHO) $(DISP_LN) "libquickusb.so.$(VERSION)"
	@$(CC)  -m$(ARCH) -shared -Wl,-soname,libquickusb.so.$(VERSION) --strip-unneeded -o $(OUTDIR)/libquickusb.so.$(VERSION) $(OBJECTS) $(LIBRARIES)
	$(ECHO) $(DISP_LN) "libquickusb.a.$(VERSION)"
	@$(AR) $(OUTDIR)/libquickusb.a.$(VERSION) $(OBJECTS)
ifeq ($(DEBUG),0)
	$(ECHO) $(DISP_STRIP)
	@$(STRIP) $(SYM) $(OUTDIR)/libquickusb.so.$(VERSION)
	@$(STRIP) $(SYM) $(OUTDIR)/libquickusb.a.$(VERSION)
endif
	$(ECHO) $(DISP_RM)
	@$(RM) $(OBJECTS)

.cpp.o:
	$(ECHO) $(DISP_CC) $<
	@$(CC) $(CFLAGS) $< -o $@



#make install
install: all
	$(INSTALL) -c $(OUTDIR)/libquickusb.so.$(VERSION) $(LIBDIR)
	$(INSTALL) -c $(OUTDIR)/libquickusb.a.$(VERSION) $(LIBDIR)
	$(LN) -f $(LIBDIR)/libquickusb.so.$(VERSION) $(LIBDIR)/libquickusb.so
	$(LN) -f $(LIBDIR)/libquickusb.a.$(VERSION) $(LIBDIR)/libquickusb.a
	$(CHMOD) --reference=$(LIBDIR)/libquickusb.so.$(VERSION) $(LIBDIR)/libquickusb.* 
	$(LDCONFIG)



#make clean
clean:
	$(RM) *.o $(OUTDIR)/*.a $(OUTDIR)/libquickusb*



#make uninstall
uninstall:
	$(RM) $(LIBDIR)/libquickusb*
	$(LDCONFIG)

