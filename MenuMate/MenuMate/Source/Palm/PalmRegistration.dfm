object frmPalmRegistration: TfrmPalmRegistration
  Tag = 1024
  Left = 279
  Top = 186
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 8
  Caption = 'Palm Registration'
  ClientHeight = 320
  ClientWidth = 458
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
  object GroupBox4: TPanel
    Left = 0
    Top = 0
    Width = 458
    Height = 320
    Align = alClient
    Color = 10011108
    ParentBackground = False
    TabOrder = 0
    object lbPalms: TListBox
      Left = 80
      Top = 16
      Width = 193
      Height = 289
      Style = lbOwnerDrawFixed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 38
      ParentColor = True
      ParentFont = False
      TabOrder = 0
    end
    object btnRemovePalm: TTouchBtn
      Left = 288
      Top = 88
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Remove Palm'
      ButtonColor = clRed
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnRemovePalmClick
    end
    object btnClose: TTouchBtn
      Left = 288
      Top = 240
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object btnUp: TTouchBtn
      Left = 8
      Top = 16
      Width = 65
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
      OnMouseDown = btnUpMouseDown
      InitialDelay = 250
      RepeatDelay = 60
    end
    object btnDown: TTouchBtn
      Left = 8
      Top = 240
      Width = 65
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
      OnMouseDown = btnDownMouseDown
      InitialDelay = 250
      RepeatDelay = 60
    end
    object tbtnReregister: TTouchBtn
      Left = 288
      Top = 16
      Width = 153
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Set Registration PIN'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnReregisterClick
    end
  end
end
