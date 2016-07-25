object frmSmartCardConfiguration: TfrmSmartCardConfiguration
  Tag = 1024
  Left = 331
  Top = 69
  BorderIcons = []
  BorderWidth = 8
  Caption = 'Smart Card Config'
  ClientHeight = 548
  ClientWidth = 536
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PrintScale = poNone
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel24: TPanel
    Left = 0
    Top = 0
    Width = 385
    Height = 548
    Align = alLeft
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    ExplicitHeight = 462
    object Label4: TLabel
      Left = 16
      Top = 8
      Width = 202
      Height = 19
      Caption = 'Smart Card Configuration'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 16
      Top = 96
      Width = 227
      Height = 19
      Caption = 'Clear all information off card.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label1: TLabel
      Left = 16
      Top = 184
      Width = 342
      Height = 19
      Caption = 'Restore from Inserted Cards Restore Point.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 368
      Width = 165
      Height = 19
      Caption = 'Explore Card History'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 16
      Top = 280
      Width = 250
      Height = 19
      Caption = 'Reassign Card (Keeping points)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lblPreloadCard: TLabel
      Left = 17
      Top = 456
      Width = 195
      Height = 19
      Caption = 'Preload Card with points'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object tbVerifactionCode: TTouchBtn
      Left = 16
      Top = 32
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Manage Syndicate Codes'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbSetVerifactionCode
    end
    object tbtnFormatCard: TTouchBtn
      Left = 16
      Top = 117
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Clear Card'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tbtnFormatCardMouseUp
    end
    object tbtnRestoreCard: TTouchBtn
      Left = 16
      Top = 205
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Restore Card'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tbtnRestoreCardMouseUp
    end
    object tbtnExploreCard: TTouchBtn
      Left = 16
      Top = 389
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Explore Card'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tbtnExploreCardMouseUp
    end
    object tbtnReassignCard: TTouchBtn
      Left = 16
      Top = 301
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Reassign Card'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseUp = tbtnReassignCardMouseUp
    end
    object tbtnPreloadCard: TTouchBtn
      Left = 17
      Top = 477
      Width = 201
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Preload Card'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = preload_card_clicked
    end
  end
  object Panel2: TPanel
    Left = 399
    Top = 0
    Width = 137
    Height = 548
    Align = alRight
    Color = 10011108
    ParentBackground = False
    TabOrder = 1
    ExplicitHeight = 462
    object TouchBtn2: TTouchBtn
      Left = 8
      Top = 8
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
  end
end
