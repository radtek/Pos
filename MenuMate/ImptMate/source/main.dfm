object frmMain: TfrmMain
  Tag = 1024
  Left = 217
  Top = 278
  BorderStyle = bsDialog
  Caption = 'Importer'
  ClientHeight = 133
  ClientWidth = 488
  Color = 4210816
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TouchButton1: TTouchButton
    Left = 8
    Top = 72
    Width = 153
    Height = 57
    Caption = 'Check Every 10 Seconds'
    Color = clGreen
    UpColour = clGreen
    DownColour = clGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    BorderStyle = bsSingle
    BevelWidth = 2
    ParentCtl3D = False
    OnClick = TouchButton1Click
  end
  object TouchButton2: TTouchButton
    Left = 168
    Top = 72
    Width = 153
    Height = 57
    Caption = 'Change Check Directory'
    Color = clGreen
    UpColour = clGreen
    DownColour = clGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    BorderStyle = bsSingle
    BevelWidth = 2
    ParentCtl3D = False
    OnClick = TouchButton2Click
  end
  object TouchButton3: TTouchButton
    Left = 328
    Top = 72
    Width = 153
    Height = 57
    Caption = 'Check Now'
    Color = clGreen
    UpColour = clGreen
    DownColour = clGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    BorderStyle = bsSingle
    BevelWidth = 2
    ParentCtl3D = False
    OnClick = TouchButton3Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 473
    Height = 49
    Caption = ' Directory to check for drsex.csv files  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 19
      Height = 20
      Caption = 'C:\'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 232
    Top = 16
  end
end
