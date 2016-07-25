object frmExcel: TfrmExcel
  Left = 431
  Top = 255
  Width = 362
  Height = 173
  Caption = 'Excel'
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
  object Label1: TLabel
    Left = 67
    Top = 83
    Width = 241
    Height = 39
    AutoSize = False
    Caption = 
      'If you want to save the data in Excel , then select File/Save fr' +
      'om the Excel Menu'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object btnCloseExcel: TBitBtn
    Left = 118
    Top = 38
    Width = 107
    Height = 33
    Caption = 'Close Excel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btnCloseExcelClick
  end
  object OpExcel: TOpExcel
    Version = '1.64'
    Caption = 'Microsoft Excel'
    Visible = False
    WindowState = xlwsNormal
    ServerLeft = -2
    ServerTop = -4
    ServerHeight = 748
    ServerWidth = 1032
    LargeButtons = False
    EnableAnimations = False
    UserName = 'Chris Watkins'
    EnableAutoComplete = False
    EnableCancelKey = xlckInterrupt
    Workbooks = <>
    OnWindowDeactivate = OpExcelWindowDeactivate
    Left = 288
    Top = 8
  end
  object Outlook: TOpOutlook
    Version = '1.64'
    NewSession = False
    ShowLoginDialog = False
    Left = 32
    Top = 16
  end
end
