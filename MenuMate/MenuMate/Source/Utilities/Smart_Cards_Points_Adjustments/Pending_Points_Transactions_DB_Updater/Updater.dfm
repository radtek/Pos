object frmUpdater: TfrmUpdater
  Left = 0
  Top = 0
  Caption = 'Insert pending points transactions to DB'
  ClientHeight = 178
  ClientWidth = 403
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 35
    Width = 353
    Height = 13
    Caption = 
      'Select the encrypted file to insert points transactions to curre' +
      'nt database'
  end
  object lblBinFilePath: TLabel
    Left = 89
    Top = 54
    Width = 3
    Height = 13
    WordWrap = True
  end
  object lblStatus: TLabel
    Left = 8
    Top = 147
    Width = 179
    Height = 13
    Caption = 'Total Transactions : 0 , Processed : 0'
  end
  object lblDBPath: TLabel
    Left = 89
    Top = 85
    Width = 3
    Height = 13
  end
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 75
    Height = 13
    Caption = 'Server Name'
  end
  object btnBrowse: TButton
    Left = 8
    Top = 54
    Width = 75
    Height = 25
    Caption = 'Browse...'
    TabOrder = 0
    OnClick = btnBrowseClick
  end
  object btnUpdateDB: TButton
    Left = 8
    Top = 116
    Width = 75
    Height = 25
    Caption = 'Update DB'
    TabOrder = 1
    OnClick = btnUpdateDBClick
  end
  object btnSetDB: TButton
    Left = 8
    Top = 85
    Width = 75
    Height = 25
    Caption = 'Set DB'
    TabOrder = 2
    OnClick = btnSetDBClick
  end
  object txtServerName: TEdit
    Left = 89
    Top = 8
    Width = 128
    Height = 21
    TabOrder = 3
    Text = 'localhost'
  end
  object OpenBinFileDialog: TOpenTextFileDialog
    Filter = 'binary files (*.bin)|*.BIN'
    Title = 'Select encrypted transactions file'
    Left = 288
    Top = 119
  end
  object OpenDBDialog: TOpenTextFileDialog
    Filter = 'firebird database files (*.fdb)|*.FDB'
    Title = 'Select database to inject'
    Left = 344
    Top = 120
  end
end
