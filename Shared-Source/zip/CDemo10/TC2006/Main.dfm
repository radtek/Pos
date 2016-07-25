object MainForm: TMainForm
  Left = 298
  Top = 123
  Caption = 'Store Drive Letter demo'
  ClientHeight = 391
  ClientWidth = 527
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
  object Panel1: TPanel
    Left = 0
    Top = 350
    Width = 527
    Height = 41
    Align = alBottom
    TabOrder = 0
    object AddBtn: TButton
      Left = 64
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Add'
      TabOrder = 0
      OnClick = AddBtnClick
    end
    object ExtBtn: TButton
      Left = 367
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Extract'
      TabOrder = 1
      OnClick = ExtBtnClick
    end
    object ContentBtn: TButton
      Left = 216
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Content'
      TabOrder = 2
      OnClick = ContentBtnClick
    end
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 527
    Height = 350
    Align = alClient
    Color = clInfoBk
    ScrollBars = ssBoth
    TabOrder = 1
    WantReturns = False
    WordWrap = False
  end
end
