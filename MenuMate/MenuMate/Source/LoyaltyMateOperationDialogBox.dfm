object frmLoyaltyMateOperationDialogBox: TfrmLoyaltyMateOperationDialogBox
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'LoyaltyMate Operation'
  ClientHeight = 162
  ClientWidth = 521
  Color = 14342874
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object grpMessageContent: TGroupBox
    Left = 8
    Top = 8
    Width = 505
    Height = 83
    TabOrder = 0
    object lblDescription: TLabel
      Left = 15
      Top = 17
      Width = 339
      Height = 16
      Caption = 'Performing operation on LoyaltyMate... Please Wait.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
  end
  object btnCancelOperation: TTouchBtn
    Left = 413
    Top = 97
    Width = 100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ParentColor = True
    Caption = 'Cancel'
    ButtonColor = 2200566
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = btnCancelOperationMouseClick
  end
end
