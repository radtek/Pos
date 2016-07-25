object frmGroupGUI: TfrmGroupGUI
  Tag = 1024
  Left = 1
  Top = 1
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Group GUI'
  ClientHeight = 675
  ClientWidth = 892
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  WindowState = wsMaximized
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMajor: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 0
    Width = 749
    Height = 675
    Margins.Top = 0
    Margins.Bottom = 0
    Align = alClient
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = 14342874
    Ctl3D = False
    ParentBackground = False
    ParentCtl3D = False
    TabOrder = 0
    object pnlGroups: TPanel
      Left = 0
      Top = 40
      Width = 625
      Height = 633
      Align = alLeft
      TabOrder = 0
      object tgridGroups: TStringGrid
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 617
        Height = 625
        Align = alClient
        Color = clCream
        ColCount = 2
        DefaultColWidth = 300
        DefaultRowHeight = 55
        FixedColor = clSilver
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        GradientEndColor = clWhite
        GridLineWidth = 2
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel3: TPanel
      Left = 0
      Top = 0
      Width = 747
      Height = 40
      Align = alTop
      Caption = 'Groups'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentFont = False
      TabOrder = 1
    end
  end
  object pnlMinor: TPanel
    Left = 755
    Top = 0
    Width = 137
    Height = 675
    Align = alRight
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnClose: TTouchBtn
      Left = 9
      Top = 13
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object btnAdd: TTouchBtn
      Left = 9
      Top = 113
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAddMouseClick
    end
    object btnEdit: TTouchBtn
      Left = 8
      Top = 211
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Edit'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnEditMouseClick
    end
    object btnDelete: TTouchBtn
      Left = 8
      Top = 309
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Delete'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDeleteMouseClick
    end
    object btnUsers: TTouchBtn
      Left = 8
      Top = 408
      Width = 121
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Users'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnUsersMouseClick
    end
  end
end
