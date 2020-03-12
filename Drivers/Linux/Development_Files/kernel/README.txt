Linux README

To Build Driver:
x86: make OUTDIR=x86
x64: make OUTDIR=x64

Load QuickUSB driver:
>	sudo insmod qusb_lnx.ko

Remove QuickUSB driver:
>	sudo rmmod qusb_lnx.ko

To create the binary installer script from the install-linux.sh script, execute:
shc -r -f install-linux.sh

To create the zip file containing all the driver files execute the makezip.sh script
