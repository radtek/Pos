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
//6.50
void TApplyParser::upgrade6_50Tables()
{
    update6_50Tables();
}
//----------------------------------------------------------------------------
void TApplyParser::upgrade6_51Tables()
{
    update6_51Tables();
}
//----------------------------------------------------------------------------
void TApplyParser::upgrade6_52Tables()
{
    update6_52Tables();
}
//----------------------------------------------------------------------------
void TApplyParser::upgrade6_53Tables()
{
    update6_53Tables();
}
//----------------------------------------------------------------------------
void TApplyParser::upgrade6_54Tables()
{
    update6_54Tables();
}

void TApplyParser::upgrade6_55Tables()
{
    update6_55Tables();
}


//::::::::::::::::::::::::Version 6.50:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_50Tables()
{
    Create6_50Generator(_dbControl);
    Alter6_50Tables(_dbControl);
    Create6_50Table(_dbControl);
}
//-----------------------------------------------------------------
void TApplyParser::update6_51Tables()
{
   Create6_51Generators(_dbControl);
   Create6_51Tables(_dbControl);
}
//-----------------------------------------------------------------
void TApplyParser::update6_52Tables()
{
   AlterTable6_52MallExport(_dbControl);
}
//-----------------------------------------------------------
void TApplyParser::update6_53Tables()
{
    Create6_53Generators(_dbControl);
    AlterTableItemSizes6_53(_dbControl);
    AlterTableOrders6_53(_dbControl);
    AlterTableDayArchive6_53(_dbControl);
    AlterTableArchive6_53(_dbControl);
    UpdateOrders6_53(_dbControl);
    UpdateDayArchive6_53(_dbControl);
    UpdateArchive6_53(_dbControl);
    AlterTableArcBills6_53(_dbControl);
    UpdateTableArcBills6_53(_dbControl);

}
//----------------------------------------
void TApplyParser::update6_54Tables()
{
  AlterTableTables6_54(_dbControl);
  AlterTableTab6_54(_dbControl);
  AlterTableOrders6_54(_dbControl);
  AlterTableArchives6_54(_dbControl);
}
//---------------------------------------------------------

void TApplyParser::update6_55Tables()
{
    Create6_55Table(_dbControl);
    Create6_55Generator(_dbControl);

}

//------------------------------------------------------------------------------
void TApplyParser::Create6_50Generator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_PMSPAYTYPEID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_PMSPAYTYPEID;", inDBControl);
		executeQuery("SET GENERATOR GEN_PMSPAYTYPEID TO 0;", inDBControl);
	}
    if(!generatorExists("GEN_ADYENSERVICEID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_ADYENSERVICEID;", inDBControl);
		executeQuery("SET GENERATOR GEN_ADYENSERVICEID TO 0;", inDBControl);
	}
    if(!generatorExists("GEN_ADYENTRANSACTIONID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_ADYENTRANSACTIONID;", inDBControl);
		executeQuery("SET GENERATOR GEN_ADYENTRANSACTIONID TO 0;", inDBControl);
	}
}
//-----------------------------------------------------------
void TApplyParser::Alter6_50Tables(TDBControl* const inDBControl)
{
    if (fieldExists( "VARSPROFILE", "VARCHAR_VAL", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE  VARSPROFILE "
        "ALTER VARCHAR_VAL TYPE VARCHAR(200) ; ",
		inDBControl);
	}
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_50Table(TDBControl* const inDBControl)
{
    if ( !tableExists( "PMSPAYMENTSCONFIG", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE PMSPAYMENTSCONFIG "
        "( "
        "  PMS_PAYTYPE_ID INTEGER NOT NULL PRIMARY KEY, "
        "  PMS_PAYTYPE_NAME VARCHAR(50),                "
        "  PMS_PAYTYPE_CODE VARCHAR(10),                " // length to be checked
        "  PMS_PAYTYPE_CATEGORY INTEGER,                "
        "  PMS_MM_PAYTYPELINK INTEGER,                  "
        "  IS_ELECTRONICPAYMENT T_TRUEFALSE DEFAULT 'F'  "
        ");",
		inDBControl );    //PAYMENT_KEY
        executeQuery(
		"ALTER TABLE PMSPAYMENTSCONFIG ADD CONSTRAINT PMS_PAYTYPE_CONSTRAINT "
		"FOREIGN KEY (PMS_MM_PAYTYPELINK) REFERENCES PAYMENTTYPES (PAYMENT_KEY) ON UPDATE CASCADE ON DELETE CASCADE;", inDBControl );
        ModifyElectronicsPayments(_dbControl);
        PopulateMMPaymentTypes(_dbControl);
        PopulateDefaultPaymentType(_dbControl);
    }
}
//------------------------------------------------------------------------------
void TApplyParser::PopulateMMPaymentTypes(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        TIBSQL *SelectQuery       = transaction.Query(transaction.AddQuery());
        TIBSQL *InsertQuery       = transaction.Query(transaction.AddQuery());
        TIBSQL *GeneratorQuery    = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT * FROM PAYMENTTYPES ";
        SelectQuery->ExecQuery();

        for (; !SelectQuery->Eof; SelectQuery->Next())
        {
            AnsiString tipProperties = SelectQuery->FieldByName("PROPERTIES")->AsString;
            if(tipProperties.Pos("-7-") && tipProperties.Length() == 3)
                continue;

            InsertQuery->Close();
            InsertQuery->SQL->Text = "INSERT INTO  PMSPAYMENTSCONFIG (PMS_PAYTYPE_ID, PMS_PAYTYPE_NAME, PMS_PAYTYPE_CODE,"
                                     " PMS_PAYTYPE_CATEGORY, PMS_MM_PAYTYPELINK, IS_ELECTRONICPAYMENT) VALUES "
                                     " (:PMS_PAYTYPE_ID, :PMS_PAYTYPE_NAME, :PMS_PAYTYPE_CODE,"
                                     " :PMS_PAYTYPE_CATEGORY, :PMS_MM_PAYTYPELINK, :IS_ELECTRONICPAYMENT)";
            GeneratorQuery->Close();
            GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_PMSPAYTYPEID, 1) FROM RDB$DATABASE ";
            GeneratorQuery->ExecQuery();
            int generatedValue = GeneratorQuery->Fields[0]->AsInteger;
            InsertQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger      =  generatedValue;
            InsertQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString     =  SelectQuery->FieldByName("PAYMENT_NAME")->AsString;
            InsertQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString     =  "";
            InsertQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsInteger =  1;
            InsertQuery->ParamByName("PMS_MM_PAYTYPELINK")->AsInteger   =  SelectQuery->FieldByName("PAYMENT_KEY")->AsInteger;
            UnicodeString valueFetched = SelectQuery->FieldByName("PROPERTIES")->AsString;
            if(valueFetched.Pos("-3-"))
                InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "T";
            else
                InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "F";
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//------------------------------------------------------------------------------
void TApplyParser::PopulateDefaultPaymentType(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        TIBSQL *SelectQuery       = transaction.Query(transaction.AddQuery());
        TIBSQL *InsertQuery       = transaction.Query(transaction.AddQuery());
        TIBSQL *GeneratorQuery    = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT * FROM VARSPROFILE WHERE VARIABLES_KEY = :VARIABLES_KEY ";
        SelectQuery->ParamByName("VARIABLES_KEY")->AsInteger = 2103;
        SelectQuery->ExecQuery();

        InsertQuery->Close();
        InsertQuery->SQL->Text = "INSERT INTO  PMSPAYMENTSCONFIG (PMS_PAYTYPE_ID, PMS_PAYTYPE_NAME, PMS_PAYTYPE_CODE,"
                                 " PMS_PAYTYPE_CATEGORY, IS_ELECTRONICPAYMENT) VALUES "
                                 " (:PMS_PAYTYPE_ID, :PMS_PAYTYPE_NAME, :PMS_PAYTYPE_CODE,"
                                 " :PMS_PAYTYPE_CATEGORY, :IS_ELECTRONICPAYMENT)";
        GeneratorQuery->Close();
        GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_PMSPAYTYPEID, 1) FROM RDB$DATABASE ";
        GeneratorQuery->ExecQuery();
        int generatedValue = GeneratorQuery->Fields[0]->AsInteger;
        InsertQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger      =  generatedValue;
        InsertQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString     =  "Default Payment Category";
        InsertQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString     =  "";
        InsertQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsInteger =  0;
        InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "F";
        InsertQuery->ExecQuery();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//------------------------------------------------------------------------------
void TApplyParser::ModifyElectronicsPayments(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *inDBControl );
    transaction.StartTransaction();

    try
    {
        TIBSQL *SelectQuery       = transaction.Query(transaction.AddQuery());
        TIBSQL *UpdateQuery       = transaction.Query(transaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT * FROM PAYMENTTYPES ";
        SelectQuery->ExecQuery();

        for (; !SelectQuery->Eof; SelectQuery->Next())
        {
            AnsiString str = SelectQuery->FieldByName("PROPERTIES")->AsString;
            if(str.Pos("-3-") != 0)
            {
                AnsiString value = SelectQuery->FieldByName("PAYMENT_NAME")->AsString;
                AnsiString newValue = "";
                const char* line = value.c_str();
                for(int i = 0; line[i] != '\0'; i++)
                {
                    if(line[i] != ' ')
                    {
                        newValue += (char)toupper(line[i]);
                    }
                }
                UpdateQuery->Close();
                UpdateQuery->SQL->Text = "UPDATE PAYMENTTYPES  SET PAYMENT_NAME = :PAYMENT_NAME WHERE PAYMENT_KEY = :PAYMENT_KEY";
                UpdateQuery->ParamByName("PAYMENT_NAME")->AsString   =  newValue;
                UpdateQuery->ParamByName("PAYMENT_KEY")->AsInteger   =  SelectQuery->FieldByName("PAYMENT_KEY")->AsInteger;
                UpdateQuery->ExecQuery();
            }
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_51Generators(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_EFTPOSZEDID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_EFTPOSZEDID;", inDBControl);
		executeQuery("SET GENERATOR GEN_EFTPOSZEDID TO 0;", inDBControl);
	}
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_51Tables(TDBControl* const inDBControl)
{
    if ( !tableExists( "EFTPOSZED", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE EFTPOSZED "
        "( "
        "  EFTPOS_ZED_ID INTEGER NOT NULL PRIMARY KEY, "
        "  TIME_STAMP TIMESTAMP,                       "
        "  EFTPOS_TYPE INT,                            "
        "  POS_TERMINALNAME VARCHAR(22),               "
        "  EFTPOS_TERMINALID VARCHAR(20),              "
        "  ZED_RECEIPT BLOB SUB_TYPE 1                 "
        ");",
		inDBControl );
    }
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTable6_52MallExport(TDBControl* const inDBControl)
{
    if (!fieldExists( "ARCMALLEXPORTHOURLY", "Z_KEY", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCMALLEXPORTHOURLY  "
        "ADD Z_KEY INTEGER ; ",
		inDBControl);
	}

    if (!fieldExists( "ARCMALLEXPORT", "Z_KEY", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCMALLEXPORT  "
        "ADD Z_KEY INTEGER ; ",
		inDBControl);
	}

     if (!fieldExists( "MALLEXPORT_HOURLY", "GIFT_CARD", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE MALLEXPORT_HOURLY  "
        "ADD GIFT_CARD Numeric(15,4) ; ",
		inDBControl);
	}

    if (!fieldExists( "MALLEXPORT_HOURLY", "CHECK_SALES", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE MALLEXPORT_HOURLY  "
        "ADD CHECK_SALES Numeric(15,4) ; ",
		inDBControl);
	}


    if (!fieldExists( "ARCMALLEXPORTHOURLY", "GIFT_CARD", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCMALLEXPORTHOURLY  "
        "ADD GIFT_CARD Numeric(15,4) ; ",
		inDBControl);
	}

    if (!fieldExists( "ARCMALLEXPORTHOURLY", "CHECK_SALES", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCMALLEXPORTHOURLY  "
        "ADD CHECK_SALES Numeric(15,4) ; ",
		inDBControl);
	}
}
//------------------------------------------------------------------------------
void TApplyParser::Create6_53Generators(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_ITEM_IDENTIFIER", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_ITEM_IDENTIFIER;", inDBControl);
		executeQuery("SET GENERATOR GEN_ITEM_IDENTIFIER TO 0;", inDBControl);
	}
    if(!generatorExists("GEN_ITEMSIZE_IDENTIFIER", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_ITEMSIZE_IDENTIFIER;", inDBControl);
		executeQuery("SET GENERATOR GEN_ITEMSIZE_IDENTIFIER TO 0;", inDBControl);
	}
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableOrders6_53(TDBControl* const inDBControl)
{
    if ( !fieldExists( "ORDERS", "ONLINE_CHIT_TYPE", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ONLINE_CHIT_TYPE INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "ONLINE_ORDER_ID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ONLINE_ORDER_ID INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "IS_DOCKET_PRINTED", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD IS_DOCKET_PRINTED T_TRUEFALSE DEFAULT 'T' ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "SITE_ID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD SITE_ID INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "ORDER_ITEM_ID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ORDER_ITEM_ID INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "ORDER_ITEM_SIZE_ID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ORDER_ITEM_SIZE_ID INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "REFERENCE_ORDER_ITEM_SIZE_ID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD REFERENCE_ORDER_ITEM_SIZE_ID INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "EMAIL", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD EMAIL Varchar(80) ",
        inDBControl);
    }
    if ( !fieldExists( "ORDERS", "ORDER_GUID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ORDERS "
        "ADD ORDER_GUID VARCHAR(50) ",
        inDBControl);
    }


}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableDayArchive6_53(TDBControl* const inDBControl)
{
    if ( !fieldExists( "DAYARCHIVE", "ONLINE_CHIT_TYPE", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE DAYARCHIVE "
        "ADD ONLINE_CHIT_TYPE INTEGER ",
        inDBControl);
    }
    if ( !fieldExists( "DAYARCHIVE", "ORDER_GUID", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE DAYARCHIVE "
        "ADD ORDER_GUID VARCHAR(50) ",
        inDBControl);
    }
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableArchive6_53(TDBControl* const inDBControl)
{
    if (!fieldExists( "ARCHIVE", "ONLINE_CHIT_TYPE", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCHIVE "
        "ADD ONLINE_CHIT_TYPE INTEGER ",
        inDBControl);
	}
    if (!fieldExists( "ARCHIVE", "ORDER_GUID", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ARCHIVE "
        "ADD ORDER_GUID VARCHAR(50) ",
        inDBControl);
	}
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableItemSizes6_53(TDBControl* const inDBControl)
{
    if (!fieldExists( "ITEM", "ITEM_IDENTIFIER", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ITEM "
        "ADD ITEM_IDENTIFIER VARCHAR(50) ",
        inDBControl);
	}
    if (!fieldExists( "ITEMSIZE", "ITEMSIZE_IDENTIFIER", _dbControl ) )
	{
        executeQuery (
        "ALTER TABLE ITEMSIZE "
        "ADD ITEMSIZE_IDENTIFIER VARCHAR(50) ",
        inDBControl);
	}
}
//-----------------------------------------------------------------------------
void TApplyParser::UpdateOrders6_53(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());

        if ( fieldExists( "ORDERS ", "ONLINE_CHIT_TYPE ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.ONLINE_CHIT_TYPE = 0 WHERE a.ONLINE_CHIT_TYPE IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "ONLINE_ORDER_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.ONLINE_ORDER_ID = 0 WHERE a.ONLINE_ORDER_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "IS_DOCKET_PRINTED ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.IS_DOCKET_PRINTED = 'T' WHERE a.IS_DOCKET_PRINTED IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "SITE_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.SITE_ID = 0 WHERE a.SITE_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "ORDER_ITEM_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.ORDER_ITEM_ID = 0 WHERE a.ORDER_ITEM_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "ORDER_ITEM_SIZE_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.ORDER_ITEM_SIZE_ID = 0 WHERE a.ORDER_ITEM_SIZE_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "REFERENCE_ORDER_ITEM_SIZE_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.REFERENCE_ORDER_ITEM_SIZE_ID = 0 WHERE a.REFERENCE_ORDER_ITEM_SIZE_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "EMAIL ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.EMAIL = '' WHERE a.EMAIL IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ORDERS ", "ORDER_GUID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDERS a SET a.ORDER_GUID = '' WHERE a.ORDER_GUID IS NULL ",
            UpdateQuery->ExecQuery();
        }

        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//-----------------------------------------------------------------------------
void TApplyParser::UpdateDayArchive6_53(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());
        if ( fieldExists( "DAYARCHIVE ", "ONLINE_CHIT_TYPE ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE DAYARCHIVE a SET a.ONLINE_CHIT_TYPE = 0 WHERE a.ONLINE_CHIT_TYPE IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "DAYARCHIVE ", "ORDER_GUID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ORDER_GUID a SET a.ORDER_GUID = '' WHERE a.ORDER_GUID IS NULL ",
            UpdateQuery->ExecQuery();
        }

        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//-----------------------------------------------------------------------------
void TApplyParser::UpdateArchive6_53(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());

        if ( fieldExists( "ARCHIVE ", "ONLINE_CHIT_TYPE ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ARCHIVE a SET a.ONLINE_CHIT_TYPE = 0 WHERE a.ONLINE_CHIT_TYPE IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ARCHIVE ", "ORDER_GUID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ARCHIVE a SET a.ORDER_GUID = '' WHERE a.ORDER_GUID IS NULL ",
            UpdateQuery->ExecQuery();
        }

        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//------------------------------------------------------------------------------
void TApplyParser::AlterTableArcBills6_53(TDBControl* const inDBControl)
{
    if ( !fieldExists( "DAYARCBILL", "EFTPOS_SERVICE_ID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE DAYARCBILL ADD EFTPOS_SERVICE_ID INTEGER ", inDBControl);
    }
    if ( !fieldExists( "ARCBILL", "EFTPOS_SERVICE_ID", _dbControl ) )
    {
        executeQuery ("ALTER TABLE ARCBILL ADD EFTPOS_SERVICE_ID INTEGER ", inDBControl);
    }
}
//-----------------------------------------------------------------------------
void TApplyParser::UpdateTableArcBills6_53(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery    = transaction.Query(transaction.AddQuery());

        if ( fieldExists( "DAYARCBILL ", "EFTPOS_SERVICE_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE DAYARCBILL a SET a.EFTPOS_SERVICE_ID = 0 WHERE a.EFTPOS_SERVICE_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }
        if ( fieldExists( "ARCBILL ", "EFTPOS_SERVICE_ID ", _dbControl ) )
        {
            UpdateQuery->SQL->Text =  "UPDATE ARCBILL a SET a.EFTPOS_SERVICE_ID = 0 WHERE a.EFTPOS_SERVICE_ID IS NULL ",
            UpdateQuery->ExecQuery();
        }

        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
void TApplyParser::AlterTableTab6_54(TDBControl* const inDBControl)
{
    if (fieldExists( "TAB", "TAB_NAME", _dbControl ) )
	{
        executeQuery ("ALTER TABLE TAB ALTER TAB_NAME TYPE VARCHAR(80) ;", inDBControl);
	}
}

void TApplyParser::AlterTableTables6_54(TDBControl* const inDBControl)
{

   if ( !fieldExists( "TABLES", "IS_TABLELOCK", _dbControl ) )
    {
        executeQuery(
		"ALTER TABLE TABLES ADD IS_TABLELOCK CHAR(1) DEFAULT 'F';",
		inDBControl);
    }
}

void TApplyParser::AlterTableOrders6_54(TDBControl* const inDBControl)
{
    if (fieldExists( "ORDERS", "TAB_NAME", _dbControl ) )
	{
        executeQuery ("ALTER TABLE ORDERS ALTER TAB_NAME TYPE VARCHAR(80) ;", inDBControl);
	}
}

void TApplyParser::AlterTableArchives6_54(TDBControl* const inDBControl)
{
    if (fieldExists( "DAYARCHIVE", "TAB_NAME", _dbControl ) )
	{
        executeQuery ("ALTER TABLE DAYARCHIVE ALTER TAB_NAME TYPE VARCHAR(80) ;", inDBControl);
	}
    if (fieldExists( "ARCHIVE", "TAB_NAME", _dbControl ) )
	{
        executeQuery ("ALTER TABLE ARCHIVE ALTER TAB_NAME TYPE VARCHAR(80) ;", inDBControl);
	}
}

//------------------------------------------------------------------------------
void TApplyParser::Create6_55Table(TDBControl* const inDBControl)
{
    if ( !tableExists( "EFTPOSREFRENECE", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE EFTPOSREFRENECE "
        "( "
        "  EFTPOSREFRENCE_ID INTEGER NOT NULL PRIMARY KEY, "
        "  INVOICE_NO VARCHAR(50),                       "
        "  ORIGINAL_REFERENCE VARCHAR(50),                            "
        "  PSREFERENCE VARCHAR(50),              "
        "  MODIFIED_REFERENCE VARCHAR(50),              "
        "  IS_SETTLED CHAR(1) DEFAULT 'F',                "
        "  MERCHANT_ID VARCHAR(50)                "
        ");",
		inDBControl );
    }
}

////------------------------------------------------------------------------------
void TApplyParser::Create6_55Generator(TDBControl* const inDBControl)
{

     if(!generatorExists("GEN_EFTPOSREFERENCE_ID", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_EFTPOSREFERENCE_ID;", inDBControl);
		executeQuery("SET GENERATOR GEN_EFTPOSREFERENCE_ID TO 0;", inDBControl);
	}

}
}






