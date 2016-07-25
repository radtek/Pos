object frmMain: TfrmMain
  Tag = 1024
  Left = 2
  Top = 2
  Align = alClient
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'ChefMate - MenuMate Kitchen Display'
  ClientHeight = 824
  ClientWidth = 1156
  Color = clBlack
  DefaultMonitor = dmDesktop
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -21
  Font.Name = 'Arial'
  Font.Style = [fsBold]
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 24
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 1156
    Height = 72
    Align = alTop
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object Panel2: TPanel
      Left = 208
      Top = 8
      Width = 793
      Height = 57
      Color = clMaroon
      ParentBackground = False
      TabOrder = 0
      object Label1: TLabel
        Left = 344
        Top = 16
        Width = 84
        Height = 24
        Caption = 'Avg Make'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object AvgSales: TLabel
        Left = 240
        Top = 4
        Width = 48
        Height = 49
        Caption = '00'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -43
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 144
        Top = 16
        Width = 84
        Height = 24
        Caption = 'Avg Sales'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
      end
      object AvgMake: TLabel
        Left = 442
        Top = 4
        Width = 72
        Height = 49
        Caption = '000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -43
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 554
        Top = 16
        Width = 118
        Height = 24
        Caption = 'Longest Make'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object LongestMake: TLabel
        Left = 682
        Top = 4
        Width = 72
        Height = 49
        Caption = '000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -43
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 8
        Top = 12
        Width = 122
        Height = 29
        Caption = 'Times Sec:'
        Color = clMaroon
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
      end
    end
    object tbtnConfig: TTouchBtn
      Left = 8
      Top = 8
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Order Count'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnConfigClick
    end
  end
  object tpDisplays: TTouchPages
    Left = 0
    Top = 72
    Width = 1156
    Height = 752
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    Color = 10011108
    ParentFont = False
    ParentColor = False
    ActivePage = tsCurrentOrders
    TabWidth = 300
    TabColor = 10011108
    OnChange = tpDisplaysChange
    object tsCurrentOrders: TTouchSheet
      Left = 2
      Top = 50
      Width = 1152
      Height = 700
      BorderWidth = 5
      Caption = 'Current (Esc or F1)'
      Color = 10011108
      ParentColor = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      object Splitter3: TSplitter
        Left = 911
        Top = 0
        Width = 8
        Height = 690
        Align = alRight
        Color = clGreen
        MinSize = 60
        ParentColor = False
        OnMoved = Splitter3Moved
        ExplicitLeft = 947
        ExplicitTop = -3
      end
      object Splitter1: TSplitter
        Left = 273
        Top = 0
        Width = 8
        Height = 690
        Color = clGreen
        ParentColor = False
        OnMoved = Splitter1Moved
        ExplicitLeft = 183
      end
      object Splitter2: TSplitter
        Left = 601
        Top = 0
        Width = 8
        Height = 690
        Color = clGreen
        ParentColor = False
        OnMoved = Splitter2Moved
        ExplicitLeft = 370
        ExplicitTop = 2
      end
      object Display4: TRichEdit
        Left = 919
        Top = 0
        Width = 223
        Height = 690
        Align = alRight
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnMouseUp = Display1MouseUp
      end
      object Display3: TRichEdit
        Left = 609
        Top = 0
        Width = 302
        Height = 690
        Align = alClient
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnMouseUp = Display1MouseUp
      end
      object Display1: TRichEdit
        Left = 0
        Top = 0
        Width = 273
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnMouseUp = Display1MouseUp
      end
      object Display2: TRichEdit
        Left = 281
        Top = 0
        Width = 320
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnMouseUp = Display1MouseUp
      end
    end
    object tsAgedOrders: TTouchSheet
      Left = 2
      Top = 50
      Width = 1152
      Height = 700
      BorderWidth = 5
      Caption = 'Aged (Ctrl or F3)'
      Color = clRed
      object SplitterAged1: TSplitter
        Left = 305
        Top = 0
        Width = 8
        Height = 690
        OnMoved = SplitterAged1Moved
        ExplicitLeft = 214
      end
      object SplitterAged2: TSplitter
        Left = 588
        Top = 0
        Width = 8
        Height = 690
        OnMoved = SplitterAged2Moved
        ExplicitLeft = 550
        ExplicitTop = 2
      end
      object SplitterAged3: TSplitter
        Left = 869
        Top = 0
        Width = 8
        Height = 690
        OnMoved = SplitterAged3Moved
        ExplicitLeft = 829
        ExplicitTop = 2
      end
      object AgedDisplay4: TRichEdit
        Left = 877
        Top = 0
        Width = 273
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object AgedDisplay3: TRichEdit
        Left = 596
        Top = 0
        Width = 273
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
      end
      object AgedDisplay2: TRichEdit
        Left = 313
        Top = 0
        Width = 275
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
      end
      object AgedDisplay1: TRichEdit
        Left = 0
        Top = 0
        Width = 305
        Height = 690
        Align = alLeft
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
      end
    end
  end
  object RefreshTimer: TTimer
    Enabled = False
    Interval = 60000
    OnTimer = RefreshTimerTimer
    Left = 248
    Top = 104
  end
  object tiCardSwipe: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tiCardSwipeTimer
    Left = 328
    Top = 104
  end
  object Fonts: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Courier New'
    Font.Style = [fsBold]
    Options = []
    Left = 168
    Top = 112
  end
end
