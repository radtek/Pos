object frmStockSubstitute: TfrmStockSubstitute
  Left = 237
  Top = 85
  Width = 998
  Height = 478
  Caption = 'Select Substitute Stock'
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
    990
    451)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 977
    Height = 434
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      977
      434)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 967
      Height = 381
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        967
        381)
      object Label3: TLabel
        Left = 8
        Top = 67
        Width = 79
        Height = 13
        Caption = 'Select Location :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 965
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 965
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
      object vtvStockSubstitute: TVirtualStringTree
        Left = 8
        Top = 104
        Width = 945
        Height = 242
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
        OnAfterItemPaint = vtvStockSubstituteAfterItemPaint
        OnAfterPaint = vtvStockSubstituteAfterPaint
        OnChecked = vtvStockSubstituteChecked
        OnCreateEditor = vtvStockSubstituteCreateEditor
        OnEdited = vtvStockSubstituteEdited
        OnEditing = vtvStockSubstituteEditing
        OnFocusChanged = vtvStockSubstituteFocusChanged
        OnFocusChanging = vtvStockSubstituteFocusChanging
        OnGetText = vtvStockSubstituteGetText
        Columns = <
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 191
            WideText = 'Stock Item'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 1
            Width = 100
            WideText = 'On Hand Qty'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 2
            Width = 100
            WideText = 'Cost'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 3
            Width = 190
            WideText = 'Substitute Item'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 4
            Width = 100
            WideText = 'On Hand Qty'
          end
          item
            Options = [coEnabled, coParentColor, coResizable, coVisible]
            Position = 5
            Width = 100
            WideText = 'Cost'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 6
            Width = 80
            WideText = 'Sub #'
          end
          item
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 7
            Width = 80
            WideText = 'Variance'
          end>
        WideDefaultText = ''
      end
      object cbStockLocation: TComboBox
        Left = 96
        Top = 62
        Width = 137
        Height = 21
        Style = csDropDownList
        Color = clInfoBk
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        TabOrder = 2
        OnChange = cbStockLocationChange
      end
      object cbRevertToOriginal: TCheckBox
        Left = 8
        Top = 355
        Width = 257
        Height = 17
        Caption = 'Update to original'
        TabOrder = 3
      end
    end
    object btnCancel: TBitBtn
      Left = 872
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
      Left = 760
      Top = 393
      Width = 100
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Commit'
      TabOrder = 2
      OnClick = btnNextClick
      NumGlyphs = 2
    end
  end
  object cbSubstituteNumber: TComboBox
    Left = 808
    Top = 136
    Width = 73
    Height = 21
    ItemHeight = 13
    TabOrder = 1
    Visible = False
    OnChange = cbSubstituteNumberChange
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 24
    Top = 400
  end
  object Query: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 138
    Top = 400
  end
  object qrGetSubstituteStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'SELECT '
      'RT.SUBSTITUTE_KEY,'
      'RT.STOCK_KEY,'
      'ST.CODE,'
      'ST.REVERT_TO_ORIGINAL,'
      'ST.DESCRIPTION STOCK_NAME,'
      'STL.LATEST_COST STOCK_COST,'
      'STL.ON_HAND STOCK_ONHAND, '
      'STL.MIN_LEVEL STOCK_MINLEVEL,'
      'RT.SUBSTITUTE_STOCK_KEY,'
      'RT.SUBSTITUTE_NAME,'
      'RT.SUBSTITUTE_CODE,'
      'RT.SUBSTITUTE_COST,'
      'RT.SUBSTITUTE_ONHAND,'
      'RT.SUBSTITUTE_MINLEVEL,'
      'RT.SUBSTITUTE_COND1,'
      'RT.SUBSTITUTE_COND2,'
      'RT.SUBSTITUTE_COND3'
      'FROM'
      '(SELECT'
      'SS.SUBSTITUTE_KEY, '
      'SS.STOCK_KEY STOCK_KEY,'
      'SS.SUBSTITUTE_STOCK_KEY SUBSTITUTE_STOCK_KEY,'
      'S.CODE SUBSTITUTE_CODE, '
      'S.DESCRIPTION SUBSTITUTE_NAME,'
      'SL.LATEST_COST SUBSTITUTE_COST,'
      'SL.ON_HAND SUBSTITUTE_ONHAND,'
      'SL.MIN_LEVEL SUBSTITUTE_MINLEVEL,'
      'SSP.SUBSTITUTE_COND1,'
      'SSP.SUBSTITUTE_COND2,'
      'SSP.SUBSTITUTE_COND3 '
      'From STOCK_SUBSTITUTES SS'
      'Left join STOCK_SUBSTITUTE_PERMISSIONS SSP on'
      'ss.SUBSTITUTE_KEY = SSP.SUBSTITUTE_KEY'
      'Left join STOCK S on'
      'SS.SUBSTITUTE_STOCK_KEY = S.STOCK_KEY '
      'Left join STOCKLOCATION SL on'
      'SS.SUBSTITUTE_STOCK_KEY = SL.STOCK_KEY'
      'where SSP.SELECTABLE = '#39'T'#39' AND'
      'SL.LOCATION = :Location) RT'
      'Left join STOCK ST on'
      'RT.STOCK_KEY = ST.STOCK_KEY '
      'Left join STOCKLOCATION STL on'
      'RT.STOCK_KEY = STL.STOCK_KEY'
      'where STL.LOCATION = :Location'
      '')
    Left = 96
    Top = 400
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'LOCATION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Location'
        ParamType = ptUnknown
      end>
  end
end
