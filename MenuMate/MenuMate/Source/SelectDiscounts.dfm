object frmSelectDiscounts: TfrmSelectDiscounts
  Tag = 1024
  Left = 98
  Top = -8
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Discounts'
  ClientHeight = 736
  ClientWidth = 842
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 842
    Height = 736
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Panel10: TPanel
      Left = 5
      Top = 5
      Width = 185
      Height = 726
      Align = alLeft
      BevelOuter = bvNone
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
      object Label4: TLabel
        Left = 24
        Top = 276
        Width = 131
        Height = 38
        Alignment = taCenter
        Caption = 'Discount Access Denied'
        WordWrap = True
      end
      object pnlItemDiscountGridContainer: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 726
        Align = alClient
        BevelOuter = bvNone
        ParentColor = True
        TabOrder = 0
        object Panel11: TPanel
          Left = 0
          Top = 0
          Width = 185
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Discounts'
          Color = clMaroon
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object tgridItemPrice: TTouchGrid
          Left = 0
          Top = 41
          Width = 185
          Height = 685
          Align = alClient
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          BevelOuter = bvNone
          DefaultButtonColor = -16777201
          DefaultButtonLatchedColor = -16777201
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tgridItemPriceMouseClick
          GridBorder = False
          RowCount = 1
          ColCount = 1
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 120
          DefaultRowHeight = 60
          ButtonGapHeight = 2
          AutoButtonWidth = True
          VertScrollButtonPos = vsAboveAndBelow
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
          ScrollBtnUp.Color = 11587808
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
          ScrollBtnDown.Color = 11587808
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
            040000003C00000000B0F213E2ACFC84FC3FA900000000C8C8C8C8C8C8C8FE3F
            800000000000800080000000050000004100720069000000000D000000010000
            0000000000000000000000000000000000050000004100720069000000000D00
            0000010000000000000000000000000000000000000000000000010000000100
            000001000000010000000100000001000000FF00000064000000010000000000
            0000010000000000000000000000}
        end
      end
    end
    object Panel5: TPanel
      Left = 194
      Top = 5
      Width = 643
      Height = 726
      Align = alRight
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      object Panel15: TPanel
        Left = 0
        Top = 0
        Width = 472
        Height = 445
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 5
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object Label5: TLabel
          Left = 150
          Top = 200
          Width = 166
          Height = 38
          Alignment = taCenter
          Caption = 'Change Price Access Denied'
          WordWrap = True
        end
        object pnlMiscPrice: TPanel
          Left = 5
          Top = 5
          Width = 462
          Height = 435
          Align = alClient
          BevelOuter = bvNone
          ParentColor = True
          TabOrder = 0
          object Panel13: TPanel
            Left = 0
            Top = 366
            Width = 462
            Height = 69
            Align = alBottom
            BevelOuter = bvNone
            BorderWidth = 5
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentBackground = False
            ParentColor = True
            ParentFont = False
            TabOrder = 0
            object Label1: TLabel
              Left = 5
              Top = 5
              Width = 90
              Height = 19
              Align = alTop
              Caption = 'Description'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object tbtnReason: TTouchBtn
              Left = 5
              Top = 24
              Width = 452
              Height = 44
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              ParentColor = True
              Caption = 'Click here to enter a reason'
              ButtonColor = -16777201
              LatchedColor = -16777201
              DisabledButtonColor = -16777201
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = ItemPriceDescriptionClick
            end
          end
          object Panel8: TPanel
            Left = 0
            Top = 0
            Width = 462
            Height = 41
            Align = alTop
            BevelOuter = bvNone
            Caption = 'Change Price'
            Color = clMaroon
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentBackground = False
            ParentFont = False
            TabOrder = 1
          end
          object Panel16: TPanel
            Left = 0
            Top = 41
            Width = 462
            Height = 325
            Align = alClient
            BevelOuter = bvNone
            BorderWidth = 5
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = True
            ParentFont = False
            TabOrder = 2
            object TouchNumpadItemPrice: TTouchNumpad
              Left = 5
              Top = 79
              Width = 452
              Height = 241
              Align = alClient
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              Color = 14342874
              ParentFont = False
              BorderStyle = bsNone
              GridBorderWidth = 0
              ButtonGapWidth = 5
              ButtonGapHeight = 5
              ButtonColor = -16777201
              CustomKey = True
              CustomKeyCaption = '00'
              NumpadDisplay = NumpadDisplayItemPrice
              OnClick = TouchNumpadItemPriceClick
            end
            object NumpadDisplayItemPrice: TNumpadDisplay
              Left = 5
              Top = 5
              Width = 452
              Height = 65
              Align = alTop
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -45
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              ParentColor = True
              BevelInner = bvLowered
              BevelOuter = bvRaised
              BorderWidth = 5
              BorderStyle = bsNone
            end
            object Panel2: TPanel
              Left = 5
              Top = 70
              Width = 452
              Height = 9
              Align = alTop
              BevelOuter = bvNone
              BorderWidth = 20
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentColor = True
              ParentFont = False
              TabOrder = 2
            end
          end
        end
      end
      object Panel3: TPanel
        Left = 477
        Top = 0
        Width = 166
        Height = 445
        Align = alRight
        BevelOuter = bvNone
        ParentColor = True
        TabOrder = 1
        object Panel4: TPanel
          Left = 0
          Top = 0
          Width = 166
          Height = 445
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 5
          ParentBackground = False
          ParentColor = True
          TabOrder = 0
          object btnOk: TTouchBtn
            Left = 5
            Top = 5
            Width = 156
            Height = 55
            Align = alTop
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            Caption = 'Ok'
            ButtonColor = -16777201
            LatchedColor = -16777201
            DisabledButtonColor = -16777201
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnOkMouseClick
          end
          object tbtnItemPriceDiscountClear: TTouchBtn
            Left = 5
            Top = 385
            Width = 156
            Height = 55
            Align = alBottom
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            Color = 14342874
            ParentFont = False
            Caption = 'Clear Discount'
            ButtonColor = 2200566
            LatchedColor = -16777201
            DisabledButtonColor = -16777201
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnItemPriceDiscountClearMouseClick
          end
        end
      end
      object Panel6: TPanel
        Left = 0
        Top = 445
        Width = 643
        Height = 281
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 1
        BorderStyle = bsSingle
        Ctl3D = False
        ParentCtl3D = False
        TabOrder = 2
        object webDisplay: TWebBrowser
          Left = 1
          Top = 1
          Width = 639
          Height = 277
          Align = alClient
          TabOrder = 0
          ExplicitWidth = 300
          ExplicitHeight = 150
          ControlData = {
            4C0000000B420000A11C00000000000000000000000000000000000000000000
            000000004C000000000000000000000001000000E0D057007335CF11AE690800
            2B2E126208000000000000004C0000000114020000000000C000000000000046
            8000000000000000000000000000000000000000000000000000000000000000
            00000000000000000100000000000000000000000000000000000000}
        end
      end
    end
  end
end
