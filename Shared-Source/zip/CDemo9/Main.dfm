object Form1: TForm1
  Left = 360
  Top = 139
  Width = 535
  Height = 426
  Caption = 'Add in Thread'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 527
    Height = 351
    TabStop = False
    Align = alClient
    Color = clInfoBk
    Lines.Strings = (
      'Memo1')
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    WantReturns = False
    WordWrap = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 351
    Width = 527
    Height = 41
    Align = alBottom
    TabOrder = 1
    object Button1: TButton
      Left = 80
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Add'
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 320
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Cancel'
      TabOrder = 1
      OnClick = Button2Click
    end
  end
end
