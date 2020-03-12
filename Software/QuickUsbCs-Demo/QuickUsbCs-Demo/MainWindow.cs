using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BitwiseSystems;


namespace QuickUsbCs_Demo
{
    public partial class Form1 : Form
    {
        // This object is nameless and won't work until named or opened.
        QuickUsb qusb = new QuickUsb();

        public Form1()
        {
            InitializeComponent();

            // This is how you call FindModules to get the names of all the connected modules
            string[] moduleNames = QuickUsb.FindModules();
            listBox1.Items.AddRange(moduleNames);
            listBox1.SelectedIndex = 0;
        }

        private void writeDataButton_Click(object sender, EventArgs e)
        {
            bool result;
            byte[] theData = new byte[16];

            // This is how you write data

            result = qusb.WriteData(theData, (uint)theData.Length);
            if (result)
            {
                toolStripStatusLabel.Text = "Wrote " + theData.Length + " bytes";
            }
            else
            {
                toolStripStatusLabel.Text = "WriteData failed";
            }
        }

        private void readDataButton_Click(object sender, EventArgs e)
        {
            bool result;
            byte[] theData = new byte[16];
            uint len = (uint) theData.Length;

            result = qusb.ReadData(theData, ref len);
            if (result)
            {
                toolStripStatusLabel.Text = "Read " + len + " bytes";
            }
            else
            {
                toolStripStatusLabel.Text = "ReadData failed";
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            qusb.Name = listBox1.SelectedItem.ToString();
            makeTextBox.Text = qusb.Make;
            modelTextBox.Text = qusb.Model;
            serialTextBox.Text = qusb.Serial;
        }
    }
}
