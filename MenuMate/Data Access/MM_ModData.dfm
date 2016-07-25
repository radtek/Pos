object modData: TmodData
  OldCreateOrder = False
  Height = 151
  Width = 493
  object SQLConnection: TSQLConnection
    ConnectionName = 'IBConnection'
    DriverName = 'Interbase'
    GetDriverFunc = 'getSQLDriverINTERBASE'
    LibraryName = 'dbexpint.dll'
    LoginPrompt = False
    Params.Strings = (
      'DriverName=Interbase'
      'Database=localhost:C:\\Program Files\\MenuMate\\MenuMate.gdb'
      'RoleName=RoleName'
      'User_Name=sysdba'
      'Password=masterkey'
      'ServerCharSet='
      'SQLDialect=3'
      'BlobSize=-1'
      'CommitRetain=False'
      'WaitOnLocks=True'
      'ErrorResourceFile='
      'LocaleCode=0000'
      'Interbase TransIsolation=ReadCommited')
    VendorLib = 'gds32.dll'
    Left = 32
    Top = 8
  end
  object SQLQueryA: TSQLQuery
    MaxBlobSize = -1
    ParamCheck = False
    Params = <>
    SQLConnection = SQLConnection
    Left = 104
    Top = 8
  end
  object SQLQueryB: TSQLQuery
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection
    Left = 176
    Top = 8
  end
  object SQLQueryC: TSQLQuery
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection
    Left = 248
    Top = 8
  end
  object SQLConnection1: TSQLConnection
    ConnectionName = 'IBConnection'
    DriverName = 'Interbase'
    GetDriverFunc = 'getSQLDriverINTERBASE'
    LibraryName = 'dbexpint.dll'
    LoginPrompt = False
    Params.Strings = (
      'DriverName=Interbase'
      'Database=localhost:C:\\Program Files\\MenuMate\\MenuMate.gdb'
      'RoleName=RoleName'
      'User_Name=sysdba'
      'Password=masterkey'
      'ServerCharSet='
      'SQLDialect=3'
      'BlobSize=-1'
      'CommitRetain=False'
      'WaitOnLocks=True'
      'ErrorResourceFile='
      'LocaleCode=0000'
      'Interbase TransIsolation=ReadCommited')
    VendorLib = 'gds32.dll'
    Left = 32
    Top = 64
  end
  object SQLQuery1: TSQLQuery
    MaxBlobSize = -1
    ParamCheck = False
    Params = <>
    SQLConnection = SQLConnection1
    Left = 104
    Top = 64
  end
  object SQLQuery2: TSQLQuery
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection1
    Left = 176
    Top = 64
  end
  object SQLQuery3: TSQLQuery
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection1
    Left = 248
    Top = 64
  end
  object IBRestore: TIBRestoreService
    Protocol = TCP
    TraceFlags = []
    PageBuffers = 0
    Left = 392
    Top = 64
  end
  object IBCopy: TIBBackupService
    Protocol = TCP
    TraceFlags = []
    BlockingFactor = 0
    Options = []
    Left = 328
    Top = 8
  end
  object SQLQuery4: TSQLQuery
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection1
    Left = 328
    Top = 64
  end
  object Zip: TZipBuilder
    AddStoreSuffixes = [assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ, assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR, assEXE, assJPG, assJPEG, ass7ZP, assMP3, assWMA, assWMV, assDVRMS, assAVI]
    SFXCaption = 'Self-extracting Archive'
    SFXLanguage = 'US'
    SFXOptions = [SFXCheckSize]
    Left = 392
    Top = 16
  end
end
