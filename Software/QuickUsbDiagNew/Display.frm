VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{0ECD9B60-23AA-11D0-B351-00A0C9055D8E}#6.0#0"; "mshflxgd.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Display 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB Diagnostics"
   ClientHeight    =   7335
   ClientLeft      =   150
   ClientTop       =   840
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
      TabIndex        =   82
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
      TabIndex        =   79
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
      Tab(0).Control(0)=   "ResetButton"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Frame8"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "QuickUsbCtl1"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "GeneralFrame"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).ControlCount=   4
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
      Tab(3).Control(0)=   "Frame11"
      Tab(3).Control(1)=   "Frame6"
      Tab(3).Control(2)=   "Frame5"
      Tab(3).ControlCount=   3
      TabCaption(4)   =   "SPI"
      TabPicture(4)   =   "Display.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame9"
      Tab(4).Control(1)=   "Frame10"
      Tab(4).Control(2)=   "SpiEndianCheckBox"
      Tab(4).ControlCount=   3
      TabCaption(5)   =   "I2C"
      TabPicture(5)   =   "Display.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "Frame7"
      Tab(5).Control(1)=   "Frame3"
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
         TabIndex        =   161
         Top             =   480
         Width           =   7155
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   162
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP2CFG"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   3600
            TabIndex        =   163
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP6CFG"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   3600
            TabIndex        =   164
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IFCONFIG"
            ToolTipTextBit0 =   "IFCFG:0 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit1 =   "IFCFG:1 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit2 =   "GSTATE 0=normal, 1=GPIF state on Port E bits 0:2"
            ToolTipTextBit3 =   "ASYNC 0=sync FIFO, 1=async FIFO"
            ToolTipTextBit4 =   "IFCLKPOL 0=normal clock polarity, 1=reverse clock polarity"
            ToolTipTextBit5 =   "IFCLKOE 0=TriState, 1=drive"
            ToolTipTextBit6 =   "3048MHZ 0=30MHz, 1=48MHz"
            ToolTipTextBit7 =   "IFCLKSRC 0=external clock, 1=internal clock"
            ToolTipTextBit0 =   "IFCFG:0 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit1 =   "IFCFG:1 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit2 =   "GSTATE 0=normal, 1=GPIF state on Port E bits 0:2"
            ToolTipTextBit3 =   "ASYNC 0=sync FIFO, 1=async FIFO"
            ToolTipTextBit4 =   "IFCLKPOL 0=normal clock polarity, 1=reverse clock polarity"
            ToolTipTextBit5 =   "IFCLKOE 0=TriState, 1=drive"
            ToolTipTextBit6 =   "3048MHZ 0=30MHz, 1=48MHz"
            ToolTipTextBit7 =   "IFCLKSRC 0=external clock, 1=internal clock"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   165
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FifoPinPolar"
            ToolTipTextBit0 =   "FF 0=active low, 1=active high"
            ToolTipTextBit1 =   "EF 0=active low, 1=active high"
            ToolTipTextBit2 =   "SLWR 0=active low, 1=active high"
            ToolTipTextBit3 =   "SLRD 0=active low, 1=active high"
            ToolTipTextBit4 =   "SLOE 0=active low, 1=active high"
            ToolTipTextBit5 =   "PKTEND 0=active low, 1=active high"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "FF 0=active low, 1=active high"
            ToolTipTextBit1 =   "EF 0=active low, 1=active high"
            ToolTipTextBit2 =   "SLWR 0=active low, 1=active high"
            ToolTipTextBit3 =   "SLRD 0=active low, 1=active high"
            ToolTipTextBit4 =   "SLOE 0=active low, 1=active high"
            ToolTipTextBit5 =   "PKTEND 0=active low, 1=active high"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   3600
            TabIndex        =   166
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Bit 0"
            ToolTipTextBit1 =   "HSPP Bit 1"
            ToolTipTextBit2 =   "HSPP Bit 2"
            ToolTipTextBit3 =   "HSPP Bit 3"
            ToolTipTextBit4 =   "HSPP Bit 4"
            ToolTipTextBit5 =   "HSPP Bit 5"
            ToolTipTextBit6 =   "HSPP Bit 6"
            ToolTipTextBit7 =   "HSPP Bit 7"
            ToolTipTextBit0 =   "HSPP Bit 0"
            ToolTipTextBit1 =   "HSPP Bit 1"
            ToolTipTextBit2 =   "HSPP Bit 2"
            ToolTipTextBit3 =   "HSPP Bit 3"
            ToolTipTextBit4 =   "HSPP Bit 4"
            ToolTipTextBit5 =   "HSPP Bit 5"
            ToolTipTextBit6 =   "HSPP Bit 6"
            ToolTipTextBit7 =   "HSPP Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   167
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Bit 8"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Clear - Enable Address Bus, Set - Disable Address Bus"
            ToolTipTextBit7 =   "Clear - Increment Address Bus, Set - Don't Increment Address Bus"
            ToolTipTextBit0 =   "HSPP Bit 8"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Clear - Enable Address Bus, Set - Disable Address Bus"
            ToolTipTextBit7 =   "Clear - Increment Address Bus, Set - Don't Increment Address Bus"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   3600
            TabIndex        =   168
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "HSPP Data Width, 0= 8 Bit, 1=16 Bit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "HSPP Data Width, 0= 8 Bit, 1=16 Bit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   169
            ToolTipText     =   "Not Currently Used"
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   3600
            TabIndex        =   170
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "Full Flag For EP2 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP2 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY1 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "Full Flag For EP2 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP2 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY1 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   171
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "Full Flag For EP6 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP6 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY0 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "Full Flag For EP6 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP6 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY0 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   3600
            TabIndex        =   172
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SPICONFIG"
            ToolTipTextBit0 =   "0= LSBit to MSBit, 1= MSBit to LSBit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "0= LSBit to MSBit, 1= MSBit to LSBit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   173
            ToolTipText     =   "I/O Port E Alternate Configuration"
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTECFG"
            ToolTipTextBit0 =   "T0OUT 0=normal, 1=timer 0 output"
            ToolTipTextBit1 =   "T1OUT 0=normal, 1=timer 1 output"
            ToolTipTextBit2 =   "T2OUT 0=normal, 1=timer 2 output"
            ToolTipTextBit3 =   "RXD0OUT 0=normal, 1=RS232 port 0 RXD"
            ToolTipTextBit4 =   "RXD1OUT 0=normal, 1=RS232 port 1 RXD"
            ToolTipTextBit5 =   "INT6 0=normal, 1=INT6 input"
            ToolTipTextBit6 =   "T2EX 0=normal, 1=timer2 restart"
            ToolTipTextBit7 =   "GPIFA8 0=normal, 1=GPIF address bit 8 output"
            ToolTipTextBit0 =   "T0OUT 0=normal, 1=timer 0 output"
            ToolTipTextBit1 =   "T1OUT 0=normal, 1=timer 1 output"
            ToolTipTextBit2 =   "T2OUT 0=normal, 1=timer 2 output"
            ToolTipTextBit3 =   "RXD0OUT 0=normal, 1=RS232 port 0 RXD"
            ToolTipTextBit4 =   "RXD1OUT 0=normal, 1=RS232 port 1 RXD"
            ToolTipTextBit5 =   "INT6 0=normal, 1=INT6 input"
            ToolTipTextBit6 =   "T2EX 0=normal, 1=timer2 restart"
            ToolTipTextBit7 =   "GPIFA8 0=normal, 1=GPIF address bit 8 output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   3600
            TabIndex        =   174
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "CPUCONFIG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE 0=tristate CLKOUT, 1=drive CLKOUT"
            ToolTipTextBit2 =   "CLKINV 0=normal, 1=invert CLKOUT"
            ToolTipTextBit3 =   "CLKSPD:0 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit4 =   "CLKSPD:1 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit5 =   "Reserved, do not change"
            ToolTipTextBit6 =   "0"
            ToolTipTextBit7 =   "0"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE 0=tristate CLKOUT, 1=drive CLKOUT"
            ToolTipTextBit2 =   "CLKINV 0=normal, 1=invert CLKOUT"
            ToolTipTextBit3 =   "CLKSPD:0 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit4 =   "CLKSPD:1 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit5 =   "Reserved, do not change"
            ToolTipTextBit6 =   "0"
            ToolTipTextBit7 =   "0"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   175
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "BusSpeed"
            ToolTipTextBit0 =   "Not Currently Used"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "USB bus speed,  0=full speed (12Mbps), 1=high speed(480Mbps)"
            ToolTipTextBit0 =   "Not Currently Used"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "USB bus speed,  0=full speed (12Mbps), 1=high speed(480Mbps)"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   3600
            TabIndex        =   176
            ToolTipText     =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit1 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit2 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit3 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit4 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit5 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit6 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit7 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit0 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit1 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit2 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit3 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit4 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit5 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit6 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit7 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   177
            ToolTipText     =   "Not Currently Used"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   3600
            TabIndex        =   178
            ToolTipText     =   "Port A Data"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOA"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   179
            ToolTipText     =   "Port A Output Enable - Set to enable output"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEA"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   3600
            TabIndex        =   180
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "0=100KHz, 1=400KHZ I2C bus speed"
            ToolTipTextBit1 =   "Reserved, do not change"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "not currently used"
            ToolTipTextBit7 =   "not currently used"
            ToolTipTextBit0 =   "0=100KHz, 1=400KHZ I2C bus speed"
            ToolTipTextBit1 =   "Reserved, do not change"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "not currently used"
            ToolTipTextBit7 =   "not currently used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   181
            ToolTipText     =   "Not Currently Used"
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   182
            ToolTipText     =   "Port C Output Enable - Set to enable output"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEC"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   183
            ToolTipText     =   "Port B Output Enable - Set to enable output"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEB"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   3600
            TabIndex        =   184
            ToolTipText     =   "Port C Data"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOC"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   3600
            TabIndex        =   185
            ToolTipText     =   "Port B Data"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOB"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   3600
            TabIndex        =   186
            ToolTipText     =   "Port E Data"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOE"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   187
            ToolTipText     =   "Port E Output Enable - Set to enable output"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEE"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   3600
            TabIndex        =   188
            ToolTipText     =   "Port D Data"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOD"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   189
            ToolTipText     =   "Port D Output Enable - Set to enable output"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OED"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   190
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSAB"
            ToolTipTextBit0 =   "FLAGA0"
            ToolTipTextBit1 =   "FLAGA1"
            ToolTipTextBit2 =   "FLAGA2"
            ToolTipTextBit3 =   "FLAGA3"
            ToolTipTextBit4 =   "FLAGB0"
            ToolTipTextBit5 =   "FLAGB1"
            ToolTipTextBit6 =   "FLAGB2"
            ToolTipTextBit7 =   "FLAGB3"
            ToolTipTextBit0 =   "FLAGA0"
            ToolTipTextBit1 =   "FLAGA1"
            ToolTipTextBit2 =   "FLAGA2"
            ToolTipTextBit3 =   "FLAGA3"
            ToolTipTextBit4 =   "FLAGB0"
            ToolTipTextBit5 =   "FLAGB1"
            ToolTipTextBit6 =   "FLAGB2"
            ToolTipTextBit7 =   "FLAGB3"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   3600
            TabIndex        =   191
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSCD"
            ToolTipTextBit0 =   "FLAGC0"
            ToolTipTextBit1 =   "FLAGC1"
            ToolTipTextBit2 =   "FLAGC2"
            ToolTipTextBit3 =   "FLAGC3"
            ToolTipTextBit4 =   "FLAGD0"
            ToolTipTextBit5 =   "FLAGD1"
            ToolTipTextBit6 =   "FLAGD2"
            ToolTipTextBit7 =   "FLAGD3"
            ToolTipTextBit0 =   "FLAGC0"
            ToolTipTextBit1 =   "FLAGC1"
            ToolTipTextBit2 =   "FLAGC2"
            ToolTipTextBit3 =   "FLAGC3"
            ToolTipTextBit4 =   "FLAGD0"
            ToolTipTextBit5 =   "FLAGD1"
            ToolTipTextBit6 =   "FLAGD2"
            ToolTipTextBit7 =   "FLAGD3"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultLsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   3600
            TabIndex        =   192
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTCCFG"
            ToolTipTextBit0 =   "GPIFA0 0=normal, 1=GPIF address bit 0 output"
            ToolTipTextBit1 =   "GPIFA1 0=normal, 1=GPIF address bit 1 output"
            ToolTipTextBit2 =   "GPIFA2 0=normal, 1=GPIF address bit 2 output"
            ToolTipTextBit3 =   "GPIFA3 0=normal, 1=GPIF address bit 3 output"
            ToolTipTextBit4 =   "GPIFA4 0=normal, 1=GPIF address bit 4 output"
            ToolTipTextBit5 =   "GPIFA5 0=normal, 1=GPIF address bit 5 output"
            ToolTipTextBit6 =   "GPIFA6 0=normal, 1=GPIF address bit 6 output"
            ToolTipTextBit7 =   "GPIFA7 0=normal, 1=GPIF address bit 7 output"
            ToolTipTextBit0 =   "GPIFA0 0=normal, 1=GPIF address bit 0 output"
            ToolTipTextBit1 =   "GPIFA1 0=normal, 1=GPIF address bit 1 output"
            ToolTipTextBit2 =   "GPIFA2 0=normal, 1=GPIF address bit 2 output"
            ToolTipTextBit3 =   "GPIFA3 0=normal, 1=GPIF address bit 3 output"
            ToolTipTextBit4 =   "GPIFA4 0=normal, 1=GPIF address bit 4 output"
            ToolTipTextBit5 =   "GPIFA5 0=normal, 1=GPIF address bit 5 output"
            ToolTipTextBit6 =   "GPIFA6 0=normal, 1=GPIF address bit 6 output"
            ToolTipTextBit7 =   "GPIFA7 0=normal, 1=GPIF address bit 7 output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox DefaultMsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   193
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTACFG"
            ToolTipTextBit0 =   "INT0 0=normal, 1=INT0 pin"
            ToolTipTextBit1 =   "INT1 0=normal, 1=INT1 pin"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "SLCS 0=normal, 1=configure PA7 as SLCS"
            ToolTipTextBit7 =   "FLAGD 0=normal, 1=configure PA7 as FLAGD"
            ToolTipTextBit0 =   "INT0 0=normal, 1=INT0 pin"
            ToolTipTextBit1 =   "INT1 0=normal, 1=INT1 pin"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "SLCS 0=normal, 1=configure PA7 as SLCS"
            ToolTipTextBit7 =   "FLAGD 0=normal, 1=configure PA7 as FLAGD"
         End
         Begin VB.Label Label28 
            Caption         =   "LSB"
            Height          =   195
            Left            =   5400
            TabIndex        =   195
            Top             =   240
            Width           =   315
         End
         Begin VB.Label Label27 
            Caption         =   "MSB"
            Height          =   195
            Left            =   1920
            TabIndex        =   194
            Top             =   240
            Width           =   315
         End
      End
      Begin VB.CommandButton ResetButton 
         Caption         =   "Reset QuickUSB"
         Height          =   495
         Left            =   -74280
         TabIndex        =   156
         Top             =   4020
         Visible         =   0   'False
         Width           =   1755
      End
      Begin VB.Frame Frame19 
         Caption         =   "Settings"
         Height          =   5655
         Index           =   0
         Left            =   -74820
         TabIndex        =   116
         Top             =   480
         Width           =   7155
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   119
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP2CFG"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   3600
            TabIndex        =   120
            Top             =   540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "EP6CFG"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
            ToolTipTextBit0 =   "BUF:0 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit1 =   "BUF:1 00=quad, 01=invalid, 10=double, 11=triple"
            ToolTipTextBit2 =   "0"
            ToolTipTextBit3 =   "SIZE 0=512 bytes, 1=1024 bytes"
            ToolTipTextBit4 =   "TYPE:0 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit5 =   "TYPE:1 00=invalid, 01=isochronous, 10=bulk, 11=interrupt"
            ToolTipTextBit6 =   "DIR 1=in, 0=out"
            ToolTipTextBit7 =   "VALID 1=activate endpoint, 0=deactivate endpoint"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   3600
            TabIndex        =   121
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IFCONFIG"
            ToolTipTextBit0 =   "IFCFG:0 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit1 =   "IFCFG:1 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit2 =   "GSTATE 0=normal, 1=GPIF state on Port E bits 0:2"
            ToolTipTextBit3 =   "ASYNC 0=sync FIFO, 1=async FIFO"
            ToolTipTextBit4 =   "IFCLKPOL 0=normal clock polarity, 1=reverse clock polarity"
            ToolTipTextBit5 =   "IFCLKOE 0=TriState, 1=drive"
            ToolTipTextBit6 =   "3048MHZ 0=30MHz, 1=48MHz"
            ToolTipTextBit7 =   "IFCLKSRC 0=external clock, 1=internal clock"
            ToolTipTextBit0 =   "IFCFG:0 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit1 =   "IFCFG:1 00=ports, 01=reserved, 10=GPIF, 11=slave FIFO"
            ToolTipTextBit2 =   "GSTATE 0=normal, 1=GPIF state on Port E bits 0:2"
            ToolTipTextBit3 =   "ASYNC 0=sync FIFO, 1=async FIFO"
            ToolTipTextBit4 =   "IFCLKPOL 0=normal clock polarity, 1=reverse clock polarity"
            ToolTipTextBit5 =   "IFCLKOE 0=TriState, 1=drive"
            ToolTipTextBit6 =   "3048MHZ 0=30MHz, 1=48MHz"
            ToolTipTextBit7 =   "IFCLKSRC 0=external clock, 1=internal clock"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   122
            Top             =   1440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FifoPinPolar"
            ToolTipTextBit0 =   "FF 0=active low, 1=active high"
            ToolTipTextBit1 =   "EF 0=active low, 1=active high"
            ToolTipTextBit2 =   "SLWR 0=active low, 1=active high"
            ToolTipTextBit3 =   "SLRD 0=active low, 1=active high"
            ToolTipTextBit4 =   "SLOE 0=active low, 1=active high"
            ToolTipTextBit5 =   "PKTEND 0=active low, 1=active high"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "FF 0=active low, 1=active high"
            ToolTipTextBit1 =   "EF 0=active low, 1=active high"
            ToolTipTextBit2 =   "SLWR 0=active low, 1=active high"
            ToolTipTextBit3 =   "SLRD 0=active low, 1=active high"
            ToolTipTextBit4 =   "SLOE 0=active low, 1=active high"
            ToolTipTextBit5 =   "PKTEND 0=active low, 1=active high"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   3600
            TabIndex        =   123
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Bit 0"
            ToolTipTextBit1 =   "HSPP Bit 1"
            ToolTipTextBit2 =   "HSPP Bit 2"
            ToolTipTextBit3 =   "HSPP Bit 3"
            ToolTipTextBit4 =   "HSPP Bit 4"
            ToolTipTextBit5 =   "HSPP Bit 5"
            ToolTipTextBit6 =   "HSPP Bit 6"
            ToolTipTextBit7 =   "HSPP Bit 7"
            ToolTipTextBit0 =   "HSPP Bit 0"
            ToolTipTextBit1 =   "HSPP Bit 1"
            ToolTipTextBit2 =   "HSPP Bit 2"
            ToolTipTextBit3 =   "HSPP Bit 3"
            ToolTipTextBit4 =   "HSPP Bit 4"
            ToolTipTextBit5 =   "HSPP Bit 5"
            ToolTipTextBit6 =   "HSPP Bit 6"
            ToolTipTextBit7 =   "HSPP Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   124
            Top             =   1140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "DataAddress"
            ToolTipTextBit0 =   "HSPP Bit 8"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Clear - Enable Address Bus, Set - Disable Address Bus"
            ToolTipTextBit7 =   "Clear - Increment Address Bus, Set - Don't Increment Address Bus"
            ToolTipTextBit0 =   "HSPP Bit 8"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Clear - Enable Address Bus, Set - Disable Address Bus"
            ToolTipTextBit7 =   "Clear - Increment Address Bus, Set - Don't Increment Address Bus"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   3600
            TabIndex        =   125
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "WORDWIDE"
            ToolTipTextBit0 =   "HSPP Data Width, 0= 8 Bit, 1=16 Bit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "HSPP Data Width, 0= 8 Bit, 1=16 Bit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   126
            ToolTipText     =   "Not Currently Used"
            Top             =   840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   3600
            TabIndex        =   127
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "Full Flag For EP2 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP2 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY1 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "Full Flag For EP2 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP2 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY1 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   128
            Top             =   2640
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SlaveFIFO"
            ToolTipTextBit0 =   "Full Flag For EP6 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP6 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY0 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "Full Flag For EP6 FIFO"
            ToolTipTextBit1 =   "Empty Flag For EP6 FIFO"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "RDY0 pin status"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   3600
            TabIndex        =   129
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "SPICONFIG"
            ToolTipTextBit0 =   "0= LSBit to MSBit, 1= MSBit to LSBit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
            ToolTipTextBit0 =   "0= LSBit to MSBit, 1= MSBit to LSBit"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "Not Currently Used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   130
            ToolTipText     =   "I/O Port E Alternate Configuration"
            Top             =   2340
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTECFG"
            ToolTipTextBit0 =   "T0OUT 0=normal, 1=timer 0 output"
            ToolTipTextBit1 =   "T1OUT 0=normal, 1=timer 1 output"
            ToolTipTextBit2 =   "T2OUT 0=normal, 1=timer 2 output"
            ToolTipTextBit3 =   "RXD0OUT 0=normal, 1=RS232 port 0 RXD"
            ToolTipTextBit4 =   "RXD1OUT 0=normal, 1=RS232 port 1 RXD"
            ToolTipTextBit5 =   "INT6 0=normal, 1=INT6 input"
            ToolTipTextBit6 =   "T2EX 0=normal, 1=timer2 restart"
            ToolTipTextBit7 =   "GPIFA8 0=normal, 1=GPIF address bit 8 output"
            ToolTipTextBit0 =   "T0OUT 0=normal, 1=timer 0 output"
            ToolTipTextBit1 =   "T1OUT 0=normal, 1=timer 1 output"
            ToolTipTextBit2 =   "T2OUT 0=normal, 1=timer 2 output"
            ToolTipTextBit3 =   "RXD0OUT 0=normal, 1=RS232 port 0 RXD"
            ToolTipTextBit4 =   "RXD1OUT 0=normal, 1=RS232 port 1 RXD"
            ToolTipTextBit5 =   "INT6 0=normal, 1=INT6 input"
            ToolTipTextBit6 =   "T2EX 0=normal, 1=timer2 restart"
            ToolTipTextBit7 =   "GPIFA8 0=normal, 1=GPIF address bit 8 output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   3600
            TabIndex        =   131
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "CPUCONFIG"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE 0=tristate CLKOUT, 1=drive CLKOUT"
            ToolTipTextBit2 =   "CLKINV 0=normal, 1=invert CLKOUT"
            ToolTipTextBit3 =   "CLKSPD:0 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit4 =   "CLKSPD:1 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit5 =   "Reserved, do not change"
            ToolTipTextBit6 =   "0"
            ToolTipTextBit7 =   "0"
            ToolTipTextBit0 =   "Reserved"
            ToolTipTextBit1 =   "CLKOE 0=tristate CLKOUT, 1=drive CLKOUT"
            ToolTipTextBit2 =   "CLKINV 0=normal, 1=invert CLKOUT"
            ToolTipTextBit3 =   "CLKSPD:0 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit4 =   "CLKSPD:1 00=12MHz, 01=24MHz, 10=48MHz, 11=reserved"
            ToolTipTextBit5 =   "Reserved, do not change"
            ToolTipTextBit6 =   "0"
            ToolTipTextBit7 =   "0"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   132
            Top             =   2040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "BusSpeed"
            ToolTipTextBit0 =   "Not Currently Used"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "USB bus speed,  0=full speed (12Mbps), 1=high speed(480Mbps)"
            ToolTipTextBit0 =   "Not Currently Used"
            ToolTipTextBit1 =   "Not Currently Used"
            ToolTipTextBit2 =   "Not Currently Used"
            ToolTipTextBit3 =   "Not Currently Used"
            ToolTipTextBit4 =   "Not Currently Used"
            ToolTipTextBit5 =   "Not Currently Used"
            ToolTipTextBit6 =   "Not Currently Used"
            ToolTipTextBit7 =   "USB bus speed,  0=full speed (12Mbps), 1=high speed(480Mbps)"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   3600
            TabIndex        =   133
            ToolTipText     =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "FPGATYPE"
            ToolTipTextBit0 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit1 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit2 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit3 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit4 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit5 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit6 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit7 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit0 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit1 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit2 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit3 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit4 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit5 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit6 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
            ToolTipTextBit7 =   "&H00=Altera passive serial, &H01=Xilinx slave serial"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   134
            ToolTipText     =   "Not Currently Used"
            Top             =   1740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   3600
            TabIndex        =   135
            ToolTipText     =   "Port A Data"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOA"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   136
            ToolTipText     =   "Port A Output Enable - Set to enable output"
            Top             =   3240
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEA"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   3600
            TabIndex        =   137
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "I2CTL"
            ToolTipTextBit0 =   "0=100KHz, 1=400KHZ I2C bus speed"
            ToolTipTextBit1 =   "Reserved, do not change"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "not currently used"
            ToolTipTextBit7 =   "not currently used"
            ToolTipTextBit0 =   "0=100KHz, 1=400KHZ I2C bus speed"
            ToolTipTextBit1 =   "Reserved, do not change"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "not currently used"
            ToolTipTextBit7 =   "not currently used"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   138
            ToolTipText     =   "Not Currently Used"
            Top             =   2940
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "TBD"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   139
            ToolTipText     =   "Port C Output Enable - Set to enable output"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEC"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   120
            TabIndex        =   140
            ToolTipText     =   "Port B Output Enable - Set to enable output"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEB"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   11
            Left            =   3600
            TabIndex        =   141
            ToolTipText     =   "Port C Data"
            Top             =   3840
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOC"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   10
            Left            =   3600
            TabIndex        =   142
            ToolTipText     =   "Port B Data"
            Top             =   3540
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOB"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   3600
            TabIndex        =   143
            ToolTipText     =   "Port E Data"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOE"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   144
            ToolTipText     =   "Port E Output Enable - Set to enable output"
            Top             =   4440
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OEE"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   3600
            TabIndex        =   145
            ToolTipText     =   "Port D Data"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "IOD"
            ToolTipTextBit0 =   "Bit 0"
            ToolTipTextBit1 =   "Bit 1"
            ToolTipTextBit2 =   "Bit 2"
            ToolTipTextBit3 =   "Bit 3"
            ToolTipTextBit4 =   "Bit 4"
            ToolTipTextBit5 =   "Bit 5"
            ToolTipTextBit6 =   "Bit 6"
            ToolTipTextBit7 =   "Bit 7"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   146
            ToolTipText     =   "Port D Output Enable - Set to enable output"
            Top             =   4140
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "OED"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
            ToolTipTextBit0 =   "Bit 0 0=input, 1=output"
            ToolTipTextBit1 =   "Bit 1 0=input, 1=output"
            ToolTipTextBit2 =   "Bit 2 0=input, 1=output"
            ToolTipTextBit3 =   "Bit 3 0=input, 1=output"
            ToolTipTextBit4 =   "Bit 4 0=input, 1=output"
            ToolTipTextBit5 =   "Bit 5 0=input, 1=output"
            ToolTipTextBit6 =   "Bit 6 0=input, 1=output"
            ToolTipTextBit7 =   "Bit 7 0=input, 1=output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   120
            TabIndex        =   147
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSAB"
            ToolTipTextBit0 =   "FLAGA0"
            ToolTipTextBit1 =   "FLAGA1"
            ToolTipTextBit2 =   "FLAGA2"
            ToolTipTextBit3 =   "FLAGA3"
            ToolTipTextBit4 =   "FLAGB0"
            ToolTipTextBit5 =   "FLAGB1"
            ToolTipTextBit6 =   "FLAGB2"
            ToolTipTextBit7 =   "FLAGB3"
            ToolTipTextBit0 =   "FLAGA0"
            ToolTipTextBit1 =   "FLAGA1"
            ToolTipTextBit2 =   "FLAGA2"
            ToolTipTextBit3 =   "FLAGA3"
            ToolTipTextBit4 =   "FLAGB0"
            ToolTipTextBit5 =   "FLAGB1"
            ToolTipTextBit6 =   "FLAGB2"
            ToolTipTextBit7 =   "FLAGB3"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   15
            Left            =   3600
            TabIndex        =   148
            Top             =   5040
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PINFLAGSCD"
            ToolTipTextBit0 =   "FLAGC0"
            ToolTipTextBit1 =   "FLAGC1"
            ToolTipTextBit2 =   "FLAGC2"
            ToolTipTextBit3 =   "FLAGC3"
            ToolTipTextBit4 =   "FLAGD0"
            ToolTipTextBit5 =   "FLAGD1"
            ToolTipTextBit6 =   "FLAGD2"
            ToolTipTextBit7 =   "FLAGD3"
            ToolTipTextBit0 =   "FLAGC0"
            ToolTipTextBit1 =   "FLAGC1"
            ToolTipTextBit2 =   "FLAGC2"
            ToolTipTextBit3 =   "FLAGC3"
            ToolTipTextBit4 =   "FLAGD0"
            ToolTipTextBit5 =   "FLAGD1"
            ToolTipTextBit6 =   "FLAGD2"
            ToolTipTextBit7 =   "FLAGD3"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingLsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   3600
            TabIndex        =   149
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTCCFG"
            ToolTipTextBit0 =   "GPIFA0 0=normal, 1=GPIF address bit 0 output"
            ToolTipTextBit1 =   "GPIFA1 0=normal, 1=GPIF address bit 1 output"
            ToolTipTextBit2 =   "GPIFA2 0=normal, 1=GPIF address bit 2 output"
            ToolTipTextBit3 =   "GPIFA3 0=normal, 1=GPIF address bit 3 output"
            ToolTipTextBit4 =   "GPIFA4 0=normal, 1=GPIF address bit 4 output"
            ToolTipTextBit5 =   "GPIFA5 0=normal, 1=GPIF address bit 5 output"
            ToolTipTextBit6 =   "GPIFA6 0=normal, 1=GPIF address bit 6 output"
            ToolTipTextBit7 =   "GPIFA7 0=normal, 1=GPIF address bit 7 output"
            ToolTipTextBit0 =   "GPIFA0 0=normal, 1=GPIF address bit 0 output"
            ToolTipTextBit1 =   "GPIFA1 0=normal, 1=GPIF address bit 1 output"
            ToolTipTextBit2 =   "GPIFA2 0=normal, 1=GPIF address bit 2 output"
            ToolTipTextBit3 =   "GPIFA3 0=normal, 1=GPIF address bit 3 output"
            ToolTipTextBit4 =   "GPIFA4 0=normal, 1=GPIF address bit 4 output"
            ToolTipTextBit5 =   "GPIFA5 0=normal, 1=GPIF address bit 5 output"
            ToolTipTextBit6 =   "GPIFA6 0=normal, 1=GPIF address bit 6 output"
            ToolTipTextBit7 =   "GPIFA7 0=normal, 1=GPIF address bit 7 output"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SettingMsbBitCheckBox 
            Height          =   255
            Index           =   14
            Left            =   120
            TabIndex        =   150
            Top             =   4740
            Width           =   3375
            _ExtentX        =   5953
            _ExtentY        =   450
            Caption         =   "PORTACFG"
            ToolTipTextBit0 =   "INT0 0=normal, 1=INT0 pin"
            ToolTipTextBit1 =   "INT1 0=normal, 1=INT1 pin"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "SLCS 0=normal, 1=configure PA7 as SLCS"
            ToolTipTextBit7 =   "FLAGD 0=normal, 1=configure PA7 as FLAGD"
            ToolTipTextBit0 =   "INT0 0=normal, 1=INT0 pin"
            ToolTipTextBit1 =   "INT1 0=normal, 1=INT1 pin"
            ToolTipTextBit2 =   "not currently used"
            ToolTipTextBit3 =   "not currently used"
            ToolTipTextBit4 =   "not currently used"
            ToolTipTextBit5 =   "not currently used"
            ToolTipTextBit6 =   "SLCS 0=normal, 1=configure PA7 as SLCS"
            ToolTipTextBit7 =   "FLAGD 0=normal, 1=configure PA7 as FLAGD"
         End
         Begin VB.Label Label13 
            Caption         =   "MSB"
            Height          =   195
            Left            =   1920
            TabIndex        =   118
            Top             =   240
            Width           =   315
         End
         Begin VB.Label Label14 
            Caption         =   "LSB"
            Height          =   195
            Left            =   5400
            TabIndex        =   117
            Top             =   240
            Width           =   315
         End
      End
      Begin VB.CheckBox WordWideCheckBox 
         Caption         =   "Word Wide"
         Height          =   255
         Left            =   360
         TabIndex        =   33
         ToolTipText     =   "Checked=16 bit command and data transfers, unchecked=8-bit command and data transfers"
         Top             =   6120
         Value           =   1  'Checked
         Width           =   1215
      End
      Begin VB.Frame Frame15 
         Caption         =   "Settings"
         Height          =   855
         Left            =   180
         TabIndex        =   91
         Top             =   4260
         Width           =   7155
         Begin VB.CommandButton SetTimeoutButton 
            Caption         =   "Set Timeout"
            CausesValidation=   0   'False
            Height          =   315
            Left            =   2040
            TabIndex        =   93
            Top             =   300
            Width           =   1095
         End
         Begin VB.TextBox TimeoutTextBox 
            Height          =   315
            Left            =   900
            TabIndex        =   92
            Text            =   "1000"
            Top             =   300
            Width           =   1035
         End
         Begin VB.Label TimeoutLabel 
            Caption         =   "Timeout"
            Height          =   195
            Left            =   180
            TabIndex        =   94
            Top             =   360
            Width           =   615
         End
      End
      Begin VB.Frame Frame17 
         Caption         =   "FIFO Setup"
         Height          =   915
         Left            =   180
         TabIndex        =   86
         Top             =   5160
         Width           =   7155
         Begin QuickUsbDiagnostics.BitCheckBox SettingFifoConfigBitCheckBox 
            Height          =   255
            Left            =   2520
            TabIndex        =   90
            Top             =   240
            Width           =   2955
            _ExtentX        =   5212
            _ExtentY        =   450
            Caption         =   ""
            ToolTipTextBit0 =   "IFCFG0"
            ToolTipTextBit1 =   "IFCFG1"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "ASYNC"
            ToolTipTextBit4 =   "IFCLKPOL"
            ToolTipTextBit5 =   "IFCLKOE"
            ToolTipTextBit6 =   "3048MHZ"
            ToolTipTextBit7 =   "IFCLKSRC"
            ToolTipTextBit0 =   "IFCFG0"
            ToolTipTextBit1 =   "IFCFG1"
            ToolTipTextBit2 =   "Reserved"
            ToolTipTextBit3 =   "ASYNC"
            ToolTipTextBit4 =   "IFCLKPOL"
            ToolTipTextBit5 =   "IFCLKOE"
            ToolTipTextBit6 =   "3048MHZ"
            ToolTipTextBit7 =   "IFCLKSRC"
         End
         Begin QuickUsbDiagnostics.BitCheckBox SlaveFifoFlagsBitCheckBox 
            Height          =   255
            Index           =   1
            Left            =   60
            TabIndex        =   88
            Top             =   540
            Width           =   2955
            _ExtentX        =   5212
            _ExtentY        =   450
            Caption         =   "SF Flags"
            ToolTipTextBit0 =   "EP6 FF"
            ToolTipTextBit1 =   "EP6 EF"
            ToolTipTextBit0 =   "EP6 FF"
            ToolTipTextBit1 =   "EP6 EF"
         End
         Begin VB.Timer SlaveFifoFlagsQueryTimer 
            Enabled         =   0   'False
            Interval        =   1000
            Left            =   180
            Top             =   240
         End
         Begin QuickUsbDiagnostics.BitCheckBox SlaveFifoFlagsBitCheckBox 
            Height          =   255
            Index           =   0
            Left            =   2520
            TabIndex        =   87
            Top             =   540
            Width           =   2955
            _ExtentX        =   5212
            _ExtentY        =   450
            Caption         =   ""
            ToolTipTextBit0 =   "EP2 FF"
            ToolTipTextBit1 =   "EP2 EF"
            ToolTipTextBit0 =   "EP2 FF"
            ToolTipTextBit1 =   "EP2 EF"
         End
         Begin VB.Label Label11 
            Caption         =   "SETTING_FIFOCONFIG"
            Height          =   195
            Left            =   840
            TabIndex        =   89
            Top             =   240
            Width           =   1755
         End
      End
      Begin VB.CheckBox SpiEndianCheckBox 
         Caption         =   "Little endian"
         Height          =   195
         Left            =   -74760
         TabIndex        =   84
         Top             =   4320
         Width           =   1275
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
         TabIndex        =   60
         Top             =   3480
         Width           =   7155
         Begin VB.TextBox RS232WriteTextBox 
            Height          =   1215
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   62
            ToolTipText     =   "Contains text to be written to the selected QuickUSB RS-232 port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.CommandButton WriteRS232Button 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   61
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
         TabIndex        =   55
         Top             =   1260
         Width           =   7155
         Begin VB.TextBox GetNumTextBox 
            BackColor       =   &H8000000F&
            Height          =   285
            Left            =   1320
            Locked          =   -1  'True
            TabIndex        =   58
            Text            =   "0"
            ToolTipText     =   "Number of characters waiting in the receive buffer"
            Top             =   240
            Width           =   495
         End
         Begin VB.CommandButton ReadRS232Button 
            Caption         =   "Read"
            Height          =   315
            Left            =   5940
            TabIndex        =   57
            ToolTipText     =   "Read the selected QuickUSB RS-232 port"
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox RS232ReadTextBox 
            Height          =   1275
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   56
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
            TabIndex        =   59
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
         TabIndex        =   50
         Top             =   2400
         Width           =   7155
         Begin VB.CommandButton SPIWriteReadButton 
            Caption         =   "WriteRead"
            Height          =   315
            Left            =   4860
            TabIndex        =   78
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox SpiWritePortNumComboBox 
            Height          =   315
            ItemData        =   "Display.frx":0522
            Left            =   840
            List            =   "Display.frx":0544
            TabIndex        =   53
            ToolTipText     =   "Selects the active QuickUSB RS-232 port"
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox SpiWriteTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   52
            ToolTipText     =   "Enter decimal or hex values (&h style) to send to the selected SPI port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.CommandButton SpiWriteButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   51
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label25 
            Caption         =   "Port"
            Height          =   195
            Left            =   120
            TabIndex        =   54
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
         TabIndex        =   43
         Top             =   480
         Width           =   7155
         Begin VB.ComboBox SpiReadPortNumComboBox 
            Height          =   315
            ItemData        =   "Display.frx":0566
            Left            =   840
            List            =   "Display.frx":0588
            TabIndex        =   48
            ToolTipText     =   "Selects the active QuickUSB RS-232 port"
            Top             =   240
            Width           =   735
         End
         Begin VB.CommandButton SpiReadButton 
            Caption         =   "Read"
            Height          =   315
            Left            =   5940
            TabIndex        =   46
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox SpiReadTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   45
            ToolTipText     =   "Contains hex values read from the selected SPI port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.TextBox SpiReadLengthTextBox 
            Height          =   315
            Left            =   2400
            TabIndex        =   44
            Text            =   "8"
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label22 
            Caption         =   "Port"
            Height          =   195
            Left            =   180
            TabIndex        =   49
            Top             =   300
            Width           =   795
         End
         Begin VB.Label Label23 
            Alignment       =   1  'Right Justify
            Caption         =   "Length"
            Height          =   195
            Left            =   1740
            TabIndex        =   47
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
         TabIndex        =   38
         Top             =   2400
         Width           =   7155
         Begin VB.CommandButton I2cWriteButton 
            Caption         =   "Write"
            Height          =   315
            Left            =   5940
            TabIndex        =   41
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox I2cWriteTextBox 
            Height          =   975
            Left            =   180
            MultiLine       =   -1  'True
            ScrollBars      =   2  'Vertical
            TabIndex        =   40
            ToolTipText     =   "Enter decimal or hex values (&h style) to send to the selected I2C port"
            Top             =   660
            Width           =   6795
         End
         Begin VB.TextBox I2cWriteAddressTextBox 
            Height          =   315
            Left            =   840
            TabIndex        =   39
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
            TabIndex        =   42
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
         Height          =   1155
         Left            =   -74820
         TabIndex        =   27
         Top             =   2280
         Width           =   7155
         Begin VB.ComboBox FpgaTypeComboBox 
            Height          =   315
            ItemData        =   "Display.frx":05AA
            Left            =   1080
            List            =   "Display.frx":05B4
            Style           =   2  'Dropdown List
            TabIndex        =   159
            Top             =   660
            Width           =   2055
         End
         Begin VB.CommandButton IsFpgaConfiguredButton 
            Caption         =   "?"
            Height          =   315
            Left            =   6720
            TabIndex        =   85
            ToolTipText     =   "Check to see if FPGA is configured"
            Top             =   240
            Width           =   255
         End
         Begin VB.CommandButton BrowseButton 
            Caption         =   "Browse"
            Height          =   315
            Left            =   180
            TabIndex        =   77
            Top             =   240
            Width           =   795
         End
         Begin VB.TextBox UploadFilenameTextBox 
            Height          =   285
            Left            =   1080
            TabIndex        =   76
            Top             =   240
            Width           =   4695
         End
         Begin VB.CommandButton FpgaUploadButton 
            Caption         =   "Upload"
            Height          =   315
            Left            =   5880
            TabIndex        =   28
            ToolTipText     =   "Upload an RBF file to the evaluation board FPGA"
            Top             =   240
            Width           =   795
         End
         Begin MSComctlLib.ProgressBar Progress 
            Height          =   75
            Left            =   1080
            TabIndex        =   37
            Top             =   540
            Width           =   4695
            _ExtentX        =   8281
            _ExtentY        =   132
            _Version        =   393216
            Appearance      =   0
            Scrolling       =   1
         End
         Begin VB.Label Label24 
            Caption         =   "FPGA Type"
            Height          =   195
            Left            =   180
            TabIndex        =   160
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
            TabIndex        =   63
            ToolTipText     =   "Flush the QuickUSB RS-232 transmit/receive buffers"
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox RS232BaudRateComboBox 
            Height          =   315
            ItemData        =   "Display.frx":05E4
            Left            =   2400
            List            =   "Display.frx":05FD
            TabIndex        =   17
            ToolTipText     =   "Selects the QuickUSB RS-232 port baud rate"
            Top             =   240
            Width           =   975
         End
         Begin VB.ComboBox RS232PortComboBox 
            Height          =   315
            ItemData        =   "Display.frx":0632
            Left            =   840
            List            =   "Display.frx":063C
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
         Left            =   -74820
         TabIndex        =   14
         Top             =   480
         Width           =   7155
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
            TabIndex        =   112
            Top             =   1300
            Width           =   4755
            Begin VB.CommandButton ReadPortBButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   113
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortBValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   114
               Top             =   480
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Value"
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortBDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   115
               Top             =   180
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Dir"
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
            TabIndex        =   108
            Top             =   3300
            Width           =   4755
            Begin VB.CommandButton ReadPortDButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   109
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortDValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   110
               Top             =   480
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Value"
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortDDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   111
               Top             =   180
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Dir"
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
            Begin QuickUsbDiagnostics.BitCheckBox PortEValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   74
               Top             =   480
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Value"
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortEDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   75
               Top             =   180
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Dir"
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
            TabIndex        =   68
            Top             =   2300
            Width           =   4755
            Begin VB.CommandButton ReadPortCButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3540
               TabIndex        =   69
               ToolTipText     =   "Read the value of port C"
               Top             =   480
               Width           =   615
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortCValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   70
               Top             =   480
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Value"
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortCDirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   71
               Top             =   180
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Dir"
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
            TabIndex        =   64
            Top             =   300
            Width           =   4755
            Begin VB.CommandButton CountPortAButton 
               Caption         =   "Count"
               Height          =   255
               Left            =   3600
               TabIndex        =   83
               Top             =   180
               Width           =   615
            End
            Begin VB.CommandButton ReadPortAButton 
               Caption         =   "Read"
               Height          =   255
               Left            =   3600
               TabIndex        =   67
               ToolTipText     =   "Read the value of port A"
               Top             =   480
               Width           =   615
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortAValueBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   65
               Top             =   480
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Value"
            End
            Begin QuickUsbDiagnostics.BitCheckBox PortADirBitCheckBox 
               Height          =   255
               Left            =   120
               TabIndex        =   66
               Top             =   180
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   450
               Caption         =   "Dir"
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
            TabIndex        =   151
            ToolTipText     =   "Write the contents of the file to the QuickUSB module in data mode"
            Top             =   240
            Width           =   675
         End
         Begin VB.TextBox BufferCountTextBox 
            Height          =   315
            Left            =   5580
            TabIndex        =   105
            ToolTipText     =   "Number of buffers to use"
            Top             =   1320
            Width           =   975
         End
         Begin VB.TextBox BufferSizeTextBox 
            Height          =   315
            Left            =   5580
            TabIndex        =   104
            ToolTipText     =   "Number of items to transfer in data mode"
            Top             =   960
            Width           =   975
         End
         Begin VB.CheckBox AsyncCheckBox 
            Caption         =   "Async"
            Height          =   195
            Left            =   2460
            TabIndex        =   103
            ToolTipText     =   "Use multi buffer asynchronous I/O if checked"
            Top             =   1320
            Value           =   1  'Checked
            Width           =   1155
         End
         Begin VB.CommandButton EditReadFileButton 
            Caption         =   "Edit"
            Height          =   315
            Left            =   4680
            TabIndex        =   100
            Top             =   600
            Width           =   495
         End
         Begin VB.CommandButton EditWriteFileButton 
            Caption         =   "Edit"
            Height          =   315
            Left            =   4680
            TabIndex        =   99
            Top             =   240
            Width           =   495
         End
         Begin VB.CommandButton BrowseReadButton 
            Height          =   315
            Left            =   3480
            Picture         =   "Display.frx":0646
            Style           =   1  'Graphical
            TabIndex        =   98
            Top             =   600
            Width           =   375
         End
         Begin VB.CommandButton BrowseWriteButton 
            Height          =   315
            Left            =   3480
            Picture         =   "Display.frx":0748
            Style           =   1  'Graphical
            TabIndex        =   97
            Top             =   240
            Width           =   375
         End
         Begin VB.TextBox ReadDataTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   96
            Top             =   600
            Width           =   2055
         End
         Begin VB.TextBox WriteDataTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   95
            Top             =   240
            Width           =   2055
         End
         Begin VB.CheckBox IncDataAddressCheckBox 
            Caption         =   "Inc Address"
            Height          =   195
            Left            =   2460
            TabIndex        =   34
            ToolTipText     =   "Increment the address during data transfers if checked"
            Top             =   1020
            Value           =   1  'Checked
            Width           =   1155
         End
         Begin VB.TextBox DataAddressTextBox 
            Height          =   315
            Left            =   1380
            TabIndex        =   29
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
            TabIndex        =   107
            Top             =   1380
            Width           =   1455
         End
         Begin VB.Label BufferSizeLabel 
            Caption         =   "Async Buffer Size"
            Height          =   195
            Left            =   3900
            TabIndex        =   106
            Top             =   1020
            Width           =   1335
         End
         Begin VB.Label Label12 
            Caption         =   "Read Filename"
            Height          =   195
            Left            =   120
            TabIndex        =   102
            Top             =   660
            Width           =   1155
         End
         Begin VB.Label Label6 
            Caption         =   "Write Filename"
            Height          =   195
            Left            =   120
            TabIndex        =   101
            Top             =   300
            Width           =   1155
         End
         Begin VB.Label Label16 
            Caption         =   "Address"
            Height          =   195
            Left            =   120
            TabIndex        =   30
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
            TabIndex        =   35
            ToolTipText     =   "Increment the address during command transfers if checked"
            Top             =   1380
            Value           =   1  'Checked
            Width           =   1215
         End
         Begin VB.TextBox CommandAddressTextBox 
            Height          =   315
            Left            =   1260
            TabIndex        =   31
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
            TabIndex        =   36
            Top             =   180
            Width           =   1455
         End
         Begin VB.Label Label19 
            Caption         =   "Address"
            Height          =   195
            Left            =   240
            TabIndex        =   32
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
      Begin QuickUsbDiagnostics.QuickUsbCtl QuickUsbCtl1 
         Height          =   735
         Left            =   -74760
         TabIndex        =   196
         Top             =   720
         Width           =   6975
         _ExtentX        =   12303
         _ExtentY        =   1296
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
         Begin VB.TextBox SerialNumberTextBox 
            BackColor       =   &H8000000F&
            Height          =   315
            Left            =   5040
            Locked          =   -1  'True
            TabIndex        =   154
            Top             =   240
            Width           =   2000
         End
         Begin VB.OptionButton FullSpeedOptionButton 
            Caption         =   "Full Speed (12Mbps)"
            Height          =   200
            Left            =   4200
            TabIndex        =   158
            Top             =   1260
            Width           =   2300
         End
         Begin VB.OptionButton HighSpeedOptionButton 
            Caption         =   "High Speed (480Mbps)"
            Height          =   200
            Left            =   4200
            TabIndex        =   157
            Top             =   1020
            Width           =   2300
         End
         Begin VB.TextBox ModuleVersion 
            BackColor       =   &H8000000F&
            Height          =   315
            Left            =   1080
            Locked          =   -1  'True
            TabIndex        =   153
            Top             =   960
            Width           =   2800
         End
         Begin VB.Label Label26 
            Caption         =   "Serial Number"
            Height          =   195
            Left            =   3780
            TabIndex        =   155
            Top             =   255
            Width           =   1020
         End
         Begin VB.Label Label17 
            Caption         =   "HW Version"
            Height          =   195
            Left            =   180
            TabIndex        =   152
            Top             =   1020
            Width           =   915
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
   Begin VB.Label Label21 
      Caption         =   "ms"
      Height          =   195
      Left            =   2520
      TabIndex        =   81
      Top             =   6660
      Width           =   255
   End
   Begin VB.Label Label7 
      Caption         =   "Period"
      Height          =   255
      Left            =   1080
      TabIndex        =   80
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
Dim lastActiveControl As control
Dim startTime As Long
Dim duration As Long
Dim oldDevList As String


Private Type MemBlockType
   bytesTransferred As Long
   transaction As Byte
   inData() As Byte
End Type


Private Sub QuickUsbCtl1_ConnectionChanged(ByVal Connected As Boolean)
   Dim ctlCount As Integer
   
   If (Connected) Then
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
      SlaveFifoFlagsQueryTimer.Enabled = True
      
      ' Initialize the form
      Form_Load
   
      UpdateSettingFifoConfigBitCheckBox
      
      ' Make sure the firmware is v2.07 or greater
      If (Val(GetFirmwareVersionString) < 2.07) Then
         SlaveFifoFlagsQueryTimer.Enabled = False
         SlaveFifoFlagsBitCheckBox(0).Enabled = False
         SlaveFifoFlagsBitCheckBox(1).Enabled = False
      End If
   Else
      ' Disable all other controls
      On Error Resume Next
      For ctlCount = 1 To Display.Controls.count - 1
         If (TypeName(Controls(ctlCount)) <> "Timer") Then
            Controls(ctlCount).Enabled = False
         End If
      Next
      
      ' Turn on the tabbed dialog, frame and rescan button
      SlaveFifoFlagsQueryTimer.Enabled = True
      FileMenu.Enabled = True
      FileExitMenu.Enabled = True
      HelpMenu.Enabled = True
      HelpAboutMenu.Enabled = True
            
      ShowStatus "Cannot find any QuickUSB modules"
   End If
End Sub


Private Sub ResetButton_Click()
   Dim result1 As Long
   Dim result2 As Long
   
   ' Start resetting the QuickUSB
'   result1 = QuickUsbCtl1.SetResetState(hDevice, 1)
   Sleep (1000)
'   result2 = QuickUsbCtl1.SetResetState(hDevice, 0)
      
   ' Notify the user
   If (result1 = 0) Or (result2 = 0) Then
      ShowStatus "QuickUSB reset failed."
   Else
      ShowStatus "QuickUSB reset successful."
   End If

End Sub


Private Sub ChangeSpeed(HighSpeed As Boolean)
   Dim result As Long
   Dim CpuConfig As Integer
   Dim NewSetting As Integer
   Dim CurrentHighSpeed As Boolean

   result = QuickUsbCtl1.ReadSetting(SETTING_CPUCONFIG, CpuConfig)
   If (result = 0) Then
      ShowStatus "Cannot get current speed Device:" & QuickUsbCtl1.devName
      Exit Sub
   End If
   
   If (CpuConfig And &H8000) Then
      CurrentHighSpeed = True
   Else
      CurrentHighSpeed = False
   End If
   
   ' Are we in the proper speed mode?
   If HighSpeed = CurrentHighSpeed Then
      ShowStatus QuickUsbCtl1.devName & " is already set to requested speed."
      Exit Sub
   End If
   
   If HighSpeed = True Then
      NewSetting = &H8000
   Else
      NewSetting = 0
   End If
   
   ' Set the CPUCS low order byte settings
   NewSetting = NewSetting Or (CpuConfig And &H7FFF)
   
   result = QuickUsbCtl1.WriteSetting(SETTING_CPUCONFIG, NewSetting)
   
   ' Wait for QuickUSB to renumerate
   Sleep (5000)
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
   
   ' Open the device
   For Index = 0 To 15 Step 1
      result = QuickUsbCtl1.ReadDefault(Index, setting)
      
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
   
   ' Disable the unused bytes
   DefaultMsbBitCheckBox(1).Visible = False
   DefaultMsbBitCheckBox(4).Visible = False
   DefaultMsbBitCheckBox(8).Visible = False
   
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
   
   ' Open the device
   For Index = 0 To 15 Step 1
      result = QuickUsbCtl1.ReadSetting(Index, setting)
      
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
   
   ' Disable the unused bytes
   SettingMsbBitCheckBox(1).Visible = False
   SettingMsbBitCheckBox(4).Visible = False
   SettingMsbBitCheckBox(8).Visible = False
   
   ' Disable the read only settings
   SettingMsbBitCheckBox(7).Enabled = False
   SettingLsbBitCheckBox(7).Enabled = False
End Sub


Private Sub UpdateDefaultSetting(Index As Integer)
   Dim result As Long
   Dim setting As Integer
   Dim LongSetting As Long
   Dim msb As Long
   Dim lsb As Long
   
   ' Get the default
   msb = DefaultMsbBitCheckBox(Index).Value
   lsb = DefaultLsbBitCheckBox(Index).Value
   LongSetting = (msb * 256) + lsb
   setting = LongSetting And &H7FFF
   ' Note kludge because integers are signed and bytes are unsigned!
   If (LongSetting And &H8000) Then
      result = QuickUsbCtl1.WriteDefault(Index, setting Or &H8000)
   Else
      result = QuickUsbCtl1.WriteDefault(Index, setting)
   End If

   If (result = 0) Then
      ShowStatus "Cannot write default " & Index
   End If
End Sub


Private Sub UpdateCurrentSetting(Index As Integer)
   Dim result As Long
   Dim setting As Integer
   Dim LongSetting As Long
   Dim msb As Long
   Dim lsb As Long
   
   msb = SettingMsbBitCheckBox(Index).Value
   lsb = SettingLsbBitCheckBox(Index).Value
   LongSetting = (msb * 256) + lsb
   setting = LongSetting And &H7FFF
   ' Note kludge because integers are signed and bytes are unsigned!
   If (LongSetting And &H8000) Then
      result = QuickUsbCtl1.WriteSetting(Index, setting Or &H8000)
   Else
      result = QuickUsbCtl1.WriteSetting(Index, setting)
   End If

   If (result = 0) Then
      ShowStatus "Cannot write setting " & Index
   End If
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
   UpdateDefaultSetting Index
End Sub

Private Sub DefaultMsbBitCheckBox_Click(Index As Integer)
   UpdateDefaultSetting Index
End Sub


Private Sub SettingLsbBitCheckBox_Click(Index As Integer)
   UpdateCurrentSetting Index
End Sub


Private Sub SettingMsbBitCheckBox_Click(Index As Integer)
   UpdateCurrentSetting Index
End Sub


Private Sub EditWriteFileButton_Click()
   Dim RetVal As Double
   On Error GoTo EditError
   RetVal = Shell("HEX.EXE " & GetShortName(WriteDataTextBox), 1)
   Exit Sub
EditError:
   ShowStatus "Hex.EXE not present in current directory!"
End Sub



Private Sub EditReadFileButton_Click()
   Dim RetVal As Double
   On Error GoTo EditError
   RetVal = Shell("HEX.EXE " & GetShortName(ReadDataTextBox), 1)
   Exit Sub
EditError:
   ShowStatus "Hex.EXE not present in current directory!"
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
      DataLengthTextBox.Text = 2097152
   Else
      DataLengthTextBox.Text = 512
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
   
   ' Set text box defaults
   CommandLengthTextBox = 8
   DataLengthTextBox = 512
   
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


Private Sub ReadPortDirBits()
   Dim result As Long
   Dim portData As Byte
   
   ' Read the port data
   result = QuickUsbCtl1.ReadPortDir(0, portData)

   ' Set the port dir bit boxes
   PortADirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbCtl1.ReadPortDir(1, portData)

   ' Set the port dir bit boxes
   PortBDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbCtl1.ReadPortDir(2, portData)

   ' Set the port dir bit boxes
   PortCDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbCtl1.ReadPortDir(3, portData)

   ' Set the port dir bit boxes
   PortDDirBitCheckBox.Value = portData

   ' Read the port data
   result = QuickUsbCtl1.ReadPortDir(4, portData)

   ' Set the port dir bit boxes
   PortEDirBitCheckBox.Value = portData

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read direction of port A"
      Exit Sub
   End If
End Sub



Private Sub PortADirBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
      
   ' Get the hex value from the text box
   portData = PortADirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePortDir(0, portData)
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port A"
      Exit Sub
   End If
End Sub



Private Sub PortBDirBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortBDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePortDir(1, portData)
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port B"
      Exit Sub
   End If
End Sub



Private Sub PortDDirBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortDDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePortDir(3, portData)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port D"
      Exit Sub
   End If
End Sub



Private Sub CountPortAButton_Click()
   Dim result As Long
   Dim count As Byte
   Dim portData(0 To 63) As Byte
      
   ' Fill the count buffer
   For count = 0 To 62
      portData(count) = count
   Next
   portData(63) = 0

   'result = QuickUsbWritePort(hDevice, 0, portData(0), 1)
   'result = QuickUsbWritePort(hDevice, 0, portData(1), 1)
   'result = QuickUsbWritePort(hDevice, 0, portData(0), 1)
   For count = 0 To 63
      QuickUsbCtl1.WritePort( 0, portData(0), 64
   Next
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port A"
      Exit Sub
   End If
End Sub



Private Sub PortCValueBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortCValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePort(2, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port C"
      Exit Sub
   End If

End Sub



Private Sub PortCDirBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortCDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePortDir(2, portData)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port C"
      Exit Sub
   End If

End Sub



Private Sub PortEValueBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortEValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePort(4, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port E"
      Exit Sub
   End If

End Sub



Private Sub PortEDirBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortEDirBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePortDir(4, portData)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write direction of port E"
      Exit Sub
   End If

End Sub



Private Sub ReadPortCButton_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortCButton
   
   ' Read the port data
   ShowStatus "Reading Port C..."
   result = QuickUsbCtl1.ReadPort(2, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port C"
      Exit Sub
   End If
   
   ' Display the hex value
   PortCValueBitCheckBox.Value = portData
End Sub



Private Sub ReadPortEButton_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortEButton
   
   ' Read the port data
   ShowStatus "Reading Port E..."
   result = QuickUsbCtl1.ReadPort(4, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port E"
      Exit Sub
   End If
   
   ' Display the hex value
   PortEValueBitCheckBox.Value = portData
End Sub



Private Sub PortAValueBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortAValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePort(0, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port A"
      Exit Sub
   End If
End Sub



Private Sub PortBValueBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortBValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePort(1, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port B"
      Exit Sub
   End If
End Sub



Private Sub PortDValueBitCheckBox_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Get the hex value from the text box
   portData = PortDValueBitCheckBox.Value

   ' Read the port data
   result = QuickUsbCtl1.WritePort(3, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write port D"
      Exit Sub
   End If
End Sub



Private Sub ReadPortAButton_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortAButton
   
   ' Read the port data
   ShowStatus "Reading Port A..."
   result = QuickUsbCtl1.ReadPort(0, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port A"
      Exit Sub
   End If
   
   ' Display the hex value
   PortAValueBitCheckBox.Value = portData
End Sub



Private Sub ReadPortBButton_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortBButton
   
   ' Read the port data
   ShowStatus "Reading Port B..."
   result = QuickUsbCtl1.ReadPort(1, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port B"
      Exit Sub
   End If
   
   ' Display the hex value
   PortBValueBitCheckBox.Value = portData
End Sub



Private Sub ReadPortDButton_Click()
   Dim result As Long
   Dim portData As Byte
   
   ' Remember this control as the last active control
   Set lastActiveControl = ReadPortDButton
   
   ' Read the port data
   ShowStatus "Reading Port D..."
   result = QuickUsbCtl1.ReadPort(3, portData, 1)

   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read port D"
      Exit Sub
   End If
   
   ' Display the hex value
   PortDValueBitCheckBox.Value = portData
End Sub



Function GetFirmwareVersionString() As String
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
   result = QuickUsbGetFirmwareVersion(QuickUsbCtl1.hDevice, majorVersion, minorVersion, buildVersion)
   
   ' Read the string descriptor
   theString = Space(160)
   result = QuickUsbGetStringDescriptor(QuickUsbCtl1.hDevice, 2, theString, Len(theString))
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
   result = QuickUsbGetStringDescriptor(QuickUsbCtl1.hDevice, 2, theString, Len(theString))
   If (result = 0) Then
      QuickUsbClose hDevice
      Exit Sub
   End If
   
   
   ' Read the string descriptor
   theString = Space(160)
   result = QuickUsbGetStringDescriptor(QuickUsbCtl1.hDevice, 3, theString, Len(theString))
   If (result = 0) Then
      QuickUsbClose hDevice
      Exit Sub
   End If
   
   ' Set the serial number string
   SerialNumberTextBox.Text = theString
   stringDescriptor = theString
   
   ' Get the FPGA type
   result = QuickUsbReadSetting(QuickUsbCtl1.hDevice, SETTING_FPGATYPE, FpgaType)
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



Private Sub CommandLengthTextBox_Change()
   Dim count As Long
   
   commandLength = Val(CommandLengthTextBox.Text)
   If (commandLength <> 0) Then
      CommandFlexGrid.Rows = commandLength
      For count = 0 To commandLength - 1
         CommandFlexGrid.TextMatrix(count, 0) = count + 1
      Next
   End If
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
   Dim result As Long
   Dim Value As Integer
   
   ' Set the new setting value
   result = QuickUsbCtl1.ReadSetting(SETTING_WORDWIDE, Value)

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
   result = QuickUsbOpen(QuickUsbCtl1.hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   Timeout = CLng(Val(TimeoutTextBox))
   result = QuickUsbSetTimeout(QuickUsbCtl1.hDevice, Timeout)

   ' Close the device when you're done
   QuickUsbClose QuickUsbCtl1.hDevice
   
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
   Dim result As Long
   Dim FpgaType As Integer
   
   ' Check for an empty filename
   If (UploadFilenameTextBox = "" Or Dir(UploadFilenameTextBox) = "") Then
      BrowseButton_Click                        ' Display the browse box
      If (UploadFilenameTextBox = "") Then      ' The user selected Cancel
         Exit Sub
      End If
   End If
      
   ' Get the current FPGA type
   result = QuickUsbCtl1.ReadSetting(SETTING_FPGATYPE, FpgaType)
   If (result = 0) Then
      MsgBox "Cannot read current FPGA type", , "Configure FPGA"
      Close inputFile
      QuickUsbClose hDevice
      Exit Sub
   End If
   FpgaType = FpgaType And &HFF
   If FpgaType <> FpgaTypeComboBox.ListIndex Then
      result = QuickUsbCtl1.WriteSetting(SETTING_FPGATYPE, FpgaTypeComboBox.ListIndex)
      If (result = 0) Then
         MsgBox "Cannot set FPGA type", , "Configure FPGA"
         Close inputFile
         QuickUsbClose hDevice
         Exit Sub
      End If
   End If
   
   ' Start configuring the FPGA
   result = QuickUsbCtl1.UploadFPGA(UploadFilenameTextBox, StatusBar)
   If (result = 0) Then
      MsgBox "Cannot FPGA configuration", , "Configure FPGA"
      Exit Sub
   End If
               
   ' Notify the user
   If (QuickUsbCtl1.IsFpgaConfigured) Then
      ShowStatus "FPGA was successfully configured"
   Else
      ShowStatus "FPGA configuration failed"
   End If
   
   Progress.Value = 0
End Sub



Private Sub IsFpgaConfiguredButton_Click()
   Dim result As Long
   
   ' Start configuring the FPGA
   result = QuickUsbCtl1.IsFpgaConfigured(isConfigured)
   
   ' Notify the user
   If (QuickUsbCtl1.IsFpgaConfigured) Then
      ShowStatus "FPGA was successfully configured"
   Else
      ShowStatus "FPGA configuration failed"
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
   
   QuickUsbGetLastError errorCode
   If (result = 0) Then
      QuickUsbGetLastError errorCode
      If (errorCode = 6) Then
         ShowStatus "Read timed out"
'         MsgBox "Timeout", , "Read Data"
      Else
         ShowStatus "Cannot read data"
'         MsgBox "Error number = " & errorCode, , "Read Data"
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
      If (errorCode = 6) Then
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
   result = QuickUsbReadSetting(hDevice, SETTING_FIFOCONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO flags setting"
      Exit Sub
   End If

   ' Read the value from the bitcheckbox
   SettingFifoConfigBitCheckBox.Value = Value
   
End Sub



Private Sub SettingFifoConfigBitCheckBox_Click()
   Dim hDevice As Long
   Dim result As Long
   Dim Value As Integer
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Read the value from the bitcheckbox
   Value = SettingFifoConfigBitCheckBox.Value
   
   ' Set the new setting value
   result = QuickUsbWriteSetting(hDevice, SETTING_FIFOCONFIG, Value)

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO flags setting"
      Exit Sub
   End If
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

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot read slave FIFO flags setting"
      Exit Sub
   End If
   
   ' Update the flags
   SlaveFifoFlagsBitCheckBox(0).Value = Value And &HF
   SlaveFifoFlagsBitCheckBox(1).Value = (Value \ &H100) And &HF
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
   
   ' Open the device
   result = QuickUsbOpen(hDevice, devName)
   If (result = 0) Then
      ShowStatus "Cannot open Device:" & devName
      Exit Sub
   End If
   
   ' Set the new setting value
   If (SpiEndianCheckBox.Value = vbChecked) Then
      result = QuickUsbWriteSetting(hDevice, SETTING_SPIENDIAN, 0)
   Else
      result = QuickUsbWriteSetting(hDevice, SETTING_SPIENDIAN, 1)
   End If

   ' Close the device when you're done
   QuickUsbClose hDevice
   
   ' Check the transaction result
   If (result = 0) Then
      ShowStatus "Cannot write SPI_ENDIAN setting"
      Exit Sub
   End If

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
   ReDim inData(length)
   
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
   ReDim inData(length)
   
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

