object frmSecurity: TfrmSecurity
  Tag = 1024
  Left = 31
  Top = 45
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Security Options'
  ClientHeight = 700
  ClientWidth = 922
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 20
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 922
    Height = 593
    Align = alTop
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 1
    object Panel13: TPanel
      Left = 761
      Top = 0
      Width = 161
      Height = 593
      Align = alRight
      BorderWidth = 5
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object TouchBtn1: TTouchBtn
        Left = 6
        Top = 513
        Width = 149
        Height = 74
        Align = alBottom
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Quit'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnCancelClick
      end
      object btnMinimise: TTouchBtn
        Left = 6
        Top = 6
        Width = 149
        Height = 73
        Align = alTop
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Ok'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnAcceptClick
      end
    end
    object Panel1: TPanel
      Left = 0
      Top = 0
      Width = 753
      Height = 593
      Align = alLeft
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      ExplicitLeft = 2
      ExplicitTop = 4
      object cbPOS: TCheckBox
        Left = 16
        Top = 16
        Width = 225
        Height = 65
        Caption = 'POS'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = cbPOSClick
      end
      object cbSetup: TCheckBox
        Left = 264
        Top = 16
        Width = 225
        Height = 65
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
      end
      object cbMaintain: TCheckBox
        Left = 512
        Top = 16
        Width = 225
        Height = 65
        Caption = 'Maintenance'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = cbMaintainClick
      end
      object cbAccessZeds: TCheckBox
        Left = 264
        Top = 85
        Width = 225
        Height = 20
        Caption = 'Access '#39'Z'#39' Reports'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnClick = cbMaintainClick
      end
      object cbPINChange: TCheckBox
        Left = 264
        Top = 205
        Width = 225
        Height = 20
        Caption = 'Change PIN'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        OnClick = cbMaintainClick
      end
      object cbQuit: TCheckBox
        Left = 264
        Top = 245
        Width = 225
        Height = 20
        Caption = 'Allow Quit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        OnClick = cbMaintainClick
      end
      object cbMenus: TCheckBox
        Left = 528
        Top = 88
        Width = 209
        Height = 41
        Caption = 'Import Menus'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 6
      end
      object cbUsers: TCheckBox
        Left = 528
        Top = 136
        Width = 209
        Height = 41
        Caption = 'Add/Edit Users'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 7
      end
      object cbPrinters: TCheckBox
        Left = 528
        Top = 184
        Width = 209
        Height = 41
        Caption = 'Configure Printers'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 8
      end
      object cbDisableFD: TCheckBox
        Left = 32
        Top = 85
        Width = 209
        Height = 37
        Caption = 'Manage Menus'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 9
      end
      object cdCredit: TCheckBox
        Left = 32
        Top = 128
        Width = 209
        Height = 37
        Caption = 'Credit/Cancel Rights'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 10
      end
      object cbPriceAj: TCheckBox
        Left = 528
        Top = 485
        Width = 209
        Height = 41
        Caption = 'Misc Discounts'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 11
      end
      object cbDiscount: TCheckBox
        Left = 512
        Top = 424
        Width = 225
        Height = 65
        Caption = 'Surcharge/Discount Bill'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 12
      end
      object cbPaymentTypes: TCheckBox
        Left = 528
        Top = 232
        Width = 209
        Height = 41
        Caption = 'Payment Types'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 13
      end
      object cbSetFloat: TCheckBox
        Left = 264
        Top = 125
        Width = 225
        Height = 20
        Caption = 'Set Float'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
        OnClick = cbMaintainClick
      end
      object cbAdjustFloat: TCheckBox
        Left = 264
        Top = 165
        Width = 225
        Height = 20
        Caption = 'Alter Float'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 15
        OnClick = cbMaintainClick
      end
      object cbSec1: TCheckBox
        Left = 544
        Top = 280
        Width = 193
        Height = 41
        Caption = 'Secure Payments 1'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 16
      end
      object cbSec2: TCheckBox
        Left = 544
        Top = 328
        Width = 193
        Height = 41
        Caption = 'Secure Payments 2'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 17
      end
      object cbSec3: TCheckBox
        Left = 544
        Top = 376
        Width = 193
        Height = 41
        Caption = 'Secure Payments 3'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 18
      end
      object cbTabPINOverride: TCheckBox
        Left = 32
        Top = 171
        Width = 209
        Height = 37
        Caption = 'Override Tab PIN'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 19
      end
      object tbOpenDrawer: TCheckBox
        Left = 32
        Top = 214
        Width = 209
        Height = 37
        Caption = 'Open Cash Drawer'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 20
      end
      object cbAccountManager: TCheckBox
        Left = 32
        Top = 257
        Width = 209
        Height = 37
        Caption = 'Account Manager'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 21
      end
      object tcbTransToInvoice: TCheckBox
        Left = 264
        Top = 325
        Width = 225
        Height = 20
        Caption = 'Transfer to Invoice'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 29
      end
      object cbTaxRemoval: TCheckBox
        Left = 264
        Top = 405
        Width = 225
        Height = 20
        Caption = 'Tax Removal'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 22
      end
      object tcbPayInvoice: TCheckBox
        Left = 264
        Top = 285
        Width = 225
        Height = 20
        Caption = 'Pay Invoice'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 23
      end
      object cbViewMembersDetials: TCheckBox
        Left = 32
        Top = 386
        Width = 209
        Height = 37
        Caption = 'View Members Details'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 24
      end
      object tcbAccountCreation: TCheckBox
        Left = 32
        Top = 300
        Width = 209
        Height = 37
        Caption = 'Account Creation'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 26
      end
      object cbWriteOff: TCheckBox
        Left = 32
        Top = 343
        Width = 209
        Height = 37
        Caption = 'Write Off Rights'
        Color = clGray
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial Narrow'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 25
      end
      object cbReprintReceipt: TCheckBox
        Left = 32
        Top = 472
        Width = 209
        Height = 37
        Caption = 'Reprint Receipt'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 30
      end
      object cbPaymentAccess: TCheckBox
        Left = 32
        Top = 429
        Width = 209
        Height = 37
        Caption = 'Payment Access'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 28
      end
      object cbAllowReprintOrders: TCheckBox
        Left = 32
        Top = 515
        Width = 209
        Height = 37
        Caption = 'Reprint Orders'
        Color = clGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 32
      end
      object tcbViewTransAudit: TCheckBox
        Left = 264
        Top = 365
        Width = 225
        Height = 20
        Caption = 'View Transactions Audit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 27
      end
      object cbAllowPointsRefund: TCheckBox
        Left = 264
        Top = 445
        Width = 185
        Height = 20
        Caption = 'Allow Points Refund'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 31
      end
      object cbAllowForcedHappyHour: TCheckBox
        Left = 264
        Top = 481
        Width = 235
        Height = 20
        Caption = 'Allow Forced Happy Hour'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 33
      end
    end
  end
  object Panel14: TPanel
    Left = 0
    Top = 603
    Width = 922
    Height = 97
    Align = alBottom
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnTemplateAdmin: TPanel
      Left = 16
      Top = 18
      Width = 145
      Height = 63
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Administrator'
      Color = 9933970
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
      OnClick = btnTemplateAdminClick
    end
    object btnTemplateSuper: TPanel
      Left = 185
      Top = 18
      Width = 145
      Height = 63
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Supervisor'
      Color = 9933970
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      OnClick = btnTemplateSuperClick
    end
    object btnTemplateSales: TPanel
      Left = 354
      Top = 18
      Width = 145
      Height = 63
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Sales'
      Color = 9933970
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 2
      OnClick = btnTemplateSalesClick
    end
    object btnTemplateNone: TPanel
      Left = 692
      Top = 18
      Width = 145
      Height = 63
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'None'
      Color = 9933970
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 3
      OnClick = btnTemplateNoneClick
    end
    object btnTemplateWaiter: TPanel
      Left = 523
      Top = 18
      Width = 145
      Height = 63
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Waiter'
      Color = 9933970
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentBackground = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 4
      OnClick = btnTemplateWaiterClick
    end
  end
end
