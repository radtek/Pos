object frmTierLevel: TfrmTierLevel
  Tag = 1024
  Left = 175
  Top = 26
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Tier Levels'
  ClientHeight = 656
  ClientWidth = 596
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMajor: TPanel
    Left = 9
    Top = 8
    Width = 432
    Height = 641
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object lbeHeader: TStaticText
      Left = 1
      Top = 1
      Width = 430
      Height = 27
      Align = alTop
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Tier Levels'
      Color = 39157
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
    object tgridSelection: TTouchGrid
      Left = 1
      Top = 28
      Width = 430
      Height = 612
      BorderWidth = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      Color = 14342874
      ParentFont = False
      DefaultButtonColor = 6315356
      DefaultButtonLatchedColor = 12557568
      LatchingType = ltRadioButton
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tgridSelectionMouseClick
      AutoScale = True
      GridBorder = False
      DefaultColWidth = 120
      DefaultRowHeight = 55
      ButtonGapHeight = 5
      AutoButtonWidth = True
      VertScrollButtonPos = vsBelow
      VertScrollButtonHeight = 55
      HorizScrollButtonWidth = 70
      ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
      ScrollBtnUp.Font.Color = clWindowText
      ScrollBtnUp.Font.Height = -11
      ScrollBtnUp.Font.Name = 'MS Sans Serif'
      ScrollBtnUp.Font.Style = []
      ScrollBtnUp.ParentFont = False
      ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnUp.LatchedFont.Color = clWindowText
      ScrollBtnUp.LatchedFont.Height = -11
      ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnUp.LatchedFont.Style = []
      ScrollBtnUp.ParentLatchedFontColor = False
      ScrollBtnUp.GlyphAlpha = 255
      ScrollBtnUp.Color = -16777201
      ScrollBtnUp.FontColor = 16777215
      ScrollBtnUp.LatchedColor = -2147483633
      ScrollBtnUp.DisabledColor = -16777201
      ScrollBtnUp.LatchedFontColor = -2147483640
      ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
      ScrollBtnDown.Font.Color = clWindowText
      ScrollBtnDown.Font.Height = -11
      ScrollBtnDown.Font.Name = 'MS Sans Serif'
      ScrollBtnDown.Font.Style = []
      ScrollBtnDown.ParentFont = False
      ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
      ScrollBtnDown.LatchedFont.Color = clWindowText
      ScrollBtnDown.LatchedFont.Height = -11
      ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
      ScrollBtnDown.LatchedFont.Style = []
      ScrollBtnDown.ParentLatchedFontColor = False
      ScrollBtnDown.GlyphAlpha = 255
      ScrollBtnDown.Color = -16777201
      ScrollBtnDown.FontColor = 16777215
      ScrollBtnDown.LatchedColor = -2147483633
      ScrollBtnDown.DisabledColor = -16777201
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
      ScrollBtnRight.GlyphAlpha = 255
      ScrollBtnRight.Color = -2147483633
      ScrollBtnRight.FontColor = 16777215
      ScrollBtnRight.LatchedColor = -2147483633
      ScrollBtnRight.DisabledColor = -2147483633
      ScrollBtnRight.LatchedFontColor = -2147483640
      Buttons = {04000000}
    end
  end
  object pnlMinor: TPanel
    Left = 448
    Top = 8
    Width = 139
    Height = 641
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnClose: TTouchBtn
      Left = 9
      Top = 13
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object btnAdd: TTouchBtn
      Left = 9
      Top = 113
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAddMouseClick
    end
    object btnEdit: TTouchBtn
      Left = 8
      Top = 213
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Edit'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnEditMouseClick
    end
    object btnDelete: TTouchBtn
      Left = 8
      Top = 313
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Delete'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDeleteMouseClick
    end
    object btnSync: TTouchBtn
      Left = 8
      Top = 413
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Sync With Cloud'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSyncMouseClick
    end
  end
end
