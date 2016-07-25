object frmParkedSales: TfrmParkedSales
  Tag = 1024
  Left = 344
  Top = 116
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Select Parked Sale'
  ClientHeight = 482
  ClientWidth = 345
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 5
    Top = 7
    Width = 332
    Height = 474
    Color = 10011108
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
    object pnlParkedSales: TPanel
      Left = 8
      Top = 40
      Width = 241
      Height = 369
      BevelOuter = bvNone
      BorderStyle = bsSingle
      ParentColor = True
      TabOrder = 3
      OnResize = pnlParkedSalesResize
      object sbSales: TScrollBox
        Left = -2
        Top = -1
        Width = 257
        Height = 365
        HorzScrollBar.Range = 233
        HorzScrollBar.Visible = False
        VertScrollBar.ButtonSize = 1
        VertScrollBar.Color = 10011108
        VertScrollBar.Increment = 45
        VertScrollBar.ParentColor = False
        VertScrollBar.Range = 369
        VertScrollBar.Size = 1
        VertScrollBar.Style = ssFlat
        VertScrollBar.ThumbSize = 1
        VertScrollBar.Tracking = True
        AutoScroll = False
        BorderStyle = bsNone
        TabOrder = 0
      end
    end
    object btnSalesUp: TTouchBtn
      Left = 256
      Top = 40
      Width = 65
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
      OnMouseClick = btnSalesUpMouseClick
      OnAutoRepeat = btnSalesUpMouseClick
    end
    object btnSeatsDown: TTouchBtn
      Left = 256
      Top = 344
      Width = 65
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
      OnMouseClick = btnSeatsDownMouseClick
      OnAutoRepeat = btnSeatsDownMouseClick
    end
    object lbeTitle: TStaticText
      Left = 8
      Top = 8
      Width = 313
      Height = 27
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSingle
      Caption = 'Parked Sales'
      Color = clMaroon
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 416
      Width = 313
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbCloseClick
    end
  end
end
