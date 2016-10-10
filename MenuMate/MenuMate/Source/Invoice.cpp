//---------------------------------------------------------------------------
#pragma hdrstop
#include "Invoice.h"
#include "MMLogging.h"
#include "Enum.h"
#include <Memory>

#include "DBTab.h"

#pragma package(smart_init)

TInvoice *Invoice;

TInvoice::TInvoice()
{
	InvoiceNumber = "";
   Enabled = false;
}

TInvoice::~TInvoice()
{
}

void __fastcall TInvoice::SetEnabled(bool value)
{
   if (fEnabled != value)
   {
		fEnabled = value;
	}
}

bool __fastcall TInvoice::GetEnabled()
{
   return fEnabled;
}

void TInvoice::Initialise()
{
	InvoiceNumber = "";
	Enabled = true;
}


//---------------------------------------------------------------------------
int TInvoice::CreateInvoice(Database::TDBTransaction &DBTransaction,int ContactKey,Currency TotalCostExcl,Currency TotalCost,UnicodeString InvoiceNumber)
{
   int InvoiceKey = 0;
   if( !fEnabled )return 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		if(InvoiceNumber == "")
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			InvoiceNumber = IntToStr(IBInternalQuery->Fields[0]->AsInteger);
		}

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICES, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
      InvoiceKey = IBInternalQuery->Fields[0]->AsInteger;

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "INSERT INTO INVOICES ("
         "INVOICE_KEY,"
         "CONTACTS_KEY,"
         "INVOICE_NUMBER,"
         "CREATION_DATE,"
         "TOTAL_INC,"
			"TOTAL_EXCL,"
         "CLOSED) "
      "VALUES ("
         ":INVOICE_KEY,"
         ":CONTACTS_KEY,"
         ":INVOICE_NUMBER,"
         ":CREATION_DATE,"
         ":TOTAL_INC,"
         ":TOTAL_EXCL,"
         ":CLOSED);";
      IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
      IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
      IBInternalQuery->ParamByName("CREATION_DATE")->AsDateTime = Now();
      IBInternalQuery->ParamByName("TOTAL_INC")->AsCurrency = TotalCost;
      IBInternalQuery->ParamByName("TOTAL_EXCL")->AsCurrency = TotalCostExcl;
      IBInternalQuery->ParamByName("CLOSED")->AsString = "F";
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return InvoiceKey;
}

int TInvoice::GetInvoiceOwner(Database::TDBTransaction &DBTransaction,int InvoiceKey)
{
   if( !fEnabled )return 0;
	int RetVal = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"INVOICES.CONTACTS_KEY "
	"FROM "
		"INVOICES "
	"WHERE "
		"INVOICES.INVOICE_KEY = :INVOICE_KEY";
	IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
   	RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
   }

   return RetVal;
}

bool TInvoice::GetOwnerHasInvoice(Database::TDBTransaction &DBTransaction,int inContactKey)
{
   if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	
   bool RetVal = false;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"INVOICES.INVOICE_KEY "
	"FROM "
		"INVOICES "
	"WHERE "
		"INVOICES.CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
   IBInternalQuery->ExecQuery();

   if(IBInternalQuery->RecordCount)
   {
   	RetVal = true;
   }

   return RetVal;
}

int TInvoice::GetInvoiceTabs(Database::TDBTransaction &DBTransaction,int InvoiceKey,std::set<long> *Tabs)
{
   if( !fEnabled )return 0;
	int RetVal = 0;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT "
         "TAB.TAB_KEY "
      "FROM "
         "TAB "
      "WHERE "
         "TAB.INVOICE_KEY = :INVOICE_KEY";
      IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
      IBInternalQuery->ExecQuery();

		for (;!IBInternalQuery->Eof;IBInternalQuery->Next())
      {
         Tabs->insert(IBInternalQuery->FieldByName("TAB_KEY")->AsInteger);
      }
      RetVal = IBInternalQuery->RecordCount;
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

bool TInvoice::GetOutStanding(Database::TDBTransaction &DBTransaction,int ContactKey)
{
   if( !fEnabled )return 0;
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT "
         "INVOICES.INVOICE_KEY "
		"FROM "
         "INVOICES "
      "WHERE "
         "INVOICES.CONTACTS_KEY = :CONTACTS_KEY AND "
         "INVOICES.CLOSED = 'F' ";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
      IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

UnicodeString TInvoice::GetInvoiceNumber(Database::TDBTransaction &DBTransaction,int InvoiceKey)
{
	if( !fEnabled )return "";
	UnicodeString InvoiceNumber = "";
	try
	{
		if(InvoiceKey != 0)
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" SELECT "
			" INVOICE_NUMBER "
			" FROM "
			"  INVOICES "
			" WHERE "
			"  INVOICE_KEY = :INVOICE_KEY";
			IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
			IBInternalQuery->ExecQuery();
			if(IBInternalQuery->RecordCount)
			{
				InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return InvoiceNumber;
}

int TInvoice::GetInvoiceExists(Database::TDBTransaction &DBTransaction,UnicodeString InvoiceNumber)
{
   if( !fEnabled )return false;
	int InvoiceKey = 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->ParamCheck = true;
   IBInternalQuery->SQL->Text =
   " SELECT "
   " INVOICE_KEY "
   " FROM "
   "  INVOICES "
   " WHERE "
   "  INVOICE_NUMBER = :INVOICE_NUMBER";
   IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = InvoiceNumber;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
   {
      InvoiceKey = IBInternalQuery->FieldByName("INVOICE_KEY")->AsInteger;
   }
	return InvoiceKey;
}

void TInvoice::SetOrdersToInvoiceFormat(Database::TDBTransaction &DBTransaction,int InvoiceKey,int InvoiceTabKey,UnicodeString InvoiceOwner)
{
	if( !fEnabled )return;
	if( InvoiceKey == 0)return;
	if( InvoiceTabKey == 0)return;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"ORDERS "
		"SET "
			"TABLE_NUMBER = :TABLE_NUMBER, "
			"SEATNO = :SEATNO, "
			"TABLE_NAME = :TABLE_NAME, "
			"TAB_NAME = :TAB_NAME, "
			"TAB_TYPE = :TAB_TYPE "
		"WHERE "
			"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->Clear();
		IBInternalQuery->ParamByName("SEATNO")->Clear();
		IBInternalQuery->ParamByName("TABLE_NAME")->Clear();
		IBInternalQuery->ParamByName("TAB_NAME")->AsString = InvoiceOwner.SubString(1,31);
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabInvoice;
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = InvoiceTabKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	}
}

void TInvoice::SetInvoiceClosed(Database::TDBTransaction &DBTransaction,int inInvoiceKey)
{
	if( !fEnabled )return;
	if( inInvoiceKey == 0)return;
	try
   {
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
			"UPDATE "
            "INVOICES "
         "SET "
				"CLOSED = :CLOSED, "
				"CLOSED_TIME_STAMP = :CLOSED_TIME_STAMP "
         "WHERE "
            "INVOICE_KEY = :INVOICE_KEY";
         IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = inInvoiceKey;
         IBInternalQuery->ParamByName("CLOSED")->AsString = "T";
			IBInternalQuery->ParamByName("CLOSED_TIME_STAMP")->AsDateTime = Now();
			IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

UnicodeString TInvoice::GetNextInvoiceNumber(Database::TDBTransaction &DBTransaction)
{
   if( !fEnabled )return "";
	InvoiceNumber = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		InvoiceNumber = IntToStr(IBInternalQuery->Fields[0]->AsInteger);

	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return InvoiceNumber;
}

UnicodeString TInvoice::GetNextInvoiceNumber(Database::TDBTransaction &DBTransaction, int TypeOfSale)
{
   if( !fEnabled )return "";
	InvoiceNumber = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
        switch(TypeOfSale)
         {
           case 0:
		        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 1) FROM RDB$DATABASE";                
                break;
           case 1:
		        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERCOMP, 1) FROM RDB$DATABASE";
                break;
           case 2:
		        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERNC, 1) FROM RDB$DATABASE";
                break;
         }
		IBInternalQuery->ExecQuery();
        if(TypeOfSale == 1)
          {
		    InvoiceNumber = "Comp " + IntToStr(IBInternalQuery->Fields[0]->AsInteger);
          }
         else if(TypeOfSale == 2)
          {
		    InvoiceNumber = "NC " + IntToStr(IBInternalQuery->Fields[0]->AsInteger);
          }
         else
           {
            InvoiceNumber = IntToStr(IBInternalQuery->Fields[0]->AsInteger);
           }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return InvoiceNumber;
}

UnicodeString TInvoice::GetVoidInvoiceNumber(Database::TDBTransaction &DBTransaction)
{
   if( !fEnabled )return "";
	InvoiceNumber = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_VOIDINVOICENUMBER, 1) FROM RDB$DATABASE";

        IBInternalQuery->ExecQuery();
        InvoiceNumber = IntToStr(IBInternalQuery->Fields[0]->AsInteger);
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return InvoiceNumber;
}
