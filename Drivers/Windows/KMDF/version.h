/*=============================================================================
 Title        : 
 Description  : 
 Notes        : 
 History      : 

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

=============================================================================*/
#ifndef __QUICKUSB_DRIVER_VERSION_HEADER__
#define __QUICKUSB_DRIVER_VERSION_HEADER__

#include <initguid.h>

// {F8729270-1A3E-45f9-8C78-3ED21A22C4B1}
DEFINE_GUID(GUID_DEVINTERFACE_QUSB, 
    0xf8729270, 0x1a3e, 0x45f9, 0x8c, 0x78, 0x3e, 0xd2, 0x1a, 0x22, 0xc4, 0xb1);

// NOTE: Keep this version infomation with that in QuickUSB.rc and Makefile.inc
#define QUSB_MAJOR_VERSION 3
#define QUSB_MINOR_VERSION 0
#define QUSB_BUILD_VERSION 0



#endif // __QUICKUSB_DRIVER_VERSION_HEADER__
