object frmPaymentType: TfrmPaymentType
  Tag = 1024
  Left = 221
  Top = 113
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'MenuMate'
  ClientHeight = 767
  ClientWidth = 1016
  Color = clBlack
  Ctl3D = False
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 22
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 395
    Height = 767
    Align = alLeft
    BevelOuter = bvNone
    BorderWidth = 5
    Color = clBlack
    TabOrder = 0
    object Panel9: TPanel
      Left = 5
      Top = 5
      Width = 385
      Height = 757
      Align = alClient
      BorderWidth = 8
      Color = 14342874
      Enabled = False
      ParentBackground = False
      TabOrder = 0
      object lbeMembership: TLabel
        Left = 9
        Top = 9
        Width = 367
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
        ExplicitWidth = 4
      end
      object Memo1: TMemo
        Left = 9
        Top = 28
        Width = 367
        Height = 720
        Align = alClient
        Ctl3D = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Courier New'
        Font.Style = []
        Lines.Strings = (
          '12345678901234567890123456789012345678901234567890')
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
        WordWrap = False
      end
    end
  end
  object Panel5: TPanel
    Left = 718
    Top = 0
    Width = 298
    Height = 767
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = clBlack
    TabOrder = 1
    object Panel12: TPanel
      Left = 5
      Top = 94
      Width = 288
      Height = 668
      Align = alClient
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 10011108
      Ctl3D = True
      ParentCtl3D = False
      TabOrder = 0
      object tgPayments: TTouchGrid
        Left = 1
        Top = 42
        Width = 286
        Height = 625
        Align = alClient
        BorderWidth = 0
        Color = 14342874
        DefaultButtonColor = clBtnFace
        DefaultButtonLatchedColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tgPaymentsMouseClick
        RowCount = 1
        ColCount = 2
        ParentGridColor = False
        GridColor = 14342874
        DefaultColWidth = 75
        DefaultRowHeight = 60
        ButtonGapWidth = 5
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
          040000003C000000D408CB3D8DB0DCD3FB3F84000000327BB7166EDA0E83FD3F
          85000000327BB7166EDA0E83FD3F0F0000800F0000800F000080050000004100
          720069080000800E00000000000000000000000000000000000000080000FF05
          0000004100720069080000800E00000000000000000000000000000000000000
          080000FF00000000010000000100000001000000010000000100000001000000
          FF0000006400000001000000000000000100000000000000000000000F000080
          0F0000800F000080050000004100720069080000800E00000000000000000000
          000000000000000000080000FF050000004100720069080000800E0000000000
          0000000000000000000000000000080000FF0000000001000000010000000100
          0000010000000100000001000000FF0000006400000001000000000000000100
          00000000000000000000}
      end
      object Panel25: TPanel
        Left = 1
        Top = 1
        Width = 286
        Height = 41
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Payment Types'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 1
      end
    end
    object Panel7: TPanel
      Left = 5
      Top = 5
      Width = 288
      Height = 89
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      object btnPrelim: TTouchBtn
        Left = 5
        Top = 10
        Width = 138
        Height = 73
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Print'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnPrelimClick
      end
      object btnCancel: TTouchBtn
        Left = 146
        Top = 10
        Width = 138
        Height = 73
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Cancel'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = pnlCancelClick
      end
    end
  end
  object Panel6: TPanel
    Left = 395
    Top = 0
    Width = 323
    Height = 767
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    Color = clBlack
    TabOrder = 2
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 313
      Height = 757
      Align = alClient
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 313
        Height = 145
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvNone
        Color = 14342874
        Ctl3D = False
        ParentBackground = False
        ParentCtl3D = False
        TabOrder = 0
        object Label2: TLabel
          Left = 160
          Top = 72
          Width = 66
          Height = 22
          Caption = 'Change'
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          Transparent = True
        end
        object Label1: TLabel
          Left = 88
          Top = 8
          Width = 126
          Height = 22
          Caption = 'Balance Owing'
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          Transparent = True
        end
        object lbeDiscountSurcharge: TLabel
          Left = 17
          Top = 72
          Width = 95
          Height = 22
          Caption = 'Adjustment'
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          Transparent = True
        end
        object stChange: TStaticText
          Left = 160
          Top = 96
          Width = 129
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = '0.00'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object stBalance: TStaticText
          Left = 16
          Top = 32
          Width = 273
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = '0.00'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object stDiscount: TStaticText
          Left = 16
          Top = 96
          Width = 129
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = '0.00'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 145
        Width = 313
        Height = 435
        Align = alClient
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
        object tnWorkingAmount: TTouchNumpad
          Left = 1
          Top = 73
          Width = 311
          Height = 361
          Align = alClient
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = 14342874
          ParentFont = False
          BorderStyle = bsNone
          GridBorderWidth = 8
          ButtonGapWidth = 2
          ButtonGapHeight = 2
          ButtonColor = clMaroon
          CustomKey = True
          CustomKeyCaption = '00'
          NumpadDisplay = ndWorkingAmount
          OnClick = tnWorkingAmountClick
        end
        object ndWorkingAmount: TNumpadDisplay
          Left = 1
          Top = 1
          Width = 311
          Height = 72
          Align = alTop
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -40
          Font.Name = 'Arial'
          Font.Style = []
          Color = 14342874
          ParentFont = False
          BevelInner = bvLowered
          BorderWidth = 4
          BorderStyle = bsNone
        end
      end
      object Panel8: TPanel
        Left = 0
        Top = 580
        Width = 313
        Height = 177
        Align = alBottom
        BevelInner = bvRaised
        BevelOuter = bvNone
        Color = 14342874
        Ctl3D = False
        ParentBackground = False
        ParentCtl3D = False
        TabOrder = 2
        object tbCredit: TTouchBtn
          Left = 208
          Top = 88
          Width = 100
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Refund'
          ButtonColor = clRed
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbCreditClick
        end
        object tbPatronCount: TTouchBtn
          Left = 88
          Top = 88
          Width = 113
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Patron Count'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbPatronCountClick
        end
        object tbtnDiscountSurcharge: TTouchBtn
          Left = 88
          Top = 8
          Width = 113
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Discount / Surcharge'
          ButtonColor = clPurple
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = lbeDiscountClick
        end
        object tbtnRemoveTaxes: TTouchBtn
          Left = 208
          Top = 8
          Width = 100
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Remove Taxes'
          ButtonColor = clPurple
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = lbeRemoveTaxClick
        end
        object btnReceiptUp: TTouchBtn
          Left = 8
          Top = 8
          Width = 73
          Height = 73
          Color = 11587808
          ButtonColor = 11587808
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atUp
          OnMouseDown = btnReceiptUpMouseDown
          OnMouseUp = btnReceiptUpMouseUp
        end
        object btnReceiptDown: TTouchBtn
          Left = 8
          Top = 88
          Width = 73
          Height = 73
          Color = 11587808
          ButtonColor = 11587808
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atDown
          OnMouseDown = btnReceiptDownMouseDown
          OnMouseUp = btnReceiptDownMouseUp
        end
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 20
    OnTimer = Timer1Timer
    Left = 344
    Top = 584
  end
end
