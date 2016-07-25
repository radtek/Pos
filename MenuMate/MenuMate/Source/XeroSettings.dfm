object frmXeroSettings: TfrmXeroSettings
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Xero Integration Settings'
  ClientHeight = 454
  ClientWidth = 831
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 831
    Height = 5
    Align = alTop
    Shape = bsSpacer
    ExplicitTop = 41
    ExplicitWidth = 654
  end
  object Bevel2: TBevel
    Left = 0
    Top = 449
    Width = 831
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
    Height = 444
    Align = alLeft
    Shape = bsSpacer
    ExplicitTop = 47
    ExplicitHeight = 535
  end
  object Bevel4: TBevel
    Left = 826
    Top = 5
    Width = 5
    Height = 444
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 0
    ExplicitTop = 581
    ExplicitHeight = 654
  end
  object Bevel5: TBevel
    Left = 708
    Top = 5
    Width = 5
    Height = 444
    Align = alRight
    Shape = bsSpacer
    ExplicitLeft = 8
    ExplicitTop = 51
    ExplicitHeight = 535
  end
  object Panel1: TPanel
    Left = 713
    Top = 5
    Width = 113
    Height = 444
    Align = alRight
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnOK: TTouchBtn
      Left = 7
      Top = 6
      Width = 100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOKMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 7
      Top = 370
      Width = 100
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
      OnMouseClick = btnCancelMouseClick
    end
  end
  object Panel2: TPanel
    Left = 5
    Top = 5
    Width = 703
    Height = 444
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 6
      Top = 70
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
          'Connect to Xero Integration Service  in the local machine. The I' +
          'nvoice Folder Name is only needed'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 33
        Top = 108
        Width = 636
        Height = 26
        AutoSize = False
        Caption = 'Connect to Xero Integration Service  in an remote machine. '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object rbXeroLocal: TRadioButton
        Left = 13
        Top = 22
        Width = 203
        Height = 17
        Caption = 'Local Xero Service'
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        TabStop = True
        OnClick = rbXeroLocalClick
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
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = rbXeroRemoteClick
      end
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 228
      Width = 678
      Height = 193
      ParentBackground = False
      TabOrder = 1
      object Label4: TLabel
        Left = 16
        Top = 26
        Width = 67
        Height = 17
        Caption = 'Host Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 16
        Top = 68
        Width = 124
        Height = 17
        Caption = 'Invoice Folder Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 16
        Top = 113
        Width = 66
        Height = 17
        Caption = 'User Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 17
        Top = 153
        Width = 58
        Height = 17
        Caption = 'Password'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object edHostName: TEdit
        Left = 151
        Top = 13
        Width = 518
        Height = 35
        AutoSize = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = btnEditHostNameMouseClick
      end
      object edFolderName: TEdit
        Left = 151
        Top = 60
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btnEditFolderNameMouseClick
      end
      object edUserName: TEdit
        Left = 151
        Top = 101
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = btnEditUserNameMouseClick
      end
      object edPassword: TEdit
        Left = 151
        Top = 145
        Width = 518
        Height = 27
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = btnEditPasswordMouseClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 6
      Top = 8
      Width = 680
      Height = 50
      ParentBackground = False
      TabOrder = 2
      object cbSendAwaitingPayment: TCheckBox
        Left = 13
        Top = 20
        Width = 400
        Height = 18
        Caption = 'Send Account Charges to Awaiting Payment in XERO'
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
  end
end
