//---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerLocations.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

void TManagerLocations::SetLocation(Database::TDBTransaction &DBTransaction,int &LocationKey,UnicodeString Location, int Type)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(LocationKey == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOCATION, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		LocationKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"insert into \"LOCATIONS\" "
		"  (\"LOCATIONS\".\"LOCATION_KEY\", \"LOCATIONS\".\"NAME\", \"LOCATIONS\".\"LOCATION_TYPE\") "
		" values "
		" (:\"LOCATION_KEY\", :\"NAME\", :\"LOCATION_TYPE\") ";
		IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
		IBInternalQuery->ParamByName("NAME")->AsString = Location;
		IBInternalQuery->ParamByName("LOCATION_TYPE")->AsInteger = Type;
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->SQL->Text =
		" update \"LOCATIONS\" "
		" set "
		 " \"LOCATIONS\".\"NAME\" = :\"NAME\", "
		 " \"LOCATIONS\".\"LOCATION_TYPE\" = :\"LOCATION_TYPE\" "
		"where "
		"  \"LOCATIONS\".\"LOCATION_KEY\" = :\"LOCATION_KEY\" ";
		IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
		IBInternalQuery->ParamByName("NAME")->AsString = Location;
		IBInternalQuery->ParamByName("LOCATION_TYPE")->AsInteger = Type;
		IBInternalQuery->ExecQuery();
	}
}

void TManagerLocations::GetLocationList(Database::TDBTransaction &DBTransaction,TStrings *Locations)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
	"select * from LOCATIONS where LOCATION_TYPE = 0 or LOCATION_TYPE = 2";
	IBInternalQuery->ExecQuery();

	for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
	{
		Locations->AddObject(UnicodeString(IBInternalQuery->FieldByName("NAME")->AsString),
								  (TObject *)IBInternalQuery->FieldByName("LOCATION_KEY")->AsInteger);


	}
}

void TManagerLocations::DeleteLocation(Database::TDBTransaction &DBTransaction,int LocationKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		" delete from \"LOCATIONS\" "
		 " where "
		 "  \"LOCATIONS\".\"LOCATION_KEY\" = :\"LOCATION_KEY\" ";
	IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
	IBInternalQuery->ExecQuery();
}

UnicodeString TManagerLocations::GetLocation(Database::TDBTransaction &DBTransaction,int LocationKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
	"select * from LOCATIONS where LOCATION_KEY = :LOCATION_KEY";
	IBInternalQuery->ParamByName("LOCATION_KEY")->AsInteger = LocationKey;
	IBInternalQuery->ExecQuery();

	return IBInternalQuery->FieldByName("NAME")->AsString;
}

int TManagerLocations::FindLocation(Database::TDBTransaction &DBTransaction,UnicodeString Location)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
	"select * from LOCATIONS where UPPER(NAME) = :NAME";
	IBInternalQuery->ParamByName("NAME")->AsString = Location.UpperCase();
	IBInternalQuery->ExecQuery();

	return IBInternalQuery->FieldByName("LOCATION_KEY")->AsInteger;
}