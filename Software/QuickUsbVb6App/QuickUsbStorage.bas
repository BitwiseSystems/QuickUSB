Attribute VB_Name = "QuickUsbStorage"
'=============================================================================
' Title        : QuickUsbStorage.bas
' Description  : QuickUSB Library Function Declarations
' Copyright    : Copyright (c) 2008 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
'
' Copyright © 2008 Bitwise Systems, Inc.  All rights reserved.
' This software contains confidential information and trade secrets of
' Bitwise Systems and is protected by United States and international
' copyright laws.  Use, disclosure, or reproduction is prohibited without
' the prior express written permission of Bitwise Systems, except as agreed
' in the QuickUSB Plug-In Module license agreement.
'
' Use, duplication or disclosure by the U.S. Government is subject to
' restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
' (1998), and FAR 12.212, as applicable.  Bitwise Systems, 697 Via Miguel
' Santa Barbara, CA  93111.
'
' Bitwise Systems
' 6489 Calle Real, Suite E
' Goleta, CA  93119
' Voice: (805) 683-6469
' Fax  : (805) 683-4833
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: $
'
'=============================================================================
Option Explicit


Public Const EEPROM_STORAGE_BASE = &H3000


Public Function QuickUsbWriteStorage(ByVal regAddr As Byte, ByVal Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   Data(0) = Value
   result = QuickUsbWriteSpi(hDevice, regAddr, Data(0), 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function



Public Function QuickUsbReadStorage(ByVal regAddr As Byte, ByRef Value As Byte) As Variant
   Dim hDevice As Long
   Dim result As Long
   Dim Data(1) As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, m_devName)
   If (result = 0) Then
      Exit Function
   End If
   
   result = QuickUsbReadSpi(hDevice, regAddr, Data(0), 1)
   Value = Data(0)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      Exit Function
   End If
End Function




