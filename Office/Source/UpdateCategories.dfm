object frmUpdateCategories: TfrmUpdateCategories
  Left = 159
  Top = 217
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Update Categories'
  ClientHeight = 451
  ClientWidth = 482
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl2: TPageControl
    Left = 8
    Top = 8
    Width = 465
    Height = 393
    ActivePage = tsSelectGroups
    TabOrder = 0
    object tsSelectGroups: TTabSheet
      Caption = 'tsSelectGroups'
      TabVisible = False
      object Label9: TLabel
        Left = 4
        Top = 8
        Width = 261
        Height = 41
        AutoSize = False
        Caption = 
          'Select the categories you DON'#39'T wish to use AND those which repr' +
          'esent category groups.'
        WordWrap = True
      end
      object Label10: TLabel
        Left = 8
        Top = 48
        Width = 162
        Height = 13
        Caption = 'e.g. Total, Beverage, Food, Other.'
      end
      object chlbOldCategories: TCheckListBox
        Left = 8
        Top = 72
        Width = 201
        Height = 273
        Color = clInfoBk
        ItemHeight = 13
        TabOrder = 0
      end
      object btnNext1: TButton
        Left = 372
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Next'
        TabOrder = 1
        OnClick = btnNext1Click
      end
    end
    object tsSelectCategories: TTabSheet
      Caption = 'tsSelectCategories'
      ImageIndex = 1
      TabVisible = False
      object Label3: TLabel
        Left = 8
        Top = 8
        Width = 233
        Height = 13
        Caption = 'Drag the categories in to their appropriate groups.'
      end
      object Label4: TLabel
        Left = 8
        Top = 32
        Width = 441
        Height = 33
        AutoSize = False
        Caption = 
          'The order of the categories is the order of importance so that i' +
          'f an item has more than one category, the upper most one will be' +
          ' used.'
        WordWrap = True
      end
      object Label8: TLabel
        Left = 8
        Top = 72
        Width = 311
        Height = 13
        Caption = 'Leave the categories you wish to become '#39'Breakdown Categories'#39
      end
      object btnNext2: TButton
        Left = 372
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Next'
        TabOrder = 0
        OnClick = btnNext2Click
      end
      object tvCategories: TTreeView
        Left = 8
        Top = 88
        Width = 201
        Height = 257
        Color = clInfoBk
        DragMode = dmAutomatic
        Images = dmUtilities.imlTreeImages
        Indent = 19
        MultiSelect = True
        MultiSelectStyle = [msControlSelect, msShiftSelect]
        ReadOnly = True
        TabOrder = 1
        OnDragDrop = tvCategoriesDragDrop
        OnDragOver = tvCategoriesDragOver
      end
      object tvGroupedCategories: TTreeView
        Left = 244
        Top = 88
        Width = 201
        Height = 225
        Color = clInfoBk
        Images = dmUtilities.imlTreeImages
        Indent = 19
        ReadOnly = True
        TabOrder = 2
        OnDragDrop = tvGroupedCategoriesDragDrop
        OnDragOver = tvGroupedCategoriesDragOver
        OnMouseDown = tvGroupedCategoriesMouseDown
      end
      object Button2: TButton
        Left = 244
        Top = 320
        Width = 75
        Height = 25
        Caption = 'Add Group'
        TabOrder = 3
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 324
        Top = 320
        Width = 75
        Height = 25
        Caption = 'Delete Group'
        TabOrder = 4
        OnClick = Button3Click
      end
      object Button4: TButton
        Left = 8
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Back'
        TabOrder = 5
        OnClick = Button4Click
      end
    end
    object tsRename: TTabSheet
      Caption = 'tsRename'
      ImageIndex = 2
      TabVisible = False
      object Label5: TLabel
        Left = 8
        Top = 8
        Width = 75
        Height = 13
        Caption = 'Rename Things'
      end
      object vtvRename: TVirtualStringTree
        Left = 8
        Top = 72
        Width = 437
        Height = 273
        BevelInner = bvNone
        BevelOuter = bvNone
        Color = clInfoBk
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Header.AutoSizeIndex = 1
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Tahoma'
        Header.Font.Style = []
        Header.Options = [hoAutoResize, hoVisible]
        Images = dmUtilities.imlTreeImages
        ParentFont = False
        TabOrder = 0
        TreeOptions.MiscOptions = [toEditable, toGridExtensions, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowButtons, toShowRoot, toShowTreeLines, toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnAfterPaint = vtvRenameAfterPaint
        OnBeforePaint = vtvRenameBeforePaint
        OnColumnDblClick = vtvRenameColumnDblClick
        OnCreateEditor = vtvRenameCreateEditor
        OnEdited = vtvRenameEdited
        OnEditing = vtvRenameEditing
        OnEnter = vtvRenameEnter
        OnExit = vtvRenameExit
        OnFocusChanged = vtvRenameFocusChanged
        OnFocusChanging = vtvRenameFocusChanging
        OnGetText = vtvRenameGetText
        OnGetImageIndex = vtvRenameGetImageIndex
        OnKeyDown = vtvRenameKeyDown
        Columns = <
          item
            MinWidth = 60
            Options = [coEnabled, coParentBidiMode, coParentColor, coResizable, coVisible]
            Position = 0
            Width = 187
            WideText = 'Old Name'
          end
          item
            Position = 1
            Width = 250
            WideText = 'New Name'
          end>
        WideDefaultText = ''
      end
      object Button1: TButton
        Left = 372
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Finish'
        TabOrder = 1
        OnClick = Button1Click
      end
      object edNewName: TEdit
        Left = 196
        Top = 96
        Width = 245
        Height = 21
        TabOrder = 2
        Text = 'edNewName'
        Visible = False
        OnChange = edNewNameChange
        OnKeyDown = edNewNameKeyDown
        OnKeyPress = edNewNameKeyPress
        OnKeyUp = edNewNameKeyUp
      end
      object cbNewName: TComboBox
        Left = 196
        Top = 120
        Width = 245
        Height = 21
        ItemHeight = 0
        TabOrder = 3
        Text = 'cbNewName'
        Visible = False
        OnChange = cbNewNameChange
        OnKeyDown = edNewNameKeyDown
        OnKeyPress = edNewNameKeyPress
      end
    end
  end
  object btnCommit: TBitBtn
    Left = 392
    Top = 408
    Width = 81
    Height = 33
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Kind = bkCancel
  end
end
