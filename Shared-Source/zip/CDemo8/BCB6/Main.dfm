object MainForm: TMainForm
  Left = 285
  Top = 141
  Width = 653
  Height = 460
  Caption = 'Versioning Demo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 407
    Width = 645
    Height = 19
    Panels = <
      item
        Width = 400
      end
      item
        Width = 125
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 366
    Width = 645
    Height = 41
    Align = alBottom
    TabOrder = 1
    object CrFBtn: TButton
      Left = 7
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Create Sample'
      TabOrder = 0
      OnClick = CrFBtnClick
    end
    object ShContBtn: TButton
      Left = 85
      Top = 8
      Width = 95
      Height = 25
      Caption = 'Show ZipContent'
      TabOrder = 1
      OnClick = ShContBtnClick
    end
    object ChangeBtn: TButton
      Left = 182
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Change Files'
      TabOrder = 2
      OnClick = ChangeBtnClick
    end
    object UpdateBtn: TButton
      Left = 258
      Top = 8
      Width = 63
      Height = 25
      Caption = 'Update Zip'
      TabOrder = 3
      OnClick = UpdateBtnClick
    end
    object DelBtn: TButton
      Left = 323
      Top = 8
      Width = 81
      Height = 25
      Caption = 'Delete Samples'
      TabOrder = 4
      OnClick = DelBtnClick
    end
    object DelAllBtn: TButton
      Left = 408
      Top = 8
      Width = 59
      Height = 25
      Caption = 'Delete All'
      TabOrder = 5
      OnClick = DelAllBtnClick
    end
    object RestLatBtn: TButton
      Left = 473
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Restore Latest'
      TabOrder = 6
      OnClick = RestLatBtnClick
    end
    object RestBtn: TButton
      Left = 551
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Restore Sel Vers'
      TabOrder = 7
      OnClick = RestBtnClick
    end
  end
  object TreeView1: TTreeView
    Left = 0
    Top = 0
    Width = 645
    Height = 366
    Align = alClient
    HideSelection = False
    Indent = 19
    TabOrder = 2
  end
  object ZipBuilder: TZipBuilder
    AddStoreSuffixes = [assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ, assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR]
    Language = 'US: Default'
    SFXCaption = 'Self-extracting Archive'
    SFXLanguage = 'US'
    SFXOptions = [SFXCheckSize]
    Version = '1.79'
    OnExtractOverwrite = ZipBuilderExtractOverwrite
    OnMessage = ZipBuilderMessage
    OnSetNewName = ZipBuilderSetNewName
    Left = 408
    Top = 152
  end
end
