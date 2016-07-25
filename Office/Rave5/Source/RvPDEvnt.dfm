object SourceEditorForm: TSourceEditorForm
  Left = 290
  Top = 133
  Width = 667
  Height = 583
  ActiveControl = DefinedLB
  Caption = '@SourceEditorForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Pitch = fpVariable
  Font.Style = []
  OldCreateOrder = True
  ShowHint = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object SourceEditorPanel: TPanel
    Left = 0
    Top = 0
    Width = 659
    Height = 556
    Align = alClient
    BevelOuter = bvNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Pitch = fpVariable
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Splitter2: TSplitter
      Left = 0
      Top = 160
      Width = 659
      Height = 3
      Cursor = crVSplit
      Align = alTop
    end
    object Panel3: TPanel
      Left = 0
      Top = 0
      Width = 659
      Height = 160
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object Splitter1: TSplitter
        Left = 185
        Top = 0
        Height = 160
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 160
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object DefinedLB: TListBox
          Left = 0
          Top = 57
          Width = 185
          Height = 103
          Hint = 'Defined Events'
          Align = alClient
          ItemHeight = 14
          Sorted = True
          TabOrder = 0
          OnClick = DefinedLBClick
        end
        object Panel4: TPanel
          Left = 0
          Top = 0
          Width = 185
          Height = 57
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          DesignSize = (
            185
            57)
          object Label1: TLabel
            Left = 3
            Top = 1
            Width = 80
            Height = 14
            Caption = 'Available Events'
          end
          object Label2: TLabel
            Left = 3
            Top = 41
            Width = 73
            Height = 14
            Caption = 'Defined Events'
          end
          object AvailableCB: TComboBox
            Left = 1
            Top = 16
            Width = 185
            Height = 22
            Hint = 'Available Events'
            Style = csDropDownList
            Anchors = [akLeft, akTop, akRight]
            ItemHeight = 14
            Sorted = True
            TabOrder = 0
            OnClick = AvailableCBClick
          end
        end
      end
      object EventDesc: TRichEdit
        Left = 188
        Top = 0
        Width = 471
        Height = 160
        Align = alClient
        Color = clInfoBk
        TabOrder = 1
      end
    end
    object Panel2: TPanel
      Left = 0
      Top = 163
      Width = 659
      Height = 31
      Align = alTop
      TabOrder = 1
      DesignSize = (
        659
        31)
      object btnShowSelectedEvents: TSpeedButton
        Left = 91
        Top = 2
        Width = 28
        Height = 28
        Hint = 'Show Events for Selected'
        GroupIndex = 1
        Down = True
        Glyph.Data = {
          F6000000424DF600000000000000760000002800000010000000100000000100
          0400000000008000000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFF222FFFFF
          FFFFFFF22222FFFFFFFFFFF22222FFFFFFFFFFF222222FFFFFFFFF2222222FFF
          FFFFF222222222FFFFFFA2222A22222FFFFFA222AFA22222FFFFFAAAFFFA2222
          FFFFFFFFFFFFA2222FFFFFFFFFFFA222222FFFFFFFFFFA222222FFFFFFFFFFA2
          2222FFFFFFFFFFFA2222FFFFFFFFFFFFA222FFFFFFFFFFFFFAAA}
        OnClick = btnShowSelectedEventsClick
      end
      object btnShowPageEvents: TSpeedButton
        Left = 118
        Top = 2
        Width = 28
        Height = 28
        Hint = 'Show All Events on Page'
        GroupIndex = 1
        Glyph.Data = {
          96010000424D9601000000000000760000002800000018000000180000000100
          0400000000002001000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          7777777777777777777777777777777777777777777777777777777777777777
          70000000000000077777777770FFFFFFFFFFFF077777777770F000F0F0000F07
          7777777770FFFFFFFFFFFF077777777770F000F0F0000F077777777770FFFFFF
          FFFFFF077777777770F000F0F0000F077777777770FFFFFFFFFFFF0777777777
          70F000F0F0000F077777777770FFFFFFFFFFFF077777777770F000F0F7777F07
          7777777770FFFFFF000000077777777770F000F00FFFF0077777777770FFFFFF
          0FFF00777777777770F000F00FF007777777777770FFFFFF0F00777777777777
          70F88FF800077777777777777000000000777777777777777777777777777777
          7777777777777777777777777777777777777777777777777777}
        OnClick = btnShowPageEventsClick
      end
      object btnDeleteEvent: TSpeedButton
        Left = 160
        Top = 2
        Width = 28
        Height = 28
        Hint = 'Delete current event'
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333000000000
          3333333777777777F3333330F777777033333337F3F3F3F7F3333330F0808070
          33333337F7F7F7F7F3333330F080707033333337F7F7F7F7F3333330F0808070
          33333337F7F7F7F7F3333330F080707033333337F7F7F7F7F3333330F0808070
          333333F7F7F7F7F7F3F33030F080707030333737F7F7F7F7F7333300F0808070
          03333377F7F7F7F773333330F080707033333337F7F7F7F7F333333070707070
          33333337F7F7F7F7FF3333000000000003333377777777777F33330F88877777
          0333337FFFFFFFFF7F3333000000000003333377777777777333333330777033
          3333333337FFF7F3333333333000003333333333377777333333}
        NumGlyphs = 2
        OnClick = btnDeleteEventClick
      end
      object StatusLabel: TLabel
        Left = 646
        Top = 8
        Width = 1
        Height = 14
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
      end
      object butnCompile: TButton
        Left = 3
        Top = 2
        Width = 74
        Height = 27
        Caption = 'Compile'
        TabOrder = 0
        OnClick = butnCompileClick
      end
    end
    object sboxEditor: TScrollBox
      Left = 0
      Top = 194
      Width = 659
      Height = 362
      VertScrollBar.Tracking = True
      Align = alClient
      TabOrder = 2
    end
  end
end
