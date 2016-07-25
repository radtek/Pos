object frmEnterKey: TfrmEnterKey
  Left = 78
  Top = 356
  BorderStyle = bsDialog
  Caption = 'Enter Key'
  ClientHeight = 135
  ClientWidth = 520
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label7: TLabel
    Left = 8
    Top = 12
    Width = 68
    Height = 13
    Caption = 'Customer Key:'
  end
  object Bevel1: TBevel
    Left = 8
    Top = 96
    Width = 505
    Height = 9
    Shape = bsTopLine
  end
  object Panel1: TPanel
    Left = 8
    Top = 40
    Width = 505
    Height = 41
    TabOrder = 0
    object Label1: TLabel
      Left = 72
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Label2: TLabel
      Left = 144
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Label3: TLabel
      Left = 216
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Label4: TLabel
      Left = 288
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Label5: TLabel
      Left = 360
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Label6: TLabel
      Left = 432
      Top = 12
      Width = 3
      Height = 13
      Caption = '-'
    end
    object Edit1: TEdit
      Left = 8
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '2CCBFA'
      OnChange = Edit1Change
      OnKeyDown = Edit1KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit2: TEdit
      Left = 80
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '592F18'
      OnChange = Edit1Change
      OnKeyDown = Edit2KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit3: TEdit
      Left = 152
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'DC92D9'
      OnChange = Edit1Change
      OnKeyDown = Edit3KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit4: TEdit
      Left = 224
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Text = '905D46'
      OnChange = Edit1Change
      OnKeyDown = Edit4KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit5: TEdit
      Left = 296
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      Text = '4AEDFB'
      OnChange = Edit1Change
      OnKeyDown = Edit5KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit6: TEdit
      Left = 368
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = '6E2958'
      OnChange = Edit1Change
      OnKeyDown = Edit6KeyDown
      OnKeyPress = Edit1KeyPress
    end
    object Edit7: TEdit
      Left = 440
      Top = 8
      Width = 57
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      Text = '950002'
      OnChange = Edit1Change
      OnKeyDown = Edit7KeyDown
      OnKeyPress = Edit1KeyPress
    end
  end
  object BitBtn1: TBitBtn
    Left = 360
    Top = 104
    Width = 73
    Height = 25
    Caption = 'OK'
    Default = True
    DoubleBuffered = True
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
    ParentDoubleBuffered = False
    TabOrder = 1
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 440
    Top = 104
    Width = 73
    Height = 25
    DoubleBuffered = True
    Kind = bkCancel
    ParentDoubleBuffered = False
    TabOrder = 2
  end
  object edRegKey: TEdit
    Left = 88
    Top = 8
    Width = 425
    Height = 21
    ParentColor = True
    ReadOnly = True
    TabOrder = 3
    Text = 'edRegKey'
  end
  object BitBtn3: TBitBtn
    Left = 8
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Clear'
    DoubleBuffered = True
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00500005000555
      555557777F777555F55500000000555055557777777755F75555005500055055
      555577F5777F57555555005550055555555577FF577F5FF55555500550050055
      5555577FF77577FF555555005050110555555577F757777FF555555505099910
      555555FF75777777FF555005550999910555577F5F77777775F5500505509990
      3055577F75F77777575F55005055090B030555775755777575755555555550B0
      B03055555F555757575755550555550B0B335555755555757555555555555550
      BBB35555F55555575F555550555555550BBB55575555555575F5555555555555
      50BB555555555555575F555555555555550B5555555555555575}
    NumGlyphs = 2
    ParentDoubleBuffered = False
    TabOrder = 4
    OnClick = BitBtn3Click
  end
end
