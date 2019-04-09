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
//6.60
void TApplyParser::upgrade6_60Tables()
{
    update6_60Tables();
}

//6.61
void TApplyParser::upgrade6_61Tables()
{
    update6_61Tables();
}
//6.62
void TApplyParser::upgrade6_62Tables()
{
    update6_62Tables();
}

//::::::::::::::::::::::::Version 6.60:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_60Tables()
{
    Alter6_60Tables(_dbControl);

}
//------------------------version 6.61---------------------------------------
void TApplyParser::update6_61Tables()
{
    Alter6_61Tables(_dbControl);

}
//------------------------version 6.62---------------------------------------
void TApplyParser::update6_62Tables()
{
    Alter6_62Tables(_dbControl);
    Create6_62Table(_dbControl);
    Create6_62Generator(_dbControl);

}

//---------------------------------------------------------------------------
void TApplyParser::Alter6_60Tables(TDBControl* const inDBControl)
{
    if (!fieldExists( "TABLES", "ACCEPT_OO", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE TABLES ADD ACCEPT_OO CHAR(1) DEFAULT 'F';", inDBControl);
	}
     if ( fieldExists( "TABLES", "ACCEPT_OO", _dbControl ) )
    {
        executeQuery ( "UPDATE TABLES SET ACCEPT_OO = 'F';", inDBControl);
    }
}

//---------------------------------------------------------------------------
void TApplyParser::Alter6_61Tables(TDBControl* const inDBControl)
{
    ALTERDSRPIVOTProcedure6_61( _dbControl ) ;
    ALTERDSR_PIVOT_BY_ITEMProcedure6_61( _dbControl ) ;
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::ALTERDSRPIVOTProcedure6_61( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"ALTER PROCEDURE CREATE_DSR_PIVOT ( "
		"ATABLENAME Varchar(50), "
		"STARTTIME Timestamp, "
		"ENDTIME Timestamp ) "
		"RETURNS ( "
		"ASTMNT Varchar(8000) ) "
		"AS "
		"DECLARE VARIABLE StmtArcBill VARCHAR(8000); "
		"DECLARE VARIABLE StmtMenuName VARCHAR(8000); "
		"DECLARE VARIABLE StmtArcOrderTax VARCHAR(8000); "
		"DECLARE VARIABLE MenuDiscountStmt VARCHAR(8000); "
		"DECLARE VARIABLE ArcBillPay VARCHAR(50); "
		"DECLARE VARIABLE MenuName VARCHAR(50); "
		"DECLARE VARIABLE ArcOrderTax VARCHAR(50);  "
		"DECLARE VARIABLE MenuDiscountCol VARCHAR(50);  "
		"DECLARE VARIABLE colname VARCHAR(50); "
		"DECLARE VARIABLE MenuDiscountColData VARCHAR(50); "
		"BEGIN StmtArcBill = '';  "
		"for "
		"SELECT distinct REPLACE ( upper(ARCBILL_PAY.PAY_TYPE), ' ', '') "
		"FROM ARCBILLPAY ARCBILL_PAY left join arcbill on  ARCBILL.ARCBILL_KEY=ARCBILL_PAY.ARCBILL_KEY  "
		"where ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime  "
		"order by 1 into ArcBillPay "
		"do begin "
		"ArcBillPay=REPLACE ( ArcBillPay, '(', ''); "
		"ArcBillPay=REPLACE ( ArcBillPay, ')', ''); "
        "ArcBillPay=REPLACE (ArcBillPay, '''','');  "
		"if (ArcBillPay <> '') "
		"then "
		"StmtArcBill = StmtArcBill || ', ' || REPLACE (ArcBillPay, ' ', '') || ' VARCHAR(50)'; "
		"end "
		"StmtArcBill = substring(StmtArcBill from 2); "
		"StmtMenuName = ''; "
		"for "
		"SELECT distinct upper(a.MENU_NAME) FROM ARCHIVE a "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=a.ARCBILL_KEY "
		"where a.MENU_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1 "
		"into MenuName "
		"do begin "
		"MenuName=REPLACE ( MenuName, '(', ''); "
		"MenuName=REPLACE ( MenuName, ')', ''); "
        "MenuName=REPLACE (MenuName, '''','');  "
		"if (MenuName <> '') "
		"then "
		"StmtMenuName = StmtMenuName || ', ' ||REPLACE ( REPLACE (REPLACE (MenuName, ' ', ''), '.', '' ),'-','' ) || ' VARCHAR(50)'; "
		"end "
		"StmtMenuName = substring(StmtMenuName from 2); "
		"MenuDiscountStmt = ''; "
		"for "
		"SELECT distinct upper(a.MENU_NAME) FROM ARCHIVE a "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=a.ARCBILL_KEY "
		"where a.MENU_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1 "
		"into MenuDiscountCol "
		"do begin "
		"MenuDiscountCol=REPLACE ( MenuDiscountCol, '(', ''); "
		"MenuDiscountCol=REPLACE ( MenuDiscountCol, ')', ''); "
         "MenuDiscountCol=REPLACE (MenuDiscountCol, '''','');  "
		"if (MenuDiscountCol <> '')  "
		"then MenuDiscountColData='_Discount_Amount'; "
		"if (MenuDiscountCol <> '')  "
		"then "
		"MenuDiscountStmt = MenuDiscountStmt|| ', ' ||REPLACE ( REPLACE (REPLACE (MenuDiscountCol, ' ', ''), '.', '' ),'-','' )|| ''|| MenuDiscountColData || ' VARCHAR(50)'; "
		"if (MenuDiscountCol <> '')  "
		"then  "
		"MenuDiscountColData='_Surcharge_Amount'; "
		"if (MenuDiscountCol <> '') "
		"then MenuDiscountStmt = MenuDiscountStmt|| ', ' ||REPLACE ( REPLACE (REPLACE (MenuDiscountCol, ' ', ''), '.', '' ),'-','' ) || ''|| MenuDiscountColData || ' VARCHAR(50)';  "
		"end "
		"MenuDiscountStmt = substring(MenuDiscountStmt from 2); "
		"StmtArcOrderTax = ''; "
		"for  "
		"SELECT  distinct upper(a.TAX_NAME) FROM ARCORDERTAXES a  "
		"left join ARCHIVE on  ARCHIVE.ARCHIVE_KEY=a.ARCHIVE_KEY  "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=ARCHIVE.ARCBILL_KEY  "
		"where a.TAX_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1  "
		"into  "
		"ArcOrderTax  "
		"do begin  "
		"ArcOrderTax=REPLACE ( ArcOrderTax, '(', ''); "
		"ArcOrderTax=REPLACE ( ArcOrderTax, ')', ''); "
        "ArcOrderTax=REPLACE (ArcOrderTax, '''','');  "
		"if (ArcOrderTax <> '') then ArcOrderTax='Tax_' ||ArcOrderTax;  "
		"StmtArcOrderTax = StmtArcOrderTax || ', ' ||REPLACE (ArcOrderTax, ' ', '') || ' VARCHAR(50)'; "
		"StmtArcOrderTax =REPLACE (StmtArcOrderTax,',  VARCHAR(50),',''); "
		"end  "
		"if (exists(select 1 from RDB$RELATIONS where RDB$RELATION_NAME = 'DSRREPORT' ))  "
		"THEN BEGIN  "
		"AStmnt = 'drop table DSRREPORT;'; "
		"execute statement AStmnt; "
		"END "
		"Astmnt = 'CREATE TABLE '||ATableName||' (SRNO Integer, Location VARCHAR(50) , INVOICE_NUMBER  VARCHAR(50) ,TIME_STAMP Timestamp ,BILLAMOUNT  Numeric(15,4) , ' || StmtMenuName || ',' || MenuDiscountStmt || ', '  ||StmtArcOrderTax || ',SUB_TOTAL_TAX Numeric(15,4),TOTAL_AMOUNT Numeric(15,4),' || StmtArcBill || ');'; "
		"AStmnt=REPLACE (AStmnt, ', ,,', ',');  "
		"AStmnt=REPLACE (AStmnt, ',,', ','); "
		"AStmnt=REPLACE (AStmnt, ',)', ')');  "
		"AStmnt=REPLACE (AStmnt, ', ,', ','); "
		"AStmnt=REPLACE (AStmnt, ',)', ')');  "
		"AStmnt=REPLACE (AStmnt, '.', ''); "
		"AStmnt=REPLACE (AStmnt, '%', ''); "
		"AStmnt=REPLACE ( AStmnt, '.', ''); "
		"AStmnt=REPLACE ( AStmnt, '<', '');  "
		"AStmnt=REPLACE ( AStmnt, '>', '');  "
		"AStmnt=REPLACE ( AStmnt, '?', '');  "
		"AStmnt=REPLACE ( AStmnt, '^', '');  "
		"AStmnt=REPLACE ( AStmnt, '&', ''); "
		"AStmnt=REPLACE ( AStmnt, '-', '');  "
		"AStmnt=REPLACE ( AStmnt, '--', '');  "
		"AStmnt=REPLACE ( AStmnt, '!', ''); "
		"AStmnt=REPLACE ( AStmnt, '*', ''); "
		"AStmnt=REPLACE ( AStmnt, '#', ''); "
		"AStmnt=REPLACE ( AStmnt, '+', ''); "
		"AStmnt=REPLACE ( AStmnt, '{', ''); "
		"AStmnt=REPLACE ( AStmnt, '[', ''); "
		"AStmnt=REPLACE ( AStmnt, '', ''); "
		"AStmnt=REPLACE ( AStmnt, '/', ''); "
		"AStmnt=REPLACE ( AStmnt, '!', '');  "
		"AStmnt=REPLACE ( AStmnt, ':', ''); "
		"AStmnt=REPLACE ( AStmnt, ';', ''); "
		"AStmnt=REPLACE (AStmnt, '0.00%', '');  "
		"AStmnt=REPLACE (AStmnt, '000%', ''); "
		"AStmnt=REPLACE (AStmnt, '@', ''); "
		"AStmnt=REPLACE (AStmnt, '000', ''); "
		"AStmnt=REPLACE (AStmnt, '00', '');  "
		"AStmnt=REPLACE (AStmnt, ', VARCHAR(50),',''); "
		"AStmnt=REPLACE (AStmnt, ',  VARCHAR(50),','');   "
        "AStmnt=REPLACE (AStmnt, '''','');  "
		"execute statement AStmnt;  "
		"SUSPEND;   "
		"END ",	inDBControl );
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}
//-------------------------------------------------------------------------------------------------------------------------
void TApplyParser::ALTERDSR_PIVOT_BY_ITEMProcedure6_61( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"ALTER PROCEDURE CREATE_DSR_BY_ITEM ( "
		"ATABLENAME Varchar(50), "
		"STARTTIME Timestamp, "
		"ENDTIME Timestamp ) "
		"RETURNS ( "
		"ASTMNT Varchar(8000) ) "
		"AS "
		"DECLARE VARIABLE StmtArcBill VARCHAR(8000); "
		"DECLARE VARIABLE StmtMenuName VARCHAR(8000); "
		"DECLARE VARIABLE StmtArcOrderTax VARCHAR(8000); "
		"DECLARE VARIABLE MenuDiscountStmt VARCHAR(8000); "
		"DECLARE VARIABLE ArcBillPay VARCHAR(50); "
		"DECLARE VARIABLE MenuName VARCHAR(50); "
		"DECLARE VARIABLE ArcOrderTax VARCHAR(50);  "
		"DECLARE VARIABLE MenuDiscountCol VARCHAR(50);  "
		"DECLARE VARIABLE colname VARCHAR(50); "
		"DECLARE VARIABLE MenuDiscountColData VARCHAR(50); "
		"BEGIN StmtArcBill = '';  "
		"for "
		"SELECT distinct REPLACE ( upper(ARCBILL_PAY.PAY_TYPE), ' ', '') "
		"FROM ARCBILLPAY ARCBILL_PAY left join arcbill on  ARCBILL.ARCBILL_KEY=ARCBILL_PAY.ARCBILL_KEY  "
		"where ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime  "
		"order by 1 into ArcBillPay "
		"do begin "
		"ArcBillPay=REPLACE ( ArcBillPay, '(', ''); "
		"ArcBillPay=REPLACE ( ArcBillPay, ')', ''); "
        "ArcBillPay=REPLACE (ArcBillPay, '''','');  "
		"if (ArcBillPay <> '') "
		"then "
		"StmtArcBill = StmtArcBill || ', ' || REPLACE (ArcBillPay, ' ', '') || ' VARCHAR(50)'; "
		"end "
		"StmtArcBill = substring(StmtArcBill from 2); "
		"StmtMenuName = ''; "
		"for "
		"SELECT distinct upper(a.MENU_NAME) FROM ARCHIVE a "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=a.ARCBILL_KEY "
		"where a.MENU_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1 "
		"into MenuName "
		"do begin "
		"MenuName=REPLACE ( MenuName, '(', ''); "
		"MenuName=REPLACE ( MenuName, ')', ''); "
        "MenuName=REPLACE (MenuName, '''',''); "
		"if (MenuName <> '') "
		"then "
		"StmtMenuName = StmtMenuName || ', ' ||REPLACE ( REPLACE (REPLACE (MenuName, ' ', ''), '.', '' ),'-','' ) || ' VARCHAR(50)'; "
		"end "
		"StmtMenuName = substring(StmtMenuName from 2); "
		"MenuDiscountStmt = ''; "
		"for "
		"SELECT distinct upper(a.MENU_NAME) FROM ARCHIVE a "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=a.ARCBILL_KEY "
		"where a.MENU_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1 "
		"into MenuDiscountCol "
		"do begin "
		"MenuDiscountCol=REPLACE ( MenuDiscountCol, '(', ''); "
		"MenuDiscountCol=REPLACE ( MenuDiscountCol, ')', ''); "
        "MenuDiscountCol=REPLACE (MenuDiscountCol, '''','');  "
		"if (MenuDiscountCol <> '')  "
		"then MenuDiscountColData='_Discount_Amount'; "
		"if (MenuDiscountCol <> '')  "
		"then "
		"MenuDiscountStmt = MenuDiscountStmt|| ', ' ||REPLACE ( REPLACE (REPLACE (MenuDiscountCol, ' ', ''), '.', '' ),'-','' )|| ''|| MenuDiscountColData || ' VARCHAR(50)'; "
		"if (MenuDiscountCol <> '')  "
		"then  "
		"MenuDiscountColData='_Surcharge_Amount'; "
		"if (MenuDiscountCol <> '') "
		"then MenuDiscountStmt = MenuDiscountStmt|| ', ' ||REPLACE ( REPLACE (REPLACE (MenuDiscountCol, ' ', ''), '.', '' ),'-','' ) || ''|| MenuDiscountColData || ' VARCHAR(50)';  "
		"end "
		"MenuDiscountStmt = substring(MenuDiscountStmt from 2); "
		"StmtArcOrderTax = ''; "
		"for  "
		"SELECT  distinct upper(a.TAX_NAME) FROM ARCORDERTAXES a  "
		"left join ARCHIVE on  ARCHIVE.ARCHIVE_KEY=a.ARCHIVE_KEY  "
		"left join arcbill on  ARCBILL.ARCBILL_KEY=ARCHIVE.ARCBILL_KEY  "
		"where a.TAX_NAME<>'' and ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime "
		"order by 1  "
		"into  "
		"ArcOrderTax  "
		"do begin  "
		"ArcOrderTax=REPLACE ( ArcOrderTax, '(', ''); "
		"ArcOrderTax=REPLACE ( ArcOrderTax, ')', ''); "
        "ArcOrderTax=REPLACE (ArcOrderTax, '''',''); "
		"if (ArcOrderTax <> '') then ArcOrderTax='Tax_' ||ArcOrderTax;  "
		"StmtArcOrderTax = StmtArcOrderTax || ', ' ||REPLACE (ArcOrderTax, ' ', '') || ' VARCHAR(50)'; "
		"StmtArcOrderTax =REPLACE (StmtArcOrderTax,',  VARCHAR(50),',''); "
		"end  "
		"if (exists(select 1 from RDB$RELATIONS where RDB$RELATION_NAME = 'DSRREPORTBYITEM' ))  "
		"THEN BEGIN  "
		"AStmnt = 'drop table DSRREPORTBYITEM;'; "
		"execute statement AStmnt; "
		"END "
		"Astmnt = 'CREATE TABLE '||ATableName||' (SRNO Integer,  INVOICE_NUMBER  VARCHAR(50) ,TIME_STAMP Timestamp , ITEMNAME VARCHAR(50) , BILLAMOUNT  Numeric(15,4) , ' || StmtMenuName || ',' || MenuDiscountStmt || ', '  ||StmtArcOrderTax || ',SUB_TOTAL_TAX Numeric(15,4),TOTAL_AMOUNT Numeric(15,4),' || StmtArcBill || ');'; "
		"AStmnt=REPLACE (AStmnt, ', ,,', ',');  "
		"AStmnt=REPLACE (AStmnt, ',,', ','); "
		"AStmnt=REPLACE (AStmnt, ',)', ')');  "
		"AStmnt=REPLACE (AStmnt, ', ,', ','); "
		"AStmnt=REPLACE (AStmnt, ',)', ')');  "
		"AStmnt=REPLACE (AStmnt, '.', ''); "
		"AStmnt=REPLACE (AStmnt, '%', ''); "
		"AStmnt=REPLACE ( AStmnt, '.', ''); "
		"AStmnt=REPLACE ( AStmnt, '<', '');  "
		"AStmnt=REPLACE ( AStmnt, '>', '');  "
		"AStmnt=REPLACE ( AStmnt, '?', '');  "
		"AStmnt=REPLACE ( AStmnt, '^', '');  "
		"AStmnt=REPLACE ( AStmnt, '&', ''); "
		"AStmnt=REPLACE ( AStmnt, '-', '');  "
		"AStmnt=REPLACE ( AStmnt, '--', '');  "
		"AStmnt=REPLACE ( AStmnt, '!', ''); "
		"AStmnt=REPLACE ( AStmnt, '*', ''); "
		"AStmnt=REPLACE ( AStmnt, '#', ''); "
		"AStmnt=REPLACE ( AStmnt, '+', ''); "
		"AStmnt=REPLACE ( AStmnt, '{', ''); "
		"AStmnt=REPLACE ( AStmnt, '[', ''); "
		"AStmnt=REPLACE ( AStmnt, '', ''); "
		"AStmnt=REPLACE ( AStmnt, '/', ''); "
		"AStmnt=REPLACE ( AStmnt, '!', '');  "
		"AStmnt=REPLACE ( AStmnt, ':', ''); "
		"AStmnt=REPLACE ( AStmnt, ';', ''); "
		"AStmnt=REPLACE (AStmnt, '0.00%', '');  "
		"AStmnt=REPLACE (AStmnt, '000%', ''); "
		"AStmnt=REPLACE (AStmnt, '@', ''); "
		"AStmnt=REPLACE (AStmnt, '000', ''); "
		"AStmnt=REPLACE (AStmnt, '00', '');  "
		"AStmnt=REPLACE (AStmnt, ', VARCHAR(50),',''); "
		"AStmnt=REPLACE (AStmnt, ',  VARCHAR(50),','');   "
        "AStmnt=REPLACE (AStmnt, '''',''); "
		"execute statement AStmnt;  "
		"SUSPEND;   "
		"END ",	inDBControl );
    }
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}

//---------------------------------------------------------------------------
void TApplyParser::Alter6_62Tables(TDBControl* const inDBControl)
{
    // Changes DayArcBill
    if (!fieldExists( "DAYARCBILL", "IS_PRINT_REQUIRED", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE DAYARCBILL ADD IS_PRINT_REQUIRED  CHAR(1) DEFAULT 'F';", inDBControl);
	}

    if (fieldExists( "DAYARCBILL", "IS_PRINT_REQUIRED", _dbControl ) )
	{
        executeQuery ( "UPDATE DAYARCBILL SET IS_PRINT_REQUIRED = 'F';", inDBControl);
	}

    if ( !fieldExists( "DAYARCBILL", "ONLINE_ORDER_ID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE DAYARCBILL ADD ONLINE_ORDER_ID INTEGER ;", inDBControl);
    }
    if (fieldExists( "DAYARCBILL", "ONLINE_ORDER_ID", _dbControl ) )
    {
        executeQuery ("UPDATE DAYARCBILL SET ONLINE_ORDER_ID = 0 ;", inDBControl);
    }

    if ( !fieldExists( "DAYARCBILL", "ORDER_GUID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE DAYARCBILL ADD ORDER_GUID VARCHAR(50) ;", inDBControl);
    }
    if (fieldExists( "DAYARCBILL", "ORDER_GUID", _dbControl ) )
    {
        executeQuery ("UPDATE DAYARCBILL SET ORDER_GUID = '';", inDBControl);
    }

    if ( !fieldExists( "DAYARCBILL", "APP_TYPE", _dbControl ) )
    {
        executeQuery ("ALTER TABLE DAYARCBILL ADD APP_TYPE INTEGER DEFAULT 0 ;", inDBControl);
    }
    if (fieldExists( "DAYARCBILL", "APP_TYPE", _dbControl ) )
    {
        executeQuery ("UPDATE DAYARCBILL SET APP_TYPE = 0 ;", inDBControl);
    }


        // Changes ArcBill
    if (!fieldExists( "ARCBILL", "IS_PRINT_REQUIRED", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE ARCBILL ADD IS_PRINT_REQUIRED  CHAR(1) DEFAULT 'F';", inDBControl);
	}
    if (fieldExists( "ARCBILL", "IS_PRINT_REQUIRED", _dbControl ) )
    {
        executeQuery ( "UPDATE ARCBILL SET IS_PRINT_REQUIRED = 'F';", inDBControl);
    }

    if ( !fieldExists( "ARCBILL", "ONLINE_ORDER_ID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE ARCBILL ADD ONLINE_ORDER_ID INTEGER ;", inDBControl);
    }
    if (fieldExists( "ARCBILL", "ONLINE_ORDER_ID", _dbControl ) )
    {
        executeQuery ("UPDATE ARCBILL SET ONLINE_ORDER_ID = 0 ;", inDBControl);
    }

    if ( !fieldExists( "ARCBILL", "ORDER_GUID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE ARCBILL ADD ORDER_GUID VARCHAR(50) ;", inDBControl);
    }
    if (fieldExists( "ARCBILL", "ORDER_GUID", _dbControl ) )
    {
        executeQuery ("UPDATE ARCBILL SET ORDER_GUID = '';", inDBControl);
    }

    if ( !fieldExists( "ARCBILL", "APP_TYPE", _dbControl ) )
    {
        executeQuery ("ALTER TABLE ARCBILL ADD APP_TYPE INTEGER DEFAULT 0 ;", inDBControl);
    }
    if (fieldExists( "ARCBILL", "APP_TYPE", _dbControl ) )
    {
        executeQuery ("UPDATE ARCBILL SET APP_TYPE = 0 ;", inDBControl);
    }

        // Changes Device table
    if (!fieldExists( "DEVICES", "UNIQUE_DEVICE_ID", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE DEVICES ADD UNIQUE_DEVICE_ID VARCHAR(50) ;", inDBControl);
	}

    if (fieldExists( "DEVICES", "UNIQUE_DEVICE_ID", _dbControl ) )
	{
        executeQuery ( "UPDATE DEVICES SET UNIQUE_DEVICE_ID = '';", inDBControl);
	}

        // Changes Archieve and DayArchieve

    if (!fieldExists( "DAYARCHIVE", "ITEMSIZE_IDENTIFIER", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE DAYARCHIVE ADD ITEMSIZE_IDENTIFIER VARCHAR(50) ;", inDBControl);
	}

    if (fieldExists( "DAYARCHIVE", "ITEMSIZE_IDENTIFIER", _dbControl ) )
	{
        executeQuery ( "UPDATE DAYARCHIVE SET ITEMSIZE_IDENTIFIER = '';", inDBControl);
	}

    if (!fieldExists( "ARCHIVE", "ITEMSIZE_IDENTIFIER", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE ARCHIVE ADD ITEMSIZE_IDENTIFIER VARCHAR(50) ;", inDBControl);
	}

    if (fieldExists( "ARCHIVE", "ITEMSIZE_IDENTIFIER", _dbControl ) )
	{
        executeQuery ( "UPDATE ARCHIVE SET ITEMSIZE_IDENTIFIER = '';", inDBControl);
	}

}

//---------------------------------------------------------------------------
void TApplyParser::Create6_62Table(TDBControl* const inDBControl)
{
    if ( !tableExists( "ONLINEORDERS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE ONLINEORDERS "
        "( "
        "  ONLINE_ORDER_KEY INTEGER NOT NULL PRIMARY KEY,  "
        "  PROFILE_KEY INTEGER,                            "
        "  EFTPOS_RECEIPT BLOB SUB_TYPE 1,                 "
        "  INVOICE_NUMBER VARCHAR(50),                     "
        "  APP_TYPE INTEGER DEFAULT 0 ,                   "
        "  ISPOSTED CHAR(1) DEFAULT 'F',                   "
        "  TERMINAL_NAME VARCHAR(22)                       "
        ");",
		inDBControl );
    }

    if ( !tableExists( "APPZEDSTATUS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE APPZEDSTATUS "
        "( "
        "  APP_ZED_STATUSKEY INTEGER NOT NULL PRIMARY KEY, "
        "  PROFILE_KEY INTEGER,                            "
        "  APP_TYPE INTEGER,                               "
        "  TERMINAL_NAME VARCHAR(22),                      "
        "  IS_ZED_REQUIRED CHAR(1) DEFAULT 'F',            "
        "  TIME_STAMP_REQUESTED TIMESTAMP                  "
        ");",
		inDBControl );
    }
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_62Generator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_ONLINEORDERS_ID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_ONLINEORDERS_ID;", inDBControl);
		executeQuery("SET GENERATOR GEN_ONLINEORDERS_ID TO 0;", inDBControl);
	}

    if(!generatorExists("GEN_APPZEDSTATUS_ID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_APPZEDSTATUS_ID;", inDBControl);
		executeQuery("SET GENERATOR GEN_APPZEDSTATUS_ID TO 0;", inDBControl);
	}
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
