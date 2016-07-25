inherited frmMembersEditBrief: TfrmMembersEditBrief
  Left = 98
  Top = 110
  ActiveControl = dbeMember
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Members Edit Brief'
  ClientHeight = 384
  ClientWidth = 569
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label16: TLabel [0]
    Tag = 88
    Left = 15
    Top = 12
    Width = 70
    Height = 15
    AutoSize = False
    Caption = 'Date of Birth'
  end
  inherited stbStatus: TStatusBar
    Top = 364
    Width = 569
  end
  inherited ControlBar1: TControlBar
    Width = 569
    Height = 93
    inherited ToolBar: TToolBar
      Width = 552
      object btnEditFields: TToolButton
        Left = 3
        Top = 0
        Hint = 'Edit Field Watch'
        Caption = 'btnEditFields'
        ImageIndex = 26
        OnClick = menuEditFieldWatchClick
      end
      object btnSaveFields: TToolButton
        Left = 46
        Top = 0
        Hint = 'Save Field Watch'
        Caption = 'btnSaveFields'
        Enabled = False
        ImageIndex = 8
        OnClick = menuSaveFieldWatchClick
      end
      object pnlSpace: TPanel
        Left = 89
        Top = 0
        Width = 414
        Height = 42
        BevelOuter = bvNone
        TabOrder = 1
        object Label2: TLabel
          Tag = 99
          Left = 78
          Top = 14
          Width = 50
          Height = 17
          AutoSize = False
          Caption = 'Member'
        end
        object dbeMember: TwwDBEdit
          Tag = 99
          Left = 146
          Top = 13
          Width = 86
          Height = 19
          HelpContext = 2001
          TabStop = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'MEMBER'
          DataSource = dm.dsrMembersBrief
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
      end
      object btnClose: TToolButton
        Left = 503
        Top = 0
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = btnCloseClick
      end
    end
    object ToolBar1: TToolBar
      Left = 11
      Top = 54
      Width = 552
      Height = 29
      AutoSize = True
      ButtonHeight = 25
      Caption = 'ToolBar1'
      TabOrder = 1
      object dbnQryMembers: TwwDBNavigator
        Left = 0
        Top = 2
        Width = 233
        Height = 25
        AutosizeStyle = asNone
        DataSource = dm.dsrMembersBrief
        Options = []
        ShowHint = True
        RepeatInterval.InitialDelay = 500
        RepeatInterval.Interval = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentShowHint = False
        object dbnQryMembersEdit: TwwNavButton
          Left = 0
          Top = 0
          Width = 55
          Height = 22
          Hint = 'Edit this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersEdit'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 0
          Style = nbsEdit
        end
        object dbnQryMembersPost: TwwNavButton
          Left = 55
          Top = 0
          Width = 55
          Height = 22
          Hint = 'Save changes to this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersPost'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 1
          Style = nbsPost
        end
        object dbnQryMembersCancel: TwwNavButton
          Left = 110
          Top = 0
          Width = 55
          Height = 22
          Hint = 'Cancel changes made to this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersCancel'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 2
          Style = nbsCancel
        end
        object dbnQryMembersRefresh: TwwNavButton
          Left = 165
          Top = 0
          Width = 55
          Height = 22
          Hint = 'Refresh this Members details'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersRefresh'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 3
          Style = nbsRefresh
        end
      end
    end
  end
  object pnlName: TPanel [3]
    Left = 0
    Top = 93
    Width = 569
    Height = 271
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object Label5: TLabel
      Tag = 99
      Left = 357
      Top = 17
      Width = 61
      Height = 23
      AutoSize = False
      Caption = 'First Name'
    end
    object Label6: TLabel
      Tag = 99
      Left = 23
      Top = 16
      Width = 53
      Height = 17
      AutoSize = False
      Caption = 'Surname'
    end
    object Label7: TLabel
      Tag = 99
      Left = 23
      Top = 47
      Width = 63
      Height = 18
      AutoSize = False
      Caption = 'Full Name'
    end
    object Label1: TLabel
      Tag = 88
      Left = 357
      Top = 142
      Width = 70
      Height = 15
      AutoSize = False
      Caption = 'Date of Birth'
    end
    object Label23: TLabel
      Tag = 88
      Left = 357
      Top = 173
      Width = 60
      Height = 15
      AutoSize = False
      Caption = 'Joined'
    end
    object lblAddress: TLabel
      Tag = 88
      Left = 23
      Top = 78
      Width = 80
      Height = 15
      AutoSize = False
      Caption = 'Address'
    end
    object lblAddress1: TLabel
      Tag = 88
      Left = 23
      Top = 109
      Width = 80
      Height = 15
      AutoSize = False
      Caption = 'Address'
    end
    object lblDistrict: TLabel
      Tag = 88
      Left = 23
      Top = 141
      Width = 80
      Height = 18
      AutoSize = False
      Caption = 'District'
    end
    object lblCity: TLabel
      Tag = 88
      Left = 23
      Top = 172
      Width = 80
      Height = 15
      AutoSize = False
      Caption = 'City'
    end
    object lblCountry: TLabel
      Tag = 88
      Left = 23
      Top = 203
      Width = 80
      Height = 21
      AutoSize = False
      Caption = 'Country'
    end
    object Label44: TLabel
      Tag = 88
      Left = 23
      Top = 234
      Width = 77
      Height = 19
      AutoSize = False
      Caption = 'Email'
    end
    object lblActive: TLabel
      Tag = 88
      Left = 357
      Top = 48
      Width = 57
      Height = 15
      AutoSize = False
      Caption = 'Active'
    end
    object Label20: TLabel
      Tag = 88
      Left = 357
      Top = 79
      Width = 64
      Height = 15
      AutoSize = False
      Caption = 'Financial'
    end
    object Label25: TLabel
      Tag = 88
      Left = 357
      Top = 110
      Width = 48
      Height = 16
      AutoSize = False
      Caption = 'Expires'
    end
    object Label13: TLabel
      Tag = 88
      Left = 357
      Top = 204
      Width = 72
      Height = 16
      AutoSize = False
      Caption = 'Phone Home'
    end
    object Label14: TLabel
      Tag = 88
      Left = 357
      Top = 235
      Width = 71
      Height = 15
      AutoSize = False
      Caption = 'Phone Work'
    end
    object dbeFirstName: TwwDBEdit
      Tag = 99
      Left = 444
      Top = 15
      Width = 104
      Height = 19
      HelpContext = 2001
      BiDiMode = bdLeftToRight
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      ParentBiDiMode = False
      DataField = 'FIRST_NAME'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 8
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeSurname: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 15
      Width = 231
      Height = 19
      HelpContext = 2001
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'SURNAME'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 0
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeFullName: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 46
      Width = 231
      Height = 19
      HelpContext = 2001
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'FULL_NAME'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 1
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbdtDOB: TwwDBDateTimePicker
      Tag = 88
      Left = 444
      Top = 140
      Width = 103
      Height = 19
      Hint = 
        'Type in a date or '#13#10'select the down arrow to show a calender'#13#10'Pr' +
        'ess Month or Year for a quick selection '
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
      CalendarAttributes.PopupYearOptions.YearsPerColumn = 25
      CalendarAttributes.PopupYearOptions.NumberColumns = 4
      CalendarAttributes.PopupYearOptions.StartYear = 1920
      Color = clWhite
      DataField = 'DATE_OF_BIRTH'
      DataSource = dm.dsrMembersBrief
      Epoch = 1950
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      ShowButton = True
      TabOrder = 12
      DisplayFormat = 'dd mmm yyyy'
      OnChange = dbdtExpiresChange
      OnExit = dbdtExpiresExit
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbdtJoined: TwwDBDateTimePicker
      Tag = 88
      Left = 444
      Top = 171
      Width = 105
      Height = 19
      Hint = 
        'The Joined date cannot be altered after the Member has been adde' +
        'd'
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
      DataField = 'JOINED'
      DateFormat = dfLong
      DataSource = dm.dsrMembersBrief
      Epoch = 1950
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      ShowButton = True
      TabOrder = 13
      DisplayFormat = 'dd mmm yyyy'
      OnChange = dbdtExpiresChange
      OnExit = dbdtExpiresExit
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeAddress: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 78
      Width = 231
      Height = 19
      HelpContext = 3
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'ADDRESS'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 2
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeAddress1: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 109
      Width = 231
      Height = 19
      HelpContext = 3
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'ADDRESS1'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 3
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeDistrict: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 140
      Width = 231
      Height = 19
      HelpContext = 3
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'DISTRICT'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 4
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeCity: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 171
      Width = 231
      Height = 19
      HelpContext = 3
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'CITY'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 5
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeCountry: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 203
      Width = 231
      Height = 19
      HelpContext = 3
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      DataField = 'COUNTRY'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 6
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbeEmail: TwwDBEdit
      Tag = 99
      Left = 99
      Top = 234
      Width = 231
      Height = 19
      AutoSize = False
      BorderStyle = bsNone
      Color = clWhite
      DataField = 'EMAIL'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 7
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbcbActive: TwwDBComboBox
      Tag = 88
      Left = 444
      Top = 46
      Width = 56
      Height = 19
      HelpContext = 8
      ShowButton = True
      Style = csDropDownList
      MapList = True
      AllowClearKey = False
      AutoSize = False
      BorderStyle = bsNone
      Color = clWhite
      DataField = 'IS_ACTIVE'
      DataSource = dm.dsrMembersBrief
      DropDownCount = 8
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      ItemHeight = 0
      Items.Strings = (
        'YES'#9'Y'
        'NO'#9'N')
      Sorted = False
      TabOrder = 9
      UnboundDataType = wwDefault
      OnKeyPress = dbeSurnameKeyPress
    end
    object wwDBComboBox1: TwwDBComboBox
      Tag = 88
      Left = 444
      Top = 78
      Width = 104
      Height = 19
      Hint = 'Financial is usually altered by making a Payment'
      HelpContext = 9
      ShowButton = True
      Style = csDropDownList
      MapList = True
      AllowClearKey = False
      AutoSize = False
      BorderStyle = bsNone
      Color = clWhite
      DataField = 'SUB_PAID'
      DataSource = dm.dsrMembersBrief
      DropDownCount = 3
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      ItemHeight = 0
      Items.Strings = (
        'NO'#9'0'
        'YES'#9'2'
        'YES (PART)'#9'1')
      Sorted = False
      TabOrder = 10
      UnboundDataType = wwDefault
      OnKeyPress = dbeSurnameKeyPress
    end
    object wwDBEdit6: TwwDBEdit
      Tag = 88
      Left = 444
      Top = 203
      Width = 103
      Height = 19
      HelpContext = 10
      AutoSize = False
      BorderStyle = bsNone
      Color = clWhite
      DataField = 'HOME_PHONE'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 14
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object wwDBEdit7: TwwDBEdit
      Tag = 88
      Left = 444
      Top = 234
      Width = 103
      Height = 19
      HelpContext = 10
      AutoSize = False
      BorderStyle = bsNone
      Color = clWhite
      DataField = 'WORK_PHONE'
      DataSource = dm.dsrMembersBrief
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      TabOrder = 15
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeSurnameKeyPress
    end
    object dbdtExpires: TwwDBDateTimePicker
      Tag = 88
      Left = 444
      Top = 109
      Width = 105
      Height = 19
      Hint = 'The EXPIRES date is altered by making a Payment'
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
      DataSource = dm.dsrMembersBrief
      Epoch = 1950
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      ShowButton = True
      TabOrder = 11
      DisplayFormat = 'dd mmm yyyy'
      OnChange = dbdtExpiresChange
      OnExit = dbdtExpiresExit
      OnKeyPress = dbeSurnameKeyPress
    end
    object FULL_NAME: TCheckBox
      Tag = 10
      Left = 74
      Top = 48
      Width = 15
      Height = 17
      Caption = 'FULL_NAME'
      TabOrder = 16
      Visible = False
    end
    object ADDRESS: TCheckBox
      Tag = 1
      Left = 74
      Top = 79
      Width = 17
      Height = 17
      Caption = 'ADDRESS'
      TabOrder = 17
      Visible = False
    end
    object SURNAME: TCheckBox
      Tag = 15
      Left = 74
      Top = 15
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 18
      Visible = False
    end
    object DISTRICT: TCheckBox
      Tag = 6
      Left = 74
      Top = 140
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 19
      Visible = False
    end
    object CITY: TCheckBox
      Tag = 3
      Left = 74
      Top = 170
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 20
      Visible = False
    end
    object COUNTRY: TCheckBox
      Tag = 4
      Left = 74
      Top = 203
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 21
      Visible = False
    end
    object ADDRESS1: TCheckBox
      Tag = 2
      Left = 74
      Top = 109
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 22
      Visible = False
    end
    object EMAIL: TCheckBox
      Tag = 7
      Left = 74
      Top = 234
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 23
      Visible = False
    end
    object FIRST_NAME: TCheckBox
      Tag = 9
      Left = 423
      Top = 14
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 24
      Visible = False
    end
    object IS_ACTIVE: TCheckBox
      Tag = 12
      Left = 423
      Top = 47
      Width = 15
      Height = 17
      Caption = 'FULL_NAME'
      TabOrder = 25
      Visible = False
    end
    object SUB_PAID: TCheckBox
      Tag = 14
      Left = 423
      Top = 78
      Width = 17
      Height = 17
      Caption = 'ADDRESS'
      TabOrder = 26
      Visible = False
    end
    object EXPIRES: TCheckBox
      Tag = 8
      Left = 423
      Top = 109
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 27
      Visible = False
    end
    object DATE_OF_BIRTH: TCheckBox
      Tag = 5
      Left = 423
      Top = 140
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 28
      Visible = False
    end
    object JOINED: TCheckBox
      Tag = 13
      Left = 423
      Top = 172
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 29
      Visible = False
    end
    object HOME_PHONE: TCheckBox
      Tag = 11
      Left = 423
      Top = 202
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 30
      Visible = False
    end
    object WORK_PHONE: TCheckBox
      Tag = 16
      Left = 423
      Top = 234
      Width = 15
      Height = 19
      Caption = 'FULL_NAME'
      TabOrder = 31
      Visible = False
    end
  end
  inherited MainMenu: TMainMenu
    Left = 482
    Top = 2
    object menuOptions: TMenuItem [1]
      Caption = 'Options'
      object menuEditFieldWatch: TMenuItem
        Caption = 'Edit Field Watch'
        OnClick = menuEditFieldWatchClick
      end
      object menuSaveFieldWatch: TMenuItem
        Caption = 'Save Field Watch'
        OnClick = menuSaveFieldWatchClick
      end
    end
  end
end
