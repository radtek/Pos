object frmStockRequestHistory: TfrmStockRequestHistory
  Tag = 1024
  Left = 426
  Top = 9
  Width = 723
  Height = 572
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Stock Requests'
  Color = clBtnFace
  Constraints.MinHeight = 368
  Constraints.MinWidth = 721
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    715
    545)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 699
    Height = 529
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      699
      529)
    object btnUpdateStockRequest: TBitBtn
      Left = 346
      Top = 488
      Width = 110
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Update'
      TabOrder = 0
      OnClick = btnUpdateStockRequestClick
    end
    object btnNewStockRequest: TBitBtn
      Left = 465
      Top = 488
      Width = 110
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&New Stock Request'
      TabOrder = 1
      OnClick = btnNewStockRequestClick
    end
    object btnClose: TBitBtn
      Left = 584
      Top = 488
      Width = 110
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      OnClick = btnCloseClick
      Kind = bkClose
    end
    object btnFind: TBitBtn
      Left = 8
      Top = 488
      Width = 89
      Height = 33
      Anchors = [akLeft, akBottom]
      Caption = '&Find'
      TabOrder = 3
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
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 689
      Height = 475
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 4
      DesignSize = (
        689
        475)
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 687
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 687
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
      object dbgDetails: TDBGrid
        Left = 216
        Top = 96
        Width = 464
        Height = 318
        Anchors = [akLeft, akTop, akRight, akBottom]
        Color = clInfoBk
        DataSource = dsStockRequestDetails
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'DESCRIPTION'
            Title.Alignment = taCenter
            Title.Caption = 'Stock Item'
            Width = 145
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'COMPANY_NAME'
            Title.Alignment = taCenter
            Title.Caption = 'Supplier'
            Width = 145
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'QTY_REQUIRED'
            Title.Alignment = taCenter
            Title.Caption = 'Quantity'
            Width = 55
            Visible = True
          end
          item
            Alignment = taRightJustify
            Expanded = False
            FieldName = 'STOCK_REQUEST_UNIT'
            Title.Alignment = taCenter
            Title.Caption = 'Unit'
            Width = 50
            Visible = True
          end
          item
            Alignment = taRightJustify
            Expanded = False
            FieldName = 'STOCK_REQUEST_UNIT_COST'
            Title.Alignment = taCenter
            Title.Caption = 'Unit Cost'
            Width = 50
            Visible = True
          end>
      end
      object tvStockRequests: TTreeView
        Left = 8
        Top = 56
        Width = 201
        Height = 408
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
        TabOrder = 2
        OnChange = tvStockRequestsChange
        OnDblClick = tvStockRequestsDblClick
        OnKeyPress = tvStockRequestsKeyPress
      end
      object Panel1: TPanel
        Left = 216
        Top = 57
        Width = 464
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        BevelOuter = bvNone
        Caption = '`'
        Color = clAppWorkSpace
        TabOrder = 3
        object lbeReference: TLabel
          Left = 0
          Top = 0
          Width = 464
          Height = 32
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'lbeReference'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
    end
    object btnDeleteStockRequest: TBitBtn
      Left = 226
      Top = 488
      Width = 110
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Delete'
      TabOrder = 5
      OnClick = btnDeleteStockRequestClick
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 80
    Top = 104
  end
  object qrStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT'
      '    MIN(SR.REQUEST_NUMBER) AS REQUEST_NUMBER,'
      '    SR.LOCATION,'
      '    SR.REQUEST_TYPE,'
      '    SR.DATE_GENERATED,'
      '    MIN(SR.ISCOMMITTED) AS ISCOMMITTED'
      'FROM STOCKREQUEST SR'
      
        'INNER JOIN STOCKREQUEST_STOCK SRS ON SR.REQUEST_NUMBER = SRS.REQ' +
        'UEST_NUMBER'
      'where SR.ISCOMMITTED <>'#39'T'#39' and SR.ISDELETED<>'#39'T'#39
      'GROUP BY SR.LOCATION, SR.REQUEST_TYPE, SR.DATE_GENERATED')
    Left = 80
    Top = 200
  end
  object dsStockRequest: TDataSource
    AutoEdit = False
    DataSet = qrStockRequest
    Left = 160
    Top = 200
  end
  object qrStockRequestDetails: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    DataSource = dsStockRequest
    SQL.Strings = (
      'SELECT'
      '    SRS.STOCK_REQUEST_STOCKKEY,'
      #9'SRS.STOCK_KEY,'
      #9'S.DESCRIPTION,'
      #9'S.CODE,'
      #9'SRS.QTY_REQUIRED,'
      #9'SRS.STOCK_REQUEST_UNIT,'
      #9'SRS.STOCK_REQUEST_UNIT_COST,'
      #9'SRS.SUPPLIER_KEY,'
      #9'C.COMPANY_NAME'
      'FROM'
      #9'STOCKREQUEST_STOCK SRS '
      'INNER JOIN STOCK S ON SRS.STOCK_KEY = S.STOCK_KEY '
      'LEFT JOIN CONTACT C ON SRS.SUPPLIER_KEY = C.CONTACT_LK '
      'WHERE '
      #9'SRS.REQUEST_NUMBER = :Request_Number  and SRS.ISREMOVED='#39'F'#39
      'ORDER BY '
      #9'S.DESCRIPTION, SRS.QTY_REQUIRED')
    Left = 80
    Top = 248
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Request_Number'
        ParamType = ptUnknown
      end>
  end
  object dsStockRequestDetails: TDataSource
    AutoEdit = False
    DataSet = qrStockRequestDetails
    Left = 160
    Top = 248
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 128
    Top = 496
  end
  object qrUpdateStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Update STOCKREQUEST'
      'Set'
      'ISDELETED   = :ISDELETED        '
      'Where'
      ' REQUEST_NUMBER = :REQUEST_NUMBER')
    Left = 112
    Top = 328
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ISDELETED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUEST_NUMBER'
        ParamType = ptUnknown
      end>
  end
end
