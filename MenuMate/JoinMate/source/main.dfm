object frmMain: TfrmMain
  Tag = 1024
  Left = 224
  Top = 239
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'frmMain'
  ClientHeight = 324
  ClientWidth = 573
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 20
  object lbProg: TLabel
    Left = 0
    Top = 280
    Width = 63
    Height = 20
    Caption = 'Progress'
  end
  object Label1: TLabel
    Left = 0
    Top = 250
    Width = 86
    Height = 20
    Caption = 'Contact Key'
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 573
    Height = 49
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 0
    object btnQuit: TButton
      Left = 480
      Top = 8
      Width = 83
      Height = 33
      Caption = 'Quit'
      TabOrder = 0
      OnClick = btnQuitClick
    end
    object Button4: TButton
      Left = 8
      Top = 8
      Width = 83
      Height = 33
      Caption = 'Help'
      TabOrder = 1
      OnClick = Button4Click
    end
  end
  object gbSource: TGroupBox
    Left = 5
    Top = 64
    Width = 561
    Height = 105
    Caption = 'Source Database'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = 16
    Font.Name = 'MS Shell Dlg'
    Font.Pitch = fpVariable
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object lbSource: TLabel
      Left = 8
      Top = 28
      Width = 16
      Height = 16
      Caption = 'C:\'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'MS Shell Dlg'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 80
      Width = 273
      Height = 16
      Caption = 'Only Bills that occur after... (MM/DD/YY)'
    end
    object btnSourceBrowse: TButton
      Left = 472
      Top = 9
      Width = 83
      Height = 40
      Caption = 'Browse...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = 16
      Font.Name = 'MS Shell Dlg'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnSourceBrowseClick
    end
    object edDateFilter: TEdit
      Left = 296
      Top = 76
      Width = 121
      Height = 24
      TabOrder = 1
      Text = '05/08/07'
    end
    object Button1: TButton
      Left = 472
      Top = 56
      Width = 83
      Height = 41
      Caption = 'Close Tills'
      TabOrder = 2
      OnClick = Button1Click
    end
  end
  object Button3: TButton
    Left = 472
    Top = 276
    Width = 83
    Height = 25
    Caption = 'Join'
    TabOrder = 2
    OnClick = Button3Click
  end
  object ProgBar: TProgressBar
    Left = 0
    Top = 306
    Width = 573
    Height = 18
    Align = alBottom
    TabOrder = 3
  end
  object gbDest: TGroupBox
    Left = 7
    Top = 176
    Width = 561
    Height = 60
    Caption = 'Destination Database'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = 16
    Font.Name = 'MS Shell Dlg'
    Font.Pitch = fpVariable
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    object lbDest: TLabel
      Left = 8
      Top = 28
      Width = 261
      Height = 16
      Caption = 'C:\Program Files\MenuMate\MenuMate.gdb'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'MS Shell Dlg'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
    end
    object btnDestBrowse: TButton
      Left = 472
      Top = 14
      Width = 83
      Height = 38
      Caption = 'Browse...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = 16
      Font.Name = 'MS Shell Dlg'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnDestBrowseClick
    end
  end
  object edContactKey: TEdit
    Left = 96
    Top = 246
    Width = 121
    Height = 28
    TabOrder = 5
    Text = '2'
  end
  object Open: TOpenDialog
    FileName = 'MenuMate.gdb'
    Filter = '*.gdb'
    Options = [ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open'
    Left = 112
    Top = 8
  end
end
