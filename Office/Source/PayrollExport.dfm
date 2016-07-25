object frmPayrollExport: TfrmPayrollExport
  Left = 218
  Top = 392
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Payroll Export'
  ClientHeight = 141
  ClientWidth = 414
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object btnExport: TBitBtn
    Left = 208
    Top = 96
    Width = 89
    Height = 33
    Caption = 'Export'
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 304
    Top = 96
    Width = 89
    Height = 33
    Caption = 'Close'
    TabOrder = 1
    OnClick = BitBtn1Click
  end
end
