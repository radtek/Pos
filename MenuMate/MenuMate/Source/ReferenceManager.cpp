//---------------------------------------------------------------------------
#pragma hdrstop
#include "ReferenceManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TManagerReference::TManagerReference()
{
}

TManagerReference::~TManagerReference()
{
}

void TManagerReference::Initialise(Database::TDBTransaction &DBTransaction)
{
   try
   {
      // Create Invoice Ref.
      if(GetReferenceByType(DBTransaction,REFTYPE_INVOICE).Key == 0)
      {
         TReference Reference;
         Reference.Key = 0;
         Reference.Type = REFTYPE_INVOICE;
         Reference.Name = "Invoice No.";
         SetReference(DBTransaction,0,Reference);
      }

      // Create Invoice Ref.
      if(GetReferenceByType(DBTransaction,REFTYPE_POCKETVOUCHER_CODE).Key == 0)
      {
         TReference Reference;
         Reference.Key = 0;
         Reference.Type = REFTYPE_POCKETVOUCHER_CODE;
         Reference.Name = "Pocket Voucher Code.";
         SetReference(DBTransaction,0,Reference);
      }
      // Create Invoice Ref.
      if(GetReferenceByType(DBTransaction,REFTYPE_POCKETVOUCHER_PVTRANS).Key == 0)
      {
         TReference Reference;
         Reference.Key = 0;
         Reference.Type = REFTYPE_POCKETVOUCHER_PVTRANS;
         Reference.Name = "Pocket V Trans";
         SetReference(DBTransaction,0,Reference);
      }

      // Create Invoice Ref.
      if(GetReferenceByType(DBTransaction,REFTYPE_POCKETVOUCHER_MMTRANS).Key == 0)
      {
         TReference Reference;
         Reference.Key = 0;
         Reference.Type = REFTYPE_POCKETVOUCHER_MMTRANS;
         Reference.Name = "MenuMate PV Trans";
         SetReference(DBTransaction,0,Reference);
      }

	   // Create General Ledger Ref.
	   if(GetReferenceByType(DBTransaction,REFTYPE_GENERAL_LEDGER).Key == 0)
	   {
		  TReference Reference;
		  Reference.Key = 0;
		  Reference.Type = REFTYPE_GENERAL_LEDGER;
		  Reference.Name = "MenuMate General Ledger Trans";
		  SetReference(DBTransaction,0,Reference);
	   }

   }
	catch(Exception &E)
	{
   }
}


void TManagerReference::GetReferences(Database::TDBTransaction &DBTransaction, TStringList *List)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT REFS_KEY,REFTYPE,NAME,CODE"
	" FROM"
	"  REFS"
	" ORDER BY NAME";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount && List != NULL)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			List->AddObject(IBInternalQuery->FieldByName("NAME")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("REFS_KEY")->AsInteger);
		}
	}
}

void TManagerReference::GetReferences(Database::TDBTransaction &DBTransaction, TStringList *List, REFTYPE TypeFilter)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT REFS_KEY,REFTYPE,NAME,CODE"
	" FROM"
	"  REFS"
	" WHERE REFTYPE = :REFTYPE"
	" ORDER BY NAME";
	IBInternalQuery->ParamByName("REFTYPE")->AsInteger = TypeFilter;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount && List != NULL)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			List->AddObject(IBInternalQuery->FieldByName("NAME")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("REFS_KEY")->AsInteger);
		}
	}
}


int TManagerReference::GetCount(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT COUNT(REFS_KEY)"
	" FROM"
	"  REFS";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;
	}
	return RetVal;
}

TReference TManagerReference::GetReference(Database::TDBTransaction &DBTransaction, int Key)
{
	TReference Ref;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT REFS_KEY,REFTYPE,NAME,CODE"
	" FROM"
	"  REFS"
	" WHERE "
	" REFS_KEY = :REFS_KEY"
	" ORDER BY REFTYPE";
	IBInternalQuery->ParamByName("REFS_KEY")->AsInteger = Key;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		Ref.Key = IBInternalQuery->FieldByName("REFS_KEY")->AsInteger;
		Ref.Type = IBInternalQuery->FieldByName("REFTYPE")->AsInteger;
		Ref.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		Ref.Code = IBInternalQuery->FieldByName("CODE")->AsString;
	}
	return Ref;
}

TReference TManagerReference::GetReferenceByType(Database::TDBTransaction &DBTransaction, REFTYPE Type)
{
	TReference Ref;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT REFS_KEY,REFTYPE,NAME,CODE"
	" FROM"
	"  REFS"
	" WHERE"
	" REFTYPE = :REFTYPE"
	" ORDER BY REFTYPE";
	IBInternalQuery->ParamByName("REFTYPE")->AsInteger = Type;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		Ref.Key = IBInternalQuery->FieldByName("REFS_KEY")->AsInteger;
		Ref.Type = IBInternalQuery->FieldByName("REFTYPE")->AsInteger;
		Ref.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		Ref.Code = IBInternalQuery->FieldByName("CODE")->AsString;
	}
	return Ref;
}

void TManagerReference::GetReferences(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid)
{
	if(GetCount(DBTransaction) < 2)
	{
		StringGrid->RowCount = 2;
	}
	else
	{
		StringGrid->RowCount = GetCount(DBTransaction) + 1;
	}
	StringGrid->Cols[0]->Clear();
	StringGrid->Cols[1]->Clear();

	StringGrid->Cols[0]->Add("Reference Name");
	StringGrid->Cols[1]->Add("Reference Code");

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT REFS_KEY,NAME,CODE"
	" FROM"
	"  REFS"
	" ORDER BY REFTYPE";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			 StringGrid->Cols[0]->AddObject(IBInternalQuery->FieldByName("NAME")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("REFS_KEY")->AsInteger);
			 StringGrid->Cols[1]->AddObject(IBInternalQuery->FieldByName("CODE")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("REFS_KEY")->AsInteger);
		}
	}
	StringGrid->FixedRows = 1;
}

void TManagerReference::SetReference(Database::TDBTransaction &DBTransaction,long Key,TReference Reference)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(Key == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_REFS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		Key = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO REFS ("
			"REFS_KEY,"
			"REFTYPE,"
			"NAME,"
			"CODE) "
		"VALUES ("
			":REFS_KEY,"
			":REFTYPE,"
			":NAME,"
			":CODE);";
		IBInternalQuery->ParamByName("REFS_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("REFTYPE")->AsInteger = Reference.Type;
		IBInternalQuery->ParamByName("NAME")->AsString = Reference.Name.SubString(1, 20);
		IBInternalQuery->ParamByName("CODE")->AsString = Reference.Code.SubString(1, 20);
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" REFS"
		" SET"
		" REFTYPE = :REFTYPE,"
		" NAME = :NAME"
		" CODE = :CODE"
		" WHERE"
		" REFS_KEY = :REFS_KEY";
		IBInternalQuery->ParamByName("REFS_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("REFTYPE")->AsInteger = Reference.Type;
		IBInternalQuery->ParamByName("NAME")->AsString = Reference.Name.SubString(1, 20);
		IBInternalQuery->ParamByName("CODE")->AsString = Reference.Code.SubString(1, 20);
		IBInternalQuery->ExecQuery();
	}
}

void TManagerReference::Delete(Database::TDBTransaction &DBTransaction,int Key)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(Key != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM REFS"
		" WHERE REFS_KEY = :REFS_KEY";
		IBInternalQuery->ParamByName("REFS_KEY")->AsInteger = Key;
		IBInternalQuery->ExecQuery();
	}
}

AnsiString TManagerReference::BuildReference(int SiteID,AnsiString TerminalName,REFTYPE TYPE)
{
   AnsiString PreOp;
	switch (TYPE) {
   	case REFTYPE_EFTPOS :
      	PreOp = "EFT";
      break;
      case REFTYPE_PMS :
      	PreOp = "PMS";
      break;
      case REFTYPE_INVOICE :
      	PreOp = "INV";
      break;
      case REFTYPE_POCKETVOUCHER_CODE :
      case REFTYPE_POCKETVOUCHER_PVTRANS :
      case REFTYPE_POCKETVOUCHER_MMTRANS :
		PreOp = "PV";
	   break;
	   case REFTYPE_GENERAL_LEDGER :
		PreOp = "GL";
		break;
      case REFTYPE_USER :
   default:
      PreOp = "USR";
   }
   return PreOp + IntToStr(SiteID) + TDateTime::CurrentDateTime().FormatString("-yyyymmddhhmmss");
}
