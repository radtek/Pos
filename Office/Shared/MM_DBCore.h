//---------------------------------------------------------------------------
#ifndef MM_DBCoreH
#define MM_DBCoreH
//---------------------------------------------------------------------------
#include <IBDatabase.hpp>
#include <IBDatabaseInfo.hpp>
#include <IBServices.hpp>
#include <IBSQL.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <set>

#ifndef NO_WIN32_LEAN_AND_MEAN
#error 'Conditional Defines' in 'Directories/Conditionals' under Project/Options must include NO_WIN32_LEAN_AND_MEAN.
#endif
//---------------------------------------------------------------------------
namespace Database
{
//---------------------------------------------------------------------------
typedef const std::auto_ptr<TIBTransaction>			TcpIBTransaction;
typedef const std::auto_ptr<TIBDatabaseInfo>			TcpIBDatabaseInfo;
typedef const std::auto_ptr<TIBBackupService>		TcpIBBackupService;
typedef const std::auto_ptr<TIBRestoreService>		TcpIBRestoreService;
typedef const std::auto_ptr<TIBValidationService>	TcpIBValidationService;
typedef const std::auto_ptr<TIBSQL>						TcpIBSQL;
//---------------------------------------------------------------------------
enum TDBType { dtInterbase, dtFirebird };
//---------------------------------------------------------------------------
struct TDBSettings
{
	TDBSettings() :
			Protocol(TCP),
			UserName("SYSDBA"),
			Password("masterkey")
	{}
	TDBSettings(
			AnsiString					iServer,
			AnsiString					iFileName,
			bool							UseFullConnectionString, // Entire string is in iFileName
			AnsiString					iUserName="SYSDBA",
			AnsiString					iPassword="masterkey",
			AnsiString					iCharacterSet="",
			Ibservices::TProtocol	iProtocol=TCP,
			TDBType						iDBType=dtInterbase,
			int							iPort=3050);

/*	TDBSettings(
			bool							UseFullConnectionString, // Dummy to force this constructor
			AnsiString					iConnectionString,
			AnsiString					iUserName="SYSDBA",
			AnsiString					iPassword="masterkey",
			AnsiString					iCharacterSet="",
			Ibservices::TProtocol	iProtocol=TCP,
			TDBType						iDBType=dtInterbase,
			int							iPort=3050);
*/
	AnsiString					Server;
	AnsiString					FileName;
	AnsiString					UserName;
	AnsiString					Password;
	AnsiString					CharacterSet;	// eg ISO8859_1
	Ibservices::TProtocol	Protocol;
	TDBType						DBType;
	int							Port;

	AnsiString					ConnectionString();	// Will throw and exception if it looks invalid.

//	void			Connection(AnsiString &Server, AnsiString &FilePath, AnsiString &FileName);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
enum TIBServiceType { stBackup, stRestore, stSweep, stRepairCheck, stRepair };
//---------------------------------------------------------------------------
class TDBControl
{
	friend class TDBUtilities;
public:
//---------------------------------------------------------------------------
	TDBControl(TIBDatabase *IBDatabase, TDBSettings const &DBSettings = TDBSettings());	// You're responsable for the TIBDatabase*
	TDBControl(TDBSettings const &DBSettings = TDBSettings());				// No TIBDatabase cleanup required.
	virtual ~TDBControl();	// No need to derive a class from this, but this could do with a pure virtual parent.

	inline operator			TIBDatabase*() { return fIBDatabase; }
									// returns the raw db. eg TIBDatabase*tmp = DBControl;
									// Use only if necessary! All functionality should be available internally.
	void							Init(TDBSettings const &DBSettings) { Disconnect(); fDBSettings = DBSettings; }
									// Set params. Use if settings not available at class creation time.
	void							Connect();
									// Disconnect[s]() first, sets params and attempts connection.
									// EIBError will be thrown as usual in the event of an error.
	void							Disconnect();
									// Attempts (Connected = false).
									// On exception, ForceClose(). Error re-thrown.
	inline bool					Connected()	{ return fIBDatabase->Connected;	}

	bool							SetIBAffinity();
									// Sets processor mask to 1 on hyperthread/multi cpu systems. Speeds up interbase.
	bool							WaitForIBServer(unsigned int Timeout);//
									// Waits for IBServer process to start. Timeout in ms. Timeout==0 will return immediately.

	AnsiString					ConnectionString();

	AnsiString					ConnectionServer();
	AnsiString					ConnectionPath();
	AnsiString					ConnectionFile();
private:
	TIBDatabase					*fIBDatabase;
	bool							fOwnDB;
	TDBSettings					fDBSettings;

	HANDLE						InitIBAffinity();
	bool							GetIBAffinity(int &Mask);
	bool							SetIBAffinity(int Mask);
	bool							EnablePrivilege(HANDLE h, AnsiString AName, bool AState);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
typedef int TQueryHandle;
//---------------------------------------------------------------------------
class TDBTransaction
{
public:
	TDBTransaction(TIBDatabase *IBDatabase);
	TDBTransaction(TDBControl &DBControl);
	virtual						~TDBTransaction();

	int							AddDatabase(TIBDatabase *IBDatabase);	// For multi-phase commits.
	int							AddDatabase(TDBControl &DBControl);

	TQueryHandle				AddQuery(int DatabaseIndex=0);
	TIBSQL *						Query(TQueryHandle Index);

	bool							Start();			//	Will throw an exception is unsuccessful. Otherwise always returns true.
	bool							Commit();		//	Will throw an exception is unsuccessful. Returns false if not in transaction.
	bool							Undo();			// Attempts a rollback. Returns false if not in transaction.
	// For backwards compatibility
	bool							StartTransaction()		{ return Start();		}
	bool							CommitTransaction()		{ return Commit();	}
	bool							UndoTransaction()			{ return Undo();		}
	bool							Rollback()					{ return Undo();		}

	int							GetGeneratorVal(AnsiString GeneratorName, int Increment=1);

	void							RegisterQuery(TcpIBSQL &Query, int DatabaseIndex=0);	// Left for backwards compatibility
private:
	void							fCreate(TIBDatabase *IBDatabase);
	int							fAddDatabase(TIBDatabase *IBDatabase);

	TcpIBTransaction			fIBTransaction;
	TcpIBSQL						sqlGetGeneratorVal;

	std::vector<TIBDatabase *>	fIBDatabases;
//	TIBDatabase					*fIBDatabase;
	std::vector<TIBSQL *>		fQueries;
};
//---------------------------------------------------------------------------
/*enum TIBFieldType
{
	ftVarying		= SQL_VARYING,		// 448
	ftText			= SQL_TEXT,			// 452
	ftDouble			= SQL_DOUBLE,		// 480
	ftFloat			= SQL_FLOAT,		// 482
	ftLong			= SQL_LONG,			// 496
	ftShort			= SQL_SHORT,		// 500
	ftTimeStamp		= SQL_TIMESTAMP,	// 510
	ftBlob			= SQL_BLOB,			// 520
	ftDFloat			= SQL_D_FLOAT,		// 530
	ftArray			= SQL_ARRAY,		// 540
	ftQuad			= SQL_QUAD,			// 550
	ftTypeTime		= SQL_TYPE_TIME,	// 560
	ftTypeDate		= SQL_TYPE_DATE,	// 570
	ftInt64			= SQL_INT64,		// 580
	ftDate			= SQL_DATE			// SQL_TIMESTAMP
};*/
enum TIBFieldType
{
	ftBlob			= 261,
	ftChar			= 14,
	ftCString		= 40,
	ftDFloat			= 11,
	ftDouble			= 27,
	ftFloat			= 10,
	ftInt64			= 16,
	ftInteger		= 8,
	ftQuad			= 9,
	ftSmallInt		= 7,
	ftDate			= 12,
	ftTime			= 13,
	ftTimeStamp		= 35,
	ftVarChar		= 37
};
struct TFieldDefinition
{
	TIBFieldType		FieldType;
//	TIBSubFieldType	SubFieldType;	// For blobs
	int					Scale;
	int					Length;
	int					Precision;
};
struct TIndexDefinition
{
	AnsiString	TableName;
	bool			Unique;
	AnsiString	ForeignKey;
	AnsiString	ForeignTable;
};
struct TConstraintDefinition
{
	AnsiString	TableName;
	AnsiString	ConstraintType;
	AnsiString	FieldName;
	AnsiString	IndexName;
	AnsiString	ForeignTable;
};
typedef std::vector<AnsiString> TTableNames;
typedef std::vector<AnsiString> TFieldNames;
typedef std::set<AnsiString> TIndexNames;
typedef std::set<AnsiString> TIndexFields;
typedef std::set<AnsiString> TConstraintNames;
//---------------------------------------------------------------------------
class TDBUtilities
{
class TServiceThread;
public:
//---------------------------------------------------------------------------
	TDBUtilities(TDBControl &DBControl);
	virtual ~TDBUtilities();

	// Create empty db as per params. Doesn't Connect.
	bool							CreateDatabase(int PageSize = 4096);
	void							DropDatabase();
	// SetAsyncMode(false). Will connect first if not already.
	// Leaves Disconnected
	bool							SetForcedWrites();
	inline int					UserCount() const	{ return fDatabaseInfo->UserNames->Count; }

	void							GetTableNames(TTableNames &TableNames);
	void							GetFieldNames(AnsiString TableName, TFieldNames &FieldNames);
	void							GetFieldDefinition(AnsiString TableName, AnsiString FieldName, TFieldDefinition &FieldDefinition);
	void							GetIndexNames(AnsiString TableName, TIndexNames &IndexNames);
	void							GetIndexDefinition(AnsiString IndexName, TIndexDefinition &IndexDefinition);
	void							GetIndexFields(AnsiString IndexName, TIndexFields &IndexFields);
	void							GetConstraintNames(AnsiString TableName, TConstraintNames &ConstraintNames);
	void							GetConstraintDefinition(AnsiString IndexName, TConstraintDefinition &ConstraintDefinition);

//---------------------------------------------------------------------------
	// These functions return immediately.
	// Make Sure you set OnServiceComplete and OnServiceStatus to get service info.
	bool							StartSweep();
	bool							StartRepairCheck();
	bool							StartRepair();
	bool							StartRepair(AnsiString ServerName, AnsiString FileName); // Make sure you're not connected to this!
	// Pass it the server name, source file (including path and name), dest file (including path and name)
	bool							StartBackup(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose = true,
											TBackupOptions const &BackupOptions =
											TBackupOptions() << NoGarbageCollection << IgnoreLimbo);
	// Pass it dest file name and dest file path. Source file and server name are those used by fDBControl's TDBSettings.
	bool							StartBackup(AnsiString DestFilePath, AnsiString DestFileName, bool Verbose = true,
											TBackupOptions const &BackupOptions =
											TBackupOptions() << NoGarbageCollection << IgnoreLimbo);
	// Pass it dest file name. Source file and server name are those used by fDBControl's TDBSettings. Dest file path is same as source.
	bool							StartBackup(AnsiString DestFileName, bool Verbose = true,
											TBackupOptions const &BackupOptions =
											TBackupOptions() << NoGarbageCollection << IgnoreLimbo);

	bool							StartRestore(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose = true,
											bool ReplaceDB=false);

	bool							StartRestore(AnsiString ServerName, AnsiString SourceFile, AnsiString DestFile, bool Verbose = true,
											TRestoreOptions const &RestoreOptions = TRestoreOptions() << CreateNewDB);

	typedef void __fastcall (__closure *TServiceCompleteEvent)(bool Success, AnsiString ErrorMsg, TIBServiceType ServiceType);
	typedef void __fastcall (__closure *TServiceGetStatus)(AnsiString Msg, TIBServiceType ServiceType);
	// Set these for use with threaded service functions.
	TServiceCompleteEvent	OnServiceComplete;
									// void __fastcall TForm1::ServiceComplete(bool Success, AnsiString ErrorMsg, Database::TIBServiceType ServiceType)
	TServiceGetStatus			OnServiceStatus;
									// void __fastcall TForm1::ServiceStatus(AnsiString Msg, Database::TIBServiceType ServiceType)
	void							CancelService();
	bool							ServiceBusy();
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
private:
//---------------------------------------------------------------------------
	TDBControl					&fDBControl;
	TDBTransaction				fDBTransaction;

	void							StartServiceThread(TIBControlAndQueryService *IBService, TIBServiceType ServiceType);
	bool							fUsingServiceThread;

	TcpIBDatabaseInfo			fDatabaseInfo;
	TcpIBBackupService		fIBBackupService;
	TcpIBRestoreService		fIBRestoreService;
	TcpIBValidationService	fIBValidationService;

	TcpIBSQL						sqlIndexList;
	TcpIBSQL						sqlIndexFields;
	TcpIBSQL						sqlIndexType;
	TcpIBSQL						sqlFieldType;
	TcpIBSQL						sqlConstraintList;
	TcpIBSQL						sqlConstraintType;

//---------------------------------------------------------------------------
// Service thread class. Controls sweeps and backups.
//---------------------------------------------------------------------------
	class TServiceThread : public TThread
	{
	public:
		__fastcall						TServiceThread(TIBControlAndQueryService	*IBService,
													TServiceGetStatus OnGetStatus,
													TServiceCompleteEvent OnServiceComplete,
													TIBServiceType ServiceType);

		void								Cancel();	// Call if terminating the application while running!

//		inline bool						ServiceSuccess()	{ return fServiceSuccess; }
//		inline AnsiString				ErrorMsg()			{ return fErrorMsg; }
//		inline TIBServiceType		ServiceType()		{ return fServiceType; }

	private:
		void __fastcall				Execute();

		void __fastcall				SetStatusText();
		void __fastcall				ServiceComplete();
//		void __fastcall				GetComplete();

		TIBControlAndQueryService	*fIBService;
		TIBServiceType					fServiceType;
		TServiceGetStatus				fOnServiceStatus;
		TServiceCompleteEvent		fOnServiceComplete;

		bool								fServiceSuccess;
//		bool								fComplete;
		AnsiString						fErrorMsg;
		AnsiString						fStatusText;
	};
//---------------------------------------------------------------------------
	TServiceThread		*fServiceThread;
	void __fastcall	fServiceTerminate(TObject *Sender);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
} // namespace Database
//---------------------------------------------------------------------------
#endif
/*
To extract non-system tables (and no views):
SELECT RDB$RELATION_NAME
FROM RDB$RELATIONS
WHERE ((RDB$SYSTEM_FLAG = 0) OR
(RDB$SYSTEM_FLAG IS NULL)) AND
(RDB$VIEW_SOURCE IS NULL)
ORDER BY RDB$RELATION_NAME

To extract system tables (no user defined tables):
SELECT RDB$RELATION_NAME
FROM RDB$RELATIONS
WHERE (RDB$SYSTEM_FLAG = 1) AND
(RDB$VIEW_SOURCE IS NULL)
ORDER BY RDB$RELATION_NAME

To extract all of the above information:
SELECT RDB$RELATION_NAME
FROM RDB$RELATIONS
WHERE (RDB$VIEW_SOURCE IS NULL)
ORDER BY RDB$RELATION_NAME

More examples:

Objects:

* Tables/Views format
* Triggers on a table
* primary/foreign keys and table constraints
* domains
* Indices
* Procedures
* UDFs
* Generators

Retrieving Information about a Table or View
This example uses the employee table in the employee database in the
examples
directory. This select will return the field name, field type, field
length,
whether it is nullable, and the column check constraints for a table or
view.

select r.rdb$field_name,
t.rdb$type_name,
f.rdb$field_length,
r.rdb$null_flag,
f.rdb$validation_source
from rdb$relation_fields r, rdb$types t, rdb$fields f
where r.rdb$relation_name='EMPLOYEE' and
f.rdb$field_name=r.rdb$field_source and
t.rdb$field_name='RDB$FIELD_TYPE' and
f.rdb$field_type=t.rdb$type;

This select returns the source for a view.

select rdb$view_source from rdb$relations
where rdb$view_source='PHONE_LIST';

This select returns the primary and foreign keys for a table and the
fields
it
is defined on.

select r.rdb$constraint_type,
i.rdb$field_name
from rdb$relation_constraints r, rdb$index_segments i
where r.rdb$relation_name='EMPLOYEE' and
(r.rdb$constraint_type='PRIMARY KEY'
or r.rdb$constraint_type='FOREIGN KEY') and
r.rdb$index_name=i.rdb$index_name;

This select returns the check constraints on a table.

select r.rdb$constraint_name,
r.rdb$constraint_type,
t.rdb$trigger_source
from rdb$relation_constraints r, rdb$check_constraints c, rdb$triggers t
where r.rdb$constraint_name=c.rdb$constraint_name and
c.rdb$trigger_name=t.rdb$trigger_name and
r.rdb$relation_name='EMPLOYEE';

This select returns all the triggers for a given table.

select t.rdb$trigger_name,
t.rdb$trigger_sequence,
y.rdb$type_name,
t.rdb$trigger_inactive
from rdb$triggers t, rdb$types y
where t.rdb$relation_name='EMPLOYEE' and
t.rdb$trigger_name not like 'CHECK%' and
y.rdb$field_name='RDB$TRIGGER_TYPE' and
t.rdb$trigger_type=y.rdb$type;

Retrieving Information on a Domain
This select returns the name, datatype and length, nullable, and check
constraints on a domain.

select f.rdb$field_name,
t.rdb$type_name,
f.rdb$field_length,
f.rdb$null_flag,
f.rdb$default_source,
f.rdb$validation_source
from rdb$fields f, rdb$types t
where f.rdb$field_name='JOBCODE' and
f.rdb$field_type=t.rdb$type and
t.rdb$field_name='RDB$FIELD_TYPE'

Retrieving Information on a Database's Indices
This select returns the indices defined for a database.

select i.rdb$index_name,
i.rdb$unique_flag,
i.rdb$relation_name,
s.rdb$field_name
from rdb$indices i, rdb$index_segments s
where i.rdb$index_name=s.rdb$index_name and
i.rdb$index_name not like 'RDB$%';
(exclude this from where clause if you want system indices)

Retrieving Information on a Procedure
This select returns the source for a procedure.

select rdb$procedure_source
from rdb$procedures
where rdb$procedure_name = 'ADD_EMP_PROJ';

This select returns the parameters' name, datatype, datatype length, and
whether it is an input or output parameter.

select p.rdb$parameter_name,
p.rdb$parameter_type,
t.rdb$type_name,
f.rdb$field_length
from rdb$procedure_parameters p, rdb$fields f, rdb$types t
where p.rdb$field_source=f.rdb$field_name and
p.rdb$procedure_name='ADD_EMP_PROJ' and
f.rdb$field_type=t.rdb$type and
t.rdb$field_name='RDB$FIELD_TYPE';

Retrieving Information on User Defined Functions
This select returns a functions name, module name, and entry point.

select f.rdb$function_name,
f.rdb$module_name,
f.rdb$entrypoint
from rdb$functions f
where rdb$function_name='UPPER';

This select returns the returning value of a function.

select a.rdb$mechanism,
t.rdb$type_name,
a.rdb$field_length
from rdb$function_arguments a, rdb$functions f, rdb$types t
where f.rdb$function_name=a.rdb$function_name and
t.rdb$field_name='RDB$FIELD_TYPE' and
t.rdb$type=a.rdb$field_type and
f.rdb$function_name='UPPER' and
a.rdb$argument_position=f.rdb$return_argument;

This select returns the parameters of a function.

select a.rdb$argument_position,
a.rdb$mechanism,
t.rdb$type_name,
a.rdb$field_length
from rdb$function_arguments a, rdb$functions f, rdb$types t
where f.rdb$function_name=a.rdb$function_name and
t.rdb$field_name='RDB$FIELD_TYPE' and
t.rdb$type=a.rdb$field_type and
f.rdb$function_name='UPPER' and
a.rdb$argument_position <> f.rdb$return_argument;

Retrieving Information about Exceptions
This select returns the exception name, exception message, and the name
and
type of object that uses the exception.

select e.rdb$exception_name,
e.rdb$message,
d.rdb$dependent_name,
t.rdb$type_name
from rdb$exceptions e, rdb$dependencies d, rdb$types t
where e.rdb$exception_name=d.rdb$depended_on_name and
d.rdb$dependent_type=t.rdb$type and
rdb$field_name='RDB$OBJECT_TYPE';

Retrieving Information about Generators
This select shows the database's generators.

select rdb$generator_name
from rdb$generators
where rdb$system_flag is null;

*/
