SOURCES	+= main.cpp \
	CQuickUsb.cpp
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= outputForm.ui \
	AboutForm.ui \
	QuickUsbDiag.ui
IMAGES	= QuickUSB_Logo.jpeg
TEMPLATE	=app
CONFIG	+= qt warn_on release
LIBS	+= -lusb -lquickusb
LANGUAGE	= C++
