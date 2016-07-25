object frmStockGroup: TfrmStockGroup
  Left = 192
  Top = 107
  Width = 286
  Height = 480
  Caption = 'Select Category'
  Color = clWindow
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 278
    Height = 453
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel1: TPanel
      Left = 4
      Top = 4
      Width = 270
      Height = 445
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Panel10: TPanel
        Left = 5
        Top = 395
        Width = 260
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          260
          45)
        object btnClose: TBitBtn
          Left = 158
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Close'
          ModalResult = 1
          TabOrder = 0
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00388888888877
            F7F787F8888888888333333F00004444400888FFF444448888888888F333FF8F
            000033334D5007FFF4333388888888883338888F0000333345D50FFFF4333333
            338F888F3338F33F000033334D5D0FFFF43333333388788F3338F33F00003333
            45D50FEFE4333333338F878F3338F33F000033334D5D0FFFF43333333388788F
            3338F33F0000333345D50FEFE4333333338F878F3338F33F000033334D5D0FFF
            F43333333388788F3338F33F0000333345D50FEFE4333333338F878F3338F33F
            000033334D5D0EFEF43333333388788F3338F33F0000333345D50FEFE4333333
            338F878F3338F33F000033334D5D0EFEF43333333388788F3338F33F00003333
            4444444444333333338F8F8FFFF8F33F00003333333333333333333333888888
            8888333F00003333330000003333333333333FFFFFF3333F00003333330AAAA0
            333333333333888888F3333F00003333330000003333333333338FFFF8F3333F
            0000}
          NumGlyphs = 2
        end
      end
      object Panel2: TPanel
        Left = 8
        Top = 8
        Width = 257
        Height = 393
        BorderWidth = 4
        TabOrder = 1
        DesignSize = (
          257
          393)
        object Panel8: TPanel
          Left = 5
          Top = 5
          Width = 247
          Height = 340
          Align = alTop
          Anchors = [akLeft, akTop, akRight, akBottom]
          BevelOuter = bvLowered
          TabOrder = 0
          object Panel9: TPanel
            Left = 1
            Top = 1
            Width = 245
            Height = 48
            Align = alTop
            BevelOuter = bvNone
            Color = clAppWorkSpace
            TabOrder = 0
            object lbeTitle: TLabel
              Left = 16
              Top = 0
              Width = 401
              Height = 49
              AutoSize = False
              Caption = 'Select A Category'
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
          object lstCategory: TListBox
            Left = 8
            Top = 56
            Width = 225
            Height = 273
            Color = 15269887
            ItemHeight = 13
            MultiSelect = True
            TabOrder = 1
          end
        end
        object btnOk: TBitBtn
          Left = 16
          Top = 352
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
          Left = 136
          Top = 352
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Cancel = True
          Caption = '&Cancel'
          TabOrder = 2
          OnClick = btnCancelClick
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
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 160
    Top = 16
  end
  object qrStockLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15,4)) On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Initialised,'
      '   StockLocation.Sales_Pending,'
      '   StockLocation.WriteOffs_Pending'
      'From'
      '   Stock Inner Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   Stock.Stock_Key = :Stock_Key'
      'Group By'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Initialised,'
      'StockLocation.Sales_Pending,'
      'StockLocation.WriteOffs_Pending'
      'Order By'
      '   Location')
    Left = 104
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object tiTreeScroll: TTimer
    Enabled = False
    Interval = 150
    Left = 128
    Top = 248
  end
  object FindDialog: TFindDialog
    Options = [frDown, frFindNext]
    Left = 40
    Top = 328
  end
  object qrFindStockGroup: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Group_Key'
      'From'
      '   StockGroup'
      'Where'
      '   Upper(Stock_Group) Like :Stock_Group And'
      '   Stock_Group_Key <> :Stock_Group_Key')
    Transaction = Transaction
    Left = 56
    Top = 64
  end
  object qrFindStockCategory: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Category_Key'
      'From'
      '   StockCategory'
      'Where'
      '   Upper(Stock_Category) = :Stock_Category And'
      '   Stock_Category_Key <> :Stock_Category_Key')
    Transaction = Transaction
    Left = 56
    Top = 16
  end
  object qrUpdateStockCategory: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 56
    Top = 168
  end
  object qrFindStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Key'
      'From'
      '   Stock'
      'Where'
      '   Upper(Description) Like :Description and'
      '   Stock_Key <> :Stock_Key')
    Transaction = Transaction
    Left = 56
    Top = 112
  end
  object qrStockList: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code Category_GL_Code,'
      '   StockCategory.Sort_Order Category_Sort_Order,'
      '   StockCategory.Deleted Category_Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      '   StockGroup.Deleted Group_Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted Item_Deleted,'
      '   '
      '   Max(StockLocation.Initialised) Initialised'
      ''
      'From'
      
        '   StockCategory Left Join StockGroup On StockCategory.Stock_Cat' +
        'egory_Key = StockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      'Group By'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code,'
      '   StockCategory.Sort_Order,'
      '   StockCategory.Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order,'
      '   StockGroup.Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      'Order by'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description')
    Transaction = Transaction
    Left = 160
    Top = 168
  end
  object qrStockRequestLocation: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      '     Select'
      '        StockCategory.Stock_Category_Key,'
      '        StockCategory.Stock_Category,'
      '        StockCategory.GL_Code Category_GL_Code,'
      '        StockCategory.Sort_Order Category_Sort_Order,'
      '        StockCategory.Deleted Category_Deleted,'
      '     '
      '        StockGroup.Stock_Group_Key,'
      '        StockGroup.Stock_Group,'
      '        StockGroup.GL_Code Group_GL_Code,'
      '        StockGroup.Stock_Category_Key,'
      '        StockGroup.Sort_Order Group_Sort_Order,'
      '        StockGroup.Deleted Group_Deleted,'
      '     '
      '        Stock.Stock_Key,'
      '        Stock.Description,'
      '        Stock.Code,'
      '        Stock.Stocktake_Unit,'
      '        Stock.Enabled,'
      '        Stock.Deleted Item_Deleted,'
      '       '
      '        Max(StockLocation.Initialised) Initialised,'
      '        StockLocation.LOCATION'
      '     '
      '     From'
      '      '
      
        '          StockCategory Left Join StockGroup On StockCategory.St' +
        'ock_Category_Key = StockGroup.Stock_Category_Key'
      '      '
      
        '          Left Join Stock On StockGroup.Stock_Group_Key = Stock.' +
        'Stock_Group_Key'
      '      '
      
        '          Left Join StockLocation On Stock.Stock_Key = StockLoca' +
        'tion.Stock_Key'
      '     Group By'
      '        StockCategory.Stock_Category_Key,'
      '        StockCategory.Stock_Category,'
      '        StockCategory.GL_Code,'
      '        StockCategory.Sort_Order,'
      '        StockCategory.Deleted,'
      '     '
      '        StockGroup.Stock_Group_Key,'
      '        StockGroup.Stock_Group,'
      '        StockGroup.GL_Code,'
      '        StockGroup.Stock_Category_Key,'
      '        StockGroup.Sort_Order,'
      '        StockGroup.Deleted,'
      '     '
      '        Stock.Stock_Key,'
      '        Stock.Description,'
      '        Stock.Code,'
      '        Stock.Stocktake_Unit,'
      '        Stock.Enabled,'
      '        Stock.Deleted,'
      '        StockLocation.LOCATION'
      '     Order by'
      '        StockCategory.Stock_Category,'
      '        StockGroup.Stock_Group,'
      '        Stock.Description')
    Transaction = Transaction
    Left = 168
    Top = 112
  end
  object qrStockCat: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   '
      '  distinct StockCategory.Stock_Category '
      ''
      'From'
      '   StockCategory '
      
        '   Left Join StockGroup On StockCategory.Stock_Category_Key = St' +
        'ockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      
        '    left join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STO' +
        'CK_KEY'
      ''
      '   where StockCategory.Deleted='#39'F'#39' and'
      '   SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY'
      'Group By'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code,'
      '   StockCategory.Sort_Order,'
      '   StockCategory.Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order,'
      '   StockGroup.Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      'Order by'
      '  STOCKCATEGORY.SORT_ORDER,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description'
      '')
    Transaction = Transaction
    Left = 200
    Top = 160
  end
  object qrStockCatAll: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   '
      '  distinct StockCategory.Stock_Category '
      ''
      'From'
      '   StockCategory '
      
        '   Left Join StockGroup On StockCategory.Stock_Category_Key = St' +
        'ockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      '     '
      '   where StockCategory.Deleted='#39'F'#39
      '   and'
      'StockLocation.LOCATION=:LOCATION'
      ''
      ' '
      '')
    Transaction = Transaction
    Left = 64
    Top = 216
  end
end
