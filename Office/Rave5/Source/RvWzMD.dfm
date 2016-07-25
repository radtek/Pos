object RvWzMDForm: TRvWzMDForm
  Left = 334
  Top = 114
  Width = 380
  Height = 340
  VertScrollBar.Range = 81
  ActiveControl = CancelBtn
  BorderStyle = bsToolWindow
  Caption = 'Master/Detail Wizard'
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
  object Panel11: TPanel
    Left = 0
    Top = 0
    Width = 372
    Height = 40
    Align = alTop
    BorderWidth = 3
    TabOrder = 0
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
        7777777770000000077777770000000077777770005555550007770005555550
        0077770555555555555070555555555555077705F55F5FF5F5F0705F55F5FF5F
        5F07770F5F5F5F55FFF070F5F5F5F55FFF07770F5F5F5FF5F5F070F5F5F5FF5F
        5F07770F5F5F5F55F5F070F5F5F5F55F5F07770F5F5F5FF5F5F070F5F5F5FF5F
        5F07770555555555555070555555555555077705500000000550705500000000
        5507770000DDDDDD000070000DDDDDD00007770DDDDDDDDDDDD070DDDDDDDDDD
        DD07777000DDDDDDD00777000DDDDDDD00777777700000000777777700000000
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
  object ButtonPanel: TPanel
    Left = 0
    Top = 272
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
  object WizardPC: TPageControl
    Left = 0
    Top = 40
    Width = 372
    Height = 232
    ActivePage = tabsKeys
    Align = alClient
    TabOrder = 2
    object tabsMasterDV: TTabSheet
      Caption = '@MasterDVPage'
      TabVisible = False
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 224
        Align = alClient
        BorderWidth = 5
        TabOrder = 0
        object MasterDVLB: TListBox
          Left = 6
          Top = 6
          Width = 352
          Height = 212
          Align = alClient
          ItemHeight = 14
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
      end
    end
    object tabsDetailDV: TTabSheet
      Caption = '@DetailDVPage'
      TabVisible = False
      object DetailDVLB: TListBox
        Left = 0
        Top = 0
        Width = 364
        Height = 224
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
      end
    end
    object tabsMasterFields: TTabSheet
      Caption = '@MasterFieldsPage'
      TabVisible = False
      object MasterFieldsCBL: TCheckListBox
        Left = 0
        Top = 37
        Width = 364
        Height = 187
        OnClickCheck = MasterFieldsCBLClick
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = MasterFieldsCBLClick
      end
      object MasterFieldsPnl: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 37
        Align = alTop
        Alignment = taLeftJustify
        BevelOuter = bvNone
        BorderWidth = 5
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
          OnClick = AllBtnClick
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
          OnClick = NoneBtnClick
        end
      end
    end
    object tabsSortMasterFields: TTabSheet
      Caption = '@SortMasterFieldsPage'
      TabVisible = False
      object Panel12: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 31
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object MasterMoveUpSB: TSpeedButton
          Left = 9
          Top = 0
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
          OnClick = MasterMoveUpSBClick
        end
        object MasterMoveDownSB: TSpeedButton
          Left = 37
          Top = 0
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
          OnClick = MasterMoveDownSBClick
        end
      end
      object SortMasterFieldsLB: TListBox
        Left = 0
        Top = 31
        Width = 364
        Height = 193
        Align = alClient
        ItemHeight = 14
        TabOrder = 1
        OnClick = SortMasterFieldsLBClick
      end
    end
    object tabsDetailFields: TTabSheet
      Caption = '@DetailFieldsPage'
      TabVisible = False
      object DetailFieldsCBL: TCheckListBox
        Left = 0
        Top = 37
        Width = 364
        Height = 187
        OnClickCheck = MasterFieldsCBLClick
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = MasterFieldsCBLClick
      end
      object DetailFieldsPnl: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 37
        Align = alTop
        Alignment = taLeftJustify
        BevelOuter = bvNone
        BorderWidth = 5
        TabOrder = 1
        object AllBtn: TBitBtn
          Left = 216
          Top = 3
          Width = 65
          Height = 26
          Hint = 'Select All Fields'
          Caption = '&All'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          OnClick = AllBtnClick
        end
        object NoneBtn: TBitBtn
          Left = 286
          Top = 3
          Width = 65
          Height = 26
          Hint = 'Clear All Selected Items'
          Caption = '&None'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          OnClick = NoneBtnClick
        end
      end
    end
    object tabsSortDetailFields: TTabSheet
      Caption = '@SortDetailFieldsPage'
      TabVisible = False
      object Panel16: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 31
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object DetailMoveUpSB: TSpeedButton
          Left = 9
          Top = 0
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
          OnClick = DetailMoveUpSBClick
        end
        object DetailMoveDownSB: TSpeedButton
          Left = 37
          Top = 0
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
          OnClick = DetailMoveDownSBClick
        end
      end
      object SortDetailFieldsLB: TListBox
        Left = 0
        Top = 31
        Width = 364
        Height = 193
        Align = alClient
        ItemHeight = 14
        TabOrder = 1
        OnClick = SortDetailFieldsLBClick
      end
    end
    object tabsKeys: TTabSheet
      Caption = '@KeysPage'
      TabVisible = False
      object DetailKeyCBL: TCheckListBox
        Left = 180
        Top = 37
        Width = 184
        Height = 185
        OnClickCheck = MasterFieldsCBLClick
        Align = alClient
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = MasterFieldsCBLClick
      end
      object Panel14: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 37
        Align = alTop
        Alignment = taLeftJustify
        BevelOuter = bvNone
        TabOrder = 1
        object DetailKeyPanel: TPanel
          Left = 180
          Top = 0
          Width = 184
          Height = 37
          Align = alClient
          BevelInner = bvRaised
          BevelOuter = bvNone
          Caption = 'Detail Keys Selected: %s'
          TabOrder = 0
        end
        object MasterKeyPanel: TPanel
          Left = 0
          Top = 0
          Width = 180
          Height = 37
          Align = alLeft
          BevelInner = bvRaised
          BevelOuter = bvNone
          Caption = 'Master Keys Selected: %s'
          TabOrder = 1
        end
      end
      object MasterKeyCBL: TCheckListBox
        Left = 0
        Top = 37
        Width = 180
        Height = 185
        OnClickCheck = MasterFieldsCBLClick
        Align = alLeft
        ItemHeight = 14
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = MasterFieldsCBLClick
      end
    end
    object tabsSortKeys: TTabSheet
      Caption = '@SortKeysPage'
      TabVisible = False
      object Panel18: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 31
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          364
          31)
        object MasterKeyUpSB: TSpeedButton
          Left = 9
          Top = 0
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
          OnClick = MasterKeyUpSBClick
        end
        object MasterKeyDownSB: TSpeedButton
          Left = 37
          Top = 0
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
          OnClick = MasterKeyDownSBClick
        end
        object DetailKeyUpSB: TSpeedButton
          Left = 189
          Top = 0
          Width = 25
          Height = 25
          Hint = 'Move the Selected Item Up One Position'
          Anchors = [akTop, akRight]
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
          OnClick = DetailKeyUpSBClick
        end
        object DetailKeyDownSB: TSpeedButton
          Left = 217
          Top = 0
          Width = 25
          Height = 25
          Hint = 'Move The Selected Item Down One Position'
          Anchors = [akTop, akRight]
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
          OnClick = DetailKeyDownSBClick
        end
      end
      object DetailKeySortLB: TListBox
        Left = 180
        Top = 31
        Width = 184
        Height = 191
        Align = alClient
        ItemHeight = 14
        TabOrder = 1
        OnClick = SortDetailFieldsLBClick
      end
      object MasterKeySortLB: TListBox
        Left = 0
        Top = 31
        Width = 180
        Height = 191
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        ItemHeight = 14
        TabOrder = 2
        OnClick = SortDetailFieldsLBClick
      end
    end
    object tabsOptions: TTabSheet
      Caption = '@OptionsPage'
      TabVisible = False
      object Label1: TLabel
        Left = 26
        Top = 92
        Width = 58
        Height = 14
        Caption = 'Detail Indent'
      end
      object GroupBox1: TGroupBox
        Left = 16
        Top = 114
        Width = 337
        Height = 110
        Caption = 'Report Margins'
        TabOrder = 0
        object Label2: TLabel
          Left = 31
          Top = 18
          Width = 19
          Height = 14
          Caption = 'Left'
        end
        object Label3: TLabel
          Left = 31
          Top = 40
          Width = 18
          Height = 14
          Caption = 'Top'
        end
        object Label4: TLabel
          Left = 31
          Top = 62
          Width = 24
          Height = 14
          Caption = 'Right'
        end
        object Label5: TLabel
          Left = 31
          Top = 84
          Width = 33
          Height = 14
          Caption = 'Bottom'
        end
        object LeftMarginEdit: TEdit
          Left = 201
          Top = 14
          Width = 121
          Height = 22
          Hint = 'Type the position of the left margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          Text = '1.0'
        end
        object TopMarginEdit: TEdit
          Left = 201
          Top = 36
          Width = 121
          Height = 22
          Hint = 'Type the position of the top margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          Text = '1.0'
        end
        object RightMarginEdit: TEdit
          Left = 201
          Top = 58
          Width = 121
          Height = 22
          Hint = 'Type the position of the right margin'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
          Text = '1.0'
        end
        object BottomMarginEdit: TEdit
          Left = 201
          Top = 80
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
          Width = 292
          Height = 22
          Hint = 'Type the Title You Want For This Report'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          Text = 'Master/Detail Report'
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
      object DetailOffsetEdit: TEdit
        Left = 217
        Top = 87
        Width = 121
        Height = 22
        TabOrder = 2
        Text = '0.5'
      end
    end
    object tabsFonts: TTabSheet
      Caption = '@FontPage'
      TabVisible = False
      object TitlePanel: TPanel
        Left = 0
        Top = 30
        Width = 364
        Height = 54
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
        TabOrder = 0
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
        TabOrder = 1
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
      object CaptionPanel: TPanel
        Left = 0
        Top = 114
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
        TabOrder = 2
      end
      object Panel8: TPanel
        Left = 0
        Top = 84
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
        TabOrder = 3
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
      object BodyPanel: TPanel
        Left = 0
        Top = 184
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
        TabOrder = 4
      end
      object Panel10: TPanel
        Left = 0
        Top = 154
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
        TabOrder = 5
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
