Attribute VB_Name = "InpOut"
'=============================================================================
' Title        : InpOut.bas
' Description  : QuickUSB Library Function Declarations
' Copyright    : Copyright (c) 2002 Bitwise Systems
' Notes        : Add this file to your project in order to use the QuickUSB
'                library.
' History      :
'   Date       Name        Description
'  6/7/2002   bhenry      Initial coding
'
' Copyright © 2002 Bitwise Systems, Inc.  All rights reserved.
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
' 697 Via Miguel
' Santa Barbara, CA  93111
' Voice: (805) 683-6469
' Fax  : (805) 683-6469
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'=============================================================================
Option Explicit


Dim devName As String


Function InitInpOut()
   ' Initialize the InpOut Module to find and use the first available
   ' QuickUSb module
   Dim hDevice As Long
   Dim result As Long
   Dim devList As String
   
   ' Get the list of modules
   devList = Space(128)
   result = QuickUsbFindModules(devList, 128)
   If (devList = "") Then
      MsgBox "Cannot find any QuickUSB Modules", , "InpOut:InitInpOut"
      InitInpOut = False
      Return
   End If
   
   ' Extract the first device name from the list
   devName = Mid(devList, 1, InStr(1, devList, Chr(0), vbBinaryCompare) - 1)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:InitInpOut"
      InitInpOut = False
      Exit Function
   End If
   
   ' Tell the device to run in byte mode only
   result = QuickUsbWriteSetting(hDevice, WORDWIDE, 0)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the result
   If (result = 0) Then
      MsgBox "Cannot write WORDWIDE setting", , "InpOut:InitInpOut"
      InitInpOut = False
      Exit Function
   End If
   
   InitInpOut = True
End Function



Sub SetDevName(theDevName As String)
' Explicitly sets the device name of the QuickUsb module to use
   devName = theDevName
End Sub



Function Inp(address As Integer)
' Use this function just like the Qbasic Inp function
' address - The address to read from
   Dim result As Long
   Dim hDevice As Long
   Dim theData As Byte
   
   ' Check devName
   If (devName = "") Then
      MsgBox "Invalid device name", , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   ' Read the data
   result = QuickUsbReadCommand(hDevice, address, theData, 1)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Inp failed", , "InpOut:Inp"
      Inp = -1
      Exit Function
   End If
   
   Inp = theData
End Function



Sub Out(address As Integer, theData As Byte)
   ' Use this function just like the Qbasic Out function
   ' address - The address to write to
   ' theData - the Byte to write
   Dim result As Long
   Dim hDevice As Long
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      MsgBox "Cannot open device:" & devName, , "InpOut:Out"
      Exit Sub
   End If
   
   ' Write the data
   result = QuickUsbWriteCommand(hDevice, address, theData, 1)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      MsgBox "Out failed", , "InpOut:Out"
      Exit Sub
   End If
   
End Sub
