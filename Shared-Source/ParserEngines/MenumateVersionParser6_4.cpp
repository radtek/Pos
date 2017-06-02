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
//6.41
void TApplyParser::upgrade6_41Tables()
{
    update6_41Tables();
}

//::::::::::::::::::::::::Version 6.40:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_40Tables()
{
   UpdateChargeToAccount(_dbControl);
}

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
//::::::::::::::::::::::::Version 6.41:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_41Tables()
{
//   Create6_41Generators(_dbControl);
   UpdateTaxCodes(_dbControl);
   UpdateRevenueCodes(_dbControl);
}
//void TApplyParser::Create6_41Generators(TDBControl* const inDBControl)
//{
//    if(!generatorExists("GEN_TAXCODESNUMBER", _dbControl))
//    {
//        executeQuery(
//            "CREATE GENERATOR GEN_TAXCODESNUMBER;", inDBControl
//        );
//        executeQuery(
//            "SET GENERATOR GEN_TAXCODESNUMBER TO 0;", inDBControl
//        );
//    }
//    if(!generatorExists("GEN_REVENUECODESNUMBER", _dbControl))
//    {
//        executeQuery(
//            "CREATE GENERATOR GEN_REVENUECODESNUMBER;", inDBControl
//        );
//        executeQuery(
//            "SET GENERATOR GEN_REVENUECODESNUMBER TO 0;", inDBControl
//        );
//    }
//}
void TApplyParser::UpdateTaxCodes(TDBControl* const inDBControl)
{
    if ( !tableExists( "TAXCODESDETAILS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TAXCODESDETAILS "
		"( "
        "   TAXCODE INT NOT NULL PRIMARY KEY, "
		"   VAT_PERCENTAGE NUMERIC(15,4), "
		"   SERVICECHARGE_PERCENTAGE NUMERIC(15,4), "
		"   TAXONSERVICECHARGE_PERCENTAGE NUMERIC(15,4) "
		");",
		inDBControl );
    }
}
void TApplyParser::UpdateRevenueCodes(TDBControl* const inDBControl)
{
    if ( !tableExists( "REVENUECODEDETAILS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE REVENUECODEDETAILS "
		"( "
        "   REVENUECODE INT NOT NULL PRIMARY KEY, "
		"   REVENUECODE_DESCRIPTION varchar(50)"
		");",
		inDBControl );
    }
}
}
