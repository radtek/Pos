object frmDelayedPaymentTabs: TfrmDelayedPaymentTabs
  Tag = 1024
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 6
  ClientHeight = 624
  ClientWidth = 1012
  Color = clBlack
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlTransactions: TPanel
    Left = 0
    Top = 0
    Width = 869
    Height = 756
    Color = 14342874
    Ctl3D = False
    ParentBackground = False
    ParentCtl3D = False
    TabOrder = 0
    object gridTransactions: TStringGrid
      Left = 8
      Top = 8
      Width = 853
      Height = 740
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      TabStop = False
      ColCount = 8
      DefaultRowHeight = 40
      FixedCols = 0
      RowCount = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
      ParentFont = False
      ScrollBars = ssNone
      TabOrder = 0
      OnDrawCell = gridTransactionsDrawCell
      ColWidths = (
        119
        120
        137
        142
        147
        390
        64
        64)
    end
  end
  object pnlButtons: TPanel
    Left = 875
    Top = 0
    Width = 137
    Height = 756
    Margins.Left = 8
    Margins.Top = 8
    Margins.Right = 8
    Margins.Bottom = 8
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnSelect: TTouchBtn
      Left = 8
      Top = 8
      Width = 121
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSelectMouseClick
    end
    object tbtnCancel: TTouchBtn
      Left = 8
      Top = 641
      Width = 121
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
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
    object btnScrollUp: TTouchBtn
      Left = 8
      Top = 73
      Width = 121
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Color = 9933970
      ParentFont = False
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseClick = btnScrollUpMouseClick
      OnAutoRepeat = btnScrollUpMouseClick
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnScrollDown: TTouchBtn
      Left = 8
      Top = 600
      Width = 121
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Color = 9933970
      ParentFont = False
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseClick = btnScrollDownMouseClick
      OnAutoRepeat = btnScrollDownMouseClick
      InitialDelay = 25
      RepeatDelay = 25
    end
  end
end
