/*=============================================================================
 Title        : QuickUSBEeprom.h
 Description  : QuickUSB EEPROM API - Not to be distributed to users.
 Notes        : None
 History      :

 Copyright (c) 2012 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 697 Via Miguel
 Santa Barbara, CA  93111.

 Bitwise Systems
 5385 Hollister Ave, Suite 215
 Santa Barbara, CA  93111
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

 $History: QuickUSBEeprom.h $
// 
// *****************  Version 13  *****************
// User: Bob Jacobson Date: 2/08/06    Time: 5:37p
// Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsb
// Changes required for common Windows/Linux/Mac support.
// 
// *****************  Version 12  *****************
// User: Bob Jacobson Date: 11/08/05   Time: 3:45p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Do not allow reading or writing to the patch status byte in the
// defaults.
// Bump DLL version number.
// Add some comments about the patch status byte.
// 
// *****************  Version 11  *****************
// User: Bob Jacobson Date: 11/04/05   Time: 4:37p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Define QuickUsbReadEeprom and QuickUsbWriteEeprom as C functions
// instead of C++ functions.
// 
// *****************  Version 10  *****************
// User: Blake Henry  Date: 7/26/04    Time: 3:17p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Fixed FindModules problem that set deviceHandle to 0
// Fixed 64MB transfer problem for non-async operation
// 
// *****************  Version 1  *****************
// User: Jay Towson   Date: 3/30/04    Time: 2:03p
// Created in $/Projects/Bitwise/QuickUSB/Software/Driver/QuickUsb
 
 *****************  Version 4  *****************
 User: Blake Henry  Date: 6/11/03    Time: 9:37a
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Fixed header to correctly display the History

=============================================================================*/
#ifndef _QUICKUSB_EEPROM_HEADER_
#define _QUICKUSB_EEPROM_HEADER_

#include "QuickUsbVersion.h"

// QuickUSB LoadRam typedef for intel hex records
// Used for patching the QuickUSB code to add special features
// or modify the default behavior of the firmware
#ifndef MAX_INTEL_HEX_RECORD_LENGTH
#define MAX_INTEL_HEX_RECORD_LENGTH 16
typedef struct _INTEL_HEX_RECORD {
   BYTE  Length;
   WORD  Address;
   BYTE  Type;
   BYTE  Data[MAX_INTEL_HEX_RECORD_LENGTH];
} INTEL_HEX_RECORD, *PINTEL_HEX_RECORD;
#endif

// Patch status word settings
// The patch status is the most significant byte of default 4 (SETTING_FPGATYPE).
// The PATCHED_DEFAULTS bit is set by the QuickUsbWriteDefault API function in
// the DLL whenever any of the defaults are written.
// The PATCHED_CUSTOMIZER bit is set by the QuickUsbCustomizer program.
// The PATCHED_GPIF bit is set by the QuickUsbGpifUpdate program.
// These bits are cleared only by the QuickUsbProgrammer program when the 
// Option Preserve menu items are unchecked.
#define SETTING_PATCHES    SETTING_FPGATYPE
#define PATCHED_DEFAULTS   0x01//00
#define PATCHED_CUSTOMIZER 0x02//00
#define PATCHED_GPIF       0x04//00

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  External variable declarations
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
extern INTEL_HEX_RECORD FX2_CODE[];

#endif
