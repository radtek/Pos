object frmPurchaseOrders: TfrmPurchaseOrders
  Tag = 1024
  Left = 539
  Top = 182
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Purchase Orders'
  ClientHeight = 468
  ClientWidth = 717
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
  DesignSize = (
    717
    468)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 702
    Height = 452
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      702
      452)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 692
      Height = 398
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        692
        398)
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 690
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 690
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
      object dbgDetails: TDBGrid
        Left = 224
        Top = 96
        Width = 459
        Height = 225
        Anchors = [akLeft, akTop, akRight, akBottom]
        Color = clInfoBk
        DataSource = dsPurchStock
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit]
        ReadOnly = True
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        OnDrawColumnCell = dbgDetailsDrawColumnCell
        Columns = <
          item
            Expanded = False
            FieldName = 'DESCRIPTION'
            Title.Caption = 'Stock Item'
            Width = 167
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'SUPPLIER_CODE'
            Title.Caption = 'Supplier Code'
            Width = 105
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'QTY_ORDERED'
            Title.Caption = 'Ordered'
            Width = 88
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'QTY_ON_ORDER'
            Title.Caption = 'To Receive'
            Width = 88
            Visible = True
          end>
      end
      object tvInvoices: TTreeView
        Left = 8
        Top = 56
        Width = 209
        Height = 331
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
        TabOrder = 2
        OnChange = tvInvoicesChange
        OnDblClick = tvInvoicesDblClick
      end
      object Panel3: TPanel
        Left = 224
        Top = 57
        Width = 459
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 3
        object lbeReference: TLabel
          Left = 0
          Top = 0
          Width = 459
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
      object btnPrint: TButton
        Left = 587
        Top = 333
        Width = 97
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = '&Print Order'
        TabOrder = 4
        OnClick = btnPrintClick
      end
      object btnEmailOrder: TButton
        Tag = 1
        Left = 480
        Top = 333
        Width = 97
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Email Order'
        TabOrder = 5
        OnClick = btnPrintClick
      end
    end
    object btnClose: TBitBtn
      Left = 595
      Top = 411
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 1
      Kind = bkClose
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 411
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
    object btnNew: TButton
      Left = 491
      Top = 411
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&New'
      TabOrder = 3
      OnClick = btnNewClick
    end
    object btnUpdate: TButton
      Left = 387
      Top = 411
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Edit'
      TabOrder = 4
      OnClick = btnUpdateClick
    end
    object btnDelete: TButton
      Left = 283
      Top = 411
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Cancel Order'
      TabOrder = 5
      OnClick = btnDeleteClick
    end
    object Button2: TButton
      Left = 107
      Top = 411
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Setup'
      TabOrder = 6
      OnClick = Button2Click
    end
  end
  object qrOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrOrderAfterScroll
    SQL.Strings = (
      'SELECT'
      '   ORDER_LK,'
      '   ORDER_NUMBER,'
      '   CREATED,'
      '   Extract(Month from Created) Order_Month,'
      '   Extract(Year from Created) Order_Year,'
      '   SUPPLIER_ADDRESS,'
      '   CREATOR_NAME,'
      '   REQUIRED,'
      '   POSTED'
      'FROM '
      '   PURCHASEORDER '
      'WHERE '
      '   CONTACT_FK = :CONTACT_FK '
      'ORDER BY '
      '   POSTED,'
      '   CREATED DESC')
    Left = 40
    Top = 88
    ParamData = <
      item
        DataType = ftString
        Name = 'CONTACT_FK'
        ParamType = ptUnknown
        Value = '66'
      end>
  end
  object dsOrder: TDataSource
    AutoEdit = False
    DataSet = qrOrder
    Left = 96
    Top = 88
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 88
    Top = 368
  end
  object qrPurchStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsOrder
    SQL.Strings = (
      'Select'
      '   PurchaseStock.Purchase_Stock_LK,'
      '   PurchaseStock.Code,'
      '   PurchaseStock.Supplier_Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order,'
      
        '   Cast(PurchaseStock.Supplier_Unit_Qty as integer) || '#39' '#39' || Pu' +
        'rchaseStock.Supplier_Unit Qty_Ordered,'
      
        '   Cast(PurchaseStock.Qty_On_Order as integer) || '#39' '#39' || Purchas' +
        'eStock.Supplier_Unit Qty_On_Order'
      'From'
      '   PurchaseStock Left Join Stock On'
      '      PurchaseStock.Code = Stock.Code'
      'Where'
      '   Order_FK = :Order_LK')
    Left = 376
    Top = 160
    ParamData = <
      item
        DataType = ftInteger
        Name = 'ORDER_LK'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object dsPurchStock: TDataSource
    AutoEdit = False
    DataSet = qrPurchStock
    Left = 448
    Top = 160
  end
  object qrUpdate: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 152
    Top = 368
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 232
    Top = 504
  end
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Company_Name,'
      '   Email'
      'From'
      '   Contact'
      'Where'
      '   Contact_LK = :Contact_Key')
    Left = 464
    Top = 16
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_Key'
        ParamType = ptUnknown
      end>
  end
  object qrDeleteOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrOrderAfterScroll
    SQL.Strings = (
      'Delete From PurchaseOrder Where'
      '   Order_LK = :OrderKey')
    Left = 40
    Top = 152
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'OrderKey'
        ParamType = ptUnknown
      end>
  end
end
