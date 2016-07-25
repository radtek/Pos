object frmProcessWebOrder: TfrmProcessWebOrder
  Tag = 1024
  Left = 441
  Top = 326
  Caption = 'Web Orders'
  ClientHeight = 639
  ClientWidth = 547
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 547
    Height = 639
    Align = alClient
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnClose: TTouchBtn
      Left = 407
      Top = 47
      Width = 130
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object btnPrev: TTouchBtn
      Left = 8
      Top = 564
      Width = 107
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '<<'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnPrevMouseClick
    end
    object tbtnAcceptOrder: TTouchBtn
      Left = 407
      Top = 564
      Width = 130
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Accept Order'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnAcceptOrderMouseClick
    end
    object btnNext: TTouchBtn
      Left = 295
      Top = 564
      Width = 107
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '>>'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnNextMouseClick
    end
    object btnBillUp: TTouchBtn
      Left = 121
      Top = 564
      Width = 81
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '/\'
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnBillUpMouseClick
      OnAutoRepeat = btnBillUpMouseClick
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnBillDown: TTouchBtn
      Left = 208
      Top = 564
      Width = 81
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '\/'
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnBillDownMouseClick
      OnAutoRepeat = btnBillDownMouseClick
      InitialDelay = 25
      RepeatDelay = 25
    end
    object memReceipt: TMemo
      Left = 8
      Top = 47
      Width = 393
      Height = 505
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Courier New'
      Font.Style = []
      Lines.Strings = (
        '123456789112345678921234567893123456789412'
        '34567895')
      ParentFont = False
      TabOrder = 6
    end
    object TouchButton1: TTouchBtn
      Left = 408
      Top = 200
      Width = 130
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Search Orders'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
    end
    object pnltitle: TPanel
      Left = 0
      Top = 0
      Width = 547
      Height = 41
      Align = alTop
      Caption = 'New Web Orders'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentFont = False
      TabOrder = 8
    end
    object tchbtnEditOrders: TTouchBtn
      Left = 407
      Top = 273
      Width = 130
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Edit Order'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
    end
  end
end
