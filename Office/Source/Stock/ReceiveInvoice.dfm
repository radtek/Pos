object frmReceiveInvoice: TfrmReceiveInvoice
  Left = 492
  Top = 140
  Width = 916
  Height = 450
  BorderIcons = [biSystemMenu]
  Caption = 'Receive Order Items'
  Color = clBtnFace
  Constraints.MaxWidth = 916
  Constraints.MinHeight = 332
  Constraints.MinWidth = 652
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  DesignSize = (
    908
    423)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 890
    Height = 407
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      890
      407)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 880
      Height = 354
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        880
        354)
      object Label1: TLabel
        Left = 565
        Top = 278
        Width = 113
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = 'Invoice Total Excl.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 565
        Top = 302
        Width = 113
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = 'TAX.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 565
        Top = 326
        Width = 113
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = 'Invoice Total Inc.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeTotalExc: TLabel
        Left = 693
        Top = 278
        Width = 81
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = '0.0000'
      end
      object lbeTotalInc: TLabel
        Left = 693
        Top = 326
        Width = 81
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = '0.0000'
      end
      object imgWarning: TImage
        Left = 8
        Top = 326
        Width = 16
        Height = 16
        Anchors = [akLeft, akBottom]
        AutoSize = True
        Picture.Data = {
          07544269746D617036030000424D360300000000000036000000280000001000
          0000100000000100180000000000000300000000000000000000000000000000
          0000FFFFFFFFFFFF606060808080808080808080808080808080808080808080
          808080808080808080808080808080202020002020009F9F007F7F007F7F007F
          7F007F7F007F7F007F7F007F7F007F7F007F7F007F7F007F7F50505080808080
          808000BFBF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
          00FFFF00FFFF00FFFF30EFEF40404080808000BFBF00FFFF00FFFF00FFFF00FF
          FF00FFFF30AFAF00000030AFAF00FFFF00FFFF00FFFF00FFFF30EFEF40404040
          404000404000FFFF00FFFF00FFFF00FFFF00FFFF30AFAF00000030AFAF00FFFF
          00FFFF00FFFF00FFFF306F6F808080FFFFFFFFFFFF00BFBF00FFFF00FFFF00FF
          FF00FFFF00FFFF00BFBF00FFFF00FFFF00FFFF00FFFF30EFEF404040404040FF
          FFFFFFFFFF00404000FFFF00FFFF00FFFF00FFFF30CFCF30505000FFFF00FFFF
          00FFFF00FFFF306F6F808080FFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FFFF00FF
          FF00FFFF306F6F00000030EFEF00FFFF00FFFF30EFEF404040404040FFFFFFFF
          FFFFFFFFFFFFFFFF00404000FFFF00FFFF00FFFF002020000000009F9F00FFFF
          00FFFF306F6F808080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FF
          FF00FFFF000000000000007F7F00FFFF30EFEF404040404040FFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF00404000FFFF00FFFF000000000000007F7F00FFFF
          306F6F808080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00BF
          BF00FFFF30AFAF007F7F30EFEF30EFEF404040404040FFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFF00404000FFFF00FFFF00FFFF00FFFF306F6F
          808080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00BFBF00FFFF00FFFF30EFEF404040404040FFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00404000FFFF00FFFF306F6F606060
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFF007F7F30AFAF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFF}
        Transparent = True
        Visible = False
      end
      object lbeWarning: TLabel
        Left = 32
        Top = 326
        Width = 230
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Some items have not been placed on back order'
        Visible = False
      end
      object Label4: TLabel
        Left = 13
        Top = 60
        Width = 75
        Height = 13
        Anchors = [akTop, akRight]
        Caption = 'Invoice Date:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblStockReceiptNumber: TLabel
        Left = 670
        Top = 60
        Width = 128
        Height = 13
        Anchors = [akTop, akRight]
        Caption = 'Stock Receipt Number:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblReceiptnumber: TLabel
        Left = 805
        Top = 60
        Width = 63
        Height = 13
        Anchors = [akTop, akRight]
        Caption = '123456789'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 878
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 878
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
      object vtvStockQty: TVirtualStringTree
        Left = 4
        Top = 84
        Width = 869
        Height = 179
        Anchors = [akLeft, akTop, akBottom]
        Color = clInfoBk
        Header.AutoSizeIndex = 9
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        TabOrder = 1
        TreeOptions.MiscOptions = [toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvStockQtyAfterPaint
        OnBeforePaint = vtvStockQtyBeforePaint
        OnCreateEditor = vtvStockQtyCreateEditor
        OnEdited = vtvStockQtyEdited
        OnEditing = vtvStockQtyEditing
        OnFocusChanged = vtvStockQtyFocusChanged
        OnFocusChanging = vtvStockQtyFocusChanging
        OnGetText = vtvStockQtyGetText
        OnGetImageIndex = vtvStockQtyGetImageIndex
        OnKeyDown = vtvStockQtyKeyDown
        Columns = <
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 195
            WideText = 'Description'
          end
          item
            Position = 1
            Width = 85
            WideText = 'Supplier Code'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 95
            WideText = 'Location'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 65
            WideText = 'Unit'
          end
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            Width = 65
            WideText = 'Quantity'
          end
          item
            Alignment = taRightJustify
            Options = [coAllowClick, coDraggable, coEnabled, coParentColor, coResizable, coShowDropMark, coVisible]
            Position = 5
            Width = 75
            WideText = 'Cost'
          end
          item
            Alignment = taRightJustify
            Position = 6
            Width = 75
            WideText = 'Total'
          end
          item
            Position = 7
            Width = 75
            WideText = 'Back Order'
          end
          item
            Position = 8
            Width = 65
            WideText = 'Latest Cost'
          end
          item
            Position = 9
            Width = 70
            WideText = 'On Hand'
          end>
        WideDefaultText = ''
      end
      object neStockQty: TNumericEdit
        Left = 448
        Top = 104
        Width = 62
        Height = 21
        Value = 1.23457
        Precision = 15
        TabOrder = 2
        Visible = False
        OnExit = neStockQtyExit
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neStockQtyKeyPress
        DecimalPlaces = 2
      end
      object GroupBox1: TGroupBox
        Left = 405
        Top = 270
        Width = 145
        Height = 73
        Anchors = [akRight, akBottom]
        Caption = 'All &Entered Costs'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        object rbExcludingGST: TRadioButton
          Left = 24
          Top = 20
          Width = 97
          Height = 17
          Caption = 'Excl TAX.'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          TabStop = True
          OnClick = rbGSTClick
        end
        object rbIncludingGST: TRadioButton
          Left = 24
          Top = 44
          Width = 97
          Height = 17
          Caption = 'Inc TAX.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = rbGSTClick
        end
      end
      object neCost: TNumericEdit
        Left = 512
        Top = 104
        Width = 74
        Height = 21
        Precision = 5
        FixedPoint = True
        TabOrder = 4
        Visible = False
        OnExit = neCostExit
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neCostKeyPress
        DecimalPlaces = 4
      end
      object neTotalCost: TNumericEdit
        Left = 588
        Top = 104
        Width = 69
        Height = 21
        Precision = 5
        FixedPoint = True
        TabOrder = 5
        Visible = False
        OnExit = neTotalCostExit
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neTotalCostKeyPress
        DecimalPlaces = 4
      end
      object btnAdd: TBitBtn
        Left = 8
        Top = 273
        Width = 105
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Add Any Item'
        TabOrder = 6
        OnClick = btnAddClick
        NumGlyphs = 2
      end
      object btnRemove: TBitBtn
        Left = 256
        Top = 273
        Width = 105
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Remove Item'
        TabOrder = 7
        OnClick = btnRemoveClick
        NumGlyphs = 2
      end
      object neBackOrder: TNumericEdit
        Left = 665
        Top = 104
        Width = 65
        Height = 21
        Value = 1.23457
        Precision = 15
        TabOrder = 12
        Visible = False
        OnExit = neTotalCostExit
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neBackOrderKeyPress
        DecimalPlaces = 2
      end
      object dtpInvoiceDate: TDateTimePicker
        Left = 96
        Top = 56
        Width = 185
        Height = 21
        CalAlignment = dtaLeft
        Date = 37911
        Time = 37911
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 8
      end
      object neGST: TNumericEdit
        Left = 697
        Top = 299
        Width = 77
        Height = 21
        Value = 1.23457
        Precision = 5
        FixedPoint = True
        Anchors = [akRight, akBottom]
        Enabled = False
        TabOrder = 9
        OnExit = neGSTExit
        OnKeyDown = neGSTKeyDown
        OnKeyPress = neGSTKeyPress
        DecimalPlaces = 4
      end
      object BitBtn1: TBitBtn
        Left = 136
        Top = 273
        Width = 105
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Add S&upplier Item'
        TabOrder = 11
        OnClick = BitBtn1Click
        NumGlyphs = 2
      end
      object chbGSTOverride: TCheckBox
        Left = 800
        Top = 300
        Width = 65
        Height = 17
        Anchors = [akRight, akBottom]
        Caption = 'Override'
        TabOrder = 10
        OnClick = chbGSTOverrideClick
      end
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 366
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 1
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
    object btnOk: TBitBtn
      Left = 495
      Top = 366
      Width = 125
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Commit Invoice'
      TabOrder = 2
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
      Left = 755
      Top = 366
      Width = 125
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 3
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
    object btnCommitPackingSlip: TBitBtn
      Left = 103
      Top = 366
      Width = 125
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Commit Packing Slip'
      TabOrder = 4
      OnClick = btnCommitPackingSlipClick
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
    object btnPrintCommitInvoice: TBitBtn
      Left = 339
      Top = 366
      Width = 145
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Commit && Print Invoice'
      TabOrder = 5
      OnClick = btnPrintCommitInvoiceClick
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
    object btnSave: TBitBtn
      Left = 632
      Top = 366
      Width = 115
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Save Packing Slip'
      TabOrder = 6
      OnClick = btnSaveClick
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
  end
  object btnPrintInvoice: TButton
    Left = 245
    Top = 374
    Width = 91
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = '&Print Invoice'
    TabOrder = 3
    OnClick = btnPrintInvoiceClick
  end
  object dbcbUnit: TComboBox
    Left = 392
    Top = 118
    Width = 67
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    Visible = False
    OnChange = dbcbUnitChange
  end
  object dbcbLocation: TComboBox
    Left = 320
    Top = 118
    Width = 67
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    Visible = False
    OnChange = dbcbLocationChange
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 64
    Top = 128
  end
  object qrPurchaseStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   PurchaseStock.Purchase_Stock_LK Purchase_Stock_Key,'
      '   PurchaseStock.Code,'
      '   Stock.Stock_Key,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.GST_Percent,'
      '   StockCategory.GL_Code,'
      '   PurchaseStock.Location,'
      '   PurchaseStock.Supplier_Code,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit_Cost,'
      '   PurchaseStock.Supplier_Unit_Size,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order,'
      '   PurchaseStock.SUPPLIER_UNIT_CHANGED,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST'
      'From'
      '   PurchaseStock Left Join Stock on'
      '       PurchaseStock.Code = Stock.Code'
      '   Left Join StockLocation On'
      
        '      (StockLocation.Location = PurchaseStock.Location And  Stoc' +
        'kLocation.Stock_Key = Stock.Stock_Key)'
      
        '   Left Join Stocktake On StockLocation.Stocktake_Key = Stocktak' +
        'e.Stocktake_Key'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Order_FK = :Order_Key'
      'Order By'
      '   Description,'
      '   Location,'
      '   Supplier_Unit   '
      ''
      '   ')
    Left = 232
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 64
    Top = 240
  end
  object qrPurchaseOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_FK,'
      '   Order_Number,'
      '   Created'
      'From'
      '   PurchaseOrder'
      'Where'
      '   Order_LK = :Order_Key')
    Left = 232
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_LK,'
      '   Company_Name,'
      '   Supplier_ID,'
      '   MYOB1,'
      '   MYOB2,'
      '   MYOB3'
      'From'
      '   Contact'
      'Where'
      '   Contact_LK = :Supplier_Key')
    Left = 120
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end>
  end
  object qrStockTransaction: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      '    ST.TRANS_NUMBER,'
      '    ST.BATCH_KEY,'
      '    ST.DESCRIPTION,'
      '    ST.CODE,'
      '    ST.STOCK_CATEGORY,'
      '    ST.ORDER_UNIT,'
      '    ST.STOCK_GROUP,'
      '    ST.GST_PERCENT,'
      '    S.STOCK_KEY,'
      '    ST.GL_CODE,'
      '    ST.LOCATION,'
      '    ST.SUPPLIER_CODE,'
      '    ST.SUPPLIER_KEY,'
      '    ST.UNIT AS SUPPLIER_UNIT,'
      '    ST.ORDER_UNIT AS UNIT,'
      '    ST.UNIT_COST AS SUPPLIER_UNIT_COST,'
      '    (ST.TOTAL_COST / ST.ORDER_QTY) AS UNIT_COST,'
      '    ST.ORDER_QTY,'
      ' ST.QTY AS SUPPLIER_QUANTITY,'
      '      STA.SUPPLIER_UNIT_SIZE, '
      '  '
      '    --SS.QTY AS SUPPLIER_UNIT_SIZE,'
      ' SS.SUPPLIER_KEY AS SUPPLIER_KEY,'
      '    ST.TOTAL_COST,'
      '    SL.ON_HAND,'
      '    SL.LATEST_COST,'
      'ST.CREATED, '
      '   ST.STOCK_REQUEST_STOCK_KEY_VAL'
      'FROM STOCKTRANS ST'
      'inner join (SELECT '
      ' ST.BATCH_KEY,'
      
        'MIN(CASE WHEN ST.UNIT =ST.ORDER_UNIT THEN (ST.QTY/ST.ORDER_QTY) ' +
        'ELSE  SS.QTY END) AS SUPPLIER_UNIT_SIZE '
      'FROM STOCKTRANS  ST'
      'INNER JOIN STOCK S ON ST.CODE = S.CODE'
      
        'INNER JOIN SUPPLIERSTOCK SS ON S.STOCK_KEY = SS.STOCK_KEY AND ST' +
        '.SUPPLIER_KEY = SS.SUPPLIER_KEY'
      ' WHERE ST.BATCH_KEY = :Batch_Key'
      'group by 1) STA on STA.BATCH_KEY=ST.BATCH_KEY'
      'INNER JOIN STOCK S ON ST.CODE = S.CODE'
      
        'INNER JOIN SUPPLIERSTOCK SS ON S.STOCK_KEY = SS.STOCK_KEY AND ST' +
        '.SUPPLIER_KEY = SS.SUPPLIER_KEY'
      
        'INNER JOIN STOCKLOCATION SL on ST.LOCATION = SL.LOCATION AND S.S' +
        'TOCK_KEY = SL.STOCK_KEY'
      'WHERE ST.BATCH_KEY = :Batch_Key'
      'group by'
      'ST.TRANS_NUMBER,'
      '    ST.BATCH_KEY,'
      '    ST.DESCRIPTION,'
      '    ST.CODE,'
      '    ST.STOCK_CATEGORY,'
      '    ST.ORDER_UNIT,'
      '    ST.STOCK_GROUP,'
      '    ST.GST_PERCENT,'
      '    S.STOCK_KEY,'
      '    ST.GL_CODE,'
      '    ST.LOCATION,'
      '    ST.SUPPLIER_CODE,'
      '    ST.SUPPLIER_KEY,'
      '    ST.UNIT ,'
      '    ST.ORDER_UNIT ,'
      '    ST.UNIT_COST ,'
      '    ST.ORDER_QTY,'
      '    ST.QTY, '
      '    --SS.QTY, '
      ' SS.SUPPLIER_KEY,'
      '    ST.TOTAL_COST,'
      '    SL.ON_HAND,'
      '    SL.LATEST_COST,'
      '   ST.STOCK_REQUEST_STOCK_KEY_VAL, '
      '    ST.CREATED , STA.SUPPLIER_UNIT_SIZE'
      '')
    Left = 376
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Batch_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Batch_Key'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateOrder: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 160
    Top = 224
  end
  object qrSelectLocationUpdate: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   PurchaseStock.Purchase_Stock_LK Purchase_Stock_Key,'
      '   PurchaseStock.Code,'
      '   Stock.Stock_Key,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.GST_Percent,'
      '   StockCategory.GL_Code,'
      '   PurchaseStock.Location,'
      '   PurchaseStock.Supplier_Code,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit_Cost,'
      '   PurchaseStock.Supplier_Unit_Size,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order,'
      '   PurchaseStock.SUPPLIER_UNIT_CHANGED,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST ,Order_FK'
      'From'
      '   PurchaseStock Left Join Stock on'
      '       PurchaseStock.Code = Stock.Code'
      '   Left Join StockLocation On'
      
        '      (StockLocation.Location = PurchaseStock.Location And  Stoc' +
        'kLocation.Stock_Key = Stock.Stock_Key)'
      
        '   Left Join Stocktake On StockLocation.Stocktake_Key = Stocktak' +
        'e.Stocktake_Key'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      
        '   Stock.Stock_Key=:Stock_Key and PurchaseStock.Supplier_Unit=:S' +
        'upplier_Unit and PurchaseStock.Location=:Location and Stock.Desc' +
        'ription=:Description'
      ''
      'Order By'
      ' '
      'StockLocation.ON_HAND ,'
      'PurchaseStock.Supplier_Unit_Cost    ')
    Left = 480
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Description'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierOrderQty: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT, a.SUPPLIER_CODE, a.PREFERRED_SUPPLIER, a.DEFAULT_ORDE' +
        'R_UNIT, a.QTY, a.LATEST_COST, a.MIN_ORDER_QTY'
      'FROM SUPPLIERSTOCK a'
      
        'where a.SUPPLIER_KEY=:SUPPLIER_KEY and a.SUPPLIER_UNIT=:SUPPLIER' +
        '_UNIT '
      'AND A.STOCK_KEY=:STOCK_KEY')
    Left = 608
    Top = 184
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrLocationUpdate: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT, a.SUPPLIER_CODE, a.PREFERRED_SUPPLIER, a.DEFAULT_ORDE' +
        'R_UNIT, a.QTY, a.MIN_ORDER_QTY,loc.ON_HAND,loc.LOCATION,loc.LATE' +
        'ST_COST'
      'FROM SUPPLIERSTOCK a'
      'inner join STOCKLOCATION loc on loc.STOCK_KEY=a.STOCK_KEY'
      
        'where a.SUPPLIER_KEY=:SUPPLIER_KEY and loc.LOCATION=:LOCATION an' +
        'd QTY = :QTY and  a.STOCK_KEY = :STOCK_KEY'
      'group by'
      
        ' a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPPLIER_U' +
        'NIT, a.SUPPLIER_CODE, a.PREFERRED_SUPPLIER, a.DEFAULT_ORDER_UNIT' +
        ', a.QTY, a.MIN_ORDER_QTY,loc.ON_HAND,loc.LOCATION,loc.LATEST_COS' +
        'T'
      '')
    Left = 672
    Top = 184
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
      end
      item
        DataType = ftUnknown
        Name = 'QTY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrPurchaseStockAuto: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   PurchaseStock.Purchase_Stock_LK Purchase_Stock_Key,'
      '   PurchaseStock.Code,'
      '   Stock.Stock_Key,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.GST_Percent,'
      '   StockCategory.GL_Code,'
      '   PurchaseStock.Location,'
      '   PurchaseStock.Supplier_Code,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit_Cost,'
      '   PurchaseStock.Supplier_Unit_Size,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order,'
      '   PurchaseStock.SUPPLIER_UNIT_CHANGED,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST'
      'From'
      '   PurchaseStock Left Join Stock on'
      '       PurchaseStock.Code = Stock.Code'
      '   Left Join StockLocation On'
      
        '      (StockLocation.Location = PurchaseStock.Location And  Stoc' +
        'kLocation.Stock_Key = Stock.Stock_Key)'
      
        '   Left Join Stocktake On StockLocation.Stocktake_Key = Stocktak' +
        'e.Stocktake_Key'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Order_FK = :Order_Key and    Stock.Stock_Key=:Stock_Key  and '
      '   PurchaseStock.Supplier_Unit  = :Supplier_Unit  '
      '   Order By'
      '   Description,'
      '   Location,'
      '   Supplier_Unit '
      ''
      '   '
      ' ')
    Left = 280
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end>
  end
  object qrUnitChanged: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT,  a.QTY, a.LATEST_COST '
      'FROM SUPPLIERSTOCK a'
      'left join  STOCK on STOCK.STOCK_KEY=a.STOCK_KEY'
      'where a.SUPPLIER_KEY=:SUPPLIER_KEY  '
      'and a.STOCK_KEY=:STOCK_KEY '
      'and a.SUPPLIER_UNIT=:SUPPLIER_UNIT')
    Left = 296
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end>
  end
  object qrUnitqty: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT,  a.QTY, a.LATEST_COST '
      'FROM SUPPLIERSTOCK a'
      'left join  STOCK on STOCK.STOCK_KEY=a.STOCK_KEY'
      'where a.SUPPLIER_KEY=:SUPPLIER_KEY  '
      'and a.STOCK_KEY=:STOCK_KEY '
      'and a.SUPPLIER_UNIT=:SUPPLIER_UNIT'
      ' ')
    Left = 328
    Top = 160
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end>
  end
  object qrBatchKey: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT, a.SUPPLIER_CODE, a.PREFERRED_SUPPLIER, a.DEFAULT_ORDE' +
        'R_UNIT, a.QTY, a.LATEST_COST, a.MIN_ORDER_QTY'
      'FROM SUPPLIERSTOCK a'
      
        'where a.SUPPLIER_KEY=:SUPPLIER_KEY and a.SUPPLIER_UNIT=:SUPPLIER' +
        '_UNIT '
      'AND A.STOCK_KEY=:STOCK_KEY')
    Left = 728
    Top = 176
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrDeleteStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Delete From STOCKTRANS st '
      'Where st.TRANS_NUMBER = :TRANS_NUMBER ')
    Transaction = Transaction
    Left = 520
    Top = 152
  end
  object qrCheckCommitted: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      #9'ISCOMMITTED_PACKINGSLIP'
      'From'
      #9'TRANSACTIONBATCH'
      'Where'
      '    TRANSACTIONBATCH.SUPPLIER_KEY = :SUPPLIER_KEY and '
      
        '    TRANSACTIONBATCH.ISCOMMITTED_PACKINGSLIP =     :ISCOMMITTED_' +
        'PACKINGSLIP and '
      '    TRANSACTIONBATCH.REFERENCE =  :REFERENCE')
    Left = 568
    Top = 224
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ISCOMMITTED_PACKINGSLIP'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REFERENCE'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierKey: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT a.SUPPLIER_UNIT, a.QTY'
      'FROM SUPPLIERSTOCK a'
      'where a.SUPPLIER_KEY=:SUPPLIER_KEY and a.STOCK_KEY=:STOCK_KEY')
    Left = 432
    Top = 176
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrCheckSupplierUnit: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT, a.SUPPLIER_CODE, a.PREFERRED_SUPPLIER, a.DEFAULT_ORDE' +
        'R_UNIT, a.QTY, a.LATEST_COST, a.MIN_ORDER_QTY'
      'FROM SUPPLIERSTOCK a'
      
        'where a.SUPPLIER_KEY=:SUPPLIER_KEY and a.SUPPLIER_UNIT=:SUPPLIER' +
        '_UNIT '
      'AND A.STOCK_KEY=:STOCK_KEY')
    Left = 776
    Top = 144
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Supplier_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_UNIT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrPurchaseStockTakeAuto: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   PurchaseStock.Purchase_Stock_LK Purchase_Stock_Key,'
      '   PurchaseStock.Code,'
      '   Stock.Stock_Key,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.GST_Percent,'
      '   StockCategory.GL_Code,'
      '   PurchaseStock.Location,'
      '   PurchaseStock.Supplier_Code,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit_Cost,'
      '   PurchaseStock.Supplier_Unit_Size,'
      '   PurchaseStock.Qty_Received,'
      '   PurchaseStock.Qty_On_Order,'
      '   PurchaseStock.SUPPLIER_UNIT_CHANGED,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time,'
      '   StockLocation.ON_HAND,'
      '   StockLocation.LATEST_COST'
      'From'
      '   PurchaseStock Left Join Stock on'
      '       PurchaseStock.Code = Stock.Code'
      '   Left Join StockLocation On'
      
        '      (StockLocation.Location = PurchaseStock.Location And  Stoc' +
        'kLocation.Stock_Key = Stock.Stock_Key)'
      
        '   Left Join Stocktake On StockLocation.Stocktake_Key = Stocktak' +
        'e.Stocktake_Key'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Order_FK = :Order_Key and    Stock.Stock_Key=:Stock_Key  and '
      '   PurchaseStock.Supplier_Unit  = :Supplier_Unit'
      '   and Stock.Stocktake_Unit = :Stocktake_Unit    '
      '   Order By'
      '   Description,'
      '   Location,'
      '   Supplier_Unit '
      '   '
      ' ')
    Left = 320
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Stocktake_Unit'
        ParamType = ptUnknown
      end>
  end
  object qrGetStockUnitSize: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'select distinct STOCKTAKE_UNIT SUPPLIER_UNIT, 1 as QTY   from ST' +
        'OCK left join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STO' +
        'CK_KEY Where STOCK.STOCK_KEY = :STOCK_KEY and SUPPLIERSTOCK.SUPP' +
        'LIER_KEY=:SUPPLIER_KEY')
    Left = 672
    Top = 224
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateDateInStockTrans: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update STOCKTRANS a '
      'Set  a.CREATED =:CREATED'
      'Where  a.BATCH_KEY=:BATCH_KEY'
      '')
    Left = 744
    Top = 168
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CREATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'BATCH_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateDateInTransactionBatch: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update TRANSACTIONBATCH a '
      'Set  a.CREATED =:CREATED,'
      ' a.REFERENCE =:REFERENCE'
      'Where  a.BATCH_KEY=:BATCH_KEY'
      '')
    Left = 784
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CREATED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REFERENCE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'BATCH_KEY'
        ParamType = ptUnknown
      end>
  end
end
