/*=============================================================================
 Title        : outputForm.ui.h
 Description  : GUI Unit for QuickUsbDiag
 Notes        : None

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
