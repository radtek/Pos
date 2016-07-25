object DataMirrorEditor: TDataMirrorEditor
  Left = 609
  Top = 142
  ClientWidth = 350
  ClientHeight = 300
  ActiveControl = AddBtn
  BorderStyle = bsSizeToolWin
  Caption = 'Data Mirror Editor'
  Color = clBtnFace
  Constraints.MinHeight = 300
  Constraints.MinWidth = 350
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
    Left = 8
    Top = 8
    Width = 334
    Height = 127
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Data Mirrors'
    TabOrder = 0
    object DataMirrorLB: TListBox
      Left = 8
      Top = 16
      Width = 318
      Height = 70
      Anchors = [akLeft, akTop, akRight, akBottom]
      Enabled = False
      ItemHeight = 13
      Sorted = True
      TabOrder = 2
      OnClick = DataMirrorLBClick
    end
    object AddBtn: TButton
      Left = 163
      Top = 93
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Add'
      TabOrder = 0
      OnClick = AddBtnClick
    end
    object DeleteBtn: TButton
      Left = 250
      Top = 93
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Delete'
      Enabled = False
      TabOrder = 1
      OnClick = DeleteBtnClick
    end
  end
  object SettingsGB: TGroupBox
    Left = 8
    Top = 139
    Width = 334
    Height = 121
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Data Mirror Settings'
    TabOrder = 1
    object ValueLabel: TLabel
      Left = 8
      Top = 24
      Width = 48
      Height = 13
      Caption = 'Field Value'
      Enabled = False
    end
    object SectionLabel: TLabel
      Left = 8
      Top = 72
      Width = 78
      Height = 13
      Caption = 'Mirrored Section'
      Enabled = False
    end
    object ValueEdit: TEdit
      Left = 8
      Top = 40
      Width = 318
      Height = 21
      Anchors = [akLeft, akRight, akBottom]
      Enabled = False
      TabOrder = 0
      OnChange = ValueEditChange
    end
    object SectionCB: TComboBox
      Left = 8
      Top = 88
      Width = 318
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akRight, akBottom]
      Enabled = False
      ItemHeight = 15
      TabOrder = 3
      OnChange = SectionCBChange
    end
    object DefaultCB: TCheckBox
      Left = 201
      Top = 16
      Width = 65
      Height = 17
      Anchors = [akTop, akRight]
      Caption = 'Default'
      Enabled = False
      TabOrder = 2
      OnClick = DefaultCBClick
    end
  end
  object OKBtn: TButton
    Left = 179
    Top = 267
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 266
    Top = 267
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 3
    OnClick = CancelBtnClick
  end
end