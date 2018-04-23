object frmStockRequestTransfer: TfrmStockRequestTransfer
  Left = 215
  Top = 198
  Width = 769
  Height = 478
  Caption = 'Select Stock Request for Transfers'
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
        OnAfterPaint = vtvStockRequestAfterPaint
        OnBeforePaint = vtvStockRequestBeforePaint
        OnChecked = vtvStockRequestChecked
        OnCreateEditor = vtvStockRequestCreateEditor
        OnEdited = vtvStockRequestEdited
        OnEditing = vtvStockRequestEditing
        OnFocusChanged = vtvStockRequestFocusChanged
        OnFocusChanging = vtvStockRequestFocusChanging
        OnGetText = vtvStockRequestGetText
        OnGetImageIndex = vtvStockRequestGetImageIndex
        OnKeyDown = vtvStockRequestKeyDown
        Columns = <
          item
            MinWidth = 100
            Position = 0
            Width = 237
            WideText = 'Stock Request Number'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 120
            WideText = 'Date Generated'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 120
            WideText = 'Date Required'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 120
            WideText = 'Location From'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            Width = 120
            WideText = 'Location To'
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
    object btnOk: TBitBtn
      Left = 528
      Top = 393
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Ok'
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
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 64
    Top = 104
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 232
    Top = 152
  end
  object qrStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      '    SR.REQUEST_NUMBER,'
      '    SR.DATE_GENERATED AS DateGenerated,'
      '    SR.DATE_REQUIRED AS DateRequired,'
      '    SR.LOCATION'
      'FROM STOCKREQUEST SR'
      
        'LEFT JOIN STOCKREQUEST_STOCK SRS ON SR.REQUEST_NUMBER = SRS.REQU' +
        'EST_NUMBER'
      'WHERE UPPER(SR.LOCATION) = UPPER(:Location) AND '
      
        'SR.ISCOMMITTED = '#39'T'#39' AND SR.ISDELETED = '#39'F'#39' AND SRS.ISPROCESSED ' +
        '= '#39'F'#39' AND SRS.ISREMOVED = '#39'F'#39' AND'
      'SR.REQUEST_TYPE = '#39'Transfer'#39
      
        'GROUP BY SR.REQUEST_NUMBER, SR.DATE_GENERATED, SR.DATE_REQUIRED,' +
        ' SR.LOCATION')
    Left = 368
    Top = 224
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
end
