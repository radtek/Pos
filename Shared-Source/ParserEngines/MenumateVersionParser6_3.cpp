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

//----------------------------------------------------------------------------------------
void TApplyParser::update6_33Tables()
{
    Create6_33Malls(_dbControl);
    Create6_33MallExportSettings(_dbControl);
    Create6_33MallTransaction(_dbControl);
}

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33Malls(TDBControl* const inDBControl)
{
     if ( !tableExists( "Malls", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLS "
                "( "
                "   MALL_ID INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_NAME VARCHAR(50), "
                "   IS_ACTIVE VARCHAR(10) "
                ");",
			inDBControl );

        }
 }

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettings(TDBControl* const inDBControl)
{
     if ( !tableExists( "MallExportSettings", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MallExportSettings "
                "( "
                "   ID INTEGER NOT NULL PRIMARY KEY, "
                "   NAME VARCHAR(50), "
                "   DISPLAY_NAME(100) "
                ");",
			inDBControl );

     }
}

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingsMapping(TDBControl* const inDBControl)
{
     if ( !tableExists( "MallExportSettingsMapping", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MallExportSettingsMapping"
                "( "
                "   ID INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_ID ADD CONSTRAINT MALL_ID FOREIGN KEY (MALL_ID) REFERENCES "
                "   MALLS (MALL_ID) ON UPDATE CASCADE ON DELETE CASCADE, "
                "   MALLEXPORT_SETTING_ID INTEGER "
                ");",
			inDBControl );

        }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingsMapping(TDBControl* const inDBControl)
{
     if ( !tableExists( "MallExportSettingsValues", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MallExportSettingsValues"
                "( "
                "   MALLEXPORTSETTING_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALLEXPORTSETTING_ID INTEGER NOT NULL , "
                "   VALUE VARCHAR(50), "
                "   VALUE_TYPE VARCHAR(50) "
                ");",
			inDBControl );
     }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingsMapping(TDBControl* const inDBControl)
{
     if ( !tableExists( "MallEXPORT_SALES", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MallEXPORT_SALES"
                "( "
                "   ID INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_KEY ADD CONSTRAINT MALL_KEY FOREIGN KEY (MALL_KEY) REFERENCES "
                "   MALLS (MALL_ID) ON UPDATE CASCADE ON DELETE CASCADE, " "
                "   FIELD VARCHAR(50), "
                "   VALUE_GROUP VARCHAR(50), "
                "   VALUE VARCHAR(50),
                "   VALUE_TYPE VARCHAR(50), "
                "   ENUM_TYPE , "
                "   DATE_CREATED
                ");",
			inDBControl );
     }
}
}

