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
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.Collections.Generic;


namespace QuickUsbDiagCs
{
	public delegate void ValueChangedHandler(object sender, uint value);
	
	public partial class BitTwiddler : UserControl
	{
		const int space = 5;
		const int pad = 2;
        const int cbWidth = 20;
		byte m_bits = 8;
		int m_captionWidth = 75;
		int m_tagint = 0;
		uint m_value = 0;
		string m_caption = "bits";
		Label label;
		CheckBox[] checkBoxes;
		TextBox textEdit;
        string[] m_tooltips;

        public String [] ToolTips
        {
            get
            {
                return m_tooltips;
            }

            set
            {
                m_tooltips = value;

                SetToolTips();
            }
        }

        public CheckBox[] CheckBoxes
        {
            get
            {
                return checkBoxes;
            }
        }

		public event ValueChangedHandler ValueChanged;

		public BitTwiddler()
		{
			InitializeComponent();
		}

        private void SetToolTips()
        {
            if (m_tooltips != null && checkBoxes != null)
            {
                ToolTip tt = new ToolTip();
                for (int k = 0; k < Math.Min(m_tooltips.Length, checkBoxes.Length); ++k)
                {
                    tt.SetToolTip(checkBoxes[k], m_tooltips[k]);
                }
            }
        }

		protected override void OnCreateControl()
		{
			// Make the control autosize to the size of the components
			AutoSize = true;
			AutoSizeMode = AutoSizeMode.GrowAndShrink;
			
			label = new Label();
			label.Name = "caption";
			label.Text = m_caption;
			label.Location = new Point(3, 4);
			label.Size = new Size(m_captionWidth+space, 14);
			label.Margin = new Padding(pad);
			Controls.Add(label);

			checkBoxes = new CheckBox[m_bits];
			for (int i = 0; i < m_bits; i++) {
				checkBoxes[i] = new CheckBox();
				checkBoxes[i].Name = "checkBox" + i;
                checkBoxes[i].Text = "checkBox" + i;
				checkBoxes[i].TabIndex = i;
                checkBoxes[i].Size = new Size(cbWidth, checkBoxes[i].Height);
                checkBoxes[i].Location = new Point(3 + space + m_captionWidth + (((m_bits - 1) - i) * cbWidth), 4);
				checkBoxes[i].Margin = new Padding(pad);
				checkBoxes[i].Click += new EventHandler(BitTwiddler_Click);
			}
			Controls.AddRange(checkBoxes);

			textEdit = new TextBox();
			textEdit.Name = "textEdit";
            textEdit.Location = new Point(3 + space + m_captionWidth + (m_bits * cbWidth) + space, 4);
			textEdit.Margin = new Padding(pad);
			textEdit.TextAlign = HorizontalAlignment.Right;
            textEdit.ReadOnly = true;
            textEdit.TextAlign = HorizontalAlignment.Center;

			UpdateCheckBoxes();
			UpdateTextEdit();
			Controls.Add(textEdit);

            SetToolTips();

 			base.OnCreateControl();
		}

		private void BitTwiddler_Load(object sender, EventArgs e)
		{
			UpdateCheckBoxes();
			UpdateTextEdit();
		}
	   
		protected virtual void OnValueChanged(uint value)
		{
			ValueChangedHandler handler = ValueChanged;
			if (handler != null) {
				handler(this, value);
			}
		}

		private void UpdateCheckBoxes()
		{
			for (int i = 0; i < m_bits; i++) {
				checkBoxes[i].Checked = (m_value & (1 << i)) != 0 ? true : false;
			}
		}

		private void UpdateTextEdit()
		{
			textEdit.Text = String.Format("0x{0:X" + m_bits / 4 + "}", m_value);
            Size size = TextRenderer.MeasureText("0xFFFF", textEdit.Font);
			textEdit.Size = new Size(size.Width + 5, size.Height);
		}

		void BitTwiddler_Click(object sender, EventArgs e)
		{
			CheckBox cb = (CheckBox) sender;
			
			if (cb.Checked == true) {
				m_value |= (uint)(1 <<  cb.TabIndex);
			}
			else {
				m_value &= ~(uint)(1 << cb.TabIndex);
			}
			UpdateTextEdit();
			OnValueChanged(m_value);
		}

		public string Caption
		{
			get {
				return m_caption;
			}
			set {
				m_caption = value;
				if (label != null)
				{
					label.Text = m_caption;
				}
			}
		}

		public int CaptionWidth
		{
			get {
				return m_captionWidth;
			}
			set {
				m_captionWidth = value;
				if (label != null)
				{
					label.Size = new Size(m_captionWidth + space, 14);
				}
				if (checkBoxes != null)
				{
					for (int i = 0; i < m_bits; i++)
					{
                        checkBoxes[i].Location = new Point(3 + space + m_captionWidth + (((m_bits - 1) - i) * cbWidth), 4);
					}
				}
				if (textEdit != null)
				{
                    textEdit.Location = new Point(3 + space + m_captionWidth + (m_bits * cbWidth) + space, 1);
				}
			}
		}

		public byte Bits
		{
			get {
				return m_bits;
			}
			set {
				m_bits = value;
			}
		}

		public uint Value
		{
			get {
				return m_value;
			}
			set {
				m_value = value;
				try {
					UpdateCheckBoxes();
					UpdateTextEdit();
				}
				catch { }
			}
		}

		public string TextValue
		{
			get
			{
				return textEdit.Text;
			}
		}

        public int TagInt
		{
			get
			{
				return m_tagint;
			}
			set
			{
				m_tagint = value;
			}
		}
	}
}
