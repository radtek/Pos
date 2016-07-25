object frmEditGrid: TfrmEditGrid
  Left = 135
  Top = 162
  Caption = 'TouchGrid Editor'
  ClientHeight = 489
  ClientWidth = 842
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    842
    489)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 12
    Width = 56
    Height = 13
    Caption = 'Row Count:'
  end
  object Label2: TLabel
    Left = 8
    Top = 36
    Width = 49
    Height = 13
    Caption = 'Col Count:'
  end
  object seRowCount: TCSpinEdit
    Left = 80
    Top = 8
    Width = 89
    Height = 22
    TabOrder = 0
    OnChange = seRowCountChange
  end
  object seColCount: TCSpinEdit
    Left = 80
    Top = 32
    Width = 89
    Height = 22
    TabOrder = 1
    OnChange = seColCountChange
  end
  object BitBtn1: TBitBtn
    Left = 664
    Top = 456
    Width = 81
    Height = 25
    Anchors = [akTop, akRight]
    DoubleBuffered = True
    Kind = bkOK
    ParentDoubleBuffered = False
    TabOrder = 2
  end
  object BitBtn2: TBitBtn
    Left = 752
    Top = 456
    Width = 81
    Height = 25
    Anchors = [akTop, akRight]
    DoubleBuffered = True
    Kind = bkCancel
    ParentDoubleBuffered = False
    TabOrder = 3
  end
  object Button6: TButton
    Left = 184
    Top = 20
    Width = 25
    Height = 25
    Caption = '<'
    TabOrder = 4
    OnMouseDown = Button6MouseDown
    OnMouseUp = Button7MouseUp
  end
  object Button7: TButton
    Left = 232
    Top = 20
    Width = 25
    Height = 25
    Caption = '>'
    TabOrder = 5
    OnMouseDown = Button7MouseDown
    OnMouseUp = Button7MouseUp
  end
  object Button8: TButton
    Left = 208
    Top = 8
    Width = 25
    Height = 25
    Caption = '/\'
    TabOrder = 7
    OnMouseDown = Button8MouseDown
    OnMouseUp = Button7MouseUp
  end
  object Button9: TButton
    Left = 208
    Top = 32
    Width = 25
    Height = 25
    Caption = '\/'
    TabOrder = 6
    OnMouseDown = Button9MouseDown
    OnMouseUp = Button7MouseUp
  end
  object TouchGrid: TTouchGrid
    Left = 8
    Top = 64
    Width = 393
    Height = 416
    Anchors = [akLeft, akTop, akRight, akBottom]
    DefaultButtonColor = clBtnFace
    DefaultButtonLatchedColor = clBtnFace
    LatchingType = ltCheckbox
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseDown = TouchGridMouseDown
    OnMouseUp = TouchGridMouseUp
    DefaultColWidth = 120
    DefaultRowHeight = 55
    VertScrollButtonHeight = 55
    HorizScrollButtonWidth = 70
    ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
    ScrollBtnUp.Font.Color = clWindowText
    ScrollBtnUp.Font.Height = -11
    ScrollBtnUp.Font.Name = 'MS Sans Serif'
    ScrollBtnUp.Font.Style = []
    ScrollBtnUp.ParentFont = False
    ScrollBtnUp.ParentFontColor = False
    ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollBtnUp.LatchedFont.Color = clWindowText
    ScrollBtnUp.LatchedFont.Height = -11
    ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
    ScrollBtnUp.LatchedFont.Style = []
    ScrollBtnUp.ParentLatchedFontColor = False
    ScrollBtnUp.Caption = 'Up'
    ScrollBtnUp.GlyphAlpha = 255
    ScrollBtnUp.Color = -2147483633
    ScrollBtnUp.FontColor = -2147483640
    ScrollBtnUp.LatchedColor = -2147483633
    ScrollBtnUp.DisabledColor = -2147483633
    ScrollBtnUp.LatchedFontColor = -2147483640
    ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
    ScrollBtnDown.Font.Color = clWindowText
    ScrollBtnDown.Font.Height = -11
    ScrollBtnDown.Font.Name = 'MS Sans Serif'
    ScrollBtnDown.Font.Style = []
    ScrollBtnDown.ParentFont = False
    ScrollBtnDown.ParentFontColor = False
    ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollBtnDown.LatchedFont.Color = clWindowText
    ScrollBtnDown.LatchedFont.Height = -11
    ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
    ScrollBtnDown.LatchedFont.Style = []
    ScrollBtnDown.ParentLatchedFontColor = False
    ScrollBtnDown.Caption = 'Down'
    ScrollBtnDown.GlyphAlpha = 255
    ScrollBtnDown.Color = -2147483633
    ScrollBtnDown.FontColor = -2147483640
    ScrollBtnDown.LatchedColor = -2147483633
    ScrollBtnDown.DisabledColor = -2147483633
    ScrollBtnDown.LatchedFontColor = -2147483640
    ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
    ScrollBtnLeft.Font.Color = clWindowText
    ScrollBtnLeft.Font.Height = -11
    ScrollBtnLeft.Font.Name = 'MS Sans Serif'
    ScrollBtnLeft.Font.Style = []
    ScrollBtnLeft.ParentFont = False
    ScrollBtnLeft.ParentFontColor = False
    ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollBtnLeft.LatchedFont.Color = clWindowText
    ScrollBtnLeft.LatchedFont.Height = -11
    ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
    ScrollBtnLeft.LatchedFont.Style = []
    ScrollBtnLeft.ParentLatchedFontColor = False
    ScrollBtnLeft.Caption = 'Left'
    ScrollBtnLeft.GlyphAlpha = 255
    ScrollBtnLeft.Color = -2147483633
    ScrollBtnLeft.FontColor = -2147483640
    ScrollBtnLeft.LatchedColor = -2147483633
    ScrollBtnLeft.DisabledColor = -2147483633
    ScrollBtnLeft.LatchedFontColor = -2147483640
    ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
    ScrollBtnRight.Font.Color = clWindowText
    ScrollBtnRight.Font.Height = -11
    ScrollBtnRight.Font.Name = 'MS Sans Serif'
    ScrollBtnRight.Font.Style = []
    ScrollBtnRight.ParentFont = False
    ScrollBtnRight.ParentFontColor = False
    ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollBtnRight.LatchedFont.Color = clWindowText
    ScrollBtnRight.LatchedFont.Height = -11
    ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
    ScrollBtnRight.LatchedFont.Style = []
    ScrollBtnRight.ParentLatchedFontColor = False
    ScrollBtnRight.Caption = 'Right'
    ScrollBtnRight.GlyphAlpha = 255
    ScrollBtnRight.Color = -2147483633
    ScrollBtnRight.FontColor = -2147483640
    ScrollBtnRight.LatchedColor = -2147483633
    ScrollBtnRight.DisabledColor = -2147483633
    ScrollBtnRight.LatchedFontColor = -2147483640
    OnScrollComplete = TouchGridScrollComplete
    Buttons = {04000000}
  end
  object Panel1: TPanel
    Left = 416
    Top = 8
    Width = 417
    Height = 441
    Anchors = [akTop, akRight]
    TabOrder = 9
    object Label3: TLabel
      Left = 8
      Top = 108
      Width = 33
      Height = 13
      Caption = 'Colour:'
    end
    object Label4: TLabel
      Left = 8
      Top = 12
      Width = 43
      Height = 13
      Caption = 'Col With:'
    end
    object Label5: TLabel
      Left = 216
      Top = 12
      Width = 59
      Height = 13
      Caption = 'Row Height:'
    end
    object Label6: TLabel
      Left = 8
      Top = 36
      Width = 39
      Height = 13
      Caption = 'Caption:'
    end
    object Label7: TLabel
      Left = 8
      Top = 180
      Width = 21
      Height = 13
      Caption = 'Font'
    end
    object Label8: TLabel
      Left = 8
      Top = 132
      Width = 75
      Height = 13
      Caption = 'Latched Colour:'
    end
    object Label9: TLabel
      Left = 8
      Top = 228
      Width = 57
      Height = 13
      Caption = 'Font Colour:'
    end
    object Label14: TLabel
      Left = 8
      Top = 204
      Width = 63
      Height = 13
      Caption = 'Latched Font'
    end
    object Label15: TLabel
      Left = 8
      Top = 252
      Width = 99
      Height = 13
      Caption = 'Latched Font Colour:'
    end
    object Label16: TLabel
      Left = 8
      Top = 156
      Width = 77
      Height = 13
      Caption = 'Disabled Colour:'
    end
    object Label17: TLabel
      Left = 8
      Top = 276
      Width = 22
      Height = 13
      Caption = 'Tag:'
    end
    object Button1: TButton
      Left = 120
      Top = 104
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 0
      OnClick = Button1Click
    end
    object edCaption: TEdit
      Left = 120
      Top = 32
      Width = 281
      Height = 21
      Enabled = False
      TabOrder = 1
      OnChange = edCaptionChange
    end
    object chbVisible: TCheckBox
      Left = 6
      Top = 60
      Width = 127
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Visible:'
      Enabled = False
      TabOrder = 2
      OnClick = chbVisibleClick
    end
    object chbEnabled: TCheckBox
      Left = 6
      Top = 84
      Width = 127
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Enabled:'
      Enabled = False
      TabOrder = 3
      OnClick = chbEnabledClick
    end
    object Button2: TButton
      Left = 120
      Top = 176
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 4
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 120
      Top = 128
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 5
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 120
      Top = 224
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 6
      OnClick = Button4Click
    end
    object seColWidth: TCSpinEdit
      Left = 120
      Top = 8
      Width = 89
      Height = 22
      TabOrder = 7
      OnChange = seColWidthChange
    end
    object seRowHeight: TCSpinEdit
      Left = 312
      Top = 8
      Width = 89
      Height = 22
      TabOrder = 8
      OnChange = seRowHeightChange
    end
    object chbParentFont: TCheckBox
      Left = 256
      Top = 177
      Width = 153
      Height = 17
      Caption = 'Parent Font'
      Enabled = False
      TabOrder = 9
      OnClick = chbParentFontClick
    end
    object chbParentFontColor: TCheckBox
      Left = 256
      Top = 225
      Width = 153
      Height = 17
      Caption = 'Parent Font Color'
      Enabled = False
      TabOrder = 10
      OnClick = chbParentFontColorClick
    end
    object chbParentLatchedFont: TCheckBox
      Left = 256
      Top = 201
      Width = 153
      Height = 17
      Caption = 'Parent Latched Font'
      Enabled = False
      TabOrder = 11
      OnClick = chbParentLatchedFontClick
    end
    object Button11: TButton
      Left = 120
      Top = 200
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 12
      OnClick = Button11Click
    end
    object chbParentLatchedFontColor: TCheckBox
      Left = 256
      Top = 249
      Width = 153
      Height = 17
      Caption = 'Parent Latched Font Color'
      Enabled = False
      TabOrder = 13
      OnClick = chbParentLatchedFontColorClick
    end
    object Button12: TButton
      Left = 120
      Top = 248
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 14
      OnClick = Button12Click
    end
    object Button17: TButton
      Left = 216
      Top = 128
      Width = 33
      Height = 21
      Caption = 'Get'
      TabOrder = 15
      OnClick = Button17Click
    end
    object Button18: TButton
      Left = 216
      Top = 104
      Width = 33
      Height = 21
      Caption = 'Get'
      TabOrder = 16
      OnClick = Button18Click
    end
    object Button19: TButton
      Left = 216
      Top = 224
      Width = 33
      Height = 21
      Caption = 'Get'
      TabOrder = 17
      OnClick = Button19Click
    end
    object Button20: TButton
      Left = 216
      Top = 248
      Width = 33
      Height = 21
      Caption = 'Get'
      TabOrder = 18
      OnClick = Button20Click
    end
    object Button21: TButton
      Left = 120
      Top = 152
      Width = 89
      Height = 21
      Caption = 'Change'
      TabOrder = 19
      OnClick = Button21Click
    end
    object Button22: TButton
      Left = 216
      Top = 152
      Width = 33
      Height = 21
      Caption = 'Get'
      TabOrder = 20
      OnClick = Button22Click
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 304
      Width = 385
      Height = 121
      Caption = 'Glyph Settings'
      TabOrder = 21
      object Label10: TLabel
        Left = 8
        Top = 28
        Width = 30
        Height = 13
        Caption = 'Glyph:'
      end
      object Label11: TLabel
        Left = 232
        Top = 28
        Width = 30
        Height = 13
        Caption = 'Alpha:'
      end
      object Label12: TLabel
        Left = 8
        Top = 52
        Width = 87
        Height = 13
        Caption = 'Transparent Color:'
      end
      object Label13: TLabel
        Left = 232
        Top = 68
        Width = 30
        Height = 13
        Caption = 'Scale:'
      end
      object Button5: TButton
        Left = 120
        Top = 24
        Width = 89
        Height = 21
        Caption = 'Load'
        TabOrder = 0
        OnClick = Button5Click
      end
      object seAlpha: TCSpinEdit
        Left = 272
        Top = 24
        Width = 81
        Height = 22
        TabOrder = 1
        OnChange = seAlphaChange
      end
      object Button10: TButton
        Left = 120
        Top = 48
        Width = 89
        Height = 21
        Caption = 'Change'
        TabOrder = 2
        OnClick = Button10Click
      end
      object chbTransparent: TCheckBox
        Left = 272
        Top = 49
        Width = 97
        Height = 17
        Caption = 'Transparent'
        Enabled = False
        TabOrder = 3
        OnClick = chbTransparentClick
      end
      object chbKeepAspect: TCheckBox
        Left = 6
        Top = 76
        Width = 127
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Keep Aspect:'
        Enabled = False
        TabOrder = 4
        OnClick = chbKeepAspectClick
      end
      object seGlyphScale: TCSpinEdit
        Left = 272
        Top = 72
        Width = 81
        Height = 22
        TabOrder = 5
        OnChange = seGlyphScaleChange
      end
    end
    object edTag: TEdit
      Left = 120
      Top = 272
      Width = 281
      Height = 21
      Enabled = False
      TabOrder = 22
      OnChange = edTagChange
    end
  end
  object Button13: TButton
    Left = 264
    Top = 8
    Width = 65
    Height = 21
    Caption = 'Delete Row'
    TabOrder = 10
    OnClick = Button13Click
  end
  object Button14: TButton
    Left = 264
    Top = 32
    Width = 65
    Height = 21
    Caption = 'Delete Col'
    TabOrder = 11
    OnClick = Button14Click
  end
  object Button15: TButton
    Left = 336
    Top = 8
    Width = 65
    Height = 21
    Caption = 'Insert Row'
    TabOrder = 12
    OnClick = Button15Click
  end
  object Button16: TButton
    Left = 336
    Top = 32
    Width = 65
    Height = 21
    Caption = 'Insert Col'
    TabOrder = 13
    OnClick = Button16Click
  end
  object FontDialog: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Left = 72
    Top = 128
  end
  object ColorDialog: TColorDialog
    Left = 128
    Top = 128
  end
  object OpenDialog: TOpenDialog
    Filter = 'Bitmap|*.bmp|All Files|*.*'
    Left = 184
    Top = 128
  end
end
