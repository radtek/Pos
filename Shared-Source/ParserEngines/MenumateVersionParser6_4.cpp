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
//6.40
void TApplyParser::upgrade6_40Tables()
{
    update6_40Tables();
}
//-----------------------------------------------------------
void TApplyParser::upgrade6_41Tables()
{
    update6_41Tables();
}
//-----------------------------------------------------------
void TApplyParser::upgrade6_42Tables()
{
    update6_42Tables();
}
//-----------------------------------------------------------
void TApplyParser::upgrade6_43Tables()
{
    update6_43Tables();
}
//-----------------------------------------------------------
void TApplyParser::upgrade6_44Tables()
{
    update6_44Tables();
}

//::::::::::::::::::::::::Version 6.40:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_40Tables()
{
   UpdateChargeToAccount(_dbControl);
}
//--------------------------------------------------------------------
void TApplyParser::update6_41Tables()
{
    AlterTable6_41(_dbControl);
   UpdateMallSalesBySalesType(_dbControl);
    UpdateTablePatronCountTable(_dbControl);
}
//--------------------------------------------------------------------
void TApplyParser::update6_42Tables()
{
    Create6_42Generator(_dbControl);
  
}
//--------------------------------------------------------------------
void TApplyParser::update6_43Tables()
{
    CreateTable6_43(_dbControl);
    Create6_43Generator(_dbControl);
    AlterTableDiscount6_43(_dbControl);
    AlterTableOrders6_43(_dbControl);

   AlterTableTIMECLOCKLOCATIONS6_43(_dbControl);
   AlterTableCONTACTTIME6_43(_dbControl);
}
//----------------------------------------------------
void TApplyParser::update6_44Tables()
{
    InsertIntoMallExportSettings6_44(_dbControl, 27, "MEZZANINE_AREA", "btnMezzanineArea", 'T');
    int settingID[1] = {27};
    InsertInTo_MallExport_Settings_Mapping(_dbControl, settingID, 1, 2);
    InsertInTo_MallExport_Settings_Values6_44(_dbControl, 27, 2);
    CreateMezzanineAreaTable6_44(_dbControl);
    CreateMezzanineSalesTable6_44(_dbControl);
    CREATEDSR_PIVOT_BY_ITEMProcedure6_46( _dbControl ) ;
	POPULATEDSR_PIVOT_BY_ITEMProcedure6_46( _dbControl ) ;
    CREATEDSRPIVOTProcedure6_46( _dbControl ) ;
	POPULATEDSRPIVOTProcedure6_46( _dbControl ) ;
}
//----------------------------------------------------
void TApplyParser::UpdateChargeToAccount(TDBControl* const inDBControl)
{
    int invoiceInterface = 25;
    char* invoiceInterfaceChar = new char[2];
    itoa(invoiceInterface,invoiceInterfaceChar,10);
    UnicodeString invoiceInterfaceString = invoiceInterfaceChar;

    int chargeToAccount = 29;
    char* chargeToAccountChar = new char[2];
    itoa(chargeToAccount,chargeToAccountChar,10);
    UnicodeString chargeToAccountString = chargeToAccountChar;

    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *FetchQuery    = transaction.Query(transaction.AddQuery());
        FetchQuery->Close();
        FetchQuery->SQL->Text =    "SELECT a.PAYMENT_KEY, a.PROPERTIES  FROM PAYMENTTYPES a";
        FetchQuery->ExecQuery();
        for (; !FetchQuery->Eof;)
        {
            AnsiString properties = FetchQuery->FieldByName("PROPERTIES")->AsString;
            AnsiString newProperty = "-";
            int paymentkey = FetchQuery->FieldByName("PAYMENT_KEY")->AsInteger;
            if(properties.Pos(invoiceInterfaceString) != 0 && properties.Pos(chargeToAccountString) == 0)
            {
                // loop for total number of payment properties
                for(int i = 1 ; i < 34 ; i++)
                {
                   AnsiString literal = "";
                   literal += "-";
                   literal += i;
                   literal += "-";
                   if(properties.Pos(literal) != 0)
                   {
                      if(i != invoiceInterface)
                      {
                         newProperty += i;
                         newProperty += "-";
                      }
                      else
                      {
                         if(properties.Pos(chargeToAccountString) == 0)
                         {
                             newProperty += chargeToAccountString + "-";
                             UpdatePaymentAttributesTable(_dbControl,paymentkey,chargeToAccount,invoiceInterface);
                         }
                         else
                         {
                             newProperty += chargeToAccountString + "-";
                             DelFromPaymentAttributesTable(_dbControl,invoiceInterface,paymentkey);
                         }
                      }
                   }
                }
                // Update property in paymenttypes
                UpdatePaymentTypesTable(_dbControl, newProperty, paymentkey);
            }
            else if(properties.Pos(invoiceInterfaceString) != 0 && properties.Pos(chargeToAccountString) != 0)
            {
                newProperty = "-";
                for(int i = 1 ; i < 34 ; i++)
                {
                   AnsiString literal = "";
                   literal += "-";
                   literal += i;
                   literal += "-";
                   if(properties.Pos(literal) != 0)
                   {
                      if(i != invoiceInterface)
                      {
                         if(newProperty.Pos(chargeToAccountString) != 0 && i == chargeToAccount)
                             newProperty += "";
                         else
                         {
                             newProperty += i ;
                             newProperty += "-";
                         }
                      }
                      else
                      {
                         newProperty += chargeToAccountString + "-";
                         DelFromPaymentAttributesTable(_dbControl,invoiceInterface,paymentkey);
                      }
                   }
                }
                // Update property in paymenttypes
                UpdatePaymentTypesTable(_dbControl, newProperty, paymentkey);
            }
            FetchQuery->Next();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
void TApplyParser::UpdatePaymentTypesTable(TDBControl* const inDBControl, AnsiString newProperty, int paymentkey)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery = transaction.Query(transaction.AddQuery());
        UpdateQuery->Close();
        UpdateQuery->SQL->Text =  "UPDATE PAYMENTTYPES a SET a.PROPERTIES = :PROPERTIES WHERE a.PAYMENT_KEY = :PAYMENT_KEY";
        UpdateQuery->ParamByName("PROPERTIES")->AsString = newProperty;
        UpdateQuery->ParamByName("PAYMENT_KEY")->AsInteger = paymentkey;
        UpdateQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }

}
void TApplyParser::UpdatePaymentAttributesTable(TDBControl* const inDBControl, int key, int chargeToAccount, int invoiceInterface)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        UpdateQuery->Close();
        UpdateQuery->SQL->Text =  " UPDATE PAYMENT_ATTRIBUTES a SET a.ATTRIBUTE_VALUE = :NEW_ATTRIBUTE WHERE a.PAYMENT_KEY = :PAYMENT_KEY "
                                  " AND a.ATTRIBUTE_VALUE = :OLD_ATTRIBUTE" ;
        UpdateQuery->ParamByName("PAYMENT_KEY")->AsInteger = key;
        UpdateQuery->ParamByName("NEW_ATTRIBUTE")->AsInteger = chargeToAccount;
        UpdateQuery->ParamByName("OLD_ATTRIBUTE")->AsInteger = invoiceInterface;
        UpdateQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
void TApplyParser::DelFromPaymentAttributesTable(TDBControl* const inDBControl, int property, int key)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        TIBSQL *DeleteQuery    = transaction.Query(transaction.AddQuery());
        DeleteQuery->Close();
        DeleteQuery->SQL->Text =  " DELETE FROM PAYMENT_ATTRIBUTES a WHERE a.PAYMENT_KEY = :PAYMENT_KEY "
                                  " AND a.ATTRIBUTE_VALUE = :PAYMENT_ATTRIBUTE" ;
        DeleteQuery->ParamByName("PAYMENT_KEY")->AsInteger = key;
        DeleteQuery->ParamByName("PAYMENT_ATTRIBUTE")->AsInteger = property;
        DeleteQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::AlterTable6_41(TDBControl* const inDBControl)
{
    if ( !fieldExists( "MALL_SALES_BY_SALES_TYPE", "DATE_CREATED", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE MALL_SALES_BY_SALES_TYPE "
        "ADD DATE_CREATED TIMESTAMP ",
        inDBControl);
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::AlterTableTIMECLOCKLOCATIONS6_43(TDBControl* const inDBControl)
{
    if ( !fieldExists( "TIMECLOCKLOCATIONS", "DATE_CREATED", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE TIMECLOCKLOCATIONS "
         "ADD DATE_CREATED TIMESTAMP;",
        inDBControl);
    }
   if ( !fieldExists( "TIMECLOCKLOCATIONS", "STATUS", _dbControl ) )
    {
        executeQuery(
		"ALTER TABLE TIMECLOCKLOCATIONS ADD STATUS CHAR(1) DEFAULT 'T' NOT NULL;",
		inDBControl);
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::AlterTableCONTACTTIME6_43(TDBControl* const inDBControl)
{
   if ( !fieldExists( "CONTACTTIME", "STATUS", _dbControl ) )
    {
        executeQuery(
		"ALTER TABLE CONTACTTIME ADD STATUS CHAR(1) DEFAULT 'T' NOT NULL;",
         inDBControl);
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::UpdateMallSalesBySalesType(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        TIBSQL *SelectQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT a.ARCBILL_KEY, a.DATE_CREATED, a.DEVICE_KEY "
                                 "FROM MALLEXPORT_SALES a "
                                 "WHERE a.FIELD_INDEX = :FIELD_INDEX "
                                 "GROUP BY 1,2,3 "
                                 "ORDER BY 1 ASC ";
        SelectQuery->ParamByName("FIELD_INDEX")->AsInteger = 1;
        SelectQuery->ExecQuery();

        for (; !SelectQuery->Eof; SelectQuery->Next())
        {
            UpdateQuery->Close();
            UpdateQuery->SQL->Text = "UPDATE MALL_SALES_BY_SALES_TYPE MSST SET MSST.DATE_CREATED = :DATE_CREATED "
                                     "WHERE MSST.ARCBILL_KEY = :ARCBILL_KEY AND "
                                     "MSST.DEVICE_KEY = :DEVICE_KEY ";
            UpdateQuery->ParamByName("DATE_CREATED")->AsDateTime =  SelectQuery->FieldByName("DATE_CREATED")->AsDateTime;
            UpdateQuery->ParamByName("ARCBILL_KEY")->AsInteger =  SelectQuery->FieldByName("ARCBILL_KEY")->AsInteger;
            UpdateQuery->ParamByName("DEVICE_KEY")->AsInteger =  SelectQuery->FieldByName("DEVICE_KEY")->AsInteger;
            UpdateQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }

}
//--------------------------------------------------------------------------------------------------
void TApplyParser::UpdateTablePatronCountTable(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();
    int _patronTypeKey = 0;
    try
    {


        TIBSQL *SelectQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *SelectPatronTypeKeyQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = " SELECT a.PATRONTYPES_KEY, a.PATRON_TYPE, a.IS_DEFAULT "
                                 " FROM PATRONTYPES a WHERE a.IS_DEFAULT = :IS_DEFAULT " ;
        SelectQuery->ParamByName("IS_DEFAULT")->AsString = "T";
        SelectQuery->ExecQuery();

        if(!SelectQuery->RecordCount)
        {
            SelectPatronTypeKeyQuery->Close();
            SelectPatronTypeKeyQuery->SQL->Text = " Select FIRST(1) a.PATRONTYPES_KEY FROM PATRONTYPES a " ;
            SelectPatronTypeKeyQuery->ExecQuery();

            if(SelectPatronTypeKeyQuery->RecordCount)
            {
                _patronTypeKey = SelectPatronTypeKeyQuery->FieldByName("PATRONTYPES_KEY")->AsInteger;
            }
        }

        if(_patronTypeKey > 0)
        {
            TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
            UpdateQuery->Close();
            UpdateQuery->SQL->Text =  " UPDATE PATRONTYPES a SET a.IS_DEFAULT = :IS_DEFAULT WHERE a.PATRONTYPES_KEY = :PATRONTYPES_KEY " ;
            UpdateQuery->ParamByName("IS_DEFAULT")->AsString = "T";
            UpdateQuery->ParamByName("PATRONTYPES_KEY")->AsInteger = _patronTypeKey;
            UpdateQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::Create6_42Generator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_BEVERAGEINVOICENUMBER", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_BEVERAGEINVOICENUMBER;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_BEVERAGEINVOICENUMBER TO 0;", inDBControl
        );
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::CreateTable6_43(TDBControl* const inDBControl)
{
    if ( !tableExists( "ROOM_GUEST_DETAILS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE ROOM_GUEST_DETAILS "
		"( "
		"   GUEST_DETAILS_KEY INT NOT NULL PRIMARY KEY,"
        "   INVOICE_NUMBER VARCHAR(50), "
		"   ACC_NUMBER VARCHAR(20),"
        "   ROOM_NUMBER VARCHAR(20), "
        "   FIRST_NAME VARCHAR(50),"
        "   LAST_NAME VARCHAR(50), "
        "   AMOUNT  NUMERIC(15,4) "
		");",
		inDBControl );
    }
}
//-------------------------------------------------------------------------------
void TApplyParser::Create6_43Generator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_GUEST_DETAILS_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_GUEST_DETAILS_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_GUEST_DETAILS_KEY TO 0;", inDBControl
        );
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::AlterTableDiscount6_43(TDBControl* const inDBControl)
{
    if (fieldExists( "DISCOUNTS", "DESCRIPTION", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE DISCOUNTS  "
        "ALTER DESCRIPTION TYPE VARCHAR(200) ; ",
		inDBControl);
	}
}
//-----------------------------------------------------------------------------------
void TApplyParser::AlterTableOrders6_43(TDBControl* const inDBControl)
{
    if ( !fieldExists( "ORDERS", "ROOM_NO", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ROOM_NO VARCHAR(20) ",
        inDBControl);
    }
     if ( !fieldExists( "ORDERS", "ACC_NO", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ACC_NO VARCHAR(20) ",
        inDBControl);
    }
     if ( !fieldExists( "ORDERS", "FIRST_NAME", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD FIRST_NAME VARCHAR(50) ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "LAST_NAME", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD LAST_NAME VARCHAR(50) ",
        inDBControl);
    }
}
//--------------------------------------------------------------------------------------------void TApplyParser::InsertIntoMallExportSettings6_44(TDBControl* const inDBControl, int settingKey, UnicodeString fiedlName, UnicodeString controlName, char isUIRequired){
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
         if ( tableExists( "MALLEXPORT_SETTINGS", _dbControl ) )
	    {
            TIBSQL *InsertQuery    = transaction.Query(transaction.AddQuery());
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_SETTINGS VALUES (:SETTING_KEY, :FIELD_NAME, :CONTROL_NAME, :IS_UI) ";
            InsertQuery->ParamByName("SETTING_KEY")->AsInteger = settingKey;
            InsertQuery->ParamByName("FIELD_NAME")->AsString = fiedlName;
            InsertQuery->ParamByName("CONTROL_NAME")->AsString = controlName;
            InsertQuery->ParamByName("IS_UI")->AsString = isUIRequired;
            InsertQuery->ExecQuery();

            transaction.Commit();
        }
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}//--------------------------------------------------------------------------------------------------void TApplyParser::CreateMezzanineAreaTable6_44(TDBControl* const inDBControl){    if ( !tableExists( "MEZZANINE_AREA_TABLES", _dbControl ) )	{
		executeQuery(
		"CREATE TABLE MEZZANINE_AREA_TABLES "
		"( "
		"   TABLE_ID INTEGER PRIMARY KEY,"
		"   TABLE_NUMBER INTEGER, "
        "   LOCATION_ID INTEGER, "
        "   FLOORPLAN_VER  INTEGER "
		");",
		inDBControl );
	}    if(!generatorExists("GEN_MEZZANINE_TABLE_ID", _dbControl))    {
        executeQuery(
            "CREATE GENERATOR GEN_MEZZANINE_TABLE_ID;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_MEZZANINE_TABLE_ID TO 0;", inDBControl
        );
    }}//--------------------------------------------------------------------------------------------void TApplyParser::CreateMezzanineSalesTable6_44(TDBControl* const inDBControl){    if ( !tableExists( "MEZZANINE_SALES", _dbControl ) )	{
		executeQuery(
		"CREATE TABLE MEZZANINE_SALES "
		"( "
		"   SALES_ID INTEGER PRIMARY KEY,"
		"   TABLE_NUMBER INTEGER, "
        "   GROSS_SALES NUMERIC(15,4), "
        "   TIME_STAMP_BILLED TIMESTAMP, "
        "   PWD NUMERIC(17,4), "
        "   SCD NUMERIC(17,4), "
        "   OTHER_DISCOUNTS NUMERIC(17,4), "
        "   VAT_EXEMPT_SALES NUMERIC(17,4), "
        "   SERVICE_CHARGE NUMERIC(17,4), "
        "   VAT NUMERIC(17,4), "
        "   LOCATION_ID INTEGER, "
        "   TERMINAL_NAME VARCHAR(22), "
        "   Z_KEY INTEGER "
		");",
		inDBControl );
	}    if(!generatorExists("GEN_MEZZANINE_SALES_ID", _dbControl))    {
        executeQuery(
            "CREATE GENERATOR GEN_MEZZANINE_SALES_ID;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_MEZZANINE_SALES_ID TO 0;", inDBControl
        );
    }}//------------------------------------------------------------------------------------------void TApplyParser::InsertInTo_MallExport_Settings_Values6_44(TDBControl* const inDBControl, int settingId, int mallId){    TDBTransaction transaction( *inDBControl );    transaction.StartTransaction();

    try
    {
        TIBSQL *SelectQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT a.DEVICE_KEY FROM MALLEXPORT_SETTINGS_VALUES a "
                                 "WHERE a.MALL_KEY = :MALL_KEY GROUP BY 1 ";
        SelectQuery->ParamByName("MALL_KEY")->AsInteger = mallId;
        SelectQuery->ExecQuery();
        int index = GetMallExportSettingValueKey(_dbControl);
        for (; !SelectQuery->Eof; SelectQuery->Next())
        {            if ( tableExists( "MALLEXPORT_SETTINGS_VALUES", _dbControl ) )            {
                TIBSQL *InsertQuery    = transaction.Query(transaction.AddQuery());
                InsertQuery->Close();
                InsertQuery->SQL->Text =
                            "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (:MALLEXPORT_SETTING_VALUE_KEY, :MALLEXPORTSETTING_ID, :FIELD_VALUE, "
                            " :FIELD_TYPE, :DEVICE_KEY, :MALL_KEY ) ";
                InsertQuery->ParamByName("MALLEXPORT_SETTING_VALUE_KEY")->AsInteger = index++;
                InsertQuery->ParamByName("MALLEXPORTSETTING_ID")->AsInteger = settingId;
                InsertQuery->ParamByName("FIELD_VALUE")->AsString = "true";
                InsertQuery->ParamByName("FIELD_TYPE")->AsString = "UnicodeString";
                InsertQuery->ParamByName("DEVICE_KEY")->AsInteger = SelectQuery->FieldByName("DEVICE_KEY")->AsInteger;
                InsertQuery->ParamByName("MALL_KEY")->AsInteger = mallId;
                InsertQuery->ExecQuery();
            }
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }}//--------------------------------------------------------------------------------------int TApplyParser::GetMallExportSettingValueKey(TDBControl* const inDBControl){    int index = 0;    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        if ( tableExists( "MALLEXPORT_SETTINGS_VALUES", _dbControl ) )
        {
            TIBSQL *selectQuery    = transaction.Query(transaction.AddQuery());
            selectQuery->SQL->Text = "SELECT MAX(A.MALLEXPORT_SETTING_VALUE_KEY) MALLEXPORT_SETTING_VALUE_KEY FROM MALLEXPORT_SETTINGS_VALUES a ";
            selectQuery->ExecQuery();

            if(selectQuery->RecordCount)
                index = selectQuery->FieldByName("MALLEXPORT_SETTING_VALUE_KEY")->AsInteger;
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
    return index + 1;}//--------------------------------------------------------------------------------------------------void TApplyParser::CREATEDSR_PIVOT_BY_ITEMProcedure6_46( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"RECREATE PROCEDURE CREATE_DSR_BY_ITEM ( "
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
		"SELECT distinct upper(ARCBILL_PAY.PAY_TYPE) "
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
		/*
"GRANT EXECUTE "
"ON PROCEDURE CREATE_DSR_PIVOT TO  SYSDBA " ;       */
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}
//-----------------------------------------------------
void TApplyParser::POPULATEDSR_PIVOT_BY_ITEMProcedure6_46( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"RECREATE PROCEDURE POPULATE_DSR_BY_ITEM ( ATABLENAME Varchar(50), STARTTIME Timestamp, ENDTIME Timestamp ) "
		"AS "
		"DECLARE VARIABLE stmnt VARCHAR(8000); "
		"DECLARE VARIABLE updatestmt VARCHAR(8000); "
		"DECLARE VARIABLE updatetaxstmt VARCHAR(8000); "
		"DECLARE VARIABLE updatemenustmt VARCHAR(8000); "
		"DECLARE VARIABLE UpdateMenuDiscountstmt VARCHAR(8000); "
		"DECLARE VARIABLE UpdatePayTypeStmt VARCHAR(8000); "
		"DECLARE VARIABLE RegNum VARCHAR(50); "
		"DECLARE VARIABLE AN VARCHAR(50); "
		"DECLARE VARIABLE PAY_TYPE VARCHAR(50); "
		"DECLARE VARIABLE ARCBILL_KEY Integer; "
		"DECLARE VARIABLE INVOICE_NUMBER VARCHAR(50); "
		"DECLARE VARIABLE TIME_STAMP VARCHAR(50); "
		"DECLARE VARIABLE ITEMNAME VARCHAR(50); "
		"DECLARE VARIABLE ITEM_NAME Timestamp ; "
		"DECLARE VARIABLE SIZE_NAME VARCHAR(50) ; "
		"DECLARE VARIABLE PRICE Numeric(15,4); "
		"DECLARE VARIABLE QTY VARCHAR(50); "
		"DECLARE VARIABLE VAT VARCHAR(50) ; "
		"DECLARE VARIABLE SERVICECHARGE  VARCHAR(50); "
		"DECLARE VARIABLE AMOUNT VARCHAR(50); "
		"DECLARE VARIABLE ARCORDERTAXES_KEY VARCHAR(50); "
		"DECLARE VARIABLE ARCHIVE_KEY VARCHAR(50); "
		"DECLARE VARIABLE TAX_NAME VARCHAR (50); "
		"DECLARE VARIABLE TAX_VALUE VARCHAR(50); "
		"DECLARE VARIABLE TAX_TYPE VARCHAR(50);  "
		"DECLARE VARIABLE MENU_NAME VARCHAR(50); "
		"DECLARE VARIABLE SUBTOTAL VARCHAR(50); "
		"DECLARE VARIABLE ARCBILLPAY_KEY VARCHAR(50); "
		"DECLARE VARIABLE TOTAL VARCHAR(50); "
		"DECLARE VARIABLE DISCOUNT VARCHAR(50); "
		"DECLARE VARIABLE SURCHARGE VARCHAR(50); "
		"DECLARE VARIABLE MenuDiscountColData VARCHAR(50); "
		"DECLARE VARIABLE Course_Name VARCHAR(50); "
		"DECLARE VARIABLE Item_Count VARCHAR(50); "
		"DECLARE VARIABLE NETSALE VARCHAR(50); "
		"DECLARE VARIABLE TAXSUM VARCHAR(50); "
		"DECLARE VARIABLE TOTALAMOUNT Numeric(15,4); "
		"DECLARE VARIABLE COMP VARCHAR(50); "
		"DECLARE VARIABLE AK bigint=0; "
		"BEGIN "

		"execute  statement 'delete from '||ATableName||';'; COMP='0'; FOR select  a.ARCHIVE_KEY from archive a  where a.TIME_STAMP_BILLED>=:StartTime and a.TIME_STAMP_BILLED<:Endtime  order by 1 into RegNum  "
		"DO begin stmnt = 'INSERT INTO '||ATABLENAME||' (SRNO) VALUES('''|| RegNum||''')';  "
		"execute statement stmnt;  "
		"end FOR SELECT b.ARCHIVE_KEY , a.TIME_STAMP, CAST(b.qty*coalesce(b.PRICE,0) AS NUMERIC(17,4)) PRICE, a.INVOICE_NUMBER,b.ITEM_NAME FROM ARCBILL a "
                    "left join ARCHIVE b on a.ARCBILL_KEY=b.ARCBILL_KEY where b.TIME_STAMP_BILLED>=:StartTime and b.TIME_STAMP_BILLED<:Endtime  "
		"INTO ARCBILL_KEY , TIME_STAMP ,TOTAL   , INVOICE_NUMBER , ITEMNAME "
		"DO begin  ITEMNAME=REPLACE ( ITEMNAME, '''', ''); "

		"updatestmt ='UPDATE '|| ATABLENAME||' set TIME_STAMP = '''||TIME_STAMP ||''' , BILLAMOUNT = '''||TOTAL||''' , INVOICE_NUMBER = '''|| INVOICE_NUMBER||''' , ITEMNAME = '''|| ITEMNAME||'''  WHERE SRNO = '''||ARCBILL_KEY||''''; "
		"execute statement updatestmt;  "
		"end "
		"FOR "
		" SELECT  b.ARCHIVE_KEY, a.PAY_TYPE, SUM(a.SUBTOTAL)AS SUBTOTAL,c.INVOICE_NUMBER  FROM  ARCHIVE b LEFT join  ARCBILLPAY a ON b .ARCBILL_KEY=A.ARCBILL_KEY left join ARCBILL c on b.ARCBILL_KEY=c.ARCBILL_KEY  "
		"where b .TIME_STAMP_BILLED>=:StartTime and b .TIME_STAMP_BILLED<:Endtime  "
		"GROUP BY  b.ARCHIVE_KEY, a.PAY_TYPE,c.INVOICE_NUMBER  INTO ARCBILL_KEY, PAY_TYPE , SUBTOTAL,INVOICE_NUMBER  "

		/*	"SELECT  b.ARCHIVE_KEY, a.PAY_TYPE, SUM(a.SUBTOTAL) AS SUBTOTAL FROM ARCBILLPAY a LEFT join ARCHIVE b ON b .ARCBILL_KEY=A.ARCBILL_KEY where b .TIME_STAMP>=:StartTime and b .TIME_STAMP<:Endtime  "
			"GROUP BY  b.ARCHIVE_KEY, a.PAY_TYPE  INTO ARCBILL_KEY, PAY_TYPE , SUBTOTAL  "  */
		"DO begin     "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,,', ','); "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',,', ',');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,', ',');   "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');    "
		"PAY_TYPE=REPLACE (PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE (PAY_TYPE, '%', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '<', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '>', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '?', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '^', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '&', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '-', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '--', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '*', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '#', ''); "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '@', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '(', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ')', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '+', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '{', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '[', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '/', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ':', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ';', '');   "
        "PAY_TYPE=REPLACE (PAY_TYPE, '''',''); "
		"if (PAY_TYPE <> '') then  "
		"if( (comp='0')or (comp<>INVOICE_NUMBER)or (comp = INVOICE_NUMBER and AK=ARCBILL_KEY ))then  "
		"begin UpdatePayTypeStmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (PAY_TYPE, ' ', '') )|| ' = ''' || SUBTOTAL|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''' and ''' || SUBTOTAL||'''!=0' ; "
		"COMP= INVOICE_NUMBER; "
		"AK=ARCBILL_KEY; "
		"execute statement UpdatePayTypeStmt; "
		"end  "
		"else  "
		"begin UpdatePayTypeStmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (PAY_TYPE, ' ', '') )|| ' = ''' || ''|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''' and ''' || SUBTOTAL||'''!=0' ;  "
		"execute statement UpdatePayTypeStmt;  "
		"end "
		"end "
		/*"if (PAY_TYPE <> '') then  "
			"UpdatePayTypeStmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (PAY_TYPE, ' ', '') )|| ' = ''' || SUBTOTAL|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''' and ''' || SUBTOTAL||'''!=0' ;  "
			"if (PAY_TYPE <> '')  "
			"then execute statement UpdatePayTypeStmt;  "
			"end    "   */
		"FOR SELECT AC.ARCHIVE_KEY, a.TAX_NAME, sum(a.TAX_VALUE) TAX_VALUE, a.TAX_TYPE FROM ARCORDERTAXES a inner join ARCHIVE AC on "
                        "AC.ARCHIVE_KEY=a.ARCHIVE_KEY INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY=AC.ARCBILL_KEY "
                        "where  a.TAX_NAME<>'' and AC.TIME_STAMP_BILLED>=:StartTime and  AC.TIME_STAMP_BILLED<:Endtime  GROUP BY AC.ARCHIVE_KEY,   a.TAX_NAME, "
                        "a.TAX_TYPE INTO ARCBILL_KEY , TAX_NAME  , TAX_VALUE   , TAX_TYPE "
		"DO begin     "
		"TAX_NAME='Tax_' ||TAX_NAME; "
		"TAX_NAME=REPLACE (TAX_NAME, '0.00%', ''); "
		"TAX_NAME=REPLACE (TAX_NAME, '000%', '');  "
		"TAX_NAME=REPLACE (TAX_NAME, '000', '');   "
		"TAX_NAME=REPLACE (TAX_NAME, '00', '');  "
		"TAX_NAME=REPLACE (TAX_NAME, ', ,,', ','); "
		"TAX_NAME=REPLACE (TAX_NAME, ',,', ',');   "
		"TAX_NAME=REPLACE (TAX_NAME, ',)', ')');   "
		"TAX_NAME=REPLACE (TAX_NAME, ', ,', ',');  "
		"TAX_NAME=REPLACE (TAX_NAME, ',)', ')');  "
		"TAX_NAME=REPLACE (TAX_NAME, '.', '');    "
		"TAX_NAME=REPLACE (TAX_NAME, '%', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, '.', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '<', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '>', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '?', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '^', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '&', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '@', ''); "
		"TAX_NAME=REPLACE ( TAX_NAME, '-', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '--', ''); "
		"TAX_NAME=REPLACE ( TAX_NAME, '!', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '*', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '#', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '(', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, ')', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '+', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '{', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '[', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, '/', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '!', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, ':', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, ';', '');    "
        "TAX_NAME=REPLACE (TAX_NAME, '''',''); "
		"updatetaxstmt = 'UPDATE '||ATABLENAME||' set ' ||upper(REPLACE (TAX_NAME, ' ', '') ) || ' = ''' || TAX_VALUE|| '''  WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"execute statement updatetaxstmt;  "
		"end   "
		"for select AB.ARCHIVE_KEY,  SUM(TAX_VALUE) TAX_VALUE   FROM ( SELECT AC.ARCHIVE_KEY,AC.TIME_STAMP_BILLED TIME_STAMP,  sum(a.TAX_VALUE) TAX_VALUE "
            "FROM ARCORDERTAXES a INNER JOIN ARCHIVE AC on AC.ARCHIVE_KEY=a.ARCHIVE_KEY where a.TAX_NAME<>'' AND AC.TIME_STAMP_BILLED>=:StartTime and  AC.TIME_STAMP_BILLED<:Endtime  group BY AC.ARCHIVE_KEY,AC.TIME_STAMP_BILLED) AB "
            "WHERE  AB.TIME_STAMP>=:StartTime and  AB.TIME_STAMP<:Endtime  "
		"group BY 1  into ARCBILL_KEY ,TAXSUM DO begin stmnt = ''; "
		"stmnt = 'UPDATE '||ATABLENAME||' set SUB_TOTAL_TAX = ''' || TAXSUM|| '''  WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"execute statement stmnt; "
		"end  "
		"FOR Select  ARCHIVE.ARCHIVE_KEY,ARCHIVE.MENU_NAME MENU_NAME, COALESCE( Cast(Sum(Archive.Discount) as Numeric(15,4)), 0) Discount, "
                "COALESCE (Cast(Sum(Archive.Surcharge) as Numeric(15,4)),0) Surcharge, "
                "Cast(Sum (abs(Archive.Qty) *  (COALESCE(Archive.BASE_PRICE,0))+ COALESCE(Archive.Discount, 0) + COALESCE(Archive.Surcharge, 0)) as Numeric(15,4)) SubTotal  "
             "From ARCHIVE Ab  Left Join   ( Select    ARCHIVE.ARCBILL_KEY, ARCHIVE.ARCHIVE_KEY, Archive.Qty, ARCHIVE.BASE_PRICE  , "
                "ARCHIVE.MENU_NAME, MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX >= 0 THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END)    AS Surcharge, "
                "MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX < 0 THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END) AS Discount From Archive Where "
                "(Archive.Order_Type = 3 or Archive.Order_Type = 0) AND (Archive.TIME_STAMP_BILLED>=:StartTime and  Archive.TIME_STAMP_BILLED<:Endtime) "
                "  Group By ARCHIVE.ARCBILL_KEY,ARCHIVE.ARCHIVE_KEY , ARCHIVE.BASE_PRICE,ARCHIVE.MENU_NAME,Archive.Qty ) Archive on Ab.ARCHIVE_KEY = "
                "Archive.ARCHIVE_KEY WHERE Ab.TIME_STAMP_BILLED>=:StartTime and  Ab.TIME_STAMP_BILLED<:Endtime Group By ARCHIVE.ARCHIVE_KEY, ARCHIVE.MENU_NAME Having Count(Archive.Archive_Key) > 0 "
        "Into ARCBILL_KEY,MENU_NAME,DISCOUNT, SURCHARGE ,SubTotal "
		"DO begin MENU_NAME=REPLACE (MENU_NAME, ', ,,', ',');  "
		"MENU_NAME=REPLACE (MENU_NAME, ',,', ',');     "
		"MENU_NAME=REPLACE (MENU_NAME, ',)', ')');    "
		"MENU_NAME=REPLACE (MENU_NAME, ', ,', ',');   "
		"MENU_NAME=REPLACE (MENU_NAME, ',)', ')');  "
		"MENU_NAME=REPLACE (MENU_NAME, '.', '');     "
		"MENU_NAME=REPLACE (MENU_NAME, '%', '');     "
		"MENU_NAME=REPLACE ( MENU_NAME, '.', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '<', '');     "
		"MENU_NAME=REPLACE ( MENU_NAME, '>', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '?', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '^', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '&', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '-', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '--', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '!', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '*', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '#', ''); "
		"MENU_NAME=REPLACE ( MENU_NAME, '@', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '(', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ')', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '+', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '{', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '[', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '/', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '!', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ':', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ';', '');  "
        "MENU_NAME=REPLACE (MENU_NAME, '''','');  "
		"updatemenustmt='';  "
		"if (MENU_NAME <> '')  "
		"then "
		"updatemenustmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' ))|| ' = ''' || SubTotal|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"if (MENU_NAME <> '') "
		"then  "
		"execute statement updatemenustmt; "
		"MenuDiscountColData='_Discount_Amount'; "
		"if (MENU_NAME <> '') then  "
		"UpdateMenuDiscountstmt = 'UPDATE '||ATABLENAME||' set ' ||upper(REPLACE ( REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' )) || ''|| MenuDiscountColData || ' = ''' || DISCOUNT|| ''' WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"if (MENU_NAME <> '') then execute statement UpdateMenuDiscountstmt;  "
		"stmnt = '';  "
		"MenuDiscountColData='_Surcharge_Amount'; "
		"if (MENU_NAME <> '') then stmnt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' ))|| ''|| MenuDiscountColData || ' = ''' || SURCHARGE|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"if (MENU_NAME <> '')  "
		"then execute statement stmnt; "
		"end       "
		"FOR  SELECT  TM.archive_key, SUM(TM.SubTotal + COALESCE( TM.TAX_VALUE,0)) Total "
                "FROM ( SELECT AC.ARCHIVE_KEY,AC.TIME_STAMP_BILLED , TA.SubTotal, sum(a.TAX_VALUE) TAX_VALUE FROM ARCHIVE   AC left join ARCORDERTAXES a  on "
                "AC.ARCHIVE_KEY = a.ARCHIVE_KEY   INNER JOIN ( SELECT ARCHIVE.ARCHIVE_KEY, Cast(Sum(abs(Archive.Qty) * (COALESCE(Archive.BASE_PRICE,0)) "
                    " + COALESCE (Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(15,4)) SubTotal  "
                                "From  Archive Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  "
                    " Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
                " Where  (Archive.Order_Type = 3 or Archive.Order_Type = 0) AND Archive.TIME_STAMP_BILLED>=:StartTime and   Archive.TIME_STAMP_BILLED<:Endtime "
                "Group By ARCHIVE.ARCHIVE_KEY Having Count (Archive.Archive_Key) > 0 ) TA on TA.ARCHIVE_KEY = AC.ARCHIVE_KEY  Where a.TAX_NAME<>'' "
                "AND AC.TIME_STAMP_BILLED>=:StartTime and   AC.TIME_STAMP_BILLED<:Endtime   GROUP BY 1,2,3) TM "
                " WHERE TM.TIME_STAMP_BILLED>=:StartTime and   TM.TIME_STAMP_BILLED<:Endtime  group BY 1  INTO ARCBILL_KEY,  TOTALAMOUNT DO begin stmnt = '';        "
		" stmnt='UPDATE '||ATABLENAME||' set TOTAL_AMOUNT = ''' || TOTALAMOUNT|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"execute statement stmnt; "
		"end "
		"SUSPEND;   "
		"END " , inDBControl );
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}
//-----------------------------------------------------
void TApplyParser::CREATEDSRPIVOTProcedure6_46( TDBControl* const inDBControl )
{	TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"RECREATE PROCEDURE CREATE_DSR_PIVOT ( "
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
		"SELECT distinct upper(ARCBILL_PAY.PAY_TYPE) "
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
		/*
"GRANT EXECUTE "
"ON PROCEDURE CREATE_DSR_PIVOT TO  SYSDBA " ;       */
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::POPULATEDSRPIVOTProcedure6_46( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *inDBControl );
	try
	{
		executeQuery(
		"RECREATE PROCEDURE POPULATE_DSR_PIVOT ( ATABLENAME Varchar(50), STARTTIME Timestamp, ENDTIME Timestamp ) "
		"AS "
		"DECLARE VARIABLE stmnt VARCHAR(8000); "
		"DECLARE VARIABLE updatestmt VARCHAR(8000); "
		"DECLARE VARIABLE updatetaxstmt VARCHAR(8000); "
		"DECLARE VARIABLE updatemenustmt VARCHAR(8000); "
		"DECLARE VARIABLE UpdateMenuDiscountstmt VARCHAR(8000); "
		"DECLARE VARIABLE UpdatePayTypeStmt VARCHAR(8000); "
		"DECLARE VARIABLE RegNum VARCHAR(50); "
		"DECLARE VARIABLE AN VARCHAR(50); "
		"DECLARE VARIABLE PAY_TYPE VARCHAR(50); "
		"DECLARE VARIABLE ARCBILL_KEY Integer; "
		"DECLARE VARIABLE INVOICE_NUMBER VARCHAR(50); "
		"DECLARE VARIABLE TIME_STAMP VARCHAR(50); "
		"DECLARE VARIABLE ITEM_NAME Timestamp ; "
		"DECLARE VARIABLE SIZE_NAME VARCHAR(50) ; "
		"DECLARE VARIABLE PRICE Numeric(15,4); "
		"DECLARE VARIABLE QTY VARCHAR(50); "
		"DECLARE VARIABLE VAT VARCHAR(50) ; "
		"DECLARE VARIABLE SERVICECHARGE  VARCHAR(50); "
		"DECLARE VARIABLE AMOUNT VARCHAR(50); "
		"DECLARE VARIABLE ARCORDERTAXES_KEY VARCHAR(50); "
		"DECLARE VARIABLE ARCHIVE_KEY VARCHAR(50); "
		"DECLARE VARIABLE TAX_NAME VARCHAR (50); "
		"DECLARE VARIABLE TAX_VALUE VARCHAR(50); "
		"DECLARE VARIABLE TAX_TYPE VARCHAR(50);  "
		"DECLARE VARIABLE MENU_NAME VARCHAR(50); "
		"DECLARE VARIABLE SUBTOTAL VARCHAR(50); "
		"DECLARE VARIABLE ARCBILLPAY_KEY VARCHAR(50); "
		"DECLARE VARIABLE TOTAL VARCHAR(50); "
		"DECLARE VARIABLE DISCOUNT VARCHAR(50); "
		"DECLARE VARIABLE SURCHARGE VARCHAR(50); "
		"DECLARE VARIABLE MenuDiscountColData VARCHAR(50); "
		"DECLARE VARIABLE Course_Name VARCHAR(50); "
		"DECLARE VARIABLE Item_Count VARCHAR(50); "
		"DECLARE VARIABLE NETSALE VARCHAR(50); "
		"DECLARE VARIABLE TAXSUM VARCHAR(50); "
		"DECLARE VARIABLE TOTALAMOUNT Numeric(15,4); "
        "DECLARE VARIABLE ORDER_LOCATION VARCHAR(50); "
		"BEGIN "
		"execute  statement 'delete from '||ATableName||';'; FOR SELECT a.ARCBILL_KEY FROM ARCBILL a  where a.TIME_STAMP>=:StartTime and a.TIME_STAMP<:Endtime  order by 1 into RegNum  "
		"DO begin stmnt = 'INSERT INTO '||ATABLENAME||' (SRNO) VALUES('''|| RegNum||''')';  "
		"execute statement stmnt;  "
		"end FOR SELECT a.ARCBILL_KEY,  a.TIME_STAMP, a.TOTAL, a.INVOICE_NUMBER FROM ARCBILL a where a.TIME_STAMP>=:StartTime and a.TIME_STAMP<:Endtime  "
		"INTO ARCBILL_KEY , TIME_STAMP ,TOTAL   , INVOICE_NUMBER  "
		"DO begin  "
		"updatestmt ='UPDATE '|| ATABLENAME||' set TIME_STAMP = '''||TIME_STAMP ||''' , BILLAMOUNT = '''||TOTAL||''' , INVOICE_NUMBER = '''|| INVOICE_NUMBER||'''   WHERE SRNO = '''||ARCBILL_KEY||''''; "
		"execute statement updatestmt;  "
		"end "
		"FOR "
		"SELECT  a.ARCBILL_KEY, a.PAY_TYPE, SUM(a.SUBTOTAL) AS SUBTOTAL FROM ARCBILLPAY a LEFT join ARCBILL ON ARCBILL.ARCBILL_KEY=A.ARCBILL_KEY "
            "WHERE ARCBILL.TIME_STAMP>=:StartTime and ARCBILL.TIME_STAMP<:Endtime  "
		"GROUP BY  a.ARCBILL_KEY, a.PAY_TYPE INTO ARCBILL_KEY, PAY_TYPE , SUBTOTAL  "
		"DO begin     "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,,', ','); "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',,', ',');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,', ',');   "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');    "
		"PAY_TYPE=REPLACE (PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE (PAY_TYPE, '%', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '<', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '>', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '?', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '^', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '&', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '-', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '--', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '*', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '#', ''); "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '@', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '(', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ')', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '+', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '{', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '[', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '/', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ':', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ';', '');   "
        "PAY_TYPE=REPLACE (PAY_TYPE, '''','');  "
		"if (PAY_TYPE <> '') then  "
		"UpdatePayTypeStmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (PAY_TYPE, ' ', '') )|| ' = ''' || SUBTOTAL|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''' and ''' || SUBTOTAL||'''!=0' ;  "
		"if (PAY_TYPE <> '')  "
		"then execute statement UpdatePayTypeStmt;  "
		"end     "
		"FOR SELECT AB.ARCBILL_KEY, a.TAX_NAME, sum(a.TAX_VALUE) TAX_VALUE, a.TAX_TYPE FROM ARCORDERTAXES a INNER JOIN ARCHIVE AC on "
                    "AC.ARCHIVE_KEY=a.ARCHIVE_KEY INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY=AC.ARCBILL_KEY WHERE  a.TAX_NAME<>'' and "
                    "AC.TIME_STAMP>=:StartTime and  AC.TIME_STAMP<:Endtime GROUP BY AB.ARCBILL_KEY,   a.TAX_NAME, a.TAX_TYPE INTO ARCBILL_KEY , TAX_NAME  , TAX_VALUE   , TAX_TYPE  "
		"DO begin     "
		"TAX_NAME='Tax_' ||TAX_NAME; "
		"TAX_NAME=REPLACE (TAX_NAME, '0.00%', ''); "
		"TAX_NAME=REPLACE (TAX_NAME, '000%', '');  "
		"TAX_NAME=REPLACE (TAX_NAME, '000', '');   "
		"TAX_NAME=REPLACE (TAX_NAME, '00', '');  "
		"TAX_NAME=REPLACE (TAX_NAME, ', ,,', ','); "
		"TAX_NAME=REPLACE (TAX_NAME, ',,', ',');   "
		"TAX_NAME=REPLACE (TAX_NAME, ',)', ')');   "
		"TAX_NAME=REPLACE (TAX_NAME, ', ,', ',');  "
		"TAX_NAME=REPLACE (TAX_NAME, ',)', ')');  "
		"TAX_NAME=REPLACE (TAX_NAME, '.', '');    "
		"TAX_NAME=REPLACE (TAX_NAME, '%', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, '.', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '<', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '>', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '?', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '^', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '&', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '@', ''); "
		"TAX_NAME=REPLACE ( TAX_NAME, '-', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '--', ''); "
		"TAX_NAME=REPLACE ( TAX_NAME, '!', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '*', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '#', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '(', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, ')', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '+', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '{', '');  "
		"TAX_NAME=REPLACE ( TAX_NAME, '[', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, '/', '');   "
		"TAX_NAME=REPLACE ( TAX_NAME, '!', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, ':', '');    "
		"TAX_NAME=REPLACE ( TAX_NAME, ';', '');    "
        "TAX_NAME=REPLACE (TAX_NAME, '''','');  "
		"updatetaxstmt = 'UPDATE '||ATABLENAME||' set ' ||upper(REPLACE (TAX_NAME, ' ', '') ) || ' = ''' || TAX_VALUE|| '''  WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"execute statement updatetaxstmt;  "
		"end  for "
		"SELECT ARCBILL_KEY,  SUM(TAX_VALUE) TAX_VALUE   "
            "FROM ( SELECT AB.ARCBILL_KEY,AB.TIME_STAMP,  sum(a.TAX_VALUE) TAX_VALUE FROM ARCORDERTAXES a INNER JOIN ARCHIVE AC on AC.ARCHIVE_KEY=a.ARCHIVE_KEY "
                    "INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY=AC.ARCBILL_KEY where a.TAX_NAME<>''  AND AB.TIME_STAMP >= :StartTime AND AB.TIME_STAMP < :Endtime "
                    "GROUP BY AB.ARCBILL_KEY,AB.TIME_STAMP) AB where  AB.TIME_STAMP>=:StartTime and  AB.TIME_STAMP<:Endtime group BY 1   "
		"into ARCBILL_KEY ,TAXSUM DO begin stmnt = '';  "
		"stmnt = 'UPDATE '||ATABLENAME||' set SUB_TOTAL_TAX = ''' || TAXSUM|| '''  WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"execute statement stmnt; "
		"end  "
		"FOR  SELECT  a.ARCBILL_KEY, a.PAY_TYPE, SUM(a.SUBTOTAL) AS SUBTOTAL FROM ARCBILLPAY a LEFT JOIN ARCBILL ON ARCBILL.ARCBILL_KEY=A.ARCBILL_KEY "
                "WHERE ARCBILL.TIME_STAMP>=:StartTime and  ARCBILL.TIME_STAMP<:Endtime GROUP BY  a.ARCBILL_KEY, a.PAY_TYPE INTO ARCBILL_KEY, PAY_TYPE , SUBTOTAL "
		" DO begin "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,,', ','); "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',,', ',');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');  "
		"PAY_TYPE=REPLACE (PAY_TYPE, ', ,', ','); "
		"PAY_TYPE=REPLACE (PAY_TYPE, ',)', ')');   "
		"PAY_TYPE=REPLACE (PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE (PAY_TYPE, '%', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '.', '');    "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '<', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '>', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '?', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '^', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '&', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '-', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '--', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '*', ''); "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '@', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '#', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '(', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ')', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '+', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '{', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '[', ''); "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '', '');   "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '/', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, '!', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ':', '');  "
		"PAY_TYPE=REPLACE ( PAY_TYPE, ';', '');  "
        "PAY_TYPE=REPLACE (PAY_TYPE, '''','');  "
		"if (PAY_TYPE <> '') then  "
		"UpdatePayTypeStmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (PAY_TYPE, ' ', '') )|| ' = ''' || SUBTOTAL|| ''' WHERE SRNO = ''' || ARCBILL_KEY||'''  and ''' || SUBTOTAL||'''!=0' ;  "
		"if (PAY_TYPE <> '')  "
		"then execute statement UpdatePayTypeStmt;  "
		"end  "
		"FOR Select  ARCHIVE.ARCBILL_KEY,Ab.ORDER_LOCATION,ARCHIVE.MENU_NAME MENU_NAME, COALESCE( Cast(Sum(Archive.Discount) as Numeric(15,4)), 0) Discount, "
                    "COALESCE (Cast(Sum(Archive.Surcharge) as Numeric(15,4)), 0) Surcharge, Cast(Sum (abs(Archive.Qty) * "
                    "(COALESCE(Archive.BASE_PRICE,0))+ COALESCE(Archive.Discount, 0)+ COALESCE(Archive.Surcharge, 0)) as Numeric(15,4)) SubTotal  From ARCHIVE Ab  Left Join "
                    "( Select    ARCHIVE.ARCBILL_KEY,ARCHIVE.TIME_STAMP_BILLED, ARCHIVE.ARCHIVE_KEY, Archive.Qty, ARCHIVE.BASE_PRICE  , ARCHIVE.MENU_NAME, "
                    "MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX >= 0 THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END)  AS Surcharge, MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX < 0 "
                    "THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END) AS Discount From Archive Where (Archive.Order_Type = 3 or Archive.Order_Type = 0) AND  "
                    "(Archive.TIME_STAMP_BILLED>=:StartTime and  Archive.TIME_STAMP_BILLED<:Endtime) Group By ARCHIVE.ARCBILL_KEY,ARCHIVE.ARCHIVE_KEY , ARCHIVE.BASE_PRICE, "
                    "ARCHIVE.MENU_NAME,Archive.Qty, ARCHIVE.TIME_STAMP_BILLED ) Archive on Ab.ARCHIVE_KEY = Archive.ARCHIVE_KEY WHERE Archive.TIME_STAMP_BILLED>=:StartTime and  "
                    "Archive.TIME_STAMP_BILLED<:Endtime Group By ARCHIVE.ARCBILL_KEY, ARCHIVE.MENU_NAME,Ab.ORDER_LOCATION Having Count(Archive.Archive_Key) > 0 "
                    "Into ARCBILL_KEY,ORDER_LOCATION,MENU_NAME,DISCOUNT, SURCHARGE ,SubTotal         "
		"DO begin MENU_NAME=REPLACE (MENU_NAME, ', ,,', ',');  "
		"MENU_NAME=REPLACE (MENU_NAME, ',,', ',');     "
		"MENU_NAME=REPLACE (MENU_NAME, ',)', ')');    "
		"MENU_NAME=REPLACE (MENU_NAME, ', ,', ',');   "
		"MENU_NAME=REPLACE (MENU_NAME, ',)', ')');  "
		"MENU_NAME=REPLACE (MENU_NAME, '.', '');     "
		"MENU_NAME=REPLACE (MENU_NAME, '%', '');     "
		"MENU_NAME=REPLACE ( MENU_NAME, '.', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '<', '');     "
		"MENU_NAME=REPLACE ( MENU_NAME, '>', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '?', '');    "
		"MENU_NAME=REPLACE ( MENU_NAME, '^', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '&', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '-', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '--', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '!', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '*', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '#', ''); "
		"MENU_NAME=REPLACE ( MENU_NAME, '@', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '(', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ')', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '+', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '{', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '[', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '', '');   "
		"MENU_NAME=REPLACE ( MENU_NAME, '/', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, '!', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ':', '');  "
		"MENU_NAME=REPLACE ( MENU_NAME, ';', '');  "
        "MENU_NAME=REPLACE (MENU_NAME, '''','');  "
		"updatemenustmt='';  "
		"if (MENU_NAME <> '')  "
		"then "
		"updatemenustmt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' ))|| ' = ''' || SubTotal|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"if (MENU_NAME <> '') "
		"then  "
		"execute statement updatemenustmt; "
		"MenuDiscountColData='_Discount_Amount'; "
		"if (MENU_NAME <> '') then  "
		"UpdateMenuDiscountstmt = 'UPDATE '||ATABLENAME||' set ' ||upper(REPLACE ( REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' )) || ''|| MenuDiscountColData || ' = ''' || DISCOUNT|| ''' WHERE SRNO = ''' || ARCBILL_KEY||'''';  "
		"if (MENU_NAME <> '') then execute statement UpdateMenuDiscountstmt;  "
		"stmnt = '';  "
        " stmnt='UPDATE '||ATABLENAME||' set Location = ''' || ORDER_LOCATION|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; execute statement stmnt;   "
         	"stmnt = '';  "
		"MenuDiscountColData='_Surcharge_Amount'; "
		"if (MENU_NAME <> '') then stmnt = 'UPDATE '||ATABLENAME||' set ' || upper(REPLACE (REPLACE (REPLACE (MENU_NAME, ' ', ''), '.', '' ),'-','' ))|| ''|| MenuDiscountColData || ' = ''' || SURCHARGE|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"if (MENU_NAME <> '')  "
		"then execute statement stmnt; "
		"end       "
		"FOR  "
		"select  TM.ARCBILL_KEY, SUM(TM.SubTotal + COALESCE( TM.TAX_VALUE,0)) Total "
                "FROM ( SELECT AB.ARCBILL_KEY,AB.TIME_STAMP, TA.SubTotal, sum(a.TAX_VALUE) TAX_VALUE FROM ARCHIVE   AC "
                "left join ARCORDERTAXES a  on AC.ARCHIVE_KEY = a.ARCHIVE_KEY INNER JOIN ARCBILL AB ON AB.ARCBILL_KEY = AC.ARCBILL_KEY  "
                "inner join ( select ARCHIVE.ARCBILL_KEY, Cast(Sum(abs(Archive.Qty) * (COALESCE(Archive.BASE_PRICE,0))+ "
                "COALESCE (Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(15,4)) SubTotal  "
                "From  Archive Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  Left Join CategoryGroups on "
                "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  Where  (Archive.Order_Type = 3 or Archive.Order_Type = 0) "
                "AND Archive.TIME_STAMP_BILLED>= :StartTime and   Archive.TIME_STAMP_BILLED< :Endtime "
                "Group By ARCHIVE.ARCBILL_KEY Having Count (Archive.Archive_Key) > 0 ) TA on TA.ARCBILL_KEY = AB.ARCBILL_KEY  Where a.TAX_NAME<>''  "
                " AND AB.TIME_STAMP>=:StartTime and   AB.TIME_STAMP<:Endtime GROUP BY 1,2,3) TM where   TM.TIME_STAMP>=:StartTime and   TM.TIME_STAMP<:Endtime  group BY 1  "
        " INTO ARCBILL_KEY,  TOTALAMOUNT DO begin stmnt = '';        "
		" stmnt='UPDATE '||ATABLENAME||' set TOTAL_AMOUNT = ''' || TOTALAMOUNT|| ''' WHERE SRNO = ''' || ARCBILL_KEY||''''; "
		"execute statement stmnt; "
		"end "
		"SUSPEND;   "
		"END "  ,	inDBControl );
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}}
