object frmDiscounts: TfrmDiscounts
  Tag = 1024
  Left = 184
  Top = 38
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 5
  Caption = 'Discounts'
  ClientHeight = 730
  ClientWidth = 599
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 459
    Height = 730
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Panel28: TPanel
      Left = 6
      Top = 6
      Width = 447
      Height = 605
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 6
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object pnlLabel: TPanel
        Left = 6
        Top = 6
        Width = 435
        Height = 34
        Align = alTop
        BorderWidth = 1
        Caption = 'Discounts / Surcharges'
        Color = 39157
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
      end
      object tgridSelection: TTouchGrid
        Left = 6
        Top = 40
        Width = 435
        Height = 559
        Align = alClient
        BorderWidth = 1
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Color = 14342874
        ParentFont = False
        BevelOuter = bvNone
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clGreen
        LatchingType = ltRadioButton
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgridSelectionMouseClick
        ParentGridColor = False
        GridColor = 14342874
        DefaultColWidth = 120
        DefaultRowHeight = 55
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
        Buttons = {04000000}
      end
    end
    object Panel3: TPanel
      Left = 6
      Top = 611
      Width = 447
      Height = 113
      Align = alBottom
      Color = 10011108
      TabOrder = 1
      object tgListOrder: TTouchGrid
        Left = 1
        Top = 1
        Width = 225
        Height = 111
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        Color = 14342874
        ParentFont = False
        BevelOuter = bvNone
        DefaultButtonColor = clNavy
        DefaultButtonLatchedColor = clGreen
        LatchingType = ltRadioButton
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgListOrderMouseClick
        RowCount = 2
        ColCount = 1
        DefaultColWidth = 120
        DefaultRowHeight = 55
        ButtonGapWidth = 5
        ButtonGapHeight = 5
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
        ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnUp.LatchedFont.Color = clWindowText
        ScrollBtnUp.LatchedFont.Height = -11
        ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnUp.LatchedFont.Style = []
        ScrollBtnUp.ParentLatchedFontColor = False
        ScrollBtnUp.GlyphAlpha = 255
        ScrollBtnUp.Color = -2147483633
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
        ScrollBtnDown.Color = -2147483633
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
        Buttons = {
          0400000031000000000000000000000000003100000000000000000000000000
          D900000000000000000000000000595A5C000080000000008000060000005400
          61006800FFFFFF000D00000000000000000000000000000000000000FFFFFF00
          06000000540061006800FFFFFF000D0000000000000000000000000000000000
          0000FFFFFF000E0000005072696F72697479204F726465720100000001000000
          01000000010000000100000001000000FF000000640000000100000000000000
          010000000000000000000000595A5C0000800000000080000600000054006100
          6800FFFFFF000D00000000000000000000000000000000000000FFFFFF000600
          0000540061006800FFFFFF000D00000000000000000000000000000000000000
          FFFFFF0010000000417070656172616E6365204F726465720100000001000000
          01000000010000000100000001000000FF000000640000000100000000000000
          010000000100000000000000}
      end
      object tgMove: TTouchGrid
        Left = 226
        Top = 1
        Width = 220
        Height = 111
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        Color = 14342874
        ParentFont = False
        BevelOuter = bvNone
        DefaultButtonColor = 6052441
        DefaultButtonLatchedColor = 6052441
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgMoveMouseClick
        RowCount = 2
        ColCount = 1
        DefaultColWidth = 120
        DefaultRowHeight = 55
        ButtonGapWidth = 5
        ButtonGapHeight = 5
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
        ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
        ScrollBtnUp.LatchedFont.Color = clWindowText
        ScrollBtnUp.LatchedFont.Height = -11
        ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
        ScrollBtnUp.LatchedFont.Style = []
        ScrollBtnUp.ParentLatchedFontColor = False
        ScrollBtnUp.GlyphAlpha = 255
        ScrollBtnUp.Color = -2147483633
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
        ScrollBtnDown.Color = -2147483633
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
        Buttons = {
          0400000031000000000000000000000000003100000000000000000000000000
          D400000000000000000000000000929497000F0000800F000080060000005400
          61006800FFFFFF000D00000000000000000000000000000000000000FFFFFF00
          06000000540061006800FFFFFF000D0000000000000000000000000000000000
          0000FFFFFF0011000000496E637265617365205072696F726974790100000001
          00000001000000010000000100000001000000FF000000640000000100000000
          000000010000000000000000000000929497000F0000800F0000800600000054
          0061006800FFFFFF000D00000000000000000000000000000000000000FFFFFF
          0006000000540061006800FFFFFF000D00000000000000000000000000000000
          000000FFFFFF00110000004465637265617365205072696F7269747901000000
          0100000001000000010000000100000001000000FF0000006400000001000000
          00000000010000000000000000000000}
      end
    end
  end
  object Panel2: TPanel
    Left = 464
    Top = 0
    Width = 135
    Height = 730
    Align = alRight
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tbtnOk: TTouchBtn
      Left = 6
      Top = 8
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = Panel19Click
    end
    object btnAddDiscount: TTouchBtn
      Left = 6
      Top = 112
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAddDiscountClick
    end
    object tbtnEditDiscount: TTouchBtn
      Left = 6
      Top = 186
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Edit'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnEditDiscountClick
    end
    object btnDelete: TTouchBtn
      Left = 6
      Top = 336
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Delete'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDeleteClick
    end
    object tbtnAssignCard: TTouchBtn
      Left = 6
      Top = 472
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Assign Card'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAssignCardClick
    end
    object tbtnClearCard: TTouchBtn
      Left = 6
      Top = 544
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Clear Card'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnClearCardClick
    end
    object TouchBtn1: TTouchBtn
      Left = 6
      Top = 261
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Configure Auto Times'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object tbtnDiscountGroup: TTouchBtn
      Left = 6
      Top = 615
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount Group'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnDiscountGroupMouseClick
    end
  end
  object Panel4: TPanel
    Left = 459
    Top = 0
    Width = 5
    Height = 730
    Align = alRight
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 2
  end
end
