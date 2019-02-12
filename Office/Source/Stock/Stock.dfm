object frmStock: TfrmStock
  Tag = 1024
  Left = 471
  Top = 110
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Stock'
  ClientHeight = 437
  ClientWidth = 630
  Color = clBtnFace
  Constraints.MinHeight = 464
  Constraints.MinWidth = 356
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 630
    Height = 437
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel1: TPanel
      Left = 4
      Top = 4
      Width = 622
      Height = 429
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Splitter1: TSplitter
        Left = 236
        Top = 5
        Width = 4
        Height = 374
        Cursor = crHSplit
        Align = alRight
        MinSize = 80
        ResizeStyle = rsUpdate
        OnCanResize = Splitter1CanResize
        OnMoved = Splitter1Moved
      end
      object tvStock: TTreeView
        Left = 5
        Top = 5
        Width = 231
        Height = 374
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        Color = clInfoBk
        Constraints.MinWidth = 32
        DragCursor = crDefault
        DragMode = dmAutomatic
        HideSelection = False
        Images = dmUtilities.imlTreeImages
        Indent = 19
        RightClickSelect = True
        TabOrder = 0
        OnChange = tvStockChange
        OnDblClick = tvStockDblClick
        OnDeletion = tvStockDeletion
        OnDragDrop = tvStockDragDrop
        OnDragOver = tvStockDragOver
        OnEdited = tvStockEdited
        OnEditing = tvStockEditing
        OnEndDrag = tvStockEndDrag
        OnKeyDown = tvStockKeyDown
        OnKeyPress = tvStockKeyPress
        OnMouseDown = tvStockMouseDown
      end
      object Panel8: TPanel
        Left = 240
        Top = 5
        Width = 377
        Height = 374
        Align = alRight
        BevelOuter = bvLowered
        Constraints.MinWidth = 330
        TabOrder = 1
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 375
          Height = 48
          Align = alTop
          BevelOuter = bvNone
          Color = clAppWorkSpace
          TabOrder = 0
          object lbeTitle: TLabel
            Left = 16
            Top = 0
            Width = 297
            Height = 49
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
        object pcStockDetails: TPageControl
          Left = 1
          Top = 49
          Width = 375
          Height = 324
          ActivePage = tsDeletedItems
          Align = alClient
          Style = tsFlatButtons
          TabOrder = 1
          object tsStockGroup: TTabSheet
            TabVisible = False
            object Panel2: TPanel
              Left = 255
              Top = 0
              Width = 112
              Height = 314
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 0
              object Bevel1: TBevel
                Left = 8
                Top = 176
                Width = 97
                Height = 9
                Shape = bsBottomLine
              end
              object Label4: TLabel
                Left = 8
                Top = 160
                Width = 69
                Height = 13
                Caption = 'Stock Items'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Label3: TLabel
                Left = 8
                Top = 0
                Width = 75
                Height = 13
                Caption = 'Stock Groups'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Bevel2: TBevel
                Left = 7
                Top = 16
                Width = 97
                Height = 9
                Shape = bsBottomLine
              end
              object btnEditStock: TButton
                Left = 8
                Top = 224
                Width = 97
                Height = 25
                Caption = '&Edit Item'
                TabOrder = 5
                OnClick = btnEditStockClick
              end
              object btnAddStock: TButton
                Left = 8
                Top = 192
                Width = 97
                Height = 25
                Caption = '&Add Item'
                TabOrder = 4
                OnClick = btnAddStockClick
              end
              object btnDeleteStockGroup: TButton
                Left = 8
                Top = 96
                Width = 97
                Height = 25
                Caption = 'Dele&te Group'
                TabOrder = 3
                OnClick = btnDeleteStockGroupClick
              end
              object btnAddStockGroup: TButton
                Left = 8
                Top = 32
                Width = 97
                Height = 25
                Caption = 'Add &Group'
                TabOrder = 1
                OnClick = btnAddStockGroupClick
              end
              object btnRenameStockGroup: TButton
                Left = 8
                Top = 64
                Width = 97
                Height = 25
                Caption = 'Re&name Group'
                TabOrder = 2
                OnClick = btnRenameStockGroupClick
              end
              object btnMoveGroup: TButton
                Left = 8
                Top = 128
                Width = 97
                Height = 25
                Caption = '&Move Group'
                Enabled = False
                TabOrder = 0
              end
            end
            object lbStockItems: TListBox
              Left = 0
              Top = 0
              Width = 255
              Height = 314
              Style = lbOwnerDrawFixed
              Align = alClient
              BevelInner = bvNone
              BevelOuter = bvNone
              Color = clInfoBk
              ExtendedSelect = False
              ItemHeight = 16
              TabOrder = 1
              OnDblClick = lbStockItemsDblClick
              OnDrawItem = lbStockItemsDrawItem
            end
          end
          object tsStockItem: TTabSheet
            ImageIndex = 1
            TabVisible = False
            object Splitter2: TSplitter
              Left = 0
              Top = 153
              Width = 367
              Height = 4
              Cursor = crVSplit
              Align = alBottom
              MinSize = 157
              ResizeStyle = rsUpdate
              OnCanResize = Splitter2CanResize
            end
            object Panel5: TPanel
              Left = 0
              Top = 157
              Width = 367
              Height = 157
              Align = alBottom
              BevelOuter = bvNone
              Constraints.MinHeight = 157
              TabOrder = 0
              DesignSize = (
                367
                157)
              object Label5: TLabel
                Left = 72
                Top = 3
                Width = 118
                Height = 13
                Caption = '( * = Preferred Supplier)'
              end
              object Label2: TLabel
                Left = 8
                Top = 3
                Width = 52
                Height = 13
                Caption = 'Suppliers'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object btnAdd: TButton
                Left = 8
                Top = 127
                Width = 97
                Height = 25
                Anchors = [akLeft, akBottom]
                Caption = '&Add Item'
                TabOrder = 0
                OnClick = btnAddStockClick
              end
              object btnEdit: TBitBtn
                Left = 112
                Top = 127
                Width = 97
                Height = 25
                Anchors = [akLeft, akBottom]
                Caption = '&Edit Item'
                TabOrder = 1
                OnClick = btnEditClick
              end
              object btnDelete: TBitBtn
                Left = 216
                Top = 127
                Width = 97
                Height = 25
                Anchors = [akLeft, akBottom]
                Caption = '&Delete Item'
                TabOrder = 2
                OnClick = btnDeleteClick
              end
              object sgSuppliers: TStringGrid
                Left = 8
                Top = 19
                Width = 351
                Height = 102
                Anchors = [akLeft, akTop, akRight, akBottom]
                Color = clInfoBk
                ColCount = 4
                DefaultRowHeight = 18
                FixedCols = 0
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goRangeSelect, goColSizing, goRowMoving]
                TabOrder = 3
                OnDrawCell = GridDrawCell
                ColWidths = (
                  14
                  113
                  85
                  84)
              end
            end
            object Panel6: TPanel
              Left = 0
              Top = 0
              Width = 367
              Height = 153
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 1
              DesignSize = (
                367
                153)
              object Label21: TLabel
                Left = 8
                Top = 0
                Width = 66
                Height = 13
                Caption = 'Stock Code:'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object lbeStockCode: TLabel
                Left = 96
                Top = 0
                Width = 65
                Height = 13
                Caption = 'lbeStockCode'
              end
              object lbeUnit: TLabel
                Left = 96
                Top = 16
                Width = 33
                Height = 13
                Caption = 'lbeUnit'
              end
              object Label13: TLabel
                Left = 8
                Top = 16
                Width = 26
                Height = 13
                Caption = 'Unit:'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Label1: TLabel
                Left = 8
                Top = 32
                Width = 57
                Height = 13
                Caption = 'Locations:'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object sgLocations: TStringGrid
                Left = 8
                Top = 48
                Width = 351
                Height = 105
                Anchors = [akLeft, akTop, akRight, akBottom]
                Color = clInfoBk
                ColCount = 10
                DefaultRowHeight = 18
                FixedCols = 0
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goRangeSelect, goColSizing, goRowMoving]
                TabOrder = 0
                OnDrawCell = GridDrawCell
              end
              object pnlStocktake: TPanel
                Left = 207
                Top = 21
                Width = 157
                Height = 24
                BevelOuter = bvNone
                TabOrder = 1
                object Label6: TLabel
                  Left = 31
                  Top = 7
                  Width = 123
                  Height = 13
                  Caption = '= Initialised for Stocktake'
                end
                object Image1: TImage
                  Left = 6
                  Top = 6
                  Width = 20
                  Height = 19
                  Picture.Data = {
                    07544269746D617042020000424D420200000000000042000000280000001000
                    0000100000000100100003000000000200000000000000000000000000000000
                    0000007C0000E00300001F0000001F7C1F7C8C31104210421042104210421042
                    1042104210421042104210428410801000400041004100410041004100410041
                    00410041004100414A29104210420040007C007C007C007C007C007C007C007C
                    007C007C007C007C0040082110420040007C007C007C007C007C397FFF7F397F
                    007C007C007C007C0040082108210021007C007C007C007C007C397FFF7F397F
                    007C007C007C007C004110421F7C1F7C0040007C007C007C007C007C397F007C
                    007C007C007C0040082108211F7C1F7C0021007C007C007C007C397F397F007C
                    007C007C007C004110421F7C1F7C1F7C1F7C0040007C007C007C397FFF7F397F
                    007C007C0040082108211F7C1F7C1F7C1F7C0021007C007C007CFF7FFF7F397F
                    007C007C004110421F7C1F7C1F7C1F7C1F7C1F7C0040007C007CFF7FFF7F397F
                    007C0040082108211F7C1F7C1F7C1F7C1F7C1F7C0021007C007CFF7FFF7F397F
                    007C004110421F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0040007C397F397F007C
                    0040082108211F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0021007C007C007C007C
                    004110421F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0040007C007C0040
                    082108211F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C0021007C007C0041
                    8C311F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C1F7C004100410000
                    1F7C1F7C1F7C1F7C1F7C1F7C1F7C}
                  Transparent = True
                end
              end
            end
          end
          object tsStockCategory: TTabSheet
            Caption = 'tsStockCategory'
            ImageIndex = 2
            TabVisible = False
            object lbStockGroups: TListBox
              Left = 0
              Top = 0
              Width = 255
              Height = 314
              Style = lbOwnerDrawFixed
              Align = alClient
              BevelInner = bvNone
              BevelOuter = bvNone
              Color = clInfoBk
              ExtendedSelect = False
              ItemHeight = 16
              TabOrder = 0
              OnDblClick = lbStockGroupsDblClick
              OnDrawItem = lbStockGroupsDrawItem
            end
            object Panel4: TPanel
              Left = 255
              Top = 0
              Width = 112
              Height = 314
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 1
              object Label8: TLabel
                Left = 8
                Top = 168
                Width = 75
                Height = 13
                Caption = 'Stock Groups'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Bevel5: TBevel
                Left = 7
                Top = 184
                Width = 97
                Height = 9
                Shape = bsBottomLine
              end
              object Label9: TLabel
                Left = 8
                Top = 0
                Width = 96
                Height = 13
                Caption = 'Stock Categories'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Bevel6: TBevel
                Left = 8
                Top = 16
                Width = 97
                Height = 9
                Shape = bsBottomLine
              end
              object Button5: TButton
                Left = 8
                Top = 200
                Width = 97
                Height = 25
                Caption = 'Add &Group'
                TabOrder = 1
                OnClick = btnAddStockGroupClick
              end
              object btnAddCategory: TButton
                Left = 8
                Top = 32
                Width = 97
                Height = 25
                Caption = 'Add Categor&y'
                TabOrder = 0
                OnClick = btnAddCategoryClick
              end
              object btnRenameCategory: TButton
                Left = 8
                Top = 64
                Width = 97
                Height = 25
                Caption = '&Rename Category'
                TabOrder = 2
                OnClick = btnRenameCategoryClick
              end
              object btnDeleteCategory: TButton
                Left = 8
                Top = 96
                Width = 97
                Height = 25
                Caption = 'De&lete Category'
                TabOrder = 3
                OnClick = btnDeleteCategoryClick
              end
              object btnSortCategories: TButton
                Left = 8
                Top = 128
                Width = 97
                Height = 25
                Caption = 'Sort Categ&ories'
                TabOrder = 4
                OnClick = btnSortCategoriesClick
              end
              object btnSortStockGroups: TButton
                Left = 7
                Top = 264
                Width = 98
                Height = 25
                Caption = '&Sort Groups'
                TabOrder = 5
                OnClick = btnSortStockGroupsClick
              end
              object btnRenameStockGroup2: TButton
                Left = 8
                Top = 232
                Width = 97
                Height = 25
                Caption = 'Re&name Group'
                TabOrder = 6
                OnClick = btnRenameStockGroupClick
              end
            end
          end
          object tsDeletedItems: TTabSheet
            Caption = 'tsDeletedItems'
            ImageIndex = 3
            TabVisible = False
            object Panel7: TPanel
              Left = 255
              Top = 0
              Width = 112
              Height = 314
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 0
              object Label7: TLabel
                Left = 8
                Top = 0
                Width = 64
                Height = 13
                Caption = 'Deleted Bin'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object Bevel4: TBevel
                Left = 8
                Top = 16
                Width = 97
                Height = 9
                Shape = bsBottomLine
              end
              object Button7: TButton
                Left = 7
                Top = 104
                Width = 98
                Height = 25
                Caption = 'Delete'
                TabOrder = 0
                OnClick = btnDeleteClick
              end
              object btnRestore: TButton
                Left = 6
                Top = 32
                Width = 98
                Height = 25
                Caption = 'Restore'
                TabOrder = 1
                OnClick = btnRestoreClick
              end
              object Button2: TButton
                Left = 8
                Top = 184
                Width = 97
                Height = 25
                Caption = 'Add Categor&y'
                TabOrder = 2
                OnClick = btnAddCategoryClick
              end
              object btnRestoreAll: TButton
                Left = 6
                Top = 64
                Width = 98
                Height = 25
                Caption = 'Restore All'
                TabOrder = 3
                OnClick = btnRestoreAllClick
              end
              object btnEmptyDeletedBin: TButton
                Left = 6
                Top = 136
                Width = 98
                Height = 25
                Caption = 'Delete All'
                TabOrder = 4
                OnClick = btnEmptyDeletedBinClick
              end
            end
            object ListView1: TListView
              Left = 0
              Top = 0
              Width = 255
              Height = 314
              Align = alClient
              Color = clInfoBk
              Columns = <
                item
                  Caption = 'Description'
                  Width = 75
                end
                item
                  Caption = 'Category'
                  Width = 75
                end
                item
                  Caption = 'Group'
                  Width = 101
                end>
              HideSelection = False
              IconOptions.Arrangement = iaLeft
              RowSelect = True
              TabOrder = 1
              ViewStyle = vsReport
              OnDblClick = ListView1DblClick
            end
          end
          object tsDeletedItem: TTabSheet
            Caption = 'tsDeletedItem'
            ImageIndex = 4
            TabVisible = False
            object Button1: TButton
              Left = 6
              Top = 8
              Width = 98
              Height = 25
              Caption = 'Restore'
              TabOrder = 0
              OnClick = btnRestoreClick
            end
            object Button3: TButton
              Left = 7
              Top = 40
              Width = 98
              Height = 25
              Caption = 'Delete'
              TabOrder = 1
              OnClick = btnDeleteClick
            end
          end
        end
      end
      object Panel10: TPanel
        Left = 5
        Top = 379
        Width = 612
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        DesignSize = (
          612
          45)
        object btnClose: TBitBtn
          Left = 510
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Close'
          ModalResult = 1
          TabOrder = 2
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
        object btnOk: TBitBtn
          Left = 414
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&OK'
          Default = True
          TabOrder = 0
          OnClick = btnOkClick
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
        object btnCancel: TBitBtn
          Left = 510
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Cancel = True
          Caption = '&Cancel'
          TabOrder = 1
          OnClick = btnCancelClick
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            333333333333333333333333000033338833333333333333333F333333333333
            0000333911833333983333333388F333333F3333000033391118333911833333
            38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
            911118111118333338F3338F833338F3000033333911111111833333338F3338
            3333F8330000333333911111183333333338F333333F83330000333333311111
            8333333333338F3333383333000033333339111183333333333338F333833333
            00003333339111118333333333333833338F3333000033333911181118333333
            33338333338F333300003333911183911183333333383338F338F33300003333
            9118333911183333338F33838F338F33000033333913333391113333338FF833
            38F338F300003333333333333919333333388333338FFF830000333333333333
            3333333333333333333888330000333333333333333333333333333333333333
            0000}
          NumGlyphs = 2
        end
        object btnFind: TBitBtn
          Left = 8
          Top = 8
          Width = 89
          Height = 33
          Caption = '&Find'
          TabOrder = 3
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
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 160
    Top = 16
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    ParamCheck = False
    Transaction = Transaction
    Left = 160
    Top = 64
  end
  object qrStockSuppliers: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select Distinct'
      '   Contact.Company_Name Supplier_Name,'
      '   SupplierStock.Supplier_Code,'
      '   SupplierStock.Supplier_Unit,'
      '   SupplierStock.Latest_Cost,'
      '   SupplierStock.Preferred_Supplier'
      ''
      'From'
      '   SupplierStock Inner Join Contact on'
      '      SupplierStock.Supplier_Key = Contact.Contact_LK'
      'Where'
      '   SupplierStock.Stock_Key = :Stock_Key'
      'Order By'
      '   Contact.Company_Name Asc')
    Left = 408
    Top = 144
    ParamData = <
      item
        DataType = ftInteger
        Name = 'STOCK_KEY'
        ParamType = ptUnknown
        Size = 4
      end>
  end
  object qrStockLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      
        '   Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_' +
        'Unit_Size) As Numeric(15,4)) On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Initialised,'
      '   StockLocation.Sales_Pending,'
      '   StockLocation.WriteOffs_Pending'
      'From'
      '   Stock Inner Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Left Join PurchaseStock On'
      
        '      PurchaseStock.Code = Stock.Code And PurchaseStock.Location' +
        ' = StockLocation.Location'
      'Where'
      '   Stock.Stock_Key = :Stock_Key'
      'Group By'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level,'
      '   StockLocation.Initialised,'
      'StockLocation.Sales_Pending,'
      'StockLocation.WriteOffs_Pending'
      'Order By'
      '   Location')
    Left = 512
    Top = 152
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Stock_Key'
        ParamType = ptUnknown
      end>
  end
  object qrUpdateStock: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 56
    Top = 264
  end
  object qrUpdateStockGroup: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 56
    Top = 216
  end
  object pmTreeView: TPopupMenu
    OnPopup = pmTreeViewPopup
    Left = 240
    Top = 64
    object ExpandTree1: TMenuItem
      Caption = 'Expand Tree'
      OnClick = ExpandTree1Click
    end
    object CollapseTree1: TMenuItem
      Caption = 'Collapse Tree'
      OnClick = CollapseTree1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miNewStockCategory: TMenuItem
      Caption = 'New Stock Category'
      OnClick = miNewStockCategoryClick
    end
    object miNewStockGroup: TMenuItem
      Caption = 'New Stock Group'
      OnClick = miNewStockGroupClick
    end
    object miNewStockItem: TMenuItem
      Caption = 'New Stock Item'
      OnClick = miNewStockItemClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miEdit: TMenuItem
      Caption = 'Edit'
      OnClick = miEditClick
    end
    object miRename: TMenuItem
      Caption = 'Rename'
      OnClick = miRenameClick
    end
    object miDelete: TMenuItem
      Caption = 'Delete'
      OnClick = miDeleteClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miRestore: TMenuItem
      Caption = 'Restore'
      OnClick = miRestoreClick
    end
  end
  object tiTreeScroll: TTimer
    Enabled = False
    Interval = 150
    OnTimer = tiTreeScrollTimer
    Left = 240
    Top = 16
  end
  object FindDialog: TFindDialog
    Options = [frDown, frFindNext]
    OnFind = FindDialogFind
    Left = 16
    Top = 392
  end
  object qrFindStockGroup: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Group_Key'
      'From'
      '   StockGroup'
      'Where'
      '   Upper(Stock_Group) Like :Stock_Group And'
      '   Stock_Group_Key <> :Stock_Group_Key')
    Transaction = Transaction
    Left = 56
    Top = 64
  end
  object qrFindStockCategory: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Category_Key'
      'From'
      '   StockCategory'
      'Where'
      '   Upper(Stock_Category) = :Stock_Category And'
      '   Stock_Category_Key <> :Stock_Category_Key')
    Transaction = Transaction
    Left = 56
    Top = 16
  end
  object qrUpdateStockCategory: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 56
    Top = 168
  end
  object qrFindStock: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Stock_Key'
      'From'
      '   Stock'
      'Where'
      '   Upper(Description) Like :Description and'
      '   Stock_Key <> :Stock_Key')
    Transaction = Transaction
    Left = 56
    Top = 112
  end
  object qrStockList: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code Category_GL_Code,'
      '   StockCategory.Sort_Order Category_Sort_Order,'
      '   StockCategory.Deleted Category_Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      '   StockGroup.Deleted Group_Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted Item_Deleted,'
      '   '
      '   Max(StockLocation.Initialised) Initialised'
      ''
      'From'
      
        '   StockCategory Left Join StockGroup On StockCategory.Stock_Cat' +
        'egory_Key = StockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      'Group By'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code,'
      '   StockCategory.Sort_Order,'
      '   StockCategory.Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order,'
      '   StockGroup.Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      'Order by'
      '   StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order,'
      '   StockCategory.STOCK_CATEGORY,'
      '   STOCKGROUP.STOCK_GROUP,'
      '   Stock.Description')
    Transaction = Transaction
    Left = 160
    Top = 168
  end
  object qrGroupList: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockCategory.Stock_Category_Key,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      ''
      'From'
      
        '   StockGroup Left Join Stock On StockGroup.Stock_Group_Key = St' +
        'ock.Stock_Group_Key'
      
        '   Left Join StockCategory On StockGroup.Stock_Category_Key = St' +
        'ockCategory.Stock_Category_Key'
      'Where'
      '   Stock.Deleted Is Not Null And'
      '   StockGroup.Stock_Group_Key = :Stock_Group_Key'
      ''
      'Union All'
      ''
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockCategory.Stock_Category_Key,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Cast('#39'F'#39' As Char)'
      ''
      'From'
      
        '   StockGroup Left Join Stock On StockGroup.Stock_Group_Key = St' +
        'ock.Stock_Group_Key'
      
        '   Left Join StockCategory On StockGroup.Stock_Category_Key = St' +
        'ockCategory.Stock_Category_Key'
      'Where'
      '   Stock.Deleted Is Null And'
      '   StockGroup.Stock_Category_Key = :Stock_Category_Key'
      'Order by'
      '   12 Desc,'
      '   6,'
      '   8')
    Transaction = Transaction
    Left = 160
    Top = 264
  end
  object qrDeletedList: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code Category_GL_Code,'
      '   StockCategory.Sort_Order Category_Sort_Order,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      ''
      'From'
      
        '   StockCategory Left Join StockGroup On StockCategory.Stock_Cat' +
        'egory_Key = StockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      'Where'
      '   Stock.Deleted = '#39'T'#39' Or'
      '   StockGroup.Deleted = '#39'T'#39' Or'
      '   StockCategory.Deleted = '#39'T'#39
      'Order by'
      '   Stock.Description,'
      '   4,'
      '   10,'
      '   12')
    Transaction = Transaction
    Left = 160
    Top = 216
  end
  object qrStockRequestLocation: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      '     Select'
      '        StockCategory.Stock_Category_Key,'
      '        StockCategory.Stock_Category,'
      '        StockCategory.GL_Code Category_GL_Code,'
      '        StockCategory.Sort_Order Category_Sort_Order,'
      '        StockCategory.Deleted Category_Deleted,'
      '     '
      '        StockGroup.Stock_Group_Key,'
      '        StockGroup.Stock_Group,'
      '        StockGroup.GL_Code Group_GL_Code,'
      '        StockGroup.Stock_Category_Key,'
      '        StockGroup.Sort_Order Group_Sort_Order,'
      '        StockGroup.Deleted Group_Deleted,'
      '     '
      '        Stock.Stock_Key,'
      '        Stock.Description,'
      '        Stock.Code,'
      '        Stock.Stocktake_Unit,'
      '        Stock.Enabled,'
      '        Stock.Deleted Item_Deleted,'
      '       '
      '        Max(StockLocation.Initialised) Initialised,'
      '        StockLocation.LOCATION'
      '     '
      '     From'
      '      '
      
        '          StockCategory Left Join StockGroup On StockCategory.St' +
        'ock_Category_Key = StockGroup.Stock_Category_Key'
      '      '
      
        '          Left Join Stock On StockGroup.Stock_Group_Key = Stock.' +
        'Stock_Group_Key'
      '      '
      
        '          Left Join StockLocation On Stock.Stock_Key = StockLoca' +
        'tion.Stock_Key'
      '     Group By'
      '        StockCategory.Stock_Category_Key,'
      '        StockCategory.Stock_Category,'
      '        StockCategory.GL_Code,'
      '        StockCategory.Sort_Order,'
      '        StockCategory.Deleted,'
      '     '
      '        StockGroup.Stock_Group_Key,'
      '        StockGroup.Stock_Group,'
      '        StockGroup.GL_Code,'
      '        StockGroup.Stock_Category_Key,'
      '        StockGroup.Sort_Order,'
      '        StockGroup.Deleted,'
      '     '
      '        Stock.Stock_Key,'
      '        Stock.Description,'
      '        Stock.Code,'
      '        Stock.Stocktake_Unit,'
      '        Stock.Enabled,'
      '        Stock.Deleted,'
      '        StockLocation.LOCATION'
      '     Order by'
      '        StockCategory.Sort_Order,'
      '        StockGroup.Sort_Order,'
      '        UPPER(StockCategory.STOCK_CATEGORY),'
      '        UPPER(STOCKGROUP.STOCK_GROUP),'
      '        Stock.Description')
    Transaction = Transaction
    Left = 168
    Top = 112
  end
  object qrStockReqPO: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      '     Select'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code Category_GL_Code,'
      '   StockCategory.Sort_Order Category_Sort_Order,'
      '   StockCategory.Deleted Category_Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code Group_GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order Group_Sort_Order,'
      '   StockGroup.Deleted Group_Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted Item_Deleted,'
      '   '
      '   Max(StockLocation.Initialised) Initialised,'
      '   SUPPLIERSTOCK.SUPPLIER_KEY,'
      '   SUPPLIERSTOCK.PREFERRED_SUPPLIER,'
      '   SUPPLIERSTOCK.QTY,'
      '   SUPPLIERSTOCK.LATEST_COST,'
      
        '   cast(SUPPLIERSTOCK.LATEST_COST / SUPPLIERSTOCK.QTY as numeric' +
        '(15, 4)) UnitPrice'
      ''
      'From'
      
        '   StockCategory Left Join StockGroup On StockCategory.Stock_Cat' +
        'egory_Key = StockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      
        '   left join SUPPLIERSTOCK on Stock.Stock_Key=SUPPLIERSTOCK.STOC' +
        'K_KEY'
      'Group By'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code,'
      '   StockCategory.Sort_Order,'
      '   StockCategory.Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order,'
      '   StockGroup.Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted,  SUPPLIERSTOCK.SUPPLIER_KEY,'
      '   SUPPLIERSTOCK.PREFERRED_SUPPLIER,SUPPLIERSTOCK.QTY,'
      '   SUPPLIERSTOCK.LATEST_COST'
      ''
      'Order by'
      ' StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description')
    Transaction = Transaction
    Left = 160
    Top = 304
  end
  object qrUpdateStockFlag: TIBSQL
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 48
    Top = 312
  end
  object sqlAddLocal: TIBSQL
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    Left = 104
    Top = 336
  end
end
