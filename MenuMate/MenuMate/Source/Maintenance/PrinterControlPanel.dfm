object frmPrinterControlPanel: TfrmPrinterControlPanel
  Tag = 1024
  Left = 271
  Top = 20
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Printing Configuration'
  ClientHeight = 687
  ClientWidth = 832
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlOptions: TPanel
    Left = 687
    Top = 2
    Width = 137
    Height = 677
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 16
      Width = 121
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
  end
  object pnlControl: TPanel
    Left = 8
    Top = 8
    Width = 673
    Height = 677
    BorderWidth = 6
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    object GroupBox1: TGroupBox
      Left = 7
      Top = 0
      Width = 481
      Height = 113
      Caption = 'Printer Profiles'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Memo4: TMemo
        Left = 2
        Top = 21
        Width = 477
        Height = 90
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          
            '   A printer profile groups an actual printer with all the menu ' +
            'courses'
          '   it must print.'
          
            '   You then just link your terminals to the profiles, so you don' +
            #39't have'
          '   to set up each terminal separately.'
          '   A printer can have more than one profile.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object GroupBox2: TGroupBox
      Left = 10
      Top = 589
      Width = 478
      Height = 73
      Caption = 'Configure Printers'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object Memo2: TMemo
        Left = 2
        Top = 21
        Width = 474
        Height = 50
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          '   Select and configure printer types.'
          '   Add a ChefMate virtual printer.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object GroupBox3: TGroupBox
      Left = 12
      Top = 510
      Width = 476
      Height = 73
      Caption = 'Receipt Docket Formatting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object Memo6: TMemo
        Left = 2
        Top = 21
        Width = 472
        Height = 50
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          '   Configure your receipt dockets for each printer. Add headers,'
          '   footers, logos etc.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object GroupBox4: TGroupBox
      Left = 7
      Top = 207
      Width = 481
      Height = 122
      Caption = 'Setup Call Away Printing'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      object Memo1: TMemo
        Left = 2
        Top = 21
        Width = 477
        Height = 99
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          '   A POS or Palm terminal can print a '#39'Call Away'#39' message in the'
          
            '   kitchen when guests have finished a serving. Eg. "Call Away E' +
            'ntrees'
          '   on Table 10".'
          
            '   You must configure which printer profile will print the '#39'Call' +
            ' Away'#39
          '   by assigning serving courses to it.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object tbPrintingOrder: TTouchBtn
      Left = 503
      Top = 32
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Printing By Course'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPrintingOrderMouseClick
    end
    object tbtnCallAwayPrinting: TTouchBtn
      Left = 503
      Top = 240
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Printing Call-Aways'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnCallAwayPrintingMouseClick
    end
    object TouchBtn4: TTouchBtn
      Left = 503
      Top = 448
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Setup Kitchen Docket Format'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn4MouseClick
    end
    object TouchBtn5: TTouchBtn
      Left = 503
      Top = 597
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Setup Printers'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn5MouseClick
    end
    object GroupBox5: TGroupBox
      Left = 7
      Top = 335
      Width = 481
      Height = 98
      Caption = 'Link Terminals to Profiles'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
      object Memo3: TMemo
        Left = 2
        Top = 21
        Width = 477
        Height = 75
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          
            '   To print orders or '#39'Call Aways'#39', you must link each terminal ' +
            'to the'
          '   printer profile(s) needed.'
          
            '   If you wish a terminal to print differently to others, then y' +
            'ou must'
          '   create a different profile.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object TouchBtn2: TTouchBtn
      Left = 503
      Top = 356
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Link Terminals to Printer Profiles'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
    object TouchBtn3: TTouchBtn
      Left = 503
      Top = 519
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Setup Receipt Docket Format'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn3MouseClick
    end
    object GroupBox6: TGroupBox
      Left = 5
      Top = 439
      Width = 483
      Height = 65
      Caption = 'Kitchen Docket Formatting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 11
      object Memo5: TMemo
        Left = 2
        Top = 21
        Width = 479
        Height = 42
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          
            '   Configure the look and layout of your kitchen dockets for eac' +
            'h'
          '   printer profile.')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object GroupBox7: TGroupBox
      Left = 7
      Top = 132
      Width = 481
      Height = 69
      Caption = 'Link Breakdown Categories to Printers'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 12
      object Memo7: TMemo
        Left = 2
        Top = 21
        Width = 477
        Height = 46
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        Lines.Strings = (
          
            'Configure which items are sent to which printer by there breakdo' +
            'wn '
          'category.'
          '')
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
    object tbtnPrintingByCat: TTouchBtn
      Left = 503
      Top = 132
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Printing by Categories'
      ButtonColor = clGray
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BreakdownCatMouseclick
    end
  end
end
