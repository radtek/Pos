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
    Create6_33MallExportSettingsMapping(_dbControl);
    Create6_33MallExportSettingsMappingValues(_dbControl);
    Create6_33MallExportSales(_dbControl);
   // Create6_33MallExportSettingValuesAttributes(_dbControl);
    Insert6_33Malls(_dbControl);
    Insert6_33MallExport_Settings(_dbControl);
    Insert6_33MallExport_Settings_Mapping(_dbControl);
    Insert6_33MallExport_Settings_Values(_dbControl);
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
                "   ID INTEGER NOT NULL PRIMARY KEY, "
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
                "   ID INTEGER NOT NULL PRIMARY KEY, "
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
                "   MALLEXPORTSETTING_KEY INTEGER NOT NULL PRIMARY KEY, "
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
                "CREATE TABLE MALLEXPORT_SETTING_VALUES_ATTRIBUTES"
                "( "
                "   MALLEXPORT_ATTRIBUTE_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   MALLEXPORT_VALUES_KEY INTEGER NOT NULL , "
                "   ATTRIBUTE_NAME VARCHAR(50), "
                "   ATTRIBUTE_VALUE VARCHAR(50), "
                "   FOREIGN KEY (MALLEXPORT_VALUES_KEY) REFERENCES MALLEXPORT_SETTINGS_VALUES (MALLEXPORTSETTING_KEY)  ON DELETE CASCADE "
                ");",
			inDBControl );
     }
}
//----------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_33MallExportSales(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SALES", inDBControl ) )
     {
		executeQuery(
                "CREATE TABLE MALLEXPORT_SALES"
                "( "
                "   ID INTEGER NOT NULL PRIMARY KEY, "
                "   MALL_KEY INTEGER , "
                "   FIELD VARCHAR(50), "
                "   VALUE_GROUP VARCHAR(50), "
                "   FIELD_VALUE VARCHAR(50), "
                "   VALUE_TYPE VARCHAR(50), "
                "   ENUM_TYPE INTEGER , "
                "   DATE_CREATED  Timestamp, "
                "   CREATED_BY VARCHAR(50), "
                "   Z_KEY INTEGER, "
                "   ARCBILL_KEY INTEGER, "
                "   FOREIGN KEY (MALL_KEY) REFERENCES MALLS (MALL_ID) ON DELETE CASCADE "
                ");",
			inDBControl );
     }
}
//-------------------------------------------------------------------------------------------------------------------------
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
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES (1,'TENANT_NUMBER','edTenantNo1','T') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(2,'FILE_LOCATION','edMallPath1','T') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES (3,'CLASS_CODE','edClassCode1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(4,'TRADE_CODE','edTradeCode1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(5,'OUTLET_NUMBER','edOutletCode1','T') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(6,'BRANCH_CODE','edBranchCode1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(7,'TERMINAL_NUMBER','edTerminalNo1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(8,'SERIAL_NUMBER','edSerialNo1','T') ";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(9,'ASSIGN_SALES_TYPE','btnAssignSalesType1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(10,'FTP_SERVER','edFTPServer1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(11,'FTP_PATH','edFTPPath1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(12,'FTP_USER_NAME','edFTPUserName1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(13,'FTP_PASSWORD','edFTPPassword1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(14,'Enable_Consolidated_Report','cbEnableConsolidatedRep1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(15,'Consolidated_DB_Paths','edConsolidatedDBPaths1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(16,'Type_Of_File','File Type','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(17,'Header_Width','Header Width','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(18,'File_Creation_Period','File Creation','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(19,'Required_On_FTP_Server','Load TO FTP Server','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(20,'Include_In_Existing','Append File','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(21,'File_Naming_Convention','File Name','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text =
                    "INSERT INTO MALLEXPORT_SETTINGS(ID,NAME,CONTROL_NAME,IS_UI_REQUIRED) VALUES(22,'File_Header','File Header','F')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
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
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (3,7,'','Integer')";
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
                    "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (6,17,'2','Integer')";
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
}

