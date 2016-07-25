object frmCreateProfile: TfrmCreateProfile
  Left = 337
  Top = 171
  Width = 312
  Height = 475
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Stocktake Profile'
  Color = clBtnFace
  Constraints.MinHeight = 280
  Constraints.MinWidth = 250
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  DesignSize = (
    304
    448)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 12
    Width = 77
    Height = 13
    Caption = 'Profile Name:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object tvStockGroups: TTreeView
    Left = 8
    Top = 40
    Width = 289
    Height = 361
    Anchors = [akLeft, akTop, akRight, akBottom]
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
    StateImages = dmUtilities.imlTreeStateImages
    TabOrder = 0
    OnKeyPress = tvStockGroupsKeyPress
    OnMouseDown = tvStockGroupsMouseDown
  end
  object btnOk: TBitBtn
    Left = 112
    Top = 408
    Width = 89
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = '&OK'
    Default = True
    TabOrder = 1
    OnClick = btnOkClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object btnCancel: TBitBtn
    Left = 208
    Top = 408
    Width = 89
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = '&Cancel'
    TabOrder = 2
    Kind = bkCancel
  end
  object edProfileName: TEdit
    Left = 96
    Top = 8
    Width = 121
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clInfoBk
    TabOrder = 3
  end
  object btnDelete: TBitBtn
    Left = 224
    Top = 6
    Width = 73
    Height = 28
    Anchors = [akTop, akRight]
    Caption = 'Delete'
    TabOrder = 4
    OnClick = btnDeleteClick
    NumGlyphs = 2
  end
  object btnSelectAll: TBitBtn
    Left = 8
    Top = 408
    Width = 73
    Height = 33
    Anchors = [akLeft, akBottom]
    Caption = 'Select All'
    TabOrder = 5
    OnClick = btnSelectAllClick
    NumGlyphs = 2
  end
  object qrStockGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockCategory
    SQL.Strings = (
      'Select'
      '   Stock_Group_Key,'
      '   Stock_Group'
      'From'
      '   StockGroup'
      'Where'
      '   Stock_Category_Key = :Stock_Category_Key'
      'Order by'
      'Stock_Group,'
      '   Sort_Order')
    Left = 32
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Category_Key'
        ParamType = ptUnknown
      end>
  end
  object dsStockGroup: TDataSource
    AutoEdit = False
    DataSet = qrStockGroup
    Left = 120
    Top = 120
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockGroup
    SQL.Strings = (
      'Select'
      '   Description'
      'From'
      '   Stock'
      'Where'
      '   Stock_Group_Key = :Stock_Group_Key'
      'Order By'
      '   Description')
    Left = 32
    Top = 176
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Group_Key'
        ParamType = ptUnknown
      end>
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 240
    Top = 352
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 216
    Top = 64
  end
  object qrDeleteProfile: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From'
      '   StocktakeProfiles'
      'Where'
      '   Stocktake_Profile_Key = :Stocktake_Profile_Key'
      '')
    Transaction = Transaction
    Left = 216
    Top = 208
  end
  object dtProfile: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from StocktakeProfiles'
      'where'
      '  STOCKTAKE_PROFILE_KEY = :OLD_STOCKTAKE_PROFILE_KEY')
    InsertSQL.Strings = (
      'insert into StocktakeProfiles'
      '  (STOCKTAKE_PROFILE_KEY, PROFILE_NAME)'
      'values'
      '  (:STOCKTAKE_PROFILE_KEY, :PROFILE_NAME)')
    RefreshSQL.Strings = (
      'Select'
      '   Stocktake_Profile_Key,'
      '   Profile_Name'
      'from StocktakeProfiles '
      'where'
      '  STOCKTAKE_PROFILE_KEY = :STOCKTAKE_PROFILE_KEY')
    SelectSQL.Strings = (
      'Select'
      '   Stocktake_Profile_Key,'
      '   Profile_Name'
      'From'
      '   StocktakeProfiles'
      'Where'
      '   Stocktake_Profile_Key = :Stocktake_Profile_Key')
    ModifySQL.Strings = (
      'update StocktakeProfiles'
      'set'
      '  STOCKTAKE_PROFILE_KEY = :STOCKTAKE_PROFILE_KEY,'
      '  PROFILE_NAME = :PROFILE_NAME'
      'where'
      '  STOCKTAKE_PROFILE_KEY = :OLD_STOCKTAKE_PROFILE_KEY')
    Left = 216
    Top = 112
  end
  object dsProfile: TDataSource
    AutoEdit = False
    DataSet = dtProfile
    Left = 256
    Top = 112
  end
  object dtProfileGroups: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from ProfileToStockGroup'
      'where'
      '  STOCKTAKE_PROFILE_KEY = :OLD_STOCKTAKE_PROFILE_KEY and'
      '  STOCK_GROUP_KEY = :OLD_STOCK_GROUP_KEY')
    InsertSQL.Strings = (
      'insert into ProfileToStockGroup'
      '  (STOCKTAKE_PROFILE_KEY, STOCK_GROUP_KEY)'
      'values'
      '  (:STOCKTAKE_PROFILE_KEY, :STOCK_GROUP_KEY)')
    RefreshSQL.Strings = (
      'Select'
      '   ProfileToStockGroup.Stocktake_Profile_Key,'
      '   ProfileToStockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group'
      'From'
      '   ProfileToStockGroup Inner Join StockGroup  on'
      
        '      ProfileToStockGroup.Stock_Group_Key = StockGroup.Stock_Gro' +
        'up_Key'
      'Where'
      
        '   ProfileToStockGroup.Stocktake_Profile_Key = :Stocktake_Profil' +
        'e_Key And'
      
        '   ProfileToStockGroup.Stock_Group_Key = ProfileToStockGroup.Sto' +
        'ck_Group_Key'
      'Order by'
      '   StockGroup.Sort_Order')
    SelectSQL.Strings = (
      'Select'
      '   Stocktake_Profile_Key,'
      '   Stock_Group_Key'
      'From'
      '   ProfileToStockGroup'
      'Where'
      '   Stocktake_Profile_Key = :Stocktake_Profile_Key')
    ModifySQL.Strings = (
      'update ProfileToStockGroup'
      'set'
      '  STOCKTAKE_PROFILE_KEY = :STOCKTAKE_PROFILE_KEY,'
      '  STOCK_GROUP_KEY = :STOCK_GROUP_KEY'
      'where'
      '  STOCKTAKE_PROFILE_KEY = :OLD_STOCKTAKE_PROFILE_KEY and'
      '  STOCK_GROUP_KEY = :OLD_STOCK_GROUP_KEY')
    DataSource = dsProfile
    Left = 216
    Top = 160
  end
  object qrStockCategory: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock_Category_Key,'
      '   Stock_Category'
      'From'
      '   StockCategory '
      'Order by'
      'Stock_Category,'
      '   Sort_Order')
    Left = 32
    Top = 72
  end
  object dsStockCategory: TDataSource
    AutoEdit = False
    DataSet = qrStockCategory
    Left = 120
    Top = 72
  end
end
