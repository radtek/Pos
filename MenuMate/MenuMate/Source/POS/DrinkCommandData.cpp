//---------------------------------------------------------------------------


#pragma hdrstop

#include "DrinkCommandData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------


#include <math.h>
#include "DeviceRealTerminal.h"
#include "DBSaleTimes.h"
#include "DBTab.h"
#include "DBOrder.h"
#include "ManagerStock.h"
#include <DateUtils.hpp>
#include "DBTables.h"
#include "DBSecurity.h"
#include "TcpStreamPrepare.h"
#include "DBTierLevel.h"
#include "SelectDish.h"
#include "SystemEvents.h"
//------------------------------------------------------------------

#pragma package(smart_init)

TDrinkCommandData::TDrinkCommandData()
{
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void TDrinkCommandData::InsertDataToTable(AnsiString memberNo ,int session_uuid)
{
    try
    {
      AnsiString session_status = "OPEN";
      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="INSERT INTO DC_SESSION_DATA (" "SESSION_ID, " "SESSION_TOKEN, " "SESSION_STATUS) " "VALUES ("
       ":SESSION_ID, " ":SESSION_TOKEN, " ":SESSION_STATUS);";

	  IBInternalQuery->ParamByName("SESSION_ID")->AsInteger = session_uuid;
	  IBInternalQuery->ParamByName("SESSION_TOKEN")->AsString = memberNo;
	  IBInternalQuery->ParamByName("SESSION_STATUS")->AsString = session_status;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}

int TDrinkCommandData::GetGroupKey(int contactKey)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =			" SELECT "
                                            " GROUPS_KEY "
                                            " FROM "
                                            "  CONTACTGROUPS "
                                            " WHERE "
                                            " CONTACTS_KEY  = :CONTACTS_KEY ";

	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
      IBInternalQuery->ExecQuery();
	  int val = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
      DBTransaction.Commit();
      return val;
    }
	catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TDrinkCommandData::GenerateSessionId()
{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DC_SESSION_DATA_KEY, 1) FROM RDB$DATABASE ";
		IBInternalQuery->ExecQuery();
		int val = IBInternalQuery->Fields[0]->AsInteger;
		IBInternalQuery->Close();
        char* id = new char[20];
        itoa(val , id , 10);
        UnicodeString session_id = id;

        return val;
}

int TDrinkCommandData::GenerateLineItemId()
{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DC_SESSION_LINE_ITEM_ID, 1) FROM RDB$DATABASE ";
		IBInternalQuery->ExecQuery();
		int val = IBInternalQuery->Fields[0]->AsInteger;
		IBInternalQuery->Close();

        return val ;
}

Currency TDrinkCommandData::GetPoints(TPaymentTransaction PointsTransaction)
{
  Currency points = 0;

     if (TGlobalSettings::Instance().EnableSeperateEarntPts)
        {
            points = PointsTransaction.Membership.Member.Points.getPointsBalance(pasDatabase,ptstAccount);
        }
        else
        {
            points = PointsTransaction.Membership.Member.Points.getPointsBalance(pasDatabase);
        }

  return points;
}
AnsiString TDrinkCommandData::GetCardIdFromMemberNo(AnsiString memberNo)
{
        int contact_key = 0;
        AnsiString card_number = "" ;

	try
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();

		IBInternalQuery->SQL->Text =
			" SELECT "
			" CONTACTS_KEY, "
            " PROX_CARD "
			" FROM "
			"  CONTACTS "
			" WHERE "
			"  MEMBER_NUMBER = :MEMBER_NUMBER ";
        IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = memberNo;
		IBInternalQuery->ExecQuery();
        contact_key = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
        card_number = IBInternalQuery->FieldByName("PROX_CARD")->AsString;
        IBInternalQuery->Close();
        if(card_number.Length() == 0)
        {

            IBInternalQuery->SQL->Text =
                " SELECT "
                " SWIPE_CARD "
                " FROM "
                "  CONTACTCARDS "
                " WHERE "
                "  CONTACTS_KEY = :CONTACTS_KEY ";
            IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contact_key;
            IBInternalQuery->ExecQuery();
            card_number = IBInternalQuery->FieldByName("SWIPE_CARD")->AsString;
            IBInternalQuery->Close();
        }
        DBTransaction.Commit();

        return card_number;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}
AnsiString TDrinkCommandData::CheckForOpenSession(AnsiString memberNo)
{
    AnsiString status = "OPEN";
    AnsiString session_uuid = "";
	try
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT "
			" SESSION_ID "
			" FROM "
			"  DC_SESSION_DATA "
			" WHERE "
			"  SESSION_STATUS = :SESSION_STATUS AND SESSION_TOKEN = :SESSION_TOKEN";
        IBInternalQuery->ParamByName("SESSION_STATUS")->AsString = status;
		IBInternalQuery->ParamByName("SESSION_TOKEN")->AsString = memberNo;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			session_uuid = IBInternalQuery->FieldByName("SESSION_ID")->AsString;
		}
        DBTransaction.Commit();
        return session_uuid;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
void TDrinkCommandData::UpdateClose(AnsiString session_uuid)
{
    try
    {
      AnsiString session_status = "CLOSE";
      int session_id = atoi(session_uuid.c_str());
      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="UPDATE DC_SESSION_DATA " "SET " "SESSION_STATUS	= :SESSION_STATUS " "WHERE " "SESSION_ID IN (" + session_uuid + ")";

//	  IBInternalQuery->ParamByName("SESSION_ID")->AsInteger = session_id;
	  IBInternalQuery->ParamByName("SESSION_STATUS")->AsString = session_status;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
void TDrinkCommandData::UpdateTimeStamp(AnsiString session_id , AnsiString time_Stamp)
{
    try
    {
      int session = atoi(session_id.c_str());
      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="UPDATE DC_SESSION_DATA " "SET " "TIME_STAMP_BILL	= :TIME_STAMP_BILL " "WHERE " "SESSION_ID = :SESSION_ID";

	  IBInternalQuery->ParamByName("SESSION_ID")->AsInteger = session;
	  IBInternalQuery->ParamByName("TIME_STAMP_BILL")->AsString = time_Stamp;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
void TDrinkCommandData::UpdateTimeStampToNull(AnsiString memberDetails)
{
    try
    {
      int memberNo = atoi(memberDetails.c_str());
      AnsiString status = "OPEN" ;
      AnsiString stamp = "";
      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="UPDATE DC_SESSION_DATA " "SET " "TIME_STAMP_BILL	= :TIME_STAMP_BILL " "WHERE " "SESSION_TOKEN = :SESSION_TOKEN AND SESSION_STATUS = :SESSION_STATUS";

	  IBInternalQuery->ParamByName("SESSION_TOKEN")->AsInteger = memberNo;
	  IBInternalQuery->ParamByName("SESSION_STATUS")->AsString = status;
	  IBInternalQuery->ParamByName("TIME_STAMP_BILL")->AsString = stamp;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDrinkCommandData::UpdateLineItemID(UnicodeString _session_uuid , int line_item)
{
    try
    {
      AnsiString session_uuid = _session_uuid;
      int session_id = atoi(session_uuid.c_str());
      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text ="UPDATE DC_SESSION_DATA " "SET " "LINE_ITEM_ID	= :LINE_ITEM_ID " "WHERE " "SESSION_ID = :SESSION_ID";

	  IBInternalQuery->ParamByName("SESSION_ID")->AsInteger = session_id;
	  IBInternalQuery->ParamByName("LINE_ITEM_ID")->AsInteger = line_item;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
void TDrinkCommandData::getItemFromThirdPartyCode(AnsiString thirdPartyCode , AnsiString qtyValue , AnsiString billValue, bool lastProduct)
{
    int thirdPartyCodeKey = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT "
			" THIRDPARTYCODES_KEY "
			" FROM "
			"  THIRDPARTYCODES "
			" WHERE "
			"  CODE = :CODE ";
        IBInternalQuery->ParamByName("CODE")->AsString = thirdPartyCode;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			thirdPartyCodeKey = IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
            fetchItemKey(thirdPartyCodeKey , qtyValue , billValue , thirdPartyCode, lastProduct);
		}
        else if(IBInternalQuery->RecordCount <= 0)
        {
            AnsiString message = "The Item code ";
            message += thirdPartyCode;
            message += " is not present in Menu";

            MessageBox(message,"Item Code Error", MB_OK);
        }
        DBTransaction.Commit();

    }
	catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
AnsiString TDrinkCommandData::FetchTimeStamp(AnsiString session_uuid)
{

    int session_id = atoi(session_uuid.c_str());
    AnsiString time = "";
	try
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT "
            " TIME_STAMP_BILL "
			" FROM "
			"  DC_SESSION_DATA "
			" WHERE "
			"  SESSION_ID = :SESSION_ID ";
        IBInternalQuery->ParamByName("SESSION_ID")->AsInteger = session_id;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
           time = IBInternalQuery->FieldByName("TIME_STAMP_BILL")->AsString;
		}
        DBTransaction.Commit();
        return time ;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
void TDrinkCommandData::fetchItemKey(int thirdPartyCodeKey , AnsiString qtyValue , AnsiString billValue , AnsiString thirdPartyCode, bool lastProduct)
{
    long itemKey = 0;
    long sizesKey = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
	try
	{

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT "
            " ITEMSIZE_KEY, "
			" ITEM_KEY "
			" FROM "
			"  ITEMSIZE "
			" WHERE "
			"  THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY ";
        IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = thirdPartyCodeKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			itemKey = IBInternalQuery->FieldByName("ITEM_KEY")->AsInteger;
            sizesKey = IBInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
            getCompleteItem(itemKey, sizesKey , qtyValue , billValue , lastProduct);
		}
        else
        {
            AnsiString message = "The Item code ";
            message += thirdPartyCode;
            message += " is not present in Menu";

            MessageBox(message,"Item Code Error", MB_OK);
        }
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}
void TDrinkCommandData::getCompleteItem(long itemKey, long itemSizeKey , AnsiString qtyValue , AnsiString billValue,bool lastProduct)
{
    ItemDetailsReceived.Occured(itemKey, itemSizeKey , qtyValue , billValue);
    if(lastProduct)
    {
        OrderComplete.Occured();
    }
}
std::vector<TOpenSessionDetails> TDrinkCommandData::GetAllDCOpenSessions()
{
      AnsiString status = "OPEN";
      TOpenSessionDetails details;

      Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
      DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =			" SELECT "
                                            " SESSION_ID, "
                                            " SESSION_TOKEN "
                                            " FROM "
                                            "  DC_SESSION_DATA "
                                            " WHERE "
                                            " SESSION_STATUS  = :SESSION_STATUS ";

      IBInternalQuery->ParamByName("SESSION_STATUS")->AsString = status;
      IBInternalQuery->ExecQuery();

      for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
      {
          details.Card_Id =  IBInternalQuery->FieldByName("SESSION_TOKEN")->AsString;
          details.Session_Id = IBInternalQuery->FieldByName("SESSION_ID")->AsInteger;
          CollectOpenSessionDetails.push_back(details);
      }
      DBTransaction.Commit();

     return CollectOpenSessionDetails;

}


