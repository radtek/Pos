object frmSelectSupplierStockRequest: TfrmSelectSupplierStockRequest
  Left = 489
  Top = 126
  Width = 769
  Height = 478
  Caption = 'Select Stock Request'
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
    761
    451)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 745
    Height = 434
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      745
      434)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 735
      Height = 381
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        735
        381)
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
      object vtvStockRequest: TVirtualStringTree
        Left = 8
        Top = 56
        Width = 721
        Height = 274
        Anchors = [akLeft, akTop, akBottom]
        CheckImageKind = ckDarkTick
        Color = clInfoBk
        Header.AutoSizeIndex = 0
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        TabOrder = 1
        TreeOptions.MiscOptions = [toCheckSupport, toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect, toMultiSelect]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterItemPaint = vtvStockRequestAfterItemPaint
        OnAfterPaint = vtvStockRequestAfterPaint
        OnChecked = vtvStockRequestChecked
        OnGetText = vtvStockRequestGetText
        Columns = <
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 200
            WideText = 'Request Number'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 157
            WideText = 'Date Generated'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 180
            WideText = 'Date Required'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 180
            WideText = 'Location'
          end>
        WideDefaultText = ''
      end
    end
    object btnCancel: TBitBtn
      Left = 640
      Top = 393
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
      NumGlyphs = 2
    end
    object btnNext: TBitBtn
      Left = 528
      Top = 393
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Next'
      TabOrder = 2
      OnClick = btnNextClick
      NumGlyphs = 2
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 24
    Top = 360
  end
  object qrGetStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      'a.REQUEST_NUMBER,'
      'a.LOCATION, '
      'a.DATE_GENERATED, '
      'a.DATE_REQUIRED'
      'FROM STOCKREQUEST a '
      'inner join STOCKREQUEST_STOCK b on'
      'a.REQUEST_NUMBER = b.REQUEST_NUMBER'
      'where a.REQUEST_TYPE = '#39'Purchase Order'#39
      'and b.ISPROCESSED = '#39'F'#39
      'and a.ISCOMMITTED = '#39'T'#39
      'group by '
      'a.REQUEST_NUMBER,'
      'a.LOCATION, '
      'a.DATE_GENERATED, '
      'a.DATE_REQUIRED')
    Left = 88
    Top = 360
  end
  object qrGetStockRequestItems: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'STOCK.CODE,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      'SUPPLIERSTOCK.SUPPLIER_CODE'
      'FROM STOCKREQUEST_STOCK '
      
        'left join STOCKREQUEST on STOCKREQUEST.REQUEST_NUMBER=STOCKREQUE' +
        'ST_STOCK.REQUEST_NUMBER'
      
        'left join CONTACT on CONTACT.CONTACT_LK=STOCKREQUEST_STOCK.SUPPL' +
        'IER_KEY'
      'left join STOCK on STOCK.STOCK_KEY=STOCKREQUEST_STOCK.STOCK_KEY'
      
        'left join SUPPLIERSTOCK on STOCKREQUEST_STOCK.SUPPLIER_KEY = SUP' +
        'PLIERSTOCK.SUPPLIER_KEY and STOCKREQUEST_STOCK.STOCK_KEY = SUPPL' +
        'IERSTOCK.STOCK_KEY '
      'where STOCKREQUEST_STOCK.REQUEST_NUMBER= :REQUEST_NUMBER and '
      'STOCKREQUEST_STOCK.ISPROCESSED = '#39'F'#39' and'
      'STOCKREQUEST_STOCK.ISREMOVED = '#39'F'#39
      'group by'
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'STOCK.CODE,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      'SUPPLIERSTOCK.SUPPLIER_CODE')
    Left = 152
    Top = 360
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
  object qrStockRequestSelect: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT '
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'STOCKREQUEST.LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      'STOCK.CODE,'
      
        'STOCKLOCATION.ON_HAND,STOCKREQUEST_STOCK.SUPPLIER_STOCK_KEY, STO' +
        'CKREQUEST_STOCK.SUPPLIER_CODE'
      'FROM STOCKREQUEST_STOCK '
      
        'left join STOCKREQUEST on STOCKREQUEST.REQUEST_NUMBER=STOCKREQUE' +
        'ST_STOCK.REQUEST_NUMBER'
      
        'left join CONTACT on CONTACT.CONTACT_LK=STOCKREQUEST_STOCK.SUPPL' +
        'IER_KEY'
      'left join STOCK on STOCK.STOCK_KEY=STOCKREQUEST_STOCK.STOCK_KEY'
      
        'left join SUPPLIERSTOCK on STOCKREQUEST_STOCK.SUPPLIER_KEY = SUP' +
        'PLIERSTOCK.SUPPLIER_KEY and STOCKREQUEST_STOCK.STOCK_KEY = SUPPL' +
        'IERSTOCK.STOCK_KEY '
      
        'left join STOCKLOCATION on STOCKLOCATION.STOCK_KEY=STOCK.STOCK_K' +
        'EY and STOCKLOCATION.LOCATION=STOCKREQUEST.LOCATION'
      #9#9#9#9#9
      'where STOCKREQUEST_STOCK.REQUEST_NUMBER= :REQUEST_NUMBER '
      ' and SUPPLIERSTOCK.SUPPLIER_STOCK_KEY=:SUPPLIER_STOCK_KEY'
      'group by'
      'STOCK_REQUEST_STOCKKEY,'
      'STOCKREQUEST.REQUEST_NUMBER, '
      'STOCKREQUEST_STOCK.STOCK_KEY,'
      'STOCK.DESCRIPTION, '
      'STOCKREQUEST_STOCK.SUPPLIER_KEY,'
      'CONTACT.COMPANY_NAME,'
      'QTY_REQUIRED, '
      'STOCK_REQUEST_UNIT, '
      'STOCK_REQUEST_UNIT_COST, '
      'ISPROCESSED, ISREMOVED, '
      'DATE_ENTERED, '
      'DATE_UPDATED,'
      'LOCATION, '
      'REQUEST_TYPE, '
      'DATE_GENERATED, '
      'DATE_REQUIRED, '
      'ISCOMMITTED, '
      'ISDELETED,'
      
        'STOCK.CODE,STOCKLOCATION.ON_HAND,STOCKREQUEST_STOCK.SUPPLIER_STO' +
        'CK_KEY, STOCKREQUEST_STOCK.SUPPLIER_CODE')
    Left = 280
    Top = 360
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'SUPPLIER_STOCK_KEY'
        ParamType = ptUnknown
      end>
  end
  object qrStockReqStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      
        'SELECT a.STOCK_REQUEST_STOCKKEY, a.REQUEST_NUMBER, a.STOCK_KEY, ' +
        'a.SUPPLIER_KEY , a.SUPPLIER_STOCK_KEY, a.SUPPLIER_CODE'
      'FROM STOCKREQUEST_STOCK a'
      'where a.REQUEST_NUMBER=:REQUEST_NUMBER')
    Left = 368
    Top = 344
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
end
