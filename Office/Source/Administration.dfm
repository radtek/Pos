object frmAdministration: TfrmAdministration
  Left = -51
  Top = 131
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Administration'
  ClientHeight = 688
  ClientWidth = 785
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 393
    Height = 409
    BorderWidth = 3
    TabOrder = 0
    object Panel13: TPanel
      Left = 292
      Top = 4
      Width = 97
      Height = 401
      Align = alRight
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object btnClose: TBitBtn
        Left = 8
        Top = 8
        Width = 81
        Height = 41
        Caption = 'Close'
        TabOrder = 0
        OnClick = btnCloseClick
        NumGlyphs = 2
      end
      object btnShowReceipt: TBitBtn
        Left = 8
        Top = 352
        Width = 81
        Height = 41
        Caption = 'Show Receipt'
        TabOrder = 1
        OnClick = btnShowReceiptClick
      end
      object BitBtn1: TBitBtn
        Left = 8
        Top = 264
        Width = 81
        Height = 41
        Caption = 'Categories'
        TabOrder = 2
        OnClick = BitBtn1Click
      end
      object btnSecurity: TButton
        Left = 8
        Top = 120
        Width = 81
        Height = 41
        Caption = 'Security'
        TabOrder = 3
        OnClick = btnSecurityClick
      end
      object btnUsers: TButton
        Left = 8
        Top = 72
        Width = 81
        Height = 41
        Caption = 'Users'
        TabOrder = 4
        OnClick = btnUsersClick
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 4
      Width = 288
      Height = 401
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object Panel10: TPanel
        Left = 0
        Top = 0
        Width = 285
        Height = 401
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 0
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 283
          Height = 33
          Align = alTop
          BevelOuter = bvNone
          Color = clMaroon
          TabOrder = 0
          object Label3: TLabel
            Left = 0
            Top = 0
            Width = 283
            Height = 33
            Align = alClient
            Alignment = taCenter
            AutoSize = False
            Caption = 'Administration'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clCaptionText
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Layout = tlCenter
            WordWrap = True
          end
        end
        object Panel12: TPanel
          Left = 8
          Top = 40
          Width = 269
          Height = 157
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 1
          object btnClearSuppliers: TBitBtn
            Left = 8
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Clear Suppliers'
            TabOrder = 0
            OnClick = btnClearSuppliersClick
          end
          object btnClearStock: TBitBtn
            Left = 8
            Top = 56
            Width = 121
            Height = 41
            Caption = 'Clear Stock'
            TabOrder = 1
            OnClick = btnClearStockClick
          end
          object btnClearHistory: TBitBtn
            Left = 8
            Top = 104
            Width = 121
            Height = 41
            Caption = 'Clear History'
            TabOrder = 2
            OnClick = btnClearHistoryClick
          end
          object btnImport: TBitBtn
            Left = 136
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Import'
            TabOrder = 3
            OnClick = btnImportClick
          end
          object btnBarcodes: TBitBtn
            Left = 136
            Top = 56
            Width = 121
            Height = 41
            Caption = 'Synchronise Barcodes'
            TabOrder = 4
            OnClick = btnBarcodesClick
          end
          object btnAddStockLocations: TBitBtn
            Left = 136
            Top = 104
            Width = 121
            Height = 41
            Caption = 'Add Stock Locations'
            TabOrder = 5
            OnClick = btnAddStockLocationsClick
          end
        end
        object Panel1: TPanel
          Left = 8
          Top = 200
          Width = 269
          Height = 61
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 2
          object btnCreateDB: TBitBtn
            Left = 8
            Top = 8
            Width = 249
            Height = 41
            Caption = 'Create && Connect New Database'
            TabOrder = 0
            OnClick = btnCreateDBClick
          end
        end
        object Panel3: TPanel
          Left = 8
          Top = 328
          Width = 269
          Height = 65
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 3
          object btnIBLog: TBitBtn
            Left = 8
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Show Interbase Log'
            TabOrder = 0
            OnClick = btnIBLogClick
          end
          object BitBtn6: TBitBtn
            Left = 136
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Watch SQL'
            TabOrder = 1
            OnClick = BitBtn6Click
          end
        end
        object Panel4: TPanel
          Left = 8
          Top = 264
          Width = 269
          Height = 61
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 4
          object btnRepairMM: TBitBtn
            Left = 8
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Repair MenuMate'
            TabOrder = 0
            OnClick = btnRepairMMClick
          end
          object btnRepairStock: TBitBtn
            Left = 136
            Top = 8
            Width = 121
            Height = 41
            Caption = 'Repair Stock'
            TabOrder = 1
            OnClick = btnRepairStockClick
          end
        end
      end
    end
  end
  object memReceipt: TMemo
    Left = 408
    Top = 8
    Width = 369
    Height = 385
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 1
    WordWrap = False
  end
  object ProgressBar1: TProgressBar
    Left = 408
    Top = 400
    Width = 369
    Height = 17
    Min = 0
    Max = 100
    TabOrder = 2
  end
  object dlgOpen: TOpenDialog
    DefaultExt = 'csv'
    Filter = 
      'Comma Separeated Values (*.csv)|*.csv|Text Files (*.txt)|*.txt|A' +
      'll Files (*.*)|*.*'
    Left = 360
    Top = 72
  end
  object dlgSave: TSaveDialog
    DefaultExt = 'csv'
    Filter = 'Comma Separeated Values (*.csv)|*.csv'
    Left = 360
    Top = 120
  end
  object qrAddSupplier: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Contact ('
      '   Contact_LK,'
      '   Contact_Group_Key,'
      '   Company_Name,'
      '   Bad_Debtor,'
      '   Deleted)'
      'Values ('
      '   :Contact_LK,'
      '   :Contact_Group_Key,'
      '   :Company_Name,'
      '   :Bad_Debtor,'
      '   :Deleted)')
    Transaction = Transaction
    Left = 112
    Top = 456
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 40
    Top = 456
  end
  object qrLK: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Contact ('
      '   Contact_LK,'
      '   Company_Name,'
      '   Phone,'
      '   FAX,'
      '   EMail,'
      '   Contact_Name,'
      '   Contact_Phone,'
      '   Location_Address,'
      '   Bill_Address)'
      'Values ('
      '   :Contact_LK,'
      '   :Company_Name,'
      '   :Phone,'
      '   :FAX,'
      '   :EMail,'
      '   :Contact_Name,'
      '   :Contact_Phone,'
      '   :Location_Address,'
      '   :Bill_Address)')
    Transaction = Transaction
    Left = 40
    Top = 504
  end
  object qrAddCategory: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into StockCategory ('
      '   Stock_Category_Key,'
      '   Stock_Category,'
      '   Sort_Order,'
      '   GL_Code,'
      '   Deleted)'
      'Values ('
      '   :Stock_Category_Key,'
      '   :Stock_Category,'
      '   :Sort_Order,'
      '   :GL_Code,'
      '   :Deleted)')
    Transaction = Transaction
    Left = 112
    Top = 504
  end
  object qrFindCategory: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock_Category_Key,'
      '   Stock_Category'
      'From'
      '   StockCategory'
      'Where '
      '   Stock_Category = :Stock_Category;')
    Left = 192
    Top = 504
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Category'
        ParamType = ptUnknown
      end>
  end
  object qrAddGroup: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into StockGroup ('
      '   Stock_Group_Key,'
      '   Stock_Category_Key,'
      '   Stock_Group,'
      '   Sort_Order,'
      '   GL_Code,'
      '   Deleted)'
      'Values ('
      '   :Stock_Group_Key,'
      '   :Stock_Category_Key,'
      '   :Stock_Group,'
      '   :Sort_Order,'
      '   :GL_Code,'
      '   :Deleted)')
    Transaction = Transaction
    Left = 112
    Top = 552
  end
  object qrFindGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock_Group_Key,'
      '   Stock_Group'
      'From'
      '   StockGroup'
      'Where'
      '   Stock_Group = :Stock_Group;')
    Left = 192
    Top = 552
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Group'
        ParamType = ptUnknown
      end>
  end
  object qrAddStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Stock ('
      '   Stock_Key,'
      '   Stock_Group_Key,'
      '   Code,'
      '   Description,'
      '   Stocktake_Unit, '
      '   REDUCTION_UNIT, '
      '   CONVERSION_FACTOR, '
      '   STOCK_SUB_GROUP,  '
      '   GST_Percent,   '
      '    Enabled, '
      '   Deleted) '
      'Values ('
      '   :Stock_Key,'
      '   :Stock_Group_Key,'
      '   :Code,'
      '   :Description, '
      '   :Stocktake_Unit,  '
      '   :REDUCTION_UNIT,'
      '   :CONVERSION_FACTOR, '
      '   :STOCK_SUB_GROUP ,   '
      '   :GST_Percent,  '
      '   :Enabled, '
      '   :Deleted)')
    Transaction = Transaction
    Left = 112
    Top = 600
  end
  object qrAddStockLocation: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into StockLocation ('
      '   Stock_Key,'
      '   Location,'
      '   Max_Level,'
      '   Min_Level,'
      '   On_Hand,'
      '   Average_Cost,'
      '   Latest_Cost,'
      '   Initialised,'
      '   Default_Location)'
      'Values ('
      '   :Stock_Key,'
      '   :Location,'
      '   :Max_Level,'
      '   :Min_Level,'
      '   :On_Hand,'
      '   :Average_Cost,'
      '   :Latest_Cost,'
      '   :Initialised,'
      '   :Default_Location)')
    Transaction = Transaction
    Left = 112
    Top = 648
  end
  object qrFindSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_LK,'
      '   Company_Name'
      'From'
      '   Contact'
      'Where'
      '   Company_Name = :Company_Name'
      '')
    Left = 192
    Top = 456
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Company_Name'
        ParamType = ptUnknown
      end>
  end
  object qrAddStockSupplier: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into SupplierStock ('
      '   Supplier_Stock_Key,'
      '   Supplier_Key,'
      '   Stock_Key,'
      '   Supplier_Unit,'
      '   Supplier_Code,'
      '   Preferred_Supplier,'
      '   Default_Order_Unit,'
      '   Qty,'
      '   Latest_Cost,'
      '   Min_Order_Qty)'
      'Values ('
      '   :Supplier_Stock_Key,'
      '   :Supplier_Key,'
      '   :Stock_Key,'
      '   :Supplier_Unit,'
      '   :Supplier_Code,'
      '   :Preferred_Supplier,'
      '   :Default_Order_Unit,'
      '   :Qty,'
      '   :Latest_Cost,'
      '   :Min_Order_Qty)')
    Transaction = Transaction
    Left = 112
    Top = 696
  end
  object qrClearSuppliers: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From Contact')
    Transaction = Transaction
    Left = 288
    Top = 456
  end
  object qrClearStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From StockCategory')
    Transaction = Transaction
    Left = 288
    Top = 504
  end
  object qrClearTransBatch: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From TransactionBatch')
    Transaction = Transaction
    Left = 288
    Top = 552
  end
  object qrClearImportTrack: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From ImportTrack')
    Transaction = Transaction
    Left = 288
    Top = 600
  end
  object qrClearPurchaseOrders: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From PurchaseOrder')
    Transaction = Transaction
    Left = 288
    Top = 648
  end
  object qrStocktakeProfiles: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From StocktakeProfiles')
    Transaction = Transaction
    Left = 288
    Top = 696
  end
  object qrStockList: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Key'
      'From'
      '   Stock'
      'Order By'
      '   Stock_Key')
    Transaction = Transaction
    Left = 368
    Top = 456
  end
  object qrReceipt: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'Select'
      '   Receipt'
      'From'
      '   ArcBill'
      'Where'
      '  Invoice_Number = :Invoice_Number')
    Left = 416
    Top = 616
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end>
  end
  object IBLogService: TIBLogService
    LoginPrompt = False
    TraceFlags = []
    Left = 544
    Top = 456
  end
  object IBSQLMonitor: TIBSQLMonitor
    OnSQL = IBSQLMonitorSQL
    TraceFlags = [tfQPrepare, tfQExecute, tfQFetch, tfError, tfStmt, tfConnect, tfTransact, tfBlob, tfService, tfMisc]
    Enabled = False
    Left = 544
    Top = 504
  end
  object IBDatabaseInfo: TIBDatabaseInfo
    Left = 544
    Top = 552
  end
  object qrContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_Group_Key,'
      '   Contact_Group'
      'From'
      '      ContactGroup')
    Left = 40
    Top = 552
  end
  object qrAddContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into ContactGroup ('
      '   Contact_Group_Key,'
      '   Contact_Group,'
      '   Sort_Order)'
      'Values ('
      '   :Contact_Group_Key,'
      '   :Contact_Group,'
      '   :Sort_Order)')
    Left = 40
    Top = 600
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_Group_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_Group'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Sort_Order'
        ParamType = ptUnknown
      end>
  end
  object qrLocations: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'Select'
      '   Name'
      'From'
      '   Locations'
      'Order By'
      '   Name')
    Left = 416
    Top = 664
  end
  object qrCategories: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'CategoryGroups.CategoryGroups_Key,'
      #9'CategoryGroups.Name Category_Group_Name,'
      #9'ArcCategories.Category_Key,'
      #9'ArcCategories.Category'
      'From'
      #9'ArcCategories Left Join CategoryGroups On'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Order By'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category')
    Transaction = dmMMData.trMenuMate
    Left = 336
    Top = 208
  end
  object qrClearPurchaseStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From PurchaseStock')
    Transaction = Transaction
    Left = 368
    Top = 512
  end
  object qrClearStocktake: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From Stocktake')
    Transaction = Transaction
    Left = 368
    Top = 560
  end
  object qrAddBarcode: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Barcode ('
      '   Stock_Key,'
      '   Barcode,'
      '   Supplier_Stock_Key)'
      'Values ('
      '   :Stock_Key,'
      '   :Barcode,'
      '   :Supplier_Stock_Key)')
    Transaction = Transaction
    Left = 176
    Top = 608
  end
  object IBQuery: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 488
    Top = 24
  end
  object qrUpdateStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update Stock '
      'Set '
      'Description = :Description ,  '
      'Stocktake_Unit = :Stocktake_Unit,  '
      'REDUCTION_UNIT = :REDUCTION_UNIT,  '
      'STOCK_SUB_GROUP = :STOCK_SUB_GROUP,      '
      'GST_Percent = :GST_Percent , '
      'CONVERSION_FACTOR = :CONVERSION_FACTOR  , '
      'STOCK_GROUP_KEY = :STOCK_GROUP_KEY '
      'Where '
      'Code = :Code ;'
      ''
      '')
    Left = 544
    Top = 24
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Description'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stocktake_Unit'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REDUCTION_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_SUB_GROUP'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'GST_Percent'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CONVERSION_FACTOR'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_GROUP_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end>
  end
  object qrFindStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock_Key'
      'From'
      '   Stock'
      'Where'
      '   Code = :Code ;')
    Left = 192
    Top = 664
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update StockLocation '
      'Set '
      'Max_Level = :Max_Level, '
      'Min_Level = :Min_Level, '
      'Average_Cost = :Average_Cost, '
      'Latest_Cost = :Latest_Cost '
      'Where '
      'Stock_Key = :Stock_Key And '
      'Location = :Location;')
    Left = 600
    Top = 24
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Max_Level'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Min_Level'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Average_Cost'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Latest_Cost'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object IBQuery1: TIBQuery
    Left = 656
    Top = 24
  end
  object qrUpdateStockSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update SupplierStock'
      'Set'
      'Supplier_Code = :Supplier_Code,'
      'Qty = :Qty,'
      'Latest_Cost = :Latest_Cost,'
      'Min_Order_Qty = :Min_Order_Qty'
      'Where'
      'Supplier_Unit = :Supplier_Unit And'
      'Stock_Key = :Stock_Key And'
      'Supplier_Key = :Supplier_Key')
    Left = 488
    Top = 72
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Code'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Qty'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Latest_Cost'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Min_Order_Qty'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end>
  end
  object qrClearBarcodes: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From Barcode')
    Transaction = Transaction
    Left = 368
    Top = 608
  end
  object qrImportSupplier: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Contact ('
      'Contact_LK, '
      'Contact_Group_Key, '
      'Company_Name,'
      'Bad_Debtor, '
      'Deleted, '
      'Contact_Name, '
      'Supplier_ID, '
      'Phone, '
      'Fax, '
      'Mobile, '
      'EMail, '
      'Location_Address, '
      'Weborder_Target_URI,'
      'Bill_Address,  '
      'Note)'
      ' VALUES ('
      ':Contact_LK, '
      ':Contact_Group_Key, '
      ':Company_Name,'
      ':Bad_Debtor, '
      ':Deleted, '
      ':Contact_Name, '
      ':Supplier_ID, '
      ':Phone, '
      ':Fax, '
      ':Mobile, '
      ':EMail, '
      ':Location_Address, '
      ':Weborder_Target_URI,'
      ':Bill_Address,  '
      ':Note'
      ')')
    Transaction = Transaction
    Left = 632
    Top = 488
  end
  object qrUpdateSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      '   Update Contact'
      'Set'
      'Contact_LK = :Contact_LK,'
      'Company_Name = :Company_Name,'
      'Bad_Debtor = :Bad_Debtor,'
      'Deleted = :Deleted,'
      'Contact_Name = :Contact_Name,'
      'Supplier_ID = :Supplier_ID,'
      'Phone = :Phone,'
      'Fax = :Fax,'
      'Mobile = :Mobile,'
      'EMail = :EMail,'
      'Location_Address = :Location_Address,'
      'Weborder_Target_URI = :Weborder_Target_URI,'
      'Bill_Address = :Bill_Address,'
      'Note = :Note'
      'Where'
      'Contact_LK = :Contact_LK ')
    Left = 616
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_LK'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Company_Name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Bad_Debtor'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Deleted'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_Name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_ID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Phone'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Fax'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Mobile'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'EMail'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Weborder_Target_URI'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Bill_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Note'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_LK'
        ParamType = ptUnknown
      end>
  end
  object MMTransaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 488
    Top = 608
  end
  object qrCSV: TIBSQL
    Database = dmMMData.dbMenuMate
    Transaction = MMTransaction
    Left = 696
    Top = 672
  end
  object qrcsv1: TIBSQL
    Database = dmMMData.dbMenuMate
    Transaction = MMTransaction
    Left = 696
    Top = 544
  end
  object qrGetPriceLevelLimit: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'SELECT '
      #9#9'INTEGER_VAL '
      #9'FROM '
      #9#9'VARIABLES'
      #9'WHERE '
      #9#9'VARIABLES_KEY = 5014;')
    Transaction = MMTransaction
    Left = 624
    Top = 672
  end
  object qrUpdateOnImport: TIBSQL
    Database = dmMMData.dbMenuMate
    Transaction = MMTransaction
    Left = 448
    Top = 504
  end
end
