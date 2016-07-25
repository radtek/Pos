inherited frmMembersMAINT: TfrmMembersMAINT
  Tag = 3
  Left = 864
  Top = 161
  Caption = 'frmMembersMAINT'
  ClientHeight = 356
  ClientWidth = 612
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 336
    Width = 612
  end
  object PageControl: TPageControl [1]
    Left = 0
    Top = 60
    Width = 612
    Height = 276
    ActivePage = tsKioskMsg
    Align = alClient
    MultiLine = True
    RaggedRight = True
    TabOrder = 0
    OnChange = PageControlChange
    object tsLinks: TTabSheet
      Caption = '&Links'
      object rgpLinks: TLMDRadioGroup
        Left = 0
        Top = 0
        Width = 604
        Height = 41
        Align = alTop
        Alignment = gcaRightTop
        Bevel.Mode = bmCustom
        Bevel.StandardStyle = lsSingleLowered
        CaptionFont.Charset = DEFAULT_CHARSET
        CaptionFont.Color = clBlack
        CaptionFont.Height = -11
        CaptionFont.Name = 'Arial'
        CaptionFont.Style = []
        CaptionParentFont = False
        Columns = 4
        Items.Strings = (
          'Subs'
          'Sections'
          'Events'
          'Groups')
        ItemOffset = 20
        TabOrder = 0
        OnChange = rgpLinksChange
        ItemIndex = 0
      end
      object dbgLinks: TwwDBGrid
        Left = 0
        Top = 41
        Width = 604
        Height = 189
        ControlType.Strings = (
          'KEEP_EXPIRES_CURRENT;CheckBox;Y;N'
          'DISCOUNTABLE;CheckBox;Y;N'
          'PRINT_ON_CARD;CheckBox;Y;N'
          'PRO_RATA;CheckBox;Y;N'
          'SUB_GROUPS;CustomEdit;dblcGroups'
          'POINTS_SPENT;CustomEdit;dblcRptGroups'
          'ON_INVOICE;CustomEdit;dbcbOnInvoice')
        Selected.Strings = (
          'DESCRIPTION'#9'24'#9'Description'
          'PRINT_ON_CARD'#9'7'#9'On Card'
          'CARD_DESC'#9'35'#9'Card Description'
          'SUB_GROUPS'#9'10'#9'Group'
          'ON_INVOICE'#9'1'#9'On Invoice'
          'POINTS_SPENT'#9'10'#9'Rpt Group')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrTblSubs
        KeyOptions = [dgEnterToTab, dgAllowDelete]
        Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
        TabOrder = 1
        TitleAlignment = taLeftJustify
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        OnCalcCellColors = dbgOtherCalcCellColors
        OnExit = dbgOtherExit
        IndicatorColor = icBlack
      end
      object dblcGroups: TwwDBLookupCombo
        Left = 256
        Top = 112
        Width = 121
        Height = 21
        DropDownAlignment = taLeftJustify
        Selected.Strings = (
          'GROUPS'#9'10'#9'Code'#9'F'
          'DESCRIPTION'#9'24'#9'Description'#9'F')
        DataField = 'SUB_GROUPS'
        DataSource = dm.dsrTblSubs
        LookupTable = dm.tblGroups
        LookupField = 'GROUPS'
        DropDownCount = 10
        TabOrder = 2
        AutoDropDown = False
        ShowButton = True
        AllowClearKey = False
      end
      object dbcbOnInvoice: TwwDBComboBox
        Left = 216
        Top = 78
        Width = 41
        Height = 19
        ShowButton = True
        Style = csDropDown
        MapList = True
        AllowClearKey = False
        BorderStyle = bsNone
        DataField = 'ON_INVOICE'
        DataSource = dm.dsrTblSubs
        DropDownCount = 8
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ItemHeight = 0
        Items.Strings = (
          'Yes'#9'Y'
          'No'#9'N')
        ItemIndex = 0
        Sorted = False
        TabOrder = 3
        UnboundDataType = wwDefault
      end
      object dblcRptGroups: TwwDBLookupCombo
        Left = 264
        Top = 136
        Width = 121
        Height = 21
        DropDownAlignment = taLeftJustify
        Selected.Strings = (
          'GROUPS'#9'10'#9'Code'#9'F'
          'DESCRIPTION'#9'24'#9'Description'#9'F')
        DataField = 'POINTS_SPENT'
        DataSource = dm.dsrTblSubs
        LookupTable = dm.tblGroups
        LookupField = 'GROUPS'
        DropDownCount = 10
        TabOrder = 4
        AutoDropDown = False
        ShowButton = True
        AllowClearKey = False
      end
    end
    object tsLuckyMember: TTabSheet
      Caption = 'L&ucky Member'
      ImageIndex = 2
      object Panel14: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 230
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 8
        TabOrder = 0
        object GroupBox6: TGroupBox
          Left = 343
          Top = 8
          Width = 253
          Height = 214
          Align = alClient
          Caption = 'Display Lucky Member Photo '
          TabOrder = 0
          object Label1: TLabel
            Left = 22
            Top = 71
            Width = 93
            Height = 13
            Caption = 'End Time ( 24 hour)'
          end
          object Label2: TLabel
            Left = 20
            Top = 107
            Width = 110
            Height = 13
            Caption = 'Member Time (Minutes)'
          end
          object Label3: TLabel
            Left = 20
            Top = 34
            Width = 99
            Height = 13
            Caption = 'Start Time ( 24 hour )'
          end
          object Label7: TLabel
            Left = 20
            Top = 144
            Width = 101
            Height = 13
            Caption = 'Increase Photo Size  '
          end
          object edtMemberTime: TwwDBEdit
            Left = 137
            Top = 104
            Width = 73
            Height = 21
            Hint = 'Time is in Minutes'
            Picture.PictureMask = '#[#][#]'
            TabOrder = 2
            UnboundDataType = wwDefault
            UnboundAlignment = taRightJustify
            WantReturns = False
            WordWrap = False
          end
          object UpDown1: TUpDown
            Left = 210
            Top = 104
            Width = 15
            Height = 21
            Associate = edtMemberTime
            Min = 0
            Position = 0
            TabOrder = 4
            Wrap = False
          end
          object btnStartPhoto: TButton
            Left = 137
            Top = 175
            Width = 87
            Height = 25
            Hint = 'Press Escape to cancel '
            Caption = 'Start'
            TabOrder = 3
            OnClick = btnStartPhotoClick
          end
          object dbdtStartTime: TwwDBDateTimePicker
            Left = 137
            Top = 32
            Width = 90
            Height = 21
            CalendarAttributes.Font.Charset = DEFAULT_CHARSET
            CalendarAttributes.Font.Color = clWindowText
            CalendarAttributes.Font.Height = -11
            CalendarAttributes.Font.Name = 'MS Sans Serif'
            CalendarAttributes.Font.Style = []
            Epoch = 1950
            ShowButton = True
            TabOrder = 0
            UnboundDataType = wwDTEdtTime
          end
          object dbdtEndTime: TwwDBDateTimePicker
            Left = 137
            Top = 68
            Width = 90
            Height = 21
            CalendarAttributes.Font.Charset = DEFAULT_CHARSET
            CalendarAttributes.Font.Color = clWindowText
            CalendarAttributes.Font.Height = -11
            CalendarAttributes.Font.Name = 'MS Sans Serif'
            CalendarAttributes.Font.Style = []
            Epoch = 1950
            ShowButton = True
            TabOrder = 1
            UnboundDataType = wwDTEdtTime
          end
          object edtPhotoSize: TwwDBEdit
            Left = 137
            Top = 140
            Width = 73
            Height = 21
            Hint = 'Increase the Photo size by a Percentage if > 0'
            Picture.PictureMask = '[-]#[#][#][#]'
            TabOrder = 5
            UnboundDataType = wwDefault
            UnboundAlignment = taRightJustify
            WantReturns = False
            WordWrap = False
          end
          object UpDown2: TUpDown
            Left = 210
            Top = 140
            Width = 15
            Height = 21
            Associate = edtPhotoSize
            Min = 0
            Position = 0
            TabOrder = 6
            Wrap = False
          end
        end
        object Panel15: TPanel
          Left = 323
          Top = 8
          Width = 20
          Height = 214
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 1
        end
        object Panel20: TPanel
          Left = 8
          Top = 8
          Width = 315
          Height = 214
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 2
          object GroupBox7: TGroupBox
            Left = 0
            Top = 0
            Width = 315
            Height = 58
            Align = alTop
            Caption = 'Select One Lucky Member '
            TabOrder = 0
            object Label6: TLabel
              Left = 141
              Top = 27
              Width = 112
              Height = 13
              Caption = 'Select a Lucky Member'
            end
            object btnLuckyMember: TButton
              Left = 35
              Top = 21
              Width = 87
              Height = 25
              Caption = 'Start'
              TabOrder = 0
              OnClick = btnLuckyMemberClick
            end
          end
          object GroupBox14: TGroupBox
            Left = 0
            Top = 65
            Width = 315
            Height = 149
            Align = alClient
            Caption = 'Kiosk Lucky Draw'
            TabOrder = 1
            object Label24: TLabel
              Left = 36
              Top = 23
              Width = 119
              Height = 13
              Caption = 'Daily number of Members'
            end
            object Label25: TLabel
              Left = 37
              Top = 46
              Width = 80
              Height = 13
              Caption = 'Kiosk Draw Date'
            end
            object dbgKioskDraw: TwwDBGrid
              Left = 8
              Top = 71
              Width = 298
              Height = 75
              Selected.Strings = (
                'KIOSK_ID'#9'11'#9'Kiosk Number'
                'KIOSK_DESC'#9'31'#9'Description')
              IniAttributes.Delimiter = ';;'
              TitleColor = clBtnFace
              FixedCols = 0
              ShowHorzScrollBar = True
              DataSource = dm.dsrKioskDraws
              TabOrder = 0
              TitleAlignment = taLeftJustify
              TitleFont.Charset = DEFAULT_CHARSET
              TitleFont.Color = clBlack
              TitleFont.Height = -11
              TitleFont.Name = 'MS Sans Serif'
              TitleFont.Style = []
              TitleLines = 1
              TitleButtons = False
              OnEnter = dbgKioskDrawEnter
              IndicatorColor = icBlack
            end
            object edtKioskDraws: TwwDBEdit
              Left = 169
              Top = 18
              Width = 38
              Height = 21
              DataField = 'KIOSK_DRAWS'
              DataSource = dm.dsrTblMisc
              TabOrder = 1
              UnboundDataType = wwDefault
              WantReturns = False
              WordWrap = False
              OnEnter = edtKioskDrawsEnter
            end
            object wwDBDateTimePicker1: TwwDBDateTimePicker
              Left = 168
              Top = 43
              Width = 98
              Height = 21
              Hint = 'Shows the Kiosk Draw Date - is controlled from the Kiosk'
              CalendarAttributes.Font.Charset = DEFAULT_CHARSET
              CalendarAttributes.Font.Color = clWindowText
              CalendarAttributes.Font.Height = -11
              CalendarAttributes.Font.Name = 'MS Sans Serif'
              CalendarAttributes.Font.Style = []
              DataField = 'KIOSK_DRAW_DATE'
              DataSource = dm.dsrTblMisc
              Epoch = 1950
              ReadOnly = True
              ShowButton = True
              TabOrder = 2
              UnboundDataType = wwDTEdtTime
              DisplayFormat = 'dd mmm yyyy'
            end
          end
          object Panel21: TPanel
            Left = 0
            Top = 58
            Width = 315
            Height = 7
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
          end
        end
      end
    end
    object tsBulkSection: TTabSheet
      Caption = '&Bulk Section'
      ImageIndex = 6
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 310
        Height = 230
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel5'
        TabOrder = 0
        object Panel6: TPanel
          Left = 5
          Top = 5
          Width = 300
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object DBText1: TDBText
            Left = 128
            Top = 11
            Width = 42
            Height = 13
            AutoSize = True
            DataField = 'DESCRIPTION'
            DataSource = dm.dsrTblSections
          end
          object btnDelete: TButton
            Left = 3
            Top = 6
            Width = 87
            Height = 25
            Caption = 'Delete Members'
            TabOrder = 0
            OnClick = btnDeleteClick
          end
        end
        object dbgSectionBulk: TwwDBGrid
          Left = 5
          Top = 46
          Width = 300
          Height = 179
          Hint = 'Select a Section for Bulk deleting or loading Members'
          Selected.Strings = (
            'SECTION'#9'6'#9'Code'#9'F'
            'DESCRIPTION'#9'21'#9'Description'#9'F'
            'EXPIRES'#9'14'#9'Expires'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrTblSections
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
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
          UseTFields = False
          IndicatorColor = icBlack
        end
      end
      object Panel7: TPanel
        Left = 310
        Top = 0
        Width = 294
        Height = 230
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 1
        object Panel8: TPanel
          Left = 5
          Top = 5
          Width = 284
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Button1: TButton
            Left = 1
            Top = 6
            Width = 87
            Height = 25
            Caption = 'Load Members'
            TabOrder = 0
            OnClick = dbnLinksLoadClick
          end
        end
        object Memo: TMemo
          Left = 5
          Top = 46
          Width = 284
          Height = 179
          Hint = 'Enter Member Numbers in here - one per line'
          Align = alClient
          Color = clWhite
          ScrollBars = ssVertical
          TabOrder = 1
        end
      end
    end
    object tsOther: TTabSheet
      Caption = '&Other'
      ImageIndex = 3
      object dbgOther: TwwDBGrid
        Left = 0
        Top = 41
        Width = 604
        Height = 189
        ControlType.Strings = (
          'HAS_ACCESS;CheckBox;Y;N')
        Selected.Strings = (
          'DESCRIPTION'#9'35'#9'DESCRIPTION'
          'NEXT_MEMBER'#9'17'#9'NEXT_MEMBER'
          'NUMBER_ID'#9'12'#9'NUMBER_ID')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrTblMailCodes
        KeyOptions = [dgEnterToTab]
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
        OnExit = dbgOtherExit
        IndicatorColor = icBlack
      end
      object rgpOther: TLMDRadioGroup
        Left = 0
        Top = 0
        Width = 604
        Height = 41
        Align = alTop
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
        Columns = 4
        Items.Strings = (
          'Mail Codes'
          'Tenders'
          'Member Numbers'
          'Clubs')
        ItemOffset = 30
        TabOrder = 1
        OnChange = rgpOtherChange
        ItemIndex = 0
      end
    end
    object tsSetupPoints: TTabSheet
      Caption = '&Bonus Points'
      ImageIndex = 4
      object Panel10: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 52
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object GroupBox4: TGroupBox
          Left = 0
          Top = 0
          Width = 604
          Height = 52
          Align = alClient
          Caption = 'Select by  '
          TabOrder = 0
          object Label11: TLabel
            Left = 307
            Top = 23
            Width = 71
            Height = 13
            Caption = 'Function Cards'
          end
          object Label12: TLabel
            Left = 62
            Top = 23
            Width = 20
            Height = 13
            Caption = 'Find'
          end
          object dblcMember: TwwDBLookupCombo
            Tag = 99
            Left = 103
            Top = 20
            Width = 121
            Height = 19
            Hint = 
              'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
              ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
            TabStop = False
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
            OnCloseUp = dblcMemberCloseUp
            OnKeyPress = dblcMemberKeyPress
          end
          object btnCards: TButton
            Left = 408
            Top = 16
            Width = 87
            Height = 25
            Caption = 'Start'
            TabOrder = 1
            OnClick = btnCardsClick
          end
        end
      end
      object Panel11: TPanel
        Left = 0
        Top = 108
        Width = 604
        Height = 122
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel11'
        TabOrder = 1
        object dbgPoints: TwwDBGrid
          Left = 5
          Top = 5
          Width = 594
          Height = 112
          ControlType.Strings = (
            'HAS_ACCESS;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'7'#9'Member'
            'FULL_NAME'#9'22'#9'Full Name'
            'KNOWN_AS'#9'15'#9'Known As'
            'PAY'#9'10'#9'Bonus Points'
            'REASON'#9'20'#9'Reason'
            'POINTS_AVAILABLE'#9'10'#9'Points Available'
            'TOTAL_EARNT'#9'10'#9'Points Earnt'
            'TOTAL_REDEEMED'#9'10'#9'Points Redeemed'
            'EARNT_CREDIT'#9'10'#9'Earnt Credit'
            'BONUS_CREDIT'#9'10'#9'Bonus Credit'
            'PRE_PAID_CREDIT'#9'10'#9'Pre Paid Credit'
            'VALUE_OF_CREDIT'#9'10'#9'Value of Credit')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPoints
          EditCalculated = True
          KeyOptions = [dgEnterToTab]
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
          OnCalcCellColors = dbgPointsCalcCellColors
          OnDblClick = dbgPointsDblClick
          OnKeyPress = dbgPointsKeyPress
          IndicatorColor = icBlack
        end
      end
      object GroupBox5: TGroupBox
        Left = 0
        Top = 52
        Width = 604
        Height = 56
        Align = alTop
        Caption = 'Options '
        TabOrder = 2
        object btnCancel: TButton
          Left = 408
          Top = 17
          Width = 87
          Height = 25
          Hint = 'Cancels any changes to the Members'
          Caption = 'Cancel'
          TabOrder = 0
          OnClick = btnCancelClick
        end
        object btnSave: TButton
          Left = 238
          Top = 18
          Width = 87
          Height = 25
          Hint = 'Saves the Points to  the Selected Member'
          Caption = 'Save'
          TabOrder = 1
          OnClick = btnSaveClick
        end
        object rgpBonusPoints: TLMDRadioGroup
          Left = 2
          Top = 15
          Width = 186
          Height = 38
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
            'Bonus Points'
            'Reset All Points and Credit')
          ItemOffset = 25
          TabOrder = 2
          ItemIndex = 0
        end
      end
    end
    object tsCards: TTabSheet
      Caption = 'Cards'
      ImageIndex = 5
      object Panel1: TPanel
        Left = 0
        Top = 170
        Width = 604
        Height = 60
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 0
        object GroupBox1: TGroupBox
          Left = 5
          Top = 5
          Width = 594
          Height = 50
          Align = alClient
          TabOrder = 0
          object Label4: TLabel
            Left = 272
            Top = 21
            Width = 59
            Height = 13
            Caption = 'Default Card'
          end
          object Label5: TLabel
            Left = 17
            Top = 21
            Width = 67
            Height = 13
            Caption = 'Selected Card'
          end
          object edtCardUsed: TEdit
            Left = 347
            Top = 17
            Width = 120
            Height = 21
            Hint = 'Double Click in the Grid to Setup up the Default Card'
            Color = clSilver
            ReadOnly = True
            TabOrder = 0
            Text = ' '
            OnDblClick = edtCardUsedDblClick
          end
          object wwDBEdit1: TwwDBEdit
            Tag = 1
            Left = 97
            Top = 18
            Width = 120
            Height = 19
            Hint = 'Cant Select here - scroll thru the Grid'
            TabStop = False
            BorderStyle = bsNone
            Color = clSilver
            DataField = 'CARD_NAME'
            DataSource = dm.dsrCardMaster
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ParentFont = False
            ReadOnly = True
            TabOrder = 1
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
          end
        end
      end
      object rgpCards: TLMDRadioGroup
        Left = 0
        Top = 0
        Width = 604
        Height = 41
        Align = alTop
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
        Columns = 2
        FocusStyle = fcControl
        ImageIndex = 1
        Items.Strings = (
          'Create Card'
          'Card Details')
        ItemOffset = 70
        TabOrder = 1
        OnChange = rgpCardsChange
        ItemIndex = 0
      end
      object Panel2: TPanel
        Left = 0
        Top = 41
        Width = 604
        Height = 129
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel2'
        TabOrder = 2
        object dbgCards: TwwDBGrid
          Left = 5
          Top = 5
          Width = 594
          Height = 119
          ControlType.Strings = (
            'HAS_ACCESS;CheckBox;Y;N'
            'FIELD_NAME;CustomEdit;dbcbFieldNames'
            'GRAPHIC_MODE;CustomEdit;dbcbGraphicMode'
            'ROTATION;CustomEdit;dbcbRotation'
            'STYLE;CustomEdit;dbcbStyle'
            'TEXT_POSITION;CustomEdit;dbcbTextPos')
          Selected.Strings = (
            'FIELD_NAME'#9'15'#9'Field Name'#9'F'
            'CARD_TEXT'#9'27'#9'Card Text'#9'F'
            'TEXT_POSITION'#9'8'#9'Text Pos'#9'F'
            'HORIZONTAL'#9'11'#9'Horizontal'#9'F'
            'VERTICAL'#9'10'#9'Vertical'#9'F'
            'STYLE'#9'7'#9'Style'#9'F'
            'SIZE'#9'6'#9'Size'#9'F'
            'PROPORTION'#9'12'#9'Proportion'#9'F'
            'ROTATION'#9'21'#9'Rotation'#9'F'
            'GRAPHIC_MODE'#9'18'#9'Graphic Mode'#9'F'
            'SIZE_REDUCE'#9'10'#9'SIZE_REDUCE'#9'F'
            'TEXT_LENGTH'#9'10'#9'TEXT_LENGTH'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrCardDetail
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
          OnCalcCellColors = dbgCardsCalcCellColors
          OnDblClick = dbgCardsDblClick
          OnExit = dbgOtherExit
          IndicatorColor = icBlack
        end
        object dbcbStyle: TwwDBComboBox
          Left = 216
          Top = 78
          Width = 41
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          BorderStyle = bsNone
          DropDownCount = 8
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'Normal'#9'0'
            'Bold *'#9'1')
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
        end
        object dbcbGraphicMode: TwwDBComboBox
          Left = 152
          Top = 78
          Width = 41
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          BorderStyle = bsNone
          DropDownCount = 8
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'Reverse Bit Map'#9'0'
            'Standard Bit Map *'#9'1'
            'Merge Bit Map'#9'2')
          Sorted = False
          TabOrder = 2
          UnboundDataType = wwDefault
        end
        object dbcbFieldNames: TwwDBComboBox
          Left = 312
          Top = 86
          Width = 89
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          BorderStyle = bsNone
          DropDownCount = 22
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'MEMBER'
            'FULL_NAME'
            'EXPIRES'
            'JOINED'
            'SURNAME'
            'TITLE'
            'INITIALS'
            'FIRST_NAME'
            'KNOWN_AS'
            'ADDRESS'
            'ADDRESS1'
            'DISTRICT'
            'CITY'
            'POSITIONS'
            'SUBSCRIPTIONS'
            'GROUPS'
            'SECTIONS'
            'CARD TYPE'
            'FIRST_NAME_SURNAME'
            'KNOWN_AS_SURNAME'
            'UPPER_FULL_NAME'
            'JOINED_YEAR')
          Sorted = False
          TabOrder = 3
          UnboundDataType = wwDefault
        end
        object dbcbRotation: TwwDBComboBox
          Left = 80
          Top = 86
          Width = 65
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          BorderStyle = bsNone
          DropDownCount = 8
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'No Rotation Lower Left *'#9'0'
            '90 Degrees Lower Left'#9'1'
            '180 Degrees Lower Left'#9'2'
            '270 Degrees Lower Left'#9'3'
            'No Rotation Centered'#9'4'
            '90 Degrees Centered'#9'5'
            '180 Degrees Centered'#9'6'
            '270 Degrees Centered'#9'7')
          Sorted = False
          TabOrder = 4
          UnboundDataType = wwDefault
        end
        object dbcbTextPos: TwwDBComboBox
          Left = 272
          Top = 86
          Width = 41
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          BorderStyle = bsNone
          DataField = 'TEXT_POSITION'
          DataSource = dm.dsrCardDetail
          DropDownCount = 8
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'Before'#9'B'
            'After'#9'A')
          Sorted = False
          TabOrder = 5
          UnboundDataType = wwDefault
        end
      end
    end
    object tsCardSetup: TTabSheet
      Caption = 'Card Setup'
      ImageIndex = 6
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 183
        Height = 230
        Align = alLeft
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel3'
        TabOrder = 0
        object GroupBox2: TGroupBox
          Left = 5
          Top = 5
          Width = 173
          Height = 220
          Align = alClient
          Caption = ' Position '
          TabOrder = 0
          object Label8: TLabel
            Left = 25
            Top = 51
            Width = 47
            Height = 13
            Caption = 'Horizontal'
          end
          object Label9: TLabel
            Left = 25
            Top = 99
            Width = 35
            Height = 13
            Caption = 'Vertical'
          end
          object edtHorizontal: TEdit
            Left = 96
            Top = 48
            Width = 40
            Height = 21
            Hint = 'Horizontal Postion of the Text'
            TabOrder = 0
            Text = '0'
            OnKeyPress = edtHorizontalKeyPress
          end
          object edtVertical: TEdit
            Left = 96
            Top = 96
            Width = 40
            Height = 21
            Hint = 'Vertical Postion of the Text'
            TabOrder = 1
            Text = '0'
            OnKeyPress = edtVerticalKeyPress
          end
          object btnCalculate: TButton
            Left = 43
            Top = 149
            Width = 87
            Height = 25
            Hint = 'Calculate approx position of the Text'
            Caption = '&Calculate'
            TabOrder = 2
            OnClick = btnCalculateClick
          end
        end
      end
      object Panel4: TPanel
        Left = 183
        Top = 0
        Width = 421
        Height = 230
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel4'
        TabOrder = 1
        object GroupBox3: TGroupBox
          Left = 5
          Top = 5
          Width = 411
          Height = 220
          Align = alClient
          Caption = ' Card '
          TabOrder = 0
          object pnlCard: TPanel
            Left = 49
            Top = 29
            Width = 255
            Height = 165
            TabOrder = 0
            OnMouseMove = pnlCardMouseMove
            object lblPosition: TLabel
              Left = 88
              Top = 72
              Width = 61
              Height = 13
              Caption = 'Text Position'
            end
          end
        end
      end
    end
    object tsManualCard: TTabSheet
      Caption = 'Manual Card'
      ImageIndex = 7
      object Panel9: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 230
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel9'
        TabOrder = 0
        object dbgManualCard: TwwDBGrid
          Left = 5
          Top = 72
          Width = 594
          Height = 153
          ControlType.Strings = (
            'HAS_ACCESS;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'20'#9'Member'
            'NUMBER_CARDS'#9'20'#9'Number of Cards')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          OnRowChanged = dbgManualCardTopRowChanged
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrCardMember
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
          OnCalcCellColors = dbgManualCardCalcCellColors
          OnExit = dbgOtherExit
          IndicatorColor = icBlack
          OnTopRowChanged = dbgManualCardTopRowChanged
        end
        object Panel13: TPanel
          Left = 5
          Top = 5
          Width = 594
          Height = 67
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object btnExportCrds: TButton
            Left = 329
            Top = 35
            Width = 102
            Height = 25
            Hint = 'Export Cards to a comma delimited file '
            Caption = 'Export Cards'
            TabOrder = 0
            OnClick = btnExportCrdsClick
          end
          object btnAllMembers: TButton
            Left = 460
            Top = 4
            Width = 102
            Height = 25
            Hint = 'Add all Members for a Card'
            Caption = 'All Members'
            TabOrder = 1
            OnClick = btnAllMembersClick
          end
          object edtFullNAme: TwwDBEdit
            Left = 3
            Top = 11
            Width = 158
            Height = 21
            Color = clSilver
            ReadOnly = True
            TabOrder = 2
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
          end
          object dbdtExpires: TwwDBDateTimePicker
            Left = 178
            Top = 11
            Width = 114
            Height = 21
            CalendarAttributes.Font.Charset = DEFAULT_CHARSET
            CalendarAttributes.Font.Color = clWindowText
            CalendarAttributes.Font.Height = -11
            CalendarAttributes.Font.Name = 'MS Sans Serif'
            CalendarAttributes.Font.Style = []
            Color = clSilver
            DateFormat = dfLong
            Epoch = 1950
            ReadOnly = True
            ShowButton = True
            TabOrder = 3
            DisplayFormat = 'dd mmm yyyy'
          end
          object btnPaidMembers: TButton
            Left = 328
            Top = 4
            Width = 102
            Height = 25
            Hint = 'Add all Financial/Part Financial Members for a Card'
            Caption = 'Financial Members'
            TabOrder = 4
            OnClick = btnAllMembersClick
          end
          object btnGroupMembers: TButton
            Left = 461
            Top = 36
            Width = 102
            Height = 25
            Hint = 'Add Members by Group for a Card'
            Caption = 'Members by Group'
            TabOrder = 5
            OnClick = btnAllMembersClick
          end
        end
      end
    end
    object tsMailingLabels: TTabSheet
      BorderWidth = 5
      Caption = 'Mailing Labels Setup'
      ImageIndex = 8
      object GroupBox8: TGroupBox
        Left = 0
        Top = 0
        Width = 594
        Height = 83
        Align = alTop
        Caption = 'Label  (mm)'
        TabOrder = 0
        object Label15: TLabel
          Left = 32
          Top = 18
          Width = 92
          Height = 26
          Caption = 'Number of Columns (Across the Page)'
          WordWrap = True
        end
        object Label14: TLabel
          Left = 186
          Top = 20
          Width = 104
          Height = 13
          Caption = 'Label (Column) Space'
        end
        object Label13: TLabel
          Left = 345
          Top = 20
          Width = 60
          Height = 13
          Caption = 'Label Height'
        end
        object Label20: TLabel
          Left = 505
          Top = 20
          Width = 44
          Height = 13
          Caption = 'Font Size'
        end
        object wwDBComboBox1: TwwDBComboBox
          Left = 46
          Top = 44
          Width = 60
          Height = 21
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          DataField = 'NUMBER_COLUMNS'
          DataSource = dm.dsrMailingLabels
          DropDownCount = 8
          ItemHeight = 0
          Items.Strings = (
            '1'
            '2'
            '3'
            '4')
          Sorted = False
          TabOrder = 0
          UnboundDataType = wwDefault
        end
        object wwDBEdit7: TwwDBEdit
          Left = 198
          Top = 44
          Width = 60
          Height = 21
          DataField = 'COLUMN_SPACE'
          DataSource = dm.dsrMailingLabels
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit8: TwwDBEdit
          Left = 349
          Top = 44
          Width = 60
          Height = 21
          DataField = 'BAND_HEIGHT'
          DataSource = dm.dsrMailingLabels
          TabOrder = 2
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit3: TwwDBEdit
          Left = 501
          Top = 44
          Width = 60
          Height = 21
          Hint = 'Default = 9'
          DataField = 'FONT_SIZE'
          DataSource = dm.dsrMailingLabels
          TabOrder = 3
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
      end
      object GroupBox9: TGroupBox
        Left = 0
        Top = 83
        Width = 594
        Height = 137
        Align = alClient
        Caption = 'Margins  (mm)'
        TabOrder = 1
        object Label19: TLabel
          Left = 68
          Top = 45
          Width = 53
          Height = 13
          Caption = 'Left Margin'
        end
        object Label16: TLabel
          Left = 263
          Top = 78
          Width = 68
          Height = 13
          Caption = 'Bottom Margin'
        end
        object Label18: TLabel
          Left = 264
          Top = 15
          Width = 54
          Height = 13
          Caption = 'Top Margin'
        end
        object Label17: TLabel
          Left = 461
          Top = 44
          Width = 60
          Height = 13
          Caption = 'Right Margin'
        end
        object wwDBEdit2: TwwDBEdit
          Left = 47
          Top = 65
          Width = 100
          Height = 21
          DataField = 'LEFT_MARGIN'
          DataSource = dm.dsrMailingLabels
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit4: TwwDBEdit
          Left = 247
          Top = 97
          Width = 100
          Height = 21
          DataField = 'BOTTOM_MARGIN'
          DataSource = dm.dsrMailingLabels
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit5: TwwDBEdit
          Left = 246
          Top = 36
          Width = 100
          Height = 21
          DataField = 'TOP_MARGIN'
          DataSource = dm.dsrMailingLabels
          TabOrder = 2
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit6: TwwDBEdit
          Left = 439
          Top = 65
          Width = 100
          Height = 21
          DataField = 'RIGHT_MARGIN'
          DataSource = dm.dsrMailingLabels
          TabOrder = 3
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
      end
    end
    object tsKiosk: TTabSheet
      BorderWidth = 5
      Caption = 'Kiosk Visits'
      ImageIndex = 9
      object wwDBGrid1: TwwDBGrid
        Left = 0
        Top = 100
        Width = 594
        Height = 120
        Selected.Strings = (
          'KIOSK_ID'#9'15'#9'Visit Number'
          'MESSAGE'#9'76'#9'Message')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        Align = alClient
        DataSource = dm.dsrKiosk
        TabOrder = 0
        TitleAlignment = taLeftJustify
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        OnEnter = wwDBGrid1Enter
        IndicatorColor = icBlack
      end
      object GroupBox13: TGroupBox
        Left = 0
        Top = 0
        Width = 594
        Height = 91
        Align = alTop
        Caption = 'Present at Message on Receipt  '
        TabOrder = 1
        object Label21: TLabel
          Left = 26
          Top = 19
          Width = 66
          Height = 13
          Caption = 'Lucky Drawer'
        end
        object Label22: TLabel
          Left = 24
          Top = 43
          Width = 38
          Height = 13
          Caption = 'Birthday'
        end
        object Label23: TLabel
          Left = 24
          Top = 66
          Width = 24
          Height = 13
          Caption = 'Visits'
        end
        object edtLuckyDrawMsg: TwwDBEdit
          Left = 118
          Top = 15
          Width = 441
          Height = 21
          DataField = 'LUCKY_DRAW_MSG'
          DataSource = dm.dsrTblMisc
          TabOrder = 0
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnEnter = edtLuckyDrawMsgEnter
        end
        object wwDBEdit12: TwwDBEdit
          Left = 118
          Top = 39
          Width = 441
          Height = 21
          DataField = 'BIRTHDAY_MSG'
          DataSource = dm.dsrTblMisc
          TabOrder = 1
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object wwDBEdit13: TwwDBEdit
          Left = 118
          Top = 63
          Width = 441
          Height = 21
          DataField = 'VISIT_MSG'
          DataSource = dm.dsrTblMisc
          TabOrder = 2
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
      end
      object Panel19: TPanel
        Left = 0
        Top = 91
        Width = 594
        Height = 9
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
      end
    end
    object tsKioskMsg: TTabSheet
      BorderWidth = 5
      Caption = 'Kiosk Other'
      ImageIndex = 10
      object GroupBox11: TGroupBox
        Left = 312
        Top = 0
        Width = 282
        Height = 220
        Align = alRight
        Caption = 'Lucky Draw, End Time and Ticket Message '
        TabOrder = 0
        object DBCheckBox2: TDBCheckBox
          Left = 56
          Top = 22
          Width = 97
          Height = 17
          Caption = 'Monday'
          DataField = 'MONDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 0
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox3: TDBCheckBox
          Left = 56
          Top = 47
          Width = 97
          Height = 17
          Caption = 'Tuesday'
          DataField = 'TUESDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 1
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox4: TDBCheckBox
          Left = 56
          Top = 71
          Width = 97
          Height = 17
          Caption = 'Wednesday'
          DataField = 'WEDNESDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 2
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox5: TDBCheckBox
          Left = 56
          Top = 94
          Width = 97
          Height = 17
          Caption = 'Thursday'
          DataField = 'THURSDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 3
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox6: TDBCheckBox
          Left = 56
          Top = 119
          Width = 97
          Height = 17
          Caption = 'Friday'
          DataField = 'FRIDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 4
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox7: TDBCheckBox
          Left = 56
          Top = 142
          Width = 89
          Height = 18
          Caption = 'Saturday'
          DataField = 'SATURDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 5
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object DBCheckBox9: TDBCheckBox
          Left = 56
          Top = 166
          Width = 97
          Height = 17
          Caption = 'Sunday'
          DataField = 'SUNDAY_PRINT'
          DataSource = dm.dsrTblMisc
          TabOrder = 6
          ValueChecked = 'T'
          ValueUnchecked = 'F'
        end
        object wwDBEdit9: TwwDBEdit
          Left = 56
          Top = 191
          Width = 194
          Height = 21
          DataField = 'LUCKY_DRAW_TEXT'
          DataSource = dm.dsrTblMisc
          TabOrder = 7
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object dbdtMonday: TwwDBDateTimePicker
          Left = 161
          Top = 20
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'MONDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 8
          UnboundDataType = wwDTEdtTime
        end
        object dbdtTuesday: TwwDBDateTimePicker
          Left = 161
          Top = 44
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'TUESDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 9
          UnboundDataType = wwDTEdtTime
        end
        object dbdtWednesday: TwwDBDateTimePicker
          Left = 161
          Top = 68
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'WEDNESDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 10
          UnboundDataType = wwDTEdtTime
        end
        object dbdtThursday: TwwDBDateTimePicker
          Left = 161
          Top = 92
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'THURSDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 11
          UnboundDataType = wwDTEdtTime
        end
        object dbdtFriday: TwwDBDateTimePicker
          Left = 161
          Top = 116
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'FRIDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 12
          UnboundDataType = wwDTEdtTime
        end
        object dbdtSaturday: TwwDBDateTimePicker
          Left = 161
          Top = 140
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'SATURDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 13
          UnboundDataType = wwDTEdtTime
        end
        object dbdtSunday: TwwDBDateTimePicker
          Left = 161
          Top = 164
          Width = 90
          Height = 21
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          DataField = 'SUNDAY_END_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          ShowButton = True
          TabOrder = 14
          UnboundDataType = wwDTEdtTime
        end
      end
      object Panel16: TPanel
        Left = 301
        Top = 0
        Width = 11
        Height = 220
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 1
      end
      object Panel17: TPanel
        Left = 0
        Top = 0
        Width = 301
        Height = 220
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 2
        object GroupBox10: TGroupBox
          Left = 0
          Top = 0
          Width = 301
          Height = 147
          Align = alTop
          Caption = 'Advertising Message '
          TabOrder = 0
          object btnKiosk: TButton
            Left = 102
            Top = 55
            Width = 87
            Height = 25
            Caption = 'Kiosk Message'
            TabOrder = 0
            OnClick = btnKioskClick
          end
        end
        object Panel18: TPanel
          Left = 0
          Top = 147
          Width = 301
          Height = 9
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
        end
        object GroupBox12: TGroupBox
          Left = 0
          Top = 156
          Width = 301
          Height = 65
          Align = alTop
          Caption = 'Birthday Draw and Ticket Message '
          TabOrder = 2
          object DBCheckBox1: TDBCheckBox
            Left = 15
            Top = 26
            Width = 65
            Height = 17
            Caption = 'Draw'
            DataField = 'BIRTHDAY_PRINT'
            DataSource = dm.dsrTblMisc
            TabOrder = 0
            ValueChecked = 'T'
            ValueUnchecked = 'F'
          end
          object wwDBEdit10: TwwDBEdit
            Left = 88
            Top = 24
            Width = 185
            Height = 21
            DataField = 'BIRTHDAY_TEXT'
            DataSource = dm.dsrTblMisc
            TabOrder = 1
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
          end
        end
      end
    end
    object tsPointsAllocation: TTabSheet
      Caption = 'Points Allocation'
      ImageIndex = 11
      object Panel12: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 45
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object btnStartPointsAllocation: TButton
          Left = 456
          Top = 8
          Width = 75
          Height = 25
          Caption = 'Start'
          TabOrder = 0
          OnClick = btnStartPointsAllocationClick
        end
      end
      object dbgPointsAllocation: TwwDBGrid
        Left = 0
        Top = 45
        Width = 604
        Height = 185
        Selected.Strings = (
          'DATE_TIME'#9'17'#9'Date Time'
          'MEMBER'#9'7'#9'Member'
          'FULL_NAME'#9'20'#9'Full Name'
          'POINTS'#9'8'#9'Points'
          'LAST_POINTS_AVAILABLE'#9'11'#9'Last Pts Avail'
          'DONATION_MEMBER'#9'14'#9'Donation Member'
          'DONATION_POINTS'#9'12'#9'Donation Points')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        Align = alClient
        DataSource = dm.dsrPointsAllocation
        KeyOptions = []
        Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
        TabOrder = 1
        TitleAlignment = taLeftJustify
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        OnCalcCellColors = dbgPointsAllocationCalcCellColors
        IndicatorColor = icBlack
      end
    end
    object tsPreferences: TTabSheet
      Caption = 'Member Preferences'
      ImageIndex = 12
      object Panel22: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 131
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Panel23: TPanel
          Left = 0
          Top = 0
          Width = 305
          Height = 131
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 5
          TabOrder = 0
          object GroupBox15: TGroupBox
            Left = 5
            Top = 5
            Width = 295
            Height = 121
            Align = alClient
            Caption = '1. Preference Heading'
            TabOrder = 0
            object dbgPrefHeading: TwwDBGrid
              Left = 9
              Top = 18
              Width = 278
              Height = 96
              Selected.Strings = (
                'HEADING_DESC'#9'39'#9'Heading Name')
              IniAttributes.Delimiter = ';;'
              TitleColor = clBtnFace
              FixedCols = 0
              ShowHorzScrollBar = True
              DataSource = dm.dsrPrefHeading
              TabOrder = 0
              TitleAlignment = taLeftJustify
              TitleFont.Charset = DEFAULT_CHARSET
              TitleFont.Color = clBlack
              TitleFont.Height = -11
              TitleFont.Name = 'MS Sans Serif'
              TitleFont.Style = []
              TitleLines = 1
              TitleButtons = False
              OnEnter = dbgPrefHeadingEnter
              OnExit = dbgPrefHeadingExit
              IndicatorColor = icBlack
            end
          end
        end
        object Panel24: TPanel
          Left = 305
          Top = 0
          Width = 299
          Height = 131
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 5
          TabOrder = 1
          object GroupBox16: TGroupBox
            Left = 5
            Top = 5
            Width = 289
            Height = 121
            Align = alClient
            Caption = '2. Preference Sub Heading'
            TabOrder = 0
            object Panel26: TPanel
              Left = 2
              Top = 15
              Width = 285
              Height = 104
              Align = alClient
              BevelOuter = bvNone
              BorderWidth = 5
              Caption = 'Panel26'
              TabOrder = 0
              object wwDBGrid2: TwwDBGrid
                Left = 5
                Top = 5
                Width = 275
                Height = 94
                ControlType.Strings = (
                  'DISPLAY_HEADING;CustomEdit;cbxDispHeading')
                Selected.Strings = (
                  'SUB_HEADING_DESC'#9'26'#9'Sub Heading Name'
                  'DISPLAY_HEADING'#9'12'#9'Display Header')
                IniAttributes.Delimiter = ';;'
                TitleColor = clBtnFace
                FixedCols = 0
                ShowHorzScrollBar = True
                Align = alClient
                DataSource = dm.dsrPrefSubHeading
                TabOrder = 0
                TitleAlignment = taLeftJustify
                TitleFont.Charset = DEFAULT_CHARSET
                TitleFont.Color = clBlack
                TitleFont.Height = -11
                TitleFont.Name = 'MS Sans Serif'
                TitleFont.Style = []
                TitleLines = 1
                TitleButtons = False
                OnEnter = dbgPrefHeadingEnter
                OnExit = dbgPrefHeadingExit
                IndicatorColor = icBlack
              end
              object cbxDispHeading: TwwDBComboBox
                Left = 176
                Top = 62
                Width = 49
                Height = 19
                ShowButton = True
                Style = csDropDown
                MapList = True
                AllowClearKey = False
                ShowMatchText = True
                BorderStyle = bsNone
                DataField = 'DISPLAY_HEADING'
                DataSource = dm.dsrPrefSubHeading
                DropDownCount = 8
                Frame.Enabled = True
                Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
                Frame.FocusStyle = efsFrameSunken
                Frame.NonFocusStyle = efsFrameSunken
                ItemHeight = 0
                Items.Strings = (
                  'Yes'#9'Y'
                  'No'#9'N')
                Sorted = False
                TabOrder = 1
                UnboundDataType = wwDefault
              end
            end
          end
        end
      end
      object Panel25: TPanel
        Left = 0
        Top = 131
        Width = 604
        Height = 99
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 1
        object GroupBox17: TGroupBox
          Left = 5
          Top = 5
          Width = 594
          Height = 89
          Align = alClient
          Caption = '3. Preference Detail'
          TabOrder = 0
          object Panel27: TPanel
            Left = 469
            Top = 15
            Width = 123
            Height = 72
            Align = alRight
            BevelOuter = bvNone
            TabOrder = 0
            object Button2: TButton
              Left = 16
              Top = 5
              Width = 87
              Height = 25
              Caption = 'Preview'
              TabOrder = 0
              OnClick = Button2Click
            end
          end
          object Panel28: TPanel
            Left = 2
            Top = 15
            Width = 467
            Height = 72
            Align = alClient
            BevelOuter = bvNone
            BorderWidth = 5
            TabOrder = 1
            object wwDBGrid3: TwwDBGrid
              Left = 5
              Top = 5
              Width = 457
              Height = 62
              ControlType.Strings = (
                'DETAIL_TYPE;CustomEdit;cbxDetailType'
                'DISPLAY_DETAIL;CustomEdit;cbxDisplayDetail')
              Selected.Strings = (
                'DETAIL_DESC'#9'32'#9'Detail Name'#9'F'
                'DETAIL_TYPE'#9'13'#9'Detail Type'#9'F'
                'LINE_NUMBER'#9'10'#9'Line Number'#9'F'
                'DISPLAY_DETAIL'#9'12'#9'Display Detail'#9'F')
              IniAttributes.Delimiter = ';;'
              TitleColor = clBtnFace
              FixedCols = 0
              ShowHorzScrollBar = True
              Align = alClient
              DataSource = dm.dsrPrefDetail
              TabOrder = 0
              TitleAlignment = taLeftJustify
              TitleFont.Charset = DEFAULT_CHARSET
              TitleFont.Color = clBlack
              TitleFont.Height = -11
              TitleFont.Name = 'MS Sans Serif'
              TitleFont.Style = []
              TitleLines = 1
              TitleButtons = False
              OnEnter = dbgPrefHeadingEnter
              OnExit = dbgPrefHeadingExit
              IndicatorColor = icBlack
            end
            object cbxDetailType: TwwDBComboBox
              Left = 136
              Top = 29
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDown
              MapList = True
              AllowClearKey = False
              BorderStyle = bsNone
              DataField = 'DETAIL_TYPE'
              DataSource = dm.dsrPrefDetail
              DropDownCount = 8
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              ItemHeight = 0
              Items.Strings = (
                'Check Box'#9'C'
                'Number'#9'N'
                'Text'#9'T'
                'Yes/No'#9'Y')
              Sorted = False
              TabOrder = 1
              UnboundDataType = wwDefault
            end
            object cbxDisplayDetail: TwwDBComboBox
              Left = 240
              Top = 29
              Width = 49
              Height = 19
              ShowButton = True
              Style = csDropDown
              MapList = True
              AllowClearKey = False
              ShowMatchText = True
              BorderStyle = bsNone
              DataField = 'DISPLAY_HEADING'
              DataSource = dm.dsrPrefSubHeading
              DropDownCount = 8
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              ItemHeight = 0
              Items.Strings = (
                'Yes'#9'Y'
                'No'#9'N')
              Sorted = False
              TabOrder = 2
              UnboundDataType = wwDefault
            end
          end
        end
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 612
    inherited ToolBar: TToolBar
      Width = 595
      ButtonHeight = 43
      ButtonWidth = 42
      inherited pnlFill: TPanel
        Height = 43
      end
      object dbnLinks: TwwDBNavigator
        Left = 3
        Top = 0
        Width = 254
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
        object dbnLinksInsert: TwwNavButton
          Left = 0
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Insert new record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'Add'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnLinksInsertClick
          Index = 0
          Style = nbsInsert
        end
        object dbnLinksEdit: TwwNavButton
          Left = 40
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Edit current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'Edit'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnLinksInsertClick
          Index = 1
          Style = nbsEdit
        end
        object dbnLinksDelete: TwwNavButton
          Left = 80
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Delete current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'Delete'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnLinksInsertClick
          Index = 2
          Style = nbsDelete
        end
        object dbnLinksPost: TwwNavButton
          Left = 120
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
          OnClick = dbnLinksInsertClick
          Index = 3
          Style = nbsPost
        end
        object dbnLinksCancel: TwwNavButton
          Left = 160
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
          OnClick = dbnLinksInsertClick
          Index = 4
          Style = nbsCancel
        end
        object dbnLinksRefresh: TwwNavButton
          Left = 200
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
          OnClick = dbnLinksInsertClick
          Index = 5
          Style = nbsRefresh
        end
      end
      object pnlSpace: TPanel
        Left = 257
        Top = 0
        Width = 288
        Height = 43
        BevelOuter = bvNone
        TabOrder = 2
        object lblPoints: TLabel
          Left = 66
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
        object pnlFind: TPanel
          Left = 0
          Top = 0
          Width = 288
          Height = 43
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          Visible = False
          object Label10: TLabel
            Left = 27
            Top = 15
            Width = 20
            Height = 13
            Caption = 'Find'
          end
          object IncSearch: TwwIncrementalSearch
            Left = 69
            Top = 12
            Width = 97
            Height = 21
            DataSource = dm.dsrClubs
            SearchField = 'DESCRIPTION'
            TabOrder = 0
          end
        end
      end
      object btnClose: TToolButton
        Left = 545
        Top = 0
        Hint = 'Close this Screen'
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 490
    Top = 10
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuOptions: TMenuItem [1]
      Caption = 'Options'
      object menuLogonBonusPoints: TMenuItem
        Caption = 'Logon Bonus Points'
        OnClick = menuLogonBonusPointsClick
      end
      object menuLogoffBonusPoints: TMenuItem
        Caption = 'Logoff Bonus Points'
        OnClick = menuLogoffBonusPointsClick
      end
    end
  end
end
