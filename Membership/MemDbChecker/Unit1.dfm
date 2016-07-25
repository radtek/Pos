object frmMain: TfrmMain
  Left = 33
  Top = 24
  Width = 918
  Height = 704
  Caption = 'CheckMate'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object memLogs: TMemo
    Left = 0
    Top = 0
    Width = 760
    Height = 677
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object pnl: TPanel
    Left = 760
    Top = 0
    Width = 150
    Height = 677
    Align = alRight
    TabOrder = 1
    object tbtnCheck: TTouchBtn
      Left = 8
      Top = 8
      Width = 137
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Check'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCheckClick
    end
    object tbtnExtract: TTouchBtn
      Left = 8
      Top = 528
      Width = 137
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Extract to Template'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnExtractClick
    end
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 608
      Width = 137
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Quit'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object tbtnClear: TTouchBtn
      Left = 8
      Top = 192
      Width = 137
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Clear Log Window'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnClearMouseClick
    end
    object cbCompareUniqueIndex: TCheckBox
      Left = 8
      Top = 80
      Width = 137
      Height = 25
      Caption = 'Compare Unique Index'
      TabOrder = 4
    end
  end
  object dlgOpenGDB: TOpenDialog
    DefaultExt = '*.gdb'
    InitialDir = 'C:\Program Files\MenuMate'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select a LOCAL gdb File ( Not over a Network or Drive Mapping ).'
    Left = 96
    Top = 8
  end
  object dlgOpenXML: TOpenDialog
    DefaultExt = '*.gdb.xml'
    InitialDir = 'C:\Program Files\MenuMate'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select XML Template for this GDB file'
    Left = 152
    Top = 8
  end
end
