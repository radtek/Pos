object frmSelectStocktake: TfrmSelectStocktake
  Left = 400
  Top = 145
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Select a Stocktake'
  ClientHeight = 435
  ClientWidth = 663
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 663
    Height = 435
    Align = alClient
    BorderWidth = 4
    TabOrder = 0
    object Panel3: TPanel
      Left = 554
      Top = 5
      Width = 104
      Height = 380
      Align = alRight
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object btnNewStocktake: TBitBtn
        Left = 7
        Top = 8
        Width = 90
        Height = 41
        Caption = 'New'
        TabOrder = 0
        OnClick = btnNewStocktakeClick
        NumGlyphs = 2
      end
      object btnDeleteStocktake: TBitBtn
        Left = 8
        Top = 56
        Width = 90
        Height = 41
        Caption = 'Delete'
        TabOrder = 1
        OnClick = btnDeleteStocktakeClick
        NumGlyphs = 2
      end
    end
    object Panel4: TPanel
      Left = 5
      Top = 385
      Width = 653
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        653
        45)
      object btnOk: TBitBtn
        Left = 462
        Top = 10
        Width = 89
        Height = 31
        Anchors = [akRight, akBottom]
        Caption = '&OK'
        Default = True
        Enabled = False
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
        Left = 558
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = '&Cancel'
        TabOrder = 1
        Kind = bkCancel
      end
    end
    object Panel7: TPanel
      Left = 550
      Top = 5
      Width = 4
      Height = 380
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 2
    end
    object Panel10: TPanel
      Left = 5
      Top = 5
      Width = 540
      Height = 380
      Align = alLeft
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 3
      object vtvStocktake: TVirtualStringTree
        Left = 1
        Top = 34
        Width = 538
        Height = 345
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clInfoBk
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Header.AutoSizeIndex = -1
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Tahoma'
        Header.Font.Style = []
        Header.Options = [hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        ParentFont = False
        TabOrder = 1
        TreeOptions.MiscOptions = [toGridExtensions, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toHideFocusRect, toShowButtons, toShowRoot, toShowTreeLines, toShowVertGridLines]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvStocktakeAfterPaint
        OnBeforePaint = vtvStocktakeBeforePaint
        OnChange = vtvStocktakeChange
        OnFocusChanging = vtvStocktakeFocusChanging
        OnGetText = vtvStocktakeGetText
        OnGetImageIndex = vtvStocktakeGetImageIndex
        Columns = <
          item
            MinWidth = 190
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 190
            WideText = 'Name'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coVisible]
            Position = 1
            Width = 100
            WideText = 'Location'
          end
          item
            MinWidth = 71
            Options = [coEnabled, coParentBidiMode, coParentColor, coVisible]
            Position = 2
            Width = 137
            WideText = 'Initialised'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coVisible]
            Position = 3
            Width = 94
            WideText = 'By'
          end>
        WideDefaultText = ''
      end
      object Panel11: TPanel
        Left = 1
        Top = 1
        Width = 538
        Height = 33
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object Label3: TLabel
          Left = 16
          Top = 0
          Width = 233
          Height = 33
          AutoSize = False
          Caption = 'Current Stocktakes'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
    end
  end
  object qrStoctakeItems: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description'
      'From'
      
        '   StockLocation Inner Join Stock On Stock.Stock_Key = StockLoca' +
        'tion.Stock_Key'
      
        '   Inner Join StockGroup On Stock.Stock_Group_Key = StockGroup.S' +
        'tock_Group_Key'
      
        '   Inner Join StockCategory On StockCategory.Stock_Category_Key ' +
        '= StockGroup.Stock_Category_Key'
      'Where'
      '   StockLocation.Stocktake_Key = :Stocktake_Key'
      'group by'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description   ,'
      '   StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order'
      '   '
      'Order By'
      'StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order,'
      ' upper(Stock.Description) asc')
    Transaction = Transaction
    Left = 208
    Top = 104
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 64
    Top = 112
  end
end
