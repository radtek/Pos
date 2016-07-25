object frmUsers: TfrmUsers
  Tag = 1024
  Left = 97
  Top = 174
  Width = 556
  Height = 406
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Users'
  Color = clBtnFace
  Constraints.MinHeight = 386
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 548
    Height = 379
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel2: TPanel
      Left = 4
      Top = 4
      Width = 540
      Height = 371
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Panel8: TPanel
        Left = 180
        Top = 5
        Width = 355
        Height = 316
        Align = alRight
        BevelOuter = bvLowered
        Constraints.MaxWidth = 355
        Constraints.MinHeight = 300
        Constraints.MinWidth = 355
        TabOrder = 0
        DesignSize = (
          355
          316)
        object Label7: TLabel
          Left = 16
          Top = 56
          Width = 65
          Height = 13
          Caption = 'Login Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object DBText6: TDBText
          Left = 129
          Top = 56
          Width = 216
          Height = 17
          DataField = 'USER_ID'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText2: TDBText
          Left = 129
          Top = 72
          Width = 216
          Height = 17
          DataField = 'INITIALS'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 16
          Top = 72
          Width = 39
          Height = 13
          Caption = 'Initials'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label4: TLabel
          Left = 16
          Top = 88
          Width = 46
          Height = 13
          Caption = 'Address'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object DBText3: TDBText
          Left = 129
          Top = 184
          Width = 216
          Height = 17
          DataField = 'PHONE'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 8
          Top = 184
          Width = 35
          Height = 13
          Caption = 'Phone'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label5: TLabel
          Left = 8
          Top = 200
          Width = 37
          Height = 13
          Caption = 'Mobile'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 8
          Top = 216
          Width = 83
          Height = 13
          Caption = 'E-Mail Address'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label8: TLabel
          Left = 8
          Top = 232
          Width = 108
          Height = 13
          Caption = 'Next Order Number'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 129
          Top = 232
          Width = 216
          Height = 16
          AutoSize = False
          Caption = 'Label2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText5: TDBText
          Left = 129
          Top = 216
          Width = 216
          Height = 17
          DataField = 'EMAIL'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object DBText4: TDBText
          Left = 129
          Top = 200
          Width = 216
          Height = 17
          DataField = 'MOBILE'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 353
          Height = 48
          Align = alTop
          BevelOuter = bvNone
          Color = clAppWorkSpace
          TabOrder = 0
          object lbeTitle: TLabel
            Left = 16
            Top = 0
            Width = 297
            Height = 49
            AutoSize = False
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
        object DBMemo1: TDBMemo
          Left = 129
          Top = 88
          Width = 216
          Height = 89
          Color = clInfoBk
          DataField = 'ADDRESS'
          DataSource = dsUsers
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object btnAdd: TBitBtn
          Left = 64
          Top = 273
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = 'Add'
          TabOrder = 2
          OnClick = btnAddClick
        end
        object btnEdit: TBitBtn
          Left = 160
          Top = 273
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = 'Edit'
          TabOrder = 3
          OnClick = btnEditClick
        end
        object btnDelete: TBitBtn
          Left = 256
          Top = 273
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = 'Delete'
          TabOrder = 4
          OnClick = btnDeleteClick
        end
      end
      object Panel10: TPanel
        Left = 5
        Top = 321
        Width = 530
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 1
        DesignSize = (
          530
          45)
        object btnClose: TBitBtn
          Left = 428
          Top = 8
          Width = 97
          Height = 33
          Anchors = [akTop, akRight]
          Cancel = True
          Caption = '&Close'
          TabOrder = 0
          OnClick = btnCloseClick
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00388888888877
            F7F787F8888888888333333F00004444400888FFF444448888888888F333FF8F
            000033334D5007FFF4333388888888883338888F0000333345D50FFFF4333333
            338F888F3338F33F000033334D5D0FFFF43333333388788F3338F33F00003333
            45D50FEFE4333333338F878F3338F33F000033334D5D0FFFF43333333388788F
            3338F33F0000333345D50FEFE4333333338F878F3338F33F000033334D5D0FFF
            F43333333388788F3338F33F0000333345D50FEFE4333333338F878F3338F33F
            000033334D5D0EFEF43333333388788F3338F33F0000333345D50FEFE4333333
            338F878F3338F33F000033334D5D0EFEF43333333388788F3338F33F00003333
            4444444444333333338F8F8FFFF8F33F00003333333333333333333333888888
            8888333F00003333330000003333333333333FFFFFF3333F00003333330AAAA0
            333333333333888888F3333F00003333330000003333333333338FFFF8F3333F
            0000}
          NumGlyphs = 2
        end
      end
      object DBGrid1: TDBGrid
        Left = 5
        Top = 5
        Width = 171
        Height = 316
        Align = alClient
        Color = clInfoBk
        DataSource = dsUsers
        Options = [dgTitles, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
        TabOrder = 2
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        OnDblClick = btnEditClick
        Columns = <
          item
            Expanded = False
            FieldName = 'USER_NAME'
            Title.Caption = 'User Name'
            Width = 166
            Visible = True
          end>
      end
      object Panel1: TPanel
        Left = 176
        Top = 5
        Width = 4
        Height = 316
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 3
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 32
    Top = 56
  end
  object dsUsers: TDataSource
    AutoEdit = False
    DataSet = dtUsers
    Left = 136
    Top = 56
  end
  object dtUsers: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    AfterScroll = dtUsersAfterScroll
    DeleteSQL.Strings = (
      'delete from USERS'
      'where'
      '  USER_ID = :OLD_USER_ID')
    InsertSQL.Strings = (
      'insert into USERS'
      
        '  (USER_NAME, INITIALS, USER_ID, ADDRESS, PHONE, MOBILE, EMAIL, ' +
        'NEXT_ORDER_NUMBER)'
      'values'
      
        '  (:USER_NAME, :INITIALS, :USER_ID, :ADDRESS, :PHONE, :MOBILE, :' +
        'EMAIL, '
      '   :NEXT_ORDER_NUMBER)')
    RefreshSQL.Strings = (
      'Select '
      '  USER_ID,'
      '  USER_NAME,'
      '  FIRST_NAME,'
      '  MIDDLE_NAME,'
      '  LAST_NAME,'
      '  INITIALS,'
      '  ADDRESS,'
      '  PHONE,'
      '  MOBILE,'
      '  EMAIL,'
      '  PIN,'
      '  ACCESS,'
      '  SWIPE_CARD,'
      '  NEXT_ORDER_NUMBER,'
      '  ORDER_PREFIX,'
      '  ORDER_SUFFIX'
      'from USERS '
      'where'
      '  USER_ID = :USER_ID')
    SelectSQL.Strings = (
      
        'select USER_NAME, INITIALS, USER_ID, ADDRESS, PHONE, MOBILE, EMA' +
        'IL, NEXT_ORDER_NUMBER, ORDER_PREFIX, ORDER_SUFFIX from USERS'
      'order by USER_NAME')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  USER_NAME = :USER_NAME,'
      '  INITIALS = :INITIALS,'
      '  USER_ID = :USER_ID,'
      '  ADDRESS = :ADDRESS,'
      '  PHONE = :PHONE,'
      '  MOBILE = :MOBILE,'
      '  EMAIL = :EMAIL,'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 88
    Top = 56
  end
end
