object frmSelectSizes: TfrmSelectSizes
  Tag = 1024
  Left = 132
  Top = 77
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Sizes'
  ClientHeight = 353
  ClientWidth = 562
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlCondiments: TPanel
    Left = 4
    Top = 4
    Width = 430
    Height = 345
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object ItemSizeGrid: TStringGrid
      Left = 2
      Top = 2
      Width = 426
      Height = 341
      ColCount = 2
      DefaultColWidth = 150
      DefaultRowHeight = 30
      FixedColor = clWhite
      FixedCols = 0
      RowCount = 2
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
      ParentFont = False
      ScrollBars = ssNone
      TabOrder = 0
      OnDrawCell = ItemSizeDisplayDrawCell
      OnSelectCell = ItemSizeDisplaySelectCell
      ColWidths = (
        151
        150)
    end
  end
  object pnlSide: TPanel
    Left = 437
    Top = 4
    Width = 122
    Height = 345
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnMsgUp: TTouchBtn
      Left = 6
      Top = 5
      Width = 110
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = 9933970
      ParentFont = False
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseClick = btnMsgUpClick
      OnAutoRepeat = btnMsgUpClick
    end
    object btnMsgDown: TTouchBtn
      Left = 6
      Top = 217
      Width = 110
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = 9933970
      ParentFont = False
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseClick = btnMsgDownClick
      OnAutoRepeat = btnMsgDownClick
    end
    object tbtnCancel: TTouchBtn
      Left = 6
      Top = 280
      Width = 110
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnCancelMouseClick
    end
  end
end
