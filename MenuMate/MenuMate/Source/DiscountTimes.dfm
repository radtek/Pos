object frmDiscountTimes: TfrmDiscountTimes
  Tag = 1024
  Left = 167
  Top = 78
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  ClientHeight = 684
  ClientWidth = 966
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 829
    Top = 0
    Width = 137
    Height = 684
    Align = alRight
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object TouchBtn1: TTouchBtn
      Left = 6
      Top = 6
      Width = 125
      Height = 65
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object btnDelete: TTouchBtn
      Left = 6
      Top = 618
      Width = 125
      Height = 60
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Delete'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDeleteMouseClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 824
    Height = 684
    Align = alClient
    Color = 10011108
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 822
      Height = 512
      Align = alTop
      Caption = 'Add Automatic Discount Period'
      Color = 14342874
      ParentBackground = False
      ParentColor = False
      TabOrder = 0
      object Panel4: TPanel
        Left = 2
        Top = 161
        Width = 561
        Height = 349
        Align = alClient
        Caption = 'Panel2'
        TabOrder = 0
        object tgDay: TTouchGrid
          Left = 1
          Top = 1
          Width = 559
          Height = 347
          Align = alClient
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          DefaultButtonColor = clHighlight
          DefaultButtonLatchedColor = clGreen
          LatchingType = ltRadioButton
          LatchingProperties = [lkLatchColor, lkStayDown]
          RowCount = 5
          ColCount = 7
          DefaultColWidth = 120
          DefaultRowHeight = 55
          AutoButtonWidth = True
          AutoButtonHeight = True
          VertScrollButtonHeight = 55
          HorizScrollButtonWidth = 70
          ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
          ScrollBtnUp.Font.Color = clWindowText
          ScrollBtnUp.Font.Height = -11
          ScrollBtnUp.Font.Name = 'MS Sans Serif'
          ScrollBtnUp.Font.Style = []
          ScrollBtnUp.ParentFont = False
          ScrollBtnUp.ParentFontColor = False
          ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnUp.LatchedFont.Color = clWindowText
          ScrollBtnUp.LatchedFont.Height = -11
          ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnUp.LatchedFont.Style = []
          ScrollBtnUp.ParentLatchedFontColor = False
          ScrollBtnUp.GlyphAlpha = 255
          ScrollBtnUp.Color = -2147483633
          ScrollBtnUp.FontColor = -2147483640
          ScrollBtnUp.LatchedColor = -2147483633
          ScrollBtnUp.DisabledColor = -2147483633
          ScrollBtnUp.LatchedFontColor = -2147483640
          ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
          ScrollBtnDown.Font.Color = clWindowText
          ScrollBtnDown.Font.Height = -11
          ScrollBtnDown.Font.Name = 'MS Sans Serif'
          ScrollBtnDown.Font.Style = []
          ScrollBtnDown.ParentFont = False
          ScrollBtnDown.ParentFontColor = False
          ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnDown.LatchedFont.Color = clWindowText
          ScrollBtnDown.LatchedFont.Height = -11
          ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnDown.LatchedFont.Style = []
          ScrollBtnDown.ParentLatchedFontColor = False
          ScrollBtnDown.GlyphAlpha = 255
          ScrollBtnDown.Color = -2147483633
          ScrollBtnDown.FontColor = -2147483640
          ScrollBtnDown.LatchedColor = -2147483633
          ScrollBtnDown.DisabledColor = -2147483633
          ScrollBtnDown.LatchedFontColor = -2147483640
          ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.Font.Color = clWindowText
          ScrollBtnLeft.Font.Height = -11
          ScrollBtnLeft.Font.Name = 'MS Sans Serif'
          ScrollBtnLeft.Font.Style = []
          ScrollBtnLeft.ParentFont = False
          ScrollBtnLeft.ParentFontColor = False
          ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.LatchedFont.Color = clWindowText
          ScrollBtnLeft.LatchedFont.Height = -11
          ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnLeft.LatchedFont.Style = []
          ScrollBtnLeft.ParentLatchedFontColor = False
          ScrollBtnLeft.GlyphAlpha = 255
          ScrollBtnLeft.Color = -2147483633
          ScrollBtnLeft.FontColor = -2147483640
          ScrollBtnLeft.LatchedColor = -2147483633
          ScrollBtnLeft.DisabledColor = -2147483633
          ScrollBtnLeft.LatchedFontColor = -2147483640
          ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
          ScrollBtnRight.Font.Color = clWindowText
          ScrollBtnRight.Font.Height = -11
          ScrollBtnRight.Font.Name = 'MS Sans Serif'
          ScrollBtnRight.Font.Style = []
          ScrollBtnRight.ParentFont = False
          ScrollBtnRight.ParentFontColor = False
          ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnRight.LatchedFont.Color = clWindowText
          ScrollBtnRight.LatchedFont.Height = -11
          ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnRight.LatchedFont.Style = []
          ScrollBtnRight.ParentLatchedFontColor = False
          ScrollBtnRight.GlyphAlpha = 255
          ScrollBtnRight.Color = -2147483633
          ScrollBtnRight.FontColor = -2147483640
          ScrollBtnRight.LatchedColor = -2147483633
          ScrollBtnRight.DisabledColor = -2147483633
          ScrollBtnRight.LatchedFontColor = -2147483640
          Buttons = {
            040000004300000083BEA02FE80BFA82FC3F4300000067B7F0AB31911EA8FC3F
            4300000067B7F0AB31911EA8FC3F4300000067B7F0AB31911EA8FC3F43000000
            67B7F0AB31911EA8FC3F4E000000B56A343013BDA4E0FC3F4E000000B56A3430
            13BDA4E0FC3F4E000000B56A343013BDA4E0FC3F4E000000B56A343013BDA4E0
            FC3F4E000000B56A343013BDA4E0FC3F4E000000B56A343013BDA4E0FC3F4F00
            0000B56A343013BDA4E0FC3F0D0000FF008000000D0000FF0500000041007200
            69080000FF0C00000000000000000000000000000000000000080000FF050000
            004100720069080000FF0C000000000000000000000000000000000000000800
            00FF00000000010000000100000001000000010000000100000001000000FF00
            00006400000001000000000000000100000000000000000000000D0000FF0080
            00000D0000FF050000004100720069080000FF0C000000000000000000000000
            00000000000000080000FF050000004100720069080000FF0C00000000000000
            000000000000000000000000080000FF00000000010000000100000001000000
            010000000100000001000000FF00000064000000010000000000000001000000
            00000000000000000D0000FF008000000D0000FF050000004100720069080000
            FF0C00000000000000000000000000000000000000080000FF05000000410072
            0069080000FF0C00000000000000000000000000000000000000080000FF0000
            0000010000000100000001000000010000000100000001000000FF0000006400
            000001000000000000000100000000000000000000000D0000FF008000000D00
            00FF050000004100720069080000FF0C00000000000000000000000000000000
            000000080000FF050000004100720069080000FF0C0000000000000000000000
            0000000000000000080000FF0000000001000000010000000100000001000000
            0100000001000000FF0000006400000001000000000000000100000000000000
            000000000D0000FF008000000D0000FF050000004100720069080000FF0C0000
            0000000000000000000000000000000000080000FF0500000041007200690800
            00FF0C00000000000000000000000000000000000000080000FF000000000100
            00000100000001000000010000000100000001000000FF000000640000000100
            0000000000000100000000000000000000000D0000FF008000000D0000FF0500
            00004100720069080000FF0C0000000000000000000000000000000000000008
            0000FF050000004100720069080000FF0C000000000000000000000000000000
            00000000080000FF000000000100000001000000010000000100000001000000
            01000000FF000000640000000100000000000000010000000000000000000000
            0D0000FF008000000D0000FF050000004100720069080000FF0C000000000000
            00000000000000000000000000080000FF050000004100720069080000FF0C00
            000000000000000000000000000000000000080000FF00000000010000000100
            000001000000010000000100000001000000FF00000064000000010000000000
            00000100000000000000000000000D0000FF008000000D0000FF050000004100
            720069080000FF0C00000000000000000000000000000000000000080000FF05
            0000004100720069080000FF0C00000000000000000000000000000000000000
            080000FF00000000010000000100000001000000010000000100000001000000
            FF0000006400000001000000000000000100000000000000000000000D0000FF
            008000000D0000FF050000004100720069080000FF0C00000000000000000000
            000000000000000000080000FF050000004100720069080000FF0C0000000000
            0000000000000000000000000000080000FF0000000001000000010000000100
            0000010000000100000001000000FF0000006400000001000000000000000100
            000000000000000000000D0000FF008000000D0000FF05000000410072006908
            0000FF0C00000000000000000000000000000000000000080000FF0500000041
            00720069080000FF0C00000000000000000000000000000000000000080000FF
            00000000010000000100000001000000010000000100000001000000FF000000
            6400000001000000000000000100000000000000000000000D0000FF00800000
            0D0000FF050000004100720069080000FF0C0000000000000000000000000000
            0000000000080000FF050000004100720069080000FF0C000000000000000000
            00000000000000000000080000FF000000000100000001000000010000000100
            00000100000001000000FF000000640000000100000000000000010000000000
            0000000000000D0000FF008000000D0000FF050000004100720069080000FF0C
            00000000000000000000000000000000000000080000FF050000004100720069
            080000FF0C00000000000000000000000000000000000000080000FF00000000
            010000000100000001000000010000000100000001000000FF00000064000000
            01000000000000000100000000000000000000000D0000FF008000000D0000FF
            050000004100720069080000FF0C000000000000000000000000000000000000
            00080000FF050000004100720069080000FF0C00000000000000000000000000
            000000000000080000FF00000000010000000100000001000000010000000100
            000001000000FF00000064000000010000000000000001000000000000000000
            00000D0000FF008000000D0000FF050000004100720069080000FF0C00000000
            000000000000000000000000000000080000FF050000004100720069080000FF
            0C00000000000000000000000000000000000000080000FF0000000001000000
            0100000001000000010000000100000001000000FF0000006400000001000000
            000000000100000000000000000000000D0000FF008000000D0000FF05000000
            4100720069080000FF0C00000000000000000000000000000000000000080000
            FF050000004100720069080000FF0C0000000000000000000000000000000000
            0000080000FF0000000001000000010000000100000001000000010000000100
            0000FF0000006400000001000000000000000100000000000000000000000D00
            00FF008000000D0000FF050000004100720069080000FF0C0000000000000000
            0000000000000000000000080000FF050000004100720069080000FF0C000000
            00000000000000000000000000000000080000FF000000000100000001000000
            01000000010000000100000001000000FF000000640000000100000000000000
            0100000000000000000000000D0000FF008000000D0000FF0500000041007200
            69080000FF0C00000000000000000000000000000000000000080000FF050000
            004100720069080000FF0C000000000000000000000000000000000000000800
            00FF00000000010000000100000001000000010000000100000001000000FF00
            00006400000001000000000000000100000000000000000000000D0000FF0080
            00000D0000FF050000004100720069080000FF0C000000000000000000000000
            00000000000000080000FF050000004100720069080000FF0C00000000000000
            000000000000000000000000080000FF00000000010000000100000001000000
            010000000100000001000000FF00000064000000010000000000000001000000
            00000000000000000D0000FF008000000D0000FF050000004100720069080000
            FF0C00000000000000000000000000000000000000080000FF05000000410072
            0069080000FF0C00000000000000000000000000000000000000080000FF0000
            0000010000000100000001000000010000000100000001000000FF0000006400
            000001000000000000000100000000000000000000000D0000FF008000000D00
            00FF050000004100720069080000FF0C00000000000000000000000000000000
            000000080000FF050000004100720069080000FF0C0000000000000000000000
            0000000000000000080000FF0000000001000000010000000100000001000000
            0100000001000000FF0000006400000001000000000000000100000000000000
            000000000D0000FF008000000D0000FF050000004100720069080000FF0C0000
            0000000000000000000000000000000000080000FF0500000041007200690800
            00FF0C00000000000000000000000000000000000000080000FF000000000100
            00000100000001000000010000000100000001000000FF000000640000000100
            0000000000000100000000000000000000000D0000FF008000000D0000FF0500
            00004100720069080000FF0C0000000000000000000000000000000000000008
            0000FF050000004100720069080000FF0C000000000000000000000000000000
            00000000080000FF000000000100000001000000010000000100000001000000
            01000000FF000000640000000100000000000000010000000000000000000000
            0D0000FF008000000D0000FF050000004100720069080000FF0C000000000000
            00000000000000000000000000080000FF050000004100720069080000FF0C00
            000000000000000000000000000000000000080000FF00000000010000000100
            000001000000010000000100000001000000FF00000064000000010000000000
            00000100000000000000000000000D0000FF008000000D0000FF050000004100
            720069080000FF0C00000000000000000000000000000000000000080000FF05
            0000004100720069080000FF0C00000000000000000000000000000000000000
            080000FF00000000010000000100000001000000010000000100000001000000
            FF0000006400000001000000000000000100000000000000000000000D0000FF
            008000000D0000FF050000004100720069080000FF0C00000000000000000000
            000000000000000000080000FF050000004100720069080000FF0C0000000000
            0000000000000000000000000000080000FF0000000001000000010000000100
            0000010000000100000001000000FF0000006400000001000000000000000100
            000000000000000000000D0000FF008000000D0000FF05000000410072006908
            0000FF0C00000000000000000000000000000000000000080000FF0500000041
            00720069080000FF0C00000000000000000000000000000000000000080000FF
            00000000010000000100000001000000010000000100000001000000FF000000
            6400000001000000000000000100000000000000000000000D0000FF00800000
            0D0000FF050000004100720069080000FF0C0000000000000000000000000000
            0000000000080000FF050000004100720069080000FF0C000000000000000000
            00000000000000000000080000FF000000000100000001000000010000000100
            00000100000001000000FF000000640000000100000000000000010000000000
            0000000000000D0000FF008000000D0000FF050000004100720069080000FF0C
            00000000000000000000000000000000000000080000FF050000004100720069
            080000FF0C00000000000000000000000000000000000000080000FF00000000
            010000000100000001000000010000000100000001000000FF00000064000000
            01000000000000000100000000000000000000000D0000FF008000000D0000FF
            050000004100720069080000FF0C000000000000000000000000000000000000
            00080000FF050000004100720069080000FF0C00000000000000000000000000
            000000000000080000FF00000000010000000100000001000000010000000100
            000001000000FF00000064000000010000000000000001000000000000000000
            00000D0000FF008000000D0000FF050000004100720069080000FF0C00000000
            000000000000000000000000000000080000FF050000004100720069080000FF
            0C00000000000000000000000000000000000000080000FF0000000001000000
            0100000001000000010000000100000001000000FF0000006400000001000000
            000000000100000000000000000000000D0000FF008000000D0000FF05000000
            4100720069080000FF0C00000000000000000000000000000000000000080000
            FF050000004100720069080000FF0C0000000000000000000000000000000000
            0000080000FF0000000001000000010000000100000001000000010000000100
            0000FF0000006400000001000000000000000100000000000000000000000D00
            00FF008000000D0000FF050000004100720069080000FF0C0000000000000000
            0000000000000000000000080000FF050000004100720069080000FF0C000000
            00000000000000000000000000000000080000FF000000000100000001000000
            01000000010000000100000001000000FF000000640000000100000000000000
            0100000000000000000000000D0000FF008000000D0000FF0500000041007200
            69080000FF0C00000000000000000000000000000000000000080000FF050000
            004100720069080000FF0C000000000000000000000000000000000000000800
            00FF00000000010000000100000001000000010000000100000001000000FF00
            00006400000001000000000000000100000000000000000000000D0000FF0080
            00000D0000FF050000004100720069080000FF0C000000000000000000000000
            00000000000000080000FF050000004100720069080000FF0C00000000000000
            000000000000000000000000080000FF00000000010000000100000001000000
            010000000100000001000000FF00000064000000010000000000000001000000
            00000000000000000D0000FF008000000D0000FF050000004100720069080000
            FF0C00000000000000000000000000000000000000080000FF05000000410072
            0069080000FF0C00000000000000000000000000000000000000080000FF0000
            0000010000000100000001000000010000000100000001000000FF0000006400
            00000100000000000000010000000000000000000000}
        end
      end
      object Panel5: TPanel
        Left = 563
        Top = 161
        Width = 257
        Height = 349
        Align = alRight
        BorderWidth = 5
        Color = 10011108
        TabOrder = 1
        object TouchGridButtons: TTouchGrid
          Left = 6
          Top = 6
          Width = 245
          Height = 194
          Align = alClient
          Color = 14342874
          BevelOuter = bvNone
          DefaultButtonColor = clBtnFace
          DefaultButtonLatchedColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = TouchGridButtonsMouseClick
          RowCount = 3
          ColCount = 1
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 120
          DefaultRowHeight = 55
          ButtonGapWidth = 5
          ButtonGapHeight = 10
          AutoButtonWidth = True
          AutoButtonHeight = True
          VertScrollButtonHeight = 55
          HorizScrollButtonWidth = 70
          ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
          ScrollBtnUp.Font.Color = clWindowText
          ScrollBtnUp.Font.Height = -11
          ScrollBtnUp.Font.Name = 'MS Sans Serif'
          ScrollBtnUp.Font.Style = []
          ScrollBtnUp.ParentFont = False
          ScrollBtnUp.ParentFontColor = False
          ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnUp.LatchedFont.Color = clWindowText
          ScrollBtnUp.LatchedFont.Height = -11
          ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnUp.LatchedFont.Style = []
          ScrollBtnUp.ParentLatchedFontColor = False
          ScrollBtnUp.GlyphAlpha = 255
          ScrollBtnUp.Color = -2147483633
          ScrollBtnUp.FontColor = -2147483640
          ScrollBtnUp.LatchedColor = -2147483633
          ScrollBtnUp.DisabledColor = -2147483633
          ScrollBtnUp.LatchedFontColor = -2147483640
          ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
          ScrollBtnDown.Font.Color = clWindowText
          ScrollBtnDown.Font.Height = -11
          ScrollBtnDown.Font.Name = 'MS Sans Serif'
          ScrollBtnDown.Font.Style = []
          ScrollBtnDown.ParentFont = False
          ScrollBtnDown.ParentFontColor = False
          ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnDown.LatchedFont.Color = clWindowText
          ScrollBtnDown.LatchedFont.Height = -11
          ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnDown.LatchedFont.Style = []
          ScrollBtnDown.ParentLatchedFontColor = False
          ScrollBtnDown.GlyphAlpha = 255
          ScrollBtnDown.Color = -2147483633
          ScrollBtnDown.FontColor = -2147483640
          ScrollBtnDown.LatchedColor = -2147483633
          ScrollBtnDown.DisabledColor = -2147483633
          ScrollBtnDown.LatchedFontColor = -2147483640
          ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.Font.Color = clWindowText
          ScrollBtnLeft.Font.Height = -11
          ScrollBtnLeft.Font.Name = 'MS Sans Serif'
          ScrollBtnLeft.Font.Style = []
          ScrollBtnLeft.ParentFont = False
          ScrollBtnLeft.ParentFontColor = False
          ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.LatchedFont.Color = clWindowText
          ScrollBtnLeft.LatchedFont.Height = -11
          ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnLeft.LatchedFont.Style = []
          ScrollBtnLeft.ParentLatchedFontColor = False
          ScrollBtnLeft.GlyphAlpha = 255
          ScrollBtnLeft.Color = -2147483633
          ScrollBtnLeft.FontColor = -2147483640
          ScrollBtnLeft.LatchedColor = -2147483633
          ScrollBtnLeft.DisabledColor = -2147483633
          ScrollBtnLeft.LatchedFontColor = -2147483640
          ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
          ScrollBtnRight.Font.Color = clWindowText
          ScrollBtnRight.Font.Height = -11
          ScrollBtnRight.Font.Name = 'MS Sans Serif'
          ScrollBtnRight.Font.Style = []
          ScrollBtnRight.ParentFont = False
          ScrollBtnRight.ParentFontColor = False
          ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnRight.LatchedFont.Color = clWindowText
          ScrollBtnRight.LatchedFont.Height = -11
          ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnRight.LatchedFont.Style = []
          ScrollBtnRight.ParentLatchedFontColor = False
          ScrollBtnRight.GlyphAlpha = 255
          ScrollBtnRight.Color = -2147483633
          ScrollBtnRight.FontColor = -2147483640
          ScrollBtnRight.LatchedColor = -2147483633
          ScrollBtnRight.DisabledColor = -2147483633
          ScrollBtnRight.LatchedFontColor = -2147483640
          Buttons = {
            0400000037000000000000000000000000003700000000000000000000000000
            3800000000000000000000000000ED00000000000000000000000000595A5C00
            0F0000800F000080050000004100720069080000FF0D00000001000000000000
            000000000000000000FFFFFF00050000004100720069080000FF0D0000000100
            0000000000000000000000000000080000FF1100000031292053657420537461
            72742054696D65010000000100000001000000000000000100000001000000FF
            000000640000000100000000000000010000000000000000000000595A5C000F
            0000800F000080050000004100720069080000FF0D0000000100000000000000
            0000000000000000FFFFFF00050000004100720069080000FF0D000000010000
            00000000000000000000000000080000FF0F00000032292053657420456E6420
            54696D65010000000100000001000000000000000100000001000000FF000000
            640000000100000000000000010000000000000000000000595A5C000F000080
            0F000080050000004100720069FFFFFF000D0000000100000000000000000000
            0000000000FFFFFF00050000004100720069FFFFFF000D000000010000000000
            00000000000000000000080000FF0F00000033292041646420446973636F756E
            74010000000100000000000000000000000000000001000000FF000000640000
            000100000000000000010000000000000000000000}
        end
        object TouchTime: TTouchTime
          Left = 6
          Top = 200
          Width = 245
          Height = 143
          Align = alBottom
          BorderWidth = 4
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -35
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          Color = 14342874
          IndicatorDelay = 100
          BevelOuter = bvNone
          GridBorderWidth = 0
          ButtonGapWidth = 4
          ButtonGapHeight = 4
          ScrollHourUp.Font.Charset = DEFAULT_CHARSET
          ScrollHourUp.Font.Color = clWindowText
          ScrollHourUp.Font.Height = -11
          ScrollHourUp.Font.Name = 'MS Sans Serif'
          ScrollHourUp.Font.Style = []
          ScrollHourUp.ParentFontColor = False
          ScrollHourUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollHourUp.LatchedFont.Color = clWindowText
          ScrollHourUp.LatchedFont.Height = -11
          ScrollHourUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollHourUp.LatchedFont.Style = []
          ScrollHourUp.ParentLatchedFontColor = False
          ScrollHourUp.GlyphAlpha = 255
          ScrollHourUp.Color = -2147483633
          ScrollHourUp.FontColor = -2147483640
          ScrollHourUp.LatchedColor = -2147483633
          ScrollHourUp.DisabledColor = -2147483633
          ScrollHourUp.LatchedFontColor = -2147483640
          ScrollHourDown.Font.Charset = DEFAULT_CHARSET
          ScrollHourDown.Font.Color = clWindowText
          ScrollHourDown.Font.Height = -11
          ScrollHourDown.Font.Name = 'MS Sans Serif'
          ScrollHourDown.Font.Style = []
          ScrollHourDown.ParentFontColor = False
          ScrollHourDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollHourDown.LatchedFont.Color = clWindowText
          ScrollHourDown.LatchedFont.Height = -11
          ScrollHourDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollHourDown.LatchedFont.Style = []
          ScrollHourDown.ParentLatchedFontColor = False
          ScrollHourDown.GlyphAlpha = 255
          ScrollHourDown.Color = -2147483633
          ScrollHourDown.FontColor = -2147483640
          ScrollHourDown.LatchedColor = -2147483633
          ScrollHourDown.DisabledColor = -2147483633
          ScrollHourDown.LatchedFontColor = -2147483640
          ScrollMinuteUp.Font.Charset = DEFAULT_CHARSET
          ScrollMinuteUp.Font.Color = clWindowText
          ScrollMinuteUp.Font.Height = -11
          ScrollMinuteUp.Font.Name = 'MS Sans Serif'
          ScrollMinuteUp.Font.Style = []
          ScrollMinuteUp.ParentFontColor = False
          ScrollMinuteUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollMinuteUp.LatchedFont.Color = clWindowText
          ScrollMinuteUp.LatchedFont.Height = -11
          ScrollMinuteUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollMinuteUp.LatchedFont.Style = []
          ScrollMinuteUp.ParentLatchedFontColor = False
          ScrollMinuteUp.GlyphAlpha = 255
          ScrollMinuteUp.Color = -2147483633
          ScrollMinuteUp.FontColor = -2147483640
          ScrollMinuteUp.LatchedColor = -2147483633
          ScrollMinuteUp.DisabledColor = -2147483633
          ScrollMinuteUp.LatchedFontColor = -2147483640
          ScrollMinuteDown.Font.Charset = DEFAULT_CHARSET
          ScrollMinuteDown.Font.Color = clWindowText
          ScrollMinuteDown.Font.Height = -11
          ScrollMinuteDown.Font.Name = 'MS Sans Serif'
          ScrollMinuteDown.Font.Style = []
          ScrollMinuteDown.ParentFontColor = False
          ScrollMinuteDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollMinuteDown.LatchedFont.Color = clWindowText
          ScrollMinuteDown.LatchedFont.Height = -11
          ScrollMinuteDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollMinuteDown.LatchedFont.Style = []
          ScrollMinuteDown.ParentLatchedFontColor = False
          ScrollMinuteDown.GlyphAlpha = 255
          ScrollMinuteDown.Color = -2147483633
          ScrollMinuteDown.FontColor = -2147483640
          ScrollMinuteDown.LatchedColor = -2147483633
          ScrollMinuteDown.DisabledColor = -2147483633
          ScrollMinuteDown.LatchedFontColor = -2147483640
          InitialDelay = 300
          RepeatDelay = 100
          ButtonWidth = 60
          DisplayFormat = 'HH:nn'
          ExplicitLeft = 5
          ExplicitTop = 202
        end
      end
      object Panel7: TPanel
        Left = 2
        Top = 21
        Width = 818
        Height = 140
        Align = alTop
        ParentColor = True
        TabOrder = 2
        object tgYear: TTouchGrid
          Left = 1
          Top = 1
          Width = 816
          Height = 64
          Align = alTop
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          BevelOuter = bvNone
          DefaultButtonColor = clSkyBlue
          DefaultButtonLatchedColor = clGreen
          LatchingType = ltRadioButton
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tgMonthMouseClick
          AutoScale = True
          RowCount = 1
          ColCount = 5
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 120
          DefaultRowHeight = 55
          ButtonGapWidth = 2
          ButtonGapHeight = 2
          AutoButtonWidth = True
          AutoButtonHeight = True
          VertScrollButtonHeight = 55
          HorizScrollButtonWidth = 70
          ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
          ScrollBtnUp.Font.Color = clWindowText
          ScrollBtnUp.Font.Height = -11
          ScrollBtnUp.Font.Name = 'MS Sans Serif'
          ScrollBtnUp.Font.Style = []
          ScrollBtnUp.ParentFont = False
          ScrollBtnUp.ParentFontColor = False
          ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnUp.LatchedFont.Color = clWindowText
          ScrollBtnUp.LatchedFont.Height = -11
          ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnUp.LatchedFont.Style = []
          ScrollBtnUp.ParentLatchedFontColor = False
          ScrollBtnUp.GlyphAlpha = 255
          ScrollBtnUp.Color = -2147483633
          ScrollBtnUp.FontColor = -2147483640
          ScrollBtnUp.LatchedColor = -2147483633
          ScrollBtnUp.DisabledColor = -2147483633
          ScrollBtnUp.LatchedFontColor = -2147483640
          ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
          ScrollBtnDown.Font.Color = clWindowText
          ScrollBtnDown.Font.Height = -11
          ScrollBtnDown.Font.Name = 'MS Sans Serif'
          ScrollBtnDown.Font.Style = []
          ScrollBtnDown.ParentFont = False
          ScrollBtnDown.ParentFontColor = False
          ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnDown.LatchedFont.Color = clWindowText
          ScrollBtnDown.LatchedFont.Height = -11
          ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnDown.LatchedFont.Style = []
          ScrollBtnDown.ParentLatchedFontColor = False
          ScrollBtnDown.GlyphAlpha = 255
          ScrollBtnDown.Color = -2147483633
          ScrollBtnDown.FontColor = -2147483640
          ScrollBtnDown.LatchedColor = -2147483633
          ScrollBtnDown.DisabledColor = -2147483633
          ScrollBtnDown.LatchedFontColor = -2147483640
          ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.Font.Color = clWindowText
          ScrollBtnLeft.Font.Height = -11
          ScrollBtnLeft.Font.Name = 'MS Sans Serif'
          ScrollBtnLeft.Font.Style = []
          ScrollBtnLeft.ParentFont = False
          ScrollBtnLeft.ParentFontColor = False
          ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.LatchedFont.Color = clWindowText
          ScrollBtnLeft.LatchedFont.Height = -11
          ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnLeft.LatchedFont.Style = []
          ScrollBtnLeft.ParentLatchedFontColor = False
          ScrollBtnLeft.GlyphAlpha = 255
          ScrollBtnLeft.Color = -2147483633
          ScrollBtnLeft.FontColor = -2147483640
          ScrollBtnLeft.LatchedColor = -2147483633
          ScrollBtnLeft.DisabledColor = -2147483633
          ScrollBtnLeft.LatchedFontColor = -2147483640
          ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
          ScrollBtnRight.Font.Color = clWindowText
          ScrollBtnRight.Font.Height = -11
          ScrollBtnRight.Font.Name = 'MS Sans Serif'
          ScrollBtnRight.Font.Style = []
          ScrollBtnRight.ParentFont = False
          ScrollBtnRight.ParentFontColor = False
          ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnRight.LatchedFont.Color = clWindowText
          ScrollBtnRight.LatchedFont.Height = -11
          ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnRight.LatchedFont.Style = []
          ScrollBtnRight.ParentLatchedFontColor = False
          ScrollBtnRight.GlyphAlpha = 255
          ScrollBtnRight.Color = -2147483633
          ScrollBtnRight.FontColor = -2147483640
          ScrollBtnRight.LatchedColor = -2147483633
          ScrollBtnRight.DisabledColor = -2147483633
          ScrollBtnRight.LatchedFontColor = -2147483640
          Buttons = {
            0400000038000000A3EC7EA3EC7EA3ECFD3FA00000005BD75ECD205E24ABFC3F
            A000000076D422E4AFD9518BFC3FA000000076D422E4AFD9518BFC3FA0000000
            76D422E4AFD9518BFC3FA000000076D422E4AFD9518BFC3FA6CAF0000F000080
            A6CAF000050000004100720069080000800C0000000000000000000000000000
            0000000000080000FF050000004100720069080000800C000000000000000000
            00000000000000000000080000FF000000000100000001000000010000000100
            00000100000001000000FF000000640000000100000000000000010000000000
            000000000000A6CAF0000F000080A6CAF000050000004100720069080000800C
            00000000000000000000000000000000000000080000FF050000004100720069
            080000800C00000000000000000000000000000000000000080000FF00000000
            010000000100000001000000010000000100000001000000FF00000064000000
            0100000000000000010000000000000000000000A6CAF0000F000080A6CAF000
            050000004100720069080000800C000000000000000000000000000000000000
            00080000FF050000004100720069080000800C00000000000000000000000000
            000000000000080000FF00000000010000000100000001000000010000000100
            000001000000FF00000064000000010000000000000001000000000000000000
            0000A6CAF0000F000080A6CAF000050000004100720069080000800C00000000
            000000000000000000000000000000080000FF05000000410072006908000080
            0C00000000000000000000000000000000000000080000FF0000000001000000
            0100000001000000010000000100000001000000FF0000006400000001000000
            00000000010000000000000000000000A6CAF0000F000080A6CAF00005000000
            4100720069080000800C00000000000000000000000000000000000000080000
            FF050000004100720069080000800C0000000000000000000000000000000000
            0000080000FF0000000001000000010000000100000001000000010000000100
            0000FF000000640000000100000000000000010000000000000000000000}
        end
        object tgMonth: TTouchGrid
          Left = 1
          Top = 65
          Width = 816
          Height = 74
          Align = alClient
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          BevelOuter = bvNone
          DefaultButtonColor = clMoneyGreen
          DefaultButtonLatchedColor = clGreen
          LatchingType = ltRadioButton
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tgMonthMouseClick
          RowCount = 1
          ColCount = 12
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 120
          DefaultRowHeight = 55
          ButtonGapWidth = 2
          ButtonGapHeight = 2
          AutoButtonWidth = True
          AutoButtonHeight = True
          VertScrollButtonHeight = 55
          HorizScrollButtonWidth = 70
          ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
          ScrollBtnUp.Font.Color = clWindowText
          ScrollBtnUp.Font.Height = -11
          ScrollBtnUp.Font.Name = 'MS Sans Serif'
          ScrollBtnUp.Font.Style = []
          ScrollBtnUp.ParentFont = False
          ScrollBtnUp.ParentFontColor = False
          ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnUp.LatchedFont.Color = clWindowText
          ScrollBtnUp.LatchedFont.Height = -11
          ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnUp.LatchedFont.Style = []
          ScrollBtnUp.ParentLatchedFontColor = False
          ScrollBtnUp.GlyphAlpha = 255
          ScrollBtnUp.Color = -2147483633
          ScrollBtnUp.FontColor = -2147483640
          ScrollBtnUp.LatchedColor = -2147483633
          ScrollBtnUp.DisabledColor = -2147483633
          ScrollBtnUp.LatchedFontColor = -2147483640
          ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
          ScrollBtnDown.Font.Color = clWindowText
          ScrollBtnDown.Font.Height = -11
          ScrollBtnDown.Font.Name = 'MS Sans Serif'
          ScrollBtnDown.Font.Style = []
          ScrollBtnDown.ParentFont = False
          ScrollBtnDown.ParentFontColor = False
          ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnDown.LatchedFont.Color = clWindowText
          ScrollBtnDown.LatchedFont.Height = -11
          ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnDown.LatchedFont.Style = []
          ScrollBtnDown.ParentLatchedFontColor = False
          ScrollBtnDown.GlyphAlpha = 255
          ScrollBtnDown.Color = -2147483633
          ScrollBtnDown.FontColor = -2147483640
          ScrollBtnDown.LatchedColor = -2147483633
          ScrollBtnDown.DisabledColor = -2147483633
          ScrollBtnDown.LatchedFontColor = -2147483640
          ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.Font.Color = clWindowText
          ScrollBtnLeft.Font.Height = -11
          ScrollBtnLeft.Font.Name = 'MS Sans Serif'
          ScrollBtnLeft.Font.Style = []
          ScrollBtnLeft.ParentFont = False
          ScrollBtnLeft.ParentFontColor = False
          ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnLeft.LatchedFont.Color = clWindowText
          ScrollBtnLeft.LatchedFont.Height = -11
          ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnLeft.LatchedFont.Style = []
          ScrollBtnLeft.ParentLatchedFontColor = False
          ScrollBtnLeft.GlyphAlpha = 255
          ScrollBtnLeft.Color = -2147483633
          ScrollBtnLeft.FontColor = -2147483640
          ScrollBtnLeft.LatchedColor = -2147483633
          ScrollBtnLeft.DisabledColor = -2147483633
          ScrollBtnLeft.LatchedFontColor = -2147483640
          ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
          ScrollBtnRight.Font.Color = clWindowText
          ScrollBtnRight.Font.Height = -11
          ScrollBtnRight.Font.Name = 'MS Sans Serif'
          ScrollBtnRight.Font.Style = []
          ScrollBtnRight.ParentFont = False
          ScrollBtnRight.ParentFontColor = False
          ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
          ScrollBtnRight.LatchedFont.Color = clWindowText
          ScrollBtnRight.LatchedFont.Height = -11
          ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
          ScrollBtnRight.LatchedFont.Style = []
          ScrollBtnRight.ParentLatchedFontColor = False
          ScrollBtnRight.GlyphAlpha = 255
          ScrollBtnRight.Color = -2147483633
          ScrollBtnRight.FontColor = -2147483640
          ScrollBtnRight.LatchedColor = -2147483633
          ScrollBtnRight.DisabledColor = -2147483633
          ScrollBtnRight.LatchedFontColor = -2147483640
          Buttons = {
            0400000042000000B290852C64210B99FE3F4100000076D422E4AFD9518BFC3F
            4100000076D422E4AFD9518BFC3F4100000076D422E4AFD9518BFC3F41000000
            76D422E4AFD9518BFC3F4100000076D422E4AFD9518BFC3F4100000076D422E4
            AFD9518BFC3F420000008EE3388EE3388EE3FB3F420000008EE3388EE3388EE3
            FB3F420000008EE3388EE3388EE3FB3F420000008EE3388EE3388EE3FB3F4200
            00008EE3388EE3388EE3FB3F420000008EE3388EE3388EE3FB3FC0DCC0000F00
            0080C0DCC000050000004100720069080000800C000000000000000000000000
            00000000000000080000FF050000004100720069080000800C00000000000000
            000000000000000000000000080000FF00000000010000000100000001000000
            010000000100000001000000FF00000064000000010000000000000001000000
            0000000000000000C0DCC0000F000080C0DCC000050000004100720069080000
            800C00000000000000000000000000000000000000080000FF05000000410072
            0069080000800C00000000000000000000000000000000000000080000FF0000
            0000010000000100000001000000010000000100000001000000FF0000006400
            00000100000000000000010000000000000000000000C0DCC0000F000080C0DC
            C000050000004100720069080000800C00000000000000000000000000000000
            000000080000FF050000004100720069080000800C0000000000000000000000
            0000000000000000080000FF0000000001000000010000000100000001000000
            0100000001000000FF0000006400000001000000000000000100000000000000
            00000000C0DCC0000F000080C0DCC000050000004100720069080000800C0000
            0000000000000000000000000000000000080000FF0500000041007200690800
            00800C00000000000000000000000000000000000000080000FF000000000100
            00000100000001000000010000000100000001000000FF000000640000000100
            000000000000010000000000000000000000C0DCC0000F000080C0DCC0000500
            00004100720069080000800C0000000000000000000000000000000000000008
            0000FF050000004100720069080000800C000000000000000000000000000000
            00000000080000FF000000000100000001000000010000000100000001000000
            01000000FF000000640000000100000000000000010000000000000000000000
            C0DCC0000F000080C0DCC000050000004100720069080000800C000000000000
            00000000000000000000000000080000FF050000004100720069080000800C00
            000000000000000000000000000000000000080000FF00000000010000000100
            000001000000010000000100000001000000FF00000064000000010000000000
            0000010000000000000000000000C0DCC00000800000C0DCC000050000004100
            720069080000800C00000000000000000000000000000000000000080000FF05
            0000004100720069080000800C00000000000000000000000000000000000000
            080000FF00000000010000000100000001000000010000000100000001000000
            FF000000640000000100000000000000010000000000000000000000C0DCC000
            00800000C0DCC000050000004100720069080000800C00000000000000000000
            000000000000000000080000FF050000004100720069080000800C0000000000
            0000000000000000000000000000080000FF0000000001000000010000000100
            0000010000000100000001000000FF0000006400000001000000000000000100
            00000000000000000000C0DCC00000800000C0DCC00005000000410072006908
            0000800C00000000000000000000000000000000000000080000FF0500000041
            00720069080000800C00000000000000000000000000000000000000080000FF
            00000000010000000100000001000000010000000100000001000000FF000000
            640000000100000000000000010000000000000000000000C0DCC00000800000
            C0DCC000050000004100720069080000800C0000000000000000000000000000
            0000000000080000FF050000004100720069080000800C000000000000000000
            00000000000000000000080000FF000000000100000001000000010000000100
            00000100000001000000FF000000640000000100000000000000010000000000
            000000000000C0DCC00000800000C0DCC000050000004100720069080000800C
            00000000000000000000000000000000000000080000FF050000004100720069
            080000800C00000000000000000000000000000000000000080000FF00000000
            010000000100000001000000010000000100000001000000FF00000064000000
            0100000000000000010000000000000000000000C0DCC00000800000C0DCC000
            050000004100720069080000800C000000000000000000000000000000000000
            00080000FF050000004100720069080000800C00000000000000000000000000
            000000000000080000FF00000000010000000100000001000000010000000100
            000001000000FF00000064000000010000000000000001000000000000000000
            0000}
        end
      end
    end
    object Panel28: TPanel
      Left = 1
      Top = 513
      Width = 822
      Height = 450
      Align = alBottom
      BorderWidth = 6
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      object pnlLabel: TPanel
        Left = 7
        Top = 7
        Width = 808
        Height = 60
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Automatic Discount Times'
        Color = 2200566
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
      end
      object tgridSelection: TTouchGrid
        Left = 7
        Top = 41
        Width = 808
        Height = 122
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clGreen
        LatchingType = ltRadioButton
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgridSelectionMouseClick
        ParentGridColor = False
        GridColor = 14342874
        DefaultColWidth = 120
        DefaultRowHeight = 59
        AutoButtonWidth = True
        VertScrollButtonPos = vsBelow
        VertScrollButtonHeight = 60
        HorizScrollButtonWidth = 70
        ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
        ScrollBtnUp.Font.Color = clWindowText
        ScrollBtnUp.Font.Height = -11
        ScrollBtnUp.Font.Name = 'MS Sans Serif'
        ScrollBtnUp.Font.Style = []
        ScrollBtnUp.ParentFont = False
        ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnUp.LatchedFont.Color = clWindowText
        ScrollBtnUp.LatchedFont.Height = -11
        ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnUp.LatchedFont.Style = []
        ScrollBtnUp.ParentLatchedFontColor = False
        ScrollBtnUp.GlyphAlpha = 255
        ScrollBtnUp.Color = 8421504
        ScrollBtnUp.FontColor = 16777215
        ScrollBtnUp.LatchedColor = -2147483633
        ScrollBtnUp.DisabledColor = -2147483633
        ScrollBtnUp.LatchedFontColor = -2147483640
        ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
        ScrollBtnDown.Font.Color = clWindowText
        ScrollBtnDown.Font.Height = -11
        ScrollBtnDown.Font.Name = 'MS Sans Serif'
        ScrollBtnDown.Font.Style = []
        ScrollBtnDown.ParentFont = False
        ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnDown.LatchedFont.Color = clWindowText
        ScrollBtnDown.LatchedFont.Height = -11
        ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnDown.LatchedFont.Style = []
        ScrollBtnDown.ParentLatchedFontColor = False
        ScrollBtnDown.GlyphAlpha = 255
        ScrollBtnDown.Color = 8421504
        ScrollBtnDown.FontColor = 16777215
        ScrollBtnDown.LatchedColor = -2147483633
        ScrollBtnDown.DisabledColor = -2147483633
        ScrollBtnDown.LatchedFontColor = -2147483640
        ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
        ScrollBtnLeft.Font.Color = clWindowText
        ScrollBtnLeft.Font.Height = -11
        ScrollBtnLeft.Font.Name = 'MS Sans Serif'
        ScrollBtnLeft.Font.Style = []
        ScrollBtnLeft.ParentFont = False
        ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnLeft.LatchedFont.Color = clWindowText
        ScrollBtnLeft.LatchedFont.Height = -11
        ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnLeft.LatchedFont.Style = []
        ScrollBtnLeft.ParentLatchedFontColor = False
        ScrollBtnLeft.GlyphAlpha = 255
        ScrollBtnLeft.Color = -2147483633
        ScrollBtnLeft.FontColor = 16777215
        ScrollBtnLeft.LatchedColor = -2147483633
        ScrollBtnLeft.DisabledColor = -2147483633
        ScrollBtnLeft.LatchedFontColor = -2147483640
        ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
        ScrollBtnRight.Font.Color = clWindowText
        ScrollBtnRight.Font.Height = -11
        ScrollBtnRight.Font.Name = 'MS Sans Serif'
        ScrollBtnRight.Font.Style = []
        ScrollBtnRight.ParentFont = False
        ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnRight.LatchedFont.Color = clWindowText
        ScrollBtnRight.LatchedFont.Height = -11
        ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnRight.LatchedFont.Style = []
        ScrollBtnRight.ParentLatchedFontColor = False
        ScrollBtnRight.GlyphAlpha = 255
        ScrollBtnRight.Color = -2147483633
        ScrollBtnRight.FontColor = 16777215
        ScrollBtnRight.LatchedColor = -2147483633
        ScrollBtnRight.DisabledColor = -2147483633
        ScrollBtnRight.LatchedFontColor = -2147483640
        VertScrollDistance = 31
        Buttons = {04000000}
      end
    end
  end
  object Panel3: TPanel
    Left = 824
    Top = 0
    Width = 5
    Height = 684
    Align = alRight
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 2
  end
end
