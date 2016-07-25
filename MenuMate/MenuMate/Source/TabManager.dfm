object frmTabManager: TfrmTabManager
  Tag = 1024
  Left = 1
  Top = 1
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Account Manager'
  ClientHeight = 739
  ClientWidth = 1014
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
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  PixelsPerInch = 96
  TextHeight = 13
  object pnlDisplay: TPanel
    Left = 400
    Top = 8
    Width = 473
    Height = 721
    Color = 10011108
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Arial'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object lbeCredit: TLabel
      Left = 264
      Top = 235
      Width = 73
      Height = 32
      Caption = 'Credit'
    end
    object lbeCreditLimit: TLabel
      Left = 264
      Top = 189
      Width = 138
      Height = 32
      Caption = 'Credit Limit'
    end
    object lbeLockedReason: TLabel
      Left = 16
      Top = 360
      Width = 441
      Height = 353
      AutoSize = False
      Caption = 'Locked Reason'
      WordWrap = True
    end
    object lbeIDType: TLabel
      Left = 264
      Top = 51
      Width = 28
      Height = 32
      Caption = 'ID'
    end
    object lbeIDNumber: TLabel
      Left = 264
      Top = 97
      Width = 131
      Height = 32
      Caption = 'ID Number'
    end
    object lbeIDExpire: TLabel
      Left = 264
      Top = 143
      Width = 74
      Height = 32
      Caption = 'Expiry'
    end
    object Label3: TLabel
      Left = 16
      Top = 328
      Width = 203
      Height = 32
      Caption = 'Locked Reason'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 16
      Top = 189
      Width = 156
      Height = 32
      Caption = 'Credit Limit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbeBalance: TLabel
      Left = 16
      Top = 235
      Width = 197
      Height = 32
      Caption = 'Balance Owing'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 16
      Top = 143
      Width = 84
      Height = 32
      Caption = 'Expiry'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label7: TLabel
      Left = 16
      Top = 97
      Width = 142
      Height = 32
      Caption = 'ID Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 16
      Top = 51
      Width = 28
      Height = 32
      Caption = 'ID'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 16
      Top = 281
      Width = 157
      Height = 32
      Caption = 'Total Points'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbeTotalPoints: TLabel
      Left = 264
      Top = 281
      Width = 140
      Height = 32
      Caption = 'Total Points'
    end
    object lbeTabName: TStaticText
      Left = 8
      Top = 8
      Width = 457
      Height = 33
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Tab Name'
      Color = clMaroon
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
  end
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 385
    Height = 721
    Color = 10011108
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 1
    object btnSeatsUp: TTouchBtn
      Left = 312
      Top = 192
      Width = 65
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clWhite
      ParentFont = False
      Caption = '/\'
      ButtonColor = clBtnFace
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnSeatsUpMouseDown
    end
    object btnSeatsDown: TTouchBtn
      Left = 312
      Top = 648
      Width = 65
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '\/'
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnSeatsDownMouseDown
    end
    object btnShowNormalTabs: TTouchBtn
      Left = 8
      Top = 40
      Width = 113
      Height = 105
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Tabs'
      ButtonColor = clInfoBk
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnShowNormalTabsClick
    end
    object btnMemberTabs: TTouchBtn
      Left = 128
      Top = 40
      Width = 129
      Height = 105
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Member Tabs'
      ButtonColor = clInfoBk
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMemberTabsClick
    end
    object lbePartyName: TStaticText
      Left = 8
      Top = 152
      Width = 369
      Height = 27
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Party Name: Johnson'
      Color = clMaroon
      DoubleBuffered = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
    end
    object Label1: TStaticText
      Left = 8
      Top = 8
      Width = 369
      Height = 27
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Select One'
      Color = clMaroon
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
    object btnStaffTabs: TTouchBtn
      Left = 264
      Top = 40
      Width = 113
      Height = 105
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Staff Tabs'
      ButtonColor = clInfoBk
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnStaffTabsClick
    end
    object pnlTabs: TPanel
      Left = 8
      Top = 192
      Width = 297
      Height = 529
      BevelOuter = bvNone
      BorderStyle = bsSingle
      ParentColor = True
      TabOrder = 7
      OnResize = pnlTabsResize
      object sbTabs: TScrollBox
        Left = 0
        Top = 0
        Width = 289
        Height = 513
        HorzScrollBar.Visible = False
        VertScrollBar.ButtonSize = 1
        VertScrollBar.Color = 10011108
        VertScrollBar.Increment = 45
        VertScrollBar.ParentColor = False
        VertScrollBar.Size = 1
        VertScrollBar.Style = ssFlat
        VertScrollBar.ThumbSize = 1
        VertScrollBar.Tracking = True
        BorderStyle = bsNone
        TabOrder = 0
      end
    end
  end
  object Panel3: TPanel
    Left = 880
    Top = 8
    Width = 129
    Height = 721
    Color = 10011108
    ParentBackground = False
    TabOrder = 2
    object btnClose: TTouchBtn
      Left = 8
      Top = 8
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Exit'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object btnLockTab: TTouchBtn
      Left = 8
      Top = 435
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Lock / Unlock Tab'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnLockTabClick
    end
    object btnManInvoice: TTouchBtn
      Left = 8
      Top = 649
      Width = 113
      Height = 65
      BorderWidth = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Manual Invoice'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnManInvoiceClick
    end
    object btnTabCredit: TTouchBtn
      Left = 8
      Top = 293
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Tab Credit'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTabCreditClick
    end
    object btnAddNewTab: TTouchBtn
      Left = 8
      Top = 80
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Create'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAddNewTabClick
    end
    object btnPermanent: TTouchBtn
      Left = 8
      Top = 506
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Make Permanent'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPermanentClick
    end
    object btnRemoveTab: TTouchBtn
      Left = 8
      Top = 576
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Remove Empty Tab'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnRemoveTabClick
    end
    object btnChangeDetails: TTouchBtn
      Left = 8
      Top = 151
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Change Details'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnChangeDetailsClick
    end
    object btnPINTab: TTouchBtn
      Left = 8
      Top = 364
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Set PIN'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPINTabClick
    end
    object TouchButton1: TTouchBtn
      Left = 8
      Top = 222
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Swipe Card'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchButton1Click
    end
  end
end
