object frmSalesforceAuthInfo: TfrmSalesforceAuthInfo
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Salesorce Credentials Settings'
  ClientHeight = 360
  ClientWidth = 820
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 820
    Height = 5
    Align = alTop
    Shape = bsSpacer
    ExplicitTop = 41
    ExplicitWidth = 654
  end
  object Bevel2: TBevel
    Left = 0
    Top = 355
    Width = 820
    Height = 5
    Align = alBottom
    Shape = bsSpacer
    ExplicitTop = 49
    ExplicitWidth = 654
  end
  object Bevel3: TBevel
    Left = 0
    Top = 5
    Width = 5
    Height = 350
    Align = alLeft
    Shape = bsSpacer
    ExplicitTop = 47
    ExplicitHeight = 535
  end
  object Bevel4: TBevel
    Left = 815
    Top = 5
    Width = 5
    Height = 350
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 0
    ExplicitTop = 581
    ExplicitHeight = 654
  end
  object Bevel5: TBevel
    Left = 697
    Top = 5
    Width = 5
    Height = 350
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 8
    ExplicitTop = 51
    ExplicitHeight = 535
  end
  object Panel1: TPanel
    Left = 702
    Top = 5
    Width = 113
    Height = 350
    Align = alRight
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    object btnUpdateCredentials: TTouchBtn
      Left = 6
      Top = 6
      Width = 100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnUpdateCredentialsMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 7
      Top = 285
      Width = 100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
  end
  object Panel2: TPanel
    Left = 5
    Top = 5
    Width = 692
    Height = 350
    Align = alClient
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 6
      Top = 4
      Width = 680
      Height = 139
      ParentBackground = False
      TabOrder = 0
      object Label1: TLabel
        Left = 33
        Top = 45
        Width = 637
        Height = 28
        AutoSize = False
        Caption = 
          'An attempt to log into salesforce failed. Please update your sal' +
          'esforce credentials'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 148
      Width = 678
      Height = 193
      ParentBackground = False
      TabOrder = 1
      object Label2: TLabel
        Left = 16
        Top = 26
        Width = 64
        Height = 16
        Caption = 'Username'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 16
        Top = 68
        Width = 63
        Height = 16
        Caption = 'Password'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 16
        Top = 113
        Width = 96
        Height = 16
        Caption = 'Security Token'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object txtSFUsername: TEdit
        Left = 151
        Top = 13
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = txtSFUsernameClick
      end
      object txtSFPassword: TEdit
        Left = 151
        Top = 60
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = txtSFPasswordClick
      end
      object txtSFToken: TEdit
        Left = 151
        Top = 101
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = txtSFTokenClick
      end
    end
  end
end
