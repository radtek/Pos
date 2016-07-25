object frmSelectLocation: TfrmSelectLocation
  Left = 259
  Top = 220
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Select Location'
  ClientHeight = 303
  ClientWidth = 370
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 353
    Height = 249
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = 14221311
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 40
      Width = 321
      Height = 33
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Select the location you wish to add or select '#39'Other'#39' to add a n' +
        'ew location.'
      WordWrap = True
    end
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 353
      Height = 24
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Location'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object DBGrid1: TDBGrid
      Left = 16
      Top = 72
      Width = 321
      Height = 121
      Ctl3D = True
      DataSource = dsLocationList
      Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = []
      OnDblClick = DBGrid1DblClick
      Columns = <
        item
          Expanded = False
          FieldName = 'NAME'
          Width = 303
          Visible = True
        end>
    end
    object BitBtn2: TBitBtn
      Left = 16
      Top = 200
      Width = 81
      Height = 33
      Caption = 'Other'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = BitBtn2Click
      NumGlyphs = 2
    end
  end
  object BitBtn1: TBitBtn
    Left = 192
    Top = 264
    Width = 81
    Height = 33
    TabOrder = 1
    Kind = bkOK
  end
  object btnGo: TBitBtn
    Left = 280
    Top = 264
    Width = 81
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object qrAddLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Insert Into Locations ('
      '   Location_Key,'
      '   Location_Type,'
      '   Name)'
      'Values ('
      '   :Location_Key,'
      '   :Location_Type,'
      '   :Name)')
    Transaction = Transaction
    Left = 48
    Top = 144
  end
  object sqlAddLK: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      'SELECT GEN_ID(GEN_LOCATION, 1) FROM RDB$DATABASE')
    Transaction = Transaction
    Left = 120
    Top = 144
  end
  object qrLocationList: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   NAME'
      'From'
      '   LOCATIONS'
      'Where'
      '   Location_Type <> 0'
      'Order By'
      '   NAME')
    Left = 48
    Top = 88
  end
  object dsLocationList: TDataSource
    AutoEdit = False
    DataSet = qrLocationList
    Left = 120
    Top = 88
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 288
    Top = 88
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
    Left = 200
    Top = 88
  end
  object qrUpdateLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Update'
      '   Locations'
      'Set'
      '   Location_Type = :Location_Type'
      'Where'
      '   Location_Key = :Location_Key')
    Transaction = Transaction
    Left = 200
    Top = 144
  end
  object qrLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT distinct STOCKLOCATION.LOCATION'
      'FROM STOCKLOCATION '
      'where'
      'STOCKLOCATION.LOCATION !='#39#39)
    Left = 288
    Top = 144
  end
end
