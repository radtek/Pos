//---------------------------------------------------------------------------


#pragma hdrstop
#include "DBDenominations.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDBDenominations::TDBDenominations()
{
}
//---------------------------------------------------------------------------
TDBDenominations::~TDBDenominations()
{
}
//--------------------------------------------------------------------------
void TDBDenominations::SaveDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination)
{
   if(inDenomination.Key == 0)
   {
      TDBDenominations::AddDenomination(DBTransaction,inDenomination);
   }
   else
   {
     TDBDenominations::UpdateDenomination(DBTransaction,inDenomination);
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::AddDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination)
{
    inDenomination.Key = TDBDenominations::GetDenominationKey(DBTransaction);
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "INSERT INTO CASHDENOMINATIONS (CASHDENOMINATION_KEY, TITLE, DENOMINATION) "
                                 "VALUES ( "
                                 ":CASHDENOMINATION_KEY, "
                                 ":TITLE, "
                                 ":DENOMINATION)";
    IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = inDenomination.Key;
    IBInternalQuery->ParamByName("TITLE")->AsString = inDenomination.Title;
    IBInternalQuery->ParamByName("DENOMINATION")->AsCurrency = inDenomination.DenominationValue;
    IBInternalQuery->ExecQuery();
}
//--------------------------------------------------------------------------
void TDBDenominations::UpdateDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "UPDATE CASHDENOMINATIONS SET DENOMINATION = :DENOMINATION,  TITLE = :TITLE "
                                  " WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY ";
    IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = inDenomination.Key;
    IBInternalQuery->ParamByName("TITLE")->AsString = inDenomination.Title;
    IBInternalQuery->ParamByName("DENOMINATION")->AsCurrency = inDenomination.DenominationValue;
    IBInternalQuery->ExecQuery();
}
//--------------------------------------------------------------------------
void TDBDenominations::LoadDenominations(Database::TDBTransaction &DBTransaction,std::vector<TDenomination> &inDenominations)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT * FROM  CASHDENOMINATIONS ORDER BY CASHDENOMINATION_KEY";
   IBInternalQuery->ExecQuery();
   for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
   {
      TDenomination denomination;
      denomination.Key =  IBInternalQuery->FieldByName("CASHDENOMINATION_KEY")->AsInteger;
      denomination.Title = IBInternalQuery->FieldByName("TITLE")->AsString;
      denomination.DenominationValue = IBInternalQuery->FieldByName("DENOMINATION")->AsCurrency;
      inDenominations.push_back(denomination);
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::DeleteDenominations(Database::TDBTransaction &DBTransaction,int key)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "DELETE FROM CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY ";
    IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
    IBInternalQuery->ExecQuery();
}
//--------------------------------------------------------------------------
Currency TDBDenominations::GetDenominationValue(Database::TDBTransaction &DBTransaction,int key)
{
   Currency retVal = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT DENOMINATION FROM  CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY";
   IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
   {
     retVal = IBInternalQuery->FieldByName("DENOMINATION")->AsCurrency;
   }
   return retVal;
}
//--------------------------------------------------------------------------
AnsiString TDBDenominations::GetDenominationTitle(Database::TDBTransaction &DBTransaction,int key)
{
   AnsiString retVal = "";
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT TITLE FROM  CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY";
   IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
   {
     retVal = IBInternalQuery->FieldByName("TITLE")->AsString;
   }
   return retVal;
}
//--------------------------------------------------------------------------
int TDBDenominations::GetDenominationKey(Database::TDBTransaction &DBTransaction)
{
    int retVal = 0;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT GEN_ID(GEN_CASHDENOMINATION, 1) FROM RDB$DATABASE";
    IBInternalQuery->ExecQuery();
    retVal = IBInternalQuery->Fields[0]->AsInteger;
    return retVal;
}

bool TDBDenominations::IsDenominationExist(Database::TDBTransaction &DBTransaction,int key,AnsiString inTitle)
{
   bool retVal = false;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT CASHDENOMINATION_KEY FROM  CASHDENOMINATIONS WHERE UPPER(TITLE) = :TITLE ";
   IBInternalQuery->ParamByName("TITLE")->AsString = inTitle.UpperCase();
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
   {
     if(key != IBInternalQuery->FieldByName("CASHDENOMINATION_KEY")->AsInteger)
       retVal = true;
     else
       retVal = false;
   }
   return retVal;
}


