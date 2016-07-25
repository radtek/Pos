object frmPalmSetup: TfrmPalmSetup
  Tag = 1024
  Left = 145
  Top = 180
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'Palm Setup'
  ClientHeight = 404
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
    Height = 404
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 104
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
    object cbReceiptPrinter: TComboBox
      Left = 16
      Top = 128
      Width = 377
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = True
      ParentFont = False
      TabOrder = 0
      OnChange = cbReceiptPrinterChange
      OnDropDown = cbReceiptPrinterDropDown
    end
    object btnClose: TTouchBtn
      Left = 408
      Top = 16
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object TouchButton2: TTouchBtn
      Left = 408
      Top = 88
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Change Server'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnChangeServer
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 8
      Width = 377
      Height = 89
      Caption = 'Bluetooth Serial Host Port Config'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      object Panel2: TPanel
        Left = 16
        Top = 24
        Width = 153
        Height = 57
        Caption = 'Panel2'
        Color = clNavy
        TabOrder = 0
        object btnBlueToothOn: TCheckBox
          Left = 8
          Top = 8
          Width = 137
          Height = 41
          Caption = 'Enabled'
          Color = clNavy
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          TabOrder = 0
          OnClick = btnBlueToothOnClick
        end
      end
      object ComcbBlueTooth: TComComboBox
        Left = 176
        Top = 40
        Width = 185
        Height = 27
        ComProperty = cpPort
        AutoApply = True
        Text = 'COM1'
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 1
        OnChange = ComcbBlueToothChange
        OnClick = ComcbBlueToothClick
      end
    end
    object cbOnLineLogging: TCheckBox
      Left = 424
      Top = 376
      Width = 137
      Height = 17
      Caption = 'Enable Online Logging'
      TabOrder = 4
      OnClick = cbOnLineLoggingClick
    end
    object btnHappyHour: TTouchBtn
      Left = 16
      Top = 168
      Width = 225
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Happy Hour Settings'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnHappyHourClick
    end
    object tbChefMateIP: TTouchBtn
      Left = 16
      Top = 239
      Width = 225
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'ChefMate Colour'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbChefMateColourClick
    end
    object TouchButton1: TTouchBtn
      Left = 408
      Top = 168
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Default Seat Name'
      ButtonColor = clPurple
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSeatName
    end
    object cbRememberLastServingCourse: TCheckBox
      Left = 16
      Top = 318
      Width = 393
      Height = 35
      Caption = 'Remember Last Serving Course (Hide Popup)'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 8
      OnClick = cbRememberLastServingCourseClick
    end
    object chkLogGetOrders: TCheckBox
      Left = 16
      Top = 376
      Width = 137
      Height = 17
      Caption = 'Log '#39'Get Orders'#39
      TabOrder = 9
      Visible = False
      OnClick = chkLogGetOrdersClick
    end
  end
  object ColorDialog: TColorDialog
    Color = clMaroon
    Options = [cdSolidColor]
    Left = 200
    Top = 264
  end
end
