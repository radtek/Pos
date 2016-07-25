object frmConfig: TfrmConfig
  Tag = 1024
  Left = 241
  Top = 64
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Control Panel'
  ClientHeight = 385
  ClientWidth = 447
  Color = 14342874
  DefaultMonitor = dmDesktop
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 297
    Height = 385
    Align = alLeft
    ParentColor = True
    TabOrder = 0
    object lbeVersion: TLabel
      Left = 229
      Top = 351
      Width = 59
      Height = 19
      Caption = 'Version'
      Color = 10011108
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object TouchBtn1: TTouchBtn
      Left = 16
      Top = 98
      Width = 273
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Configure Font Size'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbConfigureFontSizeClick
    end
    object btnMon: TTouchBtn
      Left = 16
      Top = 8
      Width = 273
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Display On Next Screen'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMonClick
    end
    object tbExit: TTouchBtn
      Left = 16
      Top = 280
      Width = 273
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Exit Program'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbExitClick
    end
    object TouchBtn5: TTouchBtn
      Left = 16
      Top = 189
      Width = 273
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Reconfigure Terminal'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbReconfigureClick
    end
  end
  object Panel2: TPanel
    Left = 304
    Top = 0
    Width = 143
    Height = 385
    Align = alRight
    ParentColor = True
    TabOrder = 1
    object Label1: TLabel
      Left = 229
      Top = 351
      Width = 59
      Height = 19
      Caption = 'Version'
      Color = 10011108
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object TouchBtn2: TTouchBtn
      Left = 8
      Top = 8
      Width = 129
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
  end
end
