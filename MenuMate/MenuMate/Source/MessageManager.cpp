//---------------------------------------------------------------------------


#pragma hdrstop

#include "MessageManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerMessage *ManagerMessage;

//---------------------------------------------------------------------------

int TManagerMessage::GetNew(Database::TDBTransaction &DBTransaction,int Filter)
{
	if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	int RetVal = 0;

   IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MESSAGES, 1) FROM RDB$DATABASE";
   IBInternalQuery->ExecQuery();
	RetVal = IBInternalQuery->Fields[0]->AsInteger;

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "INSERT INTO MESSAGES ("
      "MESSAGES_KEY,"
		"APPERANCE_ORDER,"
      "TITLE,"
      "MESSAGE_TEXT,"
      "MESSAGE_TYPE) "
   "VALUES ("
      ":MESSAGES_KEY,"
      ":APPERANCE_ORDER,"
      ":TITLE,"
      ":MESSAGE_TEXT,"
      ":MESSAGE_TYPE);";
   IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = RetVal;
	IBInternalQuery->ParamByName("APPERANCE_ORDER")->AsInteger = GetCount(DBTransaction,Filter)+1;
   IBInternalQuery->ParamByName("TITLE")->AsString = "";
   IBInternalQuery->ParamByName("MESSAGE_TEXT")->AsString = "";
   IBInternalQuery->ParamByName("MESSAGE_TYPE")->AsInteger = Filter;
	IBInternalQuery->ExecQuery();

   
   return RetVal;
}

int TManagerMessage::GetCount(Database::TDBTransaction &DBTransaction,int Filter)
{
   if( !fEnabled )return 0;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	int RetVal = 0;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT COUNT(MESSAGES_KEY) "
   "FROM "
   "MESSAGES "
   "WHERE MESSAGE_TYPE = :MESSAGE_TYPE";
	IBInternalQuery->ParamByName("MESSAGE_TYPE")->AsInteger = Filter;
   IBInternalQuery->ExecQuery();
	RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;
   
   return RetVal;
}

UnicodeString TManagerMessage::GetContent(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return "";
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString RetVal = "";
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT MESSAGE_TEXT "
   "FROM "
   "MESSAGES "
   "WHERE "
   "MESSAGES_KEY = :MESSAGES_KEY";
   IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
   {
      RetVal = IBInternalQuery->FieldByName("MESSAGE_TEXT")->AsString;
   }
   
   return RetVal;
}

UnicodeString TManagerMessage::GetTitle(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return "";
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString RetVal = "";
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT TITLE "
   "FROM "
   "MESSAGES "
   "WHERE "
   "MESSAGES_KEY = :MESSAGES_KEY";
   IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
   {
      RetVal = IBInternalQuery->FieldByName("TITLE")->AsString;
   }
   
   return RetVal;
}


void TManagerMessage::GetListTitle(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   List->Add("Button Title");

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT MESSAGES_KEY,TITLE "
   "FROM "
   "MESSAGES "
   "WHERE MESSAGE_TYPE = :MESSAGE_TYPE "
   "ORDER BY APPERANCE_ORDER";
   IBInternalQuery->ParamByName("MESSAGE_TYPE")->AsInteger = Filter;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount && List != NULL)
   {
      for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
      {
		 int Index = List->Add(IBInternalQuery->FieldByName("TITLE")->AsString);
         // Little bit of pointer abuse but we just want to store the int somewhere.
         List->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("MESSAGES_KEY")->AsInteger;
		}
   }
   
}

void TManagerMessage::GetListContent(Database::TDBTransaction &DBTransaction,TStrings *List,int Filter)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   List->Add("Message");

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT MESSAGES_KEY,MESSAGE_TEXT "
   "FROM "
   "MESSAGES "
   "WHERE MESSAGE_TYPE = :MESSAGE_TYPE "
   "ORDER BY APPERANCE_ORDER";
   IBInternalQuery->ParamByName("MESSAGE_TYPE")->AsInteger = Filter;   
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount && List != NULL)
   {
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
      {
         int Index = List->Add(IBInternalQuery->FieldByName("MESSAGE_TEXT")->AsString);
         // Little bit of pointer abuse but we just want to store the int somewhere.
         List->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("MESSAGES_KEY")->AsInteger;
      }
   }
   
}

void TManagerMessage::SetContent(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Content)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE MESSAGES "
      "SET "
      "MESSAGE_TEXT = :MESSAGE_TEXT "
      "WHERE "
      "MESSAGES_KEY	= :MESSAGES_KEY";
   	IBInternalQuery->ParamByName("MESSAGE_TEXT")->AsString = Content;
      IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }

   
}

void TManagerMessage::SetTitle(Database::TDBTransaction &DBTransaction,int Key,UnicodeString Title)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
		"UPDATE MESSAGES "
      "SET "
      "TITLE = :TITLE "
      "WHERE "
      "MESSAGES_KEY	= :MESSAGES_KEY";
      IBInternalQuery->ParamByName("TITLE")->AsString = Title;
      IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }
}

void TManagerMessage::SetOrder(Database::TDBTransaction &DBTransaction,int Key,int AppearanceOrder)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE MESSAGES "
      "SET "
      "APPERANCE_ORDER = :APPERANCE_ORDER "
      "WHERE "
      "MESSAGES_KEY	= :MESSAGES_KEY";
      IBInternalQuery->ParamByName("APPERANCE_ORDER")->AsInteger = AppearanceOrder;
      IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }

   
}

void TManagerMessage::SetFilter(Database::TDBTransaction &DBTransaction,int Key,int Filter)
{
   if( !fEnabled )return;
   
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE MESSAGES "
      "SET "
      "MESSAGE_TYPE = :MESSAGE_TYPE "
      "WHERE "
      "MESSAGES_KEY	= :MESSAGES_KEY";
      IBInternalQuery->ParamByName("MESSAGE_TYPE")->AsInteger = Filter;
      IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
      IBInternalQuery->ExecQuery();
   }

   
}

int TManagerMessage::Add(Database::TDBTransaction &DBTransaction,UnicodeString Title,UnicodeString Content,int Order,int Filter)
{
   if( !fEnabled )return 0;
   
	int RetVal = GetNew(DBTransaction,Filter);
	SetTitle(DBTransaction,RetVal,Title);
	SetContent(DBTransaction,RetVal,Content);
	SetOrder(DBTransaction,RetVal,GetCount(DBTransaction,Filter));
	SetFilter(DBTransaction,RetVal,Filter);

   return RetVal;
}


void TManagerMessage::Delete(Database::TDBTransaction &DBTransaction,int Key)
{
   if( !fEnabled )return;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   if ( Key != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "DELETE FROM MESSAGES "
	  "WHERE "
      "MESSAGES_KEY	= :MESSAGES_KEY";
		IBInternalQuery->ParamByName("MESSAGES_KEY")->AsInteger = Key;
		IBInternalQuery->ExecQuery();
   }
}

void TManagerMessage::LoadMessages(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid, int Filter)
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


}

TManagerMessage::TManagerMessage()
{
	Enabled = true;
}
