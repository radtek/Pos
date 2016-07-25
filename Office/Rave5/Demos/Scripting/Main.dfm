object Form1: TForm1
  Left = 200
  Top = 107
  Width = 328
  Height = 206
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 120
    Top = 64
    Width = 75
    Height = 25
    Caption = '&Print'
    TabOrder = 0
    OnClick = Button1Click
  end
  object RvProject1: TRvProject
    ProjectFile = 'RaveDemo.rav'
    Left = 24
    Top = 24
  end
end
