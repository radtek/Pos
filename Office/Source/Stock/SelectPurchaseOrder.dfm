object frmSelectPurchaseOrder: TfrmSelectPurchaseOrder
  Tag = 1024
  Left = 214
  Top = 133
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Select Purchase Order'
  ClientHeight = 456
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
  OnKeyUp = FormKeyUp
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtn2: TBitBtn
    Left = 392
    Top = 8
    Width = 81
    Height = 33
    Caption = '&Find'
    TabOrder = 0
    Visible = False
  end
  object dbgResult: TDBGrid
    Left = 8
    Top = 128
    Width = 465
    Height = 281
    Color = 15269887
    DataSource = dsOrder
    Options = [dgTitles, dgColLines, dgRowLines, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnDblClick = dbgResultDblClick
    OnKeyPress = dbgResultKeyPress
    Columns = <
      item
        Expanded = False
        FieldName = 'SUPPLIER_NAME'
        Title.Caption = 'Supplier Name'
        Width = 285
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ORDER_NUMBER'
        Title.Caption = 'Order Number'
        Width = 174
        Visible = True
      end>
  end
  object btnCancel: TBitBtn
    Left = 392
    Top = 88
    Width = 81
    Height = 33
    Cancel = True
    Caption = '&Close'
    TabOrder = 2
    OnClick = btnCancelClick
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
    object Label2: TLabel
      Left = 8
      Top = 38
      Width = 361
      Height = 33
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Enter a few letters/words contained within the purchase order nu' +
        'mber  you want.'
      WordWrap = True
    end
    object Label3: TLabel
      Left = 8
      Top = 8
      Width = 361
      Height = 24
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Order Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object edSearch: TEdit
      Left = 16
      Top = 72
      Width = 257
      Height = 21
      Color = 15269887
      Ctl3D = True
      ParentCtl3D = False
      TabOrder = 0
      OnKeyPress = edSearchKeyPress
    end
    object btnGo: TBitBtn
      Left = 288
      Top = 64
      Width = 73
      Height = 33
      Caption = '&Go'
      TabOrder = 1
      OnClick = btnGoClick
    end
  end
  object qrOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'SELECT'
      '   SUPPLIER_NAME,'#9#9#9#9
      '   ORDER_NUMBER '
      'FROM '
      '   PURCHASEORDER '
      'WHERE '
      '   UPPER(ORDER_NUMBER) LIKE :ORDER_NUMBER '
      'ORDER BY '
      '   SUPPLIER_NAME ASC')
    Left = 8
    Top = 408
    ParamData = <
      item
        DataType = ftString
        Name = 'ORDER_NUMBER'
        ParamType = ptUnknown
        Value = '1'
      end>
  end
  object dsOrder: TDataSource
    AutoEdit = False
    DataSet = qrOrder
    Left = 72
    Top = 408
  end
  object Transaction: TIBTransaction
    Active = False
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 136
    Top = 408
  end
end
