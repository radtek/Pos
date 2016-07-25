object frmItemSettings: TfrmItemSettings
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Item Settings'
  ClientHeight = 341
  ClientWidth = 511
  Color = clBackground
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = [fsBold]
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object pnlAll: TPanel
    Left = 0
    Top = 0
    Width = 369
    Height = 341
    Align = alLeft
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    VerticalAlignment = taAlignTop
    object gbItemSettings: TGroupBox
      Left = 8
      Top = 8
      Width = 353
      Height = 102
      Caption = ' Item settings '
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object lblPrice: TLabel
        Left = 16
        Top = 28
        Width = 40
        Height = 19
        Caption = 'Price:'
      end
      object lblSpecialPrice: TLabel
        Left = 16
        Top = 63
        Width = 94
        Height = 19
        Caption = 'Special price:'
        Visible = False
      end
      object edPrice: TEdit
        Left = 118
        Top = 24
        Width = 219
        Height = 27
        TabOrder = 0
        OnClick = price_clicked
      end
      object edSpecialPrice: TEdit
        Left = 118
        Top = 59
        Width = 219
        Height = 27
        TabOrder = 1
        Visible = False
        OnClick = special_price_clicked
      end
    end
    object gbProductCountSettings: TGroupBox
      Left = 8
      Top = 118
      Width = 353
      Height = 215
      Caption = ' Product count settings '
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 1
      object lblCurrentCount: TLabel
        Left = 16
        Top = 141
        Width = 129
        Height = 19
        Caption = 'Number available:'
      end
      object lblWarnWhenBelow: TLabel
        Left = 16
        Top = 173
        Width = 133
        Height = 19
        Caption = 'Warn when below:'
      end
      object lblDefaultValue: TLabel
        Left = 16
        Top = 106
        Width = 75
        Height = 19
        Caption = 'Default to:'
      end
      object edCurrentCount: TEdit
        Left = 157
        Top = 137
        Width = 180
        Height = 27
        TabOrder = 0
        OnClick = available_quantity_clicked
      end
      object edWarnWhenBelow: TEdit
        Left = 157
        Top = 172
        Width = 180
        Height = 27
        TabOrder = 1
        OnClick = warn_when_below_clicked
      end
      object cbResetToDefaultValue: TCheckBox
        Left = 16
        Top = 76
        Width = 321
        Height = 18
        Caption = 'Reset to default value on zed'
        TabOrder = 3
        OnClick = reset_to_default_value_clicked
      end
      object edDefaultValue: TEdit
        Left = 157
        Top = 102
        Width = 180
        Height = 27
        TabOrder = 4
        OnClick = default_clicked
      end
      object cbEnableCountingFunctionality: TCheckBox
        Left = 16
        Top = 24
        Width = 321
        Height = 18
        Caption = 'Enable counting functionality'
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 5
        OnClick = EnableCountingFunctionalityClicked
      end
      object cbEnableWarnings: TCheckBox
        Left = 16
        Top = 50
        Width = 321
        Height = 18
        Caption = 'Enable warnings'
        TabOrder = 2
        OnClick = EnableWarningsClicked
      end
    end
  end
  object Panel2: TPanel
    Left = 375
    Top = 0
    Width = 136
    Height = 341
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    DesignSize = (
      136
      341)
    object btnCancel: TTouchBtn
      Left = 8
      Top = 275
      Width = 120
      Height = 58
      Anchors = [akLeft, akBottom]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = cancel_clicked
    end
    object btnSave: TTouchBtn
      Left = 8
      Top = 8
      Width = 120
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = save_clicked
    end
  end
end
