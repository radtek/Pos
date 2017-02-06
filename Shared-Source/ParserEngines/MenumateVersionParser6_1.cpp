#pragma hdrstop
#include "MenumateVersionParser.h"
#include <sstream>
#include <Predicates.h>
#include <DateUtils.hpp>
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

//6.10
void TApplyParser::upgrade6_10Tables()
{
	update6_10Tables();
}
//6.11
void TApplyParser::upgrade6_11Tables()
{
	update6_11Tables();

}
//6.12
void TApplyParser::upgrade6_12Tables()
{
	update6_12Tables();
}
//6.13
void TApplyParser::upgrade6_13Tables()
{
	update6_13Tables();
}
//6.14
void TApplyParser::upgrade6_14Tables()
{
	update6_14Tables();

}
//6.15
void TApplyParser::upgrade6_15Tables()
{
	update6_15Tables();

}
//6.16
void TApplyParser::upgrade6_16Tables()
{
	update6_16Tables();
}
//6.17
void TApplyParser::upgrade6_17Tables()
{
	update6_17Tables();
}
//6.18
void TApplyParser::upgrade6_18Tables()
{
	update6_18Tables();
}
//6.19
void TApplyParser::upgrade6_19Tables()
{
	update6_19Tables();
}

//::::::::::::::::::::::::Version 6.10::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_10Tables()
{
    CreateTabDiscountTable6_10(_dbControl);
    createGenerators6_10(_dbControl);
    ChangePaymentTypesFieldSize6_10(_dbControl);
    CreateResetPointsTable6_10(_dbControl);// 4579
    UpdateContacts_Access_Level6_10(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateContacts_Access_Level6_10(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "select distinct CONTACTS.ACCESS_LEVEL from CONTACTS where CONTACTS.CONTACT_ID >0 order by contacts.ACCESS_LEVEL asc";
        SelectQuery->ExecQuery();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           double AccessLevel = SelectQuery->FieldByName("ACCESS_LEVEL")->AsDouble;
           UpdateQuery->Close();
           if(AccessLevel==-540542081)
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '6438779775' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
           else if(AccessLevel==-662242448)
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '2022112112' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
           else if(AccessLevel==-939513856)
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '1744840704' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
        }
        SelectQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//-----------------------------------------
void TApplyParser::CreateTabDiscountTable6_10(TDBControl* const inDBControl)
{
  if ( !tableExists( "TABDISCOUNTS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TABDISCOUNTS "
            "( "
             " TABDISCOUNTS_KEY Integer NOT NULL, "
             " TAB_KEY Integer NOT NULL,        "
             " DISCOUNT_KEY Integer NOT NULL,  "
             " PRIMARY KEY (TABDISCOUNTS_KEY) "
            "); ",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::createGenerators6_10( TDBControl* const inDBControl)
{
	if( !generatorExists("GEN_TABDISCOUNTS", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_TABDISCOUNTS;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_TABDISCOUNTS TO 0; ",
		inDBControl );
	}
}
//-----------------------------------------
void TApplyParser::ChangePaymentTypesFieldSize6_10( TDBControl* const inDBControl)
{
	  if ( fieldExists("PAYMENTTYPES", "PAYMENT_NAME", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE PAYMENTTYPES ALTER PAYMENT_NAME TYPE varchar(50);",
        inDBControl );
    }
}
//-----------------------------------------
void TApplyParser::CreateResetPointsTable6_10(TDBControl* const inDBControl)
{
  if ( !tableExists( "RESETPOINTS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE RESETPOINTS "
		"( "
		"   RESET_KEY INTEGER NOT NULL PRIMARY KEY , "
        "   TIMESTAMPS Timestamp , "
		"   CONTACTS_KEY INTEGER,   "
        "   POINTS INTEGER,  "
        "   ADJUSTMENT_TYPE VARCHAR(10)"
		"); ",
		inDBControl );      //4579
	}
}

//::::::::::::::::::::::::Version 6.11::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_11Tables()
{
    UpdateHeldOrderItems6_11(_dbControl);
    UpdateHeldOrderView6_11(_dbControl);
    //added from V6.08 as call to this method was missed under release of 6.08
	UpdateTabTable6_08(_dbControl);
    CreateReprintOrderTable6_11(_dbControl);
    createGenerators6_11(_dbControl);
    create6_11ArcMallExport(_dbControl);
    create6_11ArcMallExportHourly(_dbControl);
    create6_11ArcMallExportTransaction(_dbControl);
    update6_11MallExportHourly(_dbControl);
    UpdateContacts_Access_Level6_11(_dbControl);// mm-4766
}
//---------------------------------------------------------------------------
void TApplyParser:: UpdateHeldOrderItems6_11( TDBControl* const inDBControl)
{
    if ( !fieldExists("HELDORDERITEMS", "ID_NUMBER", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE HELDORDERITEMS "
        "ADD ID_NUMBER varchar(50);",
         inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser:: UpdateHeldOrderView6_11( TDBControl* const inDBControl)
{
    if ( !fieldExists("HELDORDERSVIEW", "ID_NUMBER", inDBControl ) )
    {
        executeQuery(
        " RECREATE VIEW HELDORDERSVIEW (HELDORDERITEM_KEY, HELDORDER_KEY, TAB_KEY, TAB_TYPE, TAB_NAME, PARTY_NAME, TABLE_NUMBER, "
        " TABLE_NAME, ORDER_TYPE, TERMINAL_NAME, ORDER_LOCATION, ORDER_TYPE_MESSAGE, CONTACTS_KEY, ACTIVECHITNUMBER_KEY, ITEM_NAME, "
        " ITEM_ID, SIZE_NAME, NOTE, SEATNO, TRANSNO, MENU_NAME, COURSE_NAME, TIME_STAMP, MASTER_CONTAINER, SETMENU_MASK, SETMENU_GROUP, "
        " ITEM_CATEGORY, SECURITY_REF, TIME_KEY, QTY, ITEM_KITCHEN_NAME, SIZE_KITCHEN_NAME, COURSE_KITCHEN_NAME, CATEGORY_KEY, "
        " SERVINGCOURSES_KEY,CANCEL_STOCK_TYPE, ITEM_TYPE, MENU_ITEM_KEY, PLU, MENU_KEY, ID_NUMBER) "
        " AS   SELECT HOI.HELDORDERITEM_KEY,HO.HELDORDER_KEY,HOI.TAB_KEY,HOI.TAB_TYPE,HOI.TAB_NAME,HO.PARTY_NAME,HO.TABLE_NUMBER,HO.TABLE_NAME, "
        " HO.ORDER_TYPE,D.DEVICE_NAME AS TERMINAL_NAME,L.NAME AS ORDER_LOCATION,HO.ORDER_TYPE_MESSAGE,HO.CONTACTS_KEY,HO.ACTIVECHITNUMBER_KEY, "
        " HOI.ITEM_NAME,HOI.ITEM_ID,HOI.SIZE_NAME,HOI.NOTE,HOI.SEATNO,HOI.TRANSNO,HOI.MENU_NAME,HOI.COURSE_NAME,HOI.TIME_STAMP,HOI.MASTER_CONTAINER, "
        " HOI.SETMENU_MASK,HOI.SETMENU_GROUP,HOI.ITEM_CATEGORY,HOI.SECURITY_REF,HOI.TIME_KEY,HOI.QTY,HOI.ITEM_KITCHEN_NAME,HOI.SIZE_KITCHEN_NAME, "
        " HOI.COURSE_KITCHEN_NAME,HOI.CATEGORY_KEY,HOI.SERVINGCOURSES_KEY,HOI.CANCEL_STOCK_TYPE,HOI.ITEM_TYPE,HOI.MENU_ITEM_KEY,HOI.PLU,MENU.MENU_KEY, "
        " HOI.ID_NUMBER FROM HELDORDERS HO INNER JOIN HELDORDERITEMS HOI ON HO.HELDORDER_KEY = HOI.HELDORDER_KEY INNER JOIN DEVICES D ON "
        " HO.DEVICE_KEY = D.DEVICE_KEY INNER JOIN LOCATIONS L ON D.LOCATION_KEY = L.LOCATION_KEY LEFT JOIN MENU ON HOI.MENU_NAME = MENU.MENU_NAME ;",
          inDBControl );
    }

}
//---------------------------------------------------------------------------
void TApplyParser::CreateReprintOrderTable6_11(TDBControl* const inDBControl)
    {
       if ( !tableExists("REPRINTORDER", _dbControl ) )
	   {
		executeQuery(
		"CREATE TABLE REPRINTORDER "
            "( "
             " REPRINTORDER_KEY Integer NOT NULL, "
             " DOCKETS_NUMBER Integer NOT NULL, "
             " USERNAME Varchar(50) ,  "
             " TERMINAL_NAME Varchar(50) ,  "
             " TIME_STAMP Timestamp  ,"
             " PRIMARY KEY (REPRINTORDER_KEY) "
            "); ",
		inDBControl );
     	}
    }
//---------------------------------------------------------------------------
void TApplyParser::UpdateContacts_Access_Level6_11(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "select distinct CONTACTS.ACCESS_LEVEL from CONTACTS where CONTACTS.CONTACT_ID >0 order by contacts.ACCESS_LEVEL asc";
        SelectQuery->ExecQuery();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           double AccessLevel = SelectQuery->FieldByName("ACCESS_LEVEL")->AsDouble;
           UpdateQuery->Close();
           if(AccessLevel==6438779775) //when updating from v10 to v11 Access level for admin become this so update it according to access value for admin in v6
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '23618648959'"
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
           else if(AccessLevel==2022112112)//when updating from v10 to v11 Access level for supervisor become this so update it according to access value for supervisor in v6
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '19201981296' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
           else if(AccessLevel==1744840704)//when updating from v10 to v11 Access level for sales become this so update it according to access value for sales in v6
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '18924709888' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           } else if(AccessLevel==671098880)//when updating from v10 to v11 Access level for sales become this so update it according to access value for sales in v6
           {
             UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '17850968064' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
             UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
             UpdateQuery->ExecQuery();
           }


        }
        SelectQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//-----------------------------------------
void TApplyParser::createGenerators6_11( TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_ARCMALLEXPORT_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_ARCMALLEXPORT_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_ARCMALLEXPORT_KEY TO 0;", inDBControl
        );
    }
    if(!generatorExists("GEN_ARCME_HOURLY_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_ARCME_HOURLY_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_ARCME_HOURLY_KEY TO 0;", inDBControl
        );
    }
    if(!generatorExists("GEN_ARCME_TRANSACTION_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_ARCME_TRANSACTION_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_ARCME_TRANSACTION_KEY TO 0;", inDBControl
        );
    }
    if( !generatorExists("GEN_REPRINTORDER_KEY", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_REPRINTORDER_KEY;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_REPRINTORDER_KEY TO 0; ",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::update6_11MallExportHourly( TDBControl* const inDBControl )
{   if ( !fieldExists("MALLEXPORT_HOURLY", "VAT_SALES", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD VAT_SALES NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "TOTALDISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD TOTALDISCOUNT NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "SCHARGE_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD SCHARGE_AMOUNT NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "REFUND_COUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD REFUND_COUNT INTEGER;",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "REFUND_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD REFUND_AMOUNT NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "VOID_COUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD VOID_COUNT INTEGER;",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "VOID_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD VOID_AMOUNT NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "SCDISCOUNT_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD SCDISCOUNT_AMOUNT NUMERIC(17,4);",
		inDBControl );
	}
	if ( !fieldExists("MALLEXPORT_HOURLY", "MALLCODE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD MALLCODE VARCHAR(50);",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_11ArcMallExport(TDBControl* const inDBControl)
{
    if ( !tableExists( "ARCMALLEXPORT", inDBControl ) )
	{
		executeQuery(
                "CREATE TABLE ARCMALLEXPORT "
                "( "
                "   ARCMALLEXPORT_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   TENANT_NAME VARCHAR(50), "
                "   TERMINAL_NAME VARCHAR(50), "
                "   GROSS_SALES NUMERIC(17,4), "
                "   VAT_SALES NUMERIC(17,4), "
                "   VOID_AMOUNT NUMERIC(17,4), "
                "   VOID_COUNT NUMERIC(17,4), "
                "   REGDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   REGDISCOUNT_COUNT NUMERIC(17,4), "
                "   REFUND_AMOUNT NUMERIC(17,4), "
                "   REFUND_COUNT NUMERIC(17,4), "
                "   SCDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   SCDISCOUNT_COUNT NUMERIC(17,4), "
                "   SCHARGE_AMOUNT NUMERIC(17,4), "
                "   EODOLD NUMERIC(17,4), "
                "   OLD_GRANDTOTAL NUMERIC(17,4), "
                "   EODNEW NUMERIC(17,4), "
                "   NEW_GRANDTOTAL NUMERIC(17,4), "
                "   DATE_VALUE TIMESTAMP, "
                "   NOVELTY_SALES NUMERIC(17,4), "
                "   MISC_SALES NUMERIC(17,4), "
                "   LOCALTAXES NUMERIC(17,4), "
                "   CHARGECRED_SALES NUMERIC(17,4), "
                "   CHARGECRED_SALES_TAX NUMERIC(17,4), "
                "   VATEXEMPT_SALES NUMERIC(17,4), "
                "   PHARMA_SALES NUMERIC(17,4), "
                "   NONPHARMA_SALES NUMERIC(17,4), "
                "   PWDDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   SALESNOTSUBTORENT NUMERIC(17,4), "
                "   REPTRANSACTION_AMOUNT NUMERIC(17,4), "
                "   REPTRANSACTION_COUNT NUMERIC(17,4), "
                "   MALLCODE VARCHAR(50), "
                "   CLASSCODE VARCHAR(50), "
                "   TRADECODE VARCHAR(50), "
                "   OUTLETNUMBER VARCHAR(50), "
                "   SALESTYPE VARCHAR(50), "
                "   EMPDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   VIPDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   GPCDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG1_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG2_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG3_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG4_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG5_AMOUNT NUMERIC(17,4), "
                "   DISCOUNTG6_AMOUNT NUMERIC(17,4), "
                "   OTHERTAX NUMERIC(17,4), "
                "   ADJUSTMENTS NUMERIC(17,4), "
                "   POS_ADJ NUMERIC(17,4), "
                "   NEG_ADJ NUMERIC(17,4), "
                "   POS_ADJ_NONTAX NUMERIC(17,4), "
                "   NEG_ADJ_NONTAX NUMERIC(17,4), "
                "   DAILY_SALES NUMERIC(17,4), "
                "   TAX_INCLUSIVE_SALES NUMERIC(17,4), "
                "   CASH_SALES NUMERIC(17,4), "
                "   GIFT_CHEQUE_SALES NUMERIC(17,4), "
                "   DEBIT_CARD_SALES NUMERIC(17,4), "
                "   OTHER_TENDER_SALES NUMERIC(17,4), "
                "   MASTERCARD_SALES NUMERIC(17,4), "
                "   VISA_SALES NUMERIC(17,4), "
                "   AMEX_SALES NUMERIC(17,4), "
                "   DINERS_SALES NUMERIC(17,4), "
                "   JCB_SALES NUMERIC(17,4), "
                "   OTHER_CARD_SALES NUMERIC(17,4), "
                "   OTHER_CHARGES NUMERIC(17,4), "
                "   FIRST_TRANSACTION NUMERIC(17,4), "
                "   LAST_TRANSACTION NUMERIC(17,4), "
                "   TRANSACTION_COUNT NUMERIC(17,4), "
                "   BEGINNING_INVOICE VARCHAR(50), "
                "   ENDING_INVOICE VARCHAR(50), "
                "   CASH_COUNT NUMERIC(17,4), "
                "   GC_COUNT NUMERIC(17,4), "
                "   DEBIT_COUNT NUMERIC(17,4), "
                "   OTHER_TENDER_COUNT NUMERIC(17,4), "
                "   MASTERCARD_COUNT NUMERIC(17,4), "
                "   VISA_COUNT NUMERIC(17,4), "
                "   AMEX_COUNT NUMERIC(17,4), "
                "   DINERS_COUNT NUMERIC(17,4), "
                "   JCB_COUNT NUMERIC(17,4), "
                "   OTHERCARD_COUNT NUMERIC(17,4), "
                "   SERIALNUMBER VARCHAR(50), "
                "   TRANSACTIONTIME TIMESTAMP, "
                "   TRANSACTIONDATE TIMESTAMP, "
                "   HOUR_VALUE VARCHAR(50), "
                "   TOTALDISCOUNT NUMERIC(17,4), "
                "   BEGINNING_OR VARCHAR(50), "
                "   ENDING_OR VARCHAR(50), "
                "   LOCTAXEXEMPTDLY_SALES NUMERIC(17,4), "
                "   FINEDINECUST_COUNT NUMERIC(17,4), "
                "   TENDERSURCHARGES NUMERIC(17,4), "
                "   DEPARTMENTSUM NUMERIC(17,4), "
                "   NONVAT_SALES NUMERIC(17,4), "
                "   CHECK_SALES NUMERIC(17,4), "
                "   EPAY_SALES NUMERIC(17,4), "
                "   NO_SALES NUMERIC(17,4), "
                "   PREVEODCOUNTER NUMERIC(17,4), "
                "   CURRENTEODCOUNTER NUMERIC(17,4), "
                "   DISCOUNT_COUNT NUMERIC(17,4), "
                "   CARD_SALES NUMERIC(17,4), "
                "   OTHER_SALES NUMERIC(17,4) "
                ");",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_11ArcMallExportHourly(TDBControl* const inDBControl)
{
    if ( !tableExists( "ARCMALLEXPORTHOURLY", inDBControl ) )
	{
		executeQuery(
                "CREATE TABLE ARCMALLEXPORTHOURLY "
                "( "
                "   AME_HOURLY_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   TERMINAL_NAME VARCHAR(50), "
                "   TENANT_NAME VARCHAR(50), "
                "   DATE_VALUE TIMESTAMP, "
                "   TIME_VALUE VARCHAR(50), "
                "   AMOUNT_VALUE NUMERIC(15,4), "
                "   TRANSACTION_COUNT INTEGER, "
                "   VAT_SALES NUMERIC(15,4), "
                "   TOTALDISCOUNT NUMERIC(15,4), "
                "   SCHARGE_AMOUNT NUMERIC(15,4), "
                "   REFUND_COUNT INTEGER, "
                "   REFUND_AMOUNT NUMERIC(15,4), "
                "   VOID_COUNT INTEGER, "
                "   VOID_AMOUNT NUMERIC(15,4), "
                "   SCDISCOUNT_AMOUNT NUMERIC(15,4), "
                "   MALLCODE VARCHAR(50) "
                ");",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_11ArcMallExportTransaction(TDBControl* const inDBControl)
{
    if ( !tableExists( "ARCMALLEXPORTTRANSACTION", inDBControl ) )
	{
		executeQuery(
                "CREATE TABLE ARCMALLEXPORTTRANSACTION "
                "( "
                "   AME_TRANSACTION_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   DATE_VALUE VARCHAR(50), "
                "   TENANT_NAME VARCHAR(50), "
                "   CURRENT_DATE_VALUE TIMESTAMP, "
                "   TIME_VALUE VARCHAR(50), "
                "   TERMINAL_NAME VARCHAR(50), "
                "   INVOICE_NUMBER VARCHAR(50), "
                "   GROSS_SALES NUMERIC(17,4), "
                "   VAT_SALES NUMERIC(17,4), "
                "   NONVAT_SALES NUMERIC(17,4), "
                "   VATEXEMPT_SALES NUMERIC(17,4), "
                "   SCDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   EMPDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   PRODISCOUNT_AMOUNT NUMERIC(17,4), "
                "   OTHERDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   REFUND_AMOUNT NUMERIC(17,4), "
                "   SCHARGE_AMOUNT NUMERIC(17,4), "
                "   OTHER_SURCHARGE NUMERIC(17,4), "
                "   NET_SALES NUMERIC(17,4), "
                "   OLD_GRANDTOTAL NUMERIC(17,4), "
                "   NEW_GRANDTOTAL NUMERIC(17,4), "
                "   ZCOUNT NUMERIC(17,4), "
                "   CASH_SALES_AMOUNT NUMERIC(17,4), "
                "   CARD_SALES_AMOUNT NUMERIC(17,4), "
                "   CHECK_SALES_AMOUNT NUMERIC(17,4), "
                "   GC_SALES_AMOUNT NUMERIC(17,4), "
                "   EPAY_SALES_AMOUNT NUMERIC(17,4), "
                "   OTHER_SALES_AMOUNT NUMERIC(17,4), "
                "   VOID_AMOUNT NUMERIC(17,4), "
                "   DEPOSIT_AMOUNT NUMERIC(17,4), "
                "   APPLIED_DEPOSIT_AMOUNT NUMERIC(17,4), "
                "   VAT_SALES_COUNT NUMERIC(17,4), "
                "   NONVAT_SALES_COUNT NUMERIC(17,4), "
                "   VATEXEMPT_SALES_COUNT NUMERIC(17,4), "
                "   SCDISCOUNT_COUNT NUMERIC(17,4), "
                "   EMPDISCOUNT_COUNT NUMERIC(17,4), "
                "   PRODISCOUNT_COUNT NUMERIC(17,4), "
                "   OTHERDISCOUNT_COUNT NUMERIC(17,4), "
                "   REFUND_COUNT NUMERIC(17,4), "
                "   SCHARGE_COUNT NUMERIC(17,4), "
                "   OTHER_SURCHARGE_COUNT NUMERIC(17,4), "
                "   CASH_COUNT NUMERIC(17,4), "
                "   CARD_COUNT NUMERIC(17,4), "
                "   CHECK_COUNT NUMERIC(17,4), "
                "   GC_COUNT NUMERIC(17,4), "
                "   EPAY_COUNT NUMERIC(17,4), "
                "   OTHER_COUNT NUMERIC(17,4), "
                "   VOID_COUNT NUMERIC(17,4), "
                "   TRANS_VOID_COUNT NUMERIC(17,4), "
                "   DEPOSIT_COUNT NUMERIC(17,4), "
                "   APPLIED_DEPOSIT_COUNT NUMERIC(17,4), "
                "   PATRON_COUNT NUMERIC(17,4), "
                "   TRANSACTION_COUNT NUMERIC(17,4), "
                "   NOSALE_COUNT NUMERIC(17,4), "
                "   SALES_FLAG VARCHAR(50), "
                "   VAT_PERCENTAGE NUMERIC(17,4) "
                ");",
			inDBControl );
	}
}

//::::::::::::::::::::::::Version 6.12::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_12Tables()
{
    update6_12ArcCategories(_dbControl);
    UpdatePaymentTypes6_12(_dbControl);
    UpdateDayArcBillPay6_12(_dbControl);
    UpdateArcBillPay6_12(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser:: update6_12ArcCategories( TDBControl* const inDBControl)
{
    if ( !fieldExists("ARCCATEGORIES", "GL_CODE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ARCCATEGORIES "
        "ADD GL_CODE varchar(30);",
         inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdatePaymentTypes6_12(TDBControl* const inDBControl)
{
    if ( !fieldExists("PAYMENTTYPES", "GL_CODE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE PAYMENTTYPES "
        "ADD GL_CODE Varchar(30);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateDayArcBillPay6_12(TDBControl* const inDBControl)
{
    if ( !fieldExists("DAYARCBILLPAY", "CHARGED_TO_XERO", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE DAYARCBILLPAY "
        "ADD CHARGED_TO_XERO Char(1);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateArcBillPay6_12(TDBControl* const inDBControl)
{
    if ( !fieldExists("ARCBILLPAY", "CHARGED_TO_XERO", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ARCBILLPAY "
        "ADD CHARGED_TO_XERO Char(1);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::Version 6.13::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_13Tables()
{
	UpdateTabHistoryTable6_13(_dbControl);

}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTabHistoryTable6_13(TDBControl* const inDBControl)
{
     if ( !fieldExists( "TABHISTORY", "IS_TABLE", _dbControl ) )
        {
            executeQuery (
            "ALTER TABLE TABHISTORY "
            "ADD IS_TABLE Char(1) ",
            inDBControl);
        }
     if ( constraintExists( "TABHISTORY_TAB_KEY", _dbControl ) )
        {
            TDBTransaction transaction( *_dbControl );
            transaction.StartTransaction();
            try
            {
                TIBSQL *query    = transaction.Query( transaction.AddQuery() );
                transaction.StartTransaction();
                query->Close();
                query->SQL->Text = "ALTER TABLE TABHISTORY DROP CONSTRAINT TABHISTORY_TAB_KEY ";
                query->ExecQuery();
                query->Close();
                transaction.Commit();
            }
            catch( Exception &E )
            {
                transaction.Rollback();
            }
        }

}

//::::::::::::::::::::::::Version 6.14::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_14Tables()
{
    UpdateContacts_Access_Level6_14(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateContacts_Access_Level6_14(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "select distinct CONTACTS.ACCESS_LEVEL from CONTACTS where CONTACTS.CONTACT_ID >0 order by contacts.ACCESS_LEVEL asc";
        SelectQuery->ExecQuery();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           double AccessLevel = SelectQuery->FieldByName("ACCESS_LEVEL")->AsDouble;
           UpdateQuery->Close();
           if(AccessLevel==23618648959) //when updating from v12 to v13 Access level for admin become this so update it according to access value for admin in v6
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '57978387327'"
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }
           else if(AccessLevel==19201981296)//when updating from v12 to v13 Access level for supervisor become this so update it according to access value for supervisor in v6
           {
           UpdateQuery->SQL->Text = "UPDATE contacts SET CONTACTS.ACCESS_LEVEL = '53561719664' "
                                    "WHERE CONTACTS.ACCESS_LEVEL = :ACCESS_LEVEL ";
           UpdateQuery->ParamByName("ACCESS_LEVEL")->AsString = AccessLevel;
           UpdateQuery->ExecQuery();
           }



        }
        SelectQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}

//::::::::::::::::::::::::Version 6.15::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_15Tables()
{
    UpdateOrdersTable6_15(_dbControl);
    ModifyStaffHourEnableZedColumns6_15(_dbControl);
    PopulateStaffHourEnableForZeds6_15( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateOrdersTable6_15(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT  ord.ORDER_KEY,ord.PRICE "
          "FROM ORDERS ord where coalesce(ord.ZED_PRICE,0)=0 ";
          SelectQuery->ExecQuery();

        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           UpdateQuery->Close();
           UpdateQuery->SQL->Text = "UPDATE Orders ord set ord.ZED_PRICE = :ZED_PRICE  where ord.ORDER_KEY =:ORDER_KEY ";

           UpdateQuery->ParamByName("ORDER_KEY")->AsString = SelectQuery->Fields[0]->AsInteger;
           UpdateQuery->ParamByName("ZED_PRICE")->AsString = SelectQuery->Fields[1]->AsDouble;

           UpdateQuery->ExecQuery();
        }

        SelectQuery->Close();
        transaction.Commit();
     }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyStaffHourEnableZedColumns6_15( TDBControl* const inDBControl )
{
    if ( !fieldExists( "ZEDS", "STAFF_HOUR_ENABLE", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ZEDS "
        "ADD STAFF_HOUR_ENABLE Integer ",
        inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::PopulateStaffHourEnableForZeds6_15(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "UPDATE ZEDS SET STAFF_HOUR_ENABLE = 1";
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

//::::::::::::::::::::::::Version 6.16::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_16Tables()
{
    createDrinkCommandSessionTable(_dbControl);
    changeThirdPartyCodeSize( _dbControl);
    createSessionTableGenerators(_dbControl);
    //Please dont alter the order of below 2 methods...
    CreateClippTabIntegrationTable(_dbControl);
    CreateClippTabPaymentsTable(_dbControl);
    CreateClippTabIntegrationForeignKey(_dbControl);
    CreateClippTabPaymentsForeignKey(_dbControl);
    CreateGenerators6_16(_dbControl);
    create6_16HappyHourProfiles( _dbControl );
    create6_16HappyHourSettings( _dbControl );
    create6_16HappyHourDays(_dbControl );
    create6_16TerminalHappyHourProfiles(_dbControl );
    UpdateChitNumberTableForSaveCustomerAndMemberDetails6_16(_dbControl);

}
//---------------------------------------------------------------------------
void TApplyParser:: createDrinkCommandSessionTable( TDBControl* const inDBControl)
{
    if ( !tableExists("DC_SESSION_DATA", inDBControl ) )
    {
       executeQuery(
                "CREATE TABLE DC_SESSION_DATA "
                "( "
                "   SESSION_ID INTEGER NOT NULL PRIMARY KEY, "
                "   SESSION_TOKEN VARCHAR(50), "
                "   SESSION_STATUS VARCHAR(50), "
                "   LINE_ITEM_ID INTEGER, "
                "   TIME_STAMP_BILL VARCHAR(50)"
                ");",
			inDBControl );
    }

}
//---------------------------------------------------------------------------
void TApplyParser::changeThirdPartyCodeSize( TDBControl* const inDBControl)
{
	  if ( fieldExists("THIRDPARTYCODES", "CODE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE THIRDPARTYCODES ALTER CODE TYPE varchar(50);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser:: createSessionTableGenerators( TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_DC_SESSION_DATA_KEY", _dbControl) )
	{
		executeQuery("CREATE GENERATOR GEN_DC_SESSION_DATA_KEY;", inDBControl);
		executeQuery("SET GENERATOR GEN_DC_SESSION_DATA_KEY TO 0; ", inDBControl );
	}

    if( !generatorExists("GEN_DC_SESSION_LINE_ITEM_ID", _dbControl) )
	{
		executeQuery("CREATE GENERATOR GEN_DC_SESSION_LINE_ITEM_ID;", inDBControl);
		executeQuery("SET GENERATOR GEN_DC_SESSION_LINE_ITEM_ID TO 0; ", inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::CreateGenerators6_16(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_CLIPP_TAB_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_CLIPP_TAB_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_CLIPP_TAB_KEY TO 0;", inDBControl
        );
    }

    if(!generatorExists("GEN_CLIPP_TAB_PAYMENT_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_CLIPP_TAB_PAYMENT_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_CLIPP_TAB_PAYMENT_KEY TO 0;", inDBControl
        );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::CreateClippTabPaymentsTable(TDBControl* const inDBControl)
{
    if (!tableExists("CLIPP_TAB_PAYMENTS", inDBControl))
	{
		executeQuery(
                "CREATE TABLE CLIPP_TAB_PAYMENTS "
                "( "
                "   CLIPP_TAB_PAYMENT_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   CLIPP_TAB_KEY INTEGER, "
                "   CLIPP_PAYMENT_REF VARCHAR(50), "
                "   CLIPP_PAYMENT NUMERIC(15, 4), "
                "   CLIPP_PAYMENT_TYPE INTEGER "
                ");",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create6_16HappyHourDays(TDBControl* const inDBControl)
{
   if ( !tableExists( "HAPPYHOURDAYS", inDBControl ) )
	{
		executeQuery(
        "CREATE TABLE HAPPYHOURDAYS "
        "( "
        "   HAPPYHOURPROFILES_KEY INTEGER NOT NULL, "
        "   HAPPYHOURDAYS_KEY INTEGER NOT NULL, "
        "   PRIMARY KEY (HAPPYHOURPROFILES_KEY , HAPPYHOURDAYS_KEY) "
        ");",
        inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_16HappyHourSettings(TDBControl* const inDBControl)
{
    if ( !tableExists( "HAPPYHOURSETTINGS", inDBControl ) )
	{
	  	executeQuery(
        "CREATE TABLE HAPPYHOURSETTINGS "
        "( "
        "   HAPPYHOURPROFILES_KEY INTEGER NOT NULL, "
        "   HAPPYHOURSETTINGS_KEY INTEGER NOT NULL, "
        "   PRIMARY KEY (HAPPYHOURPROFILES_KEY , HAPPYHOURSETTINGS_KEY) "

        ");",
        inDBControl );
	}
 }
//---------------------------------------------------------------------------
void TApplyParser::create6_16TerminalHappyHourProfiles(TDBControl* const inDBControl)
{
    if ( !tableExists( "TERMINALHAPPYHOURPROFILES", inDBControl ) )
	{
	  	executeQuery(
        "CREATE TABLE TERMINALHAPPYHOURPROFILES "
        "( "
        "   HAPPYHOURPROFILES_KEY INTEGER NOT NULL, "
        "   TERMINALS_KEY INTEGER NOT NULL, "
        "   PRIMARY KEY (HAPPYHOURPROFILES_KEY , TERMINALS_KEY) "

        ");",
        inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::CreateClippTabIntegrationForeignKey(TDBControl* const inDBControl)
{
    if (tableExists("CLIPP_TAB", inDBControl) && fieldExists("CLIPP_TAB", "CLIPP_TAB_TAB_KEY", inDBControl)
                                    && !constraintExists( "CLIPPTAB_TABKEY", _dbControl ))
	{
        executeQuery(
                "ALTER TABLE CLIPP_TAB ADD CONSTRAINT CLIPPTAB_TABKEY FOREIGN KEY (CLIPP_TAB_TAB_KEY) REFERENCES "
                "TAB (TAB_KEY) ON UPDATE CASCADE ON DELETE CASCADE;", inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::CreateClippTabPaymentsForeignKey(TDBControl* const inDBControl)
{
    if (tableExists("CLIPP_TAB_PAYMENTS", inDBControl) && fieldExists("CLIPP_TAB_PAYMENTS", "CLIPP_TAB_KEY", inDBControl)
                            && !constraintExists( "CLIPPTABPAYMENT_CLIPPTABKEY", _dbControl ))
	{
        executeQuery(
                "ALTER TABLE CLIPP_TAB_PAYMENTS ADD CONSTRAINT CLIPPTABPAYMENT_CLIPPTABKEY FOREIGN KEY (CLIPP_TAB_KEY) REFERENCES "
                "CLIPP_TAB (CLIPP_TAB_KEY) ON UPDATE CASCADE ON DELETE CASCADE;", inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::CreateClippTabIntegrationTable(TDBControl* const inDBControl)
{
    if (!tableExists("CLIPP_TAB", inDBControl))
	{
		executeQuery(
                "CREATE TABLE CLIPP_TAB "
                "( "
                "   CLIPP_TAB_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   CLIPP_TAB_TAB_KEY INTEGER, "
                "   TAB_CREATED_DATE TIMESTAMP, "
                "   CLIPP_TAB_REF VARCHAR(50), "
                "   CLIPP_USER_REF VARCHAR(50), "
                "   CUSTOMER_FIRST_NAME VARCHAR(50), "
                "   CUSTOMER_LAST_NAME VARCHAR(50), "
                "   CUSTOMER_PHOTO_URL VARCHAR(50), "
                "   IS_TAB_CLOSED T_TRUEFALSE DEFAULT 'F' "
                ");",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create6_16HappyHourProfiles(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_HAPPYHOURPROFILES_KEY", _dbControl) )
   	{
		executeQuery(
		"CREATE GENERATOR GEN_HAPPYHOURPROFILES_KEY;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_HAPPYHOURPROFILES_KEY TO 0; ",
		inDBControl );
    }

    if ( !tableExists( "HAPPYHOURPROFILES", inDBControl ) )
	{
		executeQuery(
        "CREATE TABLE HAPPYHOURPROFILES "
        "( "
        "   HAPPYHOURPROFILES_KEY INTEGER NOT NULL PRIMARY KEY, "
        "   HAPPYHOURPROFILE_NAME VARCHAR(50), "
        "   ISACTIVE CHAR(1), "
        "   HAPPYHOURDAY_STARTTIME TIMESTAMP , "
        "   HAPPYHOURDAY_ENDTIME TIMESTAMP, "
        "   HAPPYHOURDAY_PROFILEDATE TIMESTAMP "
        ");",
        inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateChitNumberTableForSaveCustomerAndMemberDetails6_16( TDBControl* const inDBControl )
{
    if ( !fieldExists( "CHITNUMBER", "SAVE_CUSTOMER_NAME", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD SAVE_CUSTOMER_NAME CHAR(1) default 'F';",
        inDBControl);
    }

    if ( !fieldExists( "CHITNUMBER", "SAVE_MEMBER_NAME", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD SAVE_MEMBER_NAME CHAR(1) default 'F';",
        inDBControl);
    }

    if ( !fieldExists( "CHITNUMBER", "SAVE_MEMBER_ADDRESS", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD SAVE_MEMBER_ADDRESS CHAR(1) default 'F';",
        inDBControl);
    }
}

//::::::::::::::::::::::::Version 6.17::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_17Tables()
{
    CreateGenerators6_17(_dbControl);
    create6_17TableForSalesTypeGroups(_dbControl);
    populateTableSalesTypeGroups6_17(_dbControl);
    updateTableMallExport6_17(_dbControl);
    update6_17MallExportHourly(_dbControl);
    update6_17ArcMallExport(_dbControl);
    update6_17ArcMallExportHourly(_dbControl);
	AlterItemSizeTable6_17(_dbControl);
    UpdateItemSizeTable6_17(_dbControl);
	AlterSizesTable6_17(_dbControl);
    UpdateSizesTable6_17(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::AlterItemSizeTable6_17(TDBControl* const inDBControl)
{
   if ( !fieldExists("ITEMSIZE", "ISINVISIBLE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ITEMSIZE "
        "ADD ISINVISIBLE CHAR(1) default 'F';",
         inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::AlterSizesTable6_17(TDBControl* const inDBControl)
{
   if ( !fieldExists("SIZES", "ISINVISIBLE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE SIZES "
        "ADD ISINVISIBLE CHAR(1) default 'F';",
         inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateItemSizeTable6_17(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "UPDATE ITEMSIZE SET ISINVISIBLE = 'F'";
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
void TApplyParser::UpdateSizesTable6_17(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "UPDATE SIZES SET ISINVISIBLE = 'F'";
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
void TApplyParser::CreateGenerators6_17(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_SALES_TYPE", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_SALES_TYPE;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_SALES_TYPE TO 0;", inDBControl
        );
    }

	if(!generatorExists("GEN_SALES_TYPE_ITEMS_RELATION",_dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_SALES_TYPE_ITEMS_RELATION;",inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_SALES_TYPE_ITEMS_RELATION TO 0;",inDBControl
		);
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_17TableForSalesTypeGroups( TDBControl* const inDBControl )
{
	if ( !tableExists( "SALES_TYPE", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE SALES_TYPE "
		"( "
		"   SALES_TYPE_KEY int primary key,"
		"   SALES_TYPE_NAME varchar(25),"
		"   DEFAULT_SALES_TYPE T_TRUEFALSE default 'F'"
		");",
		inDBControl );
	}
	if ( !tableExists( "SALES_TYPE_ITEMS_RELATION", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE SALES_TYPE_ITEMS_RELATION "
		"( "
		"   STI_KEY int primary key,"
		"   ITEM_KEY int,"
		"   SALES_TYPE_KEY int,"
		"   foreign key(ITEM_KEY) references ITEM(ITEM_KEY) ON DELETE CASCADE,"
		"   foreign key(SALES_TYPE_KEY) references SALES_TYPE(SALES_TYPE_KEY) ON DELETE CASCADE"
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::populateTableSalesTypeGroups6_17( TDBControl* const inDBControl )
{    TDBTransaction transaction( *inDBControl );
	const int NUMBER_OF_FIELDS = 5;
	UnicodeString salestypeGroups[NUMBER_OF_FIELDS] = { "Food", "Non-Food", "Groceries", "Medicines", "Others"};
	try
	{
		transaction.StartTransaction();
		TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );
		fieldCountQuery->SQL->Text = "SELECT COUNT(SALES_TYPE_KEY) FIELD_COUNT FROM SALES_TYPE;";
		fieldCountQuery->ExecQuery();
		for (int i = fieldCountQuery->FieldByName("FIELD_COUNT")->AsInteger; i < NUMBER_OF_FIELDS; i++)
		{
			TIBSQL *keyGeneratorQuery    = transaction.Query( transaction.AddQuery() );
			keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_SALES_TYPE, 1) FROM RDB$DATABASE";
			keyGeneratorQuery->ExecQuery();
			int key = keyGeneratorQuery->Fields[0]->AsInteger;
			//:::::::::::::::::::::::::::::::::::::::
			TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
			insertQuery->SQL->Text = "INSERT INTO SALES_TYPE "
			"( "
			"   SALES_TYPE_KEY,"
			"   SALES_TYPE_NAME,"
			"   DEFAULT_SALES_TYPE "
			") "
			"VALUES "
			"( "
			"   :SALES_TYPE_KEY,"
			"   :SALES_TYPE_NAME,"
			"   :DEFAULT_SALES_TYPE "
			");";
			insertQuery->ParamByName("SALES_TYPE_KEY")->AsInteger = key;
			insertQuery->ParamByName("SALES_TYPE_NAME")->AsString = salestypeGroups[i];
			insertQuery->ParamByName("DEFAULT_SALES_TYPE")->AsString  = "T";
			insertQuery->ExecQuery();
		}
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
};
//---------------------------------------------------------------------------
void TApplyParser::updateTableMallExport6_17( TDBControl* const inDBControl )
{
    enum ExportValueType {STRING, INTEGER, CURRENCY, TIMESTAMP};
    const int NUMBER_OF_FIELDS = 5;

    TDBTransaction transaction( *inDBControl );

    UnicodeString fieldNames[NUMBER_OF_FIELDS] =
        {
            "SalesTypeFood",
            "SalesTypeNon-Food",
            "SalesTypeGroceries",
            "SalesTypeMedicines",
            "SalesTypeOthers"
        };

    ExportValueType fieldTypes[NUMBER_OF_FIELDS] =
        {
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY
        };

    try
	{
		transaction.StartTransaction();

        TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );

        fieldCountQuery->SQL->Text = "SELECT COUNT(MALLEXPORT_KEY) FIELD_COUNT FROM MALLEXPORT;";
        fieldCountQuery->ExecQuery();
        int MEfieldcount = fieldCountQuery->FieldByName("FIELD_COUNT")->AsInteger;

        for (int i = 0; i < NUMBER_OF_FIELDS; i++)
        {
            TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
            UnicodeString valueType = "";

            switch (fieldTypes[i])
            {
                case STRING:
                    valueType = "STRING_VALUE";
                    break;
                case INTEGER:
                    valueType = "INTEGER_VALUE";
                    break;
                case CURRENCY:
                    valueType = "CURRENCY_VALUE";
                    break;
                case TIMESTAMP:
                    valueType = "TIMESTAMP_VALUE";
                    break;
                default:
                    break;
            }

            insertQuery->SQL->Text = "INSERT INTO MALLEXPORT "
                                     "( "
                                     "   MALLEXPORT_KEY,"
                                     "   FIELD_NAME,"
                                     "   " + valueType + ""
                                     ") "
                                     "VALUES "
                                     "( "
                                     "   :MALLEXPORT_KEY,"
                                     "   :FIELD_NAME,"
                                     "   :" + valueType + ""
                                     ");";

            insertQuery->ParamByName("MALLEXPORT_KEY")->AsInteger = (MEfieldcount + i + 1);
            insertQuery->ParamByName("FIELD_NAME"    )->AsString  = fieldNames[i];

            switch (fieldTypes[i])
            {
                case STRING:
                    insertQuery->ParamByName(valueType)->AsString = "";
                    break;
                case INTEGER:
                    insertQuery->ParamByName(valueType)->AsInteger = 0;
                    break;
                case CURRENCY:
                    insertQuery->ParamByName(valueType)->AsCurrency = 0;
                    break;
                case TIMESTAMP:
                    insertQuery->ParamByName(valueType)->AsDateTime = 0;
                    break;
                default:
                    break;
            }

            insertQuery->ExecQuery();
        }

		transaction.Commit();
	}
	catch( ... )
	{
        transaction.Rollback();
        throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::update6_17MallExportHourly( TDBControl* const inDBControl )
{   if ( !fieldExists("MALLEXPORT_HOURLY", "PATRON_COUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE MALLEXPORT_HOURLY ADD PATRON_COUNT INTEGER;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::update6_17ArcMallExport(TDBControl* const inDBControl)
{
    if ( !fieldExists("ARCMALLEXPORT", "SALESTYPE_FOOD", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORT ADD SALESTYPE_FOOD NUMERIC(17,4);",
		inDBControl );
	}
    if ( !fieldExists("ARCMALLEXPORT", "SALESTYPE_NONFOOD", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORT ADD SALESTYPE_NONFOOD NUMERIC(17,4);",
		inDBControl );
	}
    if ( !fieldExists("ARCMALLEXPORT", "SALESTYPE_GROCERIES", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORT ADD SALESTYPE_GROCERIES NUMERIC(17,4);",
		inDBControl );
	}
    if ( !fieldExists("ARCMALLEXPORT", "SALESTYPE_MEDICINES", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORT ADD SALESTYPE_MEDICINES NUMERIC(17,4);",
		inDBControl );
	}
    if ( !fieldExists("ARCMALLEXPORT", "SALESTYPE_OTHERS", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORT ADD SALESTYPE_OTHERS NUMERIC(17,4);",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::update6_17ArcMallExportHourly(TDBControl* const inDBControl)
{
    if ( !fieldExists("ARCMALLEXPORTHOURLY", "PATRON_COUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCMALLEXPORTHOURLY ADD PATRON_COUNT INTEGER;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
//::::::::::::::::::::::::Version 6.18::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_18Tables()
{
	UpdateTabHistoryTable6_13(_dbControl);
}

//::::::::::::::::::::::::Version 6.19::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_19Tables()
{
	UpdateChitNumberTableForPickupAndDeliveryChit6_19(_dbControl);
	create6_19MallViews(_dbControl);
}
//-------------------------------------------------------------------------------------
void TApplyParser::create6_19MallViews( TDBControl* const inDBControl )
{
    if ( !tableExists( "MEGAWORLD_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW MEGAWORLD_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       02, " // Terminal Number
		"                       03, " // Gross Sales
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       07, " // Total Regular Discount
		"                       09, " // Total Refund
		"                       11, " // Total Senior Citizen Discount
		"                       13, " // Total Service Charge
		"                       15, " // Grand Total Old
		"                       17, " // Grand Total
		"                       24, " // Tax Exempt Sales
		"                       51, " // Daily Sales
		"                       53, " // Cash Sales
		"                       66, " // Transaction Count
		"                       81, " // Transaction Date
		"                       87, " // FineDiningCustomerCount
		"                       95, " // CurrentEODCounter
		"                       97, " // CardSales
		"                       98, " // OtherSales
		"                       99, " // SalesTypeFood
		"                       100, " // SalesTypeNon-Food
		"                       101, " // SalesTypeGroceries
		"                       102, " // SalesTypeMedicines
		"                       103 )", // SalesTypeOthers
		inDBControl );
	}
	if ( !tableExists( "MEGAWORLD_HOURLY_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW MEGAWORLD_HOURLY_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       02, " // Terminal Number
		"                       51, " // Daily Sales
		"                       66, " // Transaction Count
		"                       81, " // Transaction Date
		"                       82, " // Hour
		"                       87 )", // Fine Dine Cutormer count
		inDBControl );
	}

    if ( !tableExists( "SHANGRILA_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW SHANGRILA_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       03, " // Gross Sales
		"                       04, " // Total Sale Tax
		"                       13, " // Total Service Charge
        "                       31, " // Mall Code
		"                       66, " // Transaction Count
		"                       80, " // Transaction Time
		"                       81, " // Transaction Date
		"                       83 )", // Total Discount
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateChitNumberTableForPickupAndDeliveryChit6_19( TDBControl* const inDBControl )
{
    if ( !fieldExists( "CHITNUMBER", "ONLINE_PICKUP_ORDER", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD ONLINE_PICKUP_ORDER CHAR(1) default 'F';",
        inDBControl);
    }

    if ( !fieldExists( "CHITNUMBER", "ONLINE_DELIVERY_ORDER", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD ONLINE_DELIVERY_ORDER CHAR(1) default 'F';",
        inDBControl);
    }


}

}
