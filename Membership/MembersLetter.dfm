inherited frmMembersLetter: TfrmMembersLetter
  Tag = 2
  Left = 58
  Top = 91
  Caption = 'frmMembersLetter'
  ClientHeight = 434
  ClientWidth = 648
  Font.Color = clBlack
  OldCreateOrder = True
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited stbStatus: TStatusBar
    Top = 414
    Width = 648
  end
  object Panel5: TPanel [1]
    Left = 0
    Top = 60
    Width = 648
    Height = 354
    Align = alClient
    BevelOuter = bvNone
    BevelWidth = 2
    BorderWidth = 5
    TabOrder = 2
    object dbreLetter: TwwDBRichEdit
      Left = 5
      Top = 52
      Width = 638
      Height = 297
      Hint = 'Right click here to Edit the Letter - change fonts, sizes etc'
      ScrollBars = ssVertical
      Align = alClient
      AutoURLDetect = False
      Color = clWhite
      DataField = 'LETTER'
      DataSource = dm.dsrLetters
      DragCursor = crHandPoint
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Frame.Transparent = True
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameEtched
      Frame.NonFocusTransparentFontColor = clBlack
      ParentFont = False
      PrintJobName = 'Delphi 5'
      TabOrder = 0
      OnDragDrop = dbreLetterDragDrop
      OnDragOver = dbreLetterDragOver
      OnEndDrag = dbreLetterEndDrag
      OnEnter = dbreLetterEnter
      PopupOptions = [rpoPopupEdit, rpoPopupCut, rpoPopupCopy, rpoPopupPaste, rpoPopupFont, rpoPopupBullet, rpoPopupParagraph, rpoPopupFind, rpoPopupReplace, rpoPopupInsertObject, rpoPopupMSWordSpellCheck]
      EditorCaption = 'Edit Rich Text'
      EditorPosition.Left = 0
      EditorPosition.Top = 0
      EditorPosition.Width = 0
      EditorPosition.Height = 0
      MeasurementUnits = muInches
      PrintMargins.Top = 1
      PrintMargins.Bottom = 1
      PrintMargins.Left = 1
      PrintMargins.Right = 1
      OnInitDialog = dbreLetterInitDialog
      OnCloseDialog = dbreLetterCloseDialog
      RichEditVersion = 2
      Data = {
        A40000007B5C727466315C616E73695C616E7369637067313235325C64656666
        305C6465666C616E67313033337B5C666F6E7474626C7B5C66305C666E696C20
        4D532053616E732053657269663B7D7D0D0A7B5C636F6C6F7274626C203B5C72
        6564305C677265656E305C626C7565303B7D0D0A5C766965776B696E64345C75
        63315C706172645C6366315C66305C6673313620646272654C65747465725C70
        61720D0A7D0D0A00}
    end
    object Panel1: TPanel
      Left = 5
      Top = 5
      Width = 638
      Height = 47
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object lblFields: TLabel
        Left = 303
        Top = 13
        Width = 27
        Height = 13
        Caption = 'Fields'
      end
      object lblLetter: TLabel
        Left = 17
        Top = 12
        Width = 58
        Height = 13
        Caption = 'Letter Name'
      end
      object dbeNAme: TwwDBEdit
        Left = 83
        Top = 10
        Width = 155
        Height = 19
        BorderStyle = bsNone
        Color = clWhite
        DataField = 'NAME'
        DataSource = dm.dsrLetters
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ParentFont = False
        TabOrder = 0
        UnboundDataType = wwDefault
        WantReturns = False
        WordWrap = False
      end
      object dbeComments: TwwDBEdit
        Left = 366
        Top = 11
        Width = 258
        Height = 19
        BorderStyle = bsNone
        Color = clWhite
        DataField = 'COMMENTS'
        DataSource = dm.dsrQueries
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Frame.Enabled = True
        Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
        Frame.FocusStyle = efsFrameSunken
        Frame.NonFocusStyle = efsFrameSunken
        ParentFont = False
        TabOrder = 1
        UnboundDataType = wwDefault
        Visible = False
        WantReturns = False
        WordWrap = False
      end
      object cbxFields: TComboBox
        Left = 352
        Top = 11
        Width = 249
        Height = 21
        Hint = 'Drag and Drop fields onto the Letter'
        DragMode = dmAutomatic
        DropDownCount = 20
        ItemHeight = 13
        TabOrder = 2
        Text = 'cbxFields'
        OnStartDrag = cbxFieldsStartDrag
      end
    end
  end
  inherited ControlBar1: TControlBar
    Width = 648
    inherited ToolBar: TToolBar
      Width = 631
      Indent = 1
      inherited pnlFill: TPanel
        Left = 1
        TabOrder = 1
      end
      object dbnLinks: TwwDBNavigator
        Left = 2
        Top = 0
        Width = 247
        Height = 42
        AutosizeStyle = asNone
        DataSource = dm.dsrLetters
        ShowHint = True
        RepeatInterval.InitialDelay = 500
        RepeatInterval.Interval = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentShowHint = False
        object dbnLinksInsert: TwwNavButton
          Left = 0
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Insert new record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksInsert'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnLinksInsertClick
          Index = 0
          Style = nbsInsert
        end
        object dbnLinksEdit: TwwNavButton
          Left = 40
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Edit current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksEdit'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 1
          Style = nbsEdit
        end
        object dbnLinksDelete: TwwNavButton
          Left = 80
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Delete current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksDelete'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          OnClick = dbnLinksDeleteClick
          Index = 2
          Style = nbsDelete
        end
        object dbnLinksPost: TwwNavButton
          Left = 120
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Post changes of current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksPost'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 3
          Style = nbsPost
        end
        object dbnLinksCancel: TwwNavButton
          Left = 160
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Cancel changes made to current record'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksCancel'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 4
          Style = nbsCancel
        end
        object dbnLinksRefresh: TwwNavButton
          Left = 200
          Top = 0
          Width = 40
          Height = 40
          Hint = 'Refresh the contents of the dataset'
          ImageIndex = -1
          NumGlyphs = 2
          Spacing = 4
          Transparent = False
          Caption = 'dbnLinksRefresh'
          Enabled = False
          DisabledTextColors.ShadeColor = clGray
          DisabledTextColors.HighlightColor = clBtnHighlight
          Index = 5
          Style = nbsRefresh
        end
      end
      object pnlSpace: TPanel
        Left = 249
        Top = 0
        Width = 334
        Height = 42
        BevelOuter = bvNone
        TabOrder = 0
        object Panel3: TPanel
          Left = 2
          Top = 0
          Width = 47
          Height = 42
          BevelOuter = bvNone
          Caption = '&Find'
          TabOrder = 0
        end
        object dbcbLetters: TwwDBComboBox
          Left = 54
          Top = 11
          Width = 155
          Height = 19
          ShowButton = True
          Style = csDropDown
          MapList = False
          AllowClearKey = False
          AutoDropDown = True
          ShowMatchText = True
          BorderStyle = bsNone
          Color = clWhite
          DropDownCount = 8
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Frame.Enabled = True
          Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
          Frame.FocusStyle = efsFrameSunken
          Frame.NonFocusStyle = efsFrameSunken
          ItemHeight = 0
          ParentFont = False
          Sorted = False
          TabOrder = 1
          UnboundDataType = wwDefault
          OnChange = dbcbLettersChange
          OnExit = dbcbLettersExit
        end
      end
      object ToolButton1: TToolButton
        Left = 583
        Top = 0
        Caption = 'ToolButton1'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  inherited MainMenu: TMainMenu
    Left = 610
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
  end
end
