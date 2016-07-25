object frmShowPrintout: TfrmShowPrintout
  Tag = 1024
  Left = 31
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Display Report'
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
  OnResize = FormResize
  OnShow = FormShow
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
      OnMouseClick = btnCancelClick
    end
    object btnClose: TTouchBtn
      Left = 424
      Top = 98
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
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
      OnAutoRepeat = btnBillUpAutoRepeat
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
      OnAutoRepeat = btnBillDownAutoRepeat
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
      Top = 178
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
      OnMouseClick = btnClosePrintClick
    end
  end
end
