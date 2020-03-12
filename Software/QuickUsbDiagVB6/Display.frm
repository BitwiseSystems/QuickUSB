VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0ECD9B60-23AA-11D0-B351-00A0C9055D8E}#6.0#0"; "mshflxgd.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "tabctl32.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Display 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB Diagnostics"
   ClientHeight    =   7335
   ClientLeft      =   150
   ClientTop       =   780
   ClientWidth     =   7755
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Display.frx":0000
   LinkTopic       =   "QuickUsb"
   MaxButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   7335
   ScaleWidth      =   7755
   StartUpPosition =   3  'Windows Default
   WhatsThisHelp   =   -1  'True
   Begin VB.TextBox RepeatPeriodTextBox 
      Height          =   315
      Left            =   1680
      TabIndex        =   80
      Text            =   "500"
      Top             =   6600
      Width           =   735
   End
   Begin VB.Timer RepeatTimer 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   3240
      Top             =   6540
   End
   Begin VB.CheckBox RepeatCheckBox 
      Caption         =   "Repeat"
      Height          =   315
      Left            =   120
      TabIndex        =   77
      Top             =   6600
      Width           =   855
   End
   Begin VB.Timer StatusTimer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   4980
      Top             =   6540
   End
   Begin TabDlg.SSTab SSTab 
      Height          =   6435
      Left            =   120
      TabIndex        =   1
      Top             =   60
      Width           =   7515
      _ExtentX        =   13256
      _ExtentY        =   11351
      _Version        =   393216
      Style           =   1
      Tabs            =   8
      Tab             =   1
      TabsPerRow      =   8
      TabHeight       =   573
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Verdana"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      TabCaption(0)   =   "General"
      TabPicture(0)   =   "Display.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Frame8"
      Tab(0).Control(1)=   "GeneralFrame"
      Tab(0).ControlCount=   2
      TabCaption(1)   =   "Cmd/Data"
      TabPicture(1)   =   "Display.frx":045E
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "Frame2"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "Frame1"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "Frame17"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "Frame15"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "WordWideCheckBox"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).ControlCount=   5
      TabCaption(2)   =   "Ports"
      TabPicture(2)   =   "Display.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Frame4"
      Tab(2).ControlCount=   1
      TabCaption(3)   =   "RS-232"
      TabPicture(3)   =   "Display.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "Frame5"
      Tab(3).Control(1)=   "Frame6"
      Tab(3).Control(2)=   "Frame11"
      Tab(3).ControlCount=   3
      TabCaption(4)   =   "SPI"
      TabPicture(4)   =   "Display.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame9"
      Tab(4).Control(1)=   "Frame10"
      Tab(4).Control(2)=   "SpiEndianCheckBox"
      Tab(4).Control(3)=   "SpiPolarityCheckBox"
      Tab(4).Control(4)=   "SpiPhaseCheckBox"
      Tab(4).ControlCount=   5
      TabCaption(5)   =   "I2C"
      TabPicture(5)   =   "Display.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "Frame3"
      Tab(5).Control(1)=   "Frame7"
      Tab(5).ControlCount=   2
      TabCaption(6)   =   "Settings"
      TabPicture(6)   =   "Display.frx":04EA
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "Frame19(0)"
      Tab(6).ControlCount=   1
      TabCaption(7)   =   "Defaults"
      TabPicture(7)   =   "Display.frx":0506
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "Frame19(1)"
      Tab(7).ControlCount=   1
      Begin VB.Frame Frame19 
         Caption         =   "Power-Up Defaults"
         Height          =   5655
         Index           =   1
         Left            =   -74820
         TabIndex        =   155
         Top             =   480
         Width           =   7155
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   156
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP2CFG"
            ToolTipTextBit0 =   "Buf0 - EP2 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit1 =   "Buf1 - EP2 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "EP2 Buffer Size: 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "Type0 - EP2 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit5 =   "Type1 - EP2 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit6 =   "EP2 Direction: 0=Output, 1=Input"
            ToolTipTextBit7 =   "EP2 Valid: 0=Endpoint not activated, 1=Endpoint activated"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   3600
            TabIndex        =   157
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP6CFG"
            ToolTipTextBit0 =   "Buf0 - EP6 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit1 =   "Buf1 - EP6 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "EP6 Buffer Size: 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "Type0 - EP6 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit5 =   "Type1 - EP6 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit6 =   "EP6 Direction: 0=Output, 1=Input"
            ToolTipTextBit7 =   "EP6 Valid: 0=Endpoint not activated, 1=Endpoint activated"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   3600
            TabIndex        =   158
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IFCONFIG"
            ToolTipTextBit0 =   "IFCFG0 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPIF master mode, 11=Slave FIFO mode"
            ToolTipTextBit1 =   "IFCFG1 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPIF master mode, 11=Slave FIFO mode"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "ASYNC - GPIF Clock Mode Select: 0=Synchronous GPIF, 1=Asynchronous GPIF"
            ToolTipTextBit4 =   "IFCLKPOL - IFCLK Polarity: 0=Normal, 1=Inverted"
            ToolTipTextBit5 =   "IFCLKOE - IFCLK Output Enable: 0=Tri-state IFCLK pin, 1=Drive IFCLK pin"
            ToolTipTextBit6 =   "3048MHZ - IFCLK Speed Select: 0=30 MHz, 1=48 MHz"
            ToolTipTextBit7 =   "IFCLKSRC - IFCLK Source Select: 0=External, 1=Internal"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   159
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FifoPinPolar"
            ToolTipTextBit0 =   "FF - FIFO Full Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit1 =   "FE - FIFO Empty Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit2 =   "SLWR - FIFO Write Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit3 =   "SLRD - FIFO Read Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit4 =   "SLOE - FIFO Output Enable Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit5 =   "PKTEND - FIFO Packet End Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   3600
            TabIndex        =   160
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Data Address Bit 0"
            ToolTipTextBit1 =   "HSPP Data Address Bit 1"
            ToolTipTextBit2 =   "HSPP Data Address Bit 2"
            ToolTipTextBit3 =   "HSPP Data Address Bit 3"
            ToolTipTextBit4 =   "HSPP Data Address Bit 4"
            ToolTipTextBit5 =   "HSPP Data Address Bit 5"
            ToolTipTextBit6 =   "HSPP Data Address Bit 6"
            ToolTipTextBit7 =   "HSPP Data Address Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   161
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Data Address Bit 8"
            ToolTipTextBit1 =   "Unused"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "0=Enable address bus, 1=Disable address bus (C[7:0] and E[7] may be used as GPIO)"
            ToolTipTextBit7 =   "0=Increment address bus, 1=Don't increment address bus"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   3600
            TabIndex        =   162
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "HSPP WORDWIDE Data Width: 0=8 bites, 1=16 bits"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   163
            ToolTipText     =   "Not Currently Used"
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "Unused"
            ToolTipTextBit1 =   "Unused"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   3600
            TabIndex        =   164
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "EP2 Full Flag"
            ToolTipTextBit1 =   "EP2 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY1"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   165
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "EP6 Full Flag"
            ToolTipTextBit1 =   "EP6 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY0"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   3600
            TabIndex        =   166
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SPICONFIG"
            ToolTipTextBit0 =   "SPIENDIAN - SPI Bit Order: 0=LSBit to MSBit, 1=MSBit to LSBit"
            ToolTipTextBit1 =   "SPICPOL - SPI Clock Polarity: 0=Normal, 1=Inverted"
            ToolTipTextBit2 =   "SPICPHA - SPI Clock Phase for Input Sampling: 0=Sample then clock, 1=Clock then sample"
            ToolTipTextBit3 =   "SPIPORT - Select Port for SPI/FPGA communication: 0=Use Port E, 1=Use Port A"
            ToolTipTextBit4 =   "NCEPIN - Select the pin for the nCE line: 0=nCE is on pin 2 of SPIPORT, 1=nCE is on pin 7 of SPIPORT"
            ToolTipTextBit5 =   "MISOPIN - Select the port for the MISO line: 0=MISO is on pin 5 of SPIPORT, 1=MISO is on pin 2 of SPIPORT"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   167
            ToolTipText     =   "I/O Port E Alternate Configuration"
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTECFG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "GPIFA8 - Enabled GPIF Address Pin: 0=Configure PE7 as GPIO, 1=Configure PE7 as GPIFADR8 output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   3600
            TabIndex        =   168
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "CPUCONFIG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE - CLKOUT Output Enable: 0=Tri-state the CLKOUT pin, 1=Drive the CLKOUT pin"
            ToolTipTextBit2 =   "CLKINV - Invert CLKOUT: 0=Normal, 1=Invert"
            ToolTipTextBit3 =   "CLKSPD0 - CPU Clock Speed: 00=12 MHz, 01=24 MHz, 10=48 MHz, 11=Reserved"
            ToolTipTextBit4 =   "CLKSPD1 - CPU Clock Speed: 00=12 MHz, 01=24 MHz, 10=48 MHz, 11=Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   169
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "BusSpeed"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "USB Bus Speed: 0=Fource High-Speed (12 Mbps), 1=Allow High-Speed (480 Mbps)"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   3600
            TabIndex        =   170
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "FPGATYPE: 0=Altera Passive Serial, 1=Xilinx Slave Serial"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   171
            ToolTipText     =   "Not Currently Used"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   3600
            TabIndex        =   172
            ToolTipText     =   "Port A Data"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOA"
            ToolTipTextBit0 =   "Port A, Bit 0"
            ToolTipTextBit1 =   "Port A, Bit 1"
            ToolTipTextBit2 =   "Port A, Bit 2"
            ToolTipTextBit3 =   "Port A, Bit 3"
            ToolTipTextBit4 =   "Port A, Bit 4"
            ToolTipTextBit5 =   "Port A, Bit 5"
            ToolTipTextBit6 =   "Port A, Bit 6"
            ToolTipTextBit7 =   "Port A, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   173
            ToolTipText     =   "Port A Output Enable - Set to enable output"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEA"
            ToolTipTextBit0 =   "Port A, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port A, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port A, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port A, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port A, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port A, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port A, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port A, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   3600
            TabIndex        =   174
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "I2C Bus Clock Speed: 0=Approx. 100 kHz, 1=Approx. 400 kHz"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Ignore ACK: 0=Handle ACK for normal I2C traffic, 1=Process I2C traffic even if device doesn't supply ACK"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   175
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "I2C Status, Bit 0: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit1 =   "I2C Status, Bit 1: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit2 =   "I2C Status, Bit 2: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit3 =   "I2C Status, Bit 3: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit4 =   "I2C Status, Bit 4: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit5 =   "I2C Status, Bit 5: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit6 =   "I2C Status, Bit 6: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit7 =   "I2C Status, Bit 7: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   176
            ToolTipText     =   "Port C Output Enable - Set to enable output"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEC"
            ToolTipTextBit0 =   "Port C, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port C, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port C, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port C, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port C, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port C, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port C, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port C, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   177
            ToolTipText     =   "Port B Output Enable - Set to enable output"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEB"
            ToolTipTextBit0 =   "Port B, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port B, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port B, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port B, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port B, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port B, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port B, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port B, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   3600
            TabIndex        =   178
            ToolTipText     =   "Port C Data"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOC"
            ToolTipTextBit0 =   "Port C, Bit 0"
            ToolTipTextBit1 =   "Port C, Bit 1"
            ToolTipTextBit2 =   "Port C, Bit 2"
            ToolTipTextBit3 =   "Port C, Bit 3"
            ToolTipTextBit4 =   "Port C, Bit 4"
            ToolTipTextBit5 =   "Port C, Bit 5"
            ToolTipTextBit6 =   "Port C, Bit 6"
            ToolTipTextBit7 =   "Port C, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   3600
            TabIndex        =   179
            ToolTipText     =   "Port B Data"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOB"
            ToolTipTextBit0 =   "Port B, Bit 0"
            ToolTipTextBit1 =   "Port B, Bit 1"
            ToolTipTextBit2 =   "Port B, Bit 2"
            ToolTipTextBit3 =   "Port B, Bit 3"
            ToolTipTextBit4 =   "Port B, Bit 4"
            ToolTipTextBit5 =   "Port B, Bit 5"
            ToolTipTextBit6 =   "Port B, Bit 6"
            ToolTipTextBit7 =   "Port B, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   3600
            TabIndex        =   180
            ToolTipText     =   "Port E Data"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOE"
            ToolTipTextBit0 =   "Port E, Bit 0"
            ToolTipTextBit1 =   "Port E, Bit 1"
            ToolTipTextBit2 =   "Port E, Bit 2"
            ToolTipTextBit3 =   "Port E, Bit 3"
            ToolTipTextBit4 =   "Port E, Bit 4"
            ToolTipTextBit5 =   "Port E, Bit 5"
            ToolTipTextBit6 =   "Port E, Bit 6"
            ToolTipTextBit7 =   "Port E, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   181
            ToolTipText     =   "Port E Output Enable - Set to enable output"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEE"
            ToolTipTextBit0 =   "Port E, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port E, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port E, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port E, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port E, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port E, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port E, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port E, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   3600
            TabIndex        =   182
            ToolTipText     =   "Port D Data"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOD"
            ToolTipTextBit0 =   "Port D, Bit 0"
            ToolTipTextBit1 =   "Port D, Bit 1"
            ToolTipTextBit2 =   "Port D, Bit 2"
            ToolTipTextBit3 =   "Port D, Bit 3"
            ToolTipTextBit4 =   "Port D, Bit 4"
            ToolTipTextBit5 =   "Port D, Bit 5"
            ToolTipTextBit6 =   "Port D, Bit 6"
            ToolTipTextBit7 =   "Port D, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   183
            ToolTipText     =   "Port D Output Enable - Set to enable output"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OED"
            ToolTipTextBit0 =   "Port D, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port D, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port D, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port D, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port D, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port D, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port D, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port D, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   184
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSAB"
            ToolTipTextBit0 =   "Flag A Select Code, Bit 0"
            ToolTipTextBit1 =   "Flag A Select Code, Bit 1"
            ToolTipTextBit2 =   "Flag A Select Code, Bit 2"
            ToolTipTextBit3 =   "Flag A Select Code, Bit 3"
            ToolTipTextBit4 =   "Flag B Select Code, Bit 0"
            ToolTipTextBit5 =   "Flag B Select Code, Bit 1"
            ToolTipTextBit6 =   "Flag B Select Code, Bit 2"
            ToolTipTextBit7 =   "Flag B Select Code, Bit 3"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   3600
            TabIndex        =   185
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSCD"
            ToolTipTextBit0 =   "Flag C Select Code, Bit 0"
            ToolTipTextBit1 =   "Flag C Select Code, Bit 1"
            ToolTipTextBit2 =   "Flag C Select Code, Bit 2"
            ToolTipTextBit3 =   "Flag C Select Code, Bit 3"
            ToolTipTextBit4 =   "Flag D Select Code, Bit 0"
            ToolTipTextBit5 =   "Flag D Select Code, Bit 1"
            ToolTipTextBit6 =   "Flag D Select Code, Bit 2"
            ToolTipTextBit7 =   "Flag D Select Code, Bit 3"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   3600
            TabIndex        =   186
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTCCFG"
            ToolTipTextBit0 =   "GPIF Address Enable, Bit 0: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit1 =   "GPIF Address Enable, Bit 1: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit2 =   "GPIF Address Enable, Bit 2: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit3 =   "GPIF Address Enable, Bit 3: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit4 =   "GPIF Address Enable, Bit 4: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit5 =   "GPIF Address Enable, Bit 5: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit6 =   "GPIF Address Enable, Bit 6: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit7 =   "GPIF Address Enable, Bit 7: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
         End
         Begin QuickUSBDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   187
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTACFG"
            ToolTipTextBit0 =   "INT0 - Interrupt Configuration: 0=PA0 not configured for interrupt use, 1=Configure as interrupt input (currently not used)"
            ToolTipTextBit1 =   "INT1 - Interrupt Configuration: 0=PA0 not configured for interrupt use, 1=Configure as interrupt input (currently not used)"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "SLCS - Slave FIFO Chip Select: 0=PA7 not configured as SLCS input, 1=PA7 configured as SLCS input"
            ToolTipTextBit7 =   "FLAGD: 0=PA7 not configured for FLAGD status, 1=Configured for FLAGD status"
         End
         Begin VB.Label Label28 
            Caption         =   "LSB"
            Height          =   195
            Left            =   5400
            TabIndex        =   189
            Top             =   240
            Width           =   315
         End
         Begin VB.Label Label27 
            Caption         =   "MSB"
            Height          =   195
            Left            =   1920
            TabIndex        =   188
            Top             =   240
            Width           =   315
         End
      End
      Begin VB.CheckBox SpiPhaseCheckBox 
         Caption         =   "CPHA (0=Sample then clock, 1=Clock then sample)"
         Height          =   195
         Left            =   -74760
         TabIndex        =   151
         Top             =   4800
         Width           =   4275
      End
      Begin VB.CheckBox SpiPolarityCheckBox 
         Caption         =   "CPOL (0=Normal clock, 1=Inverted clock)"
         Height          =   195
         Left            =   -74760
         TabIndex        =   150
         Top             =   4560
         Width           =   4395
      End
      Begin VB.Frame Frame19 
         Caption         =   "Settings"
         Height          =   5655
         Index           =   0
         Left            =   -74820
         TabIndex        =   106
         Top             =   480
         Width           =   7155
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   109
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP2CFG"
            ToolTipTextBit0 =   "Buf0 - EP2 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit1 =   "Buf1 - EP2 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "EP2 Buffer Size: 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "Type0 - EP2 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit5 =   "Type1 - EP2 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit6 =   "EP2 Direction: 0=Output, 1=Input"
            ToolTipTextBit7 =   "EP2 Valid: 0=Endpoint not activated, 1=Endpoint activated"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   3600
            TabIndex        =   110
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP6CFG"
            ToolTipTextBit0 =   "Buf0 - EP6 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit1 =   "Buf1 - EP6 Buffering: 00=Quad, 01=Invalid, 10=Double, 11=Triple"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "EP6 Buffer Size: 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "Type0 - EP6 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit5 =   "Type1 - EP6 Type: 00=Invalid, 01=Isochronous, 10=Bulk, 11=Interrupt"
            ToolTipTextBit6 =   "EP6 Direction: 0=Output, 1=Input"
            ToolTipTextBit7 =   "EP6 Valid: 0=Endpoint not activated, 1=Endpoint activated"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   3600
            TabIndex        =   111
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IFCONFIG"
            ToolTipTextBit0 =   "IFCFG0 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPIF master mode, 11=Slave FIFO mode"
            ToolTipTextBit1 =   "IFCFG1 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPIF master mode, 11=Slave FIFO mode"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "ASYNC - GPIF Clock Mode Select: 0=Synchronous GPIF, 1=Asynchronous GPIF"
            ToolTipTextBit4 =   "IFCLKPOL - IFCLK Polarity: 0=Normal, 1=Inverted"
            ToolTipTextBit5 =   "IFCLKOE - IFCLK Output Enable: 0=Tri-state IFCLK pin, 1=Drive IFCLK pin"
            ToolTipTextBit6 =   "3048MHZ - IFCLK Speed Select: 0=30 MHz, 1=48 MHz"
            ToolTipTextBit7 =   "IFCLKSRC - IFCLK Source Select: 0=External, 1=Internal"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   112
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FifoPinPolar"
            ToolTipTextBit0 =   "FF - FIFO Full Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit1 =   "FE - FIFO Empty Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit2 =   "SLWR - FIFO Write Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit3 =   "SLRD - FIFO Read Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit4 =   "SLOE - FIFO Output Enable Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit5 =   "PKTEND - FIFO Packet End Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   3600
            TabIndex        =   113
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Data Address Bit 0"
            ToolTipTextBit1 =   "HSPP Data Address Bit 1"
            ToolTipTextBit2 =   "HSPP Data Address Bit 2"
            ToolTipTextBit3 =   "HSPP Data Address Bit 3"
            ToolTipTextBit4 =   "HSPP Data Address Bit 4"
            ToolTipTextBit5 =   "HSPP Data Address Bit 5"
            ToolTipTextBit6 =   "HSPP Data Address Bit 6"
            ToolTipTextBit7 =   "HSPP Data Address Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   114
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Data Address Bit 8"
            ToolTipTextBit1 =   "Unused"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "0=Enable address bus, 1=Disable address bus (C[7:0] and E[7] may be used as GPIO)"
            ToolTipTextBit7 =   "0=Increment address bus, 1=Don't increment address bus"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   3600
            TabIndex        =   115
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "HSPP WORDWIDE Data Width: 0=8 bites, 1=16 bits"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   116
            ToolTipText     =   "Not Currently Used"
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "Unused"
            ToolTipTextBit1 =   "Unused"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   3600
            TabIndex        =   117
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "EP2 Full Flag"
            ToolTipTextBit1 =   "EP2 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY1"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   118
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "EP6 Full Flag"
            ToolTipTextBit1 =   "EP6 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY0"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   3600
            TabIndex        =   119
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SPICONFIG"
            ToolTipTextBit0 =   "SPIENDIAN - SPI Bit Order: 0=LSBit to MSBit, 1=MSBit to LSBit"
            ToolTipTextBit1 =   "SPICPOL - SPI Clock Polarity: 0=Normal, 1=Inverted"
            ToolTipTextBit2 =   "SPICPHA - SPI Clock Phase for Input Sampling: 0=Sample then clock, 1=Clock then sample"
            ToolTipTextBit3 =   "SPIPORT - Select Port for SPI/FPGA communication: 0=Use Port E, 1=Use Port A"
            ToolTipTextBit4 =   "NCEPIN - Select the pin for the nCE line: 0=nCE is on pin 2 of SPIPORT, 1=nCE is on pin 7 of SPIPORT"
            ToolTipTextBit5 =   "MISOPIN - Select the port for the MISO line: 0=MISO is on pin 5 of SPIPORT, 1=MISO is on pin 2 of SPIPORT"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   120
            ToolTipText     =   "I/O Port E Alternate Configuration"
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTECFG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "GPIFA8 - Enabled GPIF Address Pin: 0=Configure PE7 as GPIO, 1=Configure PE7 as GPIFADR8 output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   3600
            TabIndex        =   121
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "CPUCONFIG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE - CLKOUT Output Enable: 0=Tri-state the CLKOUT pin, 1=Drive the CLKOUT pin"
            ToolTipTextBit2 =   "CLKINV - Invert CLKOUT: 0=Normal, 1=Invert"
            ToolTipTextBit3 =   "CLKSPD0 - CPU Clock Speed: 00=12 MHz, 01=24 MHz, 10=48 MHz, 11=Reserved"
            ToolTipTextBit4 =   "CLKSPD1 - CPU Clock Speed: 00=12 MHz, 01=24 MHz, 10=48 MHz, 11=Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   122
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "BusSpeed"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "USB Bus Speed: 0=Fource High-Speed (12 Mbps), 1=Allow High-Speed (480 Mbps)"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   3600
            TabIndex        =   123
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "FPGATYPE: 0=Altera Passive Serial, 1=Xilinx Slave Serial"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   124
            ToolTipText     =   "Not Currently Used"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   3600
            TabIndex        =   125
            ToolTipText     =   "Port A Data"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOA"
            ToolTipTextBit0 =   "Port A, Bit 0"
            ToolTipTextBit1 =   "Port A, Bit 1"
            ToolTipTextBit2 =   "Port A, Bit 2"
            ToolTipTextBit3 =   "Port A, Bit 3"
            ToolTipTextBit4 =   "Port A, Bit 4"
            ToolTipTextBit5 =   "Port A, Bit 5"
            ToolTipTextBit6 =   "Port A, Bit 6"
            ToolTipTextBit7 =   "Port A, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   126
            ToolTipText     =   "Port A Output Enable - Set to enable output"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEA"
            ToolTipTextBit0 =   "Port A, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port A, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port A, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port A, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port A, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port A, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port A, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port A, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   3600
            TabIndex        =   127
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "I2C Bus Clock Speed: 0=Approx. 100 kHz, 1=Approx. 400 kHz"
            ToolTipTextBit1 =   "Reserved"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "Reserved"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Ignore ACK: 0=Handle ACK for normal I2C traffic, 1=Process I2C traffic even if device doesn't supply ACK"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   128
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "I2C Status, Bit 0: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit1 =   "I2C Status, Bit 1: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit2 =   "I2C Status, Bit 2: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit3 =   "I2C Status, Bit 3: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit4 =   "I2C Status, Bit 4: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit5 =   "I2C Status, Bit 5: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit6 =   "I2C Status, Bit 6: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
            ToolTipTextBit7 =   "I2C Status, Bit 7: 0110=Bus error, 0111=No ACK, 1000=Normal completion, 1010=Slave wait, 1011=Timeout"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   129
            ToolTipText     =   "Port C Output Enable - Set to enable output"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEC"
            ToolTipTextBit0 =   "Port C, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port C, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port C, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port C, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port C, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port C, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port C, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port C, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   130
            ToolTipText     =   "Port B Output Enable - Set to enable output"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEB"
            ToolTipTextBit0 =   "Port B, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port B, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port B, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port B, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port B, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port B, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port B, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port B, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   3600
            TabIndex        =   131
            ToolTipText     =   "Port C Data"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOC"
            ToolTipTextBit0 =   "Port C, Bit 0"
            ToolTipTextBit1 =   "Port C, Bit 1"
            ToolTipTextBit2 =   "Port C, Bit 2"
            ToolTipTextBit3 =   "Port C, Bit 3"
            ToolTipTextBit4 =   "Port C, Bit 4"
            ToolTipTextBit5 =   "Port C, Bit 5"
            ToolTipTextBit6 =   "Port C, Bit 6"
            ToolTipTextBit7 =   "Port C, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   3600
            TabIndex        =   132
            ToolTipText     =   "Port B Data"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOB"
            ToolTipTextBit0 =   "Port B, Bit 0"
            ToolTipTextBit1 =   "Port B, Bit 1"
            ToolTipTextBit2 =   "Port B, Bit 2"
            ToolTipTextBit3 =   "Port B, Bit 3"
            ToolTipTextBit4 =   "Port B, Bit 4"
            ToolTipTextBit5 =   "Port B, Bit 5"
            ToolTipTextBit6 =   "Port B, Bit 6"
            ToolTipTextBit7 =   "Port B, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   3600
            TabIndex        =   133
            ToolTipText     =   "Port E Data"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOE"
            ToolTipTextBit0 =   "Port E, Bit 0"
            ToolTipTextBit1 =   "Port E, Bit 1"
            ToolTipTextBit2 =   "Port E, Bit 2"
            ToolTipTextBit3 =   "Port E, Bit 3"
            ToolTipTextBit4 =   "Port E, Bit 4"
            ToolTipTextBit5 =   "Port E, Bit 5"
            ToolTipTextBit6 =   "Port E, Bit 6"
            ToolTipTextBit7 =   "Port E, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   134
            ToolTipText     =   "Port E Output Enable - Set to enable output"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEE"
            ToolTipTextBit0 =   "Port E, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port E, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port E, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port E, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port E, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port E, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port E, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port E, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   3600
            TabIndex        =   135
            ToolTipText     =   "Port D Data"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOD"
            ToolTipTextBit0 =   "Port D, Bit 0"
            ToolTipTextBit1 =   "Port D, Bit 1"
            ToolTipTextBit2 =   "Port D, Bit 2"
            ToolTipTextBit3 =   "Port D, Bit 3"
            ToolTipTextBit4 =   "Port D, Bit 4"
            ToolTipTextBit5 =   "Port D, Bit 5"
            ToolTipTextBit6 =   "Port D, Bit 6"
            ToolTipTextBit7 =   "Port D, Bit 7"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   136
            ToolTipText     =   "Port D Output Enable - Set to enable output"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OED"
            ToolTipTextBit0 =   "Port D, Bit 0 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit1 =   "Port D, Bit 1 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit2 =   "Port D, Bit 2 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit3 =   "Port D, Bit 3 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit4 =   "Port D, Bit 4 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit5 =   "Port D, Bit 5 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit6 =   "Port D, Bit 6 Output Enable: 0=Input, 1=Output"
            ToolTipTextBit7 =   "Port D, Bit 7 Output Enable: 0=Input, 1=Output"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   137
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSAB"
            ToolTipTextBit0 =   "Flag A Select Code, Bit 0"
            ToolTipTextBit1 =   "Flag A Select Code, Bit 1"
            ToolTipTextBit2 =   "Flag A Select Code, Bit 2"
            ToolTipTextBit3 =   "Flag A Select Code, Bit 3"
            ToolTipTextBit4 =   "Flag B Select Code, Bit 0"
            ToolTipTextBit5 =   "Flag B Select Code, Bit 1"
            ToolTipTextBit6 =   "Flag B Select Code, Bit 2"
            ToolTipTextBit7 =   "Flag B Select Code, Bit 3"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   3600
            TabIndex        =   138
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSCD"
            ToolTipTextBit0 =   "Flag C Select Code, Bit 0"
            ToolTipTextBit1 =   "Flag C Select Code, Bit 1"
            ToolTipTextBit2 =   "Flag C Select Code, Bit 2"
            ToolTipTextBit3 =   "Flag C Select Code, Bit 3"
            ToolTipTextBit4 =   "Flag D Select Code, Bit 0"
            ToolTipTextBit5 =   "Flag D Select Code, Bit 1"
            ToolTipTextBit6 =   "Flag D Select Code, Bit 2"
            ToolTipTextBit7 =   "Flag D Select Code, Bit 3"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   3600
            TabIndex        =   139
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTCCFG"
            ToolTipTextBit0 =   "GPIF Address Enable, Bit 0: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit1 =   "GPIF Address Enable, Bit 1: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit2 =   "GPIF Address Enable, Bit 2: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit3 =   "GPIF Address Enable, Bit 3: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit4 =   "GPIF Address Enable, Bit 4: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit5 =   "GPIF Address Enable, Bit 5: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit6 =   "GPIF Address Enable, Bit 6: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
            ToolTipTextBit7 =   "GPIF Address Enable, Bit 7: 0=Disable GPIF address (allows for Port C GPIO use), 1=Enable GPIF address"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   140
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTACFG"
            ToolTipTextBit0 =   "INT0 - Interrupt Configuration: 0=PA0 not configured for interrupt use, 1=Configure as interrupt input (currently not used)"
            ToolTipTextBit1 =   "INT1 - Interrupt Configuration: 0=PA0 not configured for interrupt use, 1=Configure as interrupt input (currently not used)"
            ToolTipTextBit2 =   "Unused"
            ToolTipTextBit3 =   "Unused"
            ToolTipTextBit4 =   "Unused"
            ToolTipTextBit5 =   "Unused"
            ToolTipTextBit6 =   "SLCS - Slave FIFO Chip Select: 0=PA7 not configured as SLCS input, 1=PA7 configured as SLCS input"
            ToolTipTextBit7 =   "FLAGD: 0=PA7 not configured for FLAGD status, 1=Configured for FLAGD status"
         End
         Begin VB.Label Label13 
            Caption         =   "MSB"
            Height          =   195
            Left            =   1920
            TabIndex        =   108
            Top             =   240
            Width           =   315
         End
         Begin VB.Label Label14 
            Caption         =   "LSB"
            Height          =   195
            Left            =   5400
            TabIndex        =   107
            Top             =   240
            Width           =   315
         End
      End
      Begin VB.CheckBox WordWideCheckBox 
         Caption         =   "Word Wide"
         Height          =   255
         Left            =   360
         TabIndex        =   37
         ToolTipText     =   "Checked=16 bit command and data transfers, unchecked=8-bit command and data transfers"
         Top             =   6120
         Value           =   1  'Checked
         Width           =   1215
      End
      Begin VB.Frame Frame15 
         Caption         =   "Settings"
         Height          =   855
         Left            =   180
         TabIndex        =   85
         Top             =   4260
         Width           =   7155
         Begin VB.CommandButton SetTimeoutButton 
            Caption         =   "Set Timeout"
            CausesValidation=   0   'False
            Height          =   315
            Left            =   2040
            TabIndex        =   87
            Top             =   300
            Width           =   1095
         End
         Begin VB.TextBox TimeoutTextBox 
            Height          =   315
            Left            =   900
            TabIndex        =   86
            Text            =   "1000"
            Top             =   300
            Width           =   1035
         End
         Begin VB.Label TimeoutLabel 
            Caption         =   "Timeout"
            Height          =   195
            Left            =   180
            TabIndex        =   88
            Top             =   360
            Width           =   615
         End
      End
      Begin VB.Frame Frame17 
         Caption         =   "FIFO Setup"
         Height          =   915
         Left            =   180
         TabIndex        =   84
         Top             =   5160
         Width           =   7155
         Begin VB.Timer SlaveFifoFlagsQueryTimer 
            Enabled         =   0   'False
            Interval        =   1000
            Left            =   6600
            Top             =   120
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingFifoConfigBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   4080
            TabIndex        =   190
            Top             =   240
            Width           =   2295
            _ExtentX        =   4048
            _ExtentY        =   450
            Caption         =   ""
            ToolTipTextBit0 =   "IFCFG0 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPFI master mode, 11=Slave FIFO mode"
            ToolTipTextBit1 =   "IFCFG1 - HSPP Configuration: 00=I/O ports, 01=Reserved, 10=GPFI master mode, 11=Slave FIFO mode"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "ASYNC - GPIF Clock Mode Select: 0=Synchronous GPIF, 1=Asynchronous GPIF"
            ToolTipTextBit4 =   "IFCLKPOL - IFCLK Polarity: 0=Normal, 1=Inverted"
            ToolTipTextBit5 =   "IFCLKOE - IFCLK Output Enable: 0=Tri-state IFCLK pin, 1=Drive IFCLK pin"
            ToolTipTextBit6 =   "3048MHZ - IFCLK Speed Select: 0=30 MHz, 1=48 MHz"
            ToolTipTextBit7 =   "IFCLKSRC - IFCLK Source Select: 0=External, 1=Internal"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SettingFifoConfigBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   360
            TabIndex        =   191
            Top             =   240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FIFOCONFIG"
            ToolTipTextBit0 =   "FF - FIFO Full Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit1 =   "FE - FIFO Empty Flag Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit2 =   "SLWR - FIFO Write Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit3 =   "SLRD - FIFO Read Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit4 =   "SLOE - FIFO Output Enable Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit5 =   "PKTEND - FIFO Packet End Polarity: 0=Active low, 1=Active high"
            ToolTipTextBit6 =   "Unused"
            ToolTipTextBit7 =   "Unused"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SlaveFifoFlagsBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   3960
            TabIndex        =   192
            Top             =   540
            Width           =   2415
            _ExtentX        =   4260
            _ExtentY        =   450
            Caption         =   ""
            ToolTipTextBit0 =   "EP2 Full Flag"
            ToolTipTextBit1 =   "EP2 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY1"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
         Begin QuickUSBDiagnostics.BitCheckBox SlaveFifoFlagsBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   360
            TabIndex        =   193
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "EP6 Full Flag"
            ToolTipTextBit1 =   "EP6 Empty Flag"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "RDY0"
            ToolTipTextBit4 =   "Reserved"
            ToolTipTextBit5 =   "Reserved"
            ToolTipTextBit6 =   "Reserved"
            ToolTipTextBit7 =   "Reserved"
         End
      End
      Begin VB.CheckBox SpiEndianCheckBox 
         Caption         =   "Little/Big endian (0=LSBit to MSBit, 1=MSBit to LSBit)"
         Height          =   195
         Left            =   -74760
         TabIndex        =   82
         Top             =   4320
         Width           =   4455
      End
      Begin VB.Frame Frame11 
         Caption         =   "Write"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2055
         Left            =   -74820
         TabIndex        =   64
         Top             =   3480
         Width           =   7155
         Begin VB.TextBox RS232WriteTextBox 
            Height          =   1215
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   66
            ToolTipText     =   "Contains text to be written to the selected QuickUSB RS-232 port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.CommandButton WriteRS232Button 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   65
            ToolTipText     =   "Write the text below to the selected QuickUSB RS-232 port"
            Top             =   240
            Width           =   975
         End
      End
      Begin VB.Frame Frame6 
         Caption         =   "Read"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2115
         Left            =   -74820
         TabIndex        =   59
         Top             =   1260
         Width           =   7155
         Begin VB.TextBox GetNumTextBox 
            BackColor       =   &H8000000F&
            Height          =   285
            Left            =   1320
            Locked          =   -1  'True
            TabIndex        =   62
            Text            =   "0"
            ToolTipText     =   "Number of characters waiting in the receive buffer"
            Top             =   240
            Width           =   495
         End
         Begin VB.CommandButton ReadRS232Button 
            Caption         =   "Read"
            Height          =   315
            Left            =   5940
            TabIndex        =   61
            ToolTipText     =   "Read the selected QuickUSB RS-232 port"
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox RS232ReadTextBox 
            Height          =   1275
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   60
            ToolTipText     =   "Contains text read from the selected QuickUSB RS-232 port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.Timer GetNumTimer 
            Interval        =   1000
            Left            =   5340
            Top             =   180
         End
         Begin VB.Label Label18 
            Caption         =   "Chars Waiting"
            Height          =   195
            Left            =   180
            TabIndex        =   63
            Top             =   300
            Width           =   1095
         End
      End
      Begin VB.Frame Frame10 
         Caption         =   "Write"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   -74820
         TabIndex        =   54
         Top             =   2400
         Width           =   7155
         Begin VB.CommandButton SPIWriteReadButton 
            Caption         =   "WriteRead"
            Height          =   315
            Left            =   4860
            TabIndex        =   76
            ToolTipText     =   "Sends space delimited data in the 'Write' panel to the selected SPI port while reading MISO"
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox SpiWritePortNumComboBox 
            Height          =   315
            ItemData        =   "Display.frx":0522
            Left            =   840
            List            =   "Display.frx":0544
            TabIndex        =   57
            ToolTipText     =   "Selects the active QuickUSB RS-232 port"
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox SpiWriteTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   56
            ToolTipText     =   "Enter space delimited decimal or hex values (&h style) to send to the selected SPI port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.CommandButton SpiWriteButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   55
            ToolTipText     =   "Sends space delimited data in the 'Write' panel to the selected SPI port"
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label25 
            Caption         =   "Port"
            Height          =   195
            Left            =   120
            TabIndex        =   58
            Top             =   300
            Width           =   795
         End
      End
      Begin VB.Frame Frame9 
         Caption         =   "Read"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   -74820
         TabIndex        =   47
         Top             =   480
         Width           =   7155
         Begin VB.ComboBox SpiReadPortNumComboBox 
            Height          =   315
            ItemData        =   "Display.frx":0566
            Left            =   840
            List            =   "Display.frx":0588
            TabIndex        =   52
            ToolTipText     =   "Selects the active QuickUSB RS-232 port"
            Top             =   240
            Width           =   735
         End
         Begin VB.CommandButton SpiReadButton 
            Caption         =   "Read"
            Height          =   315
            Left            =   5940
            TabIndex        =   50
            ToolTipText     =   "Read data from selected SPI port and displays it in hex in the 'Read' panel"
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox SpiReadTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   49
            ToolTipText     =   "Contains hex values read from the selected SPI port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.TextBox SpiReadLengthTextBox 
            Height          =   315
            Left            =   2400
            TabIndex        =   48
            Text            =   "8"
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label22 
            Caption         =   "Port"
            Height          =   195
            Left            =   180
            TabIndex        =   53
            Top             =   300
            Width           =   795
         End
         Begin VB.Label Label23 
            Alignment       =   1  'Right Justify
            Caption         =   "Length"
            Height          =   195
            Left            =   1740
            TabIndex        =   51
            Top             =   300
            Width           =   555
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "Write"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   -74820
         TabIndex        =   42
         Top             =   2400
         Width           =   7155
         Begin VB.CommandButton I2cCachedWriteButton 
            Caption         =   "Cached Write"
            Height          =   315
            Left            =   4680
            TabIndex        =   204
            Top             =   240
            Width           =   1215
         End
         Begin VB.CommandButton I2cWriteButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   45
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox I2cWriteTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   44
            ToolTipText     =   "Enter decimal or hex values (&h style) to send to the selected I2C port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.TextBox I2cWriteAddressTextBox 
            Height          =   315
            Left            =   840
            TabIndex        =   43
            Text            =   "0"
            ToolTipText     =   "The I2C device address"
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label15 
            Alignment       =   1  'Right Justify
            Caption         =   "Address"
            Height          =   195
            Left            =   120
            TabIndex        =   46
            Top             =   300
            Width           =   615
         End
      End
      Begin VB.Frame Frame8 
         Caption         =   "FPGA"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1635
         Left            =   -74820
         TabIndex        =   29
         Top             =   2280
         Width           =   7155
         Begin VB.ComboBox FpgaTypeComboBox 
            Height          =   315
            ItemData        =   "Display.frx":05AA
            Left            =   1080
            List            =   "Display.frx":05B4
            Style           =   2  'Dropdown List
            TabIndex        =   148
            Top             =   660
            Width           =   2055
         End
         Begin VB.CommandButton IsFpgaConfiguredButton 
            Caption         =   "?"
            Height          =   315
            Left            =   6720
            TabIndex        =   83
            ToolTipText     =   "Check to see if FPGA is configured"
            Top             =   240
            Width           =   255
         End
         Begin VB.CommandButton BrowseButton 
            Caption         =   "Browse"
            Height          =   315
            Left            =   180
            TabIndex        =   75
            Top             =   240
            Width           =   795
         End
         Begin VB.TextBox UploadFilenameTextBox 
            Height          =   285
            Left            =   1080
            TabIndex        =   74
            Top             =   240
            Width           =   4695
         End
         Begin VB.CommandButton FpgaUploadButton 
            Caption         =   "Upload"
            Height          =   315
            Left            =   5880
            TabIndex        =   30
            ToolTipText     =   "Upload an RBF file to the evaluation board FPGA"
            Top             =   240
            Width           =   795
         End
         Begin MSComctlLib.ProgressBar Progress 
            Height          =   75
            Left            =   1080
            TabIndex        =   41
            Top             =   540
            Width           =   4695
            _ExtentX        =   8281
            _ExtentY        =   132
            _Version        =   393216
            Appearance      =   0
            Scrolling       =   1
         End
         Begin VB.Label Label31 
            Caption         =   $"Display.frx":05E4
            Height          =   495
            Left            =   120
            TabIndex        =   154
            Top             =   1080
            Width           =   6975
         End
         Begin VB.Label Label24 
            Caption         =   "FPGA Type"
            Height          =   195
            Left            =   180
            TabIndex        =   149
            Top             =   720
            Width           =   855
         End
      End
      Begin VB.Frame Frame7 
         Caption         =   "Read"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   -74820
         TabIndex        =   20
         Top             =   480
         Width           =   7155
         Begin VB.TextBox I2cReadAddressTextBox 
            Height          =   315
            Left            =   840
            TabIndex        =   25
            Text            =   "0"
            ToolTipText     =   "The I2C device address"
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox I2cReadLengthTextBox 
            Height          =   315
            Left            =   2400
            TabIndex        =   23
            Text            =   "8"
            ToolTipText     =   "The number of bytes to read"
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox I2CReadTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   22
            ToolTipText     =   "Contains hex values read from the selected I2C port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.CommandButton I2CReadButton 
            Caption         =   "Read"
            Height          =   315
            Left            =   5940
            TabIndex        =   21
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label9 
            Alignment       =   1  'Right Justify
            Caption         =   "Address"
            Height          =   195
            Left            =   120
            TabIndex        =   26
            Top             =   300
            Width           =   615
         End
         Begin VB.Label Label8 
            Alignment       =   1  'Right Justify
            Caption         =   "Length"
            Height          =   195
            Left            =   1740
            TabIndex        =   24
            Top             =   300
            Width           =   555
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "General"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   675
         Left            =   -74820
         TabIndex        =   15
         Top             =   480
         Width           =   7155
         Begin VB.CommandButton FlushRS232Button 
            Caption         =   "Flush"
            Height          =   315
            Left            =   5940
            TabIndex        =   67
            ToolTipText     =   "Flush the QuickUSB RS-232 transmit/receive buffers"
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox RS232BaudRateComboBox 
            Height          =   315
            ItemData        =   "Display.frx":067B
            Left            =   2400
            List            =   "Display.frx":0694
            TabIndex        =   17
            ToolTipText     =   "Selects the QuickUSB RS-232 port baud rate"
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox RS232PortComboBox 
            Height          =   315
            ItemData        =   "Display.frx":06C9
            Left            =   840
            List            =   "Display.frx":06D3
            TabIndex        =   16
            ToolTipText     =   "Selects the active QuickUSB RS-232 port"
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label4 
            Caption         =   "Baud"
            Height          =   195
            Left            =   1800
            TabIndex        =   19
            Top             =   300
            Width           =   435
         End
         Begin VB.Label Label5 
            Caption         =   "Port"
            Height          =   195
            Left            =   180
            TabIndex        =   18
            Top             =   300
            Width           =   435
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "Port I/O"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   5415
         Left            =   -74760
         TabIndex        =   14
         Top             =   480
         Width           =   7155
         Begin VB.TextBox Text1 
            BackColor       =   &H8000000F&
            BorderStyle     =   0  'None
            Height          =   1095
            Left            =   5160
            MultiLine       =   -1  'True
            TabIndex        =   153
            Text            =   "Display.frx":06DD
            Top             =   360
            Width           =   1815
         End
         Begin VB.Frame Frame18 
            Caption         =   "Port B"
            BeginProperty Font 
               Name            =   "Verdana"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   180
            TabIndex        =   104
            Top             =   1320
            Width           =   4755
            Begin VB.CommandButton ReadPortBButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   105
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortBDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   195
               ToolTipText     =   "Port B Output Enable - Set to enable output"
               Top             =   180
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Dir"
               ToolTipTextBit0 =   "Port B, Bit 0 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit1 =   "Port B, Bit 1 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit2 =   "Port B, Bit 2 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit3 =   "Port B, Bit 3 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit4 =   "Port B, Bit 4 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit5 =   "Port B, Bit 5 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit6 =   "Port B, Bit 6 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit7 =   "Port B, Bit 7 Output Enable: 0=Input, 1=Output"
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortBValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   198
               ToolTipText     =   "Port B Data"
               Top             =   480
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Value"
               ToolTipTextBit0 =   "Port B, Bit 0"
               ToolTipTextBit1 =   "Port B, Bit 1"
               ToolTipTextBit2 =   "Port B, Bit 2"
               ToolTipTextBit3 =   "Port B, Bit 3"
               ToolTipTextBit4 =   "Port B, Bit 4"
               ToolTipTextBit5 =   "Port B, Bit 5"
               ToolTipTextBit6 =   "Port B, Bit 6"
               ToolTipTextBit7 =   "Port B, Bit 7"
            End
         End
         Begin VB.Frame Frame16 
            Caption         =   "Port D"
            BeginProperty Font 
               Name            =   "Verdana"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   180
            TabIndex        =   102
            Top             =   3300
            Width           =   4755
            Begin VB.CommandButton ReadPortDButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   103
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortDDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   197
               ToolTipText     =   "Port D Output Enable - Set to enable output"
               Top             =   180
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Dir"
               ToolTipTextBit0 =   "Port D, Bit 0 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit1 =   "Port D, Bit 1 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit2 =   "Port D, Bit 2 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit3 =   "Port D, Bit 3 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit4 =   "Port D, Bit 4 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit5 =   "Port D, Bit 5 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit6 =   "Port D, Bit 6 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit7 =   "Port D, Bit 7 Output Enable: 0=Input, 1=Output"
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortDValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   203
               ToolTipText     =   "Port D Data"
               Top             =   480
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Value"
               ToolTipTextBit0 =   "Port D, Bit 0"
               ToolTipTextBit1 =   "Port D, Bit 1"
               ToolTipTextBit2 =   "Port D, Bit 2"
               ToolTipTextBit3 =   "Port D, Bit 3"
               ToolTipTextBit4 =   "Port D, Bit 4"
               ToolTipTextBit5 =   "Port D, Bit 5"
               ToolTipTextBit6 =   "Port D, Bit 6"
               ToolTipTextBit7 =   "Port D, Bit 7"
            End
         End
         Begin VB.Frame Frame14 
            Caption         =   "Port E"
            BeginProperty Font 
               Name            =   "Verdana"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   180
            TabIndex        =   72
            Top             =   4300
            Width           =   4755
            Begin VB.CommandButton ReadPortEButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   73
               ToolTipText     =   "Read the value of port E"
               Top             =   480
               Width           =   615
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortEDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   201
               ToolTipText     =   "Port E Output Enable - Set to enable output"
               Top             =   180
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Dir"
               ToolTipTextBit0 =   "Port E, Bit 0 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit1 =   "Port E, Bit 1 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit2 =   "Port E, Bit 2 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit3 =   "Port E, Bit 3 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit4 =   "Port E, Bit 4 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit5 =   "Port E, Bit 5 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit6 =   "Port E, Bit 6 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit7 =   "Port E, Bit 7 Output Enable: 0=Input, 1=Output"
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortEValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   202
               ToolTipText     =   "Port E Data"
               Top             =   480
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Value"
               ToolTipTextBit0 =   "Port E, Bit 0"
               ToolTipTextBit1 =   "Port E, Bit 1"
               ToolTipTextBit2 =   "Port E, Bit 2"
               ToolTipTextBit3 =   "Port E, Bit 3"
               ToolTipTextBit4 =   "Port E, Bit 4"
               ToolTipTextBit5 =   "Port E, Bit 5"
               ToolTipTextBit6 =   "Port E, Bit 6"
               ToolTipTextBit7 =   "Port E, Bit 7"
            End
         End
         Begin VB.Frame Frame13 
            Caption         =   "Port C"
            BeginProperty Font 
               Name            =   "Verdana"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   180
            TabIndex        =   70
            Top             =   2300
            Width           =   4755
            Begin VB.CommandButton ReadPortCButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   71
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortCDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   196
               ToolTipText     =   "Port C Output Enable - Set to enable output"
               Top             =   180
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Dir"
               ToolTipTextBit0 =   "Port C, Bit 0 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit1 =   "Port C, Bit 1 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit2 =   "Port C, Bit 2 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit3 =   "Port C, Bit 3 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit4 =   "Port C, Bit 4 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit5 =   "Port C, Bit 5 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit6 =   "Port C, Bit 6 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit7 =   "Port C, Bit 7 Output Enable: 0=Input, 1=Output"
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortCValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   199
               ToolTipText     =   "Port C Data"
               Top             =   480
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Value"
               ToolTipTextBit0 =   "Port C, Bit 0"
               ToolTipTextBit1 =   "Port C, Bit 1"
               ToolTipTextBit2 =   "Port C, Bit 2"
               ToolTipTextBit3 =   "Port C, Bit 3"
               ToolTipTextBit4 =   "Port C, Bit 4"
               ToolTipTextBit5 =   "Port C, Bit 5"
               ToolTipTextBit6 =   "Port C, Bit 6"
               ToolTipTextBit7 =   "Port C, Bit 7"
            End
         End
         Begin VB.Frame Frame12 
            Caption         =   "Port A"
            BeginProperty Font 
               Name            =   "Verdana"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   180
            TabIndex        =   68
            Top             =   300
            Width           =   4755
            Begin VB.CommandButton CountPortAButton 
               Caption         =   "Count"
               Height          =   255
               Left            =   3600
               TabIndex        =   81
               Top             =   180
               Width           =   615
            End
            Begin VB.CommandButton ReadPortAButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3600
               TabIndex        =   69
               ToolTipText     =   "Read the value of port A"
               Top             =   480
               Width           =   615
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortAValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   194
               ToolTipText     =   "Port A Data"
               Top             =   480
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Value"
               ToolTipTextBit0 =   "Port A, Bit 0"
               ToolTipTextBit1 =   "Port A, Bit 1"
               ToolTipTextBit2 =   "Port A, Bit 2"
               ToolTipTextBit3 =   "Port A, Bit 3"
               ToolTipTextBit4 =   "Port A, Bit 4"
               ToolTipTextBit5 =   "Port A, Bit 5"
               ToolTipTextBit6 =   "Port A, Bit 6"
               ToolTipTextBit7 =   "Port A, Bit 7"
            End
            Begin QuickUSBDiagnostics.BitCheckBox PortADirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   200
               ToolTipText     =   "Port A Output Enable - Set to enable output"
               Top             =   180
               Width           =   3375
               _ExtentX        =   5953
               _ExtentY        =   450
               Caption         =   "Dir"
               ToolTipTextBit0 =   "Port A, Bit 0 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit1 =   "Port A, Bit 1 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit2 =   "Port A, Bit 2 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit3 =   "Port A, Bit 3 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit4 =   "Port A, Bit 4 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit5 =   "Port A, Bit 5 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit6 =   "Port A, Bit 6 Output Enable: 0=Input, 1=Output"
               ToolTipTextBit7 =   "Port A, Bit 7 Output Enable: 0=Input, 1=Output"
            End
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "Data"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   180
         TabIndex        =   9
         Top             =   2400
         Width           =   7155
         Begin VB.CommandButton WriteDataCountButton 
            Caption         =   "Count"
            Height          =   315
            Left            =   5220
            TabIndex        =   141
            ToolTipText     =   "Write the contents of the file to the QuickUSB module in data mode"
            Top             =   240
            Width           =   675
         End
         Begin VB.TextBox BufferCountTextBox 
            Height          =   315
            Left            =   5580
            TabIndex        =   99
            ToolTipText     =   "Number of buffers to use"
            Top             =   1320
            Width           =   975
         End
         Begin VB.TextBox BufferSizeTextBox 
            Height          =   315
            Left            =   5580
            TabIndex        =   98
            ToolTipText     =   "Number of items to transfer in data mode"
            Top             =   960
            Width           =   975
         End
         Begin VB.CheckBox AsyncCheckBox 
            Caption         =   "Async"
            Height          =   195
            Left            =   2460
            TabIndex        =   97
            ToolTipText     =   "Use multi buffer asynchronous I/O if checked"
            Top             =   1320
            Value           =   1  'Checked
            Width           =   1155
         End
         Begin VB.CommandButton EditReadFileButton 
            Caption         =   "Edit"
            Height          =   315
            Left            =   4680
            TabIndex        =   94
            Top             =   600
            Width           =   495
         End
         Begin VB.CommandButton EditWriteFileButton 
            Caption         =   "Edit"
            Height          =   315
            Left            =   4680
            TabIndex        =   93
            Top             =   240
            Width           =   495
         End
         Begin VB.CommandButton BrowseReadButton 
            Height          =   315
            Left            =   3480
            Picture         =   "Display.frx":072E
            Style           =   1  'Graphical
            TabIndex        =   92
            Top             =   600
            Width           =   375
         End
         Begin VB.CommandButton BrowseWriteButton 
            Height          =   315
            Left            =   3480
            Picture         =   "Display.frx":0830
            Style           =   1  'Graphical
            TabIndex        =   91
            Top             =   240
            Width           =   375
         End
         Begin VB.TextBox ReadDataTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   90
            Top             =   600
            Width           =   2055
         End
         Begin VB.TextBox WriteDataTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   89
            Top             =   240
            Width           =   2055
         End
         Begin VB.CheckBox IncDataAddressCheckBox 
            Caption         =   "Inc Address"
            Height          =   195
            Left            =   2460
            TabIndex        =   38
            ToolTipText     =   "Increment the address during data transfers if checked"
            Top             =   1020
            Value           =   1  'Checked
            Width           =   1155
         End
         Begin VB.TextBox DataAddressTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   33
            Text            =   "0"
            ToolTipText     =   "Starting GPIFADR address for the data mode transfer"
            Top             =   1320
            Width           =   975
         End
         Begin VB.CommandButton WriteDataButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   3960
            TabIndex        =   12
            ToolTipText     =   "Write the contents of the file to the QuickUSB module in data mode"
            Top             =   240
            Width           =   675
         End
         Begin VB.TextBox DataLengthTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   11
            ToolTipText     =   "Number of items to transfer in data mode"
            Top             =   960
            Width           =   975
         End
         Begin VB.CommandButton ReadDataButton 
            Caption         =   "Read"
            Height          =   315
            Left            =   3960
            TabIndex        =   10
            ToolTipText     =   "Read from the QuickUSB module in data mode and write it to the file"
            Top             =   600
            Width           =   675
         End
         Begin VB.Label BufferCountLabel 
            Caption         =   "Async Buffer Count"
            Height          =   195
            Left            =   3900
            TabIndex        =   101
            Top             =   1380
            Width           =   1455
         End
         Begin VB.Label BufferSizeLabel 
            Caption         =   "Async Buffer Size"
            Height          =   195
            Left            =   3900
            TabIndex        =   100
            Top             =   1020
            Width           =   1335
         End
         Begin VB.Label Label12 
            Caption         =   "Read Filename"
            Height          =   195
            Left            =   120
            TabIndex        =   96
            Top             =   660
            Width           =   1155
         End
         Begin VB.Label Label6 
            Caption         =   "Write Filename"
            Height          =   195
            Left            =   120
            TabIndex        =   95
            Top             =   300
            Width           =   1155
         End
         Begin VB.Label Label16 
            Caption         =   "Address"
            Height          =   195
            Left            =   120
            TabIndex        =   34
            Top             =   1380
            Width           =   615
         End
         Begin VB.Label Label2 
            Caption         =   "Data Length"
            Height          =   195
            Left            =   120
            TabIndex        =   13
            Top             =   1020
            Width           =   975
         End
      End
      Begin VB.Frame GeneralFrame 
         Caption         =   "General"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1575
         Left            =   -74820
         TabIndex        =   8
         Top             =   480
         Width           =   7155
         Begin VB.OptionButton FullSpeedOptionButton 
            Caption         =   "Full Speed (12Mbps)"
            Height          =   200
            Left            =   4200
            TabIndex        =   147
            Top             =   1260
            Width           =   2300
         End
         Begin VB.OptionButton HighSpeedOptionButton 
            Caption         =   "High Speed (480Mbps)"
            Height          =   200
            Left            =   4200
            TabIndex        =   146
            Top             =   960
            Width           =   2300
         End
         Begin VB.TextBox SerialNumberTextBox 
            BackColor       =   &H8000000F&
            Height          =   315
            Left            =   4975
            Locked          =   -1  'True
            TabIndex        =   144
            Top             =   200
            Width           =   2000
         End
         Begin VB.TextBox ModuleVersion 
            BackColor       =   &H8000000F&
            Height          =   315
            Left            =   1080
            Locked          =   -1  'True
            TabIndex        =   143
            Top             =   1000
            Width           =   2800
         End
         Begin VB.Timer ModuleScanTimer 
            Enabled         =   0   'False
            Interval        =   3000
            Left            =   60
            Top             =   600
         End
         Begin VB.TextBox ModuleStringDescriptorTextBox 
            BackColor       =   &H8000000F&
            Height          =   315
            Left            =   1080
            Locked          =   -1  'True
            TabIndex        =   32
            Top             =   600
            Width           =   5895
         End
         Begin VB.ComboBox ModuleComboBox 
            Height          =   315
            Left            =   1080
            Style           =   2  'Dropdown List
            TabIndex        =   27
            ToolTipText     =   "Selects the active QuickUSB Module"
            Top             =   240
            Width           =   1215
         End
         Begin VB.Label Label26 
            Caption         =   "Serial Number"
            Height          =   195
            Left            =   3855
            TabIndex        =   145
            Top             =   250
            Width           =   1020
         End
         Begin VB.Label Label17 
            Caption         =   "HW Version"
            Height          =   195
            Left            =   180
            TabIndex        =   142
            Top             =   1060
            Width           =   915
         End
         Begin VB.Label Label10 
            Caption         =   "Description"
            Height          =   195
            Left            =   180
            TabIndex        =   31
            Top             =   660
            Width           =   915
         End
         Begin VB.Label Label1 
            Caption         =   "Module"
            Height          =   195
            Left            =   180
            TabIndex        =   28
            Top             =   300
            Width           =   735
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "Command"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   180
         TabIndex        =   2
         Top             =   480
         Width           =   7155
         Begin VB.CheckBox IncCommandAddressCheckBox 
            Caption         =   "Inc Address"
            Height          =   375
            Left            =   240
            TabIndex        =   39
            ToolTipText     =   "Increment the address during command transfers if checked"
            Top             =   1380
            Value           =   1  'Checked
            Width           =   1215
         End
         Begin VB.TextBox CommandAddressTextBox 
            Height          =   315
            Left            =   1260
            TabIndex        =   35
            Text            =   "0"
            ToolTipText     =   "Starting GPIFADR value for the command mode transfer"
            Top             =   240
            Width           =   975
         End
         Begin VB.CommandButton WriteCommandButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   1260
            TabIndex        =   5
            ToolTipText     =   "Write 'length' items from the command editor to the QuickUSB module"
            Top             =   1020
            Width           =   975
         End
         Begin VB.TextBox CommandLengthTextBox 
            Height          =   315
            Left            =   1260
            TabIndex        =   4
            ToolTipText     =   "Number of items to transfer in command mode"
            Top             =   600
            Width           =   975
         End
         Begin VB.CommandButton ReadCommandButton 
            Caption         =   "Read"
            Height          =   315
            Left            =   180
            TabIndex        =   3
            ToolTipText     =   "Read 'length' items from the QuickUSB module into the command editor"
            Top             =   1020
            Width           =   975
         End
         Begin MSHierarchicalFlexGridLib.MSHFlexGrid CommandFlexGrid 
            Height          =   1035
            Left            =   2580
            TabIndex        =   6
            ToolTipText     =   "Command editor for command mode data"
            Top             =   420
            Width           =   2295
            _ExtentX        =   4048
            _ExtentY        =   1826
            _Version        =   393216
            Rows            =   16
            FixedRows       =   0
            AllowUserResizing=   1
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            _NumberOfBands  =   1
            _Band(0).Cols   =   2
         End
         Begin VB.Label Label20 
            Caption         =   "Command Editor"
            Height          =   195
            Left            =   2580
            TabIndex        =   40
            Top             =   180
            Width           =   1455
         End
         Begin VB.Label Label19 
            Caption         =   "Address"
            Height          =   195
            Left            =   240
            TabIndex        =   36
            Top             =   300
            Width           =   675
         End
         Begin VB.Label Label3 
            Caption         =   "Length"
            Height          =   195
            Left            =   240
            TabIndex        =   7
            Top             =   660
            Width           =   555
         End
      End
   End
   Begin MSComctlLib.StatusBar StatusBar 
      Align           =   2  'Align Bottom
      Height          =   330
      Left            =   0
      TabIndex        =   0
      Top             =   7005
      Width           =   7755
      _ExtentX        =   13679
      _ExtentY        =   582
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin MSComDlg.CommonDialog CommonDialog 
      Left            =   2160
      Top             =   180
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
   End
   Begin VB.Label Label30 
      Caption         =   "adfasdf"
      Height          =   495
      Left            =   3240
      TabIndex        =   152
      Top             =   3480
      Width           =   1215
   End
   Begin VB.Label Label21 
      Caption         =   "ms"
      Height          =   195
      Left            =   2520
      TabIndex        =   79
      Top             =   6660
      Width           =   255
   End
   Begin VB.Label Label7 
      Caption         =   "Period"
      Height          =   255
      Left            =   1080
      TabIndex        =   78
      Top             =   6660
      Width           =   495
   End
   Begin VB.Menu FileMenu 
      Caption         =   "&File"
      Begin VB.Menu FileExitMenu 
         Caption         =   "&Exit"
         Shortcut        =   ^Q
      End
   End
   Begin VB.Menu HelpMenu 
      Caption         =   "&Help"
      Begin VB.Menu HelpAboutMenu 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "Display"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'=============================================================================
' Title        : Display.frm
' Description  : QuickUSB Control Panel Form
' Notes        : None
' History      :
'   Date       Name        Description
'  3/30/2002   bhenry      Initial coding
'
' Copyright © 2002 Bitwise Systems, Inc.  All rights reserved.
' This software contains confidential information and trade secrets of
' Bitwise Systems and is protected by United States and international
' copyright laws.  Use, disclosure, or reproduction is prohibited without
' the prior express written permission of Bitwise Systems, except as agreed
' in the QuickUSB Plug-In Module license agreement.
'
' Use, duplication or disclosure by the U.S. Government is subject to
' restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
' (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real,
' Suite E, Goleta, CA  93117.
'
' Bitwise Systems
' 6489 Calle Real, Suite E
' Goleta, CA  93117
' Voice: (805) 683-6469
' Fax  : (805) 683-4833
' Web  : www.bitwisesys.com
' email: support@bitwisesys.com
'
' $History: Display.frm $
'
' *****************  Version 55  *****************
' User: Brian Thoma  Date: 5/04/06    Time: 10:28a
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' v2.11.4 - Masked off FIFO Config value in
' UpdateSettingFifoConfigBitCheckBox function since reading the setting
' in v2.11.10 of the firmware can return a word now and we only wanted to
' write a byte to FifoConfigBitCheckBox without it overflowing.
'
' *****************  Version 54  *****************
' User: Bob Jacobson Date: 3/31/06    Time: 5:20p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Bump version to 2.11.3
' Rework for compatibility with new QuickUsbGetFirmwareVersion
'
' *****************  Version 53  *****************
' User: Bob Jacobson Date: 2/16/06    Time: 3:51p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Bump to 2.11.2
' Fix bug in I2C read to only display read data if read was successful.
' If I2C read failed, erase read data text box.
' Mantis bug 5
'
' *****************  Version 52  *****************
' User: Bob Jacobson Date: 2/15/06    Time: 11:01a
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Add new tab for current settings, in additional to the default tab.
' If old firmware is loaded in module, only display available settings.
' Fix bug in async write function with empty input file.
' Bump to version 2.11.1
'
' *****************  Version 51  *****************
' User: Bob Jacobson Date: 2/13/06    Time: 2:20p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Correct icon in help/about display.
' Add better tool tips to the default page.
' Bump version to 2.11.0
'
' *****************  Version 50  *****************
' User: Bob Jacobson Date: 2/09/06    Time: 8:11a
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Add ability to set FPGA type when uploading FPGA.
' Add Xilinx FPGA type.
'
' *****************  Version 49  *****************
' User: Bob Jacobson Date: 12/20/05   Time: 4:35p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Change the USB bus speed selection to radio buttons.
' Change the Company address and Fax number.
'
' *****************  Version 48  *****************
' User: Bob Jacobson Date: 12/19/05   Time: 11:15a
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Remove 1200 and 2400 baud from RS232 page, as the baud rate generator
' is not accurate enough as those rates.
'
' *****************  Version 47  *****************
' User: Bob Jacobson Date: 12/15/05   Time: 5:35p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Use timeGetTime instead of GetTickCount to measure time intervals.
'
' *****************  Version 46  *****************
' User: Bob Jacobson Date: 12/15/05   Time: 12:10p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Change the logo in the HelpAbout form.
' Move all HelpAbout form code to frmAbout.frm.
'
' *****************  Version 45  *****************
' User: Bob Jacobson Date: 12/13/05   Time: 5:39p
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Add ability to set power on default USB bus speed.
' Add dynamic USB bus speed change button.
' Add reset QuickUSB module botton.
'
' *****************  Version 44  *****************
' User: Brian Thoma  Date: 12/02/05   Time: 10:25a
' Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiag
' Changed WriteDataCount function to change the array size based on the
' wordWide boolean.
'
' *****************  Version 43  *****************
' User: Bob Jacobson Date: 11/17/05   Time: 5:06p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Fix overflow in write count function if wordwide=0.
'
' *****************  Version 42  *****************
' User: Bob Jacobson Date: 11/17/05   Time: 10:45a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add code from Microsoft Knowledge Base article Q175512 to convert a
' long path name to a short path name for the hex.exe editor.
'
' *****************  Version 41  *****************
' User: Bob Jacobson Date: 10/18/05   Time: 2:12p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Use SETTING_VERSIONSPEED to get REVID and USB speed.
' Bump version number.
'
' *****************  Version 40  *****************
' User: Bob Jacobson Date: 10/11/05   Time: 2:18p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add constants for the chip IDs and speed rates.
' Add serial number (string 3) to the general tab.
'
' *****************  Version 39  *****************
' User: Bob Jacobson Date: 10/11/05   Time: 12:03p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add USB speed and chip type to display.
'
' *****************  Version 38  *****************
' User: Bob Jacobson Date: 10/06/05   Time: 2:24p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Rewrite QuickUsbGetFirmwareVersion to use major, minor, and build
' versions, instead of a single version.
'
' *****************  Version 37  *****************
' User: Bob Jacobson Date: 10/06/05   Time: 11:44a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Use new QuickUsbGetFirmwareVersion API function (if available) to
' obtain firmware version, instead of extracting same from string 1.
' Continue to extract from string 1 if user has an old version of the
' DLL.
'
' *****************  Version 36  *****************
' User: Blake Henry  Date: 9/27/05    Time: 11:38a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Reorganized the display
'
' *****************  Version 34  *****************
' User: Blake Henry  Date: 9/26/05    Time: 2:54p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Reorganized display form
' Added code to load file into HEX editor
'
' *****************  Version 32  *****************
' User: Bob Jacobson Date: 9/23/05    Time: 11:37a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Check for proper DLL and firmware version before displaying defaults.
' Check for proper DLL and firmware version before enabling async I/O.
' Bump version to 2.10.2.
'
' *****************  Version 31  *****************
' User: Bob Jacobson Date: 9/22/05    Time: 1:52p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add ports B and D to the port tab.
' Bump version to 2.10.1.
'
' *****************  Version 30  *****************
' User: Bob Jacobson Date: 9/21/05    Time: 5:29p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Use new QuickUsbReadDefault and QuickUsbWriteDefault API functions to
' read and write defaults.
'
' *****************  Version 29  *****************
' User: Bob Jacobson Date: 9/13/05    Time: 10:32a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Start program on correct tab.
' Fix some error path code.
'
' *****************  Version 28  *****************
' User: Bob Jacobson Date: 8/30/05    Time: 11:38a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Changes to allow setting defaults
'
' *****************  Version 26  *****************
' User: Blake Henry  Date: 8/04/05    Time: 10:08a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 25  *****************
' User: Bob Jacobson Date: 8/04/05    Time: 10:03a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Test of multiple check out feature
'
' *****************  Version 24  *****************
' User: Bob Jacobson Date: 8/04/05    Time: 9:26a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Add async read and write capability
'
' *****************  Version 22  *****************
' User: Bob Jacobson Date: 7/18/05    Time: 12:11p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Remove defaults tab.
' Fix bug to reload all values in display form when changing modules.
'
' *****************  Version 21  *****************
' User: Eugenia Kaplun Date: 7/18/05    Time: 10:38a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Eugenia's final changes
'
' *****************  Version 20  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 11:47a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 19  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 10:15a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added a new About window that pops up if the old dll is being used.
'
' *****************  Version 18  *****************
' User: Eugenia Kaplun Date: 7/06/05    Time: 9:24a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Merged the features from all the different versions (timeout, about,
' etc.)
'
' *****************  Version 17  *****************
' User: Bob Jacobson Date: 7/05/05    Time: 11:49a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added programmable settings
'
' *****************  Version 16  *****************
' User: Blake Henry  Date: 2/16/05    Time: 9:35a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added SETTING_FIFOCONFIG bit checkbox
' Added SETTING_SLAVEFIFOFLAGS bit checkboxes
'
' *****************  Version 15  *****************
' User: Blake Henry  Date: 2/04/05    Time: 2:16p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 13  *****************
' User: Blake Henry  Date: 8/22/03    Time: 11:51a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Made OutputWindow integral to the Display window
'
' *****************  Version 12  *****************
' User: Blake Henry  Date: 8/21/03    Time: 9:02a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added timer functions
'
' *****************  Version 11  *****************
' User: Blake Henry  Date: 8/08/03    Time: 6:34p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag4
' Updated timer support
'
' *****************  Version 10  *****************
' User: Blake Henry  Date: 8/07/03    Time: 7:12p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added timer support
'
' *****************  Version 9  *****************
' User: Blake Henry  Date: 8/07/03    Time: 10:23a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Increased the string descriptor length
'
' *****************  Version 8  *****************
' User: Blake Henry  Date: 7/03/03    Time: 5:27p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Integrated new BitCheckBox control
'
' *****************  Version 7  *****************
' User: Blake Henry  Date: 7/03/03    Time: 5:07p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Truncate data display for data longer than 4k
'
' *****************  Version 6  *****************
' User: Blake Henry  Date: 6/20/03    Time: 11:27a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
' *****************  Version 5  *****************
' User: Blake Henry  Date: 6/20/03    Time: 10:13a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Fixed bugs in SPIWriteRead and others
'
' *****************  Version 4  *****************
' User: Blake Henry  Date: 6/20/03    Time: 9:52a
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Added repeat button and period adjustment
'
' *****************  Version 2  *****************
' User: Blake Henry  Date: 6/18/03    Time: 8:04p
' Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
' Fixed problems with byte wide data for command and data transfers
'
' *****************  Version 1  *****************
' User: Blake Henry  Date: 6/17/03    Time: 8:47a
' Created in $/Projects/Bitwise/QuickUSB/Software/QuickUsbDiag
'
'=============================================================================
Option Explicit

Dim commandLength As Integer
Dim devName As String
Dim stringDescriptor As String
Dim lastActiveControl As control
Dim startTime As Long
Dim duration As Long
Dim devList As String
Dim oldDevList As String
   
Private Type MemBlockType
   bytesTransferred As Long
   transaction As Byte
   inData() As Byte
End Type



Private Sub DataLengthTextBox_Change()
   SaveSetting App.Title, "DataLength", "DataLengthTextBox", DataLengthTextBox.Text
End Sub



Private Sub ChangeSpeed(HighSpeed As Boolean)
   Dim CpuConfig As Integer
   Dim NewSetting As Integer
   Dim result As Long
   Dim hDevice As Long
   Dim CurrentHighSpeed As Boolean

   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   result = QuickUsbReadSetting(hDevice, SETTING_CPUCONFIG, CpuConfig)
   If (result = 0) Then
      ShowStatus "Cannot get current speed Device:" & devName
      QuickUsbClose (hDevice)
      Exit Sub
   End If
   
   If (CpuConfig And &H8000) Then
      CurrentHighSpeed = True
   Else
      CurrentHighSpeed = False
   End If
   
   ' Are we in the proper speed mode?
   If HighSpeed = CurrentHighSpeed Then
      QuickUsbClose hDevice
      ShowStatus devName & " is already set to requested speed."
      Exit Sub
   End If
   
   If HighSpeed = True Then
      NewSetting = &H8000
   Else
      NewSetting = 0
   End If
   
   ' Set the CPUCS low order byte settings
   NewSetting = NewSetting Or (CpuConfig And &H7FFF)
   ModuleScanTimer.Enabled = False
   
   result = QuickUsbWriteSetting(hDevice, SETTING_CPUCONFIG, NewSetting)
   QuickUsbClose (hDevice)
   
   ' Wait for QuickUSB to renumerate
   Sleep (5000)
   Display.Refresh
   ModuleScanTimer.Enabled = True
   UpdateStringDescriptorTextBox
   Display.Refresh
End Sub


Private Sub HighSpeedOptionButton_mousedown(button As Integer, Shift As Integer, x As Single, y As Single)
   ChangeSpeed True
End Sub

Private Sub FullSpeedOptionButton_mousedown(button As Integer, Shift As Integer, x As Single, y As Single)
   ChangeSpeed False
End Sub

Private Sub ReadAllDefaults()
   Dim Index As Integer
   Dim setting As Integer
   Dim LongSetting As Long
   Dim result As Long
   Dim msb As Byte
   Dim lsb As Byte
   Dim hDevice As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   For Index = 0 To 15 Step 1
      result = QuickUsbReadDefault(hDevice, Index, setting)
      
      If (result = 0) Then
         ShowStatus "Cannot read default " & Index
         setting = 0
      End If
         
     ' Note kludge because integers are signed, and bytes are unsigned!
      LongSetting = setting
      LongSetting = LongSetting And &H7FFF
      lsb = LongSetting And &HFF
      msb = (LongSetting And &HFF00) \ &H100
      
      If (setting And &H8000) Then
         DefaultMsbBitCheckBox(Index).Value = msb Or &H80
      Else
         DefaultMsbBitCheckBox(Index).Value = msb
      End If
      
      DefaultLsbBitCheckBox(Index).Value = lsb
      DefaultMsbBitCheckBox(Index).Enabled = True
      DefaultLsbBitCheckBox(Index).Enabled = True
      DefaultMsbBitCheckBox(Index).Visible = True
      DefaultLsbBitCheckBox(Index).Visible = True
   Next Index
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Disable the unused bytes
   DefaultMsbBitCheckBox(1).Visible = False
   DefaultMsbBitCheckBox(4).Visible = False
   'DefaultMsbBitCheckBox(8).Visible = False
   
   ' Disable the read only settings
   DefaultMsbBitCheckBox(7).Enabled = False
   DefaultLsbBitCheckBox(7).Enabled = False
End Sub


Private Sub ReadAllSettings()
   Dim Index As Integer
   Dim setting As Integer
   Dim LongSetting As Long
   Dim result As Long
   Dim msb As Byte
   Dim lsb As Byte
   Dim hDevice As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   For Index = 0 To 15 Step 1
      result = QuickUsbReadSetting(hDevice, Index, setting)
      
      If (result = 0) Then
         ShowStatus "Cannot read setting " & Index
         setting = 0
         SettingMsbBitCheckBox(Index).Enabled = False
         SettingLsbBitCheckBox(Index).Enabled = False
         SettingMsbBitCheckBox(5).Enabled = False
         SettingMsbBitCheckBox(5).Visible = False
      Else
         SettingMsbBitCheckBox(Index).Enabled = True
         SettingLsbBitCheckBox(Index).Enabled = True
         SettingMsbBitCheckBox(Index).Visible = True
         SettingLsbBitCheckBox(Index).Visible = True
      End If
         
     ' Note kludge because integers are signed, and bytes are unsigned!
      LongSetting = setting
      LongSetting = LongSetting And &H7FFF
      lsb = LongSetting And &HFF
      msb = (LongSetting And &HFF00) \ &H100
      
      If (setting And &H8000) Then
         SettingMsbBitCheckBox(Index).Value = msb Or &H80
      Else
         SettingMsbBitCheckBox(Index).Value = msb
      End If
      
      SettingLsbBitCheckBox(Index).Value = lsb
   Next Index
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Disable the unused bytes
   SettingMsbBitCheckBox(1).Visible = False
   SettingMsbBitCheckBox(4).Visible = False
   'SettingMsbBitCheckBox(8).Visible = False
   
   ' Disable the read only settings
'   SettingMsbBitCheckBox(7).Enabled = False
'   SettingLsbBitCheckBox(7).Enabled = False
End Sub


Private Sub UpdateDefaultSetting(Index As Integer)
   Dim result As Long
   Dim hDevice As Long
   Dim setting As Integer
   Dim LongSetting As Long
   Dim msb As Long
   Dim lsb As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If

   msb = DefaultMsbBitCheckBox(Index).Value
   lsb = DefaultLsbBitCheckBox(Index).Value
   LongSetting = (msb * 256) + lsb
   setting = LongSetting And &H7FFF
   ' Note kludge because integers are signed and bytes are unsigned!
   If (LongSetting And &H8000) Then
      result = QuickUsbWriteDefault(hDevice, Index, setting Or &H8000)
   Else
      result = QuickUsbWriteDefault(hDevice, Index, setting)
   End If

   If (result = 0) Then
      ShowStatus "Cannot write default " & Index
   End If
         
   ' Close the device when you're done
   QuickUsbClose hDevice
End Sub


Private Sub UpdateCurrentSetting(Index As Integer)
   Dim result As Long
   Dim hDevice As Long
   Dim setting As Integer
   Dim LongSetting As Long
   Dim msb As Long
   Dim lsb As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If

   msb = SettingMsbBitCheckBox(Index).Value
   lsb = SettingLsbBitCheckBox(Index).Value
   LongSetting = (msb * 256) + lsb
   setting = LongSetting And &H7FFF
   ' Note kludge because integers are signed and bytes are unsigned!
   If (LongSetting And &H8000) Then
      result = QuickUsbWriteSetting(hDevice, Index, setting Or &H8000)
   Else
      result = QuickUsbWriteSetting(hDevice, Index, setting)
   End If

   If (result = 0) Then
      ShowStatus "Cannot write setting " & Index
   End If
         
   ' Close the device when you're done
   QuickUsbClose hDevice
End Sub


Private Sub BrowseReadButton_Click()
   ' Open a programming file
   CommonDialog.InitDir = CurDir
   CommonDialog.fileName = ReadDataTextBox
   CommonDialog.Filter = "Binary Files (*.bin)|*.bin"
   
   On Error Resume Next
   CommonDialog.ShowOpen
   If Err.Number = cdlCancel Then
      ReadDataTextBox = ""
      Exit Sub
   End If
   
   ReadDataTextBox = CommonDialog.fileName
   
   ' Save the filename to the registry
   SaveSetting App.Title, "Write", "Filename", ReadDataTextBox
End Sub

Private Sub BrowseWriteButton_Click()
   ' Open a programming file
   CommonDialog.InitDir = CurDir
   CommonDialog.fileName = WriteDataTextBox
   CommonDialog.Filter = "Binary Files (*.bin)|*.bin"
   
   On Error Resume Next
   CommonDialog.ShowOpen
   If Err.Number = cdlCancel Then
      WriteDataTextBox = ""
      Exit Sub
   End If
   
   WriteDataTextBox = CommonDialog.fileName
   
   ' Save the filename to the registry
   SaveSetting App.Title, "Read", "Filename", WriteDataTextBox
End Sub


Private Sub DefaultLsbBitCheckBox_Click(Index As Integer)
   UpdateDefaultSetting (Index)
End Sub

Private Sub DefaultMsbBitCheckBox_Click(Index As Integer)
   UpdateDefaultSetting (Index)
End Sub


Private Sub SettingFifoConfigBitCheckBox_Click(Index As Integer)
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   Dim msb As Long
   Dim lsb As Long
   Dim LongSetting As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the value from the bitcheckbox
   msb = SettingFifoConfigBitCheckBox(1).Value
   lsb = SettingFifoConfigBitCheckBox(0).Value
   LongSetting = (msb * 256) + lsb
   Value = LongSetting And &H7FFF
   
   ' Note kludge because integers are signed and bytes are unsigned!
   If (LongSetting And &H8000) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_FIFO_CONFIG, Value Or &H8000)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_FIFO_CONFIG, Value)
   End If

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO flags setting"
      Exit Sub
   End If
End Sub

Private Sub SettingLsbBitCheckBox_Click(Index As Integer)
   UpdateCurrentSetting (Index)
End Sub

Private Sub SettingMsbBitCheckBox_Click(Index As Integer)
   UpdateCurrentSetting (Index)
End Sub

Private Function CheckPath(strPath As String) As Boolean
    If Dir$(strPath) <> "" Then
        CheckPath = True
    Else
        CheckPath = False
    End If
End Function


Private Sub RunFileEditor(datafile As String)
   Dim installDir As String
   Dim hexPath As String
   Dim winhexPath As String
   Dim RetVal As Double
   
   'Make sure the path exists
   If Not CheckPath(datafile) Then
      ShowStatus "File does not exist!"
      Exit Sub
   End If
   
   'Get the QuickUSB Install directory location
   installDir = Environ$("QUICKUSB_DIR")
   
   'Look for hex.exe
   If CheckPath("hex\\HEX.EXE") Then
      hexPath = "hex\\HEX.EXE"
   ElseIf (installDir <> "") Then
      If CheckPath(installDir & "\\Tools\\hex\\HEX.EXE") Then
         hexPath = installDir & "\\Tools\\hex\\HEX.EXE"
      End If
   End If
   
   'Look for winhex.exe
   If CheckPath("winhex\\winhex.exe") Then
      winhexPath = "winhex\\winhex.exe"
   ElseIf (installDir <> "") Then
      If CheckPath(installDir & "\\Tools\\winhex\\winhex.exe") Then
         winhexPath = installDir & "\\Tools\\winhex\\winhex.exe"
      End If
   End If
   
   'Make sure that at least one exists
   If (hexPath = "") And (winhexPath = "") Then
      ShowStatus "Unable to locate hex editor"
      Exit Sub
   End If
   
   'Try running hex.exe
   If (hexPath <> "") Then
      On Error GoTo HexError
      RetVal = Shell(hexPath & " """ & datafile & """", 1)
      Exit Sub
   End If
   
HexError:
   'If hex.exe failed, try winhex.exe
   If (winhexPath <> "") Then
      On Error GoTo WinHexError
      RetVal = Shell(winhexPath & " """ & datafile & """", 1)
      Exit Sub
   End If
   
WinHexError:
   ShowStatus "Unable to run hex editor"
End Sub


Private Sub EditWriteFileButton_Click()
   RunFileEditor (WriteDataTextBox)
End Sub



Private Sub EditReadFileButton_Click()
   RunFileEditor (ReadDataTextBox)
End Sub


Private Sub AsyncCheckBox_Click()
   BufferSizeLabel.Enabled = AsyncCheckBox.Value
   BufferCountLabel.Enabled = AsyncCheckBox.Value
   BufferSizeTextBox.Enabled = AsyncCheckBox.Value
   BufferCountTextBox.Enabled = AsyncCheckBox.Value
   BufferSizeTextBox.Visible = AsyncCheckBox.Value
   BufferCountTextBox.Visible = AsyncCheckBox.Value
   
   If AsyncCheckBox.Value = 1 Then
      BufferSizeTextBox.Text = 262144
      BufferCountTextBox.Text = 4
   End If
End Sub


Private Sub Form_Load()
   Dim count As Integer
   Dim mnuFileList(0 To 5) As Menu
   Dim result As Integer
   Dim ctlCount As Integer
   Dim commandLength As Integer
   Dim major As Integer
   Dim minor As Integer
   Dim revision As Integer
   
   ' Select the first tab
   SSTab.Tab = GetSetting(App.Title, "Settings", "Tab", 0)
   SSTab_Click SSTab.Tab
   
   ' Remember the repeat setting
   RepeatTimer.Interval = GetSetting(App.Title, "Settings", "RepeatPeriod", 500)
   RepeatPeriodTextBox = RepeatTimer.Interval
   
   ' Get some other settings from the registry
   ReadDataTextBox = GetSetting(App.Title, "Write", "Filename", "")
   WriteDataTextBox = GetSetting(App.Title, "Read", "Filename", "")
   CommandLengthTextBox = GetSetting(App.Title, "CommandLength", "CommandLengthTextBox", "8")
   DataLengthTextBox = GetSetting(App.Title, "DataLength", "DataLengthTextBox", "512")

   ' Go find all the connected modules at start up time
   result = UpdateModuleComboBox
   If (result = 0) Then
      Exit Sub
   End If
   
   ' Set defaults
   UpdateWordWideCheckBox
   RS232PortComboBox.ListIndex = 0
   RS232BaudRateComboBox.ListIndex = 1
   SpiReadPortNumComboBox.ListIndex = 0
   SpiWritePortNumComboBox.ListIndex = 0
   AsyncCheckBox.Value = False
   AsyncCheckBox_Click
   
   ' Make the first column the right width
   CommandFlexGrid.ColWidth(0) = 768
   
   
   ' Fill the grid with zeroes
   commandLength = Val(CommandLengthTextBox.Text)
   If (commandLength <> 0) Then
      CommandFlexGrid.Rows = commandLength
      For count = 0 To commandLength - 1
         CommandFlexGrid.TextMatrix(count, 1) = 0
      Next
   End If
   
   ' Update the port states
   ReadPortDirBits
   ReadPortAButton_Click
   ReadPortBButton_Click
   ReadPortCButton_Click
   ReadPortDButton_Click
   ReadPortEButton_Click
   ReadAllSettings
   
   ' Make sure the firmware and DLL are v2.10 or greater to support the defaults
   ' and asynchronous I/O
   SSTab.TabEnabled(7) = False
   AsyncCheckBox.Enabled = False
   major = 0
   minor = 0
   On Error Resume Next
   result = QuickUsbGetDllVersion(major, minor, revision)
   If (major < 2) Or ((major = 2) And (minor < 10)) Then
      ShowStatus "DLL version less than 2.11, defaults not supported."
      GoTo EndDefaults
   End If
   
   If (Val(GetFirmwareVersionString) < 2.1) Then
      ShowStatus "Firmware version less than 2.11, defaults not supported."
      GoTo EndDefaults
   End If
   SSTab.TabEnabled(7) = True
   ReadAllDefaults
   AsyncCheckBox.Enabled = True
EndDefaults:

   ' Clear the last button
   Set lastActiveControl = Nothing
   
   ' Update the file to upload
   UploadFilenameTextBox = GetSetting(App.Title, "Upload", "Filename")
End Sub


Private Sub WriteCountDataButton_Click()
End Sub


Private Sub FileSendMenu_Click()
   WriteCountDataButton_Click
End Sub



Private Sub FileReceiveMenu_Click()
   WriteCountDataButton_Click
End Sub



Private Sub FileExitMenu_Click()
   End
End Sub



Private Sub HelpAboutMenu_Click()
   frmAbout.Show vbModal
End Sub


Private Sub ModuleScanTimer_Timer()
   UpdateModuleComboBox
   SetTimeoutButton_Click
End Sub



Private Sub ReadPortDirBits()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, 0, portData)

   ' Set the port dir bit boxes
   PortADirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, 1, portData)

   ' Set the port dir bit boxes
   PortBDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, 2, portData)

   ' Set the port dir bit boxes
   PortCDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, 3, portData)

   ' Set the port dir bit boxes
   PortDDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbReadPortDir(hDevice, 4, portData)

   ' Set the port dir bit boxes
   PortEDirBitCheckBox.Value = portData

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read direction of port A"
      Exit Sub
   End If
End Sub



Private Sub PortADirBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortADirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, 0, portData)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port A"
      Exit Sub
   End If
End Sub

Private Sub PortBDirBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortBDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, 1, portData)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port B"
      Exit Sub
   End If
End Sub

Private Sub PortDDirBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortDDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, 3, portData)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port D"
      Exit Sub
   End If
End Sub



Private Sub CountPortAButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim count As Byte
   Dim portData(0 To 63) As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Fill the count buffer
   For count = 0 To 62
      portData(count) = count
   Next
   portData(63) = 0

   'result = QuickUsbWritePort(hDevice, 0, portData(0), 1)
   'result = QuickUsbWritePort(hDevice, 0, portData(1), 1)
   'result = QuickUsbWritePort(hDevice, 0, portData(0), 1)
   For count = 0 To 63
      QuickUsbWritePort hDevice, 0, portData(0), 64
   Next
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port A"
      Exit Sub
   End If
End Sub



Private Sub PortCValueBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortCValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePort(hDevice, 2, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port C"
      Exit Sub
   End If

End Sub



Private Sub PortCDirBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortCDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, 2, portData)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port C"
      Exit Sub
   End If

End Sub



Private Sub PortEValueBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortEValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePort(hDevice, 4, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port E"
      Exit Sub
   End If

End Sub



Private Sub PortEDirBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortEDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePortDir(hDevice, 4, portData)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port E"
      Exit Sub
   End If

End Sub



Private Sub ReadPortCButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortCButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   ShowStatus "Reading Port C..."
   result = QuickUsbReadPort(hDevice, 2, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port C"
      Exit Sub
   End If
   
   ' Display the hex value
   PortCValueBitCheckBox.Value = portData

End Sub



Private Sub ReadPortEButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortEButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   ShowStatus "Reading Port E..."
   result = QuickUsbReadPort(hDevice, 4, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port E"
      Exit Sub
   End If
   
   ' Display the hex value
   PortEValueBitCheckBox.Value = portData

End Sub



Private Sub PortAValueBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortAValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePort(hDevice, 0, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port A"
      Exit Sub
   End If
End Sub

Private Sub PortBValueBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortBValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePort(hDevice, 1, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port B"
      Exit Sub
   End If
End Sub

Private Sub PortDValueBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the hex value from the text box
   portData = PortDValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbWritePort(hDevice, 3, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port D"
      Exit Sub
   End If
End Sub



Private Sub ReadPortAButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortAButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   ShowStatus "Reading Port A..."
   result = QuickUsbReadPort(hDevice, 0, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port A"
      Exit Sub
   End If
   
   ' Display the hex value
   PortAValueBitCheckBox.Value = portData
End Sub

Private Sub ReadPortBButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortBButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   ShowStatus "Reading Port B..."
   result = QuickUsbReadPort(hDevice, 1, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port B"
      Exit Sub
   End If
   
   ' Display the hex value
   PortBValueBitCheckBox.Value = portData
End Sub

Private Sub ReadPortDButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortDButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the port data
   ShowStatus "Reading Port D..."
   result = QuickUsbReadPort(hDevice, 3, portData, 1)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port D"
      Exit Sub
   End If
   
   ' Display the hex value
   PortDValueBitCheckBox.Value = portData
End Sub



Private Function FindModules(ByRef str As String, ByVal length As Long) As Long
   Dim result As Long
   Dim Index As Long
   Dim strlen As Long
   Dim databytes() As Byte

   ' Set up storage for the module list
   strlen = length
   ReDim databytes(0 To length)
   
   ' Get the list of modules
   result = QuickUsbFindModulesByte(databytes(0), strlen)
   If (result = 0) Then
      FindModules = 0
      Exit Function
   End If
   
   ' Parse the dev names from the byte array and build up the string
   ' Stop when you find two consecutive nulls
   str = ""
   Index = 0
   Do While (databytes(Index) <> 0 Or databytes(Index + 1) <> 0)
      str = str & Chr(databytes(Index))
      Index = Index + 1
   Loop
   
   ' Append two nulls to the string
   str = str & Chr(0) & Chr(0)
   
   ' Free the space
   ReDim databytes(0)
   
   FindModules = result
End Function



Private Function UpdateModuleComboBox() As Integer
   ' Update the module combo box with the a list of all the connected modules
   Dim result As Long
   Dim count As Integer
   Dim devCount As Integer
   Dim ctlCount As Integer
   Dim devNameStr As String
   Dim startIndex As Integer
   Dim nullIndex As Integer
   
   ' Get the list of modules
   devList = Space(1024)
   result = FindModules(devList, 1024)
   
   ' If we can't get the list or the list is empty
   ' then disable all the controls and exit
   If (result = 0 Or Mid(devList, 1, 1) = Chr(0)) Then
      ' Disable all other controls
      On Error Resume Next
      For ctlCount = 1 To Display.Controls.count - 1
         If (TypeName(Controls(ctlCount)) <> "Timer") Then
            Controls(ctlCount).Enabled = False
         End If
      Next
      
      ' Turn on the tabbed dialog, frame and rescan button
      ModuleScanTimer.Enabled = True
      SlaveFifoFlagsQueryTimer.Enabled = True
      ModuleComboBox.Clear
      ModuleStringDescriptorTextBox.Text = ""
      FileMenu.Enabled = True
      FileExitMenu.Enabled = True
      HelpMenu.Enabled = True
      HelpAboutMenu.Enabled = True
            
      ShowStatus "Cannot find any QuickUSB modules"
         
      UpdateModuleComboBox = 0
      Exit Function
   End If
   
   ' If the list has items and it hasn't changed, just exit
   If (ModuleComboBox.ListCount > 0 And devList = oldDevList) Then
      UpdateModuleComboBox = ModuleComboBox.ListCount
      Exit Function
   End If
   
   ' Clear the list and add the modules to the listbox
   devCount = 0
   startIndex = 1
   ModuleComboBox.Clear
   Do While (Mid(devList, startIndex, 1) <> Chr(0))
      nullIndex = InStr(startIndex, devList, Chr(0), vbBinaryCompare)
      devNameStr = Mid(devList, startIndex, nullIndex - 1)
      ModuleComboBox.AddItem devNameStr
      startIndex = nullIndex + 1
      devCount = devCount + 1
   Loop
   oldDevList = ""
   oldDevList = devList
   ModuleComboBox.ListIndex = 0
   
   ' Enable all other controls
   On Error Resume Next
   GeneralFrame.Enabled = True
   For ctlCount = 1 To Display.Controls.count - 1
      If (TypeName(Controls(ctlCount)) <> "Timer") Then
         Controls(ctlCount).Enabled = True
      End If
   Next
   
   ' Turn on the tabbed dialog, frame and rescan button
   FileMenu.Enabled = True
   FileExitMenu.Enabled = True
   HelpMenu.Enabled = True
   HelpAboutMenu.Enabled = True
   ModuleScanTimer.Enabled = True
   SlaveFifoFlagsQueryTimer.Enabled = True
         
   ' Update the descriptor string
   UpdateStringDescriptorTextBox
   
   ' Initialize the form
   Form_Load

   UpdateSettingFifoConfigBitCheckBox
   
   ' Make sure the firmware is v2.07 or greater
   If (Val(GetFirmwareVersionString) < 2.07) Then
      SlaveFifoFlagsQueryTimer.Enabled = False
      SlaveFifoFlagsBitCheckBox(0).Enabled = False
      SlaveFifoFlagsBitCheckBox(1).Enabled = False
   End If
      
   ' Return the number of modules
   UpdateModuleComboBox = devCount
End Function



Function GetFirmwareVersionString() As String
   Dim hDevice As Long
   Dim result As Long
   Dim length As Byte
   Dim theString As String
   Dim majorVersion As Integer
   Dim minorVersion As Integer
   Dim buildVersion As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      GetFirmwareVersionString = ""
      Exit Function
   End If
   
   ' The following API function was added in version 2.11, and will not
   ' work with an older version of the DLL.
   On Error Resume Next
   majorVersion = 0
   result = QuickUsbGetFirmwareVersion(hDevice, majorVersion, minorVersion, buildVersion)
   
   ' Read the string descriptor
   theString = Space(160)
   result = QuickUsbGetStringDescriptor(hDevice, 2, theString, Len(theString))
   If (result = 0) Then
      QuickUsbClose hDevice
      GetFirmwareVersionString = ""
      Exit Function
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Extract the version
   If majorVersion <> 0 Then
      GetFirmwareVersionString = majorVersion & "." & minorVersion & "rc" & buildVersion
   Else
      GetFirmwareVersionString = Mid(theString, Len("QuickUSB QUSB2 Module v") + 1, 4)
   End If
End Function



Sub UpdateStringDescriptorTextBox()
   Dim hDevice As Long
   Dim result As Long
   Dim length As Byte
   Dim theString As String
   Dim VersionSpeed As Integer
   Dim speedCode As Integer
   Dim chipCode As Integer
   Dim FpgaType As Integer
   
   ' Assume Altera FPGA type
   If FpgaTypeComboBox.ListCount > 0 Then
      FpgaTypeComboBox.ListIndex = 0
   End If
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the string descriptor
   theString = Space(160)
   result = QuickUsbGetStringDescriptor(hDevice, 2, theString, Len(theString))
   If (result = 0) Then
      QuickUsbClose hDevice
      Exit Sub
   End If
   
   ' Set the product description string
   ModuleStringDescriptorTextBox.Text = theString
   stringDescriptor = theString
   
   ' Read the string descriptor
   theString = Space(160)
   result = QuickUsbGetStringDescriptor(hDevice, 3, theString, Len(theString))
   If (result = 0) Then
      QuickUsbClose hDevice
      Exit Sub
   End If
   
   ' Set the serial number string
   SerialNumberTextBox.Text = theString
   stringDescriptor = theString
   
   ' Get the FPGA type
   result = QuickUsbReadSetting(hDevice, SETTING_FPGATYPE, FpgaType)
   If (result = 0) Then
      QuickUsbClose hDevice
      Exit Sub
   End If
   FpgaType = FpgaType And &HFF
   If FpgaType > FpgaTypeComboBox.ListCount Then
      FpgaType = 0
   End If
   FpgaTypeComboBox.ListIndex = FpgaType
   
   ' Get the High Speed / Full Speed setting
   ' NOTE This function was added in version 2.11 firmware and library.
   result = False
   On Error Resume Next
   result = QuickUsbReadSetting(hDevice, SETTING_VERSIONSPEED, VersionSpeed)
   FullSpeedOptionButton.Enabled = False
   HighSpeedOptionButton.Enabled = False
   
   If result = False Then
      FullSpeedOptionButton.Visible = False
      HighSpeedOptionButton.Visible = False
   Else
      speedCode = VersionSpeed And &HFF
      FullSpeedOptionButton.Visible = True
      HighSpeedOptionButton.Visible = True
      FullSpeedOptionButton.Enabled = True
      HighSpeedOptionButton.Enabled = True
      If speedCode = 0 Then
         FullSpeedOptionButton.Value = True
      Else
         HighSpeedOptionButton.Value = True
      End If
   End If
   
   ' Get the processor code setting
   If result = False Then
      ModuleVersion.Text = ""
   Else
      chipCode = (VersionSpeed And &HFF00) / &H100
      If chipCode = 0 Then
         ModuleVersion.Text = QUICKUSB_CHIPID_STRING0
      ElseIf chipCode = 1 Then
         ModuleVersion.Text = QUICKUSB_CHIPID_STRING1
      ElseIf chipCode = 2 Then
         ModuleVersion.Text = QUICKUSB_CHIPID_STRING2
      ElseIf chipCode = 4 Then
         ModuleVersion.Text = QUICKUSB_CHIPID_STRING4
      Else
         ModuleVersion.Text = "Unknown"
      End If
   End If
      
   ' Close the device when you're done
   QuickUsbClose hDevice
End Sub



Private Sub ModuleComboBox_Click()
   devName = ModuleComboBox.List(ModuleComboBox.ListIndex)
   UpdateStringDescriptorTextBox
   Form_Load
   UpdateSettingFifoConfigBitCheckBox
End Sub



Private Sub CommandLengthTextBox_Change()
   Dim count As Long
   
   commandLength = Val(CommandLengthTextBox.Text)
   If (commandLength <> 0) Then
      CommandFlexGrid.Rows = commandLength
      For count = 0 To commandLength - 1
         CommandFlexGrid.TextMatrix(count, 0) = count + 1
      Next
   End If

   SaveSetting App.Title, "CommandLength", "CommandLengthTextBox", CommandLengthTextBox.Text
End Sub



Private Sub CommandFlexGrid_KeyUp(KeyCode As Integer, Shift As Integer)
   Select Case KeyCode
   Case vbKeyBack
      If (Len(CommandFlexGrid.Text) > 0) Then
         CommandFlexGrid.Text = Left(CommandFlexGrid.Text, Len(CommandFlexGrid.Text) - 1)
      End If
   Case vbKeyDelete
      CommandFlexGrid.Text = ""
   Case vbKeyUp
      If (CommandFlexGrid.Row > 0) Then
         CommandFlexGrid.Row = CommandFlexGrid.Row - 1
      End If
   Case vbKeyDown
      If (CommandFlexGrid.Row < CommandFlexGrid.Rows) Then
         CommandFlexGrid.Row = CommandFlexGrid.Row + 1
      End If
   End Select
End Sub



Private Sub CommandFlexGrid_KeyPress(KeyAscii As Integer)
   Select Case KeyAscii
   Case Asc("0") To Asc("9"), Asc("&"), Asc("h"), Asc("a") To Asc("f")
      CommandFlexGrid.Text = CommandFlexGrid.Text & Chr(KeyAscii)
   End Select
End Sub



Private Sub UpdateWordWideCheckBox()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, SETTING_WORDWIDE, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read WORDWIDE setting"
      Exit Sub
   End If
   
   ' Set the checkbox state
   If (Value = 1) Then
      WordWideCheckBox.Value = vbChecked
   Else
      WordWideCheckBox.Value = vbUnchecked
   End If
End Sub



Private Sub SetTimeoutButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Timeout As Long
   
   ' Handle cases where they are using the old DLL
   On Error GoTo OldDll
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   Timeout = CLng(Val(TimeoutTextBox))
   result = QuickUsbSetTimeout(hDevice, Timeout)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write timeout setting"
      Exit Sub
   End If
   Exit Sub
   
OldDll:
   Frame15.Enabled = False
   TimeoutTextBox.Enabled = False
   SetTimeoutButton.Enabled = False
   Resume Next
End Sub

Private Sub SSTab_Click(PreviousTab As Integer)
  ' If (SSTab.TabCaption(SSTab.Tab) = "Cmd/Data") Then
      'RepositionOutputForm
      'OutputForm.Show vbModeless
     ' Width = OutputTextBox.Left + OutputTextBox.Width + 200
  ' Else
'      OutputForm.Hide
   '   Width = SSTab.Left + SSTab.Width + 200
  ' End If
   
   ' Save the most recently selected tab in the registry
   SaveSetting App.Title, "Settings", "Tab", SSTab.Tab
   UpdateStringDescriptorTextBox
   If SSTab.Tab = 6 Then
      ReadAllSettings
   End If
End Sub



Private Sub WordWideCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   If (WordWideCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_WORDWIDE, 1)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_WORDWIDE, 0)
   End If

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write WORDWIDE setting"
      Exit Sub
   End If
      
End Sub


Private Sub BrowseButton_Click()
   ' Open a programming file
   CommonDialog.InitDir = CurDir
   CommonDialog.fileName = UploadFilenameTextBox
   CommonDialog.Filter = "Altera RBF Files (*.rbf)|*.rbf|Xilinx BIN Files (*.bin)|*.bin"
   
   On Error Resume Next
   CommonDialog.ShowOpen
   If Err.Number = cdlCancel Then
      UploadFilenameTextBox = ""
      Exit Sub
   End If
   
   UploadFilenameTextBox = CommonDialog.fileName
   
   ' Save the filename to the registry
   SaveSetting App.Title, "Upload", "Filename", UploadFilenameTextBox
End Sub


Private Sub FpgaUploadButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim length As Integer
   Dim isConfigured As Integer
   Dim fileLength As Long
   Dim blockLength As Integer
   Dim hDevice As Long
   Dim fpgaData() As Byte
   Dim inputFile As Integer
   Dim FpgaType As Integer
   Dim errorCode As Long
   
   ' Check for an empty filename
   If (UploadFilenameTextBox = "" Or Dir(UploadFilenameTextBox) = "") Then
      BrowseButton_Click                        ' Display the browse box
      If (UploadFilenameTextBox = "") Then      ' The user selected Cancel
         Exit Sub
      End If
   End If
   
   ' Allocate storage for data elements
   blockLength = 64
   ReDim fpgaData(blockLength)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Open the input file for reading
   inputFile = FreeFile
   Open UploadFilenameTextBox For Binary Access Read As inputFile
   fileLength = LOF(inputFile)
   
   ' Get the current FPGA type
   result = QuickUsbReadSetting(hDevice, SETTING_FPGATYPE, FpgaType)
   If (result = 0) Then
      MsgBox "Cannot read current FPGA type", , "Configure FPGA"
      Close inputFile
      QuickUsbClose hDevice
      Exit Sub
   End If
   FpgaType = FpgaType And &HFF
   If FpgaType <> FpgaTypeComboBox.ListIndex Then
      result = QuickUsbWriteSetting(hDevice, SETTING_FPGATYPE, FpgaTypeComboBox.ListIndex)
      If (result = 0) Then
         MsgBox "Cannot set FPGA type", , "Configure FPGA"
         Close inputFile
         QuickUsbClose hDevice
         Exit Sub
      End If
   End If
   
   ' Start configuring the FPGA
   result = QuickUsbStartFpgaConfiguration(hDevice)
   If (result = 0) Then
      MsgBox "Cannot start FPGA configuration", , "Configure FPGA"
      Close inputFile
      QuickUsbClose hDevice
      Exit Sub
   End If
   
   ' Send the FPGA configuration data one block at a time
   Index = 0
   Progress.Value = 0
   Do
      ' Read the data
      length = Min(blockLength, fileLength - Index)
      
      'Read the data from the file
      Get inputFile, Index + 1, fpgaData
            
      ' Write the data to the EEPROM
      ShowStatus "Configuring FPGA..." & CInt(Progress.Value) & "%"
      result = QuickUsbWriteFpgaData(hDevice, fpgaData(0), length)
      If (result = 0) Then
         QuickUsbGetLastError (errorCode)
         Exit Do
      End If
      
      ' Update the counters
      Index = Index + length
      
      ' Update the progress bar
      Progress.Value = (Index / fileLength) * 100
      
      ' Redraw the screen
      DoEvents
   Loop Until Index >= fileLength
      
   ' Close the file
   Close inputFile
   
   ' Start configuring the FPGA
   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Free the memory
   ReDim fpgaData(0)
   
   ' Notify the user
   If (isConfigured = 0) Then
      MsgBox "FPGA configuration failed", , "Configure FPGA"
      'ShowStatus "FPGA configuration failed"
   Else
      ShowStatus "FPGA was successfully configured"
   End If
   
   Progress.Value = 0
End Sub



Private Sub IsFpgaConfiguredButton_Click()
   Dim result As Long
   Dim hDevice As Long
   Dim isConfigured As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Start configuring the FPGA
   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Notify the user
   If (isConfigured = 0) Then
'      MsgBox "FPGA configuration failed", , "Configure FPGA"
      ShowStatus "FPGA configuration failed"
   Else
      ShowStatus "FPGA was successfully configured"
   End If

End Sub


Private Sub ReadCommandButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim length As Integer
   Dim hDevice As Long
   Dim commandData() As Byte
   Dim commandAddress As Integer
   Dim wordWide As Boolean
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadCommandButton
   
   commandAddress = Val(CommandAddressTextBox.Text)
   
   ' Set the command address increment flag if needed
   If (IncCommandAddressCheckBox.Value = vbChecked) Then
      commandAddress = commandAddress And &H7FFF
   Else
      commandAddress = commandAddress Or &H8000
   End If
   
   ' Handle wordwide
   If (WordWideCheckBox.Value = vbChecked) Then
      wordWide = True
   Else
      wordWide = False
   End If

   ' Allocate storage for writeDataLength elements
   commandLength = Val(CommandLengthTextBox.Text)
   If (wordWide) Then
      length = commandLength * 2
   Else
      length = commandLength
   End If
   ReDim commandData(length)
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the data
   ShowStatus "Reading " & commandLength & " " & IIf(wordWide, "words", "bytes") & " of command..."
   result = QuickUsbReadCommand(hDevice, commandAddress, commandData(0), length)
   
   
   ' Close the device when you're done
   QuickUsbClose hDevice
      
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read command"
      Exit Sub
   End If
   
   ' Fill the array with a count
   For Index = 0 To commandLength - 1
      If wordWide Then
         CommandFlexGrid.TextMatrix(Index, 1) = Val(commandData(Index * 2) + (&H100 * CLng(commandData((Index * 2) + 1))))
      Else
         CommandFlexGrid.TextMatrix(Index, 1) = Val(commandData(Index))
      End If
   Next
   
   ' Display the data if the form is up
   If (SSTab.TabCaption(SSTab.Tab) = "Cmd/Data") Then
'      ShowByteData commandData
   End If
   
   ' Free the memory
   ReDim commandData(0)
End Sub



Private Sub WriteCommandButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim hDevice As Long
   Dim length As Long
   Dim commandData() As Byte
   Dim commandAddress As Integer
   Dim wordWide As Boolean
   Dim startTime As Long
   
   ' Remember this control as the last active control
   Set lastActiveControl = WriteCommandButton
   
   ' Set the command address increment flag if needed
   commandAddress = Val(CommandAddressTextBox.Text)
   If (IncCommandAddressCheckBox.Value = vbChecked) Then
      commandAddress = commandAddress And &H7FFF
   Else
      commandAddress = commandAddress Or &H8000
   End If
   
   ' Handle wordwide
   If (WordWideCheckBox.Value = vbChecked) Then
      wordWide = True
   Else
      wordWide = False
   End If

   commandLength = Val(CommandLengthTextBox.Text)
   If (wordWide) Then
      length = commandLength * 2
   Else
      length = commandLength
   End If
   ReDim commandData(length)
   
   ' Fill the data array with the contents of the matrix
   For Index = 0 To commandLength - 1
      If wordWide Then
         commandData(Index * 2) = CByte(Val(CommandFlexGrid.TextMatrix(Index, 1)) And &HFF)
         commandData((Index * 2) + 1) = CByte((Val(CommandFlexGrid.TextMatrix(Index, 1)) \ &H100) And &HFF)
      Else
         commandData(Index) = CByte(Val(CommandFlexGrid.TextMatrix(Index, 1)) And &HFF)
      End If
   Next
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Write the data
   ShowStatus "Writing " & commandLength & " " & IIf(wordWide, "words", "bytes") & " from the command editor..."
   startTime = timeGetTime
   result = QuickUsbWriteCommand(hDevice, commandAddress, commandData(0), length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write command"
      Exit Sub
   End If
   
   ' Display the data if the form is up
   If (SSTab.TabCaption(SSTab.Tab) = "Cmd/Data") Then
'      ShowByteData commandData
   End If
   
   ' Free the memory
   ReDim commandData(0)
End Sub


Private Sub ReadAsyncData()
   Dim BufSize As Long
   Dim BufCount As Integer
   Dim TotalBlocks As Long
   Dim errorCode As Long
   Dim startTime As Long
   Dim endTime As Long
   Dim dataAddress As Integer
   Dim outputFile As Integer
   Dim hDevice As Long
   Dim Index As Integer
   Dim lastIndex As Integer
   Dim result As Long
   Dim length As Long
   Dim memBlocks() As MemBlockType
   Dim BlocksQueued As Long
   Dim BlocksFinished As Long
   
   BlocksQueued = 0
   BlocksFinished = 0
   length = 0
   
   BufSize = Val(BufferSizeTextBox.Text)
   BufCount = Val(BufferCountTextBox.Text)
   
   ' Make sure the values are reasonable
   If ((BufSize < 1) Or (BufSize > 1048576)) Then
      ShowStatus "Buffer size must be from 1 to 1048576."
      Exit Sub
   End If

   If ((BufCount < 2) Or (BufCount > 253)) Then
      ShowStatus "Buffer count must be from 2 to 253."
      Exit Sub
   End If

   TotalBlocks = Val(DataLengthTextBox.Text) / BufSize
   
   If ((TotalBlocks * BufSize) <> Val(DataLengthTextBox.Text)) Then
      ShowStatus "Read Length must be a multiple of Buffer Size"
      Exit Sub
   End If
   
   ' Get the datalength from the text box
   dataAddress = Val(DataAddressTextBox.Text)
   
   ' Open the output file for writing
   ' If the file exists, delete it first
   If (Dir(ReadDataTextBox) <> "") Then
      Kill ReadDataTextBox
   End If
   
   outputFile = FreeFile
   Open ReadDataTextBox For Binary Access Write As outputFile
   
   ' Don't use more buffers than the file size
   If (TotalBlocks < BufCount) Then
      BufCount = TotalBlocks
   End If
   
   ' Allocate buffers
   ReDim memBlocks(BufCount - 1)
   For Index = 0 To (BufCount - 1) Step 1
      ReDim memBlocks(Index).inData(BufSize - 1)
   Next Index
      
   Display.MousePointer = vbHourglass
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Close outputFile
      GoTo FreeMem
   End If
   
   ' Set the data address
   If (IncDataAddressCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress Or &H8000)
   End If
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write data address setting"
      Close outputFile
      QuickUsbClose (hDevice)
      GoTo FreeMem
   End If
   
   ' Get the current time
   startTime = timeGetTime

   ' Start up several asynchronous read commands
   For Index = 0 To (BufCount - 1) Step 1
      result = QuickUsbReadDataAsync(hDevice, memBlocks(Index).inData(0), BufSize, memBlocks(Index).transaction)
      If (result = False) Then
         QuickUsbGetLastError (errorCode)
         ShowStatus "QuickUsbReadDataAsync error " & errorCode & " index " & Index
         Close (outputFile)
         QuickUsbClose (hDevice)
         GoTo FreeMem
      End If
   
      BlocksQueued = BlocksQueued + 1
   Next Index

   Index = 0

   Do While (True)
      ' Wait for the next QuickUSB read operation to finish
      result = QuickUsbAsyncWait(hDevice, memBlocks(Index).bytesTransferred, memBlocks(Index).transaction, False)

      If (result = False) Then
         QuickUsbGetLastError (errorCode)
         ShowStatus "QuickUsbAsyncWait error " & errorCode & " index " & Index
         Close (outputFile)
         QuickUsbClose (hDevice)
         GoTo FreeMem
      End If

      ' Did write function complete?
      If (memBlocks(Index).bytesTransferred = BufSize) Then
         length = length + BufSize
      End If

      ' Write the block to disk
      Put outputFile, (BlocksFinished * BufSize) + 1, memBlocks(Index).inData
      BlocksFinished = BlocksFinished + 1
      
      If (BlocksFinished = TotalBlocks) Then
         Exit Do
      End If
      
      ' Start a new QuickUSB read
      If (BlocksQueued < TotalBlocks) Then
         lastIndex = Index
         result = QuickUsbReadDataAsync(hDevice, memBlocks(Index).inData(0), BufSize, memBlocks(Index).transaction)

         If (result = False) Then
            QuickUsbGetLastError (errorCode)
            ShowStatus "QuickUsbReadDataAsync error " & errorCode & " index " & Index
            Close (outputFile)
            QuickUsbClose (hDevice)
            GoTo FreeMem
         End If
         
         BlocksQueued = BlocksQueued + 1
      End If

      ' Increment index
      Index = Index + 1

      If (Index = BufCount) Then
         Index = 0
      End If
   Loop
   
   ' End of Async test
   endTime = timeGetTime
   Close (outputFile)
   QuickUsbClose (hDevice)
   
   ' Do the bandwidth calculation
   If (startTime <> endTime) Then
      ShowStatus "Transferred " & length & " bytes at " & Format(length / (1000 * (endTime - startTime)), "0.000") & " MB/sec"
   End If
   
   ' Free the memory
FreeMem:
   For Index = 0 To (BufCount - 1) Step 1
      ReDim memBlocks(Index).inData(0)
   Next Index
   
   ReDim memBlocks(0)
   
   Display.MousePointer = vbNormal
End Sub


Private Sub WriteAsyncData()
   Dim BufSize As Long
   Dim BufCount As Integer
   Dim TotalBlocks As Long
   Dim errorCode As Long
   Dim startTime As Long
   Dim endTime As Long
   Dim dataAddress As Integer
   Dim inputFile As Integer
   Dim hDevice As Long
   Dim Index As Integer
   Dim result As Long
   Dim fileLength As Long
   Dim length As Long
   Dim memBlocks() As MemBlockType
   Dim BlocksQueued As Long
   Dim BlocksFinished As Long
   
   BlocksQueued = 0
   BlocksFinished = 0
   length = 0
   
   BufSize = Val(BufferSizeTextBox.Text)
   BufCount = Val(BufferCountTextBox.Text)
   
   ' Make sure the values are reasonable
   If ((BufSize < 1) Or (BufSize > 1048576)) Then
      ShowStatus "Buffer size must be from 1 to 1048576."
      Exit Sub
   End If

   If ((BufCount < 2) Or (BufCount > 253)) Then
      ShowStatus "Buffer count must be from 2 to 253."
      Exit Sub
   End If

   ' Get the datalength from the text box
   dataAddress = Val(DataAddressTextBox.Text)
   
   ' Open the input disk file for reading
   inputFile = FreeFile
   Open WriteDataTextBox For Binary Access Read As inputFile
   
   ' Calculate the length
   fileLength = LOF(inputFile)
   TotalBlocks = fileLength / BufSize
   
   ' Check for a reasonable length file
   If fileLength = 0 Then
      ShowStatus "Input file is empty."
      Close inputFile
      Exit Sub
   End If
   If fileLength < BufSize Then
      ShowStatus "File length is less than buffer size"
      Close inputFile
      Exit Sub
   End If
   
   ' Don't use more buffers than the file size
   If (TotalBlocks < BufCount) Then
      BufCount = TotalBlocks
   End If
   
   ' Allocate buffers
   ReDim memBlocks(BufCount - 1)
   For Index = 0 To (BufCount - 1) Step 1
      ReDim memBlocks(Index).inData(BufSize - 1)
   Next Index
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      GoTo FreeMem
   End If
   
   ' Set the data address
   If (IncDataAddressCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress Or &H8000)
   End If
      
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write data address setting"
      QuickUsbClose (hDevice)
      GoTo FreeMem
   End If
   
   ' Do initial write commands
   Display.MousePointer = vbHourglass
   startTime = timeGetTime
   
   For Index = 0 To (BufCount - 1) Step 1
      Get inputFile, (BlocksQueued * BufSize) + 1, memBlocks(Index).inData

      result = QuickUsbWriteDataAsync(hDevice, memBlocks(Index).inData(0), BufSize, memBlocks(Index).transaction)

      If (result = False) Then
         QuickUsbGetLastError (errorCode)
         ShowStatus "QuickUsbWriteDataAsync error " & errorCode & " index " & Index
         Close (inputFile)
         QuickUsbClose (hDevice)
         GoTo FreeMem
      End If

      BlocksQueued = BlocksQueued + 1
   
      If (BlocksQueued = TotalBlocks) Then
         Exit For
      End If
      
   Next Index

   Index = 0
   
   ' Wait for current writes to finish, and start up more writes to the QuickUSB
   Do While (True)

      ' Wait for the next QuickUSB write operation to finish
      result = QuickUsbAsyncWait(hDevice, memBlocks(Index).bytesTransferred, memBlocks(Index).transaction, False)

      If (result = False) Then
         QuickUsbGetLastError (errorCode)
         ShowStatus "QuickUsbAsyncWait error " & errorCode & " index " & Index
         Close (inputFile)
         QuickUsbClose (hDevice)
         GoTo FreeMem
      End If

      length = length + memBlocks(Index).bytesTransferred
      BlocksFinished = BlocksFinished + 1
      
      If (BlocksFinished = TotalBlocks) Then
         Exit Do
      End If
      
      ' Read another block from disk, and write to the QuickUSB
      If (BlocksQueued < TotalBlocks) Then
         Get inputFile, (BlocksQueued * BufSize) + 1, memBlocks(Index).inData

         result = QuickUsbWriteDataAsync(hDevice, memBlocks(Index).inData(0), BufSize, memBlocks(Index).transaction)

         If (result = False) Then
            QuickUsbGetLastError (errorCode)
            ShowStatus "QuickUsbWriteDataAsync error " & errorCode & " index " & Index
            Close (inputFile)
            QuickUsbClose (hDevice)
            GoTo FreeMem
         End If

         BlocksQueued = BlocksQueued + 1
      End If

      ' Increment index
      Index = Index + 1

      If (Index = BufCount) Then
         Index = 0
      End If
   Loop
   
   ' End of Async test
   endTime = timeGetTime
   Close (inputFile)
   QuickUsbClose (hDevice)
   
   ' Do the bandwidth calculation
   If (startTime <> endTime) Then
      ShowStatus "Transferred " & length & " bytes at " & Format(length / (1000 * (endTime - startTime)), "0.000") & " MB/sec"
   End If
   
   ' Free the memory
FreeMem:
   For Index = 0 To (BufCount - 1) Step 1
      ReDim memBlocks(Index).inData(0)
   Next Index
   
   ReDim memBlocks(0)
   
   Display.MousePointer = vbNormal
   DataLengthTextBox.Text = length
End Sub


Private Sub ReadDataButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim length As Long
   Dim hDevice As Long
   Dim testData() As Byte
   Dim dataAddress As Integer
   Dim dataLength As Long
   Dim wordWide As Boolean
   Dim errorCode As Long
   Dim startTime As Long
   Dim endTime As Long
   Dim outputFile As Integer
   
   If (ReadDataTextBox = "" Or Dir(ReadDataTextBox) = "") Then
      BrowseReadButton_Click                        ' Display the browse box
      If (ReadDataTextBox = "") Then      ' The user selected Cancel
         Exit Sub
      End If
   End If
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadDataButton
   
   ' Is this an async read request?
   If (AsyncCheckBox.Value = vbChecked) Then
      ReadAsyncData
      Exit Sub
   End If
      
   ' Get the datalength from the text box
   dataAddress = Val(DataAddressTextBox.Text)
   
   ' Handle wordwide
   If (WordWideCheckBox.Value = vbChecked) Then
      wordWide = True
   Else
      wordWide = False
   End If

   ' Calculate the length
   dataLength = Val(DataLengthTextBox.Text)
   If (wordWide) Then
      length = dataLength * 2
   Else
      length = dataLength
   End If
   ReDim testData(length - 1)
   
   ' Open the output file for writing
   ' If the file exists, delete it first
   If (Dir(ReadDataTextBox) <> "") Then
      Kill ReadDataTextBox
   End If
   
   outputFile = FreeFile
   Open ReadDataTextBox For Binary As outputFile
   
   Display.MousePointer = vbHourglass
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Set the data address
   If (IncDataAddressCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress Or &H8000)
   End If
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write data address setting"
      QuickUsbClose (hDevice)
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Read the data
   ShowStatus "Reading " & dataLength & " " & IIf(wordWide, "words", "bytes") & " of data..."
   startTime = timeGetTime
   result = QuickUsbReadData(hDevice, testData(0), length)
   endTime = timeGetTime
   
   'Write the data to the file
   'For Index = 0 To length - 1
   Put outputFile, 1, testData
   'Next
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Do the bandwidth calculation
   If (startTime <> endTime) Then
      ShowStatus "Transferred " & length & " bytes at " & Format(length / (1000 * (endTime - startTime)), "0.000") & " MB/sec"
   End If
      
   Close outputFile
   
   ' Check the transaction result
   On Error Resume Next
   
   If (result = 0) Then
      QuickUsbGetLastError errorCode
      If (errorCode = QUICKUSB_ERROR_TIMEOUT) Then
         'ShowStatus "Read timed out"
         MsgBox "Read timed out", , "Read Data"
      Else
         ShowStatus "Cannot read data"
         MsgBox "Error number = " & errorCode, , "Read Data"
      End If
   End If
   
   ' Display the data if the form is up
  ' If (SSTab.TabCaption(SSTab.Tab) = "Cmd/Data") Then
  '    ShowByteData testData
  ' End If
   
   ' Free the memory
   ReDim testData(0)
   
   Display.MousePointer = vbNormal
End Sub



Private Sub WriteDataButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim hDevice As Long
   Dim testData() As Byte
   Dim dataAddress As Integer
   Dim wordWide As Boolean
   Dim errorCode As Long
   Dim startTime As Long
   Dim endTime As Long
   Dim inputFile As Integer
   Dim fileLength As Long
   
   If (WriteDataTextBox = "" Or Dir(WriteDataTextBox) = "") Then
      BrowseWriteButton_Click                        ' Display the browse box
      If (WriteDataTextBox = "") Then      ' The user selected Cancel
         Exit Sub
      End If
   End If
   
   ' Remember this control as the last active control
   Set lastActiveControl = WriteDataButton
   
   ' Is this an async write request?
   If (AsyncCheckBox.Value = vbChecked) Then
      WriteAsyncData
      Exit Sub
   End If
      
   ' Get the address from the text box
   dataAddress = Val(DataAddressTextBox.Text)
   
   ' Handle wordwide
   If (WordWideCheckBox.Value = vbChecked) Then
      wordWide = True
   Else
      wordWide = False
   End If
      
   Display.MousePointer = vbHourglass

   ' Open the input file for reading
   inputFile = FreeFile
   Open WriteDataTextBox For Binary As inputFile
   
   ' Calculate the length
   fileLength = LOF(inputFile)
   ReDim testData(fileLength)
   
   'For Index = 1 To fileLength
      Get inputFile, 1, testData
   'Next
      
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Set the data address
   If (IncDataAddressCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress Or &H8000)
   End If
      
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write data address setting"
      QuickUsbClose (hDevice)
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Write the data
   If wordWide = True Then
      ShowStatus "Writing " & fileLength / 2 & " words of data..."
   Else
      ShowStatus "Writing " & fileLength & " bytes of data..."
   End If
   startTime = timeGetTime
   result = QuickUsbWriteData(hDevice, testData(0), fileLength)
   endTime = timeGetTime
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   'Close the input file
   Close inputFile
   ' Do the bandwidth calculation
   If (startTime <> endTime) Then
      ShowStatus "Transferred " & fileLength & " bytes at " & Format(fileLength / (1000 * (endTime - startTime)), "0.000") & " MB/sec"
   End If
   
   ' Check the transaction result
   If (result = 0) Then
      QuickUsbGetLastError errorCode
      If (errorCode = QUICKUSB_ERROR_TIMEOUT) Then
         ShowStatus "Write timed out"
'         MsgBox "Timeout", , "Write Data"
      Else
         ShowStatus "Cannot write data"
'         MsgBox "Error number = " & errorCode, , "Write Data"
      End If
   End If
   
   ' Display the data if the form is up
 '  If (SSTab.TabCaption(SSTab.Tab) = "Cmd/Data") Then
  '    ShowByteData testData
  ' End If
   
   ' Free the memory
   ReDim testData(0)
   
   Display.MousePointer = vbNormal
End Sub



Private Sub WriteDataCountButton_Click()
   Dim Index As Long
   Dim result As Long
   Dim hDevice As Long
   Dim testData() As Byte
   Dim dataAddress As Integer
   Dim wordWide As Boolean
   Dim errorCode As Long
   Dim startTime As Long
   Dim endTime As Long
   Dim inputFile As Integer
   Dim dataLength As Long
   Dim length As Long
   
   ' Remember this control as the last active control
   Set lastActiveControl = WriteDataButton
   
   ' Is this an async write request?
   If (AsyncCheckBox.Value = vbChecked) Then
      WriteAsyncData
      Exit Sub
   End If
         
     ' Handle wordwide
   If (WordWideCheckBox.Value = vbChecked) Then
      wordWide = True
   Else
      wordWide = False
   End If
   
   ' Get the address from the text box
   dataAddress = Val(DataAddressTextBox.Text)
   
   ' Calculate the length
   dataLength = Val(DataLengthTextBox.Text)
   If (wordWide) Then
      length = dataLength * 2
   Else
      length = dataLength
   End If
   
   ReDim testData(length - 1)
   
   ' Fill the array with a count
   If (wordWide) Then
      For Index = 0 To ((length \ 2) - 1)
         testData(Index * 2) = CByte(Index And &HFF)
         testData(Index * 2 + 1) = CByte((Index \ &H100) And &HFF)
      Next
   Else
   '   ReDim testData(dataLength)
      For Index = 0 To length - 1
         testData(Index) = CByte(Index And &HFF)
      Next
   End If
      
   Display.MousePointer = vbHourglass

   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Set the data address
   If (IncDataAddressCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_DATAADDRESS, dataAddress Or &H8000)
   End If
      
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write data address setting"
      QuickUsbClose (hDevice)
      Display.MousePointer = vbNormal
      Exit Sub
   End If
   
   ' Write the data
   ShowStatus "Writing " & dataLength & " " & IIf(wordWide, "words", "bytes") & " of data..."
   startTime = timeGetTime
   result = QuickUsbWriteData(hDevice, testData(0), length)
   endTime = timeGetTime
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Do the bandwidth calculation
   If (startTime <> endTime) Then
      ShowStatus "Transferred " & length & " bytes at " & Format(length / (1000 * (endTime - startTime)), "0.000") & " MB/sec"
   End If
   
   ' Check the transaction result
   If (result = 0) Then
      QuickUsbGetLastError errorCode
      If (errorCode = 6) Then
         ShowStatus "Write timed out"
'         MsgBox "Timeout", , "Write Data"
      Else
         ShowStatus "Cannot write data"
'         MsgBox "Error number = " & errorCode, , "Write Data"
      End If
   End If
      
   ' Free the memory
   ReDim testData(0)
   
   Display.MousePointer = vbNormal
End Sub


Private Sub UpdateSettingFifoConfigBitCheckBox()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, SETTING_FIFO_CONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO flags setting"
      Exit Sub
   End If

   ' Read the value from the bitcheckbox
   SettingFifoConfigBitCheckBox(0).Value = Value And &HFF
   SettingFifoConfigBitCheckBox(1).Value = (Value And &HFF00) \ &H100
   
End Sub







Private Sub SlaveFifoFlagsQueryTimer_Timer()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, SETTING_SLAVEFIFOFLAGS, Value)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO setting"
      Exit Sub
    Else
      ' Update the flags
      SlaveFifoFlagsBitCheckBox(0).Value = Value And &HF
      SlaveFifoFlagsBitCheckBox(1).Value = (Value \ &H100) And &HF
   End If
   
   ' Set the new setting value
   result = QuickUsbReadSetting(hDevice, SETTING_FIFO_CONFIG, Value)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read FIFO config setting"
      Exit Sub
    Else
      ' Update the flags
      SettingFifoConfigBitCheckBox(0).Value = Value And &HF
      SettingFifoConfigBitCheckBox(1).Value = (Value \ &H100) And &HF
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice
End Sub



Private Sub RS232BaudRateComboBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   Dim baudRate As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   baudRate = RS232BaudRateComboBox.List(RS232BaudRateComboBox.ListIndex)
   result = QuickUsbSetRS232BaudRate(hDevice, baudRate)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot set RS232 baud rate"
      Exit Sub
   End If
   
End Sub



Private Sub GetNumTimer_Timer()
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim baudRate As Long
   
   If (RS232PortComboBox.ListIndex = -1) Then
      Exit Sub
   End If
   
   portNum = RS232PortComboBox.ListIndex
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   result = QuickUsbGetNumRS232(hDevice, portNum, length)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot get number of RS232 bytes outstanding"
      Exit Sub
   End If
   
   ' Update the length
   GetNumTextBox.Text = length
End Sub



Private Sub FlushRS232Button_Click()
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   portNum = RS232PortComboBox.ListIndex
   
   ' Read length characters from the RS232 port
   result = QuickUsbFlushRS232(hDevice, portNum)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot flush RS232"
      Exit Sub
   End If
   
End Sub



Private Sub ReadRS232Button_Click()
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim inData As String
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadRS232Button
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   portNum = RS232PortComboBox.ListIndex
   
   ' Find out how many characters are waiting for us
   result = QuickUsbGetNumRS232(hDevice, portNum, length)
   If (result = 0) Then
      QuickUsbClose hDevice
      ShowStatus "Cannot read number of RS232 characters waiting"
      Exit Sub
   End If
   
   ' Fill in a blank string to allocate string storage for the read call
   inData = String(length, 0)
   
   ' Read length characters from the RS232 port
   result = QuickUsbReadRS232String(hDevice, portNum, inData, length)
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Put the string into the text box
   RS232ReadTextBox.Text = inData
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read string from RS232"
      Exit Sub
   End If
   
End Sub



Private Sub SPIWriteReadButton_Click()
   Dim count As Integer
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim inOutData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = SPIWriteReadButton
   
   ' Convert the text string to a byte array
   length = StringToByteArray(inOutData, SpiWriteTextBox.Text)
   
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the port number and the length
   portNum = SpiWritePortNumComboBox.ListIndex
   
   ' Write the data and read it to the SPI if not zero length
   If (length <> 0) Then
      ShowStatus "Writing and reading from SPI port " & portNum & "..."
      result = QuickUsbWriteReadSpi(hDevice, portNum, inOutData(0), length)
   Else
      ShowStatus "Cannot write zero length data"
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SPI data"
      Exit Sub
   End If
   
   ' Convert the byte array to a string
   If (length > 0) Then
      SpiReadTextBox.Text = ByteArrayToString(inOutData)
   End If
End Sub



Private Sub WriteRS232Button_Click()
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Long
   
   ' Remember this control as the last active control
   Set lastActiveControl = WriteRS232Button
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   portNum = RS232PortComboBox.ListIndex
   length = Len(RS232WriteTextBox.Text)
   
   If (length <> 0) Then
      ShowStatus "Writing " & RS232WriteTextBox.Text & " to RS-232 port " & portNum & "..."
      result = QuickUsbWriteRS232String(hDevice, portNum, RS232WriteTextBox.Text, length)
   Else
      ShowStatus "Cannot write zero length data"
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write string to RS232"
      Exit Sub
   End If
   
End Sub



Private Sub SpiEndianCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the old register value
   result = QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, Value)
   
   ' Set the new setting value
   If (SpiEndianCheckBox.Value = vbChecked) Then
      Value = Value Or 1
   Else
      Value = Value And Not (1)
   End If
   result = QuickUsbWriteSetting(hDevice, SETTING_SPICONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SETTING_SPICONFIG setting"
      Exit Sub
   End If
   
   ' Refresh all settings
   ReadAllSettings
End Sub



Private Sub SpiPolarityCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the old register value
   result = QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, Value)
   
   ' Set the new setting value
   If (SpiPolarityCheckBox.Value = vbChecked) Then
      Value = Value Or 2
   Else
      Value = Value And Not (2)
   End If
   result = QuickUsbWriteSetting(hDevice, SETTING_SPICONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SPI_ENDIAN setting"
      Exit Sub
   End If
   
   ' Refresh all settings
   ReadAllSettings
End Sub



Private Sub SpiPhaseCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the old register value
   result = QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, Value)
   
   ' Set the new setting value
   If (SpiPhaseCheckBox.Value = vbChecked) Then
      Value = Value Or 4
   Else
      Value = Value And Not (4)
   End If
   result = QuickUsbWriteSetting(hDevice, SETTING_SPICONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SPI_ENDIAN setting"
      Exit Sub
   End If

   ' Refresh all settings
   ReadAllSettings
End Sub


Private Sub SpiReadButton_Click()
   Dim count As Integer
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim inData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = SpiReadButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Calculate the length
   portNum = SpiReadPortNumComboBox.ListIndex
   length = Val(SpiReadLengthTextBox.Text)
   
   ' Allocate storage for writeDataLength elements
   ReDim inData(0 To length - 1)
   
   ShowStatus "Reading from SPI port " & portNum & "..."
   result = QuickUsbReadSpi(hDevice, portNum, inData(0), length)
  
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Convert the byte array to a string
   SpiReadTextBox.Text = ByteArrayToString(inData)
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read SPI data"
      Exit Sub
   End If
End Sub



Private Sub SpiWriteButton_Click()
   Dim portNum As Byte
   Dim hDevice As Long
   Dim result As Long
   Dim length As Integer
   Dim outData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = SpiWriteButton
   
   ' Convert the text string to a byte array
   length = StringToByteArray(outData, SpiWriteTextBox.Text)
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the port number and the length
   portNum = SpiWritePortNumComboBox.ListIndex
   
   ' Write the data to the SPI if not zero length
   If (length <> 0) Then
      ShowStatus "Writing " & SpiWriteTextBox.Text & " to SPI port " & portNum & "..."
      result = QuickUsbWriteSpi(hDevice, portNum, outData(0), length)
   Else
      ShowStatus "Cannot write zero length data"
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SPI data"
      Exit Sub
   End If
End Sub



Private Sub I2CReadButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim address As Integer
   Dim length As Integer
   Dim inData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = I2CReadButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Calculate the length
   address = Val(I2cReadAddressTextBox.Text)
   length = Val(I2cReadLengthTextBox.Text)
   
   ' Allocate storage for writeDataLength elements
   ReDim inData(length - 1)
   
   ShowStatus "Reading from I2C device at address " & address & "..."
   result = QuickUsbReadI2C(hDevice, address, inData(0), length)
   
   ' Convert the byte array to a string
   If result = 0 Then
      I2CReadTextBox.Text = ""
   Else
      I2CReadTextBox.Text = ByteArrayToString(inData)
   End If
      
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read I2C data"
      Exit Sub
   End If
End Sub



Private Sub I2cWriteButton_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim address As Integer
   Dim length As Integer
   Dim outData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = I2cWriteButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the address and the length
   address = Val(I2cWriteAddressTextBox.Text)
   
   ' Convert the text string to a byte array
   length = StringToByteArray(outData, I2cWriteTextBox.Text)
   
   ' Actually write the text out the I2C port
   If (length <> 0) Then
      ShowStatus "Writing " & I2cWriteTextBox.Text & " to I2C device at address " & address & "..."
      result = QuickUsbWriteI2C(hDevice, address, outData(0), length)
   Else
      ShowStatus "Cannot write zero length data"
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write I2C data"
      Exit Sub
   End If

End Sub


Private Sub I2cCachedWriteButton_Click()

   Dim hDevice As Long
   Dim result As Long
   Dim address As Integer
   Dim length As Integer
   Dim outData() As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = I2cWriteButton
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Get the address and the length
   address = Val(I2cWriteAddressTextBox.Text)
   
   ' Convert the text string to a byte array
   length = StringToByteArray(outData, I2cWriteTextBox.Text)
   
   ' Actually write the text out the I2C port
   If (length <> 0) Then
      ShowStatus "Caching write of " & I2cWriteTextBox.Text & " to I2C device at address " & address & "..."
      result = QuickUsbWriteI2C(hDevice, address Or (length * 256), outData(0), length)
   Else
      ShowStatus "Cannot write zero length data"
   End If
   
   ' Close the device when you're done
   QuickUsbClose hDevice

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write I2C data"
      Exit Sub
   End If

End Sub


Private Sub RepeatCheckBox_Click()
   
   If (RepeatCheckBox.Value = vbChecked) Then
      RepeatTimer.Enabled = True
   Else
      RepeatTimer.Enabled = False
   End If
End Sub



Private Sub RepeatTimer_Timer()
   Dim period As Long
   
   ' Change the period if its valid
   period = Val(RepeatPeriodTextBox)
   If (period >= 10 And period <= 65535) Then
      RepeatTimer.Interval = period
      SaveSetting App.Title, "Settings", "RepeatPeriod", period
   End If
      
   ' Check to make sure there was a last active control
   If lastActiveControl Is Nothing Then
      Exit Sub
   End If
   
   ' Run different functions based on the last button pressed
   Select Case lastActiveControl.Name
   Case "ReadCommandButton"
      ReadCommandButton_Click
   Case "WriteCommandButton"
      WriteCommandButton_Click
   Case "ReadDataButton"
      ReadDataButton_Click
   Case "WriteDataButton"
      WriteDataButton_Click
   Case "WriteCountDataButton"
      WriteCountDataButton_Click
   Case "ReadPortAButton"
      ReadPortAButton_Click
   Case "ReadPortCButton"
      ReadPortCButton_Click
   Case "ReadPortEButton"
      ReadPortEButton_Click
   Case "ReadRS232Button"
      ReadRS232Button_Click
   Case "WriteRS232Button"
      WriteRS232Button_Click
   Case "SpiReadButton"
      SpiReadButton_Click
   Case "SpiWriteButton"
      SpiWriteButton_Click
   Case "SpiWriteReadButton"
      SPIWriteReadButton_Click
   Case "I2CReadButton"
      I2CReadButton_Click
   Case "I2CWriteButton"
      I2cWriteButton_Click
   Case Else
      Debug.Print "I don't know about " & lastActiveControl.Name
   End Select
   
End Sub



Public Sub ShowStatus(status As String)
   StatusBar.SimpleText = status
   StatusTimer.Enabled = False
   StatusTimer.Interval = 3000
   StatusTimer.Enabled = True
End Sub



Private Sub StatusTimer_Timer()
   StatusBar.SimpleText = ""
   StatusTimer.Enabled = False
End Sub


Public Sub Clear()
'   OutputTextBox.Text = ""
End Sub


'Public Sub ShowByteData(theData() As Byte)
'   Dim col As Long
 '  Dim maxCol As Long
 '  Dim count As Long
 '  Dim start As Long
 '  Dim displayString As String
   
'   start = OutputTextBox.SelStart
'   OutputTextBox.Text = ""
'   col = 0
 '  maxCol = 15
  ' For count = 0 To Min(UBound(theData, 1), 4096) - 1
   '   If (col = 0) Then
'         displayString = displayString & asHex(count, 4) & ": "
'      End If

'      displayString = displayString & asHex(theData(count), 2)
      
 '     If (col = maxCol) Then
   '      displayString = displayString & vbCrLf
  '    End If
  '    If (col < maxCol) Then
  '       col = col + 1
  '    Else
  '       col = 0
  '    End If
  ' Next
  ' OutputTextBox.Text = displayString
  ' OutputTextBox.SelStart = start
' End Sub


'Public Sub ShowByteDataComparison(theDataA() As Byte, theDataB() As Byte)
'   Dim count As Long
'   Dim start As Long
'   Dim displayString As String
'
'   start = OutputTextBox.SelStart
'   OutputTextBox.Text = ""
'   For count = 0 To Min(UBound(theDataA, 1), 4096) - 1
'      displayString = displayString & asHex(count, 4) & ": " & asHex(theDataA(count), 2) & " - " & asHex(theDataB(count), 2) & vbCrLf
'   Next
'   OutputTextBox.Text = displayString
'   OutputTextBox.SelStart = start
'End Sub

