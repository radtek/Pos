inherited frmMembersSYS: TfrmMembersSYS
  Tag = 4
  Left = 301
  Top = 119
  Caption = 'frmMembersSYS'
  ClientHeight = 446
  ClientWidth = 564
  Font.Color = clBlack
  OldCreateOrder = True
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 426
    Width = 564
  end
  object PageControl: TPageControl [1]
    Left = 0
    Top = 60
    Width = 564
    Height = 366
    ActivePage = tsSite
    Align = alClient
    MultiLine = True
    ParentShowHint = False
    RaggedRight = True
    ShowHint = True
    TabOrder = 1
    OnChange = PageControlChange
    object tsSite: TTabSheet
      BorderWidth = 5
      Caption = '&Site'
      object GroupBox8: TGroupBox
        Left = 0
        Top = 280
        Width = 546
        Height = 48
        Align = alBottom
        TabOrder = 0
        object Label17: TLabel
          Left = 75
          Top = 19
          Width = 296
          Height = 13
          Caption = 
            '    The system needs restarting for the changes to take affect. ' +
            ' '
        end
        object btnSave: TButton
          Left = 418
          Top = 14
          Width = 108
          Height = 25
          Hint = 'Save the Registry Settings'
          Caption = 'Save Registry'
          TabOrder = 0
          OnClick = btnSaveClick
        end
      end
      object pcSite: TPageControl
        Left = 0
        Top = 0
        Width = 546
        Height = 280
        ActivePage = tsDefaults
        Align = alClient
        TabOrder = 1
        OnChange = pcSiteChange
        object tsDefaults: TTabSheet
          Caption = 'Defaults'
          object rvpMisc: TwwRecordViewPanel
            Left = 0
            Top = 0
            Width = 538
            Height = 252
            HorzScrollBar.Margin = 6
            HorzScrollBar.Range = 506
            VertScrollBar.Increment = 21
            VertScrollBar.Margin = 6
            VertScrollBar.Range = 869
            Align = alClient
            AutoScroll = False
            BorderStyle = bsNone
            Color = clSilver
            ParentColor = False
            TabOrder = 0
            ControlType.Strings = (
              'AUTO_PRINT_CARD;CustomEdit;dbcbCard'
              'AUTO_PRINT_RECEIPT;CustomEdit;dbcbReceipt'
              'CAPITALS;CustomEdit;dbcbCaps'
              'NAME_FIELDS;CustomEdit;dbcbNameFields'
              'DISABLE_SERVICES;CustomEdit;dbcbServices'
              'CONFIRM_PAYMENT;CustomEdit;dbcbConfirmPayment'
              'CARD_ENCODE_MEMBER;CustomEdit;dbcbCardEncode'
              'PAYMENT_CARD;CustomEdit;dbcbCard'
              'PAYMENT_RECEIPT;CustomEdit;dbcbReceipt'
              'PART_PAY_ADVANCE;CustomEdit;dbcbPartPay'
              'ANNUAL_CARD;CustomEdit;dbcbFirstPayment')
            DataSource = dm.dsrTblMisc
            EditFrame.Enabled = True
            EditFrame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            EditFrame.FocusStyle = efsFrameSunken
            EditFrame.NonFocusStyle = efsFrameSunken
            EditFrame.NonFocusTransparentFontColor = clBlack
            ControlOptions = []
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clBlack
            LabelFont.Height = -11
            LabelFont.Name = 'MS Sans Serif'
            LabelFont.Style = []
            Selected.Strings = (
              'KEEP_INACTIVE'#9'10'#9'Keep Inactive'#9'F'
              'MEMBER_EXPIRY'#9'18'#9'Member Expiry'#9'F'
              'MEMBER_PRE_EXPIRY'#9'10'#9'Member Pre Expiry Days'#9'F'
              'REVOLVE_DAYS'#9'10'#9'Revolve Days'#9'F'
              'AREV_IMPORT_FILE_PATH'#9'40'#9'Arev Import File Path'#9'F'
              'AREV_FILE_PATH'#9'40'#9'Arev Second File Path'#9'F'
              'CONTROLLER'#9'40'#9'Stockmaster Controller PC'#9'F'
              'CAPITALS'#9'10'#9'Name and Address as Capitals'#9'F'
              'NAME_FIELDS'#9'45'#9'Full Name Make Up'#9'F'
              'SECTION_REVOLVE_DAYS'#9'10'#9'Section Revolve Days'#9'F'
              'MSG'#9'200'#9'Invoice Message'#9'F'
              'DISABLE_SERVICES'#9'10'#9'Disable Services'#9'F'
              'CARD_ENCODE_MEMBER'#9'20'#9'Card Encoding'#9'F'
              'CONFIRM_PAYMENT'#9'10'#9'Payment Confirm'#9'F'
              'PAYMENT_GRIDS'#9'5'#9'Payment View Subs/Sect/Join'#9'F'
              'PAYMENT_CARD'#9'20'#9'Payment Card Options'#9'F'
              'PAYMENT_RECEIPT'#9'20'#9'Payment Receipt Options'#9'F'
              'ANNUAL_CARD'#9'10'#9'First Payment Non Active'#9'F'
              'POS_PAY_CARD_REQD'#9'1'#9'POS Payment Card Reqd'#9'F'
              'TOP_LINE_FEED'#9'10'#9'Top Line Feed'#9'F'
              'BOTTOM_LINE_FEED'#9'10'#9'Bottom Line Feed'#9'F'
              'PAY_BEFORE_EXPIRES'#9'1'#9'Pay Before Expires'#9'F'
              'CARD_DEFAULT'#9'1'#9'Card Default'#9'F'
              'RECEIPT_DEFAULT'#9'1'#9'Receipt Default'#9'F'
              'EXPORT_SUBS'#9'1'#9'Export Subs to File'#9'F'
              'EXPORT_TIMER'#9'5'#9'Export Timer Interval (Seconds)'#9'F'
              'PRINT_MEMBER_PAYMENT'#9'1'#9'Print Member Payment'#9'F'
              'PHM_INSTALLED'#9'1'#9'PHM Installed'#9'F'
              'PHM_PATH'#9'40'#9'PHM File Path'#9'F'
              'PHM_CONTROLLER'#9'40'#9'PHM Controller PC'#9'F'
              'GROUP_DELETE'#9'1'#9'Sub Group Delete'#9'F'
              'PARENT_CODE_ADDRESS'#9'1'#9'Parent Code alter Address'#9'F'
              'KNOWN_AS_DEFAULTED'#9'1'#9'Known As Defaulted'#9'F'
              
                'PRINT_RECEIPT_AS_TEXT'#9'1'#9'D - Dot Mtrx, T - thermal, P - Posiflex'#9 +
                'F'
              'CHECK_GROUPS'#9'1'#9'Check on Subs'#9'F'
              'MIN_GROUP'#9'10'#9'Min # of Groups'#9'F'
              'MAX_GROUP'#9'10'#9'Max # of Groups'#9'F'
              'CHECK_SUBS'#9'1'#9'Check on Groups'#9'F'
              'MIN_SUB'#9'10'#9'Min # of Subs'#9'F'
              'MAX_SUB'#9'10'#9'Max # of Subs'#9'F'
              'CLEAR_INTERESTS'#9'1'#9'Clear Section Interests after Report'#9'F')
            object dbcbCaps: TwwDBComboBox
              Left = 181
              Top = 152
              Width = 69
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'CAPITALS'
              DataSource = dm.dsrTblMisc
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
              TabOrder = 7
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbNameFields: TwwDBComboBox
              Left = 181
              Top = 173
              Width = 279
              Height = 19
              ShowButton = True
              Style = csDropDown
              MapList = False
              AllowClearKey = False
              BorderStyle = bsNone
              ButtonStyle = cbsEllipsis
              DataField = 'NAME_FIELDS'
              DataSource = dm.dsrTblMisc
              DropDownCount = 0
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Sorted = False
              TabOrder = 0
              UnboundDataType = wwDefault
              Visible = False
              OnClick = dbcbNameFieldsClick
              OnDropDown = dbcbNameFieldsClick
            end
            object dbcbServices: TwwDBComboBox
              Left = 181
              Top = 236
              Width = 69
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'DISABLE_SERVICES'
              DataSource = dm.dsrTblMisc
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
              TabOrder = 1
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbCardEncode: TwwDBComboBox
              Left = 181
              Top = 257
              Width = 129
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'CARD_ENCODE_MEMBER'
              DataSource = dm.dsrTblMisc
              DropDownCount = 3
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'Member Only'#9'Y'
                'Member and Expires'#9'N'
                'No Encoding'#9'X')
              Sorted = False
              TabOrder = 2
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbConfirmPayment: TwwDBComboBox
              Left = 181
              Top = 278
              Width = 69
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'CONFIRM_PAYMENT'
              DataSource = dm.dsrTblMisc
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
              Visible = False
            end
            object dbcbCard: TwwDBComboBox
              Left = 181
              Top = 320
              Width = 129
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'PAYMENT_CARD'
              DataSource = dm.dsrTblMisc
              DropDownCount = 3
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'Print'#9'0'
                'Save'#9'1'
                'Bulk Card'#9'2')
              Sorted = False
              TabOrder = 4
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbReceipt: TwwDBComboBox
              Left = 181
              Top = 341
              Width = 129
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'PAYMENT_RECEIPT'
              DataSource = dm.dsrTblMisc
              DropDownCount = 3
              Frame.Enabled = True
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'Print'#9'0'
                'Save'#9'1'
                'Bulk Receipt'#9'2'
                'Letter'#9'3')
              Sorted = False
              TabOrder = 5
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbFirstPayment: TwwDBComboBox
              Left = 181
              Top = 362
              Width = 69
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              BorderStyle = bsNone
              Color = clWhite
              DataField = 'ANNUAL_CARD'
              DataSource = dm.dsrTblMisc
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
              Visible = False
            end
          end
        end
        object tsRegistry: TTabSheet
          Caption = 'Registry'
          ImageIndex = 1
          object pnlRegistry: TPanel
            Left = 0
            Top = 0
            Width = 538
            Height = 252
            Align = alClient
            BevelOuter = bvNone
            Color = clSilver
            TabOrder = 0
            object GroupBox11: TGroupBox
              Left = 7
              Top = 6
              Width = 526
              Height = 135
              Caption = 'Membership Database '
              TabOrder = 0
              object Label10: TLabel
                Left = 25
                Top = 68
                Width = 131
                Height = 13
                Caption = 'Database Directory + Name'
              end
              object Label9: TLabel
                Left = 25
                Top = 24
                Width = 147
                Height = 26
                Caption = 'Server or Computer Name'#13#10'where the Database is installed'
                WordWrap = True
              end
              object Label11: TLabel
                Left = 360
                Top = 102
                Width = 55
                Height = 13
                Caption = 'User Logon'
              end
              object Label12: TLabel
                Left = 25
                Top = 102
                Width = 82
                Height = 13
                Caption = 'Local User Name'
              end
              object edtDataBaseName: TFilenameEdit
                Left = 192
                Top = 62
                Width = 322
                Height = 21
                Filter = 'DataBase Files|*.GDB|All Files |*.*'
                DialogTitle = 'Database Name'
                Color = clWhite
                NumGlyphs = 1
                TabOrder = 0
                OnKeyPress = edtServerNameKeyPress
              end
              object edtServerName: TEdit
                Left = 192
                Top = 26
                Width = 318
                Height = 21
                Color = clWhite
                TabOrder = 1
                OnKeyPress = edtServerNameKeyPress
              end
              object cbxUserLogon: TComboBox
                Left = 446
                Top = 98
                Width = 68
                Height = 21
                Color = clWhite
                ItemHeight = 13
                TabOrder = 2
                OnKeyPress = edtServerNameKeyPress
                Items.Strings = (
                  'No'
                  'Yes')
              end
              object edtLocalUserName: TEdit
                Left = 192
                Top = 98
                Width = 139
                Height = 21
                Color = clWhite
                TabOrder = 3
                OnKeyPress = edtServerNameKeyPress
              end
            end
            object GroupBox12: TGroupBox
              Left = 7
              Top = 148
              Width = 526
              Height = 96
              Caption = 'MenuMate Database '
              TabOrder = 1
              object Label13: TLabel
                Left = 22
                Top = 68
                Width = 131
                Height = 13
                Caption = 'Database Directory + Name'
              end
              object Label32: TLabel
                Left = 21
                Top = 24
                Width = 147
                Height = 26
                Caption = 'Server or Computer Name'#13#10'where the Database is installed'
                WordWrap = True
              end
              object edtMenuMateDataBaseName: TFilenameEdit
                Left = 192
                Top = 64
                Width = 322
                Height = 21
                Filter = 'DataBase Files|*.GDB|All Files |*.*'
                DialogTitle = 'Database Name'
                Color = clWhite
                NumGlyphs = 1
                TabOrder = 0
                OnKeyPress = edtServerNameKeyPress
              end
              object edtMenuMateServerName: TEdit
                Left = 192
                Top = 26
                Width = 322
                Height = 21
                Color = clWhite
                TabOrder = 1
                OnKeyPress = edtServerNameKeyPress
              end
            end
          end
        end
        object tsSites: TTabSheet
          Caption = 'Site'
          ImageIndex = 2
          object rvpSite: TwwRecordViewPanel
            Left = 0
            Top = 0
            Width = 538
            Height = 252
            HorzScrollBar.Margin = 6
            HorzScrollBar.Range = 286
            VertScrollBar.Increment = 21
            VertScrollBar.Margin = 6
            VertScrollBar.Range = 281
            Align = alClient
            AutoScroll = False
            BorderStyle = bsNone
            Color = clSilver
            ParentColor = False
            TabOrder = 0
            DataSource = dm.dsrTblSite
            EditFrame.Enabled = True
            EditFrame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
            EditFrame.FocusStyle = efsFrameSunken
            EditFrame.NonFocusStyle = efsFrameSunken
            EditFrame.NonFocusTransparentFontColor = clBlack
            ControlOptions = [rvcTransparentLabels, rvcTransparentButtons, rvcFlatButtons]
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clBlack
            LabelFont.Height = -11
            LabelFont.Name = 'MS Sans Serif'
            LabelFont.Style = []
            Selected.Strings = (
              'COMP_NAME'#9'80'#9'Company Name'#9'F'
              'ADDRESS1'#9'30'#9'Address'#9'F'
              'ADDRESS2'#9'30'#9'Address'#9'F'
              'ADDRESS3'#9'30'#9'Address'#9'F'
              'POSTCODE'#9'8'#9'PostCode'#9'F'
              'PHONE'#9'20'#9'Phone'#9'F'
              'GST_NO'#9'10'#9'GST Number'#9'F'
              'GST_RATE'#9'10'#9'GST Rate'#9'F'
              'SITE_LICENSE'#9'10'#9'SITE License'#9'F'
              'MEMBER_LICENSE'#9'20'#9'MEMBER License'#9'F'
              'POS_LICENSE'#9'20'#9'POS License'#9'F'
              'HELP_DESK'#9'20'#9'Help Desk'#9'F'
              'FAX'#9'20'#9'Fax '#9'F')
          end
        end
        object tsMenuMate: TTabSheet
          Caption = 'MenuMate'
          ImageIndex = 3
          object Label23: TLabel
            Left = 369
            Top = 29
            Width = 176
            Height = 13
            Caption = 'Database Trigger required/removed  '
          end
          object Label24: TLabel
            Left = 376
            Top = 49
            Width = 154
            Height = 26
            Caption = 'example  ;member?   ;fullname?  '#13#10'NB is case sensitive !!!'
          end
          object Label25: TLabel
            Left = 376
            Top = 86
            Width = 142
            Height = 13
            Caption = 'Edit does not alter Swipe Card'
          end
          object Label26: TLabel
            Left = 376
            Top = 108
            Width = 161
            Height = 16
            AutoSize = False
            Caption = 'example of 4 would be   5 = 0005'
            WordWrap = True
          end
          object Label34: TLabel
            Left = 377
            Top = 156
            Width = 161
            Height = 27
            AutoSize = False
            Caption = 'requires ver 4 or above of MenuMate'
            WordWrap = True
          end
          object Label35: TLabel
            Left = 377
            Top = 131
            Width = 161
            Height = 16
            AutoSize = False
            Caption = 'required for Reports'
            WordWrap = True
          end
          object rvpMenuMate: TwwRecordViewPanel
            Left = 0
            Top = 0
            Width = 369
            Height = 252
            HorzScrollBar.Margin = 6
            HorzScrollBar.Range = 369
            VertScrollBar.Increment = 24
            VertScrollBar.Margin = 6
            VertScrollBar.Range = 179
            Align = alLeft
            AutoScroll = False
            BorderStyle = bsNone
            Color = clSilver
            ParentColor = False
            TabOrder = 0
            ControlType.Strings = (
              'MENUMATE_INSTALLED;CustomEdit;dbcbMMInstalled'
              'TRIGGER_INSTALLED;CustomEdit;dbcdMMTrigger'
              'ON_INSERT;CustomEdit;dbcbOnInsert'
              'DISCOUNTS_INSTALLED;CustomEdit;dbcdMMDiscounts')
            DataSource = dm.dsrMenuMate
            EditSpacing.HorizontalView.BetweenRow = 5
            EditSpacing.VerticalView.BetweenRow = 5
            ControlOptions = []
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clWindowText
            LabelFont.Height = -11
            LabelFont.Name = 'MS Sans Serif'
            LabelFont.Style = []
            Selected.Strings = (
              'MENUMATE_INSTALLED'#9'8'#9'MenuMate Installed'#9'F'
              'TRIGGER_INSTALLED'#9'8'#9'Donation Points Installed'#9'F'
              'SWIPE_TEXT'#9'60'#9'Swipe Card Text'#9'F'
              'ON_INSERT'#9'8'#9'Alter Swipe Card on Insert'#9'F'
              'SWIPE_LENGTH'#9'10'#9'Prefill of Member Number'#9'F'
              'VIP_CREDIT_NAME'#9'20'#9'VIP Credit Name'#9'F'
              'DISCOUNTS_INSTALLED'#9'8'#9'Discounts Installed'#9'F')
            object dbcbMMInstalled: TwwDBComboBox
              Left = 132
              Top = 5
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'MENUMATE_INSTALLED'
              DataSource = dm.dsrMenuMate
              DropDownCount = 2
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'YES'#9'Y'
                'NO'#9'N')
              ItemIndex = 0
              Sorted = False
              TabOrder = 0
              UnboundDataType = wwDefault
            end
            object dbcdMMTrigger: TwwDBComboBox
              Left = 132
              Top = 29
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'TRIGGER_INSTALLED'
              DataSource = dm.dsrMenuMate
              DropDownCount = 2
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'YES'#9'Y'
                'NO'#9'N')
              ItemIndex = 0
              Sorted = False
              TabOrder = 1
              UnboundDataType = wwDefault
              OnEnter = dbcdMMTriggerEnter
            end
            object dbcbOnInsert: TwwDBComboBox
              Left = 132
              Top = 79
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'ON_INSERT'
              DataSource = dm.dsrMenuMate
              DropDownCount = 2
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'YES'#9'Y'
                'NO'#9'N')
              ItemIndex = 0
              Sorted = False
              TabOrder = 3
              UnboundDataType = wwDefault
            end
            object dbcdMMDiscounts: TwwDBComboBox
              Left = 132
              Top = 155
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'DISCOUNTS_INSTALLED'
              DataSource = dm.dsrMenuMate
              DropDownCount = 2
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'YES'#9'Y'
                'NO'#9'N')
              ItemIndex = 1
              Sorted = False
              TabOrder = 6
              UnboundDataType = wwDefault
              OnEnter = dbcdMMDiscountsEnter
            end
          end
          object btnSwipeCard: TButton
            Left = 413
            Top = 202
            Width = 108
            Height = 25
            Hint = 'Updates the Swipe Card field - usually on Installation'
            Caption = 'Update Swipe Card'
            TabOrder = 1
            OnClick = btnSwipeCardClick
          end
        end
        object tsKiosk: TTabSheet
          Caption = 'Kiosk'
          ImageIndex = 4
          object Label22: TLabel
            Left = 288
            Top = 7
            Width = 217
            Height = 13
            Caption = 'Show Dice or Company Logo ( need logo.jpg )'
          end
          object Label27: TLabel
            Left = 288
            Top = 33
            Width = 148
            Height = 13
            Caption = 'Show Points as 1000 or $10.00'
          end
          object Label28: TLabel
            Left = 288
            Top = 59
            Width = 201
            Height = 13
            Caption = 'Show the message for (4 seconds default )'
          end
          object Label29: TLabel
            Left = 288
            Top = 84
            Width = 200
            Height = 13
            Caption = 'Check 30 days before the Member Expires'
          end
          object Label30: TLabel
            Left = 288
            Top = 110
            Width = 229
            Height = 13
            Caption = 'Member start digit off the Swipe Card ( usually 2 )'
          end
          object Label31: TLabel
            Left = 288
            Top = 136
            Width = 219
            Height = 13
            Caption = 'Member length off the Swipe Card ( usually  4 )'
          end
          object Label33: TLabel
            Left = 288
            Top = 195
            Width = 232
            Height = 39
            Caption = 
              'NB Kiosk only refreshes every 5 minutes , so any '#13#10'changes that ' +
              'are made here may not appear immediately on the Kiosk'
            WordWrap = True
          end
          object rvpKiosk: TwwRecordViewPanel
            Left = 0
            Top = 0
            Width = 273
            Height = 252
            HorzScrollBar.Margin = 6
            HorzScrollBar.Range = 186
            VertScrollBar.Increment = 26
            VertScrollBar.Margin = 6
            VertScrollBar.Range = 155
            Align = alLeft
            AutoScroll = False
            BorderStyle = bsNone
            Color = clSilver
            ParentColor = False
            TabOrder = 0
            ControlType.Strings = (
              'MENUMATE_INSTALLED;CustomEdit;dbcbMMInstalled'
              'TRIGGER_INSTALLED;CustomEdit;dbcdMMTrigger'
              'ON_INSERT;CustomEdit;dbcbOnInsert'
              'SHOW_DICE;CustomEdit;dbcbShowDice'
              'POINTS_AS_DOLLARS;CustomEdit;dbcbPointsASDollars'
              'CHECK_EXPIRES;CustomEdit;dbcbCheckExpires')
            DataSource = dm.dsrKioskSettings
            EditSpacing.HorizontalView.BetweenRow = 5
            EditSpacing.VerticalView.BetweenRow = 5
            ControlOptions = []
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clWindowText
            LabelFont.Height = -11
            LabelFont.Name = 'MS Sans Serif'
            LabelFont.Style = []
            Selected.Strings = (
              'SHOW_DICE'#9'8'#9'Show Dice'#9'F'
              'POINTS_AS_DOLLARS'#9'8'#9'Points as Dollars'#9'F'
              'KIOSK_TIMER'#9'10'#9'Kiosk Timer Seconds'#9'F'
              'CHECK_EXPIRES'#9'8'#9'Check Before Expires'#9'F'
              'KIOSK_MEMBER_START'#9'10'#9'Member Start Digit'#9'F'
              'KIOSK_MEMBER_LENGHT'#9'10'#9'Member Digit Length'#9'F')
            object dbcbShowDice: TwwDBComboBox
              Left = 112
              Top = 5
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'SHOW_DICE'
              DataSource = dm.dsrKioskSettings
              DropDownCount = 2
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
              Visible = False
            end
            object dbcbPointsASDollars: TwwDBComboBox
              Left = 112
              Top = 29
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'POINTS_AS_DOLLARS'
              DataSource = dm.dsrKioskSettings
              DropDownCount = 2
              Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
              Frame.FocusStyle = efsFrameSunken
              Frame.NonFocusStyle = efsFrameSunken
              Frame.NonFocusTransparentFontColor = clBlack
              ItemHeight = 0
              Items.Strings = (
                'YES'#9'Y'
                'NO'#9'N')
              Sorted = False
              TabOrder = 1
              UnboundDataType = wwDefault
              Visible = False
            end
            object dbcbCheckExpires: TwwDBComboBox
              Left = 112
              Top = 79
              Width = 57
              Height = 19
              ShowButton = True
              Style = csDropDownList
              MapList = True
              AllowClearKey = False
              AutoSize = False
              Color = clWhite
              DataField = 'CHECK_EXPIRES'
              DataSource = dm.dsrKioskSettings
              DropDownCount = 2
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
              Visible = False
            end
          end
        end
      end
    end
    object tsOther: TTabSheet
      Caption = '&Other'
      ImageIndex = 6
      object dbgOther: TwwDBGrid
        Left = 0
        Top = 41
        Width = 556
        Height = 297
        ControlType.Strings = (
          'HAS_ACCESS;CheckBox;Y;N'
          'DEF_ACCESS;CheckBox;Y;N'
          'PRINTER_NAME;CustomEdit;cbPrinters'
          'EMULATION;CustomEdit;cbEmulation'
          'COMM_PORT;CustomEdit;cbCommPort')
        Selected.Strings = (
          'PRINTER_ID'#9'10'#9'PRINTER_ID'
          'DESCRIPTION'#9'20'#9'DESCRIPTION'
          'PRINTER_NAME'#9'32'#9'PRINTER_NAME'
          'PRINTER_INDEX'#9'9'#9'PRINTER_INDEX'
          'PC_NAME'#9'50'#9'PC_NAME'
          'EMULATION'#9'10'#9'EMULATION'
          'COMM_PORT'#9'11'#9'COMM_PORT')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrTblPrinters
        KeyOptions = []
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
        OnCalcCellColors = dbgOtherCalcCellColors
        OnExit = dbgOtherExit
        IndicatorColor = icBlack
      end
      object rgpOther: TLMDRadioGroup
        Left = 0
        Top = 0
        Width = 556
        Height = 41
        Align = alTop
        Alignment = gcaLeftTop
        Bevel.BorderSides = []
        Bevel.BorderWidth = 5
        Bevel.EdgeStyle = etBump
        Bevel.Mode = bmStandard
        Bevel.StandardStyle = lsNone
        CaptionFont.Charset = DEFAULT_CHARSET
        CaptionFont.Color = clBlack
        CaptionFont.Height = -11
        CaptionFont.Name = 'Arial'
        CaptionFont.Style = []
        CaptionParentFont = False
        Columns = 3
        Items.Strings = (
          'Printers / Cash Drawer'
          'Screen Defaults'
          'Version Control')
        ItemOffset = 30
        TabOrder = 1
        OnChange = rgpOtherChange
        ItemIndex = 0
      end
      object cbPrinters: TwwDBComboBox
        Left = 72
        Top = 64
        Width = 153
        Height = 21
        ShowButton = True
        Style = csDropDown
        MapList = False
        AllowClearKey = False
        AutoDropDown = True
        Color = clWhite
        DropDownCount = 8
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ItemHeight = 0
        Sorted = False
        TabOrder = 2
        UnboundDataType = wwDefault
        OnCloseUp = cbPrintersCloseUp
      end
      object cbEmulation: TwwDBComboBox
        Left = 128
        Top = 88
        Width = 97
        Height = 21
        ShowButton = True
        Style = csDropDown
        MapList = True
        AllowClearKey = False
        AutoDropDown = True
        Color = clWhite
        DropDownCount = 8
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ItemHeight = 0
        Items.Strings = (
          'None'#9'0'
          'Epson'#9'1'
          'Star'#9'2')
        Sorted = False
        TabOrder = 3
        UnboundDataType = wwDefault
        OnCloseUp = cbEmulationCloseUp
      end
      object cbCommPort: TwwDBComboBox
        Left = 128
        Top = 120
        Width = 97
        Height = 21
        ShowButton = True
        Style = csDropDown
        MapList = True
        AllowClearKey = False
        AutoDropDown = True
        Color = clWhite
        DropDownCount = 8
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ItemHeight = 0
        Items.Strings = (
          'None'#9'0'
          'Com1'#9'1'
          'Com2'#9'2'
          'Com3'#9'3'
          'Com4'#9'4'
          'Com5'#9'5'
          'Com6'#9'6')
        Sorted = False
        TabOrder = 4
        UnboundDataType = wwDefault
        OnCloseUp = cbCommPortCloseUp
      end
    end
    object tsUsersSecurity: TTabSheet
      Caption = '&Users/Security'
      ImageIndex = 2
      object Splitter1: TSplitter
        Left = 0
        Top = 92
        Width = 556
        Height = 3
        Cursor = crVSplit
        Align = alTop
      end
      object dbgUserSecurity: TwwDBGrid
        Left = 0
        Top = 95
        Width = 556
        Height = 243
        ControlType.Strings = (
          'HAS_ACCESS;CheckBox;Y;N')
        Selected.Strings = (
          'USER_CODE'#9'7'#9'User Code'#9'F'
          'SCREEN_CODE'#9'10'#9'Screen Code'#9'F'
          'SCREEN_DESC'#9'41'#9'Screen Desc'#9'F'
          'HAS_ACCESS'#9'6'#9'Access'#9'F')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrUserSecView
        KeyOptions = []
        Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
        TabOrder = 0
        TitleAlignment = taLeftJustify
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        OnCalcCellColors = dbgUserSecurityCalcCellColors
        OnExit = dbgUserSecurityExit
        IndicatorColor = icBlack
      end
      object dbeName: TwwDBEdit
        Left = 408
        Top = 64
        Width = 57
        Height = 19
        BorderStyle = bsNone
        CharCase = ecUpperCase
        Frame.Enabled = True
        Frame.Transparent = True
        Frame.FocusStyle = efsFrameSingle
        Frame.NonFocusTransparentFontColor = clHighlight
        TabOrder = 1
        UnboundDataType = wwDefault
        Visible = False
        WantReturns = False
        WordWrap = False
      end
      object Panel8: TPanel
        Left = 0
        Top = 0
        Width = 556
        Height = 92
        Align = alTop
        BevelOuter = bvNone
        BorderWidth = 4
        Caption = 'Panel8'
        TabOrder = 2
        object GroupBox2: TGroupBox
          Left = 4
          Top = 4
          Width = 548
          Height = 84
          Align = alClient
          Caption = ' Users '
          TabOrder = 0
          object ListBox: TLMDRadioGroup
            Left = 2
            Top = 15
            Width = 544
            Height = 67
            Hint = 'User Code and User Name'
            Align = alClient
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
            ItemOffset = 10
            TabOrder = 0
            OnChange = ListBoxChange
          end
          object pnlPassword: TPanel
            Left = 2
            Top = 15
            Width = 544
            Height = 67
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            Visible = False
            object Label1: TLabel
              Left = 12
              Top = 7
              Width = 53
              Height = 13
              Caption = 'User Name'
            end
            object Label2: TLabel
              Left = 12
              Top = 34
              Width = 46
              Height = 13
              Caption = 'Password'
            end
            object Label7: TLabel
              Left = 220
              Top = 30
              Width = 46
              Height = 26
              Caption = 'Confirm Password'
              WordWrap = True
            end
            object edtUserName: TEdit
              Left = 78
              Top = 4
              Width = 121
              Height = 21
              CharCase = ecUpperCase
              Color = clWhite
              TabOrder = 0
              OnKeyPress = edtUserNameKeyPress
            end
            object edtConfirmPassword: TEdit
              Left = 286
              Top = 34
              Width = 121
              Height = 21
              CharCase = ecLowerCase
              Color = clWhite
              PasswordChar = '*'
              TabOrder = 2
              OnKeyPress = edtUserNameKeyPress
            end
            object edtPassword: TEdit
              Left = 78
              Top = 32
              Width = 121
              Height = 21
              CharCase = ecLowerCase
              Color = clWhite
              PasswordChar = '*'
              TabOrder = 1
              OnKeyPress = edtUserNameKeyPress
            end
            object btnOK: TButton
              Left = 443
              Top = 0
              Width = 65
              Height = 25
              Caption = 'OK'
              TabOrder = 3
              OnClick = btnOKClick
            end
            object btnCancel: TButton
              Left = 444
              Top = 35
              Width = 65
              Height = 25
              Caption = 'Cancel'
              TabOrder = 4
              OnClick = btnCancelClick
            end
          end
        end
      end
    end
    object tsUserLog: TTabSheet
      Caption = '&Logs'
      ImageIndex = 3
      object Splitter2: TSplitter
        Left = 0
        Top = 129
        Width = 556
        Height = 4
        Cursor = crVSplit
        Align = alTop
      end
      object dbgLogs: TwwDBGrid
        Left = 0
        Top = 133
        Width = 556
        Height = 205
        Selected.Strings = (
          'BEF_POINTS_AVAILABLE'#9'10'#9'BEF_POINTS_AVAILABLE'
          'POINTS_AVAILABLE'#9'10'#9'POINTS_AVAILABLE')
        MemoAttributes = [mSizeable, mWordWrap, mViewOnly, mDisableDialog]
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = True
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrQryPointsLog
        Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgWordWrap]
        ReadOnly = True
        TabOrder = 0
        TitleAlignment = taCenter
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clBlack
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        TitleLines = 1
        TitleButtons = False
        OnCalcCellColors = dbgLogsCalcCellColors
        OnDblClick = dbgLogsDblClick
        IndicatorColor = icBlack
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 556
        Height = 129
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object Panel10: TPanel
          Left = 0
          Top = 0
          Width = 185
          Height = 129
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 5
          Caption = 'Panel10'
          TabOrder = 0
          object GroupBox7: TGroupBox
            Left = 5
            Top = 5
            Width = 175
            Height = 119
            Align = alClient
            TabOrder = 0
            object rgpLog: TLMDRadioGroup
              Left = 2
              Top = 15
              Width = 171
              Height = 102
              Align = alClient
              Alignment = gcaBottomLeft
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
                'Process Log'
                'Backup Log'
                'Points Log')
              ItemOffset = 25
              TabOrder = 0
              OnChange = rgpLogChange
              ItemIndex = 0
            end
          end
        end
        object Panel11: TPanel
          Left = 185
          Top = 0
          Width = 371
          Height = 129
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel11'
          TabOrder = 1
          object Panel9: TPanel
            Left = 0
            Top = 0
            Width = 371
            Height = 59
            Align = alTop
            BevelOuter = bvNone
            BorderWidth = 5
            TabOrder = 0
            object GroupBox5: TGroupBox
              Left = 5
              Top = 5
              Width = 361
              Height = 49
              Align = alClient
              Caption = 'Process Log Options'
              TabOrder = 0
              object cbxAllUsers: TCheckBox
                Left = 30
                Top = 19
                Width = 65
                Height = 17
                Hint = 'Show All Users in the log'
                Caption = 'All Users'
                TabOrder = 0
                OnClick = cbxAllUsersClick
              end
              object dbcbProcess: TwwDBComboBox
                Left = 143
                Top = 19
                Width = 178
                Height = 19
                Hint = 'Break down the log into Menu sections'
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
                ItemIndex = 0
                Sorted = False
                TabOrder = 1
                UnboundDataType = wwDefault
                OnChange = dbcbProcessChange
              end
            end
          end
          object Panel6: TPanel
            Left = 0
            Top = 59
            Width = 371
            Height = 70
            Align = alClient
            BevelOuter = bvNone
            BorderWidth = 5
            TabOrder = 1
            object GroupBox6: TGroupBox
              Left = 5
              Top = 5
              Width = 361
              Height = 60
              Align = alClient
              Caption = 'Users'
              TabOrder = 0
              object LogListBox: TLMDRadioGroup
                Left = 2
                Top = 15
                Width = 357
                Height = 43
                Hint = 'User Code and User Name'
                Align = alClient
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
                ItemOffset = 10
                TabOrder = 0
                OnChange = LogListBoxChange
              end
            end
          end
        end
      end
    end
    object tsBackup: TTabSheet
      Caption = '&Backup'
      ImageIndex = 5
      object GroupBox1: TGroupBox
        Left = 0
        Top = 201
        Width = 556
        Height = 137
        Align = alClient
        Caption = ' Backup Log '
        TabOrder = 0
        object BackupMemo: TMemo
          Left = 2
          Top = 15
          Width = 552
          Height = 120
          Hint = 'Backup Log '
          Align = alClient
          BorderStyle = bsNone
          Color = clSilver
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 0
        Width = 556
        Height = 89
        Align = alTop
        Caption = ' Manual DataBase Backup '
        TabOrder = 1
        object Label15: TLabel
          Left = 32
          Top = 24
          Width = 39
          Height = 13
          Caption = 'Enabled'
        end
        object Label20: TLabel
          Left = 32
          Top = 55
          Width = 82
          Height = 13
          Caption = 'Backup Directory'
        end
        object Label21: TLabel
          Left = 220
          Top = 24
          Width = 24
          Height = 13
          Caption = 'Type'
        end
        object cbxManBackupEnabled: TwwDBComboBox
          Left = 126
          Top = 22
          Width = 50
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = True
          AllowClearKey = False
          AutoDropDown = True
          ShowMatchText = True
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'M_BACKUP_ENABLED'
          DataSource = dm.dsrTblMisc
          DropDownCount = 2
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          Items.Strings = (
            'YES'#9'Y'
            'NO'#9'N')
          Sorted = False
          TabOrder = 0
          UnboundDataType = wwDefault
          OnCloseUp = cbxManBackupEnabledCloseUp
          OnKeyUp = cbxManBackupEnabledKeyUp
        end
        object dirManBackup: TDirectoryEdit
          Left = 125
          Top = 52
          Width = 269
          Height = 21
          Hint = 'Select a Drive and Directory to backup to '
          DialogKind = dkWin32
          Color = clWhite
          NumGlyphs = 1
          TabOrder = 1
          OnChange = dirManBackupChange
        end
        object cbxManBackupType: TwwDBComboBox
          Left = 273
          Top = 22
          Width = 120
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'M_BACKUP_TYPE'
          DataSource = dm.dsrTblMisc
          DropDownCount = 3
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'Internal'#9'0')
          Sorted = False
          TabOrder = 2
          UnboundDataType = wwDefault
        end
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 89
        Width = 556
        Height = 112
        Align = alTop
        Caption = 'Automatic DataBase Backup '
        TabOrder = 2
        object Label16: TLabel
          Left = 28
          Top = 24
          Width = 39
          Height = 13
          Caption = 'Enabled'
        end
        object Label5: TLabel
          Left = 28
          Top = 55
          Width = 24
          Height = 13
          Caption = 'Type'
        end
        object Label6: TLabel
          Left = 163
          Top = 25
          Width = 82
          Height = 13
          Caption = 'Backup Directory'
        end
        object Label8: TLabel
          Left = 220
          Top = 55
          Width = 23
          Height = 13
          Caption = 'Date'
        end
        object Label14: TLabel
          Left = 404
          Top = 55
          Width = 23
          Height = 13
          Caption = 'Time'
        end
        object Label18: TLabel
          Left = 28
          Top = 84
          Width = 143
          Height = 13
          Caption = 'Revolving Backup File Names'
        end
        object Label19: TLabel
          Left = 284
          Top = 84
          Width = 74
          Height = 13
          Caption = 'Keep for ? days'
        end
        object cbxAutoBackupEnabled: TwwDBComboBox
          Left = 85
          Top = 22
          Width = 50
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'A_BACKUP_ENABLED'
          DataSource = dm.dsrTblMisc
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
          OnCloseUp = cbxAutoBackupEnabledCloseUp
          OnKeyUp = cbxAutoBackupEnabledKeyUp
        end
        object cbxAutoBackupType: TwwDBComboBox
          Left = 84
          Top = 53
          Width = 120
          Height = 19
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'A_BACKUP_TYPE'
          DataSource = dm.dsrTblMisc
          DropDownCount = 3
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'Internal'#9'0')
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
        end
        object dtpAutoBackupTime: TwwDBDateTimePicker
          Left = 450
          Top = 53
          Width = 86
          Height = 19
          Hint = 'Enter start backup time - 24 hour format eg 02:30:00'
          BorderStyle = bsNone
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          Color = clWhite
          DataField = 'A_BACKUP_TIME'
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ShowButton = True
          TabOrder = 2
          UnboundDataType = wwDTEdtTime
          DisplayFormat = 'hh:mm:ss'
          OnExit = dtpAutoBackupTimeExit
          OnKeyPress = dtpAutoBackupTimeKeyPress
        end
        object dtpAutoBackupDate: TwwDBDateTimePicker
          Left = 257
          Top = 53
          Width = 120
          Height = 19
          Hint = 'The next Automatic backup date - cannot change it manually'
          BorderStyle = bsNone
          CalendarAttributes.Font.Charset = DEFAULT_CHARSET
          CalendarAttributes.Font.Color = clWindowText
          CalendarAttributes.Font.Height = -11
          CalendarAttributes.Font.Name = 'MS Sans Serif'
          CalendarAttributes.Font.Style = []
          Color = clWhite
          DataField = 'A_BACKUP_DATE'
          DateFormat = dfLong
          DataSource = dm.dsrTblMisc
          Epoch = 1950
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ReadOnly = True
          ShowButton = True
          TabOrder = 3
          DisplayFormat = 'dd mmm yyyy'
        end
        object dirAutoBackup: TDirectoryEdit
          Left = 256
          Top = 22
          Width = 281
          Height = 21
          Hint = 'Select a Drive and Directory to backup to '
          DialogKind = dkWin32
          Color = clWhite
          NumGlyphs = 1
          TabOrder = 4
          OnChange = dirManBackupChange
        end
        object cbxAutoBackupFileNames: TwwDBComboBox
          Left = 205
          Top = 82
          Width = 50
          Height = 19
          Hint = 'Use different Backup file names'
          ShowButton = True
          Style = csDropDownList
          MapList = True
          AllowClearKey = False
          AutoDropDown = True
          ShowMatchText = True
          AutoSize = False
          BorderStyle = bsNone
          Color = clWhite
          DataField = 'A_BACKUP_FILE_NAMES'
          DataSource = dm.dsrTblMisc
          DropDownCount = 2
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          ItemHeight = 0
          Items.Strings = (
            'NO'#9'N'
            'YES'#9'Y')
          Sorted = False
          TabOrder = 5
          UnboundDataType = wwDefault
          OnCloseUp = cbxAutoBackupFileNamesCloseUp
        end
        object dbeAutoBackupKeep: TwwDBEdit
          Left = 376
          Top = 82
          Width = 38
          Height = 19
          Hint = 'Keep Backups for ??? days'
          BiDiMode = bdLeftToRight
          AutoSize = False
          BorderStyle = bsNone
          CharCase = ecUpperCase
          Color = clWhite
          ParentBiDiMode = False
          DataField = 'A_BACKUP_KEEP_DAYS'
          DataSource = dm.dsrTblMisc
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          Frame.NonFocusTransparentFontColor = clBlack
          Picture.PictureMask = '#*#'
          TabOrder = 6
          UnboundDataType = wwDefault
          WantReturns = False
          WordWrap = False
        end
        object btnAutoBackup: TButton
          Left = 484
          Top = 81
          Width = 50
          Height = 20
          Hint = 'Test the Automatic Backup'
          Caption = 'Test'
          TabOrder = 7
          OnClick = menuManualBackupClick
        end
      end
    end
    object tsDaily: TTabSheet
      Caption = '&Daily Jobs'
      ImageIndex = 7
      object GroupBox9: TGroupBox
        Left = 0
        Top = 0
        Width = 556
        Height = 57
        Align = alTop
        TabOrder = 0
        object Label3: TLabel
          Left = 189
          Top = 24
          Width = 199
          Height = 13
          Caption = 'Import Points Information from the Rollover'
        end
        object btnImport: TButton
          Left = 46
          Top = 18
          Width = 87
          Height = 25
          Hint = 'Import Points Information from the Overnight'
          Caption = 'Start Import'
          TabOrder = 0
          OnClick = btnImportClick
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 57
        Width = 556
        Height = 281
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 2
        Caption = 'Panel3'
        TabOrder = 1
        object GroupBox10: TGroupBox
          Left = 2
          Top = 2
          Width = 552
          Height = 277
          Align = alClient
          Caption = 'Rollover Files '
          TabOrder = 0
          object rgpImport: TLMDRadioGroup
            Left = 2
            Top = 15
            Width = 548
            Height = 260
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
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemOffset = 20
            ParentFont = False
            ReadOnly = True
            TabOrder = 0
          end
        end
      end
    end
    object tsInterface: TTabSheet
      Caption = '&Interface'
      ImageIndex = 6
      OnExit = tsInterfaceExit
      object gbxInterface: TGroupBox
        Left = 0
        Top = 0
        Width = 556
        Height = 57
        Align = alTop
        TabOrder = 0
        object Label4: TLabel
          Left = 36
          Top = 24
          Width = 466
          Height = 13
          Caption = 
            '  As long as you are in this tab - you are disabling any Member ' +
            'changes to the POS - exit to resume '
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 57
        Width = 556
        Height = 281
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 2
        TabOrder = 1
        object dbgInterface: TwwDBGrid
          Left = 2
          Top = 2
          Width = 552
          Height = 277
          Hint = 'Double Click to allow this record to be exported'
          ControlType.Strings = (
            'HAS_ACCESS;CheckBox;Y;N')
          Selected.Strings = (
            'MEMBER'#9'13'#9'Member'
            'EXPORT_DATE'#9'28'#9'Export Date'
            'EXPORTED'#9'11'#9'Exported'
            'IN_TRANS'#9'11'#9'In Trans'
            'POINTS_USED'#9'10'#9'Points '
            'RESET_POINTS'#9'10'#9'Reset Points')
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = False
          ShowVertScrollBar = False
          EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
          Align = alClient
          Color = clWhite
          DataSource = dm.dsrExport
          KeyOptions = []
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
          OnCalcCellColors = dbgInterfaceCalcCellColors
          OnDblClick = dbgInterfaceDblClick
          IndicatorColor = icBlack
        end
      end
    end
    object tsMenuMatePoints: TTabSheet
      Caption = 'MenuMate Points'
      ImageIndex = 7
      object dbgMenuMatePoints: TwwDBGrid
        Left = 0
        Top = 57
        Width = 556
        Height = 281
        ControlType.Strings = (
          'HAS_ACCESS;CheckBox;Y;N')
        Selected.Strings = (
          'MEMBER'#9'7'#9'Member'
          'FULL_NAME'#9'19'#9'Full Name'
          'EARNED'#9'7'#9'Earned'
          'REDEEMED'#9'9'#9'Redeemed'
          'ADJUSTMENT_TYPE'#9'7'#9'Adj Type'
          'POINTS_TYPE'#9'7'#9'Pts Type'
          'TIME_STAMP'#9'11'#9'Date Time'
          'INVOICE_NUMBER'#9'16'#9'Invoice #')
        IniAttributes.Delimiter = ';;'
        TitleColor = clBtnFace
        FixedCols = 0
        ShowHorzScrollBar = False
        ShowVertScrollBar = False
        EditControlOptions = [ecoCheckboxSingleClick, ecoSearchOwnerForm]
        Align = alClient
        Color = clWhite
        DataSource = dm.dsrMenuMatePoints
        KeyOptions = []
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
        IndicatorColor = icBlack
      end
      object GroupBox13: TGroupBox
        Left = 0
        Top = 0
        Width = 556
        Height = 57
        Align = alTop
        TabOrder = 1
        object btnMMPoints: TButton
          Left = 46
          Top = 18
          Width = 87
          Height = 25
          Hint = 'Update points from MenuMate'
          Caption = 'Update Points'
          TabOrder = 0
          OnClick = btnMMPointsClick
        end
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 564
    TabOrder = 2
    inherited ToolBar: TToolBar
      Width = 547
      ButtonHeight = 43
      inherited pnlFill: TPanel
        Height = 43
      end
      object btnBackup: TToolButton
        Left = 3
        Top = 0
        Hint = 'Start the Manual Backup'
        Caption = 'btnBackup'
        ImageIndex = 25
        OnClick = menuManualBackupClick
      end
      object btnAddUser: TToolButton
        Left = 46
        Top = 0
        Hint = 'Add a User'
        Caption = 'btnAddUser'
        ImageIndex = 27
        OnClick = menuAddUserClick
      end
      object btnChangeUser: TToolButton
        Left = 89
        Top = 0
        Hint = 'Change a User'#39's Password'
        Caption = 'btnChangeUser'
        ImageIndex = 26
        OnClick = menuChangeUserClick
      end
      object btnDeleteUser: TToolButton
        Left = 132
        Top = 0
        Hint = 'Delete a User'
        Caption = 'btnDeleteUser'
        ImageIndex = 28
        OnClick = menuDeleteUserClick
      end
      object dbnNavRefresh: TwwDBNavigator
        Left = 175
        Top = 0
        Width = 50
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
        object dbnRefresh: TwwNavButton
          Left = 0
          Top = 0
          Width = 40
          Height = 43
          Hint = 'Refresh the data'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnRefresh'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnRefreshClick
          Index = 0
          Style = nbsRefresh
        end
      end
      object pnlSpace: TPanel
        Left = 225
        Top = 0
        Width = 273
        Height = 43
        BevelOuter = bvNone
        TabOrder = 2
      end
      object btnClose: TToolButton
        Left = 498
        Top = 0
        Caption = 'btnClose'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 330
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuOptions: TMenuItem [1]
      Caption = '&Options'
      object menuManualBackup: TMenuItem
        Caption = 'Manual &Backup'
        ShortCut = 16450
        OnClick = menuManualBackupClick
      end
      object break1: TMenuItem
        Caption = '-'
      end
      object menuAddUser: TMenuItem
        Caption = 'Add User'
        OnClick = menuAddUserClick
      end
      object menuChangeUser: TMenuItem
        Caption = 'Change User Password'
        OnClick = menuChangeUserClick
      end
      object menuDeleteUser: TMenuItem
        Caption = 'Delete User'
        OnClick = menuDeleteUserClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object MenuMatePointsLog1: TMenuItem
        Caption = 'MenuMate Points Log'
      end
    end
  end
  object popMenu: TPopupMenu
    Left = 292
    Top = 25
    object Applicationtoall: TMenuItem
      Caption = 'Membership to all PC'#39's'
      OnClick = ApplicationtoallClick
    end
    object Applicationto1: TMenuItem
      Caption = 'Membership to selected PC '
      OnClick = Applicationto1Click
    end
    object Helptoall: TMenuItem
      Caption = 'Help to all PC'#39's'
      OnClick = HelptoallClick
    end
    object HelpTo1: TMenuItem
      Caption = 'Help to 1'
      OnClick = HelpTo1Click
    end
    object ClearAllUsers1: TMenuItem
      Caption = 'Clear All Users'
      OnClick = ClearAllUsers1Click
    end
  end
  object FieldsDialog: TDualListDialog
    Sorted = False
    Title = 'Select Member Fields for Export'
    Label1Caption = 'Member Fields'
    Label2Caption = 'Export in order of list'
    HelpContext = 0
    ShowHelp = False
    Left = 512
    Top = 56
  end
  object ibdsMembers: TIBDataSet
    Database = dm.IBDatabase
    Transaction = dm.IBRunOnce
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into MEMBERS'
      '  (MEMBER, INITIALS, TITLE, FIRST_NAME, SURNAME, FULL_NAME)'
      'values'
      
        '  (:MEMBER, :INITIALS, :TITLE, :FIRST_NAME, :SURNAME, :FULL_NAME' +
        ')')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  TITLE,'
      '  INITIALS,'
      '  FIRST_NAME,'
      '  SURNAME,'
      '  FULL_NAME'
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'Select MEMBER,INITIALS,TITLE,FIRST_NAME,SURNAME,FULL_NAME from M' +
        'EMBERS order by MEMBER'
      '')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  MEMBER = :MEMBER,'
      '  INITIALS = :INITIALS,'
      '  TITLE = :TITLE,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  SURNAME = :SURNAME,'
      '  FULL_NAME = :FULL_NAME'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 440
    Top = 16
    object ibdsMembersMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'MEMBERS.MEMBER'
      Required = True
    end
    object ibdsMembersFIRST_NAME: TIBStringField
      FieldName = 'FIRST_NAME'
      Origin = 'MEMBERS.FIRST_NAME'
    end
    object ibdsMembersSURNAME: TIBStringField
      FieldName = 'SURNAME'
      Origin = 'MEMBERS.SURNAME'
      Size = 25
    end
    object ibdsMembersFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = 'MEMBERS.FULL_NAME'
      Size = 45
    end
    object ibdsMembersINITIALS: TIBStringField
      FieldName = 'INITIALS'
      Origin = 'MEMBERS.INITIALS'
      FixedChar = True
      Size = 6
    end
    object ibdsMembersTITLE: TIBStringField
      FieldName = 'TITLE'
      Origin = 'MEMBERS.TITLE'
      FixedChar = True
      Size = 6
    end
  end
  object IBDataSet: TIBDataSet
    Database = dm.IBDatabase
    Transaction = dm.IBRunOnce
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_SUBS'
      'where'
      '  MEMBER = :OLD_MEMBER and'
      '  SUBSCRIPTION = :OLD_SUBSCRIPTION')
    InsertSQL.Strings = (
      'insert into M_SUBS'
      '  (AMOUNT, AMOUNT_PAID, EDIT_DATE, INVOICED, MEMBER, PAID, PAY, '
      'PRINT_CARD, '
      '   PRINT_RECEIPT, RECEIPT, SUBSCRIPTION)'
      'values'
      
        '  (:AMOUNT, :AMOUNT_PAID, :EDIT_DATE, :INVOICED, :MEMBER, :PAID,' +
        ' :PAY, '
      '   :PRINT_CARD, :PRINT_RECEIPT, :RECEIPT, :SUBSCRIPTION)')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  SUBSCRIPTION,'
      '  AMOUNT_PAID,'
      '  AMOUNT,'
      '  PAID,'
      '  EDIT_DATE,'
      '  RECEIPT,'
      '  PAY,'
      '  INVOICED,'
      '  PRINT_CARD,'
      '  PRINT_RECEIPT'
      'from M_SUBS '
      'where'
      '  MEMBER = :MEMBER and'
      '  SUBSCRIPTION = :SUBSCRIPTION')
    SelectSQL.Strings = (
      
        'select AMOUNT, AMOUNT_PAID, EDIT_DATE, INVOICED, MEMBER, PAID, P' +
        'AY, PRINT_CARD, PRINT_RECEIPT, RECEIPT, SUBSCRIPTION from M_SUBS' +
        ' where MEMBER = :pMEMBER')
    ModifySQL.Strings = (
      'update M_SUBS'
      'set'
      '  AMOUNT = :AMOUNT,'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  INVOICED = :INVOICED,'
      '  MEMBER = :MEMBER,'
      '  PAID = :PAID,'
      '  PAY = :PAY,'
      '  PRINT_CARD = :PRINT_CARD,'
      '  PRINT_RECEIPT = :PRINT_RECEIPT,'
      '  RECEIPT = :RECEIPT,'
      '  SUBSCRIPTION = :SUBSCRIPTION'
      'where'
      '  MEMBER = :OLD_MEMBER and'
      '  SUBSCRIPTION = :OLD_SUBSCRIPTION')
    Left = 384
    Top = 16
    object IBDataSetAMOUNT: TIBBCDField
      FieldName = 'AMOUNT'
      Origin = '"M_SUBS"."AMOUNT"'
      Precision = 9
      Size = 2
    end
    object IBDataSetAMOUNT_PAID: TIBBCDField
      FieldName = 'AMOUNT_PAID'
      Origin = '"M_SUBS"."AMOUNT_PAID"'
      Precision = 9
      Size = 2
    end
    object IBDataSetEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Origin = '"M_SUBS"."EDIT_DATE"'
    end
    object IBDataSetINVOICED: TIBStringField
      FieldName = 'INVOICED'
      Origin = '"M_SUBS"."INVOICED"'
      FixedChar = True
      Size = 1
    end
    object IBDataSetMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"M_SUBS"."MEMBER"'
      Required = True
    end
    object IBDataSetPAID: TIBStringField
      FieldName = 'PAID'
      Origin = '"M_SUBS"."PAID"'
      FixedChar = True
      Size = 1
    end
    object IBDataSetPAY: TIBBCDField
      FieldName = 'PAY'
      Origin = '"M_SUBS"."PAY"'
      Precision = 9
      Size = 2
    end
    object IBDataSetPRINT_CARD: TIBStringField
      FieldName = 'PRINT_CARD'
      Origin = '"M_SUBS"."PRINT_CARD"'
      FixedChar = True
      Size = 1
    end
    object IBDataSetPRINT_RECEIPT: TIBStringField
      FieldName = 'PRINT_RECEIPT'
      Origin = '"M_SUBS"."PRINT_RECEIPT"'
      FixedChar = True
      Size = 1
    end
    object IBDataSetRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
      Origin = '"M_SUBS"."RECEIPT"'
    end
    object IBDataSetSUBSCRIPTION: TIntegerField
      FieldName = 'SUBSCRIPTION'
      Origin = '"M_SUBS"."SUBSCRIPTION"'
      Required = True
    end
  end
  object CalendarDialog: TOvcCalendarDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [doSizeable]
    Placement.Height = 200
    Placement.Width = 225
    Left = 256
    Top = 16
  end
end
