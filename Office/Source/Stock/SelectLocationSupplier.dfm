object frmSelectLocationSupplier: TfrmSelectLocationSupplier
  Tag = 1024
  Left = 331
  Top = 171
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Select Contact'
  ClientHeight = 454
  ClientWidth = 480
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
  object btnFind: TBitBtn
    Left = 392
    Top = 8
    Width = 81
    Height = 33
    Caption = '&Find/Add'
    TabOrder = 0
    OnClick = btnFindClick
  end
  object dbgResult: TDBGrid
    Left = 8
    Top = 128
    Width = 465
    Height = 281
    Color = 15269887
    DataSource = dsContact
    Options = [dgTitles, dgColLines, dgRowLines, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnDblClick = dbgResultDblClick
    OnKeyPress = dbgResultKeyPress
    OnKeyUp = dbgResultKeyUp
    Columns = <
      item
        Expanded = False
        FieldName = 'COMPANY_NAME'
        Title.Caption = 'Company Name'
        Width = 460
        Visible = True
      end>
  end
  object btnCancel: TBitBtn
    Left = 392
    Top = 88
    Width = 81
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object btnOk: TBitBtn
    Left = 392
    Top = 416
    Width = 81
    Height = 33
    Caption = '&Select'
    TabOrder = 3
    OnClick = dbgResultDblClick
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 377
    Height = 113
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = 14221311
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 4
    object Label1: TLabel
      Left = 8
      Top = 38
      Width = 361
      Height = 27
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Enter a few letters/words contained within the company name you ' +
        'want.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 361
      Height = 24
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Supplier'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnGo: TBitBtn
      Left = 288
      Top = 64
      Width = 73
      Height = 33
      Caption = '&Go'
      TabOrder = 0
      OnClick = btnGoClick
    end
    object edSearch: TEdit
      Left = 16
      Top = 72
      Width = 257
      Height = 21
      Color = 15269887
      Ctl3D = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      OnChange = edSearchChange
      OnKeyPress = edSearchKeyPress
    end
  end
  object dsContact: TDataSource
    AutoEdit = False
    DataSet = qrContactforStockReq
    Left = 80
    Top = 176
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 136
    Top = 176
  end
  object qrContactforStockReq: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   CONTACT_LK,'
      '   COMPANY_NAME'#9#9#9#9
      'FROM '
      '   CONTACT'
      
        '   left join SUPPLIERSTOCK on SUPPLIERSTOCK.SUPPLIER_KEY=CONTACT' +
        '.CONTACT_LK'
      
        '   left join STOCKLOCATION on STOCKLOCATION.STOCK_KEY=SUPPLIERST' +
        'OCK.STOCK_KEY'
      'WHERE '
      '  UPPER(COMPANY_NAME) LIKE :COMPANY_NAME '
      '   and '
      '   STOCKLOCATION.LOCATION=:LOCATION'
      ' group by   CONTACT_LK,'
      '   COMPANY_NAME'#9
      'ORDER BY '
      '   COMPANY_NAME ASC')
    Left = 24
    Top = 232
    ParamData = <
      item
        DataType = ftString
        Name = 'COMPANY_NAME'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
end
