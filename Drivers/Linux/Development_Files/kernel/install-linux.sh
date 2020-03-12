#!/bin/bash

#The QuickUSB Library Version
QUSBLIBVERSION=2.15.2

#Variables for color-friendly messages
ECHO="echo"
DISP_HEADER="Bitwise Systems Linux QuickUSB Library v$QUSBLIBVERSION Installer"
DISP_FOOTER="QuickUSB Library installation completed successfully!"
DISP_INST="Installing"
DISP_ERR="Error"
DISP_WARN="Warning"
DISP_RM="Removing"



#Display welcome message
$ECHO $DISP_HEADER


#The user must be root to perform an installation
if [ $(id -u) != "0" ]; then
    $ECHO $DISP_ERR "You must have root permmissions to perform the install"
    exit 1
fi


#Check that we're running a supported kernel version
#version=`uname -r | awk -F. '{ printf("%d.%d.%d\n",$1,$2,$3); }'`

#Verify all required command line programs are installed
hash make 2>&- || { echo >&2 "Installation failed: Command 'make' not found"; }
hash /sbin/insmod 2>&- || { echo >&2 "Installation failed: Command 'insmod' not found"; }
hash /sbin/rmmod 2>&- || { echo >&2 "Installation failed: Command 'rmmod' not found"; }
hash /sbin/ldconfig 2>&- || { echo >&2 "Installation failed: Command 'ldconfig' not found"; }


#Check that required libraries are installed
#libaio-dev (Ubuntu)
#libaio-devel.i386 (x86 CentOS)

#This command will likely fail on non-Debian platforms
apt-get install libaio-dev


#Remove old QuickUSB kernel object, if present
result="$(/sbin/lsmod | grep qusb_lnx)"
if [ ${#result} -ne  0 ]; then
	$ECHO $DISP_RM "currently installed quickusb driver"
	/sbin/rmmod qusb_lnx
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to remove old driver"
		exit 1
	fi
fi


#Build and install driver
if [ "`getconf LONG_BIT`" = "64" ]; then
	$ECHO $DISP_INST "64-bit driver"
	cd Drivers/v$QUSBLIBVERSION
	make install OUTDIR=x64
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to build/install driver"
		exit 1
	fi
	cd ../..
else
	$ECHO $DISP_INST "32-bit driver"
	cd Drivers/v$QUSBLIBVERSION
	make install OUTDIR=x86
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to build/install driver"
		exit 1
	fi
	cd ../..
fi


#Install library
if [ "`getconf LONG_BIT`" = "64" ]; then
	if [ ! -d /usr/lib32 ]; then
		$ECHO $DISP_INST "x64 library to /usr/lib (NOTE: /usr/lib32 not found)"
		cp Library/Linux/x64/* /usr/lib
		if [ $? -ne 0 ]; then
			$ECHO $DISP_ERR "Installation failed: Unable to copy library files"
			exit 1
		fi
	else
		$ECHO $DISP_INST "x64 libraries to /usr/lib"
		cp Library/Linux/x64/* /usr/lib
		if [ $? -ne 0 ]; then
			$ECHO $DISP_ERR "Installation failed: Unable to copy library files"
			exit 1
		fi

		$ECHO $DISP_INST "x86 libraries to /usr/lib32"
		cp Library/Linux/x86/* /usr/lib32
		if [ $? -ne 0 ]; then
			$ECHO $DISP_ERR "Installation failed: Unable to copy library files"
			exit 1
		fi

		ln -fs /usr/lib32/libquickusb.a.$QUSBLIBVERSION /usr/lib32/libquickusb.a
		if [ $? -ne 0 ]; then
			$ECHO $DISP_ERR "Installation failed: Unable to create symbolic links"
			exit 1
		fi

		ln -fs /usr/lib32/libquickusb.so.$QUSBLIBVERSION /usr/lib32/libquickusb.so
		if [ $? -ne 0 ]; then
			$ECHO $DISP_ERR "Installation failed: Unable to create symbolic links"
			exit 1
		fi
	fi
else
	$ECHO $DISP_INST "x86 libraries to /usr/lib"
	cp Library/Linux/x86/* /usr/lib
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to copy library files"
		exit 1
	fi
fi

ln -fs /usr/lib/libquickusb.so.$QUSBLIBVERSION /usr/lib/libquickusb.so
if [ $? -ne 0 ]; then
	$ECHO $DISP_ERR "Installation failed: Unable to create symbolic links"
	exit 1
fi

ln -fs /usr/lib/libquickusb.a.$QUSBLIBVERSION /usr/lib/libquickusb.a
if [ $? -ne 0 ]; then
	$ECHO $DISP_ERR "Installation failed: Unable to create symbolic links"
	exit 1
fi

/sbin/ldconfig
if [ $? -ne 0 ]; then
	$ECHO $DISP_WARN "Unable to execute /sbin/ldconfig"
fi


#Install QuickUSB.h header file
$ECHO $DISP_INST "QuickUSB.h to /usr/include"
cp Library/C/QuickUSB.h /usr/include
if [ $? -ne 0 ]; then
	$ECHO $DISP_WARN "Unable to install QuickUSB.h"
fi


#Create udev rule
$ECHO $DISP_INST "udev rule"
if [ -d /etc/udev/rules.d ]; then
	cp Drivers/v$QUSBLIBVERSION/90-qusb.rules /etc/udev/rules.d
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to copy 90-qusb.rules to /etc/udev/rules.d"
		exit 1
	fi

	chmod 775 /etc/udev/rules.d/90-qusb.rules 
	if [ $? -ne 0 ]; then
		$ECHO $DISP_ERR "Installation failed: Unable to set 90-qusb.rules file permissions"
		exit 1
	fi
else
	$ECHO $DISP_ERR "Unable to locate udev directory"
	exit
fi

cp Drivers/v$QUSBLIBVERSION/qusb_script.sh /usr/local/bin/qusb_script.sh
if [ $? -ne 0 ]; then
	$ECHO $DISP_ERR "Installation failed: Unable to install udev boot script"
	exit 1
fi

chmod 775 /usr/local/bin/qusb_script.sh
if [ $? -ne 0 ]; then
	$ECHO $DISP_ERR "Installation failed: Unable to set qusb_script.sh file permissions"
	exit 1
fi


#The installation is complete
$ECHO $DISP_FOOTER
