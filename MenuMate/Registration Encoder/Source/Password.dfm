object frmPassword: TfrmPassword
  Left = 259
  Top = 231
  BorderStyle = bsDialog
  Caption = 'Password'
  ClientHeight = 98
  ClientWidth = 282
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
    Left = 8
    Top = 16
    Width = 128
    Height = 13
    Caption = 'Please enter the password.'
  end
  object edPassword: TEdit
    Left = 8
    Top = 32
    Width = 265
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
  end
  object btnOk: TButton
    Left = 64
    Top = 64
    Width = 73
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 1
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 144
    Top = 64
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = btnCancelClick
  end
end
