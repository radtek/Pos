object frmDebtorInvoices: TfrmDebtorInvoices
  Left = 2204
  Top = 307
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Invoices'
  ClientHeight = 573
  ClientWidth = 721
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    721
    573)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 705
    Height = 557
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      705
      557)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 695
      Height = 503
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        695
        503)
      object Label3: TLabel
        Left = 504
        Top = 466
        Width = 89
        Height = 16
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Total:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 512
        Top = 480
        Width = 81
        Height = 16
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Payments'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbePayments: TLabel
        Left = 616
        Top = 480
        Width = 73
        Height = 16
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = 'PaymentsTotal'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeInvoiceTotal: TLabel
        Left = 616
        Top = 466
        Width = 73
        Height = 16
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = 'InvoiceTotal'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 693
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 693
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
      object tvInvoices: TTreeView
        Left = 8
        Top = 56
        Width = 201
        Height = 436
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
        OnChange = tvInvoicesChange
        OnDeletion = tvInvoicesDeletion
      end
      object Panel1: TPanel
        Left = 216
        Top = 57
        Width = 470
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        BevelOuter = bvNone
        Caption = '`'
        Color = clAppWorkSpace
        TabOrder = 2
        object lbeReference: TLabel
          Left = 0
          Top = 0
          Width = 470
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
      object PageControl: TPageControl
        Left = 210
        Top = 89
        Width = 484
        Height = 376
        ActivePage = tsPayments
        Anchors = [akLeft, akTop, akRight, akBottom]
        Style = tsFlatButtons
        TabOrder = 3
        object tsInvoice: TTabSheet
          Caption = 'tsInvoice'
          TabVisible = False
          object dbgDetails: TDBGrid
            Left = 0
            Top = 0
            Width = 476
            Height = 366
            Align = alClient
            Color = clInfoBk
            DataSource = dsInvoiceDetail
            Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
            ReadOnly = True
            TabOrder = 0
            TitleFont.Charset = DEFAULT_CHARSET
            TitleFont.Color = clWindowText
            TitleFont.Height = -11
            TitleFont.Name = 'Tahoma'
            TitleFont.Style = []
            Columns = <
              item
                Expanded = False
                FieldName = 'QTY'
                Title.Caption = 'Quantity'
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'SIZE_NAME'
                Title.Caption = 'Size'
                Width = 91
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'ITEM_NAME'
                Title.Caption = 'Item'
                Width = 214
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'PRICE'
                Title.Caption = 'Price'
                Width = 93
                Visible = True
              end>
          end
        end
        object tsPayments: TTabSheet
          Caption = 'tsPayments'
          ImageIndex = 1
          TabVisible = False
          object dbgPayments: TDBGrid
            Left = 0
            Top = 0
            Width = 476
            Height = 366
            Align = alClient
            Color = clInfoBk
            DataSource = dsPayments
            Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
            ReadOnly = True
            TabOrder = 0
            TitleFont.Charset = DEFAULT_CHARSET
            TitleFont.Color = clWindowText
            TitleFont.Height = -11
            TitleFont.Name = 'Tahoma'
            TitleFont.Style = []
            Columns = <
              item
                Expanded = False
                FieldName = 'PAY_TYPE'
                Title.Caption = 'Pay Type'
                Width = 106
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'INVOICE_NUMBER'
                Title.Caption = 'Receipt Number'
                Width = 141
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'TIME_STAMP'
                Title.Caption = 'Date Time'
                Width = 118
                Visible = True
              end
              item
                Expanded = False
                FieldName = 'SUBTOTAL'
                Title.Caption = 'Total'
                Width = 99
                Visible = True
              end>
          end
        end
      end
    end
    object btnPrintInvoice: TBitBtn
      Left = 496
      Top = 516
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = 'Preview Invoice'
      TabOrder = 1
      OnClick = btnPrintInvoiceClick
    end
    object btnClose: TBitBtn
      Left = 600
      Top = 516
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      Kind = bkClose
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 516
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 3
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
    object BitBtn1: TBitBtn
      Left = 392
      Top = 516
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = 'Preview Details'
      TabOrder = 4
      OnClick = btnPrintInvoiceClick
    end
  end
  object qrDebtorInvoices: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contacts.Contacts_Key,'
      '   Contacts.Name,'
      '   Invoices.Invoice_Number,'
      '   Invoices.Invoice_Key,'
      '   Invoices.Closed,'
      '   Extract(Month from Invoices.Creation_Date) Invoice_Month,'
      '   Extract(Year from Invoices.Creation_Date) Invoice_Year,'
      '   Invoices.Creation_Date'
      'From'
      '   Contacts Inner Join Invoices on'
      '      Contacts.Contacts_Key = Invoices.Contacts_Key'
      'Where'
      '   Contacts.Contacts_Key = :Contacts_Key'
      'Order By'
      '   Invoices.Closed,'
      '   Invoices.Creation_Date,'
      '   Invoices.Invoice_Number')
    Left = 56
    Top = 136
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contacts_Key'
        ParamType = ptUnknown
      end>
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 56
    Top = 80
  end
  object qrInvoiceDetail: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Size_Name,'
      '   Item_Name,'
      '   Sum( Qty ) as Qty,'
      '   Sum( Price ) as Price'
      'from'
      '('
      'Select'
      '   Orders.Size_Name,'
      '   Cast(Orders.Item_Name as VarChar(50)) Item_Name,'
      '   Sum(Orders.Qty) Qty,'
      
        '   Cast(Sum((Orders.Price * Orders.Qty) + Orders.Discount) as nu' +
        'meric(15,4)) Price'
      'From'
      '   Invoices Inner Join Tab on'
      '      Invoices.Invoice_Key = Tab.Invoice_Key'
      '   Left Join Orders on'
      '      Tab.Tab_Key = Orders.Tab_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      'Group By'
      '   Orders.Item_Name,'
      '   Orders.Size_Name'
      ''
      'Union All'
      ''
      'Select'
      '   DayArchive.Size_Name,'
      '   Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      '   Sum(DayArchive.Qty) Qty,'
      
        '   cast(Sum((DayArchive.Price * DayArchive.QTY) + DayArchive.Dis' +
        'count) as numeric(15,4)) Price'
      'From'
      '   Invoices Inner Join DayArcBill on'
      '      Invoices.Invoice_Key = DayArcBill.Invoice_Key'
      '   Left Join DayArchive on'
      '      DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      'Group By'
      '   DayArchive.Item_Name,'
      '   DayArchive.Size_Name'
      ''
      'Union All'
      ''
      'Select'
      '   Archive.Size_Name,'
      '   Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      '   Sum(Archive.Qty) Qty,'
      
        '   cast(Sum((Archive.Price * Archive.Qty) + Archive.Discount) as' +
        ' numeric(15,4)) Price'
      'From'
      '   Invoices Inner Join ArcBill on'
      '      Invoices.Invoice_Key = ArcBill.Invoice_Key'
      '   Left Join Archive on'
      '      ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      'Group By'
      '   Archive.Item_Name,'
      '   Archive.Size_Name'
      ')'
      'Group By'
      '   Item_Name,'
      '   Size_Name'
      'Order by 2,1')
    Left = 56
    Top = 192
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end>
  end
  object dsInvoiceDetail: TDataSource
    AutoEdit = False
    DataSet = qrInvoiceDetail
    Left = 136
    Top = 192
  end
  object qrtotalPriceTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    Left = 56
    Top = 192
  end
  object dsTotalPriceTotal: TDataSource
    AutoEdit = False
    DataSet = qrtotalPriceTotal
    Left = 136
    Top = 192
  end
  object qrInvoiceTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Sum(Orders.Price + Orders.Discount) Total_Price'
      'From'
      '   Invoices Inner Join Tab on'
      '      Invoices.Invoice_Key = Tab.Invoice_Key'
      '   Left Join Orders on'
      '      Tab.Tab_Key = Orders.Tab_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      ''
      'Union All'
      ''
      'Select'
      '   Sum(DayArchive.Price + DayArchive.Discount) Total_Price'
      'From'
      '   Invoices Inner Join DayArcBill on'
      '      Invoices.Invoice_Key = DayArcBill.Invoice_Key'
      '   Left Join DayArchive on'
      '      DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      ''
      'Union All'
      ''
      'Select'
      '   Sum(Archive.Price + Archive.Discount) Total_Price'
      'From'
      '   Invoices Inner Join ArcBill on'
      '      Invoices.Invoice_Key = ArcBill.Invoice_Key'
      '   Left Join Archive on'
      '      ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      'Where'
      '   Invoices.Invoice_Number = :Invoice_Number'
      ' ')
    Left = 56
    Top = 248
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Invoice_Number'
        ParamType = ptUnknown
      end>
  end
  object dsInvoiceTotal: TDataSource
    AutoEdit = False
    DataSet = qrInvoiceTotal
    Left = 136
    Top = 248
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 128
    Top = 520
  end
  object qrPayments: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      
        '   DayArcBill.Invoice_Key,DayArcBillPay.SubTotal,DayArcBill.Invo' +
        'ice_Number,DayArcBill.Time_Stamp,DayArcBillPay.Pay_Type'
      'From'
      
        '   DayArcBill left join DayArcBillPay on DayArcBillPay.ArcBill_K' +
        'ey = DayArcBill.ArcBill_Key '
      'where'
      '    DayArcBillPay.SubTotal > 0 and'
      '    DayArcBill.Invoice_Key = :InvoiceKey and '
      '    DayArcBill.Invoice_Key is not null '
      'Union All'
      ''
      'Select'
      
        '  ArcBill.Invoice_Key, ArcBillPay.SubTotal,ArcBill.Invoice_Numbe' +
        'r,ArcBill.Time_Stamp,ArcBillPay.Pay_Type'
      'From'
      
        '   ArcBill left join ArcBillPay on ArcBillPay.ArcBill_Key = ArcB' +
        'ill.ArcBill_Key '
      'Where'
      '       ArcBillPay.SubTotal > 0 and'
      '       ArcBill.Invoice_Key = :InvoiceKey and'
      '       ArcBill.Invoice_Key is not null '
      ''
      'Order By 4, 3')
    Left = 48
    Top = 320
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'InvoiceKey'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'InvoiceKey'
        ParamType = ptUnknown
      end>
  end
  object dsPayments: TDataSource
    AutoEdit = False
    DataSet = qrPayments
    Left = 128
    Top = 320
  end
end
