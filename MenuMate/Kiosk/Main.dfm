object frmMain: TfrmMain
  Left = 0
  Top = 0
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Kiosk'
  ClientHeight = 357
  ClientWidth = 586
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 586
    Height = 357
    Align = alClient
    PopupMenu = PopupMenu1
    TabOrder = 0
    object webDisplay: TWebBrowser
      Left = 1
      Top = 1
      Width = 512
      Height = 434
      PopupMenu = PopupMenu1
      TabOrder = 0
      OnBeforeNavigate2 = webDisplayBeforeNavigate2
      ControlData = {
        4C000000EB340000DB2C00000000000000000000000000000000000000000000
        000000004C000000000000000000000001000000E0D057007335CF11AE690800
        2B2E126208000000000000004C0000000114020000000000C000000000000046
        8000000000000000000000000000000000000000000000000000000000000000
        00000000000000000100000000000000000000000000000000000000}
    end
    object Button1: TButton
      Left = 1
      Top = 1
      Width = 75
      Height = 25
      Caption = 'System'
      TabOrder = 1
      OnClick = Button1Click
    end
  end
  object tiCardSwipe: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tiCardSwipeTimer
    Left = 96
    Top = 8
  end
  object PopupMenu1: TPopupMenu
    Left = 24
    Top = 56
    object Close1: TMenuItem
      Caption = 'Close'
      OnClick = Close1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object ERSClubMembership1: TMenuItem
      Tag = 1
      AutoCheck = True
      Caption = 'ERS Club Membership'
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = ERSClubMembership1Click
    end
    object ExternalMembership1: TMenuItem
      Tag = 1
      AutoCheck = True
      Caption = 'External Membership'
      GroupIndex = 1
      RadioItem = True
      OnClick = ExternalMembership1Click
    end
    object Refresh1: TMenuItem
      Caption = '-'
      GroupIndex = 2
    end
    object PointsMode1: TMenuItem
      AutoCheck = True
      Caption = 'Points Mode'
      Checked = True
      GroupIndex = 2
      RadioItem = True
      OnClick = PointsMode1Click
    end
    object MembershipMode1: TMenuItem
      AutoCheck = True
      Caption = 'Memberhsip Valid Mode'
      GroupIndex = 2
      RadioItem = True
      OnClick = MembershipMode1Click
    end
    object N1: TMenuItem
      Caption = '-'
      GroupIndex = 2
    end
    object ShowEntrances1: TMenuItem
      Caption = 'Show Entrances'
      GroupIndex = 2
      OnClick = ShowEntrances1Click
    end
    object ShowEntrances2: TMenuItem
      Caption = 'Hide Entrances'
      GroupIndex = 2
      OnClick = Refresh2Click
    end
    object N2: TMenuItem
      Caption = '-'
      GroupIndex = 2
    end
    object Refresh2: TMenuItem
      Caption = 'Refresh'
      GroupIndex = 2
      OnClick = Refresh2Click
    end
  end
end
