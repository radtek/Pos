inherited frmMembersREPORT: TfrmMembersREPORT
  Tag = 2
  Left = 211
  Top = 80
  Caption = 'frmMembersREPORT'
  ClientHeight = 478
  ClientWidth = 668
  Font.Color = clBlack
  OldCreateOrder = True
  PopupMenu = PopupMenu
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 457
    Width = 668
    Height = 21
  end
  object Panel1: TPanel [1]
    Left = 0
    Top = 60
    Width = 668
    Height = 397
    Align = alClient
    BevelInner = bvLowered
    BevelOuter = bvNone
    BevelWidth = 2
    TabOrder = 2
    object Splitter1: TSplitter
      Left = 227
      Top = 2
      Width = 4
      Height = 393
      Cursor = crHSplit
      Align = alRight
      Beveled = True
    end
    object TreeView: TTreeView
      Left = 8
      Top = 2
      Width = 219
      Height = 393
      Hint = 
        'Choose a report, click on + to see more reports, right click to ' +
        'hold a report '
      Align = alClient
      BiDiMode = bdLeftToRight
      BorderStyle = bsNone
      Color = clSilver
      HideSelection = False
      Indent = 19
      ParentBiDiMode = False
      ReadOnly = True
      StateImages = dm.ImageList1
      TabOrder = 0
      OnChange = TreeViewChange
      OnDblClick = TreeViewDblClick
      Items.Data = {
        1700000025000000FFFFFFFF00000000FFFFFFFFFFFFFFFF0000000000000000
        0C41676520416E616C797369731E000000FFFFFFFF02000000FFFFFFFFFFFFFF
        FF0000000003000000054361726473270000000000000030000000FFFFFFFFFF
        FFFFFF00000000000000000E43617264204C61737420557365642A000000FFFF
        FFFF26000000FFFFFFFFFFFFFFFF000000000000000011437265617465204361
        72642046696C6520240000000000000027000000FFFFFFFFFFFFFFFF00000000
        000000000B5072696E742043617264731E000000FFFFFFFF01000000FFFFFFFF
        FFFFFFFF000000000000000005436C75627326000000000000003F000000FFFF
        FFFFFFFFFFFF00000000000000000D456D61696C204D656D626572731F000000
        FFFFFFFF03000000FFFFFFFFFFFFFFFF0000000002000000064576656E747326
        0000000500000004000000FFFFFFFFFFFFFFFF00000000000000000D4576656E
        7473205265706F7274290000000000000005000000FFFFFFFFFFFFFFFF000000
        0000000000104D6F6E74686C7920416E616C797369731F000000000000000600
        0000FFFFFFFFFFFFFFFF0000000005000000064578706F727429000000010000
        0007000000FFFFFFFFFFFFFFFF0000000000000000104578706F727420436172
        642046696C65370000000000000025000000FFFFFFFFFFFFFFFF000000000000
        00001E4578706F727420436F64652C4B6E6F776E2041732C20506F736974696F
        6E260000000000000008000000FFFFFFFFFFFFFFFF00000000000000000D4578
        706F7274204576656E7473280000000000000009000000FFFFFFFFFFFFFFFF00
        000000000000000F4578706F727420537562732044756529000000000000000A
        000000FFFFFFFFFFFFFFFF0000000000000000104578706F72742053656C6563
        7469766527000000000000000C000000FFFFFFFFFFFFFFFF0000000000000000
        0E47726F757020416E616C797369732D0000000000000044000000FFFFFFFFFF
        FFFFFF0000000000000000144B696F736B20486973746F7279205265706F7274
        25000000000000003B000000FFFFFFFFFFFFFFFF00000000000000000C4B696F
        736B205265706F727420000000000000000D000000FFFFFFFFFFFFFFFF000000
        0007000000074C657474657273250000000000000028000000FFFFFFFFFFFFFF
        FF00000000000000000C4461746573204C657474657226000000000000000E00
        0000FFFFFFFFFFFFFFFF00000000000000000D4576656E7473204C6574746572
        2C000000000000000F000000FFFFFFFFFFFFFFFF000000000000000013536563
        74696F6E7320447565204C6574746572290000000000000010000000FFFFFFFF
        FFFFFFFF00000000000000001053656C656374697665204C6574746572280000
        000000000011000000FFFFFFFFFFFFFFFF00000000000000000F537562732044
        7565204C657474657231000000000000002B000000FFFFFFFFFFFFFFFF000000
        00000000001853756273204475652052656D696E646572204C65747465722E00
        0000000000002C000000FFFFFFFFFFFFFFFF0000000000000000155375627320
        4475652046696E616C204C6574746572260000000000000012000000FFFFFFFF
        FFFFFFFF00000000000000000D4C75636B7920486973746F7279240000000000
        000040000000FFFFFFFFFFFFFFFF00000000040000000B4D61696E74656E616E
        63652F0000000000000041000000FFFFFFFFFFFFFFFF0000000000000000164D
        656D626572732077697468204E6F2047726F7570732D00000000000000420000
        00FFFFFFFFFFFFFFFF0000000000000000144D656D626572732077697468204E
        6F2053756273310000000000000046000000FFFFFFFFFFFFFFFF000000000000
        0000184D656D626572732077697468204D616E792047726F7570732F00000000
        00000047000000FFFFFFFFFFFFFFFF0000000000000000164D656D6265727320
        77697468204D616E792053756273270000000000000013000000FFFFFFFFFFFF
        FFFF00000000000000000E4D61696C696E67204C6162656C7320000000000000
        0014000000FFFFFFFFFFFFFFFF000000000D000000074D656D62657273240000
        000000000035000000FFFFFFFFFFFFFFFF00000000000000000B4D656D626572
        2041676573240000000000000039000000FFFFFFFFFFFFFFFF00000000000000
        000B4D656D62657220447261772B0000000000000015000000FFFFFFFFFFFFFF
        FF0000000000000000124D656D62657220496E666F726D6174696F6E24000000
        0000000037000000FFFFFFFFFFFFFFFF00000000000000000B4D656D62657220
        4C697374260000000000000016000000FFFFFFFFFFFFFFFF0000000000000000
        0D4D656D62657220506F696E74732C0000000000000045000000FFFFFFFFFFFF
        FFFF0000000000000000134D656D62657220506F696E74732041756469743200
        0000000000003A000000FFFFFFFFFFFFFFFF0000000000000000194D656D6265
        7220506F696E74732053746F636B6D617374657231000000000000003C000000
        FFFFFFFFFFFFFFFF0000000000000000184D656D6265722050726F706F736572
        205365636F6E646572240000000000000018000000FFFFFFFFFFFFFFFF000000
        00000000000B4E6577204D656D62657273300000000000000036000000FFFFFF
        FFFFFFFFFF0000000000000000174E6577204D656D62657273204A6F696E6564
        20446174652B0000000000000038000000FFFFFFFFFFFFFFFF00000000000000
        0012506172656E7420436F6465204D656D6265722E0000000000000019000000
        FFFFFFFFFFFFFFFF000000000000000015546F70203530204D656D6265727320
        506F696E74732D0000000000000048000000FFFFFFFFFFFFFFFF000000000000
        000014546F70203530204D656D626572732053616C65732B0000000000000049
        000000FFFFFFFFFFFFFFFF0000000000000000124D656E754D61746520446973
        636F756E747335000000000000003D000000FFFFFFFFFFFFFFFF000000000000
        00001C4D656E754D61746520506F696E74732053706C6974205265706F72742E
        000000000000003E000000FFFFFFFFFFFFFFFF00000000000000001550484D20
        506F696E7473204C6F67205265706F7274230000000000000034000000FFFFFF
        FFFFFFFFFF00000000000000000A506F696E7473204C6F671E00000000000000
        2D000000FFFFFFFFFFFFFFFF0000000002000000055175657279250000000000
        000017000000FFFFFFFFFFFFFFFF00000000000000000C437265617465205175
        65727922000000000000002E000000FFFFFFFFFFFFFFFF000000000000000009
        52756E205175657279270000000000000029000000FFFFFFFFFFFFFFFF000000
        00000000000E52656365697074205265706F727421000000000000001B000000
        FFFFFFFFFFFFFFFF00000000060000000853656374696F6E7329000000000000
        001C000000FFFFFFFFFFFFFFFF00000000000000001053656374696F6E20416E
        616C797369732C000000000000001D000000FFFFFFFFFFFFFFFF000000000000
        00001353656374696F6E20496E666F726D6174696F6E31000000000000003300
        0000FFFFFFFFFFFFFFFF00000000000000001853656374696F6E20496E746572
        65737473205265706F727427000000000000001E000000FFFFFFFFFFFFFFFF00
        000000000000000E53656374696F6E20506F696E74732E000000000000004A00
        000000000000FFFFFFFF00000000000000001553706F6E736F72736869702062
        79204D656D6265722F000000000000001F000000FFFFFFFFFFFFFFFF00000000
        000000001653706F6E736F72736869702062792053656374696F6E2600000000
        00000020000000FFFFFFFFFFFFFFFF00000000060000000D5375627363726970
        74696F6E7321000000000000002F000000FFFFFFFFFFFFFFFF00000000000000
        00085375627320416765260000000000000021000000FFFFFFFFFFFFFFFF0000
        0000000000000D5375627320416E616C79736973260000000000000032000000
        FFFFFFFFFFFFFFFF00000000000000000D537562732042792047726F75702500
        00000000000022000000FFFFFFFFFFFFFFFF00000000000000000C5375627320
        496E766F69636529000000000000002A000000FFFFFFFFFFFFFFFF0000000000
        000000105375627320496E666F726D6174696F6E250000000000000023000000
        FFFFFFFFFFFFFFFF00000000000000000C53756273204D6F6E74686C79240000
        000000000024000000FFFFFFFFFFFFFFFF00000000000000000B566F74696E67
        204C697374}
    end
    object scbOptions: TScrollBox
      Left = 231
      Top = 2
      Width = 423
      Height = 393
      Hint = 'Choose from these criteria for the report'
      HorzScrollBar.Color = clHighlight
      HorzScrollBar.ParentColor = False
      VertScrollBar.Position = 1586
      Align = alRight
      BorderStyle = bsNone
      Color = clSilver
      ParentColor = False
      TabOrder = 1
      object gbxDates: TGroupBox
        Left = 0
        Top = -1243
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 0
        object lblDateFrom: TLabel
          Left = 14
          Top = 21
          Width = 80
          Height = 13
          Alignment = taRightJustify
          Caption = 'Expiry Date From'
        end
        object lblDateTo: TLabel
          Left = 206
          Top = 22
          Width = 70
          Height = 13
          Alignment = taRightJustify
          Caption = 'Expiry Date To'
        end
        object dbdtFrom: TwwDBDateTimePicker
          Left = 98
          Top = 18
          Width = 100
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
          Color = clWhite
          DateFormat = dfLong
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 0
          UnboundDataType = wwDTEdtDate
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtFromChange
          OnExit = dbdtFromExit
          OnKeyPress = dbdtFromKeyPress
        end
        object dbdtTo: TwwDBDateTimePicker
          Left = 279
          Top = 19
          Width = 100
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
          Color = clWhite
          DateFormat = dfLong
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 1
          UnboundDataType = wwDTEdtDate
          DisplayFormat = 'dd mmm yyyy'
          OnCloseUp = dbdtToCloseUp
          OnChange = dbdtFromChange
          OnExit = dbdtFromExit
          OnKeyPress = dbdtFromKeyPress
        end
      end
      object rgpSortBy: TRadioGroup
        Left = 0
        Top = -1047
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Sort By'
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'Member'
          'Name'
          'Surname')
        TabOrder = 1
      end
      object rgpMembers: TRadioGroup
        Left = 0
        Top = -1341
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All Members'
          'Financial'
          'Non Financial'
          'Active'
          'Non Active'
          'Part Financial')
        TabOrder = 2
      end
      object gbxEvents: TGroupBox
        Left = 0
        Top = -900
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 3
        object btnEvents: TSpeedButton
          Left = 213
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Events'
          OnClick = btnEventsClick
        end
        object cbxAllEvents: TCheckBox
          Left = 80
          Top = 17
          Width = 81
          Height = 17
          Caption = 'All Events'
          TabOrder = 0
          OnClick = cbxAllEventsClick
        end
      end
      object gbxGroups: TGroupBox
        Left = 0
        Top = -802
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 4
        object btnGroups: TSpeedButton
          Left = 213
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Groups'
          OnClick = btnEventsClick
        end
        object cbxAllGroups: TCheckBox
          Left = 80
          Top = 17
          Width = 81
          Height = 17
          Caption = 'All Groups'
          TabOrder = 0
          OnClick = cbxAllGroupsClick
        end
      end
      object gbxSubs: TGroupBox
        Left = 0
        Top = -949
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 5
        object btnSubs: TSpeedButton
          Left = 213
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Subs'
          OnClick = btnEventsClick
        end
        object cbxAllSubs: TCheckBox
          Left = 80
          Top = 17
          Width = 81
          Height = 17
          Caption = 'All Subs'
          TabOrder = 0
          OnClick = cbxAllSubsClick
        end
      end
      object rgpProduct: TRadioGroup
        Left = 0
        Top = -1488
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'Product'
          'Supplier'
          'Group'
          'Type'
          'Category')
        TabOrder = 6
      end
      object gbxLocations: TGroupBox
        Left = 0
        Top = -998
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 7
        object btnLocations: TSpeedButton
          Left = 214
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Locations'
          OnClick = btnEventsClick
        end
        object cbxAllLocations: TCheckBox
          Left = 82
          Top = 17
          Width = 81
          Height = 17
          Caption = 'All Locations'
          TabOrder = 0
          OnClick = cbxAllLocationsClick
        end
      end
      object gbxMemo: TGroupBox
        Left = 0
        Top = -557
        Width = 407
        Height = 59
        Align = alTop
        Caption = 'Heading'
        TabOrder = 8
        object Memo: TMemo
          Left = 13
          Top = 15
          Width = 367
          Height = 34
          Hint = 'Type in a heading for the file'
          Color = clMenu
          TabOrder = 0
          OnEnter = MemoEnter
          OnExit = MemoExit
        end
      end
      object rgpMembersOther: TRadioGroup
        Left = 0
        Top = -1439
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All Members'
          'Senior'
          'Foundation '
          'Donated'
          'Refunded'
          'not Refunded')
        TabOrder = 9
      end
      object gbxMailJoin: TGroupBox
        Left = 0
        Top = -1586
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Optional'
        TabOrder = 10
        object lblMailJoin: TLabel
          Left = 30
          Top = 21
          Width = 61
          Height = 13
          Caption = 'Mailing Code'
        end
        object Label6: TLabel
          Left = 182
          Top = 21
          Width = 85
          Height = 13
          Caption = 'OR    Joining Fee '
        end
        object dblcMailCode: TwwDBLookupCombo
          Left = 99
          Top = 18
          Width = 68
          Height = 19
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MAILING_CODE'#9'10'#9'MAILING_CODE'#9'F')
          LookupTable = dm.qryReport1
          Options = [loColLines, loRowLines]
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 0
          AutoDropDown = True
          ShowButton = True
          UseTFields = False
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcMailCodeDropDown
          OnKeyPress = dbdtFromKeyPress
        end
        object dblcJoiningFee: TwwDBLookupCombo
          Left = 270
          Top = 18
          Width = 70
          Height = 19
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'JOINING_FEE'#9'10'#9'JOINING_FEE'#9'F')
          LookupTable = dm.qryReport1
          Options = [loColLines, loRowLines]
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 1
          AutoDropDown = True
          ShowButton = True
          UseTFields = False
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcMailCodeDropDown
          OnKeyPress = dbdtFromKeyPress
        end
      end
      object gbxMonths: TGroupBox
        Left = 0
        Top = -851
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 11
        object Label10: TLabel
          Left = 109
          Top = 20
          Width = 72
          Height = 13
          Caption = 'Select a Month'
        end
        object cbxMonths: TwwDBComboBox
          Left = 197
          Top = 17
          Width = 91
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 12
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'January'
            'Feburary'
            'March'
            'April'
            'May'
            'June'
            'July'
            'August'
            'September'
            'October'
            'November'
            'December')
          ItemIndex = 0
          Sorted = False
          TabOrder = 0
          UnboundDataType = wwDefault
        end
      end
      object gbxNoOptions: TGroupBox
        Left = 0
        Top = 566
        Width = 407
        Height = 53
        Align = alTop
        TabOrder = 12
        object Label8: TLabel
          Left = 16
          Top = 22
          Width = 360
          Height = 13
          Caption = 
            'There are no selection criteria for this report - press one of t' +
            'he above buttons'
        end
      end
      object rgpLines: TRadioGroup
        Left = 0
        Top = -1537
        Width = 407
        Height = 49
        Align = alTop
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Print Individual Lines'
          'Print Totals Only')
        TabOrder = 13
      end
      object rgpAlpha: TRadioGroup
        Left = 0
        Top = -1145
        Width = 407
        Height = 49
        Align = alTop
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Alpha  ( by Surname )'
          'Numeric ( by Member )')
        TabOrder = 14
      end
      object gbxSections: TGroupBox
        Left = 0
        Top = -753
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 15
        object btnSections: TSpeedButton
          Left = 214
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Sections'
          OnClick = btnEventsClick
        end
        object cbxAllSections: TCheckBox
          Left = 80
          Top = 17
          Width = 81
          Height = 17
          Caption = 'All Sections'
          TabOrder = 0
          OnClick = cbxAllSectionsClick
        end
      end
      object rgpMembersSections: TRadioGroup
        Left = 0
        Top = -4
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All Members'
          'Financial'
          'Non Financial')
        TabOrder = 16
      end
      object rgpSortBySections: TRadioGroup
        Left = 0
        Top = -1194
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Report By'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Member by Section'
          'Section by Member')
        TabOrder = 17
      end
      object rgpSelection: TRadioGroup
        Left = 0
        Top = -1292
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Members'
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'Normal'
          'New '
          'Recently Paid')
        TabOrder = 18
      end
      object gbxQuery: TGroupBox
        Left = 0
        Top = -655
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 19
        object SpeedButton1: TSpeedButton
          Left = 144
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Query'
          OnClick = btnQueryClick
        end
      end
      object gbxExport: TGroupBox
        Left = 0
        Top = -606
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Export To'
        TabOrder = 20
        OnClick = gbxExportClick
        object dbcbExport: TwwDBComboBox
          Left = 46
          Top = 18
          Width = 115
          Height = 19
          Hint = 
            'Select = choose your own directory or drop down to pick one in t' +
            'he list '
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 6
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'Select'
            'C:\'
            'A:\'
            'C:\My Documents'
            '')
          ItemIndex = 0
          Sorted = False
          TabOrder = 0
          UnboundDataType = wwDefault
          OnChange = dbcbExportChange
        end
        object dirEdit: TDirectoryEdit
          Left = 199
          Top = 17
          Width = 149
          Height = 21
          Hint = ' choose your own directory'
          InitialDir = 'c:\'
          Color = clWhite
          NumGlyphs = 1
          TabOrder = 1
          Text = 'c:\'
          OnEnter = dirEditEnter
          OnExit = dirEditExit
        end
      end
      object gbxFields: TGroupBox
        Left = 0
        Top = -704
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 21
        object btnFields: TSpeedButton
          Left = 144
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Fields'
          OnClick = btnFieldsClick
        end
      end
      object rgpNewMember: TRadioGroup
        Left = 0
        Top = -498
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Members'
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All'
          'New'
          'Recently Paid')
        TabOrder = 22
        OnClick = rgpNewMemberClick
      end
      object rgpSortLabels: TRadioGroup
        Left = 0
        Top = -1096
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Sort By'
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'Member'
          'Name'
          'Surname'
          'Surname + MailCode'
          'Surname + JoinFee')
        TabOrder = 23
      end
      object rgpLinks: TRadioGroup
        Left = 0
        Top = -1390
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'Groups'
          'Sections'
          'Subscriptions')
        TabOrder = 24
        OnClick = rgpLinksClick
      end
      object gbxLetter: TGroupBox
        Left = 0
        Top = -200
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 25
        object Label3: TLabel
          Left = 59
          Top = 20
          Width = 69
          Height = 13
          Caption = 'Select a Letter'
        end
        object dblcLetters: TwwDBLookupCombo
          Left = 138
          Top = 18
          Width = 182
          Height = 19
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'NAME'#9'25'#9'NAME'#9'F')
          LookupTable = dm.qryReport1
          Options = [loColLines, loRowLines]
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 0
          AutoDropDown = True
          ShowButton = True
          UseTFields = False
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcMailCodeDropDown
          OnKeyPress = dbdtFromKeyPress
        end
      end
      object gbxAges: TGroupBox
        Left = 0
        Top = -151
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 26
        object Label4: TLabel
          Left = 104
          Top = 21
          Width = 53
          Height = 13
          Caption = 'Years From'
        end
        object Label7: TLabel
          Left = 252
          Top = 21
          Width = 43
          Height = 13
          Caption = 'Years To'
        end
        object dbeYearFrom: TwwDBEdit
          Left = 173
          Top = 18
          Width = 60
          Height = 21
          Picture.PictureMask = '[-]#[#][#][#]'
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbdtFromKeyPress
        end
        object dbeYearTo: TwwDBEdit
          Left = 304
          Top = 18
          Width = 60
          Height = 21
          Picture.PictureMask = '[-]#[#][#][#]'
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbdtFromKeyPress
        end
        object cbxDOB: TCheckBox
          Left = 15
          Top = 19
          Width = 61
          Height = 17
          Caption = 'No DOB'
          TabOrder = 2
          OnClick = cbxDOBClick
        end
      end
      object rpgReceiptSortBy: TRadioGroup
        Left = 0
        Top = -449
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Sort By'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Receipt'
          'Parent Code , Surname')
        TabOrder = 27
      end
      object rgpExpires: TRadioGroup
        Left = 0
        Top = -400
        Width = 407
        Height = 49
        Align = alTop
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Expires Date Range'
          'All Members')
        TabOrder = 28
        OnClick = rgpExpiresClick
      end
      object gbxQueries: TGroupBox
        Left = 0
        Top = -351
        Width = 407
        Height = 102
        Align = alTop
        TabOrder = 29
        object Label9: TLabel
          Left = 59
          Top = 20
          Width = 70
          Height = 13
          Caption = 'Select a Query'
        end
        object dblcQueries: TwwDBLookupCombo
          Left = 138
          Top = 18
          Width = 182
          Height = 19
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'NAME'#9'25'#9'NAME'#9'F')
          LookupTable = dm.qryReport1
          Options = [loColLines, loRowLines]
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 0
          AutoDropDown = True
          ShowButton = True
          UseTFields = False
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcMailCodeDropDown
          OnCloseUp = dblcQueriesCloseUp
          OnKeyPress = dbdtFromKeyPress
        end
        object memoQueries: TMemo
          Left = 13
          Top = 49
          Width = 367
          Height = 43
          Hint = 'Comments'
          Color = clMenu
          TabOrder = 1
          OnEnter = MemoEnter
          OnExit = MemoExit
        end
      end
      object gbxAge: TGroupBox
        Left = 0
        Top = 94
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 30
        object Label11: TLabel
          Left = 13
          Top = 21
          Width = 114
          Height = 13
          Caption = 'Age Greater or Equal  to'
        end
        object Label12: TLabel
          Left = 203
          Top = 22
          Width = 54
          Height = 13
          Caption = 'at this Date'
        end
        object dbdtThisDAte: TwwDBDateTimePicker
          Left = 274
          Top = 19
          Width = 100
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
          Color = clWhite
          DateFormat = dfLong
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ShowButton = True
          TabOrder = 1
          UnboundDataType = wwDTEdtDate
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dbdtFromChange
          OnExit = dbdtFromExit
          OnKeyPress = dbdtFromKeyPress
        end
        object dbeAge: TwwDBEdit
          Left = 139
          Top = 18
          Width = 45
          Height = 21
          Picture.PictureMask = '#[#][#]'
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbdtFromKeyPress
        end
      end
      object rgpExportOptions: TRadioGroup
        Left = 0
        Top = -53
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All Members'
          'Direct Debit'
          'Non Direct Debit')
        TabOrder = 31
      end
      object gbxMemberMulti: TGroupBox
        Left = 0
        Top = -249
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 32
        object cbxAllMembers: TCheckBox
          Left = 12
          Top = 10
          Width = 97
          Height = 17
          Caption = 'All Members'
          TabOrder = 0
        end
        object cbxFinancial: TCheckBox
          Left = 12
          Top = 28
          Width = 97
          Height = 17
          Caption = 'Financial'
          TabOrder = 1
        end
        object cbxNonFinancial: TCheckBox
          Left = 141
          Top = 10
          Width = 97
          Height = 17
          Caption = 'Non Financial'
          TabOrder = 2
        end
        object cbxActive: TCheckBox
          Left = 141
          Top = 28
          Width = 97
          Height = 17
          Caption = 'Active'
          TabOrder = 3
        end
        object cbxNonActive: TCheckBox
          Left = 265
          Top = 10
          Width = 97
          Height = 17
          Caption = 'Non Active'
          TabOrder = 4
        end
        object cbxPartFinancial: TCheckBox
          Left = 265
          Top = 28
          Width = 97
          Height = 17
          Caption = 'Part Financial'
          TabOrder = 5
        end
      end
      object gbxClubs: TGroupBox
        Left = 0
        Top = 45
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 33
        object btnClubs: TSpeedButton
          Left = 275
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Clubs'
          OnClick = btnEventsClick
        end
        object Label13: TLabel
          Left = 27
          Top = 20
          Width = 35
          Height = 13
          Caption = 'Gender'
        end
        object cbxAllClubs: TCheckBox
          Left = 193
          Top = 19
          Width = 65
          Height = 17
          Caption = 'All Clubs'
          TabOrder = 0
          OnClick = cbxAllClubsClick
        end
        object dbcbGender: TwwDBComboBox
          Left = 77
          Top = 18
          Width = 69
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = False
          AllowClearKey = False
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 4
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'All'
            'Male'
            'Female')
          ItemIndex = 0
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
        end
      end
      object rgpToday: TRadioGroup
        Left = 0
        Top = -102
        Width = 407
        Height = 49
        Align = alTop
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Today'
          'Yesterday')
        TabOrder = 34
      end
      object rgpMembersEmail: TRadioGroup
        Left = 0
        Top = 143
        Width = 407
        Height = 49
        Align = alTop
        Columns = 3
        ItemIndex = 0
        Items.Strings = (
          'All Members'
          'Receive Email = Yes'
          'Receive Email = No')
        TabOrder = 35
      end
      object rgpPayment: TRadioGroup
        Left = 0
        Top = 192
        Width = 407
        Height = 68
        Align = alTop
        ItemIndex = 0
        Items.Strings = (
          'Dont Reset Payment'
          'Reset Payment and get Latest Fees'
          
            'Reset Payment and get Latest Fees if within 40 days of Expiry Da' +
            'te')
        TabOrder = 36
      end
      object rgpReportByGroup: TRadioGroup
        Left = 0
        Top = 413
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Report By'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'by Group'
          'by Member')
        TabOrder = 37
      end
      object gbxMMDates: TGroupBox
        Left = 0
        Top = 462
        Width = 407
        Height = 55
        Align = alTop
        Caption = 'MenuMate Dates '
        TabOrder = 38
        object Label1: TLabel
          Left = 16
          Top = 26
          Width = 49
          Height = 13
          Alignment = taRightJustify
          Caption = 'Date From'
        end
        object Label2: TLabel
          Left = 223
          Top = 26
          Width = 39
          Height = 13
          Alignment = taRightJustify
          Caption = 'Date To'
        end
        object dbdtMMDateFrom: TwwDBDateTimePicker
          Left = 71
          Top = 23
          Width = 128
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
          Color = clWhite
          DateFormat = dfLong
          Date = 0.208333333333333
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          Time = 0.208333333333333
          ShowButton = True
          TabOrder = 0
          DisplayFormat = 'dd/mm/yyyy hh:mm:ss'
        end
        object dbdtMMDateTo: TwwDBDateTimePicker
          Left = 268
          Top = 23
          Width = 128
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
          Color = clWhite
          DateFormat = dfLong
          Date = 0.208333333333333
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          Time = 0.208333333333333
          ShowButton = True
          TabOrder = 1
          DisplayFormat = 'dd/mm/yyyy hh:mm:ss'
        end
      end
      object rgpMemberOnLabel: TRadioGroup
        Left = 0
        Top = 364
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'Member Numbers on Label'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'No'
          'Yes')
        TabOrder = 39
      end
      object rgpExportParentCode: TRadioGroup
        Left = 0
        Top = 517
        Width = 407
        Height = 49
        Align = alTop
        Caption = 'by Parent Code'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'No'
          'Yes')
        TabOrder = 40
      end
      object gbxTextInput: TGroupBox
        Left = 0
        Top = 260
        Width = 407
        Height = 55
        Align = alTop
        TabOrder = 41
        object Label5: TLabel
          Left = 94
          Top = 23
          Width = 95
          Height = 13
          Alignment = taRightJustify
          Caption = 'Number of Members'
        end
        object edtTextInput: TEdit
          Left = 199
          Top = 20
          Width = 52
          Height = 21
          TabOrder = 0
          Text = '50'
        end
      end
      object gbxDiscounts: TGroupBox
        Left = 0
        Top = 315
        Width = 407
        Height = 49
        Align = alTop
        TabOrder = 42
        object btnDiscounts: TSpeedButton
          Left = 279
          Top = 15
          Width = 89
          Height = 25
          Caption = 'Select Discounts'
          OnClick = btnEventsClick
        end
        object cbxAllDiscounts: TCheckBox
          Left = 174
          Top = 19
          Width = 81
          Height = 17
          Caption = 'All Discounts'
          TabOrder = 0
          OnClick = cbxAllDiscountsClick
        end
        object cbxNoDiscounts: TCheckBox
          Left = 37
          Top = 19
          Width = 86
          Height = 17
          Caption = 'No Discounts'
          TabOrder = 1
          OnClick = cbxNoDiscountsClick
        end
      end
    end
    object Panel3: TPanel
      Left = 654
      Top = 2
      Width = 12
      Height = 393
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 2
    end
    object Panel4: TPanel
      Left = 2
      Top = 2
      Width = 6
      Height = 393
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 3
    end
  end
  inherited ControlBar1: TControlBar
    Width = 668
    inherited ToolBar: TToolBar
      Width = 651
      ButtonWidth = 41
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object btnPrint: TToolButton
        Left = 3
        Top = 0
        Hint = 'Print the selected report'
        Caption = 'btnPrint'
        ImageIndex = 12
        OnClick = menuPrintClick
      end
      object btnPreview: TToolButton
        Tag = 1
        Left = 44
        Top = 0
        Hint = 'Preview the selected report'
        Caption = 'btnPreview'
        ImageIndex = 15
        OnClick = menuPrintClick
      end
      object btnExport: TToolButton
        Tag = 2
        Left = 85
        Top = 0
        Hint = 'Export the selected report to a comma delimited file'
        Caption = 'btnExport'
        ImageIndex = 17
        OnClick = menuPrintClick
      end
      object btnExcel: TToolButton
        Tag = 3
        Left = 126
        Top = 0
        Hint = 'Export to Excel'
        Caption = 'btnExcel'
        ImageIndex = 16
        OnClick = menuPrintClick
      end
      object btnWord: TToolButton
        Tag = 5
        Left = 167
        Top = 0
        Hint = 'Word Mail Merge'
        Caption = 'btnWord'
        ImageIndex = 31
        OnClick = menuPrintClick
      end
      object btnEmail: TToolButton
        Tag = 4
        Left = 208
        Top = 0
        Hint = 'Send this report via email'
        Caption = 'btnEmail'
        ImageIndex = 34
        OnClick = menuPrintClick
      end
      object btnLetters: TToolButton
        Left = 249
        Top = 0
        Hint = 'Add Edit or Delete Letters'
        Caption = 'btnLetters'
        ImageIndex = 13
        OnClick = menuLettersClick
      end
      object btnQueries: TToolButton
        Left = 290
        Top = 0
        Hint = 'Add Edit or Delete Queries'
        Caption = 'btnQueries'
        ImageIndex = 9
        OnClick = menuQueriesClick
      end
      object btnMailingLabels: TToolButton
        Tag = 7
        Left = 331
        Top = 0
        Hint = 'Create Mailing Labels from the selected Report'
        Caption = 'btnMailingLabels'
        ImageIndex = 30
        OnClick = menuPrintClick
      end
      object btnPrinterSetup: TToolButton
        Left = 372
        Top = 0
        Hint = 'Check the selected Printer'
        Caption = 'btnPrinterSetup'
        ImageIndex = 18
        OnClick = menuPrinterSetupClick
      end
      object btnEmailMembers: TToolButton
        Tag = 8
        Left = 413
        Top = 0
        Hint = 'Send an Email to Members'
        Caption = 'btnEmailMembers'
        ImageIndex = 14
        OnClick = menuPrintClick
      end
      object btnQuery: TToolButton
        Left = 454
        Top = 0
        Caption = 'btnQuery'
        ImageIndex = 10
        OnClick = btnQueryClick
      end
      object pnlSpace: TPanel
        Left = 495
        Top = 0
        Width = 107
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
      end
      object btnClose: TToolButton
        Left = 602
        Top = 0
        Hint = 'Close this Screen'
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 154
    Top = 186
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuOptions: TMenuItem [1]
      Caption = '&Options'
      object menuPrint: TMenuItem
        Caption = 'Print '
        ShortCut = 16464
        OnClick = menuPrintClick
      end
      object menuPrinterSetup: TMenuItem
        Caption = '&Printer Setup'
        OnClick = menuPrinterSetupClick
      end
      object menuPreview: TMenuItem
        Tag = 1
        Caption = 'Pre&view'
        OnClick = menuPrintClick
      end
      object menuExportAscii: TMenuItem
        Tag = 2
        Caption = '&Export to Ascii'
        OnClick = menuPrintClick
      end
      object menuExportExcel: TMenuItem
        Tag = 3
        Caption = 'Export to Ex&cel'
        Hint = 'menuExportExcel'
        OnClick = menuPrintClick
      end
      object menuWordMailingLabels: TMenuItem
        Tag = 5
        Caption = 'Word Mailing Letter'
        OnClick = menuPrintClick
      end
      object menuWordMailingLabelsNew: TMenuItem
        Tag = 6
        Caption = 'Word Mailing Letter New Doc'
        OnClick = menuPrintClick
      end
      object menuEmailReport: TMenuItem
        Tag = 4
        Caption = 'E&mail Report'
        OnClick = menuPrintClick
      end
      object menuLetters: TMenuItem
        Caption = '&Letters'
        OnClick = menuLettersClick
      end
      object menuQueries: TMenuItem
        Caption = 'Queries'
        OnClick = menuQueriesClick
      end
      object menuMailingLabels: TMenuItem
        Tag = 7
        Caption = 'Mailing Labels from Last Report'
        OnClick = menuPrintClick
      end
      object menuEmailMembers: TMenuItem
        Tag = 8
        Caption = 'Email Members'
        OnClick = menuPrintClick
      end
    end
    inherited menuHelpTop: TMenuItem
      inherited menuHelp: TMenuItem
        Caption = '&Help'
      end
    end
  end
  object FieldsDialog: TDualListDialog
    Sorted = False
    Title = 'Select Member Fields for Export'
    Label1Caption = 'Member Fields'
    Label2Caption = 'Export in order of list'
    HelpContext = 0
    ShowHelp = False
    Left = 152
    Top = 160
  end
  object OpOutlook: TOpOutlook
    Version = '1.64'
    NewSession = False
    ShowLoginDialog = False
    Left = 152
    Top = 120
  end
  object PopupMenu: TPopupMenu
    Left = 152
    Top = 96
    object ReportHoldFree1: TMenuItem
      Caption = 'Report Hold/Free'
      OnClick = ReportHoldFree1Click
    end
  end
end
