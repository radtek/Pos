object frmCardSwipe: TfrmCardSwipe
  Left = 424
  Top = 252
  BorderIcons = []
  BorderStyle = bsSingle
  BorderWidth = 5
  Caption = 'MenuMate'
  ClientHeight = 168
  ClientWidth = 358
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Panel16: TPanel
    Left = 0
    Top = 0
    Width = 217
    Height = 168
    Align = alLeft
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 1
      Top = 1
      Width = 215
      Height = 16
      Align = alTop
      Alignment = taCenter
      Caption = 'Waiting for card swipe...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ExplicitWidth = 166
    end
    object Animate1: TAnimate
      Left = 62
      Top = 56
      Width = 80
      Height = 50
      CommonAVI = aviFindFolder
      StopFrame = 27
    end
  end
  object Panel1: TPanel
    Left = 224
    Top = 0
    Width = 134
    Height = 168
    Align = alRight
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object tbtnCancel: TTouchBtn
      Left = 7
      Top = 88
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Color = 14342874
      ParentFont = False
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BitBtn1Click
    end
    object tbOtherOpt: TTouchBtn
      Left = 7
      Top = 9
      Width = 121
      Height = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Color = 14342874
      ParentFont = False
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbOtherOptClick
    end
  end
  object tiSwipe: TTimer
    Enabled = False
    Interval = 500
    Left = 160
    Top = 64
  end
end
