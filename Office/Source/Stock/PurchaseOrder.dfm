object frmPurchaseOrder: TfrmPurchaseOrder
  Tag = 1024
  Left = 125
  Top = 96
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Purchase Order'
  ClientHeight = 486
  ClientWidth = 777
  Color = clBtnFace
  Constraints.MaxWidth = 785
  Constraints.MinHeight = 492
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
    777
    486)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 761
    Height = 478
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      761
      478)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 751
      Height = 425
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        751
        425)
      object Label6: TLabel
        Left = 560
        Top = 365
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
      object Label7: TLabel
        Left = 560
        Top = 381
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
      object Label8: TLabel
        Left = 560
        Top = 405
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
        Left = 680
        Top = 365
        Width = 65
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = '$0.00'
      end
      object lbeGST: TLabel
        Left = 680
        Top = 381
        Width = 65
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = '$0.00'
      end
      object lbeTotalInc: TLabel
        Left = 680
        Top = 405
        Width = 65
        Height = 13
        Alignment = taRightJustify
        Anchors = [akRight, akBottom]
        AutoSize = False
        Caption = '$0.00'
      end
      object Label1: TLabel
        Left = 8
        Top = 64
        Width = 98
        Height = 13
        Caption = 'Supplier Address:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label14: TLabel
        Left = 384
        Top = 64
        Width = 99
        Height = 13
        Caption = 'Delivery Address:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label9: TLabel
        Left = 8
        Top = 164
        Width = 82
        Height = 13
        Caption = 'Order Number:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label11: TLabel
        Left = 8
        Top = 192
        Width = 84
        Height = 13
        Caption = 'Date Required:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label16: TLabel
        Left = 384
        Top = 144
        Width = 72
        Height = 13
        Caption = 'Instructions:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 8
        Top = 220
        Width = 73
        Height = 13
        Caption = 'Supplier FAX:'
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
        Width = 749
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 749
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
      object btnAdd: TBitBtn
        Left = 8
        Top = 365
        Width = 89
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Add Any Item'
        TabOrder = 1
        OnClick = btnAddClick
        NumGlyphs = 2
      end
      object btnRemove: TBitBtn
        Left = 216
        Top = 365
        Width = 97
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = '&Remove Item'
        TabOrder = 2
        OnClick = btnRemoveClick
        NumGlyphs = 2
      end
      object btnSelectDeliveryAdd: TBitBtn
        Left = 384
        Top = 80
        Width = 113
        Height = 33
        Caption = 'Selec&t'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Visible = False
        OnClick = btnSelectDeliveryAddClick
      end
      object btnNextOrderNumber: TBitBtn
        Left = 288
        Top = 158
        Width = 81
        Height = 25
        Caption = '&Get Next'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = btnNextOrderNumberClick
      end
      object btnRequiredDate: TBitBtn
        Left = 288
        Top = 186
        Width = 81
        Height = 25
        Caption = '&Select'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = btnRequiredDateClick
      end
      object memInstructions: TMemo
        Left = 384
        Top = 160
        Width = 361
        Height = 73
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 6
      end
      object btnAuto: TBitBtn
        Left = 320
        Top = 365
        Width = 97
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'A&uto'
        TabOrder = 7
        OnClick = btnAutoClick
      end
      object vtvStockQty: TVirtualStringTree
        Left = 8
        Top = 248
        Width = 737
        Height = 110
        Anchors = [akLeft, akTop, akBottom]
        Color = clInfoBk
        Header.AutoSizeIndex = 2
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        TabOrder = 8
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
        OnKeyDown = vtvStockQtyKeyDown
        Columns = <
          item
            MaxWidth = 300
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 200
            WideText = 'Description'
          end
          item
            Position = 1
            Width = 90
            WideText = 'Supplier Code'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 163
            WideText = 'Location'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 85
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
            Width = 65
            WideText = 'Cost'
          end
          item
            Alignment = taRightJustify
            Position = 6
            Width = 65
            WideText = 'Total'
          end>
        WideDefaultText = ''
      end
      object neStockQty: TNumericEdit
        Left = 552
        Top = 384
        Width = 62
        Height = 21
        Value = 1.23457
        Precision = 15
        TabOrder = 9
        Visible = False
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neStockQtyKeyPress
        DecimalPlaces = 4
      end
      object neCost: TNumericEdit
        Left = 616
        Top = 384
        Width = 62
        Height = 21
        Value = 1.23
        Precision = 15
        NumericType = ntCurrency
        TabOrder = 10
        Visible = False
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neStockQtyKeyPress
        DecimalPlaces = 4
      end
      object neTotalCost: TNumericEdit
        Left = 680
        Top = 384
        Width = 62
        Height = 21
        Value = 1.23
        Precision = 15
        NumericType = ntCurrency
        TabOrder = 11
        Visible = False
        OnKeyDown = neStockQtyKeyDown
        OnKeyPress = neStockQtyKeyPress
        DecimalPlaces = 2
      end
      object memSupplierAddress: TMemo
        Left = 128
        Top = 64
        Width = 241
        Height = 81
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 12
        WordWrap = False
      end
      object memDeliveryAddress: TMemo
        Left = 504
        Top = 64
        Width = 241
        Height = 81
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 13
        WordWrap = False
      end
      object edOrderNumber: TEdit
        Left = 128
        Top = 160
        Width = 153
        Height = 21
        Color = clInfoBk
        TabOrder = 14
      end
      object edDateRequired: TEdit
        Left = 128
        Top = 188
        Width = 153
        Height = 21
        Color = clInfoBk
        TabOrder = 15
        OnKeyDown = edDateRequiredKeyDown
      end
      object edFAX: TEdit
        Left = 128
        Top = 216
        Width = 153
        Height = 21
        Color = clInfoBk
        TabOrder = 16
        OnKeyDown = edDateRequiredKeyDown
      end
      object BitBtn1: TBitBtn
        Left = 104
        Top = 365
        Width = 105
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Add Su&pplier Item'
        TabOrder = 17
        OnClick = BitBtn1Click
        NumGlyphs = 2
      end
    end
    object btnSave: TBitBtn
      Left = 552
      Top = 437
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnOkClick
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00550000000005
        555555777777777FF5555500000000805555557777777777FF555550BBBBB008
        05555557F5FFF7777FF55550B000B03080555557F77757F777F55550BBBBB033
        00555557F55557F577555550BBBBB03305555557FFFFF7F57F55555000000033
        05555557777777F57F555550BBBBB03305555557F5FFF7F57F555550B000B033
        05555557F77757F57F555550BBBBB03305555557F55557F57F555550BBBBB033
        05555557FFFFF7FF7F55550000000003055555777777777F7F55550777777700
        05555575FF5555777F5555500B3B3B300555555775FF55577FF555555003B3B3
        005555555775FFFF77F555555570000000555555555777777755}
      NumGlyphs = 2
    end
    object btnCancel: TBitBtn
      Left = 656
      Top = 437
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
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
    object btnCommit: TBitBtn
      Left = 16
      Top = 437
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit'
      TabOrder = 3
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
    object btnCommitEmail: TBitBtn
      Tag = 1
      Left = 128
      Top = 437
      Width = 129
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit and Email'
      TabOrder = 4
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
    object btnCommitAndSubmitWeborder: TBitBtn
      Tag = 1
      Left = 272
      Top = 437
      Width = 185
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit and submit web-order'
      Enabled = False
      TabOrder = 5
      OnClick = btnCommitAndSubmitWeborderClick
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
  object btnCommitOld: TBitBtn
    Left = 792
    Top = 424
    Width = 97
    Height = 33
    Caption = 'Commit'
    TabOrder = 1
    OnClick = btnOkClick
  end
  object btnOk: TBitBtn
    Left = 792
    Top = 464
    Width = 97
    Height = 33
    Caption = 'Save'
    TabOrder = 2
    OnClick = btnOkClick
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 56
    Top = 280
  end
  object qrSupplier: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_LK,'
      '   Company_Name,'
      '   BILL_ADDRESS Postal_Address,'
      '   Location_Address,'
      '   FAX,'
      'EMAIL'
      'From'
      '   Contact'
      'Where'
      '   Contact_LK = :Contact_Key')
    Left = 160
    Top = 96
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_Key'
        ParamType = ptUnknown
      end>
  end
  object dtUser: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    RefreshSQL.Strings = (
      'Select'
      '   USER_NAME,'
      '   USER_ID,'
      '   NEXT_ORDER_NUMBER,'
      '   ORDER_PREFIX,'
      '   ORDER_SUFFIX'
      'From'
      '   USERS'
      'Where'
      '  USER_ID = :USER_ID')
    SelectSQL.Strings = (
      'Select'
      '   User_Name,'
      '   User_ID,'
      '   Next_Order_Number,'
      '   Order_Prefix,'
      '   Order_Suffix'
      'From'
      '   Users'
      'Where'
      '   User_ID = :User_ID')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 528
    Top = 88
  end
  object qrDeliveryAddress: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'select'
      '   LOCATION_ADDRESS,'
      '   PHONE'
      'From'
      '   CONTACT'
      'Where'
      '   CONTACT_LK = :CONTACT_LK')
    Left = 592
    Top = 88
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CONTACT_LK'
        ParamType = ptUnknown
      end>
  end
  object qrSupplierStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '   STOCK_KEY,'
      '   SUPPLIER_CODE,'
      '   LATEST_COST,'
      '   QTY,'
      '   Min_Order_Qty,'
      '   SUPPLIER_UNIT,'
      '   DEFAULT_ORDER_UNIT'
      'FROM'
      '   SUPPLIERSTOCK'
      'WHERE'
      '   SUPPLIER_KEY = :SUPPLIER_KEY AND'
      '   STOCK_KEY = :STOCK_KEY And'
      '   PREFERRED_SUPPLIER = '#39'T'#39)
    Left = 288
    Top = 432
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
  object qrOrderedStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Sum(Supplier_Unit_Qty * Supplier_Unit_Size) Qty'
      'From'
      '   PurchaseStock Left Join PurchaseOrder on'
      '      PurchaseStock.Order_FK = PurchaseOrder.Order_LK'
      'Where'
      '   PurchaseOrder.Posted = '#39'F'#39'  And'
      '   Code = :Code And'
      '   Location = :Location')
    Left = 488
    Top = 432
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'CODE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update'
      '   StockLocation'
      'Set'
      '   On_Order = On_Order + :ORDER_QTY'
      'Where'
      '   STOCK_KEY = '
      '      (Select STOCK_KEY from STOCK '
      '      where STOCK.CODE = :CODE) and'
      '   LOCATION = :LOCATION')
    Left = 408
    Top = 376
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ORDER_QTY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CODE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end>
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   Stock.GL_Code,'
      '   Stock.GST_Percent,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15,4)) On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'From'
      '   Stock Inner Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   Stock.Stock_Key = :Stock_Key'
      'and  '
      'StockLocation.DEFAULT_LOCATION='#39'T'#39
      'Group By'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   Stock.GL_Code,'
      '   Stock.GST_Percent,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'Order By'
      '   Stock.Description,'
      '   StockLocation.Location')
    Left = 344
    Top = 432
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrSuppliedStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT DISTINCT'
      '   STOCK_KEY'
      'FROM'
      '   SUPPLIERSTOCK'
      'WHERE'
      '   SUPPLIER_KEY =  :SUPPLIER_KEY And'
      '   PREFERRED_SUPPLIER = '#39'T'#39)
    Left = 408
    Top = 432
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrPurchaseOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Supplier_Address,'
      '   Supplier_FAX,'
      '   Delivery_Address,'
      '   Order_Number,'
      '   Instructions,'
      '   Required'
      'From'
      '   PurchaseOrder'
      'Where'
      ' Order_LK = :Order_Key')
    Left = 144
    Top = 280
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrPurchaseStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.GST_Percent,'
      '   Stock.Stocktake_Unit,'
      '   StockCategory.GL_Code,'
      '   PurchaseStock.Location,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Code,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      '   PurchaseStock.Supplier_Unit_Size,'
      '   PurchaseStock.Supplier_Unit_Cost'
      'From'
      '   PurchaseStock Left Join Stock On'
      '      PurchaseStock.Code = Stock.Code'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockCategory.Stock_Category_Key = StockGroup.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Order_FK = :Order_Key'
      'Order By'
      '   Description,'
      '   Location')
    Left = 144
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrClearOrderStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Delete From'
      '   PurchaseStock'
      'Where'
      '   Order_FK = :Order_Key')
    Left = 232
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrDeleteOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Delete From'
      '   PurchaseOrder'
      'Where'
      '   Order_LK = :Order_Key')
    Left = 232
    Top = 280
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Key'
        ParamType = ptUnknown
      end>
  end
  object qrAddPurchaseOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into PurchaseOrder ('
      ''
      '   Order_LK,'
      '   Order_Number,'
      '   Contact_FK,'
      '   Supplier_Address,'
      '   Supplier_FAX,'
      '   Delivery_Address,'
      '   Instructions,'
      '   Creator_Name,'
      '   Creator_ID,'
      '   Created,'
      '   Required,'
      '   Posted,'
      '   Received )'
      ''
      'Values ('
      ''
      '   :Order_LK,'
      '   :Order_Number,'
      '   :Contact_FK,'
      '   :Supplier_Address,'
      '   :Supplier_FAX,'
      '   :Delivery_Address,'
      '   :Instructions,'
      '   :Creator_Name,'
      '   :Creator_ID,'
      '   Current_TimeStamp,'
      '   :Required,'
      '   '#39'F'#39','
      '   '#39'F'#39' )')
    Left = 320
    Top = 280
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_LK'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Order_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_FK'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_FAX'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Delivery_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Instructions'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Creator_Name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Creator_ID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Required'
        ParamType = ptUnknown
      end>
  end
  object qrAddOrderStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into PurchaseStock ('
      ''
      '   Purchase_Stock_LK,'
      '   Order_FK,'
      '   Code,'
      '   Location,'
      '   Supplier_Unit,'
      '   Supplier_Code,'
      '   Supplier_Unit_Qty,'
      '   Supplier_Unit_Cost,'
      '   Supplier_Unit_Size,'
      '   Qty_Received,'
      '   Qty_On_Order)'
      ''
      'Values ('
      ''
      '   :Purchase_Stock_LK,'
      '   :Order_FK,'
      '   :Code,'
      '   :Location,'
      '   :Supplier_Unit,'
      '   :Supplier_Code,'
      '   :Supplier_Unit_Qty,'
      '   :Supplier_Unit_Cost,'
      '   :Supplier_Unit_Size,'
      '   0,'
      '   0)')
    Left = 320
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Purchase_Stock_LK'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Order_FK'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Code'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Code'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit_Qty'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit_Cost'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Unit_Size'
        ParamType = ptUnknown
      end>
  end
  object qrOrderKey: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select Gen_ID(GEN_PURCHASEORDER, 1) from rdb$database')
    Transaction = Transaction
    Left = 56
    Top = 328
  end
  object qrUpdateOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update'
      '   PurchaseOrder'
      'Set'
      '   Order_Number = :Order_Number,'
      '   Supplier_Address = :Supplier_Address,'
      '   Supplier_FAX = :Supplier_FAX,'
      '   Delivery_Address = :Delivery_Address,'
      '   Instructions = :Instructions,'
      '   Required = :Required'
      'Where'
      '      Order_LK = :Order_LK')
    Left = 408
    Top = 280
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_Number'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Supplier_FAX'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Delivery_Address'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Instructions'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Required'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Order_LK'
        ParamType = ptUnknown
      end>
  end
  object qrOrderStockKey: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select Gen_ID(GEN_PURCHASESTOCK, 1) from rdb$database')
    Transaction = Transaction
    Left = 56
    Top = 376
  end
  object qrPostOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update'
      '   PurchaseOrder'
      'Set'
      '   Posted = '#39'T'#39
      'Where'
      '      Order_LK = :Order_LK')
    Left = 408
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_LK'
        ParamType = ptUnknown
      end>
  end
  object qrPostOrderItems: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update'
      '   PurchaseStock'
      'Set'
      '   Qty_On_Order = Supplier_Unit_Qty'
      'Where'
      '      Order_FK = :Order_FK')
    Left = 480
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Order_FK'
        ParamType = ptUnknown
      end>
  end
  object sqlOrderNumber: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select Gen_ID(GEN_PURCHASEORDER, 1) from rdb$database')
    Transaction = Transaction
    Left = 136
    Top = 384
  end
  object qrSupplierStockAuto: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '      SUPPLIERSTOCK.STOCK_KEY,'
      '   SUPPLIERSTOCK.SUPPLIER_CODE,'
      '   SUPPLIERSTOCK.LATEST_COST,'
      '   SUPPLIERSTOCK.QTY,'
      '   SUPPLIERSTOCK.Min_Order_Qty,'
      '   SUPPLIERSTOCK.SUPPLIER_UNIT,'
      '   SUPPLIERSTOCK.DEFAULT_ORDER_UNIT'
      'FROM'
      '   SUPPLIERSTOCK'
      ''
      ' left join STOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STOCK_KEY'
      
        '       left join StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.' +
        'STOCK_GROUP_KEY'
      
        '   Left Join StockCategory On StockCategory.Stock_Category_Key =' +
        ' StockGroup.Stock_Category_Key'
      ''
      'WHERE'
      '  SUPPLIERSTOCK.STOCK_KEY= :STOCK_KEY'
      ''
      'order by'
      '  StockCategory.SORT_ORDER,'
      '  STOCKGROUP.SORT_ORDER,'
      '  STOCK.DESCRIPTION')
    Left = 224
    Top = 416
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockLocationchk: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT STOCKLOCATION .STOCK_KEY,'
      'STOCKLOCATION .DEFAULT_LOCATION'
      'FROM STOCKLOCATION '
      'where STOCKLOCATION.STOCK_KEY=:STOCK_KEY')
    Left = 488
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockchk: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   Stock.GL_Code,'
      '   Stock.GST_Percent,'
      '   StockLocation.Location,'
      '   StockLocation.DEFAULT_LOCATION,'
      '   StockLocation.On_Hand,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15,4)) On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'From'
      '   Stock Inner Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   Stock.Stock_Key = :STOCK_KEY'
      'and  '
      'StockLocation.DEFAULT_LOCATION=:DEFAULT_LOCATION'
      'Group By'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   Stock.GL_Code,'
      '   Stock.GST_Percent,'
      '   StockLocation.Location,'
      '   StockLocation.DEFAULT_LOCATION,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'Order By'
      '   Stock.Description,'
      '   StockLocation.Location')
    Left = 520
    Top = 392
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'DEFAULT_LOCATION'
        ParamType = ptUnknown
      end>
  end
  object qrStockCat: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   '
      '  distinct Stock.STOCK_KEY '
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
      
        '   where StockCategory.Deleted='#39'F'#39' and STOCK.STOCK_KEY is not nu' +
        'll and'
      'StockCategory.Stock_Category =:Stock_Category '
      'and SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY'
      ''
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
      'StockCategory.SORT_ORDER,'
      '  STOCKGROUP.SORT_ORDER,'
      '  STOCK.DESCRIPTION')
    Left = 608
    Top = 288
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Category'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockAutoPopulate: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      
        '                                                      Stock.Stoc' +
        'k_Key,Stock.Code,Stock.Description,Stock.Stocktake_Unit,Stock.GL' +
        '_Code,Stock.GST_Percent,StockLocation.Location, '
      
        '                                                      StockLocat' +
        'ion.DEFAULT_LOCATION,StockLocation.On_Hand,'
      
        '                                                      Cast(Sum(P' +
        'urchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Size) As' +
        ' Numeric(15,4)) On_Order, '
      
        '                                                      StockLocat' +
        'ion.Average_Cost,StockLocation.Min_Level,StockLocation.Max_Level' +
        ',SUPPLIERSTOCK.STOCK_KEY, '
      
        '                                                      SUPPLIERST' +
        'OCK.SUPPLIER_CODE,SUPPLIERSTOCK.LATEST_COST,SUPPLIERSTOCK.QTY,SU' +
        'PPLIERSTOCK.Min_Order_Qty,SUPPLIERSTOCK.SUPPLIER_UNIT, '
      
        '                                                      SUPPLIERST' +
        'OCK.DEFAULT_ORDER_UNIT,STOCKLOCATION.LOCATION  FROM '
      
        '                                                      SUPPLIERST' +
        'OCK '
      
        '                                                      left join ' +
        'STOCK on SUPPLIERSTOCK.STOCK_KEY = STOCK.STOCK_KEY '
      
        '                                                      left join ' +
        'StockGroup on STOCKGROUP.STOCK_GROUP_KEY=STOCK.STOCK_GROUP_KEY '
      
        '                                                      Left Join ' +
        'StockCategory On StockCategory.Stock_Category_Key = StockGroup.S' +
        'tock_Category_Key '
      
        '                                                      left join ' +
        'STOCKLOCATION on STOCK.STOCK_KEY = STOCKLOCATION.STOCK_KEY '
      
        '                                                      Left Join ' +
        'PurchaseStock On PurchaseStock.Code = Stock.Code And PurchaseSto' +
        'ck.Location = StockLocation.Location '
      
        '                                                      where Stoc' +
        'kCategory.Deleted='#39'F'#39' and STOCK.STOCK_KEY is not null and STOCK.' +
        'STOCK_KEY=:STOCK_KEY    AND StockLocation.Location=:Location'
      '                                                      group by '
      
        '                                                      1,2,3,4,5,' +
        '6,7,8,9,11,12,13,14,15,16,17,18,19,20,21  '
      ' order by SUPPLIERSTOCK.DEFAULT_ORDER_UNIT'
      '                                                     ')
    Left = 640
    Top = 320
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
  object TransactionMM: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 536
    Top = 312
  end
  object qrLocationSetting: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'SELECT  a.CATEGORY_TYPE'
      'FROM LOCATIONS a'
      'where a.NAME=:NAME')
    Left = 568
    Top = 312
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'NAME'
        ParamType = ptUnknown
      end>
  end
end
