object frmUpdateSupplierCosts: TfrmUpdateSupplierCosts
  Left = 77
  Top = 70
  Width = 772
  Height = 538
  Caption = 'Update Supplier Costs'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    764
    511)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 745
    Height = 494
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      745
      494)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 735
      Height = 441
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        735
        441)
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 733
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 733
          Height = 48
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'lbeTitle'
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
      object vtvStockItem: TVirtualStringTree
        Left = 8
        Top = 56
        Width = 721
        Height = 334
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
        TabOrder = 3
        TreeOptions.MiscOptions = [toCheckSupport, toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvStockItemAfterPaint
        OnBeforePaint = vtvStockItemBeforePaint
        OnCreateEditor = vtvStockItemCreateEditor
        OnEdited = vtvStockItemEdited
        OnEditing = vtvStockItemEditing
        OnFocusChanged = vtvStockItemFocusChanged
        OnFocusChanging = vtvStockItemFocusChanging
        OnGetText = vtvStockItemGetText
        OnGetImageIndex = vtvStockItemGetImageIndex
        OnKeyDown = vtvStockItemKeyDown
        Columns = <
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 100
            WideText = 'Supplier Code'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 157
            WideText = 'Description'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 100
            WideText = 'Stock Unit'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 100
            WideText = 'Order Unit'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            Width = 100
            WideText = 'Stock Quantity'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 5
            Width = 100
            WideText = 'Cost Exc Tax'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 6
            Width = 60
            WideText = 'Preferred Supplier'
          end>
        WideDefaultText = ''
      end
      object neStockNumericEdit: TNumericEdit
        Left = 465
        Top = 73
        Width = 104
        Height = 21
        Value = 1.2345
        Precision = 15
        TabOrder = 5
        Visible = False
        OnKeyDown = KeyDownHandler
        OnKeyPress = neStockNumericEditKeyPress
        DecimalPlaces = 4
      end
      object dbcbPreferredSupplier: TComboBox
        Left = 664
        Top = 72
        Width = 65
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        Visible = False
        OnKeyDown = KeyDownHandler
      end
      object neTextEdit: TEdit
        Left = 8
        Top = 72
        Width = 97
        Height = 21
        TabOrder = 1
        Visible = False
        OnKeyDown = KeyDownHandler
      end
      object dbcbUnit: TComboBox
        Left = 368
        Top = 72
        Width = 97
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 2
        Visible = False
        OnKeyDown = KeyDownHandler
      end
    end
    object btnCancel: TBitBtn
      Left = 640
      Top = 453
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Cancel'
      TabOrder = 1
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
    object btnCommit: TBitBtn
      Left = 528
      Top = 453
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit '
      TabOrder = 2
      OnClick = btnCommitClick
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
    object btnImport: TBitBtn
      Left = 16
      Top = 453
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Import '
      TabOrder = 3
      OnClick = btnImportClick
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 328
    Top = 216
  end
  object qrGetStockUnit: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'select distinct STOCKTAKE_UNIT from '
      '(select Distinct STOCKTAKE_UNIT from STOCK'
      ' where STOCK_KEY = :STOCK_KEY '
      'union all '
      'select distinct SUPPLIER_UNIT  from SUPPLIERSTOCK'
      'where STOCK_KEY = :STOCK_KEY ) '
      'Order By '
      'Stocktake_Unit')
    Left = 168
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrGetSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      'b.SUPPLIER_STOCK_KEY, '
      'b.STOCK_KEY,'
      'a.CODE,'
      'b.SUPPLIER_KEY,'
      'b.SUPPLIER_CODE,'
      'a.DESCRIPTION,'
      'a.STOCKTAKE_UNIT,'
      'b.SUPPLIER_UNIT,'
      'b.QTY,b.LATEST_COST,'
      'b.PREFERRED_SUPPLIER '
      'from STOCK a '
      'inner join SUPPLIERSTOCK b on a.STOCK_KEY = b.STOCK_KEY '
      'where a.ENABLED = '#39'T'#39' and b.SUPPLIER_KEY = :SUPPLIER_KEY'
      'order by a.DESCRIPTION')
    Left = 248
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 208
    Top = 216
  end
  object PreferredSupplierQuery: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 288
    Top = 216
  end
  object AddSupplierStockQuery: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 374
    Top = 224
  end
  object dlgOpen: TOpenDialog
    DefaultExt = 'csv'
    Filter = 
      'Comma Separeated Values (*.csv)|*.csv|Text Files (*.txt)|*.txt|A' +
      'll Files (*.*)|*.*'
    Left = 40
    Top = 304
  end
end
