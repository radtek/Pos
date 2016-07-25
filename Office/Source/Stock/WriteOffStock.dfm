object frmWriteOffStock: TfrmWriteOffStock
  Tag = 1024
  Left = 107
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Write Off Stock Item'
  ClientHeight = 477
  ClientWidth = 560
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
    Top = 8
    Width = 561
    Height = 465
    BorderWidth = 4
    TabOrder = 0
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 551
      Height = 396
      Align = alTop
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 549
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 16
          Top = 0
          Width = 401
          Height = 41
          AutoSize = False
          Caption = 'Write Off'
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
      object sgWriteOffs: TStringGrid
        Left = 16
        Top = 48
        Width = 513
        Height = 105
        Color = clInfoBk
        ColCount = 6
        DefaultColWidth = 60
        DefaultRowHeight = 18
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goRangeSelect, goColSizing, goRowMoving, goRowSelect]
        TabOrder = 1
        OnSelectCell = sgWriteOffsSelectCell
        ColWidths = (
          60
          60
          77
          54
          56
          213)
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 168
        Width = 521
        Height = 217
        Caption = 'Write Off Details'
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 2
        object Label7: TLabel
          Left = 24
          Top = 132
          Width = 42
          Height = 13
          Caption = 'Quantity'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 24
          Top = 80
          Width = 19
          Height = 13
          Caption = 'Unit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 368
          Top = 84
          Width = 41
          Height = 13
          Caption = 'Cost per'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 360
          Top = 112
          Width = 47
          Height = 13
          Caption = 'Total cost'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 24
          Top = 104
          Width = 40
          Height = 13
          Caption = 'Location'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 24
          Top = 52
          Width = 22
          Height = 13
          Caption = 'Item'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 24
          Top = 20
          Width = 23
          Height = 13
          Caption = 'Date'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label6: TLabel
          Left = 24
          Top = 165
          Width = 45
          Height = 13
          Caption = 'Comment'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lUnit: TLabel
          Left = 376
          Top = 96
          Width = 21
          Height = 13
          Caption = 'lUnit'
        end
        object Label5: TLabel
          Left = 352
          Top = 140
          Width = 70
          Height = 13
          Caption = 'Write Off total'
        end
        object btnAddSupplierUnit: TBitBtn
          Left = 368
          Top = 186
          Width = 65
          Height = 25
          Caption = 'Include'
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnAddOnClick
        end
        object neCost1: TNumericEdit
          Left = 440
          Top = 80
          Width = 65
          Height = 21
          Precision = 15
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          DecimalPlaces = 2
        end
        object neCost3: TNumericEdit
          Left = 440
          Top = 108
          Width = 65
          Height = 21
          Precision = 15
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 3
          DecimalPlaces = 2
        end
        object neQty: TNumericEdit
          Left = 128
          Top = 128
          Width = 65
          Height = 21
          Alignment = taLeftJustify
          Precision = 15
          Color = clInfoBk
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 1
          OnChange = QuantityOnChange
          DecimalPlaces = 4
        end
        object Edit1: TEdit
          Left = 128
          Top = 160
          Width = 225
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object DateTimePicker1: TDateTimePicker
          Left = 128
          Top = 16
          Width = 153
          Height = 21
          CalAlignment = dtaLeft
          Date = 40757.3923384259
          Time = 40757.3923384259
          DateFormat = dfShort
          DateMode = dmComboBox
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          Kind = dtkDate
          ParseInput = False
          ParentFont = False
          TabOrder = 5
        end
        object cbLocations: TComboBox
          Left = 128
          Top = 100
          Width = 153
          Height = 21
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          TabOrder = 6
          OnSelect = LocationOnSelect
        end
        object SuppliedUnit: TEdit
          Left = 128
          Top = 74
          Width = 153
          Height = 21
          Enabled = False
          TabOrder = 7
        end
        object btnRemove: TBitBtn
          Left = 440
          Top = 186
          Width = 65
          Height = 25
          Caption = 'Remove'
          Enabled = False
          TabOrder = 8
          OnClick = btnRemoveOnClick
        end
        object neTotal: TNumericEdit
          Left = 440
          Top = 136
          Width = 65
          Height = 21
          Precision = 10
          FixedPoint = True
          Color = clInfoBk
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 9
          DecimalPlaces = 2
        end
        object EItem: TEdit
          Left = 128
          Top = 45
          Width = 153
          Height = 21
          Enabled = False
          TabOrder = 10
        end
        object btnAddItem: TBitBtn
          Left = 288
          Top = 43
          Width = 75
          Height = 25
          Caption = 'Add Item'
          TabOrder = 11
          OnClick = btnAddItemOnClick
        end
      end
    end
    object btnOk: TBitBtn
      Left = 352
      Top = 416
      Width = 81
      Height = 33
      Caption = 'OK'
      Default = True
      Enabled = False
      TabOrder = 1
      OnClick = btnokOnClick
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
      Left = 440
      Top = 416
      Width = 81
      Height = 33
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 2
      OnClick = btncancelOnClick
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
  object qrLocations: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select Location, Latest_Cost From StockLocation'
      'Where Stock_Key = :Stock_Key;')
    Left = 128
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object dsLocation: TDataSource
    DataSet = qrLocations
    Left = 192
    Top = 104
  end
  object IBTransaction1: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 40
    Top = 104
  end
  object qrItem: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select Stock_Key, Stock_Group_Key, Description, Stocktake_Unit '
      'From Stock'
      'Where Code = :Code;')
    Left = 240
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end>
  end
  object qrPrice: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select Latest_Cost From StockLocation'
      'Where Stock_Key = :Stock_Key and Location = :Location')
    Left = 288
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object dsSuppliedUnit: TDataSource
    DataSet = qrPrice
    Left = 344
    Top = 104
  end
  object qrStockLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'update stocklocation'
      
        'set on_hand = on_hand - :Quantity, writeoff = writeoff - :Quanti' +
        'ty'
      'where Location = :Location and Stock_Key = :Stock_Key;')
    Left = 408
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Quantity'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Quantity'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object IBQuery2: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select Stock_Key, Code from Stock '
      'Where Description = :Description;')
    Left = 456
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Description'
        ParamType = ptUnknown
      end>
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = IBTransaction1
    Left = 496
    Top = 104
  end
end
