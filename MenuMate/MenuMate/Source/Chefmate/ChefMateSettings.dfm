object frmChefmateSettings: TfrmChefmateSettings
  Left = 0
  Top = 0
  Caption = 'Chefmate Settings'
  ClientHeight = 272
  ClientWidth = 810
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 810
    Height = 5
    Align = alTop
    Shape = bsSpacer
    ExplicitLeft = -10
    ExplicitWidth = 820
  end
  object Bevel4: TBevel
    Left = 687
    Top = 5
    Width = 5
    Height = 262
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 805
    ExplicitTop = -4
    ExplicitHeight = 350
  end
  object Bevel3: TBevel
    Left = 0
    Top = 5
    Width = 5
    Height = 262
    Align = alLeft
    Shape = bsSpacer
    ExplicitTop = -4
    ExplicitHeight = 350
  end
  object Bevel2: TBevel
    Left = 0
    Top = 267
    Width = 810
    Height = 5
    Align = alBottom
    Shape = bsSpacer
    ExplicitLeft = -10
    ExplicitTop = 341
    ExplicitWidth = 820
  end
  object Bevel5: TBevel
    Left = 692
    Top = 5
    Width = 5
    Height = 262
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 697
    ExplicitTop = -4
    ExplicitHeight = 350
  end
  object Panel1: TPanel
    Left = 697
    Top = 5
    Width = 113
    Height = 262
    Align = alRight
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    object btnOK: TTouchBtn
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
      OnMouseClick = btnOKMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 6
      Top = 182
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
  object GroupBox1: TGroupBox
    Left = 8
    Top = 92
    Width = 678
    Height = 172
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 1
    object lbIPAddr: TLabel
      Left = 16
      Top = 26
      Width = 146
      Height = 16
      Caption = 'Chefmate'#39's IP address'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbIPPort: TLabel
      Left = 16
      Top = 68
      Width = 122
      Height = 16
      Caption = 'Chefmate'#39's IP port'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbDynamic: TLabel
      Left = 36
      Top = 133
      Width = 637
      Height = 28
      AutoSize = False
      Caption = 'Show orders as the items are added'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edIPAddr: TEdit
      Left = 168
      Top = 13
      Width = 501
      Height = 35
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = edIPAddrClick
    end
    object edIPPort: TEdit
      Left = 168
      Top = 60
      Width = 501
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = edIPPortClick
    end
    object chkbDynamic: TCheckBox
      Left = 16
      Top = 110
      Width = 185
      Height = 17
      Caption = 'Show dynamic orders'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = chkbDynamicClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 6
    Top = 6
    Width = 680
    Height = 80
    Color = clBtnFace
    ParentBackground = False
    ParentColor = False
    TabOrder = 2
    object lbActive: TLabel
      Left = 33
      Top = 45
      Width = 637
      Height = 28
      AutoSize = False
      Caption = 'Send orders to Chefmate'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 33
      Top = 108
      Width = 636
      Height = 26
      AutoSize = False
      Caption = 'Connect to Xero Integration Service  in an remote machine. '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object rbXeroRemote: TRadioButton
      Left = 13
      Top = 85
      Width = 203
      Height = 17
      Caption = 'Remote Xero Service'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object chkbActive: TCheckBox
      Left = 13
      Top = 22
      Width = 137
      Height = 17
      Caption = 'Use Chefmate'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = chkbActiveClick
    end
  end
end
