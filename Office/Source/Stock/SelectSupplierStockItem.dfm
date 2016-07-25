object frmSelectSupplierStockItem: TfrmSelectSupplierStockItem
  Tag = 1024
  Left = 432
  Top = 171
  Width = 340
  Height = 472
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Select Stock Item'
  Color = clBtnFace
  Constraints.MinHeight = 350
  Constraints.MinWidth = 340
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 332
    Height = 445
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel1: TPanel
      Left = 4
      Top = 4
      Width = 324
      Height = 437
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      DesignSize = (
        324
        437)
      object Panel8: TPanel
        Left = 5
        Top = 5
        Width = 314
        Height = 383
        Align = alTop
        Anchors = [akLeft, akTop, akRight, akBottom]
        BevelOuter = bvLowered
        TabOrder = 0
        DesignSize = (
          314
          383)
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 312
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          BorderWidth = 8
          Color = clAppWorkSpace
          TabOrder = 0
          object Label1: TLabel
            Left = 8
            Top = 8
            Width = 208
            Height = 40
            Align = alClient
            Caption = 
              'Select a stock item to add from the invoice. If it is not shown ' +
              'below, select '#39'Other'#39'.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            Layout = tlCenter
            WordWrap = True
          end
          object Panel3: TPanel
            Left = 216
            Top = 8
            Width = 88
            Height = 40
            Align = alRight
            BevelOuter = bvNone
            ParentColor = True
            TabOrder = 0
            object btnOther: TBitBtn
              Left = 4
              Top = 4
              Width = 81
              Height = 33
              Caption = '&Other'
              TabOrder = 0
              OnClick = btnOtherClick
            end
          end
        end
        object tvStock: TTreeView
          Left = 8
          Top = 64
          Width = 297
          Height = 312
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
          TabOrder = 1
          OnChange = tvStockChange
          OnDblClick = tvStockDblClick
        end
      end
      object btnSelect: TBitBtn
        Left = 143
        Top = 396
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Select'
        Default = True
        TabOrder = 1
        OnClick = btnSelectClick
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
      object btnClose: TBitBtn
        Left = 231
        Top = 396
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Cancel = True
        Caption = '&Close'
        TabOrder = 2
        OnClick = btnCloseClick
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
      object btnFind: TBitBtn
        Left = 8
        Top = 396
        Width = 81
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
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 264
    Top = 144
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 264
    Top = 96
  end
  object qrSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   StockCategory.Stock_Category,'
      '   StockCategory.Stock_Category_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.Stock_Group_Key,'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description'
      'From'
      '   SupplierStock inner join Stock on'
      '      SupplierStock.Stock_Key = Stock.Stock_Key'
      '   Inner Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Inner Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Supplier_Key = :Supplier_Key And'
      '   Stock.Enabled = '#39'T'#39' And'
      '   Stock.Deleted = '#39'F'#39
      'Order By'
      
        '   StockCategory.STOCK_CATEGORY, StockGroup.Stock_Group,   Stock' +
        '.Description ASC')
    Left = 56
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end>
  end
  object qrLocationSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   StockCategory.Stock_Category,'
      '   StockCategory.Stock_Category_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.Stock_Group_Key,'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description'
      'From'
      '   SupplierStock inner join Stock on'
      '      SupplierStock.Stock_Key = Stock.Stock_Key'
      '   Inner Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Inner Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      
        ' inner join STOCKLOCATION on STOCKLOCATION.STOCK_KEY= SupplierSt' +
        'ock.Stock_Key'
      'Where'
      '   Supplier_Key = :Supplier_Key  And'
      '   Stock.Enabled = '#39'T'#39' And'
      '   Stock.Deleted = '#39'F'#39' and STOCKLOCATION.LOCATION=:LOCATION'
      'Order By'
      
        '   StockCategory.STOCK_CATEGORY, StockGroup.Stock_Group, Stock.D' +
        'escription')
    Left = 56
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
end
