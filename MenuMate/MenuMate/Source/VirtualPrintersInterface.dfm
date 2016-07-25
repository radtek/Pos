object frmVirtualPrintersInterface: TfrmVirtualPrintersInterface
  Tag = 1024
  Left = 148
  Top = 8
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Virtual Printers'
  ClientHeight = 644
  ClientWidth = 584
  Color = clBlack
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
  object pnl1: TPanel
    Left = 0
    Top = 0
    Width = 432
    Height = 644
    Align = alLeft
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 10011108
    TabOrder = 0
    object Label17: TLabel
      Left = 16
      Top = 8
      Width = 63
      Height = 19
      Caption = 'Printers'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbVirtualPrinters: TListBox
      Left = 16
      Top = 32
      Width = 337
      Height = 601
      Style = lbOwnerDrawFixed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 40
      Items.Strings = (
        'Testing'
        'Testing'
        'Testing')
      ParentFont = False
      TabOrder = 0
    end
  end
  object pnl2: TPanel
    Left = 439
    Top = 0
    Width = 145
    Height = 644
    Align = alRight
    BevelInner = bvRaised
    BevelOuter = bvNone
    Color = 10011108
    TabOrder = 1
    object pnlAdd: TPanel
      Left = 8
      Top = 96
      Width = 130
      Height = 65
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Add Printer'
      Color = clMaroon
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
      OnClick = pnlAddClick
    end
    object pnlEdit: TPanel
      Left = 8
      Top = 168
      Width = 130
      Height = 65
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Rename Printer'
      Color = clGreen
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      OnClick = pnlEditClick
    end
    object pnlDelete: TPanel
      Left = 8
      Top = 552
      Width = 130
      Height = 65
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Delete Printer'
      Color = clRed
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 2
      OnClick = pnlDeleteClick
    end
    object btnOk: TPanel
      Tag = -1
      Left = 8
      Top = 8
      Width = 129
      Height = 65
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Ok'
      Color = clGreen
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 3
      OnClick = btnOkClick
    end
    object pnlReassign: TPanel
      Left = 7
      Top = 240
      Width = 130
      Height = 65
      BevelInner = bvRaised
      BiDiMode = bdLeftToRight
      BorderStyle = bsSingle
      Caption = 'Assign To...'
      Color = clNavy
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 4
      OnClick = pnlReassignClick
    end
  end
end
