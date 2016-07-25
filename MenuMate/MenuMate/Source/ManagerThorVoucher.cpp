//---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerThorVoucher.h"
#include "MMLogging.h"
#include <memory>
#include <StrUtils.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerThorVoucher *ManagerThorVoucher;

TManagerThorVoucher::TManagerThorVoucher()
{
	Enabled = false;
    isThorIndex = false;
}

TManagerThorVoucher::~TManagerThorVoucher()
{
}

void TManagerThorVoucher::Initialise()
{
	Enabled = true;
}

void __fastcall TManagerThorVoucher::SetEnabled(bool value)
{
	if (fEnabled != value)
	{
		fEnabled = value;
	}
}

bool __fastcall TManagerThorVoucher::GetEnabled()
{
	return fEnabled;
}

int TManagerThorVoucher::SetThorVoucherCategories(Database::TDBTransaction &DBTransaction)
{
    int ReturnVal = 0;
    try
    {
    UnicodeString Cat = "HOT BEVERAGES";
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT CATEGORY_KEY"
		" FROM"
		" ARCCATEGORIES"
		" WHERE"
		" UPPER(CATEGORY) = :CATEGORY";
		IBInternalQuery->ParamByName("CATEGORY")->AsString = Cat.UpperCase();
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
		{
         ReturnVal = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
        }

    }
    catch(Exception & E)
               {
                    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
                    throw;
                }
         return ReturnVal;
	}

std::set<int> TManagerThorVoucher::ThorVoucherKeys(Database::TDBTransaction &DBTransaction)

{
    std::set<int> ThorKeys;
try
    {

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();

		IBInternalQuery->SQL->Text =
        " SELECT THOR_VOUCHERS_KEY"
		" FROM"
		"  THORLINK_VOUCHERS"
		" WHERE"
		" VOUCHER_GROUP = :VOUCHER_GROUP";
        IBInternalQuery->ParamByName("VOUCHER_GROUP")->AsInteger = 1;
		for(IBInternalQuery->ExecQuery();!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			ThorKeys.insert(IBInternalQuery->FieldByName("THOR_VOUCHERS_KEY")->AsInteger);

		}

    }
    catch(Exception & E)
        {
         TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
         throw;
        }
     return ThorKeys;
}

void TManagerThorVoucher::UpdateThorVoucherCategories(Database::TDBTransaction &DBTransaction)
{
try
   {
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT *"
		" FROM"
		"  THORVOUCHERS_CATEGORIES";
        IBInternalQuery->ExecQuery();

    if (IBInternalQuery->RecordCount)
    {
       IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE THORVOUCHERS_CATEGORIES SET Is_THORVOUCHERS =:Is_THORVOUCHER ";
        IBInternalQuery->ParamByName("Is_THORVOUCHER")->AsString = "F";
        IBInternalQuery->ExecQuery();
    }

    }
    catch(Exception & E)
        {
         TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
         throw;
        }

}

void TManagerThorVoucher::InsertThorVoucherCategories(Database::TDBTransaction &DBTransaction,std::set<int> ThorKeys,int categoryKey)
{
try
   {
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    if(categoryKey!=0)
    {
    IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO THORVOUCHERS_CATEGORIES ("
		"THORVOUCHERS_CATEGORIES_KEY,"
        "Is_THORVOUCHERS,"
		"THOR_VOUCHERS_KEY,"
		"CATEGORY_KEY) "
		"VALUES ("
		"(SELECT GEN_ID(GEN_THORCATEGORIES_TYPE, 1) FROM RDB$DATABASE),"
        ":Is_THORVOUCHER,"
		":THOR_VOUCHERS_KEY,"
		":CATEGORY_KEY);";

        for (std::set<int>::iterator ptrCatKey = ThorKeys.begin();
		ptrCatKey != ThorKeys.end(); ptrCatKey++)
		{
			IBInternalQuery->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = *ptrCatKey;
            IBInternalQuery->ParamByName("Is_THORVOUCHER")->AsString = "T";
            IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = categoryKey;
			IBInternalQuery->ExecQuery();
		}

     }
    }
    catch(Exception & E)
        {
         TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
         throw;
        }


}
