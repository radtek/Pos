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
    try
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
   catch(Exception &E)
   {	
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::AddDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination)
{
    try
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
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::UpdateDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "UPDATE CASHDENOMINATIONS SET DENOMINATION = :DENOMINATION,  TITLE = :TITLE "
                                      " WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY ";
        IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = inDenomination.Key;
        IBInternalQuery->ParamByName("TITLE")->AsString = inDenomination.Title;
        IBInternalQuery->ParamByName("DENOMINATION")->AsCurrency = inDenomination.DenominationValue;
        IBInternalQuery->ExecQuery();
   }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::LoadDenominations(Database::TDBTransaction &DBTransaction,std::vector<TDenomination> &inDenominations)
{
    try
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
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
}
//--------------------------------------------------------------------------
void TDBDenominations::DeleteDenominations(Database::TDBTransaction &DBTransaction,int key)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "DELETE FROM CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY ";
        IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
        IBInternalQuery->ExecQuery();
    }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
}
//--------------------------------------------------------------------------
Currency TDBDenominations::GetDenominationValue(Database::TDBTransaction &DBTransaction,int key)
{
   Currency retVal = 0;
   try
   {
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "SELECT DENOMINATION FROM  CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY";
       IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
       IBInternalQuery->ExecQuery();
       if(!IBInternalQuery->Eof)
       {
         retVal = IBInternalQuery->FieldByName("DENOMINATION")->AsCurrency;
       }
   }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
   return retVal;
}
//--------------------------------------------------------------------------
AnsiString TDBDenominations::GetDenominationTitle(Database::TDBTransaction &DBTransaction,int key)
{
   AnsiString retVal = "";
   try
   {
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "SELECT TITLE FROM  CASHDENOMINATIONS WHERE CASHDENOMINATION_KEY = :CASHDENOMINATION_KEY";
       IBInternalQuery->ParamByName("CASHDENOMINATION_KEY")->AsInteger = key;
       IBInternalQuery->ExecQuery();
       if(!IBInternalQuery->Eof)
       {
         retVal = IBInternalQuery->FieldByName("TITLE")->AsString;
       }
   }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
   return retVal;
}
//--------------------------------------------------------------------------
int TDBDenominations::GetDenominationKey(Database::TDBTransaction &DBTransaction)
{
    int retVal = 0;
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT GEN_ID(GEN_CASHDENOMINATION, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();
        retVal = IBInternalQuery->Fields[0]->AsInteger;
    }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
    return retVal;
}
//--------------------------------------------------------------------------
bool TDBDenominations::IsDenominationExist(Database::TDBTransaction &DBTransaction,int key,AnsiString inTitle)
{
   bool retVal = false;
   try
   {
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
   }
   catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
   }
   return retVal;
}
//--------------------------------------------------------------------------
void TDBDenominations::SaveZedDenominations(Database::TDBTransaction &DBTransaction,
                                            int zedKey,
                                            UnicodeString inTerminalName,
                                            UnicodeString inDenominationTitle,
                                            Currency inDenominationValue,
                                            int  inDenominationQty)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "INSERT INTO ZED_CASHDENOMINATIONS ("
                                     "Z_KEY, "
                                     "TERMINAL_NAME, "
                                     "DENOMINATION_TITLE, "
                                     "DENOMINATION_VALUE, "
                                     "DENOMINATION_QUANTITY "
                                     ")VALUES ( "
                                     ":Z_KEY, "
                                     ":TERMINAL_NAME, "
                                     ":DENOMINATION_TITLE, "
                                     ":DENOMINATION_VALUE, "
                                     ":DENOMINATION_QUANTITY) ";
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = zedKey;
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = inTerminalName;
        IBInternalQuery->ParamByName("DENOMINATION_TITLE")->AsString = inDenominationTitle;
        IBInternalQuery->ParamByName("DENOMINATION_VALUE")->AsCurrency = inDenominationValue;
        IBInternalQuery->ParamByName("DENOMINATION_QUANTITY")->AsInteger = inDenominationQty;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception &E)
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}

