object frmAddSalesType: TfrmAddSalesType
  Tag = 1024
  Left = 33
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Add Sales Type'
  ClientHeight = 206
  ClientWidth = 368
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
    object btnOk: TTouchBtn
      Left = 26
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
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 184
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
      OnMouseClick = btnCancelMouseClick
    end
    object btnName: TTouchBtn
      Left = 24
      Top = 27
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Name:'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = AddSalesTypeDetails
    end
    object btnCode: TTouchBtn
      Tag = 1
      Left = 184
      Top = 27
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Code:'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = AddSalesTypeDetails
    end
  end
end
