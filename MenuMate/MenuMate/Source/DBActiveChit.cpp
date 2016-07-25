//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBActiveChit.h"
#include "MMLogging.h"
#include "ManagerChitNumber.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TDBActiveChit::TDBActiveChit()
{
}

TDBActiveChit::~TDBActiveChit()
{
}

int TDBActiveChit::GetOrCreateActiveChit(Database::TDBTransaction &DBTransaction, TChitNumber ActiveChitNumber)
{
	int ActiveChitKey = -1;
	try
	{
		UnicodeString ActiveChitValue = ActiveChitNumber.Format == ectList ? ActiveChitNumber.ActiveOption() : ActiveChitNumber.ChitNumber;

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		//Check if there is already a record of this chit
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"ac.ACTIVECHITNUMBER_KEY "
			"FROM "
				"ACTIVECHITNUMBER ac "
			"WHERE "
				"ac.CHITNUMBER_KEY = :CHITNUMBER_KEY "
			"AND "
				"ac.CHIT_VALUE = :CHIT_VALUE";
		IBInternalQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = ActiveChitNumber.ChitNumberKey;
		IBInternalQuery->ParamByName("CHIT_VALUE"	 )->AsString  = ActiveChitValue;
		IBInternalQuery->ExecQuery();

		//If there is a record of the chit simply return it's key
		//Otherwise create a new record and return the key
		if(IBInternalQuery->RecordCount > 0)
		{
			ActiveChitKey = IBInternalQuery->Fields[0]->AsInteger;
		}
		else
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ACTIVECHIT, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			ActiveChitKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO ACTIVECHITNUMBER"
				"("
				"ACTIVECHITNUMBER_KEY,"
				"CHITNUMBER_KEY,"
				"CHIT_VALUE"
				")"
				" VALUES "
				"("
				":ACTIVECHITNUMBER_KEY,"
				":CHITNUMBER_KEY,"
				":CHIT_VALUE"
				")";
			IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY"	)->AsInteger = ActiveChitKey;
			IBInternalQuery->ParamByName("CHITNUMBER_KEY"       )->AsInteger = ActiveChitNumber.ChitNumberKey;
			IBInternalQuery->ParamByName("CHIT_VALUE")->AsString = ActiveChitValue;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Error storing chit number: " + ActiveChitNumber.GetChitNumber());

		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return ActiveChitKey;
}

TChitNumber TDBActiveChit::LoadChitNumberFromKey(Database::TDBTransaction &DBTransaction, int ActiveChitNumberKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"CHITNUMBER_KEY,"
				"CHIT_VALUE "
			"FROM "
				"ACTIVECHITNUMBER "
			"WHERE "
				"ACTIVECHITNUMBER_KEY = :ACTIVECHITNUMBER_KEY";
		IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY"	)->AsInteger = ActiveChitNumberKey;
		IBInternalQuery->ExecQuery();

		UnicodeString ActiveChitValue = IBInternalQuery->FieldByName("CHIT_VALUE")->AsString;
		TChitNumber ChitNumber = TManagerChitNumber::Instance().LoadFromKey(DBTransaction,IBInternalQuery->FieldByName("CHITNUMBER_KEY")->AsInteger);
		if (ChitNumber.Format == ectList)
		{
			ChitNumber.ActiveOption(ActiveChitValue);
		}
		else
		{
			ChitNumber.ChitNumber = ActiveChitValue;
        }
        return ChitNumber;
	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBActiveChit::DeleteActiveChit(Database::TDBTransaction &DBTransaction, int ActiveChitKey)
{
	bool removed = false;
	if(!IsActiveChitInUse(DBTransaction, ActiveChitKey))
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Text =
			"DELETE FROM "
					"ACTIVECHITNUMBER "
			"WHERE "
				"ACTIVECHITNUMBER_KEY = :ACTIVE_CHIT_KEY";
		IBInternalQuery->ParamByName("ACTIVE_CHIT_KEY")->AsInteger = ActiveChitKey;

		removed = (IBInternalQuery->ExecQuery(), IBInternalQuery->RowsAffected);

		IBInternalQuery->Close();
	}
    return removed;
}

bool TDBActiveChit::IsActiveChitInUse(Database::TDBTransaction &DBTransaction, int ActiveChitKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	bool inUse = false;

	IBInternalQuery->SQL->Text =
		"SELECT "
			"ACTIVECHITNUMBER_KEY "
		"FROM "
			"ORDERS "
		"WHERE "
			"ACTIVECHITNUMBER_KEY = :ACTIVE_CHIT_KEY";
	IBInternalQuery->ParamByName("ACTIVE_CHIT_KEY")->AsInteger = ActiveChitKey;
	IBInternalQuery->ExecQuery();

	inUse = !IBInternalQuery->Eof;

	if(!inUse)
	{
		IBInternalQuery->Close();
    	IBInternalQuery->SQL->Text =
			"SELECT "
				"ACTIVECHITNUMBER_KEY "
			"FROM "
				"HELDORDERS "
			"WHERE "
				"ACTIVECHITNUMBER_KEY = :ACTIVE_CHIT_KEY";
		IBInternalQuery->ParamByName("ACTIVE_CHIT_KEY")->AsInteger = ActiveChitKey;
		IBInternalQuery->ExecQuery();

		inUse = !IBInternalQuery->Eof;
	}

	IBInternalQuery->Close();
    return inUse;
}
