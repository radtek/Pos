object frmReceiveStockItem: TfrmReceiveStockItem
  Tag = 1024
  Left = 428
  Top = 75
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Select Stock Item'
  ClientHeight = 489
  ClientWidth = 428
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 409
    Height = 473
    BorderWidth = 4
    TabOrder = 0
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 399
      Height = 420
      Align = alTop
      BevelOuter = bvLowered
      TabOrder = 0
      object Label17: TLabel
        Left = 16
        Top = 56
        Width = 66
        Height = 13
        Caption = 'Stock Code:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeStockCode: TLabel
        Left = 96
        Top = 56
        Width = 65
        Height = 13
        Caption = 'lbeStockCode'
      end
      object Label4: TLabel
        Left = 176
        Top = 56
        Width = 87
        Height = 13
        Caption = 'Stocktake Unit:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeStocktakeUnit: TLabel
        Left = 272
        Top = 56
        Width = 80
        Height = 13
        Caption = 'lbeStocktakeUnit'
      end
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 397
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 16
          Top = 0
          Width = 401
          Height = 49
          AutoSize = False
          Caption = 'lbeTitle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object sgLocations: TStringGrid
        Left = 16
        Top = 72
        Width = 369
        Height = 105
        Color = clInfoBk
        DefaultColWidth = 72
        DefaultRowHeight = 18
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goRangeSelect, goColSizing, goRowMoving, goRowSelect]
        TabOrder = 1
        OnClick = sgLocationsClick
        OnDrawCell = sgLocationsDrawCell
        OnKeyPress = sgLocationsKeyPress
      end
      object GroupBox4: TGroupBox
        Left = 16
        Top = 184
        Width = 369
        Height = 225
        Caption = 'Receipt Details'
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 2
        object lbeCost1: TLabel
          Left = 128
          Top = 144
          Width = 50
          Height = 13
          Caption = 'Excl. TAX'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label7: TLabel
          Left = 16
          Top = 108
          Width = 42
          Height = 13
          Caption = 'Quantity'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtQtyOrderUnit: TDBText
          Left = 216
          Top = 108
          Width = 137
          Height = 16
          DataField = 'SUPPLIER_UNIT'
          DataSource = dsSupplierStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label3: TLabel
          Left = 16
          Top = 80
          Width = 62
          Height = 13
          Caption = 'Supplied Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 16
          Top = 152
          Width = 41
          Height = 13
          Caption = 'Cost per'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object dbtCostOrderUnit: TDBText
          Left = 16
          Top = 168
          Width = 101
          Height = 16
          DataField = 'SUPPLIER_UNIT'
          DataSource = dsSupplierStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeCost2: TLabel
          Left = 216
          Top = 144
          Width = 46
          Height = 13
          Caption = 'Inc. TAX'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label16: TLabel
          Left = 16
          Top = 192
          Width = 47
          Height = 13
          Caption = 'Total cost'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 24
          Width = 40
          Height = 13
          Caption = 'Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 16
          Top = 52
          Width = 38
          Height = 13
          Caption = 'Supplier'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object btnAddSupplierUnit: TBitBtn
          Left = 288
          Top = 74
          Width = 65
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btnAddSupplierUnitClick
        end
        object dbluSupplierUnit: TDBLookupComboBox
          Left = 128
          Top = 76
          Width = 153
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          KeyField = 'SUPPLIER_UNIT'
          ListField = 'SUPPLIER_UNIT'
          ListSource = dsSupplierStock
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 4
          OnCloseUp = dbluSupplierUnitCloseUp
        end
        object neCost1: TNumericEdit
          Left = 128
          Top = 160
          Width = 65
          Height = 21
          Precision = 15
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 7
          OnChange = neCostChange
          DecimalPlaces = 4
        end
        object neCost2: TNumericEdit
          Left = 216
          Top = 160
          Width = 65
          Height = 21
          Precision = 15
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 8
          OnChange = neCostChange
          DecimalPlaces = 4
        end
        object dbluLocation: TDBLookupComboBox
          Left = 128
          Top = 20
          Width = 153
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          KeyField = 'LOCATION'
          ListField = 'LOCATION'
          ListSource = dsStock
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 0
          OnCloseUp = dbluLocationCloseUp
        end
        object btnAddLocation: TBitBtn
          Left = 288
          Top = 18
          Width = 65
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnAddLocationClick
        end
        object neCost3: TNumericEdit
          Left = 128
          Top = 188
          Width = 65
          Height = 21
          Precision = 15
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 9
          OnChange = neCostChange
          DecimalPlaces = 4
        end
        object neCost4: TNumericEdit
          Left = 216
          Top = 188
          Width = 65
          Height = 21
          Precision = 15
          NumericType = ntPercent
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 10
          OnChange = neCostChange
          DecimalPlaces = 4
        end
        object neQty: TNumericEdit
          Left = 128
          Top = 104
          Width = 65
          Height = 21
          Alignment = taLeftJustify
          Precision = 15
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 6
          OnChange = neQtyChange
          DecimalPlaces = 4
        end
        object dbluSupplier: TDBLookupComboBox
          Left = 128
          Top = 48
          Width = 153
          Height = 21
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          KeyField = 'CONTACT_LK'
          ListField = 'COMPANY_NAME'
          ListSource = dsSupplier
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          OnCloseUp = dbluSupplierCloseUp
        end
        object btnAddSupplier: TBitBtn
          Left = 288
          Top = 46
          Width = 65
          Height = 25
          Caption = 'Add'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btnAddSupplierUnitClick
        end
      end
    end
    object btnOk: TBitBtn
      Left = 232
      Top = 432
      Width = 81
      Height = 33
      Caption = 'OK'
      Default = True
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
      Left = 320
      Top = 432
      Width = 81
      Height = 33
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 2
      OnClick = btnCancelClick
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        333333333333333333333333000033338833333333333333333F333333333333
        0000333911833333983333333388F333333F3333000033391118333911833333
        38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
        911118111118333338F3338F833338F3000033333911111111833333338F3338
        3333F8330000333333911111183333333338F333333F83330000333333311111
        8333333333338F3333383333000033333339111183333333333338F333833333
        00003333339111118333333333333833338F3333000033333911181118333333
        33338333338F333300003333911183911183333333383338F338F33300003333
        9118333911183333338F33838F338F33000033333913333391113333338FF833
        38F338F300003333333333333919333333388333338FFF830000333333333333
        3333333333333333333888330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
    end
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrStockAfterOpen
    AfterScroll = qrStockAfterScroll
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.GST_Percent,'
      '   StockGroup.Stock_Group,'
      '   StockCategory.Stock_Category,'
      '   Stock.Stock_Group_Key,'
      '   Stock.Description,'
      '   StockCategory.GL_Code,'
      '   StockLocation.Location,'
      '   Stock.Stocktake_Unit,'
      '   StockLocation.On_Hand,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15, 4)) On_Order,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Default_Location,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time'
      'From'
      '   Stock inner join StockLocation on'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join Stocktake On'
      '      StockLocation.Stocktake_Key = Stocktake.Stocktake_Key'
      '   Inner Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Inner Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      '   Left Join PurchaseStock On'
      
        '      (PurchaseStock.Code = Stock.Code And PurchaseStock.Locatio' +
        'n = StockLocation.Location)'
      'Where'
      '   Stock.Code = :Code '
      'and StockLocation.LOCATION LIKE :LOCATION'
      'Group By'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.GST_Percent,'
      '   StockGroup.Stock_Group,'
      '   StockCategory.Stock_Category,'
      '   Stock.Stock_Group_Key,'
      '   Stock.Description,'
      '   StockCategory.GL_Code,'
      '   StockLocation.Location,'
      '   Stock.Stocktake_Unit,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Default_Location,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time'
      'Order By'
      '   Location')
    Left = 40
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
  object dsStock: TDataSource
    AutoEdit = False
    DataSet = qrStock
    Left = 80
    Top = 128
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 192
    Top = 16
  end
  object qrSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrSupplierStockAfterScroll
    SQL.Strings = (
      'Select'
      '   Contact.Company_Name,'
      '   Supplier_Stock_Key,'
      '   Supplier_Code,'
      '   Latest_Cost,'
      '   Supplier_Unit,'
      '   Qty,'
      '   Default_Order_Unit'
      'From'
      '   SupplierStock Inner Join Contact On'
      '      SupplierStock.Supplier_Key = Contact.Contact_LK'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key And'
      '   SupplierStock.Supplier_Key = :Supplier_Key')
    Left = 272
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object dsSupplierStock: TDataSource
    AutoEdit = False
    DataSet = qrSupplierStock
    Left = 352
    Top = 128
  end
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrSupplierAfterScroll
    SQL.Strings = (
      'SELECT DISTINCT'
      'SUPPLIERSTOCK.SUPPLIER_KEY,'
      'Contact.Company_Name,'
      'Contact.Weborder_target_uri,'
      'Contact.Contact_LK,'
      'SupplierStock.Preferred_Supplier'
      'FROM CONTACT '
      
        'INNER JOIN SupplierStock ON Contact.Contact_LK = SupplierStock.S' +
        'upplier_Key'
      
        'WHERE SupplierStock.Stock_Key = :Stock_Key AND Preferred_Supplie' +
        'r IS NOT NULL'
      'ORDER BY Contact.Company_Name')
    Left = 144
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object dsSupplier: TDataSource
    AutoEdit = False
    DataSet = qrSupplier
    Left = 200
    Top = 128
  end
  object qrCheapestSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT FIRST 1'
      'SS.SUPPLIER_KEY,'
      'CONTACT.WEBORDER_TARGET_URI,'
      'Contact.Company_Name,'
      'Contact.Contact_LK'
      'FROM SUPPLIERSTOCK SS'
      
        'INNER JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST / SS' +
        'T.QTY) AS COST '
      
        'FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = :Stock_Key) MIN_COS' +
        'T '
      'ON SS.SUPPLIER_STOCK_KEY = MIN_COST.SUPPLIER_STOCK_KEY'
      'inner join CONTACT on SS.SUPPLIER_KEY = CONTACT.CONTACT_LK'
      
        'WHERE SS.STOCK_KEY = :Stock_Key AND SS.PREFERRED_SUPPLIER IS NOT' +
        ' NULL'
      'ORDER BY COST')
    Left = 240
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierSelection: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT FIRST 1'
      'SS.SUPPLIER_KEY,'
      'CONTACT.WEBORDER_TARGET_URI,'
      'Contact.Company_Name,'
      'Contact.Contact_LK'
      'FROM SUPPLIERSTOCK SS'
      
        'INNER JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST / SS' +
        'T.QTY) AS COST '
      
        'FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = :Stock_Key) MIN_COS' +
        'T '
      'ON SS.SUPPLIER_STOCK_KEY = MIN_COST.SUPPLIER_STOCK_KEY'
      'inner join CONTACT on SS.SUPPLIER_KEY = CONTACT.CONTACT_LK'
      
        'WHERE SS.STOCK_KEY = :Stock_Key AND SS.SUPPLIER_KEY=:SUPPLIER_KE' +
        'Y'
      'ORDER BY COST')
    Left = 288
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
end
