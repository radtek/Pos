object frmSelectDate: TfrmSelectDate
  Left = 150
  Top = 218
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Select Date'
  ClientHeight = 340
  ClientWidth = 508
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 489
    Height = 321
    BorderWidth = 4
    TabOrder = 0
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 479
      Height = 266
      Align = alClient
      BevelOuter = bvLowered
      Constraints.MinWidth = 330
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 477
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 8
          Top = 0
          Width = 297
          Height = 49
          AutoSize = False
          Caption = 'lbeTitle'
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
      object Calendar: TMonthCalendar
        Left = 1
        Top = 49
        Width = 477
        Height = 216
        Align = alClient
        CalColors.BackColor = 14221311
        CalColors.TextColor = clWhite
        CalColors.TitleBackColor = clNavy
        CalColors.MonthBackColor = clTeal
        CalColors.TrailingTextColor = clBlack
        Date = 37036.801357419
        TabOrder = 1
        OnDblClick = CalendarDblClick
      end
    end
    object Panel10: TPanel
      Left = 5
      Top = 271
      Width = 479
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object btnOk: TBitBtn
        Left = 304
        Top = 8
        Width = 81
        Height = 33
        Caption = 'Ok'
        TabOrder = 0
        Kind = bkOK
      end
      object btnCancel: TBitBtn
        Left = 392
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 1
        Kind = bkCancel
      end
    end
  end
end
