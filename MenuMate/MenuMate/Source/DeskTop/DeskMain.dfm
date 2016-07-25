object frmMain: TfrmMain
  Tag = 1024
  Left = 458
  Top = 148
  BorderStyle = bsDialog
  Caption = 'DeskMate'
  ClientHeight = 516
  ClientWidth = 191
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pButtons: TPanel
    Left = 0
    Top = 0
    Width = 191
    Height = 516
    Align = alClient
    BevelInner = bvLowered
    BevelWidth = 2
    BorderWidth = 1
    Color = 14342874
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object lbeVersion: TLabel
      Left = 16
      Top = 488
      Width = 47
      Height = 16
      Caption = 'Version'
    end
    object TouchBtn1: TTouchBtn
      Left = 16
      Top = 16
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'System'
      ButtonColor = 33023
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnStartClick
    end
    object TouchBtn5: TTouchBtn
      Left = 16
      Top = 392
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseClick
    end
    object tbtnOnlineUpdate: TTouchBtn
      Left = 16
      Top = 155
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Update'
      ButtonColor = clPurple
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOnlineUpdateMouseClick
    end
    object tbtnOnlineHelp: TTouchBtn
      Left = 16
      Top = 321
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Technical Support'
      ButtonColor = clSkyBlue
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOnlineHelpMouseClick
      OnMouseDown = tbtnOnlineHelpMouseDown
    end
    object tbtnTills: TTouchBtn
      Left = 16
      Top = 85
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Tills'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnTillsMouseClick
    end
    object tbtnDatabase: TTouchBtn
      Left = 16
      Top = 227
      Width = 161
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Database'
      ButtonColor = clTeal
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnDatabaseMouseClick
    end
  end
end
