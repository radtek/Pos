object frmBillGroup: TfrmBillGroup
  Tag = 1024
  Left = 131
  Top = 92
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'MenuMate - Bill'
  ClientHeight = 746
  ClientWidth = 1024
  Color = clBlack
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  PixelsPerInch = 96
  TextHeight = 23
  object pnlButtons: TPanel
    Left = 0
    Top = 0
    Width = 1024
    Height = 33
    Align = alTop
    BorderWidth = 3
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object lbePartyName: TStaticText
      Left = 4
      Top = 4
      Width = 1016
      Height = 25
      Align = alClient
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Party Name:'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      Transparent = False
    end
  end
  object Panel3: TPanel
    Left = 880
    Top = 33
    Width = 144
    Height = 713
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 3
    Caption = 'Panel3'
    Color = clBlack
    ParentBackground = False
    TabOrder = 1
    object Panel6: TPanel
      Left = 3
      Top = 3
      Width = 138
      Height = 707
      Align = alClient
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object tbtnReprintReceipts: TTouchBtn
        Left = 0
        Top = 519
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Print Prelim Receipt'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnReprintReceiptsMouseClick
      end
      object btnBillTable: TTouchBtn
        Left = 0
        Top = 65
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Bill Entire Table'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnBillTableMouseClick
      end
      object btnBillSelected: TTouchBtn
        Left = 0
        Top = 130
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Bill Selected'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnBillSelectedMouseClick
      end
      object btnPartialPayment: TTouchBtn
        Left = 0
        Top = 195
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Partial Payment'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnPartialPaymentMouseClick
      end
      object btnSplitPayment: TTouchBtn
        Left = 0
        Top = 260
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Split Payment'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnSplitPaymentMouseClick
      end
      object btnTransfer: TTouchBtn
        Left = 0
        Top = 325
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Transfer'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnTransferMouseClick
      end
      object btnApplyMembership: TTouchBtn
        Left = 0
        Top = 390
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Membership'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnApplyMembershipMouseClick
      end
      object btnClose: TTouchBtn
        Left = 0
        Top = 0
        Width = 138
        Height = 65
        Align = alTop
        BorderWidth = 3
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
        OnMouseClick = btnCloseMouseClick
      end
      object tbtnDiscount: TTouchBtn
        Left = 0
        Top = 455
        Width = 138
        Height = 64
        Align = alTop
        BorderWidth = 3
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Discount'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnDiscountMouseClick
      end
    end
  end
  object Panel7: TPanel
    Left = 551
    Top = 33
    Width = 329
    Height = 713
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 3
    Caption = 'Panel7'
    Color = clBlack
    ParentBackground = False
    TabOrder = 2
    object Panel9: TPanel
      Left = 3
      Top = 3
      Width = 323
      Height = 707
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 8
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object lbeMembership: TLabel
        Left = 8
        Top = 8
        Width = 4
        Height = 19
        Align = alTop
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        Visible = False
        WordWrap = True
      end
      object Panel2: TPanel
        Left = 8
        Top = 635
        Width = 307
        Height = 64
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 8
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object btnScrollBillUp: TTouchBtn
          Left = 8
          Top = 8
          Width = 97
          Height = 48
          Align = alLeft
          Color = 11587808
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atUp
          OnAutoRepeat = btnScrollBillUpAutoRepeat
        end
        object btnScrollBillDown: TTouchBtn
          Left = 202
          Top = 8
          Width = 97
          Height = 48
          Align = alRight
          Color = 11587808
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atDown
          OnAutoRepeat = btnScrollBillDownAutoRepeat
        end
      end
      object Memo1: TMemo
        Left = 8
        Top = 27
        Width = 307
        Height = 608
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 33
    Width = 551
    Height = 713
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 3
    Caption = 'Panel1'
    Color = clBlack
    ParentBackground = False
    TabOrder = 3
    object Panel4: TPanel
      Left = 3
      Top = 3
      Width = 545
      Height = 707
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
      object pnlgridItemListBoarder: TPanel
        Left = 272
        Top = 72
        Width = 269
        Height = 631
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 4
        BorderStyle = bsSingle
        ParentBackground = False
        ParentColor = True
        TabOrder = 0
        object tgridItemList: TTouchGrid
          Left = 4
          Top = 4
          Width = 259
          Height = 621
          Align = alClient
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          BevelOuter = bvNone
          DefaultButtonColor = clInfoBk
          DefaultButtonLatchedColor = clNavy
          LatchingProperties = [lkIndicator]
          OnMouseDown = tgridItemListMouseDown
          OnMouseUp = tgridItemListMouseUp
          AutoScale = True
          GridBorder = False
          RowCount = 1
          ColCount = 1
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 240
          DefaultRowHeight = 55
          ButtonGapHeight = 5
          AutoButtonWidth = True
          VertScrollButtonPos = vsBelow
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
          Buttons = {
            04000000400000008A213ADC9F9D77F1FB3FF700000059AB884EA9EE62ADFE3F
            000080000080000000008000050000004100720069080000800C000000010000
            00000000000000000000000000080000FF050000004100720069080000800C00
            000001000000000000000000000000000000080000FF00000000010000000100
            000001000000010000000100000001000000FF00000064000000010000000000
            0000010000000000000000000000}
        end
      end
      object pnlgridContainerListBoarder: TPanel
        Left = 4
        Top = 72
        Width = 268
        Height = 631
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 4
        BorderStyle = bsSingle
        ParentColor = True
        TabOrder = 1
        object tgridContainerList: TTouchGrid
          Left = 4
          Top = 4
          Width = 258
          Height = 558
          Align = alClient
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          BevelOuter = bvNone
          DefaultButtonColor = clWhite
          DefaultButtonLatchedColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tgridContainerListMouseClick
          AutoScale = True
          GridBorder = False
          RowCount = 1
          ColCount = 2
          ParentGridColor = False
          GridColor = 14342874
          DefaultColWidth = 120
          DefaultRowHeight = 55
          ButtonGapHeight = 5
          VertScrollButtonPos = vsBelow
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
          Buttons = {
            04000000330000005CF3C32265E0E9D6FB3F4800000089DA95A85D89DA95FD3F
            AD00000053083485405308B4FE3F000080000080000000008000050000004100
            720069080000FF0C00000001000000000000000000000000000000080000FF05
            0000004100720069080000FF0C00000001000000000000000000000000000000
            080000FF00000000010000000100000001000000010000000100000001000000
            FF000000640000000100000000000000010000000000000000000000FFFFFF00
            0F000080FFFFFF00050000004100720069080000FF0C00000001000000000000
            000000000000000000080000FF050000004100720069080000FF0C0000000100
            0000000000000000000000000000080000FF0000000001000000010000000100
            0000010000000100000001000000FF0000006400000001000000000000000100
            00000000000000000000}
        end
        object Panel5: TPanel
          Left = 4
          Top = 562
          Width = 258
          Height = 63
          Align = alBottom
          Anchors = [akLeft, akRight]
          BevelOuter = bvNone
          BorderWidth = 2
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -27
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
          object tbtnSelectAll: TTouchBtn
            Left = 2
            Top = 2
            Width = 113
            Height = 59
            Align = alLeft
            BorderWidth = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            Caption = 'Select All'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSelectAllMouseClick
          end
          object tbtnClearAll: TTouchBtn
            Left = 115
            Top = 2
            Width = 141
            Height = 59
            Align = alClient
            BorderWidth = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            Caption = 'Clear All'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnClearAllMouseClick
          end
        end
      end
      object Panel8: TPanel
        Left = 4
        Top = 4
        Width = 537
        Height = 68
        Align = alTop
        BevelOuter = bvNone
        ParentBackground = False
        ParentColor = True
        TabOrder = 2
        object tbtnCancel: TTouchBtn
          Left = 267
          Top = 0
          Width = 88
          Height = 68
          Align = alLeft
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = 14342874
          ParentFont = False
          Caption = 'Cancel Items'
          ButtonColor = 2200566
          LatchedColor = clMaroon
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnCancelMouseClick
        end
        object tbtnSplit: TTouchBtn
          Left = 443
          Top = 0
          Width = 96
          Height = 68
          Align = alLeft
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Split Items'
          ButtonColor = 6052441
          LatchedColor = clMaroon
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnSplitMouseClick
        end
        object tbtnMove: TTouchBtn
          Left = 355
          Top = 0
          Width = 88
          Height = 68
          Align = alLeft
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Move Items'
          ButtonColor = 6052441
          LatchedColor = clMaroon
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnMoveMouseClick
        end
        object tbtnSelectZone: TTouchBtn
          Left = 0
          Top = 0
          Width = 267
          Height = 68
          Align = alLeft
          BorderWidth = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Select'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnSelectZoneMouseClick
        end
      end
    end
  end
  object IBQuery: TIBQuery
    Left = 536
    Top = 168
  end
  object SplitTimer: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = SplitTimerTick
    Left = 892
    Top = 244
  end
end
