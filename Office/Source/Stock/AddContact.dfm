object frmAddContact: TfrmAddContact
  Tag = 1024
  Left = 362
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Add Contact'
  ClientHeight = 509
  ClientWidth = 448
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
  object btnOk: TBitBtn
    Left = 240
    Top = 472
    Width = 97
    Height = 33
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = btnOkClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object btnCancel: TBitBtn
    Left = 344
    Top = 472
    Width = 97
    Height = 33
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = btnCancelClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      333333333333333333333333000033338833333333333333333F333333333333
      0000333911833333983333333388F333333F3333000033391118333911833333
      38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
      911118111118333338F3338F833338F3000033333911111111833333338F3338
      3333F8330000333333911111183333333338F333333F83330000333333311111
      8333333333338F3333383333000033333339111183333333333338F333833333
      00003333339111118333333333333833338F3333000033333911181118333333
      33338333338F333300003333911183911183333333383338F338F33300003333
      9118333911183333338F33838F338F33000033333913333391113333338FF833
      38F338F300003333333333333919333333388333338FFF830000333333333333
      3333333333333333333888330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 433
    Height = 457
    Caption = 'Company Details'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    object Label3: TLabel
      Left = 16
      Top = 24
      Width = 79
      Height = 13
      Caption = 'Company Name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 16
      Top = 52
      Width = 72
      Height = 13
      Caption = 'Contact Name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 16
      Top = 108
      Width = 34
      Height = 13
      Caption = 'Phone:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 16
      Top = 192
      Width = 32
      Height = 13
      Caption = 'E-Mail:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 16
      Top = 136
      Width = 22
      Height = 13
      Caption = 'Fax:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 16
      Top = 164
      Width = 34
      Height = 13
      Caption = 'Mobile:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 16
      Top = 252
      Width = 86
      Height = 13
      Caption = 'Location Address:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 16
      Top = 320
      Width = 75
      Height = 13
      Caption = 'Postal Address:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 16
      Top = 388
      Width = 27
      Height = 13
      Caption = 'Note:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 16
      Top = 80
      Width = 89
      Height = 13
      Caption = 'Supplier Identifier:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lblWebOrderTarget: TLabel
      Left = 16
      Top = 224
      Width = 99
      Height = 13
      Caption = 'Send web-orders to:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object dbeCompanyName: TDBEdit
      Left = 136
      Top = 20
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'COMPANY_NAME'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
      OnKeyPress = dbeKeyPress
    end
    object DBMemo1: TDBMemo
      Left = 136
      Top = 248
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'LOCATION_ADDRESS'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 7
    end
    object DBMemo2: TDBMemo
      Left = 136
      Top = 316
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'BILL_ADDRESS'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 9
    end
    object dbeContact: TDBEdit
      Left = 136
      Top = 48
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'CONTACT_NAME'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      OnKeyPress = dbeKeyPress
    end
    object DBEdit3: TDBEdit
      Left = 136
      Top = 104
      Width = 137
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'PHONE'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 3
      OnKeyPress = dbeKeyPress
    end
    object DBEdit4: TDBEdit
      Left = 136
      Top = 188
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'EMAIL'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 6
      OnKeyPress = dbeKeyPress
    end
    object DBEdit5: TDBEdit
      Left = 136
      Top = 132
      Width = 137
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'FAX'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 4
      OnKeyPress = dbeKeyPress
    end
    object DBEdit6: TDBEdit
      Left = 136
      Top = 160
      Width = 137
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MOBILE'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 5
      OnKeyPress = dbeKeyPress
    end
    object DBMemo3: TDBMemo
      Left = 136
      Top = 384
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'NOTE'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 10
    end
    object BitBtn1: TBitBtn
      Left = 16
      Top = 344
      Width = 105
      Height = 33
      Caption = 'Copy From Above'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      OnClick = BitBtn1Click
    end
    object DBEdit1: TDBEdit
      Left = 136
      Top = 76
      Width = 137
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'SUPPLIER_ID'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 2
      OnKeyPress = dbeKeyPress
    end
    object BitBtn2: TBitBtn
      Left = 280
      Top = 74
      Width = 137
      Height = 25
      Caption = 'Update Existing Invoices'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
      OnClick = BitBtn2Click
    end
    object dbeWeborderTarget: TDBEdit
      Left = 136
      Top = 220
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'WEBORDER_TARGET_URI'
      DataSource = dsContact
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 12
      OnKeyPress = dbeKeyPress
    end
  end
  object dsContact: TDataSource
    AutoEdit = False
    DataSet = dtContact
    Left = 104
    Top = 472
  end
  object dtContact: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from CONTACT'
      'where'
      '  CONTACT_LK = :OLD_CONTACT_LK')
    InsertSQL.Strings = (
      'insert into CONTACT'
      
        '  (BILL_ADDRESS, COMPANY_NAME, CONTACT_GROUP_KEY, CONTACT_LK, CO' +
        'NTACT_NAME, '
      
        '   CONTACT_PHONE, EMAIL, FAX, LOCATION_ADDRESS, MOBILE, NOTE, PH' +
        'ONE, SUPPLIER_ID, WEBORDER_TARGET_URI)'
      'values'
      
        '  (:BILL_ADDRESS, :COMPANY_NAME, :CONTACT_GROUP_KEY, :CONTACT_LK' +
        ', :CONTACT_NAME, '
      
        '   :CONTACT_PHONE, :EMAIL, :FAX, :LOCATION_ADDRESS, :MOBILE, :NO' +
        'TE, :PHONE, '
      '   :SUPPLIER_ID, :WEBORDER_TARGET_URI)')
    RefreshSQL.Strings = (
      'Select '
      '  CONTACT_LK,'
      '  CONTACT_GROUP_KEY,'
      '  CONTACT_TYPE,'
      '  COMPANY_NAME,'
      '  PHONE,'
      '  FAX,'
      '  MOBILE,'
      '  EMAIL,'
      '  LOCATION_ADDRESS,'
      '  CONTACT_NAME,'
      '  CONTACT_PHONE,'
      '  BILL_NAME,'
      '  BILL_ATTN,'
      '  BILL_PHONE,'
      '  BILL_ADDRESS,'
      '  BAD_DEBTOR,'
      '  NOTE,'
      '  DELETED,'
      '  SUPPLIER_ID,'
      '  MYOB1,'
      '  MYOB2,'
      '  MYOB3,'
      '  WEBORDER_TARGET_URI'
      'from CONTACT '
      'where'
      '  CONTACT_LK = :CONTACT_LK')
    SelectSQL.Strings = (
      'SELECT'
      '   CONTACT_LK,'
      '   CONTACT_GROUP_KEY,'
      '   COMPANY_NAME,'
      '   CONTACT_NAME,'
      '   CONTACT_PHONE,'
      '   PHONE, FAX,'
      '   MOBILE, EMAIL,'
      '   LOCATION_ADDRESS,'
      '   BILL_ADDRESS,'
      '   SUPPLIER_ID,'
      '   NOTE,'
      '   WEBORDER_TARGET_URI'
      'FROM'
      '   CONTACT'
      'WHERE'
      '   CONTACT_LK = :CONTACT_LK')
    ModifySQL.Strings = (
      'update CONTACT'
      'set'
      '  BILL_ADDRESS = :BILL_ADDRESS,'
      '  COMPANY_NAME = :COMPANY_NAME,'
      '  CONTACT_GROUP_KEY = :CONTACT_GROUP_KEY,'
      '  CONTACT_LK = :CONTACT_LK,'
      '  CONTACT_NAME = :CONTACT_NAME,'
      '  CONTACT_PHONE = :CONTACT_PHONE,'
      '  EMAIL = :EMAIL,'
      '  FAX = :FAX,'
      '  LOCATION_ADDRESS = :LOCATION_ADDRESS,'
      '  MOBILE = :MOBILE,'
      '  NOTE = :NOTE,'
      '  PHONE = :PHONE,'
      '  SUPPLIER_ID = :SUPPLIER_ID,'
      '  WEBORDER_TARGET_URI = :WEBORDER_TARGET_URI'
      'where'
      '  CONTACT_LK = :OLD_CONTACT_LK')
    GeneratorField.Field = 'CONTACT_LK'
    GeneratorField.Generator = 'GEN_CONTACT'
    Left = 72
    Top = 472
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 136
    Top = 472
  end
  object qrContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_Group_Key,'
      '   Contact_Group'
      'From'
      '      ContactGroup'
      'Order By'
      '      Contact_Group')
    Left = 40
    Top = 472
  end
  object qrAddContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into ContactGroup ('
      '   Contact_Group_Key,'
      '   Contact_Group,'
      '   Sort_Order)'
      'Values ('
      '   :Contact_Group_Key,'
      '   :Contact_Group,'
      '   :Sort_Order)')
    Left = 8
    Top = 472
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_Group_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_Group'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Sort_Order'
        ParamType = ptUnknown
      end>
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    ParamCheck = False
    SQL.Strings = (
      'select gen_id(gen_contact_group_key, 1) from rdb$database')
    Transaction = Transaction
    Left = 168
    Top = 472
  end
  object qrUpdateInvoiceSupplierID: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Update'
      '   TransactionBatch'
      'Set'
      '   Supplier_ID = :SupplierID'
      'Where'
      '   Supplier_Key = :SupplierKey')
    Transaction = Transaction
    Left = 200
    Top = 472
  end
end
