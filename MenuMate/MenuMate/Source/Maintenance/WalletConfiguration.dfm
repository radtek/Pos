object frmWalletConfiguration: TfrmWalletConfiguration
  Left = 0
  Top = 0
  AutoSize = True
  Caption = 'Wallet Configuration'
  ClientHeight = 400
  ClientWidth = 789
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
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 639
    Height = 394
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object lbeMechentId: TLabel
      Left = 190
      Top = 254
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbePassword: TLabel
      Left = 190
      Top = 114
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeTerminalId: TLabel
      Left = 190
      Top = 324
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeUserName: TLabel
      Left = 190
      Top = 44
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeSecurityToken: TLabel
      Left = 192
      Top = 184
      Width = 5
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object btnMerchentId: TTouchBtn
      Left = 20
      Top = 240
      Width = 150
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Merchant Id'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      Enabled = False
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMerchentIdMouseClick
    end
    object btnUserName: TTouchBtn
      Tag = 2
      Left = 20
      Top = 30
      Width = 150
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'User Name'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnUserNameMouseClick
    end
    object btnWalletPassword: TTouchBtn
      Tag = 3
      Left = 20
      Top = 100
      Width = 150
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Password'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnWalletPasswordMouseClick
    end
    object btnTerminalId: TTouchBtn
      Tag = 1
      Left = 20
      Top = 310
      Width = 150
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Terminal Id'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      Enabled = False
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTerminalIdMouseClick
    end
    object btnSecurityToken: TTouchBtn
      Tag = 3
      Left = 20
      Top = 170
      Width = 150
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'SecurityToken'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSecurityTokenMouseClick
    end
  end
  object Panel2: TPanel
    AlignWithMargins = True
    Left = 648
    Top = 3
    Width = 138
    Height = 394
    Align = alRight
    Alignment = taRightJustify
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnCancel: TTouchBtn
      Left = 10
      Top = 310
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
      OnMouseClick = btnCancelMouseClick
    end
    object btnOk: TTouchBtn
      Left = 9
      Top = 30
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
  end
end
