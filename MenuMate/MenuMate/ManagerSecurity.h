//---------------------------------------------------------------------------

#ifndef ManagerSecurityH
#define ManagerSecurityH
//---------------------------------------------------------------------------
#include "ProcessSecurityQueries.h"
#include "MM_DBCore.h"

class ManagerSecurity
{
	public:
	void __fastcall ProcessSecurity(Database::TDBTransaction &DBTransaction,TListSecurityRefContainer *Security);
	void __fastcall ProcessSecurity( Database::TDBTransaction &DBTransaction,
												int SecRef,int UserKey,
												UnicodeString Event,UnicodeString FromVal,
												UnicodeString ToVal,TDateTime TimeStamp,
												UnicodeString TerminalName,UnicodeString Note = "");

	void __fastcall ProcessSecurity(TProcessSecurityQueries &Queries, TListSecurityRefContainer *Security);

};
#endif
