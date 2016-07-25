object DlDBXConfigureForm: TDlDBXConfigureForm
  Left = 434
  Top = 416
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Database Connection Parameters'
  ClientHeight = 354
  ClientWidth = 442
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 313
    Width = 442
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      442
      41)
    object butnOk: TButton
      Left = 278
      Top = 8
      Width = 75
      Height = 25
      Action = actnOk
      Anchors = [akTop, akRight]
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object butnCancel: TButton
      Left = 358
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 313
    Align = alLeft
    BorderWidth = 6
    TabOrder = 1
    object lboxConnection: TListBox
      Left = 7
      Top = 57
      Width = 171
      Height = 249
      Align = alClient
      ItemHeight = 13
      TabOrder = 0
      OnClick = lboxConnectionClick
    end
    object Panel4: TPanel
      Left = 7
      Top = 7
      Width = 171
      Height = 50
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object Label1: TLabel
        Left = 0
        Top = 8
        Width = 88
        Height = 13
        Caption = 'Connection Name:'
        Transparent = False
      end
      object editConnection: TEdit
        Left = 0
        Top = 25
        Width = 169
        Height = 21
        TabOrder = 0
        OnChange = editConnectionChange
        OnKeyPress = editConnectionKeyPress
      end
    end
  end
  object Panel3: TPanel
    Left = 185
    Top = 0
    Width = 257
    Height = 313
    Align = alClient
    TabOrder = 2
    object Label2: TLabel
      Left = 16
      Top = 16
      Width = 95
      Height = 13
      Caption = 'Connection Settings'
      Transparent = False
    end
    object sgridConnectionSettings: TStringGrid
      Left = 6
      Top = 32
      Width = 245
      Height = 273
      ColCount = 2
      DefaultColWidth = 120
      DefaultRowHeight = 21
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
      TabOrder = 0
    end
  end
  object ActionList1: TActionList
    Left = 8
    Top = 320
    object actnOk: TAction
      Caption = '&Ok'
      OnUpdate = actnOkUpdate
    end
  end
end
