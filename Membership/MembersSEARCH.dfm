object frmMembersSEARCH: TfrmMembersSEARCH
  Left = 256
  Top = 181
  Width = 270
  Height = 187
  BorderIcons = [biSystemMenu]
  Caption = 'Member Search'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnStart: TButton
    Left = 25
    Top = 124
    Width = 87
    Height = 25
    Hint = 'Search for the next unused number'
    Caption = 'Start'
    TabOrder = 0
    Visible = False
    OnClick = btnStartClick
  end
  object btnCabcel: TButton
    Left = 153
    Top = 124
    Width = 87
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object gbxCopy: TGroupBox
    Left = 13
    Top = 10
    Width = 237
    Height = 99
    TabOrder = 2
    Visible = False
    object Label56: TLabel
      Left = 30
      Top = 28
      Width = 88
      Height = 13
      Caption = 'Copy from Member'
    end
    object lblMemberNumber: TLabel
      Left = 32
      Top = 62
      Width = 163
      Height = 13
      Caption = 'Member Number 1000 will be used'
    end
    object dbeCopyMember: TwwDBEdit
      Tag = 99
      Left = 149
      Top = 26
      Width = 60
      Height = 19
      HelpContext = 2
      AutoSize = False
      BorderStyle = bsNone
      CharCase = ecUpperCase
      Color = clWhite
      Frame.Enabled = True
      Frame.NonFocusBorders = [efLeftBorder, efTopBorder, efRightBorder, efBottomBorder]
      Frame.FocusStyle = efsFrameSunken
      Frame.NonFocusStyle = efsFrameSunken
      Frame.NonFocusTransparentFontColor = clBlack
      Picture.PictureMask = '#*#'
      TabOrder = 0
      UnboundDataType = wwDefault
      WantReturns = False
      WordWrap = False
      OnKeyPress = dbeMaxKeyPress
    end
  end
  object rgpMemberNumbers: TRadioGroup
    Left = 13
    Top = 11
    Width = 235
    Height = 101
    ItemIndex = 0
    Items.Strings = (
      'Next Number'
      'Find Blank Number'
      'Use Non Active Member'
      'Use Non Financial Member'
      'Use Non Financial Member with no Points')
    TabOrder = 3
    Visible = False
  end
  object btnSave: TButton
    Left = 30
    Top = 124
    Width = 87
    Height = 25
    Caption = 'Save and Close'
    TabOrder = 4
    Visible = False
    OnClick = btnSaveClick
  end
end
