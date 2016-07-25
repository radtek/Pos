inherited frmMembersLookup: TfrmMembersLookup
  Tag = 6
  Left = 334
  Top = 147
  ActiveControl = dbgLookup
  BorderIcons = [biSystemMenu]
  Caption = 'frmMembersLookup'
  ClientHeight = 439
  ClientWidth = 429
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnKeyUp = nil
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 419
    Width = 429
  end
  object dbgLookup: TwwDBGrid [1]
    Left = 0
    Top = 60
    Width = 429
    Height = 359
    Hint = 
      'Double Click to select One record, Ctrl - Left Click to select  ' +
      'Multpile records'
    IniAttributes.Delimiter = ';;'
    TitleColor = clBtnFace
    FixedCols = 0
    ShowHorzScrollBar = True
    Align = alClient
    Color = clWhite
    KeyOptions = []
    MultiSelectOptions = [msoShiftSelect]
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgMultiSelect]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    TitleAlignment = taLeftJustify
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clBlack
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    TitleLines = 1
    TitleButtons = False
    OnCalcCellColors = dbgLookupCalcCellColors
    OnDblClick = btnOKClick
    OnKeyPress = dbgLookupKeyPress
    IndicatorColor = icBlack
  end
  inherited ControlBar1: TControlBar
    Width = 429
    inherited ToolBar: TToolBar
      Width = 412
      ButtonWidth = 42
      inherited pnlFill: TPanel
        TabOrder = 2
      end
      object pnlFind: TPanel
        Left = 3
        Top = 0
        Width = 34
        Height = 42
        BevelOuter = bvNone
        Caption = '&Find'
        TabOrder = 0
      end
      object pnlSpace: TPanel
        Left = 37
        Top = 0
        Width = 285
        Height = 42
        BevelOuter = bvNone
        TabOrder = 1
        object incSearch: TwwIncrementalSearch
          Left = 8
          Top = 11
          Width = 81
          Height = 21
          ShowMatchText = True
          TabOrder = 0
          OnKeyPress = incSearchKeyPress
        end
        object rpgSearch: TLMDRadioGroup
          Left = 106
          Top = 1
          Width = 99
          Height = 42
          Alignment = gcaRightTop
          Bevel.BorderSides = []
          Bevel.EdgeStyle = etBump
          Bevel.Mode = bmStandard
          Bevel.StandardStyle = lsNone
          CaptionFont.Charset = DEFAULT_CHARSET
          CaptionFont.Color = clBlack
          CaptionFont.Height = -11
          CaptionFont.Name = 'Arial'
          CaptionFont.Style = []
          CaptionParentFont = False
          Items.Strings = (
            'Description'
            'Code'
            'Short Code')
          ItemOffset = 10
          TabOrder = 1
          OnChange = rpgSearchChange
          ItemIndex = 1
        end
      end
      object btnCancel: TToolButton
        Left = 322
        Top = 0
        ImageIndex = 7
        OnClick = btnCancelClick
      end
      object btnOK: TToolButton
        Left = 364
        Top = 0
        Caption = 'btnOK'
        ImageIndex = 8
        OnClick = btnOKClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 290
    Top = 74
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
  end
end
