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
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BitwiseSystems;
using System.IO;

namespace QuickUsbStorageCs
{
	public partial class QuickUsbDiagCs : Form
	{
		// The number of columns in the EEPROM explorer grid
		private const int numByteCols = 8;

		private Queue<string> statusText = new Queue<string>();

		public QuickUsbDiagCs()
		{
			InitializeComponent();

			// Setup status bar
			statusTimer.Stop();
			toolStripStatusLabel.Text = String.Empty;

			// Setup the EEPROM grin control
			int k, j;
			for (k = 0; k < numByteCols; ++k)
			{
				dgvEeprom.Columns.Add(String.Format("0x{0:x2}", k), String.Format("0x{0:x2}", k));
				dgvEeprom.Columns[k].SortMode = DataGridViewColumnSortMode.NotSortable;
			}
			dgvEeprom.Rows.Add(2048 / numByteCols);
			for (k = 0; k < dgvEeprom.Rows.Count; ++k)
			{
				dgvEeprom.Rows[k].HeaderCell.Value = String.Format("0x{0:x4}", (k * numByteCols));
				dgvEeprom.AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode.AutoSizeToAllHeaders);
				for (j = 0; j < numByteCols; ++j)
				{
					dgvEeprom.Rows[k].Cells[j].Value = "0x00";
				}
			}
			dgvEeprom.Enabled = (qusbControl.SelectedModules.Length == 1);
		}

		private void bReadToFile_Click(object sender, EventArgs e)
		{
			// Show the save as file dialog
			using (var diag = new SaveFileDialog())
			{
				diag.Filter = "Raw data file (*.txt)|*.txt|All files (*.*)|*.*";
				diag.OverwritePrompt = true;
				if (diag.ShowDialog() == DialogResult.OK)
				{
					// Read all eeprom and write to a file
					DisplayStatusMessage("Reading EEPROM...");
					Application.DoEvents();

					byte[] memory;
					memory = new byte[2048];
					if (qusbControl.SelectedModule.ReadStorage(0, memory, (ushort)memory.Length))
					{
						using (var fout = new BinaryWriter(diag.OpenFile()))
						{
							fout.Write(memory, 0, memory.Length);
						}
					}
					else
					{
						DisplayStatusMessage(String.Format("QuickMux.ReadEeprom returned error code: QuickMux.Error.{0}", QuickUsb.GetLastError().ToString()));
					}
				}
			}
		}

		private void bWriteFromFile_Click(object sender, EventArgs e)
		{
			// Show the open file dialog
			using (var diag = new OpenFileDialog())
			{
				diag.Filter = "Raw data file (*.txt)|*.txt|All files (*.*)|*.*";
				if (diag.ShowDialog() == DialogResult.OK)
				{
					// Read file
					byte[] memory = new byte[2048];
					ushort bytes;
					using (var fout = diag.OpenFile())
					{
						bytes = (ushort)fout.Read(memory, 0, memory.Length);
					}

					// Write the data to eeprom
					DisplayStatusMessage("Writing EEPROM...");
					Application.DoEvents();

					Array.Resize(ref memory, bytes);
					if (!qusbControl.SelectedModule.WriteStorage(0, memory, (ushort)memory.Length))
					{
						DisplayStatusMessage(String.Format("QuickMux.WriteEeprom returned error code: QuickMux.Error.{0}", QuickUsb.GetLastError().ToString()));
						return;
					}

					// Now perform a read to update the grid with the actual memory contents
					readEepromToGrid();
				}
			}
		}

		private void bClearAll_Click(object sender, EventArgs e)
		{
			if (MessageBox.Show("Are you sure you want to clear all memory?  This set all user EEPROM to zero.", "Clear Memory", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
			{
				// Clear eeprom
				DisplayStatusMessage("Clearing EEPROM...");

				byte[] memory = new byte[2048];
				for (int k = 0; k < 2048; ++k)
				{
					memory[k] = 0xff;
				}
				if (!qusbControl.SelectedModule.WriteStorage(0, memory, (ushort)memory.Length))
				{
					DisplayStatusMessage(String.Format("QuickMux.WriteEeprom returned error code: QuickMux.Error.{0}", QuickUsb.GetLastError()));
					return;
				}

				// Now perform a read to update the grid with the actual memory contents
				readEepromToGrid();
			}
		}

		private void dgvEeprom_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			byte value;
			try
			{
				value = Convert.ToByte(e.FormattedValue.ToString(), 16);
			}
			catch
			{
				dgvEeprom.Rows[e.RowIndex].ErrorText = "You must enter a valid 1-byte hex number";
				e.Cancel = true;
			}

		}

		private void dgvEeprom_CellEndEdit(object sender, DataGridViewCellEventArgs e)
		{
			byte value = Convert.ToByte(dgvEeprom[e.ColumnIndex, e.RowIndex].Value.ToString(), 16);

			// Write new value
			byte[] mem = new byte[1] { value };
			qusbControl.SelectedModule.WriteStorage((ushort)((e.RowIndex * numByteCols) + e.ColumnIndex), mem, (ushort)mem.Length);

			// Read back value
			qusbControl.SelectedModule.ReadStorage((ushort)((e.RowIndex * numByteCols) + e.ColumnIndex), mem, (ushort)mem.Length);
			dgvEeprom[e.ColumnIndex, e.RowIndex].Value = String.Format("0x{0}", mem[0].ToString("X2"));

			// Clear any cell error messages
			dgvEeprom.Rows[e.RowIndex].ErrorText = String.Empty;
		}

		private void readEepromToGrid()
		{
			dgvEeprom.Enabled = (qusbControl.SelectedModules.Length == 1);
			if (qusbControl.SelectedModules.Length != 1)
			{
				return;
			}

			DisplayStatusMessage("Reading EEPROM...");

			byte[] memory = new byte[2048];
			if (!qusbControl.SelectedModule.ReadStorage(0, memory, (ushort)memory.Length))
			{
				DisplayStatusMessage(String.Format("QuickMux.ReadEeprom returned error code: QuickMux.Error.{0}", QuickUsb.GetLastError().ToString()));
			}
			else
			{
				// Write memory contents to grid control (This process is way too slow!)
				DisplayStatusMessage("Filling in grid...");
				int k, j;
				for (k = 0; k < dgvEeprom.Rows.Count; ++k)
				{
					for (j = 0; j < numByteCols; ++j)
					{
						dgvEeprom.Rows[k].Cells[j].Value = String.Format("0x{0}", memory[(k * numByteCols) + j].ToString("X2"));
					}
					Application.DoEvents();
				}
			}
		}

		private void DisplayStatusMessage(string text)
		{
			if (toolStripStatusLabel.Text == String.Empty)
			{
				toolStripStatusLabel.Text = text;
			}
			else
			{
				if ((statusText.Count == 0 && text == toolStripStatusLabel.Text) || (statusText.Count != 0 && statusText.Peek() == text))
				{
					return;
				}
				statusText.Enqueue(text);
			}
			statusTimer.Start();
			Application.DoEvents();
		}

		private void statusTimer_Tick(object sender, EventArgs e)
		{
			if (statusText.Count != 0)
			{
				toolStripStatusLabel.Text = statusText.Dequeue();
			}
			else
			{
				toolStripStatusLabel.Text = String.Empty;
				statusTimer.Stop();
			}
		}

		private void qusbControl_SelectionChanged(object sender, EventArgs e)
		{
			readEepromToGrid();
		}
	}
}
