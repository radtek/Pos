object frmStocktakeProfiles: TfrmStocktakeProfiles
  Tag = 1024
  Left = 464
  Top = 200
  Width = 581
  Height = 331
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Select the stock groups for the stocktake'
  Color = clBtnFace
  Constraints.MinHeight = 190
  Constraints.MinWidth = 200
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
  object Panel6: TPanel
    Left = 0
    Top = 0
    Width = 573
    Height = 304
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel1: TPanel
      Left = 4
      Top = 4
      Width = 565
      Height = 296
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Splitter1: TSplitter
        Left = 209
        Top = 5
        Width = 4
        Height = 241
        Cursor = crHSplit
      end
      object Panel2: TPanel
        Left = 452
        Top = 5
        Width = 108
        Height = 241
        Align = alRight
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 2
        object btnEditProfile: TBitBtn
          Left = 8
          Top = 48
          Width = 89
          Height = 33
          Caption = 'Edit Profile'
          TabOrder = 1
          OnClick = btnEditProfileClick
        end
        object btnCreateProfile: TBitBtn
          Left = 8
          Top = 8
          Width = 89
          Height = 33
          Caption = 'Create Profile'
          TabOrder = 0
          OnClick = btnCreateProfileClick
        end
      end
      object Panel4: TPanel
        Left = 5
        Top = 246
        Width = 555
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 3
        DesignSize = (
          555
          45)
        object Image1: TImage
          Left = 14
          Top = 16
          Width = 20
          Height = 19
          Picture.Data = {
            07544269746D617042020000424D420200000000000042000000280000001000
            0000100000000100100003000000000200000000000000000000000000000000
            0000007C0000E00300001F0000001F7C1F7C8C31104210421042104210421042
            1042104210421042104210428410801000400041004100410041004100410041
            00410041004100414A29104210420040007C007C007C007C007C007C007C007C
            007C007C007C007C0040082110420040007C007C007C007C007C397FFF7F397F
            007C007C007C007C0040082108210021007C007C007C007C007C397FFF7F397F
            007C007C007C007C004110421F7C1F7C0040007C007C007C007C007C397F007C
            007C007C007C0040082108211F7C1F7C0021007C007C007C007C397F397F007C
            007C007C007C004110421F7C1F7C1F7C1F7C0040007C007C007C397FFF7F397F
            007C007C0040082108211F7C1F7C1F7C1F7C0021007C007C007CFF7FFF7F397F
            007C007C004110421F7C1F7C1F7C1F7C1F7C1F7C0040007C007CFF7FFF7F397F
            007C0040082108211F7C1F7C1F7C1F7C1F7C1F7C0021007C007CFF7FFF7F397F
            007C004110421F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0040007C397F397F007C
            0040082108211F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0021007C007C007C007C
            004110421F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0040007C007C0040
            082108211F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0021007C007C0041
            8C311F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C004100410000
            1F7C1F7C1F7C1F7C1F7C1F7C1F7C}
          Transparent = True
        end
        object Label6: TLabel
          Left = 39
          Top = 17
          Width = 304
          Height = 13
          Caption = 
            '= Profile contains items that are already included in a stocktak' +
            'e'
        end
        object btnOk: TBitBtn
          Left = 364
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&OK'
          Default = True
          TabOrder = 0
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
          Left = 460
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Cancel'
          TabOrder = 1
          Kind = bkCancel
        end
      end
      object lbLocations: TListBox
        Left = 5
        Top = 5
        Width = 204
        Height = 241
        Align = alLeft
        Color = clInfoBk
        Constraints.MinWidth = 100
        ItemHeight = 13
        TabOrder = 0
        OnClick = lbLocationsClick
      end
      object Panel3: TPanel
        Left = 213
        Top = 5
        Width = 235
        Height = 241
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object tvProfiles: TTreeView
          Left = 0
          Top = 0
          Width = 235
          Height = 241
          Align = alClient
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clInfoBk
          Constraints.MinWidth = 90
          DragCursor = crDefault
          HideSelection = False
          Images = dmUtilities.imlTreeImages
          Indent = 19
          ReadOnly = True
          RightClickSelect = True
          StateImages = dmUtilities.imlTreeStateImages
          TabOrder = 0
          OnKeyPress = tvProfilesKeyPress
          OnMouseDown = tvProfilesMouseDown
        end
      end
      object Panel5: TPanel
        Left = 448
        Top = 5
        Width = 4
        Height = 241
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 4
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 48
    Top = 40
  end
  object sqlLocation: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'select Distinct(LOCATION) from STOCKLOCATION'
      'where LOCATION <> '#39#39
      'Order By LOCATION')
    Transaction = Transaction
    Left = 48
    Top = 88
  end
  object qrStockProfiles: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StocktakeProfiles.Stocktake_Profile_Key,'
      '   StocktakeProfiles.Profile_Name,'
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      ''
      '    StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category'
      ''
      'From'
      '   StocktakeProfiles Inner Join ProfileToStockGroup On'
      
        '      StocktakeProfiles.Stocktake_Profile_Key = ProfileToStockGr' +
        'oup.Stocktake_Profile_Key'
      '   Inner Join StockGroup  on'
      
        '      ProfileToStockGroup.Stock_Group_Key = StockGroup.Stock_Gro' +
        'up_Key'
      '   Inner Join StockCategory On'
      
        '      StockCategory.Stock_Category_Key = StockGroup.Stock_Catego' +
        'ry_Key'
      'Where'
      
        '   (StockCategory.Deleted = '#39'F'#39' Or StockCategory.Deleted Is Null' +
        ') And   '
      '   (StockGroup.Deleted = '#39'F'#39' Or StockGroup.Deleted Is Null) '
      '   '
      '   group by'
      '   StocktakeProfiles.Stocktake_Profile_Key,'
      '   StocktakeProfiles.Profile_Name,'
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category'
      '   '
      'Order by'
      '        upper(StocktakeProfiles.Profile_Name) asc ,'
      '        upper(StockCategory.Stock_Category) asc,'
      '         upper(StockGroup.Stock_Group) asc    ')
    Transaction = Transaction
    Left = 48
    Top = 136
  end
  object sqlInitialised: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockLocation.Initialised'
      'From'
      '   StocktakeProfiles Inner Join ProfileToStockGroup On'
      
        '      StocktakeProfiles.Stocktake_Profile_Key = ProfileToStockGr' +
        'oup.Stocktake_Profile_Key'
      '   Inner Join Stock  on'
      
        '      ProfileToStockGroup.Stock_Group_Key = Stock.Stock_Group_Ke' +
        'y'
      '   Inner Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Inner Join Stocktake On'
      '      StockLocation.Stocktake_Key = Stocktake.Stocktake_Key'
      'Where'
      
        '   StocktakeProfiles.Stocktake_Profile_Key = :StocktakeProfileKe' +
        'y'
      '   and StockLocation.Location = :Location')
    Transaction = Transaction
    Left = 48
    Top = 184
  end
end
