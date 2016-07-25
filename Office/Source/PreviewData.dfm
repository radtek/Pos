object frmPreviewData: TfrmPreviewData
  Left = 253
  Top = 183
  Width = 696
  Height = 480
  Caption = 'Preview Data'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object dgPreviewData: TDBGrid
    Left = 0
    Top = 0
    Width = 688
    Height = 453
    Align = alClient
    DataSource = dsPreviewData
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object dsPreviewData: TDataSource
    Left = 648
    Top = 16
  end
end
