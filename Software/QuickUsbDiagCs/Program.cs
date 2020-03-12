/* ========================================================================
	Title        : 
	Description  : 
	Notes        : 
	History      :

	Copyright © 2012 Bitwise Systems.  All rights reserved.
	This software contains confidential information and trade secrets of
	Bitwise Systems and is protected by United States and international
	copyright laws.  Use, disclosure, or reproduction is prohibited
	without the prior express written permission of Bitwise Systems,
	except as agreed in the QuickUSB license agreement.

	Use, duplication or disclosure by the U.S. Government is subject to
	restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
	(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle
	Real, Suite E, Goleta, CA  93117.

	Bitwise Systems
	6489 Calle Real, Suite E
	Santa Barbara, CA  93117
	Voice: (805) 683-6469
	Fax  : (805) 683-4833
	Web  : www.bitwisesys.com
	email: support@bitwisesys.com
   ======================================================================== */
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace QuickUsbDiagCs
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		/// 

		static QuickUsbDiag qusbUsbDiag;

		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			qusbUsbDiag = new QuickUsbDiag();
			Application.Run(qusbUsbDiag);
		}

		static public QuickUsbDiag QuickUsbDiag {
			get { return qusbUsbDiag; }
		}
	}
}