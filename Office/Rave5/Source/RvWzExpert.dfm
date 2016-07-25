object FormReportExpert: TFormReportExpert
  Left = 250
  Top = 144
  BorderStyle = bsToolWindow
  Caption = 'Rave Report Expert'
  ClientHeight = 384
  ClientWidth = 563
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    563
    384)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 30
    Height = 13
    Caption = 'Ignore'
  end
  object Label2: TLabel
    Left = 56
    Top = 8
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object Button1: TButton
    Left = 392
    Top = 352
    Width = 75
    Height = 25
    Caption = 'Continue'
    ModalResult = 1
    TabOrder = 0
  end
  object ItemList: TCheckListBox
    Left = 0
    Top = 24
    Width = 561
    Height = 313
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    TabOrder = 1
  end
  object Button2: TButton
    Left = 480
    Top = 351
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Abort'
    ModalResult = 2
    TabOrder = 2
  end
end
