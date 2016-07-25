object frmRoomConfiguration: TfrmRoomConfiguration
  Tag = 1024
  Left = 1225
  Top = 232
  Caption = 'Room Configuration'
  ClientHeight = 192
  ClientWidth = 527
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 384
    Top = 8
    Width = 137
    Height = 177
    Color = 10011108
    TabOrder = 0
    object btnOk: TTouchBtn
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
      OnMouseClick = btnOkClick
    end
  end
  object Panel24: TPanel
    Left = 8
    Top = 8
    Width = 369
    Height = 177
    Color = 10011108
    TabOrder = 1
    object Label4: TLabel
      Left = 16
      Top = 8
      Width = 167
      Height = 19
      Caption = 'Rooms Configuration'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label1: TLabel
      Left = 16
      Top = 48
      Width = 340
      Height = 19
      Caption = 'Path to read and write Room Interface files.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object cbImportRoomFiles: TCheckBox
      Left = 16
      Top = 144
      Width = 257
      Height = 17
      Caption = 'Import Room Interface files'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = cbImportRoomFilesClick
    end
    object tbGuestMaster: TTouchBtn
      Left = 16
      Top = 72
      Width = 345
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbGuestMasterClick
    end
  end
end
