object frmAddDebtorCompany: TfrmAddDebtorCompany
  Left = 207
  Top = 148
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Add Company'
  ClientHeight = 441
  ClientWidth = 577
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
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 433
    Height = 385
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
      Top = 60
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
      Top = 144
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
      Top = 88
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
      Top = 116
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
      Top = 172
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
      Top = 240
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
    object Label16: TLabel
      Left = 16
      Top = 32
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
    object Label1: TLabel
      Left = 16
      Top = 312
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
    object dbmLocationAddress: TDBMemo
      Left = 136
      Top = 168
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'LOCATION_ADDRESS'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 5
    end
    object dbmPostalAddress: TDBMemo
      Left = 136
      Top = 236
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MAILING_ADDRESS'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 6
    end
    object dbePhone: TDBEdit
      Left = 136
      Top = 56
      Width = 145
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'PHONE'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
    end
    object dbeEmail: TDBEdit
      Left = 136
      Top = 140
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'EMAIL'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 4
    end
    object dbeFax: TDBEdit
      Left = 136
      Top = 84
      Width = 145
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'FAX'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 2
    end
    object dbeMobile: TDBEdit
      Left = 136
      Top = 112
      Width = 145
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'MOBILE'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 3
    end
    object dbeCompanyName: TDBEdit
      Left = 136
      Top = 28
      Width = 281
      Height = 21
      Color = clInfoBk
      Ctl3D = True
      DataField = 'COMPANY_NAME'
      DataSource = dsCompany
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
    end
    object DBMemo1: TDBMemo
      Left = 136
      Top = 308
      Width = 281
      Height = 61
      Color = clInfoBk
      Ctl3D = True
      DataField = 'NOTE'
      DataSource = dsCompany
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
  end
  object btnOk: TBitBtn
    Left = 264
    Top = 400
    Width = 81
    Height = 33
    Caption = 'OK'
    Default = True
    ModalResult = 1
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
    Top = 400
    Width = 89
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object dsCompany: TDataSource
    DataSet = dtCompanyEdit
    Left = 528
    Top = 56
  end
  object dtCompanyEdit: TIBDataSet
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from COMPANIES'
      'where'
      '  COMPANY_KEY = :OLD_COMPANY_KEY')
    InsertSQL.Strings = (
      'insert into COMPANIES'
      
        '  (COMPANY_KEY, COMPANY_NAME, PHONE, FAX, MOBILE, EMAIL, LOCATIO' +
        'N_ADDRESS, '
      '   MAILING_ADDRESS, NOTE)'
      'values'
      
        '  (:COMPANY_KEY, :COMPANY_NAME, :PHONE, :FAX, :MOBILE, :EMAIL, :' +
        'LOCATION_ADDRESS, '
      '   :MAILING_ADDRESS, :NOTE)')
    RefreshSQL.Strings = (
      'Select '
      '  COMPANY_KEY,'
      '  COMPANY_TYPE,'
      '  COMPANY_NAME,'
      '  PHONE,'
      '  FAX,'
      '  MOBILE,'
      '  EMAIL,'
      '  LOCATION_ADDRESS,'
      '  MAILING_ADDRESS,'
      '  BILLING_NAME,'
      '  BAD_DEBTOR,'
      '  NOTE'
      'from COMPANIES '
      'where'
      '  COMPANY_KEY = :COMPANY_KEY')
    SelectSQL.Strings = (
      'Select'
      '   COMPANY_KEY,'
      '   COMPANY_NAME,'
      '   PHONE,'
      '   FAX,'
      '   MOBILE,'
      '   EMAIL,'
      '   LOCATION_ADDRESS,'
      '   MAILING_ADDRESS,'
      '   NOTE'
      'From'
      '   COMPANIES'
      'Where'
      '   COMPANY_KEY = :COMPANY_KEY')
    ModifySQL.Strings = (
      'update COMPANIES'
      'set'
      '  COMPANY_KEY = :COMPANY_KEY,'
      '  COMPANY_NAME = :COMPANY_NAME,'
      '  PHONE = :PHONE,'
      '  FAX = :FAX,'
      '  MOBILE = :MOBILE,'
      '  EMAIL = :EMAIL,'
      '  LOCATION_ADDRESS = :LOCATION_ADDRESS,'
      '  MAILING_ADDRESS = :MAILING_ADDRESS,'
      '  NOTE = :NOTE'
      'where'
      '  COMPANY_KEY = :OLD_COMPANY_KEY')
    GeneratorField.Field = 'COMPANY_KEY'
    GeneratorField.Generator = 'GEN_COMPANIES'
    Left = 464
    Top = 56
  end
  object Transaction: TIBTransaction
    Active = False
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 464
    Top = 8
  end
end
