object frmSelectZed: TfrmSelectZed
  Tag = 1024
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select Z Report'
  ClientHeight = 592
  ClientWidth = 738
  Color = clBtnFace
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 738
    Height = 592
    Align = alClient
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object Label1: TLabel
      Left = 432
      Top = 296
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
      Left = 592
      Top = 231
      Width = 137
      Height = 57
      GroupIndex = 2
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
      Left = 448
      Top = 231
      Width = 137
      Height = 57
      GroupIndex = 2
      Down = True
      Caption = 'This Terminal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = sbAllClick
    end
    object btnClose: TTouchBtn
      Left = 448
      Top = 8
      Width = 281
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object btnPrev: TTouchBtn
      Left = 8
      Top = 520
      Width = 105
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '<<'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPrevClick
    end
    object btnPrint: TTouchBtn
      Left = 448
      Top = 82
      Width = 281
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
    object btnSendEmail: TTouchBtn
      Left = 448
      Top = 156
      Width = 281
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Email Zed'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSendEmailClick
    end
    object btnNext: TTouchBtn
      Left = 312
      Top = 520
      Width = 105
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = '>>'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNextClick
    end
    object calZed: TMonthCalendar
      Left = 423
      Top = 325
      Width = 288
      Height = 263
      AutoSize = True
      Date = 37497.878441041670000000
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = calZedClick
    end
    object btnBillUp: TTouchBtn
      Left = 128
      Top = 520
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
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnBillUpMouseDown
      InitialDelay = 50
      RepeatDelay = 25
    end
    object btnBillDown: TTouchBtn
      Left = 224
      Top = 520
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
      ButtonColor = clWhite
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseDown = btnBillDownMouseDown
      InitialDelay = 25
      RepeatDelay = 25
    end
    object memReceipt: TMemo
      Left = 8
      Top = 8
      Width = 409
      Height = 505
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
      TabOrder = 7
    end
  end
end
