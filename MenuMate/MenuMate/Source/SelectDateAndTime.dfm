object frmSelectDateTime: TfrmSelectDateTime
  Tag = 1024
  Left = 33
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Select Date And Time'
  ClientHeight = 376
  ClientWidth = 394
  Color = clBlack
  TransparentColorValue = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 593
    Height = 377
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label1: TLabel
      Left = 32
      Top = 39
      Width = 33
      Height = 22
      Caption = 'Day'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 152
      Top = 39
      Width = 51
      Height = 22
      Caption = 'Month'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 272
      Top = 39
      Width = 39
      Height = 22
      Caption = 'Year'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object LblHour: TLabel
      Left = 32
      Top = 155
      Width = 40
      Height = 22
      Caption = 'Hour'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object LblMinute: TLabel
      Left = 152
      Top = 155
      Width = 55
      Height = 22
      Caption = 'Minute'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object LblAMPM: TLabel
      Left = 272
      Top = 155
      Width = 61
      Height = 22
      Caption = 'AM/PM'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object btnOk: TTouchBtn
      Left = 48
      Top = 294
      Width = 120
      Height = 55
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
    object tbtnCancel: TTouchBtn
      Left = 225
      Top = 294
      Width = 120
      Height = 55
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnCancelMouseClick
    end
    object BtnDate: TTouchBtn
      Left = 32
      Top = 67
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnDateMouseClick
    end
    object BtnMonth: TTouchBtn
      Left = 152
      Top = 67
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnMonthMouseClick
    end
    object BtnYear: TTouchBtn
      Left = 272
      Top = 67
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnYearMouseClick
    end
    object BtnHour: TTouchBtn
      Left = 32
      Top = 180
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnHourMouseClick
    end
    object BtnMinute: TTouchBtn
      Left = 152
      Top = 180
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnMinuteMouseClick
    end
    object BtnAMPM: TTouchBtn
      Left = 272
      Top = 180
      Width = 97
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnAMPMMouseClick
    end
  end
end
