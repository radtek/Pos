object frmRecipeBox: TfrmRecipeBox
  Left = 383
  Top = 245
  Width = 386
  Height = 212
  Caption = 'Manufacture'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 353
    Height = 161
    BorderWidth = 1
    BorderStyle = bsSingle
    TabOrder = 0
    object Panel3: TPanel
      Left = 0
      Top = 0
      Width = 353
      Height = 33
      Color = clAppWorkSpace
      TabOrder = 0
      object Label1: TLabel
        Left = 9
        Top = 1
        Width = 104
        Height = 24
        Caption = 'Manufacture'
        Color = clAppWorkSpace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clCaptionText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        Layout = tlCenter
      end
    end
    object Panel4: TPanel
      Left = 8
      Top = 40
      Width = 337
      Height = 113
      BorderStyle = bsSingle
      Color = clInfoBk
      TabOrder = 1
      object BitBtn1: TBitBtn
        Left = 8
        Top = 8
        Width = 153
        Height = 41
        Caption = 'Create Recipe'
        TabOrder = 0
        OnClick = CreateRecipeOnClick
      end
      object BitBtn2: TBitBtn
        Left = 8
        Top = 56
        Width = 153
        Height = 41
        Caption = 'Delete Recipe'
        TabOrder = 1
        OnClick = DeleteRecipeOnClick
      end
      object BitBtn3: TBitBtn
        Left = 168
        Top = 8
        Width = 153
        Height = 41
        Caption = 'Manufacture Stock'
        TabOrder = 2
        OnClick = UseRecipeOnClick
      end
      object BitBtn4: TBitBtn
        Left = 168
        Top = 56
        Width = 153
        Height = 41
        Caption = 'Edit Recipe'
        TabOrder = 3
        OnClick = EditRecipeOnClick
      end
    end
  end
end
