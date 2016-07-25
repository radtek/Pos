object frmPause: TfrmPause
  Left = 384
  Top = 333
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Waiting...'
  ClientHeight = 113
  ClientWidth = 394
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  Scaled = False
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Progress: TProgressBar
    Left = 8
    Top = 8
    Width = 377
    Height = 33
    Min = 0
    Max = 100
    TabOrder = 0
  end
  object btnExit: TBitBtn
    Left = 192
    Top = 48
    Width = 195
    Height = 57
    Caption = '&Exit'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btnExitClick
  end
end
