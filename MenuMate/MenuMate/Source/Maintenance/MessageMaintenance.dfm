object frmMessageMaintenance: TfrmMessageMaintenance
  Tag = 1024
  Left = 114
  Top = 141
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Messages'
  ClientHeight = 526
  ClientWidth = 694
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Arial'
  Font.Style = [fsBold]
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 19
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 793
    Height = 513
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Panel9: TPanel
      Left = 8
      Top = 8
      Width = 522
      Height = 497
      BorderWidth = 6
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object pnlLabel: TPanel
        Left = 7
        Top = 7
        Width = 508
        Height = 34
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Message List'
        Color = 2200566
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
      end
      object sgDisplay: TStringGrid
        Left = 7
        Top = 48
        Width = 508
        Height = 442
        Align = alBottom
        ColCount = 2
        FixedCols = 0
        RowCount = 17
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        ScrollBars = ssVertical
        TabOrder = 1
        ColWidths = (
          169
          320)
      end
    end
    object Panel10: TPanel
      Left = 536
      Top = 8
      Width = 146
      Height = 497
      BevelInner = bvRaised
      BevelOuter = bvNone
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      object btnAdd: TTouchBtn
        Left = 8
        Top = 80
        Width = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Add'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnAddMessageClick
      end
      object btnEdit: TTouchBtn
        Left = 8
        Top = 166
        Width = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Edit'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnEditMessageClick
      end
      object btnDelete: TTouchBtn
        Left = 8
        Top = 432
        Width = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Delete'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnDelMessageClick
      end
      object TouchBtn1: TTouchBtn
        Left = 1
        Top = 1
        Width = 144
        Align = alTop
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Close'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = imgExitClick
      end
    end
  end
end
