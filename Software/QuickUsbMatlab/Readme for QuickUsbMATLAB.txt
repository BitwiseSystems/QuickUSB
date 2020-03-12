Welcome to QuickUSB for MATLAB



QuickUSB for MATLAB is a collection of MATLAB code that makes it very easy to use a QuickUSB Module under MATLAB.

We have created a MATLAB class for our QuickUSB Library which gives you full access to the power of QuickUSB while making it easy to use from MATLAB.


To get started, from MATLAB you must navigate your current directory to the QuickUsbMatlab directory.

Then type 'QuickUsbSpiTest'.  There will be a long delay as MATLAB loads up the QuickUSB Libary, then the script will run.

Subsequent calls will run much faster as the library has already been loaded.

The script creates an QuickUSB object called qusb which is available to be used from the command line.

Next you can type in: qusb.ReadSpiBytes(0, 4)

This will cause the library to read 4 bytes from SPI device 0.


You can learn more about QuickUSB and the QuickUSB Library from www.quickusb.com

Please let us know about any problems that you may have with this code by emailing support@quickusb.com.



Best regards,

The QuickUSB Team
