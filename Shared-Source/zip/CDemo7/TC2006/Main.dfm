object Form1: TForm1
  Left = 298
  Top = 123
  Caption = 'Form1'
  ClientHeight = 190
  ClientWidth = 231
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 33
    Width = 209
    Height = 121
    Stretch = True
  end
  object StaticText1: TStaticText
    Left = 8
    Top = 8
    Width = 4
    Height = 4
    TabOrder = 0
  end
  object btnPic1: TButton
    Left = 8
    Top = 160
    Width = 65
    Height = 25
    Caption = 'Pic &1'
    TabOrder = 1
    OnClick = btnPic1Click
  end
  object btnPic2: TButton
    Left = 80
    Top = 160
    Width = 65
    Height = 25
    Caption = 'Pic &2'
    TabOrder = 2
    OnClick = btnPic2Click
  end
  object btnPic3: TButton
    Left = 152
    Top = 160
    Width = 65
    Height = 25
    Caption = 'Pic &3'
    TabOrder = 3
    OnClick = btnPic3Click
  end
end
