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


}

