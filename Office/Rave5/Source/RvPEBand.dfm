object BandStyleDialog: TBandStyleDialog
  Left = 309
  Top = 111
  Width = 328
  Height = 397
  ActiveControl = ScrollBox2
  BorderStyle = bsSizeToolWin
  Caption = 'Band Style Editor'
  Color = clBtnFace
  Constraints.MinHeight = 370
  Constraints.MinWidth = 320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = True
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 165
    Height = 370
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 2
    TabOrder = 0
    object Panel1: TPanel
      Left = 2
      Top = 2
      Width = 161
      Height = 18
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object Label1: TLabel
        Left = 2
        Top = 2
        Width = 63
        Height = 14
        Caption = 'Band Display'
      end
    end
    object Panel3: TPanel
      Left = 2
      Top = 20
      Width = 161
      Height = 348
      Align = alClient
      BevelOuter = bvLowered
      BorderWidth = 1
      TabOrder = 1
      object ScrollBox1: TScrollBox
        Left = 2
        Top = 2
        Width = 157
        Height = 344
        HorzScrollBar.Visible = False
        VertScrollBar.Color = clBtnFace
        VertScrollBar.ParentColor = False
        Align = alClient
        AutoScroll = False
        BorderStyle = bsNone
        Color = clWhite
        ParentColor = False
        TabOrder = 0
        object Image1: TImage
          Left = 0
          Top = 0
          Width = 0
          Height = 0
          AutoSize = True
        end
      end
    end
  end
  object ScrollBox2: TScrollBox
    Left = 165
    Top = 0
    Width = 155
    Height = 370
    HorzScrollBar.Range = 136
    HorzScrollBar.Visible = False
    VertScrollBar.Range = 353
    Align = alRight
    AutoScroll = False
    BorderStyle = bsNone
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 20
      Top = 72
      Width = 116
      Height = 185
      Caption = 'Print &Location'
      TabOrder = 0
      object BodyHeaderCB: TCheckBox
        Left = 8
        Top = 16
        Width = 106
        Height = 17
        Caption = '&Body Header (B)'
        TabOrder = 0
        OnClick = CheckBoxClick
      end
      object GroupHeaderCB: TCheckBox
        Left = 8
        Top = 40
        Width = 106
        Height = 17
        Caption = '&Group Header (G)'
        TabOrder = 1
        OnClick = CheckBoxClick
      end
      object RowHeaderCB: TCheckBox
        Left = 8
        Top = 64
        Width = 106
        Height = 17
        Caption = '&Row Header (R)'
        TabOrder = 2
        OnClick = CheckBoxClick
      end
      object DetailCB: TCheckBox
        Left = 8
        Top = 88
        Width = 106
        Height = 17
        Caption = '&Detail (D)'
        TabOrder = 3
        OnClick = CheckBoxClick
      end
      object RowFooterCB: TCheckBox
        Left = 8
        Top = 112
        Width = 106
        Height = 17
        Caption = 'Ro&w Footer (r)'
        TabOrder = 4
        OnClick = CheckBoxClick
      end
      object GroupFooterCB: TCheckBox
        Left = 8
        Top = 136
        Width = 106
        Height = 17
        Caption = 'Grou&p Footer (g)'
        TabOrder = 5
        OnClick = CheckBoxClick
      end
      object BodyFooterCB: TCheckBox
        Left = 8
        Top = 160
        Width = 106
        Height = 17
        Caption = 'Bod&y Footer (b)'
        TabOrder = 6
        OnClick = CheckBoxClick
      end
    end
    object GroupBox1: TGroupBox
      Left = 20
      Top = 264
      Width = 116
      Height = 89
      Caption = 'Print Occ&urrence'
      TabOrder = 1
      object FirstCB: TCheckBox
        Left = 8
        Top = 16
        Width = 106
        Height = 17
        Caption = '&First (1)'
        TabOrder = 0
        OnClick = CheckBoxClick
      end
      object NewPageCB: TCheckBox
        Left = 8
        Top = 40
        Width = 106
        Height = 17
        Caption = '&New Page (P)'
        TabOrder = 1
        OnClick = CheckBoxClick
      end
      object NewColumnCB: TCheckBox
        Left = 8
        Top = 64
        Width = 106
        Height = 17
        Caption = 'N&ew Column (C)'
        TabOrder = 2
        OnClick = CheckBoxClick
      end
    end
    object Button1: TButton
      Left = 61
      Top = 8
      Width = 75
      Height = 25
      Caption = '&OK'
      Default = True
      ModalResult = 1
      TabOrder = 2
    end
    object Button2: TButton
      Left = 61
      Top = 40
      Width = 75
      Height = 25
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 3
    end
  end
end
