object frmMallExportResendReport: TfrmMallExportResendReport
  Left = 351
  Top = 0
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Resend Report'
  ClientHeight = 561
  ClientWidth = 436
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 303
    Top = 0
    Width = 133
    Height = 561
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnOk: TTouchBtn
      Left = 8
      Top = 8
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 8
      Top = 491
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
    object btnResendReport: TTouchBtn
      Left = 8
      Top = 81
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnResendReportMouseClick
    end
    object btnSelectAll: TTouchBtn
      Left = 8
      Top = 155
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Visible = False
      Caption = 'Select All'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSelectAllMouseClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 297
    Height = 561
    Align = alLeft
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tgReportList: TTouchGrid
      Left = 1
      Top = 1
      Width = 295
      Height = 559
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      BevelOuter = bvNone
      DefaultButtonColor = clBtnFace
      DefaultButtonLatchedColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tgReportListMouseClick
      GridBorder = False
      RowCount = 1
      ColCount = 1
      ParentGridColor = False
      GridColor = 14342874
      DefaultColWidth = 120
      DefaultRowHeight = 55
      ButtonGapHeight = 2
      AutoButtonWidth = True
      VertScrollButtonPos = vsAboveAndBelow
      VertScrollButtonHeight = 55
      HorizScrollButtonWidth = 70
      ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
      ScrollBtnUp.Font.Color = clWindowText
      ScrollBtnUp.Font.Height = -11
      ScrollBtnUp.Font.Name = 'Tahoma'
      ScrollBtnUp.Font.Style = []
      ScrollBtnUp.ParentFont = False
      ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnUp.LatchedFont.Color = clWindowText
      ScrollBtnUp.LatchedFont.Height = -11
      ScrollBtnUp.LatchedFont.Name = 'Tahoma'
      ScrollBtnUp.LatchedFont.Style = []
      ScrollBtnUp.GlyphAlpha = 255
      ScrollBtnUp.Color = -16777201
      ScrollBtnUp.FontColor = 16777215
      ScrollBtnUp.LatchedColor = -16777201
      ScrollBtnUp.DisabledColor = -16777201
      ScrollBtnUp.LatchedFontColor = -16777208
      ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
      ScrollBtnDown.Font.Color = clWindowText
      ScrollBtnDown.Font.Height = -11
      ScrollBtnDown.Font.Name = 'Tahoma'
      ScrollBtnDown.Font.Style = []
      ScrollBtnDown.ParentFont = False
      ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnDown.LatchedFont.Color = clWindowText
      ScrollBtnDown.LatchedFont.Height = -11
      ScrollBtnDown.LatchedFont.Name = 'Tahoma'
      ScrollBtnDown.LatchedFont.Style = []
      ScrollBtnDown.GlyphAlpha = 255
      ScrollBtnDown.Color = -16777201
      ScrollBtnDown.FontColor = 16777215
      ScrollBtnDown.LatchedColor = -16777201
      ScrollBtnDown.DisabledColor = -16777201
      ScrollBtnDown.LatchedFontColor = -16777208
      ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
      ScrollBtnLeft.Font.Color = clWindowText
      ScrollBtnLeft.Font.Height = -11
      ScrollBtnLeft.Font.Name = 'Tahoma'
      ScrollBtnLeft.Font.Style = []
      ScrollBtnLeft.ParentFont = False
      ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnLeft.LatchedFont.Color = clWindowText
      ScrollBtnLeft.LatchedFont.Height = -11
      ScrollBtnLeft.LatchedFont.Name = 'Tahoma'
      ScrollBtnLeft.LatchedFont.Style = []
      ScrollBtnLeft.GlyphAlpha = 255
      ScrollBtnLeft.Color = -16777201
      ScrollBtnLeft.FontColor = 16777215
      ScrollBtnLeft.LatchedColor = -16777201
      ScrollBtnLeft.DisabledColor = -16777201
      ScrollBtnLeft.LatchedFontColor = -16777208
      ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
      ScrollBtnRight.Font.Color = clWindowText
      ScrollBtnRight.Font.Height = -11
      ScrollBtnRight.Font.Name = 'Tahoma'
      ScrollBtnRight.Font.Style = []
      ScrollBtnRight.ParentFont = False
      ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnRight.LatchedFont.Color = clWindowText
      ScrollBtnRight.LatchedFont.Height = -11
      ScrollBtnRight.LatchedFont.Name = 'Tahoma'
      ScrollBtnRight.LatchedFont.Style = []
      ScrollBtnRight.GlyphAlpha = 255
      ScrollBtnRight.Color = -16777201
      ScrollBtnRight.FontColor = 16777215
      ScrollBtnRight.LatchedColor = -16777201
      ScrollBtnRight.DisabledColor = -16777201
      ScrollBtnRight.LatchedFontColor = -16777208
      Buttons = {
        04000000370000000088848484848484FC3F170100000068DFB0F60D6BDFFD3F
        0F0000FF0F0000FF0F0000FF06000000540061006800FFFFFF000E0000000000
        0000000000000000000000000000FFFFFF0006000000540061006800FFFFFF00
        0E00000000000000000000000000000000000000FFFFFF000000000001000000
        0100000001000000010000000100000001000000FF0000006400000001000000
        00000000010000000000000000000000}
    end
  end
end
