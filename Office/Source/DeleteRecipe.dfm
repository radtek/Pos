object frmDeleteRecipe: TfrmDeleteRecipe
  Left = 268
  Top = 151
  Width = 455
  Height = 425
  Caption = 'Delete Recipe'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 425
    Height = 377
    BorderWidth = 1
    BorderStyle = bsSingle
    TabOrder = 0
    object Panel2: TPanel
      Left = 8
      Top = 8
      Width = 401
      Height = 257
      BorderWidth = 1
      BorderStyle = bsSingle
      TabOrder = 0
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 401
        Height = 49
        Color = clAppWorkSpace
        TabOrder = 0
        object Label1: TLabel
          Left = 1
          Top = 1
          Width = 399
          Height = 47
          Align = alClient
          Alignment = taCenter
          Caption = 'Delete Recipe'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
        end
      end
    end
    object btnOk: TBitBtn
      Left = 192
      Top = 328
      Width = 113
      Height = 33
      Caption = 'Delete Recipe'
      TabOrder = 1
      OnClick = DeleteRecipeOnClick
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
      Left = 312
      Top = 328
      Width = 91
      Height = 33
      Caption = 'Cancel'
      TabOrder = 2
      OnClick = btnCancelOnClick
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
    object vtvStock: TVirtualStringTree
      Left = 20
      Top = 64
      Width = 377
      Height = 185
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Header.AutoSizeIndex = 0
      Header.Font.Charset = DEFAULT_CHARSET
      Header.Font.Color = clWindowText
      Header.Font.Height = -11
      Header.Font.Name = 'MS Sans Serif'
      Header.Font.Style = [fsBold]
      Header.Options = [hoColumnResize, hoDrag, hoVisible]
      ParentFont = False
      TabOrder = 3
      TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowTreeLines, toShowVertGridLines, toThemeAware, toUseBlendedImages]
      TreeOptions.SelectionOptions = [toFullRowSelect, toMultiSelect]
      OnGetText = vtvStockGetText
      Columns = <
        item
          Position = 0
          Width = 200
          WideText = 'Recipe'
        end
        item
          Position = 1
          Width = 170
          WideText = 'Final Stock'
        end>
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 80
    Top = 104
  end
  object dtRecipes: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    SelectSQL.Strings = (
      
        'Select Distinct Recipe, Final_Stock From Recipes Where Deleted =' +
        ' '#39'F'#39';')
    Left = 176
    Top = 96
  end
  object dsRecipes: TDataSource
    DataSet = dtRecipes
    Left = 120
    Top = 152
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 272
    Top = 128
  end
  object qrRecipe: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct Recipe, Recipes_ID, Final_Stock From Recipes'
      'Where Deleted = '#39'F'#39';')
    Left = 328
    Top = 120
  end
end
