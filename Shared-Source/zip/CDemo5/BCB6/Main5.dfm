object MainForm: TMainForm
  Left = 344
  Top = 169
  Width = 630
  Height = 440
  Caption = 'Create Self-Extracting Archive (*.EXE)'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 15
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 622
    Height = 101
    Align = alTop
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 160
      Height = 99
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object DeleteZipBut: TButton
        Left = 12
        Top = 55
        Width = 137
        Height = 26
        Caption = 'Destroy Current Archive'
        TabOrder = 1
        OnClick = DeleteZipButClick
      end
      object NewZipBut: TButton
        Left = 12
        Top = 12
        Width = 137
        Height = 26
        Caption = '&Create New Archive'
        Default = True
        TabOrder = 0
        OnClick = NewZipButClick
      end
    end
    object Panel1: TPanel
      Left = 518
      Top = 1
      Width = 103
      Height = 99
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object Bevel2: TBevel
        Left = 9
        Top = 60
        Width = 85
        Height = 33
      end
      object Label1: TLabel
        Left = 56
        Top = 70
        Width = 23
        Height = 15
        Caption = 'files'
      end
      object FilesLabel: TLabel
        Left = 28
        Top = 70
        Width = 7
        Height = 15
        Caption = '0'
      end
      object CloseBut: TButton
        Left = 9
        Top = 16
        Width = 85
        Height = 27
        Caption = 'E&xit'
        TabOrder = 0
        OnClick = CloseButClick
      end
    end
    object Panel4: TPanel
      Left = 161
      Top = 1
      Width = 357
      Height = 99
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object ZipFName: TLabel
        Left = 40
        Top = 70
        Width = 305
        Height = 15
        AutoSize = False
        Caption = '<none>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
      end
      object Label2: TLabel
        Left = 6
        Top = 70
        Width = 30
        Height = 14
        Caption = 'Name:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Bevel1: TBevel
        Left = 16
        Top = 9
        Width = 329
        Height = 48
      end
      object DeleteBut: TButton
        Left = 210
        Top = 20
        Width = 85
        Height = 24
        Caption = '&Delete Files'
        TabOrder = 1
        OnClick = DeleteButClick
      end
      object AddBut: TButton
        Left = 62
        Top = 21
        Width = 79
        Height = 24
        Caption = '&Add Files'
        TabOrder = 0
        OnClick = AddButClick
      end
    end
  end
  object StringGrid1: TStringGrid
    Left = 0
    Top = 101
    Width = 622
    Height = 305
    Align = alClient
    FixedCols = 0
    RowCount = 2
    TabOrder = 1
    OnDrawCell = StringGrid1DrawCell
    OnSelectCell = StringGrid1SelectCell
    ColWidths = (
      158
      184
      192
      64
      0)
  end
  object OpenDialog1: TOpenDialog
    Left = 448
    Top = 64
  end
  object ZipBuilder1: TZipBuilder
    AddStoreSuffixes = [assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ, assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR]
    Language = 'US: Default'
    SFXCaption = 'Self-extracting Archive'
    SFXLanguage = 'US'
    SFXOptions = [SFXCheckSize]
    Version = '1.79'
    OnDirUpdate = ZipBuilder1DirUpdate
    OnMessage = ZipBuilder1Message
    Left = 384
    Top = 64
  end
end
