object frmAddStock: TfrmAddStock
  Tag = 1024
  Left = 1
  Top = 1
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Stock Properties'
  ClientHeight = 604
  ClientWidth = 1356
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 473
    Height = 393
    Hint = 'The sales unit refers to the measure deducted by sales.'
    ActivePage = tsLocations
    MultiLine = True
    TabIndex = 1
    TabOrder = 0
    OnChange = PageControl1Change
    object tsItemDetails: TTabSheet
      Caption = 'Details'
      object gbItemDetail: TGroupBox
        Left = 8
        Top = 8
        Width = 449
        Height = 297
        Caption = 'Item Details'
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 55
          Width = 53
          Height = 13
          Caption = 'Description'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 16
          Top = 85
          Width = 58
          Height = 13
          Caption = 'Stock Group'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 25
          Width = 54
          Height = 13
          Caption = 'Stock Code'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtStockCode: TDBText
          Left = 176
          Top = 24
          Width = 185
          Height = 17
          DataField = 'CODE'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 16
          Top = 145
          Width = 69
          Height = 13
          Caption = 'Stocktake Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lblReductionUnit: TLabel
          Left = 16
          Top = 175
          Width = 70
          Height = 13
          Caption = 'Reduction Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lblConversion: TLabel
          Left = 16
          Top = 205
          Width = 153
          Height = 13
          Caption = 'Stocktake Unit = Reduction Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 16
          Top = 316
          Width = 42
          Height = 13
          Caption = 'Sale Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label6: TLabel
          Left = 24
          Top = 320
          Width = 64
          Height = 13
          Caption = 'Sale Unit Size'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label16: TLabel
          Left = 16
          Top = 235
          Width = 45
          Height = 13
          Caption = 'TAX. (%)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label18: TLabel
          Left = 256
          Top = 238
          Width = 13
          Height = 13
          Caption = '%'
          FocusControl = btnAddLocation
        end
        object Label33: TLabel
          Left = 16
          Top = 324
          Width = 101
          Height = 13
          Caption = 'General Ledger Code'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label31: TLabel
          Left = 16
          Top = 115
          Width = 80
          Height = 13
          Caption = 'Stock Sub-Group'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbeStockCode: TDBEdit
          Left = 176
          Top = 20
          Width = 137
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'CODE'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 0
          OnKeyPress = dbeStockCodeKeyPress
        end
        object btnAddStockGroup: TButton
          Left = 328
          Top = 85
          Width = 73
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Visible = False
          OnClick = btnAddStockGroupClick
        end
        object dbcEnabled: TDBCheckBox
          Left = 14
          Top = 265
          Width = 175
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Enabled'
          Color = clBtnFace
          DataField = 'ENABLED'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          TabOrder = 8
          ValueChecked = 'T'
          ValueUnchecked = 'F'
          OnKeyPress = dbeKeyPress
        end
        object dbeDescription: TDBEdit
          Left = 176
          Top = 55
          Width = 225
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'DESCRIPTION'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object btnNextCode: TButton
          Left = 328
          Top = 20
          Width = 73
          Height = 25
          Caption = 'Get Next'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnNextCodeClick
        end
        object cbStockGroup: TComboBox
          Left = 176
          Top = 85
          Width = 137
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          TabOrder = 3
          OnChange = cbStockGroupChange
          OnKeyDown = cbStockGroupKeyDown
          OnKeyPress = cbStockGroupKeyPress
        end
        object dbcbPurchaseUnit: TDBComboBox
          Left = 176
          Top = 145
          Width = 137
          Height = 21
          Hint = 
            'This is the unit you will count during a stocktake e.g. Bottle (' +
            'spirits), litre (bulk beer).'
          Style = csDropDownList
          Color = clInfoBk
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          Sorted = True
          TabOrder = 5
          OnKeyDown = dbcbPurchaseUnitKeyDown
          OnKeyPress = dbcbPurchaseUnitKeyPress
        end
        object dbcbReductionUnit: TDBComboBox
          Left = 176
          Top = 175
          Width = 137
          Height = 21
          Hint = 
            'This is the unit you will count during a stock reduction e.g. Bo' +
            'ttle (spirits), litre (bulk beer).'
          Style = csDropDownList
          Color = clInfoBk
          DataField = 'REDUCTION_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          Sorted = True
          TabOrder = 16
        end
        object btnAddPurchaseUnit: TButton
          Left = 328
          Top = 145
          Width = 73
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          OnClick = btnAddPurchaseUnitClick
        end
        object dbcSaleUnit: TDBComboBox
          Left = 176
          Top = 324
          Width = 137
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          DataField = 'SALE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          Sorted = True
          TabOrder = 9
          Visible = False
        end
        object btnAddSalesUnit: TButton
          Left = 296
          Top = 330
          Width = 73
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          Visible = False
          OnClick = btnAddSalesUnitClick
        end
        object dbeItemSize: TDBEdit
          Left = 176
          Top = 320
          Width = 137
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'ITEM_SIZE'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 11
          Visible = False
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object dbeGST: TDBEdit
          Left = 176
          Top = 235
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'GST_PERCENT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 7
          OnKeyPress = dbeKeyPress
        end
        object dbeGLCode: TDBEdit
          Left = 176
          Top = 328
          Width = 137
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'GL_CODE'
          DataSource = dsStockEdit
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 12
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object dbeConversionFactor: TDBEdit
          Left = 176
          Top = 205
          Width = 137
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'CONVERSION_FACTOR'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 17
          OnChange = dbeConversionFactorChange
          OnKeyPress = dbeConversionFactorKeyPress
        end
        object btnAddReductionUnit: TButton
          Left = 328
          Top = 175
          Width = 73
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 15
          OnClick = btnAddReductionUnitClick
        end
        object btnAddStockSubGroup: TButton
          Left = 328
          Top = 115
          Width = 73
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
          OnClick = btnAddStockSubGroupClick
        end
        object cbStockSubGroup: TDBComboBox
          Left = 176
          Top = 115
          Width = 137
          Height = 21
          Hint = 'This is the Stock Sub-Group you will take during a stocktake .'
          Style = csDropDownList
          Color = clInfoBk
          DataField = 'STOCK_SUB_GROUP'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          Sorted = True
          TabOrder = 14
          OnKeyDown = cbStockGroupKeyDown
          OnKeyPress = cbStockGroupKeyPress
        end
      end
      object btnNext1: TBitBtn
        Left = 384
        Top = 328
        Width = 73
        Height = 25
        Caption = 'Locations'
        TabOrder = 1
        OnClick = btnNext1Click
      end
    end
    object tsLocations: TTabSheet
      Caption = 'Locations'
      ImageIndex = 2
      object gbLocations: TGroupBox
        Left = 8
        Top = 8
        Width = 449
        Height = 305
        Caption = 'Locations'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label8: TLabel
          Left = 16
          Top = 121
          Width = 72
          Height = 13
          Caption = 'Minimum Level:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 16
          Top = 151
          Width = 76
          Height = 13
          Caption = 'Maximum Level:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 16
          Top = 275
          Width = 46
          Height = 13
          Caption = 'On Hand:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 238
          Top = 275
          Width = 49
          Height = 13
          Caption = 'On Order:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtOnOrder: TDBText
          Left = 326
          Top = 275
          Width = 89
          Height = 17
          DataField = 'ON_ORDER'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText1: TDBText
          Left = 238
          Top = 121
          Width = 81
          Height = 17
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText5: TDBText
          Left = 238
          Top = 152
          Width = 81
          Height = 14
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtOnHand: TDBText
          Left = 140
          Top = 276
          Width = 88
          Height = 16
          DataField = 'ON_HAND'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeLatestCost: TLabel
          Left = 16
          Top = 182
          Width = 111
          Height = 13
          Caption = 'Latest Cost (excl TAX):'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText6: TDBText
          Left = 262
          Top = 216
          Width = 153
          Height = 16
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label30: TLabel
          Left = 238
          Top = 184
          Width = 16
          Height = 13
          Caption = 'per'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeAveCost: TLabel
          Left = 16
          Top = 214
          Width = 122
          Height = 13
          Caption = 'Average Cost (excl TAX):'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText7: TDBText
          Left = 262
          Top = 184
          Width = 145
          Height = 16
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 238
          Top = 216
          Width = 16
          Height = 13
          Caption = 'per'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label44: TLabel
          Left = 16
          Top = 244
          Width = 101
          Height = 13
          Caption = 'Assessed Sale Value:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label45: TLabel
          Left = 239
          Top = 247
          Width = 19
          Height = 13
          Caption = 'inc.'
          FocusControl = btnAddLocation
        end
        object dbeMinLevel: TDBEdit
          Left = 140
          Top = 117
          Width = 89
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'MIN_LEVEL'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 4
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object dbeMaxLevel: TDBEdit
          Left = 140
          Top = 148
          Width = 89
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'MAX_LEVEL'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 5
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeMaxLevelKeyPress
        end
        object btnAddLocation: TButton
          Left = 16
          Top = 24
          Width = 105
          Height = 25
          Caption = 'Add Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnAddLocationClick
        end
        object dbgLocations: TDBGrid
          Left = 140
          Top = 24
          Width = 294
          Height = 81
          Color = clInfoBk
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentFont = False
          ReadOnly = True
          TabOrder = 2
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = [fsBold]
          Columns = <
            item
              Expanded = False
              FieldName = 'LOCATION'
              Width = 279
              Visible = True
            end>
        end
        object btnDeleteLocation: TButton
          Left = 16
          Top = 56
          Width = 105
          Height = 25
          Caption = 'Delete Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnDeleteLocationClick
        end
        object dbeLatestCost: TDBEdit
          Left = 140
          Top = 180
          Width = 89
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'LATEST_COST'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 6
          OnChange = dbeLatestCostChange
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeLatestCostKeyPress
        end
        object dbeAveCost: TDBEdit
          Left = 140
          Top = 212
          Width = 89
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'AVERAGE_COST'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 7
          OnChange = dbeAveCostChange
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeAveCostKeyPress
        end
        object chbDefaultLocation: TCheckBox
          Left = 20
          Top = 88
          Width = 103
          Height = 17
          Caption = 'Default Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = chbDefaultLocationClick
        end
        object dbeAssessedValue: TDBEdit
          Left = 140
          Top = 244
          Width = 89
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'ASSESSED_VALUE'
          DataSource = dsStockLocationEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 8
          OnChange = dbeAssessedValueChange
          OnKeyPress = dbeAssessedValueKeyPress
        end
      end
      object btnNext2: TBitBtn
        Left = 384
        Top = 320
        Width = 73
        Height = 25
        Caption = 'Suppliers'
        TabOrder = 2
        OnClick = btnNext1Click
      end
      object btnPrev1: TBitBtn
        Left = 8
        Top = 320
        Width = 73
        Height = 25
        Caption = 'Details'
        TabOrder = 1
        OnClick = btnPrev1Click
      end
    end
    object tsSuppliers: TTabSheet
      Caption = 'Suppliers'
      ImageIndex = 1
      object gbSuppliers: TGroupBox
        Left = 8
        Top = 8
        Width = 449
        Height = 337
        Caption = 'Stock Suppliers'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label7: TLabel
          Left = 16
          Top = 164
          Width = 50
          Height = 13
          Caption = 'Order Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 188
          Top = 180
          Width = 71
          Height = 13
          Caption = 'Stock Quantity'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label23: TLabel
          Left = 176
          Top = 180
          Width = 8
          Height = 13
          Caption = '='
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtStocktakeUnit: TDBText
          Left = 360
          Top = 180
          Width = 81
          Height = 17
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStockEdit
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label32: TLabel
          Left = 360
          Top = 208
          Width = 86
          Height = 13
          Caption = 'Order Unit Cost'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label21: TLabel
          Left = 16
          Top = 100
          Width = 385
          Height = 25
          AutoSize = False
          Caption = 
            'These are the units you will order from your suppliers. The Stoc' +
            'k Quantity field is the number of Stocktake Units this will orde' +
            'r.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label19: TLabel
          Left = 188
          Top = 265
          Width = 66
          Height = 13
          Caption = 'Supplier Code'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label25: TLabel
          Left = 188
          Top = 210
          Width = 68
          Height = 13
          Caption = 'Cost Exc. TAX'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label35: TLabel
          Left = 16
          Top = 130
          Width = 20
          Height = 13
          Caption = 'e.g.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label22: TLabel
          Left = 48
          Top = 130
          Width = 38
          Height = 13
          Caption = '[Dozen]'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label34: TLabel
          Left = 48
          Top = 146
          Width = 46
          Height = 13
          Caption = '[50L Keg]'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label36: TLabel
          Left = 192
          Top = 130
          Width = 8
          Height = 13
          Caption = '='
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label37: TLabel
          Left = 192
          Top = 146
          Width = 8
          Height = 13
          Caption = '='
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label39: TLabel
          Left = 296
          Top = 130
          Width = 100
          Height = 13
          Caption = '[    12    ]     Stubbies'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label38: TLabel
          Left = 296
          Top = 146
          Width = 85
          Height = 13
          Caption = '[    50    ]     Litres'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label40: TLabel
          Left = 188
          Top = 315
          Width = 68
          Height = 13
          Caption = 'Min Order Qty'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label41: TLabel
          Left = 360
          Top = 315
          Width = 81
          Height = 13
          Caption = 'Min Order Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label20: TLabel
          Left = 188
          Top = 238
          Width = 85
          Height = 13
          Caption = 'Supplier Unit Cost'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object btnRemoveSupplier: TButton
          Left = 16
          Top = 52
          Width = 105
          Height = 22
          Caption = 'Remove Supplier'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnRemoveSupplierClick
        end
        object btnAddSupplier: TButton
          Left = 16
          Top = 20
          Width = 105
          Height = 22
          Caption = 'Add Supplier'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnAddSupplierClick
        end
        object dbgSuppliers: TDBGrid
          Left = 128
          Top = 20
          Width = 305
          Height = 75
          Color = clInfoBk
          DataSource = dsSuppliers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentFont = False
          ReadOnly = True
          TabOrder = 2
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clMaroon
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = [fsBold]
          Columns = <
            item
              Expanded = False
              FieldName = 'SUPPLIER_NAME'
              Width = 241
              Visible = True
            end>
        end
        object dbcOrderQty: TDBEdit
          Left = 280
          Top = 176
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'QTY'
          DataSource = dsSuppliersStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 5
          OnChange = dbcOrderQtyChange
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object btnAddOrderUnit: TButton
          Left = 16
          Top = 310
          Width = 73
          Height = 22
          Caption = 'Add Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
          OnClick = btnAddOrderUnitClick
        end
        object dbeCost: TDBEdit
          Left = 280
          Top = 205
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'LATEST_COST'
          DataSource = dsSuppliersStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 6
          OnChange = dbeCostChange
          OnKeyPress = dbeCostKeyPress
        end
        object btnDeleteOrderUnit: TButton
          Left = 96
          Top = 310
          Width = 73
          Height = 22
          Caption = 'Delete Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          OnClick = btnDeleteOrderUnitClick
        end
        object dbgOrderUnit: TDBGrid
          Left = 16
          Top = 180
          Width = 153
          Height = 120
          Color = clInfoBk
          DataSource = dsSuppliersStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentFont = False
          ReadOnly = True
          TabOrder = 4
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clMaroon
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = [fsBold]
          Columns = <
            item
              Expanded = False
              FieldName = 'SUPPLIER_UNIT'
              Width = 151
              Visible = True
            end>
        end
        object dbeSupplierCode: TDBEdit
          Left = 280
          Top = 262
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'SUPPLIER_CODE'
          DataSource = dsSuppliersStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 7
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object chbDefaultOrderUnit: TCheckBox
          Left = 188
          Top = 288
          Width = 163
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Default Order Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          OnClick = chbDefaultOrderUnitClick
          OnKeyPress = chbDefaultOrderUnitKeyPress
        end
        object chbPreferredSupplier: TCheckBox
          Left = 16
          Top = 80
          Width = 111
          Height = 15
          Caption = 'Preferred Supplier'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = chbPreferredSupplierClick
        end
        object dbeMinOrderQty: TDBEdit
          Left = 280
          Top = 310
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'MIN_ORDER_QTY'
          DataSource = dsSuppliersStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 11
          OnKeyDown = dbeKeyDown
          OnKeyPress = dbeKeyPress
        end
        object dbeSupplierCost: TDBEdit
          Left = 280
          Top = 234
          Width = 73
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          DataField = 'Supplier_Cost'
          DataSource = dsSuppliersStock
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 12
        end
      end
    end
    object tsBarcodes: TTabSheet
      Caption = 'Barcodes'
      ImageIndex = 3
      object gbDetailsBarcodes: TGroupBox
        Left = 8
        Top = 19
        Width = 445
        Height = 150
        Caption = 'Item Barcodes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object btnAddItemBarcode: TButton
          Left = 327
          Top = 25
          Width = 105
          Height = 25
          Caption = 'Manual Barcode'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnAddItemBarcodeClick
        end
        object btnDeleteItemBarcode: TButton
          Left = 329
          Top = 113
          Width = 105
          Height = 25
          Caption = 'Delete Barcode'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnDeleteItemBarcodeClick
        end
        object btnScanItemBarcode: TBitBtn
          Left = 329
          Top = 69
          Width = 105
          Height = 26
          Caption = '&Scan'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btnScanItemBarcodeClick
          Glyph.Data = {
            5E060000424D5E06000000000000360400002800000018000000170000000100
            08000000000028020000C40E0000C40E00000001000000000000FFFFFF000202
            0100080A0200100D130013120A00191B0600241D0A001B1B1500201D16001712
            2200301E3E0023250A002A2B0C001E201600242317002B2A1B0030310B003033
            1900383B1A00443A1F00282520002E2C230032293500343328003B3C27003634
            3500413D2900423F3100321D5D0026036900390E7400391A68003E187700431B
            7600372057003E374200443749003428600042286D0042207600374707003C48
            1900404A000044421F00444C1B003D4227003F42320044442B00484C2B004644
            35004D4D38004C532900575B2E0052533C0057593A005B613800616239004A4A
            4400555542005B5B4700605E460055555500595A5900184B71005E5963005D61
            470063644B00696A4B00646452006C6C55006F734A006F7A4A0074745800797B
            5A0033607F0068686400716B7400747563007A7C650073767800817E5D00817E
            62002E078100390D960038118800410C8F00471886004A1997004501A000501D
            A1004F288D0055289900533C8900533B9D00572DA8005F2DB5005C30A200623B
            A4003500DC005712CF00572DC3006837D6005222E400672DF900165A9B006F4E
            9900705C82001057A500215CA200446F9F00776392007C7187001A67AA00226F
            B9003577AB002A7BBF00406EA3004B7BAF005A7AB70084688C002677C0007789
            390082903F007A8547007C8258007C8366007F8C68007B857100818A47008385
            5B0084944A008B975F00858466008A8B6A00868575008A8D74008E926B00949A
            6D0092947700999B7B00A09E7E008EA44B009EB458009AA37B00A6A67C009EB0
            6000AAB678003585BB004E87A9004E8EBC005E94AB004190BD008A898A008F8A
            950093958400979B8600A19E8400949494009A9C9800A29F98009F9FA7009BA3
            88009DAA8700A4A48800A7AA8C009DA79300A3A49400A8AC9400ACB18A00ADB8
            8C00AEB39A00B3B99D0083A7A800A6A5A600AAACA60092A8B200AFAEB1009AB1
            A400AFB4A300B6BBA400B3B3B400B9BBB700C1BBA700C1BFB8002386D0003E95
            CF004C87C200418EDB005295CA00659CC600559CD300619EDD003E99E50072A5
            C8005BA6DA0067A9DA005FB8DE006BB0DA0055A0E40066ABE4006BB2E30081AF
            C50090BAC700B1BBC10094B9DD00C1BDC600B4C19E00C2C09B00EBD99F00BCC4
            AC00BDC9AC00BEC6B100BDCAB500C2C5AC00C5CAAB00C2C6B300C3CAB400CED3
            A900CCD3BB00D5DABA00F2E4B900AEC5C20099C2D900A7C3DE00C5C5C500CBCC
            C900CECED600D1D4C500D6DBC700D6D6D600DBDBDB00E0DEDB009CC6E700DDDB
            E200DCE2CD00DDE8CE00E1E5CB00E4EACE00DEE3D300DFE8D500E2E5D800E6EB
            DB00EEF2C900EEF2DC00EBF8DB00D2E4E300E4E4E400ECECEA00DDE7F200EBE3
            F000EEEDF300EFF3E800F7FBEB00F4F4F400FFFFFF0000000000000000000000
            000000000000000000000000000000000000000000000000000000F800000000
            0000000000000000000000CFF9CFF89E77F2F8F2F8F2F8F2F9F2000000000000
            0000003838383833384BF2E1F2E1F2F8F2F80000000000000000003838384B38
            1910F9EBF8E1F8F2F9F2000000000000000000B13819384B4BFF00F8F2F800F8
            0000000000000000000000B1B1899ECFE18900F2F90000000000000000000000
            00000000CFB1CFDECFDEF20000000000000000000000000000000000CFB1CDCF
            E1CFCD0000000000000000000000000000000000009ECF9ECFE1CFE100000000
            00000000000000000000000000CF9ECFCDCFE1CF000000000000000000000000
            0000000000F8CFB1CFDEE1DEF200000000000000000000000000000000009ECF
            AFCFE1CFAF0000000000000000000000000000000000F29ECFB1CFDEBDDE0000
            00000000000000000000000000000089AFCFE1CF78DE00F2F9E19E9EAFE10000
            00000000000000E1CFB1CFB1CF9ECFB1CF193819384BF2000000000000000000
            9ECF9E8E9ECFF8F2E1384B794B894B000000000000000000DCB1CF9EDCE1F2F8
            00898E9ECF25389E00000000000000000089E1E1F8F2F8F2F9004B899E601938
            E1000000000000000000CFE1CFE1F200F2DEF24B896E10197900000000000000
            000000E19E384B89E1CFCDCF4B894B386E000000000000000000000000E1CF96
            CFE1CFE1B64BB1E1000000000000000000000000000000000000000000000000
            0000}
        end
        object dbgItemBarcode: TDBGrid
          Left = 17
          Top = 22
          Width = 293
          Height = 115
          Color = clInfoBk
          DataSource = dsItemBarcode
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete]
          ParentFont = False
          ReadOnly = True
          TabOrder = 3
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'DESCRIPTION'
              Title.Caption = 'Description'
              Width = 101
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'STOCKTAKE_UNIT'
              Title.Caption = 'Stocktake Unit'
              Width = 74
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'BARCODE'
              Title.Caption = 'Barcode'
              Width = 112
              Visible = True
            end>
        end
      end
      object gbSuppliersBarcodes: TGroupBox
        Left = 8
        Top = 184
        Width = 447
        Height = 150
        Caption = 'Suppliers Barcodes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object btnAddSupplierBarcode: TButton
          Left = 327
          Top = 25
          Width = 105
          Height = 25
          Caption = 'Manual Barcode'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnAddItemBarcodeClick
        end
        object btnDeleteSupplierBarcode: TButton
          Left = 327
          Top = 115
          Width = 105
          Height = 25
          Caption = 'Delete Barcode'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnDeleteSupplierBarcodeClick
        end
        object btnScanSupplierBarcode: TBitBtn
          Left = 329
          Top = 68
          Width = 105
          Height = 26
          Caption = '&Scan'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btnScanItemBarcodeClick
          Glyph.Data = {
            5E060000424D5E06000000000000360400002800000018000000170000000100
            08000000000028020000C40E0000C40E00000001000000000000FFFFFF000202
            0100080A0200100D130013120A00191B0600241D0A001B1B1500201D16001712
            2200301E3E0023250A002A2B0C001E201600242317002B2A1B0030310B003033
            1900383B1A00443A1F00282520002E2C230032293500343328003B3C27003634
            3500413D2900423F3100321D5D0026036900390E7400391A68003E187700431B
            7600372057003E374200443749003428600042286D0042207600374707003C48
            1900404A000044421F00444C1B003D4227003F42320044442B00484C2B004644
            35004D4D38004C532900575B2E0052533C0057593A005B613800616239004A4A
            4400555542005B5B4700605E460055555500595A5900184B71005E5963005D61
            470063644B00696A4B00646452006C6C55006F734A006F7A4A0074745800797B
            5A0033607F0068686400716B7400747563007A7C650073767800817E5D00817E
            62002E078100390D960038118800410C8F00471886004A1997004501A000501D
            A1004F288D0055289900533C8900533B9D00572DA8005F2DB5005C30A200623B
            A4003500DC005712CF00572DC3006837D6005222E400672DF900165A9B006F4E
            9900705C82001057A500215CA200446F9F00776392007C7187001A67AA00226F
            B9003577AB002A7BBF00406EA3004B7BAF005A7AB70084688C002677C0007789
            390082903F007A8547007C8258007C8366007F8C68007B857100818A47008385
            5B0084944A008B975F00858466008A8B6A00868575008A8D74008E926B00949A
            6D0092947700999B7B00A09E7E008EA44B009EB458009AA37B00A6A67C009EB0
            6000AAB678003585BB004E87A9004E8EBC005E94AB004190BD008A898A008F8A
            950093958400979B8600A19E8400949494009A9C9800A29F98009F9FA7009BA3
            88009DAA8700A4A48800A7AA8C009DA79300A3A49400A8AC9400ACB18A00ADB8
            8C00AEB39A00B3B99D0083A7A800A6A5A600AAACA60092A8B200AFAEB1009AB1
            A400AFB4A300B6BBA400B3B3B400B9BBB700C1BBA700C1BFB8002386D0003E95
            CF004C87C200418EDB005295CA00659CC600559CD300619EDD003E99E50072A5
            C8005BA6DA0067A9DA005FB8DE006BB0DA0055A0E40066ABE4006BB2E30081AF
            C50090BAC700B1BBC10094B9DD00C1BDC600B4C19E00C2C09B00EBD99F00BCC4
            AC00BDC9AC00BEC6B100BDCAB500C2C5AC00C5CAAB00C2C6B300C3CAB400CED3
            A900CCD3BB00D5DABA00F2E4B900AEC5C20099C2D900A7C3DE00C5C5C500CBCC
            C900CECED600D1D4C500D6DBC700D6D6D600DBDBDB00E0DEDB009CC6E700DDDB
            E200DCE2CD00DDE8CE00E1E5CB00E4EACE00DEE3D300DFE8D500E2E5D800E6EB
            DB00EEF2C900EEF2DC00EBF8DB00D2E4E300E4E4E400ECECEA00DDE7F200EBE3
            F000EEEDF300EFF3E800F7FBEB00F4F4F400FFFFFF0000000000000000000000
            000000000000000000000000000000000000000000000000000000F800000000
            0000000000000000000000CFF9CFF89E77F2F8F2F8F2F8F2F9F2000000000000
            0000003838383833384BF2E1F2E1F2F8F2F80000000000000000003838384B38
            1910F9EBF8E1F8F2F9F2000000000000000000B13819384B4BFF00F8F2F800F8
            0000000000000000000000B1B1899ECFE18900F2F90000000000000000000000
            00000000CFB1CFDECFDEF20000000000000000000000000000000000CFB1CDCF
            E1CFCD0000000000000000000000000000000000009ECF9ECFE1CFE100000000
            00000000000000000000000000CF9ECFCDCFE1CF000000000000000000000000
            0000000000F8CFB1CFDEE1DEF200000000000000000000000000000000009ECF
            AFCFE1CFAF0000000000000000000000000000000000F29ECFB1CFDEBDDE0000
            00000000000000000000000000000089AFCFE1CF78DE00F2F9E19E9EAFE10000
            00000000000000E1CFB1CFB1CF9ECFB1CF193819384BF2000000000000000000
            9ECF9E8E9ECFF8F2E1384B794B894B000000000000000000DCB1CF9EDCE1F2F8
            00898E9ECF25389E00000000000000000089E1E1F8F2F8F2F9004B899E601938
            E1000000000000000000CFE1CFE1F200F2DEF24B896E10197900000000000000
            000000E19E384B89E1CFCDCF4B894B386E000000000000000000000000E1CF96
            CFE1CFE1B64BB1E1000000000000000000000000000000000000000000000000
            0000}
        end
        object dbgSupplierBarcode: TDBGrid
          Left = 17
          Top = 22
          Width = 293
          Height = 115
          Color = clInfoBk
          DataSource = dsSupplierBarcode
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete]
          ParentFont = False
          ReadOnly = True
          TabOrder = 3
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'COMPANY_NAME'
              Title.Caption = 'Company Name'
              Width = 104
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'SUPPLIER_UNIT'
              Title.Caption = 'Supplier Unit'
              Width = 65
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'BARCODE'
              Title.Caption = 'Barcode'
              Width = 120
              Visible = True
            end>
        end
      end
    end
    object tsSubstitutes: TTabSheet
      Caption = 'Substitutes'
      ImageIndex = 4
      object gbDetailsSubstitutes1: TGroupBox
        Left = 8
        Top = 19
        Width = 445
        Height = 100
        Caption = 'Substitute1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object Label42: TLabel
          Left = 8
          Top = 17
          Width = 83
          Height = 13
          Caption = 'Select Stock Item'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label43: TLabel
          Left = 8
          Top = 43
          Width = 139
          Height = 13
          Caption = 'Substitute when stock item is'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label46: TLabel
          Left = 8
          Top = 69
          Width = 164
          Height = 13
          Caption = 'Don'#39't Substitute when Substitute1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label47: TLabel
          Left = 312
          Top = 45
          Width = 41
          Height = 13
          Caption = 'Variance'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object btnGo: TBitBtn
          Left = 368
          Top = 14
          Width = 73
          Height = 21
          Caption = '&Select'
          TabOrder = 0
          OnClick = btnGoClick
        end
        object edSearch: TEdit
          Left = 176
          Top = 15
          Width = 185
          Height = 21
          Color = 15269887
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object DBCmbsubststockitem1: TComboBox
          Left = 176
          Top = 40
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 2
        end
        object DBCmbsubsttem1: TComboBox
          Left = 176
          Top = 68
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 3
        end
        object txtvariance1: TNumericEdit
          Left = 360
          Top = 40
          Width = 79
          Height = 21
          Alignment = taLeftJustify
          Precision = 15
          NumericType = ntPercent
          FixedPoint = True
          Color = clInfoBk
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 4
        end
      end
      object gbDetailsSubstitutes2: TGroupBox
        Left = 8
        Top = 132
        Width = 445
        Height = 100
        Caption = 'Substitute2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object Label52: TLabel
          Left = 8
          Top = 17
          Width = 83
          Height = 13
          Caption = 'Select Stock Item'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label53: TLabel
          Left = 8
          Top = 43
          Width = 139
          Height = 13
          Caption = 'Substitute when stock item is'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label56: TLabel
          Left = 8
          Top = 69
          Width = 164
          Height = 13
          Caption = 'Don'#39't Substitute when Substitute2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label48: TLabel
          Left = 312
          Top = 45
          Width = 41
          Height = 13
          Caption = 'Variance'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object btnGo1: TBitBtn
          Left = 368
          Top = 14
          Width = 73
          Height = 23
          Caption = '&Select'
          TabOrder = 0
          OnClick = btnGo1Click
        end
        object edSearch1: TEdit
          Left = 176
          Top = 15
          Width = 185
          Height = 21
          Color = 15269887
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object DBCmbsubststockitem2: TComboBox
          Left = 176
          Top = 40
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 2
        end
        object DBCmbsubsttem2: TComboBox
          Left = 176
          Top = 68
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 3
        end
        object txtvariance2: TNumericEdit
          Left = 360
          Top = 40
          Width = 79
          Height = 21
          Alignment = taLeftJustify
          Precision = 15
          NumericType = ntPercent
          FixedPoint = True
          Color = clInfoBk
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 4
        end
      end
      object gbDetailsSubstitutes3: TGroupBox
        Left = 8
        Top = 245
        Width = 445
        Height = 100
        Caption = 'Substitute3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label62: TLabel
          Left = 8
          Top = 17
          Width = 83
          Height = 13
          Caption = 'Select Stock Item'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label63: TLabel
          Left = 8
          Top = 43
          Width = 139
          Height = 13
          Caption = 'Substitute when stock item is'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label49: TLabel
          Left = 312
          Top = 45
          Width = 41
          Height = 13
          Caption = 'Variance'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label66: TLabel
          Left = 8
          Top = 69
          Width = 164
          Height = 13
          Caption = 'Don'#39't Substitute when Substitute3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object btnGo3: TBitBtn
          Left = 368
          Top = 14
          Width = 73
          Height = 23
          Caption = '&Select'
          TabOrder = 0
          OnClick = btnGo3Click
        end
        object edSearch2: TEdit
          Left = 176
          Top = 15
          Width = 185
          Height = 21
          Color = 15269887
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
        end
        object DBCmbsubststockitem3: TComboBox
          Left = 176
          Top = 40
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 2
        end
        object DBCmbsubsttem3: TComboBox
          Left = 176
          Top = 68
          Width = 130
          Height = 21
          Style = csDropDownList
          Color = clInfoBk
          ItemHeight = 13
          TabOrder = 3
        end
        object txtvariance3: TNumericEdit
          Left = 360
          Top = 40
          Width = 79
          Height = 21
          Alignment = taLeftJustify
          Precision = 15
          NumericType = ntPercent
          FixedPoint = True
          Color = clInfoBk
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 4
        end
      end
      object chbminLevel: TCheckBox
        Left = 4
        Top = 344
        Width = 341
        Height = 17
        Caption = 'Revert to Original Stock Item when On Hand > 0 '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = chbminLevelClick
      end
      object cbReadyForSubstitute1: TCheckBox
        Left = 0
        Top = 0
        Width = 153
        Height = 15
        Caption = 'Ready for substitution1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object cbReadyForSubstitute2: TCheckBox
        Left = 0
        Top = 120
        Width = 153
        Height = 15
        Caption = 'Ready for substitution2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object cbReadyForSubstitute3: TCheckBox
        Left = 0
        Top = 232
        Width = 153
        Height = 15
        Caption = 'Ready for substitution3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
    end
  end
  object gbSalesUnit: TGroupBox
    Left = 8
    Top = 532
    Width = 417
    Height = 233
    Caption = 'Sales Units'
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 4
    object Label13: TLabel
      Left = 16
      Top = 48
      Width = 353
      Height = 17
      AutoSize = False
      Caption = 
        'e.g. milli-litres for spirits, litres for bulk beer, Unit if wor' +
        'king in fractions.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label14: TLabel
      Left = 16
      Top = 32
      Width = 268
      Height = 13
      Caption = 'The sales unit refers to the measure deducted by sales.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label15: TLabel
      Left = 16
      Top = 80
      Width = 353
      Height = 17
      AutoSize = False
      Caption = 'Stock Item Size is the number of Sales Units in the Stock Unit.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label17: TLabel
      Left = 48
      Top = 112
      Width = 147
      Height = 13
      Caption = '1125 (ml per bottle) for spirits.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label26: TLabel
      Left = 48
      Top = 128
      Width = 90
      Height = 13
      Caption = '1 (L) for bulk beer.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label27: TLabel
      Left = 16
      Top = 112
      Width = 20
      Height = 13
      Caption = 'e.g.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label28: TLabel
      Left = 48
      Top = 144
      Width = 40
      Height = 13
      Caption = '1 (Unit).'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object btnOk: TBitBtn
    Left = 304
    Top = 408
    Width = 81
    Height = 33
    Caption = '&OK'
    TabOrder = 1
    OnClick = btnOkClick
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
  end
  object btnCancel: TBitBtn
    Left = 392
    Top = 408
    Width = 89
    Height = 33
    TabOrder = 2
    OnClick = btnCancelClick
    Kind = bkCancel
  end
  object gbItemClass: TGroupBox
    Left = 8
    Top = 448
    Width = 417
    Height = 97
    Caption = 'Assistant'
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 3
    object Label29: TLabel
      Left = 304
      Top = 56
      Width = 15
      Height = 13
      Caption = 'mls'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object rbUnitClass: TRadioButton
      Left = 16
      Top = 24
      Width = 281
      Height = 17
      Caption = 'Unit (e.g. Bottled Beer, RTD, Cigar)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object rbBottleClass: TRadioButton
      Left = 16
      Top = 48
      Width = 113
      Height = 17
      Caption = 'Wine / Spirits'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object rbBulkClass: TRadioButton
      Left = 16
      Top = 72
      Width = 113
      Height = 17
      Caption = 'Bulk Beer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object cbBottleSize: TComboBox
      Left = 176
      Top = 44
      Width = 121
      Height = 21
      Color = clInfoBk
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      TabOrder = 2
      TabStop = False
      Items.Strings = (
        '1500'
        '1125'
        '1000'
        '750'
        '700'
        '375'
        '350'
        '300')
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 520
    Top = 8
  end
  object dsStockEdit: TDataSource
    AutoEdit = False
    DataSet = dtStockEdit
    Left = 824
    Top = 192
  end
  object dtStockEdit: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from STOCK'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY')
    InsertSQL.Strings = (
      'insert into STOCK'
      '  (  CODE, DESCRIPTION, ENABLED, GL_CODE, GST_PERCENT, '
      ' ITEM_SIZE, SALE_UNIT, STOCK_GROUP_KEY, STOCKTAKE_UNIT, '
      'STOCK_SUB_GROUP,REDUCTION_UNIT, CONVERSION_FACTOR)'
      'values'
      '  ( :CODE, :DESCRIPTION, :ENABLED, :GL_CODE,:GST_PERCENT, '
      ':ITEM_SIZE, :SALE_UNIT, :STOCK_GROUP_KEY, :STOCKTAKE_UNIT, '
      ':STOCK_SUB_GROUP, :REDUCTION_UNIT, :CONVERSION_FACTOR)')
    RefreshSQL.Strings = (
      'Select '
      '  STOCK_KEY,'
      '  STOCK_GROUP_KEY,'
      '  CODE,'
      '  GL_CODE,'
      '  DESCRIPTION,'
      '  STOCKTAKE_UNIT,'
      '  SALE_UNIT,'
      '  GST_PERCENT,'
      '  ITEM_SIZE,'
      '  ENABLED,'
      '  DELETED,'
      '  REDUCTION_UNIT,'
      '  CONVERSION_FACTOR,'
      '  STOCK_SUB_GROUP '
      'from STOCK '
      'where'
      '  STOCK_KEY = :STOCK_KEY')
    SelectSQL.Strings = (
      'SELECT'
      '   Stock_Key,'
      '   Stock.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   Stock.CODE,'
      '   Stock.GL_Code,'
      '   Stock.DESCRIPTION,'
      '   Stock.STOCKTAKE_UNIT,'
      '   Stock.SALE_UNIT,'
      '   Stock.ITEM_SIZE,'
      '   Stock.GST_PERCENT,'
      '   Stock.ENABLED,'
      '   Stock.REDUCTION_UNIT,'
      '   Stock.CONVERSION_FACTOR,'
      '   Stock.STOCK_SUB_GROUP '
      'FROM'
      '   STOCK Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      'WHERE'
      '   Stock_Key = :Stock_Key')
    ModifySQL.Strings = (
      'update STOCK'
      'set'
      '  CODE = :CODE,'
      '  DESCRIPTION = :DESCRIPTION,'
      '  ENABLED = :ENABLED,'
      '  GL_CODE = :GL_CODE,'
      '  GST_PERCENT = :GST_PERCENT,'
      '  ITEM_SIZE = :ITEM_SIZE,'
      '  SALE_UNIT = :SALE_UNIT,'
      '  STOCK_GROUP_KEY = :STOCK_GROUP_KEY,'
      '  REDUCTION_UNIT = :REDUCTION_UNIT,'
      '  CONVERSION_FACTOR = :CONVERSION_FACTOR,'
      '  STOCKTAKE_UNIT = :STOCKTAKE_UNIT,'
      '  STOCK_SUB_GROUP = :STOCK_SUB_GROUP'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY')
    GeneratorField.Field = 'STOCK_KEY'
    GeneratorField.Generator = 'GEN_STOCK_KEY'
    Left = 712
    Top = 192
  end
  object qrAddStockGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'INSERT INTO StockGroup ('
      '   Stock_Group,'
      '   SORT_ORDER,'
      '   STOCKTAKE_VISIBLE)'
      'VALUES ('
      '   :Stock_Group,'
      '   :SORT_ORDER,'
      '   '#39'T'#39')')
    Left = 520
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Group'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SORT_ORDER'
        ParamType = ptUnknown
      end>
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 520
    Top = 168
  end
  object IBQuery: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 584
    Top = 8
  end
  object qrAddSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into SUPPLIERSTOCK('
      '   SUPPLIER_STOCK_KEY,'
      '   STOCK_KEY,'
      '   SUPPLIER_KEY,'
      '   SUPPLIER_UNIT,'
      '   MIN_ORDER_QTY,'
      '   PREFERRED_SUPPLIER,'
      '   DEFAULT_ORDER_UNIT)'
      'Values('
      '   :SUPPLIER_STOCK_KEY,'
      '   :STOCK_KEY,'
      '   :SUPPLIER_KEY,'
      '   :SUPPLIER_UNIT,'
      '   1,'
      '   :PREFERRED_SUPPLIER,'
      '   :DEFAULT_ORDER_UNIT)')
    UniDirectional = True
    Left = 616
    Top = 72
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'PREFERRED_SUPPLIER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DEFAULT_ORDER_UNIT'
        ParamType = ptUnknown
      end>
  end
  object qrSuppliers: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrSuppliersAfterScroll
    DataSource = dsStockEdit
    SQL.Strings = (
      'Select Distinct'
      '   SupplierStock.Stock_Key,'
      '   SupplierStock.Supplier_Key,'
      '   Contact.Company_Name Supplier_Name'
      'From'
      '   SupplierStock Inner Join Contact on'
      '      SupplierStock.Supplier_Key = Contact.Contact_LK'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key'
      'Order By'
      '   Contact.Company_Name Asc')
    Left = 520
    Top = 224
    ParamData = <
      item
        DataType = ftInteger
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object dsSuppliers: TDataSource
    AutoEdit = False
    DataSet = qrSuppliers
    Left = 608
    Top = 224
  end
  object sqlFindSupplier: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select Count(*)'
      'From'
      '   SupplierStock'
      'Where'
      '   Supplier_Key = :Supplier_Key And'
      '   Stock_Key = :Stock_Key')
    Transaction = Transaction
    Left = 520
    Top = 320
  end
  object dtStockLocationEdit: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = dtStockLocationEditAfterOpen
    AfterScroll = dtStockLocationEditAfterScroll
    BeforeInsert = dtStockLocationEditBeforeInsert
    DeleteSQL.Strings = (
      'delete from STOCKLOCATION'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY and'
      '  LOCATION = :OLD_LOCATION')
    InsertSQL.Strings = (
      'insert into STOCKLOCATION'
      '  (STOCK_KEY, LOCATION, MIN_LEVEL, MAX_LEVEL, ON_HAND,'
      'LATEST_COST, '
      '   AVERAGE_COST, DEFAULT_LOCATION,ASSESSED_VALUE'
      ')'
      'values'
      '  (:STOCK_KEY, :LOCATION, :MIN_LEVEL, :MAX_LEVEL, :ON_HAND,'
      
        '   :LATEST_COST, :AVERAGE_COST, :DEFAULT_LOCATION, :ASSESSED_VAL' +
        'UE'
      ')')
    RefreshSQL.Strings = (
      'Select '
      '  STOCK_KEY,'
      '  LOCATION,'
      '  MAX_LEVEL,'
      '  MIN_LEVEL,'
      '  ON_HAND,'
      '  AVERAGE_COST,'
      '  LATEST_COST,'
      '  INITIALISED,'
      '  STOCKTAKE,'
      '  VARIANCE,'
      '  DEFAULT_LOCATION,'
      '  ASSESSED_VALUE'
      'from STOCKLOCATION '
      'where'
      '  STOCK_KEY = :STOCK_KEY and'
      '  LOCATION = :LOCATION')
    SelectSQL.Strings = (
      'Select'
      '   StockLocation.STOCK_KEY,'
      '   StockLocation.LOCATION,'
      '   StockLocation.MIN_LEVEL,'
      '   StockLocation.MAX_LEVEL,'
      '   StockLocation.ON_HAND,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15, 4)) On_Order,'
      '   StockLocation.LATEST_COST,'
      '   StockLocation.AVERAGE_COST,'
      '   StockLocation.DEFAULT_LOCATION,'
      '    StockLocation.ASSESSED_VALUE,'
      '    StockLocation.INITIALISED,'
      '   StockLocation.SALES_PENDING,'
      '   StockLocation.WRITEOFFS_PENDING '
      'From'
      '   Stock Right Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   StockLocation.STOCK_KEY = :STOCK_KEY'
      'Group By'
      '   StockLocation.STOCK_KEY,'
      '   StockLocation.LOCATION,'
      '   StockLocation.MIN_LEVEL,'
      '   StockLocation.MAX_LEVEL,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST,'
      '   StockLocation.AVERAGE_COST,'
      '   StockLocation.DEFAULT_LOCATION,'
      '   StockLocation.ASSESSED_VALUE,'
      '   StockLocation.INITIALISED,'
      '   StockLocation.SALES_PENDING,'
      '   StockLocation.WRITEOFFS_PENDING ')
    ModifySQL.Strings = (
      'update STOCKLOCATION'
      'set'
      '  STOCK_KEY = :STOCK_KEY,'
      '  LOCATION = :LOCATION,'
      '  MIN_LEVEL = :MIN_LEVEL,'
      '  MAX_LEVEL = :MAX_LEVEL,'
      '  ON_HAND = :ON_HAND,'
      '  LATEST_COST = :LATEST_COST,'
      '  AVERAGE_COST = :AVERAGE_COST,'
      '  DEFAULT_LOCATION = :DEFAULT_LOCATION,'
      '  ASSESSED_VALUE = :ASSESSED_VALUE'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY and'
      '  LOCATION = :OLD_LOCATION')
    DataSource = dsStockEdit
    Left = 712
    Top = 240
  end
  object dsStockLocationEdit: TDataSource
    DataSet = dtStockLocationEdit
    Left = 824
    Top = 240
  end
  object qrNewItem: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into STOCK('
      '   STOCK_KEY,'
      '   Stock_Group_Key,'
      '   CODE,'
      '   DESCRIPTION,'
      '   ENABLED,'
      '   Deleted,'
      '   ITEM_SIZE,'
      '   STOCKTAKE_UNIT,'
      '   SALE_UNIT,'
      '   GST_Percent,'
      '   REDUCTION_UNIT,'
      '   CONVERSION_FACTOR,'
      '   STOCK_SUB_GROUP'
      ')'
      'Values('
      '   :STOCK_KEY,'
      '   :Stock_Group_Key,'
      '   :CODE,'
      '   :DESCRIPTION,'
      '   '#39'T'#39','
      '   '#39'F'#39','
      '   :ITEM_SIZE,'
      '   :STOCKTAKE_UNIT,'
      '   :SALE_UNIT,'
      '   :GST_Percent,'
      '   :REDUCTION_UNIT,'
      '   :CONVERSION_FACTOR,'
      '   :STOCK_SUB_GROUP'
      ')')
    Left = 520
    Top = 72
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Group_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CODE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DESCRIPTION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ITEM_SIZE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCKTAKE_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SALE_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'GST_Percent'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REDUCTION_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CONVERSION_FACTOR'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_SUB_GROUP'
        ParamType = ptUnknown
      end>
  end
  object qrRemoveSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Delete from'
      '   SUPPLIERSTOCK'
      'where'
      '   SUPPLIER_KEY = :SUPPLIER_KEY AND'
      '   STOCK_KEY = :STOCK_KEY')
    UniDirectional = True
    Left = 616
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object dtSuppliersStock: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = dtSuppliersStockAfterOpen
    AfterScroll = dtSuppliersStockAfterScroll
    BeforePost = dtSuppliersStockBeforePost
    DeleteSQL.Strings = (
      'delete from SUPPLIERSTOCK'
      'where'
      '  SUPPLIER_STOCK_KEY = :OLD_SUPPLIER_STOCK_KEY')
    InsertSQL.Strings = (
      'insert into SUPPLIERSTOCK'
      '  (SUPPLIER_STOCK_KEY, STOCK_KEY, SUPPLIER_KEY, SUPPLIER_UNIT, '
      'PREFERRED_SUPPLIER, '
      
        '   DEFAULT_ORDER_UNIT, QTY, Min_Order_Qty, LATEST_COST, SUPPLIER' +
        '_CODE)'
      'values'
      
        '  (:SUPPLIER_STOCK_KEY, :STOCK_KEY, :SUPPLIER_KEY, :SUPPLIER_UNI' +
        'T, '
      ':PREFERRED_SUPPLIER, '
      
        '   :DEFAULT_ORDER_UNIT, :QTY, :Min_Order_Qty, :LATEST_COST, :SUP' +
        'PLIER_CODE)')
    RefreshSQL.Strings = (
      'Select '
      '  SUPPLIER_STOCK_KEY,'
      '  SUPPLIER_KEY,'
      '  STOCK_KEY,'
      '  SUPPLIER_UNIT,'
      '  SUPPLIER_CODE,'
      '  PREFERRED_SUPPLIER,'
      '  DEFAULT_ORDER_UNIT,'
      '  QTY,'
      '  Min_Order_Qty,'
      '  LATEST_COST,'
      '  Cast(Latest_Cost/Qty as numeric(15, 4)) AS Supplier_Cost '
      ''
      'from SUPPLIERSTOCK '
      'where'
      '  SUPPLIER_STOCK_KEY = :SUPPLIER_STOCK_KEY')
    SelectSQL.Strings = (
      'Select'
      '    Supplier_Stock_Key,'
      '    Stock_Key,'
      '    Supplier_Key,'
      '    Supplier_Unit,'
      '    Preferred_Supplier,'
      '    Default_Order_Unit,'
      '    Qty,'
      '    Min_Order_Qty,'
      '    Latest_Cost,'
      '    Supplier_Code,'
      '    Cast(Latest_Cost/Qty as numeric(15, 4)) AS Supplier_Cost '
      'From'
      '   SUPPLIERSTOCK'
      'Where'
      '   Stock_Key = :Stock_Key And'
      '   Supplier_Key = :Supplier_Key'
      'Order By'
      '   Supplier_Unit Asc'
      '')
    ModifySQL.Strings = (
      'update SUPPLIERSTOCK'
      'set'
      '  SUPPLIER_STOCK_KEY = :SUPPLIER_STOCK_KEY,'
      '  STOCK_KEY = :STOCK_KEY,'
      '  SUPPLIER_KEY = :SUPPLIER_KEY,'
      '  SUPPLIER_UNIT = :SUPPLIER_UNIT,'
      '  PREFERRED_SUPPLIER = :PREFERRED_SUPPLIER,'
      '  DEFAULT_ORDER_UNIT = :DEFAULT_ORDER_UNIT,'
      '  QTY = :QTY,'
      '  Min_Order_Qty = :Min_Order_Qty,'
      '  LATEST_COST = :LATEST_COST,'
      '  SUPPLIER_CODE = :SUPPLIER_CODE'
      'where'
      '  SUPPLIER_STOCK_KEY = :OLD_SUPPLIER_STOCK_KEY')
    GeneratorField.Field = 'SUPPLIER_STOCK_KEY'
    GeneratorField.Generator = 'GEN_SUPPLIERSTOCK'
    DataSource = dsSuppliers
    Left = 520
    Top = 272
  end
  object dsSuppliersStock: TDataSource
    DataSet = dtSuppliersStock
    OnDataChange = dsSuppliersStockDataChange
    Left = 608
    Top = 272
  end
  object qrClearPreferredSupplier: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Update'
      '   SupplierStock'
      'Set'
      '   Preferred_Supplier = '#39'F'#39
      'Where'
      '   Stock_Key = :Stock_Key')
    Transaction = Transaction
    Left = 616
    Top = 176
  end
  object qrSupplierName: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select '
      '   Company_Name'
      'From'
      '   Contact'
      'Where'
      '   Contact_LK = :Supplier_Key')
    Transaction = Transaction
    Left = 520
    Top = 368
  end
  object IBDataSet1: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = dtStockLocationEditAfterOpen
    AfterScroll = dtStockLocationEditAfterScroll
    BeforeInsert = dtStockLocationEditBeforeInsert
    DeleteSQL.Strings = (
      'delete from STOCKLOCATION'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY and'
      '  LOCATION = :OLD_LOCATION')
    InsertSQL.Strings = (
      'insert into STOCKLOCATION'
      '  (STOCK_KEY, LOCATION, MIN_LEVEL, MAX_LEVEL, ON_HAND,'
      'LATEST_COST, '
      '   AVERAGE_COST, DEFAULT_LOCATION)'
      'values'
      '  (:STOCK_KEY, :LOCATION, :MIN_LEVEL, :MAX_LEVEL, :ON_HAND,'
      '   :LATEST_COST, :AVERAGE_COST, :DEFAULT_LOCATION)')
    RefreshSQL.Strings = (
      'Select '
      '  STOCK_KEY,'
      '  LOCATION,'
      '  MAX_LEVEL,'
      '  MIN_LEVEL,'
      '  ON_HAND,'
      '  AVERAGE_COST,'
      '  LATEST_COST,'
      '  INITIALISED,'
      '  STOCKTAKE,'
      '  VARIANCE,'
      '  DEFAULT_LOCATION'
      'from STOCKLOCATION '
      'where'
      '  STOCK_KEY = :STOCK_KEY and'
      '  LOCATION = :LOCATION')
    SelectSQL.Strings = (
      'Select'
      '   Stock.STOCK_KEY,'
      '   StockLocation.LOCATION,'
      '   StockLocation.MIN_LEVEL,'
      '   StockLocation.MAX_LEVEL,'
      '   StockLocation.ON_HAND,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15, 4)) On_Order,'
      '   StockLocation.LATEST_COST,'
      '   StockLocation.AVERAGE_COST,'
      '   StockLocation.DEFAULT_LOCATION'
      'From'
      '   Stock Left Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   Stock.STOCK_KEY = :STOCK_KEY'
      'Group By'
      '   Stock.STOCK_KEY,'
      '   StockLocation.LOCATION,'
      '   StockLocation.MIN_LEVEL,'
      '   StockLocation.MAX_LEVEL,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST,'
      '   StockLocation.AVERAGE_COST,'
      '   StockLocation.DEFAULT_LOCATION'
      'Order By'
      '   StockLocation.LOCATION')
    ModifySQL.Strings = (
      'update STOCKLOCATION'
      'set'
      '  STOCK_KEY = :STOCK_KEY,'
      '  LOCATION = :LOCATION,'
      '  MIN_LEVEL = :MIN_LEVEL,'
      '  MAX_LEVEL = :MAX_LEVEL,'
      '  ON_HAND = :ON_HAND,'
      '  LATEST_COST = :LATEST_COST,'
      '  AVERAGE_COST = :AVERAGE_COST,'
      '  DEFAULT_LOCATION = :DEFAULT_LOCATION'
      'where'
      '  STOCK_KEY = :OLD_STOCK_KEY and'
      '  LOCATION = :OLD_LOCATION')
    DataSource = dsStockEdit
    Left = 712
    Top = 288
  end
  object qrLocations: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = trMenuMate
    SQL.Strings = (
      'Select Distinct '
      '   NAME'
      'From'
      '   LOCATIONS'
      'Where'
      '   Location_Type <> 0'
      'Order By'
      '   NAME')
    Left = 808
    Top = 336
  end
  object trMenuMate: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 584
    Top = 8
  end
  object dtItemBarcode: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from Barcode'
      'where'
      '  BARCODE = :OLD_BARCODE and'
      '  STOCK_KEY = :OLD_STOCK_KEY')
    InsertSQL.Strings = (
      'insert into Barcode'
      '  (BARCODE, STOCK_KEY, SUPPLIER_STOCK_KEY)'
      'values'
      '  (:BARCODE,  :STOCK_KEY,  :SUPPLIER_STOCK_KEY)')
    RefreshSQL.Strings = (
      'Select'
      '   Barcode.STOCK_KEY,'
      '   Barcode.SUPPLIER_STOCK_KEY,   '
      '   Barcode.Barcode,'
      '   Stock.Description,'
      '    Stock.Stocktake_Unit'
      'From'
      '   Barcode Left Join Stock On'
      '      Barcode.STOCK_KEY = Stock.STOCK_KEY'
      'Where'
      '   Stock.STOCK_KEY = :OLD_STOCK_KEY')
    SelectSQL.Strings = (
      'Select'
      '   Barcode.STOCK_KEY,'
      '   Barcode.SUPPLIER_STOCK_KEY,   '
      '   Barcode.Barcode,'
      '   Stock.Description,'
      '    Stock.Stocktake_Unit'
      'From'
      '   Barcode Left Join Stock On'
      '      BarCode.STOCK_KEY = Stock.STOCK_KEY'
      'Where'
      '   Stock.STOCK_KEY = :STOCK_KEY')
    ModifySQL.Strings = (
      'update Barcode'
      'set'
      '  BARCODE = :BARCODE,'
      '  STOCK_KEY = :STOCK_KEY,'
      '  SUPPLIER_STOCK_KEY = :SUPPLIER_STOCK_KEY'
      'where'
      '  BARCODE = :OLD_BARCODE and'
      '  STOCK_KEY = :OLD_STOCK_KEY')
    Left = 768
    Top = 16
  end
  object IBSQL1: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Update'
      '   SupplierStock'
      'Set'
      '   Preferred_Supplier = '#39'F'#39
      'Where'
      '   Stock_Key = :Stock_Key')
    Transaction = Transaction
    Left = 704
    Top = 360
  end
  object dsItemBarcode: TDataSource
    DataSet = dtItemBarcode
    Left = 840
    Top = 16
  end
  object dsSupplierBarcode: TDataSource
    DataSet = dtSupplierBarcode
    Left = 840
    Top = 72
  end
  object dtSupplierBarcode: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from Barcode'
      'where'
      '  BARCODE = :OLD_BARCODE and'
      '  SUPPLIER_STOCK_KEY = :OLD_SUPPLIER_STOCK_KEY')
    InsertSQL.Strings = (
      'insert into Barcode'
      '  (BARCODE, STOCK_KEY, SUPPLIER_STOCK_KEY)'
      'values'
      '  (:BARCODE, :STOCK_KEY, :SUPPLIER_STOCK_KEY)')
    RefreshSQL.Strings = (
      'Select'
      '   Barcode.STOCK_KEY,'
      '   Barcode.SUPPLIER_STOCK_KEY,   '
      '   Barcode.Barcode,'
      '   SupplierStock.Supplier_Unit,'
      '    Contact.Company_Name'
      'From'
      '   Barcode Left Join SupplierStock On'
      
        '      BarCode.SUPPLIER_STOCK_KEY = SupplierStock.SUPPLIER_STOCK_' +
        'KEY'
      '   Left Join Contact on'
      '      SupplierStock.Supplier_Key = Contact_LK'
      'Where'
      '   SupplierStock.SUPPLIER_STOCK_KEY = :OLD_SUPPLIER_STOCK_KEY')
    SelectSQL.Strings = (
      'Select'
      '   Barcode.STOCK_KEY,'
      '   Barcode.SUPPLIER_STOCK_KEY,   '
      '   Barcode.Barcode,'
      '   SupplierStock.Supplier_Unit,'
      '    Contact.Company_Name'
      'From'
      '   Barcode Left Join SupplierStock On'
      
        '      BarCode.SUPPLIER_STOCK_KEY = SupplierStock.SUPPLIER_STOCK_' +
        'KEY'
      '   Left Join Contact on'
      '      SupplierStock.Supplier_Key = Contact_LK'
      'Where'
      '   SupplierStock.STOCK_KEY = :STOCK_KEY')
    ModifySQL.Strings = (
      'update Barcode'
      'set'
      '  BARCODE = :BARCODE,'
      '  STOCK_KEY = :STOCK_KEY,'
      '  SUPPLIER_STOCK_KEY = :SUPPLIER_STOCK_KEY'
      'where'
      '  BARCODE = :OLD_BARCODE and'
      '  SUPPLIER_STOCK_KEY = :OLD_SUPPLIER_STOCK_KEY')
    Left = 768
    Top = 72
  end
  object qrSupplierUnits: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   SupplierStock.Supplier_Stock_Key,'
      '   SupplierStock.Supplier_Key,'
      '   SupplierStock.Supplier_Unit,'
      '   Contact.Company_Name '
      'From'
      '   SupplierStock Inner Join Contact on'
      '      SupplierStock.Supplier_Key = Contact.Contact_LK'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key'
      'Order By'
      '   Contact.Company_Name Asc')
    Left = 696
    Top = 16
    ParamData = <
      item
        DataType = ftInteger
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object sqlBarcode: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 696
    Top = 72
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   STOCK.STOCK_KEY,'
      '   STOCK.CODE,'#9#9#9#9
      '   STOCK.DESCRIPTION,'
      '   STOCK.UNIT,'
      '   StockGroup.Stock_Group '
      'FROM '
      
        '   STOCK INNER JOIN StockGroup ON STOCK.Stock_Group_Key = StockG' +
        'roup.Stock_Group_Key'
      'WHERE '
      '   (Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And'
      '   Stock.Enabled = '#39'T'#39' And'
      '   UPPER(DESCRIPTION) LIKE :DESCRIPTION '
      'ORDER BY '
      '   StockGroup.Sort_Order ASC')
    Left = 608
    Top = 336
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'DESCRIPTION'
        ParamType = ptUnknown
      end>
  end
  object qrSubstitute: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      
        'INSERT INTO STOCK_SUBSTITUTE_PERMISSIONS (SUBSTITUTE_KEY, SUBSTI' +
        'TUTE_COND1, SUBSTITUTE_COND2, SUBSTITUTE_COND3, TIME_STAMP, SELE' +
        'CTABLE)'
      ' VALUES ('
      ':SUBSTITUTE_KEY, '
      ':SUBSTITUTE_COND1, '
      ':SUBSTITUTE_COND2, '
      ':SUBSTITUTE_COND3, '
      ':TIME_STAMP, '
      ':SELECTABLE'
      ')')
    Left = 784
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND1'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND2'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND3'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'TIME_STAMP'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SELECTABLE'
        ParamType = ptUnknown
      end>
  end
  object dtStockSubstituteCount: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    RefreshSQL.Strings = (
      'Select'
      '   USER_NAME,'
      '   USER_ID,'
      '   NEXT_ORDER_NUMBER,'
      '   ORDER_PREFIX,'
      '   ORDER_SUFFIX'
      'From'
      '   USERS'
      'Where'
      '  USER_ID = :USER_ID')
    SelectSQL.Strings = (
      'SELECT max(SUBSTITUTE_KEY) SUBSTITUTE_KEY'
      'FROM STOCK_SUBSTITUTES')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 744
    Top = 392
  end
  object qrSelectSubstitute: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrSuppliersAfterScroll
    DataSource = dsStockEdit
    SQL.Strings = (
      'SELECT count(STOCK_KEY) STOCK_KEY'
      'FROM STOCK_SUBSTITUTES where STOCK_KEY=:STOCK_KEY')
    Left = 824
    Top = 392
    ParamData = <
      item
        DataType = ftInteger
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object STOCK_SUBSTITUTE_PERMISSIONS: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'INSERT INTO STOCK_SUBSTITUTES ('
      'STOCK_KEY, '
      'SUBSTITUTE_STOCK_KEY,'
      'SUBSTITUTE_COND1, '
      'SUBSTITUTE_COND2, '
      'SUBSTITUTE_COND3)'
      ' VALUES ('
      ':STOCK_KEY, '
      ':SUBSTITUTE_STOCK_KEY, '
      ':SUBSTITUTE_COND1, '
      ':SUBSTITUTE_COND2, '
      ':SUBSTITUTE_COND3'
      ')')
    Left = 864
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND1'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND2'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND3'
        ParamType = ptUnknown
      end>
  end
  object qrSubstituteId: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'select SUBSTITUTE_KEY  from STOCK_SUBSTITUTES'
      ' WHERE      STOCK_KEY=:STOCK_KEY AND '
      'SUBSTITUTE_STOCK_KEY=:SUBSTITUTE_STOCK_KEY')
    Left = 704
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrChkStockIdExist: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'select 1 as IdExist from rdb$database'
      'where exists (SELECT     SUBSTITUTE_KEY'
      '                        FROM         STOCK_SUBSTITUTES'
      '                        WHERE      '
      ' SUBSTITUTE_KEY=:SUBSTITUTE_KEY )'
      'UNION'
      'select 0 as IdExist from rdb$database'
      'where NOT exists (SELECT     SUBSTITUTE_KEY'
      '                        FROM         STOCK_SUBSTITUTES'
      
        '                        WHERE       SUBSTITUTE_KEY=:SUBSTITUTE_K' +
        'EY  )'
      '                        ')
    Left = 672
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrInsertSTOCK_SUBSTITUTES: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'INSERT INTO STOCK_SUBSTITUTES '
      '('
      ' '
      'STOCK_KEY,'
      ' SUBSTITUTE_STOCK_KEY)'
      ' VALUES ('
      ' '
      ':STOCK_KEY, '
      ':SUBSTITUTE_STOCK_KEY'
      ')')
    Left = 640
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateSTOCK_SUBSTITUTES: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'Update STOCK_SUBSTITUTES'
      'Set SUBSTITUTE_KEY = :SUBSTITUTE_KEY,'#9' '
      'STOCK_KEY = :STOCK_KEY, '
      'SUBSTITUTE_STOCK_KEY = :SUBSTITUTE_STOCK_KEY '
      'Where  SUBSTITUTE_KEY= :SUBSTITUTE_KEY')
    Left = 600
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrpopulateSubstitute: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      
        'SELECT   SUBSTITUTE_COND1,  SUBSTITUTE_COND2,  SUBSTITUTE_COND3,' +
        '  TIME_STAMP,  SELECTABLE '
      
        'FROM STOCK_SUBSTITUTE_PERMISSIONS  where STOCK_SUBSTITUTE_PERMIS' +
        'SIONS.SUBSTITUTE_KEY=:SUBSTITUTE_KEY ')
    Left = 560
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrSetMinLevel: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'Update STOCK'
      'Set'
      '    REVERT_TO_ORIGINAL = :REVERT_TO_ORIGINAL '
      ' Where'
      '        STOCK_KEY = :STOCK_KEY')
    Left = 520
    Top = 400
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REVERT_TO_ORIGINAL'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockSubstitute: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   STOCK.STOCK_KEY,'
      '   STOCK.CODE,'#9#9#9#9
      '   STOCK.DESCRIPTION,'
      '   STOCK.UNIT,'
      '   StockGroup.Stock_Group '
      'FROM '
      
        '   STOCK INNER JOIN StockGroup ON STOCK.Stock_Group_Key = StockG' +
        'roup.Stock_Group_Key'
      'WHERE '
      '   (Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And'
      '   Stock.Enabled = '#39'T'#39' And'
      '   UPPER(DESCRIPTION) LIKE :DESCRIPTION '
      'ORDER BY '
      '   StockGroup.Sort_Order ASC')
    Left = 824
    Top = 288
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'DESCRIPTION'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStockPermission: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'Update STOCK_SUBSTITUTE_PERMISSIONS'
      'Set SELECTABLE = :SELECTABLE '#9' '
      'Where  SUBSTITUTE_KEY= :SUBSTITUTE_KEY')
    Left = 584
    Top = 456
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SELECTABLE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrSubstitutePersmissionID: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'select 1 as IdExist from rdb$database'
      'where exists (SELECT     SUBSTITUTE_KEY'
      
        '                        FROM         STOCK_SUBSTITUTE_PERMISSION' +
        'S'
      '                        WHERE      '
      ' SUBSTITUTE_KEY=:SUBSTITUTE_KEY )'
      'UNION'
      'select 0 as IdExist from rdb$database'
      'where NOT exists (SELECT     SUBSTITUTE_KEY'
      
        '                        FROM         STOCK_SUBSTITUTE_PERMISSION' +
        'S'
      
        '                        WHERE       SUBSTITUTE_KEY=:SUBSTITUTE_K' +
        'EY  )'
      '                        ')
    Left = 680
    Top = 440
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrupdateSubstituteSetting: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockEdit
    SQL.Strings = (
      'Update STOCK_SUBSTITUTE_PERMISSIONS'
      'Set SUBSTITUTE_KEY = :SUBSTITUTE_KEY,'#9' '
      'SUBSTITUTE_COND1 = :SUBSTITUTE_COND1, '
      'SUBSTITUTE_COND2 = :SUBSTITUTE_COND2 ,'
      'SUBSTITUTE_COND3 = :SUBSTITUTE_COND3 ,'
      'TIME_STAMP = :TIME_STAMP ,'
      'SELECTABLE = :SELECTABLE '
      'Where  SUBSTITUTE_KEY= :SUBSTITUTE_KEY')
    Left = 728
    Top = 440
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND1'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND2'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_COND3'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'TIME_STAMP'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SELECTABLE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUBSTITUTE_KEY'
        ParamType = ptUnknown
      end>
  end
end
