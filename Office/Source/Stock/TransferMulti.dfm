object frmTransferMulti: TfrmTransferMulti
  Left = 433
  Top = 89
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Stock Transfer'
  ClientHeight = 443
  ClientWidth = 792
  Color = clBtnFace
  Constraints.MaxWidth = 800
  Constraints.MinHeight = 357
  Constraints.MinWidth = 652
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  DesignSize = (
    792
    443)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 777
    Height = 457
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      777
      457)
    object Panel9: TPanel
      Left = 5
      Top = 5
      Width = 767
      Height = 48
      Align = alTop
      BevelOuter = bvNone
      Color = clAppWorkSpace
      TabOrder = 7
      object lbeTitle: TLabel
        Left = 72
        Top = 0
        Width = 577
        Height = 49
        Alignment = taCenter
        AutoSize = False
        Caption = 'Select the locations you wish to transfer between'
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
    object tvTransfers: TTreeView
      Left = 8
      Top = 56
      Width = 137
      Height = 324
      Anchors = [akLeft, akTop, akBottom]
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
      TabOrder = 8
      OnChange = tvTransfersChange
    end
    object vtvStockQty: TVirtualStringTree
      Left = 152
      Top = 128
      Width = 617
      Height = 248
      Anchors = [akLeft, akTop, akBottom]
      Color = clInfoBk
      Header.AutoSizeIndex = 4
      Header.Font.Charset = DEFAULT_CHARSET
      Header.Font.Color = clWindowText
      Header.Font.Height = -11
      Header.Font.Name = 'MS Sans Serif'
      Header.Font.Style = []
      Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
      Images = dmUtilities.imlTreeImages
      TabOrder = 0
      TreeOptions.MiscOptions = [toEditable, toToggleOnDblClick, toWheelPanning]
      TreeOptions.PaintOptions = [toShowButtons, toShowRoot, toShowTreeLines, toShowVertGridLines]
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
          Width = 300
          WideText = 'Stock Item'
        end
        item
          Position = 1
          Width = 89
          WideText = 'Unit'
        end
        item
          Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
          Position = 2
          Width = 60
          WideText = 'Source'
        end
        item
          Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
          Position = 3
          Width = 65
          WideText = 'Destination'
        end
        item
          MaxWidth = 300
          MinWidth = 60
          Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
          Position = 4
          Width = 99
          WideText = 'Transfer'
        end>
      WideDefaultText = ''
    end
    object dbgTransferedStock: TDBGrid
      Left = 152
      Top = 133
      Width = 617
      Height = 253
      Anchors = [akLeft, akTop, akBottom]
      Color = clInfoBk
      Ctl3D = True
      DataSource = dsTransferedStock
      Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
      ParentCtl3D = False
      TabOrder = 9
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'DESCRIPTION'
          Title.Caption = 'Stock Item'
          Width = 187
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'UNIT'
          Title.Caption = 'Unit'
          Width = 107
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'LOCATION'
          Title.Caption = 'Source'
          Width = 107
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'PURCHASER_NAME'
          Title.Caption = 'Destination'
          Width = 107
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'QTY'
          Title.Caption = 'Quantity'
          Width = 90
          Visible = True
        end>
    end
    object neStockQty: TNumericEdit
      Left = 528
      Top = 152
      Width = 89
      Height = 21
      Value = 1.23457
      Alignment = taLeftJustify
      Precision = 15
      TabOrder = 1
      Visible = False
      OnKeyDown = neStockQtyKeyDown
      OnKeyPress = neStockQtyKeyPress
      DecimalPlaces = 4
    end
    object Panel1: TPanel
      Left = 152
      Top = 56
      Width = 617
      Height = 75
      BevelOuter = bvNone
      BorderStyle = bsSingle
      TabOrder = 2
      object Label5: TLabel
        Left = 8
        Top = 10
        Width = 32
        Height = 13
        Caption = 'From:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeFrom: TLabel
        Left = 64
        Top = 10
        Width = 29
        Height = 13
        Caption = 'From'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeTo: TLabel
        Left = 336
        Top = 10
        Width = 14
        Height = 13
        Caption = 'To'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 264
        Top = 10
        Width = 17
        Height = 13
        Caption = 'To:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label1: TLabel
        Left = 8
        Top = 30
        Width = 30
        Height = 13
        Caption = 'Date:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 8
        Top = 54
        Width = 98
        Height = 13
        Caption = 'Transfer Number:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeTransferNumber: TLabel
        Left = 120
        Top = 54
        Width = 95
        Height = 13
        Caption = 'Transfer Number'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 264
        Top = 30
        Width = 61
        Height = 13
        Caption = 'Reference:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object dtpTransferDate: TDateTimePicker
        Left = 64
        Top = 30
        Width = 186
        Height = 21
        CalAlignment = dtaLeft
        Date = 39230
        Time = 39230
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 1
      end
      object edReference: TEdit
        Left = 336
        Top = 30
        Width = 265
        Height = 21
        MaxLength = 25
        TabOrder = 0
      end
    end
    object btnOk: TBitBtn
      Left = 568
      Top = 393
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'OK'
      TabOrder = 3
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
    object btnReprint: TBitBtn
      Left = 462
      Top = 393
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Reprint'
      TabOrder = 10
      OnClick = btnReprintClick
    end
    object btnCancel: TBitBtn
      Left = 672
      Top = 393
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 4
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
    object btnFind: TBitBtn
      Left = 152
      Top = 393
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 5
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
    object btnScan: TBitBtn
      Left = 248
      Top = 393
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Scan'
      TabOrder = 6
      OnClick = btnScanClick
      Glyph.Data = {
        5E060000424D5E06000000000000360400002800000018000000170000000100
        08000000000028020000C40E0000C40E00000001000000000000FFFFFF000202
        0100080A0200100D130013120A00191B0600241D0A001B1B1500201D16001712
        2200301E3E0023250A002A2B0C001E201600242317002B2A1B0030310B003033
        1900383B1A00443A1F00282520002E2C230032293500343328003B3C27003634
        3500413D2900423F3100321D5D0026036900390E7400391A68003E187700431B
        7600372057003E374200443749003428600042286D0042207600374707003C48
        1900404A000044421F00444C1B003D4227003F42320044442B00484C2B004644
        35004D4D38004C532900575B2E0052533C0057593A005B613800616239004A4A
        4400555542005B5B4700605E460055555500595A5900184B71005E5963005D61
        470063644B00696A4B00646452006C6C55006F734A006F7A4A0074745800797B
        5A0033607F0068686400716B7400747563007A7C650073767800817E5D00817E
        62002E078100390D960038118800410C8F00471886004A1997004501A000501D
        A1004F288D0055289900533C8900533B9D00572DA8005F2DB5005C30A200623B
        A4003500DC005712CF00572DC3006837D6005222E400672DF900165A9B006F4E
        9900705C82001057A500215CA200446F9F00776392007C7187001A67AA00226F
        B9003577AB002A7BBF00406EA3004B7BAF005A7AB70084688C002677C0007789
        390082903F007A8547007C8258007C8366007F8C68007B857100818A47008385
        5B0084944A008B975F00858466008A8B6A00868575008A8D74008E926B00949A
        6D0092947700999B7B00A09E7E008EA44B009EB458009AA37B00A6A67C009EB0
        6000AAB678003585BB004E87A9004E8EBC005E94AB004190BD008A898A008F8A
        950093958400979B8600A19E8400949494009A9C9800A29F98009F9FA7009BA3
        88009DAA8700A4A48800A7AA8C009DA79300A3A49400A8AC9400ACB18A00ADB8
        8C00AEB39A00B3B99D0083A7A800A6A5A600AAACA60092A8B200AFAEB1009AB1
        A400AFB4A300B6BBA400B3B3B400B9BBB700C1BBA700C1BFB8002386D0003E95
        CF004C87C200418EDB005295CA00659CC600559CD300619EDD003E99E50072A5
        C8005BA6DA0067A9DA005FB8DE006BB0DA0055A0E40066ABE4006BB2E30081AF
        C50090BAC700B1BBC10094B9DD00C1BDC600B4C19E00C2C09B00EBD99F00BCC4
        AC00BDC9AC00BEC6B100BDCAB500C2C5AC00C5CAAB00C2C6B300C3CAB400CED3
        A900CCD3BB00D5DABA00F2E4B900AEC5C20099C2D900A7C3DE00C5C5C500CBCC
        C900CECED600D1D4C500D6DBC700D6D6D600DBDBDB00E0DEDB009CC6E700DDDB
        E200DCE2CD00DDE8CE00E1E5CB00E4EACE00DEE3D300DFE8D500E2E5D800E6EB
        DB00EEF2C900EEF2DC00EBF8DB00D2E4E300E4E4E400ECECEA00DDE7F200EBE3
        F000EEEDF300EFF3E800F7FBEB00F4F4F400FFFFFF0000000000000000000000
        000000000000000000000000000000000000000000000000000000F800000000
        0000000000000000000000CFF9CFF89E77F2F8F2F8F2F8F2F9F2000000000000
        0000003838383833384BF2E1F2E1F2F8F2F80000000000000000003838384B38
        1910F9EBF8E1F8F2F9F2000000000000000000B13819384B4BFF00F8F2F800F8
        0000000000000000000000B1B1899ECFE18900F2F90000000000000000000000
        00000000CFB1CFDECFDEF20000000000000000000000000000000000CFB1CDCF
        E1CFCD0000000000000000000000000000000000009ECF9ECFE1CFE100000000
        00000000000000000000000000CF9ECFCDCFE1CF000000000000000000000000
        0000000000F8CFB1CFDEE1DEF200000000000000000000000000000000009ECF
        AFCFE1CFAF0000000000000000000000000000000000F29ECFB1CFDEBDDE0000
        00000000000000000000000000000089AFCFE1CF78DE00F2F9E19E9EAFE10000
        00000000000000E1CFB1CFB1CF9ECFB1CF193819384BF2000000000000000000
        9ECF9E8E9ECFF8F2E1384B794B894B000000000000000000DCB1CF9EDCE1F2F8
        00898E9ECF25389E00000000000000000089E1E1F8F2F8F2F9004B899E601938
        E1000000000000000000CFE1CFE1F200F2DEF24B896E10197900000000000000
        000000E19E384B89E1CFCDCF4B894B386E000000000000000000000000E1CF96
        CFE1CFE1B64BB1E1000000000000000000000000000000000000000000000000
        0000}
    end
  end
  object qrGetTransferStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT a.DESCRIPTION,a.STOCK_GROUP,'
      'a.LOCATION,a.REFERENCE,'
      'a.PURCHASER_NAME,a.UNIT,'
      '(-1*a.QTY) as QTY,a.CREATED,a.BATCH_KEY,b.REFERENCE'
      'from TRANSACTIONBATCH b'
      'inner join STOCKTRANS a on '
      'a.BATCH_KEY = b.BATCH_KEY'
      'where b.TRANSACTION_TYPE = '#39'Transfer'#39' '
      'and a.QTY  < 0 and a.BATCH_KEY = :BATCH_KEY '
      '   ')
    Left = 360
    Top = 256
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'BATCH_KEY'
        ParamType = ptUnknown
      end>
  end
  object dsTransferedStock: TDataSource
    AutoEdit = False
    DataSet = qrGetTransferStock
    Left = 432
    Top = 256
  end
  object qrStockLoc1Old: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   Location'
      'From '
      '   StockLocation'
      'Order By'
      '   Location')
    Left = 361
    Top = 401
  end
  object dsStockLoc1: TDataSource
    AutoEdit = False
    DataSet = qrStockLoc2Old
    Left = 417
    Top = 401
  end
  object qrStockLoc2Old: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select Distinct'
      '   Location'
      'From '
      '   StockLocation'
      'Order By'
      '   Location')
    Left = 456
    Top = 328
  end
  object qrGetPreviousTransfers: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select a.CREATED,'
      'a.BATCH_KEY,'
      'b.REFERENCE,'
      'a.LOCATION,'
      'a.REFERENCE,'
      'a.PURCHASER_NAME'
      'from TRANSACTIONBATCH b'
      'inner join STOCKTRANS a on '
      'a.BATCH_KEY = b.BATCH_KEY'
      'where b.TRANSACTION_TYPE = '#39'Transfer'#39
      'and a.QTY < 0'
      'and a.LOCATION = :LOCATION '
      'and a.PURCHASER_NAME = :PURCHASER_NAME'
      'group by '
      'a.CREATED,'
      'a.BATCH_KEY,'
      'b.REFERENCE,'
      'a.LOCATION,'
      'a.REFERENCE,'
      'a.PURCHASER_NAME')
    Left = 488
    Top = 248
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'PURCHASER_NAME'
        ParamType = ptUnknown
      end>
  end
  object dsStockLoc2: TDataSource
    AutoEdit = False
    DataSet = qrStockLoc2Old
    Left = 377
    Top = 345
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 248
    Top = 208
  end
  object qrUsersOld: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'select USER_NAME from USERS where USER_ID = :USER_ID')
    Left = 208
    Top = 264
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'USER_ID'
        ParamType = ptUnknown
      end>
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '    SR.REQUEST_NUMBER,'
      '    SRS.STOCK_REQUEST_STOCKKEY AS STOCK_REQUEST_KEY,'
      '    SC.STOCK_CATEGORY,'
      '    SG.STOCK_GROUP,'
      '    S.STOCK_KEY,'
      '    S.CODE,'
      '    S.DESCRIPTION,'
      '    S.STOCKTAKE_UNIT AS STOCK_REQUEST_UNIT,'
      '    SL.LOCATION AS SOURCE_LOCATION,'
      '    SL.ON_HAND,'
      '    SL.INITIALISED,'
      '    ST.INITIALISED_TIME,'
      '    (SRS.QTY_REQUIRED) AS QTY_REQUIRED,'
      '    SR.LOCATION AS DESTINATION_LOCATION'
      'FROM STOCKCATEGORY SC '
      
        'LEFT JOIN STOCKGROUP SG ON SC.STOCK_CATEGORY_KEY = SG.STOCK_CATE' +
        'GORY_KEY'
      'LEFT JOIN STOCK S ON SG.STOCK_GROUP_KEY = S.STOCK_GROUP_KEY'
      'LEFT JOIN STOCKLOCATION SL ON S.STOCK_KEY = SL.STOCK_KEY'
      'LEFT JOIN STOCKTAKE ST ON SL.STOCKTAKE_KEY = ST.STOCKTAKE_KEY'
      'LEFT JOIN STOCKREQUEST_STOCK SRS ON S.STOCK_KEY = SRS.STOCK_KEY'
      
        'INNER JOIN SUPPLIERSTOCK SS ON S.STOCK_KEY = SS.STOCK_KEY AND SR' +
        'S.SUPPLIER_KEY = SS.SUPPLIER_KEY'
      
        'INNER JOIN STOCKREQUEST SR ON SRS.REQUEST_NUMBER = SR.REQUEST_NU' +
        'MBER'
      
        'WHERE S.ENABLED = '#39'T'#39' AND (S.DELETED IS NULL OR S.DELETED = '#39'F'#39')' +
        ' AND SRS.ISPROCESSED != '#39'T'#39' AND SR.REQUEST_TYPE = '#39'Transfer'#39
      'ORDER BY SC.SORT_ORDER, SG.SORT_ORDER, S.DESCRIPTION'
      '')
    Left = 208
    Top = 312
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 208
    Top = 360
  end
  object qrFindBarcode: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'select '
      '   Barcode.Stock_Key, '
      '   Barcode.Supplier_Stock_Key, '
      '   Barcode.Barcode,'
      '   SupplierStock.Stock_Key as Supplier_Stock_Stock_Key '
      'from'
      
        '   Barcode Left Join SupplierStock on Barcode.Supplier_Stock_Key' +
        ' =    SupplierStock.Supplier_Stock_Key'
      'where'
      '   Barcode.Barcode = :Barcode')
    Left = 296
    Top = 264
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Barcode'
        ParamType = ptUnknown
      end>
  end
  object IBQuery1: TIBQuery
    Left = 304
    Top = 208
  end
  object qrUpdateStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'UPDATE '
      '    STOCKREQUEST_STOCK'
      'SET'
      '    STOCKREQUEST_STOCK.ISPROCESSED = '#39'T'#39
      
        'WHERE STOCKREQUEST_STOCK.STOCK_REQUEST_STOCKKEY =:STOCK_REQUEST_' +
        'STOCKKEY')
    Left = 304
    Top = 208
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockTransferManual: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Stock_Key,'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Initialised,'
      '   Stocktake.Initialised_Time'
      'From'
      '   StockCategory Left Join StockGroup On'
      
        '      StockCategory.Stock_Category_Key = StockGroup.Stock_Catego' +
        'ry_Key'
      '   Left Join Stock On'
      '      StockGroup.Stock_Group_Key = Stock.Stock_Group_Key'
      '   Left Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join Stocktake On'
      '      StockLocation.Stocktake_Key = Stocktake.Stocktake_Key'
      'Where'
      '   Stock.Enabled = '#39'T'#39
      '   And (Stock.Deleted Is Null Or Stock.Deleted = '#39'F'#39')'
      'Order By'
      '   StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order,'
      '   Stock.Description')
    Left = 296
    Top = 312
  end
  object qrdelete_stock_request: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'UPDATE  STOCKREQUEST  SET STOCKREQUEST.ISDELETED='#39'T'#39' WHERE STOCK' +
        'REQUEST.REQUEST_NUMBER=:REQUEST_NUMBER')
    Left = 633
    Top = 265
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
  object qrfetch_request_number: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT  a.REQUEST_NUMBER'
      
        'FROM STOCKREQUEST_STOCK a where a.STOCK_REQUEST_STOCKKEY=:STOCK_' +
        'REQUEST_STOCKKEY')
    Left = 592
    Top = 272
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStockRequestQuantity: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'UPDATE '
      '    STOCKREQUEST_STOCK'
      'SET'
      '    STOCKREQUEST_STOCK.QTY_REQUIRED=:QTY_REQUIRED'
      
        'WHERE STOCKREQUEST_STOCK.STOCK_REQUEST_STOCKKEY=:STOCK_REQUEST_S' +
        'TOCKKEY')
    Left = 689
    Top = 265
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'QTY_REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object sqlTransferNumber: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'SELECT GEN_ID(GEN_TRANSFER_NUMBER, 1) FROM RDB$DATABASE'
      '')
    Transaction = Transaction
    Left = 560
    Top = 240
  end
  object qrfindCommitedRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT  ss.TRANSFER_ID,ss.STOCK_REQUEST_NO, ss.CREATED,'
      'ss.USER_NAME'
      'FROM STOCKTRANS ss where ss.BATCH_KEY=:BATCH_KEY'
      '   ')
    Left = 656
    Top = 248
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'BATCH_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrfetch_request_no: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT  a.REQUEST_NUMBER'
      
        'FROM STOCKREQUEST_STOCK a where a.STOCK_REQUEST_STOCKKEY = :STOC' +
        'K_REQUEST_STOCKKEY ')
    Left = 640
    Top = 304
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateRealRequestedQty: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'UPDATE '
      '    STOCKREQUEST_STOCK'
      'SET'
      
        '    STOCKREQUEST_STOCK.REQUESTED_QTY_ORIGINAL_VAL=:REQUESTED_QTY' +
        '_ORIGINAL_VAL'
      
        'WHERE STOCKREQUEST_STOCK.STOCK_REQUEST_STOCKKEY=:STOCK_REQUEST_S' +
        'TOCKKEY')
    Left = 680
    Top = 337
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUESTED_QTY_ORIGINAL_VAL'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
  object qrQuantityUpdate: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.SUPPLIER_STOCK_KEY, a.SUPPLIER_KEY, a.STOCK_KEY, a.SUPP' +
        'LIER_UNIT, a.QTY '
      'FROM SUPPLIERSTOCK a '
      'WHERE'
      ' A.SUPPLIER_UNIT=:SUPPLIER_UNIT'
      'AND A.STOCK_KEY=:STOCK_KEY')
    Left = 689
    Top = 305
    ParamData = <
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
  object qrStockReqUnit: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT a.STOCK_REQUEST_STOCKKEY ,a.STOCK_REQUEST_UNIT'
      'FROM STOCKREQUEST_STOCK a '
      ''
      'where a.STOCK_REQUEST_STOCKKEY=:STOCK_REQUEST_STOCKKEY')
    Left = 529
    Top = 329
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'STOCK_REQUEST_STOCKKEY'
        ParamType = ptUnknown
      end>
  end
end
