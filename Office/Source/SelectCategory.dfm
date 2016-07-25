object frmSelectCategory: TfrmSelectCategory
  Tag = 1024
  Left = 396
  Top = 193
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Select A Category'
  ClientHeight = 361
  ClientWidth = 217
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 201
    Height = 345
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      201
      345)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 191
      Height = 292
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 189
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 16
          Top = 0
          Width = 401
          Height = 49
          AutoSize = False
          Caption = 'Select A Category'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object lbCategory: TListBox
        Left = 8
        Top = 56
        Width = 173
        Height = 225
        Color = 15269887
        ItemHeight = 13
        TabOrder = 1
        OnDblClick = lbCategoryDblClick
      end
    end
    object btnCancel: TBitBtn
      Left = 104
      Top = 304
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 1
      Kind = bkCancel
    end
    object BitBtn1: TBitBtn
      Left = 8
      Top = 304
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      Kind = bkOK
    end
  end
end
