object frmMenuEdit: TfrmMenuEdit
  Tag = 1023
  Left = 335
  Top = 89
  Width = 675
  Height = 625
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'Menu Editor'
  Color = clBtnFace
  Constraints.MinHeight = 518
  Constraints.MinWidth = 648
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 667
    Height = 598
    Align = alClient
    BorderWidth = 4
    TabOrder = 0
    object Label10: TLabel
      Left = 16
      Top = 24
      Width = 64
      Height = 13
      Caption = 'Course Name'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Panel8: TPanel
      Left = 306
      Top = 5
      Width = 356
      Height = 543
      Align = alRight
      BevelOuter = bvLowered
      Constraints.MinWidth = 330
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 354
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 5
        object lbeTitle: TLabel
          Left = 16
          Top = 0
          Width = 297
          Height = 41
          AutoSize = False
          Caption = 'lbeTitle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object PageControl4: TPageControl
        Left = 1
        Top = 72
        Width = 354
        Height = 470
        ActivePage = tsProperties
        Align = alBottom
        Anchors = [akLeft, akTop, akRight, akBottom]
        Style = tsFlatButtons
        TabOrder = 4
        TabStop = False
        object tsProperties: TTabSheet
          Caption = 'Properties'
          TabVisible = False
          object PageControl1: TPageControl
            Left = 0
            Top = 0
            Width = 346
            Height = 460
            ActivePage = tsItemSize
            Align = alClient
            Style = tsFlatButtons
            TabOrder = 0
            TabStop = False
            object tsMenu: TTabSheet
              Caption = 'tsMenu'
              TabVisible = False
              object GroupBox5: TGroupBox
                Left = 0
                Top = 8
                Width = 337
                Height = 81
                Caption = 'Menu Type'
                TabOrder = 1
                OnEnter = GroupBox5Enter
                object rbDrinkMenu: TRadioButton
                  Left = 24
                  Top = 48
                  Width = 153
                  Height = 17
                  Caption = 'Beverage Menu'
                  TabOrder = 0
                  TabStop = True
                  OnKeyPress = rbFoodMenuKeyPress
                end
                object rbFoodMenu: TRadioButton
                  Left = 24
                  Top = 24
                  Width = 113
                  Height = 17
                  Caption = 'Food Menu'
                  TabOrder = 1
                  TabStop = True
                  OnKeyPress = rbFoodMenuKeyPress
                end
              end
              object btnEditMenu: TButton
                Left = 0
                Top = 128
                Width = 121
                Height = 25
                Caption = 'Create New Duplicate'
                TabOrder = 2
                OnClick = btnEditMenuClick
              end
              object chbAvailableOnPalm: TCheckBox
                Left = 0
                Top = 104
                Width = 153
                Height = 17
                Caption = 'Menu Available On Palms'
                TabOrder = 0
                OnKeyPress = chbAvailableOnPalmKeyPress
              end
            end
            object tsCourse: TTabSheet
              Caption = 'tsCourse'
              ImageIndex = 1
              TabVisible = False
              DesignSize = (
                338
                450)
              object Label5: TLabel
                Left = 0
                Top = 4
                Width = 64
                Height = 13
                Caption = 'Course Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label23: TLabel
                Left = 0
                Top = 32
                Width = 65
                Height = 13
                Caption = 'Kitchen Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Bevel9: TBevel
                Left = 0
                Top = 158
                Width = 335
                Height = 9
                Shape = bsTopLine
              end
              object Label41: TLabel
                Left = 0
                Top = 141
                Width = 129
                Height = 13
                Caption = 'Default Serving Course'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object lblCourseReceiptName: TLabel
                Left = 0
                Top = 88
                Width = 66
                Height = 13
                Caption = 'Receipt Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object lblCourseHandheldName: TLabel
                Left = 0
                Top = 60
                Width = 75
                Height = 13
                Caption = 'Handheld Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object sgCourse: TStringGrid
                Left = 0
                Top = 248
                Width = 337
                Height = 202
                Anchors = [akLeft, akTop, akBottom]
                ColCount = 4
                DefaultColWidth = 82
                DefaultRowHeight = 17
                FixedCols = 0
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goAlwaysShowEditor]
                TabOrder = 4
                OnDrawCell = sgCourseDrawCell
                OnEnter = sgCourseEnter
                OnExit = sgCourseExit
                OnKeyDown = sgCourseKeyDown
                OnKeyUp = sgCourseKeyUp
                OnSelectCell = sgCourseSelectCell
                OnSetEditText = sgCourseSetEditText
              end
              object edCourseName: TEdit
                Left = 87
                Top = 0
                Width = 248
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnChange = edCourseNameChange
                OnKeyPress = edKeyPress
              end
              object cbClassEnable: TCheckBox
                Left = 0
                Top = 113
                Width = 84
                Height = 17
                Caption = 'Available'
                Color = clBtnFace
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 2
                OnClick = cbClassEnableClick
                OnKeyPress = cbClassEnableKeyPress
              end
              object btnDeleteCourse: TButton
                Left = 229
                Top = 113
                Width = 106
                Height = 25
                Caption = 'Delete Course'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = btnDeleteCourseClick
              end
              object tntedCourseKitchenName: TTntEdit
                Left = 87
                Top = 29
                Width = 248
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Shell Dlg 2'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnChange = tntedCourseKitchenNameChange
                OnKeyPress = edKeyPress
              end
              object cbServingCourse: TComboBox
                Left = 148
                Top = 170
                Width = 187
                Height = 21
                Style = csDropDownList
                Color = clInfoBk
                DropDownCount = 16
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ItemHeight = 13
                ParentFont = False
                TabOrder = 5
                OnChange = cbServingCourseChange
              end
              object cbNoDefaultServingCourse: TCheckBox
                Left = 0
                Top = 203
                Width = 102
                Height = 17
                Caption = 'Always Prompt'
                TabOrder = 6
                OnClick = cbNoDefaultServingCourseClick
              end
              object btnSyncNoDefaultSC: TButton
                Left = 229
                Top = 197
                Width = 106
                Height = 25
                Caption = 'Sync Menu'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 7
                OnClick = btnSyncNoDefaultSCClick
              end
              object btnCoursePrices: TButton
                Left = 148
                Top = 197
                Width = 74
                Height = 25
                Caption = 'Prices'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 8
                OnClick = btnCoursePricesClick
              end
              object cbPalm: TCheckBox
                Left = 0
                Top = 400
                Width = 65
                Height = 17
                Caption = 'Palm'
                Color = clBtnFace
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 9
                Visible = False
                OnClick = cbBarClick
                OnKeyPress = cbPalmKeyPress
              end
              object cbBar: TCheckBox
                Left = 0
                Top = 416
                Width = 145
                Height = 17
                Caption = 'Bar/Bottle Store'
                Color = clBtnFace
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 10
                Visible = False
                OnClick = cbBarClick
                OnKeyPress = cbBarKeyPress
              end
              object cbRestaurant: TCheckBox
                Left = 0
                Top = 432
                Width = 121
                Height = 17
                Caption = 'Restaurant'
                Color = clBtnFace
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 11
                Visible = False
                OnClick = cbBarClick
                OnKeyPress = cbRestaurantKeyPress
              end
              object tntedCourseHandheldName: TTntEdit
                Left = 87
                Top = 57
                Width = 248
                Height = 21
                Color = clInfoBk
                TabOrder = 12
                OnChange = droidwareNameChange
              end
              object tntedCourseReceiptName: TTntEdit
                Tag = 1
                Left = 87
                Top = 87
                Width = 248
                Height = 21
                Color = clInfoBk
                TabOrder = 13
                Visible = False
                OnChange = droidwareNameChange
              end
            end
            object tsItem: TTabSheet
              Caption = 'tsItem'
              ImageIndex = 2
              TabVisible = False
              object pcItemDetails: TPageControl
                Left = 0
                Top = 0
                Width = 338
                Height = 450
                ActivePage = tsItemSizes
                Align = alClient
                TabIndex = 1
                TabOrder = 0
                OnChange = pcItemDetailsChange
                object tsItemDetails: TTabSheet
                  Caption = 'Item Detail'
                  object Label1: TLabel
                    Left = 8
                    Top = 12
                    Width = 52
                    Height = 13
                    Caption = 'Item Name'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object lbeDrinkColour: TLabel
                    Left = 8
                    Top = 128
                    Width = 66
                    Height = 13
                    Caption = 'Button Colour'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label2: TLabel
                    Left = 8
                    Top = 40
                    Width = 65
                    Height = 13
                    Caption = 'Kitchen Name'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object lblItemHandheldName: TLabel
                    Left = 8
                    Top = 68
                    Width = 75
                    Height = 13
                    Caption = 'Handheld Name'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object lblItemReceiptName: TLabel
                    Left = 8
                    Top = 96
                    Width = 66
                    Height = 13
                    Caption = 'Receipt Name'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    Visible = False
                  end
                  object cbItemName: TComboBox
                    Left = 96
                    Top = 8
                    Width = 225
                    Height = 21
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    TabOrder = 0
                    OnChange = cbItemNameChange
                    OnDropDown = cbItemNameDropDown
                    OnKeyDown = cbItemNameKeyDown
                    OnKeyPress = edKeyPress
                  end
                  object cbItemEnable: TCheckBox
                    Left = 8
                    Top = 232
                    Width = 105
                    Height = 17
                    Caption = 'Available'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = cbItemEnableClick
                    OnKeyPress = cbItemEnableKeyPress
                  end
                  object btnItemColour: TTouchButton
                    Left = 8
                    Top = 144
                    Width = 145
                    Height = 73
                    Color = clWhite
                    UpColour = clWhite
                    DownColour = clWhite
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -15
                    Font.Name = 'Arial'
                    Font.Style = [fsBold]
                    ParentFont = False
                    BorderStyle = bsSingle
                    BorderWidth = 2
                    BevelWidth = 2
                    ParentCtl3D = False
                    OnClick = stDrinkColourClick
                  end
                  object btnDeleteItem: TButton
                    Left = 224
                    Top = 248
                    Width = 97
                    Height = 25
                    Caption = 'Delete Item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = btnDeleteItemClick
                  end
                  object chbItemOnlySide: TCheckBox
                    Left = 8
                    Top = 256
                    Width = 153
                    Height = 17
                    Caption = 'Only available as a side'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = chbItemOnlySideClick
                    OnKeyPress = chbItemOnlySideKeyPress
                  end
                  object tntedKitchenName: TTntEdit
                    Left = 96
                    Top = 36
                    Width = 225
                    Height = 21
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Shell Dlg 2'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnChange = kitchenNameChange
                    OnKeyPress = tntedKitchenNameKeyPress
                  end
                  object tntedItemHandheldName: TTntEdit
                    Left = 96
                    Top = 64
                    Width = 225
                    Height = 21
                    Color = clInfoBk
                    TabOrder = 6
                    OnChange = droidwareNameChange
                  end
                  object tntedItemReceiptName: TTntEdit
                    Tag = 1
                    Left = 96
                    Top = 92
                    Width = 225
                    Height = 21
                    Color = clInfoBk
                    TabOrder = 7
                    Visible = False
                    OnChange = droidwareNameChange
                  end
                end
                object tsItemSizes: TTabSheet
                  Caption = 'Sizes'
                  ImageIndex = 1
                  DesignSize = (
                    330
                    422)
                  object btnAddItemSize: TBitBtn
                    Left = 8
                    Top = 8
                    Width = 153
                    Height = 25
                    Caption = 'Add to this item >'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = btnAddItemSizeClick
                  end
                  object lbSizesUnused: TListBox
                    Left = 8
                    Top = 40
                    Width = 153
                    Height = 89
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    Sorted = True
                    TabOrder = 1
                    OnClick = lbSizesUnusedClick
                    OnDblClick = btnAddItemSizeClick
                    OnEnter = lbSizesUnusedClick
                    OnKeyPress = lbSizesUnusedKeyPress
                  end
                  object btnDeleteItemSize: TBitBtn
                    Left = 8
                    Top = 136
                    Width = 153
                    Height = 25
                    Caption = '< Delete from this item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = btnDeleteItemSizeClick
                  end
                  object lbSizesUsed: TListBox
                    Left = 168
                    Top = 40
                    Width = 153
                    Height = 121
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    Items.Strings = (
                      '1.'
                      '2.'
                      '3.'
                      '4.'
                      '5.')
                    ParentFont = False
                    TabOrder = 5
                    OnClick = lbSizesUsedClick
                    OnEnter = lbSizesUsedClick
                  end
                  object btnMoveSizeDown: TBitBtn
                    Left = 248
                    Top = 8
                    Width = 73
                    Height = 25
                    Caption = 'Move Down'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = btnMoveSizeDownClick
                  end
                  object btnMoveSizeUp: TBitBtn
                    Left = 168
                    Top = 8
                    Width = 73
                    Height = 25
                    Caption = 'Move Up'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = btnMoveSizeUpClick
                  end
                  object sgItem: TStringGrid
                    Left = 8
                    Top = 168
                    Width = 313
                    Height = 244
                    Anchors = [akLeft, akTop, akBottom]
                    ColCount = 3
                    DefaultColWidth = 102
                    DefaultRowHeight = 17
                    FixedCols = 0
                    RowCount = 6
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goAlwaysShowEditor]
                    ParentFont = False
                    TabOrder = 6
                    OnDrawCell = sgItemDrawCell
                    OnEnter = sgItemEnter
                    OnExit = sgItemExit
                    OnKeyDown = sgItemKeyDown
                    OnKeyPress = sgItemKeyPress
                    OnKeyUp = sgItemKeyUp
                    OnSelectCell = sgItemSelectCell
                    OnSetEditText = sgItemSetEditText
                  end
                end
                object TabSheet6: TTabSheet
                  Caption = 'Forced Sides'
                  ImageIndex = 2
                  DesignSize = (
                    330
                    422)
                  object rbForcedSideGrp1: TRadioButton
                    Tag = 1
                    Left = 8
                    Top = 8
                    Width = 81
                    Height = 17
                    Caption = 'Group 1'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp5: TRadioButton
                    Tag = 5
                    Left = 8
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 5'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp6: TRadioButton
                    Tag = 6
                    Left = 88
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 6'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 5
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp2: TRadioButton
                    Tag = 2
                    Left = 88
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 2'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp3: TRadioButton
                    Tag = 3
                    Left = 168
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 3'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp7: TRadioButton
                    Tag = 7
                    Left = 168
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 7'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 6
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp8: TRadioButton
                    Tag = 8
                    Left = 248
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 8'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 7
                    OnClick = rbForcedSideGrpClick
                  end
                  object rbForcedSideGrp4: TRadioButton
                    Tag = 4
                    Left = 248
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 4'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = rbForcedSideGrpClick
                  end
                  object chbSideMultiSelect: TCheckBox
                    Left = 8
                    Top = 64
                    Width = 177
                    Height = 17
                    Caption = 'Allow multi-select for this group'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 8
                    OnClick = chbSideMultiSelectClick
                  end
                  object lbForcedSides: TListBox
                    Left = 8
                    Top = 88
                    Width = 313
                    Height = 292
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    PopupMenu = pmForcedSides
                    TabOrder = 9
                    OnMouseDown = lbForcedSidesMouseDown
                  end
                  object btnAddSide: TButton
                    Left = 8
                    Top = 387
                    Width = 70
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Add Side'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 10
                    OnClick = btnAddSideClick
                  end
                  object btnRemoveSide: TButton
                    Left = 89
                    Top = 387
                    Width = 70
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Remove'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 11
                    OnClick = btnRemoveSideClick
                  end
                  object btnForcedSideMoveUp: TButton
                    Left = 252
                    Top = 387
                    Width = 70
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Move Up'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 12
                    OnClick = btnForcedSideMoveUpClick
                  end
                  object chbSideGroupSkip: TCheckBox
                    Left = 184
                    Top = 64
                    Width = 145
                    Height = 17
                    Caption = 'Allow skip for this group'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 13
                    OnClick = chbSideGroupSkipClick
                  end
                  object btnSyncSide: TButton
                    Left = 170
                    Top = 387
                    Width = 75
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Sides'
                    TabOrder = 14
                    OnClick = btnSyncSideClick
                  end
                end
                object TabSheet10: TTabSheet
                  Caption = 'Forced Options'
                  ImageIndex = 3
                  DesignSize = (
                    330
                    422)
                  object rbCondimentGrp1: TRadioButton
                    Tag = 1
                    Left = 8
                    Top = 10
                    Width = 65
                    Height = 15
                    Caption = 'Group 1'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp5: TRadioButton
                    Tag = 5
                    Left = 8
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 5'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp6: TRadioButton
                    Tag = 6
                    Left = 88
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 6'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 5
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp2: TRadioButton
                    Tag = 2
                    Left = 88
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 2'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp3: TRadioButton
                    Tag = 3
                    Left = 168
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 3'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp7: TRadioButton
                    Tag = 7
                    Left = 168
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 7'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 6
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp8: TRadioButton
                    Tag = 8
                    Left = 248
                    Top = 32
                    Width = 65
                    Height = 17
                    Caption = 'Group 8'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 7
                    OnClick = rbCondimentGrpClick
                  end
                  object rbCondimentGrp4: TRadioButton
                    Tag = 4
                    Left = 248
                    Top = 8
                    Width = 65
                    Height = 17
                    Caption = 'Group 4'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = rbCondimentGrpClick
                  end
                  object chklstCondiments: TCheckListBox
                    Left = 8
                    Top = 88
                    Width = 313
                    Height = 292
                    OnClickCheck = chklstCondimentsClickCheck
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    HeaderBackgroundColor = clWindow
                    ItemHeight = 13
                    ParentFont = False
                    TabOrder = 8
                  end
                  object BitBtn1: TBitBtn
                    Left = 8
                    Top = 387
                    Width = 113
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Select All Options'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 9
                    OnClick = BitBtn1Click
                  end
                  object chbOptionMultiSelect: TCheckBox
                    Left = 8
                    Top = 64
                    Width = 177
                    Height = 17
                    Caption = 'Allow multi-select for this group'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 10
                    OnClick = chbOptionMultiSelectClick
                  end
                  object chbOptionGroupSkip: TCheckBox
                    Left = 192
                    Top = 65
                    Width = 145
                    Height = 17
                    Caption = 'Allow skip for this group'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 11
                    OnClick = chbOptionGroupSkipClick
                  end
                  object btnSyncOptions: TButton
                    Left = 208
                    Top = 388
                    Width = 113
                    Height = 25
                    Caption = 'Sync Forced Options'
                    TabOrder = 12
                    OnClick = btnSyncOptionsClick
                  end
                end
                object tsPrinters: TTabSheet
                  Caption = 'Printing'
                  ImageIndex = 4
                  object Label43: TLabel
                    Left = 13
                    Top = 25
                    Width = 90
                    Height = 13
                    Caption = 'Printing Options'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Bevel8: TBevel
                    Left = 13
                    Top = 43
                    Width = 307
                    Height = 17
                    Shape = bsTopLine
                  end
                  object cbUnderline: TCheckBox
                    Left = 32
                    Top = 64
                    Width = 105
                    Height = 17
                    Caption = 'Underline'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                  end
                  object cbBold: TCheckBox
                    Left = 32
                    Top = 99
                    Width = 105
                    Height = 17
                    Caption = 'Bold'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                  end
                  object cbLargeFont: TCheckBox
                    Left = 32
                    Top = 166
                    Width = 105
                    Height = 17
                    Caption = 'Large Font'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                  end
                  object cbDoubleWidth: TCheckBox
                    Left = 32
                    Top = 201
                    Width = 105
                    Height = 17
                    Caption = 'Double Width'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                  end
                  object cbDoubleHeight: TCheckBox
                    Left = 32
                    Top = 236
                    Width = 105
                    Height = 17
                    Caption = 'Double Height'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                  end
                  object cbColour: TCheckBox
                    Left = 32
                    Top = 134
                    Width = 89
                    Height = 17
                    Caption = 'Black'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 5
                    OnClick = cbColourClick
                  end
                  object btnSyncPrinting: TButton
                    Left = 208
                    Top = 388
                    Width = 113
                    Height = 25
                    Caption = 'Sync Printing Options'
                    TabOrder = 6
                    OnClick = btnSyncPrintingClick
                  end
                end
              end
            end
            object tsItemSize: TTabSheet
              Caption = 'tsItemSize'
              ImageIndex = 3
              TabVisible = False
              object pcItemSizeDetails: TPageControl
                Left = 0
                Top = 0
                Width = 338
                Height = 450
                ActivePage = tsSizeCategories
                Align = alClient
                TabIndex = 2
                TabOrder = 0
                object tsItemSizePrice: TTabSheet
                  Caption = 'Price / Cost'
                  object lbeTareWeight: TLabel
                    Left = 8
                    Top = 397
                    Width = 81
                    Height = 13
                    Caption = 'Tare Weight (kg)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object btnDeleteSize: TButton
                    Left = 248
                    Top = 393
                    Width = 70
                    Height = 25
                    Caption = 'Delete Size'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = btnDeleteSizeClick
                  end
                  object GroupBox1: TGroupBox
                    Left = 8
                    Top = 8
                    Width = 313
                    Height = 209
                    Caption = 'Prices'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                    TabOrder = 0
                    DesignSize = (
                      313
                      209)
                    object lblMaxRetailPrice: TLabel
                      Left = 135
                      Top = 15
                      Width = 100
                      Height = 13
                      Caption = 'Maximum Retail Price'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                    end
                    object cbFreeItem: TCheckBox
                      Left = 16
                      Top = 15
                      Width = 50
                      Height = 17
                      Alignment = taLeftJustify
                      Caption = 'Free'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 0
                      OnClick = cbFreeItemClick
                      OnKeyPress = cbFreeItemKeyPress
                    end
                    object edItemMaxPrice: TNumericEdit
                      Left = 248
                      Top = 15
                      Width = 49
                      Height = 21
                      Alignment = taLeftJustify
                      Precision = 4
                      FixedPoint = True
                      Color = clInfoBk
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      MaxLength = 8
                      ParentFont = False
                      TabOrder = 1
                      OnEnter = edItemPriceEnter
                      OnKeyPress = edKeyPress
                      DecimalPlaces = 2
                    end
                    object sgitemsize: TStringGrid
                      Left = 16
                      Top = 40
                      Width = 281
                      Height = 161
                      Anchors = [akLeft, akTop, akBottom]
                      ColCount = 3
                      DefaultColWidth = 91
                      DefaultRowHeight = 17
                      FixedCols = 0
                      RowCount = 2
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goAlwaysShowEditor]
                      ParentFont = False
                      ScrollBars = ssVertical
                      TabOrder = 2
                      OnDrawCell = sgItemSizeDrawCell
                      OnEnter = sgItemSizeEnter
                      OnExit = sgItemSizeExit
                      OnKeyDown = sgItemSizeKeyDown
                      OnKeyPress = sgItemSizeKeyPress
                      OnKeyUp = sgItemSizeKeyUp
                      OnSelectCell = sgItemSizeSelectCell
                      OnSetEditText = sgItemSizeEditText
                    end
                  end
                  object GroupBox2: TGroupBox
                    Left = 8
                    Top = 216
                    Width = 313
                    Height = 81
                    Caption = 'Cost'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                    TabOrder = 1
                    object Label14: TLabel
                      Left = 16
                      Top = 51
                      Width = 47
                      Height = 13
                      Caption = 'Item Cost'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                    end
                    object Label16: TLabel
                      Left = 160
                      Top = 51
                      Width = 50
                      Height = 13
                      Caption = 'Excl. TAX'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object Label13: TLabel
                      Left = 16
                      Top = 24
                      Width = 41
                      Height = 13
                      Caption = 'TAX (%)'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                    end
                    object edItemCost: TNumericEdit
                      Left = 88
                      Top = 47
                      Width = 57
                      Height = 21
                      Alignment = taLeftJustify
                      Precision = 15
                      FixedPoint = True
                      Color = clInfoBk
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 1
                      OnChange = edItemCostChange
                      OnEnter = edItemPriceEnter
                      OnExit = edItemCostExit
                      OnKeyPress = edKeyPress
                      DecimalPlaces = 2
                    end
                    object CheckBox1: TCheckBox
                      Left = 219
                      Top = 50
                      Width = 85
                      Height = 17
                      Alignment = taLeftJustify
                      Caption = 'Use Recipe'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 0
                      Visible = False
                    end
                    object edCostGST: TNumericEdit
                      Left = 88
                      Top = 19
                      Width = 57
                      Height = 21
                      Alignment = taLeftJustify
                      Precision = 10
                      NumericType = ntPercent
                      Color = clInfoBk
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 2
                      OnExit = edCostGSTExit
                      OnKeyPress = edKeyPress
                      DecimalPlaces = 2
                    end
                  end
                  object btnScanBarcode: TButton
                    Left = 168
                    Top = 392
                    Width = 70
                    Height = 25
                    Caption = 'Scan Barcode'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = btnScanBarcodeClick
                  end
                  object neTareWeight: TNumericEdit
                    Left = 96
                    Top = 393
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 15
                    FixedPoint = True
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnEnter = neTareWeightEnter
                    OnExit = neTareWeightExit
                    OnKeyPress = edKeyPress
                    DecimalPlaces = 3
                  end
                  object GroupBox4: TGroupBox
                    Left = 8
                    Top = 298
                    Width = 313
                    Height = 89
                    Caption = 'Patron Count'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                    TabOrder = 5
                    object lblDefaultPatronCount: TLabel
                      Left = 16
                      Top = 24
                      Width = 102
                      Height = 13
                      Caption = 'Default Patron Count'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                    end
                    object edDefaultPatronCount: TEdit
                      Left = 130
                      Top = 20
                      Width = 167
                      Height = 21
                      AutoSize = False
                      Color = clInfoBk
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'Tahoma'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 0
                      OnExit = OnLeaveDefaultPatronCount
                    end
                    object btnSyncDefaultPatronCount: TButton
                      Left = 16
                      Top = 48
                      Width = 161
                      Height = 25
                      Caption = 'Set for all sizes in course'
                      TabOrder = 1
                      OnClick = SyncDefaultPatronCountClicked
                    end
                  end
                end
                object tsTaxProfiles: TTabSheet
                  Caption = 'Tax Profiles'
                  ImageIndex = 5
                  object Label44: TLabel
                    Left = 0
                    Top = 4
                    Width = 119
                    Height = 13
                    Caption = 'Available tax profiles'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Label45: TLabel
                    Left = 168
                    Top = 4
                    Width = 109
                    Height = 13
                    Caption = 'Applied tax profiles'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object lbUnassigned: TListBox
                    Left = 0
                    Top = 24
                    Width = 160
                    Height = 193
                    Color = clInfoBk
                    ExtendedSelect = False
                    ItemHeight = 13
                    TabOrder = 0
                    OnDblClick = lbUnassignedDblClick
                  end
                  object lbAssigned: TListBox
                    Left = 168
                    Top = 24
                    Width = 160
                    Height = 193
                    Color = clInfoBk
                    ExtendedSelect = False
                    ItemHeight = 13
                    TabOrder = 1
                    OnDblClick = lbAssignedDblClick
                  end
                  object btnAddTP: TBitBtn
                    Left = 1
                    Top = 224
                    Width = 160
                    Height = 25
                    Caption = 'Add to item size >'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = btnAddTPMouseClick
                  end
                  object btnRemoveTP: TBitBtn
                    Left = 169
                    Top = 224
                    Width = 160
                    Height = 25
                    Caption = '< Remove from item size'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = btnRemoveTPMouseClick
                  end
                  object btnSyncTP: TBitBtn
                    Left = 169
                    Top = 256
                    Width = 160
                    Height = 25
                    Caption = 'Sync'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = btnSyncTPClick
                  end
                end
                object tsSizeCategories: TTabSheet
                  Caption = 'Categories'
                  ImageIndex = 3
                  DesignSize = (
                    330
                    422)
                  object Label32: TLabel
                    Left = 8
                    Top = 64
                    Width = 127
                    Height = 13
                    Caption = 'Breakdown Categories'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Label35: TLabel
                    Left = 16
                    Top = 217
                    Width = 226
                    Height = 13
                    Anchors = [akLeft, akBottom]
                    Caption = '3rd Party Group Code                                   '
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Label31: TLabel
                    Left = 8
                    Top = 8
                    Width = 52
                    Height = 13
                    Caption = 'Category'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Bevel3: TBevel
                    Left = 8
                    Top = 24
                    Width = 313
                    Height = 9
                    Shape = bsTopLine
                  end
                  object Bevel4: TBevel
                    Left = 8
                    Top = 80
                    Width = 313
                    Height = 9
                    Shape = bsTopLine
                  end
                  object Bevel6: TBevel
                    Left = 16
                    Top = 233
                    Width = 313
                    Height = 9
                    Anchors = [akLeft, akBottom]
                    Shape = bsTopLine
                  end
                  object Bevel10: TBevel
                    Left = 17
                    Top = 353
                    Width = 313
                    Height = 9
                    Anchors = [akLeft, akBottom]
                    Shape = bsTopLine
                  end
                  object Label3: TLabel
                    Left = 16
                    Top = 337
                    Width = 126
                    Height = 13
                    Anchors = [akLeft, akBottom]
                    Caption = 'Revenue Code               '
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object btnAddCategory: TButton
                    Left = 8
                    Top = 88
                    Width = 89
                    Height = 25
                    Caption = 'Add'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = btnAddCategoryClick
                  end
                  object btnRemoveCategory: TButton
                    Left = 8
                    Top = 120
                    Width = 89
                    Height = 25
                    Caption = 'Remove'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnClick = btnRemoveCategoryClick
                  end
                  object btnSyncCategories: TButton
                    Left = 8
                    Top = 152
                    Width = 89
                    Height = 25
                    Caption = 'Sync'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = btnSyncCategoriesClick
                  end
                  object lbCategories: TListBox
                    Left = 112
                    Top = 88
                    Width = 209
                    Height = 121
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    Sorted = True
                    TabOrder = 3
                    OnKeyDown = lbCategoriesKeyDown
                  end
                  object btnSyncCategory: TButton
                    Left = 248
                    Top = 31
                    Width = 73
                    Height = 25
                    Caption = 'Sync'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    OnClick = btnSyncCategoryClick
                  end
                  object cbCategory: TComboBox
                    Left = 8
                    Top = 32
                    Width = 233
                    Height = 24
                    Style = csOwnerDrawFixed
                    Color = clInfoBk
                    DropDownCount = 16
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 18
                    ParentFont = False
                    TabOrder = 5
                    OnDrawItem = cbCategoryDrawItem
                    OnSelect = cbCategorySelect
                    Items.Strings = (
                      '0'
                      '1'
                      '2'
                      '3'
                      '4'
                      '5'
                      '6'
                      '7'
                      '8'
                      '9')
                  end
                  object cb3rdPartyGroupCode: TComboBox
                    Left = 16
                    Top = 241
                    Width = 305
                    Height = 21
                    Anchors = [akLeft, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    Sorted = True
                    TabOrder = 6
                    Text = 'cb3rdPartyGroupCode'
                    OnChange = cb3rdPartyGroupCodeChange
                  end
                  object btnSync3rdPartyGroupItem: TButton
                    Left = 16
                    Top = 299
                    Width = 97
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 7
                    OnClick = btnSync3rdPartyGroupItemClick
                  end
                  object btnSync3rdPartyGroupCourse: TButton
                    Left = 120
                    Top = 299
                    Width = 105
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Course'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 8
                    OnClick = btnSync3rdPartyGroupCourseClick
                  end
                  object btnSync3rdPartyGroupMenu: TButton
                    Left = 232
                    Top = 299
                    Width = 97
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Menu'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 9
                    OnClick = btnSync3rdPartyGroupMenuClick
                  end
                  object Button1: TButton
                    Left = 120
                    Top = 267
                    Width = 105
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Auto-assign'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 10
                    OnClick = Button1Click
                  end
                  object cbRevenueGroupCode: TComboBox
                    Left = 16
                    Top = 360
                    Width = 305
                    Height = 19
                    Style = csOwnerDrawFixed
                    Anchors = [akLeft, akBottom]
                    Color = clInfoBk
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    Sorted = True
                    TabOrder = 11
                    OnSelect = cbRevenueGroupCodeSelect
                  end
                  object btnSyncRevenueCodeItem: TButton
                    Left = 16
                    Top = 387
                    Width = 97
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 12
                    OnClick = btnSyncRevenueCodeItemClick
                  end
                  object btnSyncRevenueCodeCourse: TButton
                    Left = 120
                    Top = 387
                    Width = 105
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Course'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 13
                    OnClick = btnSyncRevenueCodeCourseClick
                  end
                  object btnSyncRevenueCodeMenu: TButton
                    Left = 232
                    Top = 387
                    Width = 97
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Sync Menu'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 14
                    OnClick = btnSyncRevenueCodeMenuClick
                  end
                end
                object TabSheet3: TTabSheet
                  Caption = 'Recipes'
                  ImageIndex = 2
                  DesignSize = (
                    330
                    422)
                  object Label8: TLabel
                    Left = 8
                    Top = 299
                    Width = 97
                    Height = 13
                    Anchors = [akLeft, akBottom]
                    Caption = 'Reduction Quantity:'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label12: TLabel
                    Left = 176
                    Top = 243
                    Width = 120
                    Height = 13
                    Anchors = [akLeft, akBottom]
                    Caption = 'Stock Reduction Location'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label20: TLabel
                    Left = 8
                    Top = 243
                    Width = 77
                    Height = 13
                    Anchors = [akLeft, akBottom]
                    Caption = 'Recipe Quantity'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object edQty: TLabel
                    Left = 112
                    Top = 301
                    Width = 3
                    Height = 13
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object btnAddRecipeToItems: TBitBtn
                    Left = 120
                    Top = 355
                    Width = 97
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Add to all items'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    Visible = False
                    OnClick = btnAddRecipeToItemsClick
                  end
                  object btnAddRecipe: TBitBtn
                    Left = 168
                    Top = 8
                    Width = 73
                    Height = 25
                    Caption = 'Add'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = btnAddRecipeClick
                  end
                  object btnDeleteRecipe: TBitBtn
                    Left = 248
                    Top = 8
                    Width = 73
                    Height = 25
                    Caption = 'Delete'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnClick = btnDeleteRecipeClick
                  end
                  object sgRecipe: TStringGrid
                    Left = 8
                    Top = 48
                    Width = 313
                    Height = 184
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    ColCount = 3
                    DefaultRowHeight = 17
                    FixedCols = 0
                    RowCount = 2
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
                    ParentFont = False
                    ScrollBars = ssVertical
                    TabOrder = 2
                    OnSelectCell = sgRecipeSelectCell
                    ColWidths = (
                      86
                      140
                      82)
                  end
                  object edLoc: TComboBox
                    Left = 176
                    Top = 259
                    Width = 145
                    Height = 21
                    Anchors = [akLeft, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    Sorted = True
                    TabOrder = 3
                    Text = 'edLoc'
                    OnChange = edLocChange
                  end
                  object btnSetLocationForAllItems: TBitBtn
                    Left = 8
                    Top = 387
                    Width = 313
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Set location for all items'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 6
                    OnClick = btnSetLocationForAllItemsClick
                  end
                  object btnAddRecipeToSizes: TBitBtn
                    Left = 8
                    Top = 355
                    Width = 153
                    Height = 25
                    Anchors = [akLeft, akBottom]
                    Caption = 'Add recipe to all sizes'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 5
                    OnClick = btnAddRecipeToSizesClick
                  end
                  object btnAddRecipesToAllSizes: TButton
                    Left = 168
                    Top = 356
                    Width = 153
                    Height = 25
                    Caption = 'Add all recipes to all sizes'
                    TabOrder = 8
                    OnClick = btnAddRecipesToAllSizesClick
                  end
                  object edRecipeQty: TNumericEdit
                    Left = 8
                    Top = 259
                    Width = 153
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 15
                    Anchors = [akLeft, akBottom]
                    Color = clInfoBk
                    Enabled = False
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 10
                    ParentFont = False
                    TabOrder = 7
                    OnChange = edLocChange
                    OnKeyPress = edQtyKeyPress
                    DecimalPlaces = 4
                  end
                end
                object tsSizeLoyalty: TTabSheet
                  Caption = 'Loyalty'
                  ImageIndex = 4
                  object Label25: TLabel
                    Left = 8
                    Top = 66
                    Width = 152
                    Height = 13
                    Caption = 'Purchase count before discount'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label26: TLabel
                    Left = 8
                    Top = 94
                    Width = 199
                    Height = 13
                    Caption = 'Discount by (set to 100% for a free item)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label27: TLabel
                    Left = 8
                    Top = 8
                    Width = 139
                    Height = 13
                    Caption = 'Members Puchase Count'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Bevel1: TBevel
                    Left = 8
                    Top = 24
                    Width = 313
                    Height = 9
                    Shape = bsTopLine
                  end
                  object Label28: TLabel
                    Left = 8
                    Top = 121
                    Width = 134
                    Height = 13
                    Caption = 'Location Puchase Count'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Bevel2: TBevel
                    Left = 8
                    Top = 137
                    Width = 313
                    Height = 17
                    Shape = bsTopLine
                  end
                  object Label29: TLabel
                    Left = 8
                    Top = 177
                    Width = 152
                    Height = 13
                    Caption = 'Purchase count before discount'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label30: TLabel
                    Left = 8
                    Top = 205
                    Width = 199
                    Height = 13
                    Caption = 'Discount by (set to 100% for a free item)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label9: TLabel
                    Left = 8
                    Top = 269
                    Width = 80
                    Height = 13
                    Caption = '% Points Earned'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label34: TLabel
                    Left = 8
                    Top = 237
                    Width = 77
                    Height = 13
                    Caption = 'Points Earned'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object Bevel5: TBevel
                    Left = 8
                    Top = 255
                    Width = 313
                    Height = 17
                    Shape = bsTopLine
                  end
                  object lblPointsRedemption: TLabel
                    Left = 8
                    Top = 333
                    Width = 107
                    Height = 13
                    Caption = 'Points Redemption'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = [fsBold]
                    ParentFont = False
                  end
                  object bvlPointsRedemption: TBevel
                    Left = 8
                    Top = 351
                    Width = 313
                    Height = 17
                    Shape = bsTopLine
                  end
                  object lblPriceForPoint: TLabel
                    Left = 172
                    Top = 362
                    Width = 74
                    Height = 13
                    Caption = 'Price For Points'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                  end
                  object neMemberPurchaseCount: TNumericEdit
                    Left = 264
                    Top = 62
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 15
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 10
                    OnExit = neMemberPurchaseCountExit
                    OnKeyPress = edKeyPress
                    DecimalPlaces = 0
                  end
                  object neMemberPurchaseDiscount: TNumericEdit
                    Left = 264
                    Top = 90
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 4
                    NumericType = ntPercent
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 2
                    OnExit = neMemberPurchaseDiscountExit
                    OnKeyPress = edKeyPress
                    DecimalPlaces = 2
                  end
                  object neLocationPurchaseCount: TNumericEdit
                    Left = 264
                    Top = 175
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 15
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 4
                    OnExit = neLocationPurchaseCountExit
                    OnKeyPress = edKeyPress
                    DecimalPlaces = 0
                  end
                  object neLocationPurchaseDiscount: TNumericEdit
                    Left = 264
                    Top = 203
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 4
                    NumericType = ntPercent
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 5
                    OnExit = neLocationPurchaseDiscountExit
                    OnKeyPress = edKeyPress
                    DecimalPlaces = 2
                  end
                  object btnSetItemsPoints: TBitBtn
                    Left = 8
                    Top = 293
                    Width = 145
                    Height = 25
                    Caption = 'Set all items in this course'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 7
                    OnClick = btnSetItemsPointsClick
                  end
                  object nePointsPercent: TNumericEdit
                    Left = 96
                    Top = 265
                    Width = 57
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 4
                    NumericType = ntPercent
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 6
                    OnExit = nePointsPercentExit
                    OnKeyPress = nePointsPercentKeyPress
                    DecimalPlaces = 2
                  end
                  object cbLocationDiscountAlways: TCheckBox
                    Left = 7
                    Top = 150
                    Width = 270
                    Height = 17
                    Alignment = taLeftJustify
                    Caption = 'Discount on every purchase'
                    TabOrder = 3
                    OnClick = cbLocationDiscountAlwaysClick
                    OnExit = neLocationPurchaseCountExit
                    OnKeyPress = edKeyPress
                  end
                  object cbMemberDiscountAlways: TCheckBox
                    Left = 7
                    Top = 38
                    Width = 269
                    Height = 17
                    Alignment = taLeftJustify
                    Caption = 'Discount on every purchase'
                    TabOrder = 0
                    OnClick = cbMemberDiscountAlwaysClick
                    OnExit = neMemberPurchaseCountExit
                    OnKeyPress = edKeyPress
                  end
                  object cbCanBePaidForUsingPoints: TCheckBox
                    Left = 8
                    Top = 360
                    Width = 161
                    Height = 17
                    Caption = 'Can be paid for using points'
                    TabOrder = 8
                    OnClick = cbCanBePaidForUsingPointsClicked
                  end
                  object nePriceForPoint: TNumericEdit
                    Left = 260
                    Top = 360
                    Width = 49
                    Height = 21
                    Alignment = taLeftJustify
                    Precision = 4
                    FixedPoint = True
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    MaxLength = 8
                    ParentFont = False
                    TabOrder = 1
                    OnChange = nePriceForPointChange
                    OnEnter = nePriceForPointEnter
                    OnExit = nePriceForPointExit
                    DecimalPlaces = 2
                  end
                  object btnSetItemsSpendingPoints: TBitBtn
                    Left = 8
                    Top = 384
                    Width = 145
                    Height = 25
                    Caption = 'Set all items in this course'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 9
                    OnClick = btnSetItemsSpendingPointsClick
                  end
                end
                object TabSheet2: TTabSheet
                  Caption = 'Set Menus'
                  ImageIndex = 1
                  DesignSize = (
                    330
                    422)
                  object lbSetMenuItems: TListBox
                    Left = 8
                    Top = 77
                    Width = 313
                    Height = 335
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    TabOrder = 4
                  end
                  object chbSetMenuMaster: TCheckBox
                    Left = 8
                    Top = 16
                    Width = 137
                    Height = 17
                    Caption = 'Set Menu Key'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                    OnClick = chbSetMenuMasterClick
                  end
                  object chbSetMenuItem: TCheckBox
                    Left = 160
                    Top = 16
                    Width = 105
                    Height = 17
                    Caption = 'Set Menu Item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    OnClick = chbSetMenuItemClick
                  end
                  object lbSetMenuMasters: TCheckListBox
                    Left = 8
                    Top = 77
                    Width = 313
                    Height = 335
                    Anchors = [akLeft, akTop, akBottom]
                    Color = clInfoBk
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ItemHeight = 13
                    ParentFont = False
                    TabOrder = 2
                    OnClick = lbSetMenuMastersClick
                  end
                  object chbSetMenuStandardItem: TCheckBox
                    Left = 8
                    Top = 43
                    Width = 193
                    Height = 17
                    Anchors = [akLeft, akBottom]
                    Caption = 'Available as standard Item'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'Tahoma'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    OnClick = chbSetMenuStandardItemClick
                  end
                end
              end
            end
            object tsOptions: TTabSheet
              Caption = 'tsOptions'
              ImageIndex = 4
              TabVisible = False
              DesignSize = (
                338
                450)
              object sgOptions: TStringGrid
                Left = 0
                Top = 0
                Width = 337
                Height = 418
                Anchors = [akLeft, akTop, akBottom]
                ColCount = 4
                DefaultColWidth = 84
                DefaultRowHeight = 17
                FixedCols = 0
                RowCount = 2
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goAlwaysShowEditor]
                ParentFont = False
                ScrollBars = ssVertical
                TabOrder = 0
                OnDrawCell = sgOptionsDrawCell
                OnKeyPress = sgOptionsKeyPress
                OnKeyUp = sgOptionsKeyUp
                OnMouseDown = sgOptionsMouseDown
                OnSelectCell = sgOptionsSelectCell
                ColWidths = (
                  113
                  70
                  94
                  51)
              end
              object ComboBox1: TComboBox
                Left = 185
                Top = 18
                Width = 97
                Height = 21
                DropDownCount = 9
                ItemHeight = 13
                TabOrder = 1
                Text = 'ComboBox1'
                OnChange = ComboBox1Change
                OnEnter = ComboBox1Enter
                OnExit = ComboBox1Exit
                OnKeyDown = ComboBox1KeyDown
                OnKeyPress = ComboBox1KeyPress
                Items.Strings = (
                  'None'
                  '1'
                  '2'
                  '3'
                  '4'
                  '5'
                  '6'
                  '7'
                  '8')
              end
              object cbOptionsEnable: TCheckBox
                Left = 0
                Top = 429
                Width = 161
                Height = 17
                Anchors = [akLeft, akBottom]
                Caption = 'Options Available'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Visible = False
                OnClick = cbOptionsEnableClick
              end
            end
            object tsOption: TTabSheet
              Caption = 'tsOption'
              ImageIndex = 5
              TabVisible = False
              object Label6: TLabel
                Left = 0
                Top = 4
                Width = 62
                Height = 13
                Caption = 'Option Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label11: TLabel
                Left = 0
                Top = 116
                Width = 100
                Height = 13
                Caption = 'Forced Option Group'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label7: TLabel
                Left = 24
                Top = 412
                Width = 56
                Height = 13
                Caption = 'Short Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                Layout = tlCenter
                Visible = False
              end
              object Label24: TLabel
                Left = 0
                Top = 32
                Width = 65
                Height = 13
                Caption = 'Kitchen Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Bevel7: TBevel
                Left = 2
                Top = 211
                Width = 334
                Height = 15
                Shape = bsTopLine
              end
              object Label42: TLabel
                Left = 1
                Top = 193
                Width = 90
                Height = 13
                Caption = 'Printing Options'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object lblOptionHandheldName: TLabel
                Left = 0
                Top = 60
                Width = 75
                Height = 13
                Caption = 'Handheld Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object lblOptionReceiptName: TLabel
                Left = 0
                Top = 88
                Width = 66
                Height = 13
                Caption = 'Receipt Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object edOptionName: TEdit
                Left = 112
                Top = 0
                Width = 225
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnChange = edOptionNameChange
                OnKeyPress = edKeyPress
              end
              object cbOptionEnable: TCheckBox
                Left = 26
                Top = 384
                Width = 97
                Height = 17
                Caption = 'Available'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
                Visible = False
                OnClick = cbOptionEnableClick
              end
              object cbOptionGroup: TComboBox
                Left = 112
                Top = 112
                Width = 97
                Height = 21
                Style = csDropDownList
                Color = clInfoBk
                DropDownCount = 9
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ItemHeight = 13
                ParentFont = False
                TabOrder = 2
                OnChange = cbOptionGroupChange
                OnKeyPress = cbOptionGroupKeyPress
                Items.Strings = (
                  'No Group'
                  '1'
                  '2'
                  '3'
                  '4'
                  '5'
                  '6'
                  '7'
                  '8')
              end
              object btnDeleteOption: TButton
                Left = 240
                Top = 167
                Width = 97
                Height = 21
                Caption = 'Delete Option'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = btnDeleteOptionClick
              end
              object edOptionShortName: TEdit
                Left = 88
                Top = 408
                Width = 41
                Height = 21
                Color = 15269887
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 5
                Visible = False
                OnChange = edOptionShortNameChange
                OnKeyPress = edOptionShortNameKeyPress
              end
              object tntedOptionKitchenName: TTntEdit
                Left = 112
                Top = 28
                Width = 225
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Shell Dlg 2'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnChange = tntedOptionKitchenNameChange
                OnKeyPress = edKeyPress
              end
              object cbUnderlineOptions: TCheckBox
                Left = 26
                Top = 224
                Width = 105
                Height = 17
                Caption = 'Underline'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 6
              end
              object cbBoldOptions: TCheckBox
                Left = 26
                Top = 251
                Width = 105
                Height = 17
                Caption = 'Bold'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 7
              end
              object cbColourOptions: TCheckBox
                Left = 26
                Top = 278
                Width = 89
                Height = 17
                Caption = 'Black'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 8
                OnClick = cbColourClick
              end
              object cbLargeFontOptions: TCheckBox
                Left = 26
                Top = 303
                Width = 105
                Height = 17
                Caption = 'Large Font'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 9
              end
              object cbDoubleWidthOptions: TCheckBox
                Left = 26
                Top = 330
                Width = 105
                Height = 17
                Caption = 'Double Width'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 10
              end
              object cbDoubleHeightOptions: TCheckBox
                Left = 26
                Top = 357
                Width = 105
                Height = 17
                Caption = 'Double Height'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 11
              end
              object cbPlusOption: TCheckBox
                Left = 0
                Top = 143
                Width = 125
                Height = 17
                Alignment = taLeftJustify
                Caption = 'Plus Option                 '
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 12
                OnClick = cbPlusOptionClick
              end
              object cbMinusOption: TCheckBox
                Left = 0
                Top = 169
                Width = 125
                Height = 17
                Alignment = taLeftJustify
                Caption = 'Minus Option'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 13
                OnClick = cbPlusOptionClick
              end
              object tntedOptionHandheldName: TTntEdit
                Left = 112
                Top = 56
                Width = 225
                Height = 21
                Color = clInfoBk
                TabOrder = 14
                OnChange = droidwareNameChange
              end
              object tntedOptionReceiptName: TTntEdit
                Tag = 1
                Left = 112
                Top = 84
                Width = 225
                Height = 21
                Color = clInfoBk
                TabOrder = 15
                Visible = False
                OnChange = droidwareNameChange
              end
            end
            object tsAvailableSizes: TTabSheet
              ImageIndex = 6
              TabVisible = False
              DesignSize = (
                338
                450)
              object GroupBox9: TGroupBox
                Left = 0
                Top = 0
                Width = 337
                Height = 446
                Anchors = [akLeft, akTop, akBottom]
                Caption = 'Available Sizes'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 0
                DesignSize = (
                  337
                  446)
                object lbAvailableSizes: TListBox
                  Left = 16
                  Top = 24
                  Width = 225
                  Height = 406
                  Anchors = [akLeft, akTop, akBottom]
                  Color = clInfoBk
                  ExtendedSelect = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ItemHeight = 13
                  ParentFont = False
                  TabOrder = 0
                end
                object btnAddSize: TBitBtn
                  Left = 248
                  Top = 24
                  Width = 73
                  Height = 25
                  Caption = 'New'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  OnClick = btnAddSizeClick
                end
                object btnSizesDelete: TBitBtn
                  Left = 248
                  Top = 88
                  Width = 73
                  Height = 25
                  Caption = 'Delete'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  OnClick = btnSizesDeleteClick
                end
                object btnSizesEdit: TBitBtn
                  Left = 248
                  Top = 56
                  Width = 73
                  Height = 25
                  Caption = 'Edit'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  OnClick = btnSizesEditClick
                end
              end
            end
            object tsCategories: TTabSheet
              ImageIndex = 7
              TabVisible = False
              DesignSize = (
                338
                450)
              object Label33: TLabel
                Left = 0
                Top = 0
                Width = 89
                Height = 13
                Caption = 'Category Group'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object lbeCategoryGroup: TLabel
                Left = 136
                Top = 0
                Width = 77
                Height = 13
                Caption = 'Category Group'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object GroupBox10: TGroupBox
                Left = 0
                Top = 56
                Width = 337
                Height = 390
                Anchors = [akLeft, akTop, akBottom]
                Caption = 'Categories'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 0
                DesignSize = (
                  337
                  390)
                object lbAvailableCategories: TListBox
                  Left = 16
                  Top = 24
                  Width = 201
                  Height = 350
                  Anchors = [akLeft, akTop, akBottom]
                  Color = clInfoBk
                  ExtendedSelect = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ItemHeight = 13
                  ParentFont = False
                  TabOrder = 0
                  OnKeyDown = lbAvailableCategoriesKeyDown
                end
                object btnNewCategory: TBitBtn
                  Left = 224
                  Top = 24
                  Width = 97
                  Height = 25
                  Caption = 'New Category'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  OnClick = btnNewCategoryClick
                end
                object btnDeleteCategory: TBitBtn
                  Left = 224
                  Top = 88
                  Width = 97
                  Height = 25
                  Caption = 'Delete / Undelete'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  OnClick = btnDeleteCategoryClick
                end
                object btnEditCategory: TBitBtn
                  Left = 224
                  Top = 56
                  Width = 97
                  Height = 25
                  Caption = 'Edit Category'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  Visible = False
                  OnClick = btnEditCategoryClick
                end
              end
              object btnDeleteCategoryGroup2: TBitBtn
                Left = 240
                Top = 24
                Width = 97
                Height = 25
                Caption = 'Delete Group'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnClick = btnDeleteCategoryGroup2Click
              end
              object btnEditCategoryGroup2: TBitBtn
                Left = 136
                Top = 24
                Width = 97
                Height = 25
                Caption = 'Edit Group'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnClick = btnEditCategoryGroup2Click
              end
            end
            object tsSize: TTabSheet
              Caption = 'tsSize'
              ImageIndex = 8
              TabVisible = False
              object Label21: TLabel
                Left = 0
                Top = 4
                Width = 49
                Height = 13
                Caption = 'Size Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label22: TLabel
                Left = 0
                Top = 32
                Width = 65
                Height = 13
                Caption = 'Kitchen Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object lblSizeHandheldName: TLabel
                Left = 0
                Top = 60
                Width = 75
                Height = 13
                Caption = 'Handheld Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object lblSizeReceiptName: TLabel
                Left = 0
                Top = 88
                Width = 66
                Height = 13
                Caption = 'Receipt Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object edSizeName: TEdit
                Left = 88
                Top = 0
                Width = 249
                Height = 21
                Color = clBtnFace
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                ReadOnly = True
                TabOrder = 0
                OnKeyPress = edKeyPress
              end
              object tntedSizeKitchenName: TTntEdit
                Left = 88
                Top = 28
                Width = 249
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Shell Dlg 2'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnChange = tntedSizeKitchenNameChange
                OnKeyPress = edKeyPress
              end
              object btnSizeDeleteSize: TButton
                Left = 232
                Top = 115
                Width = 101
                Height = 25
                Caption = 'Delete Size'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnClick = btnSizeDeleteSizeClick
              end
              object chbSizeWeighed: TCheckBox
                Left = 0
                Top = 115
                Width = 101
                Height = 17
                Hint = 
                  'Select this option if you want the POS to dispay the scales inte' +
                  'rface and calculate a price dependant on its weight'
                Alignment = taLeftJustify
                Caption = 'Weighed (kg)'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 3
                OnClick = chbSizeWeighedClick
              end
              object tntedSizeHandheldName: TTntEdit
                Left = 88
                Top = 56
                Width = 249
                Height = 21
                Color = clInfoBk
                TabOrder = 4
                OnChange = droidwareNameChange
              end
              object tntedSizeReceiptName: TTntEdit
                Tag = 1
                Left = 88
                Top = 88
                Width = 249
                Height = 21
                Color = clInfoBk
                TabOrder = 5
                Visible = False
                OnChange = droidwareNameChange
              end
            end
            object tsCategoryGroups: TTabSheet
              Caption = 'tsCategoryGroups'
              ImageIndex = 9
              TabVisible = False
              DesignSize = (
                338
                450)
              object GroupBox3: TGroupBox
                Left = 0
                Top = 0
                Width = 337
                Height = 446
                Anchors = [akLeft, akTop, akBottom]
                Caption = 'Category Groups'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 0
                DesignSize = (
                  337
                  446)
                object lbCategoryGroups: TListBox
                  Left = 16
                  Top = 24
                  Width = 201
                  Height = 404
                  Anchors = [akLeft, akTop, akBottom]
                  Color = clInfoBk
                  ExtendedSelect = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ItemHeight = 13
                  ParentFont = False
                  TabOrder = 0
                end
                object btnNewCategoryGroup: TBitBtn
                  Left = 224
                  Top = 24
                  Width = 97
                  Height = 25
                  Caption = 'New Group'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  OnClick = btnNewCategoryGroupClick
                end
                object btnDeleteCategoryGroup: TBitBtn
                  Left = 224
                  Top = 88
                  Width = 97
                  Height = 25
                  Caption = 'Delete Group'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  OnClick = btnDeleteCategoryGroupClick
                end
                object btnEditCategoryGroup: TBitBtn
                  Left = 224
                  Top = 56
                  Width = 97
                  Height = 25
                  Caption = 'Edit Group'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  OnClick = btnEditCategoryGroupClick
                end
              end
            end
            object tsServingCourses: TTabSheet
              Caption = 'ServingCourses'
              ImageIndex = 11
              TabVisible = False
              object GroupBox6: TGroupBox
                Left = 0
                Top = 0
                Width = 338
                Height = 450
                Align = alClient
                Caption = 'Available Serving Courses '
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 0
                DesignSize = (
                  338
                  450)
                object Label37: TLabel
                  Left = 261
                  Top = 271
                  Width = 53
                  Height = 13
                  Caption = 'Print Order '
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  WordWrap = True
                end
                object btnNewServingCourse: TBitBtn
                  Left = 254
                  Top = 24
                  Width = 73
                  Height = 25
                  Caption = 'New'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 0
                  OnClick = btnNewServingCourseClick
                end
                object btnServingCoursesEdit: TBitBtn
                  Left = 254
                  Top = 56
                  Width = 73
                  Height = 25
                  Caption = 'Edit'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  OnClick = btnServingCoursesEditClick
                end
                object btnUpServingCourse: TBitBtn
                  Left = 254
                  Top = 297
                  Width = 73
                  Height = 25
                  Caption = 'Up'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  OnClick = btnUpServingCourseClick
                end
                object btnDownServingCourse: TBitBtn
                  Left = 254
                  Top = 338
                  Width = 73
                  Height = 25
                  Caption = 'Down'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  OnClick = btnDownServingCourseClick
                end
                object btnServingCoursesDelete: TBitBtn
                  Left = 255
                  Top = 90
                  Width = 73
                  Height = 25
                  Caption = 'Delete'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 4
                  OnClick = btnServingCoursesDeleteClick
                end
                object lbAvailableServingCourses: TListBox
                  Left = 16
                  Top = 24
                  Width = 225
                  Height = 413
                  Style = lbOwnerDrawFixed
                  Anchors = [akLeft, akTop, akBottom]
                  Color = clInfoBk
                  ExtendedSelect = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ItemHeight = 13
                  ParentFont = False
                  TabOrder = 5
                  OnDrawItem = lbAvailableServingCoursesDrawItem
                  OnEnter = lbAvailableServingCoursesEnter
                  OnKeyDown = lbAvailableServingCoursesKeyDown
                end
                object cbServingCoursesEnable: TCheckBox
                  Left = 260
                  Top = 172
                  Width = 65
                  Height = 17
                  Caption = 'Available'
                  Checked = True
                  Color = clBtnFace
                  Enabled = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  State = cbChecked
                  TabOrder = 6
                  Visible = False
                  OnClick = cbServingCoursesEnableClick
                end
              end
            end
            object tsServingCourse: TTabSheet
              Caption = 'tsServingCourse'
              ImageIndex = 12
              TabVisible = False
              object Label38: TLabel
                Left = 0
                Top = 3
                Width = 103
                Height = 13
                Caption = 'Serving Course Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label39: TLabel
                Left = 0
                Top = 32
                Width = 65
                Height = 13
                Caption = 'Kitchen Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object Label40: TLabel
                Left = 0
                Top = 62
                Width = 31
                Height = 13
                Caption = 'Colour'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object edServingCourseName: TEdit
                Left = 110
                Top = 0
                Width = 225
                Height = 21
                Color = clBtnFace
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                ReadOnly = True
                TabOrder = 0
                OnKeyPress = edKeyPress
              end
              object tntedServingCourseKitchenName: TTntEdit
                Left = 111
                Top = 28
                Width = 224
                Height = 21
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Shell Dlg 2'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnChange = tntedServingCourseKitchenNameChange
                OnKeyPress = edKeyPress
              end
              object btnServingCourseDelete: TButton
                Left = 222
                Top = 118
                Width = 114
                Height = 25
                Caption = 'Delete Serving Course'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
                OnClick = btnServingCourseDeleteClick
                OnKeyPress = edKeyPress
              end
              object cbServingCourseEnable: TCheckBox
                Left = 0
                Top = 121
                Width = 134
                Height = 17
                Caption = 'Available in this Menu'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = cbServingCourseEnableClick
                OnKeyPress = edKeyPress
              end
              object cbTextColor: TColorBox
                Left = 111
                Top = 57
                Width = 225
                Height = 22
                Style = [cbStandardColors, cbExtendedColors, cbCustomColor, cbPrettyNames]
                Color = clInfoBk
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ItemHeight = 16
                ParentFont = False
                TabOrder = 2
                OnChange = cbTextColorChange
                OnKeyPress = edKeyPress
              end
              object btnSyncServingCourse: TButton
                Left = 223
                Top = 421
                Width = 114
                Height = 25
                Caption = 'Sync Menu'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 5
                OnClick = btnSyncServingCourseClick
              end
              object cbSelectable: TCheckBox
                Left = 0
                Top = 91
                Width = 129
                Height = 17
                Caption = 'Selectable on the POS'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 6
                OnClick = cbSelectableClick
                OnKeyPress = edKeyPress
              end
            end
            object tsCategory: TTabSheet
              Caption = 'tsCategory'
              ImageIndex = 10
              TabVisible = False
              object Label36: TLabel
                Left = 3
                Top = 4
                Width = 75
                Height = 13
                Caption = 'Category Name'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object lblGlCode: TLabel
                Left = 3
                Top = 36
                Width = 40
                Height = 13
                Caption = 'GL Code'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
              end
              object btnCategoryDelete: TBitBtn
                Left = 240
                Top = 69
                Width = 97
                Height = 25
                Caption = 'Delete Category'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnClick = btnCategoryDeleteClick
              end
              object edCategoryName: TEdit
                Left = 88
                Top = 1
                Width = 249
                Height = 21
                Color = clBtnFace
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                ReadOnly = True
                TabOrder = 1
                OnKeyPress = edKeyPress
              end
              object edGlCode: TEdit
                Left = 89
                Top = 33
                Width = 249
                Height = 21
                Color = clWhite
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnChange = edGlCodeChange
                OnEnter = edGlCodeEnter
                OnExit = edGlCodeExit
                OnKeyPress = edKeyPress
              end
            end
          end
        end
        object tsErrorsWarnings: TTabSheet
          Caption = 'Errors / Warnings'
          ImageIndex = 1
          TabVisible = False
          DesignSize = (
            346
            460)
          object btnErrorWarningSettings: TButton
            Left = 264
            Top = 429
            Width = 73
            Height = 25
            Anchors = [akRight, akBottom]
            Caption = 'Settings'
            TabOrder = 0
            OnClick = btnErrorWarningSettingsClick
          end
          object lbeErrorsCount: TStaticText
            Left = 8
            Top = 8
            Width = 161
            Height = 21
            AutoSize = False
            BorderStyle = sbsSunken
            Caption = ' Errors: 0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object lbeWarningsCount: TStaticText
            Left = 176
            Top = 8
            Width = 161
            Height = 21
            AutoSize = False
            BorderStyle = sbsSunken
            Caption = ' Warnings: 0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pbWarnings: TProgressBar
            Left = 8
            Top = 431
            Width = 249
            Height = 21
            Anchors = [akLeft, akBottom]
            Min = 0
            Max = 100
            TabOrder = 3
          end
          object lbWarnings: TListBox
            Left = 8
            Top = 40
            Width = 329
            Height = 382
            Style = lbOwnerDrawFixed
            Anchors = [akLeft, akTop, akRight, akBottom]
            Color = clInfoBk
            ItemHeight = 17
            TabOrder = 4
            OnDblClick = lbWarningsDblClick
            OnDrawItem = lbWarningsDrawItem
          end
        end
      end
      object btnNewCourse: TButton
        Left = 96
        Top = 48
        Width = 81
        Height = 25
        Caption = 'New Course'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btnNewCourseClick
      end
      object btnNewItem: TButton
        Left = 184
        Top = 48
        Width = 75
        Height = 25
        Caption = 'New Item'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = btnNewItemClick
      end
      object btnNewOption: TButton
        Left = 266
        Top = 48
        Width = 81
        Height = 25
        Caption = 'New Option'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = btnNewOptionClick
      end
      object btnNewMenu: TButton
        Left = 8
        Top = 48
        Width = 81
        Height = 25
        Caption = 'New Menu'
        TabOrder = 0
        OnClick = btnNewMenuClick
      end
    end
    object Panel3: TPanel
      Left = 5
      Top = 548
      Width = 657
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        657
        45)
      object LoadProgress: TProgressBar
        Left = 8
        Top = 8
        Width = 644
        Height = 33
        Anchors = [akLeft, akTop, akRight]
        Min = 0
        Max = 100
        TabOrder = 0
        Visible = False
      end
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 657
        Height = 45
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        DesignSize = (
          657
          45)
        object btnFind: TBitBtn
          Left = 8
          Top = 8
          Width = 73
          Height = 33
          Caption = '&Find'
          TabOrder = 0
          OnClick = btnFindClick
          Glyph.Data = {
            42010000424D4201000000000000760000002800000011000000110000000100
            040000000000CC00000000000000000000001000000010000000000000000000
            BF0000BF000000BFBF00BF000000BF00BF00BFBF0000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
            77777000000077777777777777777000000070000077777000007000000070B0
            00777770F0007000000070F000777770B0007000000070000000700000007000
            0000700B000000B0000070000000700F000700F0000070000000700B000700B0
            0000700000007700000000000007700000007770B00070B00077700000007770
            0000700000777000000077770007770007777000000077770B07770B07777000
            0000777700077700077770000000777777777777777770000000777777777777
            777770000000}
        end
        object btnSavMenu: TBitBtn
          Left = 184
          Top = 8
          Width = 89
          Height = 33
          Caption = 'Export File'
          TabOrder = 1
          OnClick = btnSavMenuClick
          Glyph.Data = {
            36030000424D3603000000000000360000002800000010000000100000000100
            1800000000000003000000000000000000000000000000000000FFFFFF000000
            0000000000000000000000000000000000000000000000000000000000000000
            00000000000000FFFFFF0000004040402020208080804040404040409F9F9F80
            8080808080808080808080808080404040404040606060000000000000404040
            000000808080000000000000BFBFBF8080808080808080808080808080804040
            40000000404040000000000000404040000000808080000000000000BFBFBF80
            8080808080808080808080808080404040000000404040000000000000404040
            0000006060604040404040409F9F9F8080808080808080808080808080802020
            2000000040404000000000000040404000000000000000000000000000000000
            0000000000000000000000000000000000000000404040000000000000404040
            0000006060606060606060606060606060606060606060606060606060606060
            60000000404040000000000000404040000000C0C0C0EFEFEFC0C0C0DFDFDFEF
            EFEFDFDFDFCFCFCFEFEFEFEFEFEFCFCFCF000000404040000000000000404040
            000000C0C0C0CFCFCFEFEFEFC0C0C0DFDFDFEFEFEFDFDFDFDFDFDFDFDFDFDFDF
            DF000000404040000000000000404040000000CFCFCFDFDFDFEFEFEFDFDFDFDF
            DFDFEFEFEFCFCFCFEFEFEFEFEFEFCFCFCF000000404040000000000000404040
            000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0000000404040000000000000404040000000DF60607F7F7F7F7F7FFF00007F
            7F7F7F7F7FFF0000C0C0C0C0C0C0C0C0C0000000404040000000000000404040
            000000DF606000FFFF7F7F7F7F7F7F00FFFF7F7F7F7F7F7FC0C0C0C0C0C0C0C0
            C0000000404040000000000000404040000000DF606000FFFF3FBFBFBF3F3F00
            FFFF3FBFBFBF3F3FC0C0C0C0C0C0C0C0C0000000404040000000000000404040
            404040EFAFAFFF7F7FFF7F7FFF7F7FFF7F7FFF7F7FFF7F7FDFDFDFDFDFDFDFDF
            DF404040606060000000FFFFFF00000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000FFFFFF}
        end
        object btnLoadMenu: TBitBtn
          Left = 88
          Top = 8
          Width = 89
          Height = 33
          Caption = 'Load Menu'
          TabOrder = 2
          OnClick = btnLoadMenuClick
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00550000000005
            555555777777777FF5555500000000805555557777777777FF555550BBBBB008
            05555557F5FFF7777FF55550B000B03080555557F77757F777F55550BBBBB033
            00555557F55557F577555550BBBBB03305555557FFFFF7F57F55555000000033
            05555557777777F57F555550BBBBB03305555557F5FFF7F57F555550B000B033
            05555557F77757F57F555550BBBBB03305555557F55557F57F555550BBBBB033
            05555557FFFFF7FF7F55550000000003055555777777777F7F55550777777700
            05555575FF5555777F5555500B3B3B300555555775FF55577FF555555003B3B3
            005555555775FFFF77F555555570000000555555555777777755}
          NumGlyphs = 2
        end
        object btnPreview: TBitBtn
          Left = 280
          Top = 8
          Width = 81
          Height = 33
          Caption = 'Preview'
          TabOrder = 3
          OnClick = btnPreviewClick
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            0400000000000001000000000000000000001000000010000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00330000000000
            033333777777777773333330777777703333333773F333773333333330888033
            33333FFFF7FFF7FFFFFF0000000000000003777777777777777F0FFFFFFFFFF9
            FF037F3333333337337F0F78888888887F037F33FFFFFFFFF37F0F7000000000
            8F037F3777777777F37F0F70AAAAAAA08F037F37F3333337F37F0F70ADDDDDA0
            8F037F37F3333337F37F0F70A99A99A08F037F37F3333337F37F0F70A99A99A0
            8F037F37F3333337F37F0F70AAAAAAA08F037F37FFFFFFF7F37F0F7000000000
            8F037F3777777777337F0F77777777777F037F3333333333337F0FFFFFFFFFFF
            FF037FFFFFFFFFFFFF7F00000000000000037777777777777773}
          NumGlyphs = 2
        end
        object btnCheck: TBitBtn
          Left = 368
          Top = 8
          Width = 81
          Height = 33
          Caption = 'Check'
          TabOrder = 4
          OnClick = btnCheckClick
          Glyph.Data = {
            36030000424D3603000000000000360000002800000010000000100000000100
            1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
            6060608080808080808080808080808080808080808080808080808080808080
            80808080808080202020002020009F9F007F7F007F7F007F7F007F7F007F7F00
            7F7F007F7F007F7F007F7F007F7F007F7F50505080808080808000BFBF00FFFF
            00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
            FF30EFEF40404080808000BFBF00FFFF00FFFF00FFFF00FFFF00FFFF30AFAF00
            000030AFAF00FFFF00FFFF00FFFF00FFFF30EFEF40404040404000404000FFFF
            00FFFF00FFFF00FFFF00FFFF30AFAF00000030AFAF00FFFF00FFFF00FFFF00FF
            FF306F6F808080FFFFFFFFFFFF00BFBF00FFFF00FFFF00FFFF00FFFF00FFFF00
            BFBF00FFFF00FFFF00FFFF00FFFF30EFEF404040404040FFFFFFFFFFFF004040
            00FFFF00FFFF00FFFF00FFFF30CFCF30505000FFFF00FFFF00FFFF00FFFF306F
            6F808080FFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FFFF00FFFF00FFFF306F6F00
            000030EFEF00FFFF00FFFF30EFEF404040404040FFFFFFFFFFFFFFFFFFFFFFFF
            00404000FFFF00FFFF00FFFF002020000000009F9F00FFFF00FFFF306F6F8080
            80FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FFFF00FFFF00000000
            0000007F7F00FFFF30EFEF404040404040FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFF00404000FFFF00FFFF000000000000007F7F00FFFF306F6F808080FFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FFFF30AFAF00
            7F7F30EFEF30EFEF404040404040FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFF00404000FFFF00FFFF00FFFF00FFFF306F6F808080FFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00BFBF00FFFF00
            FFFF30EFEF404040404040FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFF00404000FFFF00FFFF306F6F606060FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF007F7F30
            AFAF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        end
        object btnCommit: TBitBtn
          Left = 462
          Top = 8
          Width = 81
          Height = 33
          Caption = 'Commit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btnCommitClick
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333333333333333330000333333333333333333333333F33333333333
            00003333344333333333333333388F3333333333000033334224333333333333
            338338F3333333330000333422224333333333333833338F3333333300003342
            222224333333333383333338F3333333000034222A22224333333338F338F333
            8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
            33333338F83338F338F33333000033A33333A222433333338333338F338F3333
            0000333333333A222433333333333338F338F33300003333333333A222433333
            333333338F338F33000033333333333A222433333333333338F338F300003333
            33333333A222433333333333338F338F00003333333333333A22433333333333
            3338F38F000033333333333333A223333333333333338F830000333333333333
            333A333333333333333338330000333333333333333333333333333333333333
            0000}
          NumGlyphs = 2
        end
        object btnClose: TBitBtn
          Left = 571
          Top = 8
          Width = 81
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Close'
          ModalResult = 1
          TabOrder = 6
          OnClick = btnCloseClick
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00388888888877
            F7F787F8888888888333333F00004444400888FFF444448888888888F333FF8F
            000033334D5007FFF4333388888888883338888F0000333345D50FFFF4333333
            338F888F3338F33F000033334D5D0FFFF43333333388788F3338F33F00003333
            45D50FEFE4333333338F878F3338F33F000033334D5D0FFFF43333333388788F
            3338F33F0000333345D50FEFE4333333338F878F3338F33F000033334D5D0FFF
            F43333333388788F3338F33F0000333345D50FEFE4333333338F878F3338F33F
            000033334D5D0EFEF43333333388788F3338F33F0000333345D50FEFE4333333
            338F878F3338F33F000033334D5D0EFEF43333333388788F3338F33F00003333
            4444444444333333338F8F8FFFF8F33F00003333333333333333333333888888
            8888333F00003333330000003333333333333FFFFFF3333F00003333330AAAA0
            333333333333888888F3333F00003333330000003333333333338FFFF8F3333F
            0000}
          NumGlyphs = 2
        end
      end
    end
    object tvMenu: TTreeView
      Left = 5
      Top = 5
      Width = 297
      Height = 543
      Align = alClient
      BevelOuter = bvNone
      Color = clInfoBk
      Ctl3D = True
      DragCursor = crArrow
      DragMode = dmAutomatic
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      HideSelection = False
      Images = dmUtilities.imlTreeImages
      Indent = 19
      ParentCtl3D = False
      ParentFont = False
      PopupMenu = pmDrinksBevs
      StateImages = dmUtilities.imlTreeStateImages
      TabOrder = 2
      OnChange = tvMenuChange
      OnChanging = tvMenuChanging
      OnDeletion = tvMenuDeletion
      OnDragDrop = tvMenuDragDrop
      OnDragOver = tvMenuDragOver
      OnEditing = tvMenuEditing
      OnEndDrag = tvMenuEndDrag
      OnEnter = tvMenuEnter
      OnGetImageIndex = tvMenuGetImageIndex
      OnGetSelectedIndex = tvMenuGetSelectedIndex
      OnKeyDown = tvMenuKeyDown
      OnMouseDown = tvMenuMouseDown
    end
    object Panel1: TPanel
      Left = 302
      Top = 5
      Width = 4
      Height = 543
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 3
    end
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 32
    Top = 424
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'xml'
    Filter = 'Menu Files (*.xml)|*.xml|All Files (*.*)|*.*'
    Left = 192
    Top = 424
  end
  object pmDrinksBevs: TPopupMenu
    OnPopup = pmDrinksBevsPopup
    Left = 32
    Top = 368
    object Expand1: TMenuItem
      Caption = 'Expand'
      OnClick = Expand1Click
    end
    object Collapse1: TMenuItem
      Caption = 'Collapse'
      OnClick = Collapse1Click
    end
    object N10: TMenuItem
      Caption = '-'
    end
    object miDeleteDrinks: TMenuItem
      Caption = '&Delete'
      OnClick = miDeleteDrinksClick
    end
    object miDuplicateDrink: TMenuItem
      Caption = 'Du&plicate'
      OnClick = miDuplicateDrinkClick
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'csv'
    Filter = 'Menu Files (*.xml)|*.xml|All Files (*.*)|*.*'
    Left = 112
    Top = 424
  end
  object pmForcedSides: TPopupMenu
    OnPopup = pmDrinksBevsPopup
    Left = 112
    Top = 368
    object miMoveSideUp: TMenuItem
      Caption = 'Move Up'
      OnClick = miMoveSideUpClick
    end
    object MenuItem3: TMenuItem
      Caption = '-'
    end
    object miDeleteSide: TMenuItem
      Caption = '&Delete'
      OnClick = btnRemoveSideClick
    end
  end
  object MMTransaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 32
    Top = 16
  end
  object StockTransaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 32
    Top = 64
  end
  object qrMenuCount: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'Count(ItemSize.ItemSize_Key)'
      'From'
      #9'Menu Inner Join Course on'
      #9#9'Menu.Menu_Key =  Course.Menu_Key'
      #9'Inner Join Item on'
      #9#9'Course.Course_Key = Item.Course_Key'
      #9'Inner Join ItemSize on'
      #9#9'Item.Item_Key = ItemSize.Item_Key'
      'Where'
      #9'Menu.Menu_Key = :Menu_Key')
    Transaction = MMTransaction
    Left = 192
    Top = 16
  end
  object qrSizes: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'Sizes_Key,'
      #9'Size_Name,'
      #9'SIze_Kitchen_Name'
      'From'
      #9'Sizes'
      'Where'
      #9'Menu_Key = :Menu_Key'
      'Order By'
      #9'Size_Name')
    Transaction = MMTransaction
    Left = 168
    Top = 72
  end
  object qrCategories: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'CategoryGroups.CategoryGroups_Key,'
      #9'CategoryGroups.Name Category_Group_Name,'
      #9'ArcCategories.Category_Key,'
      #9'ArcCategories.Category'
      'From'
      #9'ArcCategories Left Join CategoryGroups On'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'ArcCategories.Visible = '#39'T'#39
      'Order By'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category')
    Transaction = MMTransaction
    Left = 224
    Top = 72
  end
  object qrMenu: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'Menu.Menu_Key,'
      #9'Menu.Menu_Name,'
      #9'Menu.Menu_Type,'
      ''
      #9'Course.Course_Key,'
      #9'Course.Course_Name,'
      #9'Course.Course_Kitchen_Name,'
      ''
      #9'Item.Item_Key,'
      #9'Item.Item_Name,'
      #9'Item.Item_Kitchen_Name,'
      #9'Item.Enabled,'
      #9'Item.Button_Colour,'
      #9'Item.Item_Category,'
      #9'Item.Exclusively_As_Side,'
      ''
      #9'ItemSize.ItemSize_Key,'
      #9'ItemSize.Size_Name,'
      #9'ItemSize.Price,'
      #9'ItemSize.Special_Price,'
      #9'ItemSize.GST_Percent,'
      #9'ItemSize.Cost,'
      #9'ItemSize.Cost_GST_Percent,'
      #9'ItemSize.Free,'
      #9'ItemSize.Points_Percent,'
      #9'ItemSize.Setmenu_Mask,'
      #9'ItemSize.Available_As_Standard,'
      #9'ItemSize.No_Recipe,'
      #9'ItemSize.Barcode,'
      #9'ItemSize.Mem_Sale_Count,'
      #9'ItemSize.Mem_Discount_Percent,'
      #9'ItemSize.Loc_Sale_Count,'
      #9'ItemSize.Loc_Discount_Percent,'
      #9'ArcCategories.Category,'
      #9'CategoryGroups.Name Category_Group_Name,'
      ''
      #9'Recipe.Recipe_Key,'
      #9'Recipe.Stock_Code,'
      #9'Recipe.Qty,'
      #9'Recipe.Stock_Location,'
      #9'Recipe.Stock_Location Order_Location'
      'From'
      #9'Menu Left Join Course on'
      #9#9'Menu.Menu_Key =  Course.Menu_Key'
      #9'Left Join Item on'
      #9#9'Course.Course_Key = Item.Course_Key'
      #9'Left Join ItemSize on'
      #9#9'Item.Item_Key = ItemSize.Item_Key'
      #9'Left Join ArcCategories on'
      #9#9'ItemSize.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGrou' +
        'ps_Key'
      #9'Left Join Recipe On'
      #9#9'ItemSize.ItemSize_Key = Recipe.ItemSize_Key'
      'Where'
      #9'Menu.Menu_Key = :Menu_Key'
      'Order By'
      #9'Menu.Menu_Name,'
      #9'Course.CAO,'
      #9'Item.IAO,'
      #9'ItemSize.ISAO,'
      #9'Recipe.Stock_Code,'
      #9'Recipe.Stock_Location,'
      #9'Recipe.Stock_Code')
    Transaction = MMTransaction
    Left = 72
    Top = 136
  end
  object qrSizeCategories: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'ItemSizeCategory.ItemSize_Key,'
      #9'ArcCategories.Category'
      'From'
      #9'Menu Left Join Course on'
      #9#9'Menu.Menu_Key =  Course.Menu_Key'
      #9'Left Join Item on'
      #9#9'Course.Course_Key = Item.Course_Key'
      #9'Left Join ItemSize on'
      #9#9' ItemSize.Item_Key = Item.Item_Key'
      #9'Left Join ItemSizeCategory on'
      #9#9'ItemSizeCategory.ItemSize_Key = ItemSize.ItemSize_Key'
      #9'Left Join ArcCategories On'
      #9#9'ArcCategories.Category_Key = ItemSizeCategory.Category_Key'
      'Where'
      #9'Menu.Menu_Key = :Menu_Key  And'
      #9'ItemSize_Key Is Not NULL'
      'Order By'
      #9'Menu.Menu_Name,'
      #9'Course.CAO,'
      #9'Item.IAO,'
      #9'ItemSize.ISAO,'
      #9'ArcCategories.Category')
    Transaction = MMTransaction
    Left = 136
    Top = 136
  end
  object qrOptions: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      #9'Options_Key,'
      #9'Option_Name,'
      #9'Option_Kitchen_Name,'
      #9'Flags,'
      #9'Forced_Mask'
      'From'
      #9'Options'
      'Where'
      #9'Course_Key = :Course_Key'
      'Order By'
      #9'Option_Order')
    Transaction = MMTransaction
    Left = 104
    Top = 184
  end
  object qrStockDetails: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   Stock.GST_Percent,'
      '   Stock.REDUCTION_UNIT,'
      '   Stock.CONVERSION_FACTOR,'
      '   StockLocation.Latest_Cost'
      'From'
      '   Stock Left Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      'Where'
      '   Code = :Code And'
      '   StockLocation.Location = :Location')
    Transaction = StockTransaction
    Left = 40
    Top = 184
  end
  object qrMMLocations: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Name'
      'From'
      '   Locations'
      'Where'
      '   Location_Type <> 0'
      'Order By'
      '   Name')
    Transaction = MMTransaction
    Left = 200
    Top = 240
  end
  object qrItemSides: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSides.Group_No Group_Number,'
      '   ItemSides.Max_Select,'
      '   ItemSides.Allow_Skip'
      'From'
      '   ItemSides Left Join Item On'
      '      ItemSides.Item_Key = Item.Item_Key'
      '   Left Join Course On'
      '      Item.Course_Key = Course.Course_Key'
      'Where'
      '   ItemSides.Master_Item_Key = :Item_Key'
      'Order By'
      '   IOO')
    Transaction = MMTransaction
    Left = 168
    Top = 184
  end
  object FindDialog: TFindDialog
    Options = [frDown, frFindNext]
    OnFind = FindDialogFind
    Left = 192
    Top = 368
  end
  object IBQuery1: TIBQuery
    Left = 248
    Top = 152
  end
  object ActionList1: TActionList
    Left = 32
    Top = 16
  end
  object qrGetNoOfPriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      #9'SELECT '
      #9#9'INTEGER_VAL '
      #9'FROM '
      #9#9'VARIABLES'
      #9'WHERE '
      #9#9'VARIABLES_KEY = 5014;')
    Transaction = MMTransaction
    Left = 96
    Top = 272
  end
  object qrGetPriceLevelNames: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'SELECT a.PRICELEVEL_KEY, a.PRICELEVEL_NAME'
      'FROM PRICELEVELS a'
      'where a.ISENABLED = '#39'T'#39
      'order by a.PRICELEVEL_KEY')
    Transaction = MMTransaction
    Left = 112
    Top = 72
  end
  object qrGetAllPriceLevelNames: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'SELECT a.PRICELEVEL_KEY, a.PRICELEVEL_NAME'
      'FROM PRICELEVELS a'
      'order by a.PRICELEVEL_KEY')
    Transaction = MMTransaction
    Left = 72
    Top = 96
  end
  object qrLocations: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock.Stock_Key,'
      '   StockLocation.LOCATION'
      'From'
      '   Stock Left Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      'Where'
      '   Stock.CODE= :CODE'
      '')
    Transaction = StockTransaction
    Left = 40
    Top = 248
  end
  object qrGetTaxSettings: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      'SELECT      '
      
        'MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8000 THEN VARSPROFILE.' +
        'INTEGER_VAL END) AS IsTax,     '
      
        'MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8001 THEN VARSPROFILE.' +
        'INTEGER_VAL END) AS IsSeviceCharge '
      'FROM VARSPROFILE ')
    Transaction = MMTransaction
    Left = 144
    Top = 296
  end
end
