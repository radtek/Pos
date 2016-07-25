//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "TableManager.h"
#include "Tabs.h"
#include "ManagerVariable.h"
#include "NetMessageSeatName.h"
#include "ManagerNet.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
TManagerTable *Tables;

void TManagerTable::Initialise(Database::TDBTransaction &DBTransaction)
{
	Enabled = true;
	SeatLabel = TManagerVariable::Instance().GetStr(DBTransaction,vmSeatLabel,"Guest");
}

//---------------------------------------------------------------------------

int TManagerTable::GetOrCreateTable(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	if(!fEnabled) return 0;
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

int TManagerTable::GetOrCreateSeat(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
	if(!fEnabled) return 0;
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
void TManagerTable::SetTableName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString TableName)
{
	if(!fEnabled) return;
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

void TManagerTable::SetSeatTabToNull(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	if(!fEnabled) return;

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

void TManagerTable::SetSeatTab(Database::TDBTransaction &DBTransaction,int inSeatKey,int inTabKey)
{
	if(!fEnabled) return;
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

void TManagerTable::ClearSeatsName(Database::TDBTransaction &DBTransaction, int TableNo)
{
	if(!fEnabled) return;
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

void TManagerTable::SetSeatName(Database::TDBControl &IBDatabase,UnicodeString Name, int TableNo, int SeatNumber)
{
	if(!fEnabled) return;
	try
	{
		if (Valid(TableNo) && Valid(SeatNumber))
		{
			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();

			int SeatKey = GetOrCreateSeat(DBTransaction,TableNo,SeatNumber);
			int TabKey = GetTabKey(DBTransaction,SeatKey);
			TDBTab::SetTabName(DBTransaction,TabKey,Name);
			DBTransaction.Commit();

			TNetMessageSeatName *Request = new TNetMessageSeatName;
			try
			{
				Request->TabLK = TabKey;
				TDeviceRealTerminal::Instance().ManagerNet->SendToAll(Request);
			}
			__finally
			{
				delete Request;
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
};


UnicodeString TManagerTable::GetTableName(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	if(!fEnabled) return "";
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

void TManagerTable::SetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo,UnicodeString PartyName)
{
	if(!fEnabled) return;

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

UnicodeString TManagerTable::GetPartyName(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	if(!fEnabled) return "";
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

UnicodeString TManagerTable::GetPartyNameShort(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	if(!fEnabled) return "";
	
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

void TManagerTable::GetSeats(Database::TDBTransaction &DBTransaction,TStringList * TabList, int TableNumber)
{
	if(!fEnabled) return;
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
				Index = TabList->Add(SeatLabel + " " + IBInternalQuery->FieldByName("SEAT_NUMBER")->AsString);
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

bool TManagerTable::Valid(int inTableNo)
{
	bool RetVal = false;
   if(inTableNo > 0 && inTableNo < 100)
   {
		RetVal = true;
   }
   return RetVal;
}

bool TManagerTable::Valid(int inTableNo, int inSeatNo)
{
	bool RetVal = false;
	if(Valid(inTableNo) && Valid(inSeatNo))
   {
		RetVal = true;
   }
   return RetVal;
}

int TManagerTable::GetTabKey(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
   int RetVal = 0;
	if(!fEnabled) return RetVal;
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

int TManagerTable::GetTabKey(Database::TDBTransaction &DBTransaction,int inSeatKey)
{
	if(!fEnabled) return 0;
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

void TManagerTable::GetTabKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedTabs)
{
	if(!fEnabled) return;
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


void TManagerTable::GetOrderKeys(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> &SelectedOrders)
{
	if(!fEnabled) return;
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

void TManagerTable::GetTabKeysWithOrders(Database::TDBTransaction &DBTransaction,int inTableNo,std::set<__int64> *SelectedTabs)
{
	if(!fEnabled) return;
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
			SelectedTDBTab::insert(IBInternalQuery->FieldByName("TAB_KEY")->AsInteger);
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

Currency TManagerTable::GetTableTotal(Database::TDBTransaction &DBTransaction,int TableNo)
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

Currency TManagerTable::GetTableBalance(Database::TDBTransaction &DBTransaction,int TableNo)
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

int TManagerTable::GetSeatKey(Database::TDBTransaction &DBTransaction,int TabKey)
{
	if(!fEnabled) return 0;
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

int TManagerTable::GetSeatNo(Database::TDBTransaction &DBTransaction,int SeatKey)
{
	if(!fEnabled) return 0;
	
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

int TManagerTable::GetSeatNoFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey)
{
	if(!fEnabled) return 0;

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


int TManagerTable::GetTableKey(Database::TDBTransaction &DBTransaction,int SeatKey)
{
	if(!fEnabled) return 0;
	
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

int TManagerTable::GetTableNo(Database::TDBTransaction &DBTransaction,int TableKey)
{
	if(!fEnabled) return 0;
	
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


bool TManagerTable::GetTableSeat(Database::TDBTransaction &DBTransaction,int TabKey,TTableSeat *Info)
{
	bool RetVal = false;
	if(!fEnabled) return RetVal;
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

UnicodeString TManagerTable::GetNameMinor(Database::TDBTransaction &DBTransaction,int TableNumber , int SeatNumber)
{
	UnicodeString 	RetVal = GetNameMinor(DBTransaction,GetTabKey(DBTransaction,TableNumber,SeatNumber));
	if(RetVal == "")
	{
		RetVal = SeatLabel + " " + IntToStr(SeatNumber);
	}
	return RetVal;
}

UnicodeString TManagerTable::GetNameMinor(Database::TDBTransaction &DBTransaction,int TabKey)
{
	UnicodeString RetVal = "";
	if(!fEnabled) return RetVal;
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
				RetVal = SeatLabel + " " +	IBInternalQuery->FieldByName("SEATNO")->AsString;
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

UnicodeString TManagerTable::GetNameMinorShort(Database::TDBTransaction &DBTransaction,int TabKey)
{
	UnicodeString RetVal = "";
	if(!fEnabled) return RetVal;

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

bool TManagerTable::IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo, int inSeatNo)
{
   bool RetVal = false;
	if(!fEnabled) return RetVal;
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

bool TManagerTable::IsEmpty(Database::TDBTransaction &DBTransaction,int inTableNo)
{
	bool RetVal = false;
	if(!fEnabled) return RetVal;
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

bool TManagerTable::GetTableExists(Database::TDBTransaction &DBTransaction,int TableNo)
{
   if( !fEnabled )return false;
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

bool TManagerTable::GetSeatExists(Database::TDBTransaction &DBTransaction,int inTableNo,int inSeatNo)
{
   if( !fEnabled )return 0;
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
