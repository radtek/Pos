object frmScales: TfrmScales
  Tag = 1024
  Left = 208
  Top = 228
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 5
  ClientHeight = 427
  ClientWidth = 639
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 489
    Height = 427
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label2: TLabel
      Left = 16
      Top = 190
      Width = 132
      Height = 24
      Caption = 'Unit Price/kg'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbUnitPrice: TLabel
      Left = 313
      Top = 172
      Width = 160
      Height = 60
      Alignment = taRightJustify
      Caption = '000.00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbItemName: TLabel
      Left = 1
      Top = 1
      Width = 8
      Height = 32
      Align = alTop
      Alignment = taCenter
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbPackagingWeight: TLabel
      Left = 313
      Top = 42
      Width = 160
      Height = 60
      Alignment = taRightJustify
      Caption = '0.0000'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 16
      Top = 60
      Width = 169
      Height = 24
      Caption = 'Packaging Weight'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 125
      Width = 136
      Height = 24
      Caption = 'Scales Weight'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object lbScalesWeight: TLabel
      Left = 313
      Top = 107
      Width = 160
      Height = 60
      Alignment = taRightJustify
      Caption = '0.0000'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Panel4: TPanel
      Left = 1
      Top = 240
      Width = 487
      Height = 186
      Align = alBottom
      BevelWidth = 2
      BorderStyle = bsSingle
      Color = 10011108
      TabOrder = 0
      object Label4: TLabel
        Left = 16
        Top = 119
        Width = 142
        Height = 32
        Caption = 'Total Price'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -27
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbTotalPrice: TLabel
        Left = 275
        Top = 96
        Width = 204
        Height = 78
        Alignment = taRightJustify
        Caption = '000.00'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -67
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object WeightLabel: TLabel
        Left = 275
        Top = 4
        Width = 204
        Height = 78
        Alignment = taRightJustify
        Caption = '0.0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -67
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label1: TLabel
        Left = 8
        Top = 27
        Width = 166
        Height = 32
        Caption = 'Total Weight'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -27
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
  end
  object Panel2: TPanel
    Left = 494
    Top = 0
    Width = 145
    Height = 427
    Align = alRight
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tbtnOk: TTouchBtn
      Left = 6
      Top = 6
      Width = 133
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      Enabled = False
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOkMouseClick
    end
    object tbtnCancel: TTouchBtn
      Left = 6
      Top = 364
      Width = 133
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
      OnMouseClick = tbtnCancelMouseClick
    end
    object tbtnAmount: TTouchBtn
      Left = 6
      Top = 92
      Width = 133
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Enter Price'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAmountMouseClick
    end
    object tbtnWeight: TTouchBtn
      Left = 6
      Top = 178
      Width = 133
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Enter Weight'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnWeightMouseClick
    end
  end
  object Panel3: TPanel
    Left = 489
    Top = 0
    Width = 5
    Height = 427
    Align = alRight
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 2
  end
end
