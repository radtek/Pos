//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DBTables.h"
//#include "NetMessageSeatName.h"
//#include "ManagerNet.h"
#include "MMLogging.h"
#include "DBTab.h"
#include "GlobalSettings.h"
#include "ManagerPatron.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

int TDBTables::GetOrCreateTable(Database::TDBTransaction &DBTransaction, int inTableNo)
{
	if(!Valid(inTableNo)) return 0;
	int RetVal = 0;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      bool CreateTable = true;
      if(inTableNo != 0)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         " SELECT "
         " TABLE_KEY "
         " FROM "
			"  TABLES "
         " WHERE "
         "  TABLE_NUMBER = :TABLE_NUMBER";
			IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
			IBInternalQuery->ExecQuery();
			if(IBInternalQuery->RecordCount)
         {
            RetVal = IBInternalQuery->FieldByName("TABLE_KEY")->AsInteger;
            CreateTable = false;
         }
		}

      if(CreateTable)
      {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TABLES, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();

        RetVal = IBInternalQuery->Fields[0]->AsInteger;

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                    "INSERT INTO TABLES ("
                                            "TABLE_KEY,"
                                            "TABLE_NUMBER,"
                                            "TABLE_NAME,"
                                            "PARTY_NAME,"
                                            "CIRCLE,"
                                            "TEMPORARY) "
                                    "VALUES ("
                                            ":TABLE_KEY,"
                                            ":TABLE_NUMBER,"
                                            ":TABLE_NAME,"
                                            ":PARTY_NAME,"
                                            ":CIRCLE,"
                                            ":TEMPORARY);";
        IBInternalQuery->ParamByName("TABLE_KEY")->AsInteger = RetVal;
        IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
        IBInternalQuery->ParamByName("TABLE_NAME")->AsString = "";
        IBInternalQuery->ParamByName("PARTY_NAME")->AsString = "";
        IBInternalQuery->ParamByName("CIRCLE")->AsString = "F";
        IBInternalQuery->ParamByName("TEMPORARY")->AsString = "F";
        IBInternalQuery->ExecQuery();
      }

	   
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	}
	return RetVal;
};

int TDBTables::GetOrCreateSeat(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
	if(!Valid(inTableNo,inSeatNo)) return 0;

	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int TableKey = GetOrCreateTable(DBTransaction,inTableNo);

      bool CreateSeat = true;
      if(inSeatNo != 0)
      {
			IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         " SELECT "
         " SEAT.SEAT_KEY "
         " FROM "
         " SEAT INNER JOIN TABLES ON SEAT.TABLE_KEY = TABLES.TABLE_KEY "
         " WHERE "
         "  TABLES.TABLE_KEY = :TABLE_KEY AND SEAT.SEATNO = :SEATNO";
         IBInternalQuery->ParamByName("TABLE_KEY")->AsInteger = TableKey;
         IBInternalQuery->ParamByName("SEATNO")->AsInteger = inSeatNo;
			IBInternalQuery->ExecQuery();
			if(IBInternalQuery->RecordCount)
         {
            RetVal = IBInternalQuery->FieldByName("SEAT_KEY")->AsInteger;
            CreateSeat = false;
         }
      }

      if(CreateSeat)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SEAT, 1) FROM RDB$DATABASE";
         IBInternalQuery->ExecQuery();
			RetVal = IBInternalQuery->Fields[0]->AsInteger;

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "INSERT INTO SEAT ("
            "SEAT_KEY,"
            "TABLE_KEY,"
            "SEATNO) "
         "VALUES ("
            ":SEAT_KEY,"
            ":TABLE_KEY,"
            ":SEATNO);";
         IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = RetVal;
         IBInternalQuery->ParamByName("TABLE_KEY")->AsInteger = TableKey;
         IBInternalQuery->ParamByName("SEATNO")->AsInteger = inSeatNo;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
	}
   return RetVal;
}

//---------------------------------------------------------------------------
// Creates the table if necessary and sets its name.
//---------------------------------------------------------------------------
void TDBTables::SetTableName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString TableName)
{
   if(!Valid(inTableNo)) return;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		int TableKey = GetOrCreateTable(DBTransaction,inTableNo);

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " UPDATE TABLES "
      " SET TABLE_NAME = :TABLE_NAME "
      " WHERE "
      " TABLE_KEY = :TABLE_KEY ;";
      IBInternalQuery->ParamByName("TABLE_KEY")->AsInteger = TableKey;
		IBInternalQuery->ParamByName("TABLE_NAME")->AsString = TableName;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
};

void TDBTables::SetSeatTabToNull(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "UPDATE SEAT SET "
         "TAB_KEY = NULL "
         "WHERE "
         "TAB_KEY = :TAB_KEY";
      IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
      IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTables::SetSeatTab(Database::TDBTransaction &DBTransaction,int inSeatKey,int inTabKey)
{
	if(inTabKey == 0) return;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " UPDATE SEAT "
      " SET TAB_KEY = :TAB_KEY "
		" WHERE "
		" SEAT_KEY = :SEAT_KEY ;";
		IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = inSeatKey;
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
};

void TDBTables::ClearSeatsName(Database::TDBTransaction &DBTransaction, int TableNo)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE TAB SET "
				"TAB_NAME = '' "
			"WHERE "
				"TAB.TAB_KEY IN "
            " ( "
            "     SELECT SEAT.TAB_KEY "
            "     FROM "
            "     TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
   			"     WHERE "
				"     TABLES.TABLE_NUMBER = :TABLE_NUMBER "
            " ) ";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTables::SetSeatName(Database::TDBTransaction &DBTransaction, UnicodeString Name, int TableNo, int SeatNumber)
{
	try
	{
		if (Valid(TableNo) && Valid(SeatNumber))
		{
			int SeatKey = GetOrCreateSeat(DBTransaction,TableNo,SeatNumber);
			int TabKey = GetTabKey(DBTransaction,SeatKey);
			TDBTab::SetTabName(DBTransaction,TabKey,Name);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
};


UnicodeString TDBTables::GetTableName(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	UnicodeString RetVal = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"TABLE_NAME "
			"FROM "
				"TABLES "
			"WHERE "
				"TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount && IBInternalQuery->FieldByName("TABLE_NAME")->AsString != "")
      {
         RetVal	= IBInternalQuery->FieldByName("TABLE_NAME")->AsString;
      }
      else
      {
      	RetVal = "Table #" + IntToStr(inTableNo);
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
};

void TDBTables::SetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString PartyName)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		GetOrCreateTable(DBTransaction,inTableNo);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE TABLES SET "
			"PARTY_NAME = :PARTY_NAME "
			"WHERE "
         "TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ParamByName("PARTY_NAME")->AsString = PartyName;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

UnicodeString TDBTables::GetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	UnicodeString RetVal = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"PARTY_NAME "
			"FROM "
				"TABLES "
			"WHERE "
				"TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
      }
      IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return RetVal;
}

void TDBTables::GetPartyNames(Database::TDBTransaction &DBTransaction, std::map<int,UnicodeString> &PartyNames)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        " SELECT "
        " t.\"TABLE_NUMBER\", t.\"PARTY_NAME\" "
        " FROM "
        "  \"TABLES\" t "
        " WHERE "
        " t.\"PARTY_NAME\" IS NOT NULL ";
        IBInternalQuery->ExecQuery();
        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int TableNo = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
            AnsiString TableCaption;
            if (IBInternalQuery->FieldByName("PARTY_NAME")->AsString != "")
            {
                PartyNames[TableNo] = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
            }
        }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTables::GetTableStatus(Database::TDBTransaction &DBTransaction, std::map<int,UnicodeString> &Status)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "Select "
            "Max(Tables.TABLE_NUMBER) TABLE_NUMBER, "
            "Max(Orders.TIME_STAMP) TIME_STAMP "
            "From "
            "((Tables inner join Seat on "
            "Tables.Table_Key = Seat.Table_Key) inner join Tab on "
            "Seat.Tab_Key = Tab.Tab_Key) inner join Orders on "
            "Tab.Tab_Key = Orders.Tab_Key "
            "Group By "
            "Tables.TABLE_NUMBER "
            "Order By "
            "Tables.TABLE_NUMBER ";
        IBInternalQuery->ExecQuery();
        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int TableNo = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
            if(TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus)
            {
               int tableStatus =  GetTableBillingStatus(DBTransaction,TableNo);
               switch(tableStatus)
               {
                //prelim
                case ePrelim:
                   Status[TableNo] = "PR";
                break;
                //call away
                case eCallAwayStatus:
                   Status[TableNo] = "CW";
                break;
                //split and partial
                case ePartialSplit:
                   Status[TableNo] = "PSP";
                break;
                default:
                  Status[TableNo] = "";
                  break;
               }
            }
            else
            {
             Status[TableNo] = "";
            }
        }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

UnicodeString TDBTables::GetPartyNameShort(Database::TDBTransaction &DBTransaction,int inTableNo)
{
   UnicodeString RetVal = "";
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"PARTY_NAME "
			"FROM "
				"TABLES "
			"WHERE "
				"TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

void TDBTables::GetSeats(Database::TDBTransaction &DBTransaction,TStringList * TabList, int TableNumber)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT DISTINCT "
				"SEAT.SEATNO SEAT_NUMBER, TAB.TAB_NAME NAME, TAB.TAB_KEY TAB_KEY "
			"FROM "
				"TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
				"INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
				"INNER JOIN ORDERS ON TAB.TAB_KEY = ORDERS.TAB_KEY "
			"WHERE "
				"TABLES.TABLE_NUMBER = :TABLE_NUMBER "
			"ORDER BY "
				"SEAT.SEATNO";

		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
		IBInternalQuery->ExecQuery();
      while (!IBInternalQuery->Eof)
      {
			int Index = 0;
			if (IBInternalQuery->FieldByName("NAME")->AsString == "")
			{
				Index = TabList->Add(TGlobalSettings::Instance().SeatLabel + " " + IBInternalQuery->FieldByName("SEAT_NUMBER")->AsString);
			}
			else
			{
				Index = TabList->Add(IBInternalQuery->FieldByName("SEAT_NUMBER")->AsString + "." + IBInternalQuery->FieldByName("NAME")->AsString);
			}
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

bool TDBTables::Valid(int inTableNo)
{
	bool RetVal = false;
   if(inTableNo > 0)
   {
		RetVal = true;
   }
   return RetVal;
}

bool TDBTables::Valid(int inTableNo, int inSeatNo)
{
	bool RetVal = false;
	if(Valid(inTableNo) && Valid(inSeatNo))
   {
		RetVal = true;
   }
   return RetVal;
}

int TDBTables::GetTabKey(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
   int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" 	a.TAB_KEY "
		" FROM "
		"  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
		"  INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
		" WHERE "
		"  t.TABLE_NUMBER = :TABLE_NUMBER AND "
		"  s.SEATNO = :SEATNO ";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ParamByName("SEATNO")->AsInteger = inSeatNo;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
	return RetVal;
}

int TDBTables::GetTabKey(Database::TDBTransaction &DBTransaction,int inSeatKey)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT TAB_KEY "
			"FROM "
				"SEAT "
			"WHERE "
				"SEAT_KEY = :SEAT_KEY ";
		IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = inSeatKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			if(!TDBTab::GetTabExists(DBTransaction,RetVal))
			{
				RetVal = TDBTab::GetOrCreateTab(DBTransaction,0);
				SetSeatTab(DBTransaction,inSeatKey,RetVal);
				TDBTab::SetTabType(DBTransaction,RetVal, TabTableSeat);
				UnicodeString TabName = GetNameMinor(DBTransaction,RetVal);
				TDBTab::SetTabName(DBTransaction,RetVal,TabName);
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

void TDBTables::GetTabKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedTabs)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT "
      " 	a.TAB_KEY "
      " FROM "
      "  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
		"  INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
      " WHERE "
      "  t.TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();
      for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
      {
			SelectedTabs.insert(IBInternalQuery->FieldByName("TAB_KEY")->AsInteger);
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}


void TDBTables::GetOrderKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedOrders)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" 	o.ORDER_KEY "
		" FROM "
		"  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
		"  INNER JOIN ORDERS o ON s.TAB_KEY = o.TAB_KEY "
		" WHERE "
		"  t.TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			SelectedOrders.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

void TDBTables::GetTabKeysWithOrders(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> *SelectedTabs)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" 	a.TAB_KEY "
		" FROM "
		"  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
		"  INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
		"  INNER JOIN ORDERS o ON a.TAB_KEY = o.TAB_KEY "
		" WHERE "
		"  t.TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			SelectedTabs->insert(IBInternalQuery->FieldByName("TAB_KEY")->AsInteger);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

Currency TDBTables::GetTableTotal(Database::TDBTransaction &DBTransaction,int TableNo)
{
   Currency Total = 0;
   std::set<__int64> SelectedTabs;
   GetTabKeys(DBTransaction,TableNo,SelectedTabs);
   for(std::set<__int64>::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end();
       advance(itTabs,1))
   {
      Total += TDBTab::GetTabTotal(DBTransaction,*itTabs);
   }
   return Total;
}

Currency TDBTables::GetTableBalance(Database::TDBTransaction &DBTransaction,int TableNo)
{
   Currency Total = 0;
   std::set<__int64> SelectedTabs;
   GetTabKeys(DBTransaction,TableNo,SelectedTabs);
   for(std::set<__int64>::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end();
       advance(itTabs,1))
   {
	  Total += TDBTab::GetTabBalance(DBTransaction,*itTabs);
   }
   return Total;
}

int TDBTables::GetSeatKey(Database::TDBTransaction &DBTransaction,int TabKey)
{
	int RetVal = 0;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT SEAT_KEY "
			"FROM "
				"SEAT "
			"WHERE "
				"TAB_KEY = :TAB_KEY ";

		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("SEAT_KEY")->AsInteger;
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

int TDBTables::GetSeatNo(Database::TDBTransaction &DBTransaction,int SeatKey)
{
   int RetVal = 0;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT SEATNO "
			"FROM "
				"SEAT "
			"WHERE "
				"SEAT_KEY = :SEAT_KEY ";

		IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = SeatKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("SEATNO")->AsInteger;
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

int TDBTables::GetSeatNoFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey)
{
   int RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT SEATNO "
			"FROM "
				"SEAT "
			"WHERE "
				"TAB_KEY = :TAB_KEY ";

		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("SEATNO")->AsInteger;
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}


int TDBTables::GetTableKey(Database::TDBTransaction &DBTransaction,int SeatKey)
{
   int RetVal = 0;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT TABLE_KEY "
			"FROM "
				"SEAT "
			"WHERE "
				"SEAT_KEY = :SEAT_KEY ";

		IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = SeatKey;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TABLE_KEY")->AsInteger;
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

int TDBTables::GetTableNo(Database::TDBTransaction &DBTransaction,int TableKey)
{
   int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT TABLE_NUMBER "
			"FROM "
				"TABLES "
			"WHERE "
				"TABLE_KEY = :TABLE_KEY ";
		IBInternalQuery->ParamByName("TABLE_KEY")->AsInteger = TableKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
      {
			RetVal = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
      }

	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}


bool TDBTables::GetTableSeat(Database::TDBTransaction &DBTransaction,int TabKey,TTableSeat *Info)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"SEAT.SEATNO, TABLES.TABLE_NUMBER, TABLES.TABLE_NAME,TABLES.PARTY_NAME, TAB.TAB_NAME "
			"FROM "
				"TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
				"INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
			"WHERE "
				"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
      if (IBInternalQuery->RecordCount)
      {
         Info->PartyName = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
			Info->TableName = IBInternalQuery->FieldByName("TABLE_NAME")->AsString;
         Info->SeatName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
			Info->SeatNo = IBInternalQuery->FieldByName("SEATNO")->AsInteger;
			Info->TableNo = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
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

UnicodeString TDBTables::GetNameMinor(Database::TDBTransaction &DBTransaction,int TableNumber , int SeatNumber)
{
	UnicodeString 	RetVal = GetNameMinor(DBTransaction,GetTabKey(DBTransaction,TableNumber,SeatNumber));
	if(RetVal == "")
	{
		RetVal = TGlobalSettings::Instance().SeatLabel + " " + IntToStr(SeatNumber);
	}
	return RetVal;
}

UnicodeString TDBTables::GetNameMinor(Database::TDBTransaction &DBTransaction,int TabKey)
{
	UnicodeString RetVal = "";
	try
	{
		RetVal = TDBTab::GetTabName(DBTransaction,TabKey);
		if(RetVal == "")
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"SELECT "
				"SEAT.SEATNO "
			"FROM "
				"SEAT "
			"WHERE "
				"SEAT.TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();

			if(IBInternalQuery->RecordCount)
			{
				RetVal = TGlobalSettings::Instance().SeatLabel + " " +	IBInternalQuery->FieldByName("SEATNO")->AsString;
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

UnicodeString TDBTables::GetNameMinorShort(Database::TDBTransaction &DBTransaction,int TabKey)
{
	UnicodeString RetVal = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
      "SELECT "
         "SEAT.SEATNO, TABLES.TABLE_NUMBER "
      "FROM "
         "SEAT LEFT JOIN TABLES ON SEAT.TABLE_KEY = TABLES.TABLE_KEY "
      "WHERE "
         "SEAT.TAB_KEY = :TAB_KEY";
      IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
      IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsString + "," +
						IBInternalQuery->FieldByName("SEATNO")->AsString;
      }

	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return RetVal;
}

bool TDBTables::IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo, int inSeatNo)
{
   bool RetVal = false;
	try
	{
	   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT "
      " 	o.ORDER_KEY "
		" FROM "
      "  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
      "  INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
      "  INNER JOIN ORDERS o ON a.TAB_KEY = o.TAB_KEY "
      " WHERE "
      "  t.TABLE_NUMBER = :TABLE_NUMBER AND "
      "  s.SEATNO = :SEATNO ";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ParamByName("SEATNO")->AsInteger = inSeatNo;
		IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount == 0)
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

//---------------------------------------------------------------------------

bool TDBTables::IsEmpty(Database::TDBTransaction &DBTransaction, int inTableNo)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      " SELECT "
      " 	ORDERS.ORDER_KEY "
		"FROM "
			"TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
			"INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
			"INNER JOIN ORDERS ON TAB.TAB_KEY = ORDERS.TAB_KEY "
		"WHERE "
			"TABLES.TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount == 0)
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

bool TDBTables::GetTableExists(Database::TDBTransaction &DBTransaction,int TableNo)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   bool RetVal = false;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	"SELECT "
		"TABLE_KEY "
	"FROM "
		"TABLES "
	"WHERE "
		"TABLE_NUMBER = :TABLE_NUMBER";
	IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
   IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = true;
	}
	return RetVal;
}

std::vector<TPatronType> TDBTables::GetPatronCount(Database::TDBTransaction &DBTransaction, int tableNo)
{
    std::vector<TPatronType> patronTypes;

    TManagerPatron::Instance().GetPatronTypes(DBTransaction, patronTypes);

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

    IBInternalQuery->SQL->Text = "SELECT "
                                    "TPC.PATRONCOUNT_KEY, "
                                    "TPC.PATRON_COUNT, "
                                    "TPC.PATRON_TYPE, "
                                    "TPC.TABLE_KEY, "
                                    "T.TABLE_NAME, "
                                    "T.TABLE_NUMBER  "
                                "FROM TABLEPATRONCOUNT TPC "
                                "INNER JOIN TABLES T ON TPC.TABLE_KEY = T.TABLE_KEY "
                                "WHERE T.TABLE_NUMBER = :TABLE_NUMBER ";

    IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = tableNo;
    IBInternalQuery->ExecQuery();

    if(IBInternalQuery->RecordCount)
    {
        for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            for (std::vector<TPatronType>::iterator ptrPatron = patronTypes.begin(); ptrPatron != patronTypes.end(); ptrPatron++)
            {
                if(ptrPatron->Name == IBInternalQuery->FieldByName("PATRON_TYPE")->AsString)
                {
                    ptrPatron->Count = IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;
                }
            }
        }
    }

    return patronTypes;
}

std::vector<TPatronType> TDBTables::GetBilledPatronCount(Database::TDBTransaction &DBTransaction, int tabKey)
{
    std::vector<TPatronType> patronTypes;
    TManagerPatron::Instance().GetPatronTypes(DBTransaction, patronTypes);
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =    "Select c.PATRON_TYPE,c.PATRON_COUNT,c.ARCBILL_KEY from TABHISTORY a "
                                    "left join DAYARCBILL b on a.INVOICE_NUMBER = b.INVOICE_NUMBER "
                                    "Left Join DAYPATRONCOUNT c on c.ARCBILL_KEY = b.ARCBILL_KEY "
                                    "where a.TAB_KEY = :TAB_KEY and a.IS_TABLE = 'T'"
                                    "union all "
                                    "Select c.PATRON_TYPE,c.PATRON_COUNT,c.ARCBILL_KEY from TABHISTORY a "
                                    "left join ARCBILL b on a.INVOICE_NUMBER = b.INVOICE_NUMBER "
                                    "Left Join PATRONCOUNT c on c.ARCBILL_KEY = b.ARCBILL_KEY "
                                    "where a.TAB_KEY = :TAB_KEY and a.IS_TABLE = 'T'";

    IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
        for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            for (std::vector<TPatronType>::iterator ptrPatron = patronTypes.begin(); ptrPatron != patronTypes.end(); ptrPatron++)
            {
                if(ptrPatron->Name == IBInternalQuery->FieldByName("PATRON_TYPE")->AsString)
                {
                    ptrPatron->Count += IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;
                }
            }
        }
    }
    return patronTypes;
}

int TDBTables::GetPatronNumbersForWebOrders( Database::TDBTransaction &DBTransaction,int TableNumber)
{
	int PatronCount = 1;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	    IBInternalQuery->SQL->Text = "SELECT "
                                    " DEFAULT_NUMBER_OF_SEATS "
                                    " FROM TABLES "
                                    "WHERE TABLE_NUMBER = :TABLE_NUMBER ";
	IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
    {
        PatronCount = IBInternalQuery->FieldByName("DEFAULT_NUMBER_OF_SEATS")->AsInteger;
    }
	return PatronCount;
}

int TDBTables::GetPatronNumbers( Database::TDBTransaction &DBTransaction,int TableNumber)
{
	int PatronCount = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT  first(1) a.PATRON_COUNT FROM ORDERS a where a.TABLE_NUMBER = :TABLE_NUMBER";
	IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
	IBInternalQuery->ExecQuery();
	if(!IBInternalQuery->Eof)
	{
		PatronCount = IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;
	}
	return PatronCount;
}


void TDBTables::SetPatronNumbers( Database::TDBTransaction &DBTransaction,int TableNumber,int PatronCount)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "UPDATE  ORDERS a SET a.PATRON_COUNT = :PATRON_COUNT  where a.TABLE_NUMBER = :TABLE_NUMBER";
	IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
    IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger  = PatronCount;
	IBInternalQuery->ExecQuery();
}

bool TDBTables::IsSeatPresent( Database::TDBTransaction &DBTransaction,int TableNumber,int SeatNumber)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * from Orders a where a.SEATNO = :SEATNO and a.TABLE_NUMBER = :TABLE_NUMBER";
	IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
    IBInternalQuery->ParamByName("SEATNO")->AsInteger  = SeatNumber;
	IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
	{
		return true;
	}
    return false;
}

void TDBTables::SetPatronCount(Database::TDBTransaction &DBTransaction, int tableNo, std::vector<TPatronType> patronTypes)
{
    try
	{
		std::vector <TPatronType> ::iterator ptrPatronTypes;
		for (ptrPatronTypes = patronTypes.begin(); ptrPatronTypes != patronTypes.end(); ptrPatronTypes++)
		{
			if (ptrPatronTypes->Count != 0)
			{
				TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TABLEPATRONCOUNT, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();

				int patronCountKey = IBInternalQuery->Fields[0]->AsInteger;
				IBInternalQuery->Close();

                IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT TABLE_KEY FROM TABLES WHERE TABLE_NUMBER = :Table_Number";
                IBInternalQuery->ParamByName("Table_Number")->AsInteger = tableNo;
				IBInternalQuery->ExecQuery();

				int tableKey = IBInternalQuery->Fields[0]->AsInteger;
				IBInternalQuery->Close();

                if(tableKey <= 0)
                {
                    tableKey = GetOrCreateTable(DBTransaction, tableNo);
                }

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
					 "INSERT INTO TABLEPATRONCOUNT (" "PATRONCOUNT_KEY, " "TABLE_KEY, " "PATRON_TYPE, " "PATRON_COUNT) "
					 "VALUES (" ":PATRONCOUNT_KEY, " ":TABLE_KEY, " ":PATRON_TYPE, " ":PATRON_COUNT) ";

				IBInternalQuery->ParamByName("PATRONCOUNT_KEY")->AsString = patronCountKey;
				IBInternalQuery->ParamByName("TABLE_KEY")->AsString = tableKey;
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

void TDBTables::ShiftPatronCountToNewTable(Database::TDBTransaction &DBTransaction, int sourceTableNo, int destinationTableNo)
{
    try
	{
		//If the user is playing around with the code and manages
		//entering source and destination tables as same then do nothing go back.
		if(sourceTableNo == destinationTableNo)
			return;
		
        std::vector<TPatronType> patronTypesOnSource = GetPatronCount(DBTransaction, sourceTableNo);
        std::vector<TPatronType> patronTypesOnDestination = GetPatronCount(DBTransaction, destinationTableNo);

        int destinationPatronCount = 0;
        if(!patronTypesOnDestination.empty())
        {
            for(std::vector<TPatronType>::iterator it = patronTypesOnDestination.begin(); it != patronTypesOnDestination.end(); ++it)
            {
                (destinationPatronCount) += it->Count;
            }
        }

        if(destinationPatronCount > 0)
        {
            int sourcePatronCount = 0;
            if(!patronTypesOnSource.empty())
            {
                for(std::vector<TPatronType>::iterator it = patronTypesOnSource.begin(); it != patronTypesOnSource.end(); ++it)
                {
                    (sourcePatronCount) += it->Count;
                }
            }

            if(sourcePatronCount <= 0)
            {
                ClearPatronCount(DBTransaction, sourceTableNo);
                return;
            }
            else
            {
                for (std::vector<TPatronType>::iterator destinationPatron = patronTypesOnDestination.begin(); destinationPatron != patronTypesOnDestination.end(); destinationPatron++)
                {
                    for (std::vector<TPatronType>::iterator sourcePatron = patronTypesOnSource.begin(); sourcePatron != patronTypesOnSource.end(); sourcePatron++)
                    {
                        if(sourcePatron->Name == destinationPatron->Name)
                        {
                            destinationPatron->Count += sourcePatron->Count;
                        }
                    }
                }
                ClearPatronCount(DBTransaction, destinationTableNo);
                ClearPatronCount(DBTransaction, sourceTableNo);
                SetPatronCount(DBTransaction, destinationTableNo, patronTypesOnDestination);
                return;
            }
        }
        else
        {
            ClearPatronCount(DBTransaction, sourceTableNo);
            SetPatronCount(DBTransaction, destinationTableNo, patronTypesOnSource);
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBTables::ClearPatronCounts(Database::TDBTransaction &DBTransaction)
{
    try
	{
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "DELETE "
                                        "FROM TABLEPATRONCOUNT TPC "
                                        "WHERE TPC.TABLE_KEY NOT IN "
                                        "(SELECT "
                                            "TABLES.TABLE_KEY "
                                        "FROM "
                                        "TABLES "
                                        "INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
                                        "INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
                                        "INNER JOIN ORDERS ON TAB.TAB_KEY = ORDERS.TAB_KEY) ";

        IBInternalQuery->ExecQuery();
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBTables::ClearPatronCount(Database::TDBTransaction &DBTransaction, int tableNo)
{
    try
	{
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT TABLE_KEY FROM TABLES WHERE TABLE_NUMBER = :Table_Number";
        IBInternalQuery->ParamByName("Table_Number")->AsInteger = tableNo;
        IBInternalQuery->ExecQuery();

        int tableKey = IBInternalQuery->Fields[0]->AsInteger;
        IBInternalQuery->Close();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "DELETE FROM TABLEPATRONCOUNT WHERE TABLE_KEY = :Table_Key";

        IBInternalQuery->ParamByName("Table_Key")->AsInteger = tableKey;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

bool TDBTables::GetSeatExists(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
	bool RetVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   " SELECT "
   " 	SEAT.SEAT_KEY "
   " FROM "
   "  TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
   " WHERE "
   "  t.TABLE_NUMBER = :TABLE_NUMBER AND "
   "  s.SEATNO = :SEATNO ";
   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = inTableNo;
   IBInternalQuery->ParamByName("SEATNO")->AsInteger = inSeatNo;
	IBInternalQuery->ExecQuery();

	if(IBInternalQuery->RecordCount)
	{
      RetVal = true;
	}
	return RetVal;
}

void TDBTables::SetTableBillingStatus(Database::TDBTransaction &DBTransaction,int TableNo ,TOrderBillingStatus status)
{
  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
  IBInternalQuery->Close();
  IBInternalQuery->SQL->Text = "Select * from TABLESSTATUS where TABLE_NUMBER = :TABLE_NUMBER ;";
  IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
  IBInternalQuery->ExecQuery();
  if(!IBInternalQuery->Eof)
  {
    IBInternalQuery->SQL->Text =" UPDATE TABLESSTATUS  "
   " SET TABLE_STATUS = :TABLE_STATUS "
   " WHERE "
   " TABLE_NUMBER = :TABLE_NUMBER ";
  }
  else
  {
    IBInternalQuery->SQL->Text ="INSERT INTO TABLESSTATUS "
                                 "(TABLE_NUMBER,TABLE_STATUS ) "
                                 "VALUES (:TABLE_NUMBER, :TABLE_STATUS );";
  }
  IBInternalQuery->Close();
  IBInternalQuery->ParamByName("TABLE_STATUS")->AsInteger = status;
  IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
  IBInternalQuery->ExecQuery();
}

int TDBTables::GetTableBillingStatus(Database::TDBTransaction &DBTransaction,int TableNo)
{
  int RetVal = 0;
  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
  IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   " SELECT TABLE_STATUS  FROM TABLESSTATUS WHERE TABLE_NUMBER = :TABLE_NUMBER ";
   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
     {
      RetVal = IBInternalQuery->FieldByName("TABLE_STATUS")->AsInteger;
     }
   return RetVal;
}

TDateTime TDBTables::GetMaxTableBillingTimeStamp(Database::TDBTransaction &DBTransaction,int TableNo)
{
   TDateTime RetVal = Now();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
  		"Select "
		"Max(TIME_STAMP) TIME_STAMP "
		"From "
		"Orders "
		"Where TABLE_NUMBER = :TABLE_NUMBER ";
   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
     {
      RetVal = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
     }
   return RetVal;
}

//------------------------------------------------------------------------------
void TDBTables::UpdateTablePartyName( Database::TDBTransaction &dbTransaction, int TableNumber)
{

    TIBSQL *iBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());

	try
	{
        TIBSQL *IBUpdateQuery = dbTransaction.Query(dbTransaction.AddQuery());
        iBUpdateQuery->Close();
        iBUpdateQuery->SQL->Text =
                                    "UPDATE "
                                    "TABLES "
                                    "SET "
                                    "PARTY_NAME = :PARTY_NAME "
                                    "WHERE "
                                    "TABLE_KEY = :TABLE_KEY ";

        iBUpdateQuery->ParamByName("TABLE_KEY")->AsInteger = TableNumber;
        iBUpdateQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
