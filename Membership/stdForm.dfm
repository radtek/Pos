object SMStdForm: TSMStdForm
  Left = 36
  Top = 175
  BorderStyle = bsSingle
  ClientHeight = 218
  ClientWidth = 746
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu
  OldCreateOrder = False
  Scaled = False
  ShowHint = True
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDeactivate = FormDeactivate
  OnKeyUp = FormKeyUp
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object stbStatus: TStatusBar
    Left = 0
    Top = 198
    Width = 746
    Height = 20
    Hint = 
      'Displays hints|Hints are displayed here as you move over parts o' +
      'f the form'
    Color = clSilver
    Panels = <
      item
        Width = 100
      end
      item
        Width = 50
      end>
    ParentFont = True
    SimplePanel = False
    UseSystemFont = False
  end
  object ControlBar1: TControlBar
    Left = 0
    Top = 0
    Width = 746
    Height = 60
    Align = alTop
    BevelEdges = [beTop, beBottom]
    BorderWidth = 2
    DockSite = False
    TabOrder = 1
    object ToolBar: TToolBar
      Left = 11
      Top = 2
      Width = 726
      Height = 48
      Align = alNone
      Anchors = [akLeft, akTop, akBottom]
      BorderWidth = 1
      ButtonHeight = 42
      ButtonWidth = 43
      Caption = 'ToolBar'
      EdgeBorders = []
      EdgeInner = esNone
      EdgeOuter = esNone
      Flat = True
      Images = dm.ImageListLarge
      Indent = 2
      TabOrder = 0
      Transparent = True
      object pnlFill: TPanel
        Left = 2
        Top = 0
        Width = 1
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
      end
    end
  end
  object MainMenu: TMainMenu
    Left = 282
    Top = 26
    object menuFile: TMenuItem
      Caption = '&File'
    end
    object menuWindows: TMenuItem
      Caption = 'Windows'
    end
    object menuHelpTop: TMenuItem
      Caption = 'Help'
      object menuHelp: TMenuItem
        Caption = 'Help                                          F1'
        OnClick = menuHelpClick
      end
      object menuDatabaseConnection: TMenuItem
        Caption = 'Test Database Connection'
        OnClick = menuDatabaseConnectionClick
      end
      object menuSystemDetails: TMenuItem
        Caption = 'System Details'
        OnClick = menuSystemDetailsClick
      end
      object menuCalculator: TMenuItem
        Caption = 'Calculator'
        OnClick = menuCalculatorClick
      end
      object menuAbout: TMenuItem
        Caption = 'About'
        OnClick = menuAboutClick
      end
    end
  end
end
