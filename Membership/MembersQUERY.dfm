inherited frmMembersQUERY: TfrmMembersQUERY
  Left = 57
  Top = 45
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'frmMembersQUERY'
  ClientHeight = 439
  ClientWidth = 604
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 419
    Width = 604
  end
  inherited ControlBar1: TControlBar
    Width = 604
    inherited ToolBar: TToolBar
      Width = 587
      ButtonWidth = 42
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object ToolButton2: TToolButton
        Left = 3
        Top = 0
        Hint = 'Execute the Query'
        Caption = 'ToolButton2'
        ImageIndex = 24
        OnClick = menuExecuteQueryClick
      end
      object dbnQry: TwwDBNavigator
        Left = 45
        Top = 0
        Width = 168
        Height = 42
        AutosizeStyle = asNone
        DataSource = dm.dsrQuery
        ImageList = dm.ImageListLarge
        ShowHint = True
        RepeatInterval.InitialDelay = 500
        RepeatInterval.Interval = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentShowHint = False
        object dbnQryFirst: TwwNavButton
          Left = 0
          Top = 0
          Width = 42
          Height = 42
          Hint = 'Move to the First record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryFirst'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 0
          Style = nbsFirst
        end
        object dbnQryPrior: TwwNavButton
          Left = 42
          Top = 0
          Width = 42
          Height = 42
          Hint = 'Move to the Prior record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryPrior'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 1
          Style = nbsPrior
        end
        object dbnQryNext: TwwNavButton
          Left = 84
          Top = 0
          Width = 42
          Height = 42
          Hint = 'Move to the Next record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnQryNext'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 2
          Style = nbsNext
        end
        object dbnLast: TwwNavButton
          Left = 126
          Top = 0
          Width = 42
          Height = 42
          Hint = 'Move to the Llast record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLast'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 3
          Style = nbsLast
        end
      end
      object btnPrintSQL: TToolButton
        Left = 213
        Top = 0
        Hint = 'Print the SQL Statement'
        Caption = 'btnPrintSQL'
        ImageIndex = 12
        OnClick = menuPrinttheSQLStatementClick
      end
      object btnPrinterSetup: TToolButton
        Left = 255
        Top = 0
        Hint = 'Check the selected Printer'
        Caption = 'btnPrinterSetup'
        ImageIndex = 18
        OnClick = menuPrinterSetupClick
      end
      object btnQueries: TToolButton
        Left = 297
        Top = 0
        Hint = 'Add Edit or Delete Queries'
        Caption = 'btnQueries'
        ImageIndex = 9
        OnClick = menuSaveSQLDataBaseClick
      end
      object btnExportExcel: TToolButton
        Left = 339
        Top = 0
        Hint = 'Execute the Query and Export to Excel'
        Caption = 'btnExportExcel'
        ImageIndex = 16
        OnClick = btnExportExcelClick
      end
      object pnlSpace: TPanel
        Left = 381
        Top = 0
        Width = 112
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
      end
      object btnCancel: TToolButton
        Left = 493
        Top = 0
        Caption = 'btnCancel'
        ImageIndex = 7
        OnClick = menuExitClick
      end
      object btnOK: TToolButton
        Left = 535
        Top = 0
        Caption = 'btnOK'
        ImageIndex = 8
        OnClick = btnOKClick
      end
    end
  end
  object Panel1: TPanel [2]
    Left = 0
    Top = 60
    Width = 185
    Height = 359
    Align = alLeft
    TabOrder = 2
    object Panel10: TPanel
      Left = 1
      Top = 1
      Width = 183
      Height = 166
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object Label1: TLabel
        Left = 58
        Top = 12
        Width = 69
        Height = 13
        Caption = 'Step 1. Tables'
      end
      object lbxTableNames: TListBox
        Left = 5
        Top = 30
        Width = 173
        Height = 132
        ItemHeight = 13
        TabOrder = 0
        OnClick = lbxTableNamesClick
        OnStartDrag = lbxTableNamesStartDrag
      end
    end
    object Panel11: TPanel
      Left = 1
      Top = 167
      Width = 183
      Height = 191
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 5
      TabOrder = 1
      object Label2: TLabel
        Left = 5
        Top = 5
        Width = 173
        Height = 13
        Align = alTop
        Alignment = taCenter
        Caption = 'Step 2.  Fields'
      end
      object lbxFieldNames: TListBox
        Left = 5
        Top = 18
        Width = 173
        Height = 168
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = lbxFieldNamesClick
        OnStartDrag = lbxTableNamesStartDrag
      end
    end
  end
  object Panel2: TPanel [3]
    Left = 185
    Top = 60
    Width = 419
    Height = 359
    Align = alClient
    BevelOuter = bvNone
    Caption = 'Panel2'
    TabOrder = 3
    object PageControl: TPageControl
      Left = 0
      Top = 0
      Width = 419
      Height = 359
      ActivePage = tsBuilder
      Align = alClient
      TabOrder = 0
      OnChange = PageControlChange
      object tsBuilder: TTabSheet
        Caption = 'Query Builder'
        object Panel3: TPanel
          Left = 0
          Top = 0
          Width = 411
          Height = 145
          Align = alTop
          BevelOuter = bvNone
          BorderWidth = 5
          Caption = 'Panel3'
          TabOrder = 0
          object Label6: TLabel
            Left = 240
            Top = 15
            Width = 23
            Height = 13
            Caption = 'From'
          end
          object PageControl1: TPageControl
            Left = 5
            Top = 5
            Width = 401
            Height = 135
            ActivePage = tsMatching
            Align = alClient
            TabOrder = 0
            OnChange = PageControl1Change
            object tsMatching: TTabSheet
              Caption = 'Step 3.  Matching'
              object Label3: TLabel
                Left = 83
                Top = 52
                Width = 21
                Height = 13
                Caption = 'Text'
              end
              object lblMatching: TLabel
                Left = 0
                Top = 94
                Width = 393
                Height = 13
                Align = alBottom
                Alignment = taCenter
                Caption = 'Text'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edtMatching: TEdit
                Left = 85
                Top = 69
                Width = 188
                Height = 21
                TabOrder = 0
              end
              object rgpOther: TLMDRadioGroup
                Left = -10
                Top = 11
                Width = 404
                Height = 36
                Alignment = gcaLeftCenter
                Bevel.BorderSides = []
                Bevel.EdgeStyle = etBump
                Bevel.Mode = bmStandard
                Bevel.StandardStyle = lsNone
                CaptionFont.Charset = DEFAULT_CHARSET
                CaptionFont.Color = clBlack
                CaptionFont.Height = -11
                CaptionFont.Name = 'Arial'
                CaptionFont.Style = []
                CaptionOffset = 0
                CaptionParentFont = False
                Columns = 3
                Items.Strings = (
                  'Exact Match'
                  'Partial Match'
                  'Is Not'
                  'Is Blank'
                  'Is Not Blank')
                ItemOffset = 0
                TabOrder = 1
                OnChange = rgpOtherChange
                ItemIndex = 0
              end
            end
            object tsRange: TTabSheet
              Caption = 'Range'
              ImageIndex = 1
              object Label5: TLabel
                Left = 91
                Top = 19
                Width = 23
                Height = 13
                Caption = 'From'
              end
              object Label4: TLabel
                Left = 94
                Top = 54
                Width = 13
                Height = 13
                Caption = 'To'
              end
              object lblRange: TLabel
                Left = 0
                Top = 94
                Width = 393
                Height = 13
                Align = alBottom
                Alignment = taCenter
                Caption = 'Text'
              end
              object edtFrom: TEdit
                Left = 147
                Top = 15
                Width = 96
                Height = 21
                TabOrder = 0
                OnKeyPress = edtFromKeyPress
              end
              object edtTo: TEdit
                Left = 147
                Top = 50
                Width = 96
                Height = 21
                TabOrder = 1
                OnKeyPress = edtFromKeyPress
              end
            end
          end
        end
        object Panel6: TPanel
          Left = 0
          Top = 268
          Width = 411
          Height = 63
          Align = alBottom
          BevelOuter = bvNone
          BorderWidth = 5
          Caption = 'Panel6'
          TabOrder = 1
          object GroupBox3: TGroupBox
            Left = 5
            Top = 5
            Width = 401
            Height = 53
            Align = alClient
            Caption = ' Step 5.  Sorting '
            TabOrder = 0
            object cbxFieldNames: TComboBox
              Left = 111
              Top = 20
              Width = 137
              Height = 21
              Enabled = False
              ItemHeight = 13
              TabOrder = 0
            end
            object cbxUseSorting: TCheckBox
              Left = 25
              Top = 22
              Width = 86
              Height = 17
              Caption = 'Use Sorting'
              TabOrder = 1
              OnClick = cbxUseSortingClick
            end
            object cbxAscending: TComboBox
              Left = 269
              Top = 19
              Width = 103
              Height = 21
              DropDownCount = 4
              Enabled = False
              ItemHeight = 13
              TabOrder = 2
              Items.Strings = (
                'Ascending'
                'Descending')
            end
          end
        end
        object Panel7: TPanel
          Left = 0
          Top = 145
          Width = 409
          Height = 123
          BevelOuter = bvNone
          BorderWidth = 5
          Caption = 'Panel7'
          TabOrder = 2
          object GroupBox4: TGroupBox
            Left = 5
            Top = 5
            Width = 400
            Height = 113
            Caption = ' Step 4. Selection Criteria'
            TabOrder = 0
            object Panel8: TPanel
              Left = 274
              Top = 15
              Width = 124
              Height = 96
              Align = alRight
              BevelOuter = bvNone
              BorderWidth = 2
              TabOrder = 0
              object btnAdd: TButton
                Left = 24
                Top = 6
                Width = 87
                Height = 25
                Caption = 'Add'
                TabOrder = 0
                OnClick = btnAddClick
              end
              object btnDelete: TButton
                Left = 24
                Top = 34
                Width = 87
                Height = 26
                Caption = 'Delete'
                TabOrder = 1
                OnClick = btnDeleteClick
              end
              object btnClearAll: TButton
                Left = 24
                Top = 63
                Width = 87
                Height = 25
                Caption = 'Clear All'
                TabOrder = 2
                OnClick = btnClearAllClick
              end
            end
            object Panel9: TPanel
              Left = 2
              Top = 15
              Width = 272
              Height = 96
              Align = alClient
              BevelOuter = bvNone
              BorderWidth = 8
              Caption = 'Panel9'
              TabOrder = 1
              object lbxQueryLines: TListBox
                Left = 8
                Top = 8
                Width = 256
                Height = 80
                Align = alClient
                ItemHeight = 13
                TabOrder = 0
              end
            end
          end
        end
      end
      object tsResults: TTabSheet
        Caption = 'Query Results'
        ImageIndex = 2
        object dbgQueryResults: TwwDBGrid
          Left = 0
          Top = 0
          Width = 411
          Height = 331
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          DataSource = dm.dsrQuery
          KeyOptions = []
          ReadOnly = True
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          IndicatorColor = icBlack
        end
      end
      object tsSQL: TTabSheet
        Caption = 'SQL'
        ImageIndex = 1
        object memoQuery: TRichEdit
          Left = 0
          Top = 0
          Width = 411
          Height = 331
          Align = alClient
          TabOrder = 0
          OnDragDrop = memoQueryDragDrop
          OnDragOver = memoQueryDragOver
          OnKeyPress = memoQueryKeyPress
        end
      end
      object tsTableData: TTabSheet
        Caption = 'Table Data'
        ImageIndex = 3
        object wwDBGrid2: TwwDBGrid
          Left = 0
          Top = 0
          Width = 411
          Height = 331
          IniAttributes.Delimiter = ';;'
          TitleColor = clBtnFace
          FixedCols = 0
          ShowHorzScrollBar = True
          Align = alClient
          DataSource = dm.dsrQuery
          KeyOptions = []
          ReadOnly = True
          TabOrder = 0
          TitleAlignment = taLeftJustify
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          TitleLines = 1
          TitleButtons = False
          IndicatorColor = icBlack
        end
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 410
    Top = 18
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuOptions: TMenuItem [1]
      Caption = '&Options'
      object menuExecuteQuery: TMenuItem
        Caption = '&Execute Query'
        ShortCut = 16453
        OnClick = menuExecuteQueryClick
      end
      object menub: TMenuItem
        Caption = '-'
      end
      object menuPrinterSetup: TMenuItem
        Caption = '&Printer Setup'
        OnClick = menuPrinterSetupClick
      end
      object menuPrinttheSQLStatement: TMenuItem
        Caption = 'Print the SQL Statement'
        ShortCut = 16464
        OnClick = menuPrinttheSQLStatementClick
      end
      object menubb: TMenuItem
        Caption = '-'
      end
      object menuLoadanSQLScript: TMenuItem
        Caption = '&Load an SQL Script'
        OnClick = menuLoadanSQLScriptClick
      end
      object menuSaveSQLScript: TMenuItem
        Caption = 'Save an SQL Script'
        OnClick = menuSaveSQLScriptClick
      end
      object menuSaveSQLDataBase: TMenuItem
        Caption = 'Save an SQL Script ( DataBase )'
        OnClick = menuSaveSQLDataBaseClick
      end
      object menuPaste: TMenuItem
        Caption = 'Paste'
        ShortCut = 116
        OnClick = menuPasteClick
      end
    end
  end
end
