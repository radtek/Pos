object frmQuickMemberSetupOptions: TfrmQuickMemberSetupOptions
  Left = 200
  Top = 200
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Quick Member Setup Options'
  ClientHeight = 297
  ClientWidth = 377
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = initialize_checkboxes
  PixelsPerInch = 96
  TextHeight = 13
  object pnlContents: TPanel
    Left = 8
    Top = 8
    Width = 361
    Height = 153
    Caption = 'pnlContents'
    TabOrder = 0
    object gbFields: TGroupBox
      Left = 8
      Top = 39
      Width = 345
      Height = 105
      Caption = ' What fields are mandatory? '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object cbPhoneNumberIsMandatory: TCheckBox
        Left = 16
        Top = 72
        Width = 185
        Height = 17
        Caption = 'Mobile Phone Number'
        Enabled = False
        TabOrder = 0
      end
      object cbEmailAddressIsMandatory: TCheckBox
        Left = 16
        Top = 49
        Width = 185
        Height = 17
        Caption = 'Email Address'
        Enabled = False
        TabOrder = 1
      end
      object cbDateOfBirthIsMandatory: TCheckBox
        Left = 16
        Top = 26
        Width = 185
        Height = 17
        Caption = 'Date of Birth'
        Enabled = False
        TabOrder = 2
      end
    end
    object cbQMCIsEnabled: TCheckBox
      Left = 8
      Top = 8
      Width = 345
      Height = 17
      Caption = 'Enable Quick Member Creation'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = enabled_clicked
    end
  end
  object btnSave: TTouchBtn
    Left = 8
    Top = 167
    Width = 361
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentColor = True
    Caption = 'Save'
    ButtonColor = 2200566
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = button_clicked
  end
  object btnCancel: TTouchBtn
    Left = 8
    Top = 230
    Width = 361
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentColor = True
    Caption = 'Cancel'
    ButtonColor = clMaroon
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = button_clicked
  end
end
