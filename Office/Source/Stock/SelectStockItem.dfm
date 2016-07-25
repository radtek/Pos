object frmSelectStockItem: TfrmSelectStockItem
  Tag = 1024
  Left = 774
  Top = 153
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Select Stock Item'
  ClientHeight = 455
  ClientWidth = 480
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object dbgResult: TDBGrid
    Left = 8
    Top = 128
    Width = 465
    Height = 281
    Color = 15269887
    DataSource = dsStock
    Options = [dgTitles, dgColLines, dgRowLines, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 0
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
        FieldName = 'STOCK_GROUP'
        Title.Caption = 'Stock Group'
        Width = 203
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DESCRIPTION'
        Title.Caption = 'Stock Item'
        Width = 256
        Visible = True
      end>
  end
  object btnOk: TBitBtn
    Left = 392
    Top = 416
    Width = 81
    Height = 33
    Caption = '&Select'
    TabOrder = 1
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
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 40
      Width = 361
      Height = 17
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Enter a few letters/words contained within the stock item you wa' +
        'nt.'
      WordWrap = True
    end
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 361
      Height = 24
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Stock Item'
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
      OnChange = edSearchChange
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
  object btnFindAdd: TBitBtn
    Left = 392
    Top = 8
    Width = 81
    Height = 33
    Caption = '&Find/Add'
    TabOrder = 3
    OnClick = btnFindAddClick
  end
  object btnCancel: TBitBtn
    Left = 392
    Top = 88
    Width = 81
    Height = 33
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
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
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      '  SELECT'
      '   STOCK.STOCK_KEY,'
      '   STOCK.CODE,'#9#9#9#9
      '   STOCK.DESCRIPTION,'
      '   STOCK.STOCKTAKE_UNIT,'
      '   STOCK.CONVERSION_FACTOR,'
      'STOCK.REDUCTION_UNIT,'
      '   StockGroup.Stock_Group '
      'FROM '
      
        '   STOCK INNER JOIN StockGroup ON STOCK.Stock_Group_Key = StockG' +
        'roup.Stock_Group_Key'
      'WHERE '
      '   (Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And'
      '   Stock.Enabled = '#39'T'#39' And'
      '   UPPER(DESCRIPTION) LIKE :DESCRIPTION '
      'ORDER BY '
      '   StockGroup.Sort_Order ASC,'
      '    STOCK.DESCRIPTION')
    Left = 24
    Top = 184
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'DESCRIPTION'
        ParamType = ptUnknown
      end>
  end
  object dsStock: TDataSource
    AutoEdit = False
    DataSet = qrStock
    Left = 72
    Top = 184
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 120
    Top = 184
  end
end
