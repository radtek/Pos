object frmLoyalty: TfrmLoyalty
  Tag = 1024
  Left = 0
  Top = 8
  Align = alClient
  BorderStyle = bsNone
  BorderWidth = 1
  Caption = 'MenuMate'
  ClientHeight = 744
  ClientWidth = 1012
  Color = clSilver
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -18
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 21
  object GroupBox4: TGroupBox
    Left = 400
    Top = 8
    Width = 489
    Height = 353
    Caption = 'Customer Details'
    TabOrder = 0
    object lbeCustomerName: TLabel
      Left = 16
      Top = 24
      Width = 117
      Height = 19
      Caption = 'Customer Name'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeContactPhone: TLabel
      Left = 16
      Top = 152
      Width = 108
      Height = 19
      Caption = 'Contact Phone'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeCustomerTotal: TLabel
      Left = 16
      Top = 280
      Width = 96
      Height = 19
      Caption = 'Total To Date'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeExpiryDate: TLabel
      Left = 256
      Top = 152
      Width = 85
      Height = 19
      Caption = 'Expiry Date'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbeCardNumber: TLabel
      Left = 16
      Top = 216
      Width = 98
      Height = 19
      Caption = 'Card Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object DBEdit1: TDBEdit
      Left = 16
      Top = 48
      Width = 457
      Height = 27
      DataField = 'NAME'
      DataSource = frmData.dsLoyalCustomer
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
      OnEnter = DBEdit1Enter
    end
    object DBEdit3: TDBEdit
      Left = 16
      Top = 176
      Width = 217
      Height = 27
      DataField = 'PHONE'
      DataSource = frmData.dsLoyalCustomer
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      OnEnter = DBEdit1Enter
    end
    object DBEdit4: TDBEdit
      Left = 256
      Top = 176
      Width = 217
      Height = 27
      DataField = 'EXPIRY_DATE'
      DataSource = frmData.dsLoyalCustomer
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 2
      OnEnter = DBEdit1Enter
    end
    object DBEdit5: TDBEdit
      Left = 16
      Top = 240
      Width = 457
      Height = 27
      DataField = 'CARD_NUMBER'
      DataSource = frmData.dsLoyalCustomer
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 3
      OnEnter = DBEdit1Enter
    end
    object DBEdit6: TDBEdit
      Left = 16
      Top = 304
      Width = 217
      Height = 27
      DataField = 'TOTAL_SPENT'
      DataSource = frmData.dsLoyalCustomer
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 4
      OnEnter = DBEdit1Enter
    end
  end
  object gbCustomerName: TGroupBox
    Left = 8
    Top = 8
    Width = 377
    Height = 729
    Caption = 'Customer Name'
    TabOrder = 1
    object DBGrid1: TDBGrid
      Left = 16
      Top = 24
      Width = 345
      Height = 681
      Ctl3D = False
      DataSource = frmData.dsLoyalCustomer
      Options = [dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -18
      TitleFont.Name = 'Arial'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'NAME'
          Title.Caption = 'Name'
          Width = 340
          Visible = True
        end>
    end
  end
  object GroupBox1: TGroupBox
    Left = 400
    Top = 368
    Width = 489
    Height = 369
    Caption = 'Use Credit'
    TabOrder = 2
    object Label1: TLabel
      Left = 16
      Top = 104
      Width = 169
      Height = 89
      AutoSize = False
      Caption = 'Enter the amount you wish to deduct from this customer'#39's total.'
      WordWrap = True
    end
    object Panel1: TPanel
      Left = 200
      Top = 24
      Width = 273
      Height = 329
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Color = clGray
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 0
      object btn3: TBitBtn
        Left = 184
        Top = 168
        Width = 81
        Height = 73
        Caption = '3'
        TabOrder = 9
        OnClick = btn7Click
      end
      object btn2: TBitBtn
        Left = 96
        Top = 168
        Width = 81
        Height = 73
        Caption = '2'
        TabOrder = 8
        OnClick = btn7Click
      end
      object btn1: TBitBtn
        Left = 8
        Top = 168
        Width = 81
        Height = 73
        Caption = '1'
        TabOrder = 4
        OnClick = btn7Click
      end
      object btn7: TBitBtn
        Left = 8
        Top = 8
        Width = 81
        Height = 73
        Caption = '7'
        TabOrder = 0
        OnClick = btn7Click
      end
      object btn8: TBitBtn
        Left = 96
        Top = 8
        Width = 81
        Height = 73
        Caption = '8'
        TabOrder = 1
        OnClick = btn7Click
      end
      object btn9: TBitBtn
        Left = 184
        Top = 8
        Width = 81
        Height = 73
        Caption = '9'
        TabOrder = 2
        OnClick = btn7Click
      end
      object btn4: TBitBtn
        Left = 8
        Top = 88
        Width = 81
        Height = 73
        Caption = '4'
        TabOrder = 3
        OnClick = btn7Click
      end
      object btn0: TBitBtn
        Left = 8
        Top = 248
        Width = 81
        Height = 73
        Caption = '0'
        TabOrder = 5
        OnClick = btn7Click
      end
      object btn5: TBitBtn
        Left = 96
        Top = 88
        Width = 81
        Height = 73
        Caption = '5'
        TabOrder = 6
        OnClick = btn7Click
      end
      object btn6: TBitBtn
        Left = 184
        Top = 88
        Width = 81
        Height = 73
        Caption = '6'
        TabOrder = 7
        OnClick = btn7Click
      end
      object btnDot: TBitBtn
        Left = 96
        Top = 248
        Width = 81
        Height = 73
        Caption = '00'
        TabOrder = 10
        OnClick = btn7Click
      end
      object btnClear: TBitBtn
        Left = 184
        Top = 248
        Width = 81
        Height = 73
        Caption = 'CLR'
        TabOrder = 11
        OnClick = btnClearClick
      end
    end
    object edDollars: TEdit
      Left = 16
      Top = 40
      Width = 169
      Height = 38
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      Text = '$0.00'
    end
    object Button1: TButton
      Left = 16
      Top = 216
      Width = 169
      Height = 73
      Caption = 'Apply'
      TabOrder = 2
    end
  end
  object Panel2: TPanel
    Left = 904
    Top = 0
    Width = 108
    Height = 744
    Align = alRight
    BevelInner = bvRaised
    BevelOuter = bvNone
    TabOrder = 3
    object Panel15: TPanel
      Left = 16
      Top = 16
      Width = 81
      Height = 81
      BevelInner = bvLowered
      BorderWidth = 2
      Ctl3D = True
      ParentCtl3D = False
      TabOrder = 0
      object imgExit: TImage
        Left = 4
        Top = 4
        Width = 73
        Height = 73
        Hint = 'Exits Maintenance, back to main menu'
        Align = alClient
        AutoSize = True
        Center = True
        ParentShowHint = False
        ShowHint = True
        Stretch = True
        OnClick = imgExitClick
      end
    end
  end
end
