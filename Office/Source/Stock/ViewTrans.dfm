object frmViewTrans: TfrmViewTrans
  Tag = 1024
  Left = 279
  Top = 9
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Stock Transactions'
  ClientHeight = 632
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 632
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object btnPrintClose: TButton
      Left = 680
      Top = 584
      Width = 91
      Height = 33
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnPrintCloseClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 632
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object pnlStock: TPanel
      Left = 8
      Top = 15
      Width = 433
      Height = 242
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Color = 14221311
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 0
      object GroupBox2: TGroupBox
        Left = 16
        Top = 8
        Width = 401
        Height = 193
        Caption = 'Stock Details'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label6: TLabel
          Left = 8
          Top = 72
          Width = 72
          Height = 16
          Caption = 'Stock Code'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label8: TLabel
          Left = 8
          Top = 120
          Width = 25
          Height = 16
          Caption = 'Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object dbtStockCode: TDBText
          Left = 144
          Top = 72
          Width = 249
          Height = 17
          DataField = 'CODE'
          DataSource = dsStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 8
          Top = 144
          Width = 54
          Height = 16
          Caption = 'On Hand'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label11: TLabel
          Left = 8
          Top = 168
          Width = 58
          Height = 16
          Caption = 'On Order'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object DBText2: TDBText
          Left = 8
          Top = 16
          Width = 385
          Height = 49
          Alignment = taCenter
          DataField = 'DESCRIPTION'
          DataSource = dsStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -20
          Font.Name = 'Tahoma'
          Font.Style = [fsUnderline]
          ParentFont = False
        end
        object DBText3: TDBText
          Left = 144
          Top = 120
          Width = 249
          Height = 17
          DataField = 'STOCKTAKE_UNIT'
          DataSource = dsStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText4: TDBText
          Left = 144
          Top = 144
          Width = 249
          Height = 17
          DataField = 'ON_HAND'
          DataSource = dsStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText5: TDBText
          Left = 144
          Top = 168
          Width = 249
          Height = 17
          DataField = 'ON_ORDER'
          DataSource = dsStock
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 8
          Top = 96
          Width = 55
          Height = 16
          Caption = 'Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object dbluLocation: TDBLookupComboBox
          Left = 144
          Top = 92
          Width = 241
          Height = 24
          Color = 15269887
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
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
      end
    end
    object GroupBox3: TGroupBox
      Left = 456
      Top = 8
      Width = 329
      Height = 249
      Caption = 'Transaction'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object Label12: TLabel
        Left = 16
        Top = 120
        Width = 63
        Height = 16
        Caption = 'Quick Find'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 32
        Top = 192
        Width = 100
        Height = 16
        Caption = 'Enter a reference'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Panel4: TPanel
        Left = 16
        Top = 24
        Width = 297
        Height = 97
        BevelOuter = bvNone
        TabOrder = 1
        object Label1: TLabel
          Left = 0
          Top = 1
          Width = 41
          Height = 16
          Caption = 'Range'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object rbThisMonth: TRadioButton
          Left = 16
          Top = 24
          Width = 129
          Height = 17
          Caption = 'Current Month'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          TabStop = True
          OnClick = rbRangeClick
        end
        object rbLastMonth: TRadioButton
          Left = 16
          Top = 48
          Width = 129
          Height = 17
          Caption = 'Last Month'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = rbRangeClick
        end
        object rbAll: TRadioButton
          Left = 16
          Top = 72
          Width = 129
          Height = 17
          Caption = 'All'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = rbRangeClick
        end
      end
      object edFind: TEdit
        Left = 32
        Top = 208
        Width = 265
        Height = 24
        Color = 15269887
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnKeyUp = edFindKeyUp
      end
      object RadioButton1: TRadioButton
        Left = 32
        Top = 144
        Width = 129
        Height = 17
        Caption = 'This stock item'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        TabStop = True
        OnClick = rbRangeClick
      end
      object RadioButton2: TRadioButton
        Left = 32
        Top = 168
        Width = 129
        Height = 17
        Caption = 'All stock'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = rbRangeClick
      end
    end
    object btnPrintAll: TButton
      Left = 568
      Top = 592
      Width = 105
      Height = 33
      Caption = 'Print Preview'
      TabOrder = 2
      Visible = False
      OnClick = btnPrintAllClick
    end
    object Button1: TButton
      Left = 680
      Top = 592
      Width = 105
      Height = 33
      Cancel = True
      Caption = 'Close'
      TabOrder = 3
      OnClick = Button1Click
    end
    object pcFilter: TPageControl
      Left = 8
      Top = 264
      Width = 777
      Height = 321
      ActivePage = tsTransfer
      TabIndex = 4
      TabOrder = 4
      OnChange = pcFilterChange
      object tsReceived: TTabSheet
        Caption = 'Stock &Received'
        object dbgReceipt: TDBGrid
          Left = 8
          Top = 10
          Width = 753
          Height = 263
          Color = 15269887
          Ctl3D = True
          DataSource = dsTransaction
          Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentCtl3D = False
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'CREATED'
              Title.Caption = 'Date'
              Width = 103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'SUPPLIER_NAME'
              Title.Caption = 'Supplier'
              Width = 239
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'REFERENCE'
              Title.Caption = 'Reference'
              Width = 162
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'ORDER_QTY'
              Title.Caption = 'Quantity'
              Width = 86
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'ORDER_UNIT'
              Title.Caption = 'Unit'
              Width = 139
              Visible = True
            end>
        end
      end
      object tsStocktake: TTabSheet
        Caption = '&Stocktake'
        ImageIndex = 1
        object dbgStocktake: TDBGrid
          Left = 8
          Top = 10
          Width = 753
          Height = 263
          Color = 15269887
          Ctl3D = True
          DataSource = dsTransaction
          Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentCtl3D = False
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'CREATED'
              Title.Caption = 'Date'
              Width = 103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'QTY'
              Title.Caption = 'Quantity'
              Width = 101
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT'
              Title.Caption = 'Unit'
              Width = 109
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT_COST'
              Title.Caption = 'Unit Cost'
              Width = 127
              Visible = True
            end>
        end
      end
      object tsManualSales: TTabSheet
        Caption = '&Manual Sales'
        ImageIndex = 2
        object dbgManualSales: TDBGrid
          Left = 8
          Top = 10
          Width = 753
          Height = 263
          Color = 15269887
          Ctl3D = True
          DataSource = dsTransaction
          Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentCtl3D = False
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'CREATED'
              Title.Caption = 'Date'
              Width = 107
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'REFERENCE'
              Title.Caption = 'Reference'
              Width = 129
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'QTY'
              Title.Caption = 'Quantity'
              Width = 80
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT'
              Title.Caption = 'Unit'
              Width = 120
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'PURCHASER_NAME'
              Title.Caption = 'Purchaser'
              Width = 219
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'SALE_PRICE'
              Title.Caption = 'Total Price'
              Width = 89
              Visible = True
            end>
        end
      end
      object tsSales: TTabSheet
        Caption = 'S&ales'
        ImageIndex = 3
        object dbgSales: TDBGrid
          Left = 8
          Top = 10
          Width = 753
          Height = 263
          Color = 15269887
          Ctl3D = True
          DataSource = dsTransaction
          Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentCtl3D = False
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'CREATED'
              Title.Caption = 'Date'
              Width = 294
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'QTY'
              Title.Caption = 'Quantity'
              Width = 253
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT'
              Title.Caption = 'Unit'
              Width = 199
              Visible = True
            end>
        end
      end
      object tsTransfer: TTabSheet
        Caption = 'Transfers'
        ImageIndex = 4
        object dbgTransfer: TDBGrid
          Left = 8
          Top = 10
          Width = 753
          Height = 263
          Color = 15269887
          Ctl3D = True
          DataSource = dsTransaction
          Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          ParentCtl3D = False
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -13
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'CREATED'
              Title.Caption = 'Date'
              Width = 103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'USER_NAME'
              Title.Caption = 'User'
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'QTY'
              Title.Caption = 'Quantity'
              Width = 101
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT'
              Title.Caption = 'Unit'
              Width = 109
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'UNIT_COST'
              Title.Caption = 'Unit Cost'
              Width = 127
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'PURCHASER_NAME'
              Title.Caption = 'Details'
              Visible = True
            end>
        end
      end
    end
  end
  object dsTransaction: TDataSource
    AutoEdit = False
    DataSet = qrManualSales
    Left = 744
    Top = 40
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select'
      '   Stock.Code,'
      '   Stock.Description, '
      '   Stock.Stocktake_Unit, '
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.On_Order'
      'From '
      '   Stock inner join StockLocation on'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      'Where'
      '   Code = :Code')
    Left = 305
    Top = 105
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end>
  end
  object dsStock: TDataSource
    AutoEdit = False
    DataSet = qrStock
    Left = 369
    Top = 105
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 608
    Top = 40
  end
  object qrReceipt: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrAfterOpen
    DataSource = dsStock
    SQL.Strings = (
      'SELECT '
      '   CREATED,'
      '   SUPPLIER_NAME,'
      '   REFERENCE,'
      '   ORDER_QTY,'
      '   ORDER_UNIT,'
      '   QTY,'
      '   UNIT,'
      '   TRANS_NUMBER,'
      '   UNIT_COST,'
      '   SUPPLIER_CODE'
      'FROM'
      '   STOCKTRANS'
      'ORDER BY CREATED DESC')
    Left = 672
    Top = 40
  end
  object qrStocktake: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrAfterOpen
    DataSource = dsStock
    SQL.Strings = (
      'SELECT '
      '   CREATED,'
      '   SUPPLIER_NAME,'
      '   REFERENCE,'
      '   ORDER_QTY,'
      '   ORDER_UNIT,'
      '   QTY,'
      '   UNIT,'
      '   TRANS_NUMBER,'
      '   UNIT_COST,'
      '   SUPPLIER_CODE'
      'FROM'
      '   STOCKTRANS'
      'ORDER BY CREATED DESC')
    Left = 672
    Top = 88
  end
  object qrManualSales: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrAfterOpen
    DataSource = dsStock
    SQL.Strings = (
      #9#9#9#9'SELECT '
      #9#9#9#9#9'CREATED,'
      #9#9#9#9#9'SUPPLIER_NAME,'
      #9#9#9#9#9'REFERENCE,'
      #9#9#9#9#9'ORDER_QTY, '
      #9#9#9#9#9'ORDER_UNIT, '
      #9#9#9#9#9'QTY,'
      #9#9#9#9#9'UNIT, '
      #9#9#9#9#9'TRANS_NUMBER,'
      #9#9#9#9#9'UNIT_COST,'
      #9#9#9#9'PURCHASER_NAME,'
      #9#9#9#9#9'SALE_PRICE,'
      #9#9#9#9#9'SUPPLIER_CODE '
      #9#9#9#9'FROM '
      #9#9#9#9#9'STOCKTRANS '
      'WHERE'
      'TRANSACTION_TYPE = '#39'Manual Sale'#39
      #9#9#9#9'ORDER BY CREATED DESC')
    Left = 672
    Top = 136
  end
  object qrSales: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrAfterOpen
    DataSource = dsStock
    SQL.Strings = (
      'SELECT '
      '   CREATED,'
      '   SUPPLIER_NAME,'
      '   REFERENCE,'
      '   ORDER_QTY,'
      '   ORDER_UNIT,'
      '   QTY,'
      '   UNIT,'
      '   TRANS_NUMBER,'
      '   UNIT_COST,'
      '   SUPPLIER_CODE'
      'FROM'
      '   STOCKTRANS'
      'ORDER BY CREATED DESC')
    Left = 672
    Top = 184
  end
  object qrTransfer: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrAfterOpen
    DataSource = dsStock
    SQL.Strings = (
      'SELECT '
      '   CREATED,'
      '   SUPPLIER_NAME,'
      '   REFERENCE,'
      '   ORDER_QTY,'
      '   ORDER_UNIT,'
      '   QTY,'
      '   UNIT,'
      '   TRANS_NUMBER,'
      '   UNIT_COST,'
      '   SUPPLIER_CODE'
      'FROM'
      '   STOCKTRANS'
      'ORDER BY CREATED DESC')
    Left = 672
    Top = 232
  end
end
