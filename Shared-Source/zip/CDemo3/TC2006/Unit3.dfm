object Form3: TForm3
  Left = 242
  Top = 173
  ActiveControl = AddBut
  Caption = 'C++ Builder Zip Demo 3'
  ClientHeight = 328
  ClientWidth = 622
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 288
    Width = 622
    Height = 40
    Align = alBottom
    TabOrder = 0
    object Button2: TButton
      Left = 216
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Exit'
      TabOrder = 0
      OnClick = Button2Click
    end
    object VersBut: TButton
      Left = 46
      Top = 8
      Width = 91
      Height = 25
      Caption = 'DLL &Versions'
      TabOrder = 1
      OnClick = VersButClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 41
    Width = 622
    Height = 247
    Align = alClient
    TabOrder = 1
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 620
      Height = 245
      Align = alClient
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -15
      Font.Name = 'Courier New'
      Font.Style = []
      Lines.Strings = (
        '')
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 622
    Height = 41
    Align = alTop
    TabOrder = 2
    object AddBut: TButton
      Left = 98
      Top = 8
      Width = 59
      Height = 25
      Caption = '&Add'
      TabOrder = 0
      OnClick = AddButClick
    end
    object ExtrBut: TButton
      Left = 273
      Top = 8
      Width = 58
      Height = 25
      Caption = 'E&xtract'
      TabOrder = 1
      OnClick = ExtrButClick
    end
    object ListBut: TButton
      Left = 20
      Top = 8
      Width = 59
      Height = 25
      Caption = '&List Zip'
      TabOrder = 2
      OnClick = ListButClick
    end
    object DeleteBut: TButton
      Left = 416
      Top = 7
      Width = 79
      Height = 26
      Caption = '&Delete from zip'
      TabOrder = 3
      OnClick = DeleteButClick
    end
    object TestBut: TButton
      Left = 345
      Top = 7
      Width = 52
      Height = 26
      Caption = '&Test'
      TabOrder = 4
      OnClick = TestButClick
    end
    object AddPwdBut: TButton
      Left = 176
      Top = 7
      Width = 78
      Height = 26
      Caption = 'Add with &Pwd'
      TabOrder = 5
      OnClick = AddPwdButClick
    end
  end
end
