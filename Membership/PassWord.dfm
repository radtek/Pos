object frmPassword: TfrmPassword
  Left = 237
  Top = 222
  ActiveControl = edtPAssword
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Enter a Password'
  ClientHeight = 113
  ClientWidth = 283
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
  object Label1: TLabel
    Left = 40
    Top = 28
    Width = 46
    Height = 13
    Caption = 'Password'
  end
  object btnOK: TButton
    Left = 45
    Top = 74
    Width = 87
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 169
    Top = 74
    Width = 87
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object edtPAssword: TEdit
    Left = 112
    Top = 24
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
    OnKeyPress = edtPAsswordKeyPress
  end
end
