VERSION 5.00
Begin VB.Form Display 
   Caption         =   "QuickUsb VB6 Application"
   ClientHeight    =   3360
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   5370
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3360
   ScaleWidth      =   5370
   StartUpPosition =   3  'Windows Default
   Begin QuickUsbVb6App.QuickUsbCtl QuickUsbCtl1 
      Height          =   795
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   5175
      _ExtentX        =   9128
      _ExtentY        =   1402
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
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
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

Private Sub mnuHelpAbout_Click()
   frmAbout.Show vbModal
End Sub
