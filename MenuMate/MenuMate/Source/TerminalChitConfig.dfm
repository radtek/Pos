object frmTerminalChitConfig: TfrmTerminalChitConfig
  Tag = 1024
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 5
  Caption = 'Terminal Chit Config'
  ClientHeight = 730
  ClientWidth = 999
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnl1: TPanel
    Left = 0
    Top = 0
    Width = 849
    Height = 720
    BorderWidth = 5
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object pnl2: TPanel
      Left = 6
      Top = 6
      Width = 378
      Height = 708
      Align = alLeft
      BevelOuter = bvNone
      BorderWidth = 4
      Color = 10011108
      ParentBackground = False
      TabOrder = 0
      object tgridDeviceList: TTouchGrid
        Left = 4
        Top = 38
        Width = 370
        Height = 666
        Align = alClient
        BorderWidth = 2
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clGreen
        LatchingType = ltRadioButton
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgridDeviceListMouseClick
        AutoScale = True
        GridBorder = False
        RowCount = 2
        ColCount = 1
        DefaultColWidth = 120
        DefaultRowHeight = 55
        ButtonGapHeight = 5
        AutoButtonWidth = True
        VertScrollButtonPos = vsBelow
        VertScrollButtonHeight = 80
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
        ScrollBtnUp.Color = 11587808
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
        ScrollBtnDown.Color = 11587808
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
        VertScrollTime = 450
        VertScrollDistance = 400
        Buttons = {
          040000003B000000DB537AB8A66208D9FB3F3B000000DB537AB8A66208D9FB3F
          620100005996655996655996FD3F000080000080000000008000050000004100
          720069080000800C00000001000000000000000000000000000000080000FF05
          0000004100720069080000800C00000001000000000000000000000000000000
          080000FF00000000010000000100000001000000010000000100000001000000
          FF00000064000000010000000000000001000000000000000000000000008000
          0080000000008000050000004100720069080000800C00000001000000000000
          000000000000000000080000FF050000004100720069080000800C0000000100
          0000000000000000000000000000080000FF0000000001000000010000000100
          0000010000000100000001000000FF0000006400000001000000000000000100
          00000000000000000000}
      end
      object pnl3: TPanel
        Left = 4
        Top = 4
        Width = 370
        Height = 34
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Terminal'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 1
      end
    end
    object pnl4: TPanel
      Left = 384
      Top = 6
      Width = 459
      Height = 708
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      Color = 10011108
      TabOrder = 1
      object tgridProfileList: TTouchGrid
        Left = 4
        Top = 38
        Width = 451
        Height = 666
        Align = alClient
        BorderWidth = 2
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clGreen
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor]
        OnMouseClick = tgridProfileListMouseClick
        AutoScale = True
        GridBorder = False
        RowCount = 1
        ColCount = 1
        ParentGridColor = False
        GridColor = 10011108
        DefaultColWidth = 120
        DefaultRowHeight = 55
        ButtonGapHeight = 5
        AutoButtonWidth = True
        VertScrollButtonPos = vsBelow
        VertScrollButtonHeight = 80
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
        ScrollBtnUp.Color = 11587808
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
        ScrollBtnDown.Color = 11587808
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
        VertScrollTime = 450
        VertScrollDistance = 400
        Buttons = {
          04000000350000009AE45FB096F035C3FB3FB301000000000000000000000000
          000080000080000000008000050000004100720069080000800C000000010000
          00000000000000000000000000080000FF050000004100720069080000800C00
          000001000000000000000000000000000000080000FF00000000010000000100
          000001000000010000000100000001000000FF00000064000000010000000000
          0000010000000000000000000000}
      end
      object pnl5: TPanel
        Left = 4
        Top = 4
        Width = 451
        Height = 34
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Chit Number'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 1
      end
    end
  end
  object pnlButtons: TPanel
    Left = 855
    Top = 0
    Width = 134
    Height = 720
    Alignment = taRightJustify
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    object tbtnOk: TTouchBtn
      Left = 7
      Top = 8
      Width = 121
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOkMouseClick
    end
  end
end
