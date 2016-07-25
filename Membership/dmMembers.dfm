object dm: Tdm
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Left = 60
  Top = 119
  Height = 656
  Width = 937
  object dsrMembers: TDataSource
    DataSet = ibdsMembers
    OnStateChange = dsrMembersStateChange
    Left = 25
    Top = 104
  end
  object dsrM_Events: TDataSource
    DataSet = ibdsM_Events
    Left = 24
    Top = 267
  end
  object dsrM_Subs: TDataSource
    DataSet = ibdsM_Subs
    Left = 24
    Top = 161
  end
  object dsrM_Sections: TDataSource
    DataSet = ibdsM_Sections
    Left = 24
    Top = 212
  end
  object IBDatabase: TIBDatabase
    Connected = True
    DatabaseName = 'C:\StockMaster\Sockmaster Sites\Papanui\STOCKMASTER0311.GDB'
    Params.Strings = (
      'user_name=DEFAULT'
      'PASSWORD=password'
      'sql_role_name=allusers'
      '')
    LoginPrompt = False
    DefaultTransaction = IBTransaction
    IdleTimer = 0
    SQLDialect = 3
    TraceFlags = []
    AllowStreamedConnected = False
    AfterConnect = IBDatabaseAfterConnect
    Left = 232
  end
  object IBTransaction: TIBTransaction
    Active = True
    DefaultDatabase = IBDatabase
    DefaultAction = TARollbackRetaining
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 213
    Top = 56
  end
  object tblTenders: TIBTable
    Database = IBDatabase
    Transaction = IBTransPay
    ForcedRefresh = True
    AfterPost = tblTendersAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'TENDER'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'TENDER_TYPE'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'EX_RATE'
        DataType = ftBCD
        Precision = 9
        Size = 3
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY16'
        Fields = 'TENDER'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'TENDERS'
    Left = 439
    Top = 288
    object tblTendersTENDER_TYPE: TIBStringField
      DisplayLabel = 'Tender'
      DisplayWidth = 48
      FieldName = 'TENDER_TYPE'
      Visible = False
    end
    object tblTendersEX_RATE: TIBBCDField
      DisplayWidth = 16
      FieldName = 'EX_RATE'
      Visible = False
      Precision = 9
      Size = 3
    end
    object tblTendersTENDER: TIntegerField
      DisplayLabel = 'Tender'
      FieldName = 'TENDER'
      Required = True
      Visible = False
      DisplayFormat = '####.00'
    end
  end
  object dsrTblTenders: TDataSource
    DataSet = tblTenders
    OnStateChange = dsrTblTendersStateChange
    Left = 447
    Top = 296
  end
  object tblEvents: TIBTable
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblEventsAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblEventsBeforeDelete
    BeforeInsert = tblEventsBeforeInsert
    BeforePost = tblEventsBeforePost
    OnPostError = tblEventsPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'EVENT'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'BONUS_POINTS'
        DataType = ftInteger
      end
      item
        Name = 'CUST_DISPLAY'
        DataType = ftString
        Size = 30
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY9'
        Fields = 'EVENT'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'EVENTS'
    Left = 107
    Top = 304
    object tblEventsEVENT: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 10
      FieldName = 'EVENT'
      Required = True
    end
    object tblEventsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 36
      FieldName = 'DESCRIPTION'
      Size = 30
    end
    object tblEventsBONUS_POINTS: TIntegerField
      DisplayLabel = 'Bonus Points'
      DisplayWidth = 17
      FieldName = 'BONUS_POINTS'
    end
    object tblEventsCUST_DISPLAY: TIBStringField
      DisplayLabel = 'Customer Display'
      DisplayWidth = 30
      FieldName = 'CUST_DISPLAY'
      Visible = False
      Size = 30
    end
  end
  object tblSubs: TIBTable
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblSubsAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblSubsBeforeDelete
    BeforeInsert = tblSubsBeforeInsert
    BeforePost = tblSubsBeforePost
    OnPostError = tblSubsPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'SUBSCRIPTION'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'POINTS_SPENT'
        DataType = ftInteger
      end
      item
        Name = 'CUST_DISPLAY'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'FEE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'SAVED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KEEP_EXPIRES_CURRENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'GL_ACCOUNT'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'PRO_RATA'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DISCOUNTABLE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PRINT_ON_CARD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_DESC'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'SPARE_CHAR'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SPARE_SMALLINT'
        DataType = ftSmallint
      end
      item
        Name = 'SUB_GROUPS'
        DataType = ftInteger
      end
      item
        Name = 'CODE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 6
      end
      item
        Name = 'AP_PREMIUM'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'ON_INVOICE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'TERM'
        DataType = ftSmallint
      end
      item
        Name = 'EXPIRES_CARD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'EXPIRES_DESC'
        DataType = ftString
        Size = 20
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY38'
        Fields = 'SUBSCRIPTION'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'SUBSCRIPTIONS'
    Left = 102
    Top = 184
    object tblSubsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 24
      FieldName = 'DESCRIPTION'
      Size = 60
    end
    object tblSubsPRINT_ON_CARD: TIBStringField
      DisplayLabel = 'On Card'
      DisplayWidth = 7
      FieldName = 'PRINT_ON_CARD'
      Size = 1
    end
    object tblSubsCARD_DESC: TIBStringField
      DisplayLabel = 'Card Description'
      DisplayWidth = 35
      FieldName = 'CARD_DESC'
      Size = 60
    end
    object tblSubsSUB_GROUPS: TIntegerField
      DisplayLabel = 'Group'
      DisplayWidth = 10
      FieldName = 'SUB_GROUPS'
    end
    object tblSubsON_INVOICE: TIBStringField
      DisplayLabel = 'On Invoice'
      DisplayWidth = 1
      FieldName = 'ON_INVOICE'
      Size = 1
    end
    object tblSubsPOINTS_SPENT: TIntegerField
      DisplayLabel = 'Rpt Group'
      DisplayWidth = 10
      FieldName = 'POINTS_SPENT'
    end
    object tblSubsCODE: TIBStringField
      DisplayLabel = 'Code'
      DisplayWidth = 6
      FieldName = 'CODE'
      Visible = False
      Size = 6
    end
    object tblSubsSUBSCRIPTION: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 6
      FieldName = 'SUBSCRIPTION'
      Visible = False
    end
    object tblSubsFEE: TIBBCDField
      DisplayLabel = 'Fee'
      DisplayWidth = 9
      FieldName = 'FEE'
      Visible = False
      currency = True
      Precision = 9
      Size = 2
    end
    object tblSubsKEEP_EXPIRES_CURRENT: TIBStringField
      DisplayLabel = 'Expires Current'
      DisplayWidth = 12
      FieldName = 'KEEP_EXPIRES_CURRENT'
      Visible = False
      Size = 1
    end
    object tblSubsDISCOUNTABLE: TIBStringField
      DisplayLabel = 'Discount'
      DisplayWidth = 7
      FieldName = 'DISCOUNTABLE'
      Visible = False
      Size = 1
    end
    object tblSubsPRO_RATA: TIBStringField
      DisplayLabel = 'Pro Rata'
      DisplayWidth = 7
      FieldName = 'PRO_RATA'
      Visible = False
      Size = 1
    end
    object tblSubsCUST_DISPLAY: TIBStringField
      DisplayLabel = 'Customer Display'
      FieldName = 'CUST_DISPLAY'
      Visible = False
      Size = 30
    end
    object tblSubsSAVED: TIBStringField
      DisplayLabel = 'Saved'
      FieldName = 'SAVED'
      Visible = False
      Size = 1
    end
    object tblSubsGL_ACCOUNT: TIBStringField
      FieldName = 'GL_ACCOUNT'
      Visible = False
      Size = 15
    end
    object tblSubsSPARE_CHAR: TIBStringField
      FieldName = 'SPARE_CHAR'
      Visible = False
      Size = 1
    end
    object tblSubsSPARE_SMALLINT: TSmallintField
      FieldName = 'SPARE_SMALLINT'
      Visible = False
    end
    object tblSubsGROUP_DESC: TStringField
      FieldKind = fkLookup
      FieldName = 'GROUP_DESC'
      LookupDataSet = tblGroups
      LookupKeyFields = 'GROUPS'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'SUB_GROUPS'
      Visible = False
      Size = 60
      Lookup = True
    end
    object tblSubsAP_PREMIUM: TIBBCDField
      FieldName = 'AP_PREMIUM'
      Visible = False
      currency = True
      Precision = 9
      Size = 2
    end
    object tblSubsTERM: TSmallintField
      FieldName = 'TERM'
      Visible = False
    end
    object tblSubsEXPIRES_CARD: TIBStringField
      FieldName = 'EXPIRES_CARD'
      Visible = False
      Size = 1
    end
    object tblSubsEXPIRES_DESC: TIBStringField
      FieldName = 'EXPIRES_DESC'
      Visible = False
    end
  end
  object tblSections: TIBTable
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblSectionsAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblSectionsBeforeDelete
    BeforeInsert = tblSectionsBeforeInsert
    BeforePost = tblEventsBeforePost
    OnPostError = tblSectionsPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'SECTION'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'FEE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'SAVED'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'EXPIRES'
        DataType = ftDate
      end
      item
        Name = 'DAY_PAYMENTS'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'GL_ACCOUNT'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'LAST_ROLL'
        DataType = ftDate
      end
      item
        Name = 'PRINT_ON_CARD'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_DESC'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'SPARE_SMALLINT'
        DataType = ftSmallint
      end
      item
        Name = 'SPARE_CHAR'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DONATION'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'MIN_FEE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY37'
        Fields = 'SECTION'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'SECTIONS'
    Left = 111
    Top = 240
    object tblSectionsSECTION: TIntegerField
      DisplayLabel = 'Section'
      DisplayWidth = 7
      FieldName = 'SECTION'
      Required = True
    end
    object tblSectionsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 30
      FieldName = 'DESCRIPTION'
      Size = 60
    end
    object tblSectionsPRINT_ON_CARD: TIBStringField
      DisplayLabel = 'On Card'
      DisplayWidth = 7
      FieldName = 'PRINT_ON_CARD'
      Visible = False
      Size = 1
    end
    object tblSectionsCARD_DESC: TIBStringField
      DisplayLabel = 'Card Description'
      DisplayWidth = 35
      FieldName = 'CARD_DESC'
      Visible = False
      Size = 60
    end
    object tblSectionsEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 11
      FieldName = 'EXPIRES'
      Visible = False
      DisplayFormat = 'dd mmm yyyy'
    end
    object tblSectionsFEE: TIBBCDField
      DisplayLabel = 'Fee'
      DisplayWidth = 10
      FieldName = 'FEE'
      Visible = False
      currency = True
      Precision = 9
      Size = 2
    end
    object tblSectionsLAST_ROLL: TDateField
      DisplayLabel = 'Last Roll'
      DisplayWidth = 11
      FieldName = 'LAST_ROLL'
      Visible = False
      DisplayFormat = 'dd mmm yyyy'
    end
    object tblSectionsSAVED: TIBStringField
      FieldName = 'SAVED'
      Visible = False
      Size = 1
    end
    object tblSectionsDAY_PAYMENTS: TIBStringField
      FieldName = 'DAY_PAYMENTS'
      Visible = False
      Size = 10
    end
    object tblSectionsGL_ACCOUNT: TIBStringField
      FieldName = 'GL_ACCOUNT'
      Visible = False
      Size = 17
    end
    object tblSectionsSPARE_SMALLINT: TSmallintField
      FieldName = 'SPARE_SMALLINT'
      Visible = False
    end
    object tblSectionsSPARE_CHAR: TIBStringField
      FieldName = 'SPARE_CHAR'
      Visible = False
      Size = 1
    end
    object tblSectionsDONATION: TIBBCDField
      FieldName = 'DONATION'
      Precision = 9
      Size = 2
    end
    object tblSectionsMIN_FEE: TIBBCDField
      FieldName = 'MIN_FEE'
      currency = True
      Precision = 9
      Size = 2
    end
  end
  object tblGroups: TIBTable
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblGroupsAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblGroupsBeforeDelete
    BeforeInsert = tblGroupsBeforeInsert
    BeforePost = tblEventsBeforePost
    OnPostError = tblGroupsPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'GROUPS'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'VIP_POINTS'
        DataType = ftInteger
      end
      item
        Name = 'CARD_DESC'
        DataType = ftString
        Size = 60
      end
      item
        Name = 'PRINT_ON_CARD'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'IN_REPORT'
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY14'
        Fields = 'GROUPS'
        Options = [ixPrimary, ixUnique]
      end>
    IndexFieldNames = 'GROUPS'
    StoreDefs = True
    TableName = 'GROUPS'
    Left = 104
    Top = 116
    object tblGroupsGROUPS: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 10
      FieldName = 'GROUPS'
    end
    object tblGroupsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 24
      FieldName = 'DESCRIPTION'
      Size = 60
    end
    object tblGroupsVIP_POINTS: TIntegerField
      DisplayLabel = 'VIP Points'
      DisplayWidth = 10
      FieldName = 'VIP_POINTS'
      Visible = False
    end
    object tblGroupsPRINT_ON_CARD: TIBStringField
      DisplayLabel = 'On Card'
      DisplayWidth = 7
      FieldName = 'PRINT_ON_CARD'
      Visible = False
      Size = 1
    end
    object tblGroupsCARD_DESC: TIBStringField
      DisplayLabel = 'Card Description'
      DisplayWidth = 35
      FieldName = 'CARD_DESC'
      Visible = False
      Size = 62
    end
    object tblGroupsIN_REPORT: TIBStringField
      FieldName = 'IN_REPORT'
      Size = 1
    end
  end
  object dsrTblGroups: TDataSource
    DataSet = tblGroups
    OnStateChange = dsrTblGroupsStateChange
    Left = 104
    Top = 129
  end
  object dsrMembersLook: TDataSource
    DataSet = qryMembersLook
    Left = 24
    Top = 48
  end
  object qryMembersLook: TIBQuery
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'SELECT MEMBER,SURNAME,FULL_NAME,ADDRESS,FIRST_NAME,EXPIRES,'
      'MAILING_LIST_CODE  FROM MEMBERS where MEMBER = -1'
      '')
    Left = 24
    Top = 58
    object qryMembersLookMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 9
      FieldName = 'MEMBER'
      Required = True
    end
    object qryMembersLookFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 18
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object qryMembersLookFIRST_NAME: TIBStringField
      DisplayLabel = 'First Name'
      DisplayWidth = 12
      FieldName = 'FIRST_NAME'
    end
    object qryMembersLookSURNAME: TIBStringField
      DisplayLabel = 'Surname'
      DisplayWidth = 15
      FieldName = 'SURNAME'
      Size = 25
    end
    object qryMembersLookADDRESS: TIBStringField
      DisplayLabel = 'Address'
      DisplayWidth = 20
      FieldName = 'ADDRESS'
      Size = 30
    end
    object qryMembersLookMAILING_LIST_CODE: TIntegerField
      DisplayLabel = 'Parent Code'
      DisplayWidth = 8
      FieldName = 'MAILING_LIST_CODE'
    end
    object qryMembersLookEXPIRES: TDateField
      FieldName = 'EXPIRES'
      Visible = False
      DisplayFormat = 'dd mmm yyyy'
    end
  end
  object ibdsMembers: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterCancel = ibdsMembersAfterCancel
    AfterDelete = tblSubsAfterPost
    AfterEdit = ibdsMembersAfterEdit
    AfterInsert = ibdsMembersAfterInsert
    AfterPost = ibdsMembersAfterPost
    BeforeCancel = ibdsMembersBeforeCancel
    BeforeDelete = ibdsMembersBeforeDelete
    BeforeEdit = ibdsMembersBeforeEdit
    BeforeInsert = ibdsMembersBeforeInsert
    BeforePost = ibdsMembersBeforePost
    OnCalcFields = ibdsMembersCalcFields
    OnPostError = ibdsMembersPostError
    BufferChunks = 40
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER'
      '')
    InsertSQL.Strings = (
      'insert into MEMBERS'
      '  (MEMBER, KNOWN_AS, MALE_FEMALE, DATE_OF_BIRTH, LUCKY_DATE, '
      'ADDRESS, ADDRESS1, '
      '   MAILING_LIST, MAILING_CODE, MAILING_LIST_CODE, '
      'NEWSLETTER, '
      '   EMAIL, HOME_PHONE, WORK_PHONE, CELL_PHONE, PROPOSER, '
      'SECONDER, JOINED, '
      '   JOINING_FEE,  RECEIPT, APPROVED, EXPIRES, TRANS_FROM, '
      'SUB_PAID, '
      '   SECTION_PAID, ALLOW_CREDIT, CREDIT_LIMIT, BONUS_CREDIT, '
      'EARNT_CREDIT, '
      '   PRE_PAID_CREDIT, EARN_POINTS,  POINTS_AVAILABLE,'
      'TOTAL_EARNT, '
      '   TOTAL_REDEEMED, PRODUCT_PURCHASE, AMOUNT_DONATED, '
      'PRICE_LEVEL, VIP_PAY_ONLY, '
      '   CARD_TYPE, GL_ACCOUNT, DISCOUNT, INCL_EOD, POSITIONS, '
      'CHARGE_CODE, DECEASED, '
      '   SENIOR, FOUNDATION, DONATED, REFUNDED, DEBENTURE, SAVED, '
      'DAIRY_NOTES, '
      '   EDIT_DATE, TITLE, INITIALS, FIRST_NAME, SURNAME, FULL_NAME, '
      'IS_ACTIVE, '
      '   ANALYSE, INACTIVE_DATE, DISTRICT, CITY, COUNTRY,SUB_PAYMENT,'
      
        'SWIPE_CARD_NUMBER,FAX_NUMBER,FAX_WORK,COMPANY,OCCUPATION,BARCODE' +
        '_NUMBER, CARD_LAST_USED,'
      'AUTO_PAYMENT,PAYMENT_PERIOD,INVOICE_NUMBER,'
      
        'PRINT_CARD,PRINT_RECEIPT,RECEIVE_EMAIL,NUMBER_CARDS,PAYMENT_DAY,' +
        'STREET_ADDRESS,STREET_ADDRESS1,'
      
        'STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50, CLU' +
        'BS, ASSOCIATION,REASON,SWIPE_CARD,'
      
        'VISITS,LAST_VISIT,DONATION,DONATION_MEMBER,PROPOSER_NAME,SECONDE' +
        'R_NAME  )'
      'values'
      
        '  (:MEMBER, :KNOWN_AS, :MALE_FEMALE, :DATE_OF_BIRTH, :LUCKY_DATE' +
        ', '
      ':ADDRESS, '
      '   :ADDRESS1, :MAILING_LIST, :MAILING_CODE, :MAILING_LIST_CODE, '
      '   :NEWSLETTER, :EMAIL, :HOME_PHONE, :WORK_PHONE, :CELL_PHONE, '
      ':PROPOSER, '
      
        '   :SECONDER, :JOINED, :JOINING_FEE, :RECEIPT, :APPROVED, :EXPIR' +
        'ES, '
      '   :TRANS_FROM, :SUB_PAID, :SECTION_PAID, :ALLOW_CREDIT, '
      ':CREDIT_LIMIT, '
      '   :BONUS_CREDIT, :EARNT_CREDIT, :PRE_PAID_CREDIT,'
      ':EARN_POINTS, :POINTS_AVAILABLE,'
      '    :TOTAL_EARNT, :TOTAL_REDEEMED, '
      ':PRODUCT_PURCHASE, '
      '   :AMOUNT_DONATED, :PRICE_LEVEL, :VIP_PAY_ONLY, :CARD_TYPE, '
      ':GL_ACCOUNT, '
      
        '   :DISCOUNT, :INCL_EOD, :POSITIONS, :CHARGE_CODE, :DECEASED, :S' +
        'ENIOR, '
      '   :FOUNDATION, :DONATED, :REFUNDED, :DEBENTURE, :SAVED, '
      ':DAIRY_NOTES, '
      
        '   :EDIT_DATE, :TITLE, :INITIALS, :FIRST_NAME, :SURNAME, :FULL_N' +
        'AME, '
      ':IS_ACTIVE, '
      
        '   :ANALYSE, :INACTIVE_DATE,:DISTRICT,:CITY, :COUNTRY,:SUB_PAYME' +
        'NT,'
      
        '  :SWIPE_CARD_NUMBER, :FAX_NUMBER,:FAX_WORK, :COMPANY, :OCCUPATI' +
        'ON,'
      '  :BARCODE_NUMBER,:CARD_LAST_USED,'
      '  :AUTO_PAYMENT,:PAYMENT_PERIOD,:INVOICE_NUMBER,'
      
        ' :PRINT_CARD, :PRINT_RECEIPT,:RECEIVE_EMAIL,:NUMBER_CARDS, :PAYM' +
        'ENT_DAY,'
      ':STREET_ADDRESS,:STREET_ADDRESS1,'
      
        ':STREET_CITY,:STREET_COUNTRY,:STREET_DISTRICT,:SPARE_VARCHAR_50 ' +
        ',:CLUBS, :ASSOCIATION,:REASON,:SWIPE_CARD,'
      
        ':VISITS, :LAST_VISIT ,:DONATION, :DONATION_MEMBER,:PROPOSER_NAME' +
        ',:SECONDER_NAME)'
      '')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  KNOWN_AS,'
      '  MALE_FEMALE,'
      '  DATE_OF_BIRTH,'
      '  LUCKY_DATE,'
      '  ADDRESS,'
      '  ADDRESS1,'
      '  MAILING_LIST,'
      '  MAILING_CODE,'
      '  MAILING_LIST_CODE,'
      '  NEWSLETTER,'
      '  EMAIL,'
      '  HOME_PHONE,'
      '  WORK_PHONE,'
      '  CELL_PHONE,'
      '  PROPOSER,'
      '  SECONDER,'
      '  JOINED,'
      '  JOINING_FEE,'
      '  RECEIPT,'
      '  APPROVED,'
      '  EXPIRES,'
      '  TRANS_FROM,'
      '  SUB_PAID,'
      '  SECTION_PAID,'
      '  ALLOW_CREDIT,'
      '  CREDIT_LIMIT,'
      '  BONUS_CREDIT,'
      '  EARNT_CREDIT,'
      '  PRE_PAID_CREDIT,'
      '   EARN_POINTS,'
      '  POINTS_AVAILABLE,'
      '  TOTAL_EARNT,'
      '  TOTAL_REDEEMED,'
      '  PRODUCT_PURCHASE,'
      '  AMOUNT_DONATED,'
      '  PRICE_LEVEL,'
      '  VIP_PAY_ONLY,'
      '  CARD_TYPE,'
      '  GL_ACCOUNT,'
      '  DISCOUNT,'
      '  INCL_EOD,'
      '  POSITIONS,'
      '  CHARGE_CODE,'
      '  DECEASED,'
      '  SENIOR,'
      '  FOUNDATION,'
      '  DONATED,'
      '  REFUNDED,'
      '  DEBENTURE,'
      '  SAVED,'
      '  DAIRY_NOTES,'
      '  EDIT_DATE,'
      '  TITLE,'
      '  INITIALS,'
      '  FIRST_NAME,'
      '  SURNAME,'
      '  FULL_NAME,'
      '  IS_ACTIVE,'
      '  ANALYSE,'
      '  INACTIVE_DATE,'
      '  DISTRICT,'
      '  CITY,'
      '  COUNTRY,'
      '  SUB_PAYMENT,'
      '  SWIPE_CARD_NUMBER,'
      '  FAX_NUMBER,FAX_WORK,'
      '  COMPANY,'
      '  OCCUPATION,'
      '  BARCODE_NUMBER,'
      '  CARD_LAST_USED,'
      '  AUTO_PAYMENT,'
      '  PAYMENT_PERIOD,'
      ' INVOICE_NUMBER,'
      'PRINT_CARD,PRINT_RECEIPT,RECEIVE_EMAIL,NUMBER_CARDS,PAYMENT_DAY,'
      ' STREET_ADDRESS,STREET_ADDRESS1, '
      ' STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50,'
      'CLUBS, ASSOCIATION ,REASON,SWIPE_CARD,VISITS,LAST_VISIT,'
      'DONATION,DONATION_MEMBER,PROPOSER_NAME,SECONDER_NAME'
      'from MEMBERS '
      'where'
      '  MEMBER = :OLD_MEMBER'
      '')
    SelectSQL.Strings = (
      
        'select MEMBER, KNOWN_AS, MALE_FEMALE, DATE_OF_BIRTH, LUCKY_DATE,' +
        ' ADDRESS, ADDRESS1, MAILING_LIST, MAILING_CODE, MAILING_LIST_COD' +
        'E, NEWSLETTER, EMAIL, HOME_PHONE, WORK_PHONE, CELL_PHONE, PROPOS' +
        'ER, SECONDER, JOINED, JOINING_FEE,  RECEIPT, APPROVED, EXPIRES, ' +
        'TRANS_FROM,  SUB_PAID, SECTION_PAID, ALLOW_CREDIT, CREDIT_LIMIT,' +
        ' BONUS_CREDIT, EARNT_CREDIT, PRE_PAID_CREDIT, EARN_POINTS, TOTAL' +
        '_EARNT, POINTS_AVAILABLE,TOTAL_REDEEMED, PRODUCT_PURCHASE, AMOUN' +
        'T_DONATED, PRICE_LEVEL, VIP_PAY_ONLY, CARD_TYPE, GL_ACCOUNT, DIS' +
        'COUNT, INCL_EOD, POSITIONS, CHARGE_CODE, DECEASED, SENIOR, FOUND' +
        'ATION, DONATED, REFUNDED, DEBENTURE, SAVED, DAIRY_NOTES, EDIT_DA' +
        'TE, TITLE, INITIALS, FIRST_NAME, SURNAME, FULL_NAME, IS_ACTIVE,A' +
        'NALYSE,INACTIVE_DATE,DISTRICT,CITY,COUNTRY,SUB_PAYMENT,'
      
        'SWIPE_CARD_NUMBER,FAX_NUMBER,FAX_WORK,COMPANY,OCCUPATION,BARCODE' +
        '_NUMBER,CARD_LAST_USED,'
      'AUTO_PAYMENT,PAYMENT_PERIOD,PAYMENT_DAY,INVOICE_NUMBER,'
      
        'PRINT_CARD,PRINT_RECEIPT,RECEIVE_EMAIL,NUMBER_CARDS,STREET_ADDRE' +
        'SS,STREET_ADDRESS1,'
      
        'STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50, CLU' +
        'BS, ASSOCIATION,REASON,SWIPE_CARD,VISITS,LAST_VISIT,DONATION,DON' +
        'ATION_MEMBER,PROPOSER_NAME,SECONDER_NAME'
      '  from MEMBERS'
      'WHERE MEMBER = :member')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  MEMBER = :MEMBER,'
      '  KNOWN_AS = :KNOWN_AS,'
      '  MALE_FEMALE = :MALE_FEMALE,'
      '  DATE_OF_BIRTH = :DATE_OF_BIRTH,'
      '  LUCKY_DATE = :LUCKY_DATE,'
      '  ADDRESS = :ADDRESS,'
      '  ADDRESS1 = :ADDRESS1,'
      '  MAILING_LIST = :MAILING_LIST,'
      '  MAILING_CODE = :MAILING_CODE,'
      '  MAILING_LIST_CODE = :MAILING_LIST_CODE,'
      '  NEWSLETTER = :NEWSLETTER,'
      '  EMAIL = :EMAIL,'
      '  HOME_PHONE = :HOME_PHONE,'
      '  WORK_PHONE = :WORK_PHONE,'
      '  CELL_PHONE = :CELL_PHONE,'
      '  PROPOSER = :PROPOSER,'
      '  SECONDER = :SECONDER,'
      '  JOINED = :JOINED,'
      '  JOINING_FEE = :JOINING_FEE,'
      '  RECEIPT = :RECEIPT,'
      '  APPROVED = :APPROVED,'
      '  EXPIRES = :EXPIRES,'
      '  TRANS_FROM = :TRANS_FROM,'
      '  SUB_PAID = :SUB_PAID,'
      '  SECTION_PAID = :SECTION_PAID,'
      '  ALLOW_CREDIT = :ALLOW_CREDIT,'
      '  CREDIT_LIMIT = :CREDIT_LIMIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  EARN_POINTS = :EARN_POINTS,'
      '  POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  PRODUCT_PURCHASE = :PRODUCT_PURCHASE,'
      '  AMOUNT_DONATED = :AMOUNT_DONATED,'
      '  PRICE_LEVEL = :PRICE_LEVEL,'
      '  VIP_PAY_ONLY = :VIP_PAY_ONLY,'
      '  CARD_TYPE = :CARD_TYPE,'
      '  GL_ACCOUNT = :GL_ACCOUNT,'
      '  DISCOUNT = :DISCOUNT,'
      '  INCL_EOD = :INCL_EOD,'
      '  POSITIONS = :POSITIONS,'
      '  CHARGE_CODE = :CHARGE_CODE,'
      '  DECEASED = :DECEASED,'
      '  SENIOR = :SENIOR,'
      '  FOUNDATION = :FOUNDATION,'
      '  DONATED = :DONATED,'
      '  REFUNDED = :REFUNDED,'
      '  DEBENTURE = :DEBENTURE,'
      '  SAVED = :SAVED,'
      '  DAIRY_NOTES = :DAIRY_NOTES,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  TITLE = :TITLE,'
      '  INITIALS = :INITIALS,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  SURNAME = :SURNAME,'
      '  FULL_NAME = :FULL_NAME,'
      '  IS_ACTIVE = :IS_ACTIVE,'
      '  ANALYSE = :ANALYSE,'
      '  INACTIVE_DATE = :INACTIVE_DATE,'
      '  DISTRICT =  :DISTRICT,'
      '  CITY = :CITY,'
      '  COUNTRY = :COUNTRY, '
      '  SUB_PAYMENT = :SUB_PAYMENT,'
      '  SWIPE_CARD_NUMBER  = :SWIPE_CARD_NUMBER,'
      '  FAX_NUMBER  = :FAX_NUMBER,'
      'FAX_WORK = :FAX_WORK,'
      '  COMPANY  = :COMPANY,'
      '  OCCUPATION  = :OCCUPATION,'
      '  BARCODE_NUMBER  = :BARCODE_NUMBER,'
      '  CARD_LAST_USED  = :CARD_LAST_USED,'
      '  AUTO_PAYMENT  = :AUTO_PAYMENT,'
      '  PAYMENT_PERIOD = :PAYMENT_PERIOD,'
      '  INVOICE_NUMBER = :INVOICE_NUMBER,'
      ' PRINT_CARD = :PRINT_CARD,'
      ' PRINT_RECEIPT = :PRINT_RECEIPT,'
      ' RECEIVE_EMAIL = :RECEIVE_EMAIL,'
      'NUMBER_CARDS = :NUMBER_CARDS,'
      'PAYMENT_DAY = :PAYMENT_DAY,'
      ' STREET_ADDRESS = :STREET_ADDRESS,'
      ' STREET_ADDRESS1 = :STREET_ADDRESS1,'
      ' STREET_CITY = :STREET_CITY,'
      ' STREET_COUNTRY = :STREET_COUNTRY,'
      ' STREET_DISTRICT = :STREET_DISTRICT,'
      'SPARE_VARCHAR_50  = :SPARE_VARCHAR_50, '
      'CLUBS = :CLUBS, '
      'ASSOCIATION = :ASSOCIATION,'
      'REASON = :REASON,'
      'SWIPE_CARD = :SWIPE_CARD,'
      'VISITS = :VISITS,'
      'LAST_VISIT = :LAST_VISIT,'
      'DONATION = :DONATION,'
      'DONATION_MEMBER = :DONATION_MEMBER,'
      'PROPOSER_NAME = :PROPOSER_NAME,'
      'SECONDER_NAME = :SECONDER_NAME'
      ''
      ''
      'where'
      '  MEMBER = :OLD_MEMBER'
      '')
    UniDirectional = True
    DataSource = dsrMembersLook
    Left = 25
    Top = 116
    object ibdsMembersMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsMembersKNOWN_AS: TIBStringField
      FieldName = 'KNOWN_AS'
      Size = 25
    end
    object ibdsMembersMALE_FEMALE: TIBStringField
      FieldName = 'MALE_FEMALE'
      Size = 1
    end
    object ibdsMembersDATE_OF_BIRTH: TDateField
      FieldName = 'DATE_OF_BIRTH'
    end
    object ibdsMembersLUCKY_DATE: TDateField
      FieldName = 'LUCKY_DATE'
    end
    object ibdsMembersADDRESS: TIBStringField
      FieldName = 'ADDRESS'
      Size = 30
    end
    object ibdsMembersADDRESS1: TIBStringField
      FieldName = 'ADDRESS1'
      Size = 30
    end
    object ibdsMembersMAILING_LIST: TIBStringField
      FieldName = 'MAILING_LIST'
      Size = 1
    end
    object ibdsMembersMAILING_CODE: TIntegerField
      FieldName = 'MAILING_CODE'
    end
    object ibdsMembersMAILING_LIST_CODE: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
    end
    object ibdsMembersNEWSLETTER: TIBStringField
      FieldName = 'NEWSLETTER'
      Size = 1
    end
    object ibdsMembersEMAIL: TIBStringField
      FieldName = 'EMAIL'
      Size = 60
    end
    object ibdsMembersHOME_PHONE: TIBStringField
      FieldName = 'HOME_PHONE'
      Size = 35
    end
    object ibdsMembersWORK_PHONE: TIBStringField
      FieldName = 'WORK_PHONE'
      Size = 14
    end
    object ibdsMembersCELL_PHONE: TIBStringField
      FieldName = 'CELL_PHONE'
      Size = 14
    end
    object ibdsMembersPROPOSER: TIntegerField
      FieldName = 'PROPOSER'
    end
    object ibdsMembersSECONDER: TIntegerField
      FieldName = 'SECONDER'
    end
    object ibdsMembersJOINED: TDateField
      FieldName = 'JOINED'
    end
    object ibdsMembersJOINING_FEE: TIBBCDField
      FieldName = 'JOINING_FEE'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsMembersRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
    end
    object ibdsMembersAPPROVED: TDateField
      FieldName = 'APPROVED'
    end
    object ibdsMembersEXPIRES: TDateField
      FieldName = 'EXPIRES'
    end
    object ibdsMembersTRANS_FROM: TIBStringField
      FieldName = 'TRANS_FROM'
      Size = 40
    end
    object ibdsMembersSUB_PAID: TSmallintField
      FieldName = 'SUB_PAID'
    end
    object SECTION_PAID: TSmallintField
      FieldName = 'SECTION_PAID'
    end
    object ibdsMembersALLOW_CREDIT: TIBStringField
      FieldName = 'ALLOW_CREDIT'
      Size = 1
    end
    object ibdsMembersCREDIT_LIMIT: TIBBCDField
      FieldName = 'CREDIT_LIMIT'
      currency = True
      Precision = 9
      Size = 3
    end
    object ibdsMembersEARN_POINTS: TIBStringField
      FieldName = 'EARN_POINTS'
      Size = 1
    end
    object ibdsMembersPRODUCT_PURCHASE: TIBBCDField
      FieldName = 'PRODUCT_PURCHASE'
      Precision = 18
      Size = 3
    end
    object ibdsMembersAMOUNT_DONATED: TIBBCDField
      FieldName = 'AMOUNT_DONATED'
      Precision = 9
      Size = 3
    end
    object ibdsMembersPRICE_LEVEL: TIntegerField
      FieldName = 'PRICE_LEVEL'
    end
    object ibdsMembersVIP_PAY_ONLY: TIBStringField
      FieldName = 'VIP_PAY_ONLY'
      Size = 1
    end
    object ibdsMembersCARD_TYPE: TIBStringField
      FieldName = 'CARD_TYPE'
      Size = 1
    end
    object ibdsMembersGL_ACCOUNT: TIBStringField
      FieldName = 'GL_ACCOUNT'
      Size = 15
    end
    object ibdsMembersDISCOUNT: TIBBCDField
      FieldName = 'DISCOUNT'
      Precision = 9
      Size = 3
    end
    object ibdsMembersINCL_EOD: TIBStringField
      FieldName = 'INCL_EOD'
      Size = 1
    end
    object ibdsMembersPOSITIONS: TIBStringField
      FieldName = 'POSITIONS'
      Size = 25
    end
    object ibdsMembersCHARGE_CODE: TIntegerField
      FieldName = 'CHARGE_CODE'
    end
    object ibdsMembersDECEASED: TIBStringField
      FieldName = 'DECEASED'
      Size = 1
    end
    object ibdsMembersSENIOR: TIBStringField
      FieldName = 'SENIOR'
      Size = 1
    end
    object ibdsMembersFOUNDATION: TIBStringField
      FieldName = 'FOUNDATION'
      Size = 1
    end
    object ibdsMembersDONATED: TIBStringField
      FieldName = 'DONATED'
      Size = 1
    end
    object ibdsMembersREFUNDED: TIBStringField
      FieldName = 'REFUNDED'
      Size = 1
    end
    object ibdsMembersDEBENTURE: TIntegerField
      FieldName = 'DEBENTURE'
    end
    object ibdsMembersSAVED: TIBStringField
      FieldName = 'SAVED'
      Size = 1
    end
    object ibdsMembersDAIRY_NOTES: TBlobField
      FieldName = 'DAIRY_NOTES'
      BlobType = ftBlob
      Size = 8
    end
    object ibdsMembersEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
    end
    object ibdsMembersTITLE: TIBStringField
      FieldName = 'TITLE'
      Size = 6
    end
    object ibdsMembersINITIALS: TIBStringField
      FieldName = 'INITIALS'
      Size = 6
    end
    object ibdsMembersFIRST_NAME: TIBStringField
      FieldName = 'FIRST_NAME'
    end
    object ibdsMembersSURNAME: TIBStringField
      FieldName = 'SURNAME'
      Size = 25
    end
    object ibdsMembersFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object ibdsMembersIS_ACTIVE: TIBStringField
      FieldName = 'IS_ACTIVE'
      Size = 1
    end
    object ibdsMembersANALYSE: TIBStringField
      FieldName = 'ANALYSE'
      Size = 1
    end
    object ibdsMembersINACTIVE_DATE: TDateField
      FieldName = 'INACTIVE_DATE'
    end
    object ibdsMembersDISTRICT: TIBStringField
      FieldName = 'DISTRICT'
    end
    object ibdsMembersCITY: TIBStringField
      FieldName = 'CITY'
      Size = 25
    end
    object ibdsMembersCOUNTRY: TIBStringField
      FieldName = 'COUNTRY'
      Size = 25
    end
    object ibdsMembersSUB_PAYMENT: TIBStringField
      FieldName = 'SUB_PAYMENT'
      Size = 1
    end
    object ibdsMembersSWIPE_CARD_NUMBER: TIntegerField
      FieldName = 'SWIPE_CARD_NUMBER'
    end
    object ibdsMembersFAX_NUMBER: TIBStringField
      FieldName = 'FAX_NUMBER'
      Size = 14
    end
    object ibdsMembersCOMPANY: TIBStringField
      FieldName = 'COMPANY'
      Size = 40
    end
    object ibdsMembersOCCUPATION: TIBStringField
      FieldName = 'OCCUPATION'
      Size = 30
    end
    object ibdsMembersBARCODE_NUMBER: TIntegerField
      FieldName = 'BARCODE_NUMBER'
    end
    object ibdsMembersCARD_LAST_USED: TDateField
      FieldName = 'CARD_LAST_USED'
    end
    object ibdsMembersAUTO_PAYMENT: TIBStringField
      FieldName = 'AUTO_PAYMENT'
      Size = 1
    end
    object ibdsMembersPAYMENT_PERIOD: TSmallintField
      FieldName = 'PAYMENT_PERIOD'
    end
    object ibdsMembersINVOICE_NUMBER: TIBStringField
      FieldName = 'INVOICE_NUMBER'
      Size = 22
    end
    object ibdsMembersPRINT_CARD: TIBStringField
      FieldName = 'PRINT_CARD'
      Size = 1
    end
    object ibdsMembersPRINT_RECEIPT: TIBStringField
      FieldName = 'PRINT_RECEIPT'
      Size = 1
    end
    object ibdsMembersRECEIVE_EMAIL: TIBStringField
      FieldName = 'RECEIVE_EMAIL'
      Size = 1
    end
    object ibdsMembersNUMBER_CARDS: TSmallintField
      FieldName = 'NUMBER_CARDS'
    end
    object ibdsMembersPAYMENT_DAY: TSmallintField
      FieldName = 'PAYMENT_DAY'
    end
    object ibdsMembersSTREET_ADDRESS: TIBStringField
      FieldName = 'STREET_ADDRESS'
      Size = 30
    end
    object ibdsMembersSTREET_ADDRESS1: TIBStringField
      FieldName = 'STREET_ADDRESS1'
      Size = 30
    end
    object ibdsMembersSTREET_CITY: TIBStringField
      FieldName = 'STREET_CITY'
      Size = 25
    end
    object ibdsMembersSTREET_COUNTRY: TIBStringField
      FieldName = 'STREET_COUNTRY'
      Size = 25
    end
    object ibdsMembersSTREET_DISTRICT: TIBStringField
      FieldName = 'STREET_DISTRICT'
    end
    object ibdsMembersFAX_WORK: TIBStringField
      FieldName = 'FAX_WORK'
      Size = 14
    end
    object ibdsMembersSPARE_VARCHAR_50: TIBStringField
      FieldName = 'SPARE_VARCHAR_50'
      Origin = 'MEMBERS.SPARE_VARCHAR_50'
      Size = 40
    end
    object ibdsMembersCLUBS: TIntegerField
      FieldName = 'CLUBS'
      Origin = 'MEMBERS.CLUBS'
    end
    object ibdsMembersASSOCIATION: TIBStringField
      FieldName = 'ASSOCIATION'
      Origin = 'MEMBERS.ASSOCIATION'
      FixedChar = True
      Size = 1
    end
    object ibdsMembersVALUE_OF_CREDIT: TFloatField
      FieldKind = fkCalculated
      FieldName = 'VALUE_OF_CREDIT'
      currency = True
      Calculated = True
    end
    object ibdsMembersBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
      currency = True
    end
    object ibdsMembersEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
      currency = True
    end
    object ibdsMembersPRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
      currency = True
    end
    object ibdsMembersTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
      currency = True
    end
    object ibdsMembersTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
      currency = True
    end
    object ibdsMembersREASON: TIBStringField
      FieldName = 'REASON'
      Origin = '"MEMBERS"."REASON"'
      Size = 37
    end
    object ibdsMembersPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
      currency = True
    end
    object ibdsMembersSWIPE_CARD: TIBStringField
      FieldName = 'SWIPE_CARD'
      Origin = '"MEMBERS"."SWIPE_CARD"'
      Size = 128
    end
    object ibdsMembersVISITS: TSmallintField
      FieldName = 'VISITS'
      Origin = '"MEMBERS"."VISITS"'
    end
    object ibdsMembersLAST_VISIT: TDateTimeField
      FieldName = 'LAST_VISIT'
      Origin = '"MEMBERS"."LAST_VISIT"'
    end
    object ibdsMembersDONATION: TIBBCDField
      FieldName = 'DONATION'
      Origin = '"MEMBERS"."DONATION"'
      Precision = 9
      Size = 2
    end
    object ibdsMembersDONATION_MEMBER: TIntegerField
      FieldName = 'DONATION_MEMBER'
      Origin = '"MEMBERS"."DONATION_MEMBER"'
    end
    object ibdsMembersPROPOSER_NAME: TIBStringField
      FieldName = 'PROPOSER_NAME'
      Origin = '"MEMBERS"."PROPOSER_NAME"'
      Size = 40
    end
    object ibdsMembersSECONDER_NAME: TIBStringField
      FieldName = 'SECONDER_NAME'
      Origin = '"MEMBERS"."SECONDER_NAME"'
      Size = 40
    end
  end
  object dsrTblEvents: TDataSource
    DataSet = tblEvents
    OnStateChange = dsrTblEventsStateChange
    Left = 109
    Top = 313
  end
  object dsrTblSections: TDataSource
    DataSet = tblSections
    OnStateChange = dsrTblSectionsStateChange
    Left = 109
    Top = 254
  end
  object dsrTblSubs: TDataSource
    DataSet = tblSubs
    OnStateChange = dsrTblSubsStateChange
    Left = 112
    Top = 192
  end
  object qryGeneral: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    Transaction = IBTransaction
    Left = 16
    Top = 368
  end
  object ibdsM_Subs: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_SubsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsM_SubsBeforeDelete
    BeforeInsert = ibdsM_SubsBeforeInsert
    BeforePost = ibdsM_EventsBeforePost
    OnPostError = ibdsM_SubsPostError
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_SUBS'
      ' where'
      '  MEMBER = :OLD_MEMBER'
      'and'
      '    SUBSCRIPTION = :OLD_SUBSCRIPTION ')
    InsertSQL.Strings = (
      'insert into M_SUBS'
      
        '  (MEMBER, SUBSCRIPTION, AMOUNT_PAID, AMOUNT, PAID, EDIT_DATE,RE' +
        'CEIPT)'
      'values'
      
        '  ( :MEMBER, :SUBSCRIPTION, :AMOUNT_PAID, :AMOUNT, :PAID, :EDIT_' +
        'DATE,:RECEIPT)')
    RefreshSQL.Strings = (
      
        'SELECT ms.MEMBER ,ms.SUBSCRIPTION,ms.AMOUNT_PAID, ms.AMOUNT,ms.P' +
        'AID,ms.EDIT_DATE,ms.RECEIPT,s.SUB_GROUPS,'
      '(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE            '
      'FROM  M_SUBS ms, SUBSCRIPTIONS s'
      'WHERE ms.MEMBER =  :Member'
      ''
      'and s.SUBSCRIPTION =ms.SUBSCRIPTION'
      'ORDER by s.SUB_GROUPS,ms.SUBSCRIPTION'
      ''
      '')
    SelectSQL.Strings = (
      
        'SELECT ms.MEMBER ,ms.SUBSCRIPTION,ms.AMOUNT_PAID, ms.AMOUNT,ms.P' +
        'AID,ms.EDIT_DATE,ms.RECEIPT,s.SUB_GROUPS,'
      '(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE      '
      'FROM  M_SUBS ms, SUBSCRIPTIONS s'
      'WHERE ms.MEMBER =  :Member'
      'and s.SUBSCRIPTION = ms.SUBSCRIPTION'
      'ORDER by s.SUB_GROUPS,ms.SUBSCRIPTION')
    ModifySQL.Strings = (
      'update M_SUBS'
      'set'
      '  MEMBER = :MEMBER,'
      '  SUBSCRIPTION = :SUBSCRIPTION,'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  AMOUNT = :AMOUNT,'
      '  PAID = :PAID,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  RECEIPT = :RECEIPT'
      'where'
      '  MEMBER = :MEMBER'
      'and'
      '    SUBSCRIPTION = :SUBSCRIPTION ')
    DataSource = dsrMembers
    Left = 24
    Top = 168
    object ibdsM_SubsMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 10
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsM_SubsSUBSCRIPTION: TIntegerField
      DisplayLabel = 'Subscription'
      DisplayWidth = 13
      FieldName = 'SUBSCRIPTION'
      ReadOnly = True
      Required = True
    end
    object ibdsM_SubsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 30
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = tblSubs
      LookupKeyFields = 'SUBSCRIPTION'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'SUBSCRIPTION'
      LookupCache = True
      Size = 30
      Lookup = True
    end
    object ibdsM_SubsRECEIPT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 8
      FieldName = 'RECEIPT'
    end
    object ibdsM_SubsAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 12
      FieldName = 'AMOUNT_PAID'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsM_SubsAMOUNT: TIBBCDField
      DisplayLabel = 'Amount'
      DisplayWidth = 10
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsM_SubsEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
    object ibdsM_SubsPAID: TIBStringField
      FieldName = 'PAID'
      Visible = False
      Size = 1
    end
    object ibdsM_SubsSUB_GROUPS: TIntegerField
      FieldName = 'SUB_GROUPS'
    end
    object ibdsM_SubsAMOUNT_DUE: TIBBCDField
      FieldName = 'AMOUNT_DUE'
      currency = True
      Precision = 18
      Size = 2
    end
    object ibdsM_SubsCODE: TStringField
      FieldKind = fkLookup
      FieldName = 'CODE'
      LookupDataSet = tblSubs
      LookupKeyFields = 'SUBSCRIPTION'
      LookupResultField = 'CODE'
      KeyFields = 'SUBSCRIPTION'
      Size = 6
      Lookup = True
    end
  end
  object ibdsM_Events: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_EventsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsM_EventsBeforeDelete
    BeforeEdit = ibdsM_EventsBeforeEdit
    BeforePost = ibdsM_EventsBeforePost
    OnPostError = ibdsM_EventsPostError
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_EVENTS'
      'where'
      '  MEMBER = :OLD_MEMBER'
      'and'
      '  EVENT = :OLD_EVENT'
      '')
    InsertSQL.Strings = (
      'insert into M_EVENTS'
      '  ( MEMBER, EVENT, DATES, DATE_LAST, EDIT_DATE)'
      'values'
      '  ( :MEMBER, :EVENT, :DATES, :DATE_LAST, :EDIT_DATE)')
    RefreshSQL.Strings = (
      'Select '
      '   MEMBER,'
      '  EVENT,'
      '  DATES,'
      '  DATE_LAST,'
      '  EDIT_DATE'
      'from M_EVENTS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'SELECT EVENT,DATES, MEMBER, DATE_LAST, EDIT_DATE  FROM  M_EVENTS' +
        ' '
      'WHERE MEMBER =  :Member'
      'ORDER by EVENT')
    ModifySQL.Strings = (
      'update M_EVENTS'
      'set'
      '  MEMBER = :MEMBER,'
      '  EVENT = :EVENT,'
      '  DATES = :DATES,'
      '  DATE_LAST = :DATE_LAST,'
      '  EDIT_DATE = :EDIT_DATE'
      'where'
      '  MEMBER = :MEMBER'
      'and'
      '  EVENT = :EVENT')
    DataSource = dsrMembers
    Left = 24
    Top = 280
    object ibdsM_EventsEVENT: TIntegerField
      DisplayLabel = 'Event'
      DisplayWidth = 16
      FieldName = 'EVENT'
      ReadOnly = True
    end
    object ibdsM_EventsDESCRIPTION: TStringField
      DisplayLabel = 'Description'
      DisplayWidth = 44
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = tblEvents
      LookupKeyFields = 'EVENT'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'EVENT'
      LookupCache = True
      ReadOnly = True
      Size = 30
      Lookup = True
    end
    object ibdsM_EventsDATES: TDateField
      DisplayLabel = 'Date'
      DisplayWidth = 26
      FieldName = 'DATES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsM_EventsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      ReadOnly = True
      Visible = False
    end
    object ibdsM_EventsDATE_LAST: TDateField
      FieldName = 'DATE_LAST'
      Visible = False
    end
    object ibdsM_EventsEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
  end
  object ibdsM_Sections: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    ForcedRefresh = True
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_SectionsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsM_EventsBeforePost
    BeforePost = ibdsM_EventsBeforePost
    OnPostError = ibdsM_SectionsPostError
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_SECTIONS'
      'where'
      '  MEMBER = :OLD_MEMBER'
      'and '
      '  SECTION = :OLD_SECTION'
      '')
    InsertSQL.Strings = (
      'insert into M_SECTIONS'
      
        '  (MEMBER,  SECTION, EXPIRES, PAID, RECEIPT, SPONSORSHIP, EDIT_D' +
        'ATE, AMOUNT,AMOUNT_PAID)'
      'values'
      '  (:MEMBER, :SECTION, :EXPIRES, :PAID, :RECEIPT, :SPONSORSHIP, '
      '   :EDIT_DATE,:AMOUNT,:AMOUNT_PAID)'
      '')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  SECTION,'
      '  EXPIRES,'
      '  PAID,'
      '  RECEIPT,'
      '  SPONSORSHIP,'
      '  EDIT_DATE,'
      '  AMOUNT,'
      '  AMOUNT_PAID'
      'from M_SECTIONS '
      'where'
      '   MEMBER =  :MEMBER'
      '')
    SelectSQL.Strings = (
      'SELECT MEMBER,SECTION,EXPIRES,PAID,RECEIPT,'
      'SPONSORSHIP,EDIT_DATE,AMOUNT,AMOUNT_PAID  FROM M_SECTIONS'
      'WHERE MEMBER = :Member'
      'ORDER by SECTION')
    ModifySQL.Strings = (
      'update M_SECTIONS'
      'set'
      '  MEMBER = :MEMBER,'
      '  SECTION = :SECTION,'
      '  EXPIRES = :EXPIRES,'
      '  PAID = :PAID,'
      '  RECEIPT = :RECEIPT,'
      '  SPONSORSHIP = :SPONSORSHIP,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  AMOUNT = :AMOUNT,'
      ' AMOUNT_PAID = :AMOUNT_PAID'
      'where'
      '  MEMBER = :MEMBER'
      'and '
      '  SECTION = :SECTION')
    DataSource = dsrMembers
    Left = 24
    Top = 224
    object ibdsM_SectionsSECTION: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 9
      FieldName = 'SECTION'
      ReadOnly = True
    end
    object ibdsM_SectionsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 27
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = tblSections
      LookupKeyFields = 'SECTION'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'SECTION'
      LookupCache = True
      Size = 30
      Lookup = True
    end
    object ibdsM_SectionsEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 17
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsM_SectionsRECEIPT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 7
      FieldName = 'RECEIPT'
      ReadOnly = True
    end
    object ibdsM_SectionsAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 14
      FieldName = 'AMOUNT_PAID'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsM_SectionsAMOUNT: TIBBCDField
      DisplayLabel = 'Amount'
      DisplayWidth = 11
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsM_SectionsSPONSORSHIP: TIBStringField
      DisplayLabel = 'Spons'
      DisplayWidth = 6
      FieldName = 'SPONSORSHIP'
      Visible = False
      Size = 1
    end
    object ibdsM_SectionsPAID: TIBStringField
      DisplayWidth = 5
      FieldName = 'PAID'
      Visible = False
      Size = 1
    end
    object ibdsM_SectionsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      ReadOnly = True
      Visible = False
    end
    object ibdsM_SectionsEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
  end
  object qryPropSec: TIBQuery
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    ForcedRefresh = True
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      
        'SELECT MEMBER,FULL_NAME,SURNAME,ADDRESS,FIRST_NAME  FROM MEMBERS' +
        ' where MEMBER = -1')
    Left = 96
    Top = 56
    object qryPropSecMEMBER: TIntegerField
      DisplayWidth = 9
      FieldName = 'MEMBER'
      Required = True
    end
    object qryPropSecSURNAME: TIBStringField
      DisplayWidth = 15
      FieldName = 'SURNAME'
      Size = 25
    end
    object qryPropSecFULL_NAME: TIBStringField
      DisplayWidth = 20
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object qryPropSecADDRESS: TIBStringField
      DisplayWidth = 20
      FieldName = 'ADDRESS'
      Size = 30
    end
    object qryPropSecFIRST_NAME: TIBStringField
      DisplayWidth = 12
      FieldName = 'FIRST_NAME'
    end
  end
  object dsrPropSec: TDataSource
    DataSet = qryPropSec
    Left = 88
    Top = 69
  end
  object qryReport1: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'select * from Queries')
    Left = 76
    Top = 8
  end
  object tblSite: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    ForcedRefresh = True
    AutoCalcFields = False
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'COMP_NAME'
        Attributes = [faRequired]
        DataType = ftString
        Size = 80
      end
      item
        Name = 'ADDRESS1'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'ADDRESS2'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'ADDRESS3'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'POSTCODE'
        DataType = ftString
        Size = 8
      end
      item
        Name = 'PHONE'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'GST_NO'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'GST_RATE'
        DataType = ftFloat
      end
      item
        Name = 'SITE_LICENSE'
        DataType = ftInteger
      end
      item
        Name = 'MEMBER_LICENSE'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'POS_LICENSE'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'HELP_DESK'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'FAX'
        DataType = ftString
        Size = 20
      end>
    StoreDefs = True
    TableName = 'SITE'
    Left = 432
    Top = 112
    object tblSiteCOMP_NAME: TIBStringField
      FieldName = 'COMP_NAME'
      Size = 80
    end
    object tblSiteADDRESS1: TIBStringField
      FieldName = 'ADDRESS1'
      Size = 30
    end
    object tblSiteADDRESS2: TIBStringField
      FieldName = 'ADDRESS2'
      Size = 30
    end
    object tblSiteADDRESS3: TIBStringField
      FieldName = 'ADDRESS3'
      Size = 30
    end
    object tblSitePOSTCODE: TIBStringField
      FieldName = 'POSTCODE'
      Size = 8
    end
    object tblSitePHONE: TIBStringField
      FieldName = 'PHONE'
    end
    object tblSiteGST_NO: TIBStringField
      FieldName = 'GST_NO'
      Size = 10
    end
    object tblSiteGST_RATE: TFloatField
      FieldName = 'GST_RATE'
    end
    object tblSiteSITE_LICENSE: TIntegerField
      FieldName = 'SITE_LICENSE'
    end
    object tblSiteMEMBER_LICENSE: TIBStringField
      FieldName = 'MEMBER_LICENSE'
    end
    object tblSitePOS_LICENSE: TIBStringField
      FieldName = 'POS_LICENSE'
    end
    object tblSiteHELP_DESK: TIBStringField
      FieldName = 'HELP_DESK'
    end
    object tblSiteFAX: TIBStringField
      FieldName = 'FAX'
    end
  end
  object tblMisc: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AutoCalcFields = False
    BeforePost = tblMiscBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'MISC'
        DataType = ftSmallint
      end
      item
        Name = 'MEMBER_EXPIRY'
        DataType = ftDate
      end
      item
        Name = 'MEMBER_PRE_EXPIRY'
        DataType = ftSmallint
      end
      item
        Name = 'KEEP_INACTIVE'
        DataType = ftInteger
      end
      item
        Name = 'SUBS_ROLL'
        DataType = ftDate
      end
      item
        Name = 'LUCKY_ROLL'
        DataType = ftDate
      end
      item
        Name = 'REVOLVE_DAYS'
        DataType = ftInteger
      end
      item
        Name = 'NAME_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'M_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'M_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'M_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'A_BACKUP_TIME'
        DataType = ftTime
      end
      item
        Name = 'A_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_FILE_NAMES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_KEEP_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'MSG'
        DataType = ftString
        Size = 200
      end
      item
        Name = 'AREV_IMPORT_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CAPITALS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_CARD'
        DataType = ftSmallint
      end
      item
        Name = 'PAYMENT_RECEIPT'
        DataType = ftSmallint
      end
      item
        Name = 'DEFAULT_CARDID'
        DataType = ftSmallint
      end
      item
        Name = 'SECTION_REVOLVE_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'EXPORT_TIMER'
        DataType = ftInteger
      end
      item
        Name = 'DISCOUNT_3'
        DataType = ftBCD
        Precision = 9
        Size = 3
      end
      item
        Name = 'ANNUAL_CARD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DISABLE_SERVICES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_GRIDS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 3
      end
      item
        Name = 'CONFIRM_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_ENCODE_MEMBER'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PART_PAY_ADVANCE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'POS_PAY_CARD_REQD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BOTTOM_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'TOP_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'CHECKED_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'DEFAULT_NUMBER'
        DataType = ftSmallint
      end
      item
        Name = 'PAY_BEFORE_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'RECEIPT_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'EXPORT_SUBS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'POINTS_PASSWORD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'NUMBER_COLUMNS'
        DataType = ftSmallint
      end
      item
        Name = 'COLUMN_SPACE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'LEFT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'RIGHT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'TOP_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BOTTOM_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BAND_HEIGHT'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'FONT_SIZE'
        DataType = ftSmallint
      end
      item
        Name = 'PRINT_MEMBER_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_MSG'
        DataType = ftBlob
        Size = 8
      end
      item
        Name = 'AREV_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'MENUMATE_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_PATH'
        DataType = ftString
        Size = 25
      end
      item
        Name = 'PHM_CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'TRIGGER_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SWIPE_TEXT'
        DataType = ftString
        Size = 62
      end
      item
        Name = 'ON_INSERT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MONDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'TUESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'WEDNESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'THURSDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'FRIDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SATURDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SUNDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BIRTHDAY_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'LUCKY_DRAW_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'KIOSK_MEMBER_START'
        DataType = ftSmallint
      end
      item
        Name = 'KIOSK_MEMBER_LENGHT'
        DataType = ftSmallint
      end
      item
        Name = 'BIRTHDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'VISIT_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'BIRTHDAY_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'LUCKY_DRAW_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'MONDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'TUESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'WEDNESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'THURSDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'FRIDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SATURDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SUNDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'KNOWN_AS_DEFAULTED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PARENT_CODE_ADDRESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'GROUP_DELETE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MENUMATE_POINTS_DATE'
        DataType = ftDate
      end
      item
        Name = 'KIOSK_DRAW_DATE'
        DataType = ftDate
      end
      item
        Name = 'KIOSK_SUCCESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_ERROR_MSG'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'KIOSK_DRAWS'
        DataType = ftSmallint
      end
      item
        Name = 'PRINT_RECEIPT_AS_TEXT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SWIPE_LENGTH'
        DataType = ftSmallint
      end
      item
        Name = 'POINTS_AS_DOLLARS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SHOW_DICE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CHECK_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'VIP_CREDIT_NAME'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'KIOSK_TIMER'
        DataType = ftSmallint
      end
      item
        Name = 'CHECK_SUBS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CHECK_GROUPS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MAX_SUB'
        DataType = ftSmallint
      end
      item
        Name = 'MAX_GROUP'
        DataType = ftSmallint
      end
      item
        Name = 'MIN_SUB'
        DataType = ftSmallint
      end
      item
        Name = 'MIN_GROUP'
        DataType = ftSmallint
      end
      item
        Name = 'CLEAR_INTERESTS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY21'
        Fields = 'MISC'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'MISC'
    Left = 376
    Top = 56
    object tblMiscMEMBER_EXPIRY: TDateField
      FieldName = 'MEMBER_EXPIRY'
      DisplayFormat = 'dd mmm yyyy'
    end
    object tblMiscMEMBER_PRE_EXPIRY: TSmallintField
      FieldName = 'MEMBER_PRE_EXPIRY'
    end
    object tblMiscKEEP_INACTIVE: TIntegerField
      FieldName = 'KEEP_INACTIVE'
    end
    object tblMiscSUBS_ROLL: TDateField
      FieldName = 'SUBS_ROLL'
    end
    object tblMiscLUCKY_ROLL: TDateField
      FieldName = 'LUCKY_ROLL'
    end
    object tblMiscREVOLVE_DAYS: TIntegerField
      FieldName = 'REVOLVE_DAYS'
    end
    object tblMiscNAME_FIELDS: TIBStringField
      FieldName = 'NAME_FIELDS'
      Size = 45
    end
    object tblMiscM_BACKUP_ENABLED: TIBStringField
      FieldName = 'M_BACKUP_ENABLED'
      Size = 1
    end
    object tblMiscM_BACKUP_DIR: TIBStringField
      FieldName = 'M_BACKUP_DIR'
      Size = 50
    end
    object tblMiscM_BACKUP_PCNAME: TIBStringField
      FieldName = 'M_BACKUP_PCNAME'
      Size = 50
    end
    object tblMiscM_BACKUP_DATE: TDateField
      FieldName = 'M_BACKUP_DATE'
    end
    object tblMiscM_BACKUP_TYPE: TSmallintField
      FieldName = 'M_BACKUP_TYPE'
    end
    object tblMiscA_BACKUP_ENABLED: TIBStringField
      FieldName = 'A_BACKUP_ENABLED'
      Size = 1
    end
    object tblMiscA_BACKUP_DIR: TIBStringField
      FieldName = 'A_BACKUP_DIR'
      Size = 50
    end
    object tblMiscA_BACKUP_PCNAME: TIBStringField
      FieldName = 'A_BACKUP_PCNAME'
      Size = 50
    end
    object tblMiscA_BACKUP_DATE: TDateField
      FieldName = 'A_BACKUP_DATE'
    end
    object tblMiscA_BACKUP_TIME: TTimeField
      FieldName = 'A_BACKUP_TIME'
    end
    object tblMiscA_BACKUP_TYPE: TSmallintField
      FieldName = 'A_BACKUP_TYPE'
    end
    object tblMiscA_BACKUP_FILE_NAMES: TIBStringField
      FieldName = 'A_BACKUP_FILE_NAMES'
      Size = 1
    end
    object tblMiscMISC: TSmallintField
      FieldName = 'MISC'
    end
    object tblMiscA_BACKUP_KEEP_DAYS: TSmallintField
      FieldName = 'A_BACKUP_KEEP_DAYS'
    end
    object tblMiscMSG: TIBStringField
      FieldName = 'MSG'
      Size = 200
    end
    object tblMiscAREV_IMPORT_FILE_PATH: TIBStringField
      FieldName = 'AREV_IMPORT_FILE_PATH'
      Size = 40
    end
    object tblMiscCONTROLLER: TIBStringField
      FieldName = 'CONTROLLER'
      Size = 40
    end
    object tblMiscCAPITALS: TIBStringField
      FieldName = 'CAPITALS'
      Size = 1
    end
    object tblMiscDEFAULT_CARDID: TSmallintField
      FieldName = 'DEFAULT_CARDID'
    end
    object tblMiscSECTION_REVOLVE_DAYS: TSmallintField
      FieldName = 'SECTION_REVOLVE_DAYS'
    end
    object tblMiscANNUAL_CARD: TIBStringField
      DisplayWidth = 5
      FieldName = 'ANNUAL_CARD'
      Size = 1
    end
    object tblMiscDISABLE_SERVICES: TIBStringField
      FieldName = 'DISABLE_SERVICES'
      Size = 1
    end
    object tblMiscPAYMENT_GRIDS: TIBStringField
      FieldName = 'PAYMENT_GRIDS'
      Size = 3
    end
    object tblMiscCONFIRM_PAYMENT: TIBStringField
      FieldName = 'CONFIRM_PAYMENT'
      Size = 1
    end
    object tblMiscCARD_ENCODE_MEMBER: TIBStringField
      FieldName = 'CARD_ENCODE_MEMBER'
      Size = 1
    end
    object tblMiscPAYMENT_CARD: TSmallintField
      FieldName = 'PAYMENT_CARD'
    end
    object tblMiscPAYMENT_RECEIPT: TSmallintField
      FieldName = 'PAYMENT_RECEIPT'
    end
    object tblMiscPART_PAY_ADVANCE: TIBStringField
      FieldName = 'PART_PAY_ADVANCE'
      Size = 1
    end
    object tblMiscPOS_PAY_CARD_REQD: TIBStringField
      FieldName = 'POS_PAY_CARD_REQD'
      Size = 1
    end
    object tblMiscBOTTOM_LINE_FEED: TSmallintField
      FieldName = 'BOTTOM_LINE_FEED'
    end
    object tblMiscTOP_LINE_FEED: TSmallintField
      FieldName = 'TOP_LINE_FEED'
    end
    object tblMiscPAY_BEFORE_EXPIRES: TIBStringField
      FieldName = 'PAY_BEFORE_EXPIRES'
      Size = 1
    end
    object tblMiscCARD_DEFAULT: TIBStringField
      FieldName = 'CARD_DEFAULT'
      Size = 1
    end
    object tblMiscRECEIPT_DEFAULT: TIBStringField
      FieldName = 'RECEIPT_DEFAULT'
      Size = 1
    end
    object tblMiscEXPORT_SUBS: TIBStringField
      FieldName = 'EXPORT_SUBS'
      FixedChar = True
      Size = 1
    end
    object tblMiscEXPORT_TIMER: TIntegerField
      FieldName = 'EXPORT_TIMER'
    end
    object tblMiscPRINT_MEMBER_PAYMENT: TIBStringField
      FieldName = 'PRINT_MEMBER_PAYMENT'
      Size = 1
    end
    object tblMiscAREV_FILE_PATH: TIBStringField
      FieldName = 'AREV_FILE_PATH'
      Size = 40
    end
    object tblMiscPHM_INSTALLED: TIBStringField
      FieldName = 'PHM_INSTALLED'
      Size = 1
    end
    object tblMiscPHM_PATH: TIBStringField
      DisplayWidth = 40
      FieldName = 'PHM_PATH'
      Size = 40
    end
    object tblMiscPHM_CONTROLLER: TIBStringField
      FieldName = 'PHM_CONTROLLER'
      Size = 40
    end
    object tblMiscKIOSK_MSG: TBlobField
      FieldName = 'KIOSK_MSG'
      BlobType = ftBlob
      Size = 8
    end
    object tblMiscMONDAY_PRINT: TIBStringField
      FieldName = 'MONDAY_PRINT'
      Size = 1
    end
    object tblMiscTUESDAY_PRINT: TIBStringField
      FieldName = 'TUESDAY_PRINT'
      Size = 1
    end
    object tblMiscWEDNESDAY_PRINT: TIBStringField
      FieldName = 'WEDNESDAY_PRINT'
      Size = 1
    end
    object tblMiscTHURSDAY_PRINT: TIBStringField
      FieldName = 'THURSDAY_PRINT'
      Size = 1
    end
    object tblMiscFRIDAY_PRINT: TIBStringField
      FieldName = 'FRIDAY_PRINT'
      Size = 1
    end
    object tblMiscSATURDAY_PRINT: TIBStringField
      FieldName = 'SATURDAY_PRINT'
      Size = 1
    end
    object tblMiscSUNDAY_PRINT: TIBStringField
      FieldName = 'SUNDAY_PRINT'
      Size = 1
    end
    object tblMiscBIRTHDAY_TEXT: TIBStringField
      FieldName = 'BIRTHDAY_TEXT'
      Size = 35
    end
    object tblMiscLUCKY_DRAW_TEXT: TIBStringField
      FieldName = 'LUCKY_DRAW_TEXT'
      Size = 35
    end
    object tblMiscBIRTHDAY_PRINT: TIBStringField
      FieldName = 'BIRTHDAY_PRINT'
      Size = 1
    end
    object tblMiscMONDAY_END_TIME: TTimeField
      FieldName = 'MONDAY_END_TIME'
    end
    object tblMiscTUESDAY_END_TIME: TTimeField
      FieldName = 'TUESDAY_END_TIME'
    end
    object tblMiscWEDNESDAY_END_TIME: TTimeField
      FieldName = 'WEDNESDAY_END_TIME'
    end
    object tblMiscTHURSDAY_END_TIME: TTimeField
      FieldName = 'THURSDAY_END_TIME'
    end
    object tblMiscFRIDAY_END_TIME: TTimeField
      FieldName = 'FRIDAY_END_TIME'
    end
    object tblMiscSATURDAY_END_TIME: TTimeField
      FieldName = 'SATURDAY_END_TIME'
    end
    object tblMiscSUNDAY_END_TIME: TTimeField
      FieldName = 'SUNDAY_END_TIME'
    end
    object tblMiscVISIT_MSG: TIBStringField
      FieldName = 'VISIT_MSG'
      Size = 70
    end
    object tblMiscBIRTHDAY_MSG: TIBStringField
      FieldName = 'BIRTHDAY_MSG'
      Size = 70
    end
    object tblMiscLUCKY_DRAW_MSG: TIBStringField
      FieldName = 'LUCKY_DRAW_MSG'
      Size = 70
    end
    object tblMiscPARENT_CODE_ADDRESS: TIBStringField
      FieldName = 'PARENT_CODE_ADDRESS'
      Size = 1
    end
    object tblMiscGROUP_DELETE: TIBStringField
      FieldName = 'GROUP_DELETE'
      Size = 1
    end
    object tblMiscKNOWN_AS_DEFAULTED: TIBStringField
      FieldName = 'KNOWN_AS_DEFAULTED'
      FixedChar = True
      Size = 1
    end
    object tblMiscKIOSK_DRAWS: TSmallintField
      FieldName = 'KIOSK_DRAWS'
    end
    object tblMiscKIOSK_DRAW_DATE: TDateField
      FieldName = 'KIOSK_DRAW_DATE'
    end
    object tblMiscKIOSK_SUCCESS: TIBStringField
      FieldName = 'KIOSK_SUCCESS'
      Size = 1
    end
    object tblMiscKIOSK_ERROR_MSG: TIBStringField
      FieldName = 'KIOSK_ERROR_MSG'
      Size = 30
    end
    object tblMiscPRINT_RECEIPT_AS_TEXT: TIBStringField
      FieldName = 'PRINT_RECEIPT_AS_TEXT'
      Size = 1
    end
    object tblMiscVIP_CREDIT_NAME: TIBStringField
      FieldName = 'VIP_CREDIT_NAME'
    end
    object tblMiscCHECK_SUBS: TIBStringField
      FieldName = 'CHECK_SUBS'
      FixedChar = True
      Size = 1
    end
    object tblMiscCHECK_GROUPS: TIBStringField
      FieldName = 'CHECK_GROUPS'
      FixedChar = True
      Size = 1
    end
    object tblMiscMAX_SUB: TSmallintField
      FieldName = 'MAX_SUB'
    end
    object tblMiscMAX_GROUP: TSmallintField
      FieldName = 'MAX_GROUP'
    end
    object tblMiscMIN_SUB: TSmallintField
      FieldName = 'MIN_SUB'
    end
    object tblMiscMIN_GROUP: TSmallintField
      FieldName = 'MIN_GROUP'
    end
    object tblMiscCLEAR_INTERESTS: TIBStringField
      FieldName = 'CLEAR_INTERESTS'
      Size = 1
    end
  end
  object dsrTblMisc: TDataSource
    DataSet = tblMisc
    Left = 356
    Top = 64
  end
  object dsrTblSite: TDataSource
    DataSet = tblSite
    Left = 432
    Top = 126
  end
  object dsrBackupLog: TDataSource
    DataSet = qryBackupLog
    Left = 373
    Top = 10
  end
  object dsrUserSecView: TDataSource
    DataSet = tblUserSecView
    Left = 435
    Top = 184
  end
  object tblUserSecView: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'USER_CODE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'SCREEN_CODE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'HAS_ACCESS'
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY19'
        Fields = 'USER_CODE;SCREEN_CODE'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'USER_SEC'
    Left = 435
    Top = 172
    object tblUserSecViewUSER_CODE: TIntegerField
      DisplayLabel = 'User Code'
      DisplayWidth = 7
      FieldName = 'USER_CODE'
    end
    object tblUserSecViewSCREEN_CODE: TIntegerField
      DisplayLabel = 'Screen Code'
      DisplayWidth = 10
      FieldName = 'SCREEN_CODE'
    end
    object tblUserSecViewSCREEN_DESC: TStringField
      DisplayLabel = 'Screen Desc'
      DisplayWidth = 41
      FieldKind = fkLookup
      FieldName = 'SCREEN_DESC'
      LookupDataSet = qryReport1
      LookupKeyFields = 'SCREEN_CODE'
      LookupResultField = 'SCREEN_DESC'
      KeyFields = 'SCREEN_CODE'
      Size = 30
      Lookup = True
    end
    object tblUserSecViewHAS_ACCESS: TIBStringField
      DisplayLabel = 'Access'
      DisplayWidth = 6
      FieldName = 'HAS_ACCESS'
      Size = 1
    end
  end
  object qryMembersPAY: TIBQuery
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'select SURNAME,MEMBER,FULL_NAME,FIRST_NAME,ADDRESS,'
      'MAILING_LIST_CODE  from MEMBERS'
      'where MEMBER = -1')
    Left = 368
    Top = 131
    object qryMembersPAYMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object qryMembersPAYSURNAME: TIBStringField
      FieldName = 'SURNAME'
      Size = 25
    end
    object qryMembersPAYFIRST_NAME: TIBStringField
      FieldName = 'FIRST_NAME'
    end
    object qryMembersPAYFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object qryMembersPAYADDRESS: TIBStringField
      FieldName = 'ADDRESS'
      Size = 30
    end
    object qryMembersPAYMAILING_LIST_CODE: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
    end
  end
  object dsrMembersPAY: TDataSource
    DataSet = qryMembersPAY
    Left = 368
    Top = 119
  end
  object qryReport: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      
        'Select BEF_BONUS_CREDIT, BEF_EARNT_CREDIT, BEF_PRE_PAID_CREDIT, ' +
        'BEF_TOTAL_EARNT, BEF_TOTAL_REDEEMED, BONUS_CREDIT, EARNT_CREDIT,' +
        ' EDIT_DATE, MEMBER, POINTS_ID, POINTS_VALUE, PRE_PAID_CREDIT, PR' +
        'OCESS, REASON, TOTAL_EARNT, TOTAL_REDEEMED, USER_CODE,'
      
        ' BEF_BONUS_CREDIT + BEF_EARNT_CREDIT+ BEF_PRE_PAID_CREDIT as BEF' +
        '_VALUE_OF_CREDIT ,'
      
        ' BONUS_CREDIT + EARNT_CREDIT+ PRE_PAID_CREDIT as VALUE_OF_CREDIT' +
        ','
      'BEF_TOTAL_EARNT - BEF_TOTAL_REDEEMED   as BEF_POINTS_AVAILABLE,'
      'TOTAL_EARNT - TOTAL_REDEEMED as POINTS_AVAILABLE'
      ''
      'from POINTS_LOG')
    Left = 114
    Top = 8
  end
  object dsrPayDet: TDataSource
    DataSet = ibdsPayDet
    Left = 304
    Top = 244
  end
  object dsrPayJoin: TDataSource
    DataSet = ibdsPayJoin
    Left = 304
    Top = 123
  end
  object ibdsPayJoin: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER'
      '')
    InsertSQL.Strings = (
      'insert into MEMBERS'
      '  (MEMBER, EXPIRES, JOINING_FEE)'
      'values'
      '  (:MEMBER, :EXPIRES, :JOINING_FEE)'
      '')
    RefreshSQL.Strings = (
      'select MEMBER,'
      'FULL_NAME,'
      'EXPIRES,'
      'JOINING_FEE,'
      'RECEIPT,'
      'PAY'
      'from MEMBERS '
      'where MEMBER = :OLDMEMBER  ')
    SelectSQL.Strings = (
      'select MEMBER,FULL_NAME,EXPIRES,JOINING_FEE, RECEIPT,PAY'
      'from MEMBERS where MEMBER in ( :MemberStr )'
      '')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '    JOINING_FEE = :JOINING_FEE,'
      '    RECEIPT = :RECEIPT,'
      '    PAY = :PAY'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 304
    Top = 136
    object ibdsPayJoinMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 9
      FieldName = 'MEMBER'
      ReadOnly = True
      Required = True
    end
    object ibdsPayJoinJOINING_FEE: TIBBCDField
      DisplayLabel = 'Joining Fee'
      DisplayWidth = 13
      FieldName = 'JOINING_FEE'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayJoinPAY: TIBBCDField
      DisplayWidth = 15
      FieldName = 'PAY'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayJoinFULL_NAME: TIBStringField
      DisplayWidth = 45
      FieldName = 'FULL_NAME'
      Visible = False
      Size = 45
    end
    object ibdsPayJoinEXPIRES: TDateField
      FieldName = 'EXPIRES'
      Visible = False
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPayJoinRECEIPT: TIntegerField
      DisplayWidth = 10
      FieldName = 'RECEIPT'
      Visible = False
    end
  end
  object ibdsPayMembers: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    AfterOpen = ibdsPayMembersAfterOpen
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      'select MEMBER,FULL_NAME,POINTS_AVAILABLE,'
      'TOTAL_REDEEMED, DISCOUNT,EXPIRES,EDIT_DATE,'
      'TOTAL_EARNT,BONUS_CREDIT,PRE_PAID_CREDIT,EARNT_CREDIT'
      'from MEMBERS '
      'where MEMBER = :member')
    SelectSQL.Strings = (
      
        'select MEMBER,FULL_NAME,POINTS_AVAILABLE,TOTAL_EARNT,BONUS_CREDI' +
        'T,PRE_PAID_CREDIT,EARNT_CREDIT,'
      'TOTAL_REDEEMED, DISCOUNT,EXPIRES,EDIT_DATE, SUB_PAID, RECEIPT'
      'from MEMBERS '
      'where MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  POINTS_AVAILABLE = :POINTS_AVAILABLE,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  SUB_PAID = :SUB_PAID,'
      '  RECEIPT = :RECEIPT,'
      '  EDIT_DATE = :EDIT_DATE'
      'where'
      '  MEMBER = :OLD_MEMBER')
    UniDirectional = True
    Left = 304
    Top = 181
    object ibdsPayMembersMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsPayMembersFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object ibdsPayMembersDISCOUNT: TIBBCDField
      FieldName = 'DISCOUNT'
      Precision = 9
      Size = 3
    end
    object ibdsPayMembersEXPIRES: TDateField
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPayMembersEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
    end
    object ibdsPayMembersSUB_PAID: TSmallintField
      FieldName = 'SUB_PAID'
    end
    object ibdsPayMembersRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
    end
    object ibdsPayMembersPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      currency = True
    end
    object ibdsPayMembersTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = 'MEMBERS.TOTAL_EARNT'
    end
    object ibdsPayMembersBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = 'MEMBERS.BONUS_CREDIT'
    end
    object ibdsPayMembersPRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = 'MEMBERS.PRE_PAID_CREDIT'
    end
    object ibdsPayMembersEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = 'MEMBERS.EARNT_CREDIT'
    end
    object ibdsPayMembersTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = 'MEMBERS.TOTAL_REDEEMED'
    end
  end
  object dsrPayMembers: TDataSource
    DataSet = ibdsPayMembers
    Left = 304
    Top = 194
  end
  object ibdsPayDet: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_SUBS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into M_SUBS'
      '  (AMOUNT_PAID,  EDIT_DATE, RECEIPT, PAID)'
      'values'
      '  (:AMOUNT_PAID,  :EDIT_DATE, :RECEIPT, :PAID)')
    RefreshSQL.Strings = (
      
        'select M.MEMBER,M.FULL_NAME,M.EXPIRES,MS.SUBSCRIPTION,MS.AMOUNT_' +
        'PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,'
      
        'S.DESCRIPTION, MS.EDIT_DATE,MS.RECEIPT,MS.PAID,MS.PAY,M.SUB_PAID' +
        ',M.MAILING_LIST_CODE,'
      
        'MS.PAID,M.AUTO_PAYMENT,m.PAYMENT_PERIOD,s.PRO_RATA,s.DISCOUNTABL' +
        'E,m.SUB_PAYMENT'
      
        ',ms.PRINT_CARD,ms.PRINT_RECEIPT,ms.INVOICED,s.SUB_GROUPS,m.DATE_' +
        'OF_BIRTH,'
      
        'm.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,m.JOINED,s.SPAR' +
        'E_CHAR'
      ' from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS'
      'where M.MEMBER = :OLDMEMBER '
      'AND S.SUBSCRIPTION = MS.SUBSCRIPTION'
      ' and M.MEMBER = MS.MEMBER'
      'order by M.MEMBER'
      '')
    SelectSQL.Strings = (
      
        'select M.MEMBER,M.FULL_NAME,M.EXPIRES,MS.SUBSCRIPTION,MS.AMOUNT_' +
        'PAID,MS.AMOUNT,M.SUB_PAID,MS.PAY,M.MAILING_LIST_CODE,'
      
        'S.DESCRIPTION,m.DATE_OF_BIRTH, MS.EDIT_DATE,MS.RECEIPT,MS.PAID,M' +
        '.AUTO_PAYMENT,m.PAYMENT_PERIOD,s.PRO_RATA,s.DISCOUNTABLE,m.SUB_P' +
        'AYMENT,ms.PRINT_CARD,ms.PRINT_RECEIPT,ms.INVOICED,s.SUB_GROUPS,m' +
        '.ADDRESS,m.ADDRESS1,m.DISTRICT,s.SPARE_CHAR,'
      'm.CITY,m.COUNTRY,m.JOINED'
      ' from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS'
      'where M.MEMBER = -1  '
      'AND S.SUBSCRIPTION = MS.SUBSCRIPTION'
      ' and M.MEMBER = MS.MEMBER  and MS.AMOUNT > 0.00'
      'order by M.MEMBER'
      ''
      '')
    ModifySQL.Strings = (
      'update M_SUBS'
      'set'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  AMOUNT = :AMOUNT,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  RECEIPT = :RECEIPT,'
      '  PAID = :PAID,'
      '  PAY = :PAY,'
      '  PRINT_CARD = :PRINT_CARD,'
      '  PRINT_RECEIPT = :PRINT_RECEIPT ,'
      '  INVOICED = :INVOICED'
      'where'
      '  MEMBER = :OLD_MEMBER'
      'and SUBSCRIPTION = :OLD_SUBSCRIPTION')
    Left = 304
    Top = 234
    object ibdsPayDetMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      ReadOnly = True
      Required = True
    end
    object ibdsPayDetSUBSCRIPTION: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 6
      FieldName = 'SUBSCRIPTION'
      ReadOnly = True
    end
    object ibdsPayDetDESCRIPTION: TIBStringField
      DisplayLabel = 'Subscription'
      DisplayWidth = 32
      FieldName = 'DESCRIPTION'
      ReadOnly = True
      Size = 60
    end
    object ibdsPayDetEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 13
      FieldName = 'EXPIRES'
      ReadOnly = True
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPayDetAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 10
      FieldName = 'AMOUNT_PAID'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayDetAMOUNT: TIBBCDField
      DisplayLabel = 'Fee'
      DisplayWidth = 9
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayDetPAY: TIBBCDField
      DisplayWidth = 9
      FieldName = 'PAY'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayDetPRINT_CARD: TIBStringField
      DisplayLabel = 'Card'
      DisplayWidth = 4
      FieldName = 'PRINT_CARD'
      Size = 1
    end
    object ibdsPayDetPRINT_RECEIPT: TIBStringField
      DisplayLabel = 'Rcpt'
      DisplayWidth = 4
      FieldName = 'PRINT_RECEIPT'
      Size = 1
    end
    object ibdsPayDetINVOICED: TIBStringField
      DisplayLabel = 'Invoiced'
      DisplayWidth = 7
      FieldName = 'INVOICED'
      ReadOnly = True
      Size = 1
    end
    object ibdsPayDetPRO_RATA: TIBStringField
      DisplayLabel = 'Pro Rata'
      DisplayWidth = 7
      FieldName = 'PRO_RATA'
      ReadOnly = True
      Size = 1
    end
    object ibdsPayDetDISCOUNTABLE: TIBStringField
      DisplayLabel = 'Disc'
      DisplayWidth = 3
      FieldName = 'DISCOUNTABLE'
      Size = 1
    end
    object ibdsPayDetDATE_OF_BIRTH: TDateField
      DisplayLabel = 'DOB'
      DisplayWidth = 10
      FieldName = 'DATE_OF_BIRTH'
      Origin = '"MEMBERS"."DATE_OF_BIRTH"'
    end
    object ibdsPayDetADDRESS: TIBStringField
      DisplayWidth = 30
      FieldName = 'ADDRESS'
      Origin = '"MEMBERS"."ADDRESS"'
      Size = 30
    end
    object ibdsPayDetADDRESS1: TIBStringField
      DisplayWidth = 30
      FieldName = 'ADDRESS1'
      Origin = '"MEMBERS"."ADDRESS1"'
      Size = 30
    end
    object ibdsPayDetDISTRICT: TIBStringField
      DisplayWidth = 20
      FieldName = 'DISTRICT'
      Origin = '"MEMBERS"."DISTRICT"'
    end
    object ibdsPayDetCITY: TIBStringField
      DisplayWidth = 25
      FieldName = 'CITY'
      Origin = '"MEMBERS"."CITY"'
      Size = 25
    end
    object ibdsPayDetCOUNTRY: TIBStringField
      DisplayWidth = 25
      FieldName = 'COUNTRY'
      Origin = '"MEMBERS"."COUNTRY"'
      Size = 25
    end
    object ibdsPayDetSPARE_CHAR: TIBStringField
      DisplayWidth = 1
      FieldName = 'SPARE_CHAR'
      Origin = '"SUBSCRIPTIONS"."SPARE_CHAR"'
      FixedChar = True
      Size = 1
    end
    object ibdsPayDetSUB_GROUPS: TIntegerField
      DisplayWidth = 10
      FieldName = 'SUB_GROUPS'
      Visible = False
    end
    object ibdsPayDetAUTO_PAYMENT: TIBStringField
      DisplayWidth = 1
      FieldName = 'AUTO_PAYMENT'
      Visible = False
      Size = 1
    end
    object ibdsPayDetPAYMENT_PERIOD: TSmallintField
      DisplayWidth = 10
      FieldName = 'PAYMENT_PERIOD'
      Visible = False
    end
    object ibdsPayDetSUB_PAYMENT: TIBStringField
      DisplayWidth = 1
      FieldName = 'SUB_PAYMENT'
      Visible = False
      Size = 1
    end
    object ibdsPayDetEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
    object ibdsPayDetRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
      Visible = False
    end
    object ibdsPayDetPAID: TIBStringField
      FieldName = 'PAID'
      Visible = False
      Size = 1
    end
    object ibdsPayDetFULL_NAME: TIBStringField
      DisplayWidth = 45
      FieldName = 'FULL_NAME'
      Visible = False
      Size = 45
    end
    object ibdsPayDetSUB_PAID: TSmallintField
      DisplayWidth = 10
      FieldName = 'SUB_PAID'
      Visible = False
    end
    object ibdsPayDetMAILING_LIST_CODE: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
      Visible = False
    end
    object ibdsPayDetJOINED: TDateField
      FieldName = 'JOINED'
      Origin = '"MEMBERS"."JOINED"'
      Visible = False
    end
  end
  object IBTransPay: TIBTransaction
    Active = False
    DefaultDatabase = IBDatabase
    DefaultAction = TARollback
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 304
    Top = 341
  end
  object sqlTransPay: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    Transaction = IBTransPay
    Left = 440
    Top = 342
  end
  object dsrPayments: TDataSource
    DataSet = ibdsPAyments
    Left = 312
    Top = 77
  end
  object dsrPay_Det: TDataSource
    DataSet = ibdsPay_Det
    Left = 312
    Top = 7
  end
  object ibdsTenders: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      'select TENDER, TENDER_TYPE, EX_RATE'
      'from TENDERS'
      'WHERE TENDER =  :OLDTENDER')
    SelectSQL.Strings = (
      'select TENDER, TENDER_TYPE, EX_RATE, '
      #39'          '#39'  as AMOUNT,'
      #39'                              '#39' as DETAILS '
      'from TENDERS'
      'order by TENDER')
    ModifySQL.Strings = (
      'update TENDERS'
      'SET  EX_RATE = :EX_RATE'
      'where TENDER = -1'
      '')
    Left = 440
    Top = 232
    object ibdsTendersTENDER_TYPE: TIBStringField
      DisplayLabel = 'Tender'
      DisplayWidth = 15
      FieldName = 'TENDER_TYPE'
      ReadOnly = True
    end
    object ibdsTendersEX_RATE: TIBBCDField
      DisplayLabel = 'Exch Rate'
      DisplayWidth = 10
      FieldName = 'EX_RATE'
      ReadOnly = True
      Precision = 9
      Size = 3
    end
    object ibdsTendersAMOUNT: TIBStringField
      DisplayLabel = 'Amount'
      DisplayWidth = 12
      FieldName = 'AMOUNT'
      Size = 10
    end
    object ibdsTendersDETAILS: TIBStringField
      Alignment = taRightJustify
      DisplayLabel = 'Details'
      DisplayWidth = 26
      FieldName = 'DETAILS'
      Size = 30
    end
    object ibdsTendersTENDER: TIntegerField
      DisplayWidth = 10
      FieldName = 'TENDER'
      ReadOnly = True
      Required = True
      Visible = False
    end
  end
  object dsrTenders: TDataSource
    DataSet = ibdsTenders
    Left = 440
    Top = 245
  end
  object qryPayTend: TIBQuery
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      
        'select PAYMENT_TEND, PAYMENT, TENDER_TYPE, DETAILS, AMOUNT, EX_R' +
        'ATE from PAYMENTS_TEND'
      'where PAYMENT = :PAYMENT')
    Left = 368
    Top = 232
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'PAYMENT'
        ParamType = ptUnknown
      end>
    object qryPayTendPAYMENT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 10
      FieldName = 'PAYMENT'
    end
    object qryPayTendTENDER_TYPE: TIBStringField
      DisplayLabel = 'Tender'
      DisplayWidth = 20
      FieldName = 'TENDER_TYPE'
    end
    object qryPayTendAMOUNT: TIBBCDField
      DisplayLabel = 'Amount'
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object qryPayTendDETAILS: TIBStringField
      DisplayLabel = 'Details'
      DisplayWidth = 30
      FieldName = 'DETAILS'
      Size = 30
    end
    object qryPayTendEX_RATE: TIBBCDField
      DisplayLabel = 'Exch Rate'
      FieldName = 'EX_RATE'
      DisplayFormat = '##.00'
      Precision = 9
      Size = 3
    end
    object qryPayTendPAYMENT_TEND: TIntegerField
      DisplayWidth = 10
      FieldName = 'PAYMENT_TEND'
      Required = True
      Visible = False
    end
  end
  object sqlRollover: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    SQL.Strings = (
      '')
    Transaction = IBRollover
    Left = 485
    Top = 3
  end
  object IBRollover: TIBTransaction
    Active = False
    DefaultDatabase = IBDatabase
    DefaultAction = TARollback
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 528
    Top = 8
  end
  object qryReport2: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'SELECT *  FROM MEMBERS')
    Left = 13
    object qryReport2MEMBER: TIntegerField
      DisplayLabel = 'Member'
      FieldName = 'MEMBER'
      Required = True
    end
    object qryReport2FULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object qryReport2ADDRESS: TIBStringField
      DisplayLabel = 'Address'
      FieldName = 'ADDRESS'
      Size = 30
    end
    object qryReport2ADDRESS1: TIBStringField
      DisplayLabel = 'Address1'
      FieldName = 'ADDRESS1'
      Size = 30
    end
    object qryReport2HOME_PHONE: TIBStringField
      DisplayLabel = 'Home Phone'
      FieldName = 'HOME_PHONE'
      Size = 14
    end
    object qryReport2FIRST_NAME: TIBStringField
      DisplayLabel = 'First Name'
      FieldName = 'FIRST_NAME'
    end
    object qryReport2IS_ACTIVE: TIBStringField
      DisplayLabel = 'Active'
      FieldName = 'IS_ACTIVE'
      Size = 1
    end
    object qryReport2KNOWN_AS: TIBStringField
      DisplayLabel = 'Known As'
      FieldName = 'KNOWN_AS'
      Size = 15
    end
    object qryReport2MALE_FEMALE: TIBStringField
      DisplayLabel = 'Gender'
      FieldName = 'MALE_FEMALE'
      Size = 1
    end
    object qryReport2MAILING_LIST: TIBStringField
      DisplayLabel = 'Mailing List'
      FieldName = 'MAILING_LIST'
      Size = 1
    end
    object qryReport2MAILING_CODE: TIntegerField
      DisplayLabel = 'Mailing Code'
      FieldName = 'MAILING_CODE'
    end
    object qryReport2MAILING_LIST_CODE: TIntegerField
      DisplayLabel = 'Mailing List Code'
      FieldName = 'MAILING_LIST_CODE'
    end
    object qryReport2NEWSLETTER: TIBStringField
      DisplayLabel = 'News Letter'
      FieldName = 'NEWSLETTER'
      Size = 1
    end
    object qryReport2EMAIL: TIBStringField
      DisplayLabel = 'Email'
      FieldName = 'EMAIL'
      Size = 60
    end
    object qryReport2WORK_PHONE: TIBStringField
      DisplayLabel = 'Work Phone'
      FieldName = 'WORK_PHONE'
      Size = 14
    end
    object qryReport2CELL_PHONE: TIBStringField
      DisplayLabel = 'Cell Phone'
      FieldName = 'CELL_PHONE'
      Size = 14
    end
    object qryReport2PROPOSER: TIntegerField
      DisplayLabel = 'Proposer'
      FieldName = 'PROPOSER'
    end
    object qryReport2SECONDER: TIntegerField
      DisplayLabel = 'Seconder'
      FieldName = 'SECONDER'
    end
    object qryReport2SUB_PAID: TSmallintField
      DisplayLabel = 'Sub Paid'
      FieldName = 'SUB_PAID'
    end
    object qryReport2RECEIPT: TIntegerField
      DisplayLabel = 'Receipt'
      FieldName = 'RECEIPT'
    end
    object qryReport2TRANS_FROM: TIBStringField
      DisplayLabel = 'Transferred From'
      FieldName = 'TRANS_FROM'
      Size = 40
    end
    object qryReport2ALLOW_CREDIT: TIBStringField
      DisplayLabel = 'Allow Credit'
      FieldName = 'ALLOW_CREDIT'
      Size = 1
    end
    object qryReport2PRICE_LEVEL: TIntegerField
      DisplayLabel = 'Price Level'
      FieldName = 'PRICE_LEVEL'
    end
    object qryReport2VIP_PAY_ONLY: TIBStringField
      DisplayLabel = 'VIP Pay Only'
      FieldName = 'VIP_PAY_ONLY'
      Size = 1
    end
    object qryReport2CARD_TYPE: TIBStringField
      DisplayLabel = 'Card Type'
      FieldName = 'CARD_TYPE'
      Size = 1
    end
    object qryReport2GL_ACCOUNT: TIBStringField
      DisplayLabel = 'GL Account'
      FieldName = 'GL_ACCOUNT'
      Size = 17
    end
    object qryReport2INCL_EOD: TIBStringField
      DisplayLabel = 'Include in EOD'
      FieldName = 'INCL_EOD'
      Size = 1
    end
    object qryReport2POSITIONS: TIBStringField
      DisplayLabel = 'Positions'
      FieldName = 'POSITIONS'
      Size = 15
    end
    object qryReport2CHARGE_CODE: TIntegerField
      DisplayLabel = 'Charge Code'
      FieldName = 'CHARGE_CODE'
    end
    object qryReport2DECEASED: TIBStringField
      DisplayLabel = 'Deceased'
      FieldName = 'DECEASED'
      Size = 1
    end
    object qryReport2SENIOR: TIBStringField
      DisplayLabel = 'Senior'
      FieldName = 'SENIOR'
      Size = 1
    end
    object qryReport2FOUNDATION: TIBStringField
      DisplayLabel = 'Foundation'
      FieldName = 'FOUNDATION'
      Size = 1
    end
    object qryReport2DONATED: TIBStringField
      DisplayLabel = 'Donated'
      FieldName = 'DONATED'
      Size = 1
    end
    object qryReport2REFUNDED: TIBStringField
      DisplayLabel = 'Refunded'
      FieldName = 'REFUNDED'
      Size = 1
    end
    object qryReport2DEBENTURE: TIntegerField
      DisplayLabel = 'Debenture'
      FieldName = 'DEBENTURE'
    end
    object qryReport2SAVED: TIBStringField
      DisplayLabel = 'New Member'
      FieldName = 'SAVED'
      Size = 1
    end
    object qryReport2TITLE: TIBStringField
      DisplayLabel = 'Title'
      FieldName = 'TITLE'
      Size = 6
    end
    object qryReport2INITIALS: TIBStringField
      DisplayLabel = 'Initials'
      FieldName = 'INITIALS'
      Size = 6
    end
    object qryReport2SURNAME: TIBStringField
      DisplayLabel = 'Surname'
      FieldName = 'SURNAME'
      Size = 25
    end
    object qryReport2ANALYSE: TIBStringField
      DisplayLabel = 'Analyse'
      FieldName = 'ANALYSE'
      Size = 1
    end
    object qryReport2DISTRICT: TIBStringField
      DisplayLabel = 'District'
      FieldName = 'DISTRICT'
    end
    object qryReport2DAIRY_NOTES: TBlobField
      DisplayLabel = 'Dairy Notes'
      FieldName = 'DAIRY_NOTES'
      BlobType = ftBlob
      Size = 8
    end
    object qryReport2EDIT_DATE: TDateTimeField
      DisplayLabel = 'Edit Date'
      FieldName = 'EDIT_DATE'
    end
    object qryReport2DATE_OF_BIRTH: TDateField
      DisplayLabel = 'Date of Birth'
      FieldName = 'DATE_OF_BIRTH'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2LUCKY_DATE: TDateField
      DisplayLabel = 'Lucky Date'
      FieldName = 'LUCKY_DATE'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2JOINED: TDateField
      DisplayLabel = 'Joined'
      FieldName = 'JOINED'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2APPROVED: TDateField
      DisplayLabel = 'Approved'
      FieldName = 'APPROVED'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2EXPIRES: TDateField
      DisplayLabel = 'Expires'
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2INACTIVE_DATE: TDateField
      DisplayLabel = 'Inactive Date'
      FieldName = 'INACTIVE_DATE'
      DisplayFormat = 'dd mmm yyyy'
    end
    object qryReport2JOINING_FEE: TIBBCDField
      DisplayLabel = 'Joining Fee'
      FieldName = 'JOINING_FEE'
      currency = True
      Precision = 9
      Size = 2
    end
    object qryReport2AMOUNT_DONATED: TIBBCDField
      DisplayLabel = 'Amount Donated'
      FieldName = 'AMOUNT_DONATED'
      currency = True
      Precision = 9
      Size = 3
    end
    object qryReport2DISCOUNT: TIBBCDField
      DisplayLabel = 'Discount'
      FieldName = 'DISCOUNT'
      Precision = 9
      Size = 3
    end
    object qryReport2CITY: TIBStringField
      DisplayLabel = 'City'
      FieldName = 'CITY'
      Size = 25
    end
    object qryReport2COUNTRY: TIBStringField
      DisplayLabel = 'Country'
      FieldName = 'COUNTRY'
      Size = 25
    end
    object qryReport2FAX_NUMBER: TIBStringField
      DisplayLabel = 'Fax Home'
      FieldName = 'FAX_NUMBER'
      Size = 14
    end
    object qryReport2SECTION_PAID: TSmallintField
      DisplayLabel = 'Section Paid'
      FieldName = 'SECTION_PAID'
    end
    object qryReport2PAY: TIBBCDField
      DisplayLabel = 'Pay'
      FieldName = 'PAY'
      Precision = 9
      Size = 2
    end
    object qryReport2SUB_PAYMENT: TIBStringField
      DisplayLabel = 'Sub Paymnet'
      FieldName = 'SUB_PAYMENT'
      Size = 1
    end
    object qryReport2BARCODE_NUMBER: TIntegerField
      DisplayLabel = 'Barcode Number'
      FieldName = 'BARCODE_NUMBER'
    end
    object qryReport2SWIPE_CARD_NUMBER: TIntegerField
      DisplayLabel = 'Swipe Card Number'
      FieldName = 'SWIPE_CARD_NUMBER'
    end
    object qryReport2CARD_LAST_USED: TDateField
      DisplayLabel = 'Card Last Used'
      FieldName = 'CARD_LAST_USED'
    end
    object qryReport2AUTO_PAYMENT: TIBStringField
      DisplayLabel = 'Auto Payment'
      FieldName = 'AUTO_PAYMENT'
      Size = 1
    end
    object qryReport2RECEIVE_EMAIL: TIBStringField
      DisplayLabel = 'Receive Email'
      FieldName = 'RECEIVE_EMAIL'
      Size = 1
    end
    object qryReport2RESIDENTIAL: TIBStringField
      DisplayLabel = 'Residential'
      FieldName = 'RESIDENTIAL'
      Size = 1
    end
    object qryReport2COMPANY: TIBStringField
      DisplayLabel = 'Company'
      FieldName = 'COMPANY'
      Size = 40
    end
    object qryReport2OCCUPATION: TIBStringField
      DisplayLabel = 'Occupation'
      FieldName = 'OCCUPATION'
      Size = 30
    end
    object qryReport2BOAT_LISTING: TIntegerField
      DisplayLabel = 'Boat Listing'
      FieldName = 'BOAT_LISTING'
    end
    object qryReport2TIME: TTimeField
      DisplayLabel = 'Time'
      FieldName = 'TIME'
    end
    object qryReport2PAYMENT_PERIOD: TSmallintField
      DisplayLabel = 'Payment Period'
      FieldName = 'PAYMENT_PERIOD'
    end
    object qryReport2INVOICE_NUMBER: TIBStringField
      DisplayLabel = 'Invoice Number'
      FieldName = 'INVOICE_NUMBER'
      Size = 22
    end
    object qryReport2PRINT_CARD: TIBStringField
      DisplayLabel = 'Print Card'
      FieldName = 'PRINT_CARD'
      Size = 1
    end
    object qryReport2PRINT_RECEIPT: TIBStringField
      DisplayLabel = 'Print Receipt'
      FieldName = 'PRINT_RECEIPT'
      Size = 1
    end
    object qryReport2NUMBER_CARDS: TSmallintField
      DisplayLabel = 'Number Cards'
      FieldName = 'NUMBER_CARDS'
    end
    object qryReport2CREDIT_LIMIT: TIBBCDField
      DisplayLabel = 'Credit Limit'
      FieldName = 'CREDIT_LIMIT'
      Precision = 9
      Size = 3
    end
    object qryReport2EARN_POINTS: TIBStringField
      DisplayLabel = 'Earn Points'
      FieldName = 'EARN_POINTS'
      Size = 1
    end
    object qryReport2PRODUCT_PURCHASE: TIBBCDField
      DisplayLabel = 'Product Purchase'
      FieldName = 'PRODUCT_PURCHASE'
      Precision = 18
      Size = 3
    end
    object qryReport2CLUBS: TIntegerField
      DisplayLabel = 'Club'
      FieldName = 'CLUBS'
      Origin = 'MEMBERS.CLUBS'
    end
    object qryReport2ASSOCIATION: TIBStringField
      DisplayLabel = 'Association'
      FieldName = 'ASSOCIATION'
      Origin = 'MEMBERS.ASSOCIATION'
      FixedChar = True
      Size = 1
    end
    object qryReport2PAYMENT_DAY: TSmallintField
      DisplayLabel = 'Payment Day'
      FieldName = 'PAYMENT_DAY'
      Origin = 'MEMBERS.PAYMENT_DAY'
    end
    object qryReport2FAX_WORK: TIBStringField
      DisplayLabel = 'Fax Work'
      FieldName = 'FAX_WORK'
      Origin = 'MEMBERS.FAX_WORK'
      Size = 14
    end
    object qryReport2STREET_ADDRESS: TIBStringField
      DisplayLabel = 'Street Address'
      FieldName = 'STREET_ADDRESS'
      Origin = 'MEMBERS.STREET_ADDRESS'
      Size = 30
    end
    object qryReport2STREET_DISTRICT: TIBStringField
      DisplayLabel = 'Street District'
      FieldName = 'STREET_DISTRICT'
      Origin = 'MEMBERS.STREET_DISTRICT'
    end
    object qryReport2STREET_ADDRESS1: TIBStringField
      DisplayLabel = 'Street Address 1'
      FieldName = 'STREET_ADDRESS1'
      Origin = 'MEMBERS.STREET_ADDRESS1'
      Size = 30
    end
    object qryReport2STREET_CITY: TIBStringField
      DisplayLabel = 'Street City'
      FieldName = 'STREET_CITY'
      Origin = 'MEMBERS.STREET_CITY'
      Size = 25
    end
    object qryReport2STREET_COUNTRY: TIBStringField
      DisplayLabel = 'Street Country'
      FieldName = 'STREET_COUNTRY'
      Origin = 'MEMBERS.STREET_COUNTRY'
      Size = 25
    end
    object qryReport2PAYMENT_DISC: TIBBCDField
      FieldName = 'PAYMENT_DISC'
      Origin = '"MEMBERS"."PAYMENT_DISC"'
      Precision = 9
      Size = 2
    end
    object qryReport2BONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object qryReport2EARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
    end
    object qryReport2PRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
    end
    object qryReport2TOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object qryReport2TOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object qryReport2REASON: TIBStringField
      FieldName = 'REASON'
      Origin = '"MEMBERS"."REASON"'
    end
  end
  object dsrReport2: TwwDataSource
    DataSet = qryReport2
    Left = 32
    Top = 8
  end
  object ibdsPaySections: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransPay
    AfterEdit = ibdsPaySectionsAfterEdit
    AfterPost = ibdsPaySectionsAfterPost
    BeforePost = ibdsPaySectionsBeforePost
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      'select M.MEMBER, M.FULL_NAME,M.SURNAME,'
      'MS.SECTION,MS.AMOUNT_PAID,MS.AMOUNT,'
      '(MS.AMOUNT - MS.AMOUNT_PAID) AS AMOUNT_DUE,    '
      'MS.EDIT_DATE,MS.EXPIRES,    '
      'MS.RECEIPT,MS.PAID from MEMBERS M,  M_SECTIONS MS '
      'where MS.SECTION = :OLDSECTION  AND M.MEMBER = :OLDMEMBER')
    SelectSQL.Strings = (
      'select M.MEMBER, M.FULL_NAME,M.SURNAME,'
      
        'MS.SECTION,MS.AMOUNT_PAID,MS.AMOUNT,(MS.AMOUNT - MS.AMOUNT_PAID)' +
        ' AS AMOUNT_DUE, MS.EXPIRES,       MS.EDIT_DATE,MS.RECEIPT,MS.PAI' +
        'D from MEMBERS M,  M_SECTIONS MS '
      '          where MS.SECTION = -1  AND M.MEMBER = MS.MEMBER')
    ModifySQL.Strings = (
      'update M_SECTIONS'
      'set'
      ' EXPIRES = :EXPIRES,'
      ' AMOUNT_PAID = :AMOUNT_PAID,'
      ' EDIT_DATE = :EDIT_DATE,'
      ' RECEIPT =  :RECEIPT,'
      ' PAID = :PAID'
      ' where MEMBER = :MEMBER'
      'and SECTION = :SECTION')
    Left = 368
    Top = 328
    object ibdsPaySectionsMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      ReadOnly = True
      Required = True
    end
    object ibdsPaySectionsSURNAME: TIBStringField
      DisplayLabel = 'Surname'
      DisplayWidth = 13
      FieldName = 'SURNAME'
      ReadOnly = True
      Size = 25
    end
    object ibdsPaySectionsFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 25
      FieldName = 'FULL_NAME'
      ReadOnly = True
      Size = 45
    end
    object ibdsPaySectionsEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 14
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPaySectionsAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 9
      FieldName = 'AMOUNT_PAID'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPaySectionsAMOUNT: TIBBCDField
      DisplayLabel = 'Amount'
      DisplayWidth = 8
      FieldName = 'AMOUNT'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPaySectionsPAID: TIBStringField
      DisplayLabel = 'PAY'
      DisplayWidth = 5
      FieldName = 'PAID'
      Size = 1
    end
    object ibdsPaySectionsSECTION: TIntegerField
      FieldName = 'SECTION'
      ReadOnly = True
      Visible = False
    end
    object ibdsPaySectionsAMOUNT_DUE: TIBBCDField
      DisplayLabel = 'Amount Due'
      FieldName = 'AMOUNT_DUE'
      Visible = False
      currency = True
      Precision = 18
      Size = 2
    end
    object ibdsPaySectionsEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
    object ibdsPaySectionsRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
      Visible = False
    end
  end
  object dsrPaySections: TDataSource
    DataSet = ibdsPaySections
    Left = 368
    Top = 340
  end
  object ibdsPaySecMem: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      
        'select MEMBER, POINTS_AVAILABLE, TOTAL_REDEEMED, EDIT_DATE,FULL_' +
        'NAME from MEMBERS'
      'WHERE MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER,FULL_NAME, POINTS_AVAILABLE, TOTAL_REDEEMED, EDIT_' +
        'DATE from MEMBERS'
      'WHERE MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'UPDATE MEMBERS'
      'SET'
      '  POINTS_AVAILABLE = :POINTS_AVAILABLE, '
      ' TOTAL_REDEEMED = :TOTAL_REDEEMED, '
      '  EDIT_DATE = :EDIT_DATE '
      'WHERE MEMBER = :MEMBER')
    Left = 368
    Top = 288
    object ibdsPaySecMemMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsPaySecMemEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
    end
    object ibdsPaySecMemFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object ibdsPaySecMemPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object ibdsPaySecMemTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
  end
  object dsrPaySecMem: TDataSource
    DataSet = ibdsPaySecMem
    Left = 376
    Top = 292
  end
  object dsrProcess: TDataSource
    DataSet = qryProcess
    Left = 432
    Top = 8
  end
  object qryProcess: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'select * FROM PRCSS_LOG WHERE USER_CODE = :USER_CODE'
      'order by EDIT_DATE desc')
    UpdateObject = IBUpdateSQL
    Left = 440
    Top = 12
    ParamData = <
      item
        DataType = ftInteger
        Name = 'USER_CODE'
        ParamType = ptUnknown
      end>
    object qryProcessUSER_CODE: TIntegerField
      DisplayLabel = 'User Code'
      DisplayWidth = 8
      FieldName = 'USER_CODE'
    end
    object qryProcessEDIT_DATE: TDateTimeField
      DisplayLabel = 'Date'
      DisplayWidth = 18
      FieldName = 'EDIT_DATE'
      DisplayFormat = 'dd mmm yyyy hh:mm:ss'
    end
    object qryProcessMSG: TIBStringField
      DisplayLabel = 'Message'
      DisplayWidth = 44
      FieldName = 'MSG'
      Size = 100
    end
  end
  object qryBackupLog: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'select * FROM BACKUP_LOG order by DATE_TIME desc')
    Left = 373
    Top = 12
    object qryBackupLogBACKUP: TIntegerField
      FieldName = 'BACKUP'
      Required = True
    end
    object qryBackupLogNAME: TIBStringField
      FieldName = 'NAME'
      Size = 60
    end
    object qryBackupLogDATE_TIME: TDateTimeField
      FieldName = 'DATE_TIME'
    end
    object qryBackupLogLOG: TMemoField
      FieldName = 'LOG'
      BlobType = ftMemo
      Size = 8
    end
    object qryBackupLogSUCCESS: TIBStringField
      FieldName = 'SUCCESS'
      Size = 1
    end
    object qryBackupLogTYPE: TSmallintField
      FieldName = 'TYPE'
    end
    object qryBackupLogMAN_AUTO: TIBStringField
      FieldName = 'MAN_AUTO'
      Size = 1
    end
    object qryBackupLogPCNAME: TIBStringField
      FieldName = 'PCNAME'
      Size = 50
    end
    object qryBackupLogNOTIFIED: TIBStringField
      FieldName = 'NOTIFIED'
      Size = 1
    end
  end
  object ibdsLetters: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsLettersAfterPost
    AfterPost = ibdsLettersAfterPost
    BeforePost = ibdsLettersBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from LETTERS'
      'where'
      '  NAME = :OLD_NAME')
    InsertSQL.Strings = (
      'insert into LETTERS'
      '  (NAME, LETTER)'
      'values'
      '  (:NAME, :LETTER)')
    RefreshSQL.Strings = (
      'select * from LETTERS where NAME = :NAME'
      '')
    SelectSQL.Strings = (
      'select * from LETTERS where NAME = :NAME')
    ModifySQL.Strings = (
      'update LETTERS'
      'set'
      '  NAME = :NAME,'
      '  LETTER = :LETTER'
      'where'
      '  NAME = :OLD_NAME')
    Left = 360
    Top = 184
    object ibdsLettersNAME: TIBStringField
      FieldName = 'NAME'
      Required = True
      Size = 25
    end
    object ibdsLettersLETTER: TBlobField
      FieldName = 'LETTER'
      BlobType = ftBlob
      Size = 8
    end
    object ibdsLettersPROTECT: TIBStringField
      FieldName = 'PROTECT'
      Size = 1
    end
  end
  object dsrLetters: TDataSource
    DataSet = ibdsLetters
    OnStateChange = dsrLettersStateChange
    Left = 368
    Top = 176
  end
  object IBBackupService: TIBBackupService
    Protocol = TCP
    Params.Strings = (
      'user_name=SYSDBA'
      'password=master')
    LoginPrompt = False
    TraceFlags = []
    Verbose = True
    BlockingFactor = 0
    DatabaseName = 'c:\stockmaster\stockmaster.gdb'
    Options = [NonTransportable]
    Left = 224
    Top = 136
  end
  object IBDatabaseInfo: TIBDatabaseInfo
    Database = IBDatabase
    Left = 224
    Top = 180
  end
  object IBSecurityService: TIBSecurityService
    ServerName = 'CHRIS-PC'
    Protocol = TCP
    Params.Strings = (
      'user_name=DEFAULT'
      'password=password')
    LoginPrompt = False
    TraceFlags = []
    SecurityAction = ActionAddUser
    UserID = 0
    GroupID = 0
    Left = 224
    Top = 104
  end
  object dsrPayTend: TDataSource
    DataSet = qryPayTend
    Left = 368
    Top = 248
  end
  object tblScreens: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    ForcedRefresh = True
    AfterPost = tblScreensAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'SCREEN_CODE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'SCREEN_DESC'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'DEF_ACCESS'
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY13'
        Fields = 'SCREEN_CODE'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'SCREENS'
    Left = 432
    Top = 64
    object tblScreensSCREEN_CODE: TIntegerField
      DisplayLabel = 'Screen Code'
      DisplayWidth = 11
      FieldName = 'SCREEN_CODE'
      ReadOnly = True
      Required = True
    end
    object tblScreensSCREEN_DESC: TIBStringField
      DisplayLabel = 'Screen Desc'
      DisplayWidth = 54
      FieldName = 'SCREEN_DESC'
      ReadOnly = True
      Size = 40
    end
    object tblScreensDEF_ACCESS: TIBStringField
      DisplayLabel = 'Access'
      DisplayWidth = 6
      FieldName = 'DEF_ACCESS'
      Size = 1
    end
  end
  object dsrScreens: TDataSource
    DataSet = tblScreens
    Left = 432
    Top = 72
  end
  object ibdsPayMemSec: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select M.MEMBER,M.FULL_NAME, S.DESCRIPTION,'
      'MS.AMOUNT_PAID,MS.AMOUNT,MS.PAY,MS.EXPIRES,MS.SECTION,MS.PAID,'
      'MS.EDIT_DATE,MS.RECEIPT,S.EXPIRES as SECTION_EXPIRES,S.MIN_FEE'
      'from M_SECTIONS MS, SECTIONS S, MEMBERS M'
      'WHERE M.MEMBER = :OLDMEMBER'
      'AND M.MEMBER = MS.MEMBER'
      'AND MS.SECTION = S.SECTION'
      ' ')
    SelectSQL.Strings = (
      'Select M.MEMBER,M.FULL_NAME, S.DESCRIPTION,MS.SECTION,'
      'MS.AMOUNT_PAID,MS.AMOUNT,MS.PAY,MS.EXPIRES,MS.PAID,'
      'MS.EDIT_DATE,MS.RECEIPT,S.EXPIRES as SECTION_EXPIRES,S.MIN_FEE'
      'from M_SECTIONS MS, SECTIONS S, MEMBERS M'
      'WHERE M.MEMBER = -1'
      'AND M.MEMBER = MS.MEMBER'
      'AND MS.SECTION = S.SECTION'
      '')
    ModifySQL.Strings = (
      'update M_SECTIONS'
      'set'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  AMOUNT = :AMOUNT,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  RECEIPT = :RECEIPT,'
      '  PAY = :PAY,'
      '  EXPIRES = :EXPIRES,'
      '  PAID = :PAID'
      'where'
      '  MEMBER = :OLD_MEMBER'
      'and SECTION = :OLD_SECTION'
      '')
    Left = 304
    Top = 300
    object ibdsPayMemSecMEMBER: TIntegerField
      FieldName = 'MEMBER'
      ReadOnly = True
      Required = True
    end
    object ibdsPayMemSecFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      ReadOnly = True
      Size = 45
    end
    object ibdsPayMemSecDESCRIPTION: TIBStringField
      FieldName = 'DESCRIPTION'
      ReadOnly = True
      Size = 60
    end
    object ibdsPayMemSecAMOUNT_PAID: TIBBCDField
      FieldName = 'AMOUNT_PAID'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayMemSecAMOUNT: TIBBCDField
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayMemSecPAY: TIBBCDField
      FieldName = 'PAY'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPayMemSecEXPIRES: TDateField
      FieldName = 'EXPIRES'
      ReadOnly = True
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPayMemSecEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
    end
    object ibdsPayMemSecRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
      ReadOnly = True
    end
    object ibdsPayMemSecSECTION: TIntegerField
      FieldName = 'SECTION'
      ReadOnly = True
    end
    object ibdsPayMemSecPAID: TIBStringField
      FieldName = 'PAID'
      Size = 1
    end
    object ibdsPayMemSecSECTION_EXPIRES: TDateField
      FieldName = 'SECTION_EXPIRES'
      Origin = '"SECTIONS"."EXPIRES"'
    end
    object ibdsPayMemSecMIN_FEE: TIBBCDField
      FieldName = 'MIN_FEE'
      Origin = '"SECTIONS"."MIN_FEE"'
      Precision = 9
      Size = 2
    end
  end
  object dsrPayMemSec: TDataSource
    DataSet = ibdsPayMemSec
    Left = 312
    Top = 289
  end
  object tblMailCodes: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterPost = tblSubsAfterPost
    BeforePost = tblEventsBeforePost
    OnPostError = tblMailCodesPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'MAIL_CODE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 30
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY48'
        Fields = 'MAIL_CODE'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'MAIL_CODES'
    Left = 88
    Top = 360
    object tblMailCodesDESCRIPTION: TIBStringField
      DisplayWidth = 35
      FieldName = 'DESCRIPTION'
      Size = 30
    end
    object tblMailCodesMAIL_CODE: TIntegerField
      DisplayLabel = 'Mail Code'
      DisplayWidth = 12
      FieldName = 'MAIL_CODE'
      Required = True
      Visible = False
    end
  end
  object dsrTblMailCodes: TDataSource
    DataSet = tblMailCodes
    OnStateChange = dsrTblMailCodesStateChange
    Left = 96
    Top = 368
  end
  object ibdsMembersDef: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AutoCalcFields = False
    AfterCancel = ibdsMembersDefAfterCancel
    AfterPost = ibdsMembersDefAfterPost
    BeforeDelete = ibdsMembersDefBeforeDelete
    BeforeInsert = ibdsMembersDefBeforeDelete
    BeforePost = ibdsMembersDefBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS_DEF'
      'where'
      '  MEMBER = :OLD_MEMBER'
      '')
    InsertSQL.Strings = (
      'insert into MEMBERS_DEF'
      '  (MEMBER, KNOWN_AS, MALE_FEMALE, DATE_OF_BIRTH, LUCKY_DATE, '
      'ADDRESS, ADDRESS1, '
      '   MAILING_LIST, MAILING_CODE, MAILING_LIST_CODE, '
      'NEWSLETTER, '
      '   EMAIL, HOME_PHONE, WORK_PHONE, CELL_PHONE, PROPOSER, '
      'SECONDER, JOINED, '
      '   JOINING_FEE,  RECEIPT, APPROVED, EXPIRES, TRANS_FROM, '
      'SUB_PAID, '
      '   SECTION_PAID, ALLOW_CREDIT, CREDIT_LIMIT, BONUS_CREDIT, '
      'EARNT_CREDIT, '
      
        '   PRE_PAID_CREDIT, EARN_POINTS,   POINTS_AVAILABLE, TOTAL_EARNT' +
        ', '
      '   TOTAL_REDEEMED, PRODUCT_PURCHASE, AMOUNT_DONATED, '
      'PRICE_LEVEL, VIP_PAY_ONLY, '
      '   CARD_TYPE, GL_ACCOUNT, DISCOUNT, INCL_EOD, POSITIONS, '
      'CHARGE_CODE, DECEASED, '
      '   SENIOR, FOUNDATION, DONATED, REFUNDED, DEBENTURE, SAVED, '
      'DAIRY_NOTES, '
      '   EDIT_DATE, TITLE, INITIALS, FIRST_NAME, SURNAME, FULL_NAME, '
      'IS_ACTIVE, '
      
        '   ANALYSE, INACTIVE_DATE, DISTRICT,CITY, COUNTRY,SWIPE_CARD_NUM' +
        'BER,FAX_NUMBER,FAX_WORK,COMPANY,OCCUPATION,BARCODE_NUMBER, CARD_' +
        'LAST_USED,'
      'AUTO_PAYMENT,PAYMENT_PERIOD,INVOICE_NUMBER,'
      
        ' PRINT_CARD, PRINT_RECEIPT,NUMBER_CARDS ,RECEIVE_EMAIL,PAYMENT_D' +
        'AY,STREET_ADDRESS,STREET_ADDRESS1,'
      
        'STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50,CLUB' +
        'S, ASSOCIATION,REASON,SWIPE_CARD,'
      
        'VISITS,LAST_VISIT,DONATION,DONATION_MEMBER,PROPOSER_NAME,SECONDE' +
        'R_NAME )'
      'values'
      
        '  (:MEMBER, :KNOWN_AS, :MALE_FEMALE, :DATE_OF_BIRTH, :LUCKY_DATE' +
        ', :ADDRESS, '
      '   :ADDRESS1, :MAILING_LIST, :MAILING_CODE, :MAILING_LIST_CODE, '
      
        '   :NEWSLETTER, :EMAIL, :HOME_PHONE, :WORK_PHONE, :CELL_PHONE, :' +
        'PROPOSER, '
      
        '   :SECONDER, :JOINED, :JOINING_FEE, :RECEIPT, :APPROVED, :EXPIR' +
        'ES, '
      
        '   :TRANS_FROM, :SUB_PAID, :SECTION_PAID, :ALLOW_CREDIT, :CREDIT' +
        '_LIMIT, '
      
        '   :BONUS_CREDIT, :EARNT_CREDIT, :PRE_PAID_CREDIT, :EARN_POINTS,' +
        '  :POINTS_AVAILABLE,'
      '    :TOTAL_EARNT, :TOTAL_REDEEMED, :PRODUCT_PURCHASE, '
      
        '   :AMOUNT_DONATED, :PRICE_LEVEL, :VIP_PAY_ONLY, :CARD_TYPE,:GL_' +
        'ACCOUNT, '
      
        '   :DISCOUNT, :INCL_EOD, :POSITIONS, :CHARGE_CODE, :DECEASED, :S' +
        'ENIOR, '
      
        '   :FOUNDATION, :DONATED, :REFUNDED, :DEBENTURE, :SAVED,:DAIRY_N' +
        'OTES, '
      
        '   :EDIT_DATE, :TITLE, :INITIALS, :FIRST_NAME, :SURNAME, :FULL_N' +
        'AME, :IS_ACTIVE, '
      
        '   :ANALYSE, :INACTIVE_DATE,:DISTRICT,:CITY,:COUNTRY,  :SWIPE_CA' +
        'RD_NUMBER, :FAX_NUMBER, :FAX_WORK,:COMPANY, :OCCUPATION,'
      '  :BARCODE_NUMBER,:CARD_LAST_USED,'
      '  :AUTO_PAYMENT,:PAYMENT_PERIOD,:INVOICE_NUMBER,'
      
        ' :PRINT_CARD, :PRINT_RECEIPT,:NUMBER_CARDS,:RECEIVE_EMAIL, :PAYM' +
        'ENT_DAY,:STREET_ADDRESS,:STREET_ADDRESS1,'
      
        ':STREET_CITY,:STREET_COUNTRY,:STREET_DISTRICT,:SPARE_VARCHAR_50,' +
        ':CLUBS, :ASSOCIATION,:REASON,:SWIPE_CARD,'
      
        ':VISITS, :LAST_VISIT,:DONATION,:DONATION_MEMBER,:PROPOSER_NAME,:' +
        'SECONDER_NAME   )'
      '')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  KNOWN_AS,'
      '  MALE_FEMALE,'
      '  DATE_OF_BIRTH,'
      '  LUCKY_DATE,'
      '  ADDRESS,'
      '  ADDRESS1,'
      '  MAILING_LIST,'
      '  MAILING_CODE,'
      '  MAILING_LIST_CODE,'
      '  NEWSLETTER,'
      '  EMAIL,'
      '  HOME_PHONE,'
      '  WORK_PHONE,'
      '  CELL_PHONE,'
      '  PROPOSER,'
      '  SECONDER,'
      '  JOINED,'
      '  JOINING_FEE,'
      '  RECEIPT,'
      '  APPROVED,'
      '  EXPIRES,'
      '  TRANS_FROM,'
      '    SUB_PAID,'
      '  SECTION_PAID,'
      '  ALLOW_CREDIT,'
      '  CREDIT_LIMIT,'
      '  BONUS_CREDIT,'
      '  EARNT_CREDIT,'
      '  PRE_PAID_CREDIT,'
      
        '  EARNT_CREDIT + PRE_PAID_CREDIT + BONUS_CREDIT as VALUE_OF_CRED' +
        'IT,'
      '  EARN_POINTS,'
      '  POINTS_AVAILABLE,'
      '  TOTAL_EARNT,'
      '  TOTAL_REDEEMED,'
      '  PRODUCT_PURCHASE,'
      '  AMOUNT_DONATED,'
      '  PRICE_LEVEL,'
      '  VIP_PAY_ONLY,'
      '  CARD_TYPE,'
      '  GL_ACCOUNT,'
      '  DISCOUNT,'
      '  INCL_EOD,'
      '  POSITIONS,'
      '  CHARGE_CODE,'
      '  DECEASED,'
      '  SENIOR,'
      '  FOUNDATION,'
      '  DONATED,'
      '  REFUNDED,'
      '  DEBENTURE,'
      '  SAVED,'
      '  DAIRY_NOTES,'
      '  EDIT_DATE,'
      '  TITLE,'
      '  INITIALS,'
      '  FIRST_NAME,'
      '  SURNAME,'
      '  FULL_NAME,'
      '  IS_ACTIVE,'
      '  ANALYSE,'
      '  INACTIVE_DATE,'
      '  DISTRICT,'
      '  CITY,'
      '  COUNTRY,'
      '  SWIPE_CARD_NUMBER,'
      '  FAX_NUMBER,FAX_WORK,'
      '  COMPANY,'
      '  OCCUPATION,'
      '  BARCODE_NUMBER,'
      '  CARD_LAST_USED,'
      '  AUTO_PAYMENT,'
      ' PAYMENT_PERIOD,'
      ' INVOICE_NUMBER,'
      
        ' PRINT_CARD,PRINT_RECEIPT ,RECEIVE_EMAIL,PAYMENT_DAY,NUMBER_CARD' +
        'S,'
      ' STREET_ADDRESS,STREET_ADDRESS1, '
      ' STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50,'
      'CLUBS, ASSOCIATION,REASON,SWIPE_CARD,VISITS,LAST_VISIT,'
      'DONATION,DONATION_MEMBER,PROPOSER_NAME,SECONDER_NAME '
      'from MEMBERS_DEF '
      'where'
      '  MEMBER = :MEMBER'
      '')
    SelectSQL.Strings = (
      
        'select MEMBER, KNOWN_AS, MALE_FEMALE, DATE_OF_BIRTH, LUCKY_DATE,' +
        ' ADDRESS, ADDRESS1, MAILING_LIST, MAILING_CODE, MAILING_LIST_COD' +
        'E, NEWSLETTER, EMAIL, HOME_PHONE, WORK_PHONE, CELL_PHONE, PROPOS' +
        'ER, SECONDER, JOINED, JOINING_FEE,  RECEIPT, APPROVED, EXPIRES, ' +
        'TRANS_FROM, SUB_PAID, SECTION_PAID, ALLOW_CREDIT, CREDIT_LIMIT, ' +
        'BONUS_CREDIT, EARNT_CREDIT, PRE_PAID_CREDIT,EARNT_CREDIT + PRE_P' +
        'AID_CREDIT + BONUS_CREDIT as VALUE_OF_CREDIT, EARN_POINTS, POINT' +
        'S_AVAILABLE, TOTAL_EARNT, TOTAL_REDEEMED, PRODUCT_PURCHASE, AMOU' +
        'NT_DONATED, PRICE_LEVEL, VIP_PAY_ONLY, CARD_TYPE, GL_ACCOUNT, DI' +
        'SCOUNT, INCL_EOD, POSITIONS, CHARGE_CODE, DECEASED, SENIOR, FOUN' +
        'DATION, DONATED, REFUNDED, DEBENTURE, SAVED, DAIRY_NOTES, EDIT_D' +
        'ATE, TITLE, INITIALS, FIRST_NAME, SURNAME, FULL_NAME, IS_ACTIVE,' +
        ' ANALYSE, INACTIVE_DATE,DISTRICT,CITY,COUNTRY,SWIPE_CARD_NUMBER,' +
        'FAX_NUMBER,FAX_WORK,COMPANY,OCCUPATION,BARCODE_NUMBER,CARD_LAST_' +
        'USED,'
      'AUTO_PAYMENT,PAYMENT_PERIOD,INVOICE_NUMBER,PAYMENT_DAY,'
      
        'PRINT_CARD,PRINT_RECEIPT,NUMBER_CARDS,RECEIVE_EMAIL,STREET_ADDRE' +
        'SS,STREET_ADDRESS1,'
      
        'STREET_CITY,STREET_COUNTRY,STREET_DISTRICT,SPARE_VARCHAR_50,CLUB' +
        'S, ASSOCIATION,REASON,SWIPE_CARD,VISITS,LAST_VISIT,'
      
        'DONATION, DONATION_MEMBER,PROPOSER_NAME,SECONDER_NAME   from MEM' +
        'BERS_DEF'
      'WHERE MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS_DEF'
      'set'
      '  MEMBER = :MEMBER,'
      '  KNOWN_AS = :KNOWN_AS,'
      '  MALE_FEMALE = :MALE_FEMALE,'
      '  DATE_OF_BIRTH = :DATE_OF_BIRTH,'
      '  LUCKY_DATE = :LUCKY_DATE,'
      '  ADDRESS = :ADDRESS,'
      '  ADDRESS1 = :ADDRESS1,'
      '  MAILING_LIST = :MAILING_LIST,'
      '  MAILING_CODE = :MAILING_CODE,'
      '  MAILING_LIST_CODE = :MAILING_LIST_CODE,'
      '  NEWSLETTER = :NEWSLETTER,'
      '  EMAIL = :EMAIL,'
      '  HOME_PHONE = :HOME_PHONE,'
      '  WORK_PHONE = :WORK_PHONE,'
      '  CELL_PHONE = :CELL_PHONE,'
      '  PROPOSER = :PROPOSER,'
      '  SECONDER = :SECONDER,'
      '  JOINED = :JOINED,'
      '  JOINING_FEE = :JOINING_FEE,'
      '  RECEIPT = :RECEIPT,'
      '  APPROVED = :APPROVED,'
      '  EXPIRES = :EXPIRES,'
      '  TRANS_FROM = :TRANS_FROM,'
      '    SUB_PAID = :SUB_PAID,'
      '  SECTION_PAID = :SECTION_PAID,'
      '  ALLOW_CREDIT = :ALLOW_CREDIT,'
      '  CREDIT_LIMIT = :CREDIT_LIMIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  EARN_POINTS = :EARN_POINTS,'
      '    POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  PRODUCT_PURCHASE = :PRODUCT_PURCHASE,'
      '  AMOUNT_DONATED = :AMOUNT_DONATED,'
      '  PRICE_LEVEL = :PRICE_LEVEL,'
      '  VIP_PAY_ONLY = :VIP_PAY_ONLY,'
      '  CARD_TYPE = :CARD_TYPE,'
      '  GL_ACCOUNT = :GL_ACCOUNT,'
      '  DISCOUNT = :DISCOUNT,'
      '  INCL_EOD = :INCL_EOD,'
      '  POSITIONS = :POSITIONS,'
      '  CHARGE_CODE = :CHARGE_CODE,'
      '  DECEASED = :DECEASED,'
      '  SENIOR = :SENIOR,'
      '  FOUNDATION = :FOUNDATION,'
      '  DONATED = :DONATED,'
      '  REFUNDED = :REFUNDED,'
      '  DEBENTURE = :DEBENTURE,'
      '  SAVED = :SAVED,'
      '  DAIRY_NOTES = :DAIRY_NOTES,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  TITLE = :TITLE,'
      '  INITIALS = :INITIALS,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  SURNAME = :SURNAME,'
      '  FULL_NAME = :FULL_NAME,'
      '  IS_ACTIVE = :IS_ACTIVE,'
      '  ANALYSE = :ANALYSE,'
      '  INACTIVE_DATE = :INACTIVE_DATE,'
      '  DISTRICT =  :DISTRICT, '
      '  CITY = :CITY,'
      '  COUNTRY = :COUNTRY,'
      '  SWIPE_CARD_NUMBER  = :SWIPE_CARD_NUMBER,'
      '  FAX_NUMBER  = :FAX_NUMBER,'
      '  FAX_WORK = :FAX_WORK,'
      '  COMPANY  = :COMPANY,'
      '  OCCUPATION  = :OCCUPATION,'
      '  BARCODE_NUMBER  = :BARCODE_NUMBER,'
      '  CARD_LAST_USED  = :CARD_LAST_USED,'
      '  AUTO_PAYMENT  = :AUTO_PAYMENT,'
      '  PAYMENT_PERIOD = :PAYMENT_PERIOD,'
      ' INVOICE_NUMBER = :INVOICE_NUMBER,'
      ' PRINT_CARD = :PRINT_CARD,'
      ' PRINT_RECEIPT = :PRINT_RECEIPT,'
      ' RECEIVE_EMAIL = :RECEIVE_EMAIL,'
      ' NUMBER_CARDS = :NUMBER_CARDS,'
      ' PAYMENT_DAY = :PAYMENT_DAY,'
      ' STREET_ADDRESS = :STREET_ADDRESS,'
      ' STREET_ADDRESS1 = :STREET_ADDRESS1,'
      ' STREET_CITY = :STREET_CITY,'
      ' STREET_COUNTRY = :STREET_COUNTRY,'
      ' STREET_DISTRICT = :STREET_DISTRICT,'
      'SPARE_VARCHAR_50  = :SPARE_VARCHAR_50,'
      'CLUBS = :CLUBS, '
      'ASSOCIATION = :ASSOCIATION,'
      'REASON = :REASON,'
      'SWIPE_CARD = :SWIPE_CARD,'
      'VISITS = :VISITS,'
      'LAST_VISIT = :LAST_VISIT,'
      'DONATION = :DONATION,'
      'DONATION_MEMBER = :DONATION_MEMBER,'
      'PROPOSER_NAME = :PROPOSER_NAME,'
      'SECONDER_NAME = :SECONDER_NAME'
      ''
      'where'
      '  MEMBER = :MEMBER'
      '')
    UniDirectional = True
    Left = 57
    Top = 116
    object IntegerField1: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object IBStringField1: TIBStringField
      FieldName = 'KNOWN_AS'
      Size = 25
    end
    object IBStringField2: TIBStringField
      FieldName = 'MALE_FEMALE'
      Size = 1
    end
    object DateField1: TDateField
      FieldName = 'DATE_OF_BIRTH'
      DisplayFormat = 'dd mmm yyyy'
    end
    object DateField2: TDateField
      FieldName = 'LUCKY_DATE'
    end
    object IBStringField3: TIBStringField
      FieldName = 'ADDRESS'
      Size = 30
    end
    object IBStringField4: TIBStringField
      FieldName = 'ADDRESS1'
      Size = 30
    end
    object IBStringField5: TIBStringField
      FieldName = 'MAILING_LIST'
      Size = 1
    end
    object IntegerField2: TIntegerField
      FieldName = 'MAILING_CODE'
    end
    object IntegerField3: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
    end
    object IBStringField6: TIBStringField
      FieldName = 'NEWSLETTER'
      Size = 1
    end
    object IBStringField7: TIBStringField
      FieldName = 'EMAIL'
      Size = 60
    end
    object IBStringField8: TIBStringField
      FieldName = 'HOME_PHONE'
      Size = 14
    end
    object IBStringField9: TIBStringField
      FieldName = 'WORK_PHONE'
      Size = 14
    end
    object IBStringField10: TIBStringField
      FieldName = 'CELL_PHONE'
      Size = 14
    end
    object IntegerField4: TIntegerField
      FieldName = 'PROPOSER'
    end
    object IntegerField5: TIntegerField
      FieldName = 'SECONDER'
    end
    object DateField3: TDateField
      FieldName = 'JOINED'
      DisplayFormat = 'dd mmm yyyy'
    end
    object IBBCDField1: TIBBCDField
      FieldName = 'JOINING_FEE'
      currency = True
      Precision = 9
      Size = 2
    end
    object IntegerField6: TIntegerField
      FieldName = 'RECEIPT'
    end
    object DateField4: TDateField
      FieldName = 'APPROVED'
    end
    object DateField5: TDateField
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object IBStringField11: TIBStringField
      FieldName = 'TRANS_FROM'
      Size = 40
    end
    object SmallintField1: TSmallintField
      FieldName = 'SUB_PAID'
    end
    object SmallintField2: TSmallintField
      FieldName = 'SECTION_PAID'
    end
    object IBStringField12: TIBStringField
      FieldName = 'ALLOW_CREDIT'
      Size = 1
    end
    object IBBCDField2: TIBBCDField
      FieldName = 'CREDIT_LIMIT'
      currency = True
      Precision = 9
      Size = 3
    end
    object IBStringField13: TIBStringField
      FieldName = 'EARN_POINTS'
      Size = 1
    end
    object IBBCDField10: TIBBCDField
      FieldName = 'PRODUCT_PURCHASE'
      currency = True
      Precision = 18
      Size = 3
    end
    object IBBCDField11: TIBBCDField
      FieldName = 'AMOUNT_DONATED'
      currency = True
      Precision = 9
      Size = 3
    end
    object IntegerField8: TIntegerField
      FieldName = 'PRICE_LEVEL'
    end
    object IBStringField14: TIBStringField
      FieldName = 'VIP_PAY_ONLY'
      Size = 1
    end
    object IBStringField15: TIBStringField
      FieldName = 'CARD_TYPE'
      Size = 1
    end
    object IBStringField16: TIBStringField
      FieldName = 'GL_ACCOUNT'
      Size = 17
    end
    object IBBCDField12: TIBBCDField
      FieldName = 'DISCOUNT'
      Precision = 9
      Size = 3
    end
    object IBStringField17: TIBStringField
      FieldName = 'INCL_EOD'
      Size = 1
    end
    object IBStringField18: TIBStringField
      FieldName = 'POSITIONS'
      Size = 25
    end
    object IntegerField9: TIntegerField
      FieldName = 'CHARGE_CODE'
    end
    object IBStringField19: TIBStringField
      FieldName = 'DECEASED'
      Size = 1
    end
    object IBStringField20: TIBStringField
      FieldName = 'SENIOR'
      Size = 1
    end
    object IBStringField21: TIBStringField
      FieldName = 'FOUNDATION'
      Size = 1
    end
    object IBStringField22: TIBStringField
      FieldName = 'DONATED'
      Size = 1
    end
    object IBStringField23: TIBStringField
      FieldName = 'REFUNDED'
      Size = 1
    end
    object IntegerField10: TIntegerField
      FieldName = 'DEBENTURE'
    end
    object IBStringField24: TIBStringField
      FieldName = 'SAVED'
      Size = 1
    end
    object BlobField1: TBlobField
      FieldName = 'DAIRY_NOTES'
      BlobType = ftBlob
      Size = 8
    end
    object DateTimeField1: TDateTimeField
      FieldName = 'EDIT_DATE'
    end
    object IBStringField25: TIBStringField
      FieldName = 'TITLE'
      Size = 6
    end
    object IBStringField26: TIBStringField
      FieldName = 'INITIALS'
      Size = 6
    end
    object IBStringField27: TIBStringField
      FieldName = 'FIRST_NAME'
    end
    object IBStringField28: TIBStringField
      FieldName = 'SURNAME'
      Size = 25
    end
    object IBStringField29: TIBStringField
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object IBStringField30: TIBStringField
      FieldName = 'IS_ACTIVE'
      Size = 1
    end
    object IBStringField31: TIBStringField
      FieldName = 'ANALYSE'
      Size = 1
    end
    object DateField6: TDateField
      FieldName = 'INACTIVE_DATE'
    end
    object IBStringField32: TIBStringField
      FieldName = 'DISTRICT'
    end
    object ibdsMembersDefCITY: TIBStringField
      FieldName = 'CITY'
      Size = 25
    end
    object ibdsMembersDefCOUNTRY: TIBStringField
      FieldName = 'COUNTRY'
      Size = 25
    end
    object ibdsMembersDefSWIPE_CARD_NUMBER: TIntegerField
      FieldName = 'SWIPE_CARD_NUMBER'
    end
    object ibdsMembersDefFAX_NUMBER: TIBStringField
      FieldName = 'FAX_NUMBER'
      Size = 14
    end
    object ibdsMembersDefCOMPANY: TIBStringField
      FieldName = 'COMPANY'
      Size = 40
    end
    object ibdsMembersDefOCCUPATION: TIBStringField
      FieldName = 'OCCUPATION'
      Size = 30
    end
    object ibdsMembersDefBARCODE_NUMBER: TIntegerField
      FieldName = 'BARCODE_NUMBER'
    end
    object ibdsMembersDefCARD_LAST_USED: TDateField
      FieldName = 'CARD_LAST_USED'
    end
    object ibdsMembersDefAUTO_PAYMENT: TIBStringField
      FieldName = 'AUTO_PAYMENT'
      Size = 1
    end
    object ibdsMembersDefPAYMENT_PERIOD: TSmallintField
      FieldName = 'PAYMENT_PERIOD'
    end
    object ibdsMembersDefINVOICE_NUMBER: TIBStringField
      FieldName = 'INVOICE_NUMBER'
      Size = 22
    end
    object ibdsMembersDefPRINT_CARD: TIBStringField
      FieldName = 'PRINT_CARD'
      Size = 1
    end
    object ibdsMembersDefPRINT_RECEIPT: TIBStringField
      FieldName = 'PRINT_RECEIPT'
      Size = 1
    end
    object ibdsMembersDefRECEIVE_EMAIL: TIBStringField
      FieldName = 'RECEIVE_EMAIL'
      Size = 1
    end
    object ibdsMembersDefPAYMENT_DAY: TSmallintField
      FieldName = 'PAYMENT_DAY'
    end
    object ibdsMembersDefSTREET_ADDRESS: TIBStringField
      FieldName = 'STREET_ADDRESS'
      Size = 30
    end
    object ibdsMembersDefSTREET_ADDRESS1: TIBStringField
      FieldName = 'STREET_ADDRESS1'
      Size = 30
    end
    object ibdsMembersDefSTREET_CITY: TIBStringField
      FieldName = 'STREET_CITY'
      Size = 25
    end
    object ibdsMembersDefSTREET_COUNTRY: TIBStringField
      FieldName = 'STREET_COUNTRY'
      Size = 25
    end
    object ibdsMembersDefSTREET_DISTRICT: TIBStringField
      FieldName = 'STREET_DISTRICT'
    end
    object ibdsMembersDefNUMBER_CARDS: TSmallintField
      FieldName = 'NUMBER_CARDS'
    end
    object ibdsMembersDefFAX_WORK: TIBStringField
      FieldName = 'FAX_WORK'
      Size = 14
    end
    object ibdsMembersDefSPARE_VARCHAR_50: TIBStringField
      FieldName = 'SPARE_VARCHAR_50'
      Origin = 'MEMBERS_DEF.SPARE_VARCHAR_50'
      Size = 40
    end
    object ibdsMembersDefCLUBS: TIntegerField
      FieldName = 'CLUBS'
      Origin = 'MEMBERS_DEF.CLUBS'
    end
    object ibdsMembersDefASSOCIATION: TIBStringField
      FieldName = 'ASSOCIATION'
      Origin = 'MEMBERS_DEF.ASSOCIATION'
      FixedChar = True
      Size = 1
    end
    object ibdsMembersDefBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS_DEF"."BONUS_CREDIT"'
    end
    object ibdsMembersDefEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS_DEF"."EARNT_CREDIT"'
    end
    object ibdsMembersDefPRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS_DEF"."PRE_PAID_CREDIT"'
    end
    object ibdsMembersDefVALUE_OF_CREDIT: TFloatField
      FieldName = 'VALUE_OF_CREDIT'
    end
    object ibdsMembersDefPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
    end
    object ibdsMembersDefTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS_DEF"."TOTAL_EARNT"'
    end
    object ibdsMembersDefTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS_DEF"."TOTAL_REDEEMED"'
    end
    object ibdsMembersDefREASON: TIBStringField
      FieldName = 'REASON'
      Origin = '"MEMBERS_DEF"."REASON"'
      Size = 37
    end
    object ibdsMembersDefSWIPE_CARD: TIBStringField
      FieldName = 'SWIPE_CARD'
      Origin = '"MEMBERS_DEF"."SWIPE_CARD"'
      Size = 128
    end
    object ibdsMembersDefVISITS: TSmallintField
      FieldName = 'VISITS'
      Origin = '"MEMBERS_DEF"."VISITS"'
    end
    object ibdsMembersDefLAST_VISIT: TDateTimeField
      FieldName = 'LAST_VISIT'
      Origin = '"MEMBERS_DEF"."LAST_VISIT"'
    end
    object ibdsMembersDefDONATION: TIBBCDField
      FieldName = 'DONATION'
      Origin = '"MEMBERS_DEF"."DONATION"'
      Precision = 9
      Size = 2
    end
    object ibdsMembersDefDONATION_MEMBER: TIntegerField
      FieldName = 'DONATION_MEMBER'
      Origin = '"MEMBERS_DEF"."DONATION_MEMBER"'
    end
    object ibdsMembersDefPROPOSER_NAME: TIBStringField
      FieldName = 'PROPOSER_NAME'
      Origin = '"MEMBERS_DEF"."PROPOSER_NAME"'
      Size = 40
    end
    object ibdsMembersDefSECONDER_NAME: TIBStringField
      FieldName = 'SECONDER_NAME'
      Origin = '"MEMBERS_DEF"."SECONDER_NAME"'
      Size = 40
    end
  end
  object OpDataSetModel: TOpDataSetModel
    Version = '1.64'
    Dataset = qryReport
    WantFullMemos = False
    Left = 256
    Top = 312
  end
  object OpExcel: TOpExcel
    Version = '1.64'
    Visible = False
    WindowState = xlwsNormal
    ServerLeft = 0
    ServerTop = 0
    ServerHeight = 480
    ServerWidth = 640
    LargeButtons = False
    EnableAnimations = False
    EnableAutoComplete = False
    EnableCancelKey = xlckDisabled
    Workbooks = <>
    Left = 256
    Top = 272
  end
  object IBEvents: TIBEvents
    AutoRegister = False
    Database = IBDatabase
    Events.Strings = (
      'MSG')
    Registered = False
    OnEventAlert = IBEventsEventAlert
    Left = 224
    Top = 264
  end
  object IBStoredProc: TIBStoredProc
    Database = IBDatabase
    Transaction = IBTransaction
    StoredProcName = 'EVENT_NOTIFY'
    Left = 224
    Top = 224
  end
  object tblNumbers: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblNumbersAfterInsert
    AfterPost = tblSubsAfterPost
    BeforePost = tblEventsBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'NUMBER_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'NEXT_MEMBER'
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY80'
        Fields = 'NUMBER_ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'M_NUMBERS'
    Left = 152
    Top = 128
    object tblNumbersDESCRIPTION: TIBStringField
      DisplayWidth = 13
      FieldName = 'DESCRIPTION'
      Size = 30
    end
    object tblNumbersNEXT_MEMBER: TIntegerField
      DisplayWidth = 10
      FieldName = 'NEXT_MEMBER'
    end
    object tblNumbersNUMBER_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'NUMBER_ID'
      Required = True
      Visible = False
    end
  end
  object dsrTblNumbers: TDataSource
    DataSet = tblNumbers
    OnStateChange = dsrTblNumbersStateChange
    Left = 160
    Top = 120
  end
  object dsrTblPrinters: TDataSource
    DataSet = tblPrinters
    Left = 152
    Top = 176
  end
  object tblPrinters: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'PRINTER_ID'
        Attributes = [faRequired]
        DataType = ftSmallint
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'PRINTER_NAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'PRINTER_INDEX'
        DataType = ftSmallint
      end
      item
        Name = 'COMM_PORT'
        DataType = ftSmallint
      end
      item
        Name = 'PC_NAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'EMULATION'
        DataType = ftSmallint
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY28'
        Fields = 'PRINTER_ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'PRINTERS'
    Left = 160
    Top = 184
    object tblPrintersPRINTER_ID: TSmallintField
      DisplayWidth = 10
      FieldName = 'PRINTER_ID'
      ReadOnly = True
      Required = True
    end
    object tblPrintersDESCRIPTION: TIBStringField
      DisplayWidth = 20
      FieldName = 'DESCRIPTION'
      Size = 30
    end
    object tblPrintersPRINTER_NAME: TIBStringField
      DisplayWidth = 32
      FieldName = 'PRINTER_NAME'
      Size = 50
    end
    object tblPrintersPRINTER_INDEX: TSmallintField
      DisplayWidth = 9
      FieldName = 'PRINTER_INDEX'
    end
    object tblPrintersPC_NAME: TIBStringField
      DisplayWidth = 50
      FieldName = 'PC_NAME'
      Size = 50
    end
    object tblPrintersEMULATION: TSmallintField
      DisplayWidth = 10
      FieldName = 'EMULATION'
    end
    object tblPrintersCOMM_PORT: TSmallintField
      DisplayWidth = 11
      FieldName = 'COMM_PORT'
    end
  end
  object tblVersions: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterPost = tblVersionsAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'PC_NAME'
        Attributes = [faRequired]
        DataType = ftString
        Size = 60
      end
      item
        Name = 'INSTALL_DIR'
        DataType = ftString
        Size = 100
      end
      item
        Name = 'VERSION'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'SHARE_NAME'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'PROGRAM'
        DataType = ftString
        Size = 12
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY3'
        Fields = 'PC_NAME'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'VERSIONS'
    Left = 152
    Top = 64
    object tblVersionsINSTALL_DIR: TIBStringField
      DisplayWidth = 35
      FieldName = 'INSTALL_DIR'
      Visible = False
      Size = 100
    end
    object tblVersionsPROGRAM: TIBStringField
      DisplayWidth = 12
      FieldName = 'PROGRAM'
      Visible = False
      Size = 12
    end
    object tblVersionsSHARE_NAME: TIBStringField
      DisplayWidth = 13
      FieldName = 'SHARE_NAME'
      Visible = False
    end
    object tblVersionsPC_NAME: TIBStringField
      DisplayLabel = 'PC Name'
      DisplayWidth = 16
      FieldName = 'PC_NAME'
      ReadOnly = True
      Size = 60
    end
    object tblVersionsVERSION: TIBStringField
      DisplayWidth = 10
      FieldName = 'VERSION'
      ReadOnly = True
      Visible = False
      Size = 10
    end
  end
  object dsrTblVersions: TDataSource
    DataSet = tblVersions
    Left = 160
    Top = 72
  end
  object ibdsExport: TIBDataSet
    Database = IBDatabase
    Transaction = IBExport
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from EXPORT_AREV'
      'where'
      '  EXPORT_ID = :OLD_EXPORT_ID')
    InsertSQL.Strings = (
      'insert into EXPORT_AREV'
      
        '  (EXPORT_DATE, EXPORT_ID, EXPORTED, IN_TRANS, MEMBER,POINTS_USE' +
        'D)'
      'values'
      
        '  (:EXPORT_DATE, :EXPORT_ID, :EXPORTED, :IN_TRANS, :MEMBER,:POIN' +
        'TS_USED)')
    RefreshSQL.Strings = (
      'Select '
      '  EXPORT_ID,'
      '  EXPORT_DATE,'
      '  MEMBER,'
      '  EXPORTED,'
      '  IN_TRANS,'
      '  POINTS_USED,'
      '  RESET_POINTS'
      'from EXPORT_AREV '
      'where'
      '  EXPORT_ID = :EXPORT_ID')
    SelectSQL.Strings = (
      
        'select EXPORT_DATE,EXPORT_ID,EXPORTED,IN_TRANS, MEMBER,POINTS_US' +
        'ED,RESET_POINTS from EXPORT_AREV '
      'where EXPORTED = :EXPORTED'
      'and IN_TRANS = '#39'Y'#39
      'order by EXPORT_DATE '
      '')
    ModifySQL.Strings = (
      'update EXPORT_AREV'
      'set'
      '  EXPORTED = :EXPORTED,'
      '  IN_TRANS = :IN_TRANS'
      'where'
      '  EXPORT_ID = :OLD_EXPORT_ID')
    Left = 158
    Top = 312
    object ibdsExportMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 13
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsExportEXPORT_DATE: TDateTimeField
      DisplayLabel = 'Export Date'
      DisplayWidth = 28
      FieldName = 'EXPORT_DATE'
    end
    object ibdsExportEXPORTED: TIBStringField
      DisplayLabel = 'Exported'
      DisplayWidth = 11
      FieldName = 'EXPORTED'
      Required = True
      Size = 1
    end
    object ibdsExportIN_TRANS: TIBStringField
      DisplayLabel = 'In Trans'
      DisplayWidth = 11
      FieldName = 'IN_TRANS'
      Required = True
      Size = 1
    end
    object ibdsExportPOINTS_USED: TIntegerField
      DisplayLabel = 'Points '
      DisplayWidth = 10
      FieldName = 'POINTS_USED'
    end
    object ibdsExportRESET_POINTS: TIBStringField
      DisplayLabel = 'Reset Points'
      DisplayWidth = 10
      FieldName = 'RESET_POINTS'
      Origin = 'EXPORT_AREV.RESET_POINTS'
      FixedChar = True
      Size = 1
    end
    object ibdsExportEXPORT_ID: TIntegerField
      FieldName = 'EXPORT_ID'
      Required = True
      Visible = False
    end
  end
  object SysInfo: TLMDSysInfo
    Left = 264
    Top = 184
  end
  object ExportTimer: TTimer
    Enabled = False
    Interval = 15000
    OnTimer = ExportTimerTimer
    Left = 264
    Top = 144
  end
  object IBExport: TIBTransaction
    Active = False
    DefaultDatabase = IBDatabase
    DefaultAction = TARollback
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 160
    Top = 232
  end
  object sqlExport: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    Transaction = IBExport
    Left = 160
    Top = 272
  end
  object dsrExport: TDataSource
    DataSet = ibdsExport
    Left = 160
    Top = 320
  end
  object ImageListLarge: TImageList
    Height = 32
    ShareImages = True
    Width = 32
    Left = 256
    Top = 104
    Bitmap = {
      494C010124002700040020002000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000800000004001000001002000000000000080
      0200000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C600FFFF
      FF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000084000000
      8400000084000084840000848400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600FFFFFF00C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000008484840000000000C6C6
      C60000FFFF0000000000000000000000000000000000C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000084000084
      840000848400C6C6C600C6C6C600000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00FFFFFF00FFFFFF00C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C60000FF
      FF0000000000FFFF0000FFFF0000FFFF0000FFFF000000000000C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000840000008400008484000084
      8400C6C6C600C6C6C600C6C6C600008484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF00000000000000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000084840000848400C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C60000FF
      FF0000000000FFFF0000FFFF0000FFFF0000FFFF000000000000C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000008400000084000084840000848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000848400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF00000000000000FFFF00C6C6
      C6000000000000000000000000000000000000FFFF00C6C6C600000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      84000084840000848400C6C6C600C6C6C600C6C6C60000848400C6C6C600C6C6
      C600C6C6C6000084840000848400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C60000FF
      FF00C6C6C6000000000000000000000000000000000000FFFF00C6C6C6000000
      00000000FF000000FF000000FF000000FF000000000000FFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000084000084
      8400C6C6C600C6C6C600C6C6C600C6C6C60000008400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600008484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF000000
      00000000FF000000FF000000FF000000FF0000000000C6C6C60000FFFF000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000840000848400C6C6
      C60000848400C6C6C600C6C6C60000008400C6C6C600C6C6C60000000000C6C6
      C600C6C6C600C6C6C600C6C6C600000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C6000000
      00000000FF000000FF000000FF000000FF000000000000FFFF00C6C6C60000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000848484000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084840000008400C6C6C600C6C6
      C600000084000084840000008400C6C6C6000000000000000000000000000000
      000000000000C6C6C600C6C6C600000084000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFF
      FF00FFFFFF00C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000084848400000000000000000000000000C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C6000000000000000000000000000000000000FFFF00C6C6C60000FFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000084848400000000000000
      0000000000000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084840000008400C6C6C600C6C6
      C6000000840000848400C6C6C600000000000000000000000000000000000000
      00000000000000848400C6C6C600000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00FF000000FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FF000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000008484840000000000000000000000
      0000000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008484000000840000008400008484000084
      8400000084000084840000848400000000000000000000000000000000000000
      0000000000000084840000848400000084000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FF00
      0000FFFFFF00FFFFFF00FFFFFF00C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000008484840000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000848484000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FF000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084848400C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF0000000000000000000000000000000000C6C6C60000FFFF00C6C6C60000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000084848400000000000000000000000000000000000000
      0000848484000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C600C6C6C60084848400848484008484840084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FF000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      000000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF000000
      00008484840084848400848484008484840000000000C6C6C60000FFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000008484840000000000000000000000000000000000000000008484
      8400000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400C6C6C600C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C6008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C6000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00C6C6C60000FFFF00000000000000000000000000000000000000000000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C6000000
      0000848484008484840084848400848484000000000000FFFF00C6C6C60000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000848484000000000000000000000000000000000000000000848484000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6
      C60000000000000000000000000000000000000000000000000000000000C6C6
      C60000FFFF000000000084008400840084008400840084008400840084000000
      000000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF000000
      00008484840084848400848484008484840000000000C6C6C60000FFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000000000008484
      8400000000000000000000000000000000000000000084848400000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C60000000000000000000000000000000000000000000000000000FF
      FF00C6C6C6000000000084008400840084008400840084008400840084000000
      0000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C6000000
      0000848484008484840084848400848484000000000000FFFF00C6C6C6000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000848484000000
      0000000000000000000000000000000000008484840000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00C6C6C6000000000000000000000000000000000000000000C6C6
      C60000FFFF000000000084008400840084008400840084008400840084000000
      000000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C6000000000000000000000000000000000000FFFF00C6C6C60000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000084848400000000000000
      0000000000000000000000000000848484000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084848400FFFFFF00848484008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C6C6C6000000000000000000000000000000000000FF
      FF00C6C6C6000000000084008400840084008400840084008400840084000000
      0000C6C6C60000FFFF0000000000000000000000000000000000C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0084848400FFFFFF0084848400FFFFFF008484
      840084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00C6C6C6000000000000000000000000000000000000000000C6C6
      C60000FFFF00C6C6C6000000000000000000000000000000000000000000C6C6
      C60000FFFF0000000000FF00FF00FF00FF00FF00FF00FF00FF0000000000C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C6C6C60000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C60000000000000000000000000000000000000000000000000000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000000000FF00FF00FF00FF00FF00FF00FF00FF000000000000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF008484840084848400FFFFFF00848484008484
      840084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00C6C6C60000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6
      C600000000000000000000000000000000000000000000000000848484000000
      000000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF0000000000FF00FF00FF00FF00FF00FF00FF00FF0000000000C6C6
      C60000FFFF00C6C6C60000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C60000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000840000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000000000FF00FF00FF00FF00FF00FF00FF00FF000000000000FF
      FF00C6C6C60000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF008484840084848400FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6
      C600000000008484840000000000C6C6C600FFFFFF00FFFFFF00FFFFFF000084
      00000084000000840000FFFFFF00FFFFFF00FFFFFF00C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C6000000000000000000000000000000000000FFFF00C6C6
      C60000FFFF000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00848484008484840084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF00FFFFFF00848484000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      000084848400848484008484840000000000C6C6C600FFFFFF00008400000084
      0000008400000084000000840000FFFFFF00C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400000000000000000000000000C6C6C600FFFFFF000084
      00000084000000840000FFFFFF00C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C600000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000000000000000000000000000C6C6C600FFFF
      FF0000840000FFFFFF00C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600FFFFFF00C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000008484840084848400000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000C6C6C6000000000000000000848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C60000000000FFFF000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000008484840000000000C6C6
      C600C6C6C600C6C6C600C6C6C600000000000000000084848400000000008484
      8400848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400848484008484840084848400848484008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF0000000000000000000000
      00000000000000000000C6C6C60000000000FFFF0000FFFF0000FFFF00000000
      0000848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C60084848400848484000000000000000000848484000000
      0000848484008484840000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C6000000000084840000FFFF0000000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60000000000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000084848400848484000000000000000000000000000000
      00000000000000000000000000008484840000000000C6C6C600C6C6C600C6C6
      C600848484008484840000000000000000008484840084848400848484008484
      8400000000008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400848484008400000084000000840000008400000084000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      00000000000084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C6000000000084840000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60000000000FFFF0000FFFF0000FFFF0000FFFF00000000
      000000000000FFFF000000000000000000000000000000000000000000000000
      000000000000000000008484840000000000C6C6C600C6C6C600848484008484
      84000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000848484008484840000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600840000008400000084000000840000008400000084000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      00000000000084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C6000000000084840000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      000000000000FFFF0000FFFF0000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C6008484840084848400000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000084848400848484000000
      0000000000000000000000000000000000000000000000000000840000008400
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084000000000000008400000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000000000008484840084848400848484008484840084848400C6C6C600C6C6
      C6000000000084840000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000000000000000
      0000848484000000000000000000848484000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      0000C6C6C600C6C6C600C6C6C600000000008484840000000000000000008484
      8400848484000000000000000000000000000000000000000000840000008400
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C6000000000000FFFF0000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C6000000000084840000FFFF00000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF0000000000848484008484
      840084848400848484008484840084848400848484008484840000000000C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000000000008484
      84000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000000000C6C6C6008400
      000000000000C6C6C600C6C6C600C6C6C6000000000084848400C6C6C6000000
      0000000000008484840084848400848484000000000000000000840000008400
      00008400000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000C6C6C600FFFF0000C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      00000000000084840000FFFF0000C6C6C600FFFF000000000000C6C6C600C6C6
      C6000000000084840000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000000000C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000848484000000
      000000000000000000000000000084848400C6C6C6000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600000000000000000000000000C6C6C6008400
      000000000000C6C6C600C6C6C600C6C6C600C6C6C6000000000084848400C6C6
      C600C6C6C6000000000000000000848484000000000000000000000000008400
      000084000000840000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C6000000000000FFFF000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000FFFF0000C6C6C600FFFF
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      00000000000084840000C6C6C600FFFF0000C6C6C60000000000C6C6C600C6C6
      C6000000000084840000FFFF00000000000000000000FFFFFF00000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      000084848400C6C6C600C6C6C600C6C6C600C6C6C60000000000C6C6C6000000
      0000FFFF0000FFFF0000FFFF000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000000000000000000084848400000000000000
      0000C6C6C6000000000000000000C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000C6C6C6000000000000000000C6C6C6008400
      00000000000000000000C6C6C60000000000C6C6C60000000000000000008484
      8400C6C6C6000000000084848400000000000000000000000000000000008400
      0000840000008400000000000000000000000000000000FFFF0000FFFF000000
      0000C6C6C600C6C6C600C6C6C600000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000C6C6C600FFFF0000C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      00000000000084840000FFFF0000C6C6C600FFFF000000000000000000000000
      000084840000FFFF0000FFFF00000000000000000000FFFFFF00C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C60000000000C6C6C6000000
      0000FFFF0000FFFF0000FFFF000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF000000000000000000000000000000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000840000000000000000000000C6C6C6008400
      000000000000C6C6C60000000000C6C6C60000000000C6C6C600000000000000
      000000000000848484008484840000000000000000000000000000000000C6C6
      C60084000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000000000C6C6C6000000000000FFFF00000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000FFFF0000C6C6C600FFFF
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      000000000000848400008484000084840000C6C6C60000000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF00000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C60000000000C6C6C60000000000FFFF
      0000FFFF0000FFFF00000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000C6C6C6000000
      0000C6C6C60000000000C6C6C600000000000000000000FFFF0000FFFF000000
      0000C6C6C6000000000084000000840000000000000000000000C6C6C6008400
      00000000000000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      000000000000848484008484840084848400000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000000000FF
      FF0000FFFF0000000000FF00000000FFFF000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000C6C6C600FFFF0000C6C6
      C60000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      000000000000000000000000000084840000FFFF000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000FFFF0000FFFF00000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      000000000000FFFF0000FFFF000000000000000000000000000000000000C6C6
      C60000000000C6C6C60000000000C6C6C600000000000000000000FFFF0000FF
      FF0000000000C6C6C60000000000000000000000000000000000C6C6C6008400
      000000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C6000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400000000000000
      000000FFFF00FF000000FF000000FF00000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000FFFF0000C6C6C600FFFF
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      000000000000848400000000000084840000C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000000000848484008484
      84000000000000000000C6C6C60000000000FFFF0000FFFF0000FFFF00000000
      000000000000FFFF0000FFFF0000000000000000000000000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C6000000000000FF
      FF0000FFFF0000000000C6C6C600000000000000000000000000C6C6C6008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000FF000000FF000000FF000000FF0000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600000000000000000000000000000000000000
      0000C6C6C60084848400848484000000000000000000C6C6C600FFFF0000C6C6
      C600FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084840000000000000000000084840000FFFF000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C60000000000FFFF0000FFFF0000FFFF00000000
      000000000000FFFF0000FFFF0000000000000000000000000000000000000000
      000000000000C6C6C60000000000000000000000000000000000C6C6C6000000
      000000FFFF0000FFFF0000000000C6C6C6000000000000000000C6C6C6008400
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000FF000000FF00000000000000FFFF000084840000000000000000
      0000C6C6C600C6C6C6000000000000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF000000000084848400848484000000000000000000FFFF0000C6C6C600FFFF
      0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF
      0000C6C6C600FFFF0000C6C6C600FFFF0000000000008484000000000000FFFF
      000000000000FFFF00000000000084840000C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60000000000000000000000000000000000FFFF000000000000C6C6
      C60000000000FFFF0000FFFF0000000000000000000000000000000000000000
      00000000000000000000C6C6C60000000000000000000000000000000000C6C6
      C6000000000000FFFF0000FFFF00000000000000000000000000C6C6C6008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400000000000000
      0000C6C6C6000000000000000000FFFF0000FFFF000084840000848400008484
      0000000000000000000000FFFF00000000000000000000000000000000000000
      00000000000084848400848484000000000000000000C6C6C600FFFF0000C6C6
      C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6
      C600FFFF0000C6C6C600FFFF0000C6C6C6000000000084840000FFFF00000000
      0000FFFF0000000000000000000084840000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6
      C600C6C6C6008484840000000000848484008484840000000000C6C6C600C6C6
      C60000000000FFFF0000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C60000000000C6C6C600000000000000
      0000C6C6C6000000000000FFFF0000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FFFF0000FF
      FF0000000000C6C6C60000000000FFFF0000FFFF000084840000848400008484
      00000000000000FFFF000000000000000000C6C6C600C6C6C600000000000000
      0000C6C6C60084848400848484000000000000000000FFFF0000C6C6C600FFFF
      0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF0000C6C6C600FFFF
      0000C6C6C600FFFF0000C6C6C600FFFF0000000000008484000000000000FFFF
      000000000000FFFF00000000000084840000C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      00000000000000000000FFFF0000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60000000000000000000000
      000000000000C6C6C6000000000000FFFF0000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FFFF0000FF
      FF000000000000000000C6C6C60000000000FFFF0000FFFF0000848400008484
      000000FFFF000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000FFFF00000000
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000C6C6C600C6C6C600FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000848484008484840000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      0000C6C6C60000000000C6C6C6000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400000000000000
      000000FFFF0000FFFF0000000000C6C6C60000000000FFFF0000848400008484
      000000FFFF000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000FFFF0000FFFF0000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6000000
      0000C6C6C6000000000000000000C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600000000000000000000FFFF000000000000000000FFFF0000FFFF00008484
      000084840000848400000000000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000FFFF00000000
      0000FFFF000000000000FFFF000000000000FFFF000000000000FFFF00000000
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C6000000000000000000C6C6C6000000000000FFFF0000FF
      FF0000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600000000000000000000FFFF0000FFFF0000000000FFFF0000FFFF
      00008484000000FFFF0000FFFF0000FFFF0000000000C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600000000000000000000000000C6C6C6000000000000FF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000000000C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000FFFF00000000
      000000000000000000000000000000000000FFFF000000000000FFFF00000000
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60084848400000000008484840084848400848484008484
      840084848400848484008484840084848400FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60000000000000000000000000000000000C6C6C6000000
      000000FFFF0000FFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C6000000000000000000C6C6C600C6C6C6000000
      000000000000000000000000000000FFFF000000000000000000C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C60000000000848484008484840000000000C6C6
      C6000000000000FFFF0000FFFF00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C6000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60000000000000000000000000000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6008484840000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF00000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C6000000000000000000000000000000
      0000C6C6C6000000000000000000000084000000840000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C60000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484000000
      000000000000C6C6C60000000000000084000000FF0000008400000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000C6C6C6000000000000FFFF0000000000C6C6
      C60000000000000000000000000000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600848484008484840084848400848484008484
      840084848400848484008484840084848400848484008484840084848400C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084848400C6C6C600C6C6C6008484
      8400000000000000000000000000000084000000000084848400000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600848484008484840000000000C6C6C600C6C6C600C6C6C6000000
      000000000000C6C6C600C6C6C6000000000000FFFF000000000000FFFF00C6C6
      C600000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60000000000C6C6C6008484
      84000000000000000000000000000000840084848400C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600848484000000000000FFFF000000000000FFFF000000
      00000000000000FFFF000000000000FFFF000000000000FFFF000000000000FF
      FF000000000000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600000000000000000000FFFF00000000000000
      0000000000000000000000FFFF000000000000FFFF000000000000FFFF000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600FFFFFF0084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF000000FF000000FF0000000000848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600848484008484
      8400848484008484840000000000C6C6C600C6C6C60000000000C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600848484000000
      0000C6C6C6008484840000000000C6C6C600C6C6C60000000000C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084848400C6C6C600C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000084840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600848484000000
      0000C6C6C6008484840000000000C6C6C600C6C6C60000000000C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600848484008484
      84008484840084848400848484008484840084848400C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C60084000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF00008484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000000000C6C6C600C6C6
      C6000000000000000000C6C6C600C6C6C6008400000084000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF000084840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000000000C6C6
      C600C6C6C60000000000C6C6C600840000008400000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000FF0000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6008484
      8400000000000000000000848400008484000000000000848400008484000000
      0000008484000084840000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF000000
      0000C6C6C600C6C6C60000000000840000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF00008484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6008484
      84000000000000FFFF00C6C6C600008484000000000000FFFF000084840000FF
      FF00C6C6C6000000000000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000000000C6C6C600C6C6C600000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF000000FF000000FF000000FF00000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000084
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000C6C6C6008484
      840000000000C6C6C600000000000000000000FFFF00C6C6C60000848400C6C6
      C60000FFFF00C6C6C60000848400000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000000000C6C6C600C6C6C6000000000000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF000000FF0000FFFF0000FFFF0000FFFF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF00008484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000C6C6C6008484
      8400000000000000000000000000000000000000000000848400C6C6C60000FF
      FF00C6C6C60000FFFF0000848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C6000000
      000000FFFF0000FFFF0000000000C6C6C600C6C6C60000000000000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000084840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000C6C6C6008484
      840000000000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6C600000000000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000008484840000000000000000000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      840000000000000000000000000000000000C6C6C60000FFFF00000000000000
      00000000000000FFFF0000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6C6000000
      0000C6C6C600C6C6C60084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF00008484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000008484840000000000000000000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6008484
      84000000000084848400848484000000000000FFFF0000848400008484000084
      8400000000000000000000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C6000000000000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000084840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF00000000000000000084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400C6C6C600848484008484840000000000C6C6C60000848400000000000000
      0000008484000000000000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C6000000000000FFFF0000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000848400008484000084840000848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000848400000000000084
      84000000000000000000008484000000000000FFFF0000848400000000000000
      00000000000000000000000000000000000000FFFF0000848400000000000000
      0000008484000000000000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C60000000000C6C6C600C6C6C6000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF00000000000000000000000000008484000000000000FF
      FF0000848400008484000084840000FFFF00C6C6C60000848400008484000084
      840000848400008484000084840000848400C6C6C60000FFFF00008484000084
      84000084840000FFFF0000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C6000000000000000000C6C6C600C6C6C60000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF00008484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840084848400848484008484
      840084848400848484008484840084848400848484000084840000000000C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C6000000000000000000C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000000000C6C6C6000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF00000000000000000000FFFF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000084840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000848400000000000000
      0000C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C6000000000000848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600000000000000000000000000C6C6C600C6C6C6000000
      000000FFFF0000FFFF0000000000C6C6C600C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF00000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C6000000
      0000000000000000000000000000000000000000000000848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600000000000000000000000000C6C6C600C6C6
      C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C6008484
      8400848484008484840084848400848484008484840084848400008484000084
      8400008484000084840000848400008484000084840000848400008484000084
      8400008484000084840000848400008484000084840000848400008484000084
      8400008484000084840000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C6000000000000000000C6C6C60000000000C6C6
      C600C6C6C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      FF00000000000000000000000000000000000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000084
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C60000000000C6C6C600000000000000
      0000C6C6C600C6C6C6000000000000FFFF0000FFFF0000000000C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      0000848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF00008484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C6000000000000000000848484008484
      840000000000C6C6C600C6C6C6000000000000FFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000084840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C60084848400000000000000
      00000000000000000000C6C6C600C6C6C6000000000000FFFF00000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400C6C6C600C6C6C600C6C6C600848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000848400000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600008400000084000000840000C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C60000000000848484008484
      8400848484008484840000000000C6C6C600C6C6C60000000000000084000000
      FF00000084000000840000008400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C6000000000000000000C6C6C600848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF00008484000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000FF000000FF000000FF0000C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      840084848400848484008484840000000000C6C6C60000000000000084000000
      FF00000084008484840000008400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C6000000000000000000C6C6C600848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000084840000000000000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400C6C6C600C6C6
      C600C6C6C60084848400848484000000000000000000000000000000FF000000
      84000000000084848400C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000848400000000000000
      0000000000000000000000000000000000000000000084848400C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      0000C6C6C6008484840084848400000000000000000000000000000084000000
      FF0084848400C6C6C600C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008484000000
      000000000000000000000000000000000000000000000000000084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      0000C6C6C6008484840084848400000000000000000000000000000000000000
      840084848400C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000084
      8400000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C60000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000C6C6C6000000
      0000FFFF000000000000FFFF000000000000FFFF000000000000FFFF00000000
      0000FFFF000000000000FFFF000000000000FFFF000000000000FFFF00000000
      0000FFFF000000000000FFFF000000000000FFFF000000000000FFFF00000000
      0000FFFF000000000000FFFF0000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600FFFFFF00C6C6C600C6C6C6000000000000000000000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000000000000000000084848400000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C60000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000FFFF000000000000FFFF000000000000FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600FFFFFF00C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C600C6C6
      C600C6C6C600FFFFFF00C6C6C600C6C6C600FFFFFF0000000000000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000000000000000
      0000FFFFFF00FFFFFF0000000000000000000000000084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      0000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      0000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000084848400848484008484
      840000000000C6C6C60000000000C6C6C60000000000C6C6C600C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF00008484840000000000000000008484000084840000FFFF
      0000FFFF0000FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      000084848400FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C6000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000FFFFFF0000000000000000000000000084848400848484000000
      0000C6C6C60000000000C6C6C60000000000C6C6C600C6C6C600C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF0000848484000000000000000000848400008484
      0000FFFF0000FFFF0000FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600C6C6
      C600FFFFFF00C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF0000000000000000000000000084848400848484000000
      000000000000C6C6C60000000000C6C6C600C6C6C600C6C6C60000000000C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C60000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000008484
      840000000000000000000000000000000000C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF0000C6C6C600FFFF0000C6C6C60000000000000000008484
      000084840000FFFF0000FFFF0000FFFF000084848400FFFF000084848400FFFF
      000084848400FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000084848400848484000000
      0000C6C6C60000000000C6C6C60000000000C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000008484
      840000000000000000000000000000000000C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF00FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000
      00008484000084840000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000C6C6C60084848400FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600FFFFFF00FFFFFF00000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00000000000000000000000000000000000000
      000000000000FFFFFF0000000000000000000000000084848400848484000000
      000000000000C6C6C60000000000C6C6C600C6C6C60000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000008484
      840000000000000000000000000000000000C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000000000000000000008484840084840000848484008484
      0000848484008484000084848400848400008484840084840000848484008484
      0000FFFF0000FFFF0000FFFF0000FFFF000084848400FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600C6C6C600FFFFFF00C6C6C600C6C6C600C6C6C600FFFF
      FF00FFFFFF00FFFFFF0000000000FF00FF000000FF00000084000000FF000000
      84000000FF0000000000FFFFFF00FFFFFF000000000000000000FFFFFF00FFFF
      FF0000000000FFFFFF0000000000000000000000000084848400848484000000
      0000C6C6C60000000000C6C6C600C6C6C600C6C6C60000000000000000000000
      00000000000000000000C6C6C600FF000000FF000000FF000000FF0000000000
      0000000000000000000000000000C6C6C600FF000000FF000000000000000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF008484840000000000000000008484000084848400848400008484
      8400848400008484840084840000848484008484000084848400848400008484
      84008484000084840000FFFF0000FFFF0000C6C6C60084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FF00FF000000FF00FF00FF000000FF00000084000000
      FF00000084000000FF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000084848400848484000000
      000000000000C6C6C600C6C6C600C6C6C60000000000C6C6C600000000000000
      0000C6C6C600FF000000FF000000FF000000FF000000FF000000FF000000FF00
      00000000000000000000C6C6C600FF000000FF000000FF000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600840000008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400000000000000000084840000848484008484
      0000848484008484000084848400848400008484840084840000848484008484
      0000848484008484000084840000FFFF0000FFFF0000FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF000000
      0000C6C6C600FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FFFF
      FF0000000000FF00FF00FF00FF00FF00FF000000FF00FF00FF000000FF000000
      84000000FF00000084000000FF0000000000FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF0000000000000000000000000084848400848484000000
      0000C6C6C60000000000C6C6C600C6C6C6000000000000000000000000000000
      0000FF000000FF000000FF000000FF00000000000000FF000000FF000000FF00
      00000000000000000000FF000000FF000000FF0000000000000000000000C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF0000000000000000000000000000000000008484
      8400000000008484840000000000848484000000000084848400848484008484
      8400848400008484840084840000FFFF0000FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF000000
      000000000000FFFFFF00C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF000000
      0000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000FF00FF00FF000000
      FF00000084000000FF00000084000000FF0000000000FFFFFF00FFFFFF000000
      000000000000FFFFFF0000000000000000000000000084848400848484000000
      000000000000C6C6C600C6C6C600000000000000000000000000000000000000
      0000FF000000FF0000000000000000000000C6C6C600FF000000FF0000000000
      000000000000C6C6C600FF000000FF000000C6C6C60000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600000000000000000000000000C6C6C600000000000000
      00000000000000000000C6C6C600C6C6C600000000000000000000000000C6C6
      C60000000000000000000000000000000000C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484000084848400C6C6C600FFFF0000FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000FF00FF00
      FF000000FF00000084000000FF00000084000000FF0000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000084848400848484008484
      8400C6C6C600C6C6C60000000000C6C6C6000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000C6C6C6000000
      000000000000FF000000FF000000FF0000000000000000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000084848400FFFF000084848400FFFF000084840000848484000000
      000084848400848484008484000084848400FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF000000
      0000000000000000000000000000C6C6C600FFFFFF00FFFFFF0000000000FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000
      FF00FF00FF000000FF00000084000000FF00000084000000FF0000000000FFFF
      FF00FFFFFF00FFFFFF000000000000000000000000008484840000000000C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000000000
      00000000000000000000FF000000FF000000FF000000FF000000000000000000
      0000C6C6C600FF000000FF000000C6C6C6000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600840000008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      000084848400FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      000084848400848400008484840084840000FFFF0000FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF000000
      0000FFFFFF00000000000000000000000000FFFFFF00FFFFFF0000000000FF00
      FF00FF00FF00FFFFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF000000FF00FF00FF000000FF00000084000000FF0000848400000000000000
      000000000000FFFFFF000000000000000000000000008484840000000000C6C6
      C600C6C6C60000000000C6C6C60000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000C6C6C60000000000000000000000
      0000FF000000FF000000FF0000000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C60000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF00008484840084848400848484008484840084848400000000000000
      000084840000848484008484000084848400FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FF00
      FF00FF00FF00FFFFFF00FFFFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF000000FF00FF00FF000000FF000084840000000000008484000000
      000000000000FFFFFF000000000000000000000000008484840000000000C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      000000000000FF000000FF000000FF000000000000000000000000000000C6C6
      C600FF000000FF000000C6C6C600000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      0000000000008484000084848400848400008484840084840000848484008484
      000084848400848400008484840084840000FFFF0000FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF000000
      000000000000000000000000000000000000FFFFFF00FFFFFF0000000000FF00
      FF00FF00FF00FFFFFF00FFFFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF000000FF00000000000000000000848400000000000084
      840000000000000000000000000000000000000000008484840000000000C6C6
      C60000000000C6C6C6000000000000000000FF000000FF000000FF0000000000
      0000C6C6C600FF000000FF000000FF00000000000000FF00000000000000FF00
      0000FF000000FF0000000000000000000000C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C60000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00000000
      0000848400008484840084840000848484008484000084848400848400008484
      8400848400008484840084840000C6C6C600FFFF000084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF000000
      0000FFFFFF00000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      0000FF00FF00FF00FF00FFFFFF00FFFFFF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00000000000000000000FFFF0000000000008484000000
      0000008484000000FF000000000000000000000000008484840000000000C6C6
      C600000000000000000000000000C6C6C600FF000000FF000000000000000000
      0000FF000000FF000000FF00000000000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF0000848484000000
      0000848484008484000084848400848400008484840084840000848484008484
      00008484840084840000C6C6C600FFFF0000FFFF0000FFFF000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      000000000000FF00FF00FF00FF00FFFFFF00FFFFFF00FF00FF00FF00FF00FF00
      FF00FF00FF00000000000000000000FFFF000000000000FFFF00000000000084
      84000000FF00008484000000FF0000000000000000008484840000000000C6C6
      C600000000000000000000000000C6C6C600FF000000FF000000FF000000FF00
      0000FF000000FF0000000000000000000000FF000000FF000000FF000000FF00
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C60000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60084000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00000000
      000084840000848484008484000084848400FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF000000000000000000FF00FF00FF00FF00FFFFFF00FF00FF00FF00FF00FF00
      FF0000000000FF00000000FFFF000000000000FFFF000000000000FFFF000000
      FF00008484000000FF00008484000000FF000000000084848400000000000000
      0000C6C6C600000000000000000000000000FF000000FF000000FF000000FF00
      0000C6C6C60000000000000000000000000000000000FF000000FF000000FF00
      0000000000000000000000000000C6C6C6000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084000000000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF0000848484000000
      00000000000084840000848484008484000084848400FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      000000000000FFFF000000000000000000000000000000000000FFFFFF000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00FFFF
      FF00FFFFFF000000000000000000FF00FF00FF00FF00FF00FF00FF00FF000000
      0000FF000000FFFFFF00FF00000000FFFF000000000000FFFF000000FF0000FF
      FF000000FF00008484000000FF00008484000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000C6C6C60000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60000000000C6C6C6000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400000000000000000084840000848484008484000084848400848400008484
      8400848400008484840084840000848484008484000084840000FFFF0000FFFF
      0000FFFF000000000000FFFF0000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000000000FF00FF00FF00FF0000000000FF00
      0000FFFFFF00FF000000FFFFFF00FF00000000FFFF0000FFFF0000FFFF000000
      FF0000FFFF000000FF00008484000000FF000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000000000848484000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      0000848484000000000000000000848400008484840084840000848484008484
      000084848400848400008484840084840000848484008484000084848400FFFF
      000000000000FFFF000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      000000000000FFFFFF00FFFFFF00000000000000000000000000FF000000FFFF
      FF00FF000000FFFFFF00FF00000000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF000000FF0000FFFF000000FF0000848400000084000000000000000000C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      000084000000840000008400000000000000C6C6C60000000000848484000000
      00008484840000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF00008484840000000000000000000000000084848400000000008484
      840000000000848484000000000084848400000000008484840000000000FFFF
      0000FFFF000000000000FFFF000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C600000000000000
      00000000000000000000FFFFFF00FFFFFF0000000000FF000000FFFFFF00FF00
      0000FFFFFF00FF00000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000000FF0000FFFF000000FF000000840000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF008400000084000000840000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C60000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C60000000000C6C6C60000000000C6C6C600C6C6C600C6C6C600000000000000
      0000C6C6C60000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFF000084848400FFFF000084848400FFFF000084848400FFFF
      000084848400FFFF000084848400848484000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFF000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FF000000FFFF
      FF00FF000000FFFFFF00FFFFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF000000FF0000FFFF000000840000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF0084000000840000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      00008400000000000000C6C6C600C6C6C600C6C6C60000000000000000008484
      8400C6C6C60000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF0084848400FFFF000084848400FFFF000084848400FFFF00008484
      8400FFFF000084848400FFFF000084848400FFFF000084848400FFFF00008484
      840000000000000000008484000084840000FFFFFF0084848400FFFF00008484
      8400FFFF000000000000FFFF0000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF0000000000C6C6C600000000000000
      000000000000FFFFFF000000000000000000FFFFFF00FFFFFF0000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF000000FF000000840000008400000084000000
      FF000000FF00840000008400000084000000840000000000FF00840000008400
      00000000FF0084000000840000008400000084000000840000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60084000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000008400
      000000000000C6C6C600C6C6C600000000000000000000000000000000008400
      00000000000000000000000000000000000000000000C6C6C600FFFF0000C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000000000008484000084840000FFFFFF00FFFFFF00FFFF
      FF0000000000FFFF000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF0000000000C6C6C60000000000FFFF
      FF0000000000FFFFFF00000000000000000000000000FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000840000008400000084000000
      FF000000FF008400000084000000840000008400000084000000840000008400
      00008400000084000000840000008400000084000000840000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000840000000000
      0000C6C6C6000000000000000000000000000000000084000000840000008400
      0000C6C6C60000000000000000000000000000000000FFFF0000C6C6C600FFFF
      0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF000000000000000000008484000084840000FFFFFF00FFFF
      0000FFFFFF0000000000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000840000008400000084000000
      FF000000FF008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C600840000008400000084000000840000008400000084000000840000008400
      000084000000840000008400000084000000840000008400000000000000C6C6
      C600000000000000000000000000C6C6C60000000000C6C6C60000000000C6C6
      C6000000000000000000000000000000000000000000C6C6C600FFFF0000FFFF
      FF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFF
      FF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFF
      FF00FFFF0000FFFFFF00FFFF000000000000000000008484000084840000FFFF
      FF00FFFF0000FFFF000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000840000008400000084000000
      FF000000FF00840000008400000084000000840000000000FF00840000008400
      00000000FF008400000084000000840000000000FF0084000000840000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      0000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF0000FFFFFF00FFFF
      0000FFFFFF00FFFF0000FFFF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000840000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000FFFF0000FFFF0000FFFF000000000000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000008400000084
      0000000000000084840000008400008484000084840000848400000000008400
      0000000000008484000084840000848400008484000084840000840084008400
      8400C6C6C600C6C6C600C6C6C600FFFFFF0000000000C6C6C600848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000008400000084
      0000000000000084840000008400008484000084840000848400000000000000
      0000000000008484840084848400848484008484000084840000840084008400
      8400C6C6C600C6C6C600C6C6C600FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C6000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF00000000000084848400C6C6
      C600C6C6C6008484840000000000000000000000000000000000000000000000
      00000000000000FFFF00C6C6C60000FFFF000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000084008400FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000008400000084
      0000000000000000840000008400008484000084840000848400000000000000
      0000000000008484840084848400848484008484840084840000840084008400
      8400FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFF00008484000000000000C6C6C600FFFF
      FF00C6C6C600C6C6C60000000000000000000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00C6C6C60000000000C6C6C600C6C6C600FFFF
      FF00C6C6C600C6C6C600C6C6C600000000000000000084008400FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000840000008400000084
      0000000000000000840000008400008484000084840000848400000000000000
      0000000000008484840084848400848484008484840084840000840084008400
      8400FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF008484840000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840000000000848484008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000008484
      8400848484008484840000000000000000000000000000000000000000000000
      00000000000000000000FFFF0000848400008484000000000000C6C6C600C6C6
      C600FFFFFF00C6C6C60000000000848484000000000000000000000000000000
      0000C6C6C60000FFFF00C6C6C60000FFFF0000000000C6C6C600FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600000000000000000084008400FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000840000008400000084
      0000000000000000840000008400008484000084840000848400000000000000
      00000000000084848400C6C6C600C6C6C600C6C6C60084840000840084008400
      8400FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF0084848400848484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400000000008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484000000
      0000848484008484840084848400000000000000000000000000000000000000
      000000000000FFFF00008484000084840000C6C6C6000000000084848400C6C6
      C600C6C6C6008484840000000000848484008484840000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000C6C6C600FFFFFF00C6C6
      C600C6C6C600FFFFFF00C6C6C600000000000000000084008400840084008400
      8400FF00FF00FF00FF0000000000000000000000000000840000008400000084
      0000000000000000840000008400008484000084840000848400000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C60084840000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600848484008484
      840000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000084848400000000008484
      8400C6C6C600C6C6C600848484008484840084848400C6C6C600C6C6C6008484
      84008484840084848400C6C6C600C6C6C600FFFFFF0084848400848484000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000008484840084848400848484000000000000000000000000000000
      0000FFFF00008484000084840000C6C6C60000FF0000C6C6C600000000000000
      0000000000000000000084840000000000008484840084848400000000000000
      00000000000000FFFF00C6C6C60000FFFF0000000000C6C6C600C6C6C600FFFF
      FF00FFFFFF00C6C6C600C6C6C600000000000000000084008400840084008400
      840084008400FF00FF0000000000000000000000000000840000008400000084
      0000000000000000000000000000008484000084840000848400000000000000
      0000000000008484840084848400848484008484840084840000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C6008484
      84008484840000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C6000000
      000084848400C6C6C600C6C6C6008484840084848400C6C6C600C6C6C600C6C6
      C6008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF00848484000000
      000084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484000000
      000084848400000000008484840084848400000000000000000000000000FFFF
      00008484000084840000C6C6C60000FF0000C6C6C60000FF00008484000000FF
      0000848400008484000084840000FFFF00000000000084848400848484000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C60000000000000000000000000084008400840084008400
      8400840084008400840000000000000000000000000000840000008484000084
      0000000000000000000000000000008484000084840000848400008484000084
      84008400840084008400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600848484008484840000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400000000000000000000000000000000000000000000000000FFFF00008484
      000084840000C6C6C60000FF0000C6C6C60000FF0000C6C6C60000FF00008484
      000000FF0000C6C6C60000FF000084840000FFFF000000000000848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400840084008400
      8400840084008400840000000000000000000000000000840000008484000084
      8400000000000000000000000000008484000084840000848400008484000084
      8400840000008400000084008400840084008400840084008400840084008400
      8400840084008400840084008400FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600848484008484840000000000C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00C6C6C600848484008484
      840084848400000000000000000000000000000000000000000000000000FFFF
      000000FF000084840000C6C6C60000FF0000C6C6C60000FF0000C6C6C60000FF
      00008484000000FF0000C6C6C60000FF000084840000FFFF0000000000008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400840084008400
      8400000000000000000000000000000000000000000000840000008484000084
      84000000000000000000000000000084840000FFFF0000FFFF00008484000084
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00C6C6C6000000
      0000C6C6C6008484840084848400848484008484840084848400848484008484
      84008484840084848400848484008484840084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600848484008484840000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600000000000000000000000000C6C6C600C6C6C600848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000FFFF00008484000000FF00008484000000FF0000C6C6C60000FF0000C6C6
      C60000FF0000C6C6C60000FF0000C6C6C60000FF000084840000FFFF00000000
      0000848484008484840000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000008400840084008400C6C6
      C600FFFFFF00FFFFFF0000000000000000000000000000840000008484000084
      84000000000000000000000000000084840000FFFF0000FFFF0000FFFF000084
      84008484840084848400FFFFFF0084848400FFFFFF00FFFFFF00C6C6C600FFFF
      FF00FFFFFF00FFFFFF00C6C6C600FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF00C6C6C600848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      000000000000FFFF00008484000000FF00008484000000FF0000C6C6C60000FF
      0000C6C6C6008484000084840000848400008484000000FF000084840000FFFF
      0000000000008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000000840000008484000084
      84000000000000000000000000000084840000FFFF0000FFFF0000FFFF000084
      84008484840084848400FFFFFF0084848400FFFFFF00FFFFFF00C6C6C600C6C6
      C600FFFFFF00FFFFFF00C6C6C600FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484840000000000C6C6
      C600C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000FF000000FF0000000000C6C6C600C6C6C600848484008484
      840084848400848484000000000000000000000000000000000000000000FFFF
      00008484000000000000FFFF00008484000000FF00008484000000FF0000C6C6
      C6008484000000FF000000FF000000FF0000848400008484000000FF00008484
      0000FFFF0000000000008484840084848400000000000000000000000000C6C6
      C600C6C6C600C6C6C60000000000000000000000000084008400840084008484
      8400C6C6C600C6C6C60000000000000000000000000000840000848484008484
      84000000000000000000000000000084840000FFFF0000FFFF0000FFFF000084
      84008484840084848400C6C6C60084848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084848400848484000000
      0000C6C6C600C6C6C600848484000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400848484008484840000000000000000000000000000000000FFFF00008484
      000084840000C6C6C60000000000FFFF00008484000000FF00008484000000FF
      00008484000000FF000084840000848400008484000084840000C6C6C60000FF
      000084840000FFFF00000000000084848400848484000000000000000000C6C6
      C600FFFFFF00C6C6C60000000000000000000000000084000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000000848400848484008484
      84000000000000000000000000000084840000FFFF0000FFFF0000FFFF000084
      8400FFFFFF00FFFFFF00FFFFFF0084848400FFFFFF00FFFFFF00C6C6C600FFFF
      FF00FFFFFF00FFFFFF00C6C6C600FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      840000000000C6C6C600848484000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484008484
      840084848400848484000000000000000000000000000000000000000000FFFF
      000000FF000084840000C6C6C60000000000FFFF00008484000000FF00008484
      00008484000084840000848400008484000000FF00008484000000FF0000C6C6
      C60000FF000084840000FFFF000000000000848484008484840000000000C6C6
      C600C6C6C600C6C6C60000000000000000000000000084000000840084008484
      8400848484008484840000000000000000000000000000848400848484008484
      84000000000000000000000000000084840000FFFF0000FFFF0000FFFF000084
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400848484000000000084848400000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000FFFF00008484000000FF00008484000000000000FFFF00008484000000FF
      000084840000848400008484000000FF000000FF000084840000C6C6C60000FF
      0000C6C6C60000FF000084840000FFFF00000000000084848400848484000000
      0000000000000000000000000000000000000000000084000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000000848400848484008484
      8400008484000000000000000000008484000084840000848400848484008484
      8400FFFFFF00FFFFFF00FFFFFF0084848400FFFFFF00FFFFFF00C6C6C600FFFF
      FF00FFFFFF00FFFFFF00C6C6C600FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600FFFFFF00000000008484840000000000000000000000000000000000C6C6
      C600C6C6C6008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C600FFFF
      FF00848484008484840000000000000000000000000000000000000000000000
      000000000000FFFF00008484000000FF00008484000000000000FFFF00008484
      000000FF000084840000848400008484000084840000C6C6C60000FF0000C6C6
      C60000FF0000C6C6C60000FF000084840000FFFF000000000000848484008484
      8400000000000000000000000000000000000000000000000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000000848400848484008484
      840084848400008484008484840084848400C6C6C600FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0084848400FFFFFF00FFFFFF00C6C6C600FFFF
      FF00FFFFFF00FFFFFF00C6C6C600FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFF
      FF0000000000C6C6C60084848400000000000000000000000000000000000000
      0000C6C6C600C6C6C600848484000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000084848400FFFFFF00848484000000000000000000FFFF
      FF00FFFFFF00C6C6C60000000000000000000000000000000000C6C6C600C6C6
      C600FFFFFF008484840000000000000000000000000000000000000000000000
      00000000000000000000FFFF00008484000000FF00008484000000000000FFFF
      00008484000000FF00008484000000FF0000C6C6C60000FF0000C6C6C6008484
      0000C6C6C60000FF0000C6C6C6008484000000FF0000FFFF0000000000008484
      8400848484000000000000000000000000000000000000000000840084008484
      8400848484008484840000000000000000000000000000848400848484008484
      8400848484000084840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF000000
      0000C6C6C600C6C6C60084848400000000000000000000000000000000000000
      000000000000C6C6C600C6C6C600848484000000000084848400FFFFFF00FFFF
      FF00FFFFFF008484840000000000FFFFFF00000000000000000084848400FFFF
      FF00FFFFFF00FFFFFF000000000084848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFF00008484000000FF0000C6C6C6000000
      0000FFFF00008484000000FF00008484000000FF0000C6C6C60000FF0000C6C6
      C60084840000C6C6C60000FF000000FF00008484000084840000FFFF00000000
      0000848484000000000000000000000000000000000000000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000084848400848484008484
      840084848400008484008484840084848400C6C6C600FFFFFF00C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF0000000000C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF00008484000000FF0000C6C6
      C60000000000FFFF00008484000000FF00008484000000FF0000C6C6C60000FF
      0000C6C6C60000FF0000C6C6C6008484000084840000FFFF0000000000008484
      8400000000000000000000000000000000000000000000000000840084008484
      8400C6C6C600C6C6C60000000000000000000000000084848400848484008484
      840084848400008484008484840084848400C6C6C600C6C6C60084848400C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C6000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF0000000000C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084848400FFFF
      FF00FFFFFF00FFFFFF0084848400000000000000000084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF00008484000000FF
      0000C6C6C60000000000FFFF00008484000000FF00008484000000FF0000C6C6
      C60000FF0000C6C6C6008484000084840000FFFF000000000000848484000000
      0000000000000000000000000000000000000000000000000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000084848400848484008484
      840084848400008484008484840084848400C6C6C600C6C6C60084848400C6C6
      C600C6C6C6008484840084848400848484008484840084848400FFFFFF008484
      8400FFFFFF0084848400FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600FFFFFF0000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00848484000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00008484
      000000FF0000C6C6C60000000000FFFF00008484000000FF0000C6C6C60000FF
      0000C6C6C6008484000084840000FFFF00000000000000000000848484008484
      8400000000000000000000000000000000000000000000000000840084008484
      8400FFFFFF00FFFFFF0000000000000000000000000084848400C6C6C600C6C6
      C6008484840000848400FFFFFF00FFFFFF00C6C6C600FFFFFF00C6C6C600C6C6
      C600FFFFFF0000000000C6C6C600FFFFFF00FFFFFF00C6C6C600C6C6C600FFFF
      FF00C6C6C60084848400FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF0000000000C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF00C6C6C60000FFFF0000000000000000000000000000000000FFFF
      00008484000000FF0000C6C6C60000000000FFFF0000848400008484000000FF
      00008484000084840000FFFF00000000000084840000FFFF0000000000008484
      8400848484000000000000000000000000000000000000000000840084008484
      8400848484008484840000000000000000000000000084848400C6C6C600C6C6
      C600C6C6C600008484008484840084848400848484008484840084848400C6C6
      C600FFFFFF0000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C6C6C600C6C6C600FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600FFFFFF0000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000000000000000000000
      0000FFFF00008484000000FF00008484000000000000FFFF000000FF00008484
      000084840000FFFF000000000000848400008484000084840000FFFF00000000
      0000848484000000000000000000000000000000000000000000840000008484
      8400FFFFFF00FFFFFF0084848400848484008484840084848400848484008484
      84008484840084848400FFFFFF00FFFFFF00C6C6C600FFFFFF00C6C6C600C6C6
      C600FFFFFF0000000000C6C6C600FFFFFF00C6C6C600FFFFFF00FFFFFF00FFFF
      FF0084848400C6C6C600C6C6C600FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600FFFFFF0000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C6000000000000000000000000000000
      000000000000FFFF00008484000000FF00008484000000000000FFFF00008484
      0000FFFF000000000000848400008484000084840000FFFF0000000000008484
      8400000000000000000000000000000000000000000000000000840000008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084848400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600FFFFFF00C6C6C600C6C6
      C600FFFFFF000000000084848400C6C6C60084848400FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600C6C6C600FFFF
      FF0000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000000000000000000000
      00000000000000000000FFFF00008484000000FF00008484000000000000FFFF
      000000000000848400008484000084840000FFFF000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484840084848400C6C6
      C600FFFFFF0000000000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00C6C6C600FFFFFF000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF00C6C6C60000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000FFFF00008484000000FF0000C6C6C6000000
      0000848400008484000084840000FFFF00000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084848400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600FFFFFF00C6C6C600C6C6
      C600FFFFFF0000000000C6C6C600FFFFFF00FFFFFF00C6C6C600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00FFFFFF0000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF00008484000084840000C6C6
      C6008484000084840000FFFF0000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400848484008484840084848400848484008484840084848400C6C6
      C600C6C6C60084848400C6C6C600C6C6C600C6C6C60084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000000000FFFFFF0000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C6000000000000000000000000000000000000000000FFFF000000FF00008484
      000084840000FFFF000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084000000840000008400000084000000840084008400840000000000C6C6
      C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C6000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600FFFFFF00C6C6
      C600000000000000000000000000000000000000000000000000FFFF00008484
      0000FFFF00000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C60000000000000000000000000000000000000000000000000000000000FFFF
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C6008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400848484008484
      840084848400FFFFFF008484840084848400848484008484840084848400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600848484000000000000000000848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000084848400C6C6
      C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400C6C6C6008484840084848400000000000000000084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400848484008484
      8400848484008484840084848400FFFFFF008484840084848400848484008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000008484
      8400C6C6C60084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400C6C6C6008484
      8400C6C6C6008484840084848400848484008484840000000000000000008484
      8400848484008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF0000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF008484840084848400FFFFFF00C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF000000
      000084848400C6C6C60084848400C6C6C600C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C6008484840084848400C6C6C600C6C6C600C6C6C6008484
      8400C6C6C6008484840084848400848484008484840084848400848484000000
      0000000000008484840084848400848484008484840000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400848484008484
      8400FFFFFF008484840084848400848484008484840084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600FFFFFF00848484008484840084848400FFFFFF0084848400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF000000000084848400C6C6C60084848400C6C6C600C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000084848400C6C6C600C6C6
      C6008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400C6C6C6008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00C6C6C600C6C6
      C600C6C6C600FFFFFF0084848400FFFFFF0084840000FFFFFF00FFFFFF008484
      840084848400848484008484840084848400FFFFFF00FFFFFF00848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000000000FFFFFF00FFFF
      FF00FFFFFF000000000084848400C6C6C60084848400C6C6C600C6C6C6000000
      0000C6C6C6000000000000000000000000000000000084848400848484008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400C6C6C6008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00848484008484840084848400FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00C6C6C600C6C6
      C600FFFFFF008484840084848400FFFFFF0084840000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840000000000FFFFFF00FFFF
      FF00848484008484840084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000084848400C6C6C60084848400C6C6C6000000
      0000C6C6C6000000000000000000000000008484840084848400C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400C6C6C6008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084848400C6C6C60084848400FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00C6C6C600C6C6
      C600FFFFFF000000000084848400FFFFFF0084840000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C600000000000000
      0000FFFFFF00FFFFFF0084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000084848400C6C6C600C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008484
      8400FFFFFF008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084848400C6C6C60084848400FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00C6C6C600FFFF
      FF00848484000000000084848400FFFFFF0084840000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C6000000000000000000FFFFFF00FFFFFF008484840084848400848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000084848400C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF008484
      840084848400C6C6C600C6C6C600848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00848484008484840084848400FFFFFF00C6C6C600000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00FFFFFF00FFFF
      FF00000000000000000084848400FFFFFF0084840000FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C6000000000000000000FFFFFF00FFFFFF00848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00C6C6C6008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF00C6C6C600C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C60084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840000000000848484000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000008484
      8400FFFFFF00FFFFFF00000000000000000084848400FFFFFF00848484000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C6000000000084848400FFFFFF00FFFFFF00848484000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00C6C6C6008484
      8400C6C6C600C6C6C600FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C6008484840084848400C6C6C600C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      840000000000C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000008484840084848400FFFFFF00FFFF
      FF00848484000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000FFFFFF008484840084848400000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00C6C6C6008484
      8400FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C6000000FF000000FF00C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C6008484840084848400C6C6
      C600C6C6C6008484840084848400848484008484840084848400848484008484
      840000000000C6C6C60084848400848484000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000848484008484
      8400FFFFFF00FFFFFF0084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000848484000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C60000000000000000000000000084848400FFFFFF00FFFFFF008484
      840084848400C6C6C600C6C6C60000FF000000FF0000C6C6C600C6C6C6008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000008484
      840084848400C6C6C600C6C6C600848484008484840084848400848484008484
      8400000000008484840000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF008484840084848400848484008484840084848400848484008484
      840084848400848484008484840084848400848484008484840084848400FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000848484008484840084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000008484840084848400FFFFFF00C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C6008484840084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000000000FFFFFF00C6C6
      C600000000008484840084848400C6C6C600C6C6C60084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000848484008484
      8400FFFFFF00C6C6C600C6C6C60084848400848484008484840084848400C6C6
      C600C6C6C600C6C6C6000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000C6C6C600C6C6C6008484840084848400C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF008484
      8400848484008484840084848400848484008484840084848400848484008484
      84008484840084848400848484008484840084848400FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF0084840000FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      00008484840084848400FFFFFF00C6C6C600C6C6C60084848400848484008484
      84000000000000000000FFFFFF00FFFFFF00FFFFFF00FF000000FF000000FFFF
      FF00C6C6C6000000000084848400C6C6C600C6C6C60084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      000000000000000000008484840084848400FFFFFF00C6C6C600C6C6C6008484
      840084848400FFFFFF00FFFFFF00FF000000FF000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400848484008484840084848400848484008484840084848400FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084840000FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840084848400FFFFFF00C6C6
      C600C6C6C60084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FF00
      0000FFFFFF00C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400FFFFFF0084848400FFFFFF00FFFFFF00FF000000FF000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FFFFFF0000008400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FFFFFF00C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00008484000000840000848400FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00C6C6C600C6C6C6008484840084848400C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00FFFFFF00FF000000FF000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF008484
      840084848400848484008484840084848400FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00008484000084840000848400FFFF
      FF00FFFFFF00C6C6C600C6C6C6008484840084848400C6C6C600C6C6C6008484
      840084848400C6C6C60000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF00FFFFFF00C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF00000084000084840000008400C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C6008484840084848400C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084848400FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF008484
      84008484840084848400848484008484840084848400FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400FFFFFF0000848400C6C6C600C6C6C6008484
      840084848400C6C6C600C6C6C6008484840084848400C6C6C600C6C6C6008484
      8400848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400C6C6C600C6C6C6008484840084848400C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C60084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084848400FFFF
      FF00FFFFFF00FFFFFF0084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000848484008484840084848400C6C6C600C6C6C6008484
      840084848400C6C6C600C6C6C600848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000848484008484840084848400C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000C6C6C600848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084848400848484008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF00848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C60084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000000000000000000000000000848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000C6C6C60084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000848484008484840000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000084848400848484008484
      8400000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600008400000084000000840000848484008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C6008484
      84000000000000000000000000000000000000000000C6C6C600C6C6C600C6C6
      C60000000000C6C6C60084848400840000008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000848484000000000000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF00000000000000848484008484
      8400848484000000000084848400848484000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C60000840000008400000084000000840000008400008484
      8400848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C6008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000C6C6C600C6C6C600C6C6
      C60000000000C6C6C60084000000FF0000008400000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008484840000000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000000000008484
      8400848484008484840084848400840000000000000000000000C6C6C600C6C6
      C600C6C6C60000FF000000840000008400000084000000840000008400000084
      000084848400848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C6008484
      840000FFFF0000FFFF0000FFFF0000FFFF0000000000C6C6C600C6C6C600C6C6
      C6000000000084000000FF000000FF000000FF00000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84000000000084000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000000000
      0000848484008400000084000000FF0000000000000000000000C6C6C600C6C6
      C60000FF00000084000000840000008400000084000000840000008400000084
      00000084000084848400848484008484840084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C6008484
      84008484840084848400848484008484840084848400C6C6C600C6C6C600C6C6
      C60084000000FF000000FF000000FF0000008400000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      000084000000FF00000084000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000008400
      00008400000084000000FF000000FFFF00000000000000000000C6C6C600C6C6
      C60000FF00000084000000840000008400000084000000840000008400000084
      0000008400000084000084848400848484008484840084848400C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6008400
      0000FF000000FF000000FF000000840000008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000FF00000084000000FF00000084000000FF000000FF000000FF000000FF00
      0000FF000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      000084000000FF000000FF000000FFFF00000000000000000000C6C6C600C6C6
      C60000FF00000084000000840000008400000084000000840000008400000084
      000000840000008400000084000084848400848484008484840084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C600C6C6
      C6008400000084000000840000008400000084000000C6C6C60084000000FF00
      0000FF000000FF00000084000000848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      000084000000FF00000084000000FF00000084000000FF000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FFFF0000848484000000000000000000C6C6C600C6C6
      C60000FF00000084000000840000008400000084000000840000008400000084
      000000840000008400000084000000840000848484008484840084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00FFFFFF00FFFFFF008400
      0000FF000000FF000000FF000000FF000000FF00000084000000FF000000FF00
      0000FF0000008400000084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000840000008400
      00008400000084000000FF00000084000000FF00000084000000000000008484
      8400000000000000000000000000000000000000000000000000000000008400
      00008400000084000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FFFF0000848484000000000000000000C6C6C600C6C6
      C60000FF00000084000000840000008400000084000000840000008400000084
      0000008400000084000000840000008400000084000084848400848484008484
      8400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400848484008484840084000000FF00
      0000C6C6C600C6C6C600FFFFFF00FFFFFF00C6C6C600FF000000840000008400
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000840000008400
      00008400000084000000840000008400000084000000FF000000000000008484
      840000000000000000000000000000000000000000008400000084000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FFFF000000000000848484000000000000000000C6C6C600C6C6
      C60000FF0000008400000084000000840000008400000084000000FF000000FF
      0000008400000084000000840000008400000084000084848400848484008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF00C6C6C600C6C6C60084000000FF000000C6C6
      C600FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600FF0000008400
      0000C6C6C600C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000840000008400
      0000840000008400000084000000840000008400000000000000848484008484
      84000000000000000000000000000000000000000000FFFF0000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FFFF000000000000848484000000000000000000C6C6C600C6C6
      C60000FF000000840000008400000084000000840000008400008484840000FF
      000000FF00000084000000840000008400000084000000840000848484008484
      840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF008484840084000000FF000000C6C6C600FFFF
      FF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00C6C6C600FF00
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000840000008400
      0000FFFFFF00FFFFFF0084000000840000008400000084848400848484008484
      840000000000000000000000000000000000000000000000000000000000FFFF
      0000FFFF0000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FFFF00008400840000000000848484000000000000000000C6C6C600C6C6
      C600C6C6C60000FF000000840000008400000084000000840000848484008484
      840000FF000000FF000000840000008400000084000000840000008400008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF008484840084000000FF000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FF00
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000840000008400
      0000FFFFFF00FF00FF00FFFFFF00FFFFFF008400000000000000848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FF000000FF000000FF000000FF00
      0000FFFF00008400840000000000848484000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C60000FF000000FF00000084000000840000848484008484
      8400C6C6C60000FF000000FF0000008400000084000000840000008400000084
      0000848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF008484840084000000FF000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00FF00
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084000000FFFF
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FFFFFF00FFFFFF00FFFFFF008484
      8400848484008484840084848400848484000000000000000000000000000000
      000000000000000000000000000084008400FFFF0000FFFF0000FF000000FFFF
      0000840084008400840000000000848484000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000FF000000FF000000840000848484008484
      8400C6C6C600C6C6C60000FF000000FF00000084000000840000008400000084
      000084848400848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF008484840084000000FF000000FFFFFF008484
      840000000000000000000000000000000000000000000000000000000000FF00
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084000000FFFF
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FFFF
      FF00FFFFFF008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000840084008400840084008400FFFF0000FFFF
      0000840084008400840000000000848484000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C60000FF000000FF0000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000FF000000FF000000840000008400000084
      00000084000084848400848484008484840084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF008484840084000000FF000000FFFFFF008484
      8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF00
      000084000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FFFFFF00000000000000000000000000000000000000
      0000000000008484840000000000840084008400840084008400840084008400
      8400840084008400840000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C60000FF000000FF0000008400000084
      00000084000000840000848484008484840084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF0084000000FF000000FFFF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00FF0000008400
      000000000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF008400840084008400000000000000000000000000000000000000
      00008484840000000000FF00FF0084008400FF00FF0084008400840084008400
      8400840084008400840000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000FF000000FF00000084
      0000008400000084000000840000848484008484840084848400C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C60084000000FF00
      0000FFFFFF0084848400848484008484840084848400FF00000084000000C6C6
      C60000000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00840084008400
      8400840084008484840084848400848484008484840084848400848484008484
      8400848484000000000084008400FF00FF0084008400FF00FF0084008400FF00
      FF00840084000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000FF000000FF
      000000840000008400000084000084848400848484008484840084848400C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00C6C6C600C6C6C6008400
      0000FF000000FF000000FF000000FF000000FF00000084000000C6C6C600C6C6
      C60000000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF008484
      8400848484000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000
      0000000000008484840084848400848484008484840084848400848484000000
      000000000000FF00FF00FF00FF0084008400FF00FF0084008400FF00FF008400
      8400FF00FF000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60000FF
      000000FF0000008400000084000000840000848484008484840084848400C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF0084848400FFFFFF00FFFFFF00FFFFFF00FFFF
      FF008400000084000000840000008400000084000000FFFFFF00FFFFFF00FFFF
      FF0000000000C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00000000000000000000000000000000000000000000000000FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF0084008400FF00FF0084008400FF00
      FF00000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000FF000000FF000000840000008400000084000084848400848484008484
      8400C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF00848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FF00FF00FF00FF008400
      8400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF0084008400FF00FF008400
      8400000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C60000FF000000FF0000008400000084000000840000848484008484
      8400C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF00FF0084008400840084008400
      840000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00840084000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60000FF000000FF00000084000000840000848484008484
      8400C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008400840000000000000000000000
      00000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C60000FF000000FF000000840000008400008484
      8400C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600FFFFFF008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C60000FF000000FF000000840000C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF0084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C60084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000084840000848400000000000000
      0000000000000084840000848400000000000000000000000000000000000084
      8400008484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C60000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484000000
      8400000084000000840000008400000084000000840000008400000084008484
      8400848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000000000000000000000000000084840000848400008484000000
      0000000000000084840000000000000000000000000000000000008484000084
      8400008484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000084000000FF
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400C6C6C600C6C6C60000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000840000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      8400000084000000840084848400848484008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000000000000000000000000000084840000FFFF00000000000084
      8400008484000084840000FFFF00000000000084840000848400008484000000
      0000008484000000000000000000000000000000000000000000000000000000
      00000000000084848400000000000000000000FF0000C6C6C60000FF0000C6C6
      C60000FF0000C6C6C6000000000000000000000000000000000000FF000000FF
      000000FF000000FF000000FF000000FF00000084000000840000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      000000000000000000008484840084848400C6C6C600C6C6C600000000008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000084000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000008400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF0000008400000000000000000000000000000000000000000000FFFF000000
      00000000000000848400000000000084840000000000000000000000000000FF
      FF00008484000000000000000000000000000000000000000000000000000000
      00008484840000000000C6C6C60000FF0000C6C6C60000FF0000C6C6C60000FF
      0000C6C6C60000FF000000000000C6C6C6000000000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000840000008400000084
      0000000000008484840000000000000000000000000000000000000000000000
      00000000000000000000848484008484840084848400C6C6C60000000000C6C6
      C600848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000008400000084000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000084000000840084848400848484008484
      8400848484000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF000000840000848400008484000084840000848400000000000000000000FF
      FF000000000000FFFF0000FFFF000000000000FFFF000000000000FFFF000084
      8400000000000084840000848400000000000000000000000000000000000000
      0000000000000000000000840000C6C6C60000FF0000C6C6C60000FF0000C6C6
      C60000FF0000C6C6C600000000000000000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF0000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484008484840000000000FFFFFF00C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF0000008400848484008484
      8400848484000000000000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF0000008400008484000084840000848400008484000000000000FFFF000000
      0000000000000000000000000000000000000000000000FFFF000000000000FF
      FF00000000000084840000848400000000000000000000000000000000008484
      84000084000000000000C6C6C60000840000C6C6C60000FF0000C6C6C60000FF
      0000C6C6C60000000000FFFF0000840000000000000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000084
      0000008400000000000084848400000000000000000000000000000000000000
      000000000000000000000000000000000000848484000000000000000000FFFF
      FF00C6C6C600C6C6C60084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      84000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000084008484
      8400848484008484840000000000000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000084840000FFFF00000000000000000000FFFF00000000000000
      00008484840000000000C6C6C60084848400000000000000000000FFFF000000
      000000FFFF000000000000FFFF00000000000000000000000000000000000000
      0000008400000000000000840000C6C6C60000840000C6C6C600C6C6C6000000
      000084840000FFFF000084000000FFFF0000008400000000000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00C6C6C600C6C6C600848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      84000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000084008484
      8400848484008484840084848400000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000000000000848400000000000000000000000000000000000000
      00008484840000000000C6C6C6008484840000000000000000000000000000FF
      FF00000000000000000000000000000000000000000000000000848484000000
      000000FF000000000000000000000000000000000000C6C6C60000000000FFFF
      0000FFFF0000FFFF0000FFFF00008484000000840000000000000084000000FF
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      000000000000FFFFFF00C6C6C600C6C6C6008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000084000000
      FF000000FF000000FF000000FF000000FF000000FF00000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      8400848484008484840084848400000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000000000000000000FF0000000084840000FFFF00000000000000
      00008484840000000000C6C6C60084848400000000000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000084
      000000FF00008484840000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000084000000840000000000000084
      00000000000084000000FFFF000000000000000000000000000000840000C6C6
      C60000FF0000C6C6C60000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000FFFF
      FF000000000000000000FFFFFF00C6C6C600C6C6C60084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF00000084008484840084848400000000000000000000000000008400000000
      000000FF00000084000000840000000000000000000000008400000000000000
      FF00000084000000000000000000FF000000008484000000000000FFFF000084
      84008484840000000000C6C6C6008484840000000000008484000000000000FF
      FF000000000000000000000000000000000000000000848484000000000000FF
      000000FF000000FF0000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000084000000840000000000000000
      000000000000FFFF000084000000FFFF000000000000C6C6C600C6C6C60000FF
      0000C6C6C60000FF0000C6C6C600000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600FFFFFF000000
      0000FFFFFF000000000000000000FFFFFF00C6C6C600C6C6C600848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      00000000000000000000000000000000FF000000FF000000FF000000FF000000
      FF00000084008484840084848400848484000000000000000000008400000000
      0000000000000000000000000000000000000000000000008400000000000000
      FF00000084000000000000000000FF0000000084840000FFFF00000000000000
      00008484840000000000C6C6C60084848400000000000000000000FFFF000000
      00000000000000000000000000000000000000000000000000000084000000FF
      000000FF000000FF000000FF0000000000008484000084840000000000000000
      000084000000FFFF0000FFFF0000FFFF00000084000000840000008400000000
      00000000000000840000FFFF00008484000000000000C6C6C60000840000C6C6
      C60000FF0000C6C6C60000FF0000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600FFFF
      FF0000000000000000000000000000000000FFFFFF00C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000848484008484
      84008484840000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF00000084008484840084848400848484000000000000000000000000000084
      0000008400000084000000840000000000000000000000008400000000000000
      FF00000084000000000000000000FF0000000000000000848400008484000000
      0000848484008484840084848400848484000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000FF000000FF000000FF000084848400000000000084000000FF00000084
      0000FFFF0000FFFF0000FFFF0000008400000084000000840000008400000084
      0000000000000084000084840000FFFF00008400000000000000C6C6C60000FF
      0000C6C6C60000FF0000C6C6C600000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600FFFFFF0000000000000000000000000000000000FFFFFF00C6C6C6000000
      0000000000000000000084848400848484008484840084848400C6C6C600C6C6
      C60084848400000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF0000000000000000000000
      000000000000000000000000FF000000FF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      8400000000008484840000000000000000000000000000008400000000000000
      FF00000084000000000000000000FF00000000000000FF000000FF0000000000
      0000848484000000000000000000C6C6C6000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000FFFF0000C6C6C600FFFF0000008400000084000000840000008400000084
      000000840000FFFF0000FFFF000084840000FFFF000000000000C6C6C600C6C6
      C60000FF0000C6C6C60000FF0000008400000000000000000000000000000000
      000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600FFFFFF0000000000848484000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      8400000000008484840000000000000000000000000000008400000000000000
      FF00000084000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000000000FFFF
      0000C6C6C600FFFF000000840000FFFF00000084000000840000008400000084
      0000FFFF0000FFFF0000FFFF0000FFFF00008484000000000000C6C6C60000FF
      0000C6C6C60000FF000084000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C6000000000000FFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      8400000000008484840000000000000000000000000000008400000000000000
      0000000000000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000848484000000000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000000000C6C6
      C600FFFF000000840000FFFF0000FFFF0000FFFF0000C6C6C600FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000C6C6C600C6C6
      C60000FF0000C6C6C6000000000000000000000000000000000000000000C6C6
      C600C6C6C60000000000C6C6C600C6C6C60084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C6000000000000FFFF0000000000000000000000
      00000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60084848400000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      8400000000008484840000000000000000000000000000000000000084000000
      8400000084000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      000000FF000000FF000000FF000000FF000000FF000000000000C6C6C600FFFF
      000000840000FFFF0000C6C6C600FFFF0000C6C6C600FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00008484000000000000C6C6C60000FF
      0000C6C6C600840000008484840000000000000000008484840084848400C6C6
      C600C6C6C6000000000000000000C6C6C6008484840084848400C6C6C600C6C6
      C600C6C6C600C6C6C60000000000848484000000000000FFFF00848484000000
      00000000000000000000000000000000FF00C6C6C600C6C6C600C6C6C600C6C6
      C60000000000000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      840000000000848484000000000000000000000000000000000084848400C6C6
      C600000000000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000000000FFFF0000FFFF
      000000840000C6C6C600FFFF000000840000FFFF0000C6C6C600FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000C6C6C6000000
      000000FF00000000000000000000000000000000000084848400848484008484
      8400C6C6C600000000008484840000000000848484008484840084848400C6C6
      C600C6C6C600C6C6C60000000000C6C6C600848484000000000000FFFF008484
      840000000000000000000000000084848400C6C6C600C6C6C600C6C6C6000000
      000000000000000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400848484000000000000000000000000008484
      8400C6C6C600C6C6C6008484840000000000000000000000000084848400C6C6
      C600000000000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000000000FFFF0000FFFF00000084
      000000840000008400000084000000840000C6C6C600FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00008484000084000000000000008484
      8400000000008484840000000000000000000000000084000000848484008484
      840000000000C6C6C60084848400848484000000000084848400848484008484
      8400C6C6C600C6C6C600C6C6C60000000000C6C6C600848484000000000000FF
      FF008484840000000000000000008484840084848400C6C6C600000000000000
      000000000000000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF00000084008484840000000000000000000000000084848400C6C6
      C6000000000000000000C6C6C60084848400000000000000000084848400C6C6
      C600000000000000000000000000FF00000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000008400000084
      00000084000000840000008400000084000000840000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C600C6C6C600848484008484840000000000848484008484
      840084848400C6C6C600C6C6C600C6C6C60000000000C6C6C600848484000000
      000000FFFF008484840000000000000000008484840000000000000000000000
      000000000000000000000000000000000000000084000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF0000000000000000000000
      000000000000000000000000FF000000FF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000840084848400000000008484840084848400C6C6C6000000
      0000000000000000000000000000C6C6C600848484000000000084848400C6C6
      C600000000000000000000000000FF000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C60000000000FFFF0000FFFF0000008400000000
      0000008400000084000000840000008400000084000000840000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00C6C6C600C6C6C600C6C6C6008484840084848400000000008484
      84008484840084848400C6C6C600C6C6C6008484840000000000C6C6C6008484
      84000000000000FFFF0084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF00000084008484840084848400000000008484840000000000848484000000
      000000000000000000000000000000000000848484000000000000000000C6C6
      C60000000000000000000000000000000000FF000000FF000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C6000000000000000000000000000084
      0000000000000084000000840000FFFF0000008400000084000000840000FFFF
      0000FFFF000000000000000000008400000000FF0000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00C6C6C600C6C6C600C6C6C60084848400848484000000
      0000848484008484840084848400C6C6C600848484000000000000000000C6C6
      C600848484000000000000FFFF00848484000000000000000000000000000000
      00000000000000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      00000000000000000000000000000000FF000000FF000000FF000000FF000000
      FF00000084008484840000000000000000008484840000000000848484000000
      000000000000000000000000000000000000848484000000000000000000C6C6
      C6008484840000000000000000000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400000000000000000000FF000000FF000000FF
      0000000000000084000000840000FFFF0000FFFF000000840000FFFF0000FFFF
      0000C6C6C600000000000000000000FF0000C6C6C60000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C600848484008484
      8400000000008484840084848400848484000000000000000000000000000000
      000000000000000000000000000000FFFF008484840000000000000000000000
      00000000000000000000000000000000000000000000000084000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF00000084000000000000000000000000008484840000000000C6C6C6008484
      8400000000000000000000000000C6C6C600848484000000000000000000C6C6
      C6008484840000000000000000000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000FF000000FF000000FF
      000000FF0000000000000084000000840000FFFF0000FFFF0000FFFF0000C6C6
      C600000000000000000000FF0000C6C6C60000FF0000C6C6C60000FF00008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C6008484
      8400848484000000000084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0084848400000000000000
      0000000000000000000000000000000000000000000000000000000084000000
      FF000000FF000000FF000000FF000000FF000000FF00000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      840084848400000000000000000000000000000000008484840000000000C6C6
      C6000000000000000000C6C6C600C6C6C600000000008484840000000000C6C6
      C6008484840000000000000000000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000084000000FF000000FF000000FF000000FF
      000000FF000000FF000000000000FFFF0000FFFF0000FFFF0000000000000000
      00000084000000FF0000C6C6C60000FF00000000000084000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6
      C600848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      84000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000084008484
      8400000000000000000000000000000000000000000000000000848484000000
      0000000000008484840000000000000000000000000084848400000000000000
      00008484840000000000000000000000000000000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000848484000000000000FF000000FF000000FF000000FF
      000000FF000000FF00000000000000000000000000000000000000000000C6C6
      C60000FF0000C6C6C60000FF0000840000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00C6C6C600C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      0000FF00FF000000000000000000000000000000000000000000000000000000
      84000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000084000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000084848400000000000000000084848400848484008484
      84008484840000000000000000008484840084848400C6C6C600848484008484
      8400000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000FF000000FF000000FF
      000000FF000000FF000000FF000000000000C6C6C60000840000C6C6C60000FF
      0000C6C6C60000FF0000C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600C6C6C600C6C6C60084848400848484000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      FF00FF00FF000000000000000000000000000000000000000000000000000000
      0000000084000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF0000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084848400C6C6
      C600C6C6C6008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484000000000000FF000000FF000000FF
      000000FF000000FF000000FF00000084000000000000C6C6C60000FF0000C6C6
      C60000FF0000C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00C6C6C600C6C6C600C6C6C600848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000008400000084000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000C6C6C6000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000084000000FF000000FF
      000000FF000000FF000000FF000000FF00000000000000840000C6C6C60000FF
      0000C6C6C6000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00C6C6C600C6C6C600848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000008400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084848400000000000084000000FF
      000000FF000000FF000000FF000000FF0000008400000000000000FF0000C6C6
      C600000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000840000008400000084000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484000000000000000000848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000FF000000FF00
      0000FFFF0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF0000FF00
      0000FFFF0000FF000000FFFF0000FF000000FFFF0000FF000000000000000000
      000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF000000FF000000FF000000
      0000848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484000000
      00000000000000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF
      0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF000000000000FFFF
      0000FFFF000000000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000000000000084848400848484008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      84000000000000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FFFF0000FF000000FFFF0000FF000000FFFF0000FF00
      0000FFFF0000FF000000FFFF0000FF000000FFFF0000FF00000000000000FFFF
      0000FFFF00008484000000000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000000000848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400848484008484840084848400848484008484
      8400848484008484840000008400000084000000840000008400848484008484
      8400848484008484840084848400848484008484840084848400000000000000
      0000000000000000000000000000000000008484840000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600848484008484
      84008484840000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FFFF0000FF000000FFFF0000FF000000FFFF
      0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF000000000000FFFF
      0000FFFF0000848400008484000000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000000008484840084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      000000000000848484008484840084848400C6C6C6008484840084848400C6C6
      C600C6C6C600C6C6C6000000840000FFFF0000FFFF0000008400C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484000000
      0000000000000000000000000000000000008484840000000000C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400848484008484840084848400C6C6C600C6C6C600848484008484
      84008484840000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FFFF0000FF000000FFFF0000FF00
      0000FFFF0000FF000000FFFF0000FF000000FFFF0000FF00000000000000FFFF
      0000FFFF000084840000848400008484000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      000000000000C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000084000000000084848400848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600000084000000840000FFFF0000FFFF000000840000008400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400000000000000000000000000000000008484840000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C6C600C6C6C600848484008484
      84008484840000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FFFF0000FF000000FFFF
      0000FF000000FFFF0000FF000000FFFF0000FF000000FFFF000000000000FFFF
      0000FFFF00008484000084840000848400008484000000000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000084000000000084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF00C6C6
      C6000000840000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000
      8400FFFFFF00C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400000000000000000000000000000000008484840000000000C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      840084848400848484008484840084848400C6C6C600C6C6C600848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000084000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF000000
      840000FFFF0000FFFF000000840000FFFF0000FFFF000000840000FFFF0000FF
      FF0000008400C6C6C600C6C6C600C6C6C600C6C6C60084848400848484008484
      8400848484000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF000000
      840000FFFF00000084000000840000FFFF0000FFFF000000840000FFFF0000FF
      FF0000008400FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      8400848484000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000084000000FF000000840000C6C6C600C6C6C600848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF000000FF000000FF000000
      FF00000000008484840000008400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400C6C6C600C6C6C600FFFFFF00FFFFFF000000
      840000008400000084000000840000FFFF0000FFFF000000840000FFFF0000FF
      FF0000008400FFFFFF00C6C6C600C6C6C600C6C6C60084848400848484008484
      8400848484000000000000000000000000008484840000000000C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C60084000000840000000000
      000000FFFF0000FFFF000000FF0000FFFF0000FFFF0000FFFF000000FF000000
      FF00000000008484840000008400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF00FFFF
      FF0000008400000084000000840000FFFF0000FFFF0000FFFF0000FFFF0000FF
      FF0000008400FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C600848484008484
      8400848484000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084848400848484008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C600840000000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF00000000008484840000008400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600C6C6C600FFFFFF000000
      84000000840000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000
      8400FFFFFF00FFFFFF00C6C6C600C6C6C600C6C6C60084848400848484008484
      8400848484000000000000000000000000000000000084848400C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084848400848484008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C600840000000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000000008400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF00FFFFFF000000
      840000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000008400000084000000
      840000008400FFFFFF00C6C6C600C6C6C600C6C6C600C6C6C600848484008484
      840084848400000000000000000000000000000000008484840084848400C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084840000000000000000
      000084848400848484008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C600C6C6C60084000000C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF000000
      840000FFFF0000FFFF000000840000FFFF0000FFFF00000084000000840000FF
      FF0000008400FFFFFF00C6C6C600C6C6C6008484840084848400848484008484
      8400848484000000000000000000000000008484840084848400C6C6C6008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084840000848400000000
      000084848400848484008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000000000000000000008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000084848400000084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400C6C6C600C6C6C600C6C6C600FFFFFF000000
      840000FFFF0000FFFF000000840000FFFF0000FFFF000000840000FFFF0000FF
      FF0000008400C6C6C600C6C6C600C6C6C600C6C6C60084848400848484008484
      84000000000000000000000000000000000084848400C6C6C60084848400C6C6
      C60084848400C6C6C6008484840000000000C6C6C60000000000C6C6C6008484
      8400848484008484840084848400FFFF00008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000848400008484
      000000000000848484008484840084848400848484008484840000000000FFFF
      0000FFFF00000000000084848400000000008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084848400848484008484840084848400C6C6C600C6C6C600C6C6C600FFFF
      FF000000840000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000
      8400C6C6C600C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400000000000000000000000000000000008484840084848400C6C6C6008484
      8400C6C6C60084848400C6C6C6008484840000000000C6C6C600848484000000
      0000000000000000000000000000848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF00008484
      000084840000000000008484840084848400848484008484840000000000FFFF
      0000FFFF000000000000FFFFFF00000000008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000848484008484840084848400C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600000084000000840000FFFF0000FFFF000000840000008400C6C6
      C600C6C6C600C6C6C600C6C6C600848484008484840084848400848484000000
      00000000000000000000000000000000000084848400C6C6C60084848400C6C6
      C60084848400C6C6C60084848400C6C6C6008484840084848400000000000000
      0000000000000000000084848400000000008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400000000008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000084840000848400000000000084848400848484008484840000000000FFFF
      0000FFFF00008484000000000000000000008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000FF000000
      FF000000FF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000848484008484840084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C6000000840000FFFF0000FFFF0000008400C6C6C600C6C6
      C600C6C6C600C6C6C60084848400848484008484840084848400000000000000
      0000000000000000000000000000000000008484840084848400C6C6C6008484
      8400C6C6C60084848400C6C6C60084848400C6C6C60084848400000000000000
      0000000000000000000084848400000000008484840084848400848484008484
      8400848484008484840000000000848484000000000084848400000000008484
      8400848484008484840084848400000000000000000084840000848400008484
      00008484000084840000848400008484000084840000FFFF0000FFFF0000FFFF
      0000FFFF0000848400008484000000000000848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000000000FF000000
      FF000000FF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084848400848484008484840084848400848484008484
      8400C6C6C600C6C6C60000008400000084000000840000008400C6C6C600C6C6
      C600C6C6C6008484840084848400848484008484840084848400000000000000
      00000000000000000000000000000000000084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C60084848400000000000000
      0000000000000000000084848400000000008484840084848400848484008484
      8400848484008484840000000000848484008484840084848400848484008484
      84008484840084848400848484000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF00008484000084840000000000008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000FFFF0000FFFF00000000000000000000FFFF0000FFFF0000FFFF0000FF
      FF000000FF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000848484008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400848484008484840000000000000000000000
      00000000000000000000000000000000000084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600FFFF000084848400000000000000
      0000000000000000000084848400000000008484840084848400848484008484
      8400848484008484840084848400000000000000000084848400848484008484
      84008484840084848400848484000000000000000000FFFF0000FFFF0000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFF0000FFFF000084840000000000008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000FF
      FF0000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF0000FFFF000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600FFFF0000C6C6C600C6C6C600848484000000
      0000000000000000000000000000848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      FF00FFFF0000FFFF00008484000000000000848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6000000000000000000000000000000
      000000FFFF0084848400000000000000000000000000000000000000FF000000
      0000000000000000000084848400000084000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084848400C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFF0000C6C6C600C6C6C60000FF000000FF00008484
      8400848484008484840084848400C6C6C6008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFFFF00FFFF
      0000FFFF0000848400000000000084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600C6C6C600C6C6C6000000000000000000C6C6C600C6C6C600C6C6C6000000
      000000FFFF000000000084848400848484008484840084848400000000000000
      0000C6C6C600C6C6C600C6C6C600C6C6C6000000840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400C6C6C600C6C6
      C600FFFF0000FFFF0000C6C6C600C6C6C60000FF000000FF0000FFFF000000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C6008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000084840000000000008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      840000000000000000000000000000000000000000000000000000000000C6C6
      C600000000000000000084000000840000008400000084000000840000008400
      0000000000008484840084848400848484008484840084848400848484000000
      000000008400000084000000840000008400C6C6C60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400FFFF0000FFFF
      0000FFFF0000C6C6C600C6C6C60000FF000000FF0000FFFF0000FFFF000000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C6008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF00008484
      000000000000848484008484840084848400848484008484840000000000FFFF
      0000FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6008484840000000000000000000000000000000000840000000000
      00000000000084848400C6C6C600C6C6C600C6C6C60084848400848484000000
      0000000000000000000000008400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400848484008484840084848400C6C6C60084848400C6C6C600848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084848400FFFF
      0000C6C6C600C6C6C60000FF000000FF000000FF0000FFFF0000FFFF000000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C6008484840084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000848400000000
      000084848400848484008484840084848400848484008484840000000000FFFF
      FF00FFFF00008484000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000C6C6C6000000000084848400848484008484840084848400000000000000
      000000000000C6C6C6000000000000000000C6C6C60084848400848484000000
      0000848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      84008484840084848400C6C6C600FFFFFF0084848400FFFFFF00848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084848400C6C6
      C600C6C6C60000FF000000FF000000FF0000FFFF0000FFFF0000FFFF000000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084840000000000000000
      0000848484008484840084848400848484008484840084848400848484000000
      0000FFFFFF00FFFF000084840000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000008484840084848400848484008484840084848400848484000000
      000000000000C6C6C6000000000000000000C6C6C60084848400848484000000
      0000848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400C6C6C60084848400FFFFFF00FFFFFF0084848400FFFFFF00C6C6C6008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      840000FF000000FF000000FF000000FF0000FFFF0000FFFF0000FFFF000000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      840000000000FFFFFF00FFFF0000848400008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      00000000000084848400C6C6C600C6C6C600C6C6C60084848400848484000000
      00000000000000000000C6C6C600C6C6C600848484008484840000000000C6C6
      C600C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      840084848400C6C6C600FFFFFF0084848400FFFFFF00C6C6C600848484008484
      8400848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008484840000FF000000FF0000FFFF0000FFFF0000FFFF000000FFFF0000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      84008484840000000000FFFFFF00FFFF00008484000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6000000000000000000C6C6C60084848400848484000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400848484008484840084848400848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000848484008484840084848400FFFF0000FFFF000000FFFF0000FF
      FF0000FFFF00C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      8400848484008484840000000000FFFFFF00FFFF000084840000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      000000000000C6C6C6000000000000000000C6C6C60084848400848484000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      0000C6C6C6008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400000000008484840084848400848484008484840084848400848484000000
      0000848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000084848400848484008484840000FFFF0000FF
      FF0000FFFF00C6C6C600C6C6C600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840084848400848484008484
      840084848400848484008484840000000000FFFFFF00FFFF0000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6C6C600C6C6C6008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600848484008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000084848400000000000000000084848400000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000080000000400100000100010000000000001400000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFFFFFFFFF003FFFF7FFFF
      FFF001FFFFFFFFFFFFFF003FFFE3FFFFFFE000FFC000003FFFFF003FFFC1FFFF
      FFC0007FF000003FFFFF807FFF80FFFFFF80003FFFFFF7DFFFFF007FFF007FFF
      FF80001FFFFFE80FFFFC007FFE003FFFFF80001FFFFFD38FFFF0007FFC001FFF
      FF80000FFFFFA78FFFE0007FF8000FFFFF800007FFFF4F8FFFC0007FF00007FF
      FF800007FFFE9F4FFF80007FE00003FFFFC00003FFFD1E9FFF00007FC00001FF
      FFC00003FFFA4D3FFE00207FC00000FFFE000003FFF49A7FFE00F07F8000007F
      F8000003FFE934FFFC00F07FC000003FF0000003FFD268FFC0000003E000001F
      E0000003FFA4D27F80000001F000000FC0000003FF49A4FF00000000F8000007
      C0000003FE9349FF80000001FC000003C0000007FD2693FFF000000FFE000001
      C0000007FA4D27FFF000000FFC000001C000000FF49A4FFFF000000FF8000000
      C000000FE8B49FFFF000000FF8000001C000001FEAA93FFFF000000FF0000003
      C000003FEAD27FFFF000000FF0000007C000007FEB64FFFFF000000FC000000F
      E00000FFEB89FFFFF000000F8000001FF00001FFE9D3FFFFF000000F0000003F
      F80003FFF427FFFFF000000F0001007FFC0007FFF80FFFFFF000000FE00380FF
      FE001FFFFC1FFFFFF000000FE007C1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE3FF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFF80000001FFFFFFFFF801FC7F
      FFE1DFFF00000000FFF800070000FC1FFFC00FFF40000000FFF800078000780F
      FF8007FF60000000FFF80000C0003803FF0003FF60000000F0000000E0001000
      FE0001FF00000000F0000000F003F000FC00007F60000000F0000000FFFFE000
      F800001F40000000F000000080000000F0000007400000000000000000000000
      E0000000400000000000000000000000C0000000600040000000000000000000
      840405416380600000000000000000002A0002A16040C0000000000000000000
      94000550602080000000000100000000CA0082AB001000000000003F80000000
      E70087F5600000000000043FC0000000F38007F3600000000000283FE0000000
      F9C007CF000001300000143FF0000000FCA0013F600002000000283FF0000000
      FE6000FF600004008000547FF00000E0FF2001FF00000400FCAAA9FFF00000F0
      FF8001FF60040000FD5555FFF00000FFFFC001FF60024000FCAAA9FFF00000FF
      FFE000FF00018000FD5155FFF00000FFFFF0007F60000000FE0403FFF00000FF
      FFF1003F60000000FF087FFFF00000FFFFF8001F00000000FE157FFFF00000FF
      FFFC001F6000000000A47FFFF00000FFFFFE009F6000000001417FFFF00000FF
      FFFE441F7FFFFFF842AAFFFFF80000FFFFFE063F7FFFFFFCA555FFFFFC0000FF
      FFFF0FFF800000010003FFFFFE0001FFFFFFFFFFFC000FFF87FFF1FFFFF807FF
      CFFFFFFFF80007FFE3FFE0FFFFE003FFC7FFFFFFFA0047FFF0FFE0FFFF8000FF
      E3FFFFFFFA0047FFF03F20FFFF80003FE1FFFFFFFA0047FFF01E00FFFF80000F
      F0FFFFFFFA0007FFF80C00FFFF80000FF07FFFFFFA0007FFFA0000FFFF80003F
      F83FFFFFFA000001F80020FFFF8000FFF81FFFFFFB7F8000FD0060FFFF8003FF
      FC0FFFFFFB3FC080FD80E0FFFF800FFFFC07FFFFC01FC100FE0060FFFF801FFF
      FE03FFFF800FC780FF0020FFFF801FFFFE01FFFF801FC000FF8000FFFF800FFF
      FF00FFFF9B3FC738FFC000FFFF000FFFFC007FFF9B7FC10CFFE0003FFF000FFF
      FC003FFFCC000114FFE0001FFF000FFFFE001FFFE7A10104FFC0001FFF000FFF
      FE000FFF80200000FFC0001FFF000FFFFF007FFF00200000FFC0001FFF030FFF
      FF003FFF40200000FFC1001FFF0C0FFFFF801FFF5FBFFFFCFFE0001FFF300FFF
      FF800FFF40000001FFE0000FFF401FFFFFC007FF5FFFFE07FFF02007FF807FFF
      FFC003FF40000007FFF8C003FFC03FFFFFE001FF40000007FFF80001FFC03FFF
      FFE000FF40000007FFFC0000FFCC3FFFFFF0007F40000007FFFE0000FFCC3FFF
      FFF0003F7FFFFF87FFFF0008FFE07FFFFFF8001F80000007FFFF3080FFF0FFFF
      FFF8000FC0000007FFFF30C1FFFFFFFFFFFC0007E000000FFFFF81E3FFFFFFFF
      FFFC0003FFFFFFFFFFFFC3FFFFFFFFFF8000000180000001800000FFFFFFFFFF
      00000000800000010000007FE000000300000000800000010000007FC0000003
      00000000800000010000003FD5555543000000008000000102800000C0000023
      0000000080000001050FFFFED000000300000000800000010A0FFFFCC0000F23
      0000000080000001051FFFFDD0000F0300000000800000010A1FFFFBC0000F23
      0000000080000001043C1E33D0000003000000008000000108300C37C0000023
      000000008000000104708C67D0000003000000008000000108F3186FC03C0F23
      000000008000000100FF18CFD0000003000000008000000101FC30DFC0000023
      000000008000000101E0719FD007FF03000000008000000103F8E1BFC0000023
      00000000800001010310A33FD007FF0300000000800002010631077FC0000023
      00000000800004000603077FD007FF03000000008000080007078E7FC0000023
      00000000800010000FFFFEFFD007FF0300000000800020000FFFFCFFC0000003
      00000000800040000FFFFDFFD000000300000000C0000000000001FFCAAAA803
      00000000E0000000000001FFD000006300000000F0000000000001FFC000018B
      00000000F8000000000003FFD000060300000000FC000000000003FFC00008AB
      00000000FE000000000003FFD555000300000000FF000000000003FFC0001FFF
      80000001FFFFFE00800007FFFFFFFFFF8000000000000000FFFFFFFFFF9FFFFF
      0000000000000000E0001FFFFF03F8C30000000000000000C000000FFE01F001
      000000000000000080000007FC01E000000000000000000080000003F800E000
      000000000000000080000001F0006000000000000000000000000000E0003000
      000000000000000000000000C000188100000000000000000000000380000FC3
      000000000000000000000003C00007FF0C0002CC0000000000000001E00003FF
      000000000000000000000001E00001E3000000000000000000000001C00000C1
      00000000000000000000000180000041000000000000000000000001C0000001
      000000000000000080000001E00000030000000000000000C0000001F000000F
      0000000000000000E0000001F80000070000000000000000F0000001FC000007
      0000001000000000F8000001FE00000F0000000000000000FF8000FFFF00001F
      0000000000000000FFC0007FF180000F0000000000000000FFC0007FE0C00007
      0000000000000000FFE0003FC06000070000000000000000FFE0003FC070000F
      0000000000000000FFFFFFFFC078001F0000000000000000FFFFFFFFE0FC003F
      0000000000000000FFFFFFFFF18E007F0000000000000000FFFFFFFFFF0700FF
      0000000000000000FFFFFFFFFF0781FF0000000000000000FFFFFFFFFF07C3FF
      0000000000000000FFFFFFFFFF8FE7FFFFFFFFFFC000001FFFFCFFFFFC000003
      FFFFFFFFC000001FFFF03FFFF8000003FFF1FFFFC0000001FFC00FFFF0000003
      FFC07FFFC0000001FF0003FFE0000003FF001FFFC0000001FC0000FFC0000003
      FC0007FFC0000001F000003FC0000003F00001FFC0000001C000000FC0000003
      C000007FC00000018000000FC00000038000001FC00000010000000FC0000003
      8000000FC00000010000000FC00000038000000FC00000010000000FC0000003
      0000000FC00000010000000FC000000300000007C00000010000000FC0000003
      00000003C00000010000000FC000000300000001C00000010000000FC0000003
      00000000C00000010000003FC000000300000000FE000001C00000FFC0000003
      00000003FE000001F00001FFC000000300000007FE000001FC0001FFC0000003
      C0000007FE000001FC0001FFC0000003F000001FFE000001FC0001FFC0000003
      FC00007FFE000001FC0001FFC0000003FF0001FFFE000001FC0001FFC0000003
      FFC000FFFE000001FC0001FFC0000003FFF0007FFE000001FC0001FFC0000003
      FFFC003FFE000001FC0001FFC0000003FFFE000FFE000003FC0001FFC0000003
      FFFF000FFE000007FC0007FFC0000003FFFF803FFE00000FFC001FFFC0000007
      FFFFC0FFFE00001FFC007FFFC000000FFFFFE3FFFE00003FFF01FFFFC000001F
      FFFFFFFFFE00007FFFC7FFFFC000003F80000001FFFFFFFFFFFFFFFFFFFFFFFF
      00000000FFFFFFFFFF807F00FFF807FF40000000F80001FFFF807F00FFE000FF
      40000000F80000FFFF807F00FF80003F40000000F800007FFF907F20FF00001F
      40000000F800003FFF907F20FC00000F40000000F800001FFF900820F8000004
      40000000F800001FFF800800F000000040000000F800001FFF800000E0000000
      40000000F800001FFF880100C000000040000000F800001FFF880100C0000000
      40000000F800001FFF880100C007E00040000000F800001F3FC80101C00FE000
      40000000F800001F1FE00003800F800040000000F800001FA7E00003800F8000
      40000000F800001FD1F20907800FE00040000000F800001FE87008078003F800
      40000000F800001FF41008078000FC0040000000F800001FFA043E1F8000FC00
      40000000F800001FFD01BEDF8001F80140000000F800001FC081BEDF8001F001
      40000000F800001FE8001E1F8000000340000000F800001FF40007FF80000003
      40000000F800001FFA0001FF0000000740000000F800001FFD01FFFF00000007
      40000000F800001FFE807FFF0000000F40000000F800001FFF401FFF7800001F
      40000000FC00001FFFA007FFFC00003F40000000FE00001FFFD001FFFF0000FF
      40000000FF00001FFFE8007FFF8001FF7FFFFFFCFF80001FFFF0003FFFF00FFF
      80000001FFFFFFFFFFFFFFFFFFFFFFFFE1FFF8FFFFFFE7FFFFFFFFFFFFF807FF
      D0FF38E7FFFFC7FFFE1FFFFFFFC000FFD0C618C3FFFFC7FFFC8FFFFFFF80007F
      D0A20203FE01801FF847FFFFFE00001FD0A20013F8018007F803FFFFFC00000F
      D0A04221F0000003F801FFFFF8000007D0A02940F0000003FC00FFFFF0000007
      D0A05020E0000001FE207FFFE0000003D0A1A414E0000001FF703FFFE0000001
      D0A05441C0000000FED81FFFC0400801D0A22417C1800080FDAC0FF880E01C01
      D0A2442780000000FB1607C081F03E00DEA2241780000000F60B040080F87C00
      E1A2804F80000000EC008000007CF800E9A2867F80000000D8000000003FF000
      E9A28FFF80000001B0000000001FE000E9BA8FFF80000001A0000000000FC000
      E9C68FFFC000000100000400000FC000E9C68FFF800000030000000F001FE000
      C0C68FFF000000230000001F003FF0018C468FFF0000003F0000003F007CF801
      12066FFFFC00003FB000007F80F87C014D071FFFFE00061FD800003F81F03E03
      4D231FFFFC00041FEC00601F80E01C0742231FFFFC00080FF600FE0FC0400807
      A4231FFFFC00200FFB01FF07E000000FD2B20FBFFC01004FFD80FF83E000001F
      ED80003FFE0000FFFEC07FC3F000003FFFC000BFFE0001FFFF607FE3F800007F
      FFD6017FFF0001FFFFB07FFFFE0001FFFFD9FEFFFF0003FFFFD0FFFFFF0003FF
      FFE600FFFF800FFFFFE1FFFFFFE01FFF00000000FFFE01FFFFFFFFFF8000001F
      00000000FFF800FFFF8001FF0000000F00000000FFE0003FFE00007F40000007
      00000000FE00000FFC00003F5FFFFF0300000000F8000003F800001F40000003
      00000000E0000003F000000F5FFFFF0300000000E000000FF000000F40000003
      00000000E000003FE000000740000003FFE0000FE000007FE000000740000003
      FFE0000FE000007FE000000740000000FFE0000FE000007FE00000077FFF0000
      FF60000FE000007FE000000780000040FF20000FE000007FE000000780000040
      FF00000FE000007FE000000700000040FF00000FC000007FF000000F01400000
      FF00000FC000003FF000000F008000C0FF00000FC000003FF800001F001C0020
      0000000FC000003FF800001F001C00A00000000FC000C03FFC00003F001C0000
      0000000FC003003FFE00007F001C00000000000FC00C003FFF8001FF00007FFE
      0000000FC0C0043FFFE007FF00007FFE0000000FC300103FFFF00FFF80007FFE
      FF00000FCC00003FFFF00FFF80007FFEFF00000FE000007FFFE007FFC0000000
      FF00000FE01301FFFFE007FFC0000AFFFF00000FF00300FFFFC003FFE00005FF
      FF20000FF00800FFFF8003FFF0000BFFFFE0000FF30C30FFFF8001FFF80027FF
      FFE0000FF30F30FFFF8811FFFE011FFFFFE0000FF81F81FFFFDDBBFFFFC0FFFF
      FFE0001FFC3FC3FFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object IBUpdateSQL: TIBUpdateSQL
    RefreshSQL.Strings = (
      'select * FROM PRCSS_LOG')
    Left = 368
    Top = 8
  end
  object PrintDialog: TPrintDialog
    Left = 160
    Top = 13
  end
  object tblCardMaster: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblCardMasterAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblCardMasterBeforeDelete
    BeforeInsert = tblCardMasterBeforeInsert
    BeforePost = tblCardMasterBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'CARD_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'CARD_NAME'
        DataType = ftString
        Size = 25
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY30'
        Fields = 'CARD_ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'CARD_MASTER'
    Left = 144
    Top = 337
    object tblCardMasterCARD_ID: TIntegerField
      DisplayLabel = 'Card ID'
      DisplayWidth = 15
      FieldName = 'CARD_ID'
    end
    object tblCardMasterCARD_NAME: TIBStringField
      DisplayLabel = 'Card Name'
      DisplayWidth = 59
      FieldName = 'CARD_NAME'
      Size = 25
    end
  end
  object tblCardDetail: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblCardDetailAfterInsert
    AfterPost = tblSubsAfterPost
    BeforeDelete = tblCardDetailBeforeDelete
    BeforePost = tblCardMasterBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'CARD_DETAIL_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'CARD_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'FIELD_NAME'
        DataType = ftString
        Size = 25
      end
      item
        Name = 'HORIZONTAL'
        DataType = ftSmallint
      end
      item
        Name = 'VERTICAL'
        DataType = ftSmallint
      end
      item
        Name = 'ROTATION'
        DataType = ftSmallint
      end
      item
        Name = 'STYLE'
        DataType = ftSmallint
      end
      item
        Name = 'PROPORTION'
        DataType = ftSmallint
      end
      item
        Name = 'SIZE'
        DataType = ftSmallint
      end
      item
        Name = 'GRAPHIC_MODE'
        DataType = ftSmallint
      end
      item
        Name = 'TEXT_POSITION'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_TEXT'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'SIZE_REDUCE'
        DataType = ftSmallint
      end
      item
        Name = 'TEXT_LENGTH'
        DataType = ftSmallint
      end
      item
        Name = 'MULTI_GAP'
        DataType = ftSmallint
      end
      item
        Name = 'EXPORT_LINES'
        DataType = ftSmallint
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY2'
        Fields = 'CARD_DETAIL_ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'CARD_DETAIL'
    Left = 184
    Top = 353
    object tblCardDetailFIELD_NAME: TIBStringField
      DisplayLabel = 'Field Name'
      DisplayWidth = 15
      FieldName = 'FIELD_NAME'
      Size = 25
    end
    object tblCardDetailCARD_TEXT: TIBStringField
      DisplayLabel = 'Card Text'
      DisplayWidth = 27
      FieldName = 'CARD_TEXT'
      Size = 40
    end
    object tblCardDetailTEXT_POSITION: TIBStringField
      DisplayLabel = 'Text Pos'
      DisplayWidth = 8
      FieldName = 'TEXT_POSITION'
      Size = 1
    end
    object tblCardDetailHORIZONTAL: TSmallintField
      DisplayLabel = 'Horizontal'
      DisplayWidth = 11
      FieldName = 'HORIZONTAL'
    end
    object tblCardDetailVERTICAL: TSmallintField
      DisplayLabel = 'Vertical'
      DisplayWidth = 10
      FieldName = 'VERTICAL'
    end
    object tblCardDetailSTYLE: TSmallintField
      DisplayLabel = 'Style'
      DisplayWidth = 7
      FieldName = 'STYLE'
    end
    object tblCardDetailSIZE: TSmallintField
      DisplayLabel = 'Size'
      DisplayWidth = 6
      FieldName = 'SIZE'
    end
    object tblCardDetailPROPORTION: TSmallintField
      DisplayLabel = 'Proportion'
      DisplayWidth = 12
      FieldName = 'PROPORTION'
    end
    object tblCardDetailROTATION: TSmallintField
      DisplayLabel = 'Rotation'
      DisplayWidth = 21
      FieldName = 'ROTATION'
    end
    object tblCardDetailGRAPHIC_MODE: TSmallintField
      DisplayLabel = 'Graphic Mode'
      DisplayWidth = 18
      FieldName = 'GRAPHIC_MODE'
    end
    object tblCardDetailSIZE_REDUCE: TSmallintField
      DisplayWidth = 10
      FieldName = 'SIZE_REDUCE'
    end
    object tblCardDetailTEXT_LENGTH: TSmallintField
      DisplayWidth = 10
      FieldName = 'TEXT_LENGTH'
    end
    object tblCardDetailCARD_DETAIL_ID: TIntegerField
      DisplayWidth = 15
      FieldName = 'CARD_DETAIL_ID'
      Required = True
      Visible = False
    end
    object tblCardDetailCARD_ID: TIntegerField
      DisplayWidth = 6
      FieldName = 'CARD_ID'
      Required = True
      Visible = False
    end
    object tblCardDetailMULTI_GAP: TSmallintField
      FieldName = 'MULTI_GAP'
      Visible = False
    end
    object tblCardDetailEXPORT_LINES: TSmallintField
      FieldName = 'EXPORT_LINES'
      Visible = False
    end
  end
  object dsrCardMaster: TDataSource
    DataSet = tblCardMaster
    OnStateChange = dsrCardMasterStateChange
    Left = 144
    Top = 329
  end
  object dsrCardDetail: TDataSource
    DataSet = tblCardDetail
    OnStateChange = dsrCardDetailStateChange
    Left = 184
    Top = 337
  end
  object qryQuery: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    Left = 328
    Top = 384
  end
  object dsrQuery: TDataSource
    DataSet = qryQuery
    Left = 360
    Top = 392
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.*'
    Filter = 'All Files |*.*'
    InitialDir = 'c:\'
    Title = 'Add file attachments'
    Left = 152
    Top = 8
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    FileName = 'Member.csv'
    Filter = 'CSV Files|*.csv|All Files |*.*'
    InitialDir = 'c:\My Documents'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save To'
    Left = 163
    Top = 16
  end
  object dsrAttachSubs: TwwDataSource
    DataSet = qryAttachSubs
    Left = 24
    Top = 408
  end
  object qryAttachSubs: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      
        'SELECT MEMBER ,SUBSCRIPTION, AMOUNT_PAID, AMOUNT,PAID,EDIT_DATE,' +
        'RECEIPT      '
      'FROM  M_SUBS'
      'WHERE MEMBER in ( :Member )'
      '')
    UniDirectional = True
    Left = 37
    Top = 408
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Member'
        ParamType = ptUnknown
      end>
    object qryAttachSubsMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 10
      FieldName = 'MEMBER'
      Required = True
    end
    object qryAttachSubsSUBSCRIPTION: TIntegerField
      DisplayLabel = 'Subscription'
      DisplayWidth = 10
      FieldName = 'SUBSCRIPTION'
      Required = True
    end
    object qryAttachSubsDescription: TStringField
      DisplayWidth = 24
      FieldKind = fkLookup
      FieldName = 'Description'
      LookupDataSet = tblSubs
      LookupKeyFields = 'SUBSCRIPTION'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'SUBSCRIPTION'
      Size = 30
      Lookup = True
    end
    object qryAttachSubsRECEIPT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 8
      FieldName = 'RECEIPT'
    end
    object qryAttachSubsAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 12
      FieldName = 'AMOUNT_PAID'
      currency = True
      Precision = 9
      Size = 2
    end
    object qryAttachSubsAMOUNT: TIBBCDField
      DisplayLabel = 'Amount'
      DisplayWidth = 10
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object qryAttachSubsPAID: TIBStringField
      FieldName = 'PAID'
      Visible = False
      Size = 1
    end
    object qryAttachSubsEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Visible = False
    end
  end
  object qryGeneral1: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    Transaction = IBTransaction
    Left = 32
    Top = 368
  end
  object ibdsBulkCardRec: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    SelectSQL.Strings = (
      
        'select pd.AMOUNT,pd.AMOUNT_PAID,pd.DESCRIPTION,pd.EXPIRES, pd.FU' +
        'LL_NAME,pd.INVOICE_NUMBER,'
      
        '             pd.MEMBER,pd.PAY_DATE, pd.PAYMENT,m.PAYMENT_PERIOD,' +
        ' m.AUTO_PAYMENT,'
      
        '             m.BARCODE_NUMBER,m.PRINT_CARD,m.PRINT_RECEIPT,m.NUM' +
        'BER_CARDS, '
      
        '             m.FIRST_NAME,m.SURNAME,pt.TENDER_TYPE,m.ADDRESS,m.A' +
        'DDRESS1,m.DISTRICT, '
      '              m.CITY, m.COUNTRY'
      '             from MEMBERS m, PAYMENTS_DET pd , PAYMENTS_TEND pt'
      
        '             where m.MEMBER = pd.MEMBER and pd.PAYMENT = pt.PAYM' +
        'ENT and m.member = -1')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  BARCODE_NUMBER = :BARCODE_NUMBER,'
      '  PRINT_CARD = :PRINT_CARD,'
      '  PRINT_RECEIPT = :PRINT_RECEIPT'
      'where'
      '  MEMBER = :MEMBER')
    Left = 312
    Top = 401
    object ibdsBulkCardRecPAYMENT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 6
      FieldName = 'PAYMENT'
      ReadOnly = True
    end
    object ibdsBulkCardRecPRINT_RECEIPT: TIBStringField
      DisplayLabel = 'Rcpt'
      DisplayWidth = 3
      FieldName = 'PRINT_RECEIPT'
      Size = 1
    end
    object ibdsBulkCardRecPRINT_CARD: TIBStringField
      DisplayLabel = 'Card'
      DisplayWidth = 3
      FieldName = 'PRINT_CARD'
      Size = 1
    end
    object ibdsBulkCardRecNUMBER_CARDS: TSmallintField
      DisplayLabel = 'Card #'
      DisplayWidth = 5
      FieldName = 'NUMBER_CARDS'
    end
    object ibdsBulkCardRecBARCODE_NUMBER: TIntegerField
      DisplayLabel = 'BarCode #'
      DisplayWidth = 11
      FieldName = 'BARCODE_NUMBER'
    end
    object ibdsBulkCardRecPAY_DATE: TDateField
      DisplayLabel = 'Date Paid'
      DisplayWidth = 12
      FieldName = 'PAY_DATE'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsBulkCardRecEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 11
      FieldName = 'EXPIRES'
      ReadOnly = True
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsBulkCardRecMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      ReadOnly = True
    end
    object ibdsBulkCardRecFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 15
      FieldName = 'FULL_NAME'
      ReadOnly = True
      Size = 25
    end
    object ibdsBulkCardRecAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Paid'
      DisplayWidth = 10
      FieldName = 'AMOUNT_PAID'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsBulkCardRecADDRESS1: TIBStringField
      DisplayWidth = 30
      FieldName = 'ADDRESS1'
      Size = 30
    end
    object ibdsBulkCardRecDESCRIPTION: TIBStringField
      DisplayLabel = 'Subscription'
      DisplayWidth = 10
      FieldName = 'DESCRIPTION'
      Size = 25
    end
    object ibdsBulkCardRecPAYMENT_PERIOD: TSmallintField
      DisplayLabel = 'Pay Period'
      DisplayWidth = 14
      FieldName = 'PAYMENT_PERIOD'
    end
    object ibdsBulkCardRecAUTO_PAYMENT: TIBStringField
      DisplayLabel = 'Auto Payment'
      DisplayWidth = 12
      FieldName = 'AUTO_PAYMENT'
      Size = 1
    end
    object ibdsBulkCardRecFIRST_NAME: TIBStringField
      DisplayWidth = 20
      FieldName = 'FIRST_NAME'
    end
    object ibdsBulkCardRecSURNAME: TIBStringField
      DisplayWidth = 25
      FieldName = 'SURNAME'
      Size = 25
    end
    object ibdsBulkCardRecTENDER_TYPE: TIBStringField
      DisplayWidth = 20
      FieldName = 'TENDER_TYPE'
    end
    object ibdsBulkCardRecADDRESS: TIBStringField
      DisplayWidth = 30
      FieldName = 'ADDRESS'
      Size = 30
    end
    object ibdsBulkCardRecDISTRICT: TIBStringField
      DisplayWidth = 20
      FieldName = 'DISTRICT'
    end
    object ibdsBulkCardRecCITY: TIBStringField
      DisplayWidth = 25
      FieldName = 'CITY'
      Size = 25
    end
    object ibdsBulkCardRecCOUNTRY: TIBStringField
      DisplayWidth = 25
      FieldName = 'COUNTRY'
      Size = 25
    end
    object ibdsBulkCardRecINVOICE_NUMBER: TIBStringField
      DisplayWidth = 13
      FieldName = 'INVOICE_NUMBER'
      Visible = False
    end
  end
  object dsrBuldCardRec: TDataSource
    DataSet = ibdsBulkCardRec
    Left = 320
    Top = 409
  end
  object tblCardMember: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblCardMemberAfterInsert
    AfterPost = tblSubsAfterPost
    BeforePost = tblEventsBeforePost
    OnPostError = tblCardMemberPostError
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'MEMBER'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NUMBER_CARDS'
        DataType = ftSmallint
      end
      item
        Name = 'CARD_TYPE'
        DataType = ftSmallint
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY10'
        Fields = 'MEMBER'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'CARD_MEMBERS'
    Left = 88
    Top = 416
    object tblCardMemberMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 20
      FieldName = 'MEMBER'
    end
    object tblCardMemberNUMBER_CARDS: TSmallintField
      DisplayLabel = 'Number of Cards'
      DisplayWidth = 20
      FieldName = 'NUMBER_CARDS'
    end
    object tblCardMemberCARD_TYPE: TSmallintField
      DisplayWidth = 10
      FieldName = 'CARD_TYPE'
      Visible = False
    end
  end
  object dsrCardMember: TDataSource
    DataSet = tblCardMember
    OnStateChange = dsrCardMemberStateChange
    Left = 96
    Top = 416
  end
  object IBRunOnce: TIBTransaction
    Active = False
    DefaultDatabase = IBDatabase
    DefaultAction = TARollback
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 457
    Top = 112
  end
  object sqlRunOnce: TIBSQL
    Database = IBDatabase
    ParamCheck = True
    SQL.Strings = (
      '')
    Transaction = IBRunOnce
    Left = 526
    Top = 147
  end
  object ImageList1: TImageList
    Left = 256
    Top = 56
    Bitmap = {
      494C010102000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000000000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF0000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFF00000000FFFFFFFF00000000
      1FFF1FFF00000000041F041F00000000000F000F00000000000F000F00000000
      0007000700000000000100010000000000000000000000000001000100000000
      003F003F00000000FC7FFC7F00000000FFFFFFFF00000000FFFFFFFF00000000
      FFFFFFFF00000000FFFFFFFF0000000000000000000000000000000000000000
      000000000000}
  end
  object ibdsAutoPay: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'select MS.PRINT_CARD,M.MEMBER,M.FIRST_NAME,M.SURNAME,'
      'M.FULL_NAME,MS.SUBSCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,'
      
        'M.SUB_PAID,MS.PAY,M.PAYMENT_PERIOD,M.AUTO_PAYMENT,m.MAILING_LIST' +
        '_CODE,'
      'S.DESCRIPTION, MS.EDIT_DATE,MS.RECEIPT,MS.PAID,M.EXPIRES'
      ' from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS'
      'where S.SUBSCRIPTION = MS.SUBSCRIPTION'
      ' and M.MEMBER = MS.MEMBER '
      'and M.MEMBER = :OLD_MEMBER')
    SelectSQL.Strings = (
      'select M.MEMBER,M.FIRST_NAME,M.FULL_NAME,M.SURNAME,'
      'MS.SUBSCRIPTION,MS.AMOUNT_PAID,MS.AMOUNT,M.SUB_PAID,'
      'MS.PAY,M.PAYMENT_PERIOD,M.AUTO_PAYMENT,m.MAILING_LIST_CODE,'
      
        'S.DESCRIPTION,MS.EDIT_DATE,MS.RECEIPT,MS.PAID,MS.PRINT_CARD,M.EX' +
        'PIRES'
      ' from MEMBERS M, SUBSCRIPTIONS S,M_SUBS MS'
      'where M.AUTO_PAYMENT =  :AUTOPAYMENT'
      ' AND S.SUBSCRIPTION = MS.SUBSCRIPTION'
      ' and M.MEMBER = MS.MEMBER ')
    ModifySQL.Strings = (
      'update M_SUBS'
      'set'
      '  PRINT_CARD = :PRINT_CARD,'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  RECEIPT = :RECEIPT,'
      '  PAID = :PAID,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  PAY = :PAY'
      '  where'
      '  MEMBER = :MEMBER'
      'and SUBSCRIPTION = :SUBSCRIPTION')
    Left = 224
    Top = 368
    object ibdsAutoPayMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsAutoPayFIRST_NAME: TIBStringField
      DisplayLabel = 'First Name'
      DisplayWidth = 10
      FieldName = 'FIRST_NAME'
      ReadOnly = True
    end
    object ibdsAutoPaySURNAME: TIBStringField
      DisplayLabel = 'Surname'
      DisplayWidth = 12
      FieldName = 'SURNAME'
      ReadOnly = True
      Size = 25
    end
    object ibdsAutoPayEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 11
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsAutoPayDESCRIPTION: TIBStringField
      DisplayLabel = 'Subscription'
      DisplayWidth = 16
      FieldName = 'DESCRIPTION'
      ReadOnly = True
      Size = 60
    end
    object ibdsAutoPayAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Paid'
      DisplayWidth = 8
      FieldName = 'AMOUNT_PAID'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsAutoPayAMOUNT: TIBBCDField
      DisplayLabel = 'Fee'
      DisplayWidth = 7
      FieldName = 'AMOUNT'
      ReadOnly = True
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsAutoPayPRINT_CARD: TIBStringField
      DisplayLabel = 'Card'
      DisplayWidth = 5
      FieldName = 'PRINT_CARD'
      Origin = 'M_SUBS.PRINT_CARD'
      FixedChar = True
      Size = 1
    end
    object ibdsAutoPayPAID: TIBStringField
      DisplayLabel = 'PAY'
      DisplayWidth = 5
      FieldName = 'PAID'
      Size = 1
    end
    object ibdsAutoPayEDIT_DATE: TDateTimeField
      DisplayLabel = 'Payment Date'
      DisplayWidth = 13
      FieldName = 'EDIT_DATE'
      DisplayFormat = 'dd mmm yyy'
    end
    object ibdsAutoPayPAY: TIBBCDField
      DisplayLabel = 'Amount'
      DisplayWidth = 10
      FieldName = 'PAY'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsAutoPayPAYMENT_PERIOD: TSmallintField
      DisplayLabel = 'Period'
      DisplayWidth = 6
      FieldName = 'PAYMENT_PERIOD'
      ReadOnly = True
    end
    object ibdsAutoPayMAILING_LIST_CODE: TIntegerField
      DisplayLabel = 'Parent Code'
      DisplayWidth = 10
      FieldName = 'MAILING_LIST_CODE'
      Origin = '"MEMBERS"."MAILING_LIST_CODE"'
    end
    object ibdsAutoPayAUTO_PAYMENT: TIBStringField
      DisplayWidth = 1
      FieldName = 'AUTO_PAYMENT'
      Visible = False
      Size = 1
    end
    object ibdsAutoPayFULL_NAME: TIBStringField
      DisplayWidth = 45
      FieldName = 'FULL_NAME'
      Visible = False
      Size = 45
    end
    object ibdsAutoPaySUBSCRIPTION: TIntegerField
      FieldName = 'SUBSCRIPTION'
      ReadOnly = True
      Required = True
      Visible = False
    end
    object ibdsAutoPaySUB_PAID: TSmallintField
      FieldName = 'SUB_PAID'
      ReadOnly = True
      Visible = False
    end
    object ibdsAutoPayRECEIPT: TIntegerField
      FieldName = 'RECEIPT'
      Visible = False
    end
  end
  object dsrAutoPAy: TDataSource
    DataSet = ibdsAutoPay
    Left = 232
    Top = 376
  end
  object ibdsPoints: TIBDataSet
    Database = IBDatabase
    Transaction = IBRunOnce
    AutoCalcFields = False
    OnCalcFields = ibdsPointsCalcFields
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  KNOWN_AS,'
      '  POINTS_AVAILABLE,'
      '  TOTAL_EARNT,'
      '  TOTAL_REDEEMED,'
      '  FULL_NAME,'
      '  PAY,EARNT_CREDIT,BONUS_CREDIT,PRE_PAID_CREDIT,REASON '
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER, TOTAL_REDEEMED, TOTAL_EARNT,   POINTS_AVAILABLE,E' +
        'ARNT_CREDIT,BONUS_CREDIT,PRE_PAID_CREDIT, KNOWN_AS, FULL_NAME,PA' +
        'Y,REASON from MEMBERS'
      'where member = -1 ')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '    POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  KNOWN_AS = :KNOWN_AS,'
      '  FULL_NAME = :FULL_NAME,'
      '  PAY = :PAY,'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  REASON = :REASON'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 416
    Top = 392
    object ibdsPointsMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsPointsFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 22
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object ibdsPointsKNOWN_AS: TIBStringField
      DisplayLabel = 'Known As'
      DisplayWidth = 15
      FieldName = 'KNOWN_AS'
      Size = 15
    end
    object ibdsPointsPAY: TIBBCDField
      DisplayLabel = 'Bonus Points'
      DisplayWidth = 10
      FieldName = 'PAY'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPointsREASON: TIBStringField
      DisplayLabel = 'Reason'
      DisplayWidth = 20
      FieldName = 'REASON'
      Origin = 'MEMBERS.REASON'
    end
    object ibdsPointsPOINTS_AVAILABLE: TFloatField
      DisplayLabel = 'Points Available'
      DisplayWidth = 10
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
      currency = True
    end
    object ibdsPointsTOTAL_EARNT: TFloatField
      DisplayLabel = 'Points Earnt'
      DisplayWidth = 10
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
      currency = True
    end
    object ibdsPointsTOTAL_REDEEMED: TFloatField
      DisplayLabel = 'Points Redeemed'
      DisplayWidth = 10
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
      currency = True
    end
    object ibdsPointsEARNT_CREDIT: TFloatField
      DisplayLabel = 'Earnt Credit'
      DisplayWidth = 10
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
      currency = True
    end
    object ibdsPointsBONUS_CREDIT: TFloatField
      DisplayLabel = 'Bonus Credit'
      DisplayWidth = 10
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
      currency = True
    end
    object ibdsPointsPRE_PAID_CREDIT: TFloatField
      DisplayLabel = 'Pre Paid Credit'
      DisplayWidth = 10
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
      currency = True
    end
    object ibdsPointsVALUE_OF_CREDIT: TFloatField
      DisplayLabel = 'Value of Credit'
      DisplayWidth = 10
      FieldKind = fkCalculated
      FieldName = 'VALUE_OF_CREDIT'
      currency = True
      Calculated = True
    end
  end
  object dsrPoints: TDataSource
    DataSet = ibdsPoints
    Left = 424
    Top = 384
  end
  object ibdsM_Groups: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_GroupsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsM_GroupsBeforeDelete
    BeforePost = ibdsM_EventsBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_GROUPS'
      'where'
      '  GROUPS = :OLD_GROUPS and'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into M_GROUPS'
      '  (GROUPS, MEMBER)'
      'values'
      '  (:GROUPS, :MEMBER)')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  GROUPS'
      'from M_GROUPS '
      'where'
      '   MEMBER = :MEMBER')
    SelectSQL.Strings = (
      'select GROUPS, MEMBER from M_GROUPS'
      'WHERE MEMBER =  :Member'
      'ORDER by GROUPS')
    ModifySQL.Strings = (
      'update M_GROUPS'
      'set'
      '  GROUPS = :GROUPS,'
      '  MEMBER = :MEMBER'
      'where'
      '  GROUPS = :OLD_GROUPS and'
      '  MEMBER = :OLD_MEMBER')
    DataSource = dsrMembers
    Left = 24
    Top = 328
    object ibdsM_GroupsGROUPS: TSmallintField
      FieldName = 'GROUPS'
      Required = True
    end
    object ibdsM_GroupsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsM_GroupsDESCRIPTION: TStringField
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = tblGroups
      LookupKeyFields = 'GROUPS'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'GROUPS'
      Size = 40
      Lookup = True
    end
  end
  object dsrM_Groups: TDataSource
    DataSet = ibdsM_Groups
    Left = 48
    Top = 331
  end
  object ibdsM_Lots: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_LotsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforePost = ibdsM_EventsBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from LOTS'
      'where'
      '  LOT_ID = :OLD_LOT_ID')
    InsertSQL.Strings = (
      'insert into LOTS'
      '  (DESCRIPTION,LOT_ID,LOT_TYPE, MEMBER)'
      'values'
      '  (:DESCRIPTION,:LOT_ID, :LOT_TYPE, :MEMBER)')
    RefreshSQL.Strings = (
      'Select '
      '  LOT_ID,'
      '  MEMBER,'
      '  LOT_TYPE,'
      '  DESCRIPTION'
      'from LOTS '
      'where'
      '  LOT_ID = :LOT_ID')
    SelectSQL.Strings = (
      'select DESCRIPTION, LOT_ID, LOT_TYPE, MEMBER from LOTS'
      'where MEMBER = :MEmber')
    ModifySQL.Strings = (
      'update LOTS'
      'set'
      '  DESCRIPTION = :DESCRIPTION,'
      '  LOT_TYPE = :LOT_TYPE,'
      '  MEMBER = :MEMBER'
      'where'
      '  LOT_ID = :OLD_LOT_ID')
    DataSource = dsrMembers
    Left = 72
    Top = 184
    object ibdsM_LotsLOT_ID: TIntegerField
      FieldName = 'LOT_ID'
      Required = True
    end
    object ibdsM_LotsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsM_LotsDESCRIPTION: TIBStringField
      FieldName = 'DESCRIPTION'
      Size = 60
    end
    object ibdsM_LotsLOT_TYPE: TIBStringField
      FieldName = 'LOT_TYPE'
      Size = 8
    end
  end
  object dsrM_Lots: TDataSource
    DataSet = ibdsM_Lots
    Left = 72
    Top = 195
  end
  object dsrM_Interests: TDataSource
    DataSet = ibdsM_Interests
    Left = 72
    Top = 259
  end
  object ibdsM_Interests: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_InterestsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforePost = ibdsM_EventsBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_INTERESTS'
      'where'
      '  MEMBER = :OLD_MEMBER and'
      '  SECTION = :OLD_SECTION')
    InsertSQL.Strings = (
      'insert into M_INTERESTS'
      '  (COMMENTS, MEMBER, SECTION)'
      'values'
      '  (:COMMENTS, :MEMBER, :SECTION)')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  SECTION,'
      '  COMMENTS'
      'from M_INTERESTS '
      'where'
      '  MEMBER = :MEMBER and'
      '  SECTION = :SECTION')
    SelectSQL.Strings = (
      'select COMMENTS, MEMBER, SECTION from M_INTERESTS'
      'where MEMBER = :Member')
    ModifySQL.Strings = (
      'update M_INTERESTS'
      'set'
      '  COMMENTS = :COMMENTS,'
      '  MEMBER = :MEMBER,'
      '  SECTION = :SECTION'
      'where'
      '  MEMBER = :OLD_MEMBER and'
      '  SECTION = :OLD_SECTION')
    DataSource = dsrMembers
    Left = 72
    Top = 240
    object ibdsM_InterestsCOMMENTS: TIBStringField
      FieldName = 'COMMENTS'
      Size = 100
    end
    object ibdsM_InterestsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsM_InterestsSECTION: TIntegerField
      FieldName = 'SECTION'
      Required = True
    end
    object ibdsM_InterestsDESCRIPTION: TStringField
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = tblSections
      LookupKeyFields = 'SECTION'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'SECTION'
      Size = 40
      Lookup = True
    end
  end
  object ibdsPay_Det: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  PAYMENT_DET,'
      '  PAYMENT,'
      '  DESCRIPTION,'
      '  EXPIRES,'
      '  SPONSORSHIP,'
      '  AMOUNT,'
      '  AMOUNT_PAID,'
      '  PAY_FULL,'
      '  PAY_TYPE,'
      '  MEMBER,'
      '  PAY_DATE,'
      '  FULL_NAME,'
      '  PAID_BY,'
      '  INVOICE_NUMBER,'
      '  DISC,'
      '  CODE'
      'from PAYMENTS_DET '
      'where'
      '  PAYMENT_DET = :PAYMENT_DET')
    SelectSQL.Strings = (
      
        'select AMOUNT, AMOUNT_PAID, CODE, DESCRIPTION, DISC, EXPIRES, FU' +
        'LL_NAME, INVOICE_NUMBER, MEMBER, PAID_BY, PAY_DATE, PAY_FULL, PA' +
        'Y_TYPE, PAYMENT, PAYMENT_DET, SPONSORSHIP from PAYMENTS_DET WHER' +
        'E PAYMENT = :PAYMENT'
      'ORDER BY MEMBER')
    ModifySQL.Strings = (
      'update PAYMENTS_DET'
      'set'
      '  AMOUNT = :AMOUNT,'
      '  AMOUNT_PAID = :AMOUNT_PAID,'
      '  CODE = :CODE,'
      '  DESCRIPTION = :DESCRIPTION,'
      '  DISC = :DISC,'
      '  EXPIRES = :EXPIRES,'
      '  FULL_NAME = :FULL_NAME,'
      '  INVOICE_NUMBER = :INVOICE_NUMBER,'
      '  MEMBER = :MEMBER,'
      '  PAID_BY = :PAID_BY,'
      '  PAY_DATE = :PAY_DATE,'
      '  PAY_FULL = :PAY_FULL,'
      '  PAY_TYPE = :PAY_TYPE,'
      '   SPONSORSHIP = :SPONSORSHIP'
      'where'
      '  PAYMENT_DET = :OLD_PAYMENT_DET')
    DataSource = dsrPayments
    Left = 312
    Top = 8
    object ibdsPay_DetPAYMENT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 7
      FieldName = 'PAYMENT'
      ReadOnly = True
    end
    object ibdsPay_DetMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 9
      FieldName = 'MEMBER'
    end
    object ibdsPay_DetFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 21
      FieldName = 'FULL_NAME'
      Size = 40
    end
    object ibdsPay_DetDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 19
      FieldName = 'DESCRIPTION'
      Size = 62
    end
    object ibdsPay_DetEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 10
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPay_DetAMOUNT: TIBBCDField
      DisplayLabel = 'Fee'
      DisplayWidth = 10
      FieldName = 'AMOUNT'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPay_DetAMOUNT_PAID: TIBBCDField
      DisplayLabel = 'Amt Paid'
      DisplayWidth = 10
      FieldName = 'AMOUNT_PAID'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPay_DetPAY_DATE: TDateField
      DisplayLabel = 'Date Paid'
      DisplayWidth = 10
      FieldName = 'PAY_DATE'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPay_DetSPONSORSHIP: TIBStringField
      DisplayLabel = 'S/Ship'
      DisplayWidth = 5
      FieldName = 'SPONSORSHIP'
      Size = 1
    end
    object ibdsPay_DetPAID_BY: TIntegerField
      DisplayLabel = 'Paid By'
      DisplayWidth = 10
      FieldName = 'PAID_BY'
      Visible = False
    end
    object ibdsPay_DetCODE: TIntegerField
      DisplayLabel = 'Code'
      DisplayWidth = 10
      FieldName = 'CODE'
      Visible = False
    end
    object ibdsPay_DetDISC: TIBBCDField
      DisplayLabel = 'Discount'
      DisplayWidth = 10
      FieldName = 'DISC'
      Visible = False
      currency = True
      Precision = 9
      Size = 3
    end
    object ibdsPay_DetINVOICE_NUMBER: TIBStringField
      DisplayWidth = 20
      FieldName = 'INVOICE_NUMBER'
      Visible = False
    end
    object ibdsPay_DetPAY_FULL: TSmallintField
      DisplayWidth = 10
      FieldName = 'PAY_FULL'
      Visible = False
    end
    object ibdsPay_DetPAY_TYPE: TSmallintField
      DisplayWidth = 10
      FieldName = 'PAY_TYPE'
      Visible = False
    end
    object ibdsPay_DetPAYMENT_DET: TIntegerField
      DisplayWidth = 10
      FieldName = 'PAYMENT_DET'
      ReadOnly = True
      Required = True
      Visible = False
    end
  end
  object ibdsPAyments: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransPay
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  PAYMENT,'
      '  PAY_AMOUNT,'
      '  POINTS_USED,'
      '  PAID_BY,'
      '  DISCOUNT,'
      '  EDIT_DATE,'
      '  ROUNDING,'
      '  PAY_CREDIT,'
      '  FULL_NAME,'
      '  INVOICE_NUMBER'
      'from PAYMENTS '
      'where'
      '  PAYMENT = :PAYMENT')
    SelectSQL.Strings = (
      
        'select DISCOUNT, EDIT_DATE, FULL_NAME, INVOICE_NUMBER, PAID_BY, ' +
        'PAY_AMOUNT, PAY_CREDIT, PAYMENT, POINTS_USED, ROUNDING from PAYM' +
        'ENTS  WHERE PAYMENT = -1'
      'ORDER BY PAYMENT desc')
    ModifySQL.Strings = (
      'update PAYMENTS'
      'set'
      '  DISCOUNT = :DISCOUNT,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  FULL_NAME = :FULL_NAME,'
      '  INVOICE_NUMBER = :INVOICE_NUMBER,'
      '  PAID_BY = :PAID_BY,'
      '  PAY_AMOUNT = :PAY_AMOUNT,'
      '  PAY_CREDIT = :PAY_CREDIT,'
      '  POINTS_USED = :POINTS_USED,'
      '  ROUNDING = :ROUNDING'
      'where'
      '  PAYMENT = :OLD_PAYMENT')
    Left = 312
    Top = 64
    object ibdsPAymentsPAYMENT: TIntegerField
      DisplayLabel = 'Receipt'
      DisplayWidth = 7
      FieldName = 'PAYMENT'
      ReadOnly = True
      Required = True
    end
    object ibdsPAymentsPAID_BY: TIntegerField
      DisplayLabel = 'Paid By'
      DisplayWidth = 11
      FieldName = 'PAID_BY'
    end
    object ibdsPAymentsFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 23
      FieldName = 'FULL_NAME'
      Size = 25
    end
    object ibdsPAymentsPAY_AMOUNT: TIBBCDField
      DisplayLabel = 'Total Paid'
      DisplayWidth = 12
      FieldName = 'PAY_AMOUNT'
      currency = True
      Precision = 18
      Size = 2
    end
    object ibdsPAymentsPOINTS_USED: TIBBCDField
      DisplayLabel = 'Points Used'
      DisplayWidth = 10
      FieldName = 'POINTS_USED'
      currency = True
      Precision = 9
      Size = 2
    end
    object ibdsPAymentsROUNDING: TIBBCDField
      DisplayLabel = 'Rounding'
      DisplayWidth = 9
      FieldName = 'ROUNDING'
      currency = True
      Precision = 9
      Size = 3
    end
    object ibdsPAymentsEDIT_DATE: TDateTimeField
      DisplayLabel = 'Date Paid'
      DisplayWidth = 15
      FieldName = 'EDIT_DATE'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsPAymentsDISCOUNT: TIBBCDField
      DisplayLabel = 'Discount'
      DisplayWidth = 7
      FieldName = 'DISCOUNT'
      currency = True
      Precision = 9
      Size = 3
    end
    object ibdsPAymentsINVOICE_NUMBER: TIBStringField
      FieldName = 'INVOICE_NUMBER'
      Visible = False
    end
    object ibdsPAymentsPAY_CREDIT: TIBBCDField
      FieldName = 'PAY_CREDIT'
      Visible = False
      currency = True
      Precision = 9
      Size = 2
    end
  end
  object ibdsRollover: TIBDataSet
    Database = IBDatabase
    Transaction = IBRollover
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  ROLL_ID,'
      '  KEEP_EXPIRES_CURRENT,'
      '  MEMBER,'
      '  MAILING_LIST_CODE,'
      '  SURNAME,'
      '  FIRST_NAME,'
      '  FULL_NAME,'
      '  ADDRESS,'
      '  DISTRICT,'
      '  ADDRESS1,'
      '  CITY,'
      '  COUNTRY,'
      '  HOME_PHONE,'
      '  EXPIRES,'
      '  SUB_PAID,'
      '  IS_ACTIVE,'
      '  NEWSLETTER,'
      '  ANALYSE,'
      '  ACCEPT'
      'from M_ROLLOVER '
      'where'
      '  ROLL_ID = :ROLL_ID')
    SelectSQL.Strings = (
      
        'select ACCEPT, ADDRESS, ADDRESS1, ANALYSE, CITY, COUNTRY, DISTRI' +
        'CT, EXPIRES, FIRST_NAME, FULL_NAME, HOME_PHONE, IS_ACTIVE, KEEP_' +
        'EXPIRES_CURRENT, MAILING_LIST_CODE, MEMBER, NEWSLETTER, ROLL_ID,' +
        ' SUB_PAID, SURNAME from M_ROLLOVER where KEEP_EXPIRES_CURRENT = ' +
        ' :KeepExpiresCurrent order by Member')
    ModifySQL.Strings = (
      'update M_ROLLOVER'
      'set'
      '  ACCEPT = :ACCEPT,'
      '  ANALYSE = :ANALYSE,'
      '  NEWSLETTER = :NEWSLETTER,'
      '  EXPIRES = :EXPIRES,'
      'KEEP_EXPIRES_CURRENT = :KEEP_EXPIRES_CURRENT'
      'where'
      '  ROLL_ID = :OLD_ROLL_ID')
    Left = 488
    Top = 64
    object ibdsRolloverMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 8
      FieldName = 'MEMBER'
      Required = True
    end
    object ibdsRolloverFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 24
      FieldName = 'FULL_NAME'
      Size = 45
    end
    object ibdsRolloverEXPIRES: TDateField
      DisplayLabel = 'Expires'
      DisplayWidth = 14
      FieldName = 'EXPIRES'
      DisplayFormat = 'dd mmm yyyy'
    end
    object ibdsRolloverACCEPT: TIBStringField
      DisplayLabel = 'Accept'
      DisplayWidth = 5
      FieldName = 'ACCEPT'
      Size = 1
    end
    object ibdsRolloverANALYSE: TIBStringField
      DisplayLabel = 'Analyse'
      DisplayWidth = 6
      FieldName = 'ANALYSE'
      Size = 1
    end
    object ibdsRolloverNEWSLETTER: TIBStringField
      DisplayLabel = 'NewsLetter'
      DisplayWidth = 9
      FieldName = 'NEWSLETTER'
      Size = 1
    end
    object ibdsRolloverIS_ACTIVE: TIBStringField
      DisplayLabel = 'Active'
      DisplayWidth = 5
      FieldName = 'IS_ACTIVE'
      Size = 1
    end
    object ibdsRolloverKEEP_EXPIRES_CURRENT: TIBStringField
      DisplayLabel = 'Exp Current'
      DisplayWidth = 9
      FieldName = 'KEEP_EXPIRES_CURRENT'
      Size = 1
    end
    object ibdsRolloverSUB_PAID: TSmallintField
      DisplayLabel = 'Financial'
      DisplayWidth = 9
      FieldName = 'SUB_PAID'
    end
    object ibdsRolloverADDRESS: TIBStringField
      FieldName = 'ADDRESS'
      Visible = False
      Size = 30
    end
    object ibdsRolloverADDRESS1: TIBStringField
      FieldName = 'ADDRESS1'
      Visible = False
      Size = 30
    end
    object ibdsRolloverCITY: TIBStringField
      FieldName = 'CITY'
      Visible = False
      Size = 25
    end
    object ibdsRolloverCOUNTRY: TIBStringField
      FieldName = 'COUNTRY'
      Visible = False
      Size = 25
    end
    object ibdsRolloverDISTRICT: TIBStringField
      FieldName = 'DISTRICT'
      Visible = False
    end
    object ibdsRolloverFIRST_NAME: TIBStringField
      FieldName = 'FIRST_NAME'
      Visible = False
      Size = 25
    end
    object ibdsRolloverHOME_PHONE: TIBStringField
      FieldName = 'HOME_PHONE'
      Visible = False
      Size = 35
    end
    object ibdsRolloverMAILING_LIST_CODE: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
      Visible = False
    end
    object ibdsRolloverROLL_ID: TIntegerField
      FieldName = 'ROLL_ID'
      Required = True
      Visible = False
    end
    object ibdsRolloverSURNAME: TIBStringField
      FieldName = 'SURNAME'
      Visible = False
      Size = 25
    end
  end
  object dsrRollover: TDataSource
    DataSet = ibdsRollover
    Left = 416
    Top = 72
  end
  object ibdsQueries: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsLettersAfterPost
    AfterInsert = ibdsQueriesAfterInsert
    AfterPost = ibdsLettersAfterPost
    BeforeInsert = ibdsQueriesBeforeInsert
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from QUERIES'
      'where'
      '  QUERIES_ID = :OLD_QUERIES_ID')
    InsertSQL.Strings = (
      'insert into QUERIES'
      '  (QUERIES_ID, NAME, COMMENTS, SQL, CALLER_ID)'
      'values'
      '  (:QUERIES_ID, :NAME, :COMMENTS, :SQL, :CALLER_ID)')
    RefreshSQL.Strings = (
      'Select '
      '  QUERIES_ID,'
      '  NAME,'
      '  COMMENTS,'
      '  SQL,'
      '  CALLER_ID'
      'from QUERIES '
      'where'
      '  QUERIES_ID = :QUERIES_ID')
    SelectSQL.Strings = (
      'select * from QUERIES')
    ModifySQL.Strings = (
      'update QUERIES'
      'set'
      '  QUERIES_ID = :QUERIES_ID,'
      '  NAME = :NAME,'
      '  COMMENTS = :COMMENTS,'
      '  SQL = :SQL,'
      '  CALLER_ID = :CALLER_ID'
      'where'
      '  QUERIES_ID = :OLD_QUERIES_ID')
    Left = 208
    Top = 312
    object ibdsQueriesQUERIES_ID: TSmallintField
      FieldName = 'QUERIES_ID'
      Required = True
    end
    object ibdsQueriesNAME: TIBStringField
      FieldName = 'NAME'
      Size = 35
    end
    object ibdsQueriesCOMMENTS: TIBStringField
      FieldName = 'COMMENTS'
      Size = 120
    end
    object ibdsQueriesSQL: TBlobField
      FieldName = 'SQL'
      BlobType = ftBlob
      Size = 8
    end
    object ibdsQueriesCALLER_ID: TSmallintField
      FieldName = 'CALLER_ID'
    end
  end
  object dsrQueries: TDataSource
    DataSet = ibdsQueries
    OnStateChange = dsrLettersStateChange
    Left = 216
    Top = 320
  end
  object OpWord: TOpWord
    Version = '1.64'
    Caption = 'Microsoft Word'
    Documents = <>
    PropDirection = pdFromServer
    Visible = False
    ScreenUpdating = True
    PrintPreview = False
    UserName = 'cwatki'
    UserInitials = 'c'
    DisplayRecentFiles = True
    DisplayScrollBars = False
    ServerLeft = 0
    ServerTop = 0
    ServerWidth = 640
    ServerHeight = 480
    WindowState = wdwsNormal
    DisplayAlerts = wdalNone
    DisplayScreenTips = True
    EnableCancelKey = wdeckInterrupt
    DefaultTableSeparator = '-'
    Left = 264
    Top = 232
  end
  object tblLuckyMember: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    TableName = 'LUCKY_MEMBER'
    Left = 464
    Top = 384
    object tblLuckyMemberLUCKY_ID: TIntegerField
      FieldName = 'LUCKY_ID'
      Required = True
    end
    object tblLuckyMemberMEMBER: TIntegerField
      FieldName = 'MEMBER'
    end
  end
  object dsrLuckyMember: TDataSource
    DataSet = tblLuckyMember
    Left = 488
    Top = 384
  end
  object dsrReport: TDataSource
    DataSet = qryReport
    Left = 120
    Top = 16
  end
  object ibdsM_Photos: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from M_PHOTOS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into M_PHOTOS'
      '  (DRAWN, MEMBER, PHOTO)'
      'values'
      '  (:DRAWN, :MEMBER, :PHOTO)')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  PHOTO,'
      '  DRAWN'
      'from M_PHOTOS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      'select DRAWN, MEMBER, PHOTO from M_PHOTOS'
      'where MEMBER = :Member')
    ModifySQL.Strings = (
      'update M_PHOTOS'
      'set'
      '  DRAWN = :DRAWN,'
      '  MEMBER = :MEMBER,'
      '  PHOTO = :PHOTO'
      'where'
      '  MEMBER = :OLD_MEMBER')
    DataSource = dsrMembers
    Left = 72
    Top = 296
    object ibdsM_PhotosDRAWN: TIBStringField
      FieldName = 'DRAWN'
      Origin = 'M_PHOTOS.DRAWN'
      FixedChar = True
      Size = 1
    end
    object ibdsM_PhotosMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'M_PHOTOS.MEMBER'
      Required = True
    end
    object ibdsM_PhotosPHOTO: TBlobField
      FieldName = 'PHOTO'
      Origin = 'M_PHOTOS.PHOTO'
      BlobType = ftBlob
      Size = 8
    end
  end
  object tblClubs: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterInsert = tblClubsAfterInsert
    AfterPost = tblSubsAfterPost
    BeforePost = tblEventsBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    DefaultIndex = False
    FieldDefs = <
      item
        Name = 'CLUB_ID'
        Attributes = [faRequired]
        DataType = ftSmallint
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 60
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY11'
        Fields = 'CLUB_ID'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'DESC_INDEX'
        Fields = 'DESCRIPTION'
      end>
    IndexFieldNames = 'DESCRIPTION'
    StoreDefs = True
    TableName = 'CLUBS'
    Left = 496
    Top = 224
    object tblClubsCLUB_ID: TSmallintField
      DisplayLabel = 'Club ID'
      DisplayWidth = 2
      FieldName = 'CLUB_ID'
      Required = True
    end
    object tblClubsDESCRIPTION: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 30
      FieldName = 'DESCRIPTION'
      Size = 60
    end
  end
  object dsrClubs: TDataSource
    DataSet = tblClubs
    Left = 488
    Top = 232
  end
  object ibdsMembersBrief: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterPost = ibdsMembersBriefAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '   DATE_OF_BIRTH,'
      '  ADDRESS,'
      '  ADDRESS1,'
      '  EMAIL,'
      '  HOME_PHONE,'
      '  WORK_PHONE,'
      '   JOINED,'
      '   SUB_PAID,'
      '  EXPIRES,'
      '  FIRST_NAME,'
      '  SURNAME,'
      '  FULL_NAME,'
      '  IS_ACTIVE,'
      '  DISTRICT,'
      '  CITY,'
      '  COUNTRY'
      '  '
      'from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select  MEMBER, ADDRESS, ADDRESS1, CITY, COUNTRY, DATE_OF_BIRTH,' +
        ' DISTRICT, EMAIL, EXPIRES, FIRST_NAME, FULL_NAME, HOME_PHONE, IS' +
        '_ACTIVE, JOINED,SUB_PAID, SURNAME, WORK_PHONE from MEMBERS where' +
        ' MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  ADDRESS = :ADDRESS,'
      '  ADDRESS1 = :ADDRESS1,'
      '  CITY = :CITY,'
      '  COUNTRY = :COUNTRY,'
      '  DATE_OF_BIRTH = :DATE_OF_BIRTH,'
      '  DISTRICT = :DISTRICT,'
      '  EMAIL = :EMAIL,'
      '  EXPIRES = :EXPIRES,'
      '  FIRST_NAME = :FIRST_NAME,'
      '  FULL_NAME = :FULL_NAME,'
      '  HOME_PHONE = :HOME_PHONE,'
      '  IS_ACTIVE = :IS_ACTIVE,'
      '  JOINED = :JOINED,'
      '  MEMBER = :MEMBER,'
      '  SUB_PAID = :SUB_PAID,'
      '  SURNAME = :SURNAME,'
      '  WORK_PHONE = :WORK_PHONE'
      'where'
      '  MEMBER = :OLD_MEMBER')
    DataSource = dsrPayDet
    Left = 512
    Top = 304
    object ibdsMembersBriefMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'MEMBERS.MEMBER'
      Required = True
    end
    object ibdsMembersBriefADDRESS: TIBStringField
      FieldName = 'ADDRESS'
      Origin = 'MEMBERS.ADDRESS'
      Size = 30
    end
    object ibdsMembersBriefADDRESS1: TIBStringField
      FieldName = 'ADDRESS1'
      Origin = 'MEMBERS.ADDRESS1'
      Size = 30
    end
    object ibdsMembersBriefCITY: TIBStringField
      FieldName = 'CITY'
      Origin = 'MEMBERS.CITY'
      Size = 25
    end
    object ibdsMembersBriefCOUNTRY: TIBStringField
      FieldName = 'COUNTRY'
      Origin = 'MEMBERS.COUNTRY'
      Size = 25
    end
    object ibdsMembersBriefDATE_OF_BIRTH: TDateField
      FieldName = 'DATE_OF_BIRTH'
      Origin = 'MEMBERS.DATE_OF_BIRTH'
    end
    object ibdsMembersBriefDISTRICT: TIBStringField
      FieldName = 'DISTRICT'
      Origin = 'MEMBERS.DISTRICT'
    end
    object ibdsMembersBriefEMAIL: TIBStringField
      FieldName = 'EMAIL'
      Origin = 'MEMBERS.EMAIL'
      Size = 60
    end
    object ibdsMembersBriefEXPIRES: TDateField
      FieldName = 'EXPIRES'
      Origin = 'MEMBERS.EXPIRES'
    end
    object ibdsMembersBriefFIRST_NAME: TIBStringField
      FieldName = 'FIRST_NAME'
      Origin = 'MEMBERS.FIRST_NAME'
    end
    object ibdsMembersBriefFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = 'MEMBERS.FULL_NAME'
      Size = 45
    end
    object ibdsMembersBriefHOME_PHONE: TIBStringField
      FieldName = 'HOME_PHONE'
      Origin = 'MEMBERS.HOME_PHONE'
      Size = 35
    end
    object ibdsMembersBriefIS_ACTIVE: TIBStringField
      FieldName = 'IS_ACTIVE'
      Origin = 'MEMBERS.IS_ACTIVE'
      FixedChar = True
      Size = 1
    end
    object ibdsMembersBriefJOINED: TDateField
      FieldName = 'JOINED'
      Origin = 'MEMBERS.JOINED'
    end
    object ibdsMembersBriefSUB_PAID: TSmallintField
      FieldName = 'SUB_PAID'
      Origin = 'MEMBERS.SUB_PAID'
    end
    object ibdsMembersBriefSURNAME: TIBStringField
      FieldName = 'SURNAME'
      Origin = 'MEMBERS.SURNAME'
      Size = 25
    end
    object ibdsMembersBriefWORK_PHONE: TIBStringField
      FieldName = 'WORK_PHONE'
      Origin = 'MEMBERS.WORK_PHONE'
      FixedChar = True
      Size = 14
    end
  end
  object dsrMembersBrief: TDataSource
    DataSet = ibdsMembersBrief
    OnStateChange = dsrMembersBriefStateChange
    Left = 512
    Top = 336
  end
  object ibdsPointsLog: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from POINTS_LOG'
      'where'
      '  POINTS_ID = :OLD_POINTS_ID')
    InsertSQL.Strings = (
      'insert into POINTS_LOG'
      '  (POINTS_ID, PROCESS,REASON, TOTAL_EARNT, TOTAL_REDEEMED, '
      'EARNT_CREDIT, '
      
        '   BONUS_CREDIT, PRE_PAID_CREDIT, POINTS_VALUE, MEMBER, EDIT_DAT' +
        'E, '
      '  POINTS_AVAILABLE,BEF_TOTAL_EARNT, '
      '   BEF_TOTAL_REDEEMED, BEF_EARNT_CREDIT, BEF_BONUS_CREDIT, '
      'BEF_PRE_PAID_CREDIT,USER_CODE,BEF_POINTS_AVAILABLE,  FULL_NAME)'
      'values'
      '  (:POINTS_ID, :PROCESS,:REASON, :TOTAL_EARNT, :TOTAL_REDEEMED, '
      ':EARNT_CREDIT, '
      '   :BONUS_CREDIT, :PRE_PAID_CREDIT, :POINTS_VALUE, :MEMBER, '
      ':EDIT_DATE,  :POINTS_AVAILABLE,'
      '   :BEF_TOTAL_EARNT, :BEF_TOTAL_REDEEMED, :BEF_EARNT_CREDIT, '
      ':BEF_BONUS_CREDIT, '
      
        '   :BEF_PRE_PAID_CREDIT,:USER_CODE,:BEF_POINTS_AVAILABLE,:FULL_N' +
        'AME)')
    RefreshSQL.Strings = (
      'Select '
      '  POINTS_ID,'
      '  PROCESS,'
      '  REASON,'
      '  POINTS_AVAILABLE,'
      '  TOTAL_EARNT,'
      '  TOTAL_REDEEMED,'
      '  EARNT_CREDIT,'
      '  BONUS_CREDIT,'
      '  PRE_PAID_CREDIT,'
      '  POINTS_VALUE,'
      '  MEMBER,'
      '  EDIT_DATE,'
      '  BEF_POINTS_AVAILABLE,'
      '  BEF_TOTAL_EARNT,'
      '  BEF_TOTAL_REDEEMED,'
      '  BEF_EARNT_CREDIT,'
      '  BEF_BONUS_CREDIT,'
      '  BEF_PRE_PAID_CREDIT,'
      '  BEF_POINTS_AVAILABLE,'
      '  USER_CODE,'
      '  FULL_NAME'
      ''
      'from POINTS_LOG '
      'where'
      '  POINTS_ID = :POINTS_ID')
    SelectSQL.Strings = (
      'Select  * from POINTS_LOG where MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update POINTS_LOG'
      'set'
      '  POINTS_ID = :POINTS_ID,'
      '  PROCESS = :PROCESS,'
      '  REASON = :REASON,'
      '  POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  POINTS_VALUE = :POINTS_VALUE,'
      '  MEMBER = :MEMBER,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  BEF_TOTAL_EARNT = :BEF_TOTAL_EARNT,'
      '  BEF_TOTAL_REDEEMED = :BEF_TOTAL_REDEEMED,'
      '  BEF_EARNT_CREDIT = :BEF_EARNT_CREDIT,'
      '  BEF_BONUS_CREDIT = :BEF_BONUS_CREDIT,'
      '  BEF_PRE_PAID_CREDIT = :BEF_PRE_PAID_CREDIT,'
      '  BEF_POINTS_AVAILABLE = :BEF_POINTS_AVAILABLE,'
      ' USER_CODE = :USER_CODE,'
      '  FULL_NAME = :FULL_NAME'
      ''
      'where'
      '  POINTS_ID = :OLD_POINTS_ID')
    Left = 521
    Top = 104
    object ibdsPointsLogPOINTS_ID: TIntegerField
      FieldName = 'POINTS_ID'
      Origin = 'POINTS_LOG.POINTS_ID'
      Required = True
    end
    object ibdsPointsLogPROCESS: TSmallintField
      FieldName = 'PROCESS'
      Origin = 'POINTS_LOG.PROCESS'
    end
    object ibdsPointsLogREASON: TIBStringField
      FieldName = 'REASON'
      Origin = 'POINTS_LOG.REASON'
      Size = 35
    end
    object ibdsPointsLogTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = 'POINTS_LOG.TOTAL_EARNT'
    end
    object ibdsPointsLogTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = 'POINTS_LOG.TOTAL_REDEEMED'
    end
    object ibdsPointsLogEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = 'POINTS_LOG.EARNT_CREDIT'
    end
    object ibdsPointsLogBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = 'POINTS_LOG.BONUS_CREDIT'
    end
    object ibdsPointsLogPRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = 'POINTS_LOG.PRE_PAID_CREDIT'
    end
    object ibdsPointsLogPOINTS_VALUE: TFloatField
      FieldName = 'POINTS_VALUE'
      Origin = 'POINTS_LOG.POINTS_VALUE'
    end
    object ibdsPointsLogMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'POINTS_LOG.MEMBER'
    end
    object ibdsPointsLogEDIT_DATE: TDateTimeField
      FieldName = 'EDIT_DATE'
      Origin = 'POINTS_LOG.EDIT_DATE'
    end
    object ibdsPointsLogBEF_TOTAL_EARNT: TFloatField
      FieldName = 'BEF_TOTAL_EARNT'
      Origin = 'POINTS_LOG.BEF_TOTAL_EARNT'
    end
    object ibdsPointsLogBEF_TOTAL_REDEEMED: TFloatField
      FieldName = 'BEF_TOTAL_REDEEMED'
      Origin = 'POINTS_LOG.BEF_TOTAL_REDEEMED'
    end
    object ibdsPointsLogBEF_EARNT_CREDIT: TFloatField
      FieldName = 'BEF_EARNT_CREDIT'
      Origin = 'POINTS_LOG.BEF_EARNT_CREDIT'
    end
    object ibdsPointsLogBEF_BONUS_CREDIT: TFloatField
      FieldName = 'BEF_BONUS_CREDIT'
      Origin = 'POINTS_LOG.BEF_BONUS_CREDIT'
    end
    object ibdsPointsLogBEF_PRE_PAID_CREDIT: TFloatField
      FieldName = 'BEF_PRE_PAID_CREDIT'
      Origin = 'POINTS_LOG.BEF_PRE_PAID_CREDIT'
    end
    object ibdsPointsLogUSER_CODE: TIntegerField
      FieldName = 'USER_CODE'
      Origin = 'POINTS_LOG.USER_CODE'
    end
    object ibdsPointsLogBEF_POINTS_AVAILABLE: TFloatField
      FieldName = 'BEF_POINTS_AVAILABLE'
      Origin = '"POINTS_LOG"."BEF_POINTS_AVAILABLE"'
    end
    object ibdsPointsLogPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"POINTS_LOG"."POINTS_AVAILABLE"'
    end
    object ibdsPointsLogFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"POINTS_LOG"."FULL_NAME"'
      Size = 35
    end
  end
  object dsrPointsLog: TDataSource
    DataSet = ibdsPointsLog
    Left = 513
    Top = 112
  end
  object dsrMemberPoints: TDataSource
    DataSet = ibdsMemberPoints
    Left = 506
    Top = 176
  end
  object ibdsMemberPoints: TIBDataSet
    Database = IBDatabase
    Transaction = IBRollover
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into MEMBERS'
      '  (MEMBER, EARNT_CREDIT, BONUS_CREDIT, POSITIONS, TOTAL_EARNT, '
      'TOTAL_REDEEMED,PRE_PAID_CREDIT,CARD_LAST_USED ,POINTS_AVAILABLE)'
      'values'
      
        '  (:MEMBER, :EARNT_CREDIT, :BONUS_CREDIT, :POSITIONS, :TOTAL_EAR' +
        'NT, '
      
        ':TOTAL_REDEEMED,:PRE_PAID_CREDIT,:CARD_LAST_USED,  :POINTS_AVAIL' +
        'ABLE)')
    RefreshSQL.Strings = (
      'Select '
      '  BONUS_CREDIT,'
      '  EARNT_CREDIT,'
      '  PRE_PAID_CREDIT,'
      '  POINTS_AVAILABLE,'
      '  TOTAL_EARNT,'
      '  TOTAL_REDEEMED,'
      '  PRE_PAID_CREDIT,'
      '  MEMBER,'
      'FULL_NAME'
      'CARD_LAST_USED'
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER,EARNT_CREDIT, BONUS_CREDIT, POSITIONS, TOTAL_EARNT' +
        ',   POINTS_AVAILABLE,TOTAL_REDEEMED,PRE_PAID_CREDIT,CARD_LAST_US' +
        'ED,FULL_NAME from MEMBERS where MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  MEMBER = :MEMBER,'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  POSITIONS = :POSITIONS,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '    POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  CARD_LAST_USED = :CARD_LAST_USED'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 498
    Top = 184
    object ibdsMemberPointsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'MEMBERS.MEMBER'
      Required = True
    end
    object ibdsMemberPointsEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
    end
    object ibdsMemberPointsBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object ibdsMemberPointsPOSITIONS: TIBStringField
      FieldName = 'POSITIONS'
      Origin = '"MEMBERS"."POSITIONS"'
      Size = 22
    end
    object ibdsMemberPointsTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object ibdsMemberPointsTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object ibdsMemberPointsPRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
    end
    object ibdsMemberPointsCARD_LAST_USED: TDateField
      FieldName = 'CARD_LAST_USED'
      Origin = '"MEMBERS"."CARD_LAST_USED"'
    end
    object ibdsMemberPointsPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object ibdsMemberPointsFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
  end
  object qryPointsLog: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    OnCalcFields = qryPointsLogCalcFields
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'Select * from POINTS_LOG'
      ''
      '')
    Left = 521
    Top = 248
    object qryPointsLogBEF_POINTS_AVAILABLE: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_POINTS_AVAILABLE'
      Origin = '"POINTS_LOG"."BEF_POINTS_AVAILABLE"'
      currency = True
    end
    object qryPointsLogPOINTS_AVAILABLE: TFloatField
      DisplayWidth = 10
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"POINTS_LOG"."POINTS_AVAILABLE"'
      currency = True
    end
    object qryPointsLogPOINTS_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'POINTS_ID'
      Origin = 'POINTS_LOG.POINTS_ID'
      Required = True
      Visible = False
    end
    object qryPointsLogPROCESS: TSmallintField
      DisplayWidth = 10
      FieldName = 'PROCESS'
      Origin = 'POINTS_LOG.PROCESS'
      Visible = False
    end
    object qryPointsLogREASON: TIBStringField
      DisplayWidth = 35
      FieldName = 'REASON'
      Origin = 'POINTS_LOG.REASON'
      Visible = False
      Size = 35
    end
    object qryPointsLogTOTAL_EARNT: TFloatField
      DisplayWidth = 10
      FieldName = 'TOTAL_EARNT'
      Origin = 'POINTS_LOG.TOTAL_EARNT'
      Visible = False
      currency = True
    end
    object qryPointsLogTOTAL_REDEEMED: TFloatField
      DisplayWidth = 10
      FieldName = 'TOTAL_REDEEMED'
      Origin = 'POINTS_LOG.TOTAL_REDEEMED'
      Visible = False
      currency = True
    end
    object qryPointsLogEARNT_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'EARNT_CREDIT'
      Origin = 'POINTS_LOG.EARNT_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogBONUS_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'BONUS_CREDIT'
      Origin = 'POINTS_LOG.BONUS_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogPOINTS_VALUE: TFloatField
      DisplayWidth = 10
      FieldName = 'POINTS_VALUE'
      Origin = 'POINTS_LOG.POINTS_VALUE'
      Visible = False
      currency = True
    end
    object qryPointsLogMEMBER: TIntegerField
      DisplayWidth = 10
      FieldName = 'MEMBER'
      Origin = 'POINTS_LOG.MEMBER'
      Visible = False
    end
    object qryPointsLogEDIT_DATE: TDateTimeField
      DisplayWidth = 18
      FieldName = 'EDIT_DATE'
      Origin = 'POINTS_LOG.EDIT_DATE'
      Visible = False
    end
    object qryPointsLogPRE_PAID_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'PRE_PAID_CREDIT'
      Origin = 'POINTS_LOG.PRE_PAID_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogBEF_TOTAL_EARNT: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_TOTAL_EARNT'
      Origin = 'POINTS_LOG.BEF_TOTAL_EARNT'
      Visible = False
      currency = True
    end
    object qryPointsLogBEF_TOTAL_REDEEMED: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_TOTAL_REDEEMED'
      Origin = 'POINTS_LOG.BEF_TOTAL_REDEEMED'
      Visible = False
      currency = True
    end
    object qryPointsLogBEF_EARNT_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_EARNT_CREDIT'
      Origin = 'POINTS_LOG.BEF_EARNT_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogBEF_BONUS_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_BONUS_CREDIT'
      Origin = 'POINTS_LOG.BEF_BONUS_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogBEF_PRE_PAID_CREDIT: TFloatField
      DisplayWidth = 10
      FieldName = 'BEF_PRE_PAID_CREDIT'
      Origin = 'POINTS_LOG.BEF_PRE_PAID_CREDIT'
      Visible = False
      currency = True
    end
    object qryPointsLogUSER_CODE: TIntegerField
      DisplayWidth = 10
      FieldName = 'USER_CODE'
      Origin = 'POINTS_LOG.USER_CODE'
      Visible = False
    end
    object qryPointsLogVALUE_OF_CREDIT: TFloatField
      DisplayWidth = 10
      FieldKind = fkCalculated
      FieldName = 'VALUE_OF_CREDIT'
      Visible = False
      currency = True
      Calculated = True
    end
    object qryPointsLogBEF_VALUE_OF_CREDIT: TFloatField
      DisplayWidth = 10
      FieldKind = fkCalculated
      FieldName = 'BEF_VALUE_OF_CREDIT'
      Visible = False
      currency = True
      Calculated = True
    end
  end
  object dsrQryPointsLog: TDataSource
    DataSet = qryPointsLog
    Left = 529
    Top = 256
  end
  object MainMenu1: TMainMenu
    Left = 528
    Top = 48
  end
  object ibdsMailingLabels: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterPost = ibdsMailingLabelsAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  MISC,'
      '  NUMBER_COLUMNS,'
      '  COLUMN_SPACE,'
      '  LEFT_MARGIN,'
      '  RIGHT_MARGIN,'
      '  TOP_MARGIN,'
      '  BOTTOM_MARGIN,'
      '  BAND_HEIGHT,'
      'FONT_SIZE'
      'from MISC '
      'where'
      '  MISC = :MISC')
    SelectSQL.Strings = (
      
        'select RIGHT_MARGIN, LEFT_MARGIN, TOP_MARGIN, BAND_HEIGHT,BOTTOM' +
        '_MARGIN, COLUMN_SPACE, NUMBER_COLUMNS,"MISC" ,FONT_SIZE from MIS' +
        'C')
    ModifySQL.Strings = (
      'update MISC'
      'set'
      '  RIGHT_MARGIN = :RIGHT_MARGIN,'
      '  LEFT_MARGIN = :LEFT_MARGIN,'
      '  TOP_MARGIN = :TOP_MARGIN,'
      '  BAND_HEIGHT = :BAND_HEIGHT,'
      '  BOTTOM_MARGIN = :BOTTOM_MARGIN,'
      '  COLUMN_SPACE = :COLUMN_SPACE,'
      '  NUMBER_COLUMNS = :NUMBER_COLUMNS,'
      '  MISC = :MISC,'
      'FONT_SIZE = :FONT_SIZE'
      'where'
      '  MISC = :OLD_MISC')
    Left = 520
    Top = 384
    object ibdsMailingLabelsRIGHT_MARGIN: TIBBCDField
      FieldName = 'RIGHT_MARGIN'
      Origin = '"MISC"."RIGHT_MARGIN"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsTOP_MARGIN: TIBBCDField
      FieldName = 'TOP_MARGIN'
      Origin = '"MISC"."TOP_MARGIN"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsBOTTOM_MARGIN: TIBBCDField
      FieldName = 'BOTTOM_MARGIN'
      Origin = '"MISC"."BOTTOM_MARGIN"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsCOLUMN_SPACE: TIBBCDField
      FieldName = 'COLUMN_SPACE'
      Origin = '"MISC"."COLUMN_SPACE"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsNUMBER_COLUMNS: TSmallintField
      FieldName = 'NUMBER_COLUMNS'
      Origin = '"MISC"."NUMBER_COLUMNS"'
    end
    object ibdsMailingLabelsMISC: TSmallintField
      FieldName = 'MISC'
      Origin = '"MISC"."MISC"'
    end
    object ibdsMailingLabelsLEFT_MARGIN: TIBBCDField
      FieldName = 'LEFT_MARGIN'
      Origin = '"MISC"."LEFT_MARGIN"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsBAND_HEIGHT: TIBBCDField
      FieldName = 'BAND_HEIGHT'
      Origin = '"MISC"."BAND_HEIGHT"'
      Precision = 9
      Size = 2
    end
    object ibdsMailingLabelsFONT_SIZE: TSmallintField
      FieldName = 'FONT_SIZE'
      Origin = '"MISC"."FONT_SIZE"'
    end
  end
  object dsrMailingLabels: TDataSource
    DataSet = ibdsMailingLabels
    Left = 520
    Top = 424
  end
  object cdsTemp: TClientDataSet
    Aggregates = <>
    FieldDefs = <
      item
        Name = 'cdsTempField1'
      end>
    IndexDefs = <>
    Params = <>
    StoreDefs = True
    Left = 272
    Top = 8
  end
  object ibdsMemberDelete: TIBDataSet
    Database = IBDatabase
    Transaction = IBRunOnce
    AutoCalcFields = False
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER'
      'from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER, POINTS_AVAILABLE, EARNT_CREDIT, TOTAL_EARNT, TOTA' +
        'L_REDEEMED, PRE_PAID_CREDIT, BONUS_CREDIT,FULL_NAME  from MEMBER' +
        'S where CARD_TYPE = '#39'N'#39
      'and SUB_PAID = 0  or  CARD_TYPE is NULL and SUB_PAID = 0')
    UniDirectional = True
    Left = 377
    Top = 437
    object ibdsMemberDeleteMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS"."MEMBER"'
      Required = True
    end
    object ibdsMemberDeletePOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object ibdsMemberDeleteEARNT_CREDIT: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
    end
    object ibdsMemberDeleteTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object ibdsMemberDeleteTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object ibdsMemberDeletePRE_PAID_CREDIT: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
    end
    object ibdsMemberDeleteBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object ibdsMemberDeleteFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
  end
  object tblKiosk: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = tblSubsAfterPost
    AfterPost = tblSubsAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'KIOSK_ID'
        Attributes = [faRequired]
        DataType = ftSmallint
      end
      item
        Name = 'MESSAGE'
        DataType = ftString
        Size = 80
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY56'
        Fields = 'KIOSK_ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'KIOSK'
    Left = 440
    Top = 441
    object tblKioskKIOSK_ID: TSmallintField
      DisplayLabel = 'Visit Number'
      DisplayWidth = 15
      FieldName = 'KIOSK_ID'
      Required = True
    end
    object tblKioskMESSAGE: TIBStringField
      DisplayLabel = 'Message'
      DisplayWidth = 76
      FieldName = 'MESSAGE'
      Size = 80
    end
  end
  object dsrKiosk: TDataSource
    DataSet = tblKiosk
    Left = 464
    Top = 433
  end
  object ibdsPointsAllocation: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsPointsAllocationBeforeDelete
    BeforeInsert = ibdsPointsAllocationBeforeInsert
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from POINTS_SPLIT'
      'where'
      '  POINTS_SPLIT_ID = :OLD_POINTS_SPLIT_ID')
    InsertSQL.Strings = (
      'insert into POINTS_SPLIT'
      '  (POINTS, DONATION_POINTS, UPDATED)'
      'values'
      '  (:POINTS, :DONATION_POINTS, :UPDATED)')
    RefreshSQL.Strings = (
      'Select '
      '  POINTS_SPLIT_ID,'
      '  DATE_TIME,'
      '  MEMBER,'
      '  POINTS,'
      '  DONATION_MEMBER,'
      '  DONATION_POINTS,'
      '  LAST_POINTS_AVAILABLE,'
      '  LAST_DONATED_POINTS_AVAILABLE,'
      '  UPDATED'
      'from POINTS_SPLIT '
      'where'
      '  POINTS_SPLIT_ID = :POINTS_SPLIT_ID')
    SelectSQL.Strings = (
      
        'select p.DATE_TIME, p.DONATION_MEMBER, p.DONATION_POINTS, p.LAST' +
        '_DONATED_POINTS_AVAILABLE, p.LAST_POINTS_AVAILABLE, p.MEMBER, p.' +
        'POINTS, p.POINTS_SPLIT_ID, p.UPDATED,m.FULL_NAME from POINTS_SPL' +
        'IT p ,MEMBERS m'
      'where m.MEMBER = p.MEMBER'
      'and p.UPDATED = '#39'N'#39
      'and p.DONATION_MEMBER > 0')
    ModifySQL.Strings = (
      'update POINTS_SPLIT'
      'set'
      '  POINTS = :POINTS,'
      '  DONATION_POINTS = :DONATION_POINTS,'
      '  UPDATED = :UPDATED'
      'where'
      '  POINTS_SPLIT_ID = :OLD_POINTS_SPLIT_ID')
    Left = 288
    Top = 353
    object ibdsPointsAllocationDATE_TIME: TDateTimeField
      DisplayLabel = 'Date Time'
      DisplayWidth = 17
      FieldName = 'DATE_TIME'
      Origin = '"POINTS_SPLIT"."DATE_TIME"'
    end
    object ibdsPointsAllocationMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      Origin = '"POINTS_SPLIT"."MEMBER"'
    end
    object ibdsPointsAllocationFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 20
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
    object ibdsPointsAllocationPOINTS: TIBBCDField
      DisplayLabel = 'Points'
      DisplayWidth = 8
      FieldName = 'POINTS'
      Origin = '"POINTS_SPLIT"."POINTS"'
      Precision = 9
      Size = 2
    end
    object ibdsPointsAllocationLAST_POINTS_AVAILABLE: TIBBCDField
      DisplayLabel = 'Last Pts Avail'
      DisplayWidth = 11
      FieldName = 'LAST_POINTS_AVAILABLE'
      Origin = '"POINTS_SPLIT"."LAST_POINTS_AVAILABLE"'
      Precision = 9
      Size = 2
    end
    object ibdsPointsAllocationDONATION_MEMBER: TIntegerField
      DisplayLabel = 'Donation Member'
      DisplayWidth = 14
      FieldName = 'DONATION_MEMBER'
      Origin = '"POINTS_SPLIT"."DONATION_MEMBER"'
    end
    object ibdsPointsAllocationDONATION_POINTS: TIBBCDField
      DisplayLabel = 'Donation Points'
      DisplayWidth = 12
      FieldName = 'DONATION_POINTS'
      Origin = '"POINTS_SPLIT"."DONATION_POINTS"'
      Precision = 9
      Size = 2
    end
    object ibdsPointsAllocationLAST_DONATED_POINTS_AVAILABLE: TIBBCDField
      DisplayLabel = 'ldpa'
      DisplayWidth = 10
      FieldName = 'LAST_DONATED_POINTS_AVAILABLE'
      Origin = '"POINTS_SPLIT"."LAST_DONATED_POINTS_AVAILABLE"'
      Visible = False
      Precision = 9
      Size = 2
    end
    object ibdsPointsAllocationPOINTS_SPLIT_ID: TIntegerField
      FieldName = 'POINTS_SPLIT_ID'
      Origin = '"POINTS_SPLIT"."POINTS_SPLIT_ID"'
      Required = True
      Visible = False
    end
    object ibdsPointsAllocationUPDATED: TIBStringField
      FieldName = 'UPDATED'
      Origin = '"POINTS_SPLIT"."UPDATED"'
      Visible = False
      FixedChar = True
      Size = 1
    end
  end
  object dsrPointsAllocation: TDataSource
    DataSet = ibdsPointsAllocation
    Left = 280
    Top = 378
  end
  object ibdsPointsAlloc: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS'
      'where'
      '  MEMBER = :OLD_MEMBER')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  POINTS_AVAILABLE,'
      'BONUS_CREDIT,TOTAL_EARNT '
      'from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER, POINTS_AVAILABLE,BONUS_CREDIT,TOTAL_EARNT from ME' +
        'MBERS'
      'where MEMBER = :MEMBER'
      '')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  MEMBER = :MEMBER,'
      '  POINTS_AVAILABLE = :POINTS_AVAILABLE,'
      '  BONUS_CREDIT =  :BONUS_CREDIT,'
      '  TOTAL_EARNT  = :TOTAL_EARNT'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 55
    Top = 401
    object ibdsPointsAllocMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS"."MEMBER"'
      Required = True
    end
    object ibdsPointsAllocPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object ibdsPointsAllocBONUS_CREDIT: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object ibdsPointsAllocTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
  end
  object PHMExportTimer: TTimer
    Enabled = False
    Interval = 15000
    OnTimer = PHMExportTimerTimer
    Left = 192
  end
  object ibdsPHMPoints: TIBDataSet
    Database = IBDatabase
    Transaction = IBExport
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      'POINTS_AVAILABLE,'
      'TOTAL_EARNT,'
      'TOTAL_REDEEMED,'
      'CARD_LAST_USED'
      'from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER, POINTS_AVAILABLE, TOTAL_EARNT, TOTAL_REDEEMED, CA' +
        'RD_LAST_USED from MEMBERS'
      'where MEMBER = :MEMBER'
      '')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  MEMBER = :MEMBER,'
      '  POINTS_AVAILABLE = :POINTS_AVAILABLE,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      'CARD_LAST_USED = :CARD_LAST_USED'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 32
    Top = 456
    object ibdsPHMPointsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS"."MEMBER"'
      Required = True
    end
    object ibdsPHMPointsPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object ibdsPHMPointsTOTAL_EARNT: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object ibdsPHMPointsTOTAL_REDEEMED: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object ibdsPHMPointsCARD_LAST_USED: TDateField
      FieldName = 'CARD_LAST_USED'
      Origin = '"MEMBERS"."CARD_LAST_USED"'
    end
  end
  object dsrMenuMate: TDataSource
    DataSet = tblMenuMate
    Left = 576
    Top = 8
  end
  object tblMenuMate: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'MISC'
        DataType = ftSmallint
      end
      item
        Name = 'MEMBER_EXPIRY'
        DataType = ftDate
      end
      item
        Name = 'MEMBER_PRE_EXPIRY'
        DataType = ftSmallint
      end
      item
        Name = 'KEEP_INACTIVE'
        DataType = ftInteger
      end
      item
        Name = 'SUBS_ROLL'
        DataType = ftDate
      end
      item
        Name = 'LUCKY_ROLL'
        DataType = ftDate
      end
      item
        Name = 'REVOLVE_DAYS'
        DataType = ftInteger
      end
      item
        Name = 'NAME_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'M_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'M_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'M_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'A_BACKUP_TIME'
        DataType = ftTime
      end
      item
        Name = 'A_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_FILE_NAMES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_KEEP_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'MSG'
        DataType = ftString
        Size = 200
      end
      item
        Name = 'AREV_IMPORT_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CAPITALS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_CARD'
        DataType = ftSmallint
      end
      item
        Name = 'PAYMENT_RECEIPT'
        DataType = ftSmallint
      end
      item
        Name = 'DEFAULT_CARDID'
        DataType = ftSmallint
      end
      item
        Name = 'SECTION_REVOLVE_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'EXPORT_TIMER'
        DataType = ftInteger
      end
      item
        Name = 'DISCOUNT_3'
        DataType = ftBCD
        Precision = 9
        Size = 3
      end
      item
        Name = 'ANNUAL_CARD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DISABLE_SERVICES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_GRIDS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 3
      end
      item
        Name = 'CONFIRM_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_ENCODE_MEMBER'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PART_PAY_ADVANCE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'POS_PAY_CARD_REQD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BOTTOM_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'TOP_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'CHECKED_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'DEFAULT_NUMBER'
        DataType = ftSmallint
      end
      item
        Name = 'PAY_BEFORE_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'RECEIPT_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'EXPORT_SUBS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'POINTS_PASSWORD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'NUMBER_COLUMNS'
        DataType = ftSmallint
      end
      item
        Name = 'COLUMN_SPACE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'LEFT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'RIGHT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'TOP_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BOTTOM_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BAND_HEIGHT'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'FONT_SIZE'
        DataType = ftSmallint
      end
      item
        Name = 'PRINT_MEMBER_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_MSG'
        DataType = ftBlob
        Size = 8
      end
      item
        Name = 'AREV_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'MENUMATE_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_PATH'
        DataType = ftString
        Size = 25
      end
      item
        Name = 'PHM_CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'TRIGGER_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SWIPE_TEXT'
        DataType = ftString
        Size = 62
      end
      item
        Name = 'ON_INSERT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MONDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'TUESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'WEDNESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'THURSDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'FRIDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SATURDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SUNDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BIRTHDAY_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'LUCKY_DRAW_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'KIOSK_MEMBER_START'
        DataType = ftSmallint
      end
      item
        Name = 'KIOSK_MEMBER_LENGHT'
        DataType = ftSmallint
      end
      item
        Name = 'BIRTHDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'VISIT_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'BIRTHDAY_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'LUCKY_DRAW_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'MONDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'TUESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'WEDNESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'THURSDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'FRIDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SATURDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SUNDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'KNOWN_AS_DEFAULTED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PARENT_CODE_ADDRESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'GROUP_DELETE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MENUMATE_POINTS_DATE'
        DataType = ftDate
      end
      item
        Name = 'KIOSK_DRAW_DATE'
        DataType = ftDate
      end
      item
        Name = 'KIOSK_SUCCESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_ERROR_MSG'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'KIOSK_DRAWS'
        DataType = ftSmallint
      end
      item
        Name = 'PRINT_RECEIPT_AS_TEXT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SWIPE_LENGTH'
        DataType = ftSmallint
      end
      item
        Name = 'POINTS_AS_DOLLARS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SHOW_DICE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CHECK_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'VIP_CREDIT_NAME'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'KIOSK_TIMER'
        DataType = ftSmallint
      end
      item
        Name = 'CHECK_SUBS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CHECK_GROUPS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MAX_SUB'
        DataType = ftSmallint
      end
      item
        Name = 'MAX_GROUP'
        DataType = ftSmallint
      end
      item
        Name = 'MIN_SUB'
        DataType = ftSmallint
      end
      item
        Name = 'MIN_GROUP'
        DataType = ftSmallint
      end
      item
        Name = 'CLEAR_INTERESTS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DISCOUNTS_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY21'
        Fields = 'MISC'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'MISC'
    Left = 592
    Top = 16
    object tblMenuMateMENUMATE_INSTALLED: TIBStringField
      FieldName = 'MENUMATE_INSTALLED'
      Size = 1
    end
    object tblMenuMateTRIGGER_INSTALLED: TIBStringField
      FieldName = 'TRIGGER_INSTALLED'
      Size = 1
    end
    object tblMenuMateSWIPE_TEXT: TIBStringField
      FieldName = 'SWIPE_TEXT'
      Size = 60
    end
    object tblMenuMateON_INSERT: TIBStringField
      FieldName = 'ON_INSERT'
      Size = 1
    end
    object tblMenuMateSWIPE_LENGTH: TSmallintField
      FieldName = 'SWIPE_LENGTH'
    end
    object tblMenuMateVIP_CREDIT_NAME: TIBStringField
      FieldName = 'VIP_CREDIT_NAME'
    end
    object tblMenuMateDISCOUNTS_INSTALLED: TIBStringField
      FieldName = 'DISCOUNTS_INSTALLED'
      Size = 1
    end
  end
  object ibdsMenuMate: TIBDataSet
    Database = IBDatabase
    Transaction = IBRunOnce
    AutoCalcFields = False
    BufferChunks = 32
    CachedUpdates = False
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER,'
      '  EARN_POINTS,'
      '  SWIPE_CARD,'
      'ALLOW_CREDIT,'
      'FULL_NAME'
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER,FULL_NAME,SWIPE_CARD,EARN_POINTS,ALLOW_CREDIT from' +
        ' MEMBERS ')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '    SWIPE_CARD = :SWIPE_CARD,'
      '    EARN_POINTS = :EARN_POINTS,'
      '   ALLOW_CREDIT = :ALLOW_CREDIT'
      'where'
      '  MEMBER = :OLD_MEMBER')
    UniDirectional = True
    Left = 377
    Top = 390
    object ibdsMenuMateMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS"."MEMBER"'
      Required = True
    end
    object ibdsMenuMateSWIPE_CARD: TIBStringField
      FieldName = 'SWIPE_CARD'
      Origin = '"MEMBERS"."SWIPE_CARD"'
      Size = 128
    end
    object ibdsMenuMateEARN_POINTS: TIBStringField
      FieldName = 'EARN_POINTS'
      Origin = '"MEMBERS"."EARN_POINTS"'
      FixedChar = True
      Size = 1
    end
    object ibdsMenuMateFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
    object ibdsMenuMateALLOW_CREDIT: TIBStringField
      FieldName = 'ALLOW_CREDIT'
      Origin = '"MEMBERS"."ALLOW_CREDIT"'
      FixedChar = True
      Size = 1
    end
  end
  object ibdsPrefDetail: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterInsert = ibdsPrefSubHeadingAfterInsert
    AfterPost = ibdsPrefSubHeadingAfterPost
    BeforePost = ibdsPrefDetailBeforePost
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from PREF_DETAIL'
      'where'
      'DETAIL_ID = :DETAIL_ID')
    InsertSQL.Strings = (
      'insert into PREF_DETAIL'
      
        '  (DISPLAY_DETAIL,DETAIL_ID,DETAIL_DESC, DETAIL_TYPE, HEADING_ID' +
        ',LINE_NUMBER,FIELD_DESC )'
      'values'
      
        '  (:DISPLAY_DETAIL,:DETAIL_ID,:DETAIL_DESC, :DETAIL_TYPE, :HEADI' +
        'NG_ID,:LINE_NUMBER,:FIELD_DESC )')
    RefreshSQL.Strings = (
      'Select '
      '  HEADING_ID,'
      '  DETAIL_DESC,'
      '  DETAIL_TYPE,'
      'LINE_NUMBER,'
      'FIELD_DESC ,'
      ' DETAIL_ID,'
      'DISPLAY_DETAIL'
      'from PREF_DETAIL '
      'where'
      'DETAIL_ID = :OLD_DETAIL_ID')
    SelectSQL.Strings = (
      'select DETAIL_DESC, DETAIL_TYPE, HEADING_ID,LINE_NUMBER,'
      'FIELD_DESC, DETAIL_ID,DISPLAY_DETAIL from PREF_DETAIL'
      'where HEADING_ID  = :HEADING_ID '
      'order by LINE_NUMBER')
    ModifySQL.Strings = (
      'update PREF_DETAIL'
      'set'
      '  DETAIL_DESC = :DETAIL_DESC,'
      '  DETAIL_TYPE = :DETAIL_TYPE,'
      '  HEADING_ID = :HEADING_ID,'
      'LINE_NUMBER  = :LINE_NUMBER,'
      'FIELD_DESC = :FIELD_DESC ,'
      'DISPLAY_DETAIL = :DISPLAY_DETAIL'
      'where'
      'DETAIL_ID = :OLD_DETAIL_ID')
    DataSource = dsrPrefHeading
    Left = 256
    Top = 402
    object ibdsPrefDetailDETAIL_DESC: TIBStringField
      DisplayLabel = 'Detail Name'
      DisplayWidth = 32
      FieldName = 'DETAIL_DESC'
      Origin = '"PREF_DETAIL"."DETAIL_DESC"'
      Size = 60
    end
    object ibdsPrefDetailDETAIL_TYPE: TIBStringField
      DisplayLabel = 'Detail Type'
      DisplayWidth = 13
      FieldName = 'DETAIL_TYPE'
      Origin = '"PREF_DETAIL"."DETAIL_TYPE"'
      FixedChar = True
      Size = 1
    end
    object ibdsPrefDetailLINE_NUMBER: TSmallintField
      DisplayLabel = 'Line Number'
      DisplayWidth = 10
      FieldName = 'LINE_NUMBER'
      Origin = '"PREF_DETAIL"."LINE_NUMBER"'
      Required = True
    end
    object ibdsPrefDetailDISPLAY_DETAIL: TIBStringField
      DisplayLabel = 'Display Detail'
      DisplayWidth = 12
      FieldName = 'DISPLAY_DETAIL'
      Origin = '"PREF_DETAIL"."DISPLAY_DETAIL"'
      FixedChar = True
      Size = 1
    end
    object ibdsPrefDetailDETAIL_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'DETAIL_ID'
      Origin = '"PREF_DETAIL"."DETAIL_ID"'
      Required = True
      Visible = False
    end
    object ibdsPrefDetailHEADING_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'HEADING_ID'
      Origin = '"PREF_DETAIL"."HEADING_ID"'
      Required = True
      Visible = False
    end
    object ibdsPrefDetailFIELD_DESC: TIBStringField
      FieldName = 'FIELD_DESC'
      Origin = '"PREF_DETAIL"."FIELD_DESC"'
      Visible = False
      Size = 60
    end
  end
  object ibdsAddress: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    RefreshSQL.Strings = (
      '')
    SelectSQL.Strings = (
      
        'select ADDRESS, ADDRESS1, DISTRICT, CITY, COUNTRY, MAILING_LIST_' +
        'CODE, MEMBER,FULL_NAME from MEMBERS'
      'where MAILING_LIST_CODE = :MailingListCode')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  ADDRESS = :ADDRESS,'
      '  ADDRESS1 = :ADDRESS1,'
      '  DISTRICT = :DISTRICT,'
      '  CITY = :CITY,'
      '  COUNTRY = :COUNTRY'
      'where'
      '  MEMBER = :OLD_MEMBER')
    Left = 511
    Top = 145
    object ibdsAddressADDRESS: TIBStringField
      FieldName = 'ADDRESS'
      Origin = '"MEMBERS"."ADDRESS"'
      Size = 30
    end
    object ibdsAddressADDRESS1: TIBStringField
      FieldName = 'ADDRESS1'
      Origin = '"MEMBERS"."ADDRESS1"'
      Size = 30
    end
    object ibdsAddressDISTRICT: TIBStringField
      FieldName = 'DISTRICT'
      Origin = '"MEMBERS"."DISTRICT"'
    end
    object ibdsAddressCITY: TIBStringField
      FieldName = 'CITY'
      Origin = '"MEMBERS"."CITY"'
      Size = 25
    end
    object ibdsAddressCOUNTRY: TIBStringField
      FieldName = 'COUNTRY'
      Origin = '"MEMBERS"."COUNTRY"'
      Size = 25
    end
    object ibdsAddressMAILING_LIST_CODE: TIntegerField
      FieldName = 'MAILING_LIST_CODE'
      Origin = '"MEMBERS"."MAILING_LIST_CODE"'
    end
    object ibdsAddressMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS"."MEMBER"'
      Required = True
    end
    object ibdsAddressFULL_NAME: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
  end
  object tblKioskDraws: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    AfterInsert = tblKioskDrawsAfterInsert
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'KIOSK_ID'
        DataType = ftSmallint
      end
      item
        Name = 'KIOSK_DESC'
        DataType = ftString
        Size = 25
      end>
    StoreDefs = True
    TableName = 'KIOSK_DRAWS'
    Left = 504
    Top = 112
    object tblKioskDrawsKIOSK_ID: TSmallintField
      DisplayLabel = 'Kiosk Number'
      DisplayWidth = 11
      FieldName = 'KIOSK_ID'
    end
    object tblKioskDrawsKIOSK_DESC: TIBStringField
      DisplayLabel = 'Description'
      DisplayWidth = 31
      FieldName = 'KIOSK_DESC'
      Size = 25
    end
  end
  object dsrKioskDraws: TDataSource
    DataSet = tblKioskDraws
    Left = 504
    Top = 88
  end
  object ibdsMenuMatePoints: TIBDataSet
    Database = IBDatabase
    Transaction = IBRunOnce
    BufferChunks = 1000
    CachedUpdates = False
    InsertSQL.Strings = (
      'insert into MENUMATE_POINTS'
      '  (MEMBER, POINTS_AVAILABLE, POINTS_DIFFERENCE, THE_DATE)'
      'values'
      '  (:MEMBER, :POINTS_AVAILABLE, :POINTS_DIFFERENCE, :THE_DATE)')
    SelectSQL.Strings = (
      
        'select MEMBER, POINTS_AVAILABLE, POINTS_DIFFERENCE, THE_DATE fro' +
        'm MENUMATE_POINTS where'
      'member = -1')
    Left = 56
    Top = 425
    object ibdsMenuMatePointsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MENUMATE_POINTS"."MEMBER"'
      Required = True
    end
    object ibdsMenuMatePointsPOINTS_AVAILABLE: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MENUMATE_POINTS"."POINTS_AVAILABLE"'
    end
    object ibdsMenuMatePointsPOINTS_DIFFERENCE: TFloatField
      FieldName = 'POINTS_DIFFERENCE'
      Origin = '"MENUMATE_POINTS"."POINTS_DIFFERENCE"'
    end
    object ibdsMenuMatePointsTHE_DATE: TDateField
      FieldName = 'THE_DATE'
      Origin = '"MENUMATE_POINTS"."THE_DATE"'
    end
  end
  object ibdsPrefHeading: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterInsert = ibdsPrefHeadingAfterInsert
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from PREF_HEADING'
      'where'
      '  HEADING_ID = :OLD_HEADING_ID')
    InsertSQL.Strings = (
      'insert into PREF_HEADING'
      '  (HEADING_DESC, HEADING_ID)'
      'values'
      '  (:HEADING_DESC, :HEADING_ID)')
    RefreshSQL.Strings = (
      'Select '
      '  HEADING_ID,'
      '  HEADING_DESC'
      'from PREF_HEADING '
      'where'
      '  HEADING_ID = :HEADING_ID')
    SelectSQL.Strings = (
      'select HEADING_DESC, HEADING_ID from PREF_HEADING')
    ModifySQL.Strings = (
      'update PREF_HEADING'
      'set'
      '  HEADING_DESC = :HEADING_DESC,'
      '  HEADING_ID = :HEADING_ID'
      'where'
      '  HEADING_ID = :OLD_HEADING_ID')
    Left = 136
    Top = 404
    object ibdsPrefHeadingHEADING_DESC: TIBStringField
      FieldName = 'HEADING_DESC'
      Origin = '"PREF_HEADING"."HEADING_DESC"'
      Size = 60
    end
    object ibdsPrefHeadingHEADING_ID: TIntegerField
      FieldName = 'HEADING_ID'
      Origin = '"PREF_HEADING"."HEADING_ID"'
      Required = True
    end
  end
  object ibdsPrefSubHeading: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    AfterInsert = ibdsPrefSubHeadingAfterInsert
    AfterPost = ibdsPrefSubHeadingAfterPost
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from PREF_SUB_HEADING'
      'where'
      'SUB_HEADING_ID = :SUB_HEADING_ID')
    InsertSQL.Strings = (
      'insert into PREF_SUB_HEADING'
      '  (SUB_HEADING_ID,DISPLAY_HEADING, HEADING_ID, SUB_HEADING_DESC)'
      'values'
      
        '  (:SUB_HEADING_ID,:DISPLAY_HEADING, :HEADING_ID, :SUB_HEADING_D' +
        'ESC)')
    RefreshSQL.Strings = (
      'Select '
      '  HEADING_ID,'
      '  SUB_HEADING_DESC,'
      '  DISPLAY_HEADING,'
      'SUB_HEADING_ID'
      'from PREF_SUB_HEADING '
      'where'
      'SUB_HEADING_ID = :OLD_SUB_HEADING_ID')
    SelectSQL.Strings = (
      
        'select DISPLAY_HEADING, HEADING_ID, SUB_HEADING_DESC, SUB_HEADIN' +
        'G_ID from PREF_SUB_HEADING'
      'WHERE HEADING_ID =  :HEADING_ID')
    ModifySQL.Strings = (
      'update PREF_SUB_HEADING'
      'set'
      '  DISPLAY_HEADING = :DISPLAY_HEADING,'
      '  HEADING_ID = :HEADING_ID,'
      '  SUB_HEADING_DESC = :SUB_HEADING_DESC,'
      'SUB_HEADING_ID = :SUB_HEADING_ID'
      'where'
      '  SUB_HEADING_ID = :SUB_HEADING_ID')
    DataSource = dsrPrefHeading
    Left = 200
    Top = 404
    object ibdsPrefSubHeadingSUB_HEADING_DESC: TIBStringField
      DisplayLabel = 'Sub Heading Name'
      DisplayWidth = 26
      FieldName = 'SUB_HEADING_DESC'
      Origin = '"PREF_SUB_HEADING"."SUB_HEADING_DESC"'
      Size = 60
    end
    object ibdsPrefSubHeadingDISPLAY_HEADING: TIBStringField
      DisplayLabel = 'Display Header'
      DisplayWidth = 12
      FieldName = 'DISPLAY_HEADING'
      Origin = '"PREF_SUB_HEADING"."DISPLAY_HEADING"'
      FixedChar = True
      Size = 1
    end
    object ibdsPrefSubHeadingSUB_HEADING_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'SUB_HEADING_ID'
      Origin = '"PREF_SUB_HEADING"."SUB_HEADING_ID"'
      Required = True
      Visible = False
    end
    object ibdsPrefSubHeadingHEADING_ID: TIntegerField
      DisplayWidth = 10
      FieldName = 'HEADING_ID'
      Origin = '"PREF_SUB_HEADING"."HEADING_ID"'
      Required = True
      Visible = False
    end
  end
  object dsrPrefHeading: TDataSource
    DataSet = ibdsPrefHeading
    Left = 152
    Top = 404
  end
  object dsrPrefDetail: TDataSource
    DataSet = ibdsPrefDetail
    Left = 272
    Top = 402
  end
  object dsrPrefSubHeading: TDataSource
    DataSet = ibdsPrefSubHeading
    Left = 216
    Top = 404
  end
  object tblPrefMembers: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    IndexFieldNames = 'MEMBER'
    MasterFields = 'MEMBER'
    MasterSource = dsrMembers
    TableName = 'PREF_MEMBERS'
    Left = 240
    Top = 448
  end
  object dsrPrefMembers: TDataSource
    DataSet = tblPrefMembers
    Left = 264
    Top = 444
  end
  object dsrKioskSettings: TDataSource
    DataSet = tblKioskSettings
    Left = 576
    Top = 80
  end
  object tblKioskSettings: TIBTable
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'MISC'
        DataType = ftSmallint
      end
      item
        Name = 'MEMBER_EXPIRY'
        DataType = ftDate
      end
      item
        Name = 'MEMBER_PRE_EXPIRY'
        DataType = ftSmallint
      end
      item
        Name = 'KEEP_INACTIVE'
        DataType = ftInteger
      end
      item
        Name = 'SUBS_ROLL'
        DataType = ftDate
      end
      item
        Name = 'LUCKY_ROLL'
        DataType = ftDate
      end
      item
        Name = 'REVOLVE_DAYS'
        DataType = ftInteger
      end
      item
        Name = 'NAME_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'M_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'M_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'M_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'M_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_ENABLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_DIR'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_PCNAME'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'A_BACKUP_DATE'
        DataType = ftDate
      end
      item
        Name = 'A_BACKUP_TIME'
        DataType = ftTime
      end
      item
        Name = 'A_BACKUP_TYPE'
        DataType = ftSmallint
      end
      item
        Name = 'A_BACKUP_FILE_NAMES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'A_BACKUP_KEEP_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'MSG'
        DataType = ftString
        Size = 200
      end
      item
        Name = 'AREV_IMPORT_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'CAPITALS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_CARD'
        DataType = ftSmallint
      end
      item
        Name = 'PAYMENT_RECEIPT'
        DataType = ftSmallint
      end
      item
        Name = 'DEFAULT_CARDID'
        DataType = ftSmallint
      end
      item
        Name = 'SECTION_REVOLVE_DAYS'
        DataType = ftSmallint
      end
      item
        Name = 'EXPORT_TIMER'
        DataType = ftInteger
      end
      item
        Name = 'DISCOUNT_3'
        DataType = ftBCD
        Precision = 9
        Size = 3
      end
      item
        Name = 'ANNUAL_CARD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DISABLE_SERVICES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PAYMENT_GRIDS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 3
      end
      item
        Name = 'CONFIRM_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_ENCODE_MEMBER'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PART_PAY_ADVANCE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'POS_PAY_CARD_REQD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BOTTOM_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'TOP_LINE_FEED'
        DataType = ftSmallint
      end
      item
        Name = 'CHECKED_FIELDS'
        DataType = ftString
        Size = 45
      end
      item
        Name = 'PAY_BEFORE_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'CARD_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'RECEIPT_DEFAULT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'EXPORT_SUBS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'DEFAULT_NUMBER'
        DataType = ftSmallint
      end
      item
        Name = 'POINTS_PASSWORD'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'NUMBER_COLUMNS'
        DataType = ftSmallint
      end
      item
        Name = 'COLUMN_SPACE'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'LEFT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'RIGHT_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'TOP_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BOTTOM_MARGIN'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'BAND_HEIGHT'
        DataType = ftBCD
        Precision = 9
        Size = 2
      end
      item
        Name = 'FONT_SIZE'
        DataType = ftSmallint
      end
      item
        Name = 'PRINT_MEMBER_PAYMENT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_MSG'
        DataType = ftBlob
        Size = 8
      end
      item
        Name = 'AREV_FILE_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'MENUMATE_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'TRIGGER_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_INSTALLED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PHM_CONTROLLER'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'PHM_PATH'
        DataType = ftString
        Size = 40
      end
      item
        Name = 'SWIPE_TEXT'
        DataType = ftString
        Size = 62
      end
      item
        Name = 'ON_INSERT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'MONDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'TUESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'WEDNESDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'THURSDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'FRIDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SATURDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SUNDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'BIRTHDAY_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'LUCKY_DRAW_TEXT'
        DataType = ftString
        Size = 35
      end
      item
        Name = 'BIRTHDAY_PRINT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_MEMBER_START'
        DataType = ftSmallint
      end
      item
        Name = 'KIOSK_MEMBER_LENGHT'
        DataType = ftSmallint
      end
      item
        Name = 'MONDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'TUESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'WEDNESDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'THURSDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'FRIDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SATURDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'SUNDAY_END_TIME'
        DataType = ftTime
      end
      item
        Name = 'VISIT_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'BIRTHDAY_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'LUCKY_DRAW_MSG'
        DataType = ftString
        Size = 70
      end
      item
        Name = 'KNOWN_AS_DEFAULTED'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'PARENT_CODE_ADDRESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'GROUP_DELETE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_DRAWS'
        DataType = ftSmallint
      end
      item
        Name = 'KIOSK_DRAW_DATE'
        DataType = ftDate
      end
      item
        Name = 'KIOSK_SUCCESS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_ERROR_MSG'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'MENUMATE_POINTS_DATE'
        DataType = ftDate
      end
      item
        Name = 'PRINT_RECEIPT_AS_TEXT'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SWIPE_LENGTH'
        DataType = ftSmallint
      end
      item
        Name = 'POINTS_AS_DOLLARS'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SHOW_DICE'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end
      item
        Name = 'KIOSK_TIMER'
        DataType = ftSmallint
      end
      item
        Name = 'CHECK_EXPIRES'
        Attributes = [faFixed]
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY21'
        Fields = 'MISC'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'MISC'
    Left = 592
    Top = 88
    object tblKioskSettingsSHOW_DICE: TIBStringField
      FieldName = 'SHOW_DICE'
      Size = 1
    end
    object tblKioskSettingsKIOSK_TIMER: TSmallintField
      FieldName = 'KIOSK_TIMER'
    end
    object tblKioskSettingsCHECK_EXPIRES: TIBStringField
      FieldName = 'CHECK_EXPIRES'
      Size = 1
    end
    object tblKioskSettingsPOINTS_AS_DOLLARS: TIBStringField
      FieldName = 'POINTS_AS_DOLLARS'
      Size = 1
    end
    object tblKioskSettingsKIOSK_MEMBER_START: TSmallintField
      FieldName = 'KIOSK_MEMBER_START'
    end
    object tblKioskSettingsKIOSK_MEMBER_LENGHT: TSmallintField
      FieldName = 'KIOSK_MEMBER_LENGHT'
    end
  end
  object ibdsPointsAudit: TIBDataSet
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from POINTS_AUDIT'
      'where'
      '  POINTS_AUDIT_ID = :OLD_POINTS_AUDIT_ID')
    InsertSQL.Strings = (
      'insert into POINTS_AUDIT'
      '  (POINTS_AUDIT_ID, ADJUSTED_POINTS, BIRTHDAY_POINTS, '
      'BONUS_POINTS, DELETED_POINTS, '
      
        '   EDIT_DATE, LOCATION, LUCKY_POINTS, POINTS_EARNT, POINTS_SPENT' +
        ', '
      'SUBS_POINTS, '
      '   VIP_CREDIT)'
      'values'
      '  (:POINTS_AUDIT_ID, :ADJUSTED_POINTS, :BIRTHDAY_POINTS, '
      ':BONUS_POINTS, '
      '   :DELETED_POINTS, :EDIT_DATE, :LOCATION, :LUCKY_POINTS, '
      ':POINTS_EARNT, '
      '   :POINTS_SPENT, :SUBS_POINTS, :VIP_CREDIT)')
    RefreshSQL.Strings = (
      'Select '
      '  POINTS_AUDIT_ID,'
      '  LOCATION,'
      '  EDIT_DATE,'
      '  POINTS_EARNT,'
      '  POINTS_SPENT,'
      '  BONUS_POINTS,'
      '  VIP_CREDIT,'
      '  ADJUSTED_POINTS,'
      '  BIRTHDAY_POINTS,'
      '  LUCKY_POINTS,'
      '  SUBS_POINTS,'
      '  DELETED_POINTS'
      'from POINTS_AUDIT '
      'where'
      '  POINTS_AUDIT_ID = :POINTS_AUDIT_ID')
    SelectSQL.Strings = (
      
        'select POINTS_AUDIT_ID,ADJUSTED_POINTS, BIRTHDAY_POINTS, BONUS_P' +
        'OINTS, DELETED_POINTS, EDIT_DATE, LOCATION, LUCKY_POINTS, POINTS' +
        '_EARNT, POINTS_SPENT, SUBS_POINTS, VIP_CREDIT from POINTS_AUDIT')
    ModifySQL.Strings = (
      'update POINTS_AUDIT'
      'set'
      '  POINTS_AUDIT_ID = :POINTS_AUDIT_ID,'
      '  ADJUSTED_POINTS = :ADJUSTED_POINTS,'
      '  BIRTHDAY_POINTS = :BIRTHDAY_POINTS,'
      '  BONUS_POINTS = :BONUS_POINTS,'
      '  DELETED_POINTS = :DELETED_POINTS,'
      '  EDIT_DATE = :EDIT_DATE,'
      '  LOCATION = :LOCATION,'
      '  LUCKY_POINTS = :LUCKY_POINTS,'
      '  POINTS_EARNT = :POINTS_EARNT,'
      '  POINTS_SPENT = :POINTS_SPENT,'
      '  SUBS_POINTS = :SUBS_POINTS,'
      '  VIP_CREDIT = :VIP_CREDIT'
      'where'
      '  POINTS_AUDIT_ID = :OLD_POINTS_AUDIT_ID')
    Left = 583
    Top = 147
  end
  object qryMenuMatePoints: TIBQuery
    Database = IBDatabase
    Transaction = IBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    SQL.Strings = (
      'Select '
      'mp.ADJUSTMENT_TYPE, '
      'mp.EARNED, '
      'mp.INVOICE_NUMBER, '
      'mp.MEMBER, '
      'mp.MEMBERS_POINTS_KEY, '
      'mp.POINTS_TYPE, '
      'mp.REDEEMED, '
      'mp.TIME_STAMP ,'
      'm.FULL_NAME from '
      'Members_Points mp left join Members m'
      'on mp.MEMBER = m.MEMBER'
      'order by Member')
    Left = 600
    Top = 224
    object qryMenuMatePointsMEMBER: TIntegerField
      DisplayLabel = 'Member'
      DisplayWidth = 7
      FieldName = 'MEMBER'
      Origin = '"MEMBERS_POINTS"."MEMBER"'
    end
    object qryMenuMatePointsFULL_NAME: TIBStringField
      DisplayLabel = 'Full Name'
      DisplayWidth = 19
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
    object qryMenuMatePointsEARNED: TIBBCDField
      DisplayLabel = 'Earned'
      DisplayWidth = 7
      FieldName = 'EARNED'
      Origin = '"MEMBERS_POINTS"."EARNED"'
      DisplayFormat = '$0.00'
      currency = True
      Precision = 18
      Size = 4
    end
    object qryMenuMatePointsREDEEMED: TIBBCDField
      DisplayLabel = 'Redeemed'
      DisplayWidth = 9
      FieldName = 'REDEEMED'
      Origin = '"MEMBERS_POINTS"."REDEEMED"'
      DisplayFormat = '$0.00'
      currency = True
      Precision = 18
      Size = 4
    end
    object qryMenuMatePointsADJUSTMENT_TYPE: TIntegerField
      DisplayLabel = 'Adj Type'
      DisplayWidth = 7
      FieldName = 'ADJUSTMENT_TYPE'
      Origin = '"MEMBERS_POINTS"."ADJUSTMENT_TYPE"'
    end
    object qryMenuMatePointsPOINTS_TYPE: TIntegerField
      DisplayLabel = 'Pts Type'
      DisplayWidth = 7
      FieldName = 'POINTS_TYPE'
      Origin = '"MEMBERS_POINTS"."POINTS_TYPE"'
    end
    object qryMenuMatePointsTIME_STAMP: TDateTimeField
      DisplayLabel = 'Date Time'
      DisplayWidth = 11
      FieldName = 'TIME_STAMP'
      Origin = '"MEMBERS_POINTS"."TIME_STAMP"'
    end
    object qryMenuMatePointsINVOICE_NUMBER: TIBStringField
      DisplayLabel = 'Invoice #'
      DisplayWidth = 16
      FieldName = 'INVOICE_NUMBER'
      Origin = '"MEMBERS_POINTS"."INVOICE_NUMBER"'
      Size = 50
    end
    object qryMenuMatePointsMEMBERS_POINTS_KEY: TIntegerField
      FieldName = 'MEMBERS_POINTS_KEY'
      Origin = '"MEMBERS_POINTS"."MEMBERS_POINTS_KEY"'
      Required = True
      Visible = False
    end
  end
  object dsrMenuMatePoints: TDataSource
    DataSet = qryMenuMatePoints
    Left = 616
    Top = 224
  end
  object ibdsM_Discounts: TIBDataSet
    Tag = 1
    Database = IBDatabase
    Transaction = IBTransaction
    AfterDelete = ibdsM_SubsAfterPost
    AfterInsert = ibdsM_DiscountsAfterInsert
    AfterPost = ibdsM_SubsAfterPost
    BeforeDelete = ibdsM_EventsBeforePost
    BeforePost = ibdsM_EventsBeforePost
    BufferChunks = 32
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS_DISCOUNTS'
      'where'
      '  DISCOUNT_KEY = :OLD_DISCOUNT_KEY and'
      '  MEMBER = :OLD_MEMBER')
    InsertSQL.Strings = (
      'insert into MEMBERS_DISCOUNTS'
      '  (DISCOUNT_KEY, MEMBER, MEMBER_DISCOUNTS_KEY)'
      'values'
      '  (:DISCOUNT_KEY, :MEMBER, :MEMBER_DISCOUNTS_KEY)')
    RefreshSQL.Strings = (
      'Select '
      '  MEMBER_DISCOUNTS_KEY,'
      '  DISCOUNT_KEY,'
      '  MEMBER'
      'from MEMBERS_DISCOUNTS '
      'where'
      '  DISCOUNT_KEY = :DISCOUNT_KEY and'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select DISCOUNT_KEY, MEMBER, MEMBER_DISCOUNTS_KEY from MEMBERS_D' +
        'ISCOUNTS'
      'where MEMBER = :MEMBER'
      'order by MEMBER_DISCOUNTS_KEY')
    ModifySQL.Strings = (
      'update MEMBERS_DISCOUNTS'
      'set'
      '  DISCOUNT_KEY = :DISCOUNT_KEY,'
      '  MEMBER = :MEMBER,'
      '  MEMBER_DISCOUNTS_KEY = :MEMBER_DISCOUNTS_KEY'
      'where'
      '  DISCOUNT_KEY = :OLD_DISCOUNT_KEY and'
      '  MEMBER = :OLD_MEMBER')
    DataSource = dsrMembers
    Left = 120
    Top = 472
    object ibdsM_DiscountsDISCOUNT_KEY: TIntegerField
      FieldName = 'DISCOUNT_KEY'
      Origin = '"MEMBERS_DISCOUNTS"."DISCOUNT_KEY"'
      Required = True
    end
    object ibdsM_DiscountsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS_DISCOUNTS"."MEMBER"'
      Required = True
    end
    object ibdsM_DiscountsMEMBER_DISCOUNTS_KEY: TIntegerField
      FieldName = 'MEMBER_DISCOUNTS_KEY'
      Origin = '"MEMBERS_DISCOUNTS"."MEMBER_DISCOUNTS_KEY"'
      Required = True
    end
    object ibdsM_DiscountsNAME: TStringField
      FieldKind = fkLookup
      FieldName = 'NAME'
      LookupDataSet = dmMM.tblDiscounts
      LookupKeyFields = 'DISCOUNT_KEY'
      LookupResultField = 'NAME'
      KeyFields = 'DISCOUNT_KEY'
      Size = 15
      Lookup = True
    end
    object ibdsM_DiscountsDESCRIPTION: TStringField
      FieldKind = fkLookup
      FieldName = 'DESCRIPTION'
      LookupDataSet = dmMM.tblDiscounts
      LookupKeyFields = 'DISCOUNT_KEY'
      LookupResultField = 'DESCRIPTION'
      KeyFields = 'DISCOUNT_KEY'
      Size = 128
      Lookup = True
    end
    object ibdsM_DiscountsMENU: TStringField
      FieldKind = fkLookup
      FieldName = 'MENU'
      LookupDataSet = dmMM.tblDiscounts
      LookupKeyFields = 'DISCOUNT_KEY'
      LookupResultField = 'MENU'
      KeyFields = 'DISCOUNT_KEY'
      Size = 15
      Lookup = True
    end
    object ibdsM_DiscountsAMOUNT: TCurrencyField
      FieldKind = fkLookup
      FieldName = 'AMOUNT'
      LookupDataSet = dmMM.tblDiscounts
      LookupKeyFields = 'DISCOUNT_KEY'
      LookupResultField = 'AMOUNT'
      KeyFields = 'DISCOUNT_KEY'
      Lookup = True
    end
    object ibdsM_DiscountsPERCENT: TFloatField
      FieldKind = fkLookup
      FieldName = 'PERCENT'
      LookupDataSet = dmMM.tblDiscounts
      LookupKeyFields = 'DISCOUNT_KEY'
      LookupResultField = 'PERCENTAGE'
      KeyFields = 'DISCOUNT_KEY'
      DisplayFormat = '0.00%'
      Lookup = True
    end
  end
  object dsrM_Discounts: TDataSource
    DataSet = ibdsM_Discounts
    Left = 168
    Top = 472
  end
end
