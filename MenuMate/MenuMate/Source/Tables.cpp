//---------------------------------------------------------------------------


#pragma hdrstop

#include "Tables.h"
#include "DBTables.h"
#include "NetMessageSeatName.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TTables::SetSeatName(Database::TDBControl &IBDatabase,UnicodeString Name, int TableNo, int SeatNumber)
{
	try
	{
		if (TDBTables::Valid(TableNo) && TDBTables::Valid(SeatNumber))
		{
			TNetMessageSeatName *Request = new TNetMessageSeatName;

			Database::TDBTransaction DBTransaction(IBDatabase);
			DBTransaction.StartTransaction();
         TDBTables::SetSeatName(DBTransaction,Name,TableNo,SeatNumber);
			Request->TabLK = TDBTables::GetTabKey(DBTransaction,TableNo,SeatNumber);
			DBTransaction.Commit();
			try
			{
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
