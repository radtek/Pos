object frmAddUser: TfrmAddUser
  Tag = 1024
  Left = 413
  Top = 242
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Add User'
  ClientHeight = 408
  ClientWidth = 465
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnOk: TBitBtn
    Left = 272
    Top = 368
    Width = 89
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
    Left = 368
    Top = 368
    Width = 89
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
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 449
    Height = 353
    ActivePage = tsUserDetails
    TabIndex = 0
    TabOrder = 2
    object tsUserDetails: TTabSheet
      Caption = 'User Details'
      object Label2: TLabel
        Left = 8
        Top = 16
        Width = 55
        Height = 13
        Caption = 'Login Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 120
        Top = 40
        Width = 60
        Height = 13
        Caption = 'First Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label14: TLabel
        Left = 216
        Top = 40
        Width = 72
        Height = 13
        Caption = 'Middle Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label12: TLabel
        Left = 312
        Top = 40
        Width = 59
        Height = 13
        Caption = 'Last Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label16: TLabel
        Left = 8
        Top = 60
        Width = 27
        Height = 13
        Caption = 'Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 8
        Top = 88
        Width = 31
        Height = 13
        Caption = 'Initials'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 8
        Top = 116
        Width = 39
        Height = 13
        Caption = 'Address'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 8
        Top = 196
        Width = 30
        Height = 13
        Caption = 'Phone'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 8
        Top = 224
        Width = 30
        Height = 13
        Caption = 'Mobile'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 8
        Top = 252
        Width = 70
        Height = 13
        Caption = 'E-Mail Address'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 8
        Top = 300
        Width = 94
        Height = 13
        Caption = 'Next Order Number'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 120
        Top = 280
        Width = 33
        Height = 13
        Caption = 'Prefix'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label10: TLabel
        Left = 216
        Top = 280
        Width = 73
        Height = 13
        Caption = 'Next Number'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label11: TLabel
        Left = 312
        Top = 280
        Width = 32
        Height = 13
        Caption = 'Suffix'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object dbtLoginID: TDBText
        Left = 120
        Top = 16
        Width = 281
        Height = 17
        DataField = 'USER_ID'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object dbeLoginID: TDBEdit
        Left = 120
        Top = 12
        Width = 281
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'USER_ID'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        OnKeyPress = dbeKeyPress
      end
      object dbeLastName: TDBEdit
        Left = 312
        Top = 56
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'LAST_NAME'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 3
        OnKeyPress = dbeKeyPress
      end
      object dbeMiddleName: TDBEdit
        Left = 216
        Top = 56
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'MIDDLE_NAME'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 2
        OnKeyPress = dbeKeyPress
      end
      object dbeFirstName: TDBEdit
        Left = 120
        Top = 56
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'FIRST_NAME'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        OnKeyPress = dbeKeyPress
      end
      object dbeInitials: TDBEdit
        Left = 120
        Top = 84
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'INITIALS'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 4
        OnKeyPress = dbeKeyPress
      end
      object DBMemo1: TDBMemo
        Left = 120
        Top = 114
        Width = 281
        Height = 71
        Color = clInfoBk
        Ctl3D = True
        DataField = 'ADDRESS'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 5
      end
      object DBEdit2: TDBEdit
        Left = 120
        Top = 192
        Width = 185
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'PHONE'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 6
        OnKeyPress = dbeKeyPress
      end
      object DBEdit3: TDBEdit
        Left = 120
        Top = 220
        Width = 185
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'MOBILE'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 7
        OnKeyPress = dbeKeyPress
      end
      object DBEdit4: TDBEdit
        Left = 120
        Top = 248
        Width = 281
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'EMAIL'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 8
        OnKeyPress = dbeKeyPress
      end
      object DBEdit6: TDBEdit
        Left = 120
        Top = 296
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'ORDER_PREFIX'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 9
        OnKeyPress = dbeKeyPress
      end
      object dbeNextOrderNumber: TDBEdit
        Left = 216
        Top = 296
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'NEXT_ORDER_NUMBER'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 10
        OnKeyPress = dbeKeyPress
      end
      object DBEdit8: TDBEdit
        Left = 312
        Top = 296
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'ORDER_SUFFIX'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 11
        OnKeyPress = dbeKeyPress
      end
    end
    object tsOldSecurity: TTabSheet
      Caption = 'Security'
      ImageIndex = 1
      object Label7: TLabel
        Left = 8
        Top = 16
        Width = 17
        Height = 13
        Caption = 'PIN'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label15: TLabel
        Left = 8
        Top = 40
        Width = 57
        Height = 13
        Caption = 'Confirm PIN'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object dbePIN: TDBEdit
        Left = 120
        Top = 12
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'PIN'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        PasswordChar = '*'
        TabOrder = 0
        OnChange = dbePINChange
        OnKeyPress = dbePINKeyPress
      end
      object edPINConfirm: TEdit
        Left = 120
        Top = 40
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        PasswordChar = '*'
        TabOrder = 1
        Text = 'edPINConfirm'
        OnChange = dbePINChange
        OnKeyPress = dbePINKeyPress
      end
    end
    object tsSecurity: TTabSheet
      Caption = 'Security'
      ImageIndex = 2
      object Label17: TLabel
        Left = 8
        Top = 16
        Width = 46
        Height = 13
        Caption = 'Password'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label18: TLabel
        Left = 8
        Top = 40
        Width = 86
        Height = 13
        Caption = 'Confirm Password'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label19: TLabel
        Left = 8
        Top = 72
        Width = 68
        Height = 13
        Caption = 'Security Roles'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object dbePassword: TDBEdit
        Left = 120
        Top = 12
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        DataField = 'PASSWRD'
        DataSource = dsUsers
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        PasswordChar = '*'
        TabOrder = 0
        OnChange = dbePINChange
        OnKeyPress = dbePINKeyPress
      end
      object edPasswordConfirm: TEdit
        Left = 120
        Top = 40
        Width = 89
        Height = 21
        Color = clInfoBk
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        PasswordChar = '*'
        TabOrder = 1
        Text = 'edPasswordConfirm'
        OnChange = dbePINChange
        OnKeyPress = dbePINKeyPress
      end
      object lbSecurityRoles: TCheckListBox
        Left = 120
        Top = 72
        Width = 313
        Height = 177
        ItemHeight = 13
        TabOrder = 2
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 16
    Top = 552
  end
  object dtUsers: TIBDataSet
    Database = dmStockData.dbStock
    Transaction = Transaction
    DeleteSQL.Strings = (
      'delete from USERS'
      'where'
      '  USER_ID = :OLD_USER_ID')
    InsertSQL.Strings = (
      'insert into USERS'
      
        '  (USER_KEY, ADDRESS, EMAIL, FIRST_NAME, INITIALS, LAST_NAME, MI' +
        'DDLE_NAME, MOBILE, '
      
        '   NEXT_ORDER_NUMBER, ORDER_PREFIX, ORDER_SUFFIX, PASSWRD, PHONE' +
        ', PIN, '
      '   USER_ID, USER_NAME)'
      'values'
      
        '  ((SELECT GEN_ID(GEN_USER_KEY, 1) FROM RDB$DATABASE), :ADDRESS,' +
        ' :EMAIL, :FIRST_NAME, :INITIALS, :LAST_NAME, :MIDDLE_NAME, '
      
        '   :MOBILE, :NEXT_ORDER_NUMBER, :ORDER_PREFIX, :ORDER_SUFFIX, :P' +
        'ASSWRD, '
      '   :PHONE, :PIN, :USER_ID, :USER_NAME)')
    RefreshSQL.Strings = (
      'Select '
      '  USER_KEY,'
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
      '  PASSWRD,'
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
        'IL, NEXT_ORDER_NUMBER, ORDER_PREFIX, ORDER_SUFFIX, PASSWRD, PIN,' +
        ' LAST_NAME, MIDDLE_NAME, FIRST_NAME from USERS')
    ModifySQL.Strings = (
      'update USERS'
      'set'
      '  ADDRESS = :ADDRESS,'
      '  EMAIL = :EMAIL,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  INITIALS = :INITIALS,'
      '  LAST_NAME = :LAST_NAME,'
      '  MIDDLE_NAME = :MIDDLE_NAME,'
      '  MOBILE = :MOBILE,'
      '  NEXT_ORDER_NUMBER = :NEXT_ORDER_NUMBER,'
      '  ORDER_PREFIX = :ORDER_PREFIX,'
      '  ORDER_SUFFIX = :ORDER_SUFFIX,'
      '  PASSWRD = :PASSWRD,'
      '  PHONE = :PHONE,'
      '  PIN = :PIN,'
      '  USER_ID = :USER_ID,'
      '  USER_NAME = :USER_NAME'
      'where'
      '  USER_ID = :OLD_USER_ID')
    Left = 72
    Top = 552
  end
  object dsUsers: TDataSource
    AutoEdit = False
    DataSet = dtUsers
    Left = 128
    Top = 552
  end
end
