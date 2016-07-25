//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DB_Utilities.h"
#include <memory>



//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace DB_Utilities
{

//---------------------------------------------------------------------------
bool StrToBoolean(UnicodeString s)
{
	s = UpperCase(s);
	if (s == "T" || s == "1"  || s == "Y" || s == "TRUE")
		return true;
	else
		return false;
}

bool FieldExists(TIBSQL *IBSQL, UnicodeString FieldName)
{
	bool RetVal = false;
	for(int i = 0; i < IBSQL->FieldCount; i++)
   	{
    	if(IBSQL->Fields[i]->Name == FieldName)
        {
           RetVal = true;
        }
   	}
    return RetVal;
}

}