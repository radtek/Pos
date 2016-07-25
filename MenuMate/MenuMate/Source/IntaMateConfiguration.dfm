object frmIntaMateConfiguration: TfrmIntaMateConfiguration
  Left = 374
  Top = 118
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'System Configuration'
  ClientHeight = 289
  ClientWidth = 531
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel24: TPanel
    Left = 8
    Top = 8
    Width = 369
    Height = 273
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object tbPAddress: TTouchBtn
      Left = 16
      Top = 98
      Width = 345
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Server TCP IP Address'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixIPAddressClick
    end
    object tbPortNumber: TTouchBtn
      Left = 16
      Top = 156
      Width = 345
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Server Port Number'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixPortNumberClick
    end
    object tbPOSID: TTouchBtn
      Left = 16
      Top = 41
      Width = 345
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'P.O.S ID'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPhoenixIDClick
    end
    object tbTimeOuts: TTouchBtn
      Left = 16
      Top = 214
      Width = 345
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'TCP Time out (ms)'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbTimeOutsMouseClick
    end
    object pnlLabel: TPanel
      Left = 0
      Top = 0
      Width = 369
      Height = 34
      Align = alTop
      BorderWidth = 1
      Caption = 'IntaMate System Configuration'
      Color = 2200566
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentBackground = False
      ParentFont = False
      TabOrder = 4
      ExplicitLeft = 6
      ExplicitTop = 6
      ExplicitWidth = 435
    end
  end
  object Panel2: TPanel
    Left = 384
    Top = 8
    Width = 137
    Height = 273
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 5
      Top = 5
      Width = 127
      Height = 65
      Align = alTop
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
      ExplicitLeft = 8
      ExplicitTop = 8
      ExplicitWidth = 121
    end
  end
end
