object frmCreateRecipe: TfrmCreateRecipe
  Left = 694
  Top = 149
  Width = 582
  Height = 596
  Caption = 'Create Recipe'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 521
    Height = 545
    BorderWidth = 1
    TabOrder = 0
    object Panel2: TPanel
      Left = 8
      Top = 352
      Width = 505
      Height = 185
      BorderWidth = 1
      BorderStyle = bsSingle
      TabOrder = 0
      DesignSize = (
        501
        181)
      object vtvStock: TVirtualStringTree
        Left = 8
        Top = 8
        Width = 489
        Height = 165
        Anchors = [akLeft, akTop, akBottom]
        Color = clInfoBk
        Header.AutoSizeIndex = 0
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        TabOrder = 0
        OnAfterPaint = vtvStockAfterPaint
        OnBeforePaint = vtvStockBeforePaint
        OnGetText = vtvStockGetText
        Columns = <
          item
            MaxWidth = 600
            MinWidth = 60
            Position = 0
            Width = 145
            WideText = 'Item'
          end
          item
            Position = 1
            Width = 130
            WideText = 'Location'
          end
          item
            Position = 2
            Width = 70
            WideText = 'Unit'
          end
          item
            Position = 3
            Width = 60
            WideText = 'Quantity'
          end
          item
            Position = 4
            Width = 80
            WideText = 'Average Cost'
          end>
        WideDefaultText = ''
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
      object ERecipeName: TEdit
        Left = 16
        Top = 24
        Width = 209
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ParentFont = False
        TabOrder = 0
        Text = 'Recipe Name'
        OnClick = RecipeNameOnClick
        OnExit = BoxExit
      end
      object Edit3: TEdit
        Left = 16
        Top = 208
        Width = 137
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Text = 'Final Stock'
      end
      object BitBtn1: TBitBtn
        Left = 160
        Top = 208
        Width = 67
        Height = 25
        Caption = 'Find Item'
        TabOrder = 2
        OnClick = FindItemMouseClick
      end
      object MemDescription: TMemo
        Left = 16
        Top = 64
        Width = 209
        Height = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        Lines.Strings = (
          'Description')
        ParentFont = False
        TabOrder = 3
        OnClick = DescriptionOnClick
        OnExit = BoxExit
      end
      object cbFinalLocation: TComboBox
        Left = 16
        Top = 240
        Width = 137
        Height = 21
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsItalic]
        ItemHeight = 13
        ParentFont = False
        TabOrder = 4
        Text = 'Final Stock Location'
        OnDropDown = LocationDropDown
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
        Left = 136
        Top = 90
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
        Left = 120
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
      object Label3: TLabel
        Left = 16
        Top = 155
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
      object Label5: TLabel
        Left = 16
        Top = 88
        Width = 90
        Height = 13
        Caption = 'Reduction Unit:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Edit4: TEdit
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
      object btnInclude: TBitBtn
        Left = 16
        Top = 192
        Width = 113
        Height = 33
        Caption = 'Include'
        TabOrder = 1
        OnClick = IncludeOnClick
      end
      object btnRemove: TBitBtn
        Left = 136
        Top = 192
        Width = 105
        Height = 33
        Caption = 'Remove'
        TabOrder = 2
        OnClick = RemoveOnClick
      end
      object btnOk: TBitBtn
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
      object BitBtn6: TBitBtn
        Left = 168
        Top = 24
        Width = 65
        Height = 25
        Caption = 'Find Item'
        TabOrder = 5
        OnClick = FindItemMouseClick
      end
      object cbStockLocation: TComboBox
        Left = 16
        Top = 120
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
        OnDropDown = LocationDropDown
      end
      object NumericEdit1: TNumericEdit
        Left = 104
        Top = 152
        Width = 97
        Height = 21
        Alignment = taLeftJustify
        Precision = 10
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clDefault
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object NumQuantity: TNumericEdit
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
        TabOrder = 8
        OnChange = NumQuantityChange
        OnClick = NumQuantityClick
        OnExit = BoxExit
        OnKeyPress = NumQuantityKeyPress
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
        Caption = 'Create Recipe'
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
  object qrDescription: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      
        'select description, stocktake_unit,reduction_unit,conversion_fac' +
        'tor from stock where code = :code;')
    Left = 240
    Top = 128
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'code'
        ParamType = ptUnknown
      end>
  end
  object IBTransaction1: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 40
    Top = 128
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = IBTransaction1
    Left = 488
    Top = 200
  end
  object dsDescription: TDataSource
    DataSet = qrDescription
    Left = 432
    Top = 168
  end
  object QueryRecipe: TIBQuery
    Database = dmStockData.dbStock
    Transaction = IBTransaction1
    Left = 448
    Top = 200
  end
end
