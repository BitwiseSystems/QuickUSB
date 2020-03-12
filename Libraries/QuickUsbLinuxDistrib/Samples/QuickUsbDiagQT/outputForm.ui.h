/*=============================================================================
 Title        : outputForm.ui.h
 Description  : GUI Unit for QuickUsbDiag
 Notes        : None

 Copyright © 2003 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable. 

 Bitwise Systems
 5385 Hollister Ave, Suite 215
 Santa Barbara, CA  93111
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qevent.h> 
#include <qnamespace.h>

outputForm * * outputPtr;
void outputForm::init() {
   QCloseEvent();
}

void outputForm::showForm(int x, int y, int height, outputForm * * ptr) {   
  outputPtr = ptr;
   move(x, y);
   show();
   setFixedSize(outputFormRichEditBox->width() + 5, height);
   setSizeGripEnabled(false);     
}

//---------------------------------------------------------------------------

void outputForm::setText(QString str){
   outputFormRichEditBox->setText(str);
}

//---------------------------------------------------------------------------

void outputForm::showByteData(unsigned char * theData, unsigned long dataLength) {
   unsigned long col, maxCol;
   QString displayString;
   unsigned int MaxDataLength = 4096;
   outputFormRichEditBox->setText("");
   col = 0;
   maxCol = 15;
   for (unsigned long count = 0; count < dataLength && count < MaxDataLength; count++) {
      if (col == 0) {
         displayString = displayString + toHex(count, 3) + ": ";
      }

      displayString = displayString + toHex(theData[count], 1);

      if (col == maxCol) {
         displayString = displayString + "\n";
      }
      if (col < maxCol) {
         col++;
      }
      else {
         col = 0;
      }
   }
   outputFormRichEditBox->setText(displayString);
}

//---------------------------------------------------------------------------

QString outputForm::toHex(int decimal, int length){
   QString hex = "";
   for (int i=length; i>=0; i--) {
      hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
   }
   return hex;
}

void outputForm::closeEvent( QCloseEvent *event ) {
   event->accept();
   *outputPtr = NULL;	
}
