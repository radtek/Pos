inherited frmMembersLook: TfrmMembersLook
  Tag = 6
  Left = 119
  Top = 96
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'frmMembersLook'
  ClientHeight = 359
  ClientWidth = 581
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 339
    Width = 581
  end
  object pnlArea: TPanel [1]
    Left = 0
    Top = 60
    Width = 581
    Height = 279
    Align = alClient
    BevelInner = bvLowered
    BevelOuter = bvNone
    BevelWidth = 2
    Caption = 'pnlArea'
    TabOrder = 2
    object dbgSubs: TwwDBGrid
      Left = 2
      Top = 2
      Width = 577
      Height = 62
      Selected.Strings = (
        'MEMBER'#9'10'#9'Member'
        'SUBSCRIPTION'#9'13'#9'Subscription'
        'DESCRIPTION'#9'30'#9'Description'
        'RECEIPT'#9'8'#9'Receipt'
        'AMOUNT_PAID'#9'12'#9'Amt Paid'
        'AMOUNT'#9'10'#9'Amount')
      IniAttributes.Delimiter = ';;'
      TitleColor = clBtnFace
      FixedCols = 0
      ShowHorzScrollBar = True
      Align = alClient
      Color = clWhite
      DataSource = dm.dsrM_Subs
      KeyOptions = []
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
      ReadOnly = True
      TabOrder = 0
      TitleAlignment = taLeftJustify
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clBlack
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      TitleLines = 1
      TitleButtons = False
      IndicatorColor = icBlack
    end
    object dbgSections: TwwDBGrid
      Left = 2
      Top = 115
      Width = 577
      Height = 103
      Selected.Strings = (
        'SECTION'#9'9'#9'Code'
        'DESCRIPTION'#9'27'#9'Description'
        'EXPIRES'#9'17'#9'Expires'
        'RECEIPT'#9'7'#9'Receipt'
        'AMOUNT_PAID'#9'14'#9'Amt Paid'
        'AMOUNT'#9'11'#9'Amount')
      IniAttributes.Delimiter = ';;'
      TitleColor = clBtnFace
      FixedCols = 0
      ShowHorzScrollBar = True
      Align = alBottom
      Color = clWhite
      DataSource = dm.dsrM_Sections
      KeyOptions = []
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
      ReadOnly = True
      TabOrder = 1
      TitleAlignment = taLeftJustify
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clBlack
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      TitleLines = 1
      TitleButtons = False
      IndicatorColor = icBlack
    end
    object dbgEvents: TwwDBGrid
      Left = 2
      Top = 218
      Width = 577
      Height = 59
      Selected.Strings = (
        'EVENT'#9'16'#9'Event'
        'DESCRIPTION'#9'44'#9'Description'
        'DATES'#9'26'#9'Date')
      IniAttributes.Delimiter = ';;'
      TitleColor = clBtnFace
      FixedCols = 0
      ShowHorzScrollBar = True
      Align = alBottom
      Color = clWhite
      DataSource = dm.dsrM_Events
      KeyOptions = []
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
      ReadOnly = True
      TabOrder = 2
      TitleAlignment = taLeftJustify
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clBlack
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      TitleLines = 1
      TitleButtons = False
      IndicatorColor = icBlack
    end
    object strGrid: TStringGrid
      Left = 2
      Top = 64
      Width = 577
      Height = 51
      Align = alBottom
      ColCount = 3
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 100
      TabOrder = 3
      ColWidths = (
        113
        266
        169)
    end
  end
  inherited ControlBar1: TControlBar
    Width = 581
    inherited ToolBar: TToolBar
      Width = 564
      inherited pnlFill: TPanel
        Width = 512
      end
      object ToolButton1: TToolButton
        Left = 514
        Top = 0
        Caption = 'ToolButton1'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
  end
end
