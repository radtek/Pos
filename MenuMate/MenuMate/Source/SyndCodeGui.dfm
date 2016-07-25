object frmSyndCodeGui: TfrmSyndCodeGui
  Tag = 1024
  Left = 1419
  Top = 307
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 5
  Caption = 'Syndicate Code'
  ClientHeight = 319
  ClientWidth = 463
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMajor: TPanel
    Left = 0
    Top = 0
    Width = 321
    Height = 319
    Align = alLeft
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object pnlLabel: TPanel
      Left = 7
      Top = 7
      Width = 307
      Height = 34
      Align = alTop
      BevelOuter = bvLowered
      Caption = 'Syndicate Code'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
    end
    object tbtnName: TTouchBtn
      Left = 8
      Top = 56
      Width = 305
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Name :'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnNameMouseClick
    end
    object tbtnEnabled: TTouchBtn
      Left = 8
      Top = 184
      Width = 305
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Enabled'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnEnabledMouseClick
    end
    object TouchBtn3: TTouchBtn
      Left = 8
      Top = 120
      Width = 305
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Syndicate Code'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn3MouseClick
    end
    object tbtnEncryption: TTouchBtn
      Left = 8
      Top = 248
      Width = 305
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Use For Encryption Code : '
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnEncryptionMouseClick
    end
  end
  object pnlMinor: TPanel
    Left = 329
    Top = 0
    Width = 134
    Height = 319
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnClose: TTouchBtn
      Left = 5
      Top = 5
      Width = 124
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object TouchBtn4: TTouchBtn
      Left = 5
      Top = 257
      Width = 124
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn4MouseClick
    end
  end
end
