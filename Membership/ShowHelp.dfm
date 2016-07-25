inherited frmShowHelp: TfrmShowHelp
  Tag = 99
  Left = 101
  Top = 105
  Width = 631
  Height = 411
  BorderStyle = bsSizeable
  Caption = 'frmShowHelp'
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 345
    Width = 623
  end
  object RichEdit: TRichEdit98 [1]
    Left = 0
    Top = 61
    Width = 623
    Height = 284
    Align = alClient
    AutoURLDetect = adNone
    Color = clWhite
    CustomURLs = <
      item
        Name = 'http'
        Color = clWindowText
        Cursor = crDefault
        Underline = True
      end>
    LangOptions = [loAutoFont]
    Language = 1033
    ScrollBars = ssVertical
    ShowSelectionBar = True
    TabOrder = 2
    URLColor = clBlue
    URLCursor = crHandPoint
    InputFormat = ifRTF
    OutputFormat = ofRTF
    SelectedInOut = False
    PlainRTF = False
    UndoLimit = 0
    IncludeOLE = True
    AllowInPlace = False
  end
  inherited ControlBar1: TControlBar
    Width = 623
    Height = 61
    inherited ToolBar: TToolBar
      Width = 606
      Align = alClient
      ButtonWidth = 41
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object btnPrint: TToolButton
        Left = 3
        Top = 0
        Hint = 'Print the Help'
        Caption = 'btnPrint'
        ImageIndex = 12
        OnClick = menuPrintClick
      end
      object pnlSpace: TPanel
        Left = 44
        Top = 0
        Width = 516
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
        object lblInfinityURL: TLabel
          Left = 192
          Top = 14
          Width = 100
          Height = 16
          Caption = 'www.infinity.co.nz'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsUnderline]
          ParentFont = False
          OnClick = lblInfinityURLClick
        end
      end
      object ToolButton1: TToolButton
        Left = 560
        Top = 0
        Caption = 'ToolButton1'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 466
    Top = 18
    inherited menuFile: TMenuItem
      object menuPrint: TMenuItem
        Caption = 'Print'
        ShortCut = 16464
        OnClick = menuPrintClick
      end
      object menuPrinterSetup: TMenuItem
        Caption = '&Printer Setup'
        OnClick = menuPrinterSetupClick
      end
      object N1: TMenuItem
        Caption = '---------------------'
      end
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
  end
end
