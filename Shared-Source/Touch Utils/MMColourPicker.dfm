object frmColourPicker: TfrmColourPicker
  Tag = 1024
  Left = 0
  Top = 0
  Caption = 'frmMMColourPicker'
  ClientHeight = 328
  ClientWidth = 592
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel16: TPanel
    Left = 431
    Top = 0
    Width = 161
    Height = 328
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object TouchBtn1: TTouchBtn
      Left = 5
      Top = 5
      Width = 151
      Align = alTop
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object TouchBtn3: TTouchBtn
      Left = 5
      Top = 266
      Width = 151
      Align = alBottom
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn3MouseClick
    end
    object tbtnOtherColour: TTouchBtn
      Left = 5
      Top = 89
      Width = 151
      ParentColor = True
      Caption = 'Other Colour'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOtherColourMouseClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 431
    Height = 328
    Align = alClient
    BorderWidth = 8
    Color = 14342874
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 1
    DesignSize = (
      431
      328)
    object lbeCaption: TLabel
      Left = 9
      Top = 9
      Width = 413
      Height = 64
      Align = alTop
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Colour'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -32
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      Transparent = False
      Layout = tlCenter
      ExplicitWidth = 827
    end
    object CColorGrid: TCColorGrid
      AlignWithMargins = True
      Left = 9
      Top = 79
      Width = 412
      Height = 244
      Anchors = [akLeft, akTop, akRight, akBottom]
      Ctl3D = True
      BackgroundEnabled = False
      ParentCtl3D = False
      TabOrder = 0
      OnChange = CColorGridChange
      OnClick = CColorGridClick
    end
  end
  object ColorDialog: TColorDialog
    Options = [cdSolidColor, cdAnyColor]
    Left = 448
    Top = 152
  end
end
