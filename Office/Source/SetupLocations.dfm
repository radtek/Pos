object frmSetupLocations: TfrmSetupLocations
  Left = 227
  Top = 101
  BorderStyle = bsDialog
  Caption = 'Setup Locations'
  ClientHeight = 453
  ClientWidth = 830
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 817
    Height = 441
    BorderWidth = 3
    TabOrder = 0
    object Panel3: TPanel
      Left = 4
      Top = 4
      Width = 809
      Height = 388
      Align = alClient
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 807
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object Label6: TLabel
          Left = 8
          Top = 0
          Width = 441
          Height = 40
          AutoSize = False
          Caption = 'Set Location Types'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object Panel1: TPanel
        Left = 8
        Top = 48
        Width = 793
        Height = 329
        BevelOuter = bvNone
        BorderStyle = bsSingle
        Color = clInfoBk
        TabOrder = 1
        object Label1: TLabel
          Left = 16
          Top = 8
          Width = 401
          Height = 33
          AutoSize = False
          Caption = 
            'All '#39'Stock Locations'#39' will be added to new stock items (as long ' +
            'as '#39'Single Location'#39' isn'#39't selected in setup) '
          WordWrap = True
        end
        object Label2: TLabel
          Left = 16
          Top = 48
          Width = 401
          Height = 25
          AutoSize = False
          Caption = ' '#39'MenuMate Locations'#39' will be available for POS terminals.'
          WordWrap = True
        end
        object StringGrid1: TStringGrid
          Left = 8
          Top = 96
          Width = 777
          Height = 185
          ColCount = 7
          DefaultRowHeight = 17
          FixedCols = 0
          Options = [goFixedVertLine, goFixedHorzLine, goDrawFocusSelected]
          TabOrder = 0
          OnDrawCell = StringGrid1DrawCell
          OnKeyPress = StringGrid1KeyPress
          OnMouseDown = StringGrid1MouseDown
          OnMouseMove = StringGrid1MouseMove
          OnMouseUp = StringGrid1MouseUp
          ColWidths = (
            183
            103
            108
            108
            108
            108
            64)
        end
        object btnAdd: TBitBtn
          Left = 16
          Top = 288
          Width = 57
          Height = 33
          Caption = 'Add'
          TabOrder = 1
          OnClick = btnAddClick
        end
        object btnDelete: TBitBtn
          Left = 80
          Top = 288
          Width = 57
          Height = 33
          Caption = 'Delete'
          TabOrder = 2
          OnClick = btnDeleteClick
        end
        object btnRename: TBitBtn
          Left = 144
          Top = 288
          Width = 57
          Height = 33
          Caption = 'Rename'
          TabOrder = 3
          OnClick = btnRenameClick
        end
        object btnSync: TBitBtn
          Left = 272
          Top = 288
          Width = 145
          Height = 33
          Caption = 'Synchronize With Stock'
          TabOrder = 4
          OnClick = btnSyncClick
        end
        object btnInfo: TBitBtn
          Left = 208
          Top = 288
          Width = 57
          Height = 33
          Caption = 'Info'
          TabOrder = 5
          OnClick = btnInfoClick
        end
        object Panel6: TPanel
          Left = 416
          Top = 75
          Width = 257
          Height = 22
          Caption = 'Prompt For Category on'
          TabOrder = 6
        end
        object Panel7: TPanel
          Left = 8
          Top = 75
          Width = 409
          Height = 22
          TabOrder = 7
        end
        object Panel8: TPanel
          Left = 672
          Top = 75
          Width = 109
          Height = 22
          Caption = 'Stock Request Auto'
          TabOrder = 8
        end
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 392
      Width = 809
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        809
        45)
      object btnOk: TBitBtn
        Left = 724
        Top = 8
        Width = 81
        Height = 33
        Anchors = [akTop, akRight]
        Caption = 'Close'
        TabOrder = 0
        OnClick = btnOkClick
        Kind = bkOK
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 56
    Top = 184
  end
  object sqlLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Location_Key,'
      '   Name,'
      '   Location_Type ,'
      'CATEGORY_TYPE ,'
      'STOCKREQUESTAUTO_KEY'
      'From'
      '   Locations'
      'Order By'
      '   Name')
    Transaction = Transaction
    Left = 56
    Top = 232
  end
  object sqlUpdateLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Update'
      '   Locations'
      'Set'
      '   Location_Type = :Location_Type,'
      '   Name = :Name ,'
      'CATEGORY_TYPE=:CATEGORY_TYPE ,'
      'STOCKREQUESTAUTO_KEY=:STOCKREQUESTAUTO_KEY'
      'Where'
      '   Location_Key = :Location_Key')
    Transaction = Transaction
    Left = 136
    Top = 184
  end
  object qrFindLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Location_Key,'
      '   Location_Type,'
      '   Name'
      'From'
      '   Locations'
      'Where'
      '   Upper(Name) = :Name')
    Transaction = Transaction
    Left = 136
    Top = 232
  end
  object sqlAddLK: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      'SELECT GEN_ID(GEN_LOCATION, 1) FROM RDB$DATABASE')
    Transaction = Transaction
    Left = 216
    Top = 184
  end
  object qrAddLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Insert Into Locations ('
      '   Location_Key,'
      '   Location_Type,'
      'Name,'
      '   CATEGORY_TYPE ,'
      'STOCKREQUESTAUTO_KEY)'
      '   '
      'Values ('
      '   :Location_Key,'
      '   :Location_Type,'
      '   :Name,'
      '    :CATEGORY_TYPE,'
      ':STOCKREQUESTAUTO_KEY)')
    Transaction = Transaction
    Left = 216
    Top = 232
  end
  object qrDeleteLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Delete From'
      '   Locations'
      'Where'
      '   Location_Key = :Location_Key')
    Transaction = Transaction
    Left = 296
    Top = 232
  end
  object qrRenameLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Update'
      '   Locations'
      'Set'
      '   Name = :Name'
      'Where'
      '   Location_Key = :Location_Key')
    Transaction = Transaction
    Left = 296
    Top = 184
  end
  object qrLocInfo: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select device_name, product'
      'from Devices '
      'where location_key = :location_key'
      'and device_type = 1'
      'order by device_name')
    Transaction = Transaction
    Left = 376
    Top = 184
  end
  object StockTransaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 376
    Top = 232
  end
  object qrStockLocInfo: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'select count(location) '
      
        'from stock inner join stocklocation on stock.stock_key = stocklo' +
        'cation.stock_key'
      'where location = :location'
      'group by location')
    Transaction = StockTransaction
    Left = 424
    Top = 200
  end
end
