object PageListEditor: TPageListEditor
  Left = 543
  Top = 120
  ClientWidth = 343
  ClientHeight = 326
  HorzScrollBar.Range = 83
  VertScrollBar.Range = 149
  ActiveControl = ReportCB
  BorderStyle = bsSizeToolWin
  Caption = 'Page List Editor'
  Color = clBtnFace
  Constraints.MinHeight = 326
  Constraints.MinWidth = 343
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  ParentFont = False
  Position = poMainFormCenter
  Scaled = False
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 75
  object Label1: TLabel
    Left = 40
    Top = 136
    Width = 43
    Height = 13
    Caption = 'Page List'
  end
  object OKBtn: TButton
    Left = 174
    Top = 296
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
    Left = 261
    Top = 296
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 328
    Height = 121
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Available Pages'
    TabOrder = 0
    object Label2: TLabel
      Left = 16
      Top = 24
      Width = 63
      Height = 13
      Caption = 'Report Pages'
    end
    object Label3: TLabel
      Left = 16
      Top = 72
      Width = 60
      Height = 13
      Caption = 'Global Pages'
    end
    object ReportCB: TComboBox
      Left = 16
      Top = 40
      Width = 208
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 15
      Sorted = True
      TabOrder = 0
    end
    object GlobalCB: TComboBox
      Left = 16
      Top = 88
      Width = 208
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 15
      Sorted = True
      TabOrder = 2
    end
    object AddPageButton: TButton
      Left = 237
      Top = 37
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Add Page'
      TabOrder = 1
      OnClick = AddPageButtonClick
    end
    object AddGlobalButton: TButton
      Left = 237
      Top = 85
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Add &Global'
      TabOrder = 3
      OnClick = AddGlobalButtonClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 136
    Width = 329
    Height = 153
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Page List'
    TabOrder = 1
    object MoveUpSB: TSpeedButton
      Left = 13
      Top = 30
      Width = 25
      Height = 25
      Hint = 'Move Page Up'
      Anchors = [akLeft]
      Glyph.Data = {
        7A010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000333
        3333333333777F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333FF7F7FFFF333333000090000
        3333333777737777F333333099999990333333373F3333373333333309999903
        333333337F33337F33333333099999033333333373F333733333333330999033
        3333333337F337F3333333333099903333333333373F37333333333333090333
        33333333337F7F33333333333309033333333333337373333333333333303333
        333333333337F333333333333330333333333333333733333333}
      NumGlyphs = 2
      OnClick = MoveUpSBClick
    end
    object MoveDownSB: TSpeedButton
      Left = 13
      Top = 107
      Width = 25
      Height = 25
      Hint = 'Move Page Down'
      Anchors = [akLeft]
      Glyph.Data = {
        7A010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333303333
        333333333337F33333333333333033333333333333373F333333333333090333
        33333333337F7F33333333333309033333333333337373F33333333330999033
        3333333337F337F33333333330999033333333333733373F3333333309999903
        333333337F33337F33333333099999033333333373333373F333333099999990
        33333337FFFF3FF7F33333300009000033333337777F77773333333333090333
        33333333337F7F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333300033333333333337773333333}
      NumGlyphs = 2
      OnClick = MoveDownSBClick
    end
    object DeleteSB: TSpeedButton
      Left = 13
      Top = 68
      Width = 25
      Height = 25
      Hint = 'Delete Page'
      Anchors = [akLeft]
      Glyph.Data = {
        7A010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333FF3333333333333003333333333333F77F33333333333009033
        333333333F7737F333333333009990333333333F773337FFFFFF330099999000
        00003F773333377777770099999999999990773FF33333FFFFF7330099999000
        000033773FF33777777733330099903333333333773FF7F33333333333009033
        33333333337737F3333333333333003333333333333377333333333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333}
      NumGlyphs = 2
      OnClick = DeleteSBClick
    end
    object PageLB: TListBox
      Left = 48
      Top = 16
      Width = 273
      Height = 129
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 13
      TabOrder = 0
      OnClick = PageLBClick
    end
  end
end