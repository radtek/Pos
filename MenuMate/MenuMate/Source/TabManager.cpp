//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#ifdef PalmMate
#include "Palm.h"
#endif

#include "TabManager.h"
#include "MMLogging.h"
#include "Invoice.h"
#include "TableManager.h"
#include "ManagerNet.h"
#include "NetMessageTabChanged.h"
#include <Memory>



#pragma package(smart_init)


TTabManager::TTabManager()
{
}

TTabManager::~TTabManager()
{
}

int TTabManager::GetOrCreateTab(Database::TDBTransaction &DBTransaction,int inTabKey)
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
      IBInternalQuery->SQL->Text =
      "INSERT INTO TAB ("
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

bool TTabManager::DeleteTab(Database::TDBTransaction &DBTransaction,long inTabKey)
{
	bool RetVal = false;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM TAB WHERE TAB_KEY = :TAB_KEY";
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


bool TTabManager::CloseTab(Database::TDBTransaction &DBTransaction,long inTabKey)
{
	bool RetVal = false;

	if(GetTabRemovable(DBTransaction,inTabKey))
	{
		DeleteTab(DBTransaction,inTabKey);
		RetVal = true;
	}
	return RetVal;
}

void TTabManager::SetTabType(Database::TDBTransaction &DBTransaction,int inTabKey, TMMTabType inType)
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

void TTabManager::SetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey, bool Permanent)
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

bool TTabManager::GetTabPermanent(Database::TDBTransaction &DBTransaction,int inTabKey)
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

void TTabManager::SetTabCredit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount)
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

Currency TTabManager::GetTabBalance(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency Balance = 0;
	try
	{
		Balance = -GetTabCredit(DBTransaction,TabKey);
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM(PRICE + DISCOUNT + REDEEMED) FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
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

int TTabManager::GetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey)
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

void TTabManager::SetSplitDivisions(Database::TDBTransaction &DBTransaction,long TabKey,int DivisionsLeft)
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

Currency TTabManager::GetTabCredit(Database::TDBTransaction &DBTransaction,long TabKey)
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

TMMTabType TTabManager::GetTabType(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	TMMTabType Type = TabNormal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT TAB_TYPE FROM TAB WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			Type = (TMMTabType)IBInternalQuery->FieldByName("TAB_TYPE")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Type;
}


Currency TTabManager::GetTabTotalExcl(Database::TDBTransaction &DBTransaction,long TabKey)
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

Currency TTabManager::GetTabDiscount(Database::TDBTransaction &DBTransaction,long TabKey)
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

Currency TTabManager::GetTabTotal(Database::TDBTransaction &DBTransaction,long TabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	Currency Total = 0;
	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM(PRICE + DISCOUNT + REDEEMED) FROM ORDERS WHERE TAB_KEY = :TAB_KEY";
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

Currency TTabManager::GetTabCreditLimit(Database::TDBTransaction &DBTransaction,long TabKey)
{
	Currency CreditLimit = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT CREDIT_LIMIT FROM TAB WHERE TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		CreditLimit = IBInternalQuery->FieldByName("CREDIT_LIMIT")->AsCurrency;
	}
	return CreditLimit;
}


void TTabManager::SetTabCreditLimit(Database::TDBTransaction &DBTransaction,int inTabKey, Currency Amount)
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

UnicodeString TTabManager::GetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey)
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

void TTabManager::SetTabPIN(Database::TDBTransaction &DBTransaction,int inTabKey,UnicodeString PIN)
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

void TTabManager::GetTabDetails(
						 Database::TDBTransaction &DBTransaction,
						 int inTabKey,
                   UnicodeString &Name,
						 UnicodeString &IDType,
                   UnicodeString &Number,
                   UnicodeString  &Date,
                   TMMTabType   &TabType)
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

void TTabManager::SetTabDetails(
						 Database::TDBTransaction &DBTransaction,
						 int inTabKey,
                   UnicodeString Name,
						 UnicodeString Type,
                   UnicodeString Number,
                   UnicodeString Date,
                   TMMTabType TabType)
{
	int TabKey = GetOrCreateTab(DBTransaction,inTabKey);

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "UPDATE "
      "TAB "
   "SET "
      "TAB_NAME = :TAB_NAME, "
      "ID_TYPE = :ID_TYPE, "
      "ID_NUMBER = :ID_NUMBER, "
      "ID_EXPIRE = :ID_EXPIRE, "
      "TAB_TYPE = :TAB_TYPE "
   "WHERE "
      "TAB_KEY = :TAB_KEY";
   IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
   IBInternalQuery->ParamByName("TAB_NAME")->AsString = Name;
   IBInternalQuery->ParamByName("ID_TYPE")->AsString = Type;
   IBInternalQuery->ParamByName("ID_NUMBER")->AsString = Number;
	IBInternalQuery->ParamByName("ID_EXPIRE")->AsString = Date;
	IBInternalQuery->ParamByName("TAB_TYPE")->AsString = TabType;
   IBInternalQuery->ExecQuery();
}

void TTabManager::SetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey,int inContactKey,TMMTabType TabType)
{
	if( inContactKey == 0)return;
	
   if(TabType == TabStaff || TabType == TabMember || TabType == TabInvoice)
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


void TTabManager::SetTabOrdersLoyalty(Database::TDBTransaction &DBTransaction,int inTabKey, int inContactKey)
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

void TTabManager::SetTabName(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString TabName)
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


void TTabManager::SetTabCard(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString Card)
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

bool TTabManager::ClearTab(Database::TDBTransaction &DBTransaction,long TabKey)
{
	if (!TDBTab::GetTabPermanent(DBTransaction,TabKey))
   {
		TDBTab::DeleteTab(DBTransaction,TabKey);
	   return true;
   }
	return false;
}


int TTabManager::GetTabByCard(Database::TDBTransaction &DBTransaction,UnicodeString Card)
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

int TTabManager::GetTabByOwner(Database::TDBTransaction &DBTransaction,int ContactKey)
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

int TTabManager::GetTabOwner(Database::TDBTransaction &DBTransaction,int TabKey)
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

int TTabManager::GetTabInvoice(Database::TDBTransaction &DBTransaction,int TabKey)
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

int TTabManager::GetTab(Database::TDBTransaction &DBTransaction,UnicodeString Name,TMMTabType TabType)
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

UnicodeString TTabManager::GetTabName(Database::TDBTransaction &DBTransaction,int TabKey)
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

bool TTabManager::GetTabExists(Database::TDBTransaction &DBTransaction,int TabKey)
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


UnicodeString TTabManager::GetTabCard(Database::TDBTransaction &DBTransaction,int TabKey)
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

bool TTabManager::GetIsEmpty(Database::TDBTransaction &DBTransaction,int inTabKey)
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

void TTabManager::SetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey,bool Locked,UnicodeString Reason)
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


bool TTabManager::GetTabLocked(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	bool RetVal = false;

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
	return RetVal;
}

UnicodeString TTabManager::GetTabLockedReason(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	UnicodeString RetVal = "";

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
	return RetVal;
}

void TTabManager::SetTabToInvoice(Database::TDBTransaction &DBTransaction,int TabKey, int InvoiceKey)
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


bool TTabManager::GetOutStanding(Database::TDBTransaction &DBTransaction,int ContactKey)
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

void TTabManager::GeTTabManager(Database::TDBTransaction &DBTransaction,TStringList * TabList, TMMTabType TabType)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      TabList->Clear();
      IBInternalQuery->SQL->Text =
         "SELECT "
            "TAB.TAB_KEY, "
            "TAB.TAB_NAME NAME "
         "FROM "
            "TAB "
         "WHERE "
            "TAB.TAB_KEY NOT IN (SELECT SEAT.TAB_KEY FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL)"
            " AND ( TAB.TAB_TYPE = " + IntToStr(TabType) + " OR TAB.TAB_TYPE = " + IntToStr(TabCashAccount) + " ) " +
         " ORDER BY "
            "TAB.TAB_KEY DESC";
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


void TTabManager::GetOrderKeysExcludeSides(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> *OrderKeys)
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
      " SIDE_ORDER_KEY IS NULL";
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


void TTabManager::SetPartialPayment(Database::TDBTransaction &DBTransaction,int inTabKey, UnicodeString InvoiceNumber)
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
		"  TABHISTORY "
		" WHERE "
		"  TAB_KEY = :TAB_KEY AND "
		"  INVOICE_NUMBER = :INVOICE_NUMBER";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
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
				"INVOICE_NUMBER) "
			"VALUES ("
				":TABHISTORY_KEY,"
				":TAB_KEY,"
				":INVOICE_NUMBER);";
			IBInternalQuery->ParamByName("TABHISTORY_KEY")->AsInteger = TabHistoryKey;
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
			IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TTabManager::GetPartialPaymentList(Database::TDBTransaction &DBTransaction,int inTabKey, TStringList *List)
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
		" AND ARCBILL.INVOICE_NUMBER != ''"

		" UNION ALL"

		" SELECT TABHISTORY.INVOICE_NUMBER,TIME_STAMP,TOTAL"
		" FROM "
		" TABHISTORY "
		" LEFT JOIN DAYARCBILL ON TABHISTORY.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBER "
		" WHERE"
		" TABHISTORY.TAB_KEY = :TAB_KEY "
		" AND DAYARCBILL.INVOICE_NUMBER IS NOT NULL"
		" AND DAYARCBILL.INVOICE_NUMBER != ''"

      " ORDER BY 1";
      IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;

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
								 FloatToStrF(IBInternalQuery->FieldByName("TOTAL")->AsFloat, ffCurrency, 15, 2));
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

bool TTabManager::GetTabRemovable(Database::TDBTransaction &DBTransaction,int TabKey)
{
	if( TabKey == 0 )return false;

	bool RetVal = true;
	try
	{
		if(!TDBTab::GetTabExists(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(TDBTab::GetTabPermanent(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(!TDBTab::GetIsEmpty(DBTransaction,TabKey))
		{
			RetVal = false;
		}
		else if(TDBTab::GetTabCredit(DBTransaction,TabKey) != 0)
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
