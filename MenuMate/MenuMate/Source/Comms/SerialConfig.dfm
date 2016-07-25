object frmSerialConfig: TfrmSerialConfig
  Tag = 1024
  Left = 404
  Top = 171
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'frmSerialConfig'
  ClientHeight = 377
  ClientWidth = 537
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
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 400
    Top = 0
    Width = 137
    Height = 377
    Align = alRight
    Color = 10011108
    TabOrder = 0
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 8
      Width = 121
      Height = 81
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object TouchBtn2: TTouchBtn
      Left = 8
      Top = 152
      Width = 121
      Height = 81
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 393
    Height = 377
    Align = alLeft
    Color = 10011108
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 8
      Top = 8
      Width = 369
      Height = 361
      Caption = 'Comm Port Details'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object lbeFlowControl: TLabel
        Left = 8
        Top = 272
        Width = 106
        Height = 22
        Caption = 'Flow Control'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbeStopBits: TLabel
        Left = 8
        Top = 216
        Width = 77
        Height = 22
        Caption = 'Stop Bits'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbeBaudRate: TLabel
        Left = 8
        Top = 48
        Width = 89
        Height = 22
        Caption = 'Baud Rate'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbeDataBits: TLabel
        Left = 8
        Top = 104
        Width = 76
        Height = 22
        Caption = 'Data Bits'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbeParity: TLabel
        Left = 8
        Top = 160
        Width = 47
        Height = 22
        Caption = 'Parity'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object btnBaudRate: TTouchBtn
        Left = 184
        Top = 32
        Width = 177
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = clNavy
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnBaudRateClick
      end
      object btnDataBits: TTouchBtn
        Left = 184
        Top = 88
        Width = 177
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = clNavy
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnDataBitsClick
      end
      object btnParity: TTouchBtn
        Left = 184
        Top = 144
        Width = 177
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = clNavy
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnParityClick
      end
      object btnStopBits: TTouchBtn
        Left = 184
        Top = 200
        Width = 177
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = clNavy
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnStopBitsClick
      end
      object btnFlowControl: TTouchBtn
        Left = 184
        Top = 256
        Width = 177
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        ButtonColor = clNavy
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnFlowControlClick
      end
      object cbAsync: TCheckBox
        Left = 8
        Top = 320
        Width = 353
        Height = 25
        Alignment = taLeftJustify
        Caption = 'Asynchronous mode'
        TabOrder = 5
        OnClick = cbAsyncClick
      end
    end
  end
end
