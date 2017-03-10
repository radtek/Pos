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
 //6.35
void TApplyParser::upgrade6_35Tables()
{
	update6_35Tables();
}
//6.36
void TApplyParser::upgrade6_36Tables()
{
	update6_36Tables();
}
//6.37
void TApplyParser::upgrade6_37Tables()
{
	update6_37Tables();
}

void TApplyParser::upgrade6_38Tables()
{
	update6_38Tables();
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
//------------------------------------------------------------------------------------
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
    Create6_34TableSCDPWDCustomerDetails(_dbControl);
    Create6_34GeneratorSCDPWDCustomerDetails(_dbControl);
    UpdateZedTable6_34(_dbControl);
    CreateGeneratorAndTableForCashDenominations6_34( _dbControl);
    Create6_34Malls(_dbControl);
    Create6_34MallExportSettings(_dbControl);
    Create6_34MallExportSettingsMapping(_dbControl);
    Create6_34MallExportSettingsMappingValues(_dbControl);
    Create6_34MallExportHeader(_dbControl);
    Create6_34MallExportSales(_dbControl);
    Create6_34GeneratorMallExportSaleKey(_dbControl);
    Create6_34MallExportSettingValuesAttributes(_dbControl);
    Insert6_34Malls(_dbControl);
    Insert6_34MallExport_Settings(_dbControl);
    Insert6_34MallExport_Settings_Mapping(_dbControl);
    Insert6_34MallExport_Settings_Values(_dbControl);
    Insert6_34Mall_ExportHeader(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::Create6_34TableSCDPWDCustomerDetails(TDBControl* const inDBControl)
{
    if ( !tableExists( "SCD_PWD_CUSTOMER_DETAILS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE SCD_PWD_CUSTOMER_DETAILS "
		"( "
		"   CUSTOMER_DETAILS_KEY INT NOT NULL PRIMARY KEY,"
        "   ARCBILL_KEY INT NOT NULL, "
		"   FIELD_HEADER varchar(50),"
        "   FIELD_VALUE VARCHAR(200),"
        "   DATA_TYPE Numeric(15,4)"
		");",
		inDBControl );
    }
}
//---------------------------------------------------------------------------------
void TApplyParser::Create6_34GeneratorSCDPWDCustomerDetails(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_CUSTOMER_DETAILS_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_CUSTOMER_DETAILS_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_CUSTOMER_DETAILS_KEY TO 0;", inDBControl
        );
    }
}
//---------------------------------------------------------------------------------------------------
void TApplyParser::UpdateZedTable6_34( TDBControl* const inDBControl )  // update zed table to add Trans_date column
{
    if ( !fieldExists( "ZEDS", "TRANS_DATE", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ZEDS "
        "ADD TRANS_DATE TIMESTAMP ",
        inDBControl);
    }
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::CreateGeneratorAndTableForCashDenominations6_34( TDBControl* const inDBControl )
{
    if( !generatorExists("GEN_CASHDENOMINATION", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_CASHDENOMINATION;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_CASHDENOMINATION TO 0; ",
		inDBControl );
	}

    if( !tableExists("CASHDENOMINATIONS", _dbControl ) )
    {
       executeQuery(
            "CREATE TABLE CASHDENOMINATIONS "
            "( "
            " CASHDENOMINATION_KEY Integer NOT NULL PRIMARY KEY , "
            " TITLE  Varchar(50), "
            " DENOMINATION NUMERIC(17,4) "
            "); ",
        inDBControl );
    }

    if( !tableExists("ZED_CASHDENOMINATIONS", _dbControl ) )
    {
       executeQuery(
            "CREATE TABLE ZED_CASHDENOMINATIONS "
            "( "
            " Z_KEY Integer, "
            " TERMINAL_NAME Varchar(50), "
            " DENOMINATION_TITLE  Varchar(50), "
            " DENOMINATION_VALUE NUMERIC(17,4), "
            " DENOMINATION_QUANTITY Integer "
            "); ",
        inDBControl );
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void TApplyParser::Create6_34Malls(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34MallExportSettings(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34MallExportSettingsMapping(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34MallExportSettingsMappingValues(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34MallExportSettingValuesAttributes(TDBControl* const inDBControl)
{
     if ( !tableExists( "MALLEXPORT_SETTING_VALUES_ATTR", inDBControl ) )
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
void TApplyParser::Create6_34MallExportHeader(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34MallExportSales(TDBControl* const inDBControl)
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
void TApplyParser::Create6_34GeneratorMallExportSaleKey(TDBControl* const inDBControl)
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
void TApplyParser::Insert6_34Malls(TDBControl* const inDBControl)
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
void TApplyParser::Insert6_34MallExport_Settings(TDBControl* const inDBControl)
{

    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        const int numberOfFields = 26;
        UnicodeString fieldNames[numberOfFields] =
        {
            "TENANT_NUMBER" ,"FILE_LOCATION" ,"CLASS_CODE" ,"TRADE_CODE" ,"OUTLET_NUMBER" ,"BRANCH_CODE" ,"TERMINAL_NUMBER" ,
            "SERIAL_NUMBER" ,"ASSIGN_SALES_TYPE" ,"FTP_SERVER" ,"FTP_PATH" ,"FTP_USER_NAME" ,"FTP_PASSWORD" ,
            "ENABLE_CONSOLIDATED_REPORT" ,"CONSOLIDATED_DB_PATHS" ,"TYPE_OF_FILE" ,"HEADER_WIDTH" ,"FILE_CREATION_PERIOD" ,
            "REQUIRED_ON_FTP_SERVER " ,"INCLUDE_IN_EXISTING" ,"FILE_NAMING_CONVENTION" ,"FILE_HEADER", "RESEND_REPORT", "REGENERATE_REPORT", "NEW_LINE",
            "Tax_Rate"
        };
        UnicodeString controlNames[numberOfFields] =
        {
            "edMallTenantNo" ,"edNewMallPath" ,"edMallClassCode" ,"edMallTradeCode" ,"edMallOutletCode" ,"edMallBranchCode" ,"edMallTerminalNo" ,
            "edMallSerialNo" ,"btnAssignMallSalesType" ,"edMallFTPServer" ,"edMallFTPPath" ,"edMallFTPUserName" ,"edMallFTPPassword" ,
            "cbEnableMallConsolidatedRep" ,"edMallConsolidatedDBPaths" ,"File Type" ,"Header Width" ,"File Creation" ,
            "Load TO FTP Server ","Append File" , "File Name" ,"File Header", "btnResendMallReport", "btnRegenMallReport", "New Line", "edTaxRate"
        };
        UnicodeString isUIRequired[numberOfFields] =
        {
            "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "T", "F", "F", "F", "F", "F", "F", "F", "T", "T", "F", "T"
        };

        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        for(int index = 0; index < numberOfFields; index++)
        {
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_SETTINGS VALUES (:SETTING_KEY, :FIELD_NAME, :CONTROL_NAME, :IS_UI) ";
            InsertQuery->ParamByName("SETTING_KEY")->AsInteger = index + 1;
            InsertQuery->ParamByName("FIELD_NAME")->AsString = fieldNames[index];
            InsertQuery->ParamByName("CONTROL_NAME")->AsString = controlNames[index];
            InsertQuery->ParamByName("IS_UI")->AsString = isUIRequired[index];
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
void TApplyParser::Insert6_34MallExport_Settings_Mapping(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
         const int numberOfFields = 12;
        int settingID[numberOfFields] =
        {
            1, 2, 7, 16, 17, 18, 19, 20, 21, 24, 25, 26
        };

        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        for(int index = 0; index < numberOfFields; index++)
        {
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_SETTINGS_MAPPING VALUES (:MAPPING_KEY, :SETTING_KEY, :MALL_KEY) ";
            InsertQuery->ParamByName("MAPPING_KEY")->AsInteger = index+1;
            InsertQuery->ParamByName("SETTING_KEY")->AsString = settingID[index];
            InsertQuery->ParamByName("MALL_KEY")->AsString = 1;
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_34MallExport_Settings_Values(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
         const int numberOfFields = 77;
         UnicodeString fieldTypes[numberOfFields] =
         {
            "UnicodeString", "UnicodeString", "int", "UnicodeString", "UnicodeString", "int", "UnicodeString", "Currency", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString",
            "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "UnicodeString", "bool", "bool",  "UnicodeString",
            "bool", "bool", "UnicodeString"
         };

         UnicodeString fieldValues[numberOfFields] =
         {
            "", "", "", ".txt", "5", "2", "8", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12",
            "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12",
            "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "12", "Z", "false", "false",
            "SNNNNTTMMDDYYYY.B", "true", "true", ""
         };

         int settingID[numberOfFields] =
         {
            1, 2, 7, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
            17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18,
            19, 20, 21, 24, 25, 26
         };

        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );

        for(int index = 0; index < numberOfFields; index++)
        {
            InsertQuery->Close();
            InsertQuery->SQL->Text =
                        "INSERT INTO MALLEXPORT_SETTINGS_VALUES VALUES (:SETTING_VALUE_KEY, :SETTING_KEY, :FIELD_VALUE, :FIELD_TYPE) ";
            InsertQuery->ParamByName("SETTING_VALUE_KEY")->AsInteger = index+1;
            InsertQuery->ParamByName("SETTING_KEY")->AsInteger = settingID[index];
            InsertQuery->ParamByName("FIELD_VALUE")->AsString = fieldValues[index];
            InsertQuery->ParamByName("FIELD_TYPE")->AsString = fieldTypes[index];
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_34Mall_ExportHeader(TDBControl* const inDBControl)
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

//::::::::::::::::::::::::Version 6.35::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_35Tables()
{
    Create6_35MemberSubsGenerator(_dbControl);
    if(!tableExists( "MEMBERSHIP_SUBS_DETAILS", _dbControl ))
    {
        Create6_35MemberSubsDetails(_dbControl);
        Insert6_35MemberSubsDetails(_dbControl);
    }
    UpdateMallExportSettingValuesTable6_35(_dbControl);
    Create6_35GeneratorMallExportSettingValues(_dbControl);
    UpdateContactIndex_6_35(_dbControl);
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_35MemberSubsGenerator(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MEMBERSHIP_SUBS_KEY", _dbControl) )
    {
        executeQuery(
        "CREATE GENERATOR GEN_MEMBERSHIP_SUBS_KEY;",
        inDBControl);
        executeQuery(
        "SET GENERATOR GEN_MEMBERSHIP_SUBS_KEY TO 0; ",
        inDBControl );
    }
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_35MemberSubsDetails(TDBControl* const inDBControl)
{
    executeQuery(
    "CREATE TABLE MEMBERSHIP_SUBS_DETAILS "
    "( "
    "   MEMBERSHIP_SUBS_KEY INTEGER NOT NULL PRIMARY KEY, "
    "   SUBS_PAID_DATE TIMESTAMP, "
    "   SUBS_EXPIRY_DATE TIMESTAMP,"
    "   SUBS_PAID_AMOUNT NUMERIC(15,4),"
    "   SUBS_PAID_RECEIPT_NO VARCHAR(25),"
    "   SUBS_TYPE VARCHAR(50),"
    "   SUBS_PAID T_TRUEFALSE DEFAULT 'F',"
    "   POINTS_RULES_SUBS INTEGER,"
    "   CONTACTS_KEY INTEGER,"
    "   FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS (CONTACTS_KEY)  ON DELETE CASCADE, "
    "   ISLOCAL_MEMBER T_TRUEFALSE DEFAULT 'F'"
    ");",
    inDBControl );
}
//---------------------------------------------------------------------------
void TApplyParser::Insert6_35MemberSubsDetails(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectGenQuery = transaction.Query( transaction.AddQuery() );
        SelectGenQuery->Close();
        InsertQuery->Close();
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        SelectQuery->Close();
        SelectQuery->SQL->Text = " Select CONTACTS_KEY,POINTS_RULES FROM CONTACTS WHERE CONTACT_TYPE <> 0"
        "AND CONTACT_TYPE <> 1 AND CONTACT_TYPE <> 3 ORDER BY CONTACTS_KEY ";
        SelectQuery->ExecQuery();
        InsertQuery->SQL->Text =
        "INSERT INTO MEMBERSHIP_SUBS_DETAILS ( MEMBERSHIP_SUBS_KEY, CONTACTS_KEY, SUBS_PAID_DATE,SUBS_PAID_AMOUNT,"
        " SUBS_PAID_RECEIPT_NO,SUBS_TYPE, POINTS_RULES_SUBS,SUBS_PAID,ISLOCAL_MEMBER)"
        " VALUES (:MEMBERSHIP_SUBS_KEY, :CONTACTS_KEY, :SUBS_PAID_DATE, :SUBS_PAID_AMOUNT, :SUBS_PAID_RECEIPT_NO,"
        ":SUBS_TYPE,:POINTS_RULES_SUBS,:SUBS_PAID,:ISLOCAL_MEMBER) ";

        for (; !SelectQuery->Eof;)
        {
            InsertQuery->Close();
            int pointsRules = 0;
            pointsRules = pointsRules | FinancialPaid;
            pointsRules = pointsRules | DiscountsAllowed;
            pointsRules |= SelectQuery->Fields[1]->AsInteger;
            SelectGenQuery->Close();
            SelectGenQuery->SQL->Text = "SELECT GEN_ID(GEN_MEMBERSHIP_SUBS_KEY, 1) FROM RDB$DATABASE";
            SelectGenQuery->ExecQuery();
            int generator_Val = 0;
            generator_Val = SelectGenQuery->Fields[0]->AsInteger;
            InsertQuery->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = generator_Val;
            InsertQuery->ParamByName("CONTACTS_KEY" )->AsInteger  = SelectQuery->Fields[0]->AsInteger;
            InsertQuery->ParamByName("SUBS_PAID_DATE" )->AsDateTime  = Now();
            InsertQuery->ParamByName("SUBS_PAID_AMOUNT" )->AsDouble  = 0.0;
            InsertQuery->ParamByName("SUBS_PAID_RECEIPT_NO" )->AsString  = "-";
            InsertQuery->ParamByName("SUBS_TYPE" )->AsString  = "AUTO";         // Old members are registered under this type
            InsertQuery->ParamByName("POINTS_RULES_SUBS" )->AsInteger = pointsRules;
            InsertQuery->ParamByName("SUBS_PAID" )->AsString = "T";
            InsertQuery->ParamByName("ISLOCAL_MEMBER" )->AsString = "T";
            InsertQuery->ExecQuery();
            SelectQuery->Next();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateContactIndex_6_35(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
        InsertQuery->Close();
        InsertQuery->SQL->Text = "DROP INDEX CONTACT_INDEX;";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "ALTER TABLE CONTACTS ADD EMAILDUMMY Varchar(80);";
        InsertQuery->ExecQuery();
        transaction.Commit();
        transaction.StartTransaction();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "UPDATE CONTACTS SET EMAILDUMMY = EMAIL;";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "ALTER TABLE CONTACTS DROP EMAIL;";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "ALTER TABLE CONTACTS ALTER EMAILDUMMY TO EMAIL;";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "CREATE UNIQUE INDEX CONTACT_INDEX ON CONTACTS (NAME,EMAIL,CONTACTS_3RDPARTY_KEY, MEMBER_NUMBER, SITE_ID );";
        InsertQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateMallExportSettingValuesTable6_35(TDBControl* const inDBControl)
{
    if (tableExists( "MALLEXPORT_SETTINGS_VALUES", inDBControl ) )
    {
        executeQuery(
                "DELETE FROM MALLEXPORT_SETTINGS_VALUES a; ",
            inDBControl );
    }

    if (!fieldExists( "MALLEXPORT_SETTINGS_VALUES", "DEVICE_KEY", _dbControl ) )
    {
        executeQuery (
            "ALTER TABLE MALLEXPORT_SETTINGS_VALUES ADD "
            "DEVICE_KEY INTEGER; ",
            inDBControl);
    }

    if (!fieldExists( "MALLEXPORT_SETTINGS_VALUES", "MALL_KEY", _dbControl ) )
    {
        executeQuery (
            "ALTER TABLE MALLEXPORT_SETTINGS_VALUES ADD "
            "MALL_KEY INTEGER; ",
            inDBControl);
    }
    if (!fieldExists( "MALLEXPORT_SALES", "DEVICE_KEY", _dbControl ) )
    {
        executeQuery (
            "ALTER TABLE MALLEXPORT_SALES ADD "
            "DEVICE_KEY INTEGER; ",
            inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::Create6_35GeneratorMallExportSettingValues(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_MALL_SETT_VAL_KEY", _dbControl) )
    {
        executeQuery(
        "CREATE GENERATOR GEN_MALL_SETT_VAL_KEY;",
        inDBControl);
        executeQuery(
        "SET GENERATOR GEN_MALL_SETT_VAL_KEY TO 0; ",
        inDBControl );
    }
}

//::::::::::::::::::::::::Version 6.36::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_36Tables()
{
    AlterTableRefloat_Skim6_36(_dbControl);
    Update6_36TableSCDPWDCustomerDetails(_dbControl);
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableRefloat_Skim6_36( TDBControl* const inDBControl )
{
    if ( !fieldExists( "REFLOAT_SKIM ", "IS_FLOAT_WITHDRAWN_FROM_CASH", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE REFLOAT_SKIM "
        "ADD IS_FLOAT_WITHDRAWN_FROM_CASH T_TRUEFALSE DEFAULT 'F' ; ",
        inDBControl);
    }
}
//-------------------------------------------------------------------------------------------------
bool TApplyParser::CheckDataTypeOfColumn(TDBControl* const inDBControl)
{
    bool retval = false;
    if( tableExists( "SCD_PWD_CUSTOMER_DETAILS", inDBControl ) )
    {

        TDBTransaction transaction( *_dbControl );
        transaction.StartTransaction();
        TIBSQL *SelectQuery = transaction.Query( transaction.AddQuery() );
        SelectQuery->Close();
        SelectQuery->SQL->Text =
        " SELECT RF.RDB$RELATION_NAME as TABLE_NAME, "
        " RF.RDB$FIELD_NAME as COLUMN_NAME, "
        " F.RDB$FIELD_TYPE "
        " FROM RDB$FIELDS F "
        " JOIN RDB$RELATION_FIELDS RF on RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME "
        " WHERE RF.RDB$RELATION_NAME = 'SCD_PWD_CUSTOMER_DETAILS' and RF.RDB$FIELD_NAME = 'DATA_TYPE' ";
        SelectQuery->ExecQuery();

        if(SelectQuery->FieldByName("RDB$FIELD_TYPE")->AsInteger == 16)
        {
           retval = true;
        }
    }
    return retval;
}
//-------------------------------------------------------------------------------------------------
void TApplyParser::Update6_36TableSCDPWDCustomerDetails(TDBControl* const inDBControl)
{
    if(CheckDataTypeOfColumn(_dbControl))
    {
       executeQuery( "ALTER TABLE SCD_PWD_CUSTOMER_DETAILS ALTER DATA_TYPE TYPE VARCHAR(25) ;", inDBControl);
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::update6_37Tables()
{
    CREATEDSR_PIVOT_BY_ITEMProcedure6_37( _dbControl ) ;
	POPULATEDSR_PIVOT_BY_ITEMProcedure6_37( _dbControl ) ;
    CREATEDSRPIVOTProcedure6_37( _dbControl ) ;
	POPULATEDSRPIVOTProcedure6_37( _dbControl ) ;
    AlterTable6_37(_dbControl);
    UpdateContacts6_37(_dbControl);
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::CREATEDSR_PIVOT_BY_ITEMProcedure6_37( TDBControl* const inDBControl )
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
void TApplyParser::POPULATEDSR_PIVOT_BY_ITEMProcedure6_37( TDBControl* const inDBControl )
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

		"execute  statement 'delete from '||ATableName||';'; COMP='0'; FOR select  a.ARCHIVE_KEY from archive a  where a.TIME_STAMP>=:StartTime and a.TIME_STAMP<:Endtime  order by 1 into RegNum  "
		"DO begin stmnt = 'INSERT INTO '||ATABLENAME||' (SRNO) VALUES('''|| RegNum||''')';  "
		"execute statement stmnt;  "
		"end FOR SELECT b.ARCHIVE_KEY , a.TIME_STAMP, CAST(b.qty*coalesce(b.PRICE,0) AS NUMERIC(17,4)) PRICE, a.INVOICE_NUMBER,b.ITEM_NAME FROM ARCBILL a "
                    "left join ARCHIVE b on a.ARCBILL_KEY=b.ARCBILL_KEY where b.TIME_STAMP>=:StartTime and b.TIME_STAMP<:Endtime  "
		"INTO ARCBILL_KEY , TIME_STAMP ,TOTAL   , INVOICE_NUMBER , ITEMNAME "
		"DO begin  ITEMNAME=REPLACE ( ITEMNAME, '''', ''); "

		"updatestmt ='UPDATE '|| ATABLENAME||' set TIME_STAMP = '''||TIME_STAMP ||''' , BILLAMOUNT = '''||TOTAL||''' , INVOICE_NUMBER = '''|| INVOICE_NUMBER||''' , ITEMNAME = '''|| ITEMNAME||'''  WHERE SRNO = '''||ARCBILL_KEY||''''; "
		"execute statement updatestmt;  "
		"end "
		"FOR "
		" SELECT  b.ARCHIVE_KEY, a.PAY_TYPE, SUM(a.SUBTOTAL)AS SUBTOTAL,c.INVOICE_NUMBER  FROM  ARCHIVE b LEFT join  ARCBILLPAY a ON b .ARCBILL_KEY=A.ARCBILL_KEY left join ARCBILL c on b.ARCBILL_KEY=c.ARCBILL_KEY  "
		"where b .TIME_STAMP>=:StartTime and b .TIME_STAMP<:Endtime  "
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
                        "where  a.TAX_NAME<>'' and AC.TIME_STAMP>=:StartTime and  AC.TIME_STAMP<:Endtime  GROUP BY AC.ARCHIVE_KEY,   a.TAX_NAME, "
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
		"for select AB.ARCHIVE_KEY,  SUM(TAX_VALUE) TAX_VALUE   FROM ( SELECT AC.ARCHIVE_KEY,AC.TIME_STAMP,  sum(a.TAX_VALUE) TAX_VALUE "
            "FROM ARCORDERTAXES a INNER JOIN ARCHIVE AC on AC.ARCHIVE_KEY=a.ARCHIVE_KEY where a.TAX_NAME<>'' AND AC.TIME_STAMP>=:StartTime and  AC.TIME_STAMP<:Endtime  group BY AC.ARCHIVE_KEY,AC.TIME_STAMP) AB "
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
                "(Archive.Order_Type = 3 or Archive.Order_Type = 0) AND (Archive.TIME_STAMP>=:StartTime and  Archive.TIME_STAMP<:Endtime) "
                "  Group By ARCHIVE.ARCBILL_KEY,ARCHIVE.ARCHIVE_KEY , ARCHIVE.BASE_PRICE,ARCHIVE.MENU_NAME,Archive.Qty ) Archive on Ab.ARCHIVE_KEY = "
                "Archive.ARCHIVE_KEY WHERE Ab.TIME_STAMP>=:StartTime and  Ab.TIME_STAMP<:Endtime Group By ARCHIVE.ARCHIVE_KEY, ARCHIVE.MENU_NAME Having Count(Archive.Archive_Key) > 0 "
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
                "FROM ( SELECT AC.ARCHIVE_KEY,AC.TIME_STAMP, TA.SubTotal, sum(a.TAX_VALUE) TAX_VALUE FROM ARCHIVE   AC left join ARCORDERTAXES a  on "
                "AC.ARCHIVE_KEY = a.ARCHIVE_KEY   INNER JOIN ( SELECT ARCHIVE.ARCHIVE_KEY, Cast(Sum(abs(Archive.Qty) * (COALESCE(Archive.BASE_PRICE,0)) "
                    " + COALESCE (Archive.DISCOUNT_WITHOUT_TAX,0)) as Numeric(15,4)) SubTotal  "
                                "From  Archive Left Join ArcCategories on Archive.Category_Key = ArcCategories.Category_Key  "
                    " Left Join CategoryGroups on ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key  "
                " Where  (Archive.Order_Type = 3 or Archive.Order_Type = 0) AND Archive.TIME_STAMP>=:StartTime and   Archive.TIME_STAMP<:Endtime "
                "Group By ARCHIVE.ARCHIVE_KEY Having Count (Archive.Archive_Key) > 0 ) TA on TA.ARCHIVE_KEY = AC.ARCHIVE_KEY  Where a.TAX_NAME<>'' "
                "AND AC.TIME_STAMP>=:StartTime and   AC.TIME_STAMP<:Endtime   GROUP BY 1,2,3) TM "
                " WHERE TM.TIME_STAMP>=:StartTime and   TM.TIME_STAMP<:Endtime  group BY 1  INTO ARCBILL_KEY,  TOTALAMOUNT DO begin stmnt = '';        "
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
void TApplyParser::CREATEDSRPIVOTProcedure6_37( TDBControl* const inDBControl )
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
void TApplyParser::POPULATEDSRPIVOTProcedure6_37( TDBControl* const inDBControl )
{ TDBTransaction transaction( *inDBControl );
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
                    "( Select    ARCHIVE.ARCBILL_KEY,ARCHIVE.TIME_STAMP, ARCHIVE.ARCHIVE_KEY, Archive.Qty, ARCHIVE.BASE_PRICE  , ARCHIVE.MENU_NAME, "
                    "MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX >= 0 THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END)  AS Surcharge, MIN(CASE WHEN Archive.DISCOUNT_WITHOUT_TAX < 0 "
                    "THEN COALESCE(Archive.DISCOUNT_WITHOUT_TAX,0) END) AS Discount From Archive Where (Archive.Order_Type = 3 or Archive.Order_Type = 0) AND  "
                    "(Archive.TIME_STAMP>=:StartTime and  Archive.TIME_STAMP<:Endtime) Group By ARCHIVE.ARCBILL_KEY,ARCHIVE.ARCHIVE_KEY , ARCHIVE.BASE_PRICE, "
                    "ARCHIVE.MENU_NAME,Archive.Qty, ARCHIVE.TIME_STAMP ) Archive on Ab.ARCHIVE_KEY = Archive.ARCHIVE_KEY WHERE Archive.TIME_STAMP>=:StartTime and  "
                    "Archive.TIME_STAMP<:Endtime Group By ARCHIVE.ARCBILL_KEY, ARCHIVE.MENU_NAME,Ab.ORDER_LOCATION Having Count(Archive.Archive_Key) > 0 "
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
                "AND Archive.TIME_STAMP>= :StartTime and   Archive.TIME_STAMP< :Endtime "
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
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTable6_37( TDBControl* const inDBControl )
{
    if ( !fieldExists( "ARCBILL ", "IS_POSTED_TO_PANASONIC_SERVER ", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ARCBILL "
        "ADD IS_POSTED_TO_PANASONIC_SERVER T_TRUEFALSE DEFAULT 'F' ; ",
        inDBControl);
    }
    if ( !fieldExists( "DAYARCBILL ", "IS_POSTED_TO_PANASONIC_SERVER ", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE DAYARCBILL "
        "ADD IS_POSTED_TO_PANASONIC_SERVER T_TRUEFALSE DEFAULT 'F' ; ",
        inDBControl);
    }
}

//------------------------------------------------------------------------------
void TApplyParser::UpdateContacts6_37( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *FetchQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateSwipeCardQuery    = transaction.Query(transaction.AddQuery());
        FetchQuery->Close();
        FetchQuery->SQL->Text =    "select c.CONTACTS_KEY,c.SWIPE_CARD from CONTACTCARDS c where c.CONTACTCARDS_KEY in "
                                    "(select CARD_KEY From  "
                                    "(SELECT max(a.CONTACTCARDS_KEY) CARD_KEY,a.CONTACTS_KEY FROM CONTACTCARDS a  "
                                    "left join CONTACTS b on a.CONTACTS_KEY = b.CONTACTS_KEY "
                                    "where b.CONTACT_TYPE = 2  "
                                    "group by a.CONTACTS_KEY))";

        UpdateQuery->SQL->Text =  "UPDATE CONTACTS A SET A.PROX_CARD =:PROX_CARD WHERE A.CONTACTS_KEY = :CONTACTS_KEY";
        UpdateSwipeCardQuery->SQL->Text =  "UPDATE CONTACTCARDS A SET A.IS_ACTIVE ='F' WHERE A.CONTACTS_KEY = :CONTACTS_KEY";
        FetchQuery->ExecQuery();
        for (; !FetchQuery->Eof;)
        {
            UpdateQuery->Close();
            UpdateQuery->ParamByName("CONTACTS_KEY")->AsInteger = FetchQuery->FieldByName("CONTACTS_KEY")->AsInteger;
            UpdateQuery->ParamByName("PROX_CARD")->AsString = FetchQuery->FieldByName("SWIPE_CARD")->AsString;
            UpdateQuery->ExecQuery();
            UpdateSwipeCardQuery->ParamByName("CONTACTS_KEY")->AsInteger = FetchQuery->FieldByName("CONTACTS_KEY")->AsInteger;
            UpdateSwipeCardQuery->ExecQuery();
            FetchQuery->Next();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//--------------------------------------------------------------------------------------------------
void TApplyParser::update6_38Tables()
{
     AlterTable_PaymentTypes(_dbControl);
     Updatetable_PaymentTypes(_dbControl);
}
//----------------------------------------------------------------------------------------------------------------
void TApplyParser::AlterTable_PaymentTypes(TDBControl* const inDBControl)
{
     if ( !fieldExists( "PAYMENTTYPES ", "IS_AUTO_POPULATE_BLIND_BALANCE ", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE PAYMENTTYPES "
        "ADD IS_AUTO_POPULATE_BLIND_BALANCE T_TRUEFALSE DEFAULT 'F' ; ",
        inDBControl);
    }
}
//--------------------------------------------------------------------------------------------------------------------------
void TApplyParser::Updatetable_PaymentTypes(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        if ( fieldExists( "PAYMENTTYPES ", "IS_AUTO_POPULATE_BLIND_BALANCE ", _dbControl ) )
        {
            TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());

            UpdateQuery->SQL->Text =  "UPDATE PAYMENTTYPES SET IS_AUTO_POPULATE_BLIND_BALANCE = 'T' "
                                        "WHERE (PAYMENTTYPES.PAYMENT_NAME = 'Gift Card' OR PAYMENTTYPES.PAYMENT_NAME = 'Voucher') ";
            UpdateQuery->ExecQuery();
        }

        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
}


