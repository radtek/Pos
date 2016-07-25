object frmHoldSend: TfrmHoldSend
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  BorderWidth = 5
  Caption = 'Hold or Send'
  ClientHeight = 81
  ClientWidth = 316
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 316
    Height = 81
    Align = alTop
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object tbtnHold: TTouchBtn
      Left = 8
      Top = 8
      Width = 145
      Height = 65
      ParentCustomHint = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Hold'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnHoldMouseClick
    end
    object tbtnSend: TTouchBtn
      Left = 163
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Send'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSendMouseClick
    end
  end
end
