object frmSystemConfig: TfrmSystemConfig
  Tag = 1024
  Left = 489
  Top = 176
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'System Configuration'
  ClientHeight = 412
  ClientWidth = 436
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
  object pButtons: TPanel
    Left = 0
    Top = 72
    Width = 436
    Height = 340
    Align = alBottom
    BevelInner = bvLowered
    BevelWidth = 2
    BorderWidth = 1
    Color = 10011108
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label8: TLabel
      Left = 14
      Top = 152
      Width = 133
      Height = 16
      Caption = 'Barcode Reader Port'
    end
    object tbOk: TTouchButton
      Left = 14
      Top = 265
      Width = 411
      Height = 60
      Caption = 'Ok'
      Color = clGreen
      UpColour = clGreen
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = tbOkClick
    end
    object tbInterbase: TTouchButton
      Left = 14
      Top = 9
      Width = 411
      Height = 60
      Caption = 'Interbase Server IP Address'
      Color = clMaroon
      UpColour = clMaroon
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = tbInterbaseClick
    end
    object tbDBLocation: TTouchButton
      Left = 14
      Top = 81
      Width = 411
      Height = 60
      Caption = 'Database Location'
      Color = clMaroon
      UpColour = clMaroon
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = tbDBLocationClick
    end
    object cbBarCodePort: TComboBox
      Left = 14
      Top = 171
      Width = 411
      Height = 30
      Style = csDropDownList
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ItemHeight = 22
      ParentFont = False
      TabOrder = 3
      OnChange = cbBarCodePortChange
      Items.Strings = (
        'None'
        'Comm 01'
        'Comm 02'
        'Comm 03'
        'Comm 04'
        'Comm 05'
        'Comm 06'
        'Comm 07'
        'Comm 08'
        'Comm 09'
        'Comm 10'
        'Comm 11'
        'Comm 12'
        'Comm 13'
        'Comm 14'
        'Comm 15'
        'Comm 16')
    end
    object cbAutoCloseWhenBumped: TCheckBox
      Left = 16
      Top = 208
      Width = 409
      Height = 17
      Caption = 'Auto close make times when "Bumped"'
      TabOrder = 4
      OnClick = cbAutoCloseWhenBumpedClick
    end
    object cbPaperless: TCheckBox
      Left = 16
      Top = 232
      Width = 409
      Height = 17
      Caption = 'Paperless Kitchen'
      TabOrder = 5
      OnClick = cbPaperlessClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 436
    Height = 70
    Align = alTop
    BevelInner = bvLowered
    BevelWidth = 2
    Color = 10011108
    TabOrder = 1
    object TouchButton2: TTouchButton
      Left = 264
      Top = 5
      Width = 161
      Height = 60
      Caption = 'Exit'
      Color = clMaroon
      UpColour = clMaroon
      DownColour = clGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      BorderStyle = bsSingle
      BevelWidth = 2
      ParentCtl3D = False
      OnClick = TouchButton2Click
    end
  end
end
