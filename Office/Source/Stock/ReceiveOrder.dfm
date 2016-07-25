object frmReceiveOrder: TfrmReceiveOrder
  Tag = 1024
  Left = 176
  Top = 92
  Width = 786
  Height = 497
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Receive Order'
  Color = clBtnFace
  Constraints.MaxWidth = 786
  Constraints.MinHeight = 357
  Constraints.MinWidth = 652
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
  DesignSize = (
    778
    470)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 761
    Height = 457
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      761
      457)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 751
      Height = 404
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        751
        404)
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 749
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 749
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
      object tvOrders: TTreeView
        Left = 8
        Top = 56
        Width = 185
        Height = 337
        Anchors = [akLeft, akTop, akBottom]
        BevelInner = bvNone
        BevelOuter = bvNone
        Color = clInfoBk
        Constraints.MinWidth = 32
        DragCursor = crDefault
        HideSelection = False
        Images = dmUtilities.imlTreeImages
        Indent = 19
        ReadOnly = True
        RightClickSelect = True
        TabOrder = 1
        OnChange = tvOrdersChange
      end
      object Panel6: TPanel
        Left = 200
        Top = 57
        Width = 545
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        BevelOuter = bvNone
        Caption = '`'
        Color = clAppWorkSpace
        TabOrder = 2
        object lbeReference: TLabel
          Left = 0
          Top = 0
          Width = 545
          Height = 32
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'lbeReference'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object dbgPurchaseStock: TDBGrid
        Left = 200
        Top = 96
        Width = 545
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        Color = clInfoBk
        Ctl3D = True
        DataSource = dsOrderStock
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
        ParentCtl3D = False
        TabOrder = 3
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        OnDblClick = btnNewInvoiceClick
        Columns = <
          item
            Expanded = False
            FieldName = 'DESCRIPTION'
            Title.Caption = 'Description'
            Width = 187
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'SUPPLIER_CODE'
            Title.Caption = 'Supplier Code'
            Width = 74
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'LOCATION'
            Title.Caption = 'Location'
            Width = 73
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'SUPPLIER_UNIT'
            Title.Caption = 'Unit'
            Width = 71
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'SUPPLIER_UNIT_QTY'
            Title.Caption = 'Ordered'
            Width = 61
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'QTY_ON_ORDER'
            Title.Caption = 'To Receive'
            Width = 69
            Visible = True
          end>
      end
    end
    object btnNewInvoice: TBitBtn
      Left = 552
      Top = 416
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = '&New Invoice'
      TabOrder = 1
      OnClick = btnNewInvoiceClick
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 416
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 2
      OnClick = btnFindClick
      Glyph.Data = {
        42010000424D4201000000000000760000002800000011000000110000000100
        040000000000CC00000000000000000000001000000010000000000000000000
        BF0000BF000000BFBF00BF000000BF00BF00BFBF0000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        77777000000077777777777777777000000070000077777000007000000070B0
        00777770F0007000000070F000777770B0007000000070000000700000007000
        0000700B000000B0000070000000700F000700F0000070000000700B000700B0
        0000700000007700000000000007700000007770B00070B00077700000007770
        0000700000777000000077770007770007777000000077770B07770B07777000
        0000777700077700077770000000777777777777777770000000777777777777
        777770000000}
    end
    object btnUndelivered: TButton
      Left = 440
      Top = 416
      Width = 105
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Cancel Order'
      TabOrder = 3
      OnClick = btnUndeliveredClick
    end
    object btnClose: TBitBtn
      Left = 656
      Top = 416
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 4
      OnClick = btnCloseClick
      Kind = bkClose
    end
  end
  object qrOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   ORDER_LK,'
      '   PurchaseOrder.Order_Number,'
      '   Contact.Company_Name,'
      '   CONTACT_FK,'
      ' Contact.CONTACT_LK,'
      '   CREATED,'
      '   CREATOR_NAME,'
      '   REQUIRED'
      'FROM'
      '   PurchaseOrder Left Join Contact On'
      '      PurchaseOrder.Contact_FK = Contact.Contact_LK'
      'Where'
      '   Order_LK = :Order_Key'
      '   ')
    Left = 360
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrOrderStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterOpen = qrOrderStockAfterOpen
    SQL.Strings = (
      'SELECT'
      '   PurchaseStock.PURCHASE_STOCK_LK,'
      '   PurchaseStock.CODE,'
      '   PurchaseStock.SUPPLIER_CODE,'
      '   Stock.DESCRIPTION,'
      '   Stock.Stocktake_Unit,'
      '   PurchaseStock.LOCATION,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order'
      'FROM'
      '   PurchaseStock Left Join Stock on'
      '      PurchaseStock.Code = Stock.Code'
      'WHERE'
      '   ORDER_FK = :Order_Key'
      'Order By'
      '   Stock.Description,'
      '   LOCATION,'
      '   CODE'
      '   ')
    Left = 360
    Top = 256
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object dsOrderStock: TDataSource
    AutoEdit = False
    DataSet = qrOrderStock
    Left = 432
    Top = 256
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 273
    Top = 425
  end
  object qrSetUndelivered: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update PURCHASESTOCK Set'
      '   QTY_NOT_RECEIVED = :QTY_NOT_RECEIVED'
      'Where'
      '   PURCHASE_STOCK_LK = :PURCHASE_STOCK_LK')
    Left = 208
    Top = 424
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'QTY_NOT_RECEIVED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'PURCHASE_STOCK_LK'
        ParamType = ptUnknown
      end>
  end
  object qrSetOnOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update STOCKLOCATION Set'
      '   ON_ORDER = :ON_ORDER'
      'Where'
      '   STOCK_KEY = :STOCK_KEY And'
      '   LOCATION = :LOCATION')
    Left = 144
    Top = 424
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ON_ORDER'
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
  object qrGetOnOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   STOCK.STOCK_KEY,'
      '   STOCKLOCATION.ON_ORDER'
      'From'
      '   STOCK Inner Join STOCKLOCATION on'
      '      STOCK.STOCK_KEY = STOCKLOCATION.STOCK_KEY'
      'Where'
      '   STOCK.CODE = :CODE And'
      '   STOCKLOCATION.LOCATION = :LOCATION')
    Left = 176
    Top = 424
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
  object qrOrders: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   PurchaseOrder.Order_LK,'
      '   PurchaseOrder.Order_Number,'
      '   Contact.Company_Name'
      'From'
      '   PurchaseOrder Left Join Contact on'
      '      PurchaseOrder.Contact_FK = Contact.Contact_LK'
      'Where'

        '   (PurchaseOrder.Received = '#39'F'#39' Or PurchaseOrder.Received Is Nu' +
        'll) And'
      '(PurchaseOrder.Posted = '#39'T'#39' or Contact.Company_Name is Null )'
      'Order By'
      '   Contact.Company_Name,'
      '   PurchaseOrder.Order_Number')
    Left = 240
    Top = 424
  end
  object qrInvoiceExists: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      #9'COUNT(*)'
      'FROM'
      #9'STOCKTRANS'
      'WHERE'
      #9'SUPPLIER_NAME = :COMPANY_NAME AND'
      #9'REFERENCE = :REFERENCE')
    Left = 112
    Top = 424
    ParamData = <
      item
        DataType = ftString
        Name = 'COMPANY_NAME'
        ParamType = ptUnknown
        Size = 51
      end
      item
        DataType = ftUnknown
        Name = 'REFERENCE'
        ParamType = ptUnknown
      end>
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 136
    Top = 496
  end
end

