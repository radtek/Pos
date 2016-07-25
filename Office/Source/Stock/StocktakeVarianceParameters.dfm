object frmVarianceParams: TfrmVarianceParams
  Left = 448
  Top = 187
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Stocktake Parameters'
  ClientHeight = 573
  ClientWidth = 335
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 313
    Height = 209
    BorderWidth = 3
    TabOrder = 0
    object Panel3: TPanel
      Left = 4
      Top = 4
      Width = 305
      Height = 156
      Align = alClient
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 303
        Height = 32
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lblTop: TLabel
          Left = 8
          Top = 0
          Width = 289
          Height = 33
          AutoSize = False
          Caption = 'Enter the parameters for the Stocktake Variance report summary'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object Panel1: TPanel
        Left = 1
        Top = 33
        Width = 303
        Height = 122
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 7
        TabOrder = 1
        object pnlVarianceParams: TPanel
          Left = 7
          Top = 10
          Width = 289
          Height = 105
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Color = clInfoBk
          TabOrder = 0
          Visible = False
          object Label1: TLabel
            Left = 16
            Top = 12
            Width = 110
            Height = 13
            Caption = 'Total Banking Incl. TAX'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
          end
          object Label2: TLabel
            Left = 16
            Top = 76
            Width = 121
            Height = 13
            Caption = 'Variances (e.g. wastage)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
          end
          object Label3: TLabel
            Left = 16
            Top = 44
            Width = 112
            Height = 13
            Caption = 'Total Banking Excl. TAX'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
          end
          object neTotalBankingInc: TNumericEdit
            Left = 176
            Top = 8
            Width = 73
            Height = 21
            Precision = 15
            FixedPoint = True
            Color = 15269887
            TabOrder = 0
            OnChange = neTotalBankingIncChange
            OnKeyPress = neTotalBankingIncKeyPress
            DecimalPlaces = 2
          end
          object neTotalBankingExcl: TNumericEdit
            Left = 176
            Top = 40
            Width = 73
            Height = 21
            Precision = 15
            FixedPoint = True
            Color = 15269887
            TabOrder = 1
            OnKeyPress = neTotalBankingIncKeyPress
            DecimalPlaces = 2
          end
          object neUserVariance: TNumericEdit
            Left = 176
            Top = 72
            Width = 73
            Height = 21
            Precision = 15
            FixedPoint = True
            Color = 15269887
            TabOrder = 2
            OnKeyPress = neTotalBankingIncKeyPress
            DecimalPlaces = 2
          end
        end
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 160
      Width = 305
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object btnOk: TBitBtn
        Left = 128
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 0
        Kind = bkOK
      end
      object btnCancel: TBitBtn
        Left = 216
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 1
        Kind = bkCancel
      end
      object btnPreview: TBitBtn
        Left = 8
        Top = 8
        Width = 81
        Height = 33
        Caption = 'Preview'
        Default = True
        TabOrder = 2
        Visible = False
        OnClick = btnPreviewClick
        NumGlyphs = 2
      end
    end
  end
  object pnlInitialisation: TPanel
    Left = 16
    Top = 222
    Width = 288
    Height = 105
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clInfoBk
    TabOrder = 1
    Visible = False
    object Label4: TLabel
      Left = 51
      Top = 68
      Width = 149
      Height = 13
      Caption = 'NB Every item must be counted'
    end
    object RadioButton2: TRadioButton
      Left = 32
      Top = 20
      Width = 209
      Height = 17
      Caption = 'On Hand values into the Count field'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioButton2Click
    end
    object RadioButton1: TRadioButton
      Tag = 1
      Left = 32
      Top = 52
      Width = 233
      Height = 17
      Caption = 'Zero into the Count field.'
      TabOrder = 1
      OnClick = RadioButton2Click
    end
  end
  object pnlVariance: TPanel
    Left = 16
    Top = 327
    Width = 288
    Height = 105
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clInfoBk
    TabOrder = 2
    Visible = False
    object RadioButton3: TRadioButton
      Left = 26
      Top = 16
      Width = 209
      Height = 17
      Caption = 'All items with variances'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioButton2Click
    end
    object RadioButton4: TRadioButton
      Tag = 1
      Left = 26
      Top = 44
      Width = 233
      Height = 17
      Caption = 'All Items'
      TabOrder = 1
      OnClick = RadioButton2Click
    end
    object rbSetVarianceAmount: TRadioButton
      Tag = 2
      Left = 26
      Top = 72
      Width = 184
      Height = 17
      Caption = 'Item variances ( +/- ) of more than'
      TabOrder = 2
      OnClick = RadioButton2Click
    end
    object neVarianceAmount: TNumericEdit
      Left = 217
      Top = 70
      Width = 49
      Height = 21
      Precision = 3
      FixedPoint = True
      Color = 15269887
      Enabled = False
      MaxLength = 5
      TabOrder = 3
      DecimalPlaces = 2
    end
  end
  object pnlReports: TPanel
    Left = 17
    Top = 432
    Width = 288
    Height = 105
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clInfoBk
    TabOrder = 3
    Visible = False
    object RadioButton6: TRadioButton
      Left = 30
      Top = 25
      Width = 235
      Height = 17
      Caption = 'Stocktake Variance Report ( with Banking )'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioButton2Click
    end
    object RadioButton7: TRadioButton
      Tag = 1
      Left = 30
      Top = 59
      Width = 233
      Height = 17
      Caption = 'Stocktake Value Report'
      TabOrder = 1
      OnClick = RadioButton2Click
    end
  end
  object pnlResetUndo: TPanel
    Left = 19
    Top = 541
    Width = 288
    Height = 105
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = clInfoBk
    TabOrder = 4
    Visible = False
    object RadioButton5: TRadioButton
      Left = 30
      Top = 17
      Width = 235
      Height = 17
      Caption = 'Reset the Count figures to On Hand values'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioButton2Click
    end
    object RadioButton8: TRadioButton
      Tag = 2
      Left = 30
      Top = 71
      Width = 233
      Height = 17
      Caption = 'Undo the Stocktake'
      TabOrder = 1
      OnClick = RadioButton2Click
    end
    object RadioButton9: TRadioButton
      Tag = 1
      Left = 30
      Top = 44
      Width = 235
      Height = 17
      Caption = 'Reset the Count figures to Zero'
      TabOrder = 2
      OnClick = RadioButton2Click
    end
  end
  object rgpReportType: TRadioGroup
    Left = 20
    Top = 624
    Width = 289
    Height = 116
    Color = clInfoBk
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'PDF'
      'HTML'
      'Text'
      'Rich Text')
    ParentColor = False
    TabOrder = 5
  end
end
