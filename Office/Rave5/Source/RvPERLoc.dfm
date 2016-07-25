object ReprintLocsDialog: TReprintLocsDialog
  Left = 288
  Top = 107
  ClientWidth = 232
  ClientHeight = 230
  HorzScrollBar.Range = 227
  VertScrollBar.Range = 225
  ActiveControl = BodyHeaderCB
  BorderStyle = bsToolWindow
  Caption = 'Reprint Settings'
  Color = clBtnFace
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  ParentFont = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 75
  object GroupBox1: TGroupBox
    Left = 4
    Top = 3
    Width = 141
    Height = 222
    Caption = 'Reprint For &Locations'
    TabOrder = 0
    object BodyHeaderCB: TCheckBox
      Left = 8
      Top = 24
      Width = 131
      Height = 17
      Caption = '&Body Header (B)'
      TabOrder = 0
    end
    object GroupHeaderCB: TCheckBox
      Left = 8
      Top = 48
      Width = 131
      Height = 17
      Caption = '&Group Header (G)'
      TabOrder = 1
    end
    object RowHeaderCB: TCheckBox
      Left = 8
      Top = 72
      Width = 131
      Height = 17
      Caption = '&Row Header (R)'
      TabOrder = 2
    end
    object DetailCB: TCheckBox
      Left = 8
      Top = 120
      Width = 131
      Height = 17
      Caption = '&Detail (D)'
      TabOrder = 4
    end
    object RowFooterCB: TCheckBox
      Left = 8
      Top = 144
      Width = 131
      Height = 17
      Caption = 'Ro&w Footer (r)'
      TabOrder = 5
    end
    object GroupFooterCB: TCheckBox
      Left = 8
      Top = 168
      Width = 131
      Height = 17
      Caption = 'Grou&p Footer (g)'
      TabOrder = 6
    end
    object BodyFooterCB: TCheckBox
      Left = 8
      Top = 192
      Width = 131
      Height = 17
      Caption = 'Bod&y Footer (b)'
      TabOrder = 7
    end
    object MasterCB: TCheckBox
      Left = 8
      Top = 96
      Width = 131
      Height = 17
      Caption = '&Master (M)'
      TabOrder = 3
    end
  end
  object OKButton: TButton
    Left = 152
    Top = 8
    Width = 75
    Height = 25
    Caption = '&OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 152
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object AllButton: TButton
    Left = 152
    Top = 80
    Width = 75
    Height = 25
    Caption = '&All'
    TabOrder = 3
    OnClick = AllButtonClick
  end
  object NoneButton: TButton
    Left = 152
    Top = 112
    Width = 75
    Height = 25
    Caption = '&None'
    TabOrder = 4
    OnClick = NoneButtonClick
  end
end