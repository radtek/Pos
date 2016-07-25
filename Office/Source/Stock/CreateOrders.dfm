object frmCreateOrders: TfrmCreateOrders
  Left = 437
  Top = 130
  Width = 769
  Height = 478
  Caption = 'Create Orders'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  DesignSize = (
    761
    451)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 745
    Height = 434
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      745
      434)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 735
      Height = 381
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        735
        381)
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 733
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 733
          Height = 48
          Align = alClient
          Alignment = taCenter
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
      object btnAuto: TBitBtn
        Left = 216
        Top = 337
        Width = 97
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'A&uto'
        TabOrder = 1
        OnClick = btnAutoClick
      end
      object vtvStockQty: TVirtualStringTree
        Left = 8
        Top = 56
        Width = 721
        Height = 274
        Anchors = [akLeft, akTop, akBottom]
        Color = clInfoBk
        Header.AutoSizeIndex = 0
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        TabOrder = 2
        TreeOptions.MiscOptions = [toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvStockQtyAfterPaint
        OnBeforePaint = vtvStockQtyBeforePaint
        OnCreateEditor = vtvStockQtyCreateEditor
        OnEdited = vtvStockQtyEdited
        OnEditing = vtvStockQtyEditing
        OnFocusChanged = vtvStockQtyFocusChanged
        OnFocusChanging = vtvStockQtyFocusChanging
        OnGetText = vtvStockQtyGetText
        OnGetImageIndex = vtvStockQtyGetImageIndex
        OnKeyDown = vtvStockQtyKeyDown
        Columns = <
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 244
            WideText = 'Description'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 150
            WideText = 'Location'
          end
          item
            Position = 2
            Width = 160
            WideText = 'Supplier'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 103
            WideText = 'Unit'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            Width = 60
            WideText = 'Quantity'
          end>
        WideDefaultText = ''
      end
      object neStockQty: TNumericEdit
        Left = 665
        Top = 73
        Width = 63
        Height = 21
        Value = 1.234567
        Precision = 15
        TabOrder = 3
        Visible = False
        OnKeyDown = neStockQtyKeyDown
        DecimalPlaces = 4
      end
      object btnAdd: TBitBtn
        Left = 8
        Top = 337
        Width = 97
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Add Item'
        TabOrder = 4
        OnClick = btnAddClick
        NumGlyphs = 2
      end
      object btnRemove: TBitBtn
        Left = 112
        Top = 337
        Width = 97
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Remove Item'
        TabOrder = 5
        OnClick = btnRemoveClick
        NumGlyphs = 2
      end
    end
    object btnCancel: TBitBtn
      Left = 640
      Top = 393
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Cancel'
      TabOrder = 1
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
    object btnCommit: TBitBtn
      Left = 480
      Top = 393
      Width = 153
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit To New Orders'
      TabOrder = 2
      OnClick = btnCommitClick
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
    object Button2: TButton
      Left = 8
      Top = 393
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Setup'
      TabOrder = 3
      OnClick = Button2Click
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 152
    Top = 96
  end
  object FindDialog: TFindDialog
    Left = 248
    Top = 392
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock_Key,'
      '   Supplier_Code,'
      '   Latest_Cost,'
      '   Qty,'
      '   Supplier_Unit,'
      '   Default_Order_Unit'
      'From'
      '   SupplierStock'
      'Where'
      '   Supplier_Key = :Supplier_Key And'
      '   Stock_Key = :Stock_Key'
      '')
    Left = 192
    Top = 96
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
  object qrGetStockInfo: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
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
      '   Stock.Code = :Code'
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
    Left = 32
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact.Company_Name,'
      '   Supplier_Stock_Key,'
      '   Supplier_Code,'
      '   Latest_Cost,'
      '   Supplier_Unit,'
      '   Qty AS Supplier_Unit_Size,'
      '   Default_Order_Unit'
      'From'
      '   SupplierStock Inner Join Contact On'
      '      SupplierStock.Supplier_Key = Contact.Contact_LK'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key And'
      '   SupplierStock.Supplier_Key = :Supplier_Key')
    Left = 120
    Top = 96
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
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   Contact.Company_Name,'
      '   Contact.Weborder_target_uri,'
      '   Contact.Contact_LK,'
      '   SupplierStock.Preferred_Supplier'
      'From'
      '   Contact Inner Join SupplierStock On'
      '      Contact.Contact_LK = SupplierStock.Supplier_Key'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key And '
      '   CONTACT.CONTACT_LK = :Supplier_Key and  '
      '   Preferred_Supplier is not null'
      'Order By'
      '   Contact.Company_Name')
    Left = 80
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateRequestStatus: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'UPDATE  STOCKREQUEST_STOCK a'
      'SET a.ISPROCESSED = '#39'T'#39
      'WHERE a.STOCK_REQUEST_STOCKKEY = :STOCK_REQUEST_STOCKKEY')
    Left = 296
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrSuppliedStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT DISTINCT'
      '   STOCK_KEY'
      'FROM'
      '   SUPPLIERSTOCK'
      'WHERE'
      '   SUPPLIER_KEY =  :SUPPLIER_KEY And'
      '   PREFERRED_SUPPLIER = '#39'T'#39)
    Left = 464
    Top = 240
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierStockAuto: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '      SUPPLIERSTOCK.STOCK_KEY,'
      '   SUPPLIERSTOCK.SUPPLIER_CODE,'
      '   SUPPLIERSTOCK.LATEST_COST,'
      '   SUPPLIERSTOCK.QTY,'
      '   SUPPLIERSTOCK.Min_Order_Qty,'
      '   SUPPLIERSTOCK.SUPPLIER_UNIT,'
      '   SUPPLIERSTOCK.DEFAULT_ORDER_UNIT'
      'FROM'
      '   SUPPLIERSTOCK'
      ''
      ' left join STOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STOCK_KEY'
      
        '       left join StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.' +
        'STOCK_GROUP_KEY'
      
        '   Left Join StockCategory On StockCategory.Stock_Category_Key =' +
        ' StockGroup.Stock_Category_Key'
      ''
      'WHERE'
      '  SUPPLIERSTOCK.STOCK_KEY= :STOCK_KEY'
      ''
      'order by'
      '  StockCategory.SORT_ORDER,'
      '  STOCKGROUP.SORT_ORDER,'
      '  STOCK.DESCRIPTION')
    Left = 264
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrOrderedStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Sum(Supplier_Unit_Qty * Supplier_Unit_Size) Qty'
      'From'
      '   PurchaseStock Left Join PurchaseOrder on'
      '      PurchaseStock.Order_FK = PurchaseOrder.Order_LK'
      'Where'
      '   PurchaseOrder.Posted = '#39'F'#39'  And'
      '   Code = :Code And'
      '   Location = :Location')
    Left = 504
    Top = 272
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CODE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
  object qrStockSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      '    SS.SUPPLIER_KEY,'
      '    C.COMPANY_NAME,'
      '    SS.SUPPLIER_CODE,'
      '    SS.SUPPLIER_UNIT,'
      '    SS.QTY AS SUPPLIER_UNIT_SIZE,'
      '    SS.PREFERRED_SUPPLIER,'
      '    SS.LATEST_COST,'
      '    MIN_COST.COST'
      'FROM SUPPLIERSTOCK SS'
      'INNER JOIN CONTACT C ON SS.SUPPLIER_KEY = C.CONTACT_LK'
      
        'INNER JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST / SS' +
        'T.QTY) AS COST FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = :Sto' +
        'ck_Key) MIN_COST ON SS.SUPPLIER_STOCK_KEY = MIN_COST.SUPPLIER_ST' +
        'OCK_KEY'
      
        'WHERE SS.STOCK_KEY = :Stock_Key AND SS.PREFERRED_SUPPLIER IS NOT' +
        ' NULL'
      'ORDER BY COST'
      '')
    Left = 328
    Top = 96
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
  object qrStockByLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '    STOCK.STOCK_KEY,'
      '    STOCK.CODE,    '
      '    STOCK.DESCRIPTION,'
      '    STOCK.STOCKTAKE_UNIT,'
      '    STOCK.GL_CODE,'
      '    STOCK.GST_PERCENT,'
      '    STOCKGROUP.STOCK_GROUP,'
      '    STOCKCATEGORY.STOCK_CATEGORY,'
      '    STOCKLOCATION.LOCATION,'
      '   STOCKLOCATION.LATEST_COST,'
      '    STOCKCATEGORY.STOCK_CATEGORY_KEY,'
      '    STOCK.STOCK_GROUP_KEY,'
      ' STOCKLOCATION.ON_HAND'
      'FROM '
      '    STOCK '
      
        '    INNER JOIN STOCKGROUP ON STOCK.STOCK_GROUP_KEY = STOCKGROUP.' +
        'STOCK_GROUP_KEY'
      
        '    INNER JOIN STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.' +
        'STOCK_KEY '
      
        '    INNER JOIN STOCKCATEGORY on STOCKGROUP.STOCK_CATEGORY_KEY = ' +
        'STOCKCATEGORY.STOCK_CATEGORY_KEY'
      'WHERE '
      
        '    STOCK.DELETED = '#39'F'#39' AND STOCK.ENABLED = '#39'T'#39' AND STOCKLOCATIO' +
        'N.LOCATION = :Location'
      'and STOCKCATEGORY.STOCK_CATEGORY=:STOCK_CATEGORY'
      'ORDER BY '
      
        '    STOCKCATEGORY.SORT_ORDER ASC, STOCKGROUP.SORT_ORDER ASC, STO' +
        'CK.DESCRIPTION ASC')
    Left = 232
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_CATEGORY'
        ParamType = ptUnknown
      end>
  end
  object qrStockSupplierAuto: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select '#9#9#9
      '            Stock.Code, '
      '            Stock.Stocktake_Unit ,'
      '            Stock.Description, '
      '            StockLocation.Location, '
      '            StockLocation.On_Hand, '
      
        '            Cast(Sum(coalesce(PurchaseStock.Qty_On_Order,0) * co' +
        'alesce( PurchaseStock.Supplier_Unit_Size,0)) As Numeric(15,4)) O' +
        'n_Order,'
      '            StockLocation.Average_Cost, '
      '            StockLocation.Latest_Cost, '
      '            StockLocation.Min_Level, '
      
        '            StockLocation.Max_Level ,coalesce(SUPPLIERSTOCK.QTY,' +
        '0) Qty ,'
      '            SUPPLIERSTOCK.SUPPLIER_KEY,'
      '            C.COMPANY_NAME,'
      '            SUPPLIERSTOCK.SUPPLIER_CODE,'
      '            SUPPLIERSTOCK.SUPPLIER_UNIT,'
      '            SUPPLIERSTOCK.QTY,'
      '            SUPPLIERSTOCK.QTY AS SUPPLIER_UNIT_SIZE,'
      '            SUPPLIERSTOCK.PREFERRED_SUPPLIER,'
      '            SUPPLIERSTOCK.LATEST_COST  , MIN_COST.COST'
      #9#9'From '
      #9#9#9'Stock Inner Join StockLocation on '
      #9#9#9#9'Stock.Stock_Key = StockLocation.Stock_Key '
      #9#9#9' '#9' '#9#9'Left Join PurchaseStock On '
      
        #9#9#9#9'PurchaseStock.Code = Stock.Code And PurchaseStock.Location =' +
        ' StockLocation.Location '
      
        #9#9#9#9'Inner join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=Stock.ST' +
        'OCK_KEY'
      
        #9#9#9#9'Inner JOIN CONTACT C ON SUPPLIERSTOCK.SUPPLIER_KEY = C.CONTA' +
        'CT_LK '
      
        #9#9#9#9'Inner JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST ' +
        '/ SST.QTY) AS COST FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = ' +
        ':STOCK_KEY ) MIN_COST ON SUPPLIERSTOCK.SUPPLIER_STOCK_KEY = MIN_' +
        'COST.SUPPLIER_STOCK_KEY'
      
        'WHERE   SUPPLIERSTOCK.STOCK_KEY = :STOCK_KEY  and STOCKLOCATION.' +
        'LOCATION=:LOCATION and SUPPLIERSTOCK.PREFERRED_SUPPLIER IS NOT N' +
        'ULL'
      ' Group By '
      #9#9#9' '#9' STOCK.STOCK_KEY,SUPPLIERSTOCK.QTY, C.COMPANY_NAME,'
      #9#9' '#9'Stock.Code, '
      #9#9#9'Stock.Stocktake_Unit, '
      #9#9#9'Stock.Description, '
      #9#9#9'StockLocation.Location, '
      #9#9#9'StockLocation.On_Hand, '
      #9#9#9'StockLocation.Average_Cost, '
      #9#9#9'StockLocation.Latest_Cost, '
      #9#9#9'StockLocation.Min_Level, '
      #9#9#9'StockLocation.Max_Level , C.COMPANY_NAME,'
      '    SUPPLIERSTOCK.SUPPLIER_CODE,'
      '    SUPPLIERSTOCK.SUPPLIER_UNIT,'
      '    SUPPLIERSTOCK.QTY,'
      '    SUPPLIERSTOCK.QTY  ,'
      '    SUPPLIERSTOCK.PREFERRED_SUPPLIER,'
      
        '    SUPPLIERSTOCK.LATEST_COST,SUPPLIERSTOCK.SUPPLIER_KEY , MIN_C' +
        'OST.COST'
      #9#9' '
      #9#9' ')
    Left = 384
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
  object qrStockPrefferedSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      '    SS.SUPPLIER_KEY,'
      '    C.COMPANY_NAME,'
      '    SS.SUPPLIER_CODE,'
      '    SS.SUPPLIER_UNIT,'
      '    SS.QTY AS SUPPLIER_UNIT_SIZE,'
      '    SS.PREFERRED_SUPPLIER,'
      '    SS.LATEST_COST,'
      'ss.SUPPLIER_STOCK_KEY,'
      'SS.DEFAULT_ORDER_UNIT,'
      '    MIN_COST.COST'
      'FROM SUPPLIERSTOCK SS'
      'INNER JOIN CONTACT C ON SS.SUPPLIER_KEY = C.CONTACT_LK'
      
        'INNER JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST / SS' +
        'T.QTY) AS COST FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = :Sto' +
        'ck_Key) MIN_COST ON SS.SUPPLIER_STOCK_KEY = MIN_COST.SUPPLIER_ST' +
        'OCK_KEY'
      'WHERE SS.STOCK_KEY = :Stock_Key AND SS.PREFERRED_SUPPLIER = '#39'T'#39
      'ORDER BY DEFAULT_ORDER_UNIT, COST '
      '')
    Left = 488
    Top = 136
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
  object qrStockSupplierSec: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      '    SS.SUPPLIER_KEY,'
      '    C.COMPANY_NAME,'
      '    SS.SUPPLIER_CODE,'
      '    SS.SUPPLIER_UNIT,'
      '    SS.QTY AS SUPPLIER_UNIT_SIZE,'
      '    SS.PREFERRED_SUPPLIER,'
      '    SS.LATEST_COST,'
      'ss.SUPPLIER_STOCK_KEY,'
      'SS.DEFAULT_ORDER_UNIT,'
      '    MIN_COST.COST'
      'FROM SUPPLIERSTOCK SS'
      'INNER JOIN CONTACT C ON SS.SUPPLIER_KEY = C.CONTACT_LK'
      
        'INNER JOIN (SELECT SST.SUPPLIER_STOCK_KEY, (SST.LATEST_COST / SS' +
        'T.QTY) AS COST FROM SUPPLIERSTOCK SST WHERE SST.STOCK_KEY = :Sto' +
        'ck_Key) MIN_COST ON SS.SUPPLIER_STOCK_KEY = MIN_COST.SUPPLIER_ST' +
        'OCK_KEY'
      
        'WHERE SS.STOCK_KEY = :Stock_Key AND SS.PREFERRED_SUPPLIER IS NOT' +
        ' NULL'
      'ORDER BY COST '
      '')
    Left = 520
    Top = 136
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
  object qrStockByLocationCat: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '    STOCK.STOCK_KEY,'
      '    STOCK.CODE,    '
      '    STOCK.DESCRIPTION,'
      '    STOCK.STOCKTAKE_UNIT,'
      '    STOCK.GL_CODE,'
      '    STOCK.GST_PERCENT,'
      '    STOCKGROUP.STOCK_GROUP,'
      '    STOCKCATEGORY.STOCK_CATEGORY,'
      '    STOCKLOCATION.LOCATION,'
      '   STOCKLOCATION.LATEST_COST,'
      '    STOCKCATEGORY.STOCK_CATEGORY_KEY,'
      '    STOCK.STOCK_GROUP_KEY,'
      ' STOCKLOCATION.ON_HAND'
      'FROM '
      '    STOCK '
      
        '    INNER JOIN STOCKGROUP ON STOCK.STOCK_GROUP_KEY = STOCKGROUP.' +
        'STOCK_GROUP_KEY'
      
        '    INNER JOIN STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.' +
        'STOCK_KEY '
      
        '    INNER JOIN STOCKCATEGORY on STOCKGROUP.STOCK_CATEGORY_KEY = ' +
        'STOCKCATEGORY.STOCK_CATEGORY_KEY'
      'WHERE '
      
        '    STOCK.DELETED = '#39'F'#39' AND STOCK.ENABLED = '#39'T'#39' AND STOCKLOCATIO' +
        'N.LOCATION = :Location'
      ''
      'ORDER BY '
      
        '    STOCKCATEGORY.SORT_ORDER ASC, STOCKGROUP.SORT_ORDER ASC, STO' +
        'CK.DESCRIPTION ASC')
    Left = 32
    Top = 144
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object qrLocationSetting: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'SELECT  a.CATEGORY_TYPE'
      'FROM LOCATIONS a'
      'where a.NAME=:NAME')
    Left = 88
    Top = 144
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'NAME'
        ParamType = ptUnknown
      end>
  end
  object TransactionMM: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 128
    Top = 144
  end
end
