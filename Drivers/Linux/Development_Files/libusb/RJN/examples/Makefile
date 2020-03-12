BINARIES = listdevices parporttest

all : $(BINARIES)

QUICKUSB_LIBS = -lquickusb -lusb

% : %.c
	$(CC) $< -o $@ $(QUICKUSB_LIBS)

clean :
	rm -f $(BINARIES)
