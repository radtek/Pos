object frmFTP: TfrmFTP
  Tag = 1024
  Left = 266
  Top = 205
  BorderStyle = bsNone
  Caption = 'frmFTP'
  ClientHeight = 219
  ClientWidth = 397
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbeData: TLabel
    Left = 160
    Top = 160
    Width = 225
    Height = 24
    AutoSize = False
    Caption = 'Data :'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Memo: TMemo
    Left = 8
    Top = 8
    Width = 377
    Height = 145
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'FTP Status :')
    ParentFont = False
    TabOrder = 0
  end
  object TouchBtn1: TTouchBtn
    Left = 8
    Top = 160
    Width = 145
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ParentColor = True
    IndicatorDelay = 300
    Caption = 'Quit'
    ButtonColor = 4227327
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = lbeOpt1Click
  end
end
