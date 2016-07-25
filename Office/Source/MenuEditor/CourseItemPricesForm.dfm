object frmCourseItemPrices: TfrmCourseItemPrices
  Left = 511
  Top = 107
  AutoScroll = False
  Caption = 'Prices'
  ClientHeight = 399
  ClientWidth = 458
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  DesignSize = (
    458
    399)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 168
    Top = 8
    Width = 53
    Height = 13
    Caption = 'Adjust by'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 280
    Top = 8
    Width = 169
    Height = 81
    Alignment = taCenter
    AutoSize = False
    Caption = 
      'Set to 0.0% to reset to the original values. Enter a negative pe' +
      'rcentage to reduce prices.'
    Layout = tlCenter
    WordWrap = True
  end
  object DoneButton: TButton
    Left = 280
    Top = 360
    Width = 81
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    TabOrder = 0
    OnClick = DoneButtonClick
  end
  object CancelButton1: TButton
    Left = 368
    Top = 360
    Width = 81
    Height = 33
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object sgCourse: TStringGrid
    Left = 8
    Top = 96
    Width = 441
    Height = 257
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 4
    DefaultColWidth = 108
    DefaultRowHeight = 17
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
    TabOrder = 2
  end
  object rgRounding: TRadioGroup
    Left = 8
    Top = 8
    Width = 153
    Height = 81
    Caption = 'Roundig'
    TabOrder = 3
  end
  object nePercent: TNumericEdit
    Left = 168
    Top = 24
    Width = 97
    Height = 21
    Precision = 10
    NumericType = ntPercent
    FixedPoint = True
    TabOrder = 4
    DecimalPlaces = 1
  end
  object btnCalculate: TButton
    Left = 168
    Top = 56
    Width = 97
    Height = 33
    Caption = 'Calculate'
    Default = True
    TabOrder = 5
    OnClick = btnCalculateClick
  end
end
