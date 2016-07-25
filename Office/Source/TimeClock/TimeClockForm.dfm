object frmTimeClockExport: TfrmTimeClockExport
  Left = 115
  Top = 53
  Width = 584
  Height = 452
  BorderIcons = [biSystemMenu]
  Caption = 'Time Clock Export'
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
    576
    425)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 560
    Height = 378
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    BorderStyle = bsSingle
    TabOrder = 0
    DesignSize = (
      556
      374)
    object Label5: TLabel
      Left = 8
      Top = 96
      Width = 104
      Height = 13
      Caption = 'Configured Export:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label1: TLabel
      Left = 8
      Top = 68
      Width = 86
      Height = 13
      Caption = 'End Login Date:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 312
      Top = 68
      Width = 54
      Height = 13
      Caption = 'End Time:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 392
      Top = 68
      Width = 3
      Height = 13
      Caption = ':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel9: TPanel
      Left = 0
      Top = 0
      Width = 556
      Height = 48
      Align = alTop
      BevelOuter = bvNone
      Color = clAppWorkSpace
      TabOrder = 0
      object lbeTitle: TLabel
        Left = 0
        Top = 0
        Width = 556
        Height = 48
        Align = alClient
        Alignment = taCenter
        AutoSize = False
        Caption = 'Payroll Export'
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
    object ListView1: TListView
      Left = 8
      Top = 112
      Width = 536
      Height = 250
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Name'
          Width = 80
        end
        item
          Caption = 'Department'
          Width = 95
        end
        item
          Caption = 'Date'
          Width = 70
        end
        item
          Caption = 'Start'
          Width = 55
        end
        item
          Caption = 'End'
          Width = 117
        end
        item
          Caption = 'Hours'
          Width = 60
        end
        item
          Caption = 'Break'
          Width = 55
        end>
      GridLines = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 1
      ViewStyle = vsReport
    end
    object dtpDate: TDateTimePicker
      Left = 112
      Top = 64
      Width = 186
      Height = 21
      CalAlignment = dtaLeft
      Date = 39407.3519391088
      Format = 'dddd, dd MMMM, yyyy'
      Time = 39407.3519391088
      DateFormat = dfShort
      DateMode = dmComboBox
      Kind = dtkDate
      ParseInput = False
      TabOrder = 2
      OnChange = DateTimeChange
    end
    object cbHours: TComboBox
      Left = 376
      Top = 64
      Width = 49
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 3
      OnChange = DateTimeChange
      Items.Strings = (
        '01'
        '02'
        '03'
        '04'
        '05'
        '06'
        '07'
        '08'
        '09'
        '10'
        '11'
        '12')
    end
    object cbMinutes: TComboBox
      Left = 440
      Top = 64
      Width = 49
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 4
      OnChange = DateTimeChange
      Items.Strings = (
        '01'
        '02'
        '03'
        '04'
        '05'
        '06'
        '07'
        '08'
        '09'
        '10'
        '11'
        '12')
    end
  end
  object btnExport: TButton
    Left = 383
    Top = 393
    Width = 89
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Payroll Export...'
    TabOrder = 1
    OnClick = btnExportClick
  end
  object btnCancel: TButton
    Left = 479
    Top = 393
    Width = 89
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = btnCancelClick
  end
  object btnDepartmentReport: TButton
    Left = 8
    Top = 393
    Width = 121
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Department Report'
    TabOrder = 3
    OnClick = btnDepartmentReportClick
  end
  object Button4: TButton
    Left = 136
    Top = 393
    Width = 121
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Staff Report'
    TabOrder = 4
    OnClick = Button4Click
  end
end
