object frmAddSupplierBarcode: TfrmAddSupplierBarcode
  Left = 474
  Top = 196
  Width = 315
  Height = 308
  BorderIcons = []
  Caption = 'frmAddSupplierBarcode'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 5
    Width = 289
    Height = 268
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      289
      268)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 279
      Height = 215
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 277
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 277
          Height = 48
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'Select a Supplier and Unit'
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
      object dbgSupplierUnits: TDBGrid
        Left = 8
        Top = 57
        Width = 262
        Height = 148
        Color = clInfoBk
        DataSource = dsSupplierUnits
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Options = [dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clMaroon
        TitleFont.Height = -13
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = [fsBold]
        OnDblClick = dbgSupplierUnitsDblClick
        Columns = <
          item
            Expanded = False
            FieldName = 'COMPANY_NAME'
            Width = 155
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'SUPPLIER_UNIT'
            Title.Caption = 'Unit'
            Width = 85
            Visible = True
          end>
      end
    end
    object btnCancel: TBitBtn
      Left = 192
      Top = 227
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 1
      Kind = bkCancel
    end
    object BitBtn1: TBitBtn
      Left = 96
      Top = 227
      Width = 89
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      Kind = bkOK
    end
  end
  object dsSupplierUnits: TDataSource
    Left = 168
    Top = 136
  end
end
