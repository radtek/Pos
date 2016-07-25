object frmPrinterConfiguration: TfrmPrinterConfiguration
  Tag = 1024
  Left = 105
  Top = 68
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'Printer Configuration'
  ClientHeight = 469
  ClientWidth = 806
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 806
    Height = 469
    Align = alClient
    Color = 10011108
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label9: TLabel
      Left = 16
      Top = 36
      Width = 54
      Height = 19
      Caption = 'Printer'
    end
    object Label11: TLabel
      Left = 440
      Top = 36
      Width = 113
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Bold Font'
    end
    object Label12: TLabel
      Left = 320
      Top = 36
      Width = 113
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Normal Font'
    end
    object Label13: TLabel
      Left = 320
      Top = 12
      Width = 231
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = '------ Chars Per Line ------'
    end
    object laCutterGap: TLabel
      Left = 560
      Top = 36
      Width = 113
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Cutter Gap'
    end
    object Label14: TLabel
      Left = 680
      Top = 16
      Width = 113
      Height = 39
      Alignment = taCenter
      AutoSize = False
      Caption = 'Orders Top Margin'
      WordWrap = True
    end
    object lbPrinters: TListBox
      Left = 16
      Top = 56
      Width = 297
      Height = 113
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 18
      ParentFont = False
      TabOrder = 0
      OnClick = lbPrintersClick
    end
    object lbLargeFnt: TListBox
      Left = 440
      Top = 56
      Width = 113
      Height = 113
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 18
      Items.Strings = (
        '25'
        '26'
        '27'
        '28'
        '29'
        '30'
        '31'
        '32'
        '33'
        '34'
        '35'
        '36'
        '37'
        '38'
        '39'
        '40'
        '41'
        '42'
        '43'
        '44'
        '45'
        '46'
        '47'
        '48'
        '49'
        '50')
      ParentFont = False
      TabOrder = 1
      OnClick = lbLargeFntClick
    end
    object lbNormalFnt: TListBox
      Left = 320
      Top = 56
      Width = 113
      Height = 113
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 18
      Items.Strings = (
        '25'
        '26'
        '27'
        '28'
        '29'
        '30'
        '31'
        '32'
        '33'
        '34'
        '35'
        '36'
        '37'
        '38'
        '39'
        '40'
        '41'
        '42'
        '43'
        '44'
        '45'
        '46'
        '47'
        '48'
        '49'
        '50')
      ParentFont = False
      TabOrder = 2
      OnClick = lbNormalFntClick
    end
    object lbCutterGap: TListBox
      Left = 560
      Top = 56
      Width = 113
      Height = 113
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 18
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9'
        '10'
        '11'
        '12'
        '13'
        '14'
        '15'
        '16'
        '17'
        '18'
        '19'
        '20'
        '21'
        '22'
        '23'
        '24'
        '25')
      ParentFont = False
      TabOrder = 3
      OnClick = lbCutterGapClick
    end
    object cbKitchenFont: TCheckBox
      Left = 24
      Top = 240
      Width = 401
      Height = 17
      Caption = 'Kitchen Orders Small Font'
      TabOrder = 4
      OnClick = cbKitchenFontClick
    end
    object lbTopMargin: TListBox
      Left = 680
      Top = 56
      Width = 113
      Height = 113
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 18
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9'
        '10')
      ParentFont = False
      TabOrder = 5
      OnClick = lbTopMarginClick
    end
    object btnClose: TTouchButton
      Left = 672
      Top = 400
      Width = 121
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
    object cbCourseHeader: TCheckBox
      Left = 24
      Top = 272
      Width = 321
      Height = 17
      Caption = 'Print header for each course'
      TabOrder = 7
      OnClick = cbCourseHeaderClick
    end
    object cbCarbonCopy: TCheckBox
      Left = 24
      Top = 312
      Width = 185
      Height = 17
      Caption = 'Duplicate Receipts'
      TabOrder = 8
      Visible = False
      OnClick = cbCarbonCopyClick
    end
    object GroupBox1: TGroupBox
      Left = 24
      Top = 360
      Width = 329
      Height = 97
      Caption = 'Cut Orders...'
      TabOrder = 9
      object rbCutByCourse: TRadioButton
        Left = 16
        Top = 36
        Width = 297
        Height = 17
        Caption = 'Between each course'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = rbCutByCourseClick
      end
      object rbCutByNone: TRadioButton
        Left = 16
        Top = 64
        Width = 297
        Height = 17
        Caption = 'At the end of each order / table'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = rbCutByNoneClick
      end
    end
    object btnTestPrint: TTouchButton
      Left = 685
      Top = 184
      Width = 108
      Height = 57
      Caption = 'Test'
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
      OnClick = btnTestPrintClick
    end
    object chbAlwaysFullCut: TCheckBox
      Left = 24
      Top = 200
      Width = 321
      Height = 17
      Caption = 'Always send full cut command'
      TabOrder = 11
      OnClick = chbAlwaysFullCutClick
    end
    object cbPrintNoteWithDiscount: TCheckBox
      Left = 376
      Top = 280
      Width = 417
      Height = 17
      Caption = 'Print the Palm'#39's Note with Discount / Surcharge'
      TabOrder = 12
      Visible = False
      OnClick = cbPrintNoteWithDiscountClick
    end
  end
end
