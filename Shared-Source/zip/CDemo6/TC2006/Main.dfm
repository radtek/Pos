object MainForm: TMainForm
  Left = 291
  Top = 229
  Caption = 'BCB disk span demo'
  ClientHeight = 372
  ClientWidth = 617
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 15
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 617
    Height = 109
    Align = alTop
    TabOrder = 0
    object Panel3: TPanel
      Left = 392
      Top = 1
      Width = 224
      Height = 107
      Align = alClient
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Label4: TLabel
        Left = 4
        Top = 8
        Width = 119
        Height = 15
        Caption = 'Keep free on first disk'
      end
      object FilesLabel: TLabel
        Left = 11
        Top = 85
        Width = 7
        Height = 15
        Caption = '0'
      end
      object Label1: TLabel
        Left = 48
        Top = 85
        Width = 23
        Height = 15
        Caption = 'files'
      end
      object Label5: TLabel
        Left = 104
        Top = 36
        Width = 114
        Height = 15
        Caption = 'Min. free volume size'
      end
      object Label6: TLabel
        Left = 4
        Top = 36
        Width = 92
        Height = 15
        Caption = 'Max. volume size'
      end
      object MaxVolSizeEdit: TEdit
        Left = 4
        Top = 52
        Width = 69
        Height = 23
        Hint = 'Maximum size of any archive part in bytes'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '0'
        OnChange = MaxVolSizeEditChange
      end
      object FreeDisk1Edit: TEdit
        Left = 132
        Top = 4
        Width = 85
        Height = 23
        Hint = 'Bytes on the first disk which will not be used for the zipfile'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '0'
        OnChange = FreeDisk1EditChange
      end
      object FormatCB: TCheckBox
        Left = 104
        Top = 84
        Width = 117
        Height = 13
        Hint = 
          'Format each disk before writing the zipfile'#13#10'You will get the ch' +
          'oice: Fast, Full format or not at all'#13
        Caption = '&Format each disk'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 3
      end
      object MinFreeVolEdit: TEdit
        Left = 104
        Top = 52
        Width = 69
        Height = 23
        Hint = 'Min. number of bytes available before disk will be used'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = '65536'
        OnChange = MinFreeVolEditChange
      end
    end
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 391
      Height = 107
      Align = alLeft
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object ZipFName: TLabel
        Left = 43
        Top = 86
        Width = 334
        Height = 17
        AutoSize = False
        Caption = '<none>'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        WordWrap = True
      end
      object Label2: TLabel
        Left = 8
        Top = 86
        Width = 32
        Height = 14
        Caption = 'Zipfile:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Bevel1: TBevel
        Left = 7
        Top = 5
        Width = 378
        Height = 76
      end
      object AddBut: TButton
        Left = 133
        Top = 12
        Width = 137
        Height = 24
        Hint = 'Open and Add to an existing zipfile and then Span'
        Caption = 'Open, &Add and Span'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = AddButClick
      end
      object ExtractBut: TButton
        Left = 12
        Top = 48
        Width = 145
        Height = 24
        Hint = 'Open a spanned zipfile and extract all files'
        Caption = '&Extract from spanned Zip'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = ExtractButClick
      end
      object WriteBttn: TButton
        Left = 270
        Top = 12
        Width = 109
        Height = 24
        Hint = 'Open an existing zipfile and Span'
        Caption = '&Open and Span'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = WriteBttnClick
      end
      object ReadBttn: TButton
        Left = 157
        Top = 48
        Width = 223
        Height = 24
        Hint = 'Transform a spanned archive to a default one'
        Caption = '&Make Standard Zip from  a spanned one'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        OnClick = ReadBttnClick
      end
      object NewZipBut: TButton
        Left = 11
        Top = 12
        Width = 122
        Height = 24
        Hint = 'Create a new zipfile, Add and Span'
        Caption = '&New, Add and Span'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = NewZipButClick
      end
    end
  end
  object StringGrid1: TStringGrid
    Left = 0
    Top = 109
    Width = 617
    Height = 263
    Align = alClient
    ColCount = 7
    FixedCols = 0
    TabOrder = 1
    OnDrawCell = StringGrid1DrawCell
    OnSelectCell = StringGrid1SelectCell
    ColWidths = (
      123
      89
      100
      101
      52
      143
      0)
  end
  object OpenDialog1: TOpenDialog
    Left = 576
    Top = 342
  end
  object MainMenu1: TMainMenu
    Left = 492
    Top = 342
    object File1: TMenuItem
      Caption = '&File'
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Project1: TMenuItem
      Caption = '&View'
      object Showlasterror1: TMenuItem
        Caption = 'Last error'
        OnClick = Showlasterror1Click
      end
      object DLLversioninfo1: TMenuItem
        Caption = 'DLL version info'
        OnClick = DLLversioninfo1Click
      end
      object Messages1: TMenuItem
        Caption = 'Messages'
        OnClick = Messages1Click
      end
    end
  end
end
