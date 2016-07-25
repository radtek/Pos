inherited frmMemberVisits: TfrmMemberVisits
  Left = 317
  Top = 155
  Caption = 'Member Visits'
  ClientHeight = 250
  ClientWidth = 326
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel [0]
    Tag = 99
    Left = 69
    Top = 117
    Width = 38
    Height = 13
    Caption = 'Member'
  end
  object Label2: TLabel [1]
    Tag = 99
    Left = 68
    Top = 187
    Width = 42
    Height = 13
    Caption = 'Last Visit'
  end
  object Label3: TLabel [2]
    Tag = 99
    Left = 68
    Top = 152
    Width = 59
    Height = 13
    Caption = 'Visit Number'
  end
  inherited stbStatus: TStatusBar
    Top = 230
    Width = 326
  end
  inherited ControlBar1: TControlBar
    Width = 326
    inherited ToolBar: TToolBar
      Width = 309
      ButtonHeight = 44
      inherited pnlFill: TPanel
        Height = 44
      end
      object btnClose: TToolButton
        Left = 3
        Top = 0
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = btnCloseClick
      end
    end
  end
  object dbeMember: TwwDBEdit [5]
    Tag = 99
    Left = 140
    Top = 116
    Width = 86
    Height = 19
    HelpContext = 2001
    TabStop = False
    AutoSize = False
    BorderStyle = bsNone
    Color = clWhite
    DataField = 'MEMBER'
    DataSource = dm.dsrMembers
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    ReadOnly = True
    TabOrder = 2
    UnboundDataType = wwDefault
    WantReturns = False
    WordWrap = False
  end
  object dbdtExpires: TwwDBDateTimePicker [6]
    Tag = 88
    Left = 140
    Top = 186
    Width = 146
    Height = 19
    BorderStyle = bsNone
    CalendarAttributes.Colors.TextColor = clBlack
    CalendarAttributes.Colors.TitleBackColor = clSilver
    CalendarAttributes.Colors.TitleTextColor = clBlack
    CalendarAttributes.Font.Charset = DEFAULT_CHARSET
    CalendarAttributes.Font.Color = clWindowText
    CalendarAttributes.Font.Height = -11
    CalendarAttributes.Font.Name = 'MS Sans Serif'
    CalendarAttributes.Font.Style = []
    CalendarAttributes.Options = [mdoDayState, mdoWeekNumbers]
    CalendarAttributes.PopupYearOptions.NumberColumns = 3
    CalendarAttributes.PopupYearOptions.ShowEditYear = True
    Color = clWhite
    DataField = 'LAST_VISIT'
    DateFormat = dfLong
    DataSource = dm.dsrMembers
    Epoch = 1950
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    ShowButton = True
    TabOrder = 3
    DisplayFormat = 'dd mmm yyyy hh:mm:ss'
  end
  object wwDBEdit1: TwwDBEdit [7]
    Tag = 99
    Left = 141
    Top = 150
    Width = 86
    Height = 19
    HelpContext = 2001
    TabStop = False
    AutoSize = False
    BorderStyle = bsNone
    Color = clWhite
    DataField = 'VISITS'
    DataSource = dm.dsrMembers
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    TabOrder = 4
    UnboundDataType = wwDefault
    WantReturns = False
    WordWrap = False
  end
  object DBNavigator: TwwDBNavigator [8]
    Left = 91
    Top = 64
    Width = 134
    Height = 43
    AutosizeStyle = asNone
    DataSource = dm.dsrMembers
    ShowHint = True
    RepeatInterval.InitialDelay = 500
    RepeatInterval.Interval = 100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentShowHint = False
    object wwNavButton4: TwwNavButton
      Left = 0
      Top = 0
      Width = 40
      Height = 40
      Hint = 'Post changes of current record'
      ImageIndex = -1
      NumGlyphs = 2
      Spacing = 4
      Transparent = False
      Caption = 'Post'
      Enabled = False
      DisabledTextColors.ShadeColor = clGray
      DisabledTextColors.HighlightColor = clBtnHighlight
      Index = 0
      Style = nbsPost
    end
    object wwNavButton5: TwwNavButton
      Left = 40
      Top = 0
      Width = 40
      Height = 40
      Hint = 'Cancel changes made to current record'
      ImageIndex = -1
      NumGlyphs = 2
      Spacing = 4
      Transparent = False
      Caption = 'Cancel'
      Enabled = False
      DisabledTextColors.ShadeColor = clGray
      DisabledTextColors.HighlightColor = clBtnHighlight
      Index = 1
      Style = nbsCancel
    end
    object wwNavButton6: TwwNavButton
      Left = 80
      Top = 0
      Width = 40
      Height = 40
      Hint = 'Refresh the contents of the dataset'
      ImageIndex = -1
      NumGlyphs = 2
      Spacing = 4
      Transparent = False
      Caption = 'Refresh'
      Enabled = False
      DisabledTextColors.ShadeColor = clGray
      DisabledTextColors.HighlightColor = clBtnHighlight
      Index = 2
      Style = nbsRefresh
    end
  end
  inherited MainMenu: TMainMenu
    Left = 234
    Top = 65522
  end
end
