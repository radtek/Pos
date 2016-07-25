object frmAddDebtor: TfrmAddDebtor
  Left = 351
  Top = 186
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Add Debtor'
  ClientHeight = 489
  ClientWidth = 585
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnOk: TBitBtn
    Left = 264
    Top = 448
    Width = 81
    Height = 33
    Caption = 'OK'
    Default = True
    TabOrder = 1
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
    Left = 352
    Top = 448
    Width = 89
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 433
    Height = 433
    Caption = 'Debtor Details'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label10: TLabel
      Left = 16
      Top = 184
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
    object Label11: TLabel
      Left = 16
      Top = 268
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
    object Label12: TLabel
      Left = 16
      Top = 212
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
    object Label13: TLabel
      Left = 16
      Top = 240
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
    object Label14: TLabel
      Left = 16
      Top = 296
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
    object Label15: TLabel
      Left = 16
      Top = 364
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
    object Label4: TLabel
      Left = 248
      Top = 184
      Width = 20
      Height = 13
      Caption = 'Ext:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label16: TLabel
      Left = 248
      Top = 100
      Width = 55
      Height = 13
      Caption = 'First Name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label17: TLabel
      Left = 16
      Top = 128
      Width = 64
      Height = 13
      Caption = 'Middle Name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label18: TLabel
      Left = 248
      Top = 128
      Width = 54
      Height = 13
      Caption = 'Last Name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 16
      Top = 44
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
    object Label2: TLabel
      Left = 16
      Top = 72
      Width = 51
      Height = 13
      Caption = 'Known As:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 156
      Width = 67
      Height = 13
      Caption = 'Date Of Birth:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 248
      Top = 156
      Width = 22
      Height = 13
      Caption = 'Sex:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 16
      Top = 100
      Width = 24
      Height = 13
      Caption = 'Title:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object DBMemo4: TDBMemo
      Left = 136
      Top = 292
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'LOCATION_ADDRESS'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 14
    end
    object DBMemo5: TDBMemo
      Left = 136
      Top = 360
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MAILING_ADDRESS'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 15
    end
    object DBEdit2: TDBEdit
      Left = 136
      Top = 180
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'PHONE'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 9
    end
    object DBEdit7: TDBEdit
      Left = 136
      Top = 264
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'EMAIL'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 13
    end
    object DBEdit8: TDBEdit
      Left = 136
      Top = 208
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'FAX'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 11
    end
    object DBEdit9: TDBEdit
      Left = 136
      Top = 236
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MOBILE'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 12
    end
    object DBEdit1: TDBEdit
      Left = 312
      Top = 180
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'PHONE_EXTENSION'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 10
    end
    object dbeFirstName: TDBEdit
      Left = 312
      Top = 96
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'FIRST_NAME'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 4
      OnExit = dbeFirstNameExit
    end
    object DBEdit11: TDBEdit
      Left = 136
      Top = 124
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MIDDLE_NAME'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 5
    end
    object DBEdit12: TDBEdit
      Left = 312
      Top = 124
      Width = 105
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'LAST_NAME'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 6
    end
    object btnAddCompany: TButton
      Left = 320
      Top = 38
      Width = 97
      Height = 25
      Caption = 'Add Company'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnAddCompanyClick
    end
    object cbCompany: TComboBox
      Left = 136
      Top = 40
      Width = 177
      Height = 21
      Style = csDropDownList
      Color = clInfoBk
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ItemIndex = 0
      ParentFont = False
      TabOrder = 0
      Text = '- None -'
      Items.Strings = (
        '- None -')
    end
    object dbeName: TDBEdit
      Left = 136
      Top = 68
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'NAME'
      DataSource = dsDetors
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 2
    end
    object dtpDateOfBirth: TDateTimePicker
      Left = 136
      Top = 152
      Width = 105
      Height = 21
      CalAlignment = dtaLeft
      CalColors.BackColor = clInfoBk
      Date = 37909
      Time = 37909
      ShowCheckbox = True
      Color = clInfoBk
      DateFormat = dfShort
      DateMode = dmComboBox
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Kind = dtkDate
      ParseInput = False
      ParentFont = False
      TabOrder = 7
    end
    object cbSex: TComboBox
      Left = 312
      Top = 152
      Width = 105
      Height = 21
      Style = csDropDownList
      Color = clInfoBk
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      TabOrder = 8
      Items.Strings = (
        ''
        'Male'
        'Female')
    end
    object cbTitle: TComboBox
      Left = 136
      Top = 96
      Width = 105
      Height = 21
      Style = csDropDownList
      Color = clInfoBk
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      TabOrder = 3
      Items.Strings = (
        ''
        'Mr.'
        'Mrs.'
        'Miss.'
        'Ms.')
    end
    object chbAllowInvoice: TCheckBox
      Left = 14
      Top = 20
      Width = 135
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Allow Invoice'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 16
    end
  end
  object btnCopyCompany: TBitBtn
    Left = 8
    Top = 448
    Width = 145
    Height = 33
    Caption = 'Copy Company Fields'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = btnCopyCompanyClick
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 464
    Top = 8
  end
  object dtDebtorEdit: TIBDataSet
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from CONTACTS'
      'where'
      '  CONTACTS_KEY = :OLD_CONTACTS_KEY')
    InsertSQL.Strings = (
      'insert into CONTACTS'
      
        '  (COMPANY_KEY, CONTACT_TYPE, CONTACTS_3RDPARTY_KEY, CONTACTS_KE' +
        'Y, DATEOFBIRTH, '
      
        '   EMAIL, FAX, FIRST_NAME, LAST_NAME, LOCATION_ADDRESS, MAILING_' +
        'ADDRESS, '
      
        '   MEMBER_NUMBER, MIDDLE_NAME, MOBILE, NAME, PHONE, PHONE_EXTENS' +
        'ION, PIN, '
      '   SEX, SITE_ID, TAB_ENALBED, TITLE, TOTAL_SPENT)'
      'values'
      
        '  (:COMPANY_KEY, :CONTACT_TYPE, :CONTACTS_3RDPARTY_KEY, :CONTACT' +
        'S_KEY, '
      
        '   :DATEOFBIRTH, :EMAIL, :FAX, :FIRST_NAME, :LAST_NAME, :LOCATIO' +
        'N_ADDRESS, '
      
        '   :MAILING_ADDRESS, :MEMBER_NUMBER, :MIDDLE_NAME, :MOBILE, :NAM' +
        'E, :PHONE, '
      
        '   :PHONE_EXTENSION, :PIN, :SEX, :SITE_ID, :TAB_ENALBED, :TITLE,' +
        ' :TOTAL_SPENT)')
    RefreshSQL.Strings = (
      'Select '
      '  CONTACTS_KEY,'
      '  COMPANY_KEY,'
      '  CONTACT_ID,'
      '  CONTACT_TYPE,'
      '  NAME,'
      '  TITLE,'
      '  SEX,'
      '  FIRST_NAME,'
      '  MIDDLE_NAME,'
      '  LAST_NAME,'
      '  INITIALS,'
      '  LOCATION_ADDRESS,'
      '  MAILING_ADDRESS,'
      '  PHONE,'
      '  PHONE_EXTENSION,'
      '  MOBILE,'
      '  FAX,'
      '  EMAIL,'
      '  TOTAL_SPENT,'
      '  PIN,'
      '  PROX_CARD,'
      '  ACCESS_LEVEL,'
      '  PALM_AUTO_OPTIONS,'
      '  PALM_LARGE_FONT,'
      '  PALM_ADVANCED_USER,'
      '  PALM_TIMEOUT,'
      '  TAB_ENALBED,'
      '  PAYROLL_ID,'
      '  START_TIME1,'
      '  START_TIME2,'
      '  START_TIME3,'
      '  START_TIME4,'
      '  START_TIME5,'
      '  START_TIME6,'
      '  DATEOFBIRTH,'
      '  NEXT_ORDER_NUMBER,'
      '  ORDER_PREFIX,'
      '  ORDER_SUFFIX,'
      '  CONTACTS_3RDPARTY_KEY,'
      '  PALM_SCREEN_LOCK,'
      '  PALM_CLASSIC_MODE,'
      '  PALM_SOUND_LEVEL,'
      '  PALM_GET_ALL_TABLES,'
      '  MEMBER_NUMBER,'
      '  SITE_ID,'
      '  CARD_CREATION_DATE,'
      '  KNOWN_AS,'
      '  LAST_MODIFIED,'
      '  NOTE'
      'from CONTACTS '
      'where'
      '  CONTACTS_KEY = :CONTACTS_KEY')
    SelectSQL.Strings = (
      'Select'
      '   CONTACTS_KEY,'
      '   Contact_Type,'
      '   COMPANY_KEY,'
      '   total_spent,'
      '   NAME,'
      '   TITLE,'
      '   FIRST_NAME,'
      '   MIDDLE_NAME,'
      '   LAST_NAME,'
      '   DATEOFBIRTH,'
      '   SEX,'
      '   PHONE,'
      '   PHONE_EXTENSION,'
      '   FAX,'
      '   MOBILE,'
      '   EMAIL,'
      '   LOCATION_ADDRESS,'
      '   MAILING_ADDRESS,'
      '   PIN,'
      '   TAB_ENALBED,'
      '   CONTACTS_3RDPARTY_KEY,'
      '   MEMBER_NUMBER,'
      '   SITE_ID'
      'From'
      '   CONTACTS'
      'Where'
      '   CONTACTS_KEY = :CONTACTS_KEY')
    ModifySQL.Strings = (
      'update CONTACTS'
      'set'
      '  COMPANY_KEY = :COMPANY_KEY,'
      '  CONTACT_TYPE = :CONTACT_TYPE,'
      '  CONTACTS_3RDPARTY_KEY = :CONTACTS_3RDPARTY_KEY,'
      '  CONTACTS_KEY = :CONTACTS_KEY,'
      '  DATEOFBIRTH = :DATEOFBIRTH,'
      '  EMAIL = :EMAIL,'
      '  FAX = :FAX,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  LAST_NAME = :LAST_NAME,'
      '  LOCATION_ADDRESS = :LOCATION_ADDRESS,'
      '  MAILING_ADDRESS = :MAILING_ADDRESS,'
      '  MEMBER_NUMBER = :MEMBER_NUMBER,'
      '  MIDDLE_NAME = :MIDDLE_NAME,'
      '  MOBILE = :MOBILE,'
      '  NAME = :NAME,'
      '  PHONE = :PHONE,'
      '  PHONE_EXTENSION = :PHONE_EXTENSION,'
      '  PIN = :PIN,'
      '  SEX = :SEX,'
      '  SITE_ID = :SITE_ID,'
      '  TAB_ENALBED = :TAB_ENALBED,'
      '  TITLE = :TITLE'
      'where'
      '  CONTACTS_KEY = :OLD_CONTACTS_KEY')
    GeneratorField.Field = 'CONTACTS_KEY'
    GeneratorField.Generator = 'GEN_CONTACTS'
    Left = 464
    Top = 104
  end
  object dsDetors: TDataSource
    DataSet = dtDebtorEdit
    Left = 528
    Top = 104
  end
  object qrCompanies: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   COMPANY_KEY,'
      '   COMPANY_NAME,'
      '   PHONE,'
      '   FAX,'
      '   MOBILE,'
      '   EMAIL,'
      '   LOCATION_ADDRESS,'
      '   MAILING_ADDRESS'
      'From'
      '   COMPANIES'
      'Order By'
      '   COMPANY_NAME')
    Left = 464
    Top = 56
  end
  object dsCompanies: TDataSource
    DataSet = qrCompanies
    Left = 528
    Top = 56
  end
end
