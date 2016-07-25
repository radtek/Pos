object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Menumate Tax Calculator Test App'
  ClientHeight = 722
  ClientWidth = 979
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 5
    Height = 703
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
  end
  object Panel4: TPanel
    Left = 974
    Top = 0
    Width = 5
    Height = 703
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 1114
  end
  object pnMiddle: TPanel
    Left = 5
    Top = 0
    Width = 969
    Height = 703
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 1109
    object pnInput: TPanel
      Left = 0
      Top = 0
      Width = 459
      Height = 703
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 0
      object gbInput: TGroupBox
        Left = 0
        Top = 0
        Width = 459
        Height = 703
        Align = alClient
        Caption = ' Input '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object pnInputTop: TPanel
          Left = 2
          Top = 49
          Width = 455
          Height = 296
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object pnInputPrice: TPanel
            Left = 0
            Top = 0
            Width = 225
            Height = 296
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 0
            object Label1: TLabel
              Left = 7
              Top = 68
              Width = 102
              Height = 13
              Caption = 'Type in the sale price'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clTeal
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object pnInputPriceHeader: TPanel
              Left = 0
              Top = 0
              Width = 225
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'Prince Info'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object LabeledEdit1: TLabeledEdit
              Left = 4
              Top = 40
              Width = 100
              Height = 21
              EditLabel.Width = 28
              EditLabel.Height = 13
              EditLabel.Caption = 'Price'
              TabOrder = 1
            end
            object cbPriceTaxInclusive: TCheckBox
              Left = 7
              Top = 92
              Width = 146
              Height = 17
              Caption = 'Tax Inclusive Price'
              TabOrder = 2
            end
            object CheckBox1: TCheckBox
              Left = 7
              Top = 115
              Width = 170
              Height = 17
              Caption = 'Final price is tax exempt'
              TabOrder = 3
            end
            object rgDiscountMultipleCalcType: TRadioGroup
              Left = 4
              Top = 222
              Width = 106
              Height = 67
              Caption = ' Multi Discount '
              ItemIndex = 1
              Items.Strings = (
                'Simple'
                'Compound')
              TabOrder = 4
            end
            object rgTaxMultipleCalcType: TRadioGroup
              Left = 113
              Top = 222
              Width = 106
              Height = 67
              Caption = ' Multi Tax '
              ItemIndex = 1
              Items.Strings = (
                'Simple'
                'Compound')
              TabOrder = 5
            end
            object cbTaxExclusiveDiscount: TCheckBox
              Left = 7
              Top = 172
              Width = 146
              Height = 17
              Caption = 'Tax Exclusive Discount'
              TabOrder = 6
            end
            object cbTaxBeforeDiscount: TCheckBox
              Left = 7
              Top = 136
              Width = 178
              Height = 17
              Caption = 'Apply tax before discount'
              TabOrder = 7
            end
            object CheckBox2: TCheckBox
              Left = 7
              Top = 192
              Width = 191
              Height = 17
              Caption = 'Tax Exclusive Service Charge'
              TabOrder = 8
            end
            object CheckBox3: TCheckBox
              Left = 7
              Top = 154
              Width = 178
              Height = 17
              Caption = 'Apply tax before discount'
              TabOrder = 9
            end
          end
          object pnInputPriceSep: TPanel
            Left = 225
            Top = 0
            Width = 5
            Height = 296
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 1
          end
          object pnInputSC: TPanel
            Left = 230
            Top = 0
            Width = 225
            Height = 296
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 2
            object Label2: TLabel
              Left = 6
              Top = 68
              Width = 185
              Height = 13
              Caption = 'Type in the service charge percentage'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clTeal
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object Label3: TLabel
              Left = 6
              Top = 142
              Width = 149
              Height = 26
              Caption = 'Type in the service charge tax percentage'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clTeal
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              WordWrap = True
            end
            object Label4: TLabel
              Left = 109
              Top = 45
              Width = 13
              Height = 13
              Caption = '%'
            end
            object Label5: TLabel
              Left = 111
              Top = 120
              Width = 13
              Height = 13
              Caption = '%'
            end
            object pnInputSCHeader: TPanel
              Left = 0
              Top = 0
              Width = 225
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'Service Charge'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object LabeledEdit2: TLabeledEdit
              Left = 4
              Top = 40
              Width = 100
              Height = 21
              EditLabel.Width = 85
              EditLabel.Height = 13
              EditLabel.Caption = 'Service Charge'
              TabOrder = 1
            end
            object LabeledEdit3: TLabeledEdit
              Left = 6
              Top = 115
              Width = 100
              Height = 21
              EditLabel.Width = 109
              EditLabel.Height = 13
              EditLabel.Caption = 'Service Charge Tax'
              TabOrder = 2
            end
          end
        end
        object pnInputSep: TPanel
          Left = 2
          Top = 345
          Width = 455
          Height = 5
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
        end
        object pnInputBottom: TPanel
          Left = 2
          Top = 350
          Width = 455
          Height = 351
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 2
          object pnInputDiscount: TPanel
            Left = 0
            Top = 0
            Width = 225
            Height = 351
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 0
            object pnInputDiscountHeader: TPanel
              Left = 0
              Top = 0
              Width = 225
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'Discount Info'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object pnInputDiscounBottom: TPanel
              Left = 0
              Top = 316
              Width = 225
              Height = 35
              Align = alBottom
              BevelOuter = bvNone
              TabOrder = 1
              object btnEditDiscount: TButton
                Left = 129
                Top = 6
                Width = 93
                Height = 25
                Caption = 'Edit Discount'
                TabOrder = 0
              end
              object btnDelDiscount: TButton
                Left = 4
                Top = 6
                Width = 100
                Height = 25
                Caption = 'Delete Discount'
                TabOrder = 1
              end
            end
            object pnInputDiscountButtons: TPanel
              Left = 0
              Top = 17
              Width = 225
              Height = 34
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 2
              object btnNewDiscount: TButton
                Left = 4
                Top = 3
                Width = 93
                Height = 25
                Caption = 'New Discount'
                TabOrder = 0
              end
              object btnCommitDiscount: TButton
                Left = 103
                Top = 3
                Width = 119
                Height = 25
                Caption = 'Commit Discount'
                TabOrder = 1
              end
            end
            object pnInputDiscountTop: TPanel
              Left = 0
              Top = 51
              Width = 225
              Height = 202
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 3
              object Label6: TLabel
                Left = 7
                Top = 45
                Width = 126
                Height = 13
                Caption = 'Type in the discount name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clTeal
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label7: TLabel
                Left = 6
                Top = 115
                Width = 97
                Height = 13
                Caption = 'Type in the discount'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clTeal
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                WordWrap = True
              end
              object Label11: TLabel
                Left = 142
                Top = 115
                Width = 74
                Height = 26
                Caption = 'Type in a numeric priority'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clTeal
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                WordWrap = True
              end
              object ledDiscountName: TLabeledEdit
                Left = 4
                Top = 17
                Width = 100
                Height = 21
                EditLabel.Width = 32
                EditLabel.Height = 13
                EditLabel.Caption = 'Name'
                TabOrder = 0
              end
              object ledDiscountValue: TLabeledEdit
                Left = 6
                Top = 88
                Width = 73
                Height = 21
                EditLabel.Width = 49
                EditLabel.Height = 13
                EditLabel.Caption = 'Discount'
                TabOrder = 1
              end
              object rbDiscountContent: TRadioButton
                Left = 86
                Top = 84
                Width = 29
                Height = 17
                Caption = '$'
                TabOrder = 2
              end
              object rbDiscountPercentage: TRadioButton
                Left = 86
                Top = 100
                Width = 39
                Height = 17
                Caption = '%'
                TabOrder = 3
              end
              object ledDiscountPriority: TLabeledEdit
                Left = 142
                Top = 88
                Width = 56
                Height = 21
                EditLabel.Width = 42
                EditLabel.Height = 13
                EditLabel.Caption = 'Priority'
                TabOrder = 4
              end
              object rgDiscountType: TRadioGroup
                Left = 4
                Top = 138
                Width = 106
                Height = 58
                ItemIndex = 0
                Items.Strings = (
                  'Discount'
                  'Surcharge')
                TabOrder = 5
              end
            end
            object ListView1: TListView
              Left = 0
              Top = 253
              Width = 225
              Height = 63
              Align = alClient
              BorderStyle = bsNone
              Columns = <>
              TabOrder = 4
            end
          end
          object Panel8: TPanel
            Left = 225
            Top = 0
            Width = 5
            Height = 351
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 1
          end
          object pnInputTax: TPanel
            Left = 230
            Top = 0
            Width = 225
            Height = 351
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 2
            object pnInputTaxHeader: TPanel
              Left = 0
              Top = 0
              Width = 225
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'Tax Info'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel5: TPanel
              Left = 0
              Top = 17
              Width = 225
              Height = 34
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              object btnNewTax: TButton
                Left = 4
                Top = 3
                Width = 93
                Height = 25
                Caption = 'New Tax'
                TabOrder = 0
              end
              object btnCommitTax: TButton
                Left = 103
                Top = 3
                Width = 119
                Height = 25
                Caption = 'Commit Tax'
                TabOrder = 1
              end
            end
            object Panel6: TPanel
              Left = 0
              Top = 51
              Width = 225
              Height = 202
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 2
              object Label8: TLabel
                Left = 7
                Top = 45
                Width = 102
                Height = 13
                Caption = 'Type in the tax name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clTeal
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label9: TLabel
                Left = 6
                Top = 115
                Width = 99
                Height = 26
                Caption = 'Type in the tax rate'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clTeal
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                WordWrap = True
              end
              object Label10: TLabel
                Left = 111
                Top = 92
                Width = 13
                Height = 13
                Caption = '%'
              end
              object ledTaxName: TLabeledEdit
                Left = 4
                Top = 17
                Width = 100
                Height = 21
                EditLabel.Width = 32
                EditLabel.Height = 13
                EditLabel.Caption = 'Name'
                TabOrder = 0
              end
              object ledTaxValue: TLabeledEdit
                Left = 6
                Top = 88
                Width = 100
                Height = 21
                EditLabel.Width = 21
                EditLabel.Height = 13
                EditLabel.Caption = 'Tax'
                TabOrder = 1
              end
              object rbSaleTaxType: TRadioButton
                Left = 128
                Top = 78
                Width = 113
                Height = 17
                Caption = 'Sale Tax'
                Checked = True
                TabOrder = 2
                TabStop = True
              end
              object rbPurchaseTaxType: TRadioButton
                Left = 128
                Top = 94
                Width = 113
                Height = 17
                Caption = 'Purchase Tax'
                TabOrder = 3
              end
              object rbLocalTaxType: TRadioButton
                Left = 128
                Top = 110
                Width = 113
                Height = 17
                Caption = 'Local Tax'
                TabOrder = 4
              end
            end
            object ListView2: TListView
              Left = 0
              Top = 253
              Width = 225
              Height = 63
              Align = alClient
              BorderStyle = bsNone
              Columns = <>
              TabOrder = 3
            end
            object Panel7: TPanel
              Left = 0
              Top = 316
              Width = 225
              Height = 35
              Align = alBottom
              BevelOuter = bvNone
              TabOrder = 4
              object btnEditTax: TButton
                Left = 129
                Top = 6
                Width = 93
                Height = 25
                Caption = 'Edit Tax'
                TabOrder = 0
              end
              object btnDelTax: TButton
                Left = 4
                Top = 6
                Width = 100
                Height = 25
                Caption = 'Delete Tax'
                TabOrder = 1
              end
            end
          end
        end
        object pnInputButtons: TPanel
          Left = 2
          Top = 15
          Width = 455
          Height = 34
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object btnCommitInput: TButton
            Left = 374
            Top = 3
            Width = 75
            Height = 25
            Caption = 'Commit'
            TabOrder = 0
            OnClick = btnCommitInputClick
          end
          object btnNewInput: TButton
            Left = 293
            Top = 3
            Width = 75
            Height = 25
            Caption = 'New'
            TabOrder = 1
          end
        end
      end
    end
    object pnSepMiddle: TPanel
      Left = 459
      Top = 0
      Width = 5
      Height = 703
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 1
    end
    object pnOuput: TPanel
      Left = 464
      Top = 0
      Width = 505
      Height = 703
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      ExplicitWidth = 645
      object gbOutput: TGroupBox
        Left = 0
        Top = 0
        Width = 505
        Height = 703
        Align = alClient
        Caption = ' Output '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        ExplicitWidth = 645
        object sptOutput: TSplitter
          Left = 2
          Top = 353
          Width = 501
          Height = 3
          Cursor = crVSplit
          Align = alTop
          ExplicitTop = 241
          ExplicitWidth = 535
        end
        object pnOutputTop: TPanel
          Left = 2
          Top = 15
          Width = 501
          Height = 338
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          ExplicitWidth = 641
          object pnOutputAllPrices: TPanel
            Left = 0
            Top = 0
            Width = 501
            Height = 338
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 0
            ExplicitWidth = 641
            object Splitter1: TSplitter
              Left = 217
              Top = 53
              Height = 285
              ExplicitLeft = 0
            end
            object Panel9: TPanel
              Left = 0
              Top = 0
              Width = 501
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'All Prices'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
              ExplicitWidth = 641
            end
            object pnOutputAllPricesButtons: TPanel
              Left = 0
              Top = 17
              Width = 501
              Height = 36
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              ExplicitWidth = 641
              object btnDeletePrice: TButton
                Left = 92
                Top = 6
                Width = 75
                Height = 25
                Caption = 'Delete'
                TabOrder = 0
              end
              object btnEditPrice: TButton
                Left = 11
                Top = 6
                Width = 75
                Height = 25
                Caption = 'Edit'
                TabOrder = 1
              end
            end
            object ListView3: TListView
              Left = 0
              Top = 53
              Width = 217
              Height = 285
              Align = alLeft
              BorderStyle = bsNone
              Columns = <>
              TabOrder = 2
            end
            object ListView4: TListView
              Left = 220
              Top = 53
              Width = 281
              Height = 285
              Align = alClient
              BorderStyle = bsNone
              Columns = <>
              TabOrder = 3
              ExplicitLeft = 324
              ExplicitWidth = 317
            end
          end
        end
        object pnOutputBottom: TPanel
          Left = 2
          Top = 356
          Width = 501
          Height = 345
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          ExplicitWidth = 641
          object pnOutputResult: TPanel
            Left = 0
            Top = 0
            Width = 501
            Height = 345
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 0
            ExplicitWidth = 641
            object pnOutputResultHeader: TPanel
              Left = 0
              Top = 0
              Width = 501
              Height = 17
              Align = alTop
              BevelOuter = bvNone
              Caption = 'Result'
              Color = clTeal
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
              ExplicitWidth = 641
            end
            object pnOutputResultButtons: TPanel
              Left = 0
              Top = 17
              Width = 501
              Height = 36
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              ExplicitWidth = 641
              object btnRefresh: TButton
                Left = 92
                Top = 5
                Width = 75
                Height = 25
                Caption = 'Refresh'
                TabOrder = 0
              end
              object btnDetailResult: TButton
                Left = 11
                Top = 5
                Width = 75
                Height = 25
                Caption = 'Details'
                TabOrder = 1
              end
            end
            object pgcOutputResult: TPageControl
              Left = 0
              Top = 53
              Width = 501
              Height = 292
              ActivePage = tbsOutputDiscount
              Align = alClient
              TabOrder = 2
              ExplicitWidth = 641
              object tbsOutputDiscount: TTabSheet
                Caption = 'Discount / Surcharge'
                ExplicitWidth = 633
                object lvOuputDiscounts: TListView
                  Left = 0
                  Top = 0
                  Width = 493
                  Height = 264
                  Align = alClient
                  BorderStyle = bsNone
                  Columns = <>
                  TabOrder = 0
                  ExplicitWidth = 633
                end
              end
              object tbsOutputTax: TTabSheet
                Caption = 'Tax'
                ImageIndex = 1
                ExplicitWidth = 633
                object lvOuputTaxes: TListView
                  Left = 0
                  Top = 0
                  Width = 493
                  Height = 264
                  Align = alClient
                  BorderStyle = bsNone
                  Columns = <>
                  TabOrder = 0
                  ExplicitWidth = 633
                end
              end
              object tbsOutputServiceCharge: TTabSheet
                Caption = 'Service Charge'
                ImageIndex = 2
                ExplicitWidth = 633
                object lvOuputSC: TListView
                  Left = 0
                  Top = 0
                  Width = 493
                  Height = 264
                  Align = alClient
                  BorderStyle = bsNone
                  Columns = <>
                  TabOrder = 0
                  ExplicitWidth = 633
                end
              end
              object tbsOutputAll: TTabSheet
                Caption = 'All'
                ImageIndex = 3
                ExplicitWidth = 633
                object lvOuputAll: TListView
                  Left = 0
                  Top = 0
                  Width = 493
                  Height = 264
                  Align = alClient
                  BorderStyle = bsNone
                  Columns = <>
                  TabOrder = 0
                  ExplicitWidth = 633
                end
              end
            end
          end
        end
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 703
    Width = 979
    Height = 19
    Panels = <>
    ExplicitWidth = 1119
  end
  object MainMenu: TMainMenu
    Left = 896
    Top = 32
    object File1: TMenuItem
      Caption = '&File'
      object OpenItem: TMenuItem
        Caption = '&Open'
        ShortCut = 16463
        OnClick = OpenItemClick
      end
      object NewItem: TMenuItem
        Caption = '&New'
        ShortCut = 16462
        OnClick = NewItemClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object SaveItem: TMenuItem
        Caption = '&Save'
        ShortCut = 16467
        OnClick = SaveItemClick
      end
      object SaveAsItem: TMenuItem
        Caption = 'Save as'
        ShortCut = 24659
        OnClick = SaveAsItemClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object ExitItem: TMenuItem
        Caption = 'E&xit'
        ShortCut = 16472
        OnClick = ExitItemClick
      end
    end
  end
end
