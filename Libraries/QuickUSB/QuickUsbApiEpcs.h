/*=============================================================================
 Title        : QuickUsbApiEpcs.h
 Description  : QuickUSB QUSB2 Module EPCS API
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
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, 
 Suite E, Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/
#include "QuickUsbUtil.h"

// EPCS Device Opcodes
#define EPCS_WRITE_ENABLE                       0x06
#define EPCS_WRITE_DISABLE                      0x04
#define EPCS_READ_STATUS                        0x05
#define EPCS_READ_BYTES                         0x03
#define EPCS_READ_ID                            0xAB    // EPCS1, EPCS4, EPCS16, EPCS64 devices only
#define EPCS_READ_FAST                          0x0B
#define EPCS_WRITE_STATUS                       0x01
#define EPCS_WRITE_BYTES                        0x02
#define EPCS_ERASE_BULK                         0xC7
#define EPCS_ERASE_SECTOR                       0xD8
#define EPCS_READ_DEVICE_ID                     0x9F    // EPCS128 devices Only

// EPCS Status bits
#define EPCS_STATUS_WIP                         0x01
#define EPCS_STATUS_WEL                         0x02



/// Function prototypes
void BitFlip(PQBYTE value);
