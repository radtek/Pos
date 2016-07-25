object frmSetupPurchaseOrders: TfrmSetupPurchaseOrders
  Left = 98
  Top = 52
  BorderStyle = bsDialog
  Caption = 'Setup Purchase Orders'
  ClientHeight = 537
  ClientWidth = 417
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    417
    537)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 401
    Height = 521
    Anchors = [akLeft, akTop, akBottom]
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      401
      521)
    object Panel4: TPanel
      Left = 5
      Top = 5
      Width = 391
      Height = 468
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      object Label14: TLabel
        Left = 8
        Top = 200
        Width = 99
        Height = 13
        Caption = 'Delivery Address:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label16: TLabel
        Left = 8
        Top = 336
        Width = 72
        Height = 13
        Caption = 'Instructions:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label1: TLabel
        Left = 8
        Top = 64
        Width = 95
        Height = 13
        Caption = 'Company Details'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 8
        Top = 168
        Width = 305
        Height = 13
        Caption = 
          'Details of your company that will appear at the top of the invoi' +
          'ce.'
      end
      object Bevel1: TBevel
        Left = 8
        Top = 184
        Width = 361
        Height = 9
        Shape = bsTopLine
      end
      object Label3: TLabel
        Left = 8
        Top = 304
        Width = 302
        Height = 13
        Caption = 'Informs your supplier as to where the goods should be delivered.'
      end
      object Bevel2: TBevel
        Left = 8
        Top = 320
        Width = 361
        Height = 9
        Shape = bsTopLine
      end
      object Label4: TLabel
        Left = 8
        Top = 440
        Width = 336
        Height = 13
        Caption = 
          'Extra instructions for the supplier that will appear on the purc' +
          'hase order.'
      end
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 389
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 3
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 389
          Height = 48
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'Purchase Order Default Settings'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object memInstructions: TMemo
        Left = 8
        Top = 352
        Width = 361
        Height = 81
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 2
      end
      object memDeliveryAddress: TMemo
        Left = 8
        Top = 216
        Width = 361
        Height = 81
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 1
        WordWrap = False
      end
      object memCompanyDetails: TMemo
        Left = 8
        Top = 80
        Width = 361
        Height = 81
        Color = clInfoBk
        ScrollBars = ssVertical
        TabOrder = 0
        WordWrap = False
      end
    end
    object btnSave: TBitBtn
      Left = 192
      Top = 480
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 1
      OnClick = btnSaveClick
      Kind = bkOK
    end
    object btnCancel: TBitBtn
      Left = 296
      Top = 480
      Width = 97
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 2
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        333333333333333333333333000033338833333333333333333F333333333333
        0000333911833333983333333388F333333F3333000033391118333911833333
        38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
        911118111118333338F3338F833338F3000033333911111111833333338F3338
        3333F8330000333333911111183333333338F333333F83330000333333311111
        8333333333338F3333383333000033333339111183333333333338F333833333
        00003333339111118333333333333833338F3333000033333911181118333333
        33338333338F333300003333911183911183333333383338F338F33300003333
        9118333911183333338F33838F338F33000033333913333391113333338FF833
        38F338F300003333333333333919333333388333338FFF830000333333333333
        3333333333333333333888330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
    end
    object cbShowItemCost: TCheckBox
      Left = 8
      Top = 488
      Width = 97
      Height = 17
      Caption = 'Show Item Cost'
      TabOrder = 3
      OnClick = cbShowItemCostClick
    end
  end
end
