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
using System.ComponentModel;
using System.Drawing;
using System.Reflection;
using System.Windows.Forms;

namespace BitwiseSystems
{
    partial class AboutBox : Form
    {
        public AboutBox()
        {
			Version v;

            InitializeComponent();

            // Dialog title
            this.Text = String.Format("About {0}", AssemblyTitle);

            // Copyright
            this.labelCopyright.Text = AssemblyCopyright;

            // Application name
			v = new Version(AssemblyVersion);
			this.labelProductName.Text = String.Format("{0} v{1}.{2}.{3}", AssemblyProduct, v.Major, v.Minor, v.Build);

            // Assembly version
			v = Assembly.GetAssembly(typeof(QuickUsb)).GetName().Version;
            this.labelAssemblyVersion.Text = String.Format("QuickUSB Assembly v{0}.{1}.{2}", v.Major, v.Minor, v.Build);

            // DLL Version
			ushort major, minor, build;
			QuickUsb.GetDllVersion(out major, out minor, out build);
            this.labelDllVersion.Text = String.Format("QuickUSB DLL v{0}.{1}.{2}", major, minor, build);

            // Driver Version
            QuickUsb.GetDriverVersion(out major, out minor, out build);
            this.labelDriverVersion.Text = String.Format("QuickUSB Driver v{0}.{1}.{2}", major, minor, build);
        }

        #region Assembly Attribute Accessors

        static public string AssemblyTitle
        {
            get
            {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
                if (attributes.Length > 0)
                {
                    AssemblyTitleAttribute titleAttribute = (AssemblyTitleAttribute)attributes[0];
                    if (titleAttribute.Title != "")
                    {
                        return titleAttribute.Title;
                    }
                }
                return System.IO.Path.GetFileNameWithoutExtension(Assembly.GetExecutingAssembly().CodeBase);
            }
        }

        static public string AssemblyVersion
        {
            get
            {
                return Assembly.GetExecutingAssembly().GetName().Version.ToString();
            }
        }

        static public string AssemblyDescription
        {
            get
            {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyDescriptionAttribute), false);
                if (attributes.Length == 0)
                {
                    return "";
                }
                return ((AssemblyDescriptionAttribute)attributes[0]).Description;
            }
        }

        static public string AssemblyProduct
        {
            get
            {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyProductAttribute), false);
                if (attributes.Length == 0)
                {
                    return "";
                }
                return ((AssemblyProductAttribute)attributes[0]).Product;
            }
        }

        static public string AssemblyCopyright
        {
            get
            {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false);
                if (attributes.Length == 0)
                {
                    return "";
                }
                return ((AssemblyCopyrightAttribute)attributes[0]).Copyright;
            }
        }

        static public string AssemblyCompany
        {
            get
            {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCompanyAttribute), false);
                if (attributes.Length == 0)
                {
                    return "";
                }
                return ((AssemblyCompanyAttribute)attributes[0]).Company;
            }
        }

        #endregion

        private void okButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}
