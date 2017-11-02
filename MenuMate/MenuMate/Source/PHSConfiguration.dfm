object frmPHSConfiguration: TfrmPHSConfiguration
  Left = 435
  Top = 231
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Property Management System Configuration'
  ClientHeight = 445
  ClientWidth = 759
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel24: TPanel
    Left = 8
    Top = 8
    Width = 600
    Height = 435
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    DesignSize = (
      600
      435)
    object Label4: TLabel
      Left = 152
      Top = 7
      Width = 348
      Height = 19
      Caption = 'Property Management System Configuration'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object tbPhoenixIPAddress: TTouchBtn
      Left = 310
      Top = 32
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Server TCP IP Address'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixIPAddressClick
    end
    object tbPhoenixPortNumber: TTouchBtn
      Left = 16
      Top = 88
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Server Port Number'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixPortNumberClick
    end
    object tbPhoenixID: TTouchBtn
      Left = 16
      Top = 32
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'P.O.S ID'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixIDClick
    end
    object tbPaymentDefCat: TTouchBtn
      Left = 310
      Top = 88
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Default Payment Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPaymentDefCatClick
    end
    object tbItemDefCat: TTouchBtn
      Left = 16
      Top = 144
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Default Item Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbItemDefCatClick
    end
    object tbPointCat: TTouchBtn
      Left = 310
      Top = 144
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Points Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPointCatClick
    end
    object tbCreditCat: TTouchBtn
      Left = 16
      Top = 200
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Credit Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbCreditCatClick
    end
    object tbDefTransAccount: TTouchBtn
      Left = 310
      Top = 255
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Default Transaction Account'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbDefTransAccountClick
    end
    object tbSurchargeCat: TTouchBtn
      Left = 310
      Top = 200
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Surcharge Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbSurchargeCatClick
    end
    object tbRoundingCategory: TTouchBtn
      Left = 16
      Top = 256
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Rounding Category'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbRoundingCategoryClick
    end
    object tbTipAccount: TTouchBtn
      Left = 16
      Top = 368
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Tip Account'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbTipAccountClick
    end
    object tbExpensesAccount: TTouchBtn
      Left = 310
      Top = 312
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Expenses Account'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbExpensesAccountClick
    end
    object tbServiceCharge: TTouchBtn
      Left = 16
      Top = 312
      Width = 266
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Service Charge'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbServiceChargeMouseClick
    end
    object cbEnableCustomerJourney: TCheckBox
      Left = 310
      Top = 368
      Width = 266
      Height = 49
      Anchors = [akLeft]
      Caption = 'Enable customer Journey'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = cbEnableCustomerJourneyClick
    end
  end
  object Panel2: TPanel
    Left = 615
    Top = 8
    Width = 137
    Height = 435
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 96
      Width = 121
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Replace Code'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object btnOk: TTouchBtn
      Left = 8
      Top = 16
      Width = 121
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
    end
  end
end
