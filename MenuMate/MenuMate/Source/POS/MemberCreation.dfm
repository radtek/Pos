object frmMemberCreation: TfrmMemberCreation
  Left = 0
  Top = 0
  AutoSize = True
  Caption = 'Member'#39's Details'
  ClientHeight = 400
  ClientWidth = 837
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 837
    Height = 400
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object lbeEmail: TLabel
      Left = 190
      Top = 45
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeHeader: TLabel
      Left = 10
      Top = 4
      Width = 400
      Height = 16
      Caption = 
        'Fields marked with * are mandatory to be entered to create membe' +
        'r.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbeName: TLabel
      Left = 190
      Top = 135
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeLastName: TLabel
      Left = 190
      Top = 195
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object btnOk: TTouchBtn
      Left = 559
      Top = 323
      Width = 120
      Height = 55
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
      OnMouseClick = btnOkMouseClick
    end
    object TouchBtn1: TTouchBtn
      Left = 694
      Top = 323
      Width = 120
      Height = 55
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
      OnMouseClick = TouchBtn1MouseClick
    end
    object btnEmail: TTouchBtn
      Left = 28
      Top = 30
      Width = 150
      Height = 53
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Email*'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = EditCustomerBasicDetails
    end
    object cbNoEmail: TCheckBox
      Left = 28
      Top = 104
      Width = 167
      Height = 25
      Caption = 'Email not available'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = cbNoEmailMouseClick
    end
    object btnLastName: TTouchBtn
      Tag = 2
      Left = 28
      Top = 220
      Width = 150
      Height = 53
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Last Name*'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = EditCustomerBasicDetails
    end
    object TouchBtn2: TTouchBtn
      Left = 424
      Top = 323
      Width = 120
      Height = 55
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Gift Card'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
    object GroupBox1: TGroupBox
      Left = 424
      Top = 4
      Width = 413
      Height = 192
      Align = alCustom
      Caption = 'Date Of Birth'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 5
      object Label10: TLabel
        Left = 21
        Top = 52
        Width = 32
        Height = 23
        Caption = 'Day'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 149
        Top = 52
        Width = 53
        Height = 23
        Caption = 'Month'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label12: TLabel
        Left = 277
        Top = 52
        Width = 38
        Height = 23
        Caption = 'Year'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object tbtnDay: TTouchBtn
        Left = 12
        Top = 80
        Width = 122
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '01'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnDayMouseClick
      end
      object tbtnMonth: TTouchBtn
        Left = 140
        Top = 80
        Width = 122
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '01'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnMonthMouseClick
      end
      object tbtnYear: TTouchBtn
        Left = 268
        Top = 80
        Width = 122
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '1899'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnYearMouseClick
      end
    end
    object btnName: TTouchBtn
      Tag = 1
      Left = 28
      Top = 143
      Width = 150
      Height = 53
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'First Name*'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = EditCustomerBasicDetails
    end
    object gbLoyaltyMateActivation: TGroupBox
      Left = 1
      Top = 297
      Width = 408
      Height = 89
      Caption = 'LoyaltyMate Activation'
      Color = 14342874
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentColor = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 7
      object lbeLoyaltyMateActive: TLabel
        Left = 12
        Top = 40
        Width = 186
        Height = 23
        Caption = 'Member Not Activated'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object btnActivateLoyaltyMate: TTouchBtn
        Left = 202
        Top = 29
        Width = 186
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Activate Member'
        ButtonColor = 6052441
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnActivateLoyaltyMateMouseClick
      end
    end
  end
end
