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
//------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
}

