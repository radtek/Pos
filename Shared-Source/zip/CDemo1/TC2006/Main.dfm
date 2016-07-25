object MainForm: TMainForm
  Left = 187
  Top = 181
  ActiveControl = ZipOpenBut
  Caption = 'BCB Zip Demo1'
  ClientHeight = 372
  ClientWidth = 794
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
    Width = 794
    Height = 125
    Align = alTop
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 124
      Height = 123
      Align = alLeft
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object DeleteZipBut: TButton
        Left = 15
        Top = 83
        Width = 96
        Height = 26
        Caption = 'Destroy Zipfile'
        TabOrder = 2
        OnClick = DeleteZipButClick
      end
      object NewZipBut: TButton
        Left = 15
        Top = 47
        Width = 96
        Height = 26
        Caption = '&New Zipfile'
        TabOrder = 1
        OnClick = NewZipButClick
      end
      object ZipOpenBut: TButton
        Left = 15
        Top = 11
        Width = 96
        Height = 25
        Caption = '&Open Zip'
        TabOrder = 0
        OnClick = ZipOpenButClick
      end
    end
    object Panel1: TPanel
      Left = 693
      Top = 1
      Width = 100
      Height = 123
      Align = alRight
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Bevel2: TBevel
        Left = 9
        Top = 96
        Width = 85
        Height = 21
      end
      object Label1: TLabel
        Left = 64
        Top = 97
        Width = 23
        Height = 15
        Caption = 'files'
      end
      object FilesLabel: TLabel
        Left = 27
        Top = 97
        Width = 7
        Height = 15
        Caption = '0'
      end
      object CloseBut: TButton
        Left = 9
        Top = 4
        Width = 85
        Height = 27
        Caption = 'E&xit'
        TabOrder = 0
        OnClick = CloseButClick
      end
      object VerboseCB: TCheckBox
        Left = 8
        Top = 36
        Width = 77
        Height = 17
        Caption = 'Verbose'
        TabOrder = 1
        OnClick = VerboseCBClick
      end
      object TraceCB: TCheckBox
        Left = 8
        Top = 54
        Width = 73
        Height = 17
        Caption = 'Trace'
        TabOrder = 2
        OnClick = TraceCBClick
      end
      object UnattendedCB: TCheckBox
        Left = 8
        Top = 76
        Width = 85
        Height = 17
        Caption = 'Unattended'
        TabOrder = 3
        OnClick = UnattendedCBClick
      end
    end
    object Panel4: TPanel
      Left = 125
      Top = 1
      Width = 568
      Height = 123
      Align = alClient
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object ZipFName: TLabel
        Left = 43
        Top = 102
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
        Top = 102
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
        Left = 6
        Top = 4
        Width = 369
        Height = 92
      end
      object TimeLabel: TLabel
        Left = 278
        Top = 69
        Width = 31
        Height = 15
        Caption = '00:00'
      end
      object Label4: TLabel
        Left = 278
        Top = 49
        Width = 31
        Height = 15
        Caption = 'Time:'
      end
      object DeleteBut: TButton
        Left = 20
        Top = 67
        Width = 101
        Height = 24
        Caption = '&Delete From Zip'
        TabOrder = 4
        OnClick = DeleteButClick
      end
      object AddBut: TButton
        Left = 20
        Top = 11
        Width = 101
        Height = 24
        Caption = '&Add  to Zip'
        TabOrder = 0
        OnClick = AddButClick
      end
      object ExtractBut: TButton
        Left = 20
        Top = 39
        Width = 101
        Height = 24
        Caption = '&Extract from Zip'
        TabOrder = 1
        OnClick = ExtractButClick
      end
      object TestBut: TButton
        Left = 138
        Top = 67
        Width = 91
        Height = 24
        Caption = '&Test Zip'
        TabOrder = 3
        OnClick = TestButClick
      end
      object MsgBut: TButton
        Left = 244
        Top = 11
        Width = 89
        Height = 24
        Caption = '&Show Msgs'
        TabOrder = 5
        OnClick = MsgButClick
      end
      object ConvertBut: TButton
        Left = 138
        Top = 11
        Width = 91
        Height = 24
        Caption = '&Convert to EXE'
        TabOrder = 2
        OnClick = ConvertButClick
      end
      object RenBut: TButton
        Left = 138
        Top = 39
        Width = 91
        Height = 24
        Caption = 'Rename'
        TabOrder = 6
        OnClick = RenButClick
      end
    end
  end
  object StringGrid1: TStringGrid
    Left = 0
    Top = 125
    Width = 794
    Height = 247
    Align = alClient
    ColCount = 7
    DefaultRowHeight = 22
    DefaultDrawing = False
    FixedCols = 0
    TabOrder = 1
    OnDrawCell = StringGrid1DrawCell
    OnSelectCell = StringGrid1SelectCell
    ColWidths = (
      179
      91
      105
      105
      52
      253
      0)
  end
  object OpenDialog1: TOpenDialog
    Left = 536
    Top = 323
  end
  object MainMenu1: TMainMenu
    Left = 476
    Top = 323
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
      object Zipcomment1: TMenuItem
        Caption = 'Zip comment'
        OnClick = Zipcomment1Click
      end
      object DLLversioninfo1: TMenuItem
        Caption = 'DLL version info'
        OnClick = DLLversioninfo1Click
      end
    end
  end
end
