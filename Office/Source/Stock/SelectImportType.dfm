object frmSelectImportType: TfrmSelectImportType
  Left = 729
  Top = 235
  Width = 346
  Height = 136
  Caption = 'Import'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 321
    Height = 97
    BorderStyle = bsSingle
    Color = clInfoBk
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
    object btnImportStock: TButton
      Left = 8
      Top = 16
      Width = 145
      Height = 57
      Caption = 'Import Stock'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnImportStockClick
    end
    object btnImportSupplier: TButton
      Left = 160
      Top = 16
      Width = 145
      Height = 57
      Caption = 'Import Supplier'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnImportSupplierClick
    end
  end
end
