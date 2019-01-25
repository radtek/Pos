object FrmSelectTable2: TFrmSelectTable2
  Tag = 1024
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Select Table'
  ClientHeight = 575
  ClientWidth = 824
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  PrintScale = poNone
  Scaled = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 681
    Top = 0
    Width = 143
    Height = 575
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 3
    BorderStyle = bsSingle
    TabOrder = 1
    object TouchBtn2: TTouchBtn
      Left = 3
      Top = 503
      Width = 133
      Height = 65
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
    object tgridLocations: TTouchGrid
      Left = 3
      Top = 3
      Width = 133
      Height = 500
      Align = alClient
      BorderWidth = 5
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      BevelOuter = bvNone
      DefaultButtonColor = 12565947
      DefaultButtonLatchedColor = clNavy
      LatchingType = ltRadioButton
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tgridLocationsMouseClick
      GridBorder = False
      RowCount = 1
      ColCount = 1
      ParentGridColor = False
      GridColor = 14342874
      DefaultColWidth = 120
      DefaultRowHeight = 65
      ButtonGapHeight = 2
      AutoButtonWidth = True
      VertScrollButtonPos = vsAboveAndBelow
      VertScrollButtonHeight = 55
      HorizScrollButtonWidth = 70
      ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
      ScrollBtnUp.Font.Color = clWindowText
      ScrollBtnUp.Font.Height = -16
      ScrollBtnUp.Font.Name = 'Arial'
      ScrollBtnUp.Font.Style = [fsBold]
      ScrollBtnUp.ParentFont = False
      ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnUp.LatchedFont.Color = clWindowText
      ScrollBtnUp.LatchedFont.Height = -11
      ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnUp.LatchedFont.Style = []
      ScrollBtnUp.ParentLatchedFontColor = False
      ScrollBtnUp.GlyphAlpha = 255
      ScrollBtnUp.Color = 9933970
      ScrollBtnUp.FontColor = 16777215
      ScrollBtnUp.LatchedColor = -2147483633
      ScrollBtnUp.DisabledColor = -2147483633
      ScrollBtnUp.LatchedFontColor = -2147483640
      ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
      ScrollBtnDown.Font.Color = clWindowText
      ScrollBtnDown.Font.Height = -16
      ScrollBtnDown.Font.Name = 'Arial'
      ScrollBtnDown.Font.Style = [fsBold]
      ScrollBtnDown.ParentFont = False
      ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnDown.LatchedFont.Color = clWindowText
      ScrollBtnDown.LatchedFont.Height = -11
      ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnDown.LatchedFont.Style = []
      ScrollBtnDown.ParentLatchedFontColor = False
      ScrollBtnDown.GlyphAlpha = 255
      ScrollBtnDown.Color = 9933970
      ScrollBtnDown.FontColor = 16777215
      ScrollBtnDown.LatchedColor = -2147483633
      ScrollBtnDown.DisabledColor = -2147483633
      ScrollBtnDown.LatchedFontColor = -2147483640
      ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
      ScrollBtnLeft.Font.Color = clWindowText
      ScrollBtnLeft.Font.Height = -11
      ScrollBtnLeft.Font.Name = 'MS Sans Serif'
      ScrollBtnLeft.Font.Style = []
      ScrollBtnLeft.ParentFont = False
      ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnLeft.LatchedFont.Color = clWindowText
      ScrollBtnLeft.LatchedFont.Height = -11
      ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnLeft.LatchedFont.Style = []
      ScrollBtnLeft.ParentLatchedFontColor = False
      ScrollBtnLeft.Caption = 'Left'
      ScrollBtnLeft.GlyphAlpha = 255
      ScrollBtnLeft.Color = -2147483633
      ScrollBtnLeft.FontColor = 16777215
      ScrollBtnLeft.LatchedColor = -2147483633
      ScrollBtnLeft.DisabledColor = -2147483633
      ScrollBtnLeft.LatchedFontColor = -2147483640
      ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
      ScrollBtnRight.Font.Color = clWindowText
      ScrollBtnRight.Font.Height = -11
      ScrollBtnRight.Font.Name = 'MS Sans Serif'
      ScrollBtnRight.Font.Style = []
      ScrollBtnRight.ParentFont = False
      ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnRight.LatchedFont.Color = clWindowText
      ScrollBtnRight.LatchedFont.Height = -11
      ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnRight.LatchedFont.Style = []
      ScrollBtnRight.ParentLatchedFontColor = False
      ScrollBtnRight.Caption = 'Right'
      ScrollBtnRight.GlyphAlpha = 255
      ScrollBtnRight.Color = -2147483633
      ScrollBtnRight.FontColor = 16777215
      ScrollBtnRight.LatchedColor = -2147483633
      ScrollBtnRight.DisabledColor = -2147483633
      ScrollBtnRight.LatchedFontColor = -2147483640
      ArrowColor = 2200566
      ArrowBorderColor = 2200566
      VertScrollTime = 450
      VertScrollDistance = 400
      Buttons = {
        0400000041000000000000000000000000007300000000000000000000000000
        BBBDBF000F0000800F000080050000004100720069FFFFFF000C000000010000
        00000000000000000000000000FFFFFF00050000004100720069FFFFFF000C00
        000001000000000000000000000000000000FFFFFF0000000000010000000100
        000001000000010000000100000001000000FF00000064000000010000000000
        0000010000000000000000000000}
    end
  end
  object panelTables: TPanel
    Left = 0
    Top = 0
    Width = 681
    Height = 575
    Align = alClient
    ParentBackground = False
    TabOrder = 0
    object imgTables: TImage
      Left = 0
      Top = 41
      Width = 1300
      Height = 1000
      OnClick = imgTablesClick
      OnMouseMove = imgTablesMouseMove
    end
    object PnlLocation: TPanel
      Left = 1
      Top = 1
      Width = 679
      Height = 41
      Align = alTop
      Caption = 'Location.'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentFont = False
      TabOrder = 0
    end
  end
  object tiUpdateFloorPlanReq: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = tiUpdateFloorPlanReqTimer
    Left = 224
    Top = 160
  end
  object tiUpdateFloorPlanRefresh: TTimer
    Enabled = False
    Interval = 600000
    OnTimer = tiUpdateFloorPlanRefreshTimer
    Left = 270
    Top = 200
  end
  object tiTimerEnableReq: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = tiTimerEnableReq
    Left = 300
    Top = 240
  end
end
