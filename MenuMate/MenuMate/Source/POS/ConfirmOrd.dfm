object frmConfirmOrder: TfrmConfirmOrder
  Tag = 1024
  Left = 207
  Top = 137
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Confirm'
  ClientHeight = 695
  ClientWidth = 934
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlInfo: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 0
    Width = 343
    Height = 695
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
    DesignSize = (
      341
      693)
    object lbeTitle: TLabel
      Left = 8
      Top = 8
      Width = 345
      Height = 41
      AutoSize = False
      Caption = 'Save Orders To'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object lbTableNo: TLabel
      Left = 8
      Top = 72
      Width = 600
      Height = 37
      AutoSize = False
      Caption = 'Table #'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ShowAccelChar = False
      Transparent = True
      WordWrap = True
    end
    object lbParty: TLabel
      Left = 8
      Top = 192
      Width = 345
      Height = 41
      AutoSize = False
      Caption = 'Party Name'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ShowAccelChar = False
      Transparent = True
    end
    object LabelSeat: TLabel
      Left = 8
      Top = 136
      Width = 345
      Height = 37
      AutoSize = False
      Caption = 'Seat #'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ShowAccelChar = False
      Transparent = True
      WordWrap = True
    end
    object Image1: TImage
      Left = 8
      Top = 239
      Width = 303
      Height = 152
      Anchors = [akLeft, akTop, akRight, akBottom]
      AutoSize = True
      Center = True
      ExplicitWidth = 305
      ExplicitHeight = 154
    end
    object lbTabLimit: TLabel
      Left = 8
      Top = 440
      Width = 550
      Height = 41
      AutoSize = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ShowAccelChar = False
      Transparent = True
    end
    object lbLimitBal: TLabel
      Left = 620
      Top = 440
      Width = 10
      Height = 37
      Alignment = taRightJustify
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object sgTotals: TStringGrid
      Left = 0
      Top = 472
      Width = 341
      Height = 221
      Align = alBottom
      BorderStyle = bsNone
      Color = 14342874
      ColCount = 2
      DefaultColWidth = 150
      DefaultRowHeight = 70
      DefaultDrawing = False
      FixedCols = 0
      RowCount = 3
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssNone
      TabOrder = 0
      OnDrawCell = sgTotalsDrawCell
    end
  end
  object pnlButtons: TPanel
    Left = 758
    Top = 0
    Width = 176
    Height = 695
    Margins.Top = 0
    Margins.Right = 0
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    DesignSize = (
      176
      695)
    object tbtnSave: TTouchBtn
      Left = 5
      Top = 5
      Width = 166
      Align = alTop
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
      OnMouseClick = btnSaveClick
    end
    object tbSavePrint: TTouchBtn
      Left = 11
      Top = 82
      Width = 161
      Anchors = [akLeft, akTop, akRight]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Save & Print Prelim Receipt'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbSavePrintClick
    end
    object btnSaveAndBill: TTouchBtn
      Left = 11
      Top = 156
      Width = 161
      Anchors = [akLeft, akTop, akRight]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Save & Bill'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSaveAndBillClick
    end
    object tbCancel: TTouchBtn
      Left = 5
      Top = 633
      Width = 166
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbCancelClick
    end
    object tbtnPartyName: TTouchBtn
      Left = 8
      Top = 364
      Width = 161
      Anchors = [akLeft, akTop, akRight]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Set Party Name'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPartyNameMouseClick
    end
  end
  object pnlReceipt: TPanel
    AlignWithMargins = True
    Left = 352
    Top = 0
    Width = 398
    Height = 695
    Margins.Top = 0
    Margins.Right = 8
    Margins.Bottom = 0
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 2
    object btnBillUp: TTouchBtn
      Left = 8
      Top = 608
      Width = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clWhite
      ParentFont = False
      Caption = '/\'
      ButtonColor = clBtnFace
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnBillUpMouseDown
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnBillDown: TTouchBtn
      Left = 304
      Top = 608
      Width = 73
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clWhite
      ParentFont = False
      Caption = '\/'
      ButtonColor = clBtnFace
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnBillDownMouseDown
      InitialDelay = 25
      RepeatDelay = 25
    end
    object tntReceipt: TRichEdit
      Left = 6
      Top = 6
      Width = 386
      Height = 619
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object pnlScroll: TPanel
      Left = 6
      Top = 625
      Width = 386
      Height = 64
      Align = alBottom
      BevelOuter = bvNone
      BorderWidth = 8
      Color = 14342874
      ParentBackground = False
      TabOrder = 3
      object tchbtnScrollBillUp: TTouchBtn
        Left = 8
        Top = 8
        Width = 97
        Height = 48
        Align = alLeft
        Color = 11587808
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        ArrowType = atUp
        OnAutoRepeat = tchbtnScrollBillUpAutoRepeat
      end
      object tchbtnScrollBillDown: TTouchBtn
        Left = 281
        Top = 8
        Width = 97
        Height = 48
        Align = alRight
        Color = 11587808
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        ArrowType = atDown
        OnAutoRepeat = tchbtnScrollBillDownAutoRepeat
      end
    end
  end
  object IdHTTP1: TIdHTTP
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    Left = 216
    Top = 256
  end
end
