object frmEFTPOSConfig: TfrmEFTPOSConfig
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'EFTPOS Network Details'
  ClientHeight = 282
  ClientWidth = 417
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object MainPanel: TPanel
    Left = 8
    Top = 8
    Width = 281
    Height = 266
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object tbEFTPOSURL: TTouchBtn
      Left = 8
      Top = 8
      Width = 266
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'IP Address'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbEFTPOSURLMouseClick
    end
    object tbAPIKey: TTouchBtn
      Left = 8
      Top = 95
      Width = 266
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'API Key'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbAPIKeyMouseClick
    end
    object tbDeviceID: TTouchBtn
      Left = 8
      Top = 190
      Width = 266
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Device ID'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbDeviceIDMouseClick
    end
  end
  object SidePanel: TPanel
    Left = 295
    Top = 8
    Width = 114
    Height = 266
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    object tbOK: TTouchBtn
      Left = 16
      Top = 9
      Width = 89
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbOKMouseClick
    end
  end
end
