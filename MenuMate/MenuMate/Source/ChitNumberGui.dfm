object frmChitNumberGui: TfrmChitNumberGui
  Tag = 1024
  Left = 487
  Top = 139
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 5
  Caption = 'Chit Number'
  ClientHeight = 755
  ClientWidth = 745
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMajor: TPanel
    Left = 0
    Top = 0
    Width = 605
    Height = 755
    Align = alLeft
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object pnlLabel: TPanel
      Left = 7
      Top = 7
      Width = 591
      Height = 34
      Align = alTop
      BevelOuter = bvLowered
      Caption = 'Chit'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
    end
    object tbtnName: TTouchBtn
      Left = 8
      Top = 55
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Name :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = NameClicked
    end
    object tbtnType: TTouchBtn
      Left = 8
      Top = 130
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Type :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TypeClicked
    end
    object tbtnMask: TTouchBtn
      Left = 318
      Top = 55
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Mask :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = MaskClicked
    end
    object tbtnFormat: TTouchBtn
      Left = 318
      Top = 130
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Format :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = FormatClicked
    end
    object tbtnDefaultMenu: TTouchBtn
      Left = 8
      Top = 205
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Default  Menu :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = DefaultMenuClicked
    end
    object tbtnMaxChitNo: TTouchBtn
      Left = 318
      Top = 205
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Max Chit Number :'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnMaxChitNoMouseClick
    end
    object tbtnRemoveTax: TTouchBtn
      Left = 8
      Top = 280
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Remove Tax'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnRemoveTaxMouseClick
    end
    object tbtnRemoveServiceCharge: TTouchBtn
      Left = 318
      Top = 280
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Remove Service Charge'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnRemoveServiceChargeMouseClick
    end
    object tbtnSelectDiscount: TTouchBtn
      Left = 8
      Top = 355
      Width = 280
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Select Discounts'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSelectDiscountMouseClick
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 420
      Width = 590
      Height = 330
      TabOrder = 7
      object cbDisableHoldAndSend: TCheckBox
        Left = 24
        Top = 20
        Width = 450
        Height = 17
        Caption = 'Don'#39't use Hold and Send'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = ToggleHoldAndSendForThisChit
      end
      object cbEnableTableDisplay: TCheckBox
        Left = 24
        Top = 45
        Width = 450
        Height = 17
        Caption = 'Display Tables'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = cbEnableTableDisplayClick
      end
      object cbNonChargeable: TCheckBox
        Left = 24
        Top = 70
        Width = 450
        Height = 17
        Caption = 'Non-Chargeable Chit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = cbNonChargeableClick
      end
      object cbCaptureCustomerNameAndPhone: TCheckBox
        Left = 24
        Top = 95
        Width = 450
        Height = 17
        Caption = 'Capture Customer Name And Phone Number'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnClick = cbCaptureCustomerNameAndPhoneClick
      end
      object cbCaptureCustomerDetails: TCheckBox
        Left = 24
        Top = 120
        Width = 450
        Height = 17
        Caption = 'Capture Customer Details'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        OnClick = cbCaptureCustomerDetailsClick
      end
      object cbAutoSaveOnTab: TCheckBox
        Left = 24
        Top = 145
        Width = 450
        Height = 17
        Caption = 'Auto Save on Tab'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        OnClick = cbAutoSaveOnTabClick
      end
      object cbPromptDeliveryTime: TCheckBox
        Left = 24
        Top = 170
        Width = 330
        Height = 17
        Caption = 'Prompt For PickUp/Delivery Time'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        OnClick = cbPromptDeliveryTimeClick
      end
      object cbSaveCustomerName: TCheckBox
        Left = 24
        Top = 195
        Width = 435
        Height = 17
        Caption = 'Save CUSTOMER NAME as Party Name on Table'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
        OnClick = cbSaveCustomerNameClick
      end
      object cbSaveMemberName: TCheckBox
        Left = 24
        Top = 220
        Width = 435
        Height = 17
        Caption = 'Save MEMBER NAME as Party Name on Table'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 9
        OnClick = cbSaveMemberNameClick
      end
      object cbSaveMemberAddress: TCheckBox
        Left = 24
        Top = 245
        Width = 470
        Height = 17
        Caption = 'Save ADDRESS (First Line) as Party Name on Table'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
        OnClick = cbSaveMemberAddressClick
      end
      object cbOnlinePickupOrder: TCheckBox
        Left = 24
        Top = 270
        Width = 470
        Height = 17
        Caption = 'Online Order Pickup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
        OnClick = cbOnlinePickupOrderClick
      end
      object cbOnlineDeliveryOrder: TCheckBox
        Left = 24
        Top = 295
        Width = 470
        Height = 17
        Caption = 'Online Order Delivery'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuText
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
        OnClick = cbOnlineDeliveryOrderClick
      end
    end
    object tbtnAddMinutes: TTouchBtn
      Left = 380
      Top = 565
      Width = 180
      Height = 47
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = '+X minutes'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAddMinutesMouseClick
    end
  end
  object pnlMinor: TPanel
    Left = 611
    Top = 0
    Width = 134
    Height = 755
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 5
      Top = 5
      Width = 124
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = OkClicked
    end
    object btnCancel: TTouchBtn
      Left = 5
      Top = 693
      Width = 124
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CancelClicked
    end
  end
end
