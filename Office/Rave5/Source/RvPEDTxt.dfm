object DataTextEditor: TDataTextEditor
  Left = 419
  Top = 191
  ClientWidth = 450
  ClientHeight = 416
  VertScrollBar.Range = 265
  ActiveControl = DataTextMemo
  BorderStyle = bsSizeToolWin
  Caption = 'Data Text Editor'
  Color = clBtnFace
  Constraints.MinHeight = 416
  Constraints.MinWidth = 450
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
  PixelsPerInch = 96
  object GroupBox1: TGroupBox
    Left = 8
    Top = 272
    Width = 433
    Height = 105
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Data Text'
    TabOrder = 0
    object PlusBtn: TSpeedButton
      Left = 400
      Top = 24
      Width = 23
      Height = 22
      Anchors = [akRight]
      Caption = '+'
      OnClick = PlusBtnClick
    end
    object AmperBtn: TSpeedButton
      Left = 400
      Top = 66
      Width = 23
      Height = 22
      Anchors = [akRight]
      Caption = '&&'
      OnClick = AmperBtnClick
    end
    object DataTextMemo: TMemo
      Left = 8
      Top = 16
      Width = 385
      Height = 81
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 0
      WantReturns = False
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 433
    Height = 89
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Data Fields'
    TabOrder = 1
    object Label1: TLabel
      Left = 256
      Top = 16
      Width = 47
      Height = 14
      Caption = 'Data Field'
    end
    object DataFieldCB: TComboBox
      Left = 256
      Top = 32
      Width = 169
      Height = 22
      TabStop = False
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      OnChange = DataFieldCBChange
    end
    object InsertFieldBtn: TButton
      Left = 332
      Top = 57
      Width = 94
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Insert &Field'
      TabOrder = 2
      TabStop = False
      OnClick = InsertFieldBtnClick
    end
    object DataViewGB: TGroupBox
      Left = 8
      Top = 16
      Width = 241
      Height = 65
      Caption = 'Data View'
      TabOrder = 3
      object DefaultRB: TRadioButton
        Left = 8
        Top = 16
        Width = 225
        Height = 17
        Caption = 'Default'
        TabOrder = 0
        OnClick = DefaultRBClick
        OnEnter = RBEnter
      end
      object SelectedRB: TRadioButton
        Left = 8
        Top = 40
        Width = 73
        Height = 17
        Caption = 'Selected'
        TabOrder = 1
        OnClick = SelectedRBClick
        OnEnter = RBEnter
      end
    end
    object DataViewCB: TComboBox
      Left = 85
      Top = 53
      Width = 157
      Height = 22
      TabStop = False
      Style = csDropDownList
      TabOrder = 0
      OnChange = DataViewCBChange
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 104
    Width = 433
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Report Variables'
    TabOrder = 2
    object VariableCB: TComboBox
      Left = 8
      Top = 16
      Width = 313
      Height = 22
      TabStop = False
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = VariableCBChange
    end
    object InsertVarBtn: TButton
      Left = 330
      Top = 14
      Width = 96
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Insert &Report Var'
      TabOrder = 1
      TabStop = False
      OnClick = InsertVarBtnClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 160
    Width = 433
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Project Parameters'
    TabOrder = 3
    object ParamCB: TComboBox
      Left = 8
      Top = 16
      Width = 313
      Height = 22
      TabStop = False
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      Sorted = True
      TabOrder = 0
      OnChange = ParamCBChange
    end
    object InsertParamBtn: TButton
      Left = 330
      Top = 14
      Width = 96
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Insert &Parameter'
      TabOrder = 1
      TabStop = False
      OnClick = InsertParamBtnClick
    end
  end
  object OKBtn: TButton
    Left = 280
    Top = 383
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    TabStop = False
  end
  object CancelBtn: TButton
    Left = 365
    Top = 383
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 5
    TabStop = False
  end
  object GroupBox5: TGroupBox
    Left = 8
    Top = 216
    Width = 433
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Post Initialize Variables'
    TabOrder = 6
    object InsertPIVarBtn: TButton
      Left = 330
      Top = 14
      Width = 96
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Insert PI &Var'
      TabOrder = 0
      TabStop = False
      OnClick = InsertPIVarBtnClick
    end
    object PIVarCB: TComboBox
      Left = 8
      Top = 16
      Width = 313
      Height = 22
      TabStop = False
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      Sorted = True
      TabOrder = 1
      OnChange = PIVarCBChange
    end
  end
end