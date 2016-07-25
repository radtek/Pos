object frmControlTransaction: TfrmControlTransaction
  Tag = 1024
  Left = 1
  Top = 1
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'MenuMate - Close Bill'
  ClientHeight = 739
  ClientWidth = 1014
  Color = clBlack
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  PixelsPerInch = 96
  TextHeight = 23
  object pnlPayment: TPanel
    Left = 112
    Top = 32
    Width = 817
    Height = 673
    BevelOuter = bvNone
    Color = clBlack
    ParentBackground = False
    TabOrder = 0
    object Panel1: TPanel
      Left = 0
      Top = 0
      Width = 641
      Height = 673
      Align = alLeft
      Color = 14342874
      ParentBackground = False
      TabOrder = 0
      object pnlCashOutTitle: TPanel
        Left = 288
        Top = 48
        Width = 137
        Height = 41
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Cash Out'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        TabOrder = 0
      end
      object pnlPaymentAmount: TPanel
        Left = 432
        Top = 48
        Width = 201
        Height = 41
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Payment'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        TabOrder = 1
      end
      object sgPay: TStringGrid
        Left = 1
        Top = 96
        Width = 639
        Height = 576
        Align = alBottom
        Color = 14342874
        ColCount = 3
        DefaultColWidth = 164
        DefaultDrawing = False
        FixedCols = 0
        FixedRows = 0
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        Options = []
        ParentFont = False
        ScrollBars = ssNone
        TabOrder = 2
        OnDrawCell = sgPayDrawCell
      end
      object Panel25: TPanel
        Left = 1
        Top = 1
        Width = 639
        Height = 41
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Payments Tendered'
        Color = 2200566
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 3
      end
    end
    object Panel3: TPanel
      Left = 648
      Top = 0
      Width = 169
      Height = 673
      Align = alRight
      Color = 14342874
      ParentBackground = False
      TabOrder = 1
      object btnCloseBill: TTouchBtn
        Left = 16
        Top = 114
        Width = 137
        Height = 73
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Close Bill'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnCloseBillClick
      end
      object btnPrintBill: TTouchBtn
        Left = 16
        Top = 200
        Width = 137
        Height = 73
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Close and Print Bill'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = stPrintBillClick
      end
      object btnBack: TTouchBtn
        Left = 16
        Top = 28
        Width = 137
        Height = 73
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Back'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnBackClick
      end
    end
  end
end
