object frmSelectCompany: TfrmSelectCompany
  Tag = 1024
  Left = 192
  Top = 240
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Select the company'
  ClientHeight = 190
  ClientWidth = 479
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 465
    Height = 169
    BorderWidth = 3
    TabOrder = 0
    object Panel3: TPanel
      Left = 4
      Top = 4
      Width = 457
      Height = 116
      Align = alClient
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 455
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object Label6: TLabel
          Left = 8
          Top = 0
          Width = 441
          Height = 40
          AutoSize = False
          Caption = 'Select the company you wish to attach to.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object Panel6: TPanel
        Left = 8
        Top = 48
        Width = 441
        Height = 57
        BevelOuter = bvNone
        BorderStyle = bsSingle
        Color = clInfoBk
        TabOrder = 1
        object Label1: TLabel
          Left = 16
          Top = 20
          Width = 75
          Height = 13
          Caption = 'Company Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
        end
        object cbCompanyNames: TComboBox
          Left = 136
          Top = 16
          Width = 281
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 120
      Width = 457
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object BitBtn5: TBitBtn
        Left = 520
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 0
        Kind = bkClose
      end
      object BitBtn2: TBitBtn
        Left = 280
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 1
        Kind = bkOK
      end
      object btnCancel: TBitBtn
        Left = 368
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 2
        Kind = bkCancel
      end
    end
  end
end
