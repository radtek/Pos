object frmSelectMenu: TfrmSelectMenu
  Left = 156
  Top = 157
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Select Menu to Edit'
  ClientHeight = 273
  ClientWidth = 361
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 345
    Height = 257
    BorderWidth = 4
    TabOrder = 0
    DesignSize = (
      345
      257)
    object Panel8: TPanel
      Left = 5
      Top = 5
      Width = 335
      Height = 204
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 333
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeTitle: TLabel
          Left = 0
          Top = 0
          Width = 333
          Height = 48
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = 'Select the menu you wish to edit'
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
      object DBGrid1: TDBGrid
        Left = 8
        Top = 56
        Width = 317
        Height = 137
        Color = clInfoBk
        DataSource = dsMenu
        Options = [dgRowSelect, dgAlwaysShowSelection]
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        OnDblClick = DBGrid1DblClick
        Columns = <
          item
            Expanded = False
            FieldName = 'MENU_NAME'
            Visible = True
          end>
      end
    end
    object BitBtn1: TBitBtn
      Left = 168
      Top = 216
      Width = 81
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 1
      Kind = bkOK
    end
    object BitBtn2: TBitBtn
      Left = 256
      Top = 216
      Width = 81
      Height = 33
      Anchors = [akRight, akBottom]
      TabOrder = 2
      Kind = bkCancel
    end
  end
  object dsMenu: TDataSource
    AutoEdit = False
    DataSet = qrMenu
    Left = 88
    Top = 80
  end
  object qrMenu: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Menu_Key,'
      '   Menu_Name'
      'From'
      '   Menu'
      'Order By'
      '   Menu_Name')
    Left = 40
    Top = 80
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 64
    Top = 136
  end
end
