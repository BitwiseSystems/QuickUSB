#!/bin/bash

#Welcome
echo "Bitwise Systems QuickUSB"
echo "This script will install the QuickUSB Library onto your computer"
echo ""

if [ $(id -u) != "0" ]; then
	echo "ERROR: You must be root for this script to work"
	exit 1
else
	#Install required software packages
	if [ "`uname -a | grep ubuntu`" != "" ]; then
		echo "ACTION: Installing GCC, G++, and QT3-DEV-TOOLS"
		sudo apt-get install gcc g++ libqt3-mt qt3-dev-tools
	else
		echo "WARNING: Please ensure that gcc, g++, libqt3-mt, and qt3-dev-tools are installed."
	fi
	echo ""

	#Build and install libusb
	echo "ACTION: Configuring libusb"
	cd ./libusb-0.1.12
	./configure
	echo ""
	echo "ACTION: Buuilding libusb"
	make
	echo ""
	echo "ACTION: Installing libusb"
	sudo make install
	echo ""
	cd ..
	
	#Hotplug (Now build into Linux Kernels starting with 2.4 and some 2.2 backports)
	if [ -f Hotplug/QuickUSB ]; then
		if [ -d /etc/hotplug/usb ]; then
			cp Hotplug/QuickUSB /etc/hotplug/usb/
		fi
	fi
	if [ -f /etc/hotplug/usb.usermap ]; then
		var=`grep -c "# Bitwise Systems QuickUSB module" /etc/hotplug/usb.usermap`
		if [ $var -ne 1 ]; then
			cat << EOF >> /etc/hotplug/usb.usermap
# Bitwise Systems QuickUSB module
QuickUSB               0x0003      0x0FBB   0x0001    0x0000       0x0000       0x00         0x00            0x00            0x00            0x00               0x00               0x00000000
EOF
		fi
	fi

	#udev
	if [ -d /lib/udev/rules.d ]; then
		cp Library/60-quickusb.rules /lib/udev/rules.d
	fi
	if [ -d /lib32/udev/rules.d ]; then
		cp Library/60-quickusb.rules /lib32/udev/rules.d
	fi
	if [ -d /lib64/udev/rules.d ]; then
		cp Library/60-quickusb.rules /lib64/udev/rules.d
	fi

	#Copy QuickUSB libraries to proper directories
	if [ "`getconf LONG_BIT`" == "64" ]; then
		cp Library/x64/* /usr/lib
	else
		cp Library/x86/* /usr/lib
	fi
	ln -fs /usr/lib/libquickusb.so.2.0.0 /usr/lib/libquickusb.so.2 
	ln -fs /usr/lib/libquickusb.so.2.0.0 /usr/lib/libquickusb.so 
	if [ -d /usr/lib32 ]; then
		cp Library/x86/* /usr/lib32
		ln -fs /usr/lib32/libquickusb.so.2.0.0 /usr/lib32/libquickusb.so.2 
		ln -fs /usr/lib32/libquickusb.so.2.0.0 /usr/lib32/libquickusb.so 
	fi
	if [ -d /usr/lib64 ]; then
		cp Library/x64/* /usr/lib64
		ln -fs /usr/lib64/libquickusb.so.2.0.0 /usr/lib64/libquickusb.so.2 
		ln -fs /usr/lib64/libquickusb.so.2.0.0 /usr/lib64/libquickusb.so 
	fi
	/sbin/ldconfig

	echo ""
	echo ""
	echo "Installation completed successfully." 
	echo "We hope you enjoy QuickUSB --The QuickUSB Development Team" 
	echo ""
fi
