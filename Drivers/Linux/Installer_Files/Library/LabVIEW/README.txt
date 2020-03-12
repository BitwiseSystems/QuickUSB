 QuickUSB LabVIEW Interface
----------------------------
There are a few ways to interface the QuickUSB library to LabVIEW.  They are 
as follows:

1) Have LabVIEW use the QuickUSB .NET Assembly (Recommended)

To use the .NET assembly, in LabVIEW you create a ‘constructor node’ object 
and use its reference output as an input to ‘invoke’ objects to call the API 
functions.  Each ‘invoke’ object takes an error as an input and has an error 
output.  The error input is used to prevent the function call if the input 
error line indicates failure.  That way if a function call fails, you can 
prevent other function calls from being made.  Note that you don't have to 
create any interface VIs to use the .NET assembly in LabVIEW.

The .NET Assembly (QuickUSB.NET.dll) is installed with the QuickUSB Library 
(Typically 'C:\Program Files\Bitwise Systems\QuickUsb\Library\Assembly')

For more information, refer to:
http://zone.ni.com/reference/en-XX/help/371361E-01/lvhowto/create_net_object/



2) Have LabVIEW generate interface VIs to the QuickUSB DLL

The files located in the 'Shared Library Import' folder (Typically 'C:\Program 
Files\Bitwise Systems\QuickUsb\Library\LabVIEW\Shared Library Import') are the 
VIs interfaces create using the 'Shared Library Import' function.  You may use 
these VIs as an interface to the QuickUSB DLL.

To regenerate the VI interfaces, select ‘Tools->Import->Shard Library (.dll)’ 
from within the LabVIEW environment.  Next, select the ‘Create VIs for a shared 
library’ option then click ‘Next’.  Specify the paths to the QuickUSB DLL 
(Typically 'C:\Program Files\Bitwise Systems\QuickUsb\Library\DLL\i386') and 
header file (Typically 'C:\Program Files\Bitwise Systems\QuickUsb\Library\VC') 
and click ‘Next’.  In the preprocessor definitions windows enter ‘_WIN32’ then 
click ‘Next’.  LabVIEW will parse the QuickUSB header file and extract functions 
for which it can create a VI for.  Continue on with the wizard to generate the 
interface VIs.  

Note: LabVIEW currently is unable to properly determine the interfaces for API 
functions that have structures (such as QBULKSTREAM) or pointers to functions 
(such as callback functions and completion routines).  Because of this, LabVIEW 
is unable to automatically generate interface methods to the EPCS, Firmware, 
Streaming, and new Bulk Async API functions.

For more information, refer to:
http://zone.ni.com/devzone/cda/tut/p/id/2818



3) Create your own interface VIs

The files located in the 'User Contributed' folder (Typically 'C:\Program Files\
Bitwise Systems\QuickUsb\Library\LabVIEW\User Contributed') are user-contributed 
files that can help you get started with creating your own interface VIs to 
QuickUSB from scratch.

For more information, refer to:
http://zone.ni.com/reference/en-XX/help/371361D-01/glang/call_library_function/
