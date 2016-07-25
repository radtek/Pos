object frmSelectGlass: TfrmSelectGlass
  Tag = 1024
  Left = 195
  Top = 114
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Select a Size'
  ClientHeight = 389
  ClientWidth = 654
  Color = clWindow
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PrintScale = poNone
  Scaled = False
  OnClose = FormClose
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 654
    Height = 389
    Align = alClient
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 5
    Caption = 'Panel1'
    Color = clBlack
    Ctl3D = True
    UseDockManager = False
    ParentCtl3D = False
    TabOrder = 0
    object pnlGlasses: TPanel
      Left = 5
      Top = 5
      Width = 508
      Height = 379
      Align = alLeft
      BevelOuter = bvNone
      BorderWidth = 4
      Color = 10011108
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
      object lbeTitle: TStaticText
        Left = 4
        Top = 4
        Width = 500
        Height = 27
        Align = alTop
        Alignment = taCenter
        AutoSize = False
        BorderStyle = sbsSingle
        Caption = 'Select a Size'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
      object tgridItemSizes: TTouchGrid
        Left = 4
        Top = 31
        Width = 500
        Height = 344
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 10011108
        ParentFont = False
        BevelOuter = bvNone
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseDown = tgridItemSizesMouseDown
        GridBorderWidth = 2
        RowCount = 4
        ColCount = 4
        ParentGridColor = False
        GridColor = 10011108
        DefaultColWidth = 110
        DefaultRowHeight = 72
        ButtonGapWidth = 4
        ButtonGapHeight = 4
        AutoButtonWidth = True
        AutoButtonHeight = True
        VertScrollButtonHeight = 55
        HorizScrollButtonWidth = 70
        ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
        ScrollBtnUp.Font.Color = clWindowText
        ScrollBtnUp.Font.Height = -16
        ScrollBtnUp.Font.Name = 'Arial'
        ScrollBtnUp.Font.Style = [fsBold]
        ScrollBtnUp.ParentFont = False
        ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnUp.LatchedFont.Color = clWindowText
        ScrollBtnUp.LatchedFont.Height = -11
        ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnUp.LatchedFont.Style = []
        ScrollBtnUp.ParentLatchedFontColor = False
        ScrollBtnUp.GlyphAlpha = 255
        ScrollBtnUp.Color = 16777215
        ScrollBtnUp.FontColor = 0
        ScrollBtnUp.LatchedColor = -2147483633
        ScrollBtnUp.DisabledColor = -2147483633
        ScrollBtnUp.LatchedFontColor = -2147483640
        ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
        ScrollBtnDown.Font.Color = clWindowText
        ScrollBtnDown.Font.Height = -16
        ScrollBtnDown.Font.Name = 'Arial'
        ScrollBtnDown.Font.Style = [fsBold]
        ScrollBtnDown.ParentFont = False
        ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnDown.LatchedFont.Color = clWindowText
        ScrollBtnDown.LatchedFont.Height = -11
        ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnDown.LatchedFont.Style = []
        ScrollBtnDown.ParentLatchedFontColor = False
        ScrollBtnDown.GlyphAlpha = 255
        ScrollBtnDown.Color = 16777215
        ScrollBtnDown.FontColor = 0
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
        ScrollBtnLeft.Caption = 'Left'
        ScrollBtnLeft.GlyphAlpha = 255
        ScrollBtnLeft.Color = -2147483633
        ScrollBtnLeft.FontColor = 0
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
        ScrollBtnRight.Caption = 'Right'
        ScrollBtnRight.GlyphAlpha = 255
        ScrollBtnRight.Color = -2147483633
        ScrollBtnRight.FontColor = 0
        ScrollBtnRight.LatchedColor = -2147483633
        ScrollBtnRight.DisabledColor = -2147483633
        ScrollBtnRight.LatchedFontColor = -2147483640
        Buttons = {
          0400000052000000B5B4B4B4B4B4B4B4FC3F52000000B5B4B4B4B4B4B4B4FC3F
          52000000B5B4B4B4B4B4B4B4FC3F52000000B5B4B4B4B4B4B4B4FC3F79000000
          C618638C31C618E3FC3F79000000C618638C31C618E3FC3F79000000C618638C
          31C618E3FC3F79000000C618638C31C618E3FC3F000080000F00008000008000
          050000004100720069000000000C000000010000000000000000000000000000
          0000000000050000004100720069000000000C00000001000000000000000000
          0000000000000000000000000000010000000100000001000000010000000100
          000001000000FF00000064000000010000000000000001000000000000000000
          0000000080000F00008000008000050000004100720069000000000C00000001
          0000000000000000000000000000000000000005000000410072006900000000
          0C00000001000000000000000000000000000000000000000000000001000000
          0100000001000000010000000100000001000000FF0000006400000001000000
          00000000010000000000000000000000000080000F0000800000800005000000
          4100720069000000000C00000001000000000000000000000000000000000000
          00050000004100720069000000000C0000000100000000000000000000000000
          0000000000000000000001000000010000000100000001000000010000000100
          0000FF0000006400000001000000000000000100000000000000000000000000
          80000F00008000008000050000004100720069000000000C0000000100000000
          000000000000000000000000000000050000004100720069000000000C000000
          0100000000000000000000000000000000000000000000000100000001000000
          01000000010000000100000001000000FF000000640000000100000000000000
          010000000000000000000000000080000F000080000080000500000041007200
          69000000000C0000000100000000000000000000000000000000000000050000
          004100720069000000000C000000010000000000000000000000000000000000
          000000000000010000000100000001000000010000000100000001000000FF00
          0000640000000100000000000000010000000000000000000000000080000F00
          008000008000050000004100720069000000000C000000010000000000000000
          0000000000000000000000050000004100720069000000000C00000001000000
          0000000000000000000000000000000000000000010000000100000001000000
          010000000100000001000000FF00000064000000010000000000000001000000
          0000000000000000000080000F00008000008000050000004100720069000000
          000C000000010000000000000000000000000000000000000005000000410072
          0069000000000C00000001000000000000000000000000000000000000000000
          0000010000000100000001000000010000000100000001000000FF0000006400
          00000100000000000000010000000000000000000000000080000F0000800000
          8000050000004100720069000000000C00000001000000000000000000000000
          00000000000000050000004100720069000000000C0000000100000000000000
          0000000000000000000000000000000001000000010000000100000001000000
          0100000001000000FF0000006400000001000000000000000100000000000000
          00000000000080000F00008000008000050000004100720069000000000C0000
          0001000000000000000000000000000000000000000500000041007200690000
          00000C0000000100000000000000000000000000000000000000000000000100
          00000100000001000000010000000100000001000000FF000000640000000100
          000000000000010000000000000000000000000080000F000080000080000500
          00004100720069000000000C0000000100000000000000000000000000000000
          000000050000004100720069000000000C000000010000000000000000000000
          0000000000000000000000000100000001000000010000000100000001000000
          01000000FF000000640000000100000000000000010000000000000000000000
          000080000F00008000008000050000004100720069000000000C000000010000
          0000000000000000000000000000000000050000004100720069000000000C00
          0000010000000000000000000000000000000000000000000000010000000100
          000001000000010000000100000001000000FF00000064000000010000000000
          0000010000000000000000000000000080000F00008000008000050000004100
          720069000000000C000000010000000000000000000000000000000000000005
          0000004100720069000000000C00000001000000000000000000000000000000
          0000000000000000010000000100000001000000010000000100000001000000
          FF00000064000000010000000000000001000000000000000000000000008000
          0F00008000008000050000004100720069000000000C00000001000000000000
          00000000000000000000000000050000004100720069000000000C0000000100
          0000000000000000000000000000000000000000000001000000010000000100
          0000010000000100000001000000FF0000006400000001000000000000000100
          00000000000000000000000080000F0000800000800005000000410072006900
          0000000C00000001000000000000000000000000000000000000000500000041
          00720069000000000C0000000100000000000000000000000000000000000000
          00000000010000000100000001000000010000000100000001000000FF000000
          640000000100000000000000010000000000000000000000000080000F000080
          00008000050000004100720069000000000C0000000100000000000000000000
          000000000000000000050000004100720069000000000C000000010000000000
          0000000000000000000000000000000000000100000001000000010000000100
          00000100000001000000FF000000640000000100000000000000010000000000
          000000000000000080000F00008000008000050000004100720069000000000C
          0000000100000000000000000000000000000000000000050000004100720069
          000000000C000000010000000000000000000000000000000000000000000000
          010000000100000001000000010000000100000001000000FF00000064000000
          0100000000000000010000000000000000000000}
      end
    end
    object Panel2: TPanel
      Left = 520
      Top = 5
      Width = 129
      Height = 379
      Align = alRight
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 10011108
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      object TouchBtn1: TTouchBtn
        Left = 8
        Top = 8
        Width = 113
        Height = 73
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Cancel'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = CancelClick
      end
    end
  end
end
