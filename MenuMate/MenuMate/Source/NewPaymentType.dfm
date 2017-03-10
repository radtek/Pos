object frmNewPaymentType: TfrmNewPaymentType
  Tag = 1024
  Left = 0
  Top = 0
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Payment Type'
  ClientHeight = 618
  ClientWidth = 955
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  DesignSize = (
    955
    618)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlButtons: TPanel
    Left = 829
    Top = 0
    Width = 130
    Height = 618
    Alignment = taRightJustify
    Anchors = [akTop, akRight, akBottom]
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object tbPayment: TTouchBtn
      Left = 10
      Top = 81
      Width = 110
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Payment'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPaymentMouseClick
    end
    object tbSecurity: TTouchBtn
      Left = 10
      Top = 155
      Width = 110
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Security'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbSecurityMouseClick
    end
    object tbInterfaces: TTouchBtn
      Left = 10
      Top = 228
      Width = 110
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Interfaces'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbInterfacesMouseClick
    end
    object tbtnOk: TTouchBtn
      Tag = -1
      Left = 10
      Top = 6
      Width = 110
      Font.Charset = ANSI_CHARSET
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
      OnMouseClick = pnlOkClick
    end
    object TouchBtn2: TTouchBtn
      Tag = -1
      Left = 10
      Top = 614
      Width = 110
      Height = 60
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = Panel20Click
    end
  end
  object Pages: TPageControl
    Left = 0
    Top = 0
    Width = 884
    Height = 618
    ActivePage = tsInterfaces
    Align = alLeft
    TabOrder = 1
    object tsPayment: TTabSheet
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 876
        Height = 608
        Align = alClient
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object Label5: TLabel
          Left = 16
          Top = 16
          Width = 40
          Height = 18
          Caption = 'Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label1: TLabel
          Left = 264
          Top = 16
          Width = 102
          Height = 18
          Caption = 'Button Colour'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object tbExchange: TTouchBtn
          Left = 256
          Top = 224
          Width = 200
          Height = 76
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Visible = False
          Caption = 'Exchange Rate 1.0000'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbExchangeClick
        end
        object GroupBox3: TGroupBox
          Left = 16
          Top = 430
          Width = 233
          Height = 220
          Caption = 'Payment Fees && Charges'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object cbSurcharge: TCheckBox
            Left = 16
            Top = 91
            Width = 200
            Height = 49
            Caption = 'Fixed Surcharge'
            Color = clGreen
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
            OnClick = cbSurchargeClick
          end
          object cbIsTip: TCheckBox
            Left = 16
            Top = 26
            Width = 200
            Height = 51
            Caption = 'Custom Surcharge'
            Color = clGreen
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
            OnClick = cbIsTipClick
          end
          object tbSurchargeTaxRate: TTouchBtn
            Left = 16
            Top = 152
            Width = 200
            Height = 60
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Surcharge Tax Rate'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbSurchargeTaxRateClick
          end
        end
        object GroupBox4: TGroupBox
          Left = 256
          Top = 430
          Width = 233
          Height = 220
          Caption = 'Payment Grouping'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object cbTaxFree: TCheckBox
            Left = 16
            Top = 88
            Width = 200
            Height = 51
            Caption = 'Tax Free'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
          end
          object tbGroupNumber: TTouchBtn
            Left = 16
            Top = 32
            Width = 200
            Height = 51
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Group Number'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbGroupNumberClick
          end
        end
        object GroupBox5: TGroupBox
          Left = 16
          Top = 216
          Width = 233
          Height = 209
          Caption = 'Payment Type'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object cbIsCash: TCheckBox
            Left = 16
            Top = 30
            Width = 200
            Height = 51
            Caption = 'Treat As Cash'
            Color = clGreen
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
            OnClick = cbIsCashClick
          end
          object cbOpendrawer: TCheckBox
            Left = 16
            Top = 90
            Width = 200
            Height = 51
            Caption = 'Opens Cashdrawer'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
          end
          object cbGetVoucherDetails: TCheckBox
            Left = 16
            Top = 150
            Width = 200
            Height = 51
            Caption = 'Collect Voucher Details'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 2
            OnClick = cbGetVoucherDetailsClick
          end
        end
        object GroupBox6: TGroupBox
          Left = 495
          Top = 430
          Width = 233
          Height = 220
          Caption = 'Payment Infomation'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          object cbGetCardDetails: TCheckBox
            Left = 16
            Top = 30
            Width = 200
            Height = 51
            Caption = 'Enter Card Details'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
            Visible = False
          end
        end
        object btnName: TTouchBtn
          Left = 16
          Top = 48
          Width = 233
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Name'
          ButtonColor = clWhite
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnNameClick
        end
        object tbPosition: TTouchBtn
          Left = 16
          Top = 136
          Width = 97
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Position'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbPositionClick
        end
        object tbRounding: TTouchBtn
          Left = 152
          Top = 136
          Width = 97
          Height = 73
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Round To'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbRoundingMouseClick
        end
        object Panel10: TPanel
          Left = 1
          Top = 1
          Width = 874
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Payment Types'
          Color = 2200566
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 8
        end
      end
    end
    object tsSecurity: TTabSheet
      Caption = 'tsSecurity'
      ImageIndex = 1
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 876
        Height = 608
        Align = alClient
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object Panel3: TPanel
          Left = 1
          Top = 1
          Width = 874
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Payment Types: Security'
          Color = 2200566
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object GroupBox1: TGroupBox
          Left = 10
          Top = 48
          Width = 242
          Height = 270
          Caption = 'Payment Security'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
          object cbSec1: TCheckBox
            Left = 10
            Top = 86
            Width = 220
            Height = 51
            Caption = 'Secure Payment 1'
            Color = clTeal
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
          end
          object cbSec2: TCheckBox
            Left = 10
            Top = 142
            Width = 220
            Height = 51
            Caption = 'Secure Payment 2'
            Color = clTeal
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
          end
          object cbSec3: TCheckBox
            Left = 10
            Top = 198
            Width = 220
            Height = 51
            Caption = 'Secure Payment 3'
            Color = clTeal
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 2
          end
          object cbReqNote: TCheckBox
            Left = 10
            Top = 29
            Width = 220
            Height = 51
            Caption = 'Requires Note'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 3
          end
        end
      end
    end
    object tsInterfaces: TTabSheet
      Caption = 'tsInterfaces'
      ImageIndex = 2
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 876
        Height = 608
        Align = alClient
        BevelOuter = bvNone
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object Panel5: TPanel
          Left = 0
          Top = 0
          Width = 876
          Height = 41
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Payment Types: Interfaces'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object GroupBox7: TGroupBox
          Left = 10
          Top = 48
          Width = 240
          Height = 430
          Caption = 'Electronic Payments'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 1
          object cbCashOut: TCheckBox
            Left = 10
            Top = 142
            Width = 220
            Height = 51
            Caption = 'Allow Cash Out'
            Color = clGreen
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
            OnClick = cbCashOutClick
          end
          object cbIntegrated: TCheckBox
            Left = 10
            Top = 86
            Width = 220
            Height = 51
            Caption = 'Integrated  EFTPOS'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
            OnClick = cbIntegratedClick
          end
          object cbAllowReversal: TCheckBox
            Left = 10
            Top = 198
            Width = 220
            Height = 51
            Caption = 'Allow Reversal'
            Color = clMaroon
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 2
            OnClick = cbAllowReversalClick
          end
          object cbElectronicTransaction: TCheckBox
            Left = 10
            Top = 30
            Width = 220
            Height = 51
            Caption = 'Electronic Transaction'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 3
            OnClick = cbElectronicTransactionClick
          end
          object tbChequeVerify: TCheckBox
            Left = 10
            Top = 254
            Width = 220
            Height = 51
            Caption = 'Enable Cheque Verification'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 4
            OnClick = tbChequeVerifyClick
          end
          object cbAllowManPan: TCheckBox
            Left = 10
            Top = 310
            Width = 220
            Height = 51
            Caption = 'Allow Manual Card Entry'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 6
          end
          object cbAllowTips: TCheckBox
            Left = 10
            Top = 366
            Width = 220
            Height = 51
            Caption = 'Allow Tips'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 5
          end
        end
        object GroupBox8: TGroupBox
          Left = 10
          Top = 483
          Width = 240
          Height = 153
          Caption = 'Manual Checks'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
          object cbCheckAccepted: TCheckBox
            Left = 10
            Top = 32
            Width = 220
            Height = 51
            Caption = 'Accepted Confirmation'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
          end
          object cbCheckSig: TCheckBox
            Left = 10
            Top = 88
            Width = 220
            Height = 51
            Caption = 'Signature Confirmation'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
          end
        end
        object GroupBox2: TGroupBox
          Left = 260
          Top = 48
          Width = 240
          Height = 296
          Caption = 'Payment Exporting'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 3
          object Bevel2: TBevel
            Left = 10
            Top = 162
            Width = 220
            Height = 2
          end
          object cbCSVPaymentType: TCheckBox
            Left = 10
            Top = 24
            Width = 220
            Height = 40
            Caption = 'CSV Payment Type'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
          end
          object tbInvoiceInterface: TCheckBox
            Left = 10
            Top = 70
            Width = 220
            Height = 40
            Caption = 'Invoice Interface'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 1
            OnClick = tbInvoiceInterfaceClick
          end
          object CheckBoxExport: TCheckBox
            Left = 10
            Top = 116
            Width = 220
            Height = 40
            Caption = 'Charge To Account'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 2
            OnClick = ExportMouseClick
          end
          object tbChargeToXero: TCheckBox
            Left = 10
            Top = 168
            Width = 220
            Height = 51
            Caption = 'Charge To Accounting System'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 3
            WordWrap = True
            OnClick = tbChargeToXeroClick
          end
          object tbGLCode: TTouchBtn
            Left = 10
            Top = 225
            Width = 220
            Height = 60
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'GL Code'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbGLCodeMouseClick
          end
        end
        object GroupBox9: TGroupBox
          Left = 260
          Top = 350
          Width = 240
          Height = 286
          Caption = 'PMS System'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 4
          object tbtnSecondaryPMSPort: TTouchBtn
            Left = 10
            Top = 199
            Width = 220
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Secondary PMS Port'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSecondaryPMSPortClick
          end
          object tbtnSecondaryIPAddress: TTouchBtn
            Left = 10
            Top = 120
            Width = 220
            Height = 73
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Secondary PMS IP Address'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnSecondaryIPAddressClick
          end
          object tbThirdPartyID: TTouchBtn
            Left = 10
            Top = 63
            Width = 220
            Height = 51
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = '3rd Party Payment ID'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbThirdPartyIDClick
          end
          object tbRoomPayment: TCheckBox
            Left = 10
            Top = 24
            Width = 220
            Height = 33
            Caption = 'PMS / Room Interface'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 3
            OnClick = tbRoomPaymentClick
          end
        end
        object GroupBox10: TGroupBox
          Left = 510
          Top = 48
          Width = 240
          Height = 310
          Caption = 'Universal Vouchers'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          object cbPocketVoucher: TCheckBox
            Left = 10
            Top = 24
            Width = 220
            Height = 41
            Caption = 'Enable Online Vouchers'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
            OnClick = cbPocketVoucherClick
          end
          object tbtnVoucherMerchant: TTouchBtn
            Left = 10
            Top = 185
            Width = 220
            Height = 51
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Voucher Merchant ID'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnVoucherMerchantMouseClick
          end
          object cbPVAcceptedMsg: TCheckBox
            Left = 10
            Top = 242
            Width = 220
            Height = 51
            Caption = 'Display Voucher Accepted Msg'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 3
            WordWrap = True
          end
          object tbtnUniUser: TTouchBtn
            Left = 10
            Top = 71
            Width = 220
            Height = 51
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Universal User'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnUniUserMouseClick
          end
          object tbtnUniPass: TTouchBtn
            Left = 10
            Top = 128
            Width = 220
            Height = 51
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Universal Password'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbtnUniPassMouseClick
          end
        end
        object GroupBox11: TGroupBox
          Left = 510
          Top = 364
          Width = 240
          Height = 151
          Caption = 'RMS System'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 6
          object tbRMSWriteLocation: TTouchBtn
            Left = 10
            Top = 103
            Width = 220
            Height = 32
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'CSV Write Location'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbRMSWriteLocationMouseClick
          end
          object tbRMSReadLocation: TTouchBtn
            Left = 10
            Top = 63
            Width = 220
            Height = 34
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'CSV Read Location'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = 10011108
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tbRMSReadLocationMouseClick
          end
          object cbRMSInterface: TCheckBox
            Left = 10
            Top = 24
            Width = 220
            Height = 33
            Caption = 'RMS / Room Interface'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 2
            OnClick = cbRMSInterfaceClick
          end
        end
        object GroupBox12: TGroupBox
          Left = 510
          Top = 510
          Width = 240
          Height = 70
          Caption = 'Blind Balance'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 7
          object cbAutoPopulateBlindBalance: TCheckBox
            Left = 10
            Top = 24
            Width = 220
            Height = 33
            Caption = 'Auto Populate Blind Balance'
            Color = clNavy
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentColor = False
            ParentFont = False
            TabOrder = 0
          end
        end
        object tbTabLink: TTouchBtn
          Left = 757
          Top = 58
          Width = 100
          Height = 51
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Tab Link'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbTabLinkMouseClick
        end
      end
    end
  end
end
