object frmCaptNamePhone: TfrmCaptNamePhone
  Tag = 1024
  Left = 33
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Capture Name and Phone'
  ClientHeight = 463
  ClientWidth = 677
  Color = clBlack
  TransparentColorValue = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 678
    Height = 600
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    DesignSize = (
      678
      600)
    object lblUsername: TLabel
      Left = 8
      Top = 13
      Width = 137
      Height = 20
      AutoSize = False
      Caption = 'Customer Name : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlBottom
      WordWrap = True
    end
    object lblPhoneNumber: TLabel
      Left = 8
      Top = 91
      Width = 217
      Height = 20
      AutoSize = False
      Caption = 'Customer Phone Number : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlBottom
      WordWrap = True
    end
    object edCustomerName: TEdit
      Left = 8
      Top = 39
      Width = 365
      Height = 35
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 20
      ParentFont = False
      TabOrder = 3
    end
    object edCustomerPhone: TEdit
      Left = 8
      Top = 119
      Width = 365
      Height = 35
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 20
      ParentFont = False
      TabOrder = 0
    end
    object btnOk: TTouchBtn
      Left = 500
      Top = 20
      Width = 120
      Height = 55
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
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
    object TouchBtn1: TTouchBtn
      Left = 500
      Top = 110
      Width = 120
      Height = 55
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object TouchKeyboard1: TMMTouchKeyboard
      Left = 2
      Top = 175
      Width = 673
      Height = 290
      ParentColor = True
      BevelOuter = bvNone
      GridBorderWidth = 8
      ButtonGapWidth = 2
      ButtonGapHeight = 2
      ButtonColor = 6052441
      OnClick = TouchKeyboard1Click
    end
  end
end
