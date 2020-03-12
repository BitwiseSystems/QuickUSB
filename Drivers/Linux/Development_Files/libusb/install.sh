#!/bin/bash
if [ $(id -u) != "0" ]; then
   echo "ERROR: You must be root for this script to work"
   exit 1
else
  if [ -a Hotplug/QuickUSB ]; then
     if [ -d /etc/hotplug/usb ]; then
        cp Hotplug/QuickUSB /etc/hotplug/usb/
     else
        echo "ERROR: The directory /etc/hotplug/usb does not exist"
	echo "QuickUSB requires Hotplug. Please install Hotplug and rerun this script."
	exit 1
     fi
  else
     echo "ERROR: The file ./Hotplug/QuickUSB does not exist"
     echo "Please reinstall QuickUSB, then rerun this script"
     exit 1
  fi
  if [ -a /etc/hotplug/usb.usermap ]; then
     var=`grep -c "# Bitwise Systems QuickUSB module" /etc/hotplug/usb.usermap`
     if [ $var -ne 1 ]; then
       cat << EOF >> /etc/hotplug/usb.usermap
# Bitwise Systems QuickUSB module
QuickUSB               0x0003      0x0FBB   0x0001    0x0000       0x0000       0x00         0x00            0x00            0x00            0x00               0x00               0x00000000
EOF
      fi
  else
     echo "The file /etc/hotplug/usb.usermap does not exist"
     echo "QuickUSB requires Hotplug. Please install Hotplug and rerun this script."
     exit 1

  fi
  cp Driver/* /usr/lib/ 
  ln -fs /usr/lib/libquickusb.so.2.0.0 /usr/lib/libquickusb.so.2 
  ln -fs /usr/lib/libquickusb.so.2.0.0 /usr/lib/libquickusb.so 
  /sbin/ldconfig
  echo "Installation completed successfully." 
  echo "We hope you enjoy QuickUSB --The QuickUSB Development Team" 
fi
