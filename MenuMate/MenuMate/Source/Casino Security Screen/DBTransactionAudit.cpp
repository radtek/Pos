//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBTransactionAudit.h"
#include <algorithm>

//---------------------------------------------------------------------------

#pragma package(smart_init)

std::list<TSecurityTransactionData> TDBTransactionAudit::GetEventsFromTheLastHour(Database::TDBTransaction &DBTransaction)
{
    TDateTime timeNow        = Now();
    TDateTime timeOneHourAgo = timeNow - EncodeTime(1, 0, 0, 0);
    std::list<TSecurityTransactionData> transactionList;

    return GetEventsBetweenTimes(DBTransaction, timeOneHourAgo, timeNow);
}

std::list<TSecurityTransactionData> TDBTransactionAudit::GetEventsSinceTime(Database::TDBTransaction &DBTransaction, TDateTime inSinceTime)
{
    return GetEventsBetweenTimes(DBTransaction, inSinceTime, Now());
}

std::list<TSecurityTransactionData> TDBTransactionAudit::GetEventsBetweenTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime)
{
    std::list<TSecurityTransactionData> transactionList;

    GetTransactionsBetweenTwoTimes(DBTransaction, inFromTime, inToTime, transactionList);
    GetFloatAdjustmentsBetweenTwoTimes(DBTransaction, inFromTime, inToTime, transactionList);
    GetOpenDrawsBetweenTwoTimes(DBTransaction, inFromTime, inToTime, transactionList);
    GetCancelItemsBetweenTwoTimes(DBTransaction, inFromTime, inToTime, transactionList);

    GetDelayedPaymentsBetweenTwoTimes(DBTransaction, inFromTime, inToTime, transactionList);
    std::stable_sort(transactionList.begin(), transactionList.end(), TSecurityTransactionData::comparator);

    return transactionList;
}

std::list<TSecurityTransactionData> TDBTransactionAudit::GetEventsSinceTheLastZed(Database::TDBTransaction &DBTransaction)
{
    std::list<TSecurityTransactionData> transactionList;

    GetTransactionsSinceTheLastZed(DBTransaction, transactionList);
    GetFloatAdjustmentsSinceTheLastZed(DBTransaction, transactionList);
    GetOpenDrawsSinceTheLastZed(DBTransaction, transactionList);

    std::stable_sort(transactionList.begin(), transactionList.end(), TSecurityTransactionData::comparator);

    return transactionList;
}

void TDBTransactionAudit::GetTransactionsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList)
{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =

       " select   part1.TIME_STAMP,  "
               " part1.INVOICE_NUMBER,  "
                "part1.BILLED_LOCATION,  "
               " part1.TERMINAL_NAME,   "
                "part1.STAFF_NAME,   "
               "part1.PAY_TYPE,  "
                "part1.AMOUNT,  "
                " part1.CASHOUT, "
                "coalesce(part1.DISCOUNT,0)+coalesce(part2.CASHOUT,0) as DISCOUNT "

                "from    "

 " (SELECT          "
               " a.TIME_STAMP,    "
               " a.INVOICE_NUMBER,    "
               " a.BILLED_LOCATION,     "
               " a.TERMINAL_NAME,     "
                "a.STAFF_NAME,    "
              " case when (b.PAY_TYPE <> 'null')   "
                     "  then b.PAY_TYPE     "
                     "  when(c.SUBTOTAL<0)    "
                      "  then 'Eftpos'  "
                        "else ''  end PAY_TYPE,    "
               " case when ((c.SUBTOTAL <0)and(b.PAY_TYPE<>'Eftpos'))   "
                      " then sum(b.SUBTOTAL+c.SUBTOTAL)  "
                     "  else b.SUBTOTAL end as AMOUNT,   "
                " abs(c.SUBTOTAL) as CASHOUT,    "
               " round(a.DISCOUNT+( d.PRICE)-coalesce(AOT.VAT,0)-coalesce(AOT.OtherServiceCharge,0)-coalesce(AOT.ServiceCharge,0),2) as DISCOUNT    "
            "FROM   "
               " DAYARCBILL a   "


              " LEFT JOIN (   "
					 " Select abc.ARCBILL_KEY, sum(abc.VAT) VAT, sum(abc.ServiceCharge) ServiceCharge ,sum(abc.OtherServiceCharge)  OtherServiceCharge from   "
 "( SELECT   "
						"arc.ARCHIVE_KEY,  arc.ARCBILL_KEY,   "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 0 THEN arc.TAX_VALUE END) AS VAT,  "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 2 THEN arc.TAX_VALUE END) AS ServiceCharge,   "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 3 THEN arc.TAX_VALUE END) AS OtherServiceCharge     "
				  "FROM (SELECT DAYARCHIVE.ARCBILL_KEY,  a.ARCHIVE_KEY,a.TAX_TYPE,   "
					   "	Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE    "
					   "	FROM DAYARCORDERTAXES a  "
					   "	left join DAYARCHIVE on DAYARCHIVE.ARCHIVE_KEY=a.ARCHIVE_KEY  "
					   "	group by DAYARCHIVE.ARCBILL_KEY, a.TAX_TYPE  , a.ARCHIVE_KEY  "
					   "	order by 1 )  arc   "
					   "	GROUP BY arc.ARCBILL_KEY, arc.ARCHIVE_KEY   "
					   "	) abc  "
					   "	group by  abc.ARCBILL_KEY ) "
						"AOT ON a.ARCBILL_KEY = AOT.ARCBILL_KEY   "

            "Left JOIN   "
               " DAYARCBILLPAY b  "
            "ON  "
                "a.ARCBILL_KEY = b.ARCBILL_KEY  "
                "AND b.CASH_OUT = 'F'   "
               " AND b.NOTE != 'Total Change.'   "
            "LEFT JOIN   "
                "DAYARCBILLPAY c   "
          "  ON   "
               " a.ARCBILL_KEY = c.ARCBILL_KEY   "
               " AND c.CASH_OUT = 'F'    "
                "AND c.NOTE = 'Total Change.'  "
          " inner  join   "
            "(select DAYARCHIVE.ARCBILL_KEY,cast(sum(dayarchive.QTY*dayarchive.PRICE)-sum(DAYARCHIVE.qty*DAYARCHIVE.BASE_PRICE)-sum(DAYARCHIVE.DISCOUNT_WITHOUT_TAX)  "
            "as numeric(15,4)) price   "
               "from DAYARCHIVE  "
                 "group by DAYARCHIVE.ARCBILL_KEY  "
               " union all    "
                "select DAYARCBILL.ARCBILL_KEY,DAYARCBILL.TOTAL*-1 price  "
               " from DAYARCBILL where DAYARCBILL.ARCBILL_KEY not in(select DAYARCHIVE.ARCBILL_KEY from DAYARCHIVE)) d on a.ARCBILL_KEY=d.ARCBILL_KEY "
                " group by    "
               " a.TIME_STAMP,  "
               " a.INVOICE_NUMBER,  "
                "a.BILLED_LOCATION,  "
                "a.TERMINAL_NAME,  "
               " a.STAFF_NAME,  "
                " b.PAY_TYPE,  "
                "b.SUBTOTAL,  "
                 "c.SUBTOTAL, "
                "a.DISCOUNT,  "
                "d.PRICE , "
               "AOT.VAT,AOT.OtherServiceCharge ,AOT.ServiceCharge )part1  "


               " left join (   "
                "Select SecAdjust.Time_Stamp ,    "
		   "	dayArcBill.Invoice_Number,   "
			"dayArchive.Order_Location BILLED_LOCATION,     "
		   "	SecAdjust.Terminal_Name,   "

			"ConOrder.Name STAFF_NAME,   "


            "DABP.PAY_TYPE,  "
		   "	cast(dayArchive.BASE_PRICE * dayArchive.Qty as numeric(17, 4)) Amount,  "
			"cast( dayArchive.BASE_PRICE * dayArchive.Qty- dayArchive.Price_Level0 * dayArchive.Qty as numeric(17, 4)) as CASHOUT,  "
			" cast(0 as int) Discount "

	   "	From   "
			"Security SecOrder Left Join dayArchive On SecOrder.Security_Ref = dayArchive.Security_Ref   "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key   "
			"Left Join dayArcBill On dayArchive.ArcBill_Key = dayArcBill.ArcBill_Key   "
			"Left Join Security SecAdjust On dayArchive.Security_Ref = SecAdjust.Security_Ref   "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key    "
		   "	left join( select distinct DAYARCBILLPAY.ARCBILL_KEY,DAYARCBILLPAY.PAY_TYPE from DAYARCBILLPAY "
			"where  DAYARCBILLPAY.NOTE != 'Total Change.')DABP on DAYARCBILL.ARCBILL_KEY=DABP.ARCBILL_KEY  "
      "Left JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM dayARCORDERDISCOUNTS a   "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)  "
	   "	dayARCORDERDISCOUNTS on dayARCHIVE.ARCHIVE_KEY = dayARCORDERDISCOUNTS.ARCHIVE_KEY  "
	  "Where  "
		     " COALESCE(dayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
            "COALESCE(dayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and   "

 //"dayARCHIVE.PRICE<>0 and   "

			"SecOrder.Security_Event = 'Ordered By' and   "

			"SecAdjust.Security_Event = 'Price Adjust' and   "
			"dayArchive.Price <> dayArchive.Price_Level0  )part2 on part1.INVOICE_NUMBER = part2.INVOICE_NUMBER  "

            "WHERE "
                "part1.TIME_STAMP > :FROM_TIME "
                "AND part1.TIME_STAMP <= :TO_TIME";


		IBInternalQuery->ParamByName("FROM_TIME")->AsDateTime = inFromTime;
		IBInternalQuery->ParamByName("TO_TIME")->AsDateTime = inToTime;
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            transaction.TransactionNo = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
            transaction.Location      = IBInternalQuery->FieldByName("BILLED_LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
            transaction.PaymentType   = IBInternalQuery->FieldByName("PAY_TYPE")->AsString;
            transaction.Amount        = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            transaction.Cashout       = IBInternalQuery->FieldByName("CASHOUT")->AsCurrency;
            transaction.Adjustment    = IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTransactionAudit::GetFloatAdjustmentsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList)
{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
            "SELECT "
                "a.TIME_STAMP, "
                "c.NAME as \"LOCATION\","
                "a.TERMINAL_NAME, "
                "a.STAFF, "
                "a.AMOUNT, "
                "a.TRANSACTION_TYPE "
            "FROM "
                "REFLOAT_SKIM a "
            "INNER JOIN "
                "DEVICES b "
            "ON "
                "a.TERMINAL_NAME = b.DEVICE_NAME "
            "INNER JOIN "
                "LOCATIONS c "
            "ON "
                "b.LOCATION_KEY = c.LOCATION_KEY "
            "WHERE "
                "a.TIME_STAMP > :FROM_TIME "
                "AND a.TIME_STAMP <= :TO_TIME";
		IBInternalQuery->ParamByName("FROM_TIME")->AsDateTime = inFromTime;
		IBInternalQuery->ParamByName("TO_TIME")->AsDateTime = inToTime;
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            if(IBInternalQuery->FieldByName("TRANSACTION_TYPE")->AsString != "Initial")
            {   transaction.TransactionNo = "Float Adjust";

             }
             else
             {   transaction.TransactionNo ="Set Float " ;
             }

            transaction.Location      = IBInternalQuery->FieldByName("LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("STAFF")->AsString;
            transaction.PaymentType   = "";
            transaction.Amount        = 0;
            transaction.Cashout       = 0;
            transaction.Adjustment    = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTransactionAudit::GetOpenDrawsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList)
{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
            "SELECT "
                "a.TIME_STAMP, "
                "c.NAME as \"LOCATION\", "
                "a.TERMINAL_NAME, "
                "d.NAME "
            "FROM "
                "SECURITY a "
            "INNER JOIN "
                "DEVICES b "
            "ON "
                "a.TERMINAL_NAME = b.DEVICE_NAME "
            "INNER JOIN "
                "LOCATIONS c "
            "ON "
                "b.LOCATION_KEY = c.LOCATION_KEY "
            "INNER JOIN "
                "CONTACTS d "
            "ON "
                "a.USER_KEY = d.CONTACTS_KEY "
            "WHERE "
                "a.SECURITY_EVENT = 'Manually Opened Cash Drawer' "
                "AND a.TIME_STAMP > :FROM_TIME "
                "AND a.TIME_STAMP <= :TO_TIME";
		IBInternalQuery->ParamByName("FROM_TIME")->AsDateTime = inFromTime;
		IBInternalQuery->ParamByName("TO_TIME")->AsDateTime = inToTime;
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            transaction.TransactionNo = "Open Drawer";
            transaction.Location      = IBInternalQuery->FieldByName("LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("NAME")->AsString;
            transaction.PaymentType   = "";
            transaction.Amount        = 0;
            transaction.Cashout       = 0;
            transaction.Adjustment    = 0;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTransactionAudit::GetTransactionsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList)
{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
              " select   part1.TIME_STAMP,  "
               " part1.INVOICE_NUMBER,  "
                "part1.BILLED_LOCATION,  "
               " part1.TERMINAL_NAME,   "
                "part1.STAFF_NAME,   "
               "part1.PAY_TYPE,  "
                "part1.AMOUNT,  "
                " part1.CASHOUT, "
                "coalesce(part1.DISCOUNT,0)+coalesce(part2.CASHOUT,0) as DISCOUNT "

                "from    "

 " (SELECT          "
               " a.TIME_STAMP,    "
               " a.INVOICE_NUMBER,    "
               " a.BILLED_LOCATION,     "
               " a.TERMINAL_NAME,     "
                "a.STAFF_NAME,    "
              " case when (b.PAY_TYPE <> 'null')   "
                     "  then b.PAY_TYPE     "
                     "  when(c.SUBTOTAL<0)    "
                      "  then 'Eftpos'  "
                        "else ''  end PAY_TYPE,    "
               " case when ((c.SUBTOTAL <0)and(b.PAY_TYPE<>'Eftpos'))   "
                      " then sum(b.SUBTOTAL+c.SUBTOTAL)  "
                     "  else b.SUBTOTAL end as AMOUNT,   "
                " abs(c.SUBTOTAL) as CASHOUT,    "
               " round(a.DISCOUNT+( d.PRICE)-coalesce(AOT.VAT,0)-coalesce(AOT.OtherServiceCharge,0)-coalesce(AOT.ServiceCharge,0),2) as DISCOUNT    "
            "FROM   "
               " DAYARCBILL a   "


              " LEFT JOIN (   "
					 " Select abc.ARCBILL_KEY, sum(abc.VAT) VAT, sum(abc.ServiceCharge) ServiceCharge ,sum(abc.OtherServiceCharge)  OtherServiceCharge from   "
 "( SELECT   "
						"arc.ARCHIVE_KEY,  arc.ARCBILL_KEY,   "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 0 THEN arc.TAX_VALUE END) AS VAT,  "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 2 THEN arc.TAX_VALUE END) AS ServiceCharge,   "
					   "	MIN(CASE WHEN arc.TAX_TYPE = 3 THEN arc.TAX_VALUE END) AS OtherServiceCharge     "
				  "FROM (SELECT DAYARCHIVE.ARCBILL_KEY,  a.ARCHIVE_KEY,a.TAX_TYPE,   "
					   "	Cast(Sum(COALESCE(a.TAX_VALUE,0) ) as Numeric(17,4)) TAX_VALUE    "
					   "	FROM DAYARCORDERTAXES a  "
					   "	left join DAYARCHIVE on DAYARCHIVE.ARCHIVE_KEY=a.ARCHIVE_KEY  "
					   "	group by DAYARCHIVE.ARCBILL_KEY, a.TAX_TYPE  , a.ARCHIVE_KEY  "
					   "	order by 1 )  arc   "
					   "	GROUP BY arc.ARCBILL_KEY, arc.ARCHIVE_KEY   "
					   "	) abc  "
					   "	group by  abc.ARCBILL_KEY ) "
						"AOT ON a.ARCBILL_KEY = AOT.ARCBILL_KEY   "

            "Left JOIN   "
               " DAYARCBILLPAY b  "
            "ON  "
                "a.ARCBILL_KEY = b.ARCBILL_KEY  "
                "AND b.CASH_OUT = 'F'   "
               " AND b.NOTE != 'Total Change.'   "
            "LEFT JOIN   "
                "DAYARCBILLPAY c   "
          "  ON   "
               " a.ARCBILL_KEY = c.ARCBILL_KEY   "
               " AND c.CASH_OUT = 'F'    "
                "AND c.NOTE = 'Total Change.'  "
          " inner  join   "
            "(select DAYARCHIVE.ARCBILL_KEY,cast(sum(dayarchive.QTY*dayarchive.PRICE)-sum(DAYARCHIVE.qty*DAYARCHIVE.BASE_PRICE)-sum(DAYARCHIVE.DISCOUNT_WITHOUT_TAX)  "
            "as numeric(15,4)) price   "
               "from DAYARCHIVE  "
                 "group by DAYARCHIVE.ARCBILL_KEY  "
               " union all    "
                "select DAYARCBILL.ARCBILL_KEY,DAYARCBILL.TOTAL*-1 price  "
               " from DAYARCBILL where DAYARCBILL.ARCBILL_KEY not in(select DAYARCHIVE.ARCBILL_KEY from DAYARCHIVE)) d on a.ARCBILL_KEY=d.ARCBILL_KEY "
                " group by    "
               " a.TIME_STAMP,  "
               " a.INVOICE_NUMBER,  "
                "a.BILLED_LOCATION,  "
                "a.TERMINAL_NAME,  "
               " a.STAFF_NAME,  "
                " b.PAY_TYPE,  "
                "b.SUBTOTAL,  "
                 "c.SUBTOTAL, "
                "a.DISCOUNT,  "
                "d.PRICE , "
               "AOT.VAT,AOT.OtherServiceCharge ,AOT.ServiceCharge )part1  "


               " left join (   "
                "Select SecAdjust.Time_Stamp ,    "
		   "	dayArcBill.Invoice_Number,   "
			"dayArchive.Order_Location BILLED_LOCATION,     "
		   "	SecAdjust.Terminal_Name,   "

			"ConOrder.Name STAFF_NAME,   "


            "DABP.PAY_TYPE,  "
		   "	cast(dayArchive.BASE_PRICE * dayArchive.Qty as numeric(17, 4)) Amount,  "
			"cast( dayArchive.BASE_PRICE * dayArchive.Qty- dayArchive.Price_Level0 * dayArchive.Qty as numeric(17, 4)) as CASHOUT,  "
			" cast(0 as int) Discount "

	   "	From   "
			"Security SecOrder Left Join dayArchive On SecOrder.Security_Ref = dayArchive.Security_Ref   "
			"Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder.User_Key   "
			"Left Join dayArcBill On dayArchive.ArcBill_Key = dayArcBill.ArcBill_Key   "
			"Left Join Security SecAdjust On dayArchive.Security_Ref = SecAdjust.Security_Ref   "
			"Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdjust.User_Key    "
		   "	left join( select distinct DAYARCBILLPAY.ARCBILL_KEY,DAYARCBILLPAY.PAY_TYPE from DAYARCBILLPAY "
			"where  DAYARCBILLPAY.NOTE != 'Total Change.')DABP on DAYARCBILL.ARCBILL_KEY=DABP.ARCBILL_KEY  "
      "Left JOIN  (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
		"FROM dayARCORDERDISCOUNTS a   "
		"group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)  "
	   "	dayARCORDERDISCOUNTS on dayARCHIVE.ARCHIVE_KEY = dayARCORDERDISCOUNTS.ARCHIVE_KEY  "
	  "Where  "
		     " COALESCE(dayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' and   "
            "COALESCE(dayARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0)<> 'Complimentary' and   "

 //"dayARCHIVE.PRICE<>0 and   "

			"SecOrder.Security_Event = 'Ordered By' and   "

			"SecAdjust.Security_Event = 'Price Adjust' and   "
			"dayArchive.Price <> dayArchive.Price_Level0  )part2 on part1.INVOICE_NUMBER = part2.INVOICE_NUMBER  ";
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            transaction.TransactionNo = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
            transaction.Location      = IBInternalQuery->FieldByName("BILLED_LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("STAFF_NAME")->AsString;
            transaction.PaymentType   = IBInternalQuery->FieldByName("PAY_TYPE")->AsString;
            transaction.Amount        = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            transaction.Cashout       = IBInternalQuery->FieldByName("CASHOUT")->AsCurrency;
            transaction.Adjustment    = IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void GetFloatAdjustmentsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList)
{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
            "SELECT "
                "a.TIME_STAMP, "
                "c.NAME as \"LOCATION\","
                "a.TERMINAL_NAME, "
                "a.STAFF, "
                "a.AMOUNT, "
                "a.TRANSACTION_TYPE "
            "FROM "
                "REFLOAT_SKIM a "
            "INNER JOIN "
                "DEVICES b "
            "ON "
                "a.TERMINAL_NAME = b.DEVICE_NAME "
            "INNER JOIN "
                "LOCATIONS c "
            "ON "
                "b.LOCATION_KEY = c.LOCATION_KEY";
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
             if(IBInternalQuery->FieldByName("TRANSACTION_TYPE")->AsString != "Initial")
            {   transaction.TransactionNo = "Float Adjust";

             }
             else
             {   transaction.TransactionNo ="Set Float " ;
             }

            transaction.Location      = IBInternalQuery->FieldByName("LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("STAFF")->AsString;
            transaction.PaymentType   = "";
            transaction.Amount        = 0;
            transaction.Cashout       = 0;
            transaction.Adjustment    = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void GetOpenDrawsSinceTheLastZed(Database::TDBTransaction &DBTransaction, std::list<TSecurityTransactionData> &outTransactionList)


{
    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
            "SELECT "
                "a.TIME_STAMP, "
                "c.NAME as \"LOCATION\", "
                "a.TERMINAL_NAME, "
                "d.NAME "
            "FROM "
                "SECURITY a "
            "INNER JOIN "
                "DEVICES b "
            "ON "
                "a.TERMINAL_NAME = b.DEVICE_NAME "
            "INNER JOIN "
                "LOCATIONS c "
            "ON "
                "b.LOCATION_KEY = c.LOCATION_KEY "
            "INNER JOIN "
                "CONTACTS d "
            "ON "
                "a.USER_KEY = d.CONTACTS_KEY "
            "WHERE "
                "a.SECURITY_EVENT = 'Manually Opened Cash Drawer'";
		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            transaction.TransactionNo = "Open Drawer";
            transaction.Location      = IBInternalQuery->FieldByName("LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("STAFF")->AsString;
            transaction.PaymentType   = "";
            transaction.Amount        = 0;
            transaction.Cashout       = 0;
            transaction.Adjustment    = 0;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTransactionAudit::GetCancelItemsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList)
{

    try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
            "SELECT "
                "a.TIME_STAMP, "
                "b.ORDER_LOCATION as \"LOCATION\", "
                "a.TERMINAL_NAME, "
                "b.REMOVE_USER, "
                "sum(b.PRICE) as Amount, "
                "b.ZED_PRINT "
            "FROM "
                "SECURITY a "
            "INNER JOIN "
                "CANCELITEMS b "
            "ON "
                "a.SECURITY_REF = b.SECURITY_REF "
            "INNER JOIN "
                "CONTACTS d "
            "ON "
                "a.USER_KEY = d.CONTACTS_KEY "
            "WHERE "
                "a.SECURITY_EVENT = 'Check Removal' "
                "AND b.ZED_PRINT = '0' "
                "AND a.TIME_STAMP > :FROM_TIME "
                "AND a.TIME_STAMP <= :TO_TIME "

                "group by  "
                "a.TIME_STAMP, "
                "b.ORDER_LOCATION, "
                "a.TERMINAL_NAME,  "
                "b.REMOVE_USER,  "
                "b.ZED_PRINT,  "
                "b.SECURITY_REF  ";


		IBInternalQuery->ParamByName("FROM_TIME")->AsDateTime = inFromTime;
		IBInternalQuery->ParamByName("TO_TIME")->AsDateTime = inToTime;

		IBInternalQuery->ExecQuery();

        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.DateTime      = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            transaction.TransactionNo = "Security Checked Removal";
            transaction.Location      = IBInternalQuery->FieldByName("LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Cashier       = IBInternalQuery->FieldByName("REMOVE_USER")->AsString;
            transaction.PaymentType   = "";
            transaction.Amount        = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            transaction.Cashout       = 0;
            transaction.Adjustment    = 0;

            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBTransactionAudit::GetDelayedPaymentsBetweenTwoTimes(Database::TDBTransaction &DBTransaction, TDateTime inFromTime, TDateTime inToTime, std::list<TSecurityTransactionData> &outTransactionList)
{
   try
    {
	 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
         IBInternalQuery->Close();
	 IBInternalQuery->SQL->Clear();
	 IBInternalQuery->SQL->Text =
                                    "SELECT  "
                                    "a.TAB_KEY,  "
                                    "sum(a.DELAYED_PAYMENT_PRICE) as AMOUNT,  "
                                    "a.TERMINAL_NAME, "
                                    "a.ORDER_LOCATION, "
                                    "a.TAB_NAME, "
                                    "sum(a.DISCOUNT) as DISCOUNT, "
                                    "a.DELAYED_INVOICE_NUMBER  "
                                    "FROM ORDERS a  "
                                    "where a.TAB_TYPE = 12 AND "
                                    "a.TIME_STAMP > :FROM_TIME AND "
                                    "a.TIME_STAMP <= :TO_TIME "
                                    "group by  "
                                    "a.TAB_KEY, "
                                    "a.TERMINAL_NAME, "
                                    "a.ORDER_LOCATION, "
                                    "a.TAB_NAME, "
                                    "a.DELAYED_INVOICE_NUMBER "
                                    "order by a.TAB_KEY desc ";
        IBInternalQuery->ParamByName("FROM_TIME")->AsDateTime = inFromTime;
	IBInternalQuery->ParamByName("TO_TIME")->AsDateTime = inToTime;
        IBInternalQuery->ExecQuery();
        AnsiString TabName = "";
        AnsiString Cashier = "";
        AnsiString Fromtable = "";
        TDateTime TimeStamp = Now();
        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TSecurityTransactionData transaction;
            transaction.TransactionNo = IBInternalQuery->FieldByName("DELAYED_INVOICE_NUMBER")->AsString;
            transaction.Location      = IBInternalQuery->FieldByName("ORDER_LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.PaymentType   = "Delayed Payment";
            transaction.Amount        = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            transaction.Cashout       = 0;
            transaction.Adjustment    = IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;
            TabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
            GetDelayedPaymentsSecurityInfo(DBTransaction,
                                           TabName,
                                           TimeStamp,
                                           Cashier,
                                           Fromtable);
            transaction.DateTime      = TimeStamp;
            transaction.Cashier       = Cashier;
            outTransactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}


}

std::list<TDelayedPaymentData> TDBTransactionAudit::GetAllDelayedPayments(Database::TDBTransaction &DBTransaction)
{
   std::list<TDelayedPaymentData> transactionList;
   try
    {
	 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
         IBInternalQuery->Close();
	 IBInternalQuery->SQL->Clear();
	 IBInternalQuery->SQL->Text =
                                    "SELECT  "
                                    "a.TAB_KEY,  "
                                    "sum(a.DELAYED_PAYMENT_PRICE) as AMOUNT,  "
                                    "a.TERMINAL_NAME, "
                                    "a.ORDER_LOCATION, "
                                    "a.TAB_NAME, "
                                    "sum(a.DISCOUNT) as DISCOUNT, "
                                    "a.DELAYED_INVOICE_NUMBER  "
                                    "FROM ORDERS a  "
                                    "where a.TAB_TYPE = 12 "
                                    "group by "
                                    "a.TAB_KEY, "
                                    "a.TERMINAL_NAME, "
                                    "a.ORDER_LOCATION, "
                                    "a.TAB_NAME, "
                                    "a.DELAYED_INVOICE_NUMBER "
                                    "order by a.TAB_KEY desc ";

        IBInternalQuery->ExecQuery();
        AnsiString TabName = "";
        AnsiString Cashier = "";
        AnsiString Fromtable = "";
        TDateTime TimeStamp = Now();
        for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TDelayedPaymentData transaction;
            transaction.TransactionNo = IBInternalQuery->FieldByName("DELAYED_INVOICE_NUMBER")->AsString;
            transaction.Location      = IBInternalQuery->FieldByName("ORDER_LOCATION")->AsString;
            transaction.Terminal      = IBInternalQuery->FieldByName("TERMINAL_NAME")->AsString;
            transaction.Amount        = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
            TabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
            GetDelayedPaymentsSecurityInfo(DBTransaction,
                                           TabName,
                                           TimeStamp,
                                           Cashier,
                                           Fromtable);
            transaction.DateTime  = TimeStamp;
            transaction.Cashier   = Cashier;
            transaction.FromTable = Fromtable;
            transactionList.push_back(transaction);
        }

	}
	catch(Exception &E)
	{
		//TODO: Come up with something to log here
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
  return transactionList;

}

void TDBTransactionAudit::GetDelayedPaymentsSecurityInfo(Database::TDBTransaction &DBTransaction,
                                                         AnsiString InvoiceNumber,
                                                         TDateTime &timestamp,
                                                         AnsiString &userName,
                                                         AnsiString &FromTable)
{
         TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
         IBInternalQuery->Close();
	 IBInternalQuery->SQL->Clear();
	 IBInternalQuery->SQL->Text =
                                    "SELECT first(1) a.TIME_STAMP,b.NAME,a.FROM_VAL  "
                                    "FROM SECURITY a inner join CONTACTS b "
                                    "on a.USER_KEY = b.CONTACTS_KEY "
                                    "where a.TO_VAL = :TO_VAL and a.SECURITY_EVENT = 'Transferred Prelim' "
                                    "order by a.TIME_STAMP asc ";
        IBInternalQuery->ParamByName("TO_VAL")->AsString = InvoiceNumber;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
        {
           timestamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
           userName = IBInternalQuery->FieldByName("NAME")->AsString;
           FromTable = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
        }
}

