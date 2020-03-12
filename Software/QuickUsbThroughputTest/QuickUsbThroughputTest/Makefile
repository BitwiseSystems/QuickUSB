PROGRAM=QuickUsbThroughput
ARCH=64
CC=g++
CFLAGS=-c -Wall -I../../QuickUSB/
RM=rm -f
COLORS=1

SOURCES = PlatformIndependentTimer.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

ifeq ($(ARCH), 64)
OUTDIR=.
else
OUTDIR=.
ARCH=32
endif

ECHO = @echo
ifeq ($(COLORS), 1)
DISP_CC  =   "\033[32;1mCompiling  \033[0m"
DISP_LN  =   "\033[34;1mLinking    \033[0m"
DISP_RM =    "\033[35;1mCleaning up\033[0m"
DISP_EXE =   "\033[35;1mExecuting  \033[0m"
else
DISP_CC  =   "Compiling   "
DISP_LN  =   "Linking     "
DISP_RM =    "Cleaning up "
DISP_EXE =   "Executing   "
endif

#make
all: $(OBJECTS)
	$(ECHO) $(DISP_LN) "$(OUTDIR)/$(PROGRAM)"
	@$(CC)  -m$(ARCH) -o $(OUTDIR)/$(PROGRAM) $(OBJECTS) -lquickusb
	$(ECHO) $(DISP_RM)
	@$(RM) $(OBJECTS)

.cpp.o:
	$(ECHO) $(DISP_CC) $<
	@$(CC) -m$(ARCH) $(CFLAGS) $< -o $@

#make run
run: all
	$(ECHO) $(DISP_EXE) "$(PROGRAM)"
	@./$(OUTDIR)/$(PROGRAM)


#make clean
clean:
	$(RM) *.o $(OUTDIR)/*
