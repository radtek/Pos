object frmTierLevelEditor: TfrmTierLevelEditor
  Tag = 1024
  Left = 33
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Editing Tier Level.'
  ClientHeight = 552
  ClientWidth = 623
  Color = clBlack
  TransparentColorValue = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = -4
    Width = 465
    Height = 557
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
    object btnTierName: TTouchBtn
      Left = 24
      Top = 24
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Tier Name'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTierNameMouseClick
    end
    object btnTierLevel: TTouchBtn
      Left = 232
      Top = 24
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Level'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTierLevelMouseClick
    end
    object btnPointsRequired: TTouchBtn
      Left = 24
      Top = 111
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Required'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPointsRequiredMouseClick
    end
    object btnBdayBonus: TTouchBtn
      Left = 232
      Top = 111
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Birthday Bonus'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnBdayBonusMouseClick
    end
    object GroupBox1: TGroupBox
      Left = 24
      Top = 200
      Width = 393
      Height = 249
      Caption = 'Spend Points'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object Label1: TLabel
        Left = 16
        Top = 32
        Width = 92
        Height = 18
        Caption = 'Priced Product'
      end
      object Label2: TLabel
        Left = 16
        Top = 96
        Width = 110
        Height = 18
        Caption = 'Weighed Product'
      end
      object Label3: TLabel
        Left = 319
        Top = 112
        Width = 43
        Height = 18
        Caption = 'Grams'
      end
      object Label4: TLabel
        Left = 319
        Top = 184
        Width = 38
        Height = 18
        Caption = 'Points'
      end
      object btnPricedPoints: TTouchBtn
        Left = 144
        Top = 32
        Width = 169
        Height = 50
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '0.00'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnPricedPointsMouseClick
      end
      object btnWeighedGrams: TTouchBtn
        Left = 144
        Top = 96
        Width = 169
        Height = 50
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '0'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnWeighedGramsMouseClick
      end
      object btnWeighedPoints: TTouchBtn
        Left = 144
        Top = 167
        Width = 169
        Height = 50
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '0'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnWeighedPointsMouseClick
      end
    end
    object cbEmailHeadOffice: TCheckBox
      Left = 24
      Top = 455
      Width = 393
      Height = 28
      Caption = 'Send Mail to Head Office On Tier Level Change'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object cbEmailMember: TCheckBox
      Left = 24
      Top = 480
      Width = 385
      Height = 28
      Caption = 'Send Mail to Member On Tier Level Change'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
    end
    object cbChangeCard: TCheckBox
      Left = 24
      Top = 505
      Width = 385
      Height = 28
      Caption = 'Change Card On Tier Level Change'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
    end
    object cbAllowEarntRedemption: TCheckBox
      Left = 24
      Top = 529
      Width = 385
      Height = 28
      Caption = 'Allow Earnt Loyalty Redemption'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
    end
  end
  object Panel2: TPanel
    Left = 471
    Top = -4
    Width = 150
    Height = 557
    BevelOuter = bvNone
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 5
      Top = 10
      Width = 140
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 5
      Top = 477
      Width = 140
      Height = 73
      Font.Charset = DEFAULT_CHARSET
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
      OnMouseClick = btnCancelMouseClick
    end
  end
end
