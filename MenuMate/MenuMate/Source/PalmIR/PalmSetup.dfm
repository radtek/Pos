object frmPalmSetup: TfrmPalmSetup
  Tag = 1024
  Left = 402
  Top = 142
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'Palm Setup'
  ClientHeight = 489
  ClientWidth = 577
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
  object GroupBox3: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 489
    Align = alClient
    Color = 10011108
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 280
      Width = 118
      Height = 19
      Caption = 'Receipt Printer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnModScan: TTouchButton
      Left = 408
      Top = 16
      Width = 153
      Height = 57
      Caption = 'Scan Modules'
      Color = clGreen
      UpColour = clGreen
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = btnModScanClick
    end
    object btnModClear: TTouchButton
      Left = 408
      Top = 80
      Width = 153
      Height = 57
      Caption = 'Clear Modules'
      Color = clGreen
      UpColour = clGreen
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = btnModClearClick
    end
    object btnModAutoProg: TTouchButton
      Left = 408
      Top = 144
      Width = 153
      Height = 57
      Caption = 'Auto Configure'
      Color = clGreen
      UpColour = clGreen
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = btnModAutoProgClick
    end
    object btnModReset: TTouchButton
      Left = 408
      Top = 208
      Width = 153
      Height = 57
      Caption = 'Reset Modules'
      Color = clGreen
      UpColour = clGreen
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = btnModResetClick
    end
    object GroupBox6: TPanel
      Left = 16
      Top = 344
      Width = 377
      Height = 137
      BevelOuter = bvNone
      BorderStyle = bsSingle
      ParentColor = True
      TabOrder = 4
      object lbRetryCount: TLabel
        Left = 16
        Top = 103
        Width = 87
        Height = 19
        Caption = 'Retry Count'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbIntervaltimeout: TLabel
        Left = 16
        Top = 74
        Width = 114
        Height = 19
        Caption = 'Interval time out'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object lbTimeOut: TLabel
        Left = 16
        Top = 44
        Width = 172
        Height = 19
        Caption = 'Initial response time out'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 16
        Top = 9
        Width = 345
        Height = 19
        Alignment = taCenter
        AutoSize = False
        Caption = 'Serial Timeouts'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object edRetry: TEdit
        Left = 208
        Top = 98
        Width = 57
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = '0'
      end
      object btnApplyTimeouts: TTouchButton
        Left = 280
        Top = 40
        Width = 81
        Height = 81
        Caption = 'Apply'
        Color = clBtnFace
        UpColour = clBtnFace
        DownColour = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        BorderStyle = bsSingle
        BevelWidth = 2
        ParentCtl3D = False
        OnClick = btnApplyTimeoutsClick
      end
      object edInitailResponse: TEdit
        Left = 208
        Top = 69
        Width = 57
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        Text = '0'
      end
      object edInterval: TEdit
        Left = 208
        Top = 39
        Width = 57
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = '0'
      end
    end
    object cbReceiptPrinter: TComboBox
      Left = 16
      Top = 304
      Width = 377
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 19
      ParentColor = True
      ParentFont = False
      TabOrder = 5
      OnChange = cbReceiptPrinterChange
      OnDropDown = cbReceiptPrinterDropDown
    end
    object btnClose: TTouchButton
      Left = 408
      Top = 424
      Width = 153
      Height = 57
      Caption = 'Close'
      Color = clMaroon
      UpColour = clMaroon
      DownColour = clMaroon
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = btnCloseClick
    end
    object Panel1: TPanel
      Left = 16
      Top = 16
      Width = 377
      Height = 249
      BevelOuter = bvNone
      Enabled = False
      ParentColor = True
      TabOrder = 7
      object lbConnections: TListBox
        Left = 0
        Top = 0
        Width = 377
        Height = 249
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ItemHeight = 19
        ParentColor = True
        ParentFont = False
        TabOrder = 0
      end
    end
    object TouchButton1: TTouchButton
      Left = 408
      Top = 288
      Width = 153
      Height = 57
      Caption = 'Configure Printer'
      Color = clBtnFace
      UpColour = clBtnFace
      DownColour = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = TouchButton1Click
    end
    object TouchButton2: TTouchButton
      Left = 408
      Top = 360
      Width = 153
      Height = 57
      Caption = 'Change Server'
      Color = clBtnFace
      UpColour = clBtnFace
      DownColour = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = TouchButton2Click
    end
  end
end
