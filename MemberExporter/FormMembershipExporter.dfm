object frmMemberExporter: TfrmMemberExporter
  Left = 0
  Top = 0
  Caption = 'Menumate Membership Exporter'
  ClientHeight = 95
  ClientWidth = 413
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = initialize
  PixelsPerInch = 96
  TextHeight = 13
  object lblPathToMenumateDatabase: TLabel
    Left = 8
    Top = 11
    Width = 127
    Height = 13
    Caption = '&Menumate database path:'
    FocusControl = edMenumateDatabasePath
  end
  object pbExportProgress: TProgressBar
    AlignWithMargins = True
    Left = 3
    Top = 36
    Width = 407
    Height = 25
    Align = alBottom
    Smooth = True
    BarColor = clHighlight
    Step = 1
    TabOrder = 0
  end
  object btnExport: TButton
    AlignWithMargins = True
    Left = 3
    Top = 67
    Width = 407
    Height = 25
    Align = alBottom
    Caption = '&Export'
    TabOrder = 1
    OnClick = export_membership_data
  end
  object edMenumateDatabasePath: TEdit
    AlignWithMargins = True
    Left = 141
    Top = 8
    Width = 269
    Height = 21
    TabOrder = 2
  end
end
