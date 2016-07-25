object dmMM: TdmMM
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Left = 243
  Top = 144
  Height = 480
  Width = 696
  object MMDatabase: TIBDatabase
    DatabaseName = 'C:\DB\Howick\Menumate34.GDB'
    Params.Strings = (
      'user_name=SYSDBA'
      'password=masterkey')
    LoginPrompt = False
    IdleTimer = 0
    SQLDialect = 3
    TraceFlags = []
    AllowStreamedConnected = False
    Left = 48
    Top = 8
  end
  object MMTransaction: TIBTransaction
    Active = False
    DefaultDatabase = MMDatabase
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 200
    Top = 8
  end
  object tblMMLocations: TIBTable
    Database = MMDatabase
    Transaction = MMTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'LOCATION_KEY'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NAME'
        DataType = ftString
        Size = 25
      end
      item
        Name = 'LOCATION_TYPE'
        DataType = ftInteger
      end
      item
        Name = 'PROFILE_KEY'
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY58'
        Fields = 'LOCATION_KEY'
        Options = [ixPrimary, ixUnique]
      end>
    ReadOnly = True
    StoreDefs = True
    TableName = 'LOCATIONS'
    Left = 48
    Top = 88
    object tblMMLocationsLOCATION_KEY: TIntegerField
      FieldName = 'LOCATION_KEY'
      Required = True
    end
    object tblMMLocationsNAME: TIBStringField
      FieldName = 'NAME'
      Size = 25
    end
    object tblMMLocationsLOCATION_TYPE: TIntegerField
      FieldName = 'LOCATION_TYPE'
    end
    object tblMMLocationsPROFILE_KEY: TIntegerField
      FieldName = 'PROFILE_KEY'
    end
  end
  object qryMMReport: TIBQuery
    Database = MMDatabase
    Transaction = MMTransaction
    BufferChunks = 1000
    CachedUpdates = False
    Left = 208
    Top = 88
  end
  object ibdsUpdateMemberPoints: TIBDataSet
    Database = dm.IBDatabase
    Transaction = IBMenuMatePoints
    BufferChunks = 1000
    CachedUpdates = False
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
      'FULL_NAME,'
      'CARD_LAST_USED,'
      'DONATION,DONATION_MEMBER'
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER,EARNT_CREDIT, BONUS_CREDIT, TOTAL_EARNT,   POINTS_' +
        'AVAILABLE,TOTAL_REDEEMED,PRE_PAID_CREDIT,CARD_LAST_USED,FULL_NAM' +
        'E,DONATION,DONATION_MEMBER from MEMBERS where MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      ''
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  POSITIONS = :POSITIONS,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  CARD_LAST_USED = :CARD_LAST_USED'
      'where'
      '  MEMBER = :MEMBER')
    UniDirectional = True
    Left = 370
    Top = 128
    object IntegerField7: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'MEMBERS.MEMBER'
      Required = True
    end
    object FloatField1: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
    end
    object FloatField2: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object FloatField3: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object FloatField4: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object FloatField5: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
    end
    object DateField7: TDateField
      FieldName = 'CARD_LAST_USED'
      Origin = '"MEMBERS"."CARD_LAST_USED"'
    end
    object FloatField6: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object IBStringField34: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
    object ibdsUpdateMemberPointsDONATION: TIBBCDField
      FieldName = 'DONATION'
      Origin = '"MEMBERS"."DONATION"'
      Precision = 9
      Size = 2
    end
    object ibdsUpdateMemberPointsDONATION_MEMBER: TIntegerField
      FieldName = 'DONATION_MEMBER'
      Origin = '"MEMBERS"."DONATION_MEMBER"'
    end
  end
  object ibdsMembersPoints: TIBDataSet
    Database = dm.IBDatabase
    Transaction = IBMenuMatePoints
    BufferChunks = 1000
    CachedUpdates = False
    DeleteSQL.Strings = (
      'delete from MEMBERS_POINTS'
      'where'
      '  MEMBERS_POINTS_KEY = :OLD_MEMBERS_POINTS_KEY')
    RefreshSQL.Strings = (
      '')
    SelectSQL.Strings = (
      
        'select MEMBERS_POINTS_KEY,ADJUSTMENT_TYPE, EARNED, MEMBER, POINT' +
        'S_TYPE, REDEEMED'
      ' from MEMBERS_POINTS')
    ModifySQL.Strings = (
      '')
    UniDirectional = True
    Left = 365
    Top = 72
    object ibdsMembersPointsMEMBERS_POINTS_KEY: TIntegerField
      FieldName = 'MEMBERS_POINTS_KEY'
      Origin = '"MEMBERS_POINTS"."MEMBERS_POINTS_KEY"'
      Required = True
    end
    object ibdsMembersPointsADJUSTMENT_TYPE: TIntegerField
      FieldName = 'ADJUSTMENT_TYPE'
      Origin = '"MEMBERS_POINTS"."ADJUSTMENT_TYPE"'
    end
    object ibdsMembersPointsEARNED: TIBBCDField
      FieldName = 'EARNED'
      Origin = '"MEMBERS_POINTS"."EARNED"'
      Precision = 18
      Size = 4
    end
    object ibdsMembersPointsMEMBER: TIntegerField
      FieldName = 'MEMBER'
      Origin = '"MEMBERS_POINTS"."MEMBER"'
    end
    object ibdsMembersPointsPOINTS_TYPE: TIntegerField
      FieldName = 'POINTS_TYPE'
      Origin = '"MEMBERS_POINTS"."POINTS_TYPE"'
    end
    object ibdsMembersPointsREDEEMED: TIBBCDField
      FieldName = 'REDEEMED'
      Origin = '"MEMBERS_POINTS"."REDEEMED"'
      Precision = 18
      Size = 4
    end
  end
  object IBMenuMatePoints: TIBTransaction
    Active = False
    DefaultDatabase = dm.IBDatabase
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    AutoStopAction = saNone
    Left = 352
    Top = 8
  end
  object ibdsUpdateDonationMember: TIBDataSet
    Database = dm.IBDatabase
    Transaction = IBMenuMatePoints
    BufferChunks = 1000
    CachedUpdates = False
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
      'FULL_NAME,'
      'CARD_LAST_USED,'
      'DONATION,DONATION_MEMBER'
      '  from MEMBERS '
      'where'
      '  MEMBER = :MEMBER')
    SelectSQL.Strings = (
      
        'select MEMBER,EARNT_CREDIT, BONUS_CREDIT, TOTAL_EARNT,   POINTS_' +
        'AVAILABLE,TOTAL_REDEEMED,PRE_PAID_CREDIT,CARD_LAST_USED,FULL_NAM' +
        'E,DONATION,DONATION_MEMBER from MEMBERS where MEMBER = :MEMBER')
    ModifySQL.Strings = (
      'update MEMBERS'
      'set'
      '  EARNT_CREDIT = :EARNT_CREDIT,'
      '  BONUS_CREDIT = :BONUS_CREDIT,'
      '  POSITIONS = :POSITIONS,'
      '  TOTAL_EARNT = :TOTAL_EARNT,'
      '  POINTS_AVAILABLE =   :POINTS_AVAILABLE,'
      '  TOTAL_REDEEMED = :TOTAL_REDEEMED,'
      '  PRE_PAID_CREDIT = :PRE_PAID_CREDIT,'
      '  CARD_LAST_USED = :CARD_LAST_USED'
      'where'
      '  MEMBER = :MEMBER')
    UniDirectional = True
    Left = 362
    Top = 192
    object IntegerField1: TIntegerField
      FieldName = 'MEMBER'
      Origin = 'MEMBERS.MEMBER'
      Required = True
    end
    object FloatField7: TFloatField
      FieldName = 'EARNT_CREDIT'
      Origin = '"MEMBERS"."EARNT_CREDIT"'
    end
    object FloatField8: TFloatField
      FieldName = 'BONUS_CREDIT'
      Origin = '"MEMBERS"."BONUS_CREDIT"'
    end
    object FloatField9: TFloatField
      FieldName = 'TOTAL_EARNT'
      Origin = '"MEMBERS"."TOTAL_EARNT"'
    end
    object FloatField10: TFloatField
      FieldName = 'TOTAL_REDEEMED'
      Origin = '"MEMBERS"."TOTAL_REDEEMED"'
    end
    object FloatField11: TFloatField
      FieldName = 'PRE_PAID_CREDIT'
      Origin = '"MEMBERS"."PRE_PAID_CREDIT"'
    end
    object DateField1: TDateField
      FieldName = 'CARD_LAST_USED'
      Origin = '"MEMBERS"."CARD_LAST_USED"'
    end
    object FloatField12: TFloatField
      FieldName = 'POINTS_AVAILABLE'
      Origin = '"MEMBERS"."POINTS_AVAILABLE"'
    end
    object IBStringField1: TIBStringField
      FieldName = 'FULL_NAME'
      Origin = '"MEMBERS"."FULL_NAME"'
      Size = 45
    end
    object IBBCDField1: TIBBCDField
      FieldName = 'DONATION'
      Origin = '"MEMBERS"."DONATION"'
      Precision = 9
      Size = 2
    end
    object IntegerField2: TIntegerField
      FieldName = 'DONATION_MEMBER'
      Origin = '"MEMBERS"."DONATION_MEMBER"'
    end
  end
  object sqlMemberPointsKey: TIBSQL
    Database = dm.IBDatabase
    ParamCheck = True
    SQL.Strings = (
      
        'select Max(MEMBERS_POINTS_KEY) as MaxMemberPointsKey from MEMBER' +
        'S_POINTS')
    Transaction = IBMenuMatePoints
    Left = 368
    Top = 256
  end
  object dsrDiscounts: TDataSource
    DataSet = tblDiscounts
    Left = 120
    Top = 176
  end
  object tblMMDiscounts: TIBTable
    Database = MMDatabase
    Transaction = MMTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'DISCOUNT_KEY'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NAME'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 128
      end
      item
        Name = 'MENU'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'AMOUNT'
        DataType = ftBCD
        Precision = 18
        Size = 4
      end
      item
        Name = 'PERCENTAGE'
        DataType = ftBCD
        Precision = 18
        Size = 4
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY58'
        Fields = 'LOCATION_KEY'
        Options = [ixPrimary, ixUnique]
      end>
    ReadOnly = True
    StoreDefs = True
    TableName = 'DISCOUNTS'
    Left = 136
    Top = 88
    object tblMMDiscountsDISCOUNT_KEY: TIntegerField
      FieldName = 'DISCOUNT_KEY'
      Required = True
    end
    object tblMMDiscountsNAME: TIBStringField
      FieldName = 'NAME'
      Size = 15
    end
    object tblMMDiscountsDESCRIPTION: TIBStringField
      FieldName = 'DESCRIPTION'
      Size = 128
    end
    object tblMMDiscountsMENU: TIBStringField
      FieldName = 'MENU'
      Size = 15
    end
    object tblMMDiscountsAMOUNT: TIBBCDField
      FieldName = 'AMOUNT'
      Precision = 18
      Size = 4
    end
    object tblMMDiscountsPERCENTAGE: TIBBCDField
      FieldName = 'PERCENTAGE'
      Precision = 18
      Size = 4
    end
  end
  object tblDiscounts: TIBTable
    Database = MMDatabase
    Transaction = MMTransaction
    BufferChunks = 1000
    CachedUpdates = False
    FieldDefs = <
      item
        Name = 'DISCOUNT_KEY'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NAME'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 128
      end
      item
        Name = 'MENU'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'AMOUNT'
        DataType = ftBCD
        Precision = 18
        Size = 4
      end
      item
        Name = 'PERCENTAGE'
        DataType = ftBCD
        Precision = 18
        Size = 4
      end>
    IndexDefs = <
      item
        Name = 'RDB$PRIMARY88'
        Fields = 'DISCOUNT_KEY'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'DISCOUNTS'
    Left = 32
    Top = 168
    object tblDiscountsDISCOUNT_KEY: TIntegerField
      FieldName = 'DISCOUNT_KEY'
      Required = True
    end
    object tblDiscountsNAME: TIBStringField
      FieldName = 'NAME'
      Size = 15
    end
    object tblDiscountsDESCRIPTION: TIBStringField
      FieldName = 'DESCRIPTION'
      Size = 128
    end
    object tblDiscountsMENU: TIBStringField
      FieldName = 'MENU'
      Size = 15
    end
    object tblDiscountsAMOUNT: TIBBCDField
      FieldName = 'AMOUNT'
      currency = True
      Precision = 18
      Size = 4
    end
    object tblDiscountsPERCENTAGE: TIBBCDField
      FieldName = 'PERCENTAGE'
      DisplayFormat = '0.00%'
      Precision = 18
      Size = 4
    end
  end
end
