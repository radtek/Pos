//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBRooms.h"
#include "MMLogging.h"
#include "DBTab.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TDBRooms::TDBRooms()
{
/*	SelectedRoom							= new TRoomData;
	MaxRememberedRooms					= 20;*/
}

TDBRooms::~TDBRooms()
{
/*	delete SelectedRoom;  */
}

void __fastcall TDBRooms::Initialise(Database::TDBTransaction &DBTransaction)
{
	SetRoomsUp(DBTransaction);
}

void TDBRooms::SetRoomStatus(Database::TDBTransaction &DBTransaction,int RoomNumber,eGMRoomStatus inStatus)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int RoomKey = FindOrCreateRoom(DBTransaction,RoomNumber);
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"ROOMS "
		"SET "
			"STATUS = :STATUS "
		"WHERE "
			"ROOM_KEY = :ROOM_KEY";
		IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
		IBInternalQuery->ParamByName("STATUS")->AsInteger = inStatus;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBRooms::SetRoomName(Database::TDBTransaction &DBTransaction,int RoomNumber, UnicodeString RoomName)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int RoomKey = FindOrCreateRoom(DBTransaction,RoomNumber);
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"ROOMS "
		"SET "
			"ROOM_NAME = :ROOM_NAME "
		"WHERE "
			"ROOM_KEY = :ROOM_KEY";
		IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
		IBInternalQuery->ParamByName("ROOM_NAME")->AsString = RoomName;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBRooms::SetBookingID(Database::TDBTransaction &DBTransaction,int RoomNumber, int BookingID)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int RoomKey = FindOrCreateRoom(DBTransaction,RoomNumber);
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"ROOMS "
		"SET "
			"BOOKING_ID = :BOOKING_ID "
		"WHERE "
			"ROOM_KEY = :ROOM_KEY";
		IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
		IBInternalQuery->ParamByName("BOOKING_ID")->AsInteger = BookingID;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

UnicodeString TDBRooms::GetRoomName(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	UnicodeString Retval = "";

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" ROOM_NAME "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		Retval = IBInternalQuery->FieldByName("ROOM_NAME")->AsString;
		if(Retval == "")
		{
			Retval = "Room # " + IntToStr(RoomNumber);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Retval;
}

void TDBRooms::SetPartyName(Database::TDBTransaction &DBTransaction,int RoomNumber, UnicodeString PartyName)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		int TabKey = FindOrCreateRoomTab(DBTransaction,RoomNumber);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
			"TAB "
		"SET "
			"TAB_NAME = :TAB_NAME "
		"WHERE "
			"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ParamByName("TAB_NAME")->AsString = PartyName;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

UnicodeString TDBRooms::GetPartyName(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	UnicodeString Retval = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int TabKey;
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY, TAB_NAME "
		" FROM "
		"  TAB "
		" WHERE "
		"  TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		Retval = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Retval;
}


int TDBRooms::FindOrCreateRoom(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	int RoomKey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" ROOM_KEY "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		RoomKey = IBInternalQuery->FieldByName("ROOM_KEY")->AsInteger;
		if(RoomKey == 0)
		{
			int key;
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ROOMS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			key = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO ROOMS ("
				"ROOM_KEY,"
				"ROOM_NUMBER) "
			"VALUES ("
				":ROOM_KEY,"
				":ROOM_NUMBER);";
			IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = key;
			IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
			IBInternalQuery->ExecQuery();
			RoomKey = key;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RoomKey;
}

int TDBRooms::FindOrCreateRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	int TabKey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int RoomKey = FindOrCreateRoom(DBTransaction,RoomNumber);

		bool CreateTab = true;
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_KEY = :ROOM_KEY ";
		IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
		IBInternalQuery->ExecQuery();
		TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		if(TabKey != 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" SELECT "
			" TAB_KEY "
			" FROM "
			"  TAB "
			" WHERE "
			"  TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
			if(TabKey == IBInternalQuery->FieldByName("TAB_KEY")->AsInteger)
			{
				CreateTab = false;
			}
		}

		if(CreateTab)
		{
      	TabKey = TDBTab::GetOrCreateTab(DBTransaction,0);
         TDBTab::SetTabType(DBTransaction,TabKey,TabRoom);
         TDBTab::SetTabName(DBTransaction,TabKey,"Room #" + IntToStr(RoomNumber));

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
				"ROOMS "
			"SET "
				"TAB_KEY = :TAB_KEY "
			"WHERE "
				"ROOM_KEY = :ROOM_KEY";
			IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return TabKey;
}

int TDBRooms::GetBookingID(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	int BookingID = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" BOOKING_ID "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		BookingID = IBInternalQuery->FieldByName("BOOKING_ID")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return BookingID;
}

eGMRoomStatus TDBRooms::GetRoomStatus(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	eGMRoomStatus Status;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" STATUS "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		Status = (eGMRoomStatus)IBInternalQuery->FieldByName("STATUS")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Status;
}


int TDBRooms::GetRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
   int Retval = 0;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		int TabKey;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY "
		" FROM "
		"  TAB "
		" WHERE "
		"  TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		Retval = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Retval;
}

int TDBRooms::GetRoomNumber(Database::TDBTransaction &DBTransaction,int TabKey)
{
	int RoomNumber;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" ROOM_NUMBER "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  TAB_KEY = :TAB_KEY ";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		RoomNumber = IBInternalQuery->FieldByName("ROOM_NUMBER")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RoomNumber;
}

bool TDBRooms::IsEmpty(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	bool Retval = true;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		int TabKey;
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" TAB_KEY "
		" FROM "
		"  ROOMS "
		" WHERE "
		"  ROOM_NUMBER = :ROOM_NUMBER ";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
		TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;

		if(TabKey != 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" SELECT "
			" TAB_KEY "
			" FROM "
			"  TAB "
			" WHERE "
			"  TAB_KEY = :TAB_KEY";
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
			IBInternalQuery->ExecQuery();
			if(IBInternalQuery->RecordCount)
			{
				Retval = false;
			}
		}

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return Retval;
}

void TDBRooms::DisconnectRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			"UPDATE ROOMS SET "
				"TAB_KEY = NULL "
			"WHERE "
				"ROOM_NUMBER = :ROOM_NUMBER";
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = RoomNumber;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}



void TDBRooms::GetRoomData(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter)
{
	RoomsList->Clear();
	try
	{

		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		// Set the rooms caption.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" select "
		" ROOM_KEY,ROOM_NUMBER,ROOM_NAME,STATUS,BOOKING_ID,ROOMS.TAB_KEY, TAB.TAB_NAME PARTY_NAME, "
		" TAB.CARD CARD, COUNT(ORDERS.ORDER_KEY) ORDER_COUNT"
		" from "
		"  ROOMS left join TAB on ROOMS.TAB_KEY = TAB.TAB_KEY"
		"  left join ORDERS on ORDERS.TAB_KEY = TAB.TAB_KEY "
		" GROUP BY ROOM_KEY,ROOM_NUMBER,ROOM_NAME,STATUS,BOOKING_ID,ROOMS.TAB_KEY, TAB.TAB_NAME,"
		" TAB.CARD";

		IBInternalQuery->ExecQuery();
		for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			eGMRoomStatus RoomStatus = (eGMRoomStatus)IBInternalQuery->FieldByName("STATUS")->AsInteger;
			if(   ((inFilter == eRoomFilterAll) ||
					(inFilter == eRoomFilterVacant && RoomStatus == eVacant) ||
					(inFilter == eRoomFilterOccupied && RoomStatus != eVacant)) )
			{
				TRoomData *RoomData = new TRoomData;
				RoomData->RoomKey	= IBInternalQuery->FieldByName("ROOM_KEY")->AsInteger;
				RoomData->Name		= IBInternalQuery->FieldByName("ROOM_NAME")->AsString;
				RoomData->Card		= IBInternalQuery->FieldByName("CARD")->AsString;
				RoomData->RoomNo	= IBInternalQuery->FieldByName("ROOM_NUMBER")->AsInteger;
				RoomData->Status	= RoomStatus;
				RoomData->BookingID	= IBInternalQuery->FieldByName("BOOKING_ID")->AsInteger;
				if (RoomData->Name == "")
				{
					RoomData->Name				= "Room " + IntToStr(RoomData->RoomNo);
				}
				RoomData->Caption = RoomData->Name;
				RoomData->PartyName = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
				if(RoomData->PartyName != "")
				{
					RoomData->Caption = RoomData->Caption + "\r" + RoomData->PartyName;
				}

				if(IBInternalQuery->FieldByName("ORDER_COUNT")->AsInteger == 0)
				{
					RoomData->LastOrder = TDateTime(0);
				}
				RoomsList->Add(RoomData);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

//------------------------------------------------------------------------------


void TDBRooms::GetList(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter)
{
	try
	{
		GetRoomData(DBTransaction,RoomsList,inFilter);
		for (int i = 0; i < RoomsList->Count; i++)
		{
			TRoomData *Room = RoomsList->Get(i);
			Room->Caption = Room->Name + " ( " + IntToStr(Room->RoomNo) + " )";
		}
		RoomsList->SortByRoomName();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TDBRooms::GetListNumeric(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter)
{
	try
	{
		GetRoomData(DBTransaction,RoomsList,inFilter);
		for (int i = 0; i < RoomsList->Count; i++)
		{
			TRoomData *Room = RoomsList->Get(i);
			Room->Caption = IntToStr(Room->RoomNo) + " ( " + Room->Name + " )";
		}
		RoomsList->SortByRoomNumber();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

void TDBRooms::GetListGuest(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter)
{
	try
	{
		GetRoomData(DBTransaction,RoomsList,inFilter);
		for (int i = 0; i < RoomsList->Count; i++)
		{
			TRoomData *Room = RoomsList->Get(i);
			Room->Caption = Room->PartyName + " ( " + IntToStr(Room->RoomNo) + " )";
		}
		RoomsList->SortByGuest();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

/*
void TDBRooms::SetSelectedRoom(TRoomData *inRoom)
{
	SelectedRoom->Assign(inRoom);
}

void TDBRooms::ClearSelectedRoom()
{
	SelectedRoom->Clear();
}

TRoomData *TDBRooms::GetSelectedRoom()
{
	return SelectedRoom;
}
 */

bool TDBRooms::GetRoom(Database::TDBTransaction &DBTransaction,int RoomKey, TRoomData *TempRoom)
{
	bool RetVal = false;
	try
	{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	TempRoom->Clear();
	// Set the rooms caption.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" select "
	" ROOM_KEY, ROOM_NUMBER, ROOM_NAME,STATUS,ROOMS.TAB_KEY, TAB.TAB_NAME PARTY_NAME, "
	" TAB.CARD CARD "
	" from "
	"  ROOMS left join TAB on ROOMS.TAB_KEY = TAB.TAB_KEY "
	" Where ROOM_KEY = :ROOM_KEY ";
	IBInternalQuery->ParamByName("ROOM_KEY")->AsInteger = RoomKey;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		eGMRoomStatus RoomStatus = (eGMRoomStatus)IBInternalQuery->FieldByName("STATUS")->AsInteger;
      TempRoom->RoomKey	= IBInternalQuery->FieldByName("ROOM_KEY")->AsInteger;
      TempRoom->Name		= IBInternalQuery->FieldByName("ROOM_NAME")->AsString;
      TempRoom->Card		= IBInternalQuery->FieldByName("CARD")->AsString;
      TempRoom->RoomNo	= IBInternalQuery->FieldByName("ROOM_NUMBER")->AsInteger;
      TempRoom->Status	= RoomStatus;
      if (TempRoom->Name == "")
      {
         TempRoom->Name	= "Room " + IntToStr(TempRoom->RoomNo);
      }
      TempRoom->Caption = TempRoom->Name;
      TempRoom->PartyName = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
      if(TempRoom->PartyName != "")
      {
         TempRoom->Caption = TempRoom->Caption + "\r" + TempRoom->PartyName;
      }

      if(TDBTab::GetIsEmpty(DBTransaction,IBInternalQuery->FieldByName("TAB_KEY")->AsInteger))
      {
         TempRoom->LastOrder = TDateTime(0);
      }
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

void TDBRooms::SetRoomsUp(Database::TDBTransaction &DBTransaction)
{
	try
	{
		if(GetRoomCount(DBTransaction) == 0)
		{
			for( int i = 1 ; i < 201 ; i ++)
         {
				FindOrCreateRoom(DBTransaction,i);
				SetRoomStatus(DBTransaction,i,eVacant);
				SetRoomName(DBTransaction,i,FormatFloat("Room 000",i));
         }
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TDBRooms::GetRoomCount(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT "
		" COUNT (ROOM_KEY) ROOM_COUNT"
		" FROM "
		" ROOMS";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->FieldByName("ROOM_COUNT")->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}
//------------------------------------------------------------------------------

