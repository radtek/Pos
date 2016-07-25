object frmSelectReceipt: TfrmSelectReceipt
  Tag = 1024
  Left = 117
  Top = 60
  BorderStyle = bsDialog
  Caption = 'Select Receipt'
  ClientHeight = 641
  ClientWidth = 776
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 776
    Height = 641
    Align = alClient
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label1: TLabel
      Left = 473
      Top = 269
      Width = 93
      Height = 23
      Caption = 'Select Day.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
    end
    object sbAllTerminals: TSpeedButton
      Left = 472
      Top = 206
      Width = 137
      Height = 57
      GroupIndex = 2
      Down = True
      Caption = 'All Terminals'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = sbAllClick
    end
    object sbThisTerminal: TSpeedButton
      Left = 624
      Top = 206
      Width = 137
      Height = 57
      GroupIndex = 2
      Caption = 'This Terminal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = sbThisTerminalClick
    end
    object calReceipt: TMonthCalendar
      Left = 473
      Top = 298
      Width = 288
      Height = 263
      AutoSize = True
      Date = 37497.666910625000000000
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = calReceiptClick
    end
    object btnBillUp: TTouchBtn
      Left = 153
      Top = 564
      Width = 81
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseClick = btnBillUpMouseClick
      OnAutoRepeat = btnBillUpMouseClick
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnBillDown: TTouchBtn
      Left = 240
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
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseClick = btnBillDownMouseClick
      OnAutoRepeat = btnBillDownMouseClick
      InitialDelay = 25
      RepeatDelay = 25
    end
    object memReceipt: TMemo
      Left = 8
      Top = 8
      Width = 449
      Height = 550
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object btnClose: TTouchBtn
      Left = 472
      Top = 16
      Width = 289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object TouchBtn1: TTouchBtn
      Left = 473
      Top = 79
      Width = 289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Print'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPrintClick
    end
    object btnSearch: TTouchBtn
      Left = 473
      Top = 567
      Width = 289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Search'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSearchMouseClick
    end
    object btnNext: TTouchBtn
      Left = 327
      Top = 567
      Width = 129
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atRight
      OnAutoRepeat = btnNextAutoRepeat
      RepeatDelay = 100
    end
    object btnPrev: TTouchBtn
      Left = 18
      Top = 567
      Width = 129
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atLeft
      OnAutoRepeat = btnPrevAutoRepeat
      RepeatDelay = 100
    end
    object btnAddTip: TTouchBtn
      Left = 473
      Top = 143
      Width = 289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add Tip'
      ButtonColor = clTeal
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAddTipMouseClick
    end
  end
end
