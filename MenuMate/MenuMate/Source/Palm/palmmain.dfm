object frmMain: TfrmMain
  Tag = 1024
  Left = 183
  Top = 162
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'PalmMate'
  ClientHeight = 345
  ClientWidth = 378
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    0000000080000080000000808000800000008000800080800000C0C0C0008080
    80000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0BBBBBB000BBBB000BBBBBB00000000000BBBBB0000BBB0000BBBBBB00000000
    000BBBB0000BBBB000BBBB00B0000000000BBBBB0000BBB0000BBB0000000000
    0000BBBB0000BBB0000BBB00000000000000BBBB0000BBB0000BBB0000000000
    00000BBBB000BBBB000BBBB00000000000000BBBB0000BBB0000BBB00000000B
    000000BBBB000BBBB000BBB00000000BB00000BBBB000BBBBB00BBB000000000
    BBB00BBBBBBBBBBBBBBBBBB00000000000BBBBBB0BBBB000BBBBBB0000000000
    000BBBB000BB00000BBBB0000000000000000000000000000000000000000000
    000000000000000000000000000000BBBBBB000BBBB000BBBBBBBB000000000B
    BBBB0000BBBB000BBBB00BB000000000BBBB0000BBBB000BBBB0000000000000
    0BBBB0000BBB0000BBB00000000000000BBBB0000BBB0000BBB0000000000000
    0BBBBB000BBBB000BBBB00000000000000BBBB000BBBB0000BBB000000000000
    00BBBB0000BBB00000BB000000000000000BBBBB00BBBB0000BB00000000000B
    000BBBBB00BBBBB000BB000000000000BBBBBBBBBBBBBBBBBBBB000000000000
    0BBBB00BBBB00BBBBBB0000000000000000B0000BB0000BBBB00000000000000
    000000000000000000000000000000000000000000000000000000000000F800
    003FF000001FE000000FC0000007800000030000000100000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000080000001C0000003E0000007F000000FF800001F}
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 104
    Width = 185
    Height = 233
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 100
      Width = 161
      Height = 40
      AutoSize = False
      Caption = 'Drinks Menu'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlCenter
      WordWrap = True
    end
    object Label2: TLabel
      Left = 16
      Top = 8
      Width = 113
      Height = 33
      AutoSize = False
      Caption = 'Food Menu'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlCenter
      WordWrap = True
    end
    object btnChangeDrinks: TTouchBtn
      Left = 16
      Top = 136
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Drinks'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnChangeDrinksClick
    end
    object btnChangeFood: TTouchBtn
      Left = 16
      Top = 40
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Food'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnChangeFoodClick
    end
  end
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 362
    Height = 90
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnMinimise: TTouchBtn
      Left = 13
      Top = 7
      Width = 153
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Minimise'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMinimiseClick
    end
    object btnHappyHour: TTouchBtn
      Left = 172
      Top = 7
      Width = 78
      Height = 74
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Happy Hour'
      ButtonColor = clPurple
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnHappyHourClick
    end
    object TouchBtn1: TTouchBtn
      Left = 264
      Top = 7
      Width = 89
      Height = 74
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Quit'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnQuitClick
    end
  end
  object Panel3: TPanel
    Left = 199
    Top = 104
    Width = 169
    Height = 233
    Color = 14342874
    ParentBackground = False
    TabOrder = 2
    object Label3: TLabel
      Left = 8
      Top = 8
      Width = 113
      Height = 33
      AutoSize = False
      Caption = 'Security'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlCenter
      WordWrap = True
    end
    object Label4: TLabel
      Left = 8
      Top = 100
      Width = 153
      Height = 40
      AutoSize = False
      Caption = 'Configuration'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlCenter
      WordWrap = True
    end
    object lbeVersion: TLabel
      Left = 104
      Top = 207
      Width = 58
      Height = 19
      Caption = 'Version'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnSetup: TTouchBtn
      Left = 8
      Top = 136
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Setup'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSetupClick
    end
    object btnRegisterPalms: TTouchBtn
      Left = 8
      Top = 40
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Register Palms'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnRegisterPalmsClick
    end
  end
  object tmLogs: TTimer
    Enabled = False
    OnTimer = tmLogsTimer
    Left = 24
    Top = 24
  end
  object tiMenuUpdateRequired: TTimer
    Enabled = False
    Interval = 30000
    OnTimer = tiMenuUpdateRequiredTimer
    Left = 64
    Top = 24
  end
end
