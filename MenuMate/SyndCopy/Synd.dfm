object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Synd Copy'
  ClientHeight = 193
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 118
    Width = 89
    Height = 38
    Caption = 'Save'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 320
    Top = 118
    Width = 90
    Height = 38
    Caption = 'Inject'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 8
    Top = 162
    Width = 402
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = 'C:\Synd.bin'
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 85
    Width = 402
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'Select Synd Code'
  end
  object Edit2: TEdit
    Left = 8
    Top = 8
    Width = 402
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = 'localhost:C:\Program Files\MenuMate\MenuMate.ib'
  end
  object Button3: TButton
    Left = 8
    Top = 41
    Width = 89
    Height = 38
    Caption = 'Connect'
    TabOrder = 5
    OnClick = Button3Click
  end
  object btnDisplay: TButton
    Left = 152
    Top = 118
    Width = 89
    Height = 38
    Caption = 'Display'
    TabOrder = 6
    OnClick = btnDisplayClick
  end
end
