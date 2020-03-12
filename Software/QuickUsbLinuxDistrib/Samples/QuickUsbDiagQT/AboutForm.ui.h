/*=============================================================================
 Title        : AboutForm.ui.h
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
