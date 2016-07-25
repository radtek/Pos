object frmDealLevels: TfrmDealLevels
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Deal Levels'
  ClientHeight = 480
  ClientWidth = 481
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 336
    Height = 480
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object sgDisplay: TStringGrid
      Left = 6
      Top = 40
      Width = 324
      Height = 434
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
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 0
      OnDrawCell = sgDisplayDrawCell
      OnSelectCell = sgDisplaySelectCell
      ColWidths = (
        151
        150)
    end
    object pnlLabel: TPanel
      Left = 6
      Top = 6
      Width = 324
      Height = 34
      BevelOuter = bvLowered
      Caption = 'Deal Levels'
      Color = 39157
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentFont = False
      TabOrder = 1
    end
  end
  object pnlMinor: TPanel
    Left = 342
    Top = 0
    Width = 139
    Height = 480
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tbtnOk: TTouchBtn
      Left = 8
      Top = 6
      Width = 123
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOkMouseClick
    end
    object tbtnAdd: TTouchBtn
      Left = 8
      Top = 132
      Width = 123
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAddMouseClick
    end
    object tbtnDelete: TTouchBtn
      Left = 8
      Top = 258
      Width = 123
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Delete'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnDeleteMouseClick
    end
    object tbtnEdit: TTouchBtn
      Left = 8
      Top = 195
      Width = 123
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Edit'
      ButtonColor = 8487296
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnEditMouseClick
    end
  end
end
