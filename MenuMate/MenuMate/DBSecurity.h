//---------------------------------------------------------------------------

#ifndef DBSecurityH
#define DBSecurityH
//---------------------------------------------------------------------------
#include "ProcessSecurityQueries.h"
#include "ListSecurityRefContainer.h"
#include "MM_DBCore.h"

class TDBSecurity
{
	public:
   static int GetNextSecurityRef(Database::TDBTransaction &inDBTransaction);
	static void __fastcall ProcessSecurity(Database::TDBTransaction &DBTransaction,TListSecurityRefContainer *Security, bool isTransactionCommit=false);
	static void __fastcall ProcessSecurity( Database::TDBTransaction &DBTransaction,
												int SecRef,int UserKey,
												UnicodeString Event,UnicodeString FromVal,
												UnicodeString ToVal,TDateTime TimeStamp,
												UnicodeString TerminalName,UnicodeString Note = "");
    	static void __fastcall ProcessSecurityFill( Database::TDBTransaction &DBTransaction,
												int SecRef,int UserKey,
												UnicodeString Event,UnicodeString FromVal,
												UnicodeString ToVal,TDateTime TimeStamp,
												UnicodeString TerminalName,UnicodeString Note );
	static void __fastcall ProcessSecurity(TProcessSecurityQueries &Queries, TListSecurityRefContainer *Security);
	static void __fastcall ClearSecurity(Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security, TSecurityTypes Type);
};
#endif
