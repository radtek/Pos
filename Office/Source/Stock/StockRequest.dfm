object frmStockRequest: TfrmStockRequest
  Left = 463
  Top = 134
  Width = 785
  Height = 514
  Caption = 'Stock Request'
  Color = clBtnFace
  Constraints.MaxWidth = 785
  Constraints.MinHeight = 486
  Constraints.MinWidth = 755
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
    777
    487)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 766
    Height = 473
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      766
      473)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 756
      Height = 419
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        756
        419)
      object Label1: TLabel
        Left = 20
        Top = 64
        Width = 84
        Height = 13
        Caption = 'Date Required:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 260
        Top = 64
        Width = 51
        Height = 13
        Caption = 'Location:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 460
        Top = 64
        Width = 81
        Height = 13
        Caption = 'Request Type:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 754
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 754
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
      object vtvStockQty: TVirtualStringTree
        Left = 8
        Top = 96
        Width = 737
        Height = 272
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
        TreeOptions.SelectionOptions = [toExtendedFocus]
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
            Width = 70
            WideText = 'Supplier'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 150
            WideText = 'Supplier code'
          end
          item
            Position = 2
            Width = 200
            WideText = 'Description'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 83
            WideText = 'Cost'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            WideText = 'Quantity'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 5
            Width = 60
            WideText = 'Unit'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 6
            Width = 60
            WideText = 'On Hand'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 7
            Width = 60
            WideText = 'Last Price'
          end>
        WideDefaultText = ''
      end
      object neStockQty: TNumericEdit
        Left = 665
        Top = 113
        Width = 63
        Height = 21
        Value = 1.234567
        Precision = 15
        TabOrder = 3
        Visible = False
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neStockQtyKeyPress
        DecimalPlaces = 4
      end
      object edDateRequired: TEdit
        Left = 130
        Top = 60
        Width = 100
        Height = 21
        Color = clInfoBk
        TabOrder = 4
        OnChange = edDateRequiredChange
        OnClick = edDateRequiredClick
      end
      object cbRequestType: TComboBox
        Left = 570
        Top = 60
        Width = 100
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 5
        OnChange = cbRequestTypeChange
        OnClick = cbRequestTypeClick
        Items.Strings = (
          'Transfer'
          'Purchase Order')
      end
      object cmbLocation: TComboBox
        Left = 340
        Top = 60
        Width = 100
        Height = 21
        Style = csDropDownList
        Color = clInfoBk
        ItemHeight = 13
        TabOrder = 1
        OnChange = cmbLocationChange
      end
      object BitBtn1: TBitBtn
        Left = 120
        Top = 381
        Width = 105
        Height = 30
        Anchors = [akLeft, akBottom]
        Caption = 'Add Su&pplier Item'
        TabOrder = 6
        OnClick = BitBtn1Click
        NumGlyphs = 2
      end
      object btnRemove: TBitBtn
        Left = 240
        Top = 381
        Width = 97
        Height = 30
        Anchors = [akLeft, akBottom]
        Caption = '&Remove Item'
        TabOrder = 7
        OnClick = btnRemoveClick
        NumGlyphs = 2
      end
      object btnAuto: TBitBtn
        Left = 352
        Top = 381
        Width = 97
        Height = 30
        Anchors = [akLeft, akBottom]
        Caption = '&Auto'
        TabOrder = 8
        OnClick = btnAutoClick
        NumGlyphs = 2
      end
    end
    object btnCancel: TBitBtn
      Left = 601
      Top = 432
      Width = 160
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
      Left = 433
      Top = 432
      Width = 160
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Commit To Stock Request'
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
      Left = 14
      Top = 432
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Setup'
      TabOrder = 3
      OnClick = Button2Click
    end
    object btnSave: TButton
      Left = 263
      Top = 432
      Width = 160
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Save'
      TabOrder = 4
      OnClick = btnSaveClick
    end
    object btnAdd: TBitBtn
      Left = 24
      Top = 385
      Width = 89
      Height = 30
      Anchors = [akLeft, akBottom]
      Caption = '&Add Any Item'
      TabOrder = 5
      OnClick = btnAddClick
      NumGlyphs = 2
    end
  end
  object FindDialog: TFindDialog
    Left = 120
    Top = 432
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select'
      '   SupplierStock.Stock_Key,SupplierStock.SUPPLIER_STOCK_KEY,'
      '   Supplier_Code,'
      '   Latest_Cost,'
      '   Qty,'
      '   Supplier_Unit,'
      '   Default_Order_Unit,'
      '   STOCK.STOCK_GROUP_KEY,'
      '   StockGroup.Stock_Category_Key'
      'From'
      '   SupplierStock Inner Join Stock on '
      'SupplierStock.Stock_Key = Stock.Stock_Key '
      'Inner Join StockGroup on '
      ' STOCK.STOCK_GROUP_KEY = StockGroup.STOCK_GROUP_KEY '
      
        'Inner Join StockCategory on '#9'StockGroup.Stock_Category_Key = Sto' +
        'ckCategory.Stock_Category_Key '
      #9#9#9
      'Where'
      '   Supplier_Key = :Supplier_Key And'
      '   SupplierStock.Stock_Key = :Stock_Key and'
      'Supplier_Unit=:Supplier_Unit'
      '')
    Left = 24
    Top = 128
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
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end>
  end
  object qrAddStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'INSERT INTO STOCKREQUEST'
      '('
      'REQUEST_NUMBER ,'
      'LOCATION ,'
      'REQUEST_TYPE ,'
      'DATE_GENERATED ,'
      'DATE_REQUIRED ,'
      'ISCOMMITTED ,'
      'ISDELETED )'
      ''
      ' VALUES ('
      ':REQUEST_NUMBER ,'
      ':LOCATION ,'
      ':REQUEST_TYPE ,'
      ':DATE_GENERATED ,'
      ':DATE_REQUIRED ,'
      ':ISCOMMITTED ,'
      ':ISDELETED '
      ')'
      '')
    Left = 120
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_TYPE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_GENERATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISCOMMITTED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISDELETED'
        ParamType = ptUnknown
      end>
  end
  object qrStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      
        'SELECT REQUEST_NUMBER, LOCATION, REQUEST_TYPE, DATE_GENERATED, D' +
        'ATE_REQUIRED, ISCOMMITTED, ISDELETED, RDB$DB_KEY'
      'FROM STOCKREQUEST '
      'Where'
      ' REQUEST_NUMBER= :REQUEST_NUMBER')
    Left = 88
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
  object qrStockReq: TIBSQL
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    Left = 24
    Top = 256
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 64
    Top = 256
  end
  object dtStockReqCount: TIBDataSet
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
      'SELECT max(REQUEST_NUMBER ) REQUEST_NUMBER '
      'FROM STOCKREQUEST ')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 104
    Top = 256
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    Left = 184
    Top = 256
  end
  object qrLocationList: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      #9#9'Select '
      #9#9#9'StockCategory.Stock_Category, '
      #9#9#9'StockGroup.Stock_Group, '
      #9#9#9'STOCKREQUEST.REQUEST_NUMBER , '
      #9#9#9'STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT, '
      #9#9#9'STOCKREQUEST_STOCK.STOCK_REQUEST_UNIT_COST, '
      #9#9#9'STOCKREQUEST_STOCK.QTY_REQUIRED , '
      #9#9#9'STOCKREQUEST.REQUEST_TYPE, '
      #9#9#9'Stock.Code, '
      #9#9#9'Stock.Stocktake_Unit, '
      #9#9#9'Stock.Description, '
      #9#9#9'StockLocation.Location, '
      #9#9#9'StockLocation.On_Hand, '
      
        #9#9#9'cast(StockLocation.On_Hand * StockLocation.Average_Cost as fl' +
        'oat) Average_Cost, '
      
        #9#9#9'cast(StockLocation.On_Hand * StockLocation.Latest_Cost as flo' +
        'at) Latest_Cost, '
      #9#9#9'StockLocation.Average_Cost Single_Average_Cost, '
      #9#9#9'StockLocation.Latest_Cost  Single_Latest_Cost '
      #9#9'From '
      #9#9#9'STOCKREQUEST_STOCK '
      
        #9#9'  '#9'Left Join STOCKREQUEST on STOCKREQUEST.REQUEST_NUMBER  = ST' +
        'OCKREQUEST.REQUEST_NUMBER  '
      
        #9#9#9'Inner Join Stock on STOCKREQUEST_STOCK.STOCK_KEY  = Stock.Sto' +
        'ck_Key '
      #9#9#9'Inner Join StockLocation on '
      #9#9#9'STOCKREQUEST_STOCK.Stock_Key = StockLocation.Stock_Key '
      #9#9#9'Left Join StockGroup on '
      #9#9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key '
      #9#9#9'Left Join StockCategory on '
      
        #9#9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category_' +
        'Key '
      #9#9'Where '
      #9#9#9'Stock.Enabled = '#39'T'#39' And '
      #9#9#9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) '
      #9#9#9'and STOCKREQUEST.REQUEST_NUMBER = :REQUEST_NUMBER '
      
        '         and STOCKREQUEST_STOCK.REQUEST_NUMBER   = :REQUEST_NUMB' +
        'ER '
      #9#9#9'and '
      #9#9#9'StockCategory.Stock_Category = :Stock_Category  '
      #9#9#9'and '
      #9#9#9'StockLocation.Location = :Location '
      'and STOCKREQUEST_STOCK.ISREMOVED='#39'F'#39
      #9#9'Order By '
      #9#9#9'StockCategory.Sort_Order, '
      #9#9#9'StockGroup.Sort_Order, '
      #9#9#9'Stock.Description, '
      #9#9#9'StockLocation.Location  ,'
      
        '                                                 StockGroup.Stoc' +
        'k_Group')
    Left = 24
    Top = 168
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Category'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object qrAddStockRequestStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'INSERT INTO STOCKREQUEST_STOCK'
      '('
      '               STOCK_REQUEST_STOCKKEY, '
      #9'REQUEST_NUMBER, '
      #9'STOCK_KEY, '
      #9'SUPPLIER_KEY, '
      #9'QTY_REQUIRED, '
      #9'STOCK_REQUEST_UNIT, '
      #9'STOCK_REQUEST_UNIT_COST, '
      #9'ISPROCESSED, '
      #9'ISREMOVED, '
      #9'DATE_ENTERED, '
      #9'DATE_UPDATED ,'
      #9'SUPPLIER_STOCK_KEY'
      ')'
      ''
      ' VALUES ('
      ':STOCK_REQUEST_STOCKKEY, '
      ':REQUEST_NUMBER, '
      ':STOCK_KEY, '
      ':SUPPLIER_KEY, '
      ':QTY_REQUIRED, '
      ':STOCK_REQUEST_UNIT, '
      ':STOCK_REQUEST_UNIT_COST, '
      ':ISPROCESSED, '
      ':ISREMOVED, '
      ':DATE_ENTERED, '
      ':DATE_UPDATED ,'
      ':SUPPLIER_STOCK_KEY '
      ')'
      '')
    Left = 152
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
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
        Name = 'QTY_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_UNIT_COST'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISPROCESSED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISREMOVED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_ENTERED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_UPDATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockRequestStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      
        'SELECT STOCK_REQUEST_STOCKKEY, REQUEST_NUMBER, STOCK_KEY, SUPPLI' +
        'ER_KEY, QTY_REQUIRED, STOCK_REQUEST_UNIT, STOCK_REQUEST_UNIT_COS' +
        'T, ISPROCESSED, ISREMOVED, DATE_ENTERED, DATE_UPDATED, RDB$DB_KE' +
        'Y'
      'FROM STOCKREQUEST_STOCK '
      'Where'
      ' REQUEST_NUMBER= :REQUEST_NUMBER and'
      'STOCKREQUEST_STOCK .ISREMOVED='#39'F'#39)
    Left = 56
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
  object dtStockReqStockCount: TIBDataSet
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
      'SELECT max(STOCK_REQUEST_STOCKKEY ) STOCK_REQUEST_STOCKKEY '
      'FROM STOCKREQUEST_STOCK')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 144
    Top = 256
  end
  object qrLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT distinct STOCKLOCATION.LOCATION'
      'FROM STOCKLOCATION '
      'where'
      'STOCKLOCATION.LOCATION !='#39#39)
    Left = 56
    Top = 168
  end
  object qrStockRequestSelect: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT '
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'STOCKREQUEST.LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      'SUPPLIERSTOCK.SUPPLIER_CODE,'
      
        'STOCKLOCATION.ON_HAND,STOCKREQUEST_STOCK.SUPPLIER_STOCK_KEY, STO' +
        'CKREQUEST_STOCK.SUPPLIER_CODE'
      'FROM STOCKREQUEST_STOCK '
      
        'left join STOCKREQUEST on STOCKREQUEST.REQUEST_NUMBER=STOCKREQUE' +
        'ST_STOCK.REQUEST_NUMBER'
      
        'left join CONTACT on CONTACT.CONTACT_LK=STOCKREQUEST_STOCK.SUPPL' +
        'IER_KEY'
      'left join STOCK on STOCK.STOCK_KEY=STOCKREQUEST_STOCK.STOCK_KEY'
      
        'left join SUPPLIERSTOCK on STOCKREQUEST_STOCK.SUPPLIER_KEY = SUP' +
        'PLIERSTOCK.SUPPLIER_KEY and STOCKREQUEST_STOCK.STOCK_KEY = SUPPL' +
        'IERSTOCK.STOCK_KEY '
      
        'left join STOCKLOCATION on STOCKLOCATION.STOCK_KEY=STOCK.STOCK_K' +
        'EY and STOCKLOCATION.LOCATION=STOCKREQUEST.LOCATION'
      #9#9#9#9#9
      'where STOCKREQUEST_STOCK.REQUEST_NUMBER= :REQUEST_NUMBER '
      ' and SUPPLIERSTOCK.SUPPLIER_STOCK_KEY=:SUPPLIER_STOCK_KEY'
      'and STOCKREQUEST_STOCK.ISREMOVED='#39'F'#39
      'group by'
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      
        'SUPPLIERSTOCK.SUPPLIER_CODE,STOCKLOCATION.ON_HAND,STOCKREQUEST_S' +
        'TOCK.SUPPLIER_STOCK_KEY, STOCKREQUEST_STOCK.SUPPLIER_CODE')
    Left = 184
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update STOCKREQUEST'
      'Set'
      'REQUEST_NUMBER = :REQUEST_NUMBER ,'
      'LOCATION  =:LOCATION ,'
      'REQUEST_TYPE =:REQUEST_TYPE , '
      'DATE_GENERATED = :DATE_GENERATED ,'
      'DATE_REQUIRED = :DATE_REQUIRED ,'
      'ISCOMMITTED = :ISCOMMITTED ,'
      'ISDELETED   = :ISDELETED          '
      ''
      'Where'
      ' '
      ' REQUEST_NUMBER = :REQUEST_NUMBER'
      '')
    Left = 256
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_TYPE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_GENERATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISCOMMITTED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISDELETED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
  object IBQuery2: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'INSERT INTO STOCKREQUEST'
      '('
      'REQUEST_NUMBER ,'
      'LOCATION ,'
      'REQUEST_TYPE ,'
      'DATE_GENERATED ,'
      'DATE_REQUIRED ,'
      'ISCOMMITTED ,'
      'ISDELETED )'
      ''
      ' VALUES ('
      ':REQUEST_NUMBER ,'
      ':LOCATION ,'
      ':REQUEST_TYPE ,'
      ':DATE_GENERATED ,'
      ':DATE_REQUIRED ,'
      ':ISCOMMITTED ,'
      ':ISDELETED '
      ')'
      '')
    Left = 224
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_TYPE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_GENERATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISCOMMITTED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISDELETED'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStockRequestStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      ''
      #9'  Update STOCKREQUEST_STOCK'
      'Set'
      '    STOCK_REQUEST_STOCKKEY = :STOCK_REQUEST_STOCKKEY, '
      #9'REQUEST_NUMBER = :REQUEST_NUMBER, '
      #9'STOCK_KEY = :STOCK_KEY, '
      #9'SUPPLIER_KEY = :SUPPLIER_KEY, '
      #9'QTY_REQUIRED = :QTY_REQUIRED, '
      #9'STOCK_REQUEST_UNIT= :STOCK_REQUEST_UNIT, '
      #9'STOCK_REQUEST_UNIT_COST= :STOCK_REQUEST_UNIT_COST, '
      #9'ISPROCESSED = :ISPROCESSED, '
      #9'ISREMOVED = :ISREMOVED, '
      #9'DATE_ENTERED = :DATE_ENTERED, '
      #9'DATE_UPDATED = :DATE_UPDATED ,'
      '               SUPPLIER_STOCK_KEY=:SUPPLIER_STOCK_KEY'
      '              '
      ''
      #9
      #9'Where'
      '        STOCK_REQUEST_STOCKKEY= :STOCK_REQUEST_STOCKKEY')
    Left = 296
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
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
        Name = 'QTY_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_UNIT_COST'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISPROCESSED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISREMOVED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_ENTERED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DATE_UPDATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
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
      ' STOCKLOCATION.ON_HAND,'
      ' STOCKLOCATION.MAX_LEVEL,'
      ' STOCKLOCATION.MIN_LEVEL'
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
      'AND STOCKCATEGORY.STOCK_CATEGORY=:STOCK_CATEGORY'
      'ORDER BY '
      
        '    STOCKCATEGORY.SORT_ORDER ASC, STOCKGROUP.SORT_ORDER ASC, STO' +
        'CK.DESCRIPTION ASC,    STOCKGROUP.STOCK_GROUP')
    Left = 328
    Top = 128
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
    Left = 360
    Top = 128
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
  object qrStockONhand: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select '
      #9#9#9
      #9#9#9' Stock.Code, '
      #9#9#9'Stock.Stocktake_Unit, '
      #9#9#9'Stock.Description, '
      #9#9#9'StockLocation.Location, '
      #9#9#9'StockLocation.On_Hand, '
      
        #9#9#9'Cast(Sum(coalesce(PurchaseStock.Qty_On_Order,0) * coalesce( P' +
        'urchaseStock.Supplier_Unit_Size,0)) As Numeric(15,4)) On_Order,'
      #9#9#9'StockLocation.Average_Cost, '
      #9#9#9'StockLocation.Latest_Cost, '
      #9#9#9'StockLocation.Min_Level, '
      #9#9#9'StockLocation.Max_Level '
      #9#9'From '
      #9#9#9'Stock Inner Join StockLocation on '
      #9#9#9#9'Stock.Stock_Key = StockLocation.Stock_Key '
      #9#9#9' '#9' '#9#9'Left Join PurchaseStock On '
      
        #9#9#9#9'PurchaseStock.Code = Stock.Code And PurchaseStock.Location =' +
        ' StockLocation.Location '
      #9#9#9#9
      #9#9'Where '
      #9#9#9'Stock.Enabled = '#39'T'#39' And '
      #9#9#9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null)  '
      
        'and STOCK.STOCK_KEY=:STOCK_KEY and StockLocation.Location=:Locat' +
        'ion'
      #9#9'  '
      #9' '
      #9#9'Group By '
      #9#9#9' '#9' STOCK.STOCK_KEY,'
      '                            '
      #9#9' '#9'Stock.Code, '
      #9#9#9'Stock.Stocktake_Unit, '
      #9#9#9'Stock.Description, '
      #9#9#9'StockLocation.Location, '
      #9#9#9'StockLocation.On_Hand, '
      #9#9#9'StockLocation.Average_Cost, '
      #9#9#9'StockLocation.Latest_Cost, '
      #9#9#9'StockLocation.Min_Level, '
      #9#9#9'StockLocation.Max_Level '
      #9#9' '
      #9#9' '
      '')
    Left = 392
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object qrStockReqStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.STOCK_REQUEST_STOCKKEY, a.REQUEST_NUMBER, a.STOCK_KEY, ' +
        'a.SUPPLIER_KEY , a.SUPPLIER_STOCK_KEY, a.SUPPLIER_CODE'
      'FROM STOCKREQUEST_STOCK a'
      'where a.REQUEST_NUMBER=:REQUEST_NUMBER'
      ' and a.ISREMOVED='#39'F'#39)
    Left = 424
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
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
  object qrCheckSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      'CONTACT.COMPANY_NAME'
      'FROM STOCK a'
      'left join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=a.STOCK_KEY'
      
        'left join CONTACT on SUPPLIERSTOCK.SUPPLIER_KEY=CONTACT.CONTACT_' +
        'LK'
      'where a.STOCK_KEY=:STOCK_KEY')
    Left = 520
    Top = 136
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStockReqKey: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      '  Update STOCKREQUEST_STOCK'
      'Set'
      '    '
      #9'ISREMOVED = :ISREMOVED'
      #9'Where'
      '        STOCK_REQUEST_STOCKKEY= :STOCK_REQUEST_STOCKKEY'
      '')
    Left = 560
    Top = 136
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ISREMOVED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrLocationSetting: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = TransactionMM
    SQL.Strings = (
      'SELECT  a.CATEGORY_TYPE , a.STOCKREQUESTAUTO_KEY'
      'FROM LOCATIONS a'
      'where a.NAME=:NAME')
    Left = 120
    Top = 168
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
    Left = 224
    Top = 256
  end
  object qrStockLcationWithoutCat: TIBQuery
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
      ' STOCKLOCATION.ON_HAND,'
      ' STOCKLOCATION.MAX_LEVEL,'
      ' STOCKLOCATION.MIN_LEVEL'
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
      ' '
      'ORDER BY '
      
        '    STOCKCATEGORY.SORT_ORDER ASC, STOCKGROUP.SORT_ORDER ASC, STO' +
        'CK.DESCRIPTION ASC,    STOCKGROUP.STOCK_GROUP')
    Left = 600
    Top = 136
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
end
