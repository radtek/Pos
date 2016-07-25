object frmProcessing: TfrmProcessing
  Left = 376
  Top = 560
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Please Wait...'
  ClientHeight = 113
  ClientWidth = 538
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCloseQuery = FormCloseQuery
  OnDeactivate = FormDeactivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 538
    Height = 113
    Align = alClient
    BevelWidth = 2
    BiDiMode = bdLeftToRight
    BorderWidth = 2
    BorderStyle = bsSingle
    Color = 14342874
    Ctl3D = True
    ParentBiDiMode = False
    ParentBackground = False
    ParentCtl3D = False
    TabOrder = 0
    object Label1: TLabel
      Left = 104
      Top = 48
      Width = 337
      Height = 16
      Alignment = taCenter
      AutoSize = False
    end
    object Label2: TLabel
      Left = 4
      Top = 4
      Width = 526
      Height = 19
      Align = alTop
      AutoSize = False
      Caption = ' Please Wait...'
      Color = clActiveCaption
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clCaptionText
      Font.Height = -14
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
    end
    object ProgressBar: TProgressBar
      Left = 16
      Top = 84
      Width = 425
      Height = 13
      TabOrder = 0
    end
    object Animate1: TAnimate
      Left = 16
      Top = 31
      Width = 80
      Height = 50
      Active = True
      CommonAVI = aviFindFolder
      StopFrame = 27
    end
    object btnCancel: TTouchBtn
      Left = 448
      Top = 32
      Width = 81
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelClick
    end
  end
end
