 QuickUSB MatLab Interface
----------------------------
To call QuickUSB API functions from within MatLab, you must use the 
loadlibrary and unloadlibrary functions to first load and unload the QuickUSB 
DLL (Typically located at 'C:\Program Files\Bitwise Systems\QuickUsb\Library\
DLL\i386') and header file (Typically located at 'C:\Program Files\Bitwise 
Systems\QuickUsb\Library\VC').  Once the library is loaded, you may make calls 
to functions using the calllib function.  The QuickUSB.m file (Typically 
located at 'C:\Program Files\Bitwise Systems\QuickUsb\Library\MatLab') 
demonstrates how you can create an interface to make calls to QuickUSB API 
functions.  The sample m script (Typically located at 'C:\Program Files\
Bitwise Systems\QuickUsb\Samples\MatLab') demonstrates how to use the 
QuickUSB.m interface class to actually perform QuickUSB function calls.

For more information, refer to:
http://www.mathworks.com/help/techdoc/ref/loadlibrary.html
http://www.mathworks.com/help/techdoc/ref/calllib.html
http://www.mathworks.com/help/techdoc/ref/unloadlibrary.html