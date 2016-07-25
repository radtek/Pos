object frmTransferItemOrGuest: TfrmTransferItemOrGuest
  Left = 376
  Top = 121
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 3
  Caption = 'Select Guest'
  ClientHeight = 194
  ClientWidth = 368
  Color = 14342874
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 24
  object Panel1: TPanel
    Left = 234
    Top = 0
    Width = 134
    Height = 194
    Align = alRight
    BorderWidth = 3
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnClose: TTouchBtn
      Left = 8
      Top = 125
      Width = 122
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object btnOK: TTouchBtn
      Left = 8
      Top = 8
      Width = 122
      Height = 65
      BorderWidth = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindow
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOKMouseClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 235
    Height = 219
    BorderWidth = 3
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object lbeTransfer: TLabel
      Left = 10
      Top = 60
      Width = 100
      Height = 33
      AutoSize = False
      Caption = 'Transfer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object lbeTo: TLabel
      Left = 10
      Top = 140
      Width = 60
      Height = 33
      AutoSize = False
      Caption = 'To'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object lbeItemName: TLabel
      Left = 10
      Top = 11
      Width = 220
      Height = 33
      AutoSize = False
      Caption = 'Transfer From'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object btnDestination: TTouchBtn
      Left = 100
      Top = 125
      Width = 122
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Dest'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDestinationClick
    end
    object btnQuantity: TTouchBtn
      Left = 100
      Top = 50
      Width = 122
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Qty'
      ButtonColor = 6052441
      LatchedColor = clNavy
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnQuantityClick
    end
  end
end
