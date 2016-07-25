object frmConfigureRoles: TfrmConfigureRoles
  Left = 579
  Top = 103
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Configure Roles'
  ClientHeight = 258
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PrintScale = poNone
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 224
    Top = 24
    Width = 55
    Height = 13
    Caption = 'Permissions'
  end
  object Roles: TListBox
    Left = 8
    Top = 40
    Width = 201
    Height = 209
    ItemHeight = 13
    TabOrder = 0
    OnClick = RolesClick
  end
  object btnNewRole: TButton
    Left = 136
    Top = 6
    Width = 73
    Height = 25
    Caption = 'Create Role'
    TabOrder = 1
    OnClick = btnNewRoleClick
  end
  object edRoleName: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 2
  end
  object Permissions: TCheckListBox
    Left = 224
    Top = 40
    Width = 169
    Height = 177
    ItemHeight = 13
    TabOrder = 3
  end
  object btnDeleteRole: TBitBtn
    Left = 224
    Top = 224
    Width = 73
    Height = 25
    Caption = 'Delete Role'
    TabOrder = 4
    OnClick = btnDeleteRoleClick
  end
  object btnSaveRole: TButton
    Left = 320
    Top = 224
    Width = 73
    Height = 25
    Caption = 'Save Role'
    TabOrder = 5
    OnClick = btnSaveRoleClick
  end
end
