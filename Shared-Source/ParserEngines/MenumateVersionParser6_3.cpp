#pragma hdrstop
#include "MenumateVersionParser.h"
#include <sstream>
#include <Predicates.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//Macro to convert a int to a string
#ifndef SSTR( x )
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
#endif
namespace MenumateVersionParser
{
void TApplyParser::upgrade6_30Tables()
{
    update6_30Tables();
}
// 6.31
void TApplyParser::upgrade6_31Tables()
{
	update6_31Tables();
}

// 6.32
void TApplyParser::upgrade6_32Tables()
{
	update6_32Tables();
}

// 6.33
void TApplyParser::upgrade6_33Tables()
{
	update6_33Tables();
}

// 6.34
void TApplyParser::upgrade6_34Tables()
{
	update6_34Tables();
}

//::::::::::::::::::::::::Version 6.30:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_30Tables()
{
  Create6_30VoidInvoiceGenerator(_dbControl);
  UpdateDayArcBillTable6_30(_dbControl);
  UpdateArcBillTable6_30(_dbControl);
}

void TApplyParser::Create6_30VoidInvoiceGenerator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_VOIDINVOICENUMBER", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_VOIDINVOICENUMBER;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_VOIDINVOICENUMBER TO 0;", inDBControl
        );
    }
}

void TApplyParser::UpdateDayArcBillTable6_30(TDBControl* const inDBControl)
{
  if (!fieldExists( "DAYARCBILL", "REFUND_REFRECEIPT", _dbControl ) )
	{
        executeQuery (
		"ALTER TABLE DAYARCBILL ADD "
        "REFUND_REFRECEIPT VARCHAR(50); ",
		inDBControl);
	}
}

void TApplyParser::UpdateArcBillTable6_30(TDBControl* const inDBControl)
{
  if (!fieldExists( "ARCBILL", "REFUND_REFRECEIPT", _dbControl ) )
	{
        executeQuery (
		"ALTER TABLE ARCBILL ADD "
        "REFUND_REFRECEIPT VARCHAR(50); ",
		inDBControl);
	}
  if (!fieldExists( "ARCBILL", "Z_KEY", _dbControl ) )
	{
        executeQuery (
		"ALTER TABLE ARCBILL ADD "
		"Z_KEY INTEGER; ",
		inDBControl);
	}
}


//::::::::::::::::::::::::Version 6.31:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_31Tables()
{
    UpdatePaymentTables6_31(_dbControl);
    UpdateContactTable6_31(_dbControl);
}

void TApplyParser::UpdatePaymentTables6_31( TDBControl* const inDBControl)
{
    if ( !fieldExists("DAYARCBILLPAY", "TIP_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCBILLPAY ADD TIP_AMOUNT NUMERIC(15,4) DEFAULT 0;",
		inDBControl );
	}
    if ( !fieldExists("ARCBILLPAY", "TIP_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCBILLPAY ADD TIP_AMOUNT NUMERIC(15,4) DEFAULT 0;",
		inDBControl );
	}
}

void TApplyParser::UpdateContactTable6_31( TDBControl* const inDBControl)
{
   executeQuery (
   "ALTER TABLE CONTACTS ALTER EMAIL TYPE VARCHAR(256);",
   inDBControl);
}

//::::::::::::::::::::::::Version 6.32::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_32Tables()
{
    UpdateDiscountsTable6_32(_dbControl);
}
//------------------------------------------------------------------------------
void TApplyParser::UpdateDiscountsTable6_32(TDBControl* const inDBControl)
{
   if ( !fieldExists( "DISCOUNTS", "IS_MEMBERSHIP_DISCOUNT", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE DISCOUNTS ADD "
        "IS_MEMBERSHIP_DISCOUNT T_TRUEFALSE DEFAULT 'F'; ",
		inDBControl);
	}
}

//---------------------------------------------------------------------------
//::::::::::::::::::::::::Version 6.33::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_33Tables()
{
    ModifyCloseZedColumns6_33(_dbControl);
    PopulateZED_StatusForContactTime6_33(_dbControl);
    ReCreateRoundedContactTimeView6_33(_dbControl);
    AlterRoundTimeProcedure6_33(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::AlterRoundTimeProcedure6_33( TDBControl* const inDBControl )
{    bool procedureExistsPredicate = procedureExists( "ROUNDTIME", _dbControl);
	std::string profileKey = "(SELECT PROFILE_KEY FROM PROFILE WHERE PROFILE_TYPE = 4 AND NAME = 'Globals')";
	std::string roundingTimesKey = "(SELECT "
	"VARSPROFILE.INTEGER_VAL "
	"FROM VARSPROFILE "
	"INNER JOIN VARIABLES ON VARSPROFILE.VARIABLES_KEY = VARIABLES.VARIABLES_KEY "
	"WHERE VARIABLES.VARIABLE_NAME LIKE '%Rounded to time%' AND PROFILE_KEY = " + profileKey + " )";
	std::string dmlStatement = procedureExistsPredicate ? "ALTER" : "CREATE";
	executeQuery(
	dmlStatement +" PROCEDURE ROUNDTIME ( inUnroundedTime Timestamp ) "
	"RETURNS ( outRoundedTime Timestamp ) "
	"AS "
	"DECLARE VARIABLE timeStampMinutes INT; "
	"DECLARE VARIABLE timeStampHours INT; "
	"DECLARE VARIABLE timeString CHAR(20); "
	"DECLARE VARIABLE minutesToRoundTo INT; "

    "DECLARE VARIABLE timeStampDay INT; "
    "DECLARE VARIABLE timeStampMonth INT; "
    "DECLARE VARIABLE timeStampYear INT; "

	"BEGIN "
	"SELECT ROUNDINGTIMES_VALUE FROM ROUNDINGTIMES WHERE ROUNDINGTIMES_KEY = " + roundingTimesKey + " INTO minutesToRoundTo; "
	"IF ( minutesToRoundTo = 0 ) THEN "
	"outRoundedTime = InUnroundedTime; "
	"ELSE "
	"BEGIN "
	"timeStampMinutes = EXTRACT ( MINUTE FROM InUnroundedTime ); "
	"timeStampMinutes = ROUND ( CAST ( timeStampMinutes as DECIMAL(18,4) ) / CAST ( minutesToRoundTo as DECIMAL(18,4) ) ) * minutesToRoundTo; "
	"timeStampHours = EXTRACT ( HOUR FROM InUnroundedTime ); "
    "timeStampDay = EXTRACT ( day FROM InUnroundedTime ); "
    "timeStampMonth = EXTRACT ( Month FROM InUnroundedTime ); "
    "timeStampYear = EXTRACT ( year FROM InUnroundedTime ); "

	"IF ( timeStampMinutes >= 60 ) THEN "
	"BEGIN "
	"timeStampMinutes = MOD ( timeStampMinutes, 60 ); "
	"timeStampHours = timeStampHours + 1; "
    "if(timeStampHours >= 24) "
    "then begin timeStampHours = 0; "
    "timeStampDay = timeStampDay+1; "
    "END "
	"END "
	"timeString = LPAD ( TRIM ( CAST ( timeStampDay as CHAR(2) ) ), 2, '0' ) || '.' || LPAD ( TRIM ( CAST ( timeStampMonth as CHAR(2) ) ), 2, '0' ) || '.' || LPAD ( TRIM ( CAST ( timeStampYear as CHAR(4) ) ), 4, '0' ) || ', ' || LPAD ( TRIM ( CAST ( timeStampHours as CHAR(2) ) ), 2, '0' ) || ':' || LPAD ( TRIM ( CAST ( timeStampMinutes as CHAR(2) ) ), 2, '0' ) || ':00';  "
	"outRoundedTime = DATEADD(SECOND, DATEDIFF(SECOND, CAST ( InUnroundedTime AS timestamp ), CAST ( timeString as timestamp ) ), InUnroundedTime); "
	"END "
	"SUSPEND; "
	"END ",
	inDBControl );
}//------


//------------------------------------------------------------------------------
void TApplyParser::PopulateZED_StatusForContactTime6_33(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TDateTime PrevZedTime;
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT MAX(ZEDS.TIME_STAMP) TIME_STAMP, PROFILE.PROFILE_KEY "
			"FROM ZEDS Left JOIN PROFILE ON ZEDS.TERMINAL_NAME = PROFILE.NAME "
			"WHERE TIME_STAMP IS NOT NULL "

         " and  TIME_STAMP IN (SELECT MAX(TIME_STAMP) FROM ZEDS where  STAFF_HOUR_ENABLE = 1)  "
			"GROUP BY TERMINAL_NAME, PROFILE_KEY "
			"ORDER BY TIME_STAMP DESC " ;
        SelectQuery->ExecQuery();
        if(SelectQuery->RecordCount)
        {
           PrevZedTime = SelectQuery->FieldByName("TIME_STAMP")->AsDateTime;
        }
        query->Close();
        query->SQL->Text = "UPDATE CONTACTTIME SET ZED_STATUS = 1 WHERE CONTACTTIME.LOGOUT_DATETIME < :ZED_TIME ";
        query->ParamByName("ZED_TIME")->AsDateTime = PrevZedTime;
        query->ExecQuery();
        query->Close();


        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
        throw;
    }
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyCloseZedColumns6_33( TDBControl* const inDBControl )
{
    if ( !fieldExists( "CONTACTTIME ", "ZED_STATUS ", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CONTACTTIME "
        "ADD ZED_STATUS Integer ; ",
        inDBControl);
    }
}

void TApplyParser::ReCreateRoundedContactTimeView6_33( TDBControl* const inDBControl )
{

    executeQuery(
        "RECREATE VIEW ROUNDEDCONTACTTIME "
        "("
            "CONTACTTIME_KEY, "
            "CONTACTS_KEY, "
            "LOGIN_DATETIME, "
            "ROUNDED_LOGIN_DATETIME, "
            "ROUNDED_LOGOUT_DATETIME, "
            "MODIFIED, "
            "EXPORTED, "
            "TIMECLOCKLOCATIONS_KEY, "
            "TOTALHOURS, "
            "BREAKS, "
            "ZED_STATUS "
        ") "
        "AS "
        "SELECT "
            "CONTACTTIME_KEY, "
            "CONTACTS_KEY, "
            "LOGIN_DATETIME, "
            "(SELECT OUTROUNDEDTIME FROM ROUNDTIME( CONTACTTIME.LOGIN_DATETIME  )) AS ROUNDED_LOGIN_DATETIME, "
            "(SELECT OUTROUNDEDTIME FROM ROUNDTIME( CONTACTTIME.LOGOUT_DATETIME )) AS ROUNDED_LOGOUT_DATETIME, "
            "MODIFIED, "
            "EXPORTED, "
            "TIMECLOCKLOCATIONS_KEY, "
            "TOTALHOURS, "
            "BREAKS, "
            "ZED_STATUS "
        "FROM "
            "CONTACTTIME",
        inDBControl );

}

//::::::::::::::::::::::::Version 6.33::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_34Tables()
{
   UpdateZedTable6_34(_dbControl);
}

//---------------------------------------------------------------------------
void TApplyParser::UpdateZedTable6_34( TDBControl* const inDBControl )
{
    if ( !fieldExists( "ZEDS", "TRANS_DATE", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ZEDS "
        "ADD TRANS_DATE TIMESTAMP ",
        inDBControl);
    }
}

}

