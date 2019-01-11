object frmTaxMaintenance: TfrmTaxMaintenance
  Tag = 1024
  Left = 0
  Top = 4
  Align = alTop
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Tax'
  ClientHeight = 768
  ClientWidth = 1019
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  WindowState = wsMaximized
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 18
  object pnlButtons: TPanel
    Left = 875
    Top = 4
    Width = 125
    Height = 768
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    DesignSize = (
      125
      768)
    object tbtnOk: TTouchBtn
      Left = 4
      Top = 8
      Width = 100
      Anchors = [akTop]
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
      OnMouseClick = tbtnOkMouseClick
    end
  end
  object Pages: TPageControl
    Left = 4
    Top = 4
    Width = 865
    Height = 768
    ActivePage = tsTaxSettings
    TabOrder = 1
    object tsTaxSettings: TTabSheet
      Caption = 'Tax Settings.'
      TabVisible = False
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 857
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object lbPrinterName: TLabel
          Left = 32
          Top = 457
          Width = 161
          Height = 23
          Caption = 'Fiscal Printer Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbLogicalName: TLabel
          Left = 32
          Top = 502
          Width = 175
          Height = 46
          Caption = 'Fiscal Printer Logical Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object cbPriceIncludeTax: TCheckBox
          Left = 32
          Top = 80
          Width = 249
          Height = 25
          Caption = 'Item Prices Include Tax'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          State = cbChecked
          TabOrder = 1
          OnClick = cbPriceIncludeTaxClick
        end
        object cbPriceIncludesServiceCharge: TCheckBox
          Left = 32
          Top = 120
          Width = 377
          Height = 25
          Caption = 'Item Prices Include Service Charge'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          OnClick = cbPriceIncludesServiceChargeClick
        end
        object cbCalculateTaxPreDiscount: TCheckBox
          Left = 32
          Top = 160
          Width = 377
          Height = 25
          Caption = 'Calculate Tax from Pre Discounted Prices'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 3
          OnClick = cbCalculateTaxPreDiscountClick
        end
        object cbCalculateTaxPostDiscount: TCheckBox
          Left = 32
          Top = 200
          Width = 377
          Height = 25
          Caption = 'Recalculate Tax After Applying Discounts'
          Checked = True
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          State = cbChecked
          TabOrder = 4
          OnClick = cbCalculateTaxPostDiscountClick
        end
        object cbCalculateServiceChargePreDiscount: TCheckBox
          Left = 32
          Top = 240
          Width = 497
          Height = 25
          Caption = 'Calculate Service Charge from Pre Discounted Prices'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 5
          OnClick = cbCalculateServiceChargePreDiscountClick
        end
        object cbCalculateServiceChargePostDiscount: TCheckBox
          Left = 32
          Top = 279
          Width = 465
          Height = 25
          Caption = 'Recalculate Service Charge After Applying Discounts'
          Checked = True
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          State = cbChecked
          TabOrder = 6
          OnClick = cbCalculateServiceChargePostDiscountClick
        end
        object cbApplyTaxToServiceCharge: TCheckBox
          Left = 32
          Top = 320
          Width = 265
          Height = 25
          Caption = 'Apply Tax to Service Charge'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 7
          OnClick = cbApplyTaxToServiceChargeClick
        end
        object cbTaxProfile: TComboBox
          Left = 303
          Top = 323
          Width = 400
          Height = 24
          Style = csDropDownList
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          OnChange = cbTaxProfileChange
        end
        object cbUseItalyFIscalPrinter: TCheckBox
          Left = 32
          Top = 410
          Width = 265
          Height = 25
          Caption = 'Use Italy Fiscal Printer'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 11
          OnClick = cbUseItalyFiscalPrinterClick
        end
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 855
          Height = 41
          Align = alTop
          Caption = 'Tax Settings'
          Color = 39157
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object cbApplyTaxToRounding: TCheckBox
          Left = 32
          Top = 365
          Width = 265
          Height = 25
          Caption = 'Apply Tax to Rounding'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 9
          OnClick = cbApplyTaxToRoundingClick
        end
        object cbTaxProfileRounding: TComboBox
          Left = 303
          Top = 368
          Width = 400
          Height = 24
          Style = csDropDownList
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          OnChange = cbRoundingTaxProfileChange
        end
        object cbPostsaletofiscal: TCheckBox
          Left = 32
          Top = 565
          Width = 377
          Height = 25
          Caption = 'Restrict Room Sales To Fiscal'
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 12
          OnClick = cbPostRoomSaleToFiscalClick
        end
      end
    end
  end
  object edPrinterName: TEdit
    Left = 248
    Top = 468
    Width = 169
    Height = 26
    Enabled = False
    TabOrder = 2
    OnClick = edPrinterNameClick
  end
  object edLogicalName: TEdit
    Left = 248
    Top = 521
    Width = 169
    Height = 26
    Enabled = False
    TabOrder = 3
    OnClick = edLogicalNameClick
  end
end
