SOURCES	+= main.cpp CQuickUsb.cpp
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

CONFIG += moc
CONFIG -= qt
CONFIG	+= warn_on release
FORMS	= QuickUsbDiag.ui \
	outputForm.ui \
	AboutForm.ui
IMAGES	= QuickUSB_Logo.jpeg
TEMPLATE	=app
CONFIG	+= qt warn_on release
INCLUDEPATH	+= $(QTDIR)/include
LIBS	+= $(QTDIR)/lib/libqt.a -L/usr/X11R6/lib /usr/lib/libusb.a -lc /usr/lib/libquickusb.a -lXp -lXt -lX11 -lXext -lXinerama -lXrender -lm
LANGUAGE	= C++
