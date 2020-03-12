Attribute VB_Name = "AVIFile256"
Option Explicit

'-----------------------------------------------------------------------------
' Constants
'-----------------------------------------------------------------------------
Public Const OF_WRITE = 1
Public Const OF_CREATE = 4096

Public Const AVIIF_LIST = &H1
Public Const AVIIF_TWOCC = &H2
Public Const AVIIF_KEYFRAME = &H10

Public Const ICQUALITY_LOW = 0
Public Const ICQUALITY_HIGH = 10000
Public Const ICQUALITY_DEFAULT = -1

Const BM As Integer = &H4D42       ' BM
Const BI_RGB As Long = 0


'-----------------------------------------------------------------------------
' User Defined Types
'-----------------------------------------------------------------------------
Public Type RECT
   Left As Long
   Top As Long
   Right As Long
   Bottom As Long
End Type

Public Type AVISTREAMINFO
   fccType As Long
   fccHandler As Long
   dwFlags As Long
   dwCaps As Long
   wPriority As Integer
   wLanguage As Integer
   dwScale As Long
   dwRate As Long
   dwStart As Long
   dwLength As Long
   dwInitialFrames As Long
   dwSuggestedBufferSize As Long
   dwQuality As Long
   dwSampleSize As Long
   rcFrame As RECT
   dwEditCount As Long
   dwFormatChangeCount As Long
   szName(0 To 63) As String
End Type

Public Type RGBQUAD
   rgbBlue As Byte
   rgbGreen As Byte
   rgbRed As Byte
   rgbReserved As Byte
End Type

Public Type RGBTRIPLE
   rgbtBlue As Byte
   rgbtGreen As Byte
   rgbtRed As Byte
End Type

Public Type BITMAPINFOHEADER
   biSize As Long
   biWidth As Long
   biHeight As Long
   biPlanes As Integer
   biBitCount As Integer
   biCompression As Long
   biSizeImage As Long
   biXPelsPerMeter As Long
   biYPelsPerMeter As Long
   biClrUsed As Long
   biClrImportant As Long
End Type

Public Type BITMAPINFO256
   bmiHeader As BITMAPINFOHEADER
   bmiColors(0 To 255) As RGBQUAD
End Type

Public Type BITMAPFILEHEADER
   bfType As Integer
   bfSize As Long
   bfReserved1 As Integer
   bfReserved2 As Integer
   bfOffBits As Long
End Type



'-----------------------------------------------------------------------------
' External Function Declarations
'-----------------------------------------------------------------------------
Private Declare Sub CopyMemory Lib "Kernel32" Alias "RtlMoveMemory" ( _
   lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)

Public Declare Sub AVIFileInit Lib "AVIFIL32" ()
Public Declare Sub AVIFileExit Lib "AVIFIL32" ()
Public Declare Function AVIFileOpen Lib "AVIFIL32" ( _
   fp As Long, _
   ByVal fileName As String, _
   ByVal mode As Integer, _
   ByVal handler As String _
) As Long
Public Declare Function AVIFileRelease Lib "AVIFIL32" ( _
   ByVal fp As Long _
) As Long
Public Declare Function AVIFileCreateStream Lib "AVIFIL32" ( _
   ByVal fp As Long, _
   sp As Long, _
   ByRef streamInfo As AVISTREAMINFO _
) As Long
Public Declare Function AVIStreamSetFormat Lib "AVIFIL32" ( _
   ByVal sp As Long, _
   ByVal lPos As Long, _
   lpFormat As Any, _
   ByVal length As Long _
) As Long
Public Declare Function AVIStreamWrite Lib "AVIFIL32" ( _
   ByVal sp As Long, _
   ByVal start As Long, _
   ByVal samples As Long, _
   lpData As Any, _
   ByVal length As Long, _
   ByVal flags As Long, _
   samplesWritten As Long, _
   bytesWritten As Long _
) As Long
Public Declare Function AVIStreamRelease Lib "AVIFIL32" ( _
   ByVal sp As Long _
) As Long


'-----------------------------------------------------------------------------
' Module private variables
'-----------------------------------------------------------------------------
Public BitmapInfo As BITMAPINFO256
Dim isOpened As Boolean


Public Function MKFOURCC(ch0 As Byte, ch1 As Byte, ch2 As Byte, ch3 As Byte) As Long
   MKFOURCC = ch0 Or (ch1 * &H100) Or (ch2 * &H10000) Or (ch3 * &H1000000)
End Function



Public Function CreateAVI256(fp As Long, sp As Long, fileName As String, rows As Integer, cols As Integer, frameRate As Integer) As Boolean
   Dim count As Long
   Dim result As Long
   Dim si As AVISTREAMINFO
      
   ' Copy file header into BitmapInfo header for the AVI file
   BitmapInfo.bmiHeader.biBitCount = 8
   BitmapInfo.bmiHeader.biClrImportant = 0
   BitmapInfo.bmiHeader.biClrUsed = 256
   BitmapInfo.bmiHeader.biCompression = 0
   BitmapInfo.bmiHeader.biHeight = -rows
   BitmapInfo.bmiHeader.biPlanes = 1
   BitmapInfo.bmiHeader.biSize = Len(BitmapInfo.bmiHeader)
   BitmapInfo.bmiHeader.biSizeImage = CLng(rows) * CLng(cols)
   BitmapInfo.bmiHeader.biWidth = cols
   BitmapInfo.bmiHeader.biXPelsPerMeter = 11811
   BitmapInfo.bmiHeader.biYPelsPerMeter = 11811
   
   ' Write a blank palette
   For count = 0 To 255
      BitmapInfo.bmiColors(count).rgbBlue = count
      BitmapInfo.bmiColors(count).rgbGreen = count
      BitmapInfo.bmiColors(count).rgbRed = count
      BitmapInfo.bmiColors(count).rgbReserved = 0
   Next
      
   AVIFileInit
   
   result = AVIFileOpen(fp, fileName, OF_CREATE Or OF_WRITE, vbNullString)
   If (result <> 0) Then
      MsgBox "Cannot open/create AVI file: error " & result
      CreateAVI256 = False
      Exit Function
   End If
   
   ' Set up the AVI file information
   si.fccType = MKFOURCC(Asc("v"), Asc("i"), Asc("d"), Asc("s"))
   si.fccHandler = 0
   'si.dwQuality = -1
   si.dwScale = 1
   si.dwRate = frameRate
   With BitmapInfo.bmiHeader
      si.dwSuggestedBufferSize = .biWidth * .biHeight
      si.rcFrame.Left = 0
      si.rcFrame.Top = 0
      si.rcFrame.Right = .biWidth
      si.rcFrame.Bottom = .biHeight
   End With
   
   result = AVIFileCreateStream(fp, sp, si)
   If (result <> 0) Then
      MsgBox "Cannot create AVI file stream: error " & result
      AVIFileRelease fp
      AVIFileExit
      CreateAVI256 = False
      Exit Function
   End If
   
   result = AVIStreamSetFormat(sp, 0, BitmapInfo, Len(BitmapInfo))
   If (result <> 0) Then
      MsgBox "Cannot set AVI stream format: error " & result
      CreateAVI256 = False
      Exit Function
   End If
   
   isOpened = True
   CreateAVI256 = True
End Function



Public Function WriteAVIFrame256(sp As Long, frameNum As Long, image() As Byte, Optional flags As Integer = AVIIF_KEYFRAME) As Boolean
   Dim result As Long
   
   If (isOpened) Then
      result = AVIStreamWrite(sp, frameNum, 1, image(0), ((UBound(image) - LBound(image)) + 1), flags, 0, 0)
      If (result <> 0) Then
         MsgBox "Cannot write AVI stream data: error " & result
         WriteAVIFrame256 = False
         Exit Function
      End If
   End If
End Function



Public Function CloseAVI256(fp As Long, sp As Long) As Boolean
   isOpened = False
   AVIStreamRelease sp
   AVIFileRelease fp
   AVIFileExit
End Function



