object frmMaintain: TfrmMaintain
  Tag = 1024
  Left = 0
  Top = 4
  Align = alClient
  Anchors = [akLeft, akTop, akRight]
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Maintenance Settings'
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
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  DesignSize = (
    1009
    768)
  PixelsPerInch = 96
  TextHeight = 18
  object pnlButtons1: TPanel
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
    object btnClose: TTouchBtn
      Left = 8
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
      OnMouseClick = imgExitClick
    end
    object tbtnMaintenance: TTouchBtn
      Left = 8
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
      Caption = 'Maintenance'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbMaintenanceClick
    end
    object tbtnQuickMessages: TTouchBtn
      Left = 8
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
      Caption = 'Quick Messages'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbQuickMessagesClick
    end
    object tbtnInterfaces: TTouchBtn
      Left = 8
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
      Caption = 'Interfaces'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingType = ltCheckbox
      LatchingProperties = [lkLatchColor]
      OnMouseClick = tbInterfacesClick
    end
  end
  object Pages: TPageControl
    Left = 4
    Top = 4
    Width = 850
    Height = 768
    ActivePage = tsInterfaces
    TabOrder = 1
    object tsMaintenance: TTabSheet
      Caption = 'Maintenance.'
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel1: TPanel
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
          Top = 49
          Width = 241
          Height = 233
          Caption = 'Designations '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          object btnChangeTable: TTouchBtn
            Left = 28
            Top = 32
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Table Names'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnTableNameClick
          end
          object tbtnLocations: TTouchBtn
            Left = 28
            Top = 160
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Location'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnLocationsClick
          end
          object btnChangeRooms: TTouchBtn
            Left = 28
            Top = 96
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Room Names'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnChangeRoomsClick
          end
        end
        object GroupBox5: TGroupBox
          Left = 268
          Top = 341
          Width = 241
          Height = 290
          Caption = 'Miscellaneous Settings '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object tbtnMiscSettings: TTouchBtn
            Left = 28
            Top = 33
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Misc Settings'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnMiscSettingsMouseClick
          end
          object tbRunPrograms: TTouchBtn
            Left = 28
            Top = 96
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Setup Run Programs'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object tbtnPatronTypes: TTouchBtn
            Left = 28
            Top = 159
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Patron Types'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object TouchBtn1: TTouchBtn
            Left = 28
            Top = 224
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Chit Numbers'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtn1MouseClick
          end
        end
        object gbContacts: TGroupBox
          Left = 268
          Top = 49
          Width = 241
          Height = 290
          Caption = 'Contacts '
          DoubleBuffered = False
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentDoubleBuffered = False
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          object btnLoyalty: TTouchBtn
            Left = 28
            Top = 96
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Membership'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnLoyaltyClick
          end
          object TouchBtn2: TTouchBtn
            Left = 28
            Top = 32
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Staff'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnUsersClick
          end
          object btnGUI: TTouchBtn
            Left = 28
            Top = 159
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Contact Groups'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnGUIMouseClick
          end
          object btnTierLevels: TTouchBtn
            Left = 28
            Top = 222
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Tier Levels'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnTierLevelsMouseClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 8
          Top = 283
          Width = 241
          Height = 350
          Caption = 'Pricing '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object tbtnDiscounts: TTouchBtn
            Left = 28
            Top = 33
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Discount & Surcharges'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnDiscountsClick
          end
          object tbtnHappyHour: TTouchBtn
            Left = 28
            Top = 96
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Happy Hour'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnHappyHourClick
          end
          object tbtnPaymentTypes: TTouchBtn
            Left = 28
            Top = 160
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Payment Types'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BtnPaymentClick
          end
          object tbtnDeals: TTouchBtn
            Left = 28
            Top = 223
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Deals'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnDealsMouseClick
          end
          object tbtnTaxSettings: TTouchBtn
            Left = 28
            Top = 286
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Tax Settings'
            ButtonColor = 6315356
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnTaxSettingsMouseClick
          end
        end
        object Panel4: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Maintenance'
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
      end
    end
    object tsQuickMessages: TTabSheet
      Caption = 'Quick Messages.'
      ImageIndex = 2
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 842
        Height = 758
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox3: TGroupBox
          Left = 8
          Top = 49
          Width = 241
          Height = 546
          Caption = 'Quick Messages '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object btnMsgCredits: TTouchBtn
            Left = 28
            Top = 32
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Refund'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object btnBroadcasts: TTouchBtn
            Left = 28
            Top = 160
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Broadcasts'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object btnMsgCancels: TTouchBtn
            Left = 28
            Top = 96
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Cancel'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object btnSkimReasons: TTouchBtn
            Left = 28
            Top = 223
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Float Adjustment Reasons'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnSkimMouseclick
          end
          object TouchBtn3: TTouchBtn
            Left = 28
            Top = 286
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Writeoff Reasons'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = WriteOffMouseClick
          end
          object TouchBtn4: TTouchBtn
            Left = 28
            Top = 349
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Customer Order Types'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = CustomerOrderTypesClick
          end
          object tbtnCashDrawer: TTouchBtn
            Left = 28
            Top = 411
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Cash Drawer'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
          object tbtnCashDenomination: TTouchBtn
            Left = 28
            Top = 473
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Cash Denominations'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = BitBtn2Click
          end
        end
        object Panel5: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Quick Messages'
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
    object tsInterfaces: TTabSheet
      Caption = 'Interfaces.'
      ImageIndex = 3
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel7: TPanel
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
          Top = 48
          Width = 465
          Height = 665
          Caption = 'Interfaces '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object tbPHSInterface: TTouchBtn
            Left = 28
            Top = 100
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'P.M.S Interface'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbPHSInterfaceClick
          end
          object tbtnPrinterMaintenance: TTouchBtn
            Left = 28
            Top = 172
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Printer Maintenance'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnPrinterMaintenanceClick
          end
          object tbtnSmartCards: TTouchBtn
            Left = 28
            Top = 240
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Smartcards'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSmartCardsMouseClick
          end
          object tbIntaMate: TTouchBtn
            Left = 28
            Top = 34
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Intermate Interface'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbIntaMateMouseClick
          end
          object tbtnPocketVouchers: TTouchBtn
            Left = 28
            Top = 310
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Pocket Vouchers'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnPocketVouchersMouseClick
          end
          object tbtnEBet: TTouchBtn
            Left = 247
            Top = 30
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'eBet'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnEBetMouseClick
          end
          object tchbtnWebMate: TTouchBtn
            Left = 247
            Top = 100
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'WebMate'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tchbtnWebMateMouseClick
          end
          object TouchBtnReservations: TTouchBtn
            Left = 247
            Top = 170
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'FloorPlan'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnReservationsMouseClick
          end
          object TouchBtnLoyaltyMate: TTouchBtn
            Left = 247
            Top = 240
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'LoyaltyMate'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnLoyaltyMateMouseClick
          end
          object TouchBtnBarExchange: TTouchBtn
            Left = 28
            Top = 380
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'BarExchange'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnBarExchangeMouseClick
          end
          object btnAccountingInterface: TTouchBtn
            Left = 247
            Top = 310
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Accounting Interface'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnAccountingInterfaceMouseClick
          end
          object tchbtnDrinkCommand: TTouchBtn
            Left = 247
            Top = 380
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'DrinkCommand'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tchbtnDrinkCommandMouseClick
          end
          object TouchBtnClipInterface: TTouchBtn
            Left = 28
            Top = 450
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Clipp Interface'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            Enabled = False
            LatchingProperties = [lkLatchColor, lkStayDown]
          end
          object TouchBtnRunRateBoard: TTouchBtn
            Left = 28
            Top = 521
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Run Rate Board'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnRunRateBoardMouseClick
          end
          object TouchBtnThorlink: TTouchBtn
            Left = 247
            Top = 450
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Thorlink'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            Enabled = False
            LatchingProperties = [lkLatchColor, lkStayDown]
          end
          object TouchBtnSecurity: TTouchBtn
            Left = 247
            Top = 520
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Security'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnSecurityMouseClick
          end
          object TouchBtnFiscalStorage: TTouchBtn
            Left = 28
            Top = 584
            Font.Charset = THAI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'POS Plus'
            ButtonColor = 6052441
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = TouchBtnFiscalMouseClick
          end
        end
        object Panel16: TPanel
          Left = 1
          Top = 1
          Width = 840
          Height = 41
          Align = alTop
          Caption = 'Interfaces'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
      end
    end
  end
end
