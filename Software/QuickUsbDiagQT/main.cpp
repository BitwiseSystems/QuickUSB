/*=============================================================================
 Title        : Main.cpp
 Description  : none
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
#include <qapplication.h>
#include <qtextedit.h>
#include <qstatusbar.h>
#include "QuickUsbDiag.h"

int main( int argc, char ** argv ){
   QApplication a( argc, argv );
   QuickUsbDisplay w;
   
   w.show();
   a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
   return a.exec();
}

