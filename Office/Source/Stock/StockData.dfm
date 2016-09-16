object dmStockData: TdmStockData
  OldCreateOrder = False
  Top = 33
  Height = 372
  Width = 640
  object DataSource1: TDataSource
    AutoEdit = False
    DataSet = IBQuery1
    Left = 96
    Top = 152
  end
  object dbStock: TIBDatabase
    DatabaseName = 'E:\MENUMATE.FDB'
    Params.Strings = (
      'user_name=SYSDBA'
      'password=masterkey'
      'lc_ctype=ISO8859_1')
    LoginPrompt = False
    DefaultTransaction = trStock
    TraceFlags = [tfQPrepare, tfQExecute, tfQFetch, tfError, tfStmt, tfConnect, tfTransact, tfBlob, tfService, tfMisc]
    AllowStreamedConnected = False
    Left = 96
    Top = 8
  end
  object trStock: TIBTransaction
    DefaultDatabase = dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 96
    Top = 56
  end
  object IBQuery1: TIBQuery
    Database = dbStock
    Transaction = trStock
    Left = 96
    Top = 104
  end
  object IBQuery2: TIBQuery
    Database = dbStock
    Transaction = trStock
    Left = 152
    Top = 104
  end
  object IBBackupService1: TIBBackupService
    Protocol = TCP
    Params.Strings = (
      'user_name=SYSDBA'
      'password=masterkey')
    LoginPrompt = False
    TraceFlags = []
    BackupFile.Strings = (
      'c:\Stock.gbk')
    BlockingFactor = 0
    DatabaseName = 'c:\Stock.gdb'
    Options = [IgnoreChecksums, IgnoreLimbo]
    Left = 168
    Top = 8
  end
  object dbinfStock: TIBDatabaseInfo
    Database = dbStock
    Left = 32
    Top = 8
  end
  object Query: TIBSQL
    Database = dbStock
    Transaction = trStock
    Left = 248
    Top = 8
  end
  object sqlNow: TIBSQL
    Database = dbStock
    SQL.Strings = (
      'SELECT CURRENT_TIMESTAMP FROM RDB$DATABASE')
    Transaction = trTime
    Left = 248
    Top = 104
  end
  object trTime: TIBTransaction
    DefaultDatabase = dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 304
    Top = 104
  end
  object ValidationService: TIBValidationService
    Protocol = TCP
    Params.Strings = (
      'user_name=SYSDBA'
      'password=masterkey')
    LoginPrompt = False
    TraceFlags = []
    Options = [SweepDB]
    GlobalAction = CommitGlobal
    Left = 32
    Top = 56
  end
  object IBRestoreService1: TIBRestoreService
    Protocol = TCP
    Params.Strings = (
      'user_name=SYSDBA'
      'password=masterkey')
    LoginPrompt = False
    TraceFlags = []
    PageBuffers = 0
    Left = 168
    Top = 56
  end
end
