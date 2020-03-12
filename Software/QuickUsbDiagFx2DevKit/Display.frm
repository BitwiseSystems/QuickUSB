VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form Display 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickUSB Diagnostics for the Cypress EZUSB-FX2 Dev Kit"
   ClientHeight    =   6360
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   7215
   HelpContextID   =   1000
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   424
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   481
   StartUpPosition =   3  'Windows Default
   Begin MSComctlLib.StatusBar StatusBar 
      Align           =   2  'Align Bottom
      Height          =   375
      Left            =   0
      TabIndex        =   5
      Top             =   5985
      Width           =   7215
      _ExtentX        =   12726
      _ExtentY        =   661
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin TabDlg.SSTab SSTab 
      Height          =   4875
      Left            =   120
      TabIndex        =   2
      Top             =   1020
      Width           =   6975
      _ExtentX        =   12303
      _ExtentY        =   8599
      _Version        =   393216
      Style           =   1
      Tabs            =   4
      Tab             =   3
      TabsPerRow      =   4
      TabHeight       =   520
      TabCaption(0)   =   "High-Speed Parallel Port"
      TabPicture(0)   =   "Display.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Frame6"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Frame7"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).ControlCount=   2
      TabCaption(1)   =   "I/O Ports"
      TabPicture(1)   =   "Display.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame5(0)"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).ControlCount=   1
      TabCaption(2)   =   "RS-232"
      TabPicture(2)   =   "Display.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Frame8"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).ControlCount=   1
      TabCaption(3)   =   "I2C"
      TabPicture(3)   =   "Display.frx":0496
      Tab(3).ControlEnabled=   -1  'True
      Tab(3).Control(0)=   "Frame1"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "Frame3"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "Write"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "Frame4"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).ControlCount=   4
      Begin VB.Frame Frame8 
         Caption         =   "Terminal Window"
         Height          =   4155
         HelpContextID   =   3000
         Left            =   -74820
         TabIndex        =   50
         Top             =   480
         Width           =   6555
         Begin VB.Timer IncomingSerialTimer 
            Enabled         =   0   'False
            Interval        =   100
            Left            =   60
            Top             =   180
         End
         Begin VB.ComboBox BaudRateComboBox 
            Height          =   315
            HelpContextID   =   4002
            ItemData        =   "Display.frx":04B2
            Left            =   3240
            List            =   "Display.frx":04D1
            TabIndex        =   55
            ToolTipText     =   "Selects the QuickUSB RS-232 port baud rate"
            Top             =   300
            Width           =   1275
         End
         Begin VB.ComboBox PortComboBox 
            Height          =   315
            HelpContextID   =   4001
            ItemData        =   "Display.frx":0512
            Left            =   900
            List            =   "Display.frx":051C
            Style           =   2  'Dropdown List
            TabIndex        =   52
            Top             =   300
            Width           =   915
         End
         Begin VB.TextBox TerminalTextBox 
            Height          =   3195
            HelpContextID   =   4003
            Left            =   180
            MultiLine       =   -1  'True
            TabIndex        =   51
            Top             =   720
            Width           =   6195
         End
         Begin VB.Label Label9 
            Caption         =   "Baud Rate"
            Height          =   195
            Left            =   2160
            TabIndex        =   54
            Top             =   360
            Width           =   975
         End
         Begin VB.Label Label8 
            Caption         =   "Port"
            Height          =   195
            Left            =   240
            TabIndex        =   53
            Top             =   360
            Width           =   555
         End
      End
      Begin VB.Frame Frame7 
         Caption         =   "Read Data"
         Height          =   1875
         HelpContextID   =   1000
         Left            =   -74820
         TabIndex        =   27
         Top             =   2580
         Width           =   6555
         Begin VB.TextBox Text3 
            Height          =   1035
            HelpContextID   =   1008
            Left            =   180
            MultiLine       =   -1  'True
            TabIndex        =   36
            Top             =   660
            Width           =   6195
         End
         Begin VB.CommandButton Command4 
            Caption         =   "Browse"
            Height          =   315
            HelpContextID   =   2006
            Left            =   4200
            TabIndex        =   35
            Top             =   240
            Width           =   1035
         End
         Begin VB.TextBox Text2 
            Height          =   315
            HelpContextID   =   2005
            Left            =   1320
            TabIndex        =   34
            Top             =   240
            Width           =   2775
         End
         Begin VB.CommandButton Command3 
            Caption         =   "Read File"
            Height          =   315
            HelpContextID   =   2007
            Left            =   5340
            TabIndex        =   32
            Top             =   240
            Width           =   1035
         End
         Begin VB.Label Label7 
            Caption         =   "Read to file"
            Height          =   195
            Left            =   180
            TabIndex        =   33
            Top             =   300
            Width           =   1035
         End
      End
      Begin VB.Frame Frame6 
         Caption         =   "Write Data"
         Height          =   1875
         HelpContextID   =   1000
         Left            =   -74820
         TabIndex        =   26
         Top             =   480
         Width           =   6555
         Begin VB.TextBox Text4 
            Height          =   975
            HelpContextID   =   2004
            Left            =   180
            MultiLine       =   -1  'True
            TabIndex        =   37
            Top             =   720
            Width           =   6195
         End
         Begin VB.CommandButton Command2 
            Caption         =   "Browse"
            Height          =   315
            HelpContextID   =   2002
            Left            =   4200
            TabIndex        =   31
            Top             =   300
            WhatsThisHelpID =   1002
            Width           =   1035
         End
         Begin VB.TextBox Text1 
            Height          =   315
            HelpContextID   =   2001
            Left            =   1320
            TabIndex        =   30
            Top             =   300
            Width           =   2775
         End
         Begin VB.CommandButton Command1 
            Caption         =   "Send File"
            Height          =   315
            HelpContextID   =   2003
            Left            =   5340
            TabIndex        =   28
            Top             =   300
            Width           =   1035
         End
         Begin VB.Label Label6 
            Caption         =   "File to write"
            Height          =   195
            Left            =   180
            TabIndex        =   29
            Top             =   360
            Width           =   1035
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "I/O Port Controls"
         Height          =   4155
         HelpContextID   =   2000
         Index           =   0
         Left            =   -74820
         TabIndex        =   25
         Top             =   480
         Width           =   6555
         Begin VB.Frame Frame12 
            Caption         =   "Port E - P6, Pins 12-19"
            Height          =   1035
            Left            =   300
            TabIndex        =   43
            Top             =   2820
            Width           =   4695
            Begin VB.CommandButton ReadPortButton 
               Caption         =   "Read"
               Height          =   615
               HelpContextID   =   3009
               Index           =   4
               Left            =   3600
               TabIndex        =   47
               Top             =   240
               Width           =   915
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortBitCheckBox 
               Height          =   315
               HelpContextID   =   3008
               Index           =   4
               Left            =   180
               TabIndex        =   48
               Top             =   600
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Value"
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortDirBitCheckBox 
               Height          =   315
               HelpContextID   =   3007
               Index           =   4
               Left            =   180
               TabIndex        =   49
               Top             =   240
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Direction"
            End
         End
         Begin VB.Frame Frame11 
            Caption         =   "Port C - P3, Pins 12-19"
            Height          =   1035
            Left            =   300
            TabIndex        =   42
            Top             =   1560
            Width           =   4695
            Begin VB.CommandButton ReadPortButton 
               Caption         =   "Read"
               Height          =   615
               HelpContextID   =   3006
               Index           =   2
               Left            =   3600
               TabIndex        =   44
               Top             =   240
               Width           =   915
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortBitCheckBox 
               Height          =   315
               HelpContextID   =   3005
               Index           =   2
               Left            =   180
               TabIndex        =   45
               Top             =   600
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Value"
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortDirBitCheckBox 
               Height          =   315
               HelpContextID   =   3004
               Index           =   2
               Left            =   180
               TabIndex        =   46
               Top             =   240
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Direction"
            End
         End
         Begin VB.Frame Frame10 
            Caption         =   "Port A - P2, Pins 12-19"
            Height          =   1035
            Left            =   300
            TabIndex        =   38
            Top             =   360
            Width           =   4695
            Begin VB.CommandButton ReadPortButton 
               Caption         =   "Read"
               Height          =   615
               HelpContextID   =   3003
               Index           =   0
               Left            =   3600
               TabIndex        =   39
               Top             =   240
               Width           =   915
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortBitCheckBox 
               Height          =   315
               HelpContextID   =   3002
               Index           =   0
               Left            =   180
               TabIndex        =   40
               ToolTipText     =   "0=Low, 1=High"
               Top             =   600
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Value"
            End
            Begin QuickUsbDiagFx2DevKit.BitCheckBox IoPortDirBitCheckBox 
               Height          =   315
               HelpContextID   =   3001
               Index           =   0
               Left            =   180
               TabIndex        =   41
               ToolTipText     =   "0=In, 1=Out"
               Top             =   240
               Width           =   3315
               _ExtentX        =   5847
               _ExtentY        =   556
               Caption         =   "Pin Direction"
            End
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "General Purpose Read"
         Height          =   1935
         HelpContextID   =   4000
         Left            =   3600
         TabIndex        =   13
         Top             =   2520
         Width           =   3135
         Begin VB.TextBox ReadI2CAddress 
            Height          =   315
            HelpContextID   =   5007
            Left            =   1080
            TabIndex        =   21
            Text            =   "&h0"
            ToolTipText     =   "I2C address to read from.  Try &h20 to read from the buttons or &h21 to read back the 7 seg display value."
            Top             =   360
            Width           =   1755
         End
         Begin VB.TextBox ReadI2CData 
            Height          =   315
            HelpContextID   =   5008
            Left            =   1080
            TabIndex        =   20
            Text            =   "&h0"
            ToolTipText     =   "Data read from I2C"
            Top             =   780
            Width           =   1755
         End
         Begin VB.CommandButton ReadI2CButton 
            Caption         =   "Read Byte from I2C (JP9)"
            Height          =   375
            HelpContextID   =   5009
            Left            =   240
            TabIndex        =   19
            Top             =   1260
            Width           =   2595
         End
         Begin VB.Label Label4 
            Caption         =   "Address"
            Height          =   195
            Left            =   180
            TabIndex        =   23
            Top             =   420
            Width           =   675
         End
         Begin VB.Label Label3 
            Caption         =   "Data"
            Height          =   195
            Left            =   180
            TabIndex        =   22
            Top             =   840
            Width           =   675
         End
      End
      Begin VB.Frame Write 
         Caption         =   "General Purpose Write"
         Height          =   1875
         HelpContextID   =   4000
         Left            =   3600
         TabIndex        =   12
         Top             =   480
         Width           =   3135
         Begin VB.CommandButton WriteI2CButton 
            Caption         =   "Write Byte to I2C (JP9)"
            Height          =   375
            HelpContextID   =   5006
            Left            =   240
            TabIndex        =   18
            Top             =   1260
            Width           =   2595
         End
         Begin VB.TextBox WriteI2CData 
            Height          =   315
            HelpContextID   =   5005
            Left            =   1080
            TabIndex        =   17
            Text            =   "&h0"
            ToolTipText     =   "I2C data to send"
            Top             =   780
            Width           =   1755
         End
         Begin VB.TextBox WriteI2CAddress 
            Height          =   315
            HelpContextID   =   5004
            Left            =   1080
            TabIndex        =   16
            Text            =   "&h0"
            ToolTipText     =   "I2C address to write to.  Try &H20 to control the 7 seg display."
            Top             =   360
            Width           =   1755
         End
         Begin VB.Label Label2 
            Caption         =   "Data"
            Height          =   195
            Left            =   180
            TabIndex        =   15
            Top             =   840
            Width           =   675
         End
         Begin VB.Label Label1 
            Caption         =   "Address"
            Height          =   195
            Left            =   180
            TabIndex        =   14
            Top             =   420
            Width           =   675
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "Buttons  (Watch out for RESET#)"
         Height          =   915
         HelpContextID   =   4000
         Left            =   180
         TabIndex        =   7
         Top             =   480
         Width           =   3195
         Begin VB.Timer ButtonSenseTimer 
            Enabled         =   0   'False
            Interval        =   250
            Left            =   60
            Top             =   180
         End
         Begin VB.CheckBox ButtonCheckBox 
            Caption         =   "f4"
            Enabled         =   0   'False
            ForeColor       =   &H00000000&
            Height          =   435
            HelpContextID   =   5001
            Index           =   3
            Left            =   2340
            Style           =   1  'Graphical
            TabIndex        =   11
            ToolTipText     =   "State of f4"
            Top             =   300
            Width           =   495
         End
         Begin VB.CheckBox ButtonCheckBox 
            Caption         =   "f3"
            Enabled         =   0   'False
            ForeColor       =   &H00000000&
            Height          =   435
            HelpContextID   =   5001
            Index           =   2
            Left            =   1680
            Style           =   1  'Graphical
            TabIndex        =   10
            ToolTipText     =   "State of f3"
            Top             =   300
            Width           =   495
         End
         Begin VB.CheckBox ButtonCheckBox 
            Caption         =   "f2"
            Enabled         =   0   'False
            ForeColor       =   &H00000000&
            Height          =   435
            HelpContextID   =   5001
            Index           =   1
            Left            =   1020
            Style           =   1  'Graphical
            TabIndex        =   9
            ToolTipText     =   "State of f2"
            Top             =   300
            Width           =   495
         End
         Begin VB.CheckBox ButtonCheckBox 
            Caption         =   "f1"
            Enabled         =   0   'False
            ForeColor       =   &H00000000&
            Height          =   435
            HelpContextID   =   5001
            Index           =   0
            Left            =   360
            Style           =   1  'Graphical
            TabIndex        =   8
            ToolTipText     =   "State of f1"
            Top             =   300
            Width           =   495
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "Seven Segment Display"
         Height          =   2955
         HelpContextID   =   4000
         Left            =   180
         TabIndex        =   3
         Top             =   1500
         Width           =   3195
         Begin MSComctlLib.ImageList LedImageList 
            Left            =   120
            Top             =   240
            _ExtentX        =   1005
            _ExtentY        =   1005
            BackColor       =   -2147483643
            ImageWidth      =   160
            ImageHeight     =   120
            MaskColor       =   12632256
            _Version        =   393216
            BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
               NumListImages   =   32
               BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":052E
                  Key             =   ""
               EndProperty
               BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":7A97
                  Key             =   ""
               EndProperty
               BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":F250
                  Key             =   ""
               EndProperty
               BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":1649F
                  Key             =   ""
               EndProperty
               BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":1D7A7
                  Key             =   ""
               EndProperty
               BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":24C2C
                  Key             =   ""
               EndProperty
               BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":2BE3B
                  Key             =   ""
               EndProperty
               BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":32FB2
                  Key             =   ""
               EndProperty
               BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":3A5B5
                  Key             =   ""
               EndProperty
               BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":41579
                  Key             =   ""
               EndProperty
               BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":488E5
                  Key             =   ""
               EndProperty
               BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":4FAAD
                  Key             =   ""
               EndProperty
               BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":56D2B
                  Key             =   ""
               EndProperty
               BeginProperty ListImage14 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":5E156
                  Key             =   ""
               EndProperty
               BeginProperty ListImage15 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":653FB
                  Key             =   ""
               EndProperty
               BeginProperty ListImage16 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":6C665
                  Key             =   ""
               EndProperty
               BeginProperty ListImage17 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":73AB6
                  Key             =   ""
               EndProperty
               BeginProperty ListImage18 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":7B013
                  Key             =   ""
               EndProperty
               BeginProperty ListImage19 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":82699
                  Key             =   ""
               EndProperty
               BeginProperty ListImage20 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":898CC
                  Key             =   ""
               EndProperty
               BeginProperty ListImage21 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":90AFC
                  Key             =   ""
               EndProperty
               BeginProperty ListImage22 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":97EB3
                  Key             =   ""
               EndProperty
               BeginProperty ListImage23 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":9F07E
                  Key             =   ""
               EndProperty
               BeginProperty ListImage24 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":A6170
                  Key             =   ""
               EndProperty
               BeginProperty ListImage25 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":AD62F
                  Key             =   ""
               EndProperty
               BeginProperty ListImage26 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":B4486
                  Key             =   ""
               EndProperty
               BeginProperty ListImage27 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":BB6D0
                  Key             =   ""
               EndProperty
               BeginProperty ListImage28 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":C27C0
                  Key             =   ""
               EndProperty
               BeginProperty ListImage29 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":C99AC
                  Key             =   ""
               EndProperty
               BeginProperty ListImage30 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":D0D53
                  Key             =   ""
               EndProperty
               BeginProperty ListImage31 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":D7F2D
                  Key             =   ""
               EndProperty
               BeginProperty ListImage32 {2C247F27-8591-11D1-B16A-00C0F0283628} 
                  Picture         =   "Display.frx":DF13C
                  Key             =   ""
               EndProperty
            EndProperty
         End
         Begin VB.CheckBox DecimalCheckBox 
            Caption         =   "Decimal"
            Height          =   315
            HelpContextID   =   5003
            Left            =   1860
            TabIndex        =   6
            ToolTipText     =   "Check to turn on the decimal point"
            Top             =   2400
            Width           =   915
         End
         Begin VB.ComboBox LedComboBox 
            Height          =   315
            HelpContextID   =   5002
            ItemData        =   "Display.frx":E646A
            Left            =   960
            List            =   "Display.frx":E649E
            Style           =   2  'Dropdown List
            TabIndex        =   4
            ToolTipText     =   "Led value"
            Top             =   2400
            Width           =   795
         End
         Begin VB.Label Label5 
            Caption         =   "Value"
            Height          =   195
            Left            =   360
            TabIndex        =   24
            Top             =   2460
            Width           =   555
         End
         Begin VB.Image LedImage 
            Height          =   1815
            Left            =   300
            Stretch         =   -1  'True
            Top             =   360
            Width           =   2475
         End
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "QuickUSB Modules"
      Height          =   795
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   6975
      Begin QuickUsbDiagFx2DevKit.QuickUsbCtl QuickUsbCtl 
         Height          =   375
         HelpContextID   =   1001
         Left            =   120
         TabIndex        =   1
         Top             =   300
         Width           =   6675
         _ExtentX        =   11774
         _ExtentY        =   661
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Connected       =   -1  'True
      End
   End
   Begin VB.Menu helpMenu 
      Caption         =   "&Help"
      Begin VB.Menu helpContentsMenu 
         Caption         =   "&Contents"
      End
      Begin VB.Menu helpIndexMenu 
         Caption         =   "&Index"
      End
      Begin VB.Menu aboutMenu 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "Display"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Const SEG_TOP = 1
Const SEG_TOPRIGHT = 2
Const SEG_BOTTOMRIGHT = 4
Const SEG_BOTTOM = 8
Const SEG_BOTTOMLEFT = 16
Const SEG_TOPLEFT = 32
Const SEG_MIDDLE = 64
Const SEG_DECIMAL = 128


' HTML Help Constants
Const HH_DISPLAY_TOPIC = &H0
Const HH_HELP_FINDER = &H0            'WinHelp equivalent
Const HH_DISPLAY_TOC = &H1
Const HH_DISPLAY_INDEX = &H2
Const HH_DISPLAY_SEARCH = &H3
Const HH_SET_WIN_TYPE = &H4
Const HH_GET_WIN_TYPE = &H5
Const HH_GET_WIN_HANDLE = &H6
Const HH_ENUM_INFO_TYPE = &H7           ' Get Info type name, call repeatedly to enumerate, -1 at end
Const HH_SET_INFO_TYPE = &H8            ' Add Info type to filter.
Const HH_SYNC = &H9
Const HH_RESERVED1 = &HA
Const HH_RESERVED2 = &HB
Const HH_RESERVED3 = &HC
Const HH_KEYWORD_LOOKUP = &HD
Const HH_DISPLAY_TEXT_POPUP = &HE       ' display string resource id or text in a popup window
Const HH_HELP_CONTEXT = &HF             ' display mapped numeric value in dwData
Const HH_TP_HELP_CONTEXTMENU = &H10     ' text popup help, same as WinHelp HELP_CONTEXTMENU
Const HH_TP_HELP_WM_HELP = &H11         ' text popup help, same as WinHelp HELP_WM_HELP
Const HH_CLOSE_ALL = &H12               ' close all windows opened directly or indirectly by the caller
Const HH_ALINK_LOOKUP = &H13            ' ALink version of HH_KEYWORD_LOOKUP
Const HH_GET_LAST_ERROR = &H14          ' not currently implemented // See HHERROR.h
Const HH_ENUM_CATEGORY = &H15          ' Get category name, call repeatedly to enumerate, -1 at end
Const HH_ENUM_CATEGORY_IT = &H16        ' Get category info type members, call repeatedly to enumerate, -1 at end
Const HH_RESET_IT_FILTER = &H17         ' Clear the info type filter of all info types.
Const HH_SET_INCLUSIVE_FILTER = &H18    ' set inclusive filtering method for untyped topics to be included in display
Const HH_SET_EXCLUSIVE_FILTER = &H19    ' set exclusive filtering method for untyped topics to be excluded from display
Const HH_INITIALIZE = &H1C               ' Initializes the help system.
Const HH_UNINITIALIZE = &H1D             ' Uninitializes the help system.
Const HH_PRETRANSLATEMESSAGE = &HFD     ' Pumps messages. (NULL, NULL, MSG*).
Const HH_SET_GLOBAL_PROPERTY = &HFC     ' Set a global property. (NULL, NULL, HH_GPROP)


Private Declare Function HtmlHelp Lib "HHCtrl.ocx" Alias "HtmlHelpA" (ByVal hwndCaller As Long, ByVal pszFile As String, ByVal uCommand As Long, dwData As Any) As Long


Private Sub Form_Load()
   SSTab_Click 0
End Sub



Private Sub helpContentsMenu_Click()
   HtmlHelp Display.hwnd, App.HelpFile, HH_DISPLAY_TOC, 0
End Sub



Private Sub helpIndexMenu_Click()
   HtmlHelp Display.hwnd, App.HelpFile, HH_DISPLAY_INDEX, 0
End Sub



Private Sub aboutMenu_Click()
   frmAbout.Show vbModal
End Sub



Private Function DecToSevenSegment(num As Byte) As Byte
   Dim retVal As Byte
   
   Select Case (num And &HF)
   Case 0
      retVal = SEG_TOP + SEG_TOPRIGHT + SEG_BOTTOMRIGHT + SEG_BOTTOM + SEG_BOTTOMLEFT + SEG_TOPLEFT
   Case 1
      retVal = SEG_TOPRIGHT + SEG_BOTTOMRIGHT
   Case 2
      retVal = SEG_TOP + SEG_TOPRIGHT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOM
   Case 3
      retVal = SEG_TOP + SEG_TOPRIGHT + SEG_MIDDLE + SEG_BOTTOMRIGHT + SEG_BOTTOM
   Case 4
      retVal = SEG_TOPLEFT + SEG_MIDDLE + SEG_TOPRIGHT + SEG_BOTTOMRIGHT
   Case 5
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_MIDDLE + SEG_BOTTOMRIGHT + SEG_BOTTOM
   Case 6
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOMRIGHT + SEG_BOTTOM
   Case 7
      retVal = SEG_TOP + SEG_TOPRIGHT + SEG_BOTTOMRIGHT
   Case 8
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_TOPRIGHT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOMRIGHT + SEG_BOTTOM
   Case 9
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_TOPRIGHT + SEG_MIDDLE + SEG_BOTTOMRIGHT
   Case &HA
      retVal = SEG_TOP + SEG_TOPRIGHT + SEG_TOPLEFT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOMRIGHT
   Case &HB
      retVal = SEG_TOPLEFT + SEG_BOTTOMLEFT + SEG_MIDDLE + SEG_BOTTOMRIGHT + SEG_BOTTOM
   Case &HC
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_BOTTOMLEFT + SEG_BOTTOM
   Case &HD
      retVal = SEG_TOPRIGHT + SEG_BOTTOMRIGHT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOM
   Case &HE
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_MIDDLE + SEG_BOTTOMLEFT + SEG_BOTTOM
   Case &HF
      retVal = SEG_TOP + SEG_TOPLEFT + SEG_MIDDLE + SEG_BOTTOMLEFT
   End Select
   
   If num > 15 Then
      retVal = retVal + SEG_DECIMAL
   End If
   
   DecToSevenSegment = retVal
End Function



Private Sub QuickUsbCtl_ConnectionChanged(ByVal Connected As Boolean)
   If Connected Then
      ' Select the RS232 defaults
      PortComboBox.ListIndex = 0       ' SIO-0
      BaudRateComboBox.ListIndex = 3   ' 9600 baud
      IncomingSerialTimer.Enabled = True
      
      ' Read the port value and direction
      ReadPortButton_Click 0
      ReadPortButton_Click 2
      ReadPortButton_Click 4
      
      ' Update LED display
      LedComboBox.Enabled = True
      ReadLedValue
      ButtonSenseTimer.Enabled = True
   Else
      ButtonSenseTimer.Enabled = False
      LedComboBox.Enabled = False
      
      If (QuickUsbCtl.devName <> "" And Mid(QuickUsbCtl.StringDescriptor, 1, 1) = Chr(0)) Then
         WriteRamFromResource QuickUsbCtl.devName, "QUICKUSBV1.30RC4.BIX", 101
      End If
   End If
End Sub



Private Sub IoPortBitCheckBox_Click(Index As Integer)
   Dim dir As Byte
   Dim Value As Byte
   
   ' Set the direction to output
   QuickUsbCtl.ReadPortDir Index, dir
   QuickUsbCtl.WritePortDir Index, dir Or Value
   
   ' Set the data value
   Value = IoPortBitCheckBox(Index).Value
   QuickUsbCtl.WritePort Index, Value
End Sub



Private Sub IoPortDirBitCheckBox_Click(Index As Integer)
   Dim dir As Byte
   Dim Value As Byte
   
   ' Set the direction to output
   Value = IoPortDirBitCheckBox(Index).Value
   QuickUsbCtl.WritePortDir Index, Value
End Sub



Private Sub ReadPortButton_Click(Index As Integer)
   Dim dir As Byte
   Dim Value As Byte
   
   ' Set the direction to output
   QuickUsbCtl.ReadPortDir Index, dir
   IoPortDirBitCheckBox(Index).Value = dir
   
   QuickUsbCtl.ReadPort Index, Value
   IoPortBitCheckBox(Index).Value = Value
End Sub


Private Sub ButtonSenseTimer_Timer()
   Dim Index As Byte
   Dim buttonValue As Byte
   Static oldButtonValue As Byte
   
   ' Exit if the device is disconnected
   If (QuickUsbCtl.Connected = False) Then
      Exit Sub
   End If
   
   ' Read the buttons
   QuickUsbCtl.ReadI2C &H20, buttonValue
   
   ' Invert it
   buttonValue = 255 - buttonValue
   
   ' Update the display
   For Index = 0 To ButtonCheckBox.count - 1
      If (buttonValue And 2 ^ Index) Then
         ButtonCheckBox(Index).Value = vbChecked
      Else
         ButtonCheckBox(Index).Value = vbUnchecked
      End If
   Next
   
   ' Update the display if the value changed
   If (buttonValue <> oldButtonValue) Then
      StatusBar.SimpleText = "ButtonValue = " & buttonValue
   End If
   oldButtonValue = buttonValue
End Sub



Private Sub ReadLedValue()
   Dim count As Byte
   Dim Value As Byte
   Dim ledValue As Byte
   
   QuickUsbCtl.ReadI2C &H21, ledValue
   ledValue = &HFF - ledValue
   
   For count = 0 To 31
      Value = DecToSevenSegment(count)
      If (Value = ledValue) Then
         Exit For
      End If
   Next
   
   If (count = 32) Then
      count = 0
   End If
   
   LedComboBox.ListIndex = count And &HF
   If (count > 15) Then
      DecimalCheckBox.Value = vbChecked
   Else
      DecimalCheckBox.Value = vbUnchecked
   End If
   
   On Error Resume Next
   LedImage = LedImageList.ListImages(count + 1).Picture
End Sub



Private Sub LedComboBox_Click()
   Dim Index As Byte
   Dim ledValue As Byte
   
   Index = LedComboBox.ListIndex
   If (DecimalCheckBox.Value = vbChecked) Then
      Index = Index + 16
   End If
   
   ledValue = DecToSevenSegment(Index)
   QuickUsbCtl.WriteI2C &H21, &HFF - ledValue
   On Error Resume Next
   If (ledValue And SEG_DECIMAL) Then
      LedImage = LedImageList.ListImages(LedComboBox.ListIndex + 1 + 16).Picture
   Else
      LedImage = LedImageList.ListImages(LedComboBox.ListIndex + 1).Picture
   End If
End Sub



Private Sub DecimalCheckBox_Click()
   LedComboBox_Click
End Sub



Private Sub ReadI2CButton_Click()
   Dim i2cData As Byte
   
   QuickUsbCtl.ReadI2C Val(ReadI2CAddress), i2cData
   ReadI2CData = i2cData
End Sub



Private Sub BaudRateComboBox_Click()
   Dim baudRate As Long
   
   baudRate = BaudRateComboBox.List(BaudRateComboBox.ListIndex)
   QuickUsbCtl.SetRS232BaudRate baudRate
End Sub



Private Sub SSTab_Click(PreviousTab As Integer)
   Select Case SSTab.Tab
   Case 0
      SSTab.HelpContextID = 2000
   Case 1
      SSTab.HelpContextID = 3000
   Case 2
      SSTab.HelpContextID = 4000
   Case 3
      SSTab.HelpContextID = 5000
   End Select
End Sub



Private Sub TerminalTextBox_KeyPress(KeyAscii As Integer)
   Dim Data(0) As Byte
   
   Data(0) = KeyAscii
   QuickUsbCtl.WriteRS232 PortComboBox.ListIndex, Data
End Sub



Private Sub IncomingSerialTimer_Timer()
   Dim numChars As Integer
   Dim str As String
   
   QuickUsbCtl.GetNumRS232 PortComboBox.ListIndex, numChars
   If (numChars <> 0) Then
      str = QuickUsbCtl.ReadRS232String(PortComboBox.ListIndex, numChars)
      TerminalTextBox.Text = TerminalTextBox.Text & "(" & str & ")"
   End If
End Sub


Private Sub WriteI2CButton_Click()
   QuickUsbCtl.WriteI2C Val(WriteI2CAddress), Val(WriteI2CData)
End Sub
