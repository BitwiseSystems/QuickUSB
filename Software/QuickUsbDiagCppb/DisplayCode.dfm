object Display: TDisplay
  Left = 317
  Top = 457
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'QuickUSB Diagnostics'
  ClientHeight = 428
  ClientWidth = 360
  Color = clBtnFace
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010002002020100000000000E80200002600000010101000000000002801
    00000E0300002800000020000000400000000100040000000000800200000000
    0000000000000000000000000000000000000000800000800000008080008000
    0000800080008080000080808000C0C0C0000000FF0000FF000000FFFF00FF00
    0000FF00FF00FFFF0000FFFFFF00000000000000000000000000000000000000
    0000000000000000000000000000000300000000000000000000000000000000
    30000000000000000000000000000000B3000000000000000000000000000000
    0B3000000000000000000000000000000BB30000000000000000000000000000
    00BB300000000000000000000000000000BBB300000000000000000000000000
    000BBB30000000000000000000000000000BBBB3000000000000000000000000
    0000BBBB3000000000000000000000000000BBBBB30000000000000000000000
    00000BBBBB300000000000000000000000000BBBBBB300000000000000000000
    000BBBBBBBBB300000000000000000000000BBBBBB3333000000000000000000
    0000BBBBBB300000000000000000000000000BBBBBB300000000000000000000
    00000BBBBBBB30000000000000000000000000BBBBBBB3000000000000000000
    000000BBBBBBBB3000000000000000000000000BBBBBBBB30000000000000000
    0000000BBBBBBBBB300000000000000000000000BBBBBBBBB300000000000000
    00000000BBBBBBBBBB30000000000000000000000BBBBBBBBBB3000000000000
    000000000BBBBBBBBBBB3000000000000000000000BBBBBBBBBBB30000000000
    0000000000BBBBBBBBBBBB300000000000000000000BBBBBBBBBBBB300000000
    0000000000000000000000000000FFFFFFFFCFFFFFFFC7FFFFFFE3FFFFFFE1FF
    FFFFF0FFFFFFF07FFFFFF83FFFFFF81FFFFFFC0FFFFFFC07FFFFFE03FFFFFE01
    FFFFFF00FFFFFC007FFFFC003FFFFE001FFFFE000FFFFF007FFFFF003FFFFF80
    1FFFFF800FFFFFC007FFFFC003FFFFE001FFFFE000FFFFF0007FFFF0003FFFF8
    001FFFF8000FFFFC0007FFFC0003280000001000000020000000010004000000
    0000C00000000000000000000000000000000000000000000000000080000080
    00000080800080000000800080008080000080808000C0C0C0000000FF0000FF
    000000FFFF00FF000000FF00FF00FFFF0000FFFFFF0000000000000000000000
    00000000000000300000000000000003000000000000000B3000000000000000
    B300000000000000BB30000000000000BBB3000000000000BBB0000000000000
    0BB30000000000000BBB30000000000000BBB3000000000000BBBB3000000000
    000BBBB300000000000BBBBB30000000000000000000BFFF00009FFF00008FFF
    0000C7FF0000C3FF0000E1FF0000C0FF0000E07F0000E03F0000F07F0000F03F
    0000F81F0000F80F0000FC070000FC030000FE010000}
  Menu = MainMenu
  OldCreateOrder = False
  Visible = True
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label12: TLabel
    Left = 12
    Top = 24
    Width = 38
    Height = 13
    Caption = 'Address'
  end
  object Label13: TLabel
    Left = 12
    Top = 52
    Width = 33
    Height = 13
    Caption = 'Length'
  end
  object Label14: TLabel
    Left = 36
    Top = 112
    Width = 56
    Height = 13
    Caption = 'Inc Address'
  end
  object Label15: TLabel
    Left = 16
    Top = 28
    Width = 38
    Height = 13
    Caption = 'Address'
  end
  object Label16: TLabel
    Left = 16
    Top = 56
    Width = 33
    Height = 13
    Caption = 'Length'
  end
  object Label17: TLabel
    Left = 40
    Top = 116
    Width = 56
    Height = 13
    Caption = 'Inc Address'
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 406
    Width = 360
    Height = 22
    Panels = <>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 360
    Height = 406
    Align = alClient
    BevelOuter = bvLowered
    BorderWidth = 4
    Caption = 'Panel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label35: TLabel
      Left = 88
      Top = 378
      Width = 30
      Height = 13
      Caption = 'Period'
    end
    object Label11: TLabel
      Left = 184
      Top = 378
      Width = 13
      Height = 13
      Caption = 'ms'
    end
    object Label37: TLabel
      Left = 32
      Top = 378
      Width = 35
      Height = 13
      Caption = 'Repeat'
    end
    object PageControl: TPageControl
      Left = 5
      Top = 5
      Width = 350
      Height = 364
      ActivePage = PortsTabSheet
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnChange = PageControlChange
      object GeneralTabSheet: TTabSheet
        Caption = 'General'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object GeneralGroupBox: TGroupBox
          Left = 8
          Top = 12
          Width = 309
          Height = 73
          Caption = 'General'
          TabOrder = 0
          object Label1: TLabel
            Left = 16
            Top = 20
            Width = 35
            Height = 13
            Caption = 'Module'
          end
          object Label2: TLabel
            Left = 16
            Top = 44
            Width = 53
            Height = 13
            Caption = 'Description'
          end
          object ModuleComboBox: TComboBox
            Left = 84
            Top = 16
            Width = 77
            Height = 21
            Hint = 'Selects the active QuickUsb Module'
            Style = csDropDownList
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = ModuleComboBoxClick
          end
          object StringDescriptorEdit: TEdit
            Left = 84
            Top = 40
            Width = 205
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 1
          end
          object CycleButton: TButton
            Left = 168
            Top = 16
            Width = 57
            Height = 21
            Caption = 'Cycle'
            TabOrder = 2
            OnClick = CycleButtonClick
          end
          object ResetButton: TButton
            Left = 232
            Top = 16
            Width = 57
            Height = 21
            Caption = 'Reset'
            TabOrder = 3
            OnClick = ResetButtonClick
          end
        end
        object GroupBox1: TGroupBox
          Left = 8
          Top = 92
          Width = 309
          Height = 85
          Caption = 'FPGA'
          TabOrder = 1
          object FpgaUploadButton: TButton
            Left = 128
            Top = -156
            Width = 65
            Height = 29
            Caption = 'Upload'
            TabOrder = 0
          end
          object BrowseButton: TButton
            Left = 12
            Top = 48
            Width = 53
            Height = 21
            Hint = 'Select an RBF file to upload to the evaluation board FPGA'
            Caption = 'Browse'
            TabOrder = 1
            OnClick = BrowseButtonClick
          end
          object Progress: TProgressBar
            Left = 84
            Top = 20
            Width = 201
            Height = 21
            Smooth = True
            TabOrder = 2
          end
          object UploadFilenameTextBox: TEdit
            Left = 84
            Top = 44
            Width = 201
            Height = 21
            ReadOnly = True
            TabOrder = 3
            Text = 'Please select a file to upload'
          end
          object Button1: TButton
            Left = 12
            Top = 20
            Width = 53
            Height = 21
            Hint = 'Upload an RBF file to the evaluation board FPGA'
            Caption = 'Upload'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 4
            OnClick = FpgaUploadButtonClick
          end
        end
      end
      object CmdDataTabSheet: TTabSheet
        Caption = 'Cmd/Data'
        ImageIndex = 1
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Label3: TLabel
          Left = 12
          Top = 24
          Width = 38
          Height = 13
          Caption = 'Address'
        end
        object Label4: TLabel
          Left = 12
          Top = 52
          Width = 33
          Height = 13
          Caption = 'Length'
        end
        object Label5: TLabel
          Left = 36
          Top = 112
          Width = 56
          Height = 13
          Caption = 'Inc Address'
        end
        object WordWideLabel: TLabel
          Left = 40
          Top = 318
          Width = 54
          Height = 13
          Caption = 'Word Wide'
        end
        object GroupBox2: TGroupBox
          Left = 8
          Top = 12
          Width = 309
          Height = 141
          Caption = 'Command'
          TabOrder = 0
          object Label9: TLabel
            Left = 12
            Top = 24
            Width = 38
            Height = 13
            Caption = 'Address'
          end
          object Label10: TLabel
            Left = 12
            Top = 52
            Width = 33
            Height = 13
            Caption = 'Length'
          end
          object Lablel1: TLabel
            Left = 36
            Top = 112
            Width = 56
            Height = 13
            Caption = 'Inc Address'
          end
          object Label19: TLabel
            Left = 152
            Top = 12
            Width = 77
            Height = 13
            Caption = 'Command Editor'
          end
          object CommandLengthTextBox: TEdit
            Left = 68
            Top = 44
            Width = 73
            Height = 21
            Hint = 'Number of items to transfer in command mode'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = CommandLengthTextBoxChange
          end
          object WriteCommandButton: TButton
            Left = 68
            Top = 76
            Width = 65
            Height = 25
            Hint = 
              'Write '#39'length'#39' items from the command editor to the QuickUSB mod' +
              'ule'
            Caption = 'Write'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnClick = WriteCommandButtonClick
          end
          object NoIncCommandAddressCheckBox: TCheckBox
            Left = 12
            Top = 112
            Width = 13
            Height = 13
            Hint = 'Increment the address during command transfers if checked'
            Caption = 'CheckBox1'
            Checked = True
            ParentShowHint = False
            ShowHint = True
            State = cbChecked
            TabOrder = 2
          end
          object ReadCommandButton: TButton
            Left = 8
            Top = 76
            Width = 53
            Height = 25
            Hint = 
              'Read '#39'length'#39' items from the QuickUSB module into the command ed' +
              'itor'
            Caption = 'Read'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
            OnClick = ReadCommandButtonClick
          end
          object CommandAddressTextBox: TEdit
            Left = 68
            Top = 20
            Width = 73
            Height = 21
            Hint = 'Starting GPIFADR value for the command mode transfer'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 4
            Text = '0'
          end
          object CommandFlexGrid: TStringGrid
            Left = 153
            Top = 27
            Width = 120
            Height = 102
            Hint = 'Command editor for command mode data'
            ColCount = 2
            DefaultColWidth = 48
            DefaultRowHeight = 14
            RowCount = 8
            FixedRows = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
            ParentShowHint = False
            ScrollBars = ssVertical
            ShowHint = True
            TabOrder = 5
            RowHeights = (
              14
              14
              14
              14
              14
              14
              14
              14)
          end
        end
        object GroupBox3: TGroupBox
          Left = 8
          Top = 160
          Width = 309
          Height = 153
          Caption = 'Data'
          TabOrder = 1
          object Label6: TLabel
            Left = 8
            Top = 104
            Width = 38
            Height = 13
            Caption = 'Address'
          end
          object Label7: TLabel
            Left = 125
            Top = 104
            Width = 33
            Height = 13
            Caption = 'Length'
          end
          object Label8: TLabel
            Left = 36
            Top = 76
            Width = 56
            Height = 13
            Caption = 'Inc Address'
          end
          object Label18: TLabel
            Left = 128
            Top = 76
            Width = 29
            Height = 13
            Caption = 'Async'
          end
          object Label36: TLabel
            Left = 8
            Top = 128
            Width = 59
            Height = 13
            Caption = 'Buffer Count'
          end
          object Label38: TLabel
            Left = 125
            Top = 128
            Width = 51
            Height = 13
            Caption = 'Buffer Size'
          end
          object ReadDataButton: TButton
            Left = 245
            Top = 15
            Width = 50
            Height = 21
            Hint = 'Read '#39'length'#39' items from the QuickUSB module in data mode'
            Caption = 'Read'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = ReadDataButtonClick
          end
          object DataAddressTextBox: TEdit
            Left = 55
            Top = 100
            Width = 60
            Height = 21
            Hint = 'Starting GPIFADR address for the data mode transfer'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Text = '0'
          end
          object DataLengthTextBox: TEdit
            Left = 165
            Top = 100
            Width = 73
            Height = 21
            Hint = 'Number of items to transfer in data mode'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
          end
          object WriteDataButton: TButton
            Left = 245
            Top = 48
            Width = 50
            Height = 21
            Hint = 'Write the file data to the QuickUSB module in data mode'
            Caption = 'Write'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
            OnClick = WriteDataButtonClick
          end
          object NoIncDataAddressCheckBox: TCheckBox
            Left = 12
            Top = 76
            Width = 12
            Height = 13
            Hint = 'Increment the address during data transfers if checked'
            Caption = 'CheckBox1'
            Checked = True
            ParentShowHint = False
            ShowHint = True
            State = cbChecked
            TabOrder = 4
          end
          object WriteCountDataButton: TButton
            Left = 245
            Top = 81
            Width = 50
            Height = 21
            Hint = 'Write a count sequence to the QuickUSB module in data mode'
            Caption = 'Count'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 5
            OnClick = WriteCountDataButtonClick
          end
          object AsyncCheckBox: TCheckBox
            Left = 110
            Top = 76
            Width = 12
            Height = 13
            Hint = 'Asynchronous I/O if checked'
            Caption = 'AsyncCheckBox'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 6
          end
          object ReadDataBrowseButton: TButton
            Left = 8
            Top = 15
            Width = 41
            Height = 21
            Caption = 'Browse'
            TabOrder = 7
            OnClick = ReadDataBrowseButtonClick
          end
          object WriteDataBrowseButton: TButton
            Left = 8
            Top = 48
            Width = 41
            Height = 21
            Caption = 'Browse'
            TabOrder = 8
            OnClick = WriteDataBrowseButtonClick
          end
          object ReadFileEditBox: TEdit
            Left = 56
            Top = 15
            Width = 177
            Height = 21
            TabOrder = 9
          end
          object WriteFileEditBox: TEdit
            Left = 56
            Top = 48
            Width = 177
            Height = 21
            TabOrder = 10
          end
          object BufferCountEditBox: TEdit
            Left = 75
            Top = 124
            Width = 40
            Height = 21
            Hint = 'A number from 2 to 253'
            ParentShowHint = False
            ReadOnly = True
            ShowHint = True
            TabOrder = 11
            Visible = False
          end
          object BufferSizeEditBox: TEdit
            Left = 184
            Top = 124
            Width = 70
            Height = 21
            ReadOnly = True
            TabOrder = 12
            Visible = False
          end
        end
        object WordWideCheckBox: TCheckBox
          Left = 20
          Top = 318
          Width = 12
          Height = 13
          Hint = 
            'Checked =16-bit command and data transfers, unchecked = 8-bit co' +
            'mmand and data transfers'
          Caption = 'WordWideCheckBox'
          Checked = True
          State = cbChecked
          TabOrder = 2
          OnClick = WordWideCheckBoxClick
        end
      end
      object PortsTabSheet: TTabSheet
        Caption = 'Ports'
        ImageIndex = 2
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object GroupBox4: TGroupBox
          Left = 4
          Top = 8
          Width = 309
          Height = 293
          Caption = 'Port I/O'
          TabOrder = 0
          object GroupBox5: TGroupBox
            Left = 8
            Top = 16
            Width = 289
            Height = 81
            Caption = 'Port A'
            TabOrder = 0
            object Label20: TLabel
              Left = 8
              Top = 20
              Width = 13
              Height = 13
              Caption = 'Dir'
            end
            object Label21: TLabel
              Left = 8
              Top = 52
              Width = 27
              Height = 13
              Caption = 'Value'
            end
            object ReadPortAButton: TButton
              Left = 232
              Top = 45
              Width = 49
              Height = 21
              Caption = 'Read'
              TabOrder = 0
              OnClick = ReadPortAButtonClick
            end
            inline PortADirBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 8
              Width = 179
              Height = 34
              TabOrder = 1
              ExplicitLeft = 48
              ExplicitTop = 8
              inherited CheckBox8: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortADirBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
            inline PortAValueBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 40
              Width = 179
              Height = 34
              TabOrder = 2
              ExplicitLeft = 48
              ExplicitTop = 40
              inherited CheckBox8: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortAValueBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
          end
          object GroupBox6: TGroupBox
            Left = 8
            Top = 112
            Width = 289
            Height = 81
            Caption = 'Port C'
            TabOrder = 1
            object Label22: TLabel
              Left = 8
              Top = 20
              Width = 13
              Height = 13
              Caption = 'Dir'
            end
            object Label23: TLabel
              Left = 8
              Top = 52
              Width = 27
              Height = 13
              Caption = 'Value'
            end
            inline PortCDirBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 8
              Width = 179
              Height = 34
              TabOrder = 0
              ExplicitLeft = 48
              ExplicitTop = 8
              inherited CheckBox8: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortCDirBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
            inline PortCValueBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 40
              Width = 179
              Height = 34
              TabOrder = 2
              ExplicitLeft = 48
              ExplicitTop = 40
              inherited CheckBox8: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortCValueBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
            object ReadPortCButton: TButton
              Left = 232
              Top = 45
              Width = 49
              Height = 21
              Caption = 'Read'
              TabOrder = 1
              OnClick = ReadPortCButtonClick
            end
          end
          object GroupBox7: TGroupBox
            Left = 8
            Top = 200
            Width = 289
            Height = 81
            Caption = 'Port E'
            TabOrder = 2
            object Label24: TLabel
              Left = 8
              Top = 20
              Width = 13
              Height = 13
              Caption = 'Dir'
            end
            object Label25: TLabel
              Left = 8
              Top = 52
              Width = 27
              Height = 13
              Caption = 'Value'
            end
            object ReadPortEButton: TButton
              Left = 232
              Top = 45
              Width = 49
              Height = 21
              Caption = 'Read'
              TabOrder = 2
              OnClick = ReadPortEButtonClick
            end
            inline PortEDirBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 8
              Width = 179
              Height = 34
              TabOrder = 0
              ExplicitLeft = 48
              ExplicitTop = 8
              inherited CheckBox8: TCheckBox
                Top = 9
                OnClick = PortEDirBitCheckBoxClick
                ExplicitTop = 9
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortEDirBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
            inline PortEValueBitCheckBox: TBitCheckBoxFrame
              Left = 48
              Top = 40
              Width = 179
              Height = 34
              TabOrder = 1
              ExplicitLeft = 48
              ExplicitTop = 40
              inherited CheckBox8: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox7: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox6: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox5: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox4: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox3: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox2: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited CheckBox1: TCheckBox
                OnClick = PortEValueBitCheckBoxClick
              end
              inherited ByteBox: TEdit
                Top = 6
                ExplicitTop = 6
              end
            end
          end
        end
      end
      object Rs232TabSheet: TTabSheet
        Caption = 'RS-232'
        ImageIndex = 3
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object GroupBox8: TGroupBox
          Left = 8
          Top = 8
          Width = 305
          Height = 49
          Caption = 'General'
          TabOrder = 0
          object Label26: TLabel
            Left = 12
            Top = 20
            Width = 19
            Height = 13
            Caption = 'Port'
          end
          object Label27: TLabel
            Left = 116
            Top = 20
            Width = 25
            Height = 13
            Caption = 'Baud'
          end
          object RS232PortComboBox: TComboBox
            Left = 44
            Top = 16
            Width = 61
            Height = 21
            Hint = 'Selects the active QuickUSB RS-232 port'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = RS232PortComboBoxChange
            Items.Strings = (
              '0'
              '1')
          end
          object RS232BaudRateComboBox: TComboBox
            Left = 148
            Top = 16
            Width = 73
            Height = 21
            Hint = 'Selects the QuickUSB RS-232 port baud rate'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnClick = RS232BaudRateComboBoxChange
            Items.Strings = (
              '4800'
              '9600'
              '19200'
              '38400'
              '57600')
          end
          object FlushRS232Button: TButton
            Left = 228
            Top = 16
            Width = 65
            Height = 21
            Hint = 
              'Removes any characters in the receive buffer of the active Quick' +
              'USB RS-232 port'
            Caption = 'Flush'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            OnClick = FlushRS232ButtonClick
          end
        end
        object GroupBox9: TGroupBox
          Left = 8
          Top = 60
          Width = 305
          Height = 105
          Caption = 'Read'
          TabOrder = 1
          object Label28: TLabel
            Left = 8
            Top = 20
            Width = 66
            Height = 13
            Caption = 'Chars Waiting'
          end
          object ReadRS232Button: TButton
            Left = 228
            Top = 16
            Width = 65
            Height = 21
            Hint = 'Read the selected QuickUSB RS-232 port'
            Caption = 'Read'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = ReadRS232ButtonClick
          end
          object GetNumTextBox: TEdit
            Left = 88
            Top = 16
            Width = 41
            Height = 21
            Hint = 'Number of characters waiting in the receive buffer'
            Color = clBtnFace
            ParentShowHint = False
            ReadOnly = True
            ShowHint = True
            TabOrder = 1
          end
          object RS232ReadTextBox: TRichEdit
            Left = 8
            Top = 44
            Width = 285
            Height = 49
            Hint = 
              'Contains text read from be written to the selected QuickUSB RS-2' +
              '32 port'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ReadOnly = True
            ShowHint = True
            TabOrder = 2
          end
        end
        object GroupBox10: TGroupBox
          Left = 8
          Top = 172
          Width = 305
          Height = 105
          Caption = 'Write'
          TabOrder = 2
          object WriteRS232Button: TButton
            Left = 228
            Top = 12
            Width = 65
            Height = 21
            Hint = 'Write the text below to the selected QuickUSB RS-232 port'
            Caption = 'Write'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = WriteRS232ButtonClick
          end
          object RS232WriteTextBox: TRichEdit
            Left = 8
            Top = 40
            Width = 285
            Height = 57
            Hint = 
              'Contains text read from or to be written to the selected QuickUS' +
              'B RS-232 port'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
          end
        end
      end
      object TabSheet5: TTabSheet
        Caption = 'SPI'
        ImageIndex = 4
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object GroupBox11: TGroupBox
          Left = 8
          Top = 16
          Width = 309
          Height = 129
          Caption = 'Read'
          TabOrder = 0
          object Label29: TLabel
            Left = 12
            Top = 24
            Width = 19
            Height = 13
            Caption = 'Port'
          end
          object Label30: TLabel
            Left = 128
            Top = 24
            Width = 33
            Height = 13
            Caption = 'Length'
          end
          object SpiReadPortNumComboBox: TComboBox
            Left = 48
            Top = 20
            Width = 57
            Height = 21
            Hint = 'Selects the active QuickUSB RS-232 port'
            Style = csDropDownList
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Items.Strings = (
              '0'
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9')
          end
          object SpiReadLengthTextBox: TEdit
            Left = 172
            Top = 20
            Width = 45
            Height = 21
            Hint = 'The number of bytes to read'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Text = '8'
          end
          object SpiReadButton: TButton
            Left = 232
            Top = 20
            Width = 65
            Height = 21
            Hint = 'Read the selected QuickUSB SPI port'
            Caption = 'Read'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            OnClick = SpiReadButtonClick
          end
          object SpiReadTextBox: TRichEdit
            Left = 8
            Top = 48
            Width = 281
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ReadOnly = True
            ShowHint = True
            TabOrder = 3
          end
        end
        object GroupBox12: TGroupBox
          Left = 8
          Top = 160
          Width = 309
          Height = 129
          Caption = 'Write'
          TabOrder = 1
          object Label31: TLabel
            Left = 12
            Top = 24
            Width = 19
            Height = 13
            Caption = 'Port'
          end
          object SpiWritePortNumComboBox: TComboBox
            Left = 48
            Top = 20
            Width = 57
            Height = 21
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Items.Strings = (
              '0'
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9')
          end
          object SpiWriteButton: TButton
            Left = 232
            Top = 20
            Width = 65
            Height = 21
            Caption = 'Write'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnClick = SpiWriteButtonClick
          end
          object SpiWriteTextBox: TRichEdit
            Left = 8
            Top = 48
            Width = 281
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
          end
          object SpiWriteReadButton: TButton
            Left = 152
            Top = 20
            Width = 65
            Height = 21
            Caption = 'WriteRead'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
            OnClick = SpiWriteReadButtonClick
          end
        end
      end
      object TabSheet6: TTabSheet
        Caption = 'I2C'
        ImageIndex = 5
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object GroupBox13: TGroupBox
          Left = 8
          Top = 16
          Width = 309
          Height = 129
          Caption = 'Read'
          TabOrder = 0
          object Label32: TLabel
            Left = 12
            Top = 24
            Width = 38
            Height = 13
            Caption = 'Address'
          end
          object Label33: TLabel
            Left = 128
            Top = 24
            Width = 33
            Height = 13
            Caption = 'Length'
          end
          object I2cReadLengthTextBox: TEdit
            Left = 172
            Top = 20
            Width = 45
            Height = 21
            TabOrder = 0
            Text = '8'
          end
          object I2cReadButton: TButton
            Left = 232
            Top = 20
            Width = 65
            Height = 21
            Caption = 'Read'
            TabOrder = 1
            OnClick = I2cReadButtonClick
          end
          object I2cReadAddressTextBox: TEdit
            Left = 60
            Top = 20
            Width = 45
            Height = 21
            TabOrder = 2
            Text = '0'
          end
          object I2cReadTextBox: TRichEdit
            Left = 8
            Top = 48
            Width = 281
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ReadOnly = True
            TabOrder = 3
          end
        end
        object GroupBox14: TGroupBox
          Left = 8
          Top = 160
          Width = 309
          Height = 129
          Caption = 'Write'
          TabOrder = 1
          object Label34: TLabel
            Left = 12
            Top = 24
            Width = 38
            Height = 13
            Caption = 'Address'
          end
          object I2cWriteButton: TButton
            Left = 232
            Top = 20
            Width = 65
            Height = 21
            Caption = 'Write'
            TabOrder = 0
            OnClick = I2cWriteButtonClick
          end
          object I2cWriteAddressTextBox: TEdit
            Left = 60
            Top = 20
            Width = 45
            Height = 21
            TabOrder = 1
            Text = '0'
          end
          object I2cWriteTextBox: TRichEdit
            Left = 8
            Top = 48
            Width = 281
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
        end
      end
    end
    object RepeatCheckBox: TCheckBox
      Left = 8
      Top = 376
      Width = 12
      Height = 17
      Hint = 'Repeats the last button clicked at the specified period'
      Caption = 'Repeat'
      TabOrder = 1
      OnClick = RepeatCheckBoxClick
    end
    object RepeatTextBox: TEdit
      Left = 129
      Top = 379
      Width = 49
      Height = 21
      Hint = 'The period at which the last button will be clicked'
      TabOrder = 2
      Text = '500'
      OnClick = RepeatTextBoxChange
    end
  end
  object MainMenu: TMainMenu
    Left = 28
    Top = 400
    object FileMenu: TMenuItem
      Caption = '&File'
      object FileExit: TMenuItem
        Caption = 'E&xit'
        OnClick = FileExitClick
      end
    end
    object HelpMenu: TMenuItem
      Caption = '&Help'
      object HelpAbout: TMenuItem
        Caption = '&About'
        OnClick = HelpAboutClick
      end
    end
  end
  object ModuleScanTimer: TTimer
    Interval = 3000
    OnTimer = ModuleScanTimerTimer
    Top = 400
  end
  object OpenDialog: TOpenDialog
    Filter = 'Altera RBF Files|*.rbf'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 53
    Top = 401
  end
  object StatusBarTimer: TTimer
    OnTimer = StatusBarTimerTimer
    Left = 113
    Top = 400
  end
  object RepeatTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = RepeatTimerTimer
    Left = 80
    Top = 400
  end
  object DataOpenDialog: TOpenDialog
    Filter = 'All files|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 144
    Top = 400
  end
end
