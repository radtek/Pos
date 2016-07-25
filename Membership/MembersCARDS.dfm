object frmMembersCARDS: TfrmMembersCARDS
  Left = 207
  Top = 195
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Print Cards '
  ClientHeight = 215
  ClientWidth = 412
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnDeactivate = FormDeactivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label11: TLabel
    Left = 204
    Top = 94
    Width = 49
    Height = 13
    Caption = '# of Cards'
  end
  object Label4: TLabel
    Left = 21
    Top = 26
    Width = 41
    Height = 13
    Caption = 'Member '
  end
  object Label1: TLabel
    Left = 21
    Top = 60
    Width = 28
    Height = 13
    Caption = 'Name'
  end
  object Label2: TLabel
    Left = 21
    Top = 93
    Width = 34
    Height = 13
    Caption = 'Expires'
  end
  object edtNumberCards: TEdit
    Left = 267
    Top = 91
    Width = 50
    Height = 21
    TabOrder = 1
    Text = '1'
    OnKeyPress = edtNumberCardsKeyPress
  end
  object UpDown1: TUpDown
    Left = 317
    Top = 91
    Width = 15
    Height = 21
    Associate = edtNumberCards
    Min = 1
    Position = 1
    TabOrder = 2
    Wrap = False
  end
  object dblcMember: TwwDBLookupCombo
    Left = 71
    Top = 24
    Width = 121
    Height = 19
    Hint = 
      'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
      ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
    AutoSize = False
    CharCase = ecUpperCase
    BorderStyle = bsNone
    DropDownAlignment = taLeftJustify
    Selected.Strings = (
      'MEMBER'#9'9'#9'Member'#9'F'
      'FULL_NAME'#9'18'#9'Full Name'#9'F'
      'FIRST_NAME'#9'12'#9'First Name'#9'F'
      'SURNAME'#9'15'#9'Surname'#9'F'
      'ADDRESS'#9'20'#9'Address'#9'F'
      'MAILING_LIST_CODE'#9'8'#9'Parent Code'#9'F')
    LookupTable = dm.qryMembersLook
    LookupField = 'SURNAME'
    Options = [loColLines, loRowLines, loTitles]
    AutoSelect = False
    Color = clWhite
    DropDownCount = 10
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    TabOrder = 0
    AutoDropDown = False
    ShowButton = True
    AllowClearKey = False
    OnDropDown = dblcMemberDropDown
    OnKeyPress = dblcMemberKeyPress
  end
  object btnOKPrintCard: TButton
    Left = 307
    Top = 174
    Width = 87
    Height = 25
    Hint = 'Print a Card now'
    Caption = 'Print Card'
    TabOrder = 3
    OnClick = btnOKPrintCardClick
  end
  object btnCancelPrintCard: TButton
    Left = 318
    Top = 13
    Width = 87
    Height = 25
    Caption = 'Close'
    TabOrder = 4
    OnClick = btnCancelPrintCardClick
  end
  object btnSaveCard: TButton
    Left = 23
    Top = 129
    Width = 87
    Height = 25
    Hint = 'Save the Member for Printing/Exporting'
    Caption = 'Save Card'
    TabOrder = 5
    OnClick = btnSaveCardClick
  end
  object dbeFullName: TwwDBEdit
    Tag = 99
    Left = 71
    Top = 58
    Width = 260
    Height = 19
    HelpContext = 2
    AutoSize = False
    BorderStyle = bsNone
    CharCase = ecUpperCase
    Color = clWhite
    DataField = 'FULL_NAME'
    DataSource = dm.dsrMembersLook
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    TabOrder = 6
    UnboundDataType = wwDefault
    WantReturns = False
    WordWrap = False
  end
  object btnEncodeCard: TButton
    Left = 23
    Top = 172
    Width = 87
    Height = 25
    Hint = 'RenCode the Members Card'
    Caption = 'Encode'
    TabOrder = 7
    OnClick = btnEncodeCardClick
  end
  object btnCardEject: TButton
    Left = 162
    Top = 128
    Width = 87
    Height = 25
    Caption = 'Card Eject'
    TabOrder = 8
    OnClick = btnCardEjectClick
  end
  object btnReadEncoding: TButton
    Left = 164
    Top = 173
    Width = 87
    Height = 25
    Caption = 'Read Encoding'
    TabOrder = 9
    OnClick = btnReadEncodingClick
  end
  object dbdtExpires: TwwDBDateTimePicker
    Tag = 88
    Left = 71
    Top = 91
    Width = 100
    Height = 19
    AutoSize = False
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
    DataField = 'EXPIRES'
    DateFormat = dfLong
    DataSource = dm.dsrMembersLook
    Epoch = 1950
    Frame.Enabled = True
    Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
    Frame.FocusStyle = efsFrameSunken
    Frame.NonFocusStyle = efsFrameSunken
    Frame.NonFocusTransparentFontColor = clBlack
    ShowButton = True
    TabOrder = 10
    DisplayFormat = 'dd mmm yyyy'
  end
  object btnResetEncoder: TButton
    Left = 308
    Top = 128
    Width = 87
    Height = 25
    Caption = 'Reset Encoder'
    TabOrder = 11
    OnClick = btnResetEncoderClick
  end
end
