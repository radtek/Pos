inherited frmMembersEDIT: TfrmMembersEDIT
  Left = 222
  Top = 112
  Caption = 'frmMembersEDIT'
  ClientHeight = 475
  ClientWidth = 656
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 455
    Width = 656
  end
  object pnlMember: TPanel [1]
    Left = 0
    Top = 89
    Width = 656
    Height = 83
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object pnlName: TPanel
      Left = 27
      Top = 4
      Width = 588
      Height = 75
      BevelOuter = bvNone
      TabOrder = 0
      object Label1: TLabel
        Tag = 99
        Left = 6
        Top = 14
        Width = 38
        Height = 13
        Caption = 'Member'
      end
      object Label3: TLabel
        Tag = 99
        Left = 155
        Top = 15
        Width = 20
        Height = 13
        Caption = 'Title'
      end
      object Label4: TLabel
        Tag = 99
        Left = 277
        Top = 15
        Width = 29
        Height = 13
        Caption = 'Initials'
      end
      object Label5: TLabel
        Tag = 99
        Left = 407
        Top = 15
        Width = 50
        Height = 13
        Caption = 'First Name'
      end
      object Label6: TLabel
        Tag = 99
        Left = 5
        Top = 48
        Width = 42
        Height = 13
        Caption = 'Surname'
      end
      object Label2: TLabel
        Tag = 99
        Left = 275
        Top = 49
        Width = 47
        Height = 13
        Caption = 'Full Name'
      end
      object dbeMember: TwwDBEdit
        Tag = 99
        Left = 58
        Top = 13
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
        TabOrder = 0
        UnboundDataType = wwDefault
        WantReturns = False
        WordWrap = False
        OnChange = dbeMemberChange
        OnExit = dbeMemberExit
        OnKeyPress = dbeMemberKeyPress
      end
      object dbcbTitle: TwwDBComboBox
        Tag = 99
        Left = 195
        Top = 13
        Width = 60
        Height = 19
        HelpContext = 2001
        ShowButton = True
        Style = csDropDown
        MapList = False
        AllowClearKey = True
        AutoSize = False
        BorderStyle = bsNone
        CharCase = ecUpperCase
        Color = clWhite
        DataField = 'TITLE'
        DataSource = dm.dsrMembers
        DropDownCount = 8
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        Frame.NonFocusTransparentFontColor = clBlack
        ItemHeight = 0
        Items.Strings = (
          'MR'#9
          'MRS'#9
          'MISS'#9
          'MS'
          'CR'#9
          'SIR')
        Sorted = False
        TabOrder = 1
        UnboundDataType = wwDefault
        UnboundAlignment = taRightJustify
        OnExit = dbcbTitleExit
        OnKeyPress = dbeMemberKeyPress
      end
      object dbeInitials: TwwDBEdit
        Tag = 99
        Left = 339
        Top = 13
        Width = 52
        Height = 19
        HelpContext = 2001
        AutoSize = False
        BorderStyle = bsNone
        CharCase = ecUpperCase
        Color = clWhite
        DataField = 'INITIALS'
        DataSource = dm.dsrMembers
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        Frame.NonFocusTransparentFontColor = clBlack
        TabOrder = 2
        UnboundDataType = wwDefault
        WantReturns = False
        WordWrap = False
        OnExit = dbcbTitleExit
        OnKeyPress = dbeMemberKeyPress
      end
      object dbeFirstName: TwwDBEdit
        Tag = 99
        Left = 474
        Top = 13
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
        DataSource = dm.dsrMembers
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        Frame.NonFocusTransparentFontColor = clBlack
        TabOrder = 3
        UnboundDataType = wwDefault
        WantReturns = False
        WordWrap = False
        OnExit = dbcbTitleExit
        OnKeyPress = dbeMemberKeyPress
      end
      object dbeSurname: TwwDBEdit
        Tag = 99
        Left = 58
        Top = 47
        Width = 195
        Height = 19
        HelpContext = 2001
        AutoSize = False
        BorderStyle = bsNone
        CharCase = ecUpperCase
        Color = clWhite
        DataField = 'SURNAME'
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
        OnExit = dbcbTitleExit
        OnKeyPress = dbeMemberKeyPress
      end
      object dbeFullName: TwwDBEdit
        Tag = 99
        Left = 339
        Top = 47
        Width = 241
        Height = 19
        HelpContext = 2001
        AutoSize = False
        BorderStyle = bsNone
        CharCase = ecUpperCase
        Color = clWhite
        DataField = 'FULL_NAME'
        DataSource = dm.dsrMembers
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        Frame.NonFocusTransparentFontColor = clBlack
        TabOrder = 5
        UnboundDataType = wwDefault
        WantReturns = False
        WordWrap = False
        OnKeyPress = dbeMemberKeyPress
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 656
    Height = 89
    TabOrder = 2
    inherited ToolBar: TToolBar
      Width = 639
      Align = alClient
      AutoSize = True
      ButtonWidth = 42
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object btnFind: TToolButton
        Left = 3
        Top = 0
        Hint = 'Search by Events'#13#10'or Groups'#13#10'or Sections '#13#10'or Subscriptions'#13#10
        Caption = 'btnFind'
        DropdownMenu = PopupMenu
        ImageIndex = 9
      end
      object btnNumbers: TToolButton
        Left = 45
        Top = 0
        Hint = 'Set up Member Number group'
        Caption = 'btnNumbers'
        DropdownMenu = PopupMenuNumbers
        ImageIndex = 27
        PopupMenu = PopupMenuNumbers
      end
      object btnQuery: TToolButton
        Left = 87
        Top = 0
        Hint = 'Search for members by Query'
        Caption = 'btnQuery'
        ImageIndex = 10
        OnClick = dbnQryMembersLookFilterDialogClick
      end
      object btnCopyMember: TToolButton
        Left = 129
        Top = 0
        Hint = 'Copy details from another Member'
        Caption = 'btnCopyMember'
        ImageIndex = 26
        OnClick = menuCopyMemberClick
      end
      object btnAttached: TToolButton
        Left = 171
        Top = 0
        Hint = 'Lookup Attached Members'
        Caption = 'btnAttached'
        ImageIndex = 28
        OnClick = menuAttachedMembersClick
      end
      object btnPrint: TToolButton
        Left = 213
        Top = 0
        Hint = 'Print Member Details'
        Caption = 'btnPrint'
        ImageIndex = 12
        OnClick = menuPrintMemberDetailsClick
      end
      object pnlFind: TPanel
        Left = 255
        Top = 0
        Width = 54
        Height = 42
        BevelOuter = bvNone
        Caption = '&Find'
        TabOrder = 0
      end
      object pnlSpace: TPanel
        Left = 309
        Top = 0
        Width = 279
        Height = 42
        BevelOuter = bvNone
        TabOrder = 2
        object lblPoints: TLabel
          Left = 160
          Top = 16
          Width = 103
          Height = 13
          Caption = 'Points Security is OFF'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object dblcSurname: TwwDBLookupCombo
          Tag = 99
          Left = 7
          Top = 12
          Width = 121
          Height = 20
          Hint = 
            'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
            ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow ' +
            #13#10'or P then a '#39'Member Numer'#39' to list the Parent Codes'
          TabStop = False
          AutoSize = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'8'#9'Member'#9'F'
            'FULL_NAME'#9'18'#9'Full Name'#9'F'
            'FIRST_NAME'#9'12'#9'First Name'#9'F'
            'SURNAME'#9'15'#9'Surname'#9'F'
            'ADDRESS'#9'15'#9'Address'#9'F'
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
          OnDropDown = dblcSurnameDropDown
          OnCloseUp = dblcSurnameCloseUp
          OnEnter = dblcSurnameEnter
          OnKeyPress = dblcSurnameKeyPress
        end
      end
      object btnClose: TToolButton
        Left = 588
        Top = 0
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
    object ToolBar1: TToolBar
      Left = 11
      Top = 54
      Width = 639
      Height = 22
      Align = alClient
      ButtonHeight = 19
      EdgeBorders = []
      EdgeInner = esNone
      EdgeOuter = esNone
      Indent = 4
      TabOrder = 1
      object dbnQryMembersLook: TwwDBNavigator
        Left = 4
        Top = 2
        Width = 220
        Height = 19
        AutosizeStyle = asNone
        DataSource = dm.dsrMembersLook
        ImageList = dm.ImageListLarge
        ShowHint = True
        RepeatInterval.InitialDelay = 500
        RepeatInterval.Interval = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentShowHint = False
        object dbnQryMembersLookFirst: TwwNavButton
          Left = 0
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Move to the first Members record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersLookFirst'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersLookLastClick
          Index = 0
          Style = nbsFirst
          OnRowChanged = dbnQryMembersLookLastRowChanged
        end
        object dbnQryMembersLookPrior: TwwNavButton
          Left = 55
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Move to the prior Members record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersLookPrior'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersLookNextClick
          Index = 1
          Style = nbsPrior
          OnRowChanged = dbnQryMembersLookNextRowChanged
        end
        object dbnQryMembersLookNext: TwwNavButton
          Left = 110
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Move to the next Members record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersLookNext'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersLookNextClick
          Index = 2
          Style = nbsNext
          OnRowChanged = dbnQryMembersLookNextRowChanged
        end
        object dbnQryMembersLookLast: TwwNavButton
          Left = 165
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Move to the last Members record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersLookLast'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersLookLastClick
          Index = 3
          Style = nbsLast
          OnRowChanged = dbnQryMembersLookLastRowChanged
        end
      end
      object dbnQryMembers: TwwDBNavigator
        Left = 224
        Top = 2
        Width = 335
        Height = 19
        AutosizeStyle = asNone
        DataSource = dm.dsrMembers
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
        object dbnQryMembersInsert: TwwNavButton
          Left = 0
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Add a new Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersInsert'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersInsertClick
          Index = 0
          Style = nbsInsert
        end
        object dbnQryMembersDelete: TwwNavButton
          Left = 55
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Delete this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersDelete'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersDeleteClick
          Index = 1
          Style = nbsDelete
        end
        object dbnQryMembersEdit: TwwNavButton
          Left = 110
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Edit this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersEdit'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 2
          Style = nbsEdit
        end
        object dbnQryMembersPost: TwwNavButton
          Left = 165
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Save changes to this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersPost'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 3
          Style = nbsPost
        end
        object dbnQryMembersCancel: TwwNavButton
          Left = 220
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Cancel changes made to this Member'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersCancel'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersCancelClick
          Index = 4
          Style = nbsCancel
        end
        object dbnQryMembersRefresh: TwwNavButton
          Left = 275
          Top = 0
          Width = 55
          Height = 19
          Hint = 'Refresh this Members details'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryMembersRefresh'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnQryMembersRefreshClick
          Index = 5
          Style = nbsRefresh
        end
      end
    end
  end
  object PageControl: TPageControl [3]
    Left = 0
    Top = 172
    Width = 656
    Height = 283
    ActivePage = tsDiscounts
    Align = alClient
    MultiLine = True
    RaggedRight = True
    TabOrder = 3
    TabStop = False
    OnChange = PageControlChange
    object tsMaintenance: TTabSheet
      Caption = 'Address'
      object pnlMaintenance: TPanel
        Left = 36
        Top = 11
        Width = 548
        Height = 229
        AutoSize = True
        BevelOuter = bvNone
        TabOrder = 0
        object lblAddress: TLabel
          Tag = 88
          Left = 0
          Top = 1
          Width = 76
          Height = 13
          Hint = 'Click to see the Postal/Street Address'
          Caption = 'Address (Postal)'
          OnClick = lblAddressClick
        end
        object lblAddress1: TLabel
          Tag = 88
          Left = 0
          Top = 31
          Width = 76
          Height = 13
          Hint = 'Click to see the Postal/Street Address'
          Caption = 'Address (Postal)'
          OnClick = lblAddressClick
        end
        object lblDistrict: TLabel
          Tag = 88
          Left = 0
          Top = 61
          Width = 70
          Height = 13
          Hint = 'Click to see the Postal/Street Address'
          Caption = 'District (Postal)'
          OnClick = lblAddressClick
        end
        object lblCity: TLabel
          Tag = 88
          Left = 0
          Top = 91
          Width = 55
          Height = 13
          Hint = 'Click to see the Postal/Street Address'
          Caption = 'City (Postal)'
          OnClick = lblAddressClick
        end
        object lblCountry: TLabel
          Tag = 88
          Left = 0
          Top = 121
          Width = 74
          Height = 13
          Caption = 'Country (Postal)'
          OnClick = lblAddressClick
        end
        object Label11: TLabel
          Tag = 88
          Left = 0
          Top = 181
          Width = 49
          Height = 13
          Caption = 'Post Code'
        end
        object Label12: TLabel
          Tag = 88
          Left = 0
          Top = 210
          Width = 59
          Height = 13
          Caption = 'Parent Code'
        end
        object lblActive: TLabel
          Tag = 88
          Left = 358
          Top = 2
          Width = 30
          Height = 13
          Caption = 'Active'
        end
        object Label20: TLabel
          Tag = 88
          Left = 358
          Top = 31
          Width = 42
          Height = 13
          Caption = 'Financial'
        end
        object Label13: TLabel
          Tag = 88
          Left = 358
          Top = 93
          Width = 62
          Height = 13
          Caption = 'Phone Home'
        end
        object Label14: TLabel
          Tag = 88
          Left = 358
          Top = 123
          Width = 60
          Height = 13
          Caption = 'Phone Work'
        end
        object Label15: TLabel
          Tag = 88
          Left = 358
          Top = 153
          Width = 51
          Height = 13
          Hint = 'Click to see the Work/Home Fax Number'
          Caption = 'Phone Cell'
        end
        object Label8: TLabel
          Tag = 88
          Left = 358
          Top = 213
          Width = 48
          Height = 13
          Caption = 'Known As'
        end
        object lblFax: TLabel
          Tag = 88
          Left = 358
          Top = 183
          Width = 52
          Height = 13
          Caption = 'Fax (Work)'
          OnClick = lblFaxClick
        end
        object Label66: TLabel
          Tag = 88
          Left = 184
          Top = 181
          Width = 68
          Height = 13
          Caption = 'Receive Email'
        end
        object Label44: TLabel
          Tag = 88
          Left = 1
          Top = 151
          Width = 66
          Height = 13
          Caption = 'Email Address'
        end
        object Label25: TLabel
          Tag = 88
          Left = 359
          Top = 61
          Width = 34
          Height = 13
          Caption = 'Expires'
        end
        object dbeAddress: TwwDBEdit
          Tag = 99
          Left = 90
          Top = 0
          Width = 231
          Height = 19
          HelpContext = 3
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'ADDRESS'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnDblClick = lblAddressClick
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeAddress1: TwwDBEdit
          Tag = 99
          Left = 90
          Top = 30
          Width = 231
          Height = 19
          HelpContext = 3
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'ADDRESS1'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnDblClick = lblAddressClick
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeDistrict: TwwDBEdit
          Tag = 99
          Left = 90
          Top = 60
          Width = 231
          Height = 19
          HelpContext = 3
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'DISTRICT'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 2
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnDblClick = lblAddressClick
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeCity: TwwDBEdit
          Tag = 99
          Left = 90
          Top = 90
          Width = 231
          Height = 19
          HelpContext = 3
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'CITY'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 3
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnDblClick = lblAddressClick
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeCountry: TwwDBEdit
          Tag = 99
          Left = 90
          Top = 119
          Width = 231
          Height = 19
          HelpContext = 3
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'COUNTRY'
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
          OnDblClick = lblAddressClick
          OnKeyPress = dbeMemberKeyPress
        end
        object dblcPostCode: TwwDBLookupCombo
          Tag = 88
          Left = 90
          Top = 179
          Width = 79
          Height = 20
          HelpContext = 4
          AutoSize = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taRightJustify
          Selected.Strings = (
            'MAIL_CODE'#9'8'#9'Mail Code'#9'F'
            'DESCRIPTION'#9'15'#9'District'#9'F')
          DataField = 'MAILING_CODE'
          DataSource = dm.dsrMembers
          LookupTable = dm.tblMailCodes
          LookupField = 'MAIL_CODE'
          Options = [loColLines, loRowLines, loTitles]
          Color = clWhite
          DropDownCount = 12
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTextOffsetX = 1
          Frame.NonFocusTextOffsetY = 1
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 6
          AutoDropDown = True
          ShowButton = True
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcPostCodeDropDown
          OnKeyPress = dblcProposerKeyPress
        end
        object dblcParentCode: TwwDBLookupCombo
          Tag = 88
          Left = 90
          Top = 209
          Width = 79
          Height = 19
          Hint = 
            'Select another family Member - for information that is sent to t' +
            'he same household'
          HelpContext = 6
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taRightJustify
          Selected.Strings = (
            'MEMBER'#9'9'#9'MEMBER'#9'F'
            'SURNAME'#9'15'#9'SURNAME'#9'F'
            'FULL_NAME'#9'20'#9'FULL_NAME'#9'F'
            'ADDRESS'#9'20'#9'ADDRESS'#9'F'
            'FIRST_NAME'#9'12'#9'FIRST_NAME'#9'F')
          DataField = 'MAILING_LIST_CODE'
          DataSource = dm.dsrMembers
          LookupTable = dm.qryPropSec
          LookupField = 'MEMBER'
          Color = clWhite
          DropDownCount = 10
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTextOffsetX = 1
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 7
          AutoDropDown = False
          ShowButton = True
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcProposerDropDown
          OnCloseUp = dblcProposerCloseUp
          OnKeyPress = dblcProposerKeyPress
        end
        object dbeParentCodeName: TwwDBEdit
          Tag = 88
          Left = 186
          Top = 209
          Width = 134
          Height = 19
          HelpContext = 6
          TabStop = False
          BorderStyle = bsNone
          Color = clWhite
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 17
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbcbActive: TwwDBComboBox
          Tag = 88
          Left = 443
          Top = 0
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
          DataSource = dm.dsrMembers
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
          OnChange = dbcbActiveChange
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox1: TwwDBComboBox
          Tag = 88
          Left = 443
          Top = 30
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
          DataSource = dm.dsrMembers
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
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit6: TwwDBEdit
          Tag = 88
          Left = 443
          Top = 92
          Width = 103
          Height = 19
          HelpContext = 10
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'HOME_PHONE'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 12
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit7: TwwDBEdit
          Tag = 88
          Left = 443
          Top = 122
          Width = 103
          Height = 19
          HelpContext = 10
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'WORK_PHONE'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 13
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbePhoneCell: TwwDBEdit
          Tag = 88
          Left = 443
          Top = 151
          Width = 103
          Height = 19
          HelpContext = 10
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'CELL_PHONE'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 14
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeKnownAs: TwwDBEdit
          Left = 443
          Top = 210
          Width = 103
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'KNOWN_AS'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 16
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeFax: TwwDBEdit
          Tag = 88
          Left = 443
          Top = 181
          Width = 103
          Height = 19
          HelpContext = 10
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'FAX_NUMBER'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 15
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnDblClick = lblFaxClick
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox3: TwwDBComboBox
          Tag = 88
          Left = 262
          Top = 178
          Width = 59
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'RECEIVE_EMAIL'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 8
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeEmail: TwwDBEdit
          Tag = 88
          Left = 90
          Top = 148
          Width = 231
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'EMAIL'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 5
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbdtExpires: TwwDBDateTimePicker
          Tag = 88
          Left = 443
          Top = 59
          Width = 105
          Height = 19
          Hint = 'The EXPIRES date is altered by making a Payment'
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
          DataSource = dm.dsrMembers
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 11
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtDOBChange
          OnExit = dbdtExpiresExit
          OnKeyPress = dbeMemberKeyPress
        end
      end
    end
    object tsProposer: TTabSheet
      Caption = 'Proposer'
      ImageIndex = 2
      object pnlProposer: TPanel
        Left = 45
        Top = 17
        Width = 534
        Height = 227
        AutoSize = True
        BevelOuter = bvNone
        TabOrder = 0
        object Label21: TLabel
          Tag = 88
          Left = 0
          Top = 72
          Width = 54
          Height = 13
          Caption = 'Joining Fee'
        end
        object Label23: TLabel
          Tag = 88
          Left = 0
          Top = 106
          Width = 31
          Height = 13
          Caption = 'Joined'
        end
        object Label24: TLabel
          Tag = 88
          Left = 0
          Top = 140
          Width = 46
          Height = 13
          Caption = 'Approved'
        end
        object Label26: TLabel
          Tag = 88
          Left = 0
          Top = 175
          Width = 42
          Height = 13
          Caption = 'Proposer'
        end
        object Label27: TLabel
          Tag = 88
          Left = 0
          Top = 209
          Width = 46
          Height = 13
          Caption = 'Seconder'
        end
        object Label28: TLabel
          Tag = 88
          Left = 244
          Top = 141
          Width = 77
          Height = 13
          Caption = 'Transferred from'
        end
        object Label59: TLabel
          Tag = 88
          Left = 244
          Top = 106
          Width = 55
          Height = 13
          Caption = 'Occupation'
        end
        object Label60: TLabel
          Tag = 88
          Left = 244
          Top = 71
          Width = 44
          Height = 13
          Caption = 'Company'
        end
        object Label17: TLabel
          Tag = 88
          Left = 0
          Top = 37
          Width = 35
          Height = 13
          Caption = 'Gender'
        end
        object Label64: TLabel
          Tag = 88
          Left = 244
          Top = 1
          Width = 64
          Height = 13
          Caption = 'Swipe Card #'
        end
        object Label65: TLabel
          Tag = 88
          Left = 244
          Top = 36
          Width = 54
          Height = 13
          Caption = 'BarCode # '
        end
        object Label22: TLabel
          Tag = 88
          Left = 244
          Top = 176
          Width = 73
          Height = 13
          Caption = 'Proposer Name'
        end
        object Label34: TLabel
          Tag = 88
          Left = 244
          Top = 211
          Width = 77
          Height = 13
          Caption = 'Seconder Name'
        end
        object Label16: TLabel
          Tag = 88
          Left = 0
          Top = 3
          Width = 59
          Height = 13
          Caption = 'Date of Birth'
        end
        object wwDBEdit11: TwwDBEdit
          Tag = 88
          Left = 94
          Top = 69
          Width = 105
          Height = 19
          Hint = 'The Joining Fee is a default value and can be altered'
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'JOINING_FEE'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 2
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbdtJoined: TwwDBDateTimePicker
          Tag = 88
          Left = 94
          Top = 104
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
          DataSource = dm.dsrMembers
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 3
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtDOBChange
          OnExit = dbdtExpiresExit
          OnKeyPress = dbeMemberKeyPress
        end
        object dbdtApproved: TwwDBDateTimePicker
          Tag = 88
          Left = 94
          Top = 139
          Width = 105
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
          CalendarAttributes.PopupYearOptions.NumberColumns = 3
          CalendarAttributes.PopupYearOptions.ShowEditYear = True
          Color = clWhite
          DataField = 'APPROVED'
          DateFormat = dfLong
          DataSource = dm.dsrMembers
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 4
          DisplayFormat = 'dd mmm yyyy'
          OnCloseUp = dbdtApprovedExit
          OnChange = dbdtDOBChange
          OnDropDown = dbdtApprovedEnter
          OnEnter = dbdtApprovedEnter
          OnExit = dbdtApprovedExit
          OnKeyPress = dbeMemberKeyPress
        end
        object dblcProposer: TwwDBLookupCombo
          Tag = 88
          Left = 94
          Top = 173
          Width = 105
          Height = 19
          Hint = 
            'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
            ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
          AutoSize = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'9'#9'MEMBER'#9'F'
            'SURNAME'#9'15'#9'SURNAME'#9'F'
            'FULL_NAME'#9'20'#9'FULL_NAME'#9'F'
            'ADDRESS'#9'20'#9'ADDRESS'#9'F'
            'FIRST_NAME'#9'12'#9'FIRST_NAME'#9'F')
          DataField = 'PROPOSER'
          DataSource = dm.dsrMembers
          LookupTable = dm.qryPropSec
          LookupField = 'MEMBER'
          Options = [loColLines, loRowLines, loTitles]
          Color = clWhite
          DropDownCount = 10
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTextOffsetX = 1
          Frame.NonFocusTextOffsetY = 1
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 5
          AutoDropDown = False
          ShowButton = True
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcProposerDropDown
          OnCloseUp = dblcProposerCloseUp
          OnExit = dblcProposerExit
          OnKeyPress = dblcProposerKeyPress
        end
        object dblcSeconder: TwwDBLookupCombo
          Tag = 88
          Left = 94
          Top = 208
          Width = 105
          Height = 19
          Hint = 
            'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
            ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
          AutoSize = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'9'#9'MEMBER'#9'F'
            'SURNAME'#9'15'#9'SURNAME'#9'F'
            'FULL_NAME'#9'20'#9'FULL_NAME'#9'F'
            'ADDRESS'#9'20'#9'ADDRESS'#9'F'
            'FIRST_NAME'#9'12'#9'FIRST_NAME'#9'F')
          DataField = 'SECONDER'
          DataSource = dm.dsrMembers
          LookupTable = dm.qryPropSec
          LookupField = 'MEMBER'
          Options = [loColLines, loRowLines, loTitles]
          Color = clWhite
          DropDownCount = 10
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTextOffsetX = 1
          Frame.NonFocusTextOffsetY = 1
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 6
          AutoDropDown = False
          ShowButton = True
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcProposerDropDown
          OnCloseUp = dblcProposerCloseUp
          OnExit = dblcSeconderExit
          OnKeyPress = dblcProposerKeyPress
        end
        object dbeTransferred: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 139
          Width = 196
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'TRANS_FROM'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 11
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeProposerName: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 173
          Width = 196
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'PROPOSER_NAME'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 12
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeSeconderName: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 208
          Width = 196
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'SECONDER_NAME'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 13
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit9: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 104
          Width = 196
          Height = 19
          Hint = 'The Joining Fee is a default value and can be altered'
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'OCCUPATION'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 10
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit10: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 69
          Width = 196
          Height = 19
          Hint = 'The Joining Fee is a default value and can be altered'
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'COMPANY'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 9
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox2: TwwDBComboBox
          Tag = 88
          Left = 94
          Top = 35
          Width = 105
          Height = 19
          HelpContext = 11
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'MALE_FEMALE'
          DataSource = dm.dsrMembers
          DropDownCount = 3
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'Male'#9'M'
            'Female'#9'F'
            'None'#9)
          ItemIndex = 2
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit14: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 0
          Width = 196
          Height = 19
          Hint = 'Menu Mate swipe card number'
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'SWIPE_CARD'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 7
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit15: TwwDBEdit
          Tag = 88
          Left = 338
          Top = 35
          Width = 196
          Height = 19
          Hint = 
            'The Receipt cannot be altered - shows the receipt for the Joinin' +
            'g Fee'
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'BARCODE_NUMBER'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 8
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbdtDOB: TwwDBDateTimePicker
          Tag = 88
          Left = 94
          Top = 0
          Width = 103
          Height = 19
          Hint = 
            'Type in a date or '#13#10'select the down arrow to show a calender'#13#10'Pr' +
            'ess Month or Year for a quick selection '
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
          DataSource = dm.dsrMembers
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 0
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtDOBChange
          OnExit = dbdtDOBExit
          OnKeyPress = dbeMemberKeyPress
        end
      end
    end
    object tsOther: TTabSheet
      Caption = 'Other'
      ImageIndex = 4
      object pnlOther: TPanel
        Left = 65
        Top = 21
        Width = 521
        Height = 223
        AutoSize = True
        BevelOuter = bvNone
        TabOrder = 0
        object Label18: TLabel
          Tag = 88
          Left = 0
          Top = 2
          Width = 37
          Height = 13
          Caption = 'Analyse'
        end
        object Label57: TLabel
          Tag = 88
          Left = 2
          Top = 31
          Width = 53
          Height = 13
          Caption = 'Price Level'
        end
        object Label31: TLabel
          Tag = 88
          Left = 0
          Top = 89
          Width = 54
          Height = 13
          Caption = 'Earn Points'
        end
        object Label32: TLabel
          Tag = 88
          Left = 0
          Top = 118
          Width = 55
          Height = 13
          Caption = 'Allow Credit'
        end
        object Label29: TLabel
          Tag = 88
          Left = 0
          Top = 147
          Width = 51
          Height = 13
          Caption = 'Credit Limit'
        end
        object Label43: TLabel
          Tag = 88
          Left = 194
          Top = 89
          Width = 50
          Height = 13
          Caption = 'Debenture'
        end
        object Label38: TLabel
          Tag = 88
          Left = 194
          Top = 118
          Width = 42
          Height = 13
          Caption = 'Discount'
        end
        object Label35: TLabel
          Tag = 88
          Left = 0
          Top = 176
          Width = 53
          Height = 13
          Caption = 'Foundation'
        end
        object Label19: TLabel
          Tag = 88
          Left = 194
          Top = 2
          Width = 57
          Height = 13
          Caption = 'News Letter'
        end
        object Label37: TLabel
          Tag = 88
          Left = 383
          Top = 90
          Width = 49
          Height = 13
          Caption = 'Deceased'
        end
        object Label33: TLabel
          Tag = 88
          Left = 0
          Top = 60
          Width = 62
          Height = 13
          Caption = 'VIP Pay Only'
        end
        object Label40: TLabel
          Tag = 88
          Left = 194
          Top = 178
          Width = 52
          Height = 13
          Caption = 'Home Club'
        end
        object Label41: TLabel
          Tag = 88
          Left = 383
          Top = 119
          Width = 41
          Height = 13
          Caption = 'Donated'
        end
        object Label42: TLabel
          Tag = 88
          Left = 383
          Top = 148
          Width = 47
          Height = 13
          Caption = 'Refunded'
        end
        object Label30: TLabel
          Tag = 88
          Left = 194
          Top = 60
          Width = 61
          Height = 13
          Caption = 'Include EOD'
        end
        object Label53: TLabel
          Tag = 88
          Left = 276
          Top = 205
          Width = 49
          Height = 13
          Caption = 'Card Type'
        end
        object Label36: TLabel
          Tag = 88
          Left = 0
          Top = 205
          Width = 37
          Height = 13
          Caption = 'Position'
        end
        object Label39: TLabel
          Tag = 88
          Left = 194
          Top = 147
          Width = 30
          Height = 13
          Caption = 'Senior'
        end
        object Label51: TLabel
          Tag = 88
          Left = 194
          Top = 31
          Width = 38
          Height = 13
          Caption = 'Mail List'
        end
        object Label62: TLabel
          Tag = 88
          Left = 383
          Top = 31
          Width = 74
          Height = 13
          Caption = 'Payment Period'
        end
        object Label63: TLabel
          Tag = 88
          Left = 383
          Top = 3
          Width = 56
          Height = 13
          Caption = 'Direct Debit'
          WordWrap = True
        end
        object Label69: TLabel
          Tag = 88
          Left = 383
          Top = 60
          Width = 40
          Height = 13
          Caption = 'Part Pay'
        end
        object dbeAnalyse: TwwDBComboBox
          Tag = 88
          Left = 83
          Top = 0
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'ANALYSE'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 0
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object dbePriceLevel: TwwDBEdit
          Tag = 88
          Left = 83
          Top = 29
          Width = 54
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'PRICE_LEVEL'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          Picture.PictureMask = '#'
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox6: TwwDBComboBox
          Tag = 88
          Left = 83
          Top = 87
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'EARN_POINTS'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 3
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox7: TwwDBComboBox
          Tag = 88
          Left = 83
          Top = 117
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'ALLOW_CREDIT'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 4
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit18: TwwDBEdit
          Tag = 88
          Left = 83
          Top = 146
          Width = 56
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'CREDIT_LIMIT'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 5
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit23: TwwDBEdit
          Tag = 88
          Left = 275
          Top = 88
          Width = 56
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'DEBENTURE'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 11
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dbcbNewsLetter: TwwDBComboBox
          Tag = 88
          Left = 275
          Top = 0
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'NEWSLETTER'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 8
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox9: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 88
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'DECEASED'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 18
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox10: TwwDBComboBox
          Tag = 88
          Left = 83
          Top = 58
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'VIP_PAY_ONLY'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 2
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox12: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 117
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'DONATED'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 19
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox13: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 146
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'REFUNDED'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 20
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox5: TwwDBComboBox
          Tag = 88
          Left = 275
          Top = 58
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'INCL_EOD'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 10
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeCardType: TwwDBComboBox
          Tag = 88
          Left = 343
          Top = 204
          Width = 178
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'CARD_TYPE'
          DataSource = dm.dsrMembers
          DropDownCount = 4
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'Normal Member'#9'N'
            'Function Card'#9'F'
            'Expense Card'#9'E'
            'Section Card'#9'S')
          Sorted = False
          TabOrder = 22
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object dbePosition: TwwDBEdit
          Tag = 88
          Left = 83
          Top = 204
          Width = 170
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'POSITIONS'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 7
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox8: TwwDBComboBox
          Tag = 88
          Left = 275
          Top = 146
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'SENIOR'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 13
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox14: TwwDBComboBox
          Tag = 88
          Left = 275
          Top = 29
          Width = 56
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'MAILING_LIST'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox4: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 29
          Width = 56
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = False
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'PAYMENT_PERIOD'
          DataSource = dm.dsrMembers
          DropDownCount = 12
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12')
          Sorted = False
          TabOrder = 16
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox16: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 0
          Width = 56
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'AUTO_PAYMENT'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 15
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox17: TwwDBComboBox
          Tag = 88
          Left = 465
          Top = 58
          Width = 56
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = False
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'PAYMENT_DAY'
          DataSource = dm.dsrMembers
          DropDownCount = 31
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            '1'
            '2'
            '3')
          Sorted = False
          TabOrder = 17
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBEdit1: TwwDBEdit
          Tag = 88
          Left = 275
          Top = 117
          Width = 56
          Height = 19
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'DISCOUNT'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 12
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object wwDBComboBox11: TwwDBComboBox
          Tag = 88
          Left = 83
          Top = 174
          Width = 56
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'FOUNDATION'
          DataSource = dm.dsrMembers
          DropDownCount = 2
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
          TabOrder = 6
          UnboundDataType = wwDefault
          OnKeyPress = dbeMemberKeyPress
        end
        object dbeHomeClub: TwwDBEdit
          Tag = 88
          Left = 344
          Top = 175
          Width = 173
          Height = 19
          TabStop = False
          AutoFillDate = False
          BorderStyle = bsNone
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 21
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeMemberKeyPress
        end
        object dblcClubID: TwwDBLookupComboDlg
          Left = 275
          Top = 175
          Width = 56
          Height = 21
          GridOptions = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgPerfectRowFit]
          GridColor = clWhite
          GridTitleAlignment = taLeftJustify
          Caption = 'Find a Club'
          MaxWidth = 0
          MaxHeight = 280
          Selected.Strings = (
            'CLUB_ID'#9'2'#9'Club ID'#9'F'
            'DESCRIPTION'#9'30'#9'Description'#9'F')
          DataField = 'CLUBS'
          DataSource = dm.dsrMembers
          LookupTable = dm.tblClubs
          LookupField = 'CLUB_ID'
          TabOrder = 14
          AutoDropDown = False
          ShowButton = True
          AllowClearKey = False
          OnCloseUp = dblcClubIDCloseUp
          OnKeyPress = dblcProposerKeyPress
        end
      end
    end
    object tsGroupsSubs: TTabSheet
      Caption = 'Groups/Subs'
      ImageIndex = 5
      object Panel13: TPanel
        Left = 0
        Top = 247
        Width = 648
        Height = 8
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
      end
      object pnlGroup: TPanel
        Left = 0
        Top = 0
        Width = 217
        Height = 247
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 1
        object pnlGroups: TPanel
          Left = 3
          Top = 3
          Width = 211
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label54: TLabel
            Tag = 99
            Left = 11
            Top = 20
            Width = 34
            Height = 13
            Caption = 'Groups'
          end
          object dbnGroups: TwwDBNavigator
            Left = 60
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Groups
            Options = []
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object dbnGroupsInsert: TwwNavButton
              Tag = 3
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Insert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsInsertClick
              Index = 0
              Style = nbsInsert
            end
            object wwNavButton20: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Delete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object wwNavButton21: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Edit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object wwNavButton22: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Post'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object wwNavButton23: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Cancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object dbnGroupsRefresh: TwwNavButton
              Tag = 3
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Refresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsRefreshClick
              Index = 5
              Style = nbsRefresh
            end
          end
        end
        object dbgGroups: TwwDBGrid
          Tag = 3
          Left = 3
          Top = 59
          Width = 211
          Height = 185
          ControlType.Strings = (
            'DATES;CustomEdit;dbdtEventDate')
          Selected.Strings = (
            'GROUPS'#9'6'#9'Group'#9'F'
            'DESCRIPTION'#9'23'#9'Description'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Groups
          KeyOptions = [dgEnterToTab, dgAllowDelete]
          Options = [dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          PopupMenu = MenuLinks
          TabOrder = 1
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          OnDblClick = dbgSectionsDblClick
          OnExit = dbgSectionsExit
          IndicatorColor = icBlack
        end
      end
      object pnlSub: TPanel
        Left = 217
        Top = 0
        Width = 431
        Height = 247
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 2
        object pnlSubs: TPanel
          Left = 3
          Top = 3
          Width = 425
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label52: TLabel
            Tag = 99
            Left = 19
            Top = 20
            Width = 63
            Height = 13
            Caption = 'Subscriptions'
          end
          object dbnSubs: TwwDBNavigator
            Left = 227
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Subs
            Options = []
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object dbnSubsInsert: TwwNavButton
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Insert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsInsertClick
              Index = 0
              Style = nbsInsert
            end
            object wwNavButton26: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Delete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object wwNavButton27: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Edit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object wwNavButton28: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Post'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object wwNavButton29: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Cancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object dbnSubsRefresh: TwwNavButton
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Refresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsRefreshClick
              Index = 5
              Style = nbsRefresh
            end
          end
          object dbeSubsAdd: TwwDBEdit
            Tag = 99
            Left = 115
            Top = 18
            Width = 52
            Height = 19
            Hint = 'Subscriptions quick add ( Subs or Code )'
            HelpContext = 2
            AutoSize = False
            BorderStyle = bsNone
            CharCase = ecUpperCase
            Color = clWhite
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            TabOrder = 0
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeSubsAddKeyPress
          end
        end
        object dbgSubs: TwwDBGrid
          Left = 3
          Top = 59
          Width = 425
          Height = 185
          Selected.Strings = (
            'SUBSCRIPTION'#9'5'#9'Subs'#9'F'
            'CODE'#9'6'#9'Code'#9'F'
            'DESCRIPTION'#9'20'#9'Description'#9'F'
            'RECEIPT'#9'6'#9'Rcpt'#9'F'
            'AMOUNT_PAID'#9'8'#9'Amt Paid'#9'F'
            'AMOUNT'#9'7'#9'Fee'#9'F'
            'AMOUNT_DUE'#9'7'#9'Due'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Subs
          KeyOptions = [dgEnterToTab, dgAllowDelete]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          PopupMenu = MenuLinks
          TabOrder = 1
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          OnCalcCellColors = dbgSubsCalcCellColors
          OnDblClick = dbgSectionsDblClick
          OnExit = dbgSectionsExit
          IndicatorColor = icBlack
        end
      end
    end
    object tsEventsSections: TTabSheet
      Caption = 'Events/Sections'
      ImageIndex = 5
      object pnlEvent: TPanel
        Left = 0
        Top = 0
        Width = 257
        Height = 255
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 0
        object pnlEvents: TPanel
          Left = 3
          Top = 3
          Width = 251
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label10: TLabel
            Tag = 99
            Left = 27
            Top = 20
            Width = 33
            Height = 13
            Caption = 'Events'
          end
          object dbnEvents: TwwDBNavigator
            Left = 90
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Events
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object dbnEventsInsert: TwwNavButton
              Tag = 2
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Insert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsInsertClick
              Index = 0
              Style = nbsInsert
            end
            object wwNavButton8: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Delete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object wwNavButton9: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Edit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object wwNavButton10: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Post'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object wwNavButton11: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Cancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object dbnEventsRefresh: TwwNavButton
              Tag = 2
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Refresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsRefreshClick
              Index = 5
              Style = nbsRefresh
            end
          end
        end
        object dbgEvents: TwwDBGrid
          Tag = 2
          Left = 3
          Top = 59
          Width = 251
          Height = 193
          Selected.Strings = (
            'EVENT'#9'6'#9'Event'#9'F'
            'DESCRIPTION'#9'16'#9'Description'#9'F'
            'DATES'#9'12'#9'Date'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Events
          KeyOptions = [dgEnterToTab, dgAllowDelete]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          PopupMenu = MenuLinks
          TabOrder = 1
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          OnDblClick = dbgSectionsDblClick
          OnExit = dbgSectionsExit
          IndicatorColor = icBlack
        end
      end
      object pnlSection: TPanel
        Left = 257
        Top = 0
        Width = 391
        Height = 255
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 1
        object dbgSections: TwwDBGrid
          Tag = 1
          Left = 3
          Top = 59
          Width = 385
          Height = 193
          ControlType.Strings = (
            'EXPIRES;CustomEdit;dbdtSectionDate')
          Selected.Strings = (
            'SECTION'#9'7'#9'Section'#9'F'
            'DESCRIPTION'#9'17'#9'Description'#9'F'
            'EXPIRES'#9'13'#9'Expires'#9'F'
            'AMOUNT_PAID'#9'8'#9'Amt Paid'#9'F'
            'AMOUNT'#9'6'#9'Fee'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Sections
          KeyOptions = [dgEnterToTab, dgAllowDelete]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          PopupMenu = MenuLinks
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          OnCalcCellColors = dbgSectionsCalcCellColors
          OnDblClick = dbgSectionsDblClick
          OnExit = dbgSectionsExit
          IndicatorColor = icBlack
        end
        object pnlSections: TPanel
          Left = 3
          Top = 3
          Width = 385
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label9: TLabel
            Tag = 99
            Left = 51
            Top = 20
            Width = 41
            Height = 13
            Caption = 'Sections'
          end
          object dbnSections: TwwDBNavigator
            Left = 194
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Sections
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object dbnSectionsInsert: TwwNavButton
              Tag = 1
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Insert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsInsertClick
              Index = 0
              Style = nbsInsert
            end
            object wwNavButton14: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Delete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object wwNavButton15: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Edit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object wwNavButton16: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Post'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object wwNavButton17: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Cancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object dbnSectionsRefresh: TwwNavButton
              Tag = 1
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Refresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnSubsRefreshClick
              Index = 5
              Style = nbsRefresh
            end
          end
        end
        object dbdtSectionDate: TwwDBDateTimePicker
          Left = 90
          Top = 111
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
          CalendarAttributes.Colors.TrailingTextColor = clSilver
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
          DataField = 'EXPIRES'
          DateFormat = dfLong
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 2
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtDOBChange
          OnExit = dbdtExpiresExit
          OnKeyPress = dbeMemberKeyPress
        end
      end
    end
    object tsDiary: TTabSheet
      Caption = 'Diary'
      ImageIndex = 5
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 648
        Height = 255
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 8
        Caption = 'Panel1'
        TabOrder = 0
        object DBRichEdit: TwwDBRichEdit
          Left = 8
          Top = 8
          Width = 632
          Height = 239
          Align = alClient
          AutoURLDetect = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'DAIRY_NOTES'
          DataSource = dm.dsrMembers
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          PrintJobName = 'Delphi 5'
          TabOrder = 0
          EditorCaption = 'Edit Rich Text'
          EditorPosition.Left = 0
          EditorPosition.Top = 0
          EditorPosition.Width = 0
          EditorPosition.Height = 0
          MeasurementUnits = muInches
          PrintMargins.Top = 1
          PrintMargins.Bottom = 1
          PrintMargins.Left = 1
          PrintMargins.Right = 1
          RichEditVersion = 2
          Data = {
            8A0000007B5C727466315C616E73695C64656666307B5C666F6E7474626C7B5C
            66305C666E696C204D532053616E732053657269663B7D7D0D0A7B5C636F6C6F
            7274626C203B5C726564305C677265656E305C626C7565303B7D0D0A5C766965
            776B696E64345C7563315C706172645C6366315C6C616E67313033335C66305C
            667331365C7061720D0A7D0D0A00}
        end
      end
    end
    object tsInterests: TTabSheet
      Caption = 'Interests'
      ImageIndex = 6
      object pnlInterest: TPanel
        Left = 328
        Top = 0
        Width = 320
        Height = 255
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 0
        object dbgInterests: TwwDBGrid
          Left = 3
          Top = 59
          Width = 314
          Height = 193
          ControlType.Strings = (
            'SECTION;CustomEdit;dblcSections')
          Selected.Strings = (
            'SECTION'#9'18'#9'Section'#9'F'
            'DESCRIPTION'#9'26'#9'Description'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Interests
          KeyOptions = [dgEnterToTab, dgAllowDelete, dgAllowInsert]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          OnDblClick = dbgInterestsDblClick
          IndicatorColor = icBlack
        end
        object pnlInterests: TPanel
          Left = 3
          Top = 3
          Width = 314
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label67: TLabel
            Tag = 99
            Left = 27
            Top = 20
            Width = 79
            Height = 13
            Caption = 'Section Interests'
          end
          object dbnInterests: TwwDBNavigator
            Left = 144
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Interests
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object dbnInterestsInsert: TwwNavButton
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsInsert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              OnClick = dbnInterestsInsertClick
              Index = 0
              Style = nbsInsert
            end
            object dbnInterestsDelete: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsDelete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object dbnInterestsEdit: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsEdit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object dbnInterestsPost: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsPost'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object dbnInterestsCancel: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsCancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object dbnInterestsRefresh: TwwNavButton
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'dbnInterestsRefresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 5
              Style = nbsRefresh
            end
          end
        end
      end
      object pnlLot: TPanel
        Left = 0
        Top = 0
        Width = 328
        Height = 255
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 1
        object dbgLots: TwwDBGrid
          Left = 3
          Top = 59
          Width = 322
          Height = 193
          ControlType.Strings = (
            'LOT_TYPE;CustomEdit;dbcbLots')
          Selected.Strings = (
            'LOT_TYPE'#9'9'#9'Type'#9'F'
            'DESCRIPTION'#9'37'#9'Description'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrM_Lots
          KeyOptions = [dgEnterToTab, dgAllowDelete, dgAllowInsert]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          UseTFields = False
          IndicatorColor = icBlack
        end
        object pnlLots: TPanel
          Left = 3
          Top = 3
          Width = 322
          Height = 56
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label68: TLabel
            Tag = 99
            Left = 12
            Top = 20
            Width = 111
            Height = 13
            Caption = 'Lots or Street Numbers '
          end
          object wwDBNavigator2: TwwDBNavigator
            Left = 137
            Top = 16
            Width = 150
            Height = 25
            AutosizeStyle = asSizeNavButtons
            DataSource = dm.dsrM_Lots
            ShowHint = True
            RepeatInterval.InitialDelay = 500
            RepeatInterval.Interval = 100
            object wwNavButton1: TwwNavButton
              Left = 0
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Insert new record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Insert'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 0
              Style = nbsInsert
            end
            object wwNavButton2: TwwNavButton
              Left = 25
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Delete current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Delete'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 1
              Style = nbsDelete
            end
            object wwNavButton3: TwwNavButton
              Left = 50
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Edit current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Edit'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 2
              Style = nbsEdit
            end
            object wwNavButton4: TwwNavButton
              Left = 75
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Post changes of current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Post'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 3
              Style = nbsPost
            end
            object wwNavButton5: TwwNavButton
              Left = 100
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Cancel changes made to current record'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Cancel'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 4
              Style = nbsCancel
            end
            object wwNavButton6: TwwNavButton
              Left = 125
              Top = 0
              Width = 25
              Height = 25
              Hint = 'Refresh the contents of the dataset'
              ImageIndex = -1
              NumGlyphs = 2
              Spacing = 4
              Transparent = False
              Caption = 'wwDBNavigator1Refresh'
              Enabled = False
              DisabledTextColors.ShadeColor = clGray
              DisabledTextColors.HighlightColor = clBtnHighlight
              Index = 5
              Style = nbsRefresh
            end
          end
        end
        object dbcbLots: TwwDBComboBox
          Left = 128
          Top = 160
          Width = 121
          Height = 21
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          DataField = 'LOT_TYPE'
          DropDownCount = 8
          ItemHeight = 0
          Items.Strings = (
            'Lot #'#9'0'
            'Block #'#9'1'
            'DPS #'#9'2')
          Sorted = False
          TabOrder = 2
          UnboundDataType = wwDefault
        end
      end
    end
    object tsCreditPoints: TTabSheet
      Caption = 'Credit/Points'
      ImageIndex = 7
      object pnlDonation: TPanel
        Left = 0
        Top = 0
        Width = 648
        Height = 57
        Align = alTop
        BevelOuter = bvNone
        BorderWidth = 3
        Caption = 'pnlDonation'
        TabOrder = 0
        object gbxDonation: TGroupBox
          Left = 3
          Top = 3
          Width = 642
          Height = 51
          Align = alClient
          Caption = 'Donation of Points '
          TabOrder = 0
          object Label61: TLabel
            Tag = 88
            Left = 35
            Top = 23
            Width = 13
            Height = 13
            Hint = 'Who gets the Points if Donation Points % is set above zero'
            Caption = 'To'
          end
          object Label55: TLabel
            Left = 477
            Top = 23
            Width = 40
            Height = 13
            Caption = 'Points %'
          end
          object Label56: TLabel
            Tag = 88
            Left = 188
            Top = 22
            Width = 28
            Height = 13
            Hint = 'Who gets the Points if Donation Points % is set above zero'
            Caption = 'Name'
          end
          object dblcDonation: TwwDBLookupCombo
            Tag = 88
            Left = 59
            Top = 20
            Width = 76
            Height = 19
            Hint = 
              'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
              ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
            AutoSize = False
            CharCase = ecUpperCase
            BorderStyle = bsNone
            DropDownAlignment = taLeftJustify
            Selected.Strings = (
              'MEMBER'#9'9'#9'MEMBER'#9'F'
              'SURNAME'#9'15'#9'SURNAME'#9'F'
              'FULL_NAME'#9'20'#9'FULL_NAME'#9'F'
              'ADDRESS'#9'20'#9'ADDRESS'#9'F'
              'FIRST_NAME'#9'12'#9'FIRST_NAME'#9'F')
            DataField = 'DONATION_MEMBER'
            DataSource = dm.dsrMembers
            LookupTable = dm.qryPropSec
            LookupField = 'MEMBER'
            Options = [loColLines, loRowLines, loTitles]
            Color = clWhite
            DropDownCount = 10
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTextOffsetX = 1
            Frame.NonFocusTextOffsetY = 1
            Frame.NonFocusTransparentFontColor = clBlack
            TabOrder = 0
            AutoDropDown = False
            ShowButton = True
            AllowClearKey = False
            ShowMatchText = True
            OnDropDown = dblcProposerDropDown
            OnCloseUp = dblcProposerCloseUp
            OnExit = dblcProposerExit
            OnKeyPress = dblcProposerKeyPress
          end
          object wwDBEdit3: TwwDBEdit
            Left = 531
            Top = 20
            Width = 76
            Height = 19
            Hint = 'What percentage the Member donates to "Donation to" member'
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'DONATION'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            TabOrder = 1
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object dbeDonationName: TwwDBEdit
            Tag = 88
            Left = 236
            Top = 20
            Width = 204
            Height = 19
            TabStop = False
            AutoSize = False
            BorderStyle = bsNone
            Color = clWhite
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
        end
      end
      object Panel6: TPanel
        Left = 0
        Top = 57
        Width = 648
        Height = 198
        Align = alClient
        BevelOuter = bvNone
        Caption = 'Panel6'
        TabOrder = 1
        object pnlPoints: TPanel
          Left = 107
          Top = 3
          Width = 473
          Height = 185
          BevelOuter = bvNone
          BorderWidth = 3
          TabOrder = 0
          object Label47: TLabel
            Left = 16
            Top = 9
            Width = 70
            Height = 13
            Caption = 'Pre Paid Credit'
          end
          object Label48: TLabel
            Left = 16
            Top = 47
            Width = 60
            Height = 13
            Caption = 'Bonus Credit'
          end
          object Label49: TLabel
            Left = 16
            Top = 84
            Width = 55
            Height = 13
            Caption = 'Earnt Credit'
          end
          object Label50: TLabel
            Left = 16
            Top = 122
            Width = 69
            Height = 13
            Caption = 'Value of Credit'
          end
          object Label7: TLabel
            Left = 259
            Top = 48
            Width = 57
            Height = 13
            Caption = 'Points Earnt'
          end
          object Label45: TLabel
            Left = 259
            Top = 82
            Width = 52
            Height = 26
            Caption = 'Points Redeemed'
            WordWrap = True
          end
          object Label46: TLabel
            Left = 259
            Top = 123
            Width = 75
            Height = 13
            Caption = 'Points Available'
          end
          object Label58: TLabel
            Left = 259
            Top = 11
            Width = 73
            Height = 13
            Caption = 'Card Last Used'
          end
          object Label71: TLabel
            Left = 17
            Top = 161
            Width = 37
            Height = 13
            Caption = 'Reason'
          end
          object dbePrepaidCredit: TwwDBEdit
            Left = 101
            Top = 8
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'PRE_PAID_CREDIT'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ParentShowHint = False
            ReadOnly = True
            ShowHint = True
            TabOrder = 0
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object dbeBonusCredit: TwwDBEdit
            Left = 101
            Top = 45
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'BONUS_CREDIT'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ReadOnly = True
            TabOrder = 1
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object dbeEarntCredit: TwwDBEdit
            Left = 101
            Top = 82
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'EARNT_CREDIT'
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
            OnKeyPress = dbeMemberKeyPress
          end
          object wwDBEdit28: TwwDBEdit
            Left = 101
            Top = 119
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'VALUE_OF_CREDIT'
            DataSource = dm.dsrMembers
            Enabled = False
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clGray
            ReadOnly = True
            TabOrder = 3
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object dbePointsEarnt: TwwDBEdit
            Left = 382
            Top = 45
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'TOTAL_EARNT'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ReadOnly = True
            TabOrder = 5
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object dbePointsRedeemed: TwwDBEdit
            Left = 382
            Top = 82
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'TOTAL_REDEEMED'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ReadOnly = True
            TabOrder = 6
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object wwDBDateTimePicker1: TwwDBDateTimePicker
            Tag = 88
            Left = 359
            Top = 8
            Width = 103
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
            CalendarAttributes.PopupYearOptions.YearsPerColumn = 25
            CalendarAttributes.PopupYearOptions.NumberColumns = 4
            CalendarAttributes.PopupYearOptions.StartYear = 1920
            Color = clWhite
            DataField = 'CARD_LAST_USED'
            DateFormat = dfLong
            DataSource = dm.dsrMembers
            Epoch = 1950
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ParentShowHint = False
            ShowHint = True
            ShowButton = True
            TabOrder = 4
            DisplayFormat = 'dd mmm yyyy'
            OnChange = dbdtDOBChange
            OnExit = dbdtExpiresExit
            OnKeyPress = dbeMemberKeyPress
          end
          object dbePAvailable: TwwDBEdit
            Left = 382
            Top = 119
            Width = 76
            Height = 19
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'POINTS_AVAILABLE'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ReadOnly = True
            TabOrder = 7
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeMemberKeyPress
          end
          object wwDBEdit2: TwwDBEdit
            Tag = 88
            Left = 100
            Top = 158
            Width = 358
            Height = 19
            TabStop = False
            AutoSize = False
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'REASON'
            DataSource = dm.dsrMembers
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            TabOrder = 8
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
          end
        end
      end
    end
    object tsPhotograph: TTabSheet
      Caption = 'Photograph'
      ImageIndex = 8
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 334
        Height = 255
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 12
        TabOrder = 0
        object Panel4: TPanel
          Left = 12
          Top = 12
          Width = 310
          Height = 231
          Align = alClient
          TabOrder = 0
          object Image: TImage
            Left = 1
            Top = 1
            Width = 308
            Height = 229
            Hint = 'Double Click to load a Photo'
            Align = alClient
            Center = True
            OnDblClick = ImageDblClick
          end
        end
      end
      object Panel3: TPanel
        Left = 334
        Top = 0
        Width = 314
        Height = 255
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 1
        object GroupBox1: TGroupBox
          Left = 0
          Top = 7
          Width = 305
          Height = 242
          Caption = 'Photograph Options '
          TabOrder = 0
          object btnLoadPhoto: TButton
            Left = 108
            Top = 134
            Width = 87
            Height = 25
            Caption = 'Load from File'
            TabOrder = 0
            OnClick = btnLoadPhotoClick
          end
          object btnDeletePhoto: TButton
            Left = 108
            Top = 183
            Width = 87
            Height = 25
            Caption = 'Delete'
            TabOrder = 1
            OnClick = btnDeletePhotoClick
          end
          object rgpPasteOptions: TRadioGroup
            Left = 19
            Top = 25
            Width = 273
            Height = 47
            Caption = 'Paste Options'
            Columns = 2
            ItemIndex = 1
            Items.Strings = (
              'Bitmap'
              'Jpeg')
            TabOrder = 2
          end
          object btnPastePhoto: TButton
            Left = 110
            Top = 86
            Width = 87
            Height = 25
            Caption = 'Paste Photo'
            TabOrder = 3
            OnClick = btnPastePhotoClick
          end
        end
      end
    end
    object tsPreference: TTabSheet
      Caption = 'Preferences'
      ImageIndex = 9
      TabVisible = False
      object Bevel1: TBevel
        Left = 0
        Top = 46
        Width = 648
        Height = 6
        Align = alTop
        Shape = bsTopLine
      end
      object ScrollBox: TScrollBox
        Left = 0
        Top = 52
        Width = 648
        Height = 203
        HorzScrollBar.Visible = False
        VertScrollBar.Size = 15
        Align = alClient
        BorderStyle = bsNone
        TabOrder = 0
      end
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 648
        Height = 46
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object Label72: TLabel
          Tag = 99
          Left = 155
          Top = 15
          Width = 104
          Height = 13
          Caption = 'Customer Preferences'
        end
        object wwDBNavigator1: TwwDBNavigator
          Left = 272
          Top = 11
          Width = 150
          Height = 25
          AutosizeStyle = asSizeNavButtons
          DataSource = dm.dsrPrefMembers
          ShowHint = True
          RepeatInterval.InitialDelay = 500
          RepeatInterval.Interval = 100
          object wwNavButton7: TwwNavButton
            Left = 0
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Insert new record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsInsert'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 0
            Style = nbsInsert
          end
          object wwNavButton12: TwwNavButton
            Left = 25
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Delete current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsDelete'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 1
            Style = nbsDelete
          end
          object wwNavButton13: TwwNavButton
            Left = 50
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Edit current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsEdit'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 2
            Style = nbsEdit
          end
          object wwNavButton18: TwwNavButton
            Left = 75
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Post changes of current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsPost'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 3
            Style = nbsPost
          end
          object wwNavButton19: TwwNavButton
            Left = 100
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Cancel changes made to current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsCancel'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 4
            Style = nbsCancel
          end
          object wwNavButton24: TwwNavButton
            Left = 125
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Refresh the contents of the dataset'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'dbnInterestsRefresh'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 5
            Style = nbsRefresh
          end
        end
      end
    end
    object tsDiscounts: TTabSheet
      Caption = 'Discounts'
      ImageIndex = 10
      object pnlDiscounts: TPanel
        Left = 0
        Top = 0
        Width = 648
        Height = 56
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Label73: TLabel
          Tag = 99
          Left = 27
          Top = 20
          Width = 129
          Height = 13
          Caption = 'MenuMate POS Discounts '
        end
        object Label74: TLabel
          Tag = 99
          Left = 423
          Top = 20
          Width = 74
          Height = 13
          Caption = 'Discount Order '
        end
        object dbnDiscounts: TwwDBNavigator
          Tag = 4
          Left = 179
          Top = 16
          Width = 150
          Height = 25
          AutosizeStyle = asSizeNavButtons
          DataSource = dm.dsrM_Discounts
          ShowHint = True
          RepeatInterval.InitialDelay = 500
          RepeatInterval.Interval = 100
          object wwNavButton25: TwwNavButton
            Tag = 4
            Left = 0
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Insert new record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Insert'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            OnClick = dbnSubsInsertClick
            Index = 0
            Style = nbsInsert
          end
          object wwNavButton30: TwwNavButton
            Left = 25
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Delete current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Delete'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 1
            Style = nbsDelete
          end
          object wwNavButton31: TwwNavButton
            Left = 50
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Edit current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Edit'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 2
            Style = nbsEdit
          end
          object wwNavButton32: TwwNavButton
            Left = 75
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Post changes of current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Post'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 3
            Style = nbsPost
          end
          object wwNavButton33: TwwNavButton
            Left = 100
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Cancel changes made to current record'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Cancel'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            Index = 4
            Style = nbsCancel
          end
          object wwNavButton34: TwwNavButton
            Tag = 4
            Left = 125
            Top = 0
            Width = 25
            Height = 25
            Hint = 'Refresh the contents of the dataset'
            ImageIndex = -1
            NumGlyphs = 2
            Spacing = 4
            Transparent = False
            Caption = 'wwDBNavigator1Refresh'
            Enabled = False
            DisabledTextColors.ShadeColor = clGray
            DisabledTextColors.HighlightColor = clBtnHighlight
            OnClick = dbnSubsRefreshClick
            Index = 5
            Style = nbsRefresh
          end
        end
        object btnUp: TButton
          Left = 504
          Top = 15
          Width = 43
          Height = 25
          Caption = 'Up'
          TabOrder = 1
          OnClick = btnUpClick
        end
        object btnDown: TButton
          Left = 558
          Top = 15
          Width = 43
          Height = 25
          Caption = 'Down'
          TabOrder = 2
          OnClick = btnDownClick
        end
      end
      object dbgDiscounts: TwwDBGrid
        Tag = 4
        Left = 0
        Top = 56
        Width = 648
        Height = 199
        Selected.Strings = (
          'MEMBER_DISCOUNTS_KEY'#9'7'#9'Order'#9'F'
          'NAME'#9'18'#9'Name'#9'F'
          'DESCRIPTION'#9'37'#9'Description'#9'F'
          'MENU'#9'15'#9'Menu'#9'F'
          'AMOUNT'#9'10'#9'Amount'#9'F'
          'PERCENT'#9'10'#9'Percent'#9'F')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrM_Discounts
        KeyOptions = [dgEnterToTab, dgAllowDelete]
        Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
        PopupMenu = MenuLinks
        TabOrder = 1
        TitleAlignment = taLeftJustify
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        UseTFields = False
        OnDblClick = dbgSectionsDblClick
        OnExit = dbgSectionsExit
        IndicatorColor = icBlack
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 615
    Top = 146
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object Options1: TMenuItem [1]
      Caption = '&Options'
      object menuAddMember: TMenuItem
        Caption = 'Add a new Member               F3'
        HelpContext = 2001
        OnClick = dbnQryMembersInsertClick
      end
      object menuCopyMember: TMenuItem
        Caption = '&Copy from a Member'
        OnClick = menuCopyMemberClick
      end
      object menuDeleteMember: TMenuItem
        Caption = 'Delete a Member                   F4'
        OnClick = menuDeleteMemberClick
      end
      object menuPrintMemberDetails: TMenuItem
        Caption = 'Print Member Details'
        ShortCut = 113
        OnClick = menuPrintMemberDetailsClick
      end
      object menuClosePreferences: TMenuItem
        Caption = 'Close Preferences'
        OnClick = menuClosePreferencesClick
      end
      object menubb: TMenuItem
        Caption = '-'
      end
      object menuDefaultMember: TMenuItem
        Caption = 'Def&ault Member'
        OnClick = menuDefaultMemberClick
      end
      object menuMemberNumber: TMenuItem
        Caption = 'Set Member N&umber '
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object menuSetupMemberNumbers: TMenuItem
        Caption = 'Setup Member Number Defaults'
        OnClick = menuSetupMemberNumbersClick
      end
      object menuNextNumber: TMenuItem
        Caption = 'Next Number'
        ShortCut = 116
        OnClick = menuNextUnUsedNumberClick
      end
      object menuReuseNonActiveMembers: TMenuItem
        Tag = 2
        Caption = 'Use Non Active Members'
        ShortCut = 117
        OnClick = menuNextUnUsedNumberClick
      end
      object menuNextUnUsedNumber: TMenuItem
        Tag = 1
        Caption = 'Find a Blank Number'
        ShortCut = 118
        OnClick = menuNextUnUsedNumberClick
      end
      object menuUseNonFinancialMember: TMenuItem
        Tag = 3
        Caption = 'Use Non Financial Member'
        ShortCut = 119
        OnClick = menuNextUnUsedNumberClick
      end
      object UsenonFinanc1: TMenuItem
        Tag = 4
        Caption = 'Use Non Financial Member No Points'
        ShortCut = 8311
        OnClick = menuNextUnUsedNumberClick
      end
      object N1: TMenuItem
        Caption = '-'
        OnClick = menuNextUnUsedNumberClick
      end
      object menuAttachedMembers: TMenuItem
        Caption = 'Lookup Attached Members'
        OnClick = menuAttachedMembersClick
      end
      object menuAddMemberCard: TMenuItem
        Caption = 'Add Member for a Card '
        ShortCut = 122
        OnClick = menuAddMemberCardClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object menuAdjustPointsCredit: TMenuItem
        Caption = 'Logon Setup Credit/Points'
        OnClick = menuAdjustPointsCreditClick
      end
      object menuResetAdjustPointsCredit: TMenuItem
        Caption = 'Logoff Setup Credit/Points '
        OnClick = menuResetAdjustPointsCreditClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object menuExportMember: TMenuItem
        Caption = 'Export Member to Stockmaster'
        ShortCut = 8310
        OnClick = menuExportMemberClick
      end
      object menuMemberVisits: TMenuItem
        Caption = 'Member Visits'
        OnClick = menuMemberVisitsClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object menuPHMTransfer: TMenuItem
        Caption = 'Import and Export to PHM'
        ShortCut = 8313
        OnClick = menuPHMTransferClick
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object ApplyDiscount1: TMenuItem
        Caption = 'Apply Discount'
        OnClick = ApplyDiscount1Click
      end
      object menuDeleteDiscount: TMenuItem
        Caption = 'Delete Discount'
        OnClick = menuDeleteDiscountClick
      end
    end
  end
  object PopupMenu: TPopupMenu
    Left = 560
    Top = 146
    object subMenuEvent: TMenuItem
      Caption = 'Event'
      OnClick = subMenuEventClick
    end
    object submenuGroups: TMenuItem
      Tag = 1
      Caption = 'Groups'
      OnClick = subMenuEventClick
    end
    object submenuSection: TMenuItem
      Tag = 2
      Caption = 'Section'
      OnClick = subMenuEventClick
    end
    object submenuSubscription: TMenuItem
      Tag = 3
      Caption = 'Subscription'
      OnClick = subMenuEventClick
    end
  end
  object PopupMenuNumbers: TPopupMenu
    Left = 608
    Top = 130
  end
  object MenuLinks: TPopupMenu
    OnPopup = MenuLinksPopup
    Left = 584
    Top = 138
  end
  object OpenPictureDialog: TOpenPictureDialog
    FilterIndex = 0
    InitialDir = 'c:\'
    Title = 'Load a Members Picture'
    Left = 604
    Top = 193
  end
  object ldlgSections: TwwLookupDialog
    Selected.Strings = (
      'SECTION'#9'7'#9'Section'#9'F'
      'DESCRIPTION'#9'30'#9'Description'#9'F')
    GridTitleAlignment = taLeftJustify
    GridColor = clWhite
    GridOptions = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgPerfectRowFit]
    LookupTable = dm.tblSections
    Caption = 'Lookup'
    MaxWidth = 0
    MaxHeight = 209
    CharCase = ecNormal
    Left = 623
    Top = 95
  end
end
