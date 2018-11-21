object frmSetUpPosPlus: TfrmSetUpPosPlus
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'POS Plus Communication Set Up'
  ClientHeight = 336
  ClientWidth = 484
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMain: TPanel
    Left = 8
    Top = 8
    Width = 356
    Height = 321
    TabOrder = 0
    object labelPortNumber: TLabel
      Left = 16
      Top = 32
      Width = 136
      Height = 36
      Caption = 'Click to Enter Port Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object labelConfigure: TLabel
      Left = 16
      Top = 102
      Width = 137
      Height = 36
      Caption = 'Click to Configure POS Plus '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object labelValidate: TLabel
      Left = 16
      Top = 179
      Width = 126
      Height = 36
      Caption = 'Click to Validate Details'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object labelOrganization: TLabel
      Left = 16
      Top = 259
      Width = 160
      Height = 36
      Caption = 'Click to Enter Organization Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object tbtnConfigure: TTouchBtn
      Left = 192
      Top = 90
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Configure'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnConfigureMouseClick
    end
    object tbtnValidate: TTouchBtn
      Left = 192
      Top = 171
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Validate'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnValidateMouseClick
    end
    object tbtnPortNumber: TTouchBtn
      Left = 192
      Top = 12
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Color = clNavy
      ParentFont = False
      Caption = 'Port Number'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPortNumberMouseClick
    end
    object tbtnOrganizationNumber: TTouchBtn
      Left = 192
      Top = 248
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Color = clNavy
      ParentFont = False
      Caption = '1234567890'
      ButtonColor = clNavy
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnOrganizationNumberMouseClick
    end
  end
  object pnlSide: TPanel
    Left = 370
    Top = 8
    Width = 106
    Height = 321
    TabOrder = 1
    object tbtnClose: TTouchBtn
      Left = 8
      Top = 16
      Width = 89
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = clGreen
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnCloseMouseClick
    end
  end
end
