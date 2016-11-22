object frmRegenerateMallReport: TfrmRegenerateMallReport
  Left = 351
  Top = 0
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Regenerate Mall Export Report'
  ClientHeight = 398
  ClientWidth = 748
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 609
    Height = 398
    Align = alCustom
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object lbGenPath: TLabel
      Left = 16
      Top = 324
      Width = 135
      Height = 18
      Caption = 'Report Location Path'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object gbRegenReport: TGroupBox
      Left = 16
      Top = 8
      Width = 577
      Height = 297
      TabOrder = 0
      object lbStartDate: TLabel
        Left = 27
        Top = 16
        Width = 67
        Height = 18
        Caption = 'Start Date'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbEndDate: TLabel
        Left = 304
        Top = 19
        Width = 60
        Height = 18
        Caption = 'End Date'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbStartHour: TLabel
        Left = 27
        Top = 240
        Width = 31
        Height = 18
        Caption = 'Hour'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbStartMin: TLabel
        Left = 151
        Top = 240
        Width = 50
        Height = 18
        Caption = 'Minutes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbEndHour: TLabel
        Left = 304
        Top = 240
        Width = 31
        Height = 18
        Caption = 'Hour'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbEndMin: TLabel
        Left = 432
        Top = 240
        Width = 50
        Height = 18
        Caption = 'Minutes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object mcStartDate: TMonthCalendar
        Left = 27
        Top = 43
        Width = 246
        Height = 183
        AutoSize = True
        Date = 42200.818103009260000000
        DoubleBuffered = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 0
        OnClick = mcStartDateClick
      end
      object mcEndDate: TMonthCalendar
        Left = 304
        Top = 43
        Width = 246
        Height = 183
        AutoSize = True
        Date = 42200.818103009260000000
        DoubleBuffered = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 1
        OnClick = mcEndDateClick
      end
      object cbStartHour: TComboBox
        Left = 27
        Top = 264
        Width = 122
        Height = 21
        Style = csDropDownList
        TabOrder = 2
        OnChange = cbStartHourChange
      end
      object cbStartMin: TComboBox
        Left = 151
        Top = 264
        Width = 122
        Height = 21
        Style = csDropDownList
        TabOrder = 3
        OnChange = cbStartMinChange
      end
      object cbEndHour: TComboBox
        Left = 304
        Top = 264
        Width = 122
        Height = 21
        Style = csDropDownList
        TabOrder = 4
        OnChange = cbEndHourChange
      end
      object cbEndMin: TComboBox
        Left = 432
        Top = 264
        Width = 122
        Height = 21
        Style = csDropDownList
        TabOrder = 5
        OnChange = cbEndMinChange
      end
    end
    object btnGenerate: TTouchBtn
      Left = 448
      Top = 323
      Width = 118
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Generate Report'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnGenerateMouseClick
    end
    object edLocationPath: TEdit
      Left = 16
      Top = 348
      Width = 273
      Height = 21
      TabOrder = 2
      OnMouseUp = edLocationPathMouseUp
    end
    object btnLoadPath: TTouchBtn
      Left = 320
      Top = 323
      Width = 122
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Load Mall Path'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnLoadPathMouseClick
    end
  end
  object Panel2: TPanel
    Left = 615
    Top = 0
    Width = 133
    Height = 398
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 8
      Top = 8
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
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
    object btnCancel: TTouchBtn
      Left = 8
      Top = 323
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
  end
end

