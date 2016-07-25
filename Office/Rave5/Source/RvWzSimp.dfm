object RvWzSimpleForm: TRvWzSimpleForm
  Left = 401
  Top = 140
  Width = 380
  Height = 339
  VertScrollBar.Range = 81
  ActiveControl = WizardPC
  BorderStyle = bsToolWindow
  Caption = 'Simple Table'
  Color = clBtnFace
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
  object WizardPC: TPageControl
    Left = 0
    Top = 40
    Width = 372
    Height = 231
    ActivePage = tabsFields
    Align = alClient
    TabOrder = 0
    object tabsDataView: TTabSheet
      Caption = '@DataViewPage'
      TabVisible = False
      object DataViewLB: TListBox
        Left = 0
        Top = 0
        Width = 364
        Height = 221
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
      end
    end
    object tabsFields: TTabSheet
      Caption = '@FieldsPage'
      TabVisible = False
      object FieldsCBL: TCheckListBox
        Left = 0
        Top = 37
        Width = 364
        Height = 184
        OnClickCheck = FieldsCBLClick
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = FieldsCBLClick
      end
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 37
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object SelectAllBtn: TBitBtn
          Left = 217
          Top = 3
          Width = 65
          Height = 26
          Hint = 'Select All Fields'
          Caption = '&All'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          OnClick = SelectAllBtnClick
        end
        object SelectNoneBtn: TBitBtn
          Left = 287
          Top = 3
          Width = 65
          Height = 26
          Hint = 'Clear All Selected Items'
          Caption = '&None'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          OnClick = SelectNoneBtnClick
        end
      end
    end
    object tabsSortFields: TTabSheet
      Caption = '@SortFieldsPage'
      TabVisible = False
      object Panel9: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 31
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object MoveUpSB: TSpeedButton
          Left = 9
          Top = 2
          Width = 25
          Height = 25
          Hint = 'Move the Selected Item Up One Position'
          Enabled = False
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
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
          ParentShowHint = False
          ShowHint = True
          OnClick = MoveUpSBClick
        end
        object MoveDownSB: TSpeedButton
          Left = 37
          Top = 2
          Width = 25
          Height = 25
          Hint = 'Move The Selected Item Down One Position'
          Enabled = False
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
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
          ParentShowHint = False
          ShowHint = True
          OnClick = MoveDownSBClick
        end
      end
      object SortFieldsLB: TListBox
        Left = 0
        Top = 31
        Width = 364
        Height = 190
        Align = alClient
        ItemHeight = 14
        TabOrder = 1
        OnClick = SortFieldsLBClick
      end
    end
    object tabsOptions: TTabSheet
      Caption = '@OptionsPage'
      TabVisible = False
      object GroupBox1: TGroupBox
        Left = 16
        Top = 80
        Width = 337
        Height = 137
        Caption = 'Report Margins'
        TabOrder = 0
        object Label2: TLabel
          Left = 29
          Top = 23
          Width = 19
          Height = 14
          Caption = 'Left'
        end
        object Label3: TLabel
          Left = 29
          Top = 51
          Width = 18
          Height = 14
          Caption = 'Top'
        end
        object Label4: TLabel
          Left = 29
          Top = 80
          Width = 24
          Height = 14
          Caption = 'Right'
        end
        object Label5: TLabel
          Left = 29
          Top = 108
          Width = 33
          Height = 14
          Caption = 'Bottom'
        end
        object LeftMarginEdit: TEdit
          Left = 182
          Top = 19
          Width = 121
          Height = 22
          Hint = 'Type the position of the left margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          Text = '1.0'
        end
        object TopMarginEdit: TEdit
          Left = 182
          Top = 47
          Width = 121
          Height = 22
          Hint = 'Type the position of the top margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          Text = '1.0'
        end
        object RightMarginEdit: TEdit
          Left = 182
          Top = 76
          Width = 121
          Height = 22
          Hint = 'Type the position of the right margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
          Text = '1.0'
        end
        object BottomMarginEdit: TEdit
          Left = 182
          Top = 104
          Width = 121
          Height = 22
          Hint = 'Type the position for the bottom margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 3
          Text = '1.0'
        end
      end
      object GroupBox2: TGroupBox
        Left = 16
        Top = 4
        Width = 337
        Height = 71
        Caption = 'Report Title'
        TabOrder = 1
        object RptTitleEdit: TEdit
          Left = 30
          Top = 18
          Width = 276
          Height = 22
          Hint = 'Type the Title You Want For This Report'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          Text = 'Simple Table Report'
        end
        object ReportTitleCB: TCheckBox
          Left = 114
          Top = 44
          Width = 109
          Height = 17
          Caption = 'Print Report Title'
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
      end
    end
    object tabsFonts: TTabSheet
      Caption = '@FontPage'
      TabVisible = False
      object BodyPanel: TPanel
        Left = 0
        Top = 183
        Width = 364
        Height = 40
        Align = alBottom
        Caption = 'The Printed Data Will Look Like This'
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object TitlePanel: TPanel
        Left = 0
        Top = 30
        Width = 364
        Height = 53
        Align = alClient
        Caption = 'Report Title'
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -48
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel8: TPanel
        Left = 0
        Top = 83
        Width = 364
        Height = 30
        Align = alBottom
        Alignment = taLeftJustify
        BorderWidth = 5
        Caption = 'Caption Font'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object CaptionFontBtn: TBitBtn
          Left = 272
          Top = 3
          Width = 81
          Height = 25
          Caption = 'Change Font'
          TabOrder = 0
          OnClick = CaptionFontBtnClick
        end
      end
      object Panel6: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 30
        Align = alTop
        Alignment = taLeftJustify
        BorderWidth = 5
        Caption = 'Title Font'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        object TitleFontBtn: TBitBtn
          Left = 272
          Top = 3
          Width = 81
          Height = 25
          Caption = 'Change Font'
          TabOrder = 0
          OnClick = TitleFontBtnClick
        end
      end
      object Panel10: TPanel
        Left = 0
        Top = 153
        Width = 364
        Height = 30
        Align = alBottom
        Alignment = taLeftJustify
        BorderWidth = 5
        Caption = 'Body Font'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        object BodyFontBtn: TBitBtn
          Left = 272
          Top = 3
          Width = 81
          Height = 25
          Caption = 'Change Font'
          TabOrder = 0
          OnClick = BodyFontBtnClick
        end
      end
      object CaptionPanel: TPanel
        Left = 0
        Top = 113
        Width = 364
        Height = 40
        Align = alBottom
        Caption = 'The Caption Font Will Look Like This'
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
    end
  end
  object ButtonPanel: TPanel
    Left = 0
    Top = 271
    Width = 372
    Height = 41
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      372
      41)
    object CancelBtn: TBitBtn
      Left = 293
      Top = 8
      Width = 65
      Height = 25
      Hint = 'Cancel The Report Wizard'
      Anchors = [akTop, akRight, akBottom]
      Caption = '&Cancel'
      ModalResult = 2
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object NextGenBtn: TBitBtn
      Left = 223
      Top = 8
      Width = 65
      Height = 25
      Anchors = [akTop, akRight, akBottom]
      Caption = '&Next >'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = NextGenBtnClick
    end
    object BackBtn: TBitBtn
      Left = 154
      Top = 8
      Width = 65
      Height = 25
      Anchors = [akTop, akRight, akBottom]
      Caption = '< &Back'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = BackBtnClick
    end
  end
  object Panel11: TPanel
    Left = 0
    Top = 0
    Width = 372
    Height = 40
    Align = alTop
    BorderWidth = 3
    TabOrder = 2
    object Image1: TImage
      Left = 4
      Top = 4
      Width = 32
      Height = 32
      Align = alLeft
      Picture.Data = {
        07544269746D617076020000424D760200000000000076000000280000002000
        0000200000000100040000000000000200000000000000000000100000000000
        000000000000000080000080000000808000800000008000800080800000C0C0
        C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
        FF00777777777777777777777777777777777777777777777777777777777777
        7777777770000000077777778888888877777770005555550007778880000008
        8877770555555555555078000000000000877705F55F5FF5F5F0780F0F0FF00F
        F087770F5F5F5F55FFF0780F0F0F0F8F0F87770F5F5F5FF5F5F0780FFF0FF00F
        0087770F5F5F5F55F5F0780F0F0F0F8F0F87770F5F5F5FF5F5F07800FF0FF00F
        F087770555555555555078000000000000877705500000000550780088888888
        0087770000DDDDDD00007888877777788887770DDDDDDDDDDDD0787777777777
        7787777000DDDDDDD00777888777777888777777700000000777777788888888
        7777777777777777777777777777777777777777777777777777777777777777
        7777777777777777777777777777777777777777777777077777077770000077
        7777777777777707777707777707777777777777777770707770707777077777
        7777777777777070777070777770777777777777777770707770707777770777
        7777777777770777070777077777077777777777777707770707770770000077
        7777777777770777070777077777777777777777777077770707777077777777
        7777777777707777707777707777777777777777777777777777777777777777
        7777777777777777777777777777777777777777777777777777777777777777
        7777}
      Transparent = True
    end
    object HeaderPanel: TPanel
      Left = 36
      Top = 4
      Width = 332
      Height = 32
      Align = alClient
      BevelOuter = bvNone
      Caption = 'Page Description Here'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Pitch = fpVariable
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
  end
  object FontDlg: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Pitch = fpVariable
    Font.Style = []
    Left = 12
    Top = 231
  end
end
