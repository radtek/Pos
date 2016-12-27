object frmDBPanasonic: TfrmDBPanasonic
  Left = 0
  Top = 0
  Caption = 'Form2'
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
  object SQLServerUniProvider1: TSQLServerUniProvider
    Left = 176
    Top = 40
  end
  object DataSource1: TDataSource
    DataSet = UniInsertQuery
    Left = 360
    Top = 72
  end
  object UniInsertQuery: TUniQuery
    Connection = UniDataBaseConnection
    SQL.Strings = (
      '')
    Left = 472
    Top = 64
  end
end
