object frmTaxProfile: TfrmTaxProfile
  Left = 2561
  Top = 258
  Width = 561
  Height = 543
  ActiveControl = vtvProfiles
  Caption = 'Tax Profiles'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  DesignSize = (
    553
    516)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 540
    Height = 498
    Anchors = [akLeft, akTop, akRight, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      540
      498)
    object Panel1: TPanel
      Left = 5
      Top = 5
      Width = 530
      Height = 440
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      DesignSize = (
        530
        440)
      object vtvProfiles: TVirtualStringTree
        Left = 4
        Top = 4
        Width = 521
        Height = 385
        Anchors = [akLeft, akTop, akRight, akBottom]
        Color = clInfoBk
        DefaultNodeHeight = 21
        Header.AutoSizeIndex = 3
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'MS Sans Serif'
        Header.Font.Style = []
        Header.Height = 21
        Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
        TabOrder = 0
        TreeOptions.MiscOptions = [toEditable, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowVertGridLines]
        TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
        TreeOptions.StringOptions = [toAutoAcceptEditChange]
        OnColumnDblClick = vtvProfilesColumnDblClick
        OnCompareNodes = vtvProfilesCompareNodes
        OnCreateEditor = vtvProfilesCreateEditor
        OnFocusChanged = vtvProfilesFocusChanged
        OnGetText = vtvProfilesGetText
        OnHeaderClick = vtvProfilesHeaderClick
        OnKeyPress = vtvProfilesKeyPress
        Columns = <
          item
            Position = 0
            Width = 180
            WideText = 'Name'
          end
          item
            Position = 1
            Width = 87
            WideText = 'Rate'
          end
          item
            Position = 2
            Width = 160
            WideText = 'Tax Type'
          end
          item
            Position = 4
            Width = 90
            WideText = 'Priority'
          end>
      end
      object btnAdd: TButton
        Left = 8
        Top = 396
        Width = 89
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Add'
        TabOrder = 1
        OnClick = btnAddClick
      end
      object btnDelete: TButton
        Left = 432
        Top = 396
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Delete'
        Enabled = False
        TabOrder = 2
        OnClick = btnDeleteClick
      end
    end
    object btnSave: TBitBtn
      Left = 344
      Top = 452
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'Ok'
      TabOrder = 1
      OnClick = btnSaveTaxProfilesClick
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
      Left = 440
      Top = 452
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      TabStop = False
      Kind = bkCancel
    end
  end
end
