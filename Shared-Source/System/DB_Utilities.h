//---------------------------------------------------------------------------
#ifndef DB_UtilitiesH
#define DB_UtilitiesH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>

namespace DB_Utilities
{
	bool StrToBoolean(UnicodeString s);
    bool FieldExists(TIBSQL *IBSQL, UnicodeString FieldName);
}
#endif
