inherited frmTenders: TfrmTenders
  Left = 161
  Top = 106
  ActiveControl = dbgTenders
  BorderIcons = [biSystemMenu]
  Caption = 'frmTenders'
  ClientHeight = 328
  ClientWidth = 471
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 308
    Width = 471
    Height = 0
  end
  object dbgTenders: TwwDBGrid [1]
    Left = 0
    Top = 61
    Width = 471
    Height = 247
    Hint = 'Press F2 to assign the Amount Due to a Tender'
    PictureMasks.Strings = (
      'AMOUNT'#9'[$][#][#][#][#][#][#][#][.][#][#]'#9'T'#9'T'
      'FCURRENCY'#9'[#][#][#][#][#][#][#][.][#][#]'#9'T'#9'T')
    Selected.Strings = (
      'TENDER_TYPE'#9'15'#9'Tender'
      'EX_RATE'#9'10'#9'Exch Rate'
      'AMOUNT'#9'12'#9'Amount'
      'DETAILS'#9'26'#9'Details')
    IniAttributes.Delimiter = ';;'
    TitleColor = clBtnFace
    OnRowChanged = dbgTendersRowChanged
    FixedCols = 0
    ShowHorzScrollBar = False
    ShowVertScrollBar = False
    Align = alClient
    Color = clWhite
    DataSource = dm.dsrTenders
    EditCalculated = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    KeyOptions = [dgEnterToTab]
    Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgFooter3DCells]
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    TitleAlignment = taLeftJustify
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clBlack
    TitleFont.Height = -15
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    TitleLines = 1
    TitleButtons = True
    OnCalcCellColors = dbgTendersCalcCellColors
    OnDblClick = dbgTendersDblClick
    IndicatorColor = icBlack
    OnFieldChanged = dbgTendersFieldChanged
  end
  inherited ControlBar1: TControlBar
    Width = 471
    Height = 61
    inherited ToolBar: TToolBar
      Width = 454
      Align = alClient
      ButtonWidth = 47
      Indent = 0
      inherited pnlFill: TPanel
        Left = 0
        TabOrder = 3
      end
      object Panel3: TPanel
        Left = 1
        Top = 0
        Width = 70
        Height = 42
        BevelOuter = bvNone
        Caption = 'Amount Due'
        TabOrder = 1
      end
      object dbePayment: TSpeedButton
        Left = 71
        Top = 0
        Width = 80
        Height = 42
        Flat = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = False
      end
      object Panel1: TPanel
        Left = 151
        Top = 0
        Width = 67
        Height = 42
        BevelOuter = bvNone
        Caption = 'Change'
        TabOrder = 2
      end
      object dbeChange: TSpeedButton
        Left = 218
        Top = 0
        Width = 80
        Height = 42
        Flat = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = False
      end
      object pnlSpace: TPanel
        Left = 298
        Top = 0
        Width = 55
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
      end
      object ToolButton2: TToolButton
        Left = 353
        Top = 0
        Caption = 'ToolButton2'
        ImageIndex = 7
        OnClick = menuExitClick
      end
      object btnOK: TToolButton
        Left = 400
        Top = 0
        Caption = 'btnOK'
        ImageIndex = 8
        OnClick = btnOKClick
      end
    end
  end
  object StatusBar1: TStatusBar [3]
    Left = 0
    Top = 308
    Width = 471
    Height = 20
    Hint = 
      'Displays hints|Hints are displayed here as you move over parts o' +
      'f the form'
    Color = clSilver
    Panels = <
      item
        Width = 100
      end
      item
        Width = 50
      end>
    ParentFont = True
    SimplePanel = False
    UseSystemFont = False
  end
  inherited MainMenu: TMainMenu
    Left = 266
    Top = 186
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
  end
end
