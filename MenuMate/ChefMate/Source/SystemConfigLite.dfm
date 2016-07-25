object frmSystemConfig: TfrmSystemConfig
  Tag = 1024
  Left = 389
  Top = 258
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'System Configuration'
  ClientHeight = 565
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
    Height = 493
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
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object Label8: TLabel
      Left = 14
      Top = 152
      Width = 133
      Height = 16
      Caption = 'Barcode Reader Port'
    end
    object Label1: TLabel
      Left = 14
      Top = 373
      Width = 235
      Height = 32
      Caption = 
        'Number of Minutes to wait before an incomplete order is redispla' +
        'yed.'
      WordWrap = True
    end
    object tbOk: TTouchBtn
      Left = 14
      Top = 425
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clGreen
      ParentFont = False
      Caption = 'Ok'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbOkClick
    end
    object tbInterbase: TTouchBtn
      Left = 14
      Top = 9
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = 'Interbase Server IP Address'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbInterbaseClick
    end
    object tbDBLocation: TTouchBtn
      Left = 14
      Top = 81
      Width = 411
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = 'Database Location'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbDBLocationClick
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
    object tbUnsSannedWarning: TTouchBtn
      Left = 280
      Top = 361
      Width = 145
      Height = 56
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = '5'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbUnsSannedWarningClick
    end
    object cbRemovedWhenScanned: TCheckBox
      Left = 16
      Top = 256
      Width = 409
      Height = 17
      Caption = 'Remove Orders when they are scanned.'
      TabOrder = 6
      OnClick = cbRemovedWhenScannedClick
    end
    object cbDisplayDynamicOrders: TCheckBox
      Left = 16
      Top = 280
      Width = 409
      Height = 17
      Caption = 'Display Dynamic Orders'
      TabOrder = 7
      OnClick = cbDisplayDynamicOrdersClick
    end
    object cbConfirmBeforeBumping: TCheckBox
      Left = 16
      Top = 304
      Width = 409
      Height = 17
      Caption = 'Confirm before bumping off orders.'
      TabOrder = 8
      OnClick = cbConfirmBeforeBumpingClick
    end
    object cbRemovedWhenClosed: TCheckBox
      Left = 16
      Top = 232
      Width = 409
      Height = 17
      Caption = 'Remove Orders when make time closed in DB.'
      TabOrder = 9
      OnClick = cbRemovedWhenClosedClick
    end
    object TouchBtn1: TTouchBtn
      Left = 14
      Top = 327
      Width = 243
      Height = 31
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = 'Reset Bar Widths'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
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
    ParentBackground = False
    TabOrder = 1
    object tbtnExit: TTouchBtn
      Left = 264
      Top = 5
      Width = 161
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Color = clBlack
      ParentFont = False
      Caption = 'Exit'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnExitClick
    end
  end
end
