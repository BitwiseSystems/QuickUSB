/*=============================================================================
 Title        : QuickUsbApiProgram.h
 Description  : QuickUSB QUSB2 Module EEPROM Programming API
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

#define QUICKUSB_PROGRAM_STAGE_READ		0x0001
#define QUICKUSB_PROGRAM_STAGE_WRITE	0x0002
#define QUICKUSB_PROGRAM_STAGE_VERIFY	0x0003

#define UNCOMPRESSED_FILE_SIZE  0x10000
#define MAX_8K_ADDRESS          0x2000 // 0x1FFF
#define MAX_16K_ADDRESS         0x4000 // 0x3FFF
#define MAX_IIC_SEGMENT         0x400  // 0x3FF

#define NUM_EEPROM_RETRIES      10

#define QUICKUSB_PROGRAM_SERIAL_MASK     0x0000FFFF
#define QUICKUSB_PROGRAM_OPTIONS_MASK    0xFFFF0000
#define QUICKUSB_PRESERVE_MASK           0x000F0000



// Exported by library, but hidden from user
#ifdef __cplusplus
extern "C" {
#endif
QUSBLIB QRETURN QuickUsbWriteRamData(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length);
QUSBLIB QRETURN QuickUsbReadEepromData(QHANDLE hDevice, QWORD address, PQBYTE data, PQWORD length);
QUSBLIB QRETURN QuickUsbWriteEepromData(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length);
#ifdef __cplusplus
}
#endif


// Helper functions
QRETURN QuickUsbReadFirmware(QHANDLE hDevice, PCQCHAR fileName, PQPROGRESS_CALLBACK callback, PQVOID tag);
void IicCompress(PQBYTE InIic, QULONG InIicLen, PQBYTE UserBin, QULONG UserBinLen);
QBOOL IicExpand(PQBYTE InIic, QULONG InIicLen, PQBYTE UserBin, QULONG UserBinLen);
QWORD FindSection(PQBYTE eepromData, QWORD length, const PQCHAR whatFor);
void WriteRamFunction(PQCHAR devName, PQCHAR FileName);
void EraseEepromFunction(PQCHAR devName);

