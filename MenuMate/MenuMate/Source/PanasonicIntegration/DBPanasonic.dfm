object frmDBPanasonic: TfrmDBPanasonic
  Left = 0
  Top = 0
  Caption = 'FrmDBPanasonic'
  ClientHeight = 278
  ClientWidth = 643
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object UniDataBaseConnection: TUniConnection
    Left = 48
    Top = 32
  end
  object SQLServerUniProvider: TSQLServerUniProvider
    Left = 176
    Top = 40
  end
  object DataSource: TDataSource
    Left = 360
    Top = 72
  end
end
