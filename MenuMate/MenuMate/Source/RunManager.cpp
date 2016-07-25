//---------------------------------------------------------------------------


#pragma hdrstop

#include "RunManager.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerRun *ManagerRun;

int TManagerRun::GetNew(Database::TDBTransaction &DBTransaction,int Filter)
{
   if( !fEnabled )return 0;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	int RetVal = 0;

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_RUNPROGRAMS, 1) FROM RDB$DATABASE";
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->Fields[0]->AsInteger;

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "INSERT INTO RUNPROGRAMS ( "
      "RUNPROGRAMS_KEY, "
      "APPEARANCE_ORDER, "
      "NAME, "
      "RUN_LINE) "
   "VALUES ("
      ":RUNPROGRAMS_KEY, "
      ":APPEARANCE_ORDER, "
      ":NAME, "
      ":RUN_LINE); ";
   IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = RetVal;
	IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = (GetCount(DBTransaction,Filter)+1);
   IBInternalQuery->ParamByName("NAME")->AsString = "";
   IBInternalQuery->ParamByName("RUN_LINE")->AsString = "";
   IBInternalQuery->ExecQuery();

   
   return RetVal;
}

int TManagerRun::GetCount(Database::TDBTransaction &DBTransaction,int Filter)
{
   if( !fEnabled )return 0;
   
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	int RetVal = 0;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT COUNT(RUNPROGRAMS_KEY) "
   "FROM "
   "RUNPROGRAMS";
	IBInternalQuery->ExecQuery();
	RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;
   
   return RetVal;
}

UnicodeString TManagerRun::GetContent(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return "";

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString RetVal = "";
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT RUN_LINE "
   "FROM "
   "RUNPROGRAMS "
   "WHERE "
   "RUNPROGRAMS_KEY = :RUNPROGRAMS_KEY";
   IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
   {
      RetVal = IBInternalQuery->FieldByName("RUN_LINE")->AsString;
   }
   
   return RetVal;
}

UnicodeString TManagerRun::GetTitle(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return "";
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString RetVal = "";
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT NAME "
   "FROM "
   "RUNPROGRAMS "
   "WHERE "
   "RUNPROGRAMS_KEY = :RUNPROGRAMS_KEY";
   IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
   {
      RetVal = IBInternalQuery->FieldByName("NAME")->AsString;
   }
   
   return RetVal;
}


void TManagerRun::GetListTitle(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   List->Add("Program Title");

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT RUNPROGRAMS_KEY,NAME "
   "FROM "
   "RUNPROGRAMS "
   "ORDER BY APPEARANCE_ORDER";
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount && List != NULL)
   {
      for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
      {
			int Index = List->Add(IBInternalQuery->FieldByName("NAME")->AsString);
         // Little bit of pointer abuse but we just want to store the int somewhere.
         List->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("RUNPROGRAMS_KEY")->AsInteger;
		}
   }
   
}

void TManagerRun::GetListContent(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   List->Add("Run Command");

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT RUNPROGRAMS_KEY,RUN_LINE "
   "FROM "
   "RUNPROGRAMS "
   "ORDER BY APPEARANCE_ORDER ";
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount && List != NULL)
   {
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
      {
         int Index = List->Add(IBInternalQuery->FieldByName("RUN_LINE")->AsString);
         // Little bit of pointer abuse but we just want to store the int somewhere.
         List->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("RUNPROGRAMS_KEY")->AsInteger;
      }
   }
   
}

void TManagerRun::SetContent(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Content)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "UPDATE RUNPROGRAMS "
         "SET "
         "RUN_LINE = :RUN_LINE "
         "WHERE "
         "RUNPROGRAMS_KEY	= :RUNPROGRAMS_KEY";
		IBInternalQuery->ParamByName("RUN_LINE")->AsString = Content;
      IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }
}

void TManagerRun::SetTitle(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Title)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "UPDATE RUNPROGRAMS "
         "SET "
         "NAME = :NAME "
         "WHERE "
         "RUNPROGRAMS_KEY	= :RUNPROGRAMS_KEY";
      IBInternalQuery->ParamByName("NAME")->AsString = Title;
      IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }
}

void TManagerRun::SetOrder(Database::TDBTransaction &DBTransaction,int Key,int AppearanceOrder)
{
   if( !fEnabled )return;
   
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "UPDATE RUNPROGRAMS "
         "SET "
         "APPEARANCE_ORDER = :APPEARANCE_ORDER "
         "WHERE "
         "RUNPROGRAMS_KEY	= :RUNPROGRAMS_KEY";
      IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = AppearanceOrder;
      IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }
}


int TManagerRun::Add(Database::TDBTransaction &DBTransaction,UnicodeString Title,UnicodeString Content,int Order,int Filter)
{
   if( !fEnabled )return 0;

	int RetVal = GetNew(DBTransaction,Filter);
	SetTitle(DBTransaction,RetVal,Title);
	SetContent(DBTransaction,RetVal,Content);
	SetOrder(DBTransaction,RetVal,GetCount(DBTransaction,Filter));

	return RetVal;
}

void TManagerRun::Delete(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   if ( Key != 0)
   {
		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "DELETE FROM RUNPROGRAMS "
         "WHERE "
         "RUNPROGRAMS_KEY	= :RUNPROGRAMS_KEY";
      IBInternalQuery->ParamByName("RUNPROGRAMS_KEY")->AsInteger = Key;
		IBInternalQuery->ExecQuery();
   }
}

bool TManagerRun::RunProgram(UnicodeString ProgramLine)
{
	STARTUPINFOW stINFO;
	PROCESS_INFORMATION ProcInfo;
	ZeroMemory(&stINFO,sizeof(stINFO));
   ZeroMemory(&ProcInfo,sizeof(ProcInfo));
   stINFO.cb = sizeof(stINFO);
   bool retval = false;
	retval = CreateProcessW( ProgramLine.w_str(),
									 UnicodeString("").w_str(),
									 NULL,
									 NULL,
									 false,
                            NORMAL_PRIORITY_CLASS & DETACHED_PROCESS & CREATE_NEW_PROCESS_GROUP & CREATE_DEFAULT_ERROR_MODE,
                            NULL,
                            NULL,
                            &stINFO,
									 &ProcInfo);
	if(!retval)
	{
		MessageBox(SysErrorMessage(GetLastError()), _T("Error"), MB_OK + MB_ICONWARNING);
	}
   return retval;
}



void TManagerRun::LoadMessages(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid, int Filter)
{
	if(GetCount(DBTransaction,Filter) < 2)
   {
	   StringGrid->RowCount = 2;
   }
   else
   {
		StringGrid->RowCount = GetCount(DBTransaction,Filter) + 1;
   }
   StringGrid->Cols[0]->Clear();
	GetListTitle(DBTransaction,StringGrid->Cols[0],Filter);
   StringGrid->Cols[1]->Clear();
   GetListContent(DBTransaction,StringGrid->Cols[1],Filter);
   StringGrid->FixedRows = 1;
}

TManagerRun::TManagerRun()
{
   SetEnabled(true);
}
