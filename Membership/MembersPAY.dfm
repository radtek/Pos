inherited frmMembersPAY: TfrmMembersPAY
  Tag = 1
  Left = 230
  Top = 72
  Caption = 'frmMembersPAY'
  ClientHeight = 459
  ClientWidth = 673
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 439
    Width = 673
    Panels = <
      item
        Width = 120
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
  end
  object PageControl: TPageControl [1]
    Left = 0
    Top = 60
    Width = 673
    Height = 379
    ActivePage = tsSubsPay
    Align = alClient
    MultiLine = True
    TabOrder = 2
    OnChange = PageControlChange
    OnChanging = PageControlChanging
    object tsSubsPay: TTabSheet
      Caption = 'Pay by &Member'
      object lblMemberExpiry: TLabel
        Left = 225
        Top = 332
        Width = 263
        Height = 14
        AutoSize = False
      end
      object Splitter2: TSplitter
        Left = 0
        Top = 138
        Width = 665
        Height = 3
        Cursor = crVSplit
        Align = alBottom
        Beveled = True
      end
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 665
        Height = 49
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Label10: TLabel
          Left = 158
          Top = 19
          Width = 36
          Height = 13
          Caption = 'Paid By'
        end
        object Label7: TLabel
          Left = 306
          Top = 19
          Width = 28
          Height = 13
          Caption = 'Name'
        end
        object Label1: TLabel
          Left = 480
          Top = 20
          Width = 67
          Height = 13
          Caption = 'Payment Date'
        end
        object Label21: TLabel
          Left = 7
          Top = 19
          Width = 20
          Height = 13
          Caption = 'Find'
        end
        object dblcSurname: TwwDBLookupCombo
          Tag = 99
          Left = 38
          Top = 17
          Width = 110
          Height = 19
          Hint = 
            'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
            ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'9'#9'Member'#9'F'
            'SURNAME'#9'12'#9'Surname'#9'F'
            'FULL_NAME'#9'15'#9'Full Name'#9'F'
            'FIRST_NAME'#9'12'#9'First Name'#9'F'
            'ADDRESS'#9'15'#9'Address'#9'F')
          LookupTable = dm.qryMembersPAY
          LookupField = 'MEMBER'
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
          ShowMatchText = True
          OnDropDown = dblcSurnameDropDown
          OnCloseUp = dblcSurnameCloseUp
          OnKeyPress = dblcSurnameKeyPress
        end
        object dblcPaidBy: TwwDBLookupCombo
          Tag = 99
          Left = 202
          Top = 17
          Width = 92
          Height = 19
          Hint = 
            'Which Member is going to pay - Available Points belong to this M' +
            'ember'
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'8'#9'Member'#9'F'
            'SURNAME'#9'10'#9'Surname'#9'F'
            'FULL_NAME'#9'12'#9'Full Name'#9'F'
            'FIRST_NAME'#9'10'#9'First Name'#9'F'
            'ADDRESS'#9'12'#9'Address'#9'F')
          DataField = 'MEMBER'
          DataSource = dm.dsrPayMembers
          LookupTable = dm.qryMembersPAY
          LookupField = 'MEMBER'
          Options = [loColLines, loRowLines, loTitles]
          Color = clWhite
          DropDownCount = 10
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 1
          AutoDropDown = False
          ShowButton = True
          AllowClearKey = False
          ShowMatchText = True
          OnDropDown = dblcSurnameDropDown
          OnCloseUp = dblcPaidByCloseUp
          OnKeyPress = dblcPaidByKeyPress
          OnKeyUp = dblcPaidByKeyUp
        end
        object dbeFullName: TwwDBEdit
          Tag = 1
          Left = 343
          Top = 17
          Width = 128
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'FULL_NAME'
          DataSource = dm.dsrPayMembers
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
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbdtPayDate: TwwDBDateTimePicker
          Left = 555
          Top = 17
          Width = 94
          Height = 19
          Hint = 
            'Type in a date or '#13#10'select the down arrow to show a calender'#13#10'Pr' +
            'ess Month or Year for a quick selection '
          BorderStyle = bsNone
          CalendarAttributes.Colors.TextColor = clBlack
          CalendarAttributes.Colors.TitleBackColor = clInactiveCaptionText
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
          TabOrder = 3
          UnboundDataType = wwDTEdtDate
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dtpDateFromChange
          OnExit = dtpDateFromExit
        end
        object RichEdit: TRichEdit
          Left = 504
          Top = 3
          Width = 13
          Height = 17
          Lines.Strings = (
            'R'
            'i'
            'c'
            'h'
            'E'
            'd'
            'it')
          PlainText = True
          TabOrder = 4
          Visible = False
        end
      end
      object pnlSubs: TPanel
        Left = 0
        Top = 49
        Width = 665
        Height = 89
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 1
        object dbgSubPay: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 79
          Hint = 'key in a value to PAY and press Enter'
          ControlType.Strings = (
            'PRO_RATA;CheckBox;Y;N'
            'INVOICED;CheckBox;Y;N'
            'PRINT_RECEIPT;CheckBox;Y;N'
            'PRINT_CARD;CheckBox;Y;N'
            'DISCOUNTABLE;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'7'#9'Member'
            'SUBSCRIPTION'#9'6'#9'Code'
            'DESCRIPTION'#9'32'#9'Subscription'
            'EXPIRES'#9'13'#9'Expires'
            'AMOUNT_PAID'#9'10'#9'Amt Paid'
            'AMOUNT'#9'9'#9'Fee'
            'PAY'#9'9'#9'PAY'
            'PRINT_CARD'#9'4'#9'Card'
            'PRINT_RECEIPT'#9'4'#9'Rcpt'
            'INVOICED'#9'7'#9'Invoiced'
            'PRO_RATA'#9'7'#9'Pro Rata'
            'DISCOUNTABLE'#9'3'#9'Disc'
            'DATE_OF_BIRTH'#9'10'#9'DOB'
            'ADDRESS'#9'30'#9'ADDRESS'
            'ADDRESS1'#9'30'#9'ADDRESS1'
            'DISTRICT'#9'20'#9'DISTRICT'
            'CITY'#9'25'#9'CITY'
            'COUNTRY'#9'25'#9'COUNTRY'
            'SPARE_CHAR'#9'1'#9'SPARE_CHAR')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          OnRowChanged = dbgSubPayRowChanged
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPayDet
          KeyOptions = [dgEnterToTab]
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgTabExitsOnLastCol]
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgSubPayCalcCellColors
          OnDblClick = dbgSubPayDblClick
          OnEnter = dbgSubPayEnter
          OnExit = dbgSubPayExit
          OnKeyPress = dbgSubPayKeyPress
          IndicatorColor = icBlack
          OnFieldChanged = dbgSubPayFieldChanged
          object dbgSubPayButton: TwwIButton
            Left = 0
            Top = 0
            Width = 13
            Height = 22
            AllowAllUp = True
            Glyph.Data = {
              F6000000424DF600000000000000760000002800000010000000100000000100
              0400000000008000000000000000000000001000000000000000000000000000
              8000008000000080800080000000800080008080000080808000C0C0C0000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
              8008088888888888888800888888888880080008888888888008F00088888888
              88000F0000000788080080F00788870800008800788FF8708008880788888F87
              78888808888888F808888807888888F8088888078FF88888088888777FF88887
              7888888077888870888888880777770888888888870007888888}
            OnClick = dbgSubPayButtonClick
          end
        end
      end
      object pnlSections: TPanel
        Left = 0
        Top = 141
        Width = 665
        Height = 87
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 2
        object dbgPaySecMem: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 77
          Hint = 'key in a value to PAY and press Enter'
          ControlType.Strings = (
            'PREPAY;CheckBox;Y;N ')
          Selected.Strings = (
            'MEMBER'#9'9'#9'Member'
            'SECTION'#9'8'#9'Code'#9'F'
            'DESCRIPTION'#9'33'#9'Section'#9'F'
            'EXPIRES'#9'14'#9'Expires'
            'AMOUNT_PAID'#9'10'#9'Amt Paid'#9'F'
            'AMOUNT'#9'10'#9'Fee'#9'F'
            'PAY'#9'11'#9'PAY'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPayMemSec
          KeyOptions = [dgEnterToTab]
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgTabExitsOnLastCol]
          ParentShowHint = False
          ShowHint = False
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
          OnCalcCellColors = dbgPaySecMemCalcCellColors
          OnDblClick = dbgPaySecMemDblClick
          OnEnter = dbgSubPayEnter
          OnExit = dbgSubPayExit
          OnKeyPress = dbgSubPayKeyPress
          IndicatorColor = icBlack
          OnFieldChanged = dbgPaySecMemFieldChanged
          object btndbgPayMemSec: TwwIButton
            Tag = 1
            Left = 0
            Top = 0
            Width = 13
            Height = 21
            Hint = 'Show the selected Members Details'
            AllowAllUp = True
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Glyph.Data = {
              F6000000424DF600000000000000760000002800000010000000100000000100
              0400000000008000000000000000000000001000000000000000000000000000
              8000008000000080800080000000800080008080000080808000C0C0C0000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
              8008088888888888888800888888888880080008888888888008F00088888888
              88000F0000000788080080F00788870800008800788FF8708008880788888F87
              78888808888888F808888807888888F8088888078FF88888088888777FF88887
              7888888077888870888888880777770888888888870007888888}
            ParentFont = False
            OnClick = dbgSubPayButtonClick
          end
        end
      end
      object pnlJoinFee: TPanel
        Left = 0
        Top = 228
        Width = 665
        Height = 51
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 3
        object dbgJoinPay: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 41
          Hint = 'key in a value to PAY and press Enter'
          ControlType.Strings = (
            'Pay;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'9'#9'Member'#9'F'
            'JOINING_FEE'#9'13'#9'Joining Fee'#9'F'
            'PAY'#9'15'#9'PAY'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm, ecoDisableCustomControls, ecoDisableDateTimePicker]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPayJoin
          KeyOptions = []
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgTabExitsOnLastCol]
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgJoinPayCalcCellColors
          OnColEnter = dbgSubPayEnter
          OnEnter = dbgSubPayEnter
          OnExit = dbgSubPayExit
          OnKeyPress = dbgSubPayKeyPress
          IndicatorColor = icBlack
          OnFieldChanged = dbgSubPayFieldChanged
        end
      end
      object Panel11: TPanel
        Left = 0
        Top = 279
        Width = 665
        Height = 72
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 4
        object Label3: TLabel
          Left = 167
          Top = 12
          Width = 77
          Height = 13
          Caption = 'Joining Fee Due'
        end
        object Label5: TLabel
          Left = 351
          Top = 13
          Width = 47
          Height = 13
          Caption = 'Subs Due'
        end
        object Label2: TLabel
          Left = 351
          Top = 41
          Width = 64
          Height = 13
          Caption = 'Sections Due'
        end
        object lblToPay: TLabel
          Left = 504
          Top = 40
          Width = 68
          Height = 13
          Hint = 
            'Joining Fee Due + Subs Due + Sections Due  ( -/+  Available Poin' +
            'ts )  '
          Caption = 'Total Payment'
        end
        object Label18: TLabel
          Left = 507
          Top = 14
          Width = 42
          Height = 13
          Caption = 'Discount'
        end
        object Label19: TLabel
          Left = 12
          Top = 11
          Width = 42
          Height = 13
          Hint = 'For New Members - divide the Fee by the remaining months  '
          Caption = 'Pro Rata'
        end
        object Label20: TLabel
          Left = 165
          Top = 39
          Width = 88
          Height = 13
          Hint = 'Type in the points as a dollar value'
          Caption = 'Pay by Points ( $c)'
        end
        object Label22: TLabel
          Left = 14
          Top = 35
          Width = 56
          Height = 29
          Hint = 'For New Members - divide the Fee by the remaining months  '
          AutoSize = False
          Caption = 'Points Available'
          WordWrap = True
        end
        object DBText1: TDBText
          Left = 64
          Top = 38
          Width = 65
          Height = 17
          Hint = 'Double Click to assign the full amount'
          Alignment = taRightJustify
          DataField = 'POINTS_AVAILABLE'
          DataSource = dm.dsrPayMembers
          OnDblClick = DBText1DblClick
        end
        object dbeJoiningFeeDue: TwwDBEdit
          Tag = 1
          Left = 262
          Top = 9
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 0
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeSubsDue: TwwDBEdit
          Tag = 1
          Left = 425
          Top = 9
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 1
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeSectionDue: TwwDBEdit
          Tag = 1
          Left = 425
          Top = 37
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 2
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeTotal: TwwDBEdit
          Left = 580
          Top = 37
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 3
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeDiscount: TwwDBEdit
          Tag = 1
          Left = 580
          Top = 9
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          TabOrder = 4
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnExit = dbeDiscountExit
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeProRata: TwwDBComboBox
          Tag = 88
          Left = 64
          Top = 10
          Width = 65
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = False
          AllowClearKey = False
          AutoSize = False
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
          ItemIndex = 11
          Sorted = False
          TabOrder = 5
          UnboundDataType = wwDefault
          OnCloseUp = dbeProRataCloseUp
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbePartPay: TwwDBComboBox
          Tag = 88
          Left = 184
          Top = 53
          Width = 65
          Height = 19
          HelpContext = 5
          ShowButton = True
          Style = csDropDownList
          MapList = False
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 3
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
          ItemIndex = 0
          Sorted = False
          TabOrder = 6
          UnboundDataType = wwDefault
          Visible = False
          OnCloseUp = dbeProRataCloseUp
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbePoints: TwwDBEdit
          Left = 262
          Top = 38
          Width = 65
          Height = 21
          Picture.PictureMask = '#[#][#][#][.][#][#]'
          TabOrder = 7
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
      end
    end
    object tsSectionsPay: TTabSheet
      Caption = 'Pay by Sect&ion'
      ImageIndex = 1
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 665
        Height = 47
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Label15: TLabel
          Left = 342
          Top = 19
          Width = 20
          Height = 13
          Caption = '&Find'
        end
        object dblcSections: TwwDBLookupCombo
          Left = 20
          Top = 16
          Width = 106
          Height = 19
          Hint = 'Add an entire Section for Payment '
          TabStop = False
          AutoSize = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'SECTION'#9'6'#9'Code'#9'F'
            'DESCRIPTION'#9'25'#9'Description'#9'F'
            'EXPIRES'#9'18'#9'Expires'#9'F')
          LookupTable = dm.tblSections
          LookupField = 'SECTION'
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
          UseTFields = False
          AllowClearKey = False
          ShowMatchText = True
          OnCloseUp = dblcSectionsCloseUp
          OnKeyPress = dblcSectionsKeyPress
        end
        object dbcbOptions: TwwDBComboBox
          Left = 186
          Top = 16
          Width = 103
          Height = 19
          Hint = 'Choose from All , Unpaid or Paid Members'
          TabStop = False
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 3
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'All Members'
            'UnPaid Members'
            'Paid Members')
          ItemIndex = 0
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
          OnChange = dbcbOptionsChange
        end
        object incSearch: TwwIncrementalSearch
          Left = 382
          Top = 17
          Width = 105
          Height = 19
          Hint = 'Search by a Member Number or Surname'
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          DataSource = dm.dsrPaySections
          SearchField = 'MEMBER'
          BorderStyle = bsNone
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnExit = incSearchExit
          OnKeyPress = incSearchKeyPress
        end
        object btnMembers: TRadioButton
          Left = 535
          Top = 9
          Width = 81
          Height = 17
          Hint = 'Sort by the Member Number'
          Caption = 'Members'
          Checked = True
          TabOrder = 3
          TabStop = True
          OnClick = btnMembersClick
        end
        object btnSurname: TRadioButton
          Left = 535
          Top = 27
          Width = 81
          Height = 17
          Hint = 'Sort by the Members Surname '
          Caption = 'Surname'
          TabOrder = 4
          OnClick = btnMembersClick
        end
      end
      object Panel5: TPanel
        Left = 0
        Top = 278
        Width = 665
        Height = 73
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 1
        object Label13: TLabel
          Left = 83
          Top = 14
          Width = 36
          Height = 13
          Caption = 'Paid By'
        end
        object Label12: TLabel
          Left = 377
          Top = 14
          Width = 64
          Height = 13
          Caption = 'Sections Due'
        end
        object Label14: TLabel
          Left = 377
          Top = 45
          Width = 68
          Height = 13
          Hint = 'Sections Due  ( -/+  Available Points )  '
          Caption = 'Total Payment'
        end
        object dblcSectionPaidBy: TwwDBLookupCombo
          Left = 155
          Top = 11
          Width = 106
          Height = 19
          Hint = 
            'Which Member is going to pay - Available Points belong to this M' +
            'ember'
          TabStop = False
          CharCase = ecUpperCase
          BorderStyle = bsNone
          DropDownAlignment = taLeftJustify
          Selected.Strings = (
            'MEMBER'#9'10'#9'MEMBER'#9'F'
            'FIRST_NAME'#9'12'#9'FIRST_NAME'#9'F'
            'SURNAME'#9'15'#9'SURNAME'#9'F'
            'FULL_NAME'#9'25'#9'FULL_NAME'#9'F'
            'ADDRESS'#9'25'#9'ADDRESS'#9'F')
          LookupTable = dm.qryMembersPAY
          LookupField = 'MEMBER'
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
          UseTFields = False
          AllowClearKey = False
          OnDropDown = dblcSurnameDropDown
          OnCloseUp = dblcSectionPaidByCloseUp
          OnKeyPress = dblcSectionPaidByKeyPress
        end
        object dbeSectionsDue: TwwDBEdit
          Tag = 1
          Left = 469
          Top = 11
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'AMOUNT_DUE'
          DataSource = dm.dsrPaySections
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 1
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object cbxUsePointsS: TCheckBox
          Left = 61
          Top = 43
          Width = 129
          Height = 17
          Hint = 
            'Click in the Box to use Available Points and reduce the Total Pa' +
            'yment'
          Caption = 'Use Available Points'
          TabOrder = 2
          OnClick = cbxUsePointsSClick
        end
        object dbePointsS: TwwDBEdit
          Tag = 1
          Left = 194
          Top = 42
          Width = 65
          Height = 19
          TabStop = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          DataField = 'POINTS_AVAILABLE'
          DataSource = dm.dsrPaySecMem
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 3
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
          OnKeyPress = dbeFullNameKeyPress
        end
        object dbeTotalSection: TwwDBEdit
          Left = 469
          Top = 42
          Width = 65
          Height = 19
          Hint = 'Sections Due  ( -/+  Available Points )  '
          BorderStyle = bsNone
          Color = clWhite
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ReadOnly = True
          TabOrder = 4
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
        end
      end
      object Panel6: TPanel
        Left = 0
        Top = 47
        Width = 665
        Height = 231
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel6'
        TabOrder = 2
        object dbgSectionsPay: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 221
          Hint = 
            'Select Paid By, Points or Tender before Tick, Cash - tick PAY or' +
            ' Double Click '
          ControlType.Strings = (
            'PAY;CheckBox;Y;N'
            'PAID;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'7'#9'Member'#9'F'
            'SURNAME'#9'13'#9'Surname'#9'F'
            'FULL_NAME'#9'25'#9'Full Name'#9'F'
            'EXPIRES'#9'14'#9'Expires'#9'F'
            'AMOUNT_PAID'#9'9'#9'Amt Paid'#9'F'
            'AMOUNT'#9'8'#9'Amount'#9'F'
            'PAID'#9'5'#9'PAY'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          OnRowChanged = dbgSectionsPayRowChanged
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPaySections
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          KeyOptions = []
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgShowFooter]
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgSectionsPayCalcCellColors
          OnDblClick = dbgSectionsPayDblClick
          IndicatorColor = icBlack
          OnFieldChanged = dbgSectionsPayFieldChanged
          object dbgSectionsPayIButton: TwwIButton
            Tag = 2
            Left = 0
            Top = 0
            Width = 13
            Height = 22
            AllowAllUp = True
            Glyph.Data = {
              F6000000424DF600000000000000760000002800000010000000100000000100
              0400000000008000000000000000000000001000000000000000000000000000
              8000008000000080800080000000800080008080000080808000C0C0C0000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
              8008088888888888888800888888888880080008888888888008F00088888888
              88000F0000000788080080F00788870800008800788FF8708008880788888F87
              78888808888888F808888807888888F8088888078FF88888088888777FF88887
              7888888077888870888888880777770888888888870007888888}
            OnClick = dbgSubPayButtonClick
          end
        end
      end
    end
    object tsReprint: TTabSheet
      Caption = 'Receipts'
      ImageIndex = 2
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 665
        Height = 49
        Align = alTop
        BevelOuter = bvNone
        Caption = 'pnlReprint1'
        TabOrder = 0
        object pnlReprint: TPanel
          Left = 0
          Top = 0
          Width = 665
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label16: TLabel
            Left = 392
            Top = 18
            Width = 35
            Height = 13
            Caption = 'or Date'
            WordWrap = True
          end
          object Label6: TLabel
            Left = 197
            Top = 18
            Width = 59
            Height = 26
            Caption = 'or Member     '
            WordWrap = True
          end
          object Label8: TLabel
            Left = 16
            Top = 18
            Width = 47
            Height = 13
            Caption = 'Receipt #'
          end
          object dtPicker: TwwDBDateTimePicker
            Left = 441
            Top = 15
            Width = 108
            Height = 19
            Hint = 
              'Select a date and press Enter - or select no date to see all rec' +
              'eipts'
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
            DisplayFormat = 'dd mmm yyyy'
            OnChange = dtpDateFromChange
            OnExit = dtpDateFromExit
            OnKeyPress = dtPickerKeyPress
          end
          object dblcSearch: TwwDBLookupCombo
            Tag = 99
            Left = 261
            Top = 15
            Width = 110
            Height = 19
            Hint = 
              'Key in a '#39'Member Number'#39' and press Enter'#13#10'or '#39'Surname'#39' and press' +
              ' the DownArrow '#13#10'or 1 then '#39'First Name'#39' and press the DownArrow '
            CharCase = ecUpperCase
            BorderStyle = bsNone
            DropDownAlignment = taLeftJustify
            Selected.Strings = (
              'MEMBER'#9'9'#9'Member'#9'F'
              'SURNAME'#9'12'#9'Surname'#9'F'
              'FULL_NAME'#9'15'#9'Full Name'#9'F'
              'FIRST_NAME'#9'12'#9'First Name'#9'F'
              'ADDRESS'#9'15'#9'Address'#9'F')
            LookupTable = dm.qryMembersPAY
            LookupField = 'MEMBER'
            Options = [loColLines, loRowLines, loTitles]
            AutoSelect = False
            Color = clWhite
            DropDownCount = 10
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clHighlight
            TabOrder = 1
            AutoDropDown = False
            ShowButton = True
            AllowClearKey = False
            ShowMatchText = True
            OnDropDown = dblcSurnameDropDown
            OnCloseUp = dblcSearchCloseUp
            OnKeyPress = dblcSearchKeyPress
          end
          object dbeReceipt: TwwDBEdit
            Left = 79
            Top = 15
            Width = 94
            Height = 19
            Hint = 'Enter a Receipt Number'
            BorderStyle = bsNone
            Color = clWhite
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clHighlight
            Picture.PictureMask = '#*#'
            TabOrder = 2
            UnboundDataType = wwDefault
            WantReturns = False
            WordWrap = False
            OnKeyPress = dbeReceiptKeyPress
          end
          object btnSaveReceipts: TButton
            Left = 564
            Top = 13
            Width = 87
            Height = 25
            Hint = 'Save the changes to Receipts'
            Caption = 'Save Receipts'
            TabOrder = 3
            OnClick = btnSaveReceiptsClick
          end
        end
      end
      object Panel1: TPanel
        Left = 0
        Top = 195
        Width = 665
        Height = 156
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel1'
        TabOrder = 1
        object dbgPayments_Det: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 146
          Hint = 'Payment details'#13#10'Double Click to view Tender details'
          TabStop = False
          Selected.Strings = (
            'PAYMENT'#9'7'#9'Receipt'
            'MEMBER'#9'9'#9'Member'
            'FULL_NAME'#9'21'#9'Full Name'
            'DESCRIPTION'#9'19'#9'Description'
            'EXPIRES'#9'10'#9'Expires'
            'AMOUNT'#9'10'#9'Fee'
            'AMOUNT_PAID'#9'10'#9'Amt Paid'
            'PAY_DATE'#9'10'#9'Date Paid'
            'SPONSORSHIP'#9'5'#9'S/Ship')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPay_Det
          KeyOptions = []
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgPayments_DetCalcCellColors
          OnDblClick = dbgPaymentsDblClick
          IndicatorColor = icBlack
          OnUpdateFooter = dbgPayments_DetUpdateFooter
          object dbgPayments_DetIButton: TwwIButton
            Tag = 3
            Left = 0
            Top = 0
            Width = 16
            Height = 22
            AllowAllUp = True
            Glyph.Data = {
              F6000000424DF600000000000000760000002800000010000000100000000100
              0400000000008000000000000000000000001000000000000000000000000000
              8000008000000080800080000000800080008080000080808000C0C0C0000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
              8008088888888888888800888888888880080008888888888008F00088888888
              88000F0000000788080080F00788870800008800788FF8708008880788888F87
              78888808888888F808888807888888F8088888078FF88888088888777FF88887
              7888888077888870888888880777770888888888870007888888}
            OnClick = dbgSubPayButtonClick
          end
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 49
        Width = 665
        Height = 146
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel3'
        TabOrder = 2
        object dbgPayments: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 136
          Hint = 
            'Select a Payment and the details are shown below'#13#10'Double Click t' +
            'o view Tender details'
          TabStop = False
          Selected.Strings = (
            'PAYMENT'#9'7'#9'Receipt'
            'PAID_BY'#9'11'#9'Paid By'
            'FULL_NAME'#9'23'#9'Full Name'
            'PAY_AMOUNT'#9'12'#9'Total Paid'
            'POINTS_USED'#9'10'#9'Points Used'
            'ROUNDING'#9'9'#9'Rounding'
            'EDIT_DATE'#9'15'#9'Date Paid'
            'DISCOUNT'#9'7'#9'Discount')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrPayments
          KeyOptions = []
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgShowFooter]
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgPaymentsCalcCellColors
          OnDblClick = dbgPaymentsDblClick
          IndicatorColor = icBlack
          object dbgPaymentsIButton: TwwIButton
            Left = 0
            Top = 0
            Width = 13
            Height = 22
            AllowAllUp = True
          end
        end
      end
    end
    object tsBulkCardReceipt: TTabSheet
      Caption = 'Bulk Card/Receipt'
      ImageIndex = 4
      object Panel12: TPanel
        Left = 0
        Top = 0
        Width = 665
        Height = 92
        Align = alTop
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 0
        object GroupBox2: TGroupBox
          Left = 5
          Top = 5
          Width = 107
          Height = 82
          Align = alLeft
          Caption = 'Payments by '
          TabOrder = 0
          object rgpBulk: TLMDRadioGroup
            Left = 2
            Top = 15
            Width = 103
            Height = 65
            Align = alClient
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
              'Payment'
              'Member')
            ItemOffset = 10
            TabOrder = 0
            ItemIndex = 0
          end
        end
        object GroupBox3: TGroupBox
          Left = 123
          Top = 5
          Width = 171
          Height = 82
          Align = alLeft
          Caption = 'Dates '
          TabOrder = 1
          object Label9: TLabel
            Left = 12
            Top = 22
            Width = 23
            Height = 13
            Caption = 'From'
            WordWrap = True
          end
          object Label17: TLabel
            Left = 17
            Top = 52
            Width = 13
            Height = 13
            Caption = 'To'
            WordWrap = True
          end
          object dtpDateFrom: TwwDBDateTimePicker
            Left = 44
            Top = 20
            Width = 108
            Height = 19
            Hint = 
              'Select a date and press Enter - or select no date to see all rec' +
              'eipts'
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
            Epoch = 1950
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ShowButton = True
            TabOrder = 0
            DisplayFormat = 'dd mmm yyyy'
            OnChange = dtpDateFromChange
            OnExit = dtpDateFromExit
            OnKeyPress = dtpDateFromKeyPress
          end
          object dtpDateTo: TwwDBDateTimePicker
            Left = 43
            Top = 49
            Width = 108
            Height = 19
            Hint = 
              'Select a date and press Enter - or select no date to see all rec' +
              'eipts'
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
            Epoch = 1950
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            ShowButton = True
            TabOrder = 1
            DisplayFormat = 'dd mmm yyyy'
            OnChange = dtpDateFromChange
            OnExit = dtpDateFromExit
            OnKeyPress = dtpDateToKeyPress
          end
        end
        object Panel17: TPanel
          Left = 112
          Top = 5
          Width = 11
          Height = 82
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 2
        end
        object Panel19: TPanel
          Left = 305
          Top = 5
          Width = 355
          Height = 82
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 3
          object GroupBox6: TGroupBox
            Left = 0
            Top = 0
            Width = 355
            Height = 82
            Align = alClient
            Caption = 'Options '
            TabOrder = 0
            object btnExportCards: TButton
              Left = 220
              Top = 15
              Width = 87
              Height = 25
              Hint = 'Save Cards and export/print  later'
              Caption = 'Save Cards'
              TabOrder = 0
              OnClick = btnExportCardsClick
            end
            object btnExportReceipts: TButton
              Left = 55
              Top = 14
              Width = 87
              Height = 25
              Hint = 'Export Receipts to a comma delimited file '
              Caption = 'Export Receipts'
              TabOrder = 1
              OnClick = btnExportReceiptsClick
            end
            object btnPrintCards: TButton
              Left = 221
              Top = 49
              Width = 87
              Height = 25
              Hint = 'Print Cards now'
              Caption = 'Print Cards'
              TabOrder = 2
              OnClick = btnPrintCardsClick
            end
            object btnExportCrds: TButton
              Left = 55
              Top = 49
              Width = 87
              Height = 25
              Hint = 'Export Cards to a comma delimited file '
              Caption = 'Export Cards'
              TabOrder = 3
              OnClick = btnExportReceiptsClick
            end
          end
        end
        object Panel14: TPanel
          Left = 294
          Top = 5
          Width = 11
          Height = 82
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 4
        end
      end
      object Panel13: TPanel
        Left = 0
        Top = 92
        Width = 665
        Height = 259
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel6'
        TabOrder = 1
        object dbgBulk: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 249
          ControlType.Strings = (
            'PAY;CheckBox;Y;N'
            'PAID;CheckBox;Y;N'
            'PRINT_CARD;CheckBox;Y;N'
            'PRINT_RECEIPT;CheckBox;Y;N')
          Selected.Strings = (
            'PAYMENT'#9'6'#9'Receipt'
            'PRINT_RECEIPT'#9'3'#9'Rcpt'
            'PRINT_CARD'#9'3'#9'Card'#9'F'
            'NUMBER_CARDS'#9'5'#9'Card #'
            'BARCODE_NUMBER'#9'11'#9'BarCode #'
            'PAY_DATE'#9'12'#9'Date Paid'
            'EXPIRES'#9'11'#9'Expires'
            'MEMBER'#9'7'#9'Member'
            'FULL_NAME'#9'15'#9'Full Name'
            'AMOUNT_PAID'#9'10'#9'Paid'
            'ADDRESS1'#9'30'#9'ADDRESS1'
            'DESCRIPTION'#9'10'#9'Subscription'
            'PAYMENT_PERIOD'#9'14'#9'Pay Period'
            'AUTO_PAYMENT'#9'12'#9'Auto Payment'
            'FIRST_NAME'#9'20'#9'FIRST_NAME'
            'SURNAME'#9'25'#9'SURNAME'
            'TENDER_TYPE'#9'20'#9'TENDER_TYPE'
            'ADDRESS'#9'30'#9'ADDRESS'
            'DISTRICT'#9'20'#9'DISTRICT'
            'CITY'#9'25'#9'CITY'
            'COUNTRY'#9'25'#9'COUNTRY')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrBuldCardRec
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          KeyOptions = []
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit]
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
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
          OnUpdateFooter = dbgBulkUpdateFooter
        end
      end
    end
    object tsAutoPay: TTabSheet
      Caption = 'Direct Debit'
      ImageIndex = 4
      object Panel15: TPanel
        Left = 0
        Top = 0
        Width = 665
        Height = 65
        Align = alTop
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 0
        object GroupBox4: TGroupBox
          Left = 178
          Top = 5
          Width = 482
          Height = 55
          Align = alClient
          TabOrder = 0
          object Label23: TLabel
            Left = 235
            Top = 24
            Width = 67
            Height = 13
            Caption = 'Payment Date'
          end
          object Label4: TLabel
            Left = 27
            Top = 24
            Width = 20
            Height = 13
            Caption = 'Find'
          end
          object dbdtSetPayDay: TwwDBDateTimePicker
            Left = 319
            Top = 21
            Width = 108
            Height = 19
            Hint = 
              'Select a date and press Enter - or select no date to see all rec' +
              'eipts'
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
            DisplayFormat = 'dd mmm yyyy'
            OnChange = dtpDateFromChange
            OnExit = dtpDateFromExit
          end
          object incAutoPayments: TwwIncrementalSearch
            Left = 79
            Top = 21
            Width = 101
            Height = 19
            Hint = 'Search for a '
            Frame.Enabled = True
            Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            Frame.FocusStyle = efsFrameSunken
            Frame.NonFocusStyle = efsFrameSunken
            Frame.NonFocusTransparentFontColor = clBlack
            DataSource = dm.dsrAutoPAy
            BorderStyle = bsNone
            Color = clWhite
            TabOrder = 1
            OnKeyPress = incAutoPaymentsKeyPress
          end
        end
        object Panel20: TPanel
          Left = 167
          Top = 5
          Width = 11
          Height = 55
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 1
        end
        object GroupBox7: TGroupBox
          Left = 5
          Top = 5
          Width = 162
          Height = 55
          Align = alLeft
          Caption = 'Find by '
          TabOrder = 2
          object rpgAutoPay: TLMDRadioGroup
            Left = 2
            Top = 15
            Width = 158
            Height = 38
            Align = alClient
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
              'Surname'
              'Member')
            ItemOffset = 10
            TabOrder = 0
            OnChange = rpgAutoPayChange
            ItemIndex = 0
          end
        end
      end
      object Panel16: TPanel
        Left = 0
        Top = 65
        Width = 665
        Height = 286
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        Caption = 'Panel6'
        TabOrder = 1
        object dbgAutoPay: TwwDBGrid
          Left = 5
          Top = 5
          Width = 655
          Height = 276
          ControlType.Strings = (
            'PAID;CheckBox;Y;N'
            'EDIT_DATE;CustomEdit;dbdtAutoPay'
            'PRINT_CARD;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'7'#9'Member'
            'FIRST_NAME'#9'10'#9'First Name'
            'SURNAME'#9'12'#9'Surname'
            'EXPIRES'#9'11'#9'Expires'
            'DESCRIPTION'#9'16'#9'Subscription'
            'AMOUNT_PAID'#9'8'#9'Paid'
            'AMOUNT'#9'7'#9'Fee'
            'PRINT_CARD'#9'5'#9'Card'
            'PAID'#9'5'#9'PAY'
            'EDIT_DATE'#9'13'#9'Payment Date'
            'PAY'#9'10'#9'Amount'
            'PAYMENT_PERIOD'#9'6'#9'Period'
            'MAILING_LIST_CODE'#9'10'#9'Parent Code')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrAutoPAy
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          KeyOptions = [dgEnterToTab]
          Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap, dgPerfectRowFit, dgShowFooter]
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgAutoPayCalcCellColors
          IndicatorColor = icBlack
          OnUpdateFooter = dbgAutoPayUpdateFooter
          object dbgAutoPayIButton: TwwIButton
            Left = 0
            Top = 0
            Width = 13
            Height = 22
            Hint = 'Lookup Payments for 1 year prior'
            AllowAllUp = True
            Glyph.Data = {
              F6000000424DF600000000000000760000002800000010000000100000000100
              0400000000008000000000000000000000001000000000000000000000000000
              8000008000000080800080000000800080008080000080808000C0C0C0000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
              8008088888888888888800888888888880080008888888888008F00088888888
              88000F0000000788080080F00788870800008800788FF8708008880788888F87
              78888808888888F808888807888888F8088888078FF88888088888777FF88887
              7888888077888870888888880777770888888888870007888888}
            OnClick = dbgAutoPayIButtonClick
          end
        end
        object dbdtAutoPay: TwwDBDateTimePicker
          Left = 450
          Top = 79
          Width = 108
          Height = 19
          Hint = 
            'Select a date and press Enter - or select no date to see all rec' +
            'eipts'
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
          DisplayFormat = 'dd mmm yyyy'
          OnChange = dtpDateFromChange
          OnExit = dtpDateFromExit
        end
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 673
    inherited ToolBar: TToolBar
      Width = 656
      Align = alClient
      AutoSize = True
      ButtonHeight = 44
      Indent = 0
      ParentColor = False
      inherited pnlFill: TPanel
        Left = 0
        Height = 44
        TabOrder = 1
      end
      object btnCash: TToolButton
        Left = 1
        Top = 0
        Hint = 'Pay Subs by Cash '
        Caption = 'btnCash'
        ImageIndex = 19
        OnClick = menuPaybyCashClick
      end
      object btnTender: TToolButton
        Left = 44
        Top = 0
        Hint = 'Pay Subs by other Tenders'
        Caption = 'btnTender'
        ImageIndex = 20
        OnClick = menuPaybyCashClick
      end
      object btnChangeSubs: TToolButton
        Left = 87
        Top = 0
        Hint = 'Change Subs for the selected Member'
        Caption = 'btnChangeSubs'
        ImageIndex = 11
        OnClick = menuChangeSubsClick
      end
      object btnPrintInvoice: TToolButton
        Left = 130
        Top = 0
        Hint = 'Print an Invoice for the currently selected Member'
        Caption = 'btnPrintInvoice'
        ImageIndex = 23
        OnClick = menuPrintInvoiceClick
      end
      object btnStart: TToolButton
        Left = 173
        Top = 0
        Hint = 'Start'
        Caption = 'btnStart'
        ImageIndex = 29
        OnClick = btnStartClick
      end
      object btnClear: TToolButton
        Left = 216
        Top = 0
        Hint = 'Clear'
        Caption = 'btnClear'
        ImageIndex = 21
        OnClick = btnClearClick
      end
      object btnSaveSection: TToolButton
        Left = 259
        Top = 0
        Hint = 'Save'
        Caption = 'btnSaveSection'
        ImageIndex = 8
        OnClick = btnSaveSectionClick
      end
      object ToolButton1: TToolButton
        Left = 302
        Top = 0
        Width = 8
        Caption = 'ToolButton1'
        ImageIndex = 1
        Style = tbsDivider
      end
      object btnPrint: TToolButton
        Left = 310
        Top = 0
        Hint = 'Print payments'
        Caption = 'btnPrint'
        ImageIndex = 12
        OnClick = menuPrintClick
      end
      object btnPreview: TToolButton
        Left = 353
        Top = 0
        Hint = 'Preview the last Receipt'
        Caption = 'btnPreview'
        ImageIndex = 15
        OnClick = menuPrintClick
      end
      object btnPrinterSetup: TToolButton
        Left = 396
        Top = 0
        Hint = 'Set a Printer '
        Caption = 'btnPrinterSetup'
        ImageIndex = 18
        OnClick = menuPrinterSetupClick
      end
      object btnTenderSection: TToolButton
        Left = 439
        Top = 0
        Hint = 'Pay Section by Tender'
        Caption = 'btnTenderSection'
        ImageIndex = 20
        OnClick = menuTenderSectionClick
      end
      object ToolButton6: TToolButton
        Left = 482
        Top = 0
        Width = 8
        Caption = 'ToolButton6'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object btnLateFee: TToolButton
        Left = 490
        Top = 0
        Hint = 'Add a Late Sub'
        Caption = 'btnLateFee'
        ImageIndex = 22
        OnClick = btnLateFeeClick
      end
      object btnChangeSection: TToolButton
        Left = 533
        Top = 0
        Hint = 'Change the Section for the selected Member'
        Caption = 'btnChangeSection'
        ImageIndex = 17
        OnClick = btnChangeSectionClick
      end
      object pnlSpace: TPanel
        Left = 576
        Top = 0
        Width = 28
        Height = 44
        BevelOuter = bvNone
        TabOrder = 0
      end
      object ToolButton2: TToolButton
        Left = 604
        Top = 0
        Caption = 'ToolButton2'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 586
    Top = 58
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuSubs: TMenuItem [1]
      Caption = 'Subs'
      ShortCut = 118
      object menuPaybyCash: TMenuItem
        Caption = 'Pay by &Cash'
        ShortCut = 120
        OnClick = menuPaybyCashClick
      end
      object menuPaybyTender: TMenuItem
        Caption = 'Pay by &Tender'
        OnClick = menuPaybyCashClick
      end
      object menubb: TMenuItem
        Caption = '-'
      end
      object menuClearSubs: TMenuItem
        Caption = 'Clea&r Subs'
        OnClick = menuClearSubsClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menuChangeSubs: TMenuItem
        Caption = 'Change S&ubs'
        ShortCut = 117
        OnClick = menuChangeSubsClick
      end
      object menuChangeSection: TMenuItem
        Caption = 'Change Section'
        ShortCut = 114
        OnClick = btnChangeSectionClick
      end
      object menuDeleteSection: TMenuItem
        Caption = 'Delete Section'
        ShortCut = 115
        OnClick = dbgPaySecMemDblClick
      end
      object menuAddaLateSub: TMenuItem
        Caption = 'Add a Late Sub'
        ShortCut = 113
        OnClick = btnLateFeeClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object menuUndoInvoice: TMenuItem
        Caption = 'Undo Invoice'
        OnClick = menuUndoInvoiceClick
      end
    end
    object menuSections: TMenuItem [2]
      Caption = 'Sections'
      object menuTenderSection: TMenuItem
        Caption = 'Pay by Tender'
        OnClick = menuTenderSectionClick
      end
      object menuBreak: TMenuItem
        Caption = '-'
      end
      object menuSavetheSection: TMenuItem
        Caption = 'Save the Section'
        OnClick = menuSavetheSectionClick
      end
      object menub: TMenuItem
        Caption = '-'
      end
      object menuClearSection: TMenuItem
        Caption = 'Clea&r Section'
        OnClick = menuClearSectionClick
      end
    end
    object menuBulkCardReceipt: TMenuItem [3]
      Caption = 'Bulk Card/Receipt'
      object menuStartCR: TMenuItem
        Caption = 'Start'
        OnClick = menuStartCRClick
      end
      object menuCancelCR: TMenuItem
        Caption = 'Cancel'
        OnClick = menuCancelCRClick
      end
      object menuSaveCR: TMenuItem
        Caption = 'Save'
        OnClick = menuSaveCRClick
      end
    end
    object menuAutoPayments: TMenuItem [4]
      Caption = 'Auto Payments'
      object menuStartAP: TMenuItem
        Caption = 'Start'
        OnClick = menuStartAPClick
      end
      object menuCancelAP: TMenuItem
        Caption = 'Cancel'
        OnClick = menuCancelAPClick
      end
      object menuSaveAP: TMenuItem
        Caption = 'Save'
        OnClick = menuSaveAPClick
      end
    end
    object Options1: TMenuItem [5]
      Caption = '&Options'
      object menuPrint: TMenuItem
        Caption = 'Print'
        ShortCut = 16464
        OnClick = menuPrintClick
      end
      object menuPrinterSetup: TMenuItem
        Caption = '&Printer Setup'
        OnClick = menuPrinterSetupClick
      end
      object menuPreview: TMenuItem
        Caption = 'Pre&view'
        OnClick = menuPrintClick
      end
      object menubbbb: TMenuItem
        Caption = '-'
      end
      object menuPrintInvoice: TMenuItem
        Caption = 'Print Invoice'
        OnClick = menuPrintInvoiceClick
      end
      object menuPrintTodaysTotals: TMenuItem
        Caption = 'Print Toda&ys Totals'
        OnClick = menuPrintTodaysTotalsClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object menuAddMemberCard: TMenuItem
        Caption = 'Add Member for a Card '
        ShortCut = 122
        OnClick = menuAddMemberCardClick
      end
      object menuMemberBrief: TMenuItem
        Caption = 'Member Brief'
        ShortCut = 116
        OnClick = menuMemberBriefClick
      end
    end
  end
  object menuLastPaid: TPopupMenu
    Left = 616
    Top = 56
    object menuLastPayment: TMenuItem
      Caption = 'Last Payments'
    end
  end
end
