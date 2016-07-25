object frmUpdateLocations: TfrmUpdateLocations
  Left = 154
  Top = 111
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Update Locations'
  ClientHeight = 433
  ClientWidth = 563
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 545
    Height = 417
    BorderWidth = 3
    TabOrder = 0
    object Panel13: TPanel
      Left = 444
      Top = 4
      Width = 97
      Height = 409
      Align = alRight
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object btnClose: TBitBtn
        Left = 8
        Top = 8
        Width = 81
        Height = 33
        Caption = 'Cancel'
        TabOrder = 0
        OnClick = btnCloseClick
        NumGlyphs = 2
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 4
      Width = 440
      Height = 409
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object Panel10: TPanel
        Left = 0
        Top = 0
        Width = 437
        Height = 409
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 0
        object pcReportProperties: TPageControl
          Left = 1
          Top = 1
          Width = 435
          Height = 407
          ActivePage = tsClashes
          Align = alClient
          Style = tsFlatButtons
          TabOrder = 0
          object tsSelectItems: TTabSheet
            ImageIndex = 3
            TabVisible = False
            DesignSize = (
              427
              397)
            object Bevel4: TBevel
              Left = 2
              Top = 352
              Width = 423
              Height = 9
              Shape = bsTopLine
            end
            object Panel11: TPanel
              Left = 0
              Top = 0
              Width = 427
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              BorderWidth = 8
              Color = clAppWorkSpace
              TabOrder = 0
              object lbeTypeTitle: TLabel
                Left = 8
                Top = 8
                Width = 411
                Height = 32
                Align = alClient
                Alignment = taCenter
                AutoSize = False
                Caption = 'Select the stock products you wish to update'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clCaptionText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                ShowAccelChar = False
                Layout = tlCenter
                WordWrap = True
              end
            end
            object btnTypePreview: TBitBtn
              Left = 328
              Top = 360
              Width = 97
              Height = 33
              Caption = 'Print Preview'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
            end
            object btnProductsRename: TBitBtn
              Left = 288
              Top = 360
              Width = 137
              Height = 33
              Caption = 'Rename Locations'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
              OnClick = btnProductsRenameClick
            end
            object tvFilterTree: TTreeView
              Left = 2
              Top = 56
              Width = 423
              Height = 289
              Anchors = [akLeft, akTop, akRight, akBottom]
              BevelInner = bvNone
              BevelOuter = bvNone
              Color = clInfoBk
              Constraints.MinWidth = 32
              DragCursor = crDefault
              HideSelection = False
              Images = dmUtilities.imlTreeImages
              Indent = 19
              ReadOnly = True
              RightClickSelect = True
              StateImages = dmUtilities.imlTreeStateImages
              TabOrder = 3
              OnKeyPress = tvFilterTreeKeyPress
              OnMouseDown = tvFilterTreeMouseDown
            end
            object btnAddLocations: TBitBtn
              Left = 144
              Top = 360
              Width = 137
              Height = 33
              Caption = 'Add Locations'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 4
              OnClick = btnAddLocationsClick
            end
          end
          object tsLocations: TTabSheet
            Caption = 'tsLocations'
            ImageIndex = 5
            TabVisible = False
            object Label1: TLabel
              Left = 64
              Top = 296
              Width = 116
              Height = 13
              Caption = 'Rename this location to:'
            end
            object Bevel1: TBevel
              Left = 2
              Top = 352
              Width = 423
              Height = 9
              Shape = bsTopLine
            end
            object Panel1: TPanel
              Left = 0
              Top = 0
              Width = 427
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              BorderWidth = 8
              Color = clAppWorkSpace
              TabOrder = 0
              object Label3: TLabel
                Left = 8
                Top = 8
                Width = 411
                Height = 32
                Align = alClient
                Alignment = taCenter
                AutoSize = False
                Caption = 
                  'Select the location to update. This will also update locations i' +
                  'n the MenuMate system as necessary.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clCaptionText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                ShowAccelChar = False
                Layout = tlCenter
                WordWrap = True
              end
            end
            object lbLocations: TListBox
              Left = 0
              Top = 56
              Width = 425
              Height = 193
              Style = lbOwnerDrawFixed
              Color = clInfoBk
              ItemHeight = 18
              TabOrder = 1
              OnDrawItem = lbLocationsDrawItem
            end
            object btnLocationsNext: TBitBtn
              Left = 328
              Top = 360
              Width = 97
              Height = 33
              Caption = 'Next ->'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
              OnClick = btnLocationsNextClick
            end
            object cbLocation: TComboBox
              Left = 208
              Top = 292
              Width = 161
              Height = 21
              Style = csDropDownList
              Color = clInfoBk
              ItemHeight = 13
              TabOrder = 3
            end
            object btnLocationsBack: TBitBtn
              Left = 0
              Top = 360
              Width = 97
              Height = 33
              Caption = '<- Back'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 4
              OnClick = btnLocationsBackClick
            end
          end
          object tsClashes: TTabSheet
            Caption = 'tsClashes'
            ImageIndex = 2
            TabVisible = False
            object Bevel3: TBevel
              Left = 2
              Top = 352
              Width = 423
              Height = 9
              Shape = bsTopLine
            end
            object Panel4: TPanel
              Left = 0
              Top = 0
              Width = 427
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              BorderWidth = 8
              Color = clAppWorkSpace
              TabOrder = 0
              object Label5: TLabel
                Left = 8
                Top = 8
                Width = 411
                Height = 32
                Align = alClient
                Alignment = taCenter
                AutoSize = False
                Caption = 'This is a list of all the transactions that will take place. '
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clCaptionText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                ShowAccelChar = False
                Layout = tlCenter
                WordWrap = True
              end
            end
            object btnFinish: TBitBtn
              Left = 328
              Top = 360
              Width = 97
              Height = 33
              Caption = 'Finish'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
              OnClick = btnFinishClick
            end
            object chbConfirm: TCheckBox
              Left = 8
              Top = 320
              Width = 305
              Height = 17
              Caption = 'I want to perform the above tasks.'
              TabOrder = 2
            end
            object memProcess: TMemo
              Left = 0
              Top = 56
              Width = 425
              Height = 241
              Color = clInfoBk
              ReadOnly = True
              ScrollBars = ssVertical
              TabOrder = 3
            end
            object ProgressBar1: TProgressBar
              Left = 2
              Top = 360
              Width = 319
              Height = 33
              Min = 0
              Max = 100
              TabOrder = 4
            end
          end
          object tsLocationToAdd: TTabSheet
            Caption = 'tsLocationToAdd'
            ImageIndex = 3
            TabVisible = False
            object Bevel2: TBevel
              Left = 2
              Top = 352
              Width = 423
              Height = 9
              Shape = bsTopLine
            end
            object lbMMLocations: TListBox
              Left = 0
              Top = 56
              Width = 425
              Height = 289
              Color = clInfoBk
              ItemHeight = 13
              TabOrder = 0
              OnDrawItem = lbLocationsDrawItem
            end
            object btnAddLocationNext: TBitBtn
              Left = 328
              Top = 359
              Width = 97
              Height = 33
              Caption = 'Next ->'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
              OnClick = btnAddLocationNextClick
            end
            object Panel3: TPanel
              Left = 0
              Top = 0
              Width = 427
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              BorderWidth = 8
              Color = clAppWorkSpace
              TabOrder = 2
              object Label4: TLabel
                Left = 8
                Top = 8
                Width = 411
                Height = 32
                Align = alClient
                Alignment = taCenter
                AutoSize = False
                Caption = 'Select the location to add'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clCaptionText
                Font.Height = -11
                Font.Name = 'Tahoma'
                Font.Style = [fsBold]
                ParentFont = False
                ShowAccelChar = False
                Layout = tlCenter
                WordWrap = True
              end
            end
            object btnAddLocationBack: TBitBtn
              Left = 0
              Top = 360
              Width = 97
              Height = 33
              Caption = '<- Back'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 3
              OnClick = btnAddLocationBackClick
            end
          end
        end
      end
    end
  end
end
