object frmReportDisplay: TfrmReportDisplay
  Tag = 1024
  Left = 2
  Top = 2
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 8
  Caption = 'Report Display'
  ClientHeight = 396
  ClientWidth = 582
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
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 431
    Height = 396
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 10011108
    TabOrder = 0
    object webDisplay: TWebBrowser
      Left = 1
      Top = 1
      Width = 429
      Height = 394
      Align = alClient
      TabOrder = 0
      ExplicitWidth = 445
      ExplicitHeight = 410
      ControlData = {
        4C000000572C0000B92800000000000000000000000000000000000000000000
        000000004C000000000000000000000001000000E0D057007335CF11AE690800
        2B2E126208000000000000004C0000000114020000000000C000000000000046
        8000000000000000000000000000000000000000000000000000000000000000
        00000000000000000100000000000000000000000000000000000000}
    end
  end
  object Panel1: TPanel
    Left = 437
    Top = 0
    Width = 145
    Height = 396
    Align = alRight
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 10011108
    TabOrder = 1
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 10
      Width = 129
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object tbtnPageUp: TTouchBtn
      Left = 8
      Top = 96
      Width = 89
      Height = 81
      Color = 11587808
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseClick = tbtnPageUpMouseClick
      OnAutoRepeat = tbtnPageUpMouseClick
    end
    object tbtnPageDown: TTouchBtn
      Left = 8
      Top = 640
      Width = 89
      Height = 81
      Color = 11587808
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseClick = tbtnPageDownMouseClick
      OnAutoRepeat = tbtnPageDownMouseClick
    end
    object tbtnLineUp: TTouchBtn
      Left = 8
      Top = 184
      Width = 89
      Height = 81
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = 11587808
      ParentFont = False
      Caption = '/\'
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnLineUpMouseClick
      OnAutoRepeat = tbtnLineUpMouseClick
    end
    object tbtnLineDown: TTouchBtn
      Left = 8
      Top = 544
      Width = 89
      Height = 81
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = 11587808
      ParentFont = False
      Caption = '\/'
      ButtonColor = 11587808
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnLineDownMouseClick
      OnAutoRepeat = tbtnLineDownMouseClick
    end
  end
end
