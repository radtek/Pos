inherited frmMembersROLL: TfrmMembersROLL
  Tag = 5
  Left = 67
  Top = 42
  Caption = 'frmMembersROLL'
  ClientHeight = 469
  ClientWidth = 642
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 449
    Width = 642
    Panels = <
      item
        Width = 120
      end
      item
        Width = 20
      end>
  end
  object PageControl: TPageControl [1]
    Left = 0
    Top = 60
    Width = 642
    Height = 389
    ActivePage = tsMemberRoll
    Align = alClient
    TabOrder = 2
    OnChange = PageControlChange
    object tsMemberRoll: TTabSheet
      Caption = '&Members Rollover'
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 634
        Height = 94
        Align = alTop
        BevelOuter = bvNone
        BorderWidth = 3
        TabOrder = 0
        object gbxOptions: TGroupBox
          Left = 3
          Top = 48
          Width = 628
          Height = 43
          Align = alBottom
          Caption = 'Options '
          TabOrder = 0
          object cbxNewsLetter: TCheckBox
            Left = 284
            Top = 15
            Width = 145
            Height = 17
            Caption = 'Set NewsLetter to '#39'NO'#39
            TabOrder = 0
          end
          object cbxAnalyse: TCheckBox
            Left = 462
            Top = 15
            Width = 145
            Height = 17
            Caption = 'Set Analyse to '#39'NO'#39
            TabOrder = 1
          end
          object cbxPrior: TCheckBox
            Left = 20
            Top = 16
            Width = 237
            Height = 17
            Hint = 'Useful for when there is no POS connected'
            Caption = 'Rollover all Members Prior to the Expires Date'
            TabOrder = 2
          end
        end
        object rgpView: TRadioGroup
          Left = 3
          Top = 6
          Width = 628
          Height = 42
          Align = alBottom
          Caption = 'View '
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            'Rollover Members'
            'Keep Expires Current Members')
          TabOrder = 1
          OnClick = rgpViewClick
        end
        object Panel5: TPanel
          Left = 3
          Top = 3
          Width = 628
          Height = 46
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object lblExpirySystem: TLabel
            Left = 8
            Top = 14
            Width = 173
            Height = 16
            Caption = 'Revolving Expiry System'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object lblRollDate: TLabel
            Left = 232
            Top = 17
            Width = 74
            Height = 13
            Caption = 'Current Expires '
            WordWrap = True
          end
          object lblRevolveDays: TLabel
            Left = 442
            Top = 17
            Width = 74
            Height = 18
            AutoSize = False
            Caption = 'Revolve Days'
            WordWrap = True
          end
          object lblNextExpires: TLabel
            Left = 442
            Top = 18
            Width = 66
            Height = 18
            AutoSize = False
            Caption = 'Next Expires'
            WordWrap = True
          end
          object dbeLastRoll: TwwDBEdit
            Left = 317
            Top = 15
            Width = 90
            Height = 19
            Hint = 'Cannot be changed - controlled by the Rollover'
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'MEMBER_EXPIRY'
            DataSource = dm.dsrTblMisc
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
          end
          object dbeRevolveDays: TwwDBEdit
            Left = 524
            Top = 15
            Width = 90
            Height = 19
            Hint = 'This can be changed in the Sytem Screen/Site tab'
            BorderStyle = bsNone
            Color = clWhite
            DataField = 'REVOLVE_DAYS'
            DataSource = dm.dsrTblMisc
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
          end
          object dbdtNextExpires: TwwDBDateTimePicker
            Tag = 88
            Left = 524
            Top = 15
            Width = 90
            Height = 19
            Hint = 'The Next Expiry Date'
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
          end
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 94
        Width = 634
        Height = 267
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 1
        object Splitter2: TSplitter
          Left = 5
          Top = 198
          Width = 624
          Height = 7
          Cursor = crVSplit
          Align = alBottom
          Beveled = True
        end
        object dbgRollover: TwwDBGrid
          Left = 5
          Top = 5
          Width = 624
          Height = 193
          ControlType.Strings = (
            'ACCEPT;CheckBox;Y;N'
            'ANALYSE;CheckBox;Y;N'
            'IS_ACTIVE;CheckBox;Y;N'
            'KEEP_EXPIRES_CURRENT;CheckBox;Y;N'
            'NEWSLETTER;CheckBox;Y;N'
            'SUB_PAID;CustomEdit;cbxFinancial')
          Selected.Strings = (
            'MEMBER'#9'8'#9'Member'#9'F'
            'FULL_NAME'#9'24'#9'Full Name'#9'F'
            'EXPIRES'#9'14'#9'Expires'#9'F'
            'ACCEPT'#9'5'#9'Accept'#9'F'
            'ANALYSE'#9'6'#9'Analyse'#9'F'
            'NEWSLETTER'#9'9'#9'NewsLetter'#9'F'
            'IS_ACTIVE'#9'5'#9'Active'#9'F'
            'KEEP_EXPIRES_CURRENT'#9'9'#9'Exp Current'#9'F'
            'SUB_PAID'#9'9'#9'Financial'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          OnRowChanged = dbgRolloverRowChanged
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          DataSource = dm.dsrRollover
          KeyOptions = [dgEnterToTab]
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgRolloverCalcCellColors
          IndicatorColor = icBlack
        end
        object dbgSubs: TwwDBGrid
          Left = 5
          Top = 205
          Width = 624
          Height = 57
          Selected.Strings = (
            'MEMBER'#9'8'#9'Member'
            'SUBSCRIPTION'#9'11'#9'Subscription'
            'Description'#9'38'#9'Description'
            'RECEIPT'#9'10'#9'Receipt'
            'AMOUNT_PAID'#9'13'#9'Amt Paid'
            'AMOUNT'#9'13'#9'Amount')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alBottom
          DataSource = dm.dsrAttachSubs
          KeyOptions = [dgEnterToTab]
          Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          TabOrder = 1
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clBlack
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          OnCalcCellColors = dbgRolloverCalcCellColors
          IndicatorColor = icBlack
        end
        object cbxFinancial: TwwDBComboBox
          Left = 448
          Top = 32
          Width = 121
          Height = 21
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          DataField = 'SUB_PAID'
          DataSource = dm.dsrRollover
          DropDownCount = 8
          ItemHeight = 0
          Items.Strings = (
            'No'#9'0'
            'Part'#9'1'
            'Yes'#9'2')
          Sorted = False
          TabOrder = 2
          UnboundDataType = wwDefault
        end
      end
    end
    object tsSectionRoll: TTabSheet
      Caption = '&Sections Rollover'
      ImageIndex = 1
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 634
        Height = 53
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object cbxAllSections: TCheckBox
          Left = 42
          Top = 19
          Width = 119
          Height = 17
          Hint = 'Tick this box to Rollover all Sections at once'
          Caption = 'Apply to all Sections'
          Color = clSilver
          ParentColor = False
          TabOrder = 0
          OnClick = cbxAllSectionsClick
        end
        object cbxClearMember: TCheckBox
          Left = 213
          Top = 19
          Width = 131
          Height = 17
          Hint = 'Clear the Members from the Section where they have NOT Paid'
          Caption = 'Clear Unpaid Members '
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 53
        Width = 634
        Height = 308
        Align = alClient
        TabOrder = 1
        object dbgSections: TwwDBGrid
          Left = 1
          Top = 1
          Width = 632
          Height = 306
          Selected.Strings = (
            'SECTION'#9'8'#9'Code'#9'F'
            'DESCRIPTION'#9'16'#9'Description'#9'F'
            'EXPIRES'#9'13'#9'Expires'#9'F'
            'LAST_ROLL'#9'15'#9'Last Roll Date'#9'F')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrTblSections
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
          ReadOnly = True
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
      end
    end
    object tsLuckyMember: TTabSheet
      Caption = '&Lucky Member Rollover'
      ImageIndex = 2
      object Label3: TLabel
        Left = 162
        Top = 130
        Width = 88
        Height = 13
        Caption = 'Last Rollover Date'
      end
      object wwDBEdit2: TwwDBEdit
        Left = 273
        Top = 127
        Width = 70
        Height = 19
        BorderStyle = bsNone
        Color = clWhite
        DataField = 'LUCKY_ROLL'
        DataSource = dm.dsrTblMisc
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
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 642
    inherited ToolBar: TToolBar
      Width = 625
      Align = alClient
      ButtonWidth = 42
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object btnRollover: TToolButton
        Left = 3
        Top = 0
        Caption = 'btnRollover'
        ImageIndex = 29
        OnClick = menuStartRolloverClick
      end
      object ToolButton1: TToolButton
        Left = 45
        Top = 0
        Width = 20
        Caption = 'ToolButton1'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object btnCancelRollover: TToolButton
        Left = 65
        Top = 0
        Hint = 'Cancel the Rollover'
        Caption = 'btnCancelRollover'
        Enabled = False
        ImageIndex = 7
        OnClick = menuCancelRolloverClick
      end
      object btnAcceptRollover: TToolButton
        Left = 107
        Top = 0
        Hint = 'Accept the Rollover'
        Caption = 'btnAcceptRollover'
        Enabled = False
        ImageIndex = 8
        OnClick = menuAcceptRolloverClick
      end
      object ToolButton5: TToolButton
        Left = 149
        Top = 0
        Width = 20
        Caption = 'ToolButton5'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object btnExportRollover: TToolButton
        Left = 169
        Top = 0
        Hint = 'Export the Members to a Comma Delimited file'
        Caption = 'btnExportRollover'
        Enabled = False
        ImageIndex = 17
        OnClick = menuExportRolloverClick
      end
      object btnLetterRollover: TToolButton
        Left = 211
        Top = 0
        Hint = 'Subs Due Final Letter'
        Caption = 'btnLetterRollover'
        Enabled = False
        ImageIndex = 13
        OnClick = menuSubsDueFinalLetterClick
      end
      object btnPrintRollover: TToolButton
        Left = 253
        Top = 0
        Hint = 'Print the Members Rollover Report'
        Caption = 'btnPrintRollover'
        Enabled = False
        ImageIndex = 12
        OnClick = menuPrintRolloverClick
      end
      object pnlSpace: TPanel
        Left = 295
        Top = 0
        Width = 282
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
      end
      object btnClose: TToolButton
        Left = 577
        Top = 0
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 554
    Top = 18
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object Options1: TMenuItem [1]
      Caption = 'Options'
      object menuStartRollover: TMenuItem
        Caption = 'Start Rollover'
        OnClick = menuStartRolloverClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menuAcceptRollover: TMenuItem
        Caption = 'Accept Rollover'
        OnClick = menuAcceptRolloverClick
      end
      object menuCancelRollover: TMenuItem
        Caption = 'Cancel Rollover'
        OnClick = menuCancelRolloverClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object menuExportRollover: TMenuItem
        Caption = 'Export Rollover'
        OnClick = menuExportRolloverClick
      end
      object menuPrintRollover: TMenuItem
        Caption = 'Print Rollover'
        OnClick = menuPrintRolloverClick
      end
      object menuSubsDueFinalLetter: TMenuItem
        Caption = 'Subs Due Final  Letter'
        OnClick = menuSubsDueFinalLetterClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object menuReceiptRollover: TMenuItem
        Caption = 'Receipt Rollover'
        OnClick = btnReceiptRolloverClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object menuPointsCreditRollover: TMenuItem
        Caption = 'Points Credit Rollover'
        OnClick = menuPointsCreditRolloverClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object menuNonFinancialMemberDelete: TMenuItem
        Caption = 'Non Financial Member Delete'
        OnClick = menuNonFinancialMemberDeleteClick
      end
    end
  end
end
