//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBSaleTimes.h"
#include "SecurityReference.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int TDBSaleTimes::GetTimeKey(Database::TDBTransaction &DBTransaction)
{
   int RetVal = 0;

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TURN_AROUND_TIMES, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;

   return RetVal;
}

int TDBSaleTimes::OpenSaleStartTime(Database::TDBTransaction &DBTransaction,int CurrentTimeKey)
{
   int RetVal = CurrentTimeKey;
   if(RetVal == 0)
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		RetVal = GetTimeKey(DBTransaction);
      if(RetVal != 0)
      {
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
         "INSERT INTO TURNAROUNDTIMES ("
            "TIME_KEY,"
            "SALE_START_TIME) "
         "VALUES ("
            ":TIME_KEY,"
            ":SALE_START_TIME);";
         IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = RetVal;
         IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = Now();
         IBInternalQuery->ExecQuery();
      }

   }
   return RetVal;
}

void TDBSaleTimes::CloseSaleStartTime(Database::TDBTransaction &DBTransaction,int TimeKey)
{
   if(TimeKey != 0)
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE TURNAROUNDTIMES "
      "SET "
         "SALE_END_TIME = :SALE_END_TIME, "
         "MAKE_START_TIME = :MAKE_START_TIME "
      "WHERE "
         "TIME_KEY	= :TIME_KEY";
      IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
      IBInternalQuery->ParamByName("SALE_END_TIME")->AsDateTime = Now();
      IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = Now();
		IBInternalQuery->ExecQuery();

   }
}

void TDBSaleTimes::CloseMakeStartTime(Database::TDBTransaction &DBTransaction,int TimeKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   if(TimeKey != 0)
   {
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE TURNAROUNDTIMES "
      "SET "
         "MAKE_END_TIME = :MAKE_END_TIME "
      "WHERE "
         "TIME_KEY	= :TIME_KEY";
      IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
      IBInternalQuery->ParamByName("MAKE_END_TIME")->AsDateTime = Now();
		IBInternalQuery->ExecQuery();
   }

}


void TDBSaleTimes::VoidSaleTime(Database::TDBTransaction &DBTransaction,int TimeKey)
{
   if(TimeKey != 0)
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "DELETE FROM TURNAROUNDTIMES "
      "WHERE "
         "TIME_KEY	= :TIME_KEY";
      IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
      IBInternalQuery->ExecQuery();
	}
}

double TDBSaleTimes::GetAverageWaitTimeMins(Database::TDBTransaction &DBTransaction)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   TDateTime Total;

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
      "SELECT "
      " AVG (MAKE_END_TIME - SALE_START_TIME) AVGERAGE"
      " FROM"
      " TURNAROUNDTIMES"
      " WHERE SALE_START_TIME IS NOT NULL AND"
      " MAKE_END_TIME IS NOT NULL AND"
      " SALE_START_TIME > :SALE_START_TIME";
	IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = (Now() - double(1.0/24.0/4.0));
	IBInternalQuery->ExecQuery();
   Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;

   if(double(Total) == 0.0)
   {
		IBInternalQuery->Close();
		IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = (Now() - 1);
      IBInternalQuery->ExecQuery();
      Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;
   }
   unsigned short hour = 0, min = 0, sec = 0, msec = 0;
   Total.DecodeTime(&hour,&min,&sec,&msec);


	return double((hour * 60) + (min));
}

TDateTime TDBSaleTimes::GetSalesAverage(Database::TDBTransaction &DBTransaction)
{
   TDateTime From;
   TDateTime Total;

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
	"SELECT MAX(TIME_STAMP)TIME_STAMP FROM ZEDS";
	IBInternalQuery->ExecQuery();

	From = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
		" AVG (SALE_END_TIME - SALE_START_TIME) AVGERAGE"
		" FROM"
		" TURNAROUNDTIMES"
		" WHERE SALE_START_TIME IS NOT NULL AND"
		" SALE_END_TIME IS NOT NULL AND"
		" SALE_START_TIME > :SALE_START_TIME";

	IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = From;
	IBInternalQuery->ExecQuery();

	Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;

	return Total;
}

TDateTime TDBSaleTimes::GetMakeAverage(Database::TDBTransaction &DBTransaction)
{
   TDateTime From;
   TDateTime Total;


   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
	"SELECT MAX(TIME_STAMP)TIME_STAMP FROM ZEDS";
	IBInternalQuery->ExecQuery();

	From = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
		" AVG (MAKE_END_TIME - MAKE_START_TIME) AVGERAGE"
		" FROM"
		" TURNAROUNDTIMES"
		" WHERE MAKE_START_TIME IS NOT NULL AND"
		" MAKE_END_TIME IS NOT NULL AND"
		" MAKE_START_TIME > :MAKE_START_TIME";

	IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = From;
	IBInternalQuery->ExecQuery();

	Total = IBInternalQuery->FieldByName("AVGERAGE")->AsFloat;

	return Total;
}

bool TDBSaleTimes::GetLongestSaleTrans(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,
				UnicodeString &Qty,UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val)
{

   bool retVal = false;
	try
   {
      TDateTime Total;
      TDateTime From;


      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		"SELECT MAX(TIME_STAMP)TIME_STAMP FROM ZEDS";
		IBInternalQuery->ExecQuery();

		From = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
      /*"SELECT TIME_KEY,SALE_START_TIME, SALE_END_TIME, (SALE_END_TIME - SALE_START_TIME) DURATION "
      " FROM TURNAROUNDTIMES "
      " WHERE SALE_START_TIME IS NOT NULL AND "
      " SALE_END_TIME IS NOT NULL AND"
      " SALE_START_TIME > :SALE_START_TIME"
      " ORDER BY 4 DESC";*/
        " SELECT a.TIME_KEY,SALE_START_TIME, SALE_END_TIME, (SALE_END_TIME - SALE_START_TIME) DURATION from DAYARCHIVE a "
        " left join DAYARCORDERDISCOUNTS b "
        " on a.ARCHIVE_KEY = b.ARCHIVE_KEY "
        " left join TURNAROUNDTIMES c  "
        " on a.TIME_KEY = c.TIME_KEY  "
        " where (( a.DISCOUNT = 0  ) or b.DISCOUNT_KEY > 0 and b.DISCOUNT_GROUPNAME <> 'Non-Chargeable' ) and "
        " c.SALE_START_TIME IS NOT NULL AND "
        " c.SALE_END_TIME IS NOT NULL AND"
        " c.SALE_START_TIME > :SALE_START_TIME"
        " group by 1,2,3,4 "
        " ORDER BY 4 DESC";

      IBInternalQuery->ParamByName("SALE_START_TIME")->AsDateTime = From;
      IBInternalQuery->ExecQuery();

      int i = 0;
      for (; i < Offest ; i++)
      {
         IBInternalQuery->Next();
      }
      retVal = IBInternalQuery->Eof;
		if(!IBInternalQuery->Eof && IBInternalQuery->RecordCount && (i == Offest))
      {
         int TimeKey = 0;
         TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
         TDateTime Temp = IBInternalQuery->FieldByName("DURATION")->AsFloat;
         unsigned short hour = 0, min = 0, sec = 0, msec = 0;
         Temp.DecodeTime(&hour,&min,&sec,&msec);
			Dur = /*IntToStr(hour)+ ":" + */IntToStr(min)+ ":" + IntToStr(sec);
			TDateTime Average =  GetSalesAverage(DBTransaction);
	      TDateTime AvgDiff = Temp - Average;
         AvgDiff.DecodeTime(&hour,&min,&sec,&msec);
         if(Temp > Average)
         {
         	Avg = "+" + IntToStr(min)+ ":" + IntToStr(sec);
         }
         else
         {
         	Avg = "-" + IntToStr(min)+ ":" + IntToStr(sec);
         }
			// Quantiy of Items.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) AMOUNT FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			Qty = IntToStr(IBInternalQuery->FieldByName("AMOUNT")->AsInteger);

			// Value of Items.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT SUM(PRICE) TOTAL FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			Val = FloatToStrF(IBInternalQuery->FieldByName("TOTAL")->AsFloat,ffNumber,15,2);
			//Val = FloatToStrF(IBInternalQuery->FieldByName("TOTAL")->AsFloat,ffCurrency,15,2);

			// Security Ref (Used to Find Operator)
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT SECURITY_REF,ARCBILL_KEY FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			int SecRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
			int ArcBillKey = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;

			// Operator
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT TO_VAL FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT" ;
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecRef;
			IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
			IBInternalQuery->ExecQuery();
			Opr = IBInternalQuery->FieldByName("TO_VAL")->AsString;

			// Docket Number.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			//IBInternalQuery->SQL->Text = "SELECT INVOICE_NUMBER FROM DAYARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY";
			IBInternalQuery->SQL->Text = " select INVOICE_NUMBER,ARCBILL_KEY from ( "
                                         " select a.ARCBILL_KEY ,a.INVOICE_NUMBER from DAYARCBILL a  "
                                         " left join DAYARCHIVE b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                         " left join DAYARCORDERDISCOUNTS c on b.ARCHIVE_KEY = c.ARCHIVE_KEY "
                                         " where (( a.DISCOUNT = 0 ) or c.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and c.DISCOUNTED_VALUE <> 0 )"
                                         " group by 1,2) where ARCBILL_KEY = :ARCBILL_KEY ";
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			IBInternalQuery->ExecQuery();
            Doc = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;

		}
		else
		{
			Doc = "";
			Opr = "";
			Qty = "";
			Dur = "";
			Avg = "";
			Val = "";
		}

	}
	catch (Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
      Doc = "";
      Opr = "";
      Qty = "";
      Dur = "";
      Avg = "";
      Val = "";
   }
   return retVal;
}

void TDBSaleTimes::GetLongestMakeTrans(Database::TDBTransaction &DBTransaction,int Offest,UnicodeString &Doc,UnicodeString &Opr,
				UnicodeString &Qty,UnicodeString &Dur,UnicodeString &Avg,UnicodeString &Val)
{
	try
   {
      TDateTime From;
      TDateTime Total;


      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
      "SELECT MAX(TIME_STAMP)TIME_STAMP FROM ZEDS";
		IBInternalQuery->ExecQuery();

		From = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;

		IBInternalQuery->Close();
      IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
      "SELECT TIME_KEY,MAKE_START_TIME, MAKE_END_TIME, (MAKE_END_TIME - MAKE_START_TIME) DURATION "
      " FROM TURNAROUNDTIMES "
      " WHERE MAKE_START_TIME IS NOT NULL AND "
      " MAKE_END_TIME IS NOT NULL AND"
      " MAKE_START_TIME > :MAKE_START_TIME"
      " ORDER BY 4 DESC";

      IBInternalQuery->ParamByName("MAKE_START_TIME")->AsDateTime = From;
      IBInternalQuery->ExecQuery();

      int i = 0;
      for (; i < Offest ; i++)
      {
         IBInternalQuery->Next();
      }

		if(!IBInternalQuery->Eof && IBInternalQuery->RecordCount && (i == Offest))
		{
         int TimeKey = 0;
         TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
         TDateTime Temp = IBInternalQuery->FieldByName("DURATION")->AsFloat;
         unsigned short hour = 0, min = 0, sec = 0, msec = 0;
         Temp.DecodeTime(&hour,&min,&sec,&msec);
         Dur = /*IntToStr(hour)+ ":" + */IntToStr(min)+ ":" + IntToStr(sec);
			TDateTime Average =  GetMakeAverage(DBTransaction);
	      TDateTime AvgDiff = Temp - Average;
         AvgDiff.DecodeTime(&hour,&min,&sec,&msec);
         if(Temp > Average)
         {
         	Avg = "+" + IntToStr(min)+ ":" + IntToStr(sec);
         }
         else
         {
         	Avg = "-" + IntToStr(min)+ ":" + IntToStr(sec);
         }


			// Quantiy of Items.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT COUNT(ARCHIVE_KEY) AMOUNT FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			Qty = IntToStr(IBInternalQuery->FieldByName("AMOUNT")->AsInteger);

			// Value of Items.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT SUM(PRICE) TOTAL FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			Val = FloatToStrF(IBInternalQuery->FieldByName("TOTAL")->AsFloat,ffCurrency,15,2);

			// Security Ref (Used to Find Operator)
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT SECURITY_REF,ARCBILL_KEY FROM DAYARCHIVE WHERE TIME_KEY = :TIME_KEY" ;
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
			IBInternalQuery->ExecQuery();
			int SecRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
			int ArcBillKey = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;

			// Operator
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT TO_VAL FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT" ;
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecRef;
			IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecurityTypes[secOrderedBy];
			IBInternalQuery->ExecQuery();
			Opr = IBInternalQuery->FieldByName("TO_VAL")->AsString;

			// Docket Number.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "SELECT INVOICE_NUMBER FROM DAYARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY";
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
			IBInternalQuery->ExecQuery();
			Doc = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		}
		else
		{
			Doc = "";
			Opr = "";
			Qty = "";
			Dur = "";
			Avg = "";
			Val = "";
		}

	}
	catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		Doc = "";
		Opr = "";
		Qty = "";
		Dur = "";
		Avg = "";
		Val = "";
	}
}

TDateTime TDBSaleTimes::GetSaleStartTimeFromTimeKey(Database::TDBTransaction &DBTransaction, int inTimeKey)
{
	TDateTime SaleStartTime;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Clear();
	IBInternalQuery->SQL->Text =
		"SELECT "
			"SALE_START_TIME "
		"FROM "
			"TURNAROUNDTIMES "
		"WHERE "
			"TIME_KEY = :TIME_KEY";

	IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = inTimeKey;
	IBInternalQuery->ExecQuery();

	SaleStartTime = IBInternalQuery->FieldByName("SALE_START_TIME")->AsDateTime;

	return SaleStartTime;
}
