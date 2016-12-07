object frmEJournal: TfrmEJournal
  Tag = 1024
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'E-Journal'
  ClientHeight = 693
  ClientWidth = 584
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 584
    Height = 693
    Align = alClient
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnCancel: TTouchBtn
      Left = 424
      Top = 16
      Width = 153
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
      OnMouseClick = btnCancelMouseClick
    end
    object btnSaveAsPDF: TTouchBtn
      Left = 423
      Top = 394
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Save as PDF'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSavePDFMouseClick
    end
    object btnBillUp: TTouchBtn
      Left = 504
      Top = 624
      Width = 73
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clWhite
      ParentFont = False
      Caption = '/\'
      ButtonColor = clBtnFace
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnReportUpAutoRepeat
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnBillDown: TTouchBtn
      Left = 424
      Top = 624
      Width = 73
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clWhite
      ParentFont = False
      Caption = '\/'
      ButtonColor = clBtnFace
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = btnReportDownAutoRepeat
      InitialDelay = 25
      RepeatDelay = 25
    end
    object memReceipt: TMemo
      Left = 8
      Top = 8
      Width = 409
      Height = 681
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Courier New'
      Font.Style = []
      Lines.Strings = (
        ''
        ''
        '          MenuMate By IQWorks Ltd           '
        '            Pre-Install Version             '
        '   NOT TO BE USED IN A COMMERCIAL CONTEXT   '
        '         PLEASE CONTACT IQWORKS LTD         '
        '            Phone : 03 343 1202             '
        '        EMail : Sales@IQWorks.co.nz         '
        '--------------------------------------------'
        'MIchael on ALPHA         26/09/2002 16:59:52'
        '--------------------------------------------'
        '   A Highland Feed                    $10.90'
        '   Eggs Benedict                      $10.50'
        '   Scrambled Eggs and Bacon            $7.50'
        '--------------------------------------------'
        'Total                                 $28.90'
        '--------------------------------------------'
        'Cash                                  $28.90'
        '                                            '
        '          MenuMate By IQWorks Ltd           '
        '            Pre-Install Version             '
        '   NOT TO BE USED IN A COMMERCIAL CONTEXT   '
        '         PLEASE CONTACT IQWORKS LTD         '
        '            Phone : 03 343 1202             '
        '        EMail : Sales@IQWorks.co.nz  ')
      ParentFont = False
      TabOrder = 4
    end
    object btnClosePrint: TTouchBtn
      Left = 423
      Top = 466
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close and Print'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPrintMouseClick
    end
    object GroupBoxFilter: TGroupBox
      Left = 423
      Top = 111
      Width = 154
      Height = 226
      Caption = 'Filter'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      object LabelToDate: TLabel
        Left = 3
        Top = 82
        Width = 50
        Height = 16
        Caption = 'To Date'
      end
      object LabelFromDate: TLabel
        Left = 3
        Top = 26
        Width = 67
        Height = 16
        Caption = 'From Date'
      end
      object FromDateTimePicker: TDateTimePicker
        Left = 4
        Top = 48
        Width = 146
        Height = 28
        Date = 42711.725326111110000000
        Time = 42711.725326111110000000
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnCloseUp = FromDateTimePickerCloseUp
        OnChange = FromDateOnChange
        OnExit = FromDateTimePickerExit
      end
      object ToDateTimePicker: TDateTimePicker
        Left = 3
        Top = 104
        Width = 146
        Height = 28
        Date = 42711.722845196760000000
        Time = 42711.722845196760000000
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnChange = ToDateOnChange
      end
      object btnGenerate: TTouchBtn
        Left = 3
        Top = 146
        Width = 146
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'Generate'
        ButtonColor = clGreen
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnGenerateMouseClick
      end
    end
  end
end
