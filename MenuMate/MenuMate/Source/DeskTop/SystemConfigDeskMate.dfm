object frmSystemConfig: TfrmSystemConfig
  Tag = 1024
  Left = 302
  Top = 100
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'System Configuration'
  ClientHeight = 338
  ClientWidth = 436
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pButtons: TPanel
    Left = 0
    Top = 71
    Width = 436
    Height = 267
    Align = alBottom
    BevelInner = bvLowered
    BevelWidth = 2
    BorderWidth = 1
    Color = 10011108
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object tbOk: TTouchBtn
      Left = 14
      Top = 185
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbOkClick
    end
    object tbInterbase: TTouchBtn
      Left = 14
      Top = 9
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Interbase Server IP Address'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbInterbaseClick
    end
    object tbDBLocation: TTouchBtn
      Left = 14
      Top = 81
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Database Location'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbDBLocationClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 436
    Height = 70
    Align = alTop
    BevelInner = bvLowered
    BevelWidth = 2
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    object TouchButton2: TTouchBtn
      Left = 264
      Top = 5
      Width = 161
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = 'Exit'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
    end
  end
end
