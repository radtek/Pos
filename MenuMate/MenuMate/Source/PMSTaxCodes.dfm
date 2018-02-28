object frmTaxCodes: TfrmTaxCodes
  Left = 0
  Top = 0
  Caption = 'TAX Codes'
  ClientHeight = 521
  ClientWidth = 812
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMain: TPanel
    Left = 8
    Top = 8
    Width = 796
    Height = 505
    BevelInner = bvRaised
    Caption = 'pnlMain'
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object pnlSelection: TPanel
      Left = 8
      Top = 4
      Width = 626
      Height = 485
      Caption = 'pnlSelection'
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      DesignSize = (
        626
        485)
      object pnlLabel: TPanel
        Left = 8
        Top = 8
        Width = 609
        Height = 41
        Caption = 'Tax Codes'
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
        Left = 8
        Top = 64
        Width = 609
        Height = 401
        Anchors = [akLeft, akTop, akBottom]
        ColCount = 4
        DefaultColWidth = 146
        DefaultDrawing = False
        DragCursor = crDefault
        FixedCols = 0
        RowCount = 16
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        ScrollBars = ssVertical
        TabOrder = 1
        ColWidths = (
          146
          146
          198
          146)
      end
    end
    object pnlConfiguration: TPanel
      Left = 640
      Top = 8
      Width = 146
      Height = 481
      Caption = 'pnlConfiguration'
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      object btnClose: TTouchBtn
        Left = 8
        Top = 8
        Width = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Close'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
      end
      object btnAdd: TTouchBtn
        Left = 8
        Top = 104
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
      end
      object btnEdit: TTouchBtn
        Left = 8
        Top = 200
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
      end
      object btnDelete: TTouchBtn
        Left = 8
        Top = 416
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
      end
    end
  end
end
