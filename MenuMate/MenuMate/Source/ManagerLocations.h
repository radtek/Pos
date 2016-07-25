//---------------------------------------------------------------------------

#ifndef ManagerLocationsH
#define ManagerLocationsH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "MM_DBCore.h"


class TManagerLocations
{
	private :
	public:
	void SetLocation(Database::TDBTransaction &DBTransaction,int &LocationKey,UnicodeString Location, int Type = 2);
	UnicodeString GetLocation(Database::TDBTransaction &DBTransaction,int LocationKey);
	void GetLocationList(Database::TDBTransaction &DBTransaction,TStrings *Locations);
	void DeleteLocation(Database::TDBTransaction &DBTransaction,int LocationKey);
	int FindLocation(Database::TDBTransaction &DBTransaction,UnicodeString Location);
};
//---------------------------------------------------------------------------
#endif
