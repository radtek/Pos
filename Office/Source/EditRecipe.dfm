object frmEditRecipe: TfrmEditRecipe
  Left = 527
  Top = 174
  Width = 566
  Height = 596
  Caption = 'Edit Recipe'
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
    Width = 521
    Height = 545
    TabOrder = 0
    object Panel2: TPanel
      Left = 8
      Top = 352
      Width = 505
      Height = 185
      BorderWidth = 1
      BorderStyle = bsSingle
      TabOrder = 0
      object vtvStock: TVirtualStringTree
        Left = 8
        Top = 8
        Width = 481
        Height = 161
        Header.AutoSizeIndex = 0
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoColumnResize, hoDrag, hoVisible]
        TabOrder = 0
        TreeOptions.MiscOptions = [toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvStockAfterPaint
        OnBeforePaint = vtvStockBeforePaint
        OnCreateEditor = vtvStockCreateEditor
        OnEdited = vtvStockOnEdited
        OnEditing = vtvStockOnEditing
        OnFocusChanged = vtvStockFocusChanged
        OnFocusChanging = vtvStockFocusChanging
        OnGetText = vtvStockGetText
        Columns = <
          item
            Position = 0
            Width = 120
            WideText = 'Item'
          end
          item
            Position = 1
            Width = 120
            WideText = 'Location'
          end
          item
            Position = 2
            Width = 70
            WideText = 'Unit'
          end
          item
            Position = 3
            Width = 65
            WideText = 'Quantity'
          end
          item
            Position = 4
            Width = 80
            WideText = 'Average Cost'
          end
          item
            Position = 5
            Width = 20
          end>
      end
      object BitBtn6: TBitBtn
        Left = 460
        Top = 27
        Width = 25
        Height = 21
        TabOrder = 1
        Visible = False
        OnClick = RemoveItemOnClick
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
    object Panel3: TPanel
      Left = 8
      Top = 64
      Width = 249
      Height = 281
      BorderWidth = 1
      BorderStyle = bsSingle
      TabOrder = 1
      object DBGrid1: TDBGrid
        Left = 8
        Top = 8
        Width = 233
        Height = 257
        DataSource = dsRecipes
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit]
        ParentFont = False
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -13
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = [fsBold]
        OnCellClick = DBGridOnClick
        Columns = <
          item
            Expanded = False
            FieldName = 'Recipe'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            Title.Alignment = taCenter
            Title.Caption = 'Recipes'
            Width = 210
            Visible = True
          end>
      end
    end
    object Panel4: TPanel
      Left = 264
      Top = 64
      Width = 249
      Height = 281
      BorderWidth = 1
      BorderStyle = bsSingle
      TabOrder = 2
      object Label2: TLabel
        Left = 16
        Top = 148
        Width = 74
        Height = 13
        Caption = 'Recipe Total'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 125
        Top = 88
        Width = 5
        Height = 13
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 125
        Top = 60
        Width = 5
        Height = 13
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label5: TLabel
        Left = 16
        Top = 88
        Width = 94
        Height = 13
        Caption = 'Reduction Unit: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Edit1: TEdit
        Left = 16
        Top = 24
        Width = 145
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
        Text = 'Stock Item'
      end
      object BitBtn1: TBitBtn
        Left = 168
        Top = 22
        Width = 65
        Height = 25
        Caption = 'Find Item'
        TabOrder = 1
        OnClick = FindItemOnClick
      end
      object BitBtn2: TBitBtn
        Left = 128
        Top = 184
        Width = 113
        Height = 33
        Caption = 'Include'
        TabOrder = 2
        OnClick = IncludeOnClick
      end
      object BitBtn4: TBitBtn
        Left = 32
        Top = 240
        Width = 113
        Height = 33
        Caption = 'Save Recipe'
        TabOrder = 3
        OnClick = OkOnClick
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
      object BitBtn5: TBitBtn
        Left = 152
        Top = 240
        Width = 91
        Height = 33
        Caption = 'Cancel'
        TabOrder = 4
        OnClick = CancelOnClick
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
      object NumericEdit1: TNumericEdit
        Left = 16
        Top = 56
        Width = 97
        Height = 21
        Alignment = taLeftJustify
        Precision = 10
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ParentFont = False
        TabOrder = 7
        OnChange = NumericEdit1Change
        OnClick = QuantityOnClick
        OnExit = BoxExit
        OnKeyPress = NumericEdit1KeyPress
      end
      object NumericEdit2: TNumericEdit
        Left = 104
        Top = 144
        Width = 97
        Height = 21
        Alignment = taLeftJustify
        Precision = 10
        TabOrder = 5
        OnKeyPress = NumericEdit2KeyPress
      end
      object cbLocation: TComboBox
        Left = 16
        Top = 112
        Width = 145
        Height = 21
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGrayText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ItemHeight = 13
        ParentFont = False
        TabOrder = 6
        Text = 'Stock Location'
        OnDropDown = LocationOnDropDown
      end
    end
    object Panel5: TPanel
      Left = 8
      Top = 8
      Width = 505
      Height = 49
      Color = clAppWorkSpace
      TabOrder = 3
      object Label1: TLabel
        Left = 1
        Top = 1
        Width = 503
        Height = 47
        Align = alClient
        Alignment = taCenter
        Caption = 'Edit Recipe'
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
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 56
    Top = 136
  end
  object dtRecipes: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SelectSQL.Strings = (
      'Select Distinct Recipe From Recipes Where Deleted = '#39'F'#39';')
    Left = 96
    Top = 232
  end
  object dsRecipes: TDataSource
    DataSet = dtRecipes
    Left = 144
    Top = 160
  end
  object qrRecipe: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'Select Recipes_ID, Recipe, Final_Stock, Final_Stock_Code, Descri' +
        'ption, Final_Stock_Location, Required_Stock, Stock_Code, Stock_Q' +
        'ty, Stock_Location, Stock_Unit, Stock_Average_Cost, Stock_Latest' +
        '_Cost From Recipes Where Recipe = :Recipe and Deleted = :Deleted' +
        ';')
    Left = 168
    Top = 232
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Recipe'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Deleted'
        ParamType = ptUnknown
      end>
  end
  object qrDescription: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      
        'select Description, stocktake_unit,reduction_unit,conversion_fac' +
        'tor from stock where code = :code;')
    Left = 192
    Top = 168
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'code'
        ParamType = ptUnknown
      end>
  end
  object QueryRecipe: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 40
    Top = 216
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 128
    Top = 288
  end
end
