//---------------------------------------------------------------------------

#pragma hdrstop

//---------------------------------------------------------------------------

#ifdef PalmMate
#include "Palm.h"
#endif

#include "DBTab.h"
#include "MMLogging.h"
#include "Invoice.h"
#include "TableManager.h"
#include "ManagerPatron.h"
#include <Memory>
#include "DeviceRealTerminal.h"



#pragma package(smart_init)


TDBTab::TDBTab()
{ }

TDBTab::~TDBTab()
{ }

int TDBTab::GetOrCreateTab(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	int RetVal = 0;
	bool CreateTab = true;

	if(inTabKey != 0 )
	{
		if(GetTabExists(DBTransaction,inTabKey))
		{
			RetVal = inTabKey;
			CreateTab = false;
		}
	}

	if(CreateTab)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TAB, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "INSERT INTO TAB ("
                                        "TAB_KEY,"
                                        "CREDIT_LIMIT,"
                                        "TAB_PERMANENT,"
                                        "TAB_TYPE) "
                                        "VALUES ("
                                        ":TAB_KEY,"
                                        ":CREDIT_LIMIT,"
                                        ":TAB_PERMANENT,"
                                        ":TAB_TYPE);";

		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = RetVal;
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabNormal;
		IBInternalQuery->ParamByName("TAB_PERMANENT")->AsString = "F";
		IBInternalQuery->ParamByName("CREDIT_LIMIT")->AsInteger = -1;

		IBInternalQuery->ExecQuery();
	}
	return RetVal;
}

bool TDBTab::DeleteTab(Database::TDBTransaction &DBTransaction, long inTabKey)
{
	bool RetVal = false;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM TAB WHERE TAB_KEY = :TAB_KEY AND TAB_TYPE != 13";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();

		DeletePaymentType(DBTransaction,inTabKey);

		if(IBInternalQuery->RowsAffected <= 0)
		{
			RetVal = false;
		}
		else
		{
			RetVal = true;
		}

		// Fixme This Message needs to go out after the transaction has been committed.
		/*		if (IBTransaction->InTransaction && StartTrans)
		{
			IBTransaction->Commit();

			TNetMessageTabChanged *Request = new TNetMessageTabChanged;
			ManagerNet->SendToAll(Request);
			delete Request;
		}*/

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

bool TDBTab::DeleteTabType(Database::TDBTransaction &DBTransaction,eTabType TabType)
{
	bool RetVal = false;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM TAB WHERE TAB_TYPE = :TAB_TYPE";
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabType;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected <= 0)
		{
			RetVal = false;
		}
		else
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}


bool TDBTab::CloseTab(Database::TDBTransaction &DBTransaction,long inTabKey, bool isNewTabAccess)
{
	bool RetVal = false;
	if(GetTabRemovable(DBTransaction,inTabKey))
	{
        if(!isNewTabAccess)
        {
            DeleteTab(DBTransaction,inTabKey);
            RetVal = true;
        }
	}
	return RetVal;
}

void TDBTab::SetTabType(Database::TDBTransaction &DBTransaction,int inTabKey, TMMTabType inType)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"TAB "
		"SET "
		"TAB_TYPE = :TAB_TYPE "
		"WHERE "
		"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = inType;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTab::SetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey, bool Permanent)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"TAB_PERMANENT = :TAB_PERMANENT "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ParamByName("TAB_PERMANENT")->AsString = Permanent ? "T":"F";
	IBInternalQuery->ExecQuery();
}

bool TDBTab::GetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	bool PermanentTab = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(inTabKey != 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" SELECT "
			" TAB_PERMANENT "
			" FROM "
			"  TAB "
			" WHERE "
			"  TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
			IBInternalQuery->ExecQuery();
			if(IBInternalQuery->RecordCount)
			{
				PermanentTab = IBInternalQuery->FieldByName("TAB_PERMANENT")->AsString.UpperCase() == "F" ? false : true;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return PermanentTab;
}

void TDBTab::SetTabCredit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"TAB "
		"SET "
		"CREDIT = :CREDIT "
		"WHERE "
		"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("CREDIT")->AsCurrency = Amount;
		IBInternalQuery->ExecQuery();

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

Currency TDBTab::GetTabBalance(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency Balance = 0;
	try
	{
		Balance = -GetTabCredit(DBTransaction,TabKey);
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM((PRICE * QTY) + DISCOUNT + REDEEMED) FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		Balance += IBInternalQuery->Fields[0]->AsCurrency;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Balance;
}

int TDBTab::GetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	int Divisions = 0;

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT DIVISIONS_LEFT FROM TAB WHERE TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	Divisions = IBInternalQuery->Fields[0]->AsInteger;
	return Divisions;
}

int TDBTab::GetTotalDivisions(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	int Divisions = 0;

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT TOTAL_DIVISIONS FROM TAB WHERE TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	Divisions = IBInternalQuery->Fields[0]->AsInteger;
	return Divisions;
}

void TDBTab::SetTotalDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int TotalDivisions)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE TAB SET TOTAL_DIVISIONS = :TOTAL_DIVISIONS WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ParamByName("TOTAL_DIVISIONS")->AsInteger = TotalDivisions;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}


void TDBTab::SetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int DivisionsLeft)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE TAB SET DIVISIONS_LEFT = :DIVISIONS_LEFT WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ParamByName("DIVISIONS_LEFT")->AsInteger = DivisionsLeft;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

Currency TDBTab::GetTabCredit(Database::TDBTransaction &DBTransaction,long TabKey)
{
	Currency CreditTotal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CREDIT FROM TAB WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			CreditTotal = IBInternalQuery->FieldByName("CREDIT")->AsCurrency;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CreditTotal;
}

Currency TDBTab::GetTabsCredit(Database::TDBTransaction &DBTransaction,std::set<__int64> &TabKeys)
{
	Currency CreditTotal = 0;
	try
	{
		if(!TabKeys.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			UnicodeString SQL			= "SELECT SUM(CREDIT) FROM TAB WHERE TAB_KEY IN (";
			bool First				= true;
			for(std::set<__int64>::iterator CrntTabKey = TabKeys.begin(); CrntTabKey != TabKeys.end(); advance(CrntTabKey,1))
			{
				if (First)
				{
					SQL += IntToStr((int)*CrntTabKey);
					First = false;
				}
				else
				{
					SQL += "," + IntToStr((int)*CrntTabKey);
				}
			}
			IBInternalQuery->SQL->Text = SQL + ")";
			IBInternalQuery->ExecQuery();
			CreditTotal = IBInternalQuery->Fields[0]->AsCurrency;
			IBInternalQuery->Close();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CreditTotal;
}

TMMTabType TDBTab::GetTabType(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTabSelected)
{
	TMMTabType Type = TabNormal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        if(!isTabSelected)
        {
          // when is tabselected is false then orderkey is sent to funtion .fetch tab key from orders table from that order key
          	IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT TAB_KEY FROM ORDERS a WHERE a.ORDER_KEY = :ORDER_KEY ";
            IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = inTabKey;
            IBInternalQuery->ExecQuery();
            if (IBInternalQuery->RecordCount)
            {
                inTabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
            }
        }
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT TAB_TYPE FROM TAB WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			Type = (TMMTabType)IBInternalQuery->FieldByName("TAB_TYPE")->AsInteger;
		}

         if(Type != TabClipp)
           {

              Type = GetLinkedTableAndClipTab(DBTransaction,inTabKey,true,Type);
           }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Type;
}


Currency TDBTab::GetTabTotalExcl(Database::TDBTransaction &DBTransaction,long TabKey)
{
	Currency Total = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT SUM((ORDERS.PRICE + ORDERS.DISCOUNT + REDEEMED) * 100 / (ORDERS.GST_PERCENT + 100))  FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		Total = IBInternalQuery->Fields[0]->AsCurrency;
	}
	return Total;
}

Currency TDBTab::GetTabDiscount(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	Currency Total = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT SUM(DISCOUNT) FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		Total = IBInternalQuery->Fields[0]->AsCurrency;
	}
	return Total;
}

Currency TDBTab::GetTabTotal(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency Total = 0;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM((PRICE*QTY) + DISCOUNT + REDEEMED) FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			Total = IBInternalQuery->Fields[0]->AsCurrency;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Total;
}

Currency TDBTab::GetTabCreditLimit(Database::TDBTransaction &DBTransaction,long TabKey)
{
	Currency CreditLimit = 0;

    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT CREDIT_LIMIT FROM TAB WHERE TAB_KEY = :TAB_KEY";
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
        IBInternalQuery->ExecQuery();
        if (IBInternalQuery->RecordCount)
        {
            CreditLimit = IBInternalQuery->FieldByName("CREDIT_LIMIT")->AsCurrency;
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CreditLimit;
}


void TDBTab::SetTabCreditLimit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"CREDIT_LIMIT = :CREDIT_LIMIT "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ParamByName("CREDIT_LIMIT")->AsCurrency = Amount;
	IBInternalQuery->ExecQuery();
}

UnicodeString TDBTab::GetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	UnicodeString RetVal = "";

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PIN"
	" FROM "
	"  TAB"
	" WHERE"
	" TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("PIN")->AsString;
	}
	return RetVal;
}

void TDBTab::SetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString PIN)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"PIN = :PIN "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ParamByName("PIN")->AsString = PIN;
	IBInternalQuery->ExecQuery();
}

void TDBTab::GetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, UnicodeString &Name, UnicodeString &IDType,
                            UnicodeString &Number, UnicodeString  &Date, TMMTabType   &TabType)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT TAB_NAME,ID_TYPE,ID_NUMBER,ID_EXPIRE,TAB_TYPE"
	" FROM "
	"  TAB"
	" WHERE"
	" TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		Name = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
		IDType = IBInternalQuery->FieldByName("ID_TYPE")->AsString;
		Number = IBInternalQuery->FieldByName("ID_NUMBER")->AsString;
		Date = IBInternalQuery->FieldByName("ID_EXPIRE")->AsString;
		TabType = (TMMTabType)IBInternalQuery->FieldByName("TAB_TYPE")->AsInteger;
	}
}

void TDBTab::SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, UnicodeString Name, UnicodeString Type,
                            UnicodeString Number, UnicodeString Date, TMMTabType TabType)
{
	int tabKey = GetOrCreateTab(DBTransaction, inTabKey);

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "UPDATE TAB "
                                    "SET "
                                    "TAB_NAME = :TAB_NAME, "
                                    "ID_TYPE = :ID_TYPE, "
                                    "ID_NUMBER = :ID_NUMBER, "
                                    "ID_EXPIRE = :ID_EXPIRE, "
                                    "TAB_TYPE = :TAB_TYPE "
                                    "WHERE TAB_KEY = :TAB_KEY";

	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
    IBInternalQuery->ParamByName("TAB_NAME")->AsString = Name;

	IBInternalQuery->ParamByName("ID_TYPE")->AsString = Type;
	IBInternalQuery->ParamByName("ID_NUMBER")->AsString = Number;
	IBInternalQuery->ParamByName("ID_EXPIRE")->AsString = Date;
	IBInternalQuery->ParamByName("TAB_TYPE")->AsString = TabType;

	IBInternalQuery->ExecQuery();
}

int TDBTab::SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, UnicodeString tabName, Currency tabCreditLimit, TMMTabType tabType)
{
    int tabKey = GetOrCreateTab(DBTransaction, inTabKey);

	TIBSQL *ibInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	ibInternalQuery->Close();
	ibInternalQuery->SQL->Text = "UPDATE TAB "
                                    "SET "
                                        "TAB_NAME = :TAB_NAME, "
                                        "CREDIT_LIMIT = :CREDIT_LIMIT, "
                                        "TAB_TYPE = :TAB_TYPE "
                                    "WHERE TAB_KEY = :TAB_KEY";

	ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
    if(tabType == 13)
    {
	    ibInternalQuery->ParamByName("TAB_NAME")->AsString = tabName + "-" + tabKey;
    }
    else
    {
	    ibInternalQuery->ParamByName("TAB_NAME")->AsString = tabName;
    }
	ibInternalQuery->ParamByName("CREDIT_LIMIT")->AsCurrency = tabCreditLimit;
	ibInternalQuery->ParamByName("TAB_TYPE")->AsString = tabType;

	ibInternalQuery->ExecQuery();

    return tabKey;
}

int TDBTab::SetTabDetails(Database::TDBTransaction &DBTransaction, int inTabKey, Currency tabCreditLimit)
{
    int tabKey = GetOrCreateTab(DBTransaction, inTabKey);

	TIBSQL *ibInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	ibInternalQuery->Close();
	ibInternalQuery->SQL->Text = "UPDATE TAB "
                                    "SET "
                                        "CREDIT_LIMIT = :CREDIT_LIMIT "
                                    "WHERE TAB_KEY = :TAB_KEY";

	ibInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
	ibInternalQuery->ParamByName("CREDIT_LIMIT")->AsCurrency = tabCreditLimit;

	ibInternalQuery->ExecQuery();

    return tabKey;
}

void TDBTab::SetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey,int inContactKey,TMMTabType TabType)
{
	if( inContactKey == 0)return;

	if(TabType == TabStaff || TabType == TabMember || TabType == TabInvoice|| TabType == TabParkedSale)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"TAB "
		"SET "
		"CONTACTS_KEY = :CONTACTS_KEY, "
		"TAB_TYPE = :TAB_TYPE "
		"WHERE "
		"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabType;
		IBInternalQuery->ExecQuery();
	}
}


void TDBTab::SetTabOrdersLoyalty(Database::TDBTransaction &DBTransaction,int inTabKey, int inContactKey)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" UPDATE ORDERS "
	" SET LOYALTY_KEY = :LOYALTY_KEY "
	" WHERE "
	" TAB_KEY = :TAB_KEY;";
	IBInternalQuery->ParamByName("TAB_KEY")->AsString = inTabKey;
	IBInternalQuery->ParamByName("LOYALTY_KEY")->AsString = inContactKey;
	IBInternalQuery->ExecQuery();
}

void TDBTab::SetTabName(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString TabName)
{
	int TabKey = GetOrCreateTab(DBTransaction,inTabKey);

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"TAB_NAME = :TAB_NAME "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ParamByName("TAB_NAME")->AsString = TabName.SubString(1,32);
	IBInternalQuery->ExecQuery();

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" UPDATE ORDERS "
	" SET "
	"  TAB_NAME = :THIS_TAB_NAME "
	" WHERE "
	"  TAB_KEY = :LOC_TAB_KEY ";
	IBInternalQuery->ParamByName("THIS_TAB_NAME")->AsString = TabName.SubString(1,32);
	IBInternalQuery->ParamByName("LOC_TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
}


void TDBTab::SetTabCard(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString Card)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"CARD = :BLANK "
	"WHERE "
	"CARD = :CARD";
	IBInternalQuery->ParamByName("BLANK")->AsString = "";
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();


	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"CARD = :CARD "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();

}

//---------------------------------------------------------------------------

bool TDBTab::ClearTab(Database::TDBTransaction &DBTransaction,long TabKey)
{
	if (!GetTabPermanent(DBTransaction,TabKey))
	{
		DeleteTab(DBTransaction,TabKey);
		return true;
	}
	return false;
}


int TDBTab::GetTabByCard(Database::TDBTransaction &DBTransaction,UnicodeString Card)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	int RetVal = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
	"TAB.TAB_KEY "
	"FROM "
	"TAB "
	"WHERE "
	"TAB.CARD = :CARD";
	IBInternalQuery->ParamByName("CARD")->AsString = Card;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
	}
	return RetVal;
}

int TDBTab::GetTabByOwner(Database::TDBTransaction &DBTransaction,int ContactKey)
{
	if( ContactKey == 0)return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	int RetVal = 0;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_KEY "
		"FROM "
		"TAB "
		"WHERE "
		"TAB.CONTACTS_KEY = :CONTACTS_KEY AND "
		" (TAB.TAB_TYPE = :STAFFTABTYPE OR "
		"  TAB.TAB_TYPE = :MEMBERTABTYPE )";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
		IBInternalQuery->ParamByName("STAFFTABTYPE")->AsInteger = TabStaff;
		IBInternalQuery->ParamByName("MEMBERTABTYPE")->AsInteger = TabMember;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

int TDBTab::GetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey)
{
	int RetVal = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.CONTACTS_KEY "
		"FROM "
		"TAB "
		"WHERE "
		"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

int TDBTab::GetTabInvoice(Database::TDBTransaction &DBTransaction,int TabKey)
{
	int RetVal = 0;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.INVOICE_KEY "
		"FROM "
		"TAB "
		"WHERE "
		"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("INVOICE_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

int TDBTab::GetTab(Database::TDBTransaction &DBTransaction,UnicodeString Name,TMMTabType TabType)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	int RetVal = 0;
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
	"TAB.TAB_KEY "
	"FROM "
	"TAB "
	"WHERE "
	"UPPER(TAB.TAB_NAME) = :TAB_NAME AND "
	"TAB.TAB_TYPE = :TABTYPE";
	IBInternalQuery->ParamByName("TAB_NAME")->AsString = Name.UpperCase();
	IBInternalQuery->ParamByName("TABTYPE")->AsInteger = TabType;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
	}
	return RetVal;
}

UnicodeString TDBTab::GetTabName(Database::TDBTransaction &DBTransaction,int TabKey)
{
	UnicodeString RetVal = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_NAME "
		"FROM "
		"TAB "
		"WHERE "
		"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

bool TDBTab::GetTabExists(Database::TDBTransaction &DBTransaction,int TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	bool RetVal = false;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_KEY "
		"FROM "
		"TAB "
		"WHERE "
		"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}


UnicodeString TDBTab::GetTabCard(Database::TDBTransaction &DBTransaction,int TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	UnicodeString RetVal = "";
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"SELECT "
	"TAB.CARD "
	"FROM "
	"TAB "
	"WHERE "
	"TAB.TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("CARD")->AsString;
	}
	return RetVal;
}

bool TDBTab::GetIsEmpty(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	bool RetVal = true;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT ORDER_KEY"
		" FROM "
		"  ORDERS "
		" WHERE "
		"  TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBTab::SetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey,bool Locked,UnicodeString Reason)
{
	if(inTabKey != 0)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		if(Locked)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"TAB "
			"SET "
			"LOCKED_REASON = :LOCKED_REASON "
			"WHERE "
			"TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
			IBInternalQuery->ParamByName("LOCKED_REASON")->AsString = Reason;
			IBInternalQuery->ExecQuery();
		}
		else
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"TAB "
			"SET "
			"LOCKED_REASON = :LOCKED_REASON "
			"WHERE "
			"TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
			IBInternalQuery->ParamByName("LOCKED_REASON")->AsString = "";
			IBInternalQuery->ExecQuery();
		}
	}
}


bool TDBTab::GetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	bool RetVal = false;

    try
    {

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT "
        "LOCKED_REASON "
        "FROM "
        "TAB "
        "WHERE "
        "TAB_KEY = :TAB_KEY";
        IBInternalQuery->ParamByName("TAB_KEY")->AsString = inTabKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
            if(IBInternalQuery->FieldByName("LOCKED_REASON")->AsString != "")
            {
                RetVal = true;
            }
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

UnicodeString TDBTab::GetTabLockedReason(Database::TDBTransaction &DBTransaction,int inTabKey)
{

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    UnicodeString RetVal = "";
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT "
        "LOCKED_REASON "
        "FROM "
        "TAB "
        "WHERE "
        "TAB_KEY = :TAB_KEY";
        IBInternalQuery->ParamByName("TAB_KEY")->AsString = inTabKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
            if(IBInternalQuery->FieldByName("LOCKED_REASON")->AsString != "")
            {
                RetVal = IBInternalQuery->FieldByName("LOCKED_REASON")->AsString;
            }
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBTab::SetTabToInvoice(Database::TDBTransaction &DBTransaction,int TabKey, int InvoiceKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	"TAB "
	"SET "
	"TAB_TYPE = :TAB_TYPE, "
	"INVOICE_KEY = :INVOICE_KEY, "
	"TAB_PERMANENT = 'F' "
	"WHERE "
	"TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
	IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabInvoice;
	IBInternalQuery->ExecQuery();
}


bool TDBTab::GetOutStanding(Database::TDBTransaction &DBTransaction,int ContactKey)
{
	bool RetVal = false;
	int TabKey = GetTabByOwner(DBTransaction,ContactKey);

	if(TabKey != 0)
	{
		if(GetTabBalance(DBTransaction,TabKey) != 0)
		{
			RetVal = true;
		}
	}
	return RetVal;
}

void TDBTab::GetTabs(Database::TDBTransaction &DBTransaction,TStringList * TabList, TMMTabType TabType)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		TabList->Clear();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_KEY, "
		"TAB.TAB_NAME NAME, "
		//MM2038
		"TAB.ID_NUMBER "
		"FROM "
		"TAB "
		"WHERE " //Slow removed the extra check "do not show any seat tabs, for speed"
		//" TAB.TAB_KEY NOT IN (SELECT SEAT.TAB_KEY FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL)"
		/*" AND*/" ( TAB.TAB_TYPE = " + IntToStr(TabType) + " OR TAB.TAB_TYPE = " + IntToStr(TabCashAccount) + " ) " +
		" ORDER BY "
		//"TAB.TAB_KEY DESC";
		"lower(TAB.TAB_NAME);";
		IBInternalQuery->ExecQuery();
		while (!IBInternalQuery->Eof)
		{
			int Index = TabList->Add(IBInternalQuery->FieldByName("NAME")->AsString);
			// Little bit of pointer abuse but we just want to store the int somewhere.
			TabList->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			IBInternalQuery->Next();
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}


void TDBTab::GetOrderKeysExcludeSides(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> *OrderKeys)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" 	ORDER_KEY "
		" FROM "
		"  ORDERS "
		" WHERE "
		"  TAB_KEY = :TAB_KEY AND "
		" (SIDE_ORDER_KEY IS NULL OR SIDE_ORDER_KEY = 0)";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			OrderKeys->insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTab::GetOrderKeys(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> &OrderKeys)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" 	ORDER_KEY "
		" FROM "
		"  ORDERS "
		" WHERE "
		"  TAB_KEY = :TAB_KEY ";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			OrderKeys.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTab::SetPartialPayment(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString InvoiceNumber,bool isTable)
{
	if(inTabKey == 0) return;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		bool CreatePaymentRecord = false;
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TABHISTORY_KEY "
		" FROM "
		" TABHISTORY "
		" WHERE "
		" TAB_KEY = :TAB_KEY AND "
		" INVOICE_NUMBER = :INVOICE_NUMBER AND "
		" IS_TABLE = :IS_TABLE ";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
		IBInternalQuery->ParamByName("IS_TABLE")->AsString = isTable == true ? "T" : "F";
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount == 0)
		{
			CreatePaymentRecord = true;
		}

		if(CreatePaymentRecord)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TABHISTORY, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int TabHistoryKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO TABHISTORY ("
			"TABHISTORY_KEY,"
			"TAB_KEY,"
			"INVOICE_NUMBER, "
			"IS_TABLE) "
			"VALUES ("
			":TABHISTORY_KEY,"
			":TAB_KEY,"
			":INVOICE_NUMBER, "
			":IS_TABLE);";
			IBInternalQuery->ParamByName("TABHISTORY_KEY")->AsInteger = TabHistoryKey;
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
			IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
			IBInternalQuery->ParamByName("IS_TABLE")->AsString = isTable == true ? "T" : "F";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTab::ClearTabHistory(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTable)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =  "Delete  from TABHISTORY "
	"where IS_TABLE = :IS_TABLE and  "
	"TAB_KEY = :TAB_KEY ";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
	IBInternalQuery->ParamByName("IS_TABLE")->AsString = isTable == true ? "T" : "F";
	IBInternalQuery->ExecQuery();
}

void TDBTab::GetPartialPaymentList(Database::TDBTransaction &DBTransaction,int inTabKey, TStringList *List,bool isTable)
{
	if( inTabKey == 0 )return;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT TABHISTORY.INVOICE_NUMBER,TIME_STAMP,TOTAL"
		" FROM "
		" TABHISTORY "
		" LEFT JOIN ARCBILL ON TABHISTORY.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER "
		" WHERE"
		" TABHISTORY.TAB_KEY = :TAB_KEY"
		" AND ARCBILL.INVOICE_NUMBER IS NOT NULL"
		" AND ARCBILL.INVOICE_NUMBER != '' "
		" AND TABHISTORY.IS_TABLE = :IS_TABLE "

		" UNION ALL"

		" SELECT TABHISTORY.INVOICE_NUMBER,TIME_STAMP,TOTAL"
		" FROM "
		" TABHISTORY "
		" LEFT JOIN DAYARCBILL ON TABHISTORY.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
		" WHERE"
		" TABHISTORY.TAB_KEY = :TAB_KEY "
		" AND DAYARCBILL.INVOICE_NUMBER IS NOT NULL"
		" AND DAYARCBILL.INVOICE_NUMBER != '' "
		" AND TABHISTORY.IS_TABLE = :IS_TABLE "
		" ORDER BY 1";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("IS_TABLE")->AsString = isTable == true ? "T" : "F";
		IBInternalQuery->ExecQuery();
		if(List != NULL)
		{
			List->Clear();
			if(IBInternalQuery->RecordCount)
			{
				for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
				{
					List->Add("Rcpt #" + IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString + " " +
					IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("dd/mm/yyyy") + " " +
					FloatToStrF(IBInternalQuery->FieldByName("TOTAL")->AsFloat, ffCurrency, 17, 2));
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;
	}
}

bool TDBTab::GetTabRemovable(Database::TDBTransaction &DBTransaction,int TabKey)
{
	if( TabKey == 0 )return false;

	bool RetVal = true;
	try
	{
		if(!GetTabExists(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(GetTabPermanent(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(!GetIsEmpty(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(GetTabCredit(DBTransaction,TabKey) != 0)
		{
			RetVal = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		throw;
	}
	return RetVal;
}

bool TDBTab::LockTabs(Database::TDBTransaction &DBTransaction,std::set <__int64> TabsToLock, UnicodeString TerminalName, bool Override)
{
	bool retval = true;

	std::set <__int64> LockedByMeTabs;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		for (std::set <__int64> ::iterator itTabs = TabsToLock.begin(); itTabs != TabsToLock.end(); advance(itTabs, 1))
		{
			long TabKey = *itTabs;
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = " SELECT TAB.TAB_KEY,TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
			IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount)
			{
				if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString == "" || Override)
				{
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = " UPDATE TAB " " SET " " TAB.LOCKED_BY = :THIS_TERMINAL_NAME " " WHERE "
					"  TAB.TAB_KEY = :LOC_TAB_KEY ";
					IBInternalQuery->ParamByName("THIS_TERMINAL_NAME")->AsString = TerminalName;
					IBInternalQuery->ParamByName("LOC_TAB_KEY")->AsInteger = TabKey;
					IBInternalQuery->ExecQuery();
					if (IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
					{
						retval = false;
					}
					else
					{
						IBInternalQuery->Close();
						IBInternalQuery->SQL->Text = " SELECT TAB.TAB_KEY,TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
						IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
						IBInternalQuery->ExecQuery();
						if (IBInternalQuery->RecordCount == 0)
						{
							retval = false;
						}
						else if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString != TerminalName)
						{
							retval = false;
						}
						else
						{
							LockedByMeTabs.insert(TabKey);
						}
					}
				}
				else if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString != TerminalName)
				{
					retval = false;
				}
			}
		}
		if (retval == false)
		{ // unlock all tabs.
			for (std::set <__int64> ::iterator unlockTabs = LockedByMeTabs.begin(); unlockTabs != LockedByMeTabs.end(); advance(unlockTabs, 1))
			{
				long TabKey = *unlockTabs;
				ReleaseTab(DBTransaction,TerminalName,TabKey);
			}
		}
	}
	catch(Exception & E)
	{
#ifdef madExcept
		HandleException();
#endif
		if (retval == false)
		{ // unlock all tabs.
			for (std::set <__int64> ::iterator unlockTabs = LockedByMeTabs.begin(); unlockTabs != LockedByMeTabs.end(); advance(unlockTabs, 1))
			{
				long TabKey = *unlockTabs;
				ReleaseTab(DBTransaction,TerminalName,TabKey);
			}
		}
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		retval = false;
	}
	return retval;
}

bool TDBTab::LockTab(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,long TabKey)
{
	bool retval = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT TAB.TAB_KEY,TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
		IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString == "")
			{
				IBInternalQuery->Close();
				IBInternalQuery->ParamCheck = true;
				IBInternalQuery->SQL->Text = " UPDATE TAB " " SET " " TAB.LOCKED_BY = :THIS_TERMINAL_NAME " " WHERE "
				"  TAB.TAB_KEY = :LOC_TAB_KEY ";
				IBInternalQuery->ParamByName("THIS_TERMINAL_NAME")->AsString = TerminalName;
				IBInternalQuery->ParamByName("LOC_TAB_KEY")->AsInteger = TabKey;
				IBInternalQuery->ExecQuery();
				if (IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
				{
					retval = false;
				}
				else
				{
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = " SELECT TAB.TAB_KEY,TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
					IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
					IBInternalQuery->ExecQuery();
					if (IBInternalQuery->RecordCount == 0)
					{
						retval = false;
					}
					else if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString == TerminalName)
					{
						retval = true;
					}
					else
					{
						retval = false;
					}
				}
			}
			else if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString == TerminalName)
			{
				retval = true;
			}
		}
	}
	catch(Exception & E)
	{
#ifdef madExcept
		HandleException();
#endif
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		retval = false;
	}
	return retval;
}

void TDBTab::ReleaseTab(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,long TabKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if (TabKey == 0)
		{ // release all
			IBInternalQuery->Close();
			IBInternalQuery->ParamCheck = true;
			IBInternalQuery->SQL->Text = " UPDATE TAB " " SET " " TAB.LOCKED_BY = NULL " " WHERE " "  TAB.LOCKED_BY = :THIS_TERMINAL_NAME ";
			IBInternalQuery->ParamByName("THIS_TERMINAL_NAME")->AsString = TerminalName;
			IBInternalQuery->ExecQuery();
		}
		else
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = " UPDATE TAB " " SET " " TAB.LOCKED_BY = NULL " " WHERE " "  TAB.TAB_KEY = :LOC_TAB_KEY ";
			IBInternalQuery->ParamByName("LOC_TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception & E)
	{
#ifdef madExcept
		HandleException();
#endif
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

// ---------------------------------------------------------------------------
void TDBTab::ReleaseTabs(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,std::set <__int64> TabsToRelease)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		for (std::set <__int64> ::iterator itTabs = TabsToRelease.begin(); itTabs != TabsToRelease.end(); advance(itTabs, 1))
		{
			long TabKey = *itTabs;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = " UPDATE TAB" " SET" " TAB.LOCKED_BY = NULL" " WHERE" " TAB.TAB_KEY = :LOC_TAB_KEY AND"
			" TAB.LOCKED_BY = :TERMINAL_NAME";
			IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
			IBInternalQuery->ParamByName("LOC_TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception & E)
	{
#ifdef madExcept
		HandleException();
#endif
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

UnicodeString TDBTab::GetLocker(Database::TDBTransaction &DBTransaction,long TabKey)
{
	UnicodeString RetVal = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT TAB.TAB_KEY,TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
		IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("LOCKED_BY")->AsString;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TDBTab::GetLocked(Database::TDBTransaction &DBTransaction,long TabKey)
{
	bool RetVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = " SELECT TAB.LOCKED_BY " " FROM TAB " " WHERE " "  TAB.TAB_KEY = :Loc_TAB_KEY";
	IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		if (IBInternalQuery->FieldByName("LOCKED_BY")->AsString != "")
		{
			RetVal = true;
		}
	}
	return RetVal;
}

int TDBTab::GetTabByDelyedInvoiceNumber(Database::TDBTransaction &DBTransaction,AnsiString DelayedInvoiceNumber,AnsiString &TabName)
{
	int RetVal = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = " SELECT first(1)  a.TAB_KEY,a.TAB_NAME,a.DELAYED_INVOICE_NUMBER "
	" FROM ORDERS a where a.DELAYED_INVOICE_NUMBER = :DELAYED_INVOICE_NUMBER";
	IBInternalQuery->ParamByName("DELAYED_INVOICE_NUMBER")->AsString = DelayedInvoiceNumber;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		TabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
	}
	return RetVal;
}

bool TDBTab::DeletePaymentType(Database::TDBTransaction &DBTransaction,long inTabKey)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM PAYMENTTYPES WHERE TABKEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected <= 0)
		{
			RetVal = false;
		}
		else
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

int TDBTab::GetDelyedInvoiceNumberCount(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = " SELECT count(a.DELAYED_INVOICE_NUMBER) NumberOfDelayedInvoice FROM ORDERS a where a.DELAYED_INVOICE_NUMBER<>''";
	IBInternalQuery->ExecQuery();
	RetVal = IBInternalQuery->FieldByName("NumberOfDelayedInvoice")->AsInteger;

	return RetVal;
}



void TDBTab::SetTabDiscounts(Database::TDBTransaction &DBTransaction, int inTabKey, int DiscountKey)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TabDISCOUNTS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int RetVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO TABDISCOUNTS (" "TABDISCOUNTS_KEY," "TAB_KEY," "DISCOUNT_KEY) " "VALUES ("
		":TABDISCOUNTS_KEY," ":TAB_KEY," ":DISCOUNT_KEY);";

		IBInternalQuery->ParamByName("TABDISCOUNTS_KEY")->AsInteger = RetVal;
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}


void TDBTab::DeleteTabDiscounts(Database::TDBTransaction &DBTransaction, int inTabKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM TABDISCOUNTS WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}


TMMContactInfo TDBTab::GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inTabKey, TMMContactInfo &Info)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT DISCOUNT_KEY" " FROM " "  TABDISCOUNTS" " WHERE"
		" TAB_KEY = :TAB_KEY ORDER BY TABDISCOUNTS_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				Info.AutoAppliedDiscounts.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
			}
		}
		return Info;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------



bool TDBTab::GetTabDiscountExists(Database::TDBTransaction &DBTransaction,int TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	bool RetVal = false;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"TABDISCOUNTS.TAB_KEY "
		"FROM "
		"TABDISCOUNTS "
		"WHERE "
		"TABDISCOUNTS.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}
//------------------------------------------------------------------------------------------------------------------
bool TDBTab::SetVisible(Database::TDBTransaction &dbTransaction, int itemKey, UnicodeString sizeName, int menuKey)
{
    TIBSQL *ibInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

    if(itemKey <= 0)
    {
        return false;
    }

    try
	{
            ibInternalQuery->Close();

            ibInternalQuery->SQL->Text =
            "UPDATE ITEMSIZE A  SET A.ISINVISIBLE= :ISINVISIBLE WHERE A.ITEM_KEY = :ITEM_KEY AND A.SIZE_NAME = :SIZE_NAME ";

            ibInternalQuery->ParamByName("ISINVISIBLE")->AsString = "F";
            ibInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemKey;
            ibInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
            ibInternalQuery->ExecQuery();

            ibInternalQuery->Close();
            ibInternalQuery->SQL->Text =
            "UPDATE SIZES A  SET A.ISINVISIBLE= :ISINVISIBLE WHERE A.MENU_KEY = :MENU_KEY AND A.SIZE_NAME = :SIZE_NAME ";

            ibInternalQuery->ParamByName("ISINVISIBLE")->AsString = "F";
            ibInternalQuery->ParamByName("MENU_KEY")->AsInteger = menuKey;
            ibInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
            ibInternalQuery->ExecQuery();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return true;
}
///-----------------------------------------------------------------------------
void TDBTab::UpdateTabName(Database::TDBTransaction &dbTransaction, long SourceKey, UnicodeString clipTabName,bool isTabSelected)
{
    TIBSQL *iBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
        TIBSQL *IBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());
        iBUpdateQuery->Close();
        iBUpdateQuery->SQL->Text =
                                "UPDATE "
                                "TAB "
                                "SET "
                                "TAB_NAME = :TAB_NAME "
                                "WHERE "
                                "TAB_KEY = :TAB_KEY ";

        iBUpdateQuery->ParamByName("TAB_KEY")->AsInteger = SourceKey;
        iBUpdateQuery->ParamByName("TAB_NAME")->AsString = clipTabName;
        iBUpdateQuery->ExecQuery();
    }

    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
///-----------------------------------------------------------------------------

void TDBTab::UpdateGuestLimit(Database::TDBTransaction &dbTransaction, long sourceKey, long destKey)
{
    TIBSQL *updateQuery = dbTransaction.Query(dbTransaction.AddQuery());
    TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text =
                                    " SELECT CREDIT_LIMIT "
                                    " FROM TAB "
                                    " WHERE "
                                    " TAB.TAB_KEY = :TAB_KEY;";
        selectQuery->ParamByName("TAB_KEY")->AsInteger = sourceKey;
        selectQuery->ExecQuery();

        if (!selectQuery->Eof)
        {
                updateQuery->Close();
                updateQuery->SQL->Text =
                                        " UPDATE TAB "
                                        " SET "
                                        " TAB.CREDIT_LIMIT = :CREDIT_LIMIT "
                                        " WHERE "
                                        " TAB.TAB_KEY = :TAB_KEY;";
                updateQuery->ParamByName("TAB_KEY")->AsInteger = destKey;
                updateQuery->ParamByName("CREDIT_LIMIT")->AsCurrency = selectQuery->FieldByName("CREDIT_LIMIT")->AsCurrency;
                updateQuery->ExecQuery();
                selectQuery->Next();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}



////--------------------------------------------------------------------------------
void TDBTab::UpdateTabType(Database::TDBTransaction &dbTransaction, long SourceKey,TMMTabType tabType)
{
    TIBSQL *iBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
        TIBSQL *IBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());
        iBUpdateQuery->Close();
        iBUpdateQuery->SQL->Text =
                                "UPDATE "
                                "TAB "
                                "SET "
                                "TAB_TYPE = :TAB_TYPE "
                                "WHERE "
                                "TAB_KEY = :TAB_KEY ";

        iBUpdateQuery->ParamByName("TAB_KEY")->AsInteger = SourceKey;
        iBUpdateQuery->ParamByName("TAB_TYPE")->AsInteger = (tabType==TabClipp)?13:0;
        iBUpdateQuery->ExecQuery();
    }

    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

////--------------------------------------------------------------------------------
TMMTabType TDBTab::GetLinkedTableAndClipTab(Database::TDBTransaction &DBTransaction,int inTabKey,bool isTabSelected,TMMTabType tabType)
{
    TMMTabType Type = tabType;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

    try
    {
      if(isTabSelected)
      {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT ta.TAB_TYPE FROM TAB  ta inner join  CLIPP_TAB ctb on  ctb.CLIPP_TAB_TAB_KEY =ta.TAB_KEY  "
                                     "WHERE ta.TAB_KEY = :TAB_KEY  and ta.TAB_TYPE = :TAB_TYPE ";
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
        IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = 3;

        IBInternalQuery->ExecQuery();
        if (!IBInternalQuery->Eof)
        {
            Type = TabClipp;
        }
      }
      else
      {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT ta.TAB_TYPE FROM TAB  ta inner join  CLIPP_TAB ctb on  ctb.CLIPP_TAB_TAB_KEY =ta.TAB_KEY  "
                                     "LEFT JOIN ORDERS ord on ord.TAB_KEY=ta.TAB_KEY "
                                     "WHERE ord.ORDER_KEY =:ORDER_KEY  and ta.TAB_TYPE = :TAB_TYPE ";
        IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = inTabKey;
        IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = 3;

        IBInternalQuery->ExecQuery();
        if (!IBInternalQuery->Eof)
        {
            Type = TabClipp;
        }
      }

     return  Type;

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

///-----------------------------------------------
int TDBTab::GetTableKeyFromSeat(Database::TDBTransaction &dbTransaction,long SourceKey)
{


    TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text =
                                    " SELECT TABLE_KEY "
                                    " FROM SEAT "
                                    " WHERE "
                                    " SEAT.TAB_KEY = :TAB_KEY;";
        selectQuery->ParamByName("TAB_KEY")->AsInteger = SourceKey;
        selectQuery->ExecQuery();

        if(selectQuery->RecordCount > 0)
        {
            return selectQuery->FieldByName("TABLE_KEY")->AsInteger;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

}

///--------------------------------------------------------------------------------
int TDBTab::GetMaxAvailableSeatKey(Database::TDBTransaction &dbTransaction, int destTableKey)
{
    TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text =
                                    "SELECT "
                                        "SEAT.SEATNO "
                                    "FROM "
                                        "SEAT INNER JOIN TABLES ON SEAT.TABLE_KEY = TABLES.TABLE_KEY "
                                    "WHERE "
                                        "TABLES.TABLE_NUMBER = :TABLE_KEY AND SEAT.TAB_KEY is NOT NULL "
                                        "ORDER by 1 ";
        selectQuery->ParamByName("TABLE_KEY")->AsInteger = destTableKey;
        selectQuery->ExecQuery();

        if(!selectQuery->Eof)
        {
            for(int SeatKey=1; SeatKey < 100; SeatKey++)
            {
                if(SeatKey != selectQuery->FieldByName("SEATNO")->AsInteger)
                {
                    return SeatKey;
                }
                selectQuery->Next();
            }
        }

        if(selectQuery->RecordCount == 0)
        {
            return 1;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
///-----------------------------------------------------------------

int TDBTab::GetInvoiceNo(Database::TDBTransaction &dbTransaction, int invoiceKey)
{
    TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
    int invoiceNo=0;

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text =
                                    "SELECT "
                                        "INVOICES.INVOICE_NUMBER "
                                    "FROM "
                                        "INVOICES "
                                    "WHERE "
                                        "INVOICES.INVOICE_KEY = :INVOICE_KEY ";

        selectQuery->ParamByName("INVOICE_KEY")->AsInteger = invoiceKey;
        selectQuery->ExecQuery();

        invoiceNo = selectQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
       return invoiceNo;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
///----------------------------------------------------------------

AnsiString TDBTab::GetInvoiceNoFromTabKey(Database::TDBTransaction &dbTransaction, int tabKey)
{
    TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
    AnsiString invoiceNo=0;

    try
    {
        selectQuery->Close();
        selectQuery->SQL->Text =
                                    "SELECT "
                                    "inv.INVOICE_NUMBER "
                                    "FROM TAB tb inner join INVOICES inv on inv.INVOICE_KEY = tb.INVOICE_KEY  "
                                    "WHERE tb.TAB_KEY = :TAB_KEY ";

        selectQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        selectQuery->ExecQuery();

        invoiceNo = selectQuery->FieldByName("INVOICE_NUMBER")->AsString;
       return invoiceNo;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//-------------------------------------------------------------------------------
UnicodeString TDBTab::GetAccountNumber(Database::TDBTransaction &dbTransaction, int tabKey)
{
    UnicodeString AccNo = "";
    try
    {
        TIBSQL *selectQuery = dbTransaction.Query(dbTransaction.AddQuery());

        selectQuery->Close();
        selectQuery->SQL->Text = "SELECT ACC_NO FROM ORDERS A WHERE TAB_KEY = :TAB_KEY "
                                  "GROUP BY 1 ";

        selectQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        selectQuery->ExecQuery();

        if(selectQuery->RecordCount)
            AccNo = selectQuery->FieldByName("ACC_NO")->AsString;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return AccNo;
}
//-----------------------------------------------------------------------------
void TDBTab::SetDelayedPatronCount(Database::TDBTransaction &DBTransaction, int _tabKey, std::vector<TPatronType> _patrons)
{
    try
	{
		std::vector <TPatronType> ::iterator ptrPatronTypes;
		for (ptrPatronTypes = _patrons.begin(); ptrPatronTypes != _patrons.end(); ptrPatronTypes++)
		{
			if (ptrPatronTypes->Count != 0)
			{
				TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TABPATRONCOUNT, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();

				int patronCountKey = IBInternalQuery->Fields[0]->AsInteger;
				IBInternalQuery->Close();

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
					 "INSERT INTO TABPATRONCOUNT (" "TABPATRONCOUNT_KEY, " "TAB_KEY, " "PATRON_TYPE, " "PATRON_COUNT) "
					 "VALUES (" ":TABPATRONCOUNT_KEY, " ":TAB_KEY, " ":PATRON_TYPE, " ":PATRON_COUNT) ";

				IBInternalQuery->ParamByName("TABPATRONCOUNT_KEY")->AsString = patronCountKey;
				IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = _tabKey;
				IBInternalQuery->ParamByName("PATRON_TYPE")->AsString = ptrPatronTypes->Name;
				IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = ptrPatronTypes->Count;

				IBInternalQuery->ExecQuery();
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------
void TDBTab::DeleteDelayedPatronCount(Database::TDBTransaction &DBTransaction, int _tabKey)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =
             "DELETE FROM TABPATRONCOUNT WHERE TAB_KEY = :TAB_KEY ";

        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = _tabKey;
        IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------
std::vector<TPatronType> TDBTab::GetDelayedPatronCount(Database::TDBTransaction &DBTransaction, int _tabKey)
{
    std::vector<TPatronType> patrons;
    patrons.clear();
    TPatronType patronInfo;
    try
    {
        std::auto_ptr<TManagerPatron> managerPatron(new TManagerPatron());
        managerPatron->GetPatronTypes(DBTransaction, patrons);
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =
             "SELECT * FROM TABPATRONCOUNT WHERE TAB_KEY = :TAB_KEY ORDER BY PATRON_TYPE";

        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = _tabKey;
        IBInternalQuery->ExecQuery();
        if (IBInternalQuery->RecordCount)
		{
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
                for(int index = 0; index < patrons.size(); index++)
                {
                    if(patrons[index].Name == IBInternalQuery->FieldByName("PATRON_TYPE")->AsString)
                        patrons[index].Count = IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;
                }
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
    return patrons;
}
//-----------------------------------------------------------------------------
bool TDBTab::HasOnlineOrders(int tabKey)
{
    bool retValue = false;
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
        query->SQL->Text = "SELECT ORDER_KEY FROM  ORDERS WHERE TAB_KEY = :TAB_KEY AND "
                           "ONLINE_ORDER_ID <> :ONLINE_ORDER_ID AND ONLINE_ORDER_ID IS NOT NULL";
        query->ParamByName("TAB_KEY")->AsInteger = tabKey;
        query->ParamByName("ONLINE_ORDER_ID")->AsString = "";
        query->ExecQuery();
        if(query->RecordCount > 0)
        {
            retValue = true;
        }

        dbTransaction.Commit();
    }
    catch(Exception &ex)
    {
       MessageBox(ex.Message,"",MB_OK);
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
       dbTransaction.Rollback();
    }
    return retValue;
}
//-----------------------------------------------------------------------------
UnicodeString TDBTab::GetMemberEmail(int tabKey)
{
    UnicodeString email = "";
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
        query->Close();
        query->SQL->Text = "SELECT EMAIL FROM  ORDERS WHERE TAB_KEY = :TAB_KEY AND "
                           "ONLINE_ORDER_ID <> :ONLINE_ORDER_ID AND ONLINE_ORDER_ID IS NOT NULL";
        query->ParamByName("TAB_KEY")->AsInteger = tabKey;
        query->ParamByName("ONLINE_ORDER_ID")->AsString = "";
        query->ExecQuery();
        if(query->RecordCount > 0)
        {
           if(query->FieldByName("EMAIL")->AsString != NULL && query->FieldByName("EMAIL")->AsString != "")
               email = query->FieldByName("EMAIL")->AsString;
           else
              MessageBox("Membership details for the order are missing.","Info",MB_OK+MB_ICONINFORMATION);
        }
        dbTransaction.Commit();
    }
    catch(Exception &ex)
    {
       dbTransaction.Rollback();
    }
    return email;
}
//-----------------------------------------------------------------------------
