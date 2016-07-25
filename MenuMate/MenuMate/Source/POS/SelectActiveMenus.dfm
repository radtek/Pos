object frmSelectActiveMenus: TfrmSelectActiveMenus
  Left = 353
  Top = 198
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 4
  Caption = 'Select Active Menus'
  ClientHeight = 279
  ClientWidth = 404
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 404
    Height = 204
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object pnlMenus: TPanel
      Left = 4
      Top = 4
      Width = 396
      Height = 196
      Align = alClient
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 204
    Width = 404
    Height = 75
    Align = alBottom
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object btnMinimise: TTouchBtn
      Left = 5
      Top = 5
      Width = 92
      Height = 65
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMinimiseMouseClick
    end
    object TouchBtn1: TTouchBtn
      Left = 310
      Top = 5
      Width = 89
      Height = 65
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
  end
end
