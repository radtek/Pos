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
    Create6_33GeneratorMallExportMallId(_dbControl);
    Create6_33MallExportSettings(_dbControl);
    Create6_33MallExportSettingsMapping(_dbControl);
    Create6_33MallExportSettingsMappingValues(_dbControl);
    Create6_33MallExportHeader(_dbControl);
    Create6_33MallExportSales(_dbControl);
    Create6_33GeneratorMallExportSaleKey(_dbControl);
    //Create6_33GeneratorMallExportsSettingKey(_dbControl);
    Create6_33GeneratorMallExportsSettingMappingKey(_dbControl);
    Create6_33GeneratorMallExportsSettingValueAttributes(_dbControl);
    Create6_33MallExportSettingValuesAttributes(_dbControl);
    Insert6_33Malls(_dbControl);
    Insert6_33MallExport_Settings(_dbControl);
    Insert6_33MallExport_Settings_Mapping(_dbControl);
    Insert6_33MallExport_Settings_Values(_dbControl);
    Insert6_33Mall_ExportHeader(_dbControl);
}

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33Malls(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLS", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLS "
                "( "
                "   MALL_ID INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_NAME VARCHAR(50), "
                "   IS_ACTIVE char(1) default 'F' "
                ");",
			inDBControl );
     }
 }

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettings(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SETTINGS", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SETTINGS "
                "( "
                "   MALLEXPORT_SETTING_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   NAME VARCHAR(50), "
                "   CONTROL_NAME VARCHAR(100), "
                "   IS_UI_REQUIRED char(1) default 'F' "
                ");",
			inDBControl );
     }
}

//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingsMapping(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SETTINGS_MAPPING", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SETTINGS_MAPPING"
                "( "
                "   MALLEXPORT_SETTING_MAP_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALLEXPORT_SETTING_ID INTEGER, "
                "   MALL_ID INTEGER, "
                " FOREIGN KEY (MALL_ID) REFERENCES MALLS (MALL_ID)  ON DELETE CASCADE "
                ");",
			inDBControl );

        }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingsMappingValues(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SETTINGS_VALUES", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SETTINGS_VALUES"
                "( "
                "   MALLEXPORT_SETTING_VALUE_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALLEXPORTSETTING_ID INTEGER NOT NULL , "
                "   FIELD_VALUE VARCHAR(50), "
                "   FIELD_TYPE VARCHAR(50) "
                ");",
			inDBControl );
     }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSettingValuesAttributes(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SETTING_VALUES_ATTRIBUTES", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SETTING_VALUES_ATTR"
                "( "
                "   MALLEXPORT_ATTRIBUTE_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALLEXPORT_VALUES_KEY INTEGER NOT NULL , "
                "   ATTRIBUTE_NAME VARCHAR(50), "
                "   ATTRIBUTE_VALUE VARCHAR(50), "
                "   FOREIGN KEY (MALLEXPORT_VALUES_KEY) REFERENCES MALLEXPORT_SETTINGS_VALUES (MALLEXPORT_SETTING_VALUE_KEY)  ON DELETE CASCADE "
                ");",
			inDBControl );
     }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportHeader(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_HEADER", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_HEADER "
                "( "
                "   MALLEXPORT_HEADER_ID INTEGER NOT NULL PRIMARY KEY, "
                "   MM_NAME VARCHAR(50), "
                "   IS_ACTIVE char(1) default 'F' "
                ");",
			inDBControl );
     }
}
//-------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSales(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SALES", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SALES"
                "( "
                "   MALLEXPORT_SALE_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_KEY INTEGER , "
                "   FIELD_INDEX INTEGER, "
                "   FIELD VARCHAR(50), "
                "   FIELD_VALUE VARCHAR(50), "
                "   VALUE_TYPE VARCHAR(50), "
                "   DATE_CREATED  Timestamp, "
                "   CREATED_BY VARCHAR(50), "
                "   Z_KEY INTEGER, "
                "   ARCBILL_KEY INTEGER, "
                "   FOREIGN KEY (MALL_KEY) REFERENCES MALLS (MALL_ID) ON DELETE CASCADE, "
                "   FOREIGN KEY (FIELD_INDEX) REFERENCES MALLEXPORT_HEADER (MALLEXPORT_HEADER_ID) ON DELETE CASCADE "
                ");",
			inDBControl );
     }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportMallId(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_MALL_ID", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_MALL_ID;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_MALL_ID TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportsSettingKey(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_SETTING_KEY", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_SETTING_KEY;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_SETTING_KEY TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportsSettingMappingKey(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_SETTING_MAP_KEY", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_SETTING_MAP_KEY;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_SETTING_MAP_KEY TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportsSettingValues(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_SETTING_VAL_KEY", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_SETTING_VAL_KEY;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_SETTING_VAL_KEY TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportsSettingValueAttributes(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_ATTRIBUTE_KEY", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_ATTRIBUTE_KEY;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_ATTRIBUTE_KEY TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33GeneratorMallExportSaleKey(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALLEXPORT_SALE_KEY", _dbControl) )
        {
            executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_SALE_KEY;",
            inDBControl);
            executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_SALE_KEY TO 0; ",
            inDBControl );
        }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_33Malls(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLS VALUES (1,'Estancia','F') ";
        InsertQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_33MallExport_Settings(TDBControl* const inDBControl)
{

    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        const int numberOfFields = 22;
        UnicodeString fieldNames[numberOfFields] =
        {
            "TENANT_NUMBER" ,"FILE_LOCATION" ,"CLASS_CODE" ,"TRADE_CODE" ,"OUTLET_NUMBER" ,"BRANCH_CODE" ,"TERMINAL_NUMBER" ,
            "SERIAL_NUMBER" ,"ASSIGN_SALES_TYPE" ,"FTP_SERVER" ,"FTP_PATH" ,"FTP_USER_NAME" ,"FTP_PASSWORD" ,
            "ENABLE_CONSOLIDATED_REPORT" ,"CONSOLIDATED_DB_PATHS" ,"TYPE_OF_FILE" ,"HEADER_WIDTH" ,"FILE_CREATION_PERIOD" ,
            "REQUIRED_ON_FTP_SERVER " ,"INCLUDE_IN_EXISTING" ,"FILE_NAMING_CONVENTION" ,"FILE_HEADER"
        };
        UnicodeString controlNames[numberOfFields] =
        {
            "edTenantNo1" ,"edMallPath1" ,"edClassCode1" ,"edTradeCode1" ,"edOutletCode1" ,"edBranchCode1" ,"edTerminalNo1" ,
            "edSerialNo1" ,"btnAssignSalesType1" ,"edFTPServer1" ,"edFTPPath1" ,"edFTPUserName1" ,"edFTPPassword1" ,
            "cbEnableConsolidatedRep1" ,"edConsolidatedDBPaths1" ,"File Type" ,"Header Width" ,"File Creation" ,
            "Load TO FTP Server ","Append File" , "File Name" ,"File Header"
        };
        UnicodeString isUIRequired[numberOfFields] =
        {
            "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "F", "F", "F", "F", "F", "F", "F"
        };

        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectGenQuery    = transaction.Query( transaction.AddQuery() );

        for(int index = 0; index < numberOfFields; index++)
        {
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_SETTINGS VALUES (:SETTING_KEY,:FIELD_NAME,:CONTROL_NAME,IS_UI) ";
            InsertQuery->ParamByName("SETTING_KEY")->AsInteger = index+1;
            InsertQuery->ParamByName("FIELD_NAME")->AsString = fieldNames[index+1];
            InsertQuery->ParamByName("CONTROL_NAME")->AsString = controlNames[index+1];
            InsertQuery->ParamByName("IS_UI")->AsString = isUIRequired[index+1];
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_33MallExport_Settings_Mapping(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (1,1,1) ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES(2,2,1) ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (3,7,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (4,16,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (5,17,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (6,18,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (7,19,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (8,20,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (9,21,1)";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_33MallExport_Settings_Values(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (1,1,'','UnicodeString') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES(2,2,'','UnicodeString') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (3,7,'','int')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (4,16,'Text','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (5,17,'5','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (6,17,'2','int')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (7,17,'8','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (8,17,'12','Currency')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (9,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (10,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (11,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                   "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (12,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                   "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (13,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (14,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                   "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (15,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (16,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (17,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                   "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (18,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                   "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (19,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (20,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
       InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (21,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (22,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (23,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (24,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (25,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (26,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (27,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (28,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (29,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (30,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (31,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (32,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (33,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (34,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (35,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (36,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (37,17,'4','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (38,17,'4','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (39,17,'4','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (40,17,'4','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (41,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (42,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (43,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (44,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (45,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (46,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (47,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (48,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (49,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (50,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (51,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (52,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (53,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (54,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (55,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (56,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (57,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (58,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (59,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (60,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (61,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (62,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (63,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (64,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (65,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (66,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (67,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (68,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (69,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (70,17,'12','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();

        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (71,18,'Z','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (72,19,'F','bool')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (73,20,'F','bool')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (74,21,'SNNNNTTMMDDYYYY.B','UnicodeString')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_33Mall_ExportHeader(TDBControl* const inDBControl)
{
    const int NUMBER_OF_FIELDS = 68;
    UnicodeString fieldNames[NUMBER_OF_FIELDS] =
        {
            "TenantCode" ,"TerminalNumber" ,"Date" ,"OldAccumulatedSalesVatable" ,"NewAccumulatedSalesVatable" ,"TotalGrossAmountVatable" ,"TotalDeductionsVatable" ,
            "TotalPromoSalesAmountVatable" ,"TotalPWDDiscountVatable" ,"TotalRefundAmountVatable" ,"TotalReturnedItemsAmountVatable" ,"TotalOtherTaxesVatable" ,"TotalServiceChargeVatable" ,
            "TotalAdjustmentDiscount" ,"TotalVoidAmount" ,"TotalDiscountCards" ,"TotalDeliveryCharges" ,"TotalGiftCertificates/Checks Redeemed" ,
            "DiscountGroup1Vatable " ,"DiscountGroup2Vatable" ,"DiscountGroup3Vatable" ,"DiscountGroup4Vatable" ,"DiscountGroup5Vatable" ,
            "TotalofallNonApprovedStoreDiscountsVatable" ,"Discount1NonApprovedVatable" ,"Discount2NonApprovedVatable" ,"Discount3NonApprovedVatable" ,"Discount4NonApprovedVatable" ,
            "Discount5NonApprovedVatable" ,"TotalVAT/TaxAmountVatable" ,"TotalNetSalesAmountVatable" ,"TotalCoverCount" ,"Z-Number" ,"TransactionCount" ,"SalesType" ,
            "Amount" ,"OldAccumulatedSalesNonVatable" ,"NewAccumulatedSalesNonVatable" ,"TotalGrossAmountNonVatable" , "TotalDeductionsNonVatable" ,"TotalPromoSalesAmountNonVatable" ,"SeniorCitizenDiscountNonVatable" ,
            "TotalRefundAmountNonVatable" , "TotalReturnedItemsAmountNonVatable" ,"TotalOtherTaxesNonVatable" ,"TotalServiceChargeAmountNonVatable" ,"TotalAdjustmentDiscountNonVatable" ,"TotalVoidAmountNonVatable" ,
            "TotalDiscountCardsNonVatable" ,"TotalDeliveryChargesNonVatable" , "TotalGiftCertificates/ChecksRedeemeNonVatable" ,"DiscountGroup1NonVatable" ,"DiscountGroup2NonVatable" ,
            "DiscountGroup3NonVatable " , "DiscountGroup4NonVatable" , "Discount5NonVatable" ,"TotalofallNonApprovedStoreDiscountsNonVatable" , "Discount1NonApprovedNonVatable" ,
            "Discount2NonApprovedNonVatable" , "Discount3NonApprovedNonVatable" , "Discount4NonApprovedNonVatable" , "Discount5NonApprovedNonVatable" ,"VAT/TaxAmountNonVatable" ,
            "TotalNetSalesAmountNonVatable" ,"GrandTotalNetSales", "HourCode", "Status", "InvoiceNumber"
        };
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        for (int i = 0; i < NUMBER_OF_FIELDS; i++)
        {
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_HEADER VALUES (:HEADER_ID, :MM_NAME, :IS_ACTIVE) ";
            InsertQuery->ParamByName("HEADER_ID")->AsInteger = (i + 1);
            InsertQuery->ParamByName("MM_NAME" )->AsString  = fieldNames[i];
            InsertQuery->ParamByName("IS_ACTIVE" )->AsString  = "T";
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------------------------------
}

