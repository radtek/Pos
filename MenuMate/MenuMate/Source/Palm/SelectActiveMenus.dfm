object frmSelectActiveMenus: TfrmSelectActiveMenus
  Tag = 1024
  Left = 230
  Top = 145
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'Select Active Menus'
  ClientHeight = 274
  ClientWidth = 405
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 405
    Height = 199
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 8
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object pnlMenus: TPanel
      Left = 8
      Top = 8
      Width = 389
      Height = 183
      Align = alClient
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 10011108
      TabOrder = 0
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 199
    Width = 405
    Height = 75
    Align = alBottom
    BevelOuter = bvNone
    Color = 10011108
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 160
      Top = 0
      Width = 113
      Height = 65
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
      OnMouseClick = btnOkClick
    end
    object btnCancel: TTouchBtn
      Left = 280
      Top = 0
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelClick
    end
    object btnNone: TTouchBtn
      Left = 8
      Top = 0
      Width = 113
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'None'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNoneClick
    end
  end
end
