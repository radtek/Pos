object frmPrinterRedirect: TfrmPrinterRedirect
  Tag = 1024
  Left = 271
  Top = 90
  BorderIcons = []
  Caption = 'Printer Selection'
  ClientHeight = 551
  ClientWidth = 598
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
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 441
    Height = 538
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object lbCaption: TLabel
      Left = 8
      Top = 8
      Width = 194
      Height = 22
      Caption = 'Please Select A Printer'
    end
    object lbPrinters: TListBox
      Left = 8
      Top = 48
      Width = 425
      Height = 481
      Style = lbOwnerDrawFixed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 45
      Items.Strings = (
        'Printer 1'
        'Printer 2'
        'Printer 3'
        'Printer 4'
        'Printer 5')
      ParentFont = False
      TabOrder = 0
    end
  end
  object pnlButtons: TPanel
    Left = 456
    Top = 8
    Width = 137
    Height = 538
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object tbCancel: TTouchBtn
      Left = 8
      Top = 464
      Width = 121
      Height = 65
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
      OnMouseClick = tbCancelClick
    end
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 8
      Width = 121
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
    end
  end
end
