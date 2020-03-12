/*=============================================================================
 Title        : AboutForm.ui.h
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
AboutForm ** aboutPtr;

void AboutForm::init() {
   QCloseEvent();
}

void AboutForm::showForm(int x, int y, AboutForm * *ptr) { 
   aboutPtr = ptr;
   int xOffset, yOffset, spacing;
   xOffset = x - this->width()/2;
   yOffset = y - this->height()/2;
   move(xOffset, yOffset);
   spacing = QuickUsbLogo->height() + 50;
   show();
   setSizeGripEnabled(false);
   aboutFormRichEdit->setText("QuickUSB Diagnostics\nVersion 2.00.00\nCopyright (c) 2002 Bitwise Systems");
   setFixedSize(267,spacing);

}

void AboutForm::closeEvent( QCloseEvent *event ) {
  event->accept();
  *aboutPtr = NULL;	
}
