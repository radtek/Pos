object frmGeneralMaintenance: TfrmGeneralMaintenance
  Tag = 1024
  Left = 0
  Top = 4
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Settings'
  ClientHeight = 768
  ClientWidth = 1009
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  WindowState = wsMaximized
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    1009
    768)
  PixelsPerInch = 96
  TextHeight = 18
  object pnlButtons: TPanel
    Left = 861
    Top = 4
    Width = 135
    Height = 768
    Alignment = taRightJustify
    Anchors = [akTop, akRight, akBottom]
    BorderStyle = bsSingle
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object tbPOS: TTouchBtn
      Left = 5
      Top = 80
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'P.O.S Operation'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbPOSClick
    end
    object tbPOSDisplay: TTouchBtn
      Left = 5
      Top = 152
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'P.O.S Display'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbPOSDisplayClick
    end
    object tbtnStaffOptions: TTouchBtn
      Left = 5
      Top = 224
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Staff'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbStaffClick
    end
    object tbtnMembershipOpts: TTouchBtn
      Left = 5
      Top = 296
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Membership'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbMembershipClick
    end
    object tbtnEftposOpts: TTouchBtn
      Left = 5
      Top = 368
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Eftpos'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbEftposClick
    end
    object TouchBtn17: TTouchBtn
      Left = 5
      Top = 8
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
    end
    object tbtnLoggingOpts: TTouchBtn
      Left = 5
      Top = 440
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Logging'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbLoggingClick
    end
    object tbEndOfDay: TTouchBtn
      Left = 5
      Top = 512
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'End Of Day'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbEndOfDayClick
    end
    object tbCustomerDisplay: TTouchBtn
      Left = 5
      Top = 584
      Width = 112
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Customer Display'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbCustomerDisplayClick
    end
  end
  object Pages: TPageControl
    Left = 4
    Top = 4
    Width = 850
    Height = 768
    ActivePage = tsEndOfDayOptions
    TabOrder = 1
    object tsPointOfSale: TTabSheet
      Caption = 'Point Of Sale.'
      TabVisible = False
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox10: TGroupBox
          Left = 8
          Top = 49
          Width = 433
          Height = 670
          Caption = 'P.O.S Options'
          ParentBackground = False
          TabOrder = 11
          object Label4: TLabel
            Left = 16
            Top = 154
            Width = 156
            Height = 18
            Caption = 'Max non service period.'
          end
          object Label13: TLabel
            Left = 280
            Top = 283
            Width = 119
            Height = 18
            Caption = 'MenuMate Subnet'
          end
          object Label15: TLabel
            Left = 12
            Top = 413
            Width = 112
            Height = 18
            Caption = 'Tab Credit Group'
          end
          object cbRevertCourse: TCheckBox
            Left = 8
            Top = 20
            Width = 297
            Height = 25
            Caption = 'Return to first course after sale'
            TabOrder = 0
            OnClick = cbRevertCourseClick
          end
          object cbForceTender: TCheckBox
            Left = 8
            Top = 45
            Width = 297
            Height = 25
            Caption = 'Force tender sales.'
            TabOrder = 2
            OnClick = cbForceTenderClick
          end
          object cbFTEFTSales: TCheckBox
            Left = 8
            Top = 70
            Width = 313
            Height = 25
            Caption = 'Do not force tender on EFT sales.'
            TabOrder = 1
            OnClick = cbFTEFTSalesClick
          end
          object cbCaptureCustomerName: TCheckBox
            Left = 8
            Top = 95
            Width = 353
            Height = 25
            Caption = 'Capture Customer Name'
            TabOrder = 12
            OnClick = cbCaptureCustomerNameClick
          end
          object cbHoldSend: TCheckBox
            Left = 8
            Top = 120
            Width = 353
            Height = 25
            Caption = 'Use Hold and Send Function'
            TabOrder = 11
            OnClick = cbHoldSendClick
          end
          object btnNonService: TTouchBtn
            Left = 249
            Top = 145
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '120'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnNonServiceClick
          end
          object cbForcedSidesFirst: TCheckBox
            Left = 8
            Top = 180
            Width = 385
            Height = 25
            Caption = 'Show Forced Sides before Forced Options'
            TabOrder = 5
            OnClick = cbForcedSidesFirstClick
          end
          object cbForcedReasons: TCheckBox
            Left = 8
            Top = 205
            Width = 409
            Height = 25
            Caption = 'Force Reasons for credit, cancels and discounts.'
            TabOrder = 3
            OnClick = cbForcedReasonsClick
          end
          object cbPromptPatronCount: TCheckBox
            Left = 8
            Top = 230
            Width = 401
            Height = 25
            Caption = 'Prompt For Patron Count.'
            TabOrder = 6
            OnClick = cbPromptPatronCountClick
          end
          object cbPromptPatronCountOnTableSales: TCheckBox
            Left = 8
            Top = 255
            Width = 401
            Height = 25
            Caption = 'Prompt For Patron Count On Table Sales Only.'
            TabOrder = 7
            OnClick = cbPromptPatronCountOnTableSalesClick
          end
          object cbRounding: TCheckBox
            Left = 8
            Top = 280
            Width = 401
            Height = 25
            Caption = 'Rounding Mid point rounds down.'
            TabOrder = 8
            OnClick = cbRoundingClick
          end
          object GroupBox11: TGroupBox
            Left = 3
            Top = 320
            Width = 249
            Height = 81
            TabOrder = 4
            object Label5: TLabel
              Left = 24
              Top = 40
              Width = 32
              Height = 18
              Caption = 'After'
            end
            object Label6: TLabel
              Left = 168
              Top = 40
              Width = 53
              Height = 18
              Caption = 'seconds'
            end
            object Label7: TLabel
              Left = 25
              Top = 0
              Width = 121
              Height = 18
              Caption = 'Auto logout in POS'
              OnClick = lbeUseAutoLogoutClick
            end
            object chbUseAutoLogout: TCheckBox
              Left = 8
              Top = 0
              Width = 17
              Height = 17
              TabOrder = 0
              OnClick = chbUseAutoLogoutClick
            end
            object btnAutoLogoutTime: TTouchBtn
              Left = 72
              Top = 24
              Width = 89
              Height = 49
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = '120'
              ButtonColor = 9933970
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = btnAutoLogoutTimeClick
            end
          end
          object tbtnMMSubNet: TTouchBtn
            Left = 296
            Top = 311
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnMMSubNetClick
          end
          object tbtnTabCreditGroup: TTouchBtn
            Left = 296
            Top = 375
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
          end
          object cbDisableSaveConfirmation: TCheckBox
            Left = 8
            Top = 435
            Width = 377
            Height = 25
            Caption = 'Disable Confirmation on Save'
            TabOrder = 9
            OnClick = cbDisableSaveConfirmationClick
          end
          object cbDisableReceiptOnConfirmation: TCheckBox
            Left = 8
            Top = 460
            Width = 377
            Height = 25
            Caption = 'Disable Receipt on Confirmation'
            TabOrder = 10
            OnClick = cbDisableReceiptOnConfirmationClick
          end
          object cbShowVoucherDetails: TCheckBox
            Left = 8
            Top = 485
            Width = 377
            Height = 25
            Caption = 'Show Voucher Details'
            TabOrder = 15
            OnClick = cbEnableShowVoucherDetails
          end
          object cbEnablePhoneOrders: TCheckBox
            Left = 8
            Top = 510
            Width = 377
            Height = 25
            Caption = 'Enable Phone Orders'
            TabOrder = 17
            OnClick = cbEnablePhoneOrdersMouseClick
          end
          object cbNagUserToSelectChit: TCheckBox
            Left = 8
            Top = 535
            Width = 412
            Height = 25
            Caption = 'Prompt for a chit on every new order'
            TabOrder = 16
            OnClick = cbNagUserToSelectChitClicked
          end
          object cbUseMenuDefaultPatronCount: TCheckBox
            Left = 8
            Top = 560
            Width = 412
            Height = 25
            Caption = 'Calculate default patron count from menu'
            TabOrder = 19
            OnClick = cbUseMenuDefaultPatronCountClicked
          end
          object cbEnableTableMode: TCheckBox
            Left = 8
            Top = 585
            Width = 412
            Height = 25
            Caption = 'Enable Table Display On Start-up'
            TabOrder = 18
            OnClick = cbEnableTableModeClick
          end
          object cbEnableParkedSales: TCheckBox
            Left = 8
            Top = 610
            Width = 412
            Height = 25
            Caption = 'Enable Parked Sales'
            TabOrder = 20
            OnClick = cbEnableParkedSalesClicked
          end
          object cbNotifyForManuallyEnteredWeight: TCheckBox
            Left = 8
            Top = 635
            Width = 412
            Height = 25
            Caption = 'Notify for Manually Entered Weight'
            TabOrder = 21
            OnClick = cbNotifyForManuallyEnteredWeightClicked
          end
        end
        object GroupBox5: TGroupBox
          Left = 448
          Top = 49
          Width = 401
          Height = 50
          Caption = 'Production Tracking'
          ParentBackground = False
          TabOrder = 0
          object cbSaleAndMakeTimes: TCheckBox
            Left = 8
            Top = 21
            Width = 361
            Height = 25
            Caption = 'Enable Sale && Make Time Tracking.'
            TabOrder = 0
            OnClick = cbSaleAndMakeTimesClick
          end
        end
        object GroupBox6: TGroupBox
          Left = 447
          Top = 101
          Width = 401
          Height = 105
          Caption = 'Storage Settings'
          ParentBackground = False
          TabOrder = 1
          object lbeSeatLabel: TLabel
            Left = 8
            Top = 71
            Width = 67
            Height = 18
            Caption = 'Seat Label'
          end
          object edSeatLabel: TEdit
            Left = 96
            Top = 71
            Width = 210
            Height = 26
            MaxLength = 6
            TabOrder = 0
            OnMouseUp = edSeatLabelMouseUp
          end
          object cbTableSeats: TCheckBox
            Left = 8
            Top = 48
            Width = 297
            Height = 17
            Caption = 'Enable Table Seat Interface'
            TabOrder = 1
            OnClick = cbTableSeatsClick
          end
          object cbTabsInterface: TCheckBox
            Left = 8
            Top = 24
            Width = 297
            Height = 17
            Caption = 'Enable Tabs Interface'
            TabOrder = 2
            OnClick = cbTabsInterfaceClick
          end
        end
        object GroupBox21: TGroupBox
          Left = 447
          Top = 215
          Width = 401
          Height = 49
          Caption = 'Invoicing'
          TabOrder = 4
          object cbDisableInternalInvoicing: TCheckBox
            Left = 8
            Top = 21
            Width = 250
            Height = 20
            Caption = 'Disable internal invoicing'
            TabOrder = 0
            OnClick = cbDisableInternalInvoicingClick
          end
        end
        object cbDisplayTaxExempt: TCheckBox
          Left = 455
          Top = 270
          Width = 377
          Height = 25
          Caption = 'Enable Tax Removal'
          TabOrder = 3
          OnClick = cbDisplayTaxExemptClick
        end
        object cbSkipConfirmationOnFastTender: TCheckBox
          Left = 455
          Top = 295
          Width = 412
          Height = 25
          Caption = 'Skip Confirmation on Fast Tender Shortcuts'
          TabOrder = 7
          OnClick = cbSkipConfirmationOnFastTenderClick
        end
        object cbRoundOnBilling: TCheckBox
          Left = 455
          Top = 370
          Width = 412
          Height = 25
          Caption = 'Round On Billing'
          TabOrder = 9
          OnClick = cbRoundOnBillingClick
        end
        object tbRoundingOnBilling: TTouchBtn
          Left = 768
          Top = 370
          Width = 80
          Height = 33
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = '0.01'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbRoundingOnBillingClick
        end
        object cbTransferTableOnPrintPrelim: TCheckBox
          Left = 455
          Top = 320
          Width = 412
          Height = 25
          Caption = 'Delayed Payment'
          TabOrder = 8
          OnClick = cbTransferTableOnPrintPrelimClick
        end
        object cbEnableCancelCheckRemoval: TCheckBox
          Left = 455
          Top = 345
          Width = 454
          Height = 25
          Caption = 'Enable Security Check on Cancel at Payment Tender'
          TabOrder = 6
          OnClick = cbEnableCancelCheckRemovalClick
        end
        object Panel2: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'POS Operation'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
        end
        object GroupBox19: TGroupBox
          Left = 447
          Top = 410
          Width = 401
          Height = 117
          Caption = 'Scales'
          ParentBackground = False
          TabOrder = 5
          object setWeightLimit: TLabel
            Left = 27
            Top = 85
            Width = 105
            Height = 18
            Caption = 'Set Weight Limit'
          end
          object cbSacleAutoConvertWeights: TCheckBox
            Left = 8
            Top = 15
            Width = 361
            Height = 25
            Caption = 'Allow Auto conversion of weights.'
            TabOrder = 0
            OnClick = cbSacleAutoConvertWeightsClick
          end
          object cbScaleEnterWeight: TCheckBox
            Left = 8
            Top = 36
            Width = 361
            Height = 25
            Caption = 'Allow to enter weight.'
            TabOrder = 1
            OnClick = cbScaleEnterWeightClick
          end
          object cbScaleEnterPrice: TCheckBox
            Left = 8
            Top = 58
            Width = 361
            Height = 25
            Caption = 'Allow to enter price.'
            TabOrder = 2
            OnClick = cbScaleEnterPriceClick
          end
          object tbEnterWeight: TTouchBtn
            Left = 250
            Top = 80
            Width = 150
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0.00'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbEnterWeightClick
          end
        end
        object GroupBox22: TGroupBox
          Left = 447
          Top = 535
          Width = 401
          Height = 49
          Caption = 'Bike Shop'
          TabOrder = 13
          object cbDeleteItemSizeAfterSale: TCheckBox
            Left = 8
            Top = 21
            Width = 250
            Height = 20
            Caption = 'Delete ItemSize After Sale'
            TabOrder = 0
            OnClick = cbDeleteItemSizeAfterSaleClick
          end
        end
        object GroupBox23: TGroupBox
          Left = 447
          Top = 600
          Width = 401
          Height = 49
          Caption = 'Item Search'
          TabOrder = 10
          object cbItemSearch: TCheckBox
            Left = 8
            Top = 21
            Width = 250
            Height = 20
            Caption = 'Enable Item Search'
            TabOrder = 0
            OnClick = cbItemSearchClick
          end
        end
      end
    end
    object tsPointOfSaleDisplay: TTabSheet
      Caption = 'Point Of Sale Display.'
      TabVisible = False
      object Panel14: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox12: TGroupBox
          Left = 8
          Top = 48
          Width = 401
          Height = 385
          Caption = 'POS Display Settings'
          ParentBackground = False
          TabOrder = 0
          object cbHideServingCourseLabels: TCheckBox
            Left = 8
            Top = 18
            Width = 369
            Height = 25
            Caption = 'Hide Serving Course Labels'
            TabOrder = 0
            OnClick = cbHideServingCourseLabelsClick
          end
          object cbRememberLastServingCourse: TCheckBox
            Left = 8
            Top = 39
            Width = 377
            Height = 25
            Caption = 'Remember Last Serving Course (Hide Popup)'
            TabOrder = 1
            OnClick = cbRememberLastServingCourseClick
          end
          object cbDefaultToBevMenu: TCheckBox
            Left = 8
            Top = 60
            Width = 377
            Height = 25
            Caption = 'POS Displays Beverage Menu First'
            TabOrder = 2
            OnClick = cbDefaultToBevMenuClick
          end
          object cbHideFreeItemsAtBilling: TCheckBox
            Left = 8
            Top = 81
            Width = 377
            Height = 25
            Caption = 'Move Free Items To Bottom At Billing'
            TabOrder = 3
            OnClick = cbHideFreeItemsAtBillingClick
          end
          object cbEnableWaiterStation: TCheckBox
            Left = 8
            Top = 102
            Width = 377
            Height = 25
            Caption = 'Waiter Station'
            TabOrder = 4
            OnClick = cbEnableWaiterStationClick
          end
          object cbEnableNmiDisplay: TCheckBox
            Left = 8
            Top = 123
            Width = 377
            Height = 25
            Caption = 'Show weight in POS'
            TabOrder = 5
            OnClick = cbEnableNmiDisplayClick
          end
          object cbEnableExtendedDisplay: TCheckBox
            Left = 8
            Top = 144
            Width = 377
            Height = 25
            Caption = 'Display 14 characters in currency'
            TabOrder = 6
            OnClick = cbEnableExtendedDisplayClicked
          end
          object cbDisplayTax: TCheckBox
            Left = 8
            Top = 165
            Width = 377
            Height = 25
            Caption = 'Display Tax'
            TabOrder = 7
            OnClick = cbDisplayTaxClick
          end
          object cbDisplayServiceCharge: TCheckBox
            Left = 8
            Top = 186
            Width = 377
            Height = 25
            Caption = 'Display Service Charge'
            TabOrder = 8
            OnClick = cbDisplayServiceChargeClick
          end
          object cbInclServiceChargeTaxInTaxVal: TCheckBox
            Left = 8
            Top = 207
            Width = 377
            Height = 25
            Caption = 'Include Service Charge Tax in Tax Value'
            TabOrder = 9
            OnClick = cbInclServiceChargeTaxInTaxValClick
          end
          object cbUpdateTableGUIOnOrderStatus: TCheckBox
            Left = 8
            Top = 228
            Width = 377
            Height = 25
            Caption = 'Update Table GUI Based On Order Status'
            TabOrder = 11
            OnClick = cbUpdateTableGUIOnOrderStatusClick
          end
          object cbShowCurrencySymbol: TCheckBox
            Left = 8
            Top = 249
            Width = 377
            Height = 25
            Caption = 'Show Currency Symbol in POS'
            TabOrder = 12
            OnClick = cbShowCurrencySymbolClick
          end
          object cbDisplayItemSizesAsList: TCheckBox
            Left = 8
            Top = 270
            Width = 377
            Height = 25
            Caption = 'Display Item Sizes As List in POS'
            TabOrder = 14
            OnClick = cbDisplayItemSizesAsListClick
          end
          object cbShowItemPricesInTransferScreen: TCheckBox
            Left = 8
            Top = 312
            Width = 377
            Height = 25
            Caption = 'Show Prices on Transfer Screen'
            TabOrder = 13
            OnClick = cbShowItemPricesInTransferScreenClick
          end
          object cbShowLargeFonts: TCheckBox
            Left = 9
            Top = 330
            Width = 377
            Height = 25
            Caption = 'Show large fonts on Menu/Courses/Table Name'
            TabOrder = 16
            OnClick = cbShowLargeFontsClick
          end
          object cbShowDarkBackgroundInPOS: TCheckBox
            Left = 9
            Top = 351
            Width = 377
            Height = 25
            Caption = 'Show Dark Background in POS'
            TabOrder = 15
            OnClick = cbShowDarkBackgroundInPOSClick
          end
          object cbTerminalExemptFromHappyHour: TCheckBox
            Left = 9
            Top = 290
            Width = 377
            Height = 25
            Caption = 'Terminal Exempt From Happy Hour'
            TabOrder = 10
            OnClick = cbTerminalExemptFromHappyHourClick
          end
        end
        object rgPOSBtnSize: TRadioGroup
          Left = 8
          Top = 439
          Width = 401
          Height = 100
          Caption = 'POS button sizes'
          Items.Strings = (
            '4x8'
            '5x9'
            '6x10'
            '7x11')
          TabOrder = 1
          OnClick = rgPOSBtnSizeClick
        end
        object btnSetPOSFontSize: TTouchBtn
          Left = 200
          Top = 462
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Set font size'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnSetPOSFontSizeMouseClick
        end
        object Panel15: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'POS Display'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 3
        end
      end
    end
    object tsStaffOptions: TTabSheet
      Caption = 'Staff Options.'
      ImageIndex = 2
      TabVisible = False
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox8: TGroupBox
          Left = 8
          Top = 48
          Width = 409
          Height = 65
          Caption = 'Login'
          TabOrder = 5
          object cbStaffDisableManualLogin: TCheckBox
            Left = 8
            Top = 32
            Width = 289
            Height = 20
            Caption = 'Disable Staff Manual Login'
            TabOrder = 0
            OnClick = cbStaffDisableManualLoginClick
          end
        end
        object rgRoundingTimes: TRadioGroup
          Left = 8
          Top = 325
          Width = 409
          Height = 200
          Caption = 'Round to nearest times'
          Items.Strings = (
            '0 minutes'
            '1 minute'
            '5 minutes'
            '10 minutes'
            '15 minutes'
            '30 minutes'
            '1 hour')
          ParentBackground = False
          TabOrder = 4
          OnClick = rgRoundingTimesClick
        end
        object groupBoxStaffSettings: TGroupBox
          Left = 8
          Top = 535
          Width = 409
          Height = 85
          Caption = 'Settings'
          TabOrder = 0
          object cbEnableApprovedLogin: TCheckBox
            Left = 8
            Top = 24
            Width = 289
            Height = 17
            Caption = 'Enable Approved Login'
            TabOrder = 1
            OnClick = cbEnableApprovedLoginClick
          end
          object cbLinkClockInOutApprovedLogin: TCheckBox
            Left = 8
            Top = 48
            Width = 289
            Height = 17
            Caption = 'Link Clock in/out to Approved Login'
            TabOrder = 0
            OnClick = cbLinkClockInOutApprovedLoginClick
          end
        end
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Staff'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
        end
        object GroupBox17: TGroupBox
          Left = 9
          Top = 119
          Width = 408
          Height = 199
          Caption = ' Staff Message (Click) '
          TabOrder = 2
          object memStaffMessage: TMemo
            Left = 2
            Top = 20
            Width = 404
            Height = 177
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = memStaffMessageClick
          end
        end
        object GroupBox18: TGroupBox
          Left = 424
          Top = 48
          Width = 425
          Height = 657
          Caption = 'Time Clock Departments'
          TabOrder = 3
          object tbtnAddDept: TTouchBtn
            Left = 287
            Top = 41
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Add Department'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnAddDeptMouseClick
          end
          object tbgTimeCodes: TTouchGrid
            Left = 2
            Top = 20
            Width = 279
            Height = 635
            Align = alLeft
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            BevelOuter = bvNone
            DefaultButtonColor = clNavy
            DefaultButtonLatchedColor = clMaroon
            LatchingType = ltRadioButton
            LatchingProperties = [lkLatchColor, lkStayDown]
            ParentGridColor = False
            GridColor = 14342874
            DefaultColWidth = 120
            DefaultRowHeight = 55
            ButtonGapHeight = 15
            AutoButtonWidth = True
            VertScrollButtonPos = vsBelow
            VertScrollButtonHeight = 55
            HorizScrollButtonWidth = 70
            ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
            ScrollBtnUp.Font.Color = clWindowText
            ScrollBtnUp.Font.Height = -11
            ScrollBtnUp.Font.Name = 'MS Sans Serif'
            ScrollBtnUp.Font.Style = []
            ScrollBtnUp.ParentFont = False
            ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnUp.LatchedFont.Color = clWindowText
            ScrollBtnUp.LatchedFont.Height = -11
            ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnUp.LatchedFont.Style = []
            ScrollBtnUp.ParentLatchedFontColor = False
            ScrollBtnUp.GlyphAlpha = 255
            ScrollBtnUp.Color = 9933970
            ScrollBtnUp.FontColor = 16777215
            ScrollBtnUp.LatchedColor = -2147483633
            ScrollBtnUp.DisabledColor = -2147483633
            ScrollBtnUp.LatchedFontColor = -2147483640
            ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
            ScrollBtnDown.Font.Color = clWindowText
            ScrollBtnDown.Font.Height = -11
            ScrollBtnDown.Font.Name = 'MS Sans Serif'
            ScrollBtnDown.Font.Style = []
            ScrollBtnDown.ParentFont = False
            ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnDown.LatchedFont.Color = clWindowText
            ScrollBtnDown.LatchedFont.Height = -11
            ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnDown.LatchedFont.Style = []
            ScrollBtnDown.ParentLatchedFontColor = False
            ScrollBtnDown.GlyphAlpha = 255
            ScrollBtnDown.Color = 9933970
            ScrollBtnDown.FontColor = 16777215
            ScrollBtnDown.LatchedColor = -2147483633
            ScrollBtnDown.DisabledColor = -2147483633
            ScrollBtnDown.LatchedFontColor = -2147483640
            ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.Font.Color = clWindowText
            ScrollBtnLeft.Font.Height = -11
            ScrollBtnLeft.Font.Name = 'MS Sans Serif'
            ScrollBtnLeft.Font.Style = []
            ScrollBtnLeft.ParentFont = False
            ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.LatchedFont.Color = clWindowText
            ScrollBtnLeft.LatchedFont.Height = -11
            ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnLeft.LatchedFont.Style = []
            ScrollBtnLeft.ParentLatchedFontColor = False
            ScrollBtnLeft.GlyphAlpha = 255
            ScrollBtnLeft.Color = -2147483633
            ScrollBtnLeft.FontColor = 16777215
            ScrollBtnLeft.LatchedColor = -2147483633
            ScrollBtnLeft.DisabledColor = -2147483633
            ScrollBtnLeft.LatchedFontColor = -2147483640
            ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
            ScrollBtnRight.Font.Color = clWindowText
            ScrollBtnRight.Font.Height = -11
            ScrollBtnRight.Font.Name = 'MS Sans Serif'
            ScrollBtnRight.Font.Style = []
            ScrollBtnRight.ParentFont = False
            ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnRight.LatchedFont.Color = clWindowText
            ScrollBtnRight.LatchedFont.Height = -11
            ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnRight.LatchedFont.Style = []
            ScrollBtnRight.ParentLatchedFontColor = False
            ScrollBtnRight.GlyphAlpha = 255
            ScrollBtnRight.Color = -2147483633
            ScrollBtnRight.FontColor = 16777215
            ScrollBtnRight.LatchedColor = -2147483633
            ScrollBtnRight.DisabledColor = -2147483633
            ScrollBtnRight.LatchedFontColor = -2147483640
            ArrowColor = 2200566
            ArrowBorderColor = 2200566
            Buttons = {04000000}
          end
          object tbtnDelDept: TTouchBtn
            Left = 288
            Top = 176
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Delete Department'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnDelDeptMouseClick
          end
          object tbtnAddDefaultDept: TTouchBtn
            Left = 288
            Top = 248
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Add Default Departments'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnAddDefaultDeptMouseClick
          end
          object tbtnDeptDown: TTouchBtn
            Left = 288
            Top = 392
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            ButtonColor = clSilver
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atDown
            OnMouseClick = tbtnDeptDownMouseClick
          end
          object tbtnDeptUp: TTouchBtn
            Left = 288
            Top = 328
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            ButtonColor = clSilver
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atUp
            OnMouseClick = tbtnDeptUpMouseClick
          end
          object tbtnDeptEdit: TTouchBtn
            Left = 288
            Top = 104
            Width = 129
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Edit Department'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnDeptEditMouseClick
          end
        end
      end
    end
    object tsMembershipOptions: TTabSheet
      Caption = 'Membership Options.'
      ImageIndex = 3
      TabVisible = False
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox1: TGroupBox
          Left = 8
          Top = 42
          Width = 409
          Height = 65
          Caption = 'Member Search'
          TabOrder = 0
          object Label1: TLabel
            Left = 26
            Top = 40
            Width = 224
            Height = 18
            Caption = 'Number of Member Quick Buttons'
          end
          object cbIncrementalSearch: TCheckBox
            Left = 8
            Top = 20
            Width = 393
            Height = 20
            Caption = 'Enable Incremental searches for Members'
            TabOrder = 0
            OnClick = cbIncrementalSearchClick
          end
          object tbMemberQuickButtons: TTouchBtn
            Left = 313
            Top = 20
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbMemberQuickButtonsClick
          end
        end
        object GroupBox7: TGroupBox
          Left = 8
          Top = 110
          Width = 409
          Height = 249
          Caption = 'Points'
          TabOrder = 1
          object Label3: TLabel
            Left = 8
            Top = 22
            Width = 159
            Height = 18
            Caption = 'Payment Group Number'
          end
          object tbtnPointsGroup: TTouchBtn
            Left = 313
            Top = 12
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnPointsGroupClick
          end
          object cbEarnPoints: TCheckBox
            Left = 8
            Top = 42
            Width = 303
            Height = 20
            Caption = 'You can earn points while redeeming points'
            TabOrder = 2
            OnClick = cbEarnPointsClick
          end
          object cbOnlyEarnPoints: TCheckBox
            Left = 8
            Top = 62
            Width = 393
            Height = 20
            Caption = 'You can only earn points from redeemed points'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = cbOnlyEarnPointsClick
          end
          object cbSeparatePts: TCheckBox
            Left = 8
            Top = 82
            Width = 353
            Height = 20
            Caption = 'Separate Earnt and Loaded Points'
            TabOrder = 10
            OnClick = cbSeparatePtsClick
          end
          object cbCountPointSpentInRevenue: TCheckBox
            Left = 8
            Top = 102
            Width = 353
            Height = 20
            Caption = 'Points Spent Not Counted As Revenue'
            TabOrder = 1
            OnClick = cbCountPointSpentInRevenueClick
          end
          object cbOnlyFullProducts: TCheckBox
            Left = 8
            Top = 122
            Width = 393
            Height = 20
            Caption = 'Only Allow Points to be Spent on Full Products'
            TabOrder = 3
            OnClick = cbOnlyFullProductsClick
          end
          object cbRedeemOnSelectedItems: TCheckBox
            Left = 8
            Top = 162
            Width = 396
            Height = 20
            Caption = 'Points to be Redeemed on Selected Items Only'
            TabOrder = 5
            OnClick = cbRedeemOnSelectedItemsClick
          end
          object cbEarnPointsOnWeight: TCheckBox
            Left = 8
            Top = 142
            Width = 396
            Height = 20
            Caption = 'Calculate Earnt Points on Weight'
            TabOrder = 6
            OnClick = cbEarnPointsOnWeightClick
          end
          object cbRefundPoints: TCheckBox
            Left = 8
            Top = 182
            Width = 390
            Height = 20
            Caption = 'Allow Refund Points'
            TabOrder = 7
            OnClick = cbRefundPointsClick
          end
          object cbPointOnly: TCheckBox
            Left = 8
            Top = 202
            Width = 300
            Height = 25
            Caption = 'Only Allow Member Sales'
            TabOrder = 9
            OnClick = cbPointOnlyClick
          end
          object cbShowScreenToSelectItems: TCheckBox
            Left = 8
            Top = 222
            Width = 300
            Height = 25
            Caption = 'Show Item Points Screen'
            TabOrder = 0
            OnClick = cbShowScreenToSelectItemsClick
          end
        end
        object Panel10: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Membership'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
        end
        object rgLocationFreeSalesRules: TRadioGroup
          Left = 424
          Top = 365
          Width = 416
          Height = 110
          Caption = 'Location Loyalty Purchase Count'
          Items.Strings = (
            'Disabled'
            'Enabled for All sales Types'
            'Enabled for Cash and Tender Sales'
            'Enabled For Saved Sales')
          ParentBackground = False
          TabOrder = 3
          OnClick = rgLocationFreeSalesRulesClick
        end
        object rgMemberFreeSalesRules: TRadioGroup
          Left = 424
          Top = 480
          Width = 416
          Height = 113
          Caption = 'Member Loyalty Purchase Count'
          Items.Strings = (
            'Disabled'
            'Enabled for All sales Types'
            'Enabled for Cash and Tender Sales'
            'Enabled For Saved Sales')
          TabOrder = 4
          OnClick = rgMemberFreeSalesRulesClick
        end
        object GroupBox14: TGroupBox
          Left = 423
          Top = 42
          Width = 417
          Height = 150
          Caption = 'Lucky Draw'
          TabOrder = 5
          object Label10: TLabel
            Left = 8
            Top = 65
            Width = 224
            Height = 18
            Caption = 'Lucky Member Period ( In Hours )'
          end
          object tbtnLuckMemberPeriod: TTouchBtn
            Left = 312
            Top = 65
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnLuckMemberPeriodClick
          end
          object tbtnLuckMemberPeriodDay: TTouchBtn
            Left = 16
            Top = 22
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'A Day'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnLuckMemberPeriodDayClick
          end
          object tbtnLuckMemberPeriodWeek: TTouchBtn
            Left = 160
            Top = 22
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'A Week'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnLuckMemberPeriodWeekClick
          end
          object tbtnLuckMemberPeriodMonth: TTouchBtn
            Left = 312
            Top = 22
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'A Month'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnLuckMemberPeriodMonthClick
          end
          object cbEnableMemberSpendChits: TCheckBox
            Left = 10
            Top = 115
            Width = 273
            Height = 20
            Caption = 'Enable Member spend Chits'
            TabOrder = 4
            OnClick = cbEnableMemberSpendChitsClick
          end
          object tbtnMemberSpendChitValue: TTouchBtn
            Left = 288
            Top = 110
            Width = 113
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '$0.00'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnMemberSpendChitValueClick
          end
          object cbDrawByLocation: TCheckBox
            Left = 10
            Top = 90
            Width = 273
            Height = 20
            Caption = 'Draw From this Location Only'
            TabOrder = 6
            OnClick = cbDrawByLocationClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 424
          Top = 195
          Width = 416
          Height = 83
          Caption = 'Synchronise With External Membership.'
          TabOrder = 6
          object Label12: TLabel
            Left = 10
            Top = 25
            Width = 173
            Height = 18
            Caption = 'Synchronise Member Info.'
          end
          object tbtnSyncroniseMembersip: TTouchBtn
            Left = 279
            Top = 18
            Width = 122
            Height = 41
            ParentColor = True
            Caption = 'Synchronise'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSyncroniseMembersipMouseClick
          end
          object cbAutoAddWebMembers: TCheckBox
            Left = 10
            Top = 55
            Width = 247
            Height = 20
            Caption = 'Automatically Add Web Members'
            TabOrder = 1
            OnClick = cbAutoAddWebMembersClick
          end
        end
        object GroupBox15: TGroupBox
          Left = 423
          Top = 280
          Width = 417
          Height = 81
          Caption = 'Membership Numbers'
          TabOrder = 7
          object Label14: TLabel
            Left = 8
            Top = 27
            Width = 199
            Height = 18
            Caption = 'Minimum Membership Number'
          end
          object tbMinMembershipNumber: TTouchBtn
            Left = 312
            Top = 20
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbMinMembershipNumberClick
          end
          object cbRecycleMembershipNumbers: TCheckBox
            Left = 8
            Top = 53
            Width = 289
            Height = 20
            Caption = 'Recycle Membership Numbers'
            TabOrder = 1
            OnClick = cbRecycleMembershipNumbersClick
          end
        end
        object GroupBox16: TGroupBox
          Left = 10
          Top = 470
          Width = 408
          Height = 130
          Caption = 'Membership Smart Cards'
          TabOrder = 8
          object lbeEntireSiteID: TLabel
            Left = 267
            Top = 17
            Width = 54
            Height = 18
            Caption = 'Site ID :'
            OnMouseUp = lbeEntireSiteIDMouseUp
          end
          object cbEnableMembershipSmartCards: TCheckBox
            Left = 9
            Top = 22
            Width = 252
            Height = 20
            Caption = 'Enable Membership Smart Cards'
            TabOrder = 0
            OnClick = cbEnableMembershipSmartCardsClick
          end
          object cbvmAllowSmartCardNotPresentMemberTransactions: TCheckBox
            Left = 9
            Top = 41
            Width = 367
            Height = 20
            Caption = 'Allow card not present member transactions'
            TabOrder = 1
            OnClick = cbvmAllowSmartCardNotPresentMemberTransactionsClick
          end
          object cbvmAllowMemberDetailscreen: TCheckBox
            Left = 9
            Top = 60
            Width = 385
            Height = 23
            Caption = 'Enable Member Details screen'
            TabOrder = 2
            OnClick = cbvmAllowMemberDetailscreenClick
          end
          object cbvmMandatoryMembershipCard: TCheckBox
            Left = 9
            Top = 80
            Width = 385
            Height = 23
            Caption = 'Mandatory Membership Card'
            TabOrder = 4
            OnClick = cbvmMandatoryMembershipCardClick
          end
          object cbGiftCardOnly: TCheckBox
            Left = 9
            Top = 100
            Width = 385
            Height = 23
            Caption = 'Gift Card Only'
            TabOrder = 3
            OnClick = cbGiftCardOnlyClick
          end
        end
        object rgMemberNameOnRecipt: TRadioGroup
          Left = 9
          Top = 360
          Width = 408
          Height = 108
          Caption = 'Receipt Membership Name Display'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          Items.Strings = (
            'Alias / Full Name'
            'Full Name / Alias'
            'Full Name'
            'Known As')
          ParentFont = False
          TabOrder = 9
          OnClick = rgMemberNameOnReciptClick
        end
        object GroupBox3: TGroupBox
          Left = 8
          Top = 600
          Width = 409
          Height = 156
          Caption = 'Notifications'
          TabOrder = 10
          object lbeBirthdayBuffer: TLabel
            Left = 9
            Top = 50
            Width = 101
            Height = 18
            Caption = 'Birthday Days :'
          end
          object lbeFirstVisitPoint: TLabel
            Left = 9
            Top = 117
            Width = 150
            Height = 18
            Caption = 'First Visit Bonus Points:'
          end
          object cbNotifyBirthday: TCheckBox
            Left = 9
            Top = 24
            Width = 289
            Height = 20
            Caption = 'Show Birthday Notification'
            TabOrder = 0
            OnClick = cbNotifyBirthdayClick
          end
          object cbAcknowledgeFirstVisit: TCheckBox
            Left = 9
            Top = 82
            Width = 225
            Height = 20
            Caption = 'Acknowledge First Visit'
            TabOrder = 1
            OnClick = cbAcknowledgeFirstVisitClick
          end
          object tbRestLastVisit: TTouchBtn
            Left = 298
            Top = 78
            Width = 80
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Reset'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbRestLastVisitClick
          end
          object tbFirstVisitPoint: TTouchBtn
            Left = 298
            Top = 117
            Width = 80
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbFirstVisitPointClick
          end
          object tbBirthdayBuffer: TTouchBtn
            Left = 297
            Top = 39
            Width = 80
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbBirthdayBufferClick
          end
        end
        object grpTierLevel: TGroupBox
          Left = 423
          Top = 595
          Width = 416
          Height = 135
          Caption = 'Tier Level'
          TabOrder = 11
          object Label24: TLabel
            Left = 10
            Top = 95
            Width = 176
            Height = 18
            Caption = 'Minimum Redemption Point'
          end
          object cbEnableTierLevels: TCheckBox
            Left = 8
            Top = 21
            Width = 289
            Height = 20
            Caption = 'Use Tier Levels'
            TabOrder = 1
            OnClick = cbEnableTierLevelsClick
          end
          object cbAllowPointPaymentByWeight: TCheckBox
            Left = 10
            Top = 45
            Width = 289
            Height = 20
            Caption = 'Allow Point Payment By Weight (grams)'
            TabOrder = 2
            OnClick = cbAllowPointPaymentByWeightClick
          end
          object cbAllowPointPaymentByValue: TCheckBox
            Left = 10
            Top = 70
            Width = 289
            Height = 20
            Caption = 'Allow Point Payment By Value (Points)'
            TabOrder = 0
            OnClick = cbAllowPointPaymentByValueClick
          end
          object tbMinRedemptionPoint: TTouchBtn
            Left = 312
            Top = 85
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbMinRedemptionPointClick
          end
        end
      end
    end
    object tsEftPos: TTabSheet
      Caption = 'EFTPos.'
      ImageIndex = 4
      TabVisible = False
      object Panel6: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Label9: TLabel
          Left = 8
          Top = 56
          Width = 525
          Height = 18
          Caption = 
            'You must Restart MenuMate for any of the settings on this page t' +
            'o take effect.'
        end
        object GroupBox9: TGroupBox
          Left = 8
          Top = 88
          Width = 505
          Height = 250
          Caption = 'Integrated EFTPOS'
          TabOrder = 0
          object cbIntegratedEftposANZ: TCheckBox
            Left = 8
            Top = 25
            Width = 273
            Height = 20
            Caption = 'EFTPOS NZ'
            TabOrder = 0
            OnClick = cbIntegratedEftposANZClick
          end
          object cbIntegratedEftposSyncro: TCheckBox
            Left = 8
            Top = 50
            Width = 273
            Height = 20
            Caption = 'Provenco'
            TabOrder = 1
            OnClick = cbIntegratedEftposSyncroClick
          end
          object cbIntegratedEftposIngenico: TCheckBox
            Left = 8
            Top = 75
            Width = 321
            Height = 20
            Caption = 'Ingenico and PC-EFTPOS Australia'
            TabOrder = 2
            OnClick = cbIntegratedEftposIngenicoClick
          end
          object cbIntegratedEftposCadmus: TCheckBox
            Left = 8
            Top = 100
            Width = 257
            Height = 20
            Caption = 'Cadmus Keylink (One Way)'
            TabOrder = 3
            OnClick = cbIntegratedEftposCadmusClick
          end
          object cbICELink: TCheckBox
            Left = 8
            Top = 150
            Width = 257
            Height = 20
            Caption = 'ICELink ICE 5000 / Hypercom'
            TabOrder = 4
            OnClick = cbICELinkClick
          end
          object cbIntegratedEftposDPS: TCheckBox
            Left = 8
            Top = 175
            Width = 257
            Height = 20
            Caption = 'DPS Payment Express'
            TabOrder = 5
            OnClick = cbIntegratedEftposDPSClick
          end
          object cbIntegratedEftposSmartPay: TCheckBox
            Left = 8
            Top = 200
            Width = 257
            Height = 20
            Caption = 'EFTPOS SmartPay'
            TabOrder = 8
            OnClick = cbIntegratedEftposSmartPayClick
          end
          object cbEnableDPSTipping: TCheckBox
            Left = 8
            Top = 225
            Width = 257
            Height = 20
            Caption = 'Enable Tipping Transactions'
            TabOrder = 7
            OnClick = cbEnableDPSTippingClick
          end
          object cbCadmusCronos: TCheckBox
            Left = 8
            Top = 125
            Width = 257
            Height = 20
            Caption = 'Cadmus Cronos'
            TabOrder = 6
            OnClick = cbCadmusCronosClick
          end
        end
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'EFTPOS'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
        end
        object GroupBox13: TGroupBox
          Left = 8
          Top = 350
          Width = 497
          Height = 145
          Caption = 'EFTPOS Serial Port ( Cadmus Keylink and ICE Link Only )'
          TabOrder = 2
          object Label20: TLabel
            Left = 9
            Top = 22
            Width = 64
            Height = 18
            Caption = 'Serial Port'
          end
          object Label8: TLabel
            Left = 8
            Top = 91
            Width = 118
            Height = 18
            Caption = 'Merchant Number'
          end
          object cbEftposCadmusSerialPort: TComboBox
            Left = 8
            Top = 46
            Width = 329
            Height = 26
            Style = csDropDownList
            TabOrder = 0
            OnChange = cbEftposCadmusSerialPortChange
            Items.Strings = (
              'None'
              'Comm 01'
              'Comm 02'
              'Comm 03'
              'Comm 04'
              'Comm 05'
              'Comm 06'
              'Comm 07'
              'Comm 08'
              'Comm 09'
              'Comm 10'
              'Comm 11'
              'Comm 12'
              'Comm 13'
              'Comm 14'
              'Comm 15'
              'Comm 16')
          end
          object tbCadmusMerchantNumber: TTouchBtn
            Left = 160
            Top = 84
            Width = 89
            Height = 33
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '0'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbCadmusMerchantNumberClick
          end
        end
        object cbDuplicateEftPos: TCheckBox
          Left = 8
          Top = 500
          Width = 289
          Height = 20
          Caption = 'Duplicate EFTPOS Receipt'
          TabOrder = 3
          OnClick = cbDuplicateEftPosClick
        end
        object smartLinkGroupBox: TGroupBox
          Left = 8
          Top = 530
          Width = 497
          Height = 100
          Caption = 'SmartLink Ip Address '
          TabOrder = 5
          object tbtnSmartLinkIp: TTouchBtn
            Left = 12
            Top = 30
            Width = 269
            Height = 49
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '127.0.0.1'
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSmartLinkIpClick
          end
        end
        object GroupBox20: TGroupBox
          Left = 8
          Top = 640
          Width = 497
          Height = 105
          Caption = 'AA Rewards '
          TabOrder = 4
          object cbEnableAARewards: TCheckBox
            Left = 8
            Top = 61
            Width = 473
            Height = 20
            Caption = 'Intergrated AA Rewards ( Provenco Eftpos Only )'
            TabOrder = 0
            OnClick = cbEnableAARewardsClick
          end
          object cbAARewardsReminder: TCheckBox
            Left = 8
            Top = 29
            Width = 289
            Height = 20
            Caption = 'AA Rewards Reminder'
            TabOrder = 1
            OnClick = cbAARewardsReminderClick
          end
        end
      end
    end
    object tsLogging: TTabSheet
      Caption = 'Logging'
      ImageIndex = 5
      TabVisible = False
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox4: TGroupBox
          Left = 8
          Top = 46
          Width = 433
          Height = 67
          Caption = 'Storage Settings'
          TabOrder = 0
          object Label11: TLabel
            Left = 8
            Top = 30
            Width = 104
            Height = 18
            Caption = 'Clear logs every'
          end
          object Label16: TLabel
            Left = 232
            Top = 30
            Width = 31
            Height = 18
            Caption = 'days'
          end
          object edClearLogsXDays: TEdit
            Left = 160
            Top = 26
            Width = 49
            Height = 26
            TabOrder = 0
            Text = '0'
            OnClick = edClearLogsXDaysClick
          end
          object udClearLogsXDays: TUpDown
            Left = 209
            Top = 26
            Width = 15
            Height = 26
            Associate = edClearLogsXDays
            TabOrder = 1
          end
        end
        object Panel12: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Logging'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
        end
        object MemLogs: TMemo
          Left = 8
          Top = 120
          Width = 433
          Height = 353
          TabOrder = 2
        end
        object cbOnLineLogging: TCheckBox
          Left = 448
          Top = 72
          Width = 401
          Height = 17
          Caption = 'Enable Online Logging'
          TabOrder = 3
          OnClick = cbOnLineLoggingClick
        end
        object TouchBtn1: TTouchBtn
          Left = 664
          Top = 56
          ParentColor = True
          Caption = 'Start LogsMate'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = TouchBtn1MouseClick
        end
        object TouchBtn2: TTouchBtn
          Left = 448
          Top = 120
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Debug Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogDebugInfoClick
        end
        object TouchBtn3: TTouchBtn
          Left = 448
          Top = 192
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Phoenix Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogPhoenixInfoClick
        end
        object TouchBtn4: TTouchBtn
          Left = 448
          Top = 264
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Bluetooth Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogBTInfoClick
        end
        object TouchBtn5: TTouchBtn
          Left = 448
          Top = 336
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Packet Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogBTPacketInfoClick
        end
        object TouchBtn6: TTouchBtn
          Left = 448
          Top = 408
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Packet De-code Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogBTDecodeInfoClick
        end
        object TouchBtn7: TTouchBtn
          Left = 8
          Top = 480
          Width = 433
          Height = 65
          ParentColor = True
          Caption = 'Reset Defaults'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbLogsResetClick
        end
        object tbSmartCard: TTouchBtn
          Left = 576
          Top = 120
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Smartcard Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbSmartCardMouseClick
        end
        object TouchBtn19: TTouchBtn
          Left = 576
          Top = 192
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'IntaMate Info'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = TouchBtn19MouseClick
        end
        object TouchBtn20: TTouchBtn
          Left = 576
          Top = 264
          Width = 121
          Height = 65
          ParentColor = True
          Caption = 'Pocket Vouchers'
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = TouchBtn20MouseClick
        end
      end
    end
    object tsEndOfDayOptions: TTabSheet
      Caption = 'Magic Memories'
      ImageIndex = 6
      TabVisible = False
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object lblEndOfDay: TLabel
          Left = 8
          Top = 364
          Width = 137
          Height = 27
          AutoSize = False
          Caption = 'Session Start Time: '
          Color = 14342874
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          WordWrap = True
        end
        object Panel8: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'End Of Day'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBtnText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object GroupBox25: TGroupBox
          Left = 400
          Top = 49
          Width = 475
          Height = 725
          Caption = 'Zed Print'
          ParentBackground = False
          TabOrder = 15
          object cbShowSessionDate: TCheckBox
            Left = 8
            Top = 20
            Width = 250
            Height = 25
            Caption = 'Show Session Date On Zed'
            TabOrder = 0
            OnClick = cbShowSessionDateClick
          end
          object cbDEnableBlindBalances: TCheckBox
            Left = 8
            Top = 45
            Width = 250
            Height = 25
            Caption = 'Enable Zed Blind Balances'
            TabOrder = 22
            OnClick = cbDEnableBlindBalancesClick
          end
          object cbShowTransactionSummaryGroups: TCheckBox
            Left = 8
            Top = 70
            Width = 280
            Height = 25
            Caption = 'Show Transaction Summary Groups'
            TabOrder = 23
            OnClick = cbShowTransactionSummaryGroupsClick
          end
          object cbShowBilledSalesTotals: TCheckBox
            Left = 8
            Top = 95
            Width = 250
            Height = 25
            Caption = 'Show Billed Sales Totals'
            TabOrder = 6
            OnClick = cbShowBilledSalesTotalsClick
          end
          object cbShowComplimentarySalesTotals: TCheckBox
            Left = 8
            Top = 120
            Width = 250
            Height = 25
            Caption = 'Show Complimentary Sales Totals'
            TabOrder = 7
            OnClick = cbShowComplimentarySalesTotalsClick
          end
          object cbShowChargedSalesTotals: TCheckBox
            Left = 8
            Top = 145
            Width = 250
            Height = 25
            Caption = 'Show Charged Sales Totals'
            TabOrder = 8
            OnClick = cbShowChargedSalesTotalsClick
          end
          object cbShowTotals: TCheckBox
            Left = 8
            Top = 170
            Width = 250
            Height = 25
            Caption = 'Show Totals'
            TabOrder = 9
            OnClick = cbShowTotalsClick
          end
          object cbShowBreakdownCategories: TCheckBox
            Left = 8
            Top = 195
            Width = 250
            Height = 25
            Caption = 'Show Breakdown Categories'
            TabOrder = 10
            OnClick = cbShowBreakdownCategoriesClick
          end
          object cbShowRemovalReport: TCheckBox
            Left = 8
            Top = 220
            Width = 250
            Height = 25
            Caption = 'Show Removals Report'
            TabOrder = 24
            OnClick = cbShowRemovalReportClick
          end
          object cbShowDiscountReport: TCheckBox
            Left = 8
            Top = 245
            Width = 250
            Height = 25
            Caption = 'Show Discount Report'
            TabOrder = 11
            OnClick = cbShowDiscountReportClick
          end
          object cbShowPointsReport: TCheckBox
            Left = 8
            Top = 270
            Width = 250
            Height = 25
            Caption = 'Show Points Report'
            TabOrder = 13
            OnClick = cbShowPointsReportClick
          end
          object cbShowPatronAverages: TCheckBox
            Left = 8
            Top = 295
            Width = 250
            Height = 25
            Caption = 'Show Patron Averages'
            TabOrder = 15
            OnClick = cbShowPatronAveragesClick
          end
          object cbShowProductionInfo: TCheckBox
            Left = 8
            Top = 320
            Width = 250
            Height = 25
            Caption = 'Show Production Info'
            TabOrder = 16
            OnClick = cbShowProductionInfoClick
          end
          object cbShowAccountPurchases: TCheckBox
            Left = 8
            Top = 345
            Width = 250
            Height = 25
            Caption = 'Show Account Purchases'
            TabOrder = 17
            OnClick = cbShowAccountPurchasesClick
          end
          object cbShowAccountBalancesTabs: TCheckBox
            Left = 8
            Top = 370
            Width = 250
            Height = 25
            Caption = 'Show Account Balances Tabs'
            TabOrder = 18
            OnClick = cbShowAccountBalancesTabsClick
          end
          object cbShowAccountBalances: TCheckBox
            Left = 8
            Top = 395
            Width = 250
            Height = 25
            Caption = 'Show Account Balances - Seated'
            TabOrder = 19
            OnClick = cbShowAccountBalancesClick
          end
          object cbShowHourlySales: TCheckBox
            Left = 8
            Top = 420
            Width = 250
            Height = 25
            Caption = 'Show Hourly Sales'
            TabOrder = 20
            OnClick = cbShowHourlySalesClick
          end
          object cbShowAccumulatedTotal: TCheckBox
            Left = 8
            Top = 445
            Width = 250
            Height = 25
            Caption = 'Show Accumulated Total'
            TabOrder = 1
            OnClick = cbShowAccumulatedTotalClick
          end
          object cbShowTaxSummary: TCheckBox
            Left = 8
            Top = 470
            Width = 250
            Height = 25
            Caption = 'Show Tax Summary'
            TabOrder = 12
            OnClick = cbShowTaxSummaryClick
          end
          object cbShowServiceChargeSummary: TCheckBox
            Left = 8
            Top = 495
            Width = 250
            Height = 25
            Caption = 'Show Service Charge Summary'
            TabOrder = 14
            OnClick = cbShowServiceChargeSummaryClick
          end
          object cbEnableStaffHours: TCheckBox
            Left = 8
            Top = 520
            Width = 217
            Height = 25
            Caption = 'Enable Staff Hours'
            TabOrder = 2
            OnClick = EnableStaffHoursOnClick
          end
          object cbEnableCommission: TCheckBox
            Left = 9
            Top = 545
            Width = 243
            Height = 25
            Caption = 'Enable Commission / Tips'
            TabOrder = 21
            OnClick = cbEnableCommissionClick
          end
          object cbRevenueFiguresAreTaxAndServiceChargeInclusive: TCheckBox
            Left = 9
            Top = 570
            Width = 370
            Height = 25
            Caption = 'Revenue figures are tax and service charge inclusive'
            TabOrder = 3
            OnClick = cbRevenueFiguresAreTaxAndServiceChargeInclusiveClick
          end
          object cbPointsOnZed: TCheckBox
            Left = 8
            Top = 595
            Width = 289
            Height = 20
            Caption = 'Show points earned on Zed'
            TabOrder = 4
            OnClick = cbPointsOnZedClick
          end
          object cbShowInvoiceInfo: TCheckBox
            Left = 8
            Top = 620
            Width = 329
            Height = 20
            Caption = 'Show Invoice Information on Zed'
            TabOrder = 25
            OnClick = cbShowInvoiceInfoClick
          end
          object cbRevenueFiguresAreDiscountInclusive: TCheckBox
            Left = 8
            Top = 645
            Width = 329
            Height = 20
            Caption = 'Revenue Figures are discount inclusive'
            TabOrder = 5
            OnClick = cbRevenueFiguresAreDiscountInclusiveClick
          end
          object cbShowPriceAdjustment: TCheckBox
            Left = 8
            Top = 670
            Width = 329
            Height = 20
            Caption = 'Show Price Adjustment on Zed'
            TabOrder = 27
            OnClick = cbShowPriceAdjustmentClick
          end
          object cbOpenCashDrawer: TCheckBox
            Left = 8
            Top = 695
            Width = 329
            Height = 20
            Caption = 'Open Cash Drawer'
            TabOrder = 26
            OnClick = cbOpenCashDrawerClick
          end
        end
        object cbEnablePrinterCounts: TCheckBox
          Left = 8
          Top = 489
          Width = 243
          Height = 25
          Caption = 'Enable Printer Counts'
          TabOrder = 2
          OnClick = cbEnablePrinterCountsClick
        end
        object cbPostToXero: TCheckBox
          Left = 8
          Top = 392
          Width = 243
          Height = 25
          Caption = 'Post Z to Accounting System'
          TabOrder = 18
          OnClick = cbPostToXeroClick
        end
        object cbPostMoneyToGLAccounts: TCheckBox
          Left = 8
          Top = 439
          Width = 243
          Height = 25
          Caption = 'Money posts to GL accounts'
          TabOrder = 19
          OnClick = cbPostMoneyToGLAccountsClick
        end
        object cbPostMoneyAsPayments: TCheckBox
          Left = 8
          Top = 414
          Width = 390
          Height = 25
          Caption = 'Money posts as payments Against Daily Summary Inv.'
          TabOrder = 17
          OnClick = cbPostMoneyAsPaymentsClick
        end
        object cbEnablePaxCount: TCheckBox
          Left = 8
          Top = 464
          Width = 243
          Height = 25
          Caption = 'Enable Pax Count'
          TabOrder = 3
          OnClick = cbEnablePaxCountClick
        end
        object cbDepositBagNum: TCheckBox
          Left = 8
          Top = 314
          Width = 314
          Height = 25
          Caption = 'Master Terminal'
          TabOrder = 5
          OnClick = cbDepositBagNumClick
        end
        object cbDontClearDataOnZed: TCheckBox
          Left = 8
          Top = 339
          Width = 314
          Height = 25
          Caption = 'Slave Terminal'
          TabOrder = 8
          OnClick = cbDontClearDataOnZedClick
        end
        object cbDisplayStockPurchaseWeborders: TCheckBox
          Left = 8
          Top = 539
          Width = 377
          Height = 25
          Caption = 'Display Stock Purchase Weborders on X/Y Report'
          TabOrder = 10
          OnClick = cbDisplayStockPurchaseWebordersClick
        end
        object cbEnableHideCredCancels: TCheckBox
          Left = 8
          Top = 564
          Width = 353
          Height = 25
          Caption = 'Hide Refunds,Cancels and Write-Off s from Zed'
          TabOrder = 12
          OnClick = cbHideCredsCancelsClick
        end
        object cbSummariseDiscountsonZed: TCheckBox
          Left = 8
          Top = 514
          Width = 377
          Height = 25
          Caption = 'Summarise Discounts on Zed'
          TabOrder = 6
          OnClick = cbSummariseDiscountsonZedClick
        end
        object cbEndOfDay: TComboBox
          Left = 151
          Top = 361
          Width = 100
          Height = 27
          Style = csDropDownList
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnChange = cbEndOfDayChange
          OnSelect = cbEndOfDaySelect
        end
        object cbCombineServiceChargeAndServiceChargeTax: TCheckBox
          Left = 8
          Top = 589
          Width = 340
          Height = 25
          Caption = 'Combine Service Charge Tax and Service Charge'
          TabOrder = 14
          OnClick = cbCombineServiceChargeAndServiceChargeTaxClick
        end
        object cbCombineServiceChargeTaxAndSalesTax: TCheckBox
          Left = 8
          Top = 614
          Width = 340
          Height = 25
          Caption = 'Combine Service Charge Tax and Sales Tax'
          TabOrder = 13
          OnClick = cbCombineServiceChargeTaxAndSalesTaxClick
        end
        object cbEmailZedClosingTill: TCheckBox
          Left = 8
          Top = 639
          Width = 340
          Height = 25
          Caption = 'Email Zed When Closing Till'
          TabOrder = 16
          OnClick = cbEmailZedClosingTillClick
        end
        object tbtnConfigureEmail: TTouchBtn
          Left = 250
          Top = 639
          Width = 121
          Height = 45
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Configure Email'
          ButtonColor = 6052441
          LatchedColor = clNavy
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor]
          OnMouseClick = tbtnConfigureEmailClick
        end
        object cbEnableClientDetails: TCheckBox
          Left = 8
          Top = 49
          Width = 243
          Height = 25
          Caption = 'Enable Client Details'
          TabOrder = 7
          OnClick = cbEnableClientDetailsClick
        end
        object cbWarnIfOpenTableAndTabs: TCheckBox
          Left = 8
          Top = 264
          Width = 307
          Height = 25
          Caption = 'Warn If Open Tables and Tabs'
          TabOrder = 9
          OnClick = cbWarnIfOpenTableAndTabsClick
        end
        object cbStopIfOpenTableAndTabs: TCheckBox
          Left = 8
          Top = 289
          Width = 307
          Height = 25
          Caption = 'Stop If Open Tables and Tabs'
          TabOrder = 11
          OnClick = cbStopIfOpenTableAndTabsClick
        end
        object tpZedInfo: TTouchPages
          Left = 7
          Top = 75
          Width = 317
          Height = 182
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ActivePage = tszedHeader
          TabHeight = 0
          TabWidth = 0
          TabColor = clBtnFace
          object tszedHeader: TTouchSheet
            Left = 2
            Top = 2
            Width = 313
            Height = 178
            BorderWidth = 3
            Color = 2200566
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            object tbzedHeaderText: TRichEdit
              Left = 0
              Top = 0
              Width = 307
              Height = 172
              Align = alClient
              Alignment = taCenter
              Color = clWhite
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              PlainText = True
              TabOrder = 0
              OnClick = tbzedHeaderTextClick
            end
          end
        end
        object GroupBox26: TGroupBox
          Left = 7
          Top = 685
          Width = 387
          Height = 124
          Caption = 'Member Reset Points'
          ParentBackground = False
          TabOrder = 1
          object lbeDay: TLabel
            Left = 241
            Top = 52
            Width = 33
            Height = 18
            Caption = 'Days'
          end
          object cbResetPointBalancedPointPurchased: TCheckBox
            Left = 3
            Top = 25
            Width = 170
            Height = 25
            Caption = 'Point Purchased'
            TabOrder = 1
            OnClick = cbResetMemberPointPurchasedClick
          end
          object cbResetPointBalancedPointRedeemed: TCheckBox
            Left = 3
            Top = 50
            Width = 157
            Height = 25
            Caption = 'Point Redemeed'
            TabOrder = 2
            OnClick = cbResetMemberPointRedeemedClick
          end
          object cbResetPointBalancedPointEarned: TCheckBox
            Left = 3
            Top = 72
            Width = 147
            Height = 25
            Caption = 'Point Earned'
            TabOrder = 4
            OnClick = cbResetMemberPointEarnedClick
          end
          object edDay: TEdit
            Left = 179
            Top = 48
            Width = 50
            Height = 26
            Enabled = False
            TabOrder = 3
            OnClick = edDayClick
          end
          object cbConfirmResetPoint: TCheckBox
            Left = 3
            Top = 96
            Width = 303
            Height = 25
            Caption = 'Confirm Points Reset At The End of Day'
            TabOrder = 0
            OnClick = cbConfirmResetPointClick
          end
        end
      end
    end
    object tsCustomerDisplay: TTabSheet
      Caption = 'Customer Display.'
      TabVisible = False
      object Panel16: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object gbMemberPoleDisplay: TGroupBox
          Left = 8
          Top = 48
          Width = 409
          Height = 120
          Caption = 'Pole Display'
          TabOrder = 2
          object rbNoGreeting: TRadioButton
            Left = 9
            Top = 24
            Width = 250
            Height = 17
            Caption = 'No Greeting'
            TabOrder = 0
            OnClick = rbNoGreetingClick
          end
          object rbShowAlias: TRadioButton
            Left = 9
            Top = 47
            Width = 300
            Height = 17
            Caption = 'Show Member'#39's Alias (Known As)'
            TabOrder = 1
            OnClick = rbShowAliasClick
          end
          object rbShowName: TRadioButton
            Left = 9
            Top = 70
            Width = 229
            Height = 17
            Caption = 'Show Member'#39's Name'
            TabOrder = 3
            OnClick = rbShowNameClick
          end
          object cbPoleDisplayShowPoints: TCheckBox
            Left = 9
            Top = 93
            Width = 289
            Height = 17
            Caption = 'Show Points On Pole Display'
            TabOrder = 2
            OnClick = cbPoleDisplayShowPointsClick
          end
        end
        object nMIStandardGroupBox: TGroupBox
          Left = 8
          Top = 170
          Width = 409
          Height = 50
          Caption = 'Display Settings'
          TabOrder = 0
          object cbUseNMIStandardFontSize: TCheckBox
            Left = 9
            Top = 24
            Width = 289
            Height = 17
            Caption = 'Enlarge Last Ordered Item'
            TabOrder = 0
            OnClick = cbUseNMIStandardFontSizeClick
          end
        end
        object Panel17: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Customer Display'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
        end
      end
    end
  end
end
