object frmSelectLoyalty: TfrmSelectLoyalty
  Tag = 1024
  Left = 2
  Top = -2
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'MenuMate'
  ClientHeight = 730
  ClientWidth = 1013
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 23
  object btnExit: TBitBtn
    Left = 888
    Top = 8
    Width = 121
    Height = 89
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    OnClick = btnExitClick
    Kind = bkCancel
  end
  object btnLogin: TBitBtn
    Left = 888
    Top = 112
    Width = 121
    Height = 89
    Caption = 'OK'
    ModalResult = 1
    ParentShowHint = False
    ShowHint = False
    TabOrder = 1
    OnClick = btnLoginClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object Panel2: TPanel
    Left = 584
    Top = 72
    Width = 297
    Height = 657
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clMaroon
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 2
    object btn7: TBitBtn
      Left = 8
      Top = 8
      Width = 89
      Height = 65
      Caption = 'A'
      TabOrder = 0
      OnClick = btn7Click
    end
    object btn8: TBitBtn
      Left = 104
      Top = 8
      Width = 89
      Height = 65
      Caption = 'B'
      TabOrder = 1
      OnClick = btn7Click
    end
    object btn9: TBitBtn
      Left = 200
      Top = 8
      Width = 89
      Height = 65
      Caption = 'C'
      TabOrder = 2
      OnClick = btn7Click
    end
    object btn6: TBitBtn
      Left = 200
      Top = 80
      Width = 89
      Height = 65
      Caption = 'F'
      TabOrder = 3
      OnClick = btn7Click
    end
    object btn5: TBitBtn
      Left = 104
      Top = 80
      Width = 89
      Height = 65
      Caption = 'E'
      TabOrder = 4
      OnClick = btn7Click
    end
    object btn4: TBitBtn
      Left = 8
      Top = 80
      Width = 89
      Height = 65
      Caption = 'D'
      TabOrder = 5
      OnClick = btn7Click
    end
    object btn1: TBitBtn
      Left = 8
      Top = 152
      Width = 89
      Height = 65
      Caption = 'G'
      TabOrder = 6
      OnClick = btn7Click
    end
    object btn2: TBitBtn
      Left = 104
      Top = 152
      Width = 89
      Height = 65
      Caption = 'H'
      TabOrder = 7
      OnClick = btn7Click
    end
    object btn3: TBitBtn
      Left = 200
      Top = 152
      Width = 89
      Height = 65
      Caption = 'I'
      TabOrder = 8
      OnClick = btn7Click
    end
    object btn0: TBitBtn
      Left = 8
      Top = 224
      Width = 89
      Height = 65
      Caption = 'J'
      TabOrder = 9
      OnClick = btn7Click
    end
    object BitBtn1: TBitBtn
      Left = 8
      Top = 296
      Width = 89
      Height = 65
      Caption = 'M'
      TabOrder = 10
      OnClick = btn7Click
    end
    object BitBtn2: TBitBtn
      Left = 8
      Top = 368
      Width = 89
      Height = 65
      Caption = 'P'
      TabOrder = 11
      OnClick = btn7Click
    end
    object BitBtn3: TBitBtn
      Left = 8
      Top = 440
      Width = 89
      Height = 65
      Caption = 'S'
      TabOrder = 12
      OnClick = btn7Click
    end
    object BitBtn4: TBitBtn
      Left = 8
      Top = 512
      Width = 89
      Height = 65
      Caption = 'V'
      TabOrder = 13
      OnClick = btn7Click
    end
    object BitBtn5: TBitBtn
      Left = 8
      Top = 584
      Width = 89
      Height = 65
      Caption = 'Y'
      TabOrder = 14
      OnClick = btn7Click
    end
    object BitBtn6: TBitBtn
      Left = 104
      Top = 224
      Width = 89
      Height = 65
      Caption = 'K'
      TabOrder = 15
      OnClick = btn7Click
    end
    object BitBtn7: TBitBtn
      Left = 104
      Top = 296
      Width = 89
      Height = 65
      Caption = 'N'
      TabOrder = 16
      OnClick = btn7Click
    end
    object BitBtn8: TBitBtn
      Left = 104
      Top = 368
      Width = 89
      Height = 65
      Caption = 'Q'
      TabOrder = 17
      OnClick = btn7Click
    end
    object BitBtn9: TBitBtn
      Left = 104
      Top = 440
      Width = 89
      Height = 65
      Caption = 'T'
      TabOrder = 18
      OnClick = btn7Click
    end
    object BitBtn10: TBitBtn
      Left = 104
      Top = 512
      Width = 89
      Height = 65
      Caption = 'W'
      TabOrder = 19
      OnClick = btn7Click
    end
    object BitBtn11: TBitBtn
      Left = 104
      Top = 584
      Width = 89
      Height = 65
      Caption = 'Z'
      TabOrder = 20
      OnClick = btn7Click
    end
    object BitBtn12: TBitBtn
      Left = 200
      Top = 224
      Width = 89
      Height = 65
      Caption = 'L'
      TabOrder = 21
      OnClick = btn7Click
    end
    object BitBtn13: TBitBtn
      Left = 200
      Top = 296
      Width = 89
      Height = 65
      Caption = 'O'
      TabOrder = 22
      OnClick = btn7Click
    end
    object BitBtn14: TBitBtn
      Left = 200
      Top = 368
      Width = 89
      Height = 65
      Caption = 'R'
      TabOrder = 23
      OnClick = btn7Click
    end
    object BitBtn15: TBitBtn
      Left = 200
      Top = 440
      Width = 89
      Height = 65
      Caption = 'U'
      TabOrder = 24
      OnClick = btn7Click
    end
    object BitBtn16: TBitBtn
      Left = 200
      Top = 512
      Width = 89
      Height = 65
      Caption = 'X'
      TabOrder = 25
      OnClick = btn7Click
    end
    object BitBtn17: TBitBtn
      Left = 200
      Top = 584
      Width = 89
      Height = 65
      Caption = 'Back'
      TabOrder = 26
      OnClick = BitBtn17Click
    end
  end
  object edSearch: TEdit
    Left = 584
    Top = 8
    Width = 297
    Height = 57
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -30
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
  end
  object Panel3: TPanel
    Left = 503
    Top = 8
    Width = 73
    Height = 721
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clGray
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 3
    object Button1: TButton
      Left = 0
      Top = 656
      Width = 72
      Height = 64
      Caption = '\/'
      TabOrder = 0
      OnMouseDown = Button1MouseDown
      OnMouseUp = Button1MouseUp
    end
    object Button2: TButton
      Left = 0
      Top = 0
      Width = 72
      Height = 65
      Caption = '/\'
      TabOrder = 1
      OnMouseDown = Button2MouseDown
      OnMouseUp = Button2MouseUp
    end
  end
  object StringGrid1: TStringGrid
    Left = 8
    Top = 8
    Width = 489
    Height = 721
    ColCount = 1
    DefaultColWidth = 485
    DefaultRowHeight = 42
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goRowSelect]
    ScrollBars = ssNone
    TabOrder = 5
    OnClick = StringGrid1Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 60
    OnTimer = Timer1Timer
    Left = 520
    Top = 81
  end
end
