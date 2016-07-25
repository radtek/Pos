object MailMergeEditor: TMailMergeEditor
  Left = 422
  Top = 135
  ClientWidth = 350
  ClientHeight = 350
  ActiveControl = MailMergeLB
  BorderStyle = bsSizeToolWin
  Caption = 'Mail Merge Editor'
  Color = clBtnFace
  Constraints.MinHeight = 350
  Constraints.MinWidth = 350
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  ParentFont = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 335
    Height = 120
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Mail Merge Items'
    TabOrder = 0
    object MailMergeLB: TListBox
      Left = 8
      Top = 16
      Width = 319
      Height = 62
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 14
      TabOrder = 0
      OnClick = MailMergeLBClick
    end
    object AddBtn: TButton
      Left = 164
      Top = 85
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Add'
      TabOrder = 1
      OnClick = AddBtnClick
    end
    object DeleteBtn: TButton
      Left = 251
      Top = 85
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Delete'
      TabOrder = 2
      OnClick = DeleteBtnClick
    end
  end
  object gboxSettings: TGroupBox
    Left = 8
    Top = 134
    Width = 335
    Height = 175
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Mail Merge Settings'
    TabOrder = 1
    object SearchLabel: TLabel
      Left = 8
      Top = 24
      Width = 67
      Height = 14
      Caption = 'Search Token'
    end
    object ReplaceLabel: TLabel
      Left = 8
      Top = 72
      Width = 93
      Height = 14
      Caption = 'Replacement String'
    end
    object SearchEdit: TEdit
      Left = 8
      Top = 40
      Width = 319
      Height = 22
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = MailMergeChange
    end
    object ReplaceMemo: TMemo
      Left = 8
      Top = 88
      Width = 319
      Height = 45
      Anchors = [akLeft, akTop, akRight]
      ScrollBars = ssVertical
      TabOrder = 1
      WantReturns = False
      OnChange = MailMergeChange
    end
    object EditBtn: TButton
      Left = 251
      Top = 141
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Edit'
      TabOrder = 2
      OnClick = EditBtnClick
    end
    object CaseCB: TCheckBox
      Left = 222
      Top = 21
      Width = 105
      Height = 17
      Anchors = [akTop, akRight]
      Caption = 'Case sensitive'
      Enabled = False
      TabOrder = 3
      OnClick = MailMergeChange
    end
  end
  object OKBtn: TButton
    Left = 179
    Top = 317
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
    Left = 267
    Top = 317
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