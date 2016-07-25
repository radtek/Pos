object frmProcessingWeborder: TfrmProcessingWeborder
  Left = 335
  Top = 554
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Processing web-order ...'
  ClientHeight = 77
  ClientWidth = 417
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblStatus: TLabel
    Left = 16
    Top = 8
    Width = 385
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Stand by...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object pbTxProgress: TProgressBar
    Left = 16
    Top = 32
    Width = 385
    Height = 33
    Min = 0
    Max = 100
    TabOrder = 0
  end
  object socket: TTcpClient
    OnError = socketError
    Top = 48
  end
  object tmrTrigger: TTimer
    OnTimer = tmrTriggerTimer
  end
end
