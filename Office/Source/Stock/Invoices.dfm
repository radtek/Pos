object frmInvoices: TfrmInvoices
  Tag = 1024
  Left = 278
  Top = 8
  Width = 723
  Height = 572
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Receive Invoice'
  Color = clBtnFace
  Constraints.MinHeight = 368
  Constraints.MinWidth = 721
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
    715
    545)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 699
    Height = 529
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      699
      529)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 689
      Height = 475
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        689
        475)
      object Label3: TLabel
        Left = 386
        Top = 419
        Width = 32
        Height = 13
        Anchors = [akRight, akBottom]
        Caption = 'Total:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeGSTCaption: TLabel
        Left = 386
        Top = 435
        Width = 22
        Height = 13
        Anchors = [akRight, akBottom]
        Caption = 'TAX'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object dbtInvoiceTotal: TDBText
        Left = 537
        Top = 419
        Width = 126
        Height = 17
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        DataField = 'TOTAL_COST'
        DataSource = dsInvoiceTotal
      end
      object dbtGST: TDBText
        Left = 537
        Top = 435
        Width = 126
        Height = 17
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        DataField = 'TOTAL_GST'
        DataSource = dsInvoiceTotal
      end
      object Label1: TLabel
        Left = 386
        Top = 451
        Width = 112
        Height = 13
        Anchors = [akRight, akBottom]
        Caption = 'Total Including TAX:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object dbtInvoiceTotalIncluding: TDBText
        Left = 537
        Top = 451
        Width = 126
        Height = 17
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        DataField = 'TOTAL_COST_INCL'
        DataSource = dsInvoiceTotal
      end
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 687
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 687
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
        Left = 216
        Top = 96
        Width = 464
        Height = 318
        Anchors = [akLeft, akTop, akRight, akBottom]
        Color = clInfoBk
        DataSource = dsInvoiceDetail
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'DESCRIPTION'
            Title.Caption = 'Stock Item'
            Width = 181
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'LOCATION'
            Title.Caption = 'Location'
            Width = 81
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'ORDER_QTY'
            Title.Caption = 'Qty'
            Width = 55
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'ORDER_UNIT'
            Title.Caption = 'Unit'
            Width = 73
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'TOTAL_COST'
            Title.Caption = 'Total'
            Width = 65
            Visible = True
          end>
      end
      object tvInvoices: TTreeView
        Left = 8
        Top = 56
        Width = 201
        Height = 408
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
        OnKeyPress = tvInvoicesKeyPress
      end
      object Panel1: TPanel
        Left = 216
        Top = 57
        Width = 464
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        BevelOuter = bvNone
        Caption = '`'
        Color = clAppWorkSpace
        TabOrder = 3
        object lbeReference: TLabel
          Left = 0
          Top = 0
          Width = 464
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
    end
    object btnUpdateInvoice: TBitBtn
      Left = 386
      Top = 488
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Update'
      TabOrder = 1
      OnClick = btnUpdateInvoiceClick
    end
    object btnNewInvoice: TBitBtn
      Left = 490
      Top = 488
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&New Invoice'
      TabOrder = 2
      OnClick = btnNewInvoiceClick
    end
    object btnClose: TBitBtn
      Left = 594
      Top = 488
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 3
      OnClick = btnCloseClick
      Kind = bkClose
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 488
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 4
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
    object btnPrintInvoice: TBitBtn
      Left = 282
      Top = 488
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Print Invoice'
      TabOrder = 5
      OnClick = btnPrintInvoiceClick
    end
  end
  object qrContact: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   CONTACT_LK,'
      '   COMPANY_NAME'
      'FROM'
      '   CONTACT'
      'WHERE'
      '   CONTACT_LK = :CONTACT_LK')
    Left = 80
    Top = 152
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CONTACT_LK'
        ParamType = ptUnknown
      end>
  end
  object dsContact: TDataSource
    AutoEdit = False
    DataSet = qrContact
    Left = 160
    Top = 152
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 40
    Top = 80
  end
  object qrInvoice: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsContact
    SQL.Strings = (
      'Select'
      #9'Min(Trans_Number) Trans_Number,'
      #9'Min(Created) Created,'
      #9'Min(Extract(Month from Created)) Trans_Month,'
      #9'Min(Extract(Year from Created)) Trans_Year,'
      #9'Reference,'
      #9'Supplier_Key'
      'From'
      #9'StockTrans'
      'Where'
      #9'Supplier_Key = :Contact_LK'
      ''
      'Group By'
      #9'Supplier_Key,'
      #9'Reference'
      'Order By'
      #9'2 Desc,'
      #9'Reference Desc')
    Left = 80
    Top = 200
    ParamData = <
      item
        DataType = ftInteger
        Name = 'CONTACT_LK'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object dsInvoice: TDataSource
    AutoEdit = False
    DataSet = qrInvoice
    Left = 160
    Top = 200
  end
  object qrInvoiceExists: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsContact
    SQL.Strings = (
      'SELECT'
      #9'COUNT(*)'
      'FROM'
      #9'STOCKTRANS'
      'WHERE'
      #9'SUPPLIER_NAME = :COMPANY_NAME AND'
      #9'REFERENCE = :REFERENCE')
    Left = 456
    Top = 248
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
  object qrInvoiceDetail: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrInvoiceDetailAfterScroll
    DataSource = dsInvoice
    SQL.Strings = (
      'SELECT'
      #9'CODE,'
      #9'DESCRIPTION,'
      #9'LOCATION,'
      #9'ORDER_QTY,'
      #9'ORDER_UNIT,'
      #9'QTY,'
      #9'UNIT_COST,'
      #9'UNIT,'
      #9'TOTAL_COST,'
      #9' TOTAL_GST,'
      '                TRANSACTION_TYPE,'
      '                TRANS_NUMBER,'
      '                BATCH_KEY'
      'FROM'
      #9'STOCKTRANS'
      'WHERE'
      #9'Supplier_Key = :Supplier_Key And'
      #9'REFERENCE = :REFERENCE'
      'ORDER BY'
      #9'DESCRIPTION, LOCATION')
    Left = 80
    Top = 248
    ParamData = <
      item
        DataType = ftInteger
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
        Size = 4
      end
      item
        DataType = ftString
        Name = 'REFERENCE'
        ParamType = ptUnknown
        Size = 26
      end>
  end
  object dsInvoiceDetail: TDataSource
    AutoEdit = False
    DataSet = qrInvoiceDetail
    Left = 160
    Top = 248
  end
  object qrInvoiceTotal: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = qrInvoiceTotalAfterScroll
    DataSource = dsInvoice
    SQL.Strings = (
      'SELECT'
      #9'Sum(TOTAL_GST) as TOTAL_GST,'
      '                 sum(TOTAL_COST) as TOTAL_COST,'
      '                sum(TOTAL_COST + TOTAL_GST) as TOTAL_COST_INCL'
      ''
      'FROM'
      #9'TRANSACTIONBATCH'
      'WHERE'
      #9'Supplier_Key = :Supplier_Key AND'
      #9'REFERENCE = :REFERENCE'
      '')
    Left = 80
    Top = 304
    ParamData = <
      item
        DataType = ftInteger
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
        Size = 4
      end
      item
        DataType = ftString
        Name = 'REFERENCE'
        ParamType = ptUnknown
        Size = 26
      end>
  end
  object dsInvoiceTotal: TDataSource
    AutoEdit = False
    DataSet = qrInvoiceTotal
    Left = 160
    Top = 304
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 128
    Top = 488
  end
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_LK,'
      '   Company_Name,'
      '   Supplier_ID,'
      '   MYOB1,'
      '   MYOB2,'
      '   MYOB3'
      'From'
      '   Contact'
      'Where'
      '   Contact_LK = :Supplier_Key')
    Left = 72
    Top = 112
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end>
  end
  object qrIsCommitted: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsContact
    SQL.Strings = (
      'Select'
      #9'ISCOMMITTED_PACKINGSLIP'
      'From'
      #9'TRANSACTIONBATCH'
      'Where'
      '    TRANSACTIONBATCH.SUPPLIER_KEY = :SUPPLIER_KEY and '
      
        '    TRANSACTIONBATCH.ISCOMMITTED_PACKINGSLIP =     :ISCOMMITTED_' +
        'PACKINGSLIP and '
      '    TRANSACTIONBATCH.REFERENCE =  :REFERENCE')
    Left = 80
    Top = 352
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISCOMMITTED_PACKINGSLIP'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REFERENCE'
        ParamType = ptUnknown
      end>
  end
end
