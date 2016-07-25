object frmTransactionAuditScreen: TfrmTransactionAuditScreen
  Tag = 1024
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 6
  ClientHeight = 720
  ClientWidth = 976
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
      ColCount = 9
      FixedCols = 0
      RowCount = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
      ParentFont = False
      TabOrder = 0
      OnDblClick = gridTransactionsDblClick
      OnDrawCell = gridTransactionsDrawCell
      ColWidths = (
        55
        120
        93
        93
        93
        105
        87
        87
        87)
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
    object btnClose: TTouchBtn
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
      OnMouseClick = btnCloseMouseClick
    end
    object tbtnFilters: TTouchBtn
      Left = 8
      Top = 73
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
      Caption = 'Filter'
      ButtonColor = 6315356
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = tbtnFiltersAutoRepeat
    end
  end
end
