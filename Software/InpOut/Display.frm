VERSION 5.00
Begin VB.Form Display 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "InpOut"
   ClientHeight    =   1395
   ClientLeft      =   45
   ClientTop       =   360
   ClientWidth     =   3285
   Icon            =   "Display.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1395
   ScaleWidth      =   3285
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox InpDataTextBox 
      BackColor       =   &H8000000F&
      Height          =   315
      Left            =   2100
      Locked          =   -1  'True
      TabIndex        =   5
      Text            =   "0"
      Top             =   420
      Width           =   915
   End
   Begin VB.TextBox OutDataTextBox 
      Height          =   315
      Left            =   2100
      TabIndex        =   4
      Text            =   "0"
      Top             =   900
      Width           =   915
   End
   Begin VB.TextBox OutAddressTextBox 
      Height          =   315
      Left            =   960
      TabIndex        =   3
      Text            =   "0"
      Top             =   900
      Width           =   915
   End
   Begin VB.TextBox InpAddressTextBox 
      Height          =   315
      Left            =   960
      TabIndex        =   2
      Text            =   "0"
      Top             =   420
      Width           =   915
   End
   Begin VB.CommandButton OutButton 
      Caption         =   "Out"
      Height          =   315
      Left            =   180
      TabIndex        =   1
      Top             =   900
      Width           =   555
   End
   Begin VB.CommandButton InpButton 
      Caption         =   "Inp"
      Height          =   315
      Left            =   180
      TabIndex        =   0
      Top             =   420
      Width           =   555
   End
   Begin VB.Label Label2 
      Caption         =   "Data"
      Height          =   195
      Left            =   2100
      TabIndex        =   7
      Top             =   120
      Width           =   915
   End
   Begin VB.Label Label1 
      Caption         =   "Address"
      Height          =   195
      Left            =   960
      TabIndex        =   6
      Top             =   120
      Width           =   915
   End
End
Attribute VB_Name = "Display"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub Form_Load()
   InitInpOut
End Sub


Private Sub Command1_Click()

End Sub


Private Sub InpButton_Click()
   Dim result As Byte
   Dim address As Integer
   
   ' Get the address from the text box
   address = Val(InpAddressTextBox)
   
   ' Read the data from QuickUSb
   result = Inp(address)
   
   InpDataTextBox = result
End Sub

Private Sub OutButton_Click()
   Dim result As Byte
   Dim address As Integer
   Dim theData As Byte
   
   ' Get the address from the text box
   address = Val(OutAddressTextBox)
   
   ' Get the data from the text box
   theData = Val(OutDataTextBox)
   
   ' Read the data from QuickUSb
   Out address, theData
   
End Sub
