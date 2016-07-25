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

//6.20
void TApplyParser::upgrade6_20Tables()
{
	update6_20Tables();
}
//6.21
void TApplyParser::upgrade6_21Tables()
{
	update6_21Tables();
}
//6.22
void TApplyParser::upgrade6_22Tables()
{
	update6_22Tables();
}
//6.23
void TApplyParser::upgrade6_23Tables()
{
	update6_23Tables();
}
//6.24
void TApplyParser::upgrade6_24Tables()
{
	update6_24Tables();
}
//6.25
void TApplyParser::upgrade6_25Tables()
{
    update6_25Tables();
}
//::::::::::::::::::::::::Version 6.20::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_20Tables()
{
    create6_20TableForThorVouchers(_dbControl);
    populateTableForThorVouchers6_20(_dbControl);
    create6_20TableForThorVoucher_Categories(_dbControl);
    create6_20ThorVoucherCategoriesGenerator(_dbControl);
    update6_20TablesForOrderDiscounts(_dbControl);
    Create6_20PriceLevels(_dbControl);
    Alter6_20HappyHour_Profile(_dbControl);
    Create6_20PriceLevelItemSize(_dbControl);
    Update6_20_New_Tables(_dbControl);
    Create6_20_HappyHour_Priority(_dbControl);
    Update6_20_HappyHour_Priority(_dbControl);
    Update6_20_HappyHour_Profiles(_dbControl);
}
//---------------------------------------------------------------------------------------------------
void TApplyParser::create6_20TableForThorVouchers( TDBControl* const inDBControl )
{
	if ( !tableExists( "THORLINK_VOUCHERS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE THORLINK_VOUCHERS "
		"( "
		"   THOR_VOUCHERS_KEY int NOT NULL primary key,"
		"   NAME varchar(85),"
        "   DESCRIPTION varchar(128),"
        "   MENU varchar(15),"
        "   PERCENTAGE Numeric(15,4),"
        "   AMOUNT Numeric(15,4),"
        "   VOUCHER_TYPE int,"
        "   VOUCHER_MODE int,"
        "   ROUNDING Numeric(15,4),"
        "   CATEGORY_KEY int,"
        "   PROPERTIES Numeric(15,4),"
        "   PRIORITY int,"
        "   VOUCHER_ID int,"
        "   APPEARANCE_ORDER int,"
        "   MEMBERS_ONLY T_TRUEFALSE default 'F',"
        "   MEMBERS_EXEMPT T_TRUEFALSE default 'F',"
        "   APPLYTOMEMBERS T_TRUEFALSE default 'F',"
        "   MAX_VALUE Numeric(15,4) default 0,"
        "   VOUCHER_GROUP int default 0,"
        "   IsThorGROUP T_TRUEFALSE default 'T',"
        "   FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES(CATEGORY_KEY) ON UPDATE CASCADE "
		");",
		inDBControl );
        }
	}
//-----------------------------------------------------------------------
void TApplyParser::Create6_20PriceLevels(TDBControl* const inDBControl)
{
     if ( !tableExists( "PRICELEVELS", inDBControl ) )
     	{
		executeQuery(
                "CREATE TABLE PRICELEVELS "
                "( "
                "   PRICELEVEL_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   PRICELEVEL_NAME VARCHAR(50), "
                "   ISENABLED VARCHAR(50) "
                ");",
			inDBControl );

        }

 }
//-----------------------------------------------------------------------
void TApplyParser::Alter6_20HappyHour_Profile(TDBControl* const inDBControl)
{
      if ( !fieldExists("HAPPYHOURPROFILES", "PRICELEVEL_KEY", inDBControl ) )
     	{
		executeQuery(  "ALTER TABLE HAPPYHOURPROFILES "
                       "ADD PRICELEVEL_KEY INTEGER ;",
         			inDBControl );
   	    }


}
//-------------------------------------------------------------------------
void TApplyParser::Create6_20PriceLevelItemSize(TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_PRICELEVELITEMSIZE_KEY", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_PRICELEVELITEMSIZE_KEY;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_PRICELEVELITEMSIZE_KEY TO 0; ",
		inDBControl );
	}



      if ( !tableExists( "PriceLevelItemSize", inDBControl ) )
     	{
		executeQuery(
                "CREATE TABLE PRICELEVELITEMSIZE "
                "( "

                "   PRICELEVELITEMSIZE_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   ITEMSIZE_KEY INTEGER , "
                "   PRICELEVEL_KEY INTEGER, "
                "   PRICE NUMERIC(17,4), "
                "   ISENABLED VARCHAR(5), "
                "   foreign key(ITEMSIZE_KEY) references ITEMSIZE(ITEMSIZE_KEY) ON DELETE cascade"
                ");",
			inDBControl );
        }
}
//-----------------------------------------------------------
void  TApplyParser::Update6_20_New_Tables(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectGenQuery    = transaction.Query( transaction.AddQuery() );
        InsertQuery->Close();
        InsertQuery->SQL->Text = "Insert into PRICELEVELS Values(1,'PriceLevel 1','T')";
        InsertQuery->ExecQuery();
        InsertQuery->Close();
        InsertQuery->SQL->Text = "Insert into PRICELEVELS Values(2,'PriceLevel 2','T')";
        InsertQuery->ExecQuery();
        InsertQuery->SQL->Text = "INSERT INTO PRICELEVELITEMSIZE "
                                "( "
                                "   PriceLevelItemSize_Key, "
                                "   ItemSize_Key, "
                                "   PRICELEVEL_KEY, "
                                "   Price, "
                                "   IsEnabled "
                                ") "
                                "VALUES "
                                "( "
                                "   :PriceLevelItemSize_Key, "
                                "   :ItemSize_Key, "
                                "   :PRICELEVEL_KEY, "
                                "   :Price, "
                                "   :IsEnabled "
                                ");";

        SelectQuery->Close();
        SelectQuery->SQL->Text = " SELECT a.ITEMSIZE_KEY,  a.PRICE, a.SPECIAL_PRICE "
        " FROM ITEMSIZE a group by a.ITEMSIZE_KEY,a.PRICE, a.SPECIAL_PRICE ";
        SelectQuery->ExecQuery();
        int generator_Val;
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
            //inserting the price level values in table pricelevelitemsize
            SelectGenQuery->Close();
            SelectGenQuery->SQL->Text = "SELECT GEN_ID(GEN_PRICELEVELITEMSIZE_KEY, 1) FROM RDB$DATABASE";
            SelectGenQuery->ExecQuery();
            generator_Val = SelectGenQuery->Fields[0]->AsInteger;
            InsertQuery->Close();
            InsertQuery->ParamByName("PriceLevelItemSize_Key")->AsInteger =generator_Val;
            InsertQuery->ParamByName("ItemSize_Key")->AsInteger = SelectQuery->Fields[0]->AsInteger;
            InsertQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = 1;
            InsertQuery->ParamByName("Price")->AsFloat = SelectQuery->Fields[1]->AsFloat;
            InsertQuery->ParamByName("IsEnabled")->AsString = "T";
            InsertQuery->ExecQuery();
            SelectGenQuery->Close();
            SelectGenQuery->SQL->Text = "SELECT GEN_ID(GEN_PriceLevelItemSize_KEY, 1) FROM RDB$DATABASE";
            SelectGenQuery->ExecQuery();
            generator_Val = SelectGenQuery->Fields[0]->AsInteger;
            InsertQuery->Close();
            InsertQuery->ParamByName("PriceLevelItemSize_Key")->AsInteger = generator_Val;
            InsertQuery->ParamByName("ItemSize_Key")->AsInteger = SelectQuery->Fields[0]->AsInteger;
            InsertQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = 2;
            InsertQuery->ParamByName("Price")->AsFloat = SelectQuery->Fields[2]->AsFloat;
            InsertQuery->ParamByName("IsEnabled")->AsString = "T";
            InsertQuery->ExecQuery();
        }
        SelectQuery->Close();
        InsertQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::Create6_20_HappyHour_Priority(TDBControl* const inDBControl)
{
    if ( !tableExists( "HAPPYHOUR_PRIORITY", inDBControl ) )
    {
        executeQuery(
        "CREATE TABLE HAPPYHOUR_PRIORITY "
        "( "
        "   HAPPYHOURPROFILES_KEY  INTEGER , "
        "   HAPPYHOUR_PRIORITY_VAL INTEGER, "
        "   PRIMARY KEY (HAPPYHOURPROFILES_KEY , HAPPYHOUR_PRIORITY_VAL) "
        ");",
        inDBControl );
    }
 }
//---------------------------------------------------------------------------
void TApplyParser::Update6_20_HappyHour_Priority(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    int happyHour_ProfileKey;
    try
    {
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *IBInternalQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectGenQuery = transaction.Query( transaction.AddQuery() );
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT hhp.HAPPYHOURPROFILES_KEY, "
        "hhp.HAPPYHOURPROFILE_NAME, "
        "hhp.HAPPYHOURDAY_STARTTIME, "
        "hhp.HAPPYHOURDAY_ENDTIME, "
        "hhp.HAPPYHOURDAY_PROFILEDATE "
        "FROM HAPPYHOURPROFILES hhp "
        "WHERE "
        "hhp.ISACTIVE  = :ISACTIVE ;";
        IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
        IBInternalQuery->ExecQuery();
        for ( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            happyHour_ProfileKey = IBInternalQuery->FieldByName("HAPPYHOURPROFILES_KEY")->AsInteger;
            InsertQuery->Close();
            InsertQuery->SQL->Text = "INSERT INTO HAPPYHOUR_PRIORITY "
            "( "
            "   HAPPYHOURPROFILES_KEY, "
            "   HAPPYHOUR_PRIORITY_VAL "
            ") "
            "VALUES "
            "( "
            "   :HAPPYHOURPROFILES_KEY, "
            "   :HAPPYHOUR_PRIORITY_VAL "
            ");";
            InsertQuery->ParamByName("HAPPYHOURPROFILES_KEY")->AsInteger =  happyHour_ProfileKey;
            InsertQuery->ParamByName("HAPPYHOUR_PRIORITY_VAL")->AsInteger =happyHour_ProfileKey;
            InsertQuery->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::Update6_20_HappyHour_Profiles(TDBControl* const inDBControl)
{
    // update the pricelevel key by 2
    if ( fieldExists("HAPPYHOURPROFILES", "PRICELEVEL_KEY", inDBControl ) )
    {
        TDBTransaction transaction( *_dbControl );
        transaction.StartTransaction();
        try
        {
            TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
            UpdateQuery->Close();
            UpdateQuery->SQL->Text = "UPDATE HAPPYHOURPROFILES a SET a.PRICELEVEL_KEY = :PRICELEVEL_KEY "
            "WHERE a.ISACTIVE = :ISACTIVE ";
            UpdateQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = 2;
            UpdateQuery->ParamByName("ISACTIVE")->AsString ="T";
            UpdateQuery->ExecQuery();
        }
        catch( Exception &E )
        {
            transaction.Rollback();
        }
        transaction.Commit();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::populateTableForThorVouchers6_20( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *inDBControl );
	const int NUMBER_OF_FIELDS = 31;
    int thorVoucherkeys[NUMBER_OF_FIELDS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
	UnicodeString names[NUMBER_OF_FIELDS] = { "01: Birthday Coffee Gift","02: 2FOR1 COFFEE","03: 20% OFF TAKE HOME COFFEE PACKS","04: COFFEE  FOR 200 PTS","05: WELCOME TO REWARDS 2FOR1 COFFEE","06: WELCOME BACK 2FOR1 COFFEE","07: FREE COFFEE",
    "08: MITRE10 BIRTHDAY 2FOR1 COFFEE","09: MITRE10 FREE BIRTHDAY COFFEE","10: 2FOR1 COFFEE GIFT WITH APP","11: 100 PT COFFEE GIFT ","12: 2FOR1 BEVERAGE","13: SMOOTHIE OR FRAPPE 325PT","14: PAPERPLUS FREE COFFEE PROMO","15: 2X BIRTHDAY COFFEES",
    "16: VODAFONE SEMBLE COFFEE","17: 5 HOT BEV","18: 10 HOT BEV","19: $25 VOUCHER","20: HFG 25% OFF","21: HFG FREE COFFEE","22: GREEN IDEAS FREE COFFEE","23: 25% OFF LIFESTYLE FAVOURITES","24: FREE KIDS HOT CHOC","25: 250 POINTS HOT BEVERAGE",
    "26: CANOE NZ 2FOR1 HOT BEVERAGE","27: 2FOR1 ANY BEVERAGE","28: FREE COFFEE BUILD MAG","29: 2FOR1 HOT BEVERAGE","30: PIT STOP COMP HOT BEVERAGE","31: AA SMARTFUEL COMP HOT BEVERAGE"};
    UnicodeString description[NUMBER_OF_FIELDS] = { "Birthday Coffee Gift","2FOR1 COFFEE","20% OFF TAKE HOME COFFEE PACKS","COFFEE  FOR 200 PTS","WELCOME TO REWARDS 2FOR1 COFFEE","WELCOME BACK 2FOR1 COFFEE","FREE COFFEE",
    "MITRE10 BIRTHDAY 2FOR1 COFFEE","MITRE10 FREE BIRTHDAY COFFEE","2FOR1 COFFEE GIFT WITH APP","100 PT COFFEE GIFT","2FOR1 BEVERAGE","SMOOTHIE OR FRAPPE 325PT","PAPERPLUS FREE COFFEE PROMO","2X BIRTHDAY COFFEES","VODAFONE SEMBLE COFFEE","5 HOT BEV","10 HOT BEV",
    "$25 VOUCHER","HFG 25% OFF","HFG FREE COFFEE","GREEN IDEAS FREE COFFEE","25% OFF LIFESTYLE FAVOURITES","FREE KIDS HOT CHOC","250 POINTS HOT BEVERAGE","CANOE NZ 2FOR1 HOT BEVERAGE","2FOR1 ANY BEVERAGE","FREE COFFEE BUILD MAG","2FOR1 HOT BEVERAGE",
    "PIT STOP COMP HOT BEVERAGE","AA SMARTFUEL COMP HOT BEVERAGE"};
    double percentage[NUMBER_OF_FIELDS] = { 0,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,0,0,25,0,0,0,0,0,0,0,0};
    double amount[NUMBER_OF_FIELDS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,0,0,0,0,0,0,0,0,0,0,0,0};
    int vouchermode[NUMBER_OF_FIELDS] ={ 5,5,1,5,5,5,5,5,5,5,5,5,5,5,0,5,5,5,0,1,5,5,1,5,5,5,5,5,5,5,5};
	try
	{
		transaction.StartTransaction();

		for (int i = 0; i < NUMBER_OF_FIELDS; i++)
		{
			TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
			insertQuery->SQL->Text = "INSERT INTO THORLINK_VOUCHERS "
			"( "
			"   THOR_VOUCHERS_KEY,"
		    "   NAME,"
            "   DESCRIPTION,"
            "   PERCENTAGE,"
            "   AMOUNT,"
            "   VOUCHER_TYPE,"
            "   VOUCHER_MODE,"
            "   ROUNDING,"
            "   PRIORITY,"
            "   APPEARANCE_ORDER,"
            "   MEMBERS_ONLY,"
            "   MEMBERS_EXEMPT,"
            "   APPLYTOMEMBERS,"
            "   MAX_VALUE,"
            "   VOUCHER_GROUP,"
            "   ISTHORGROUP"
			") "
			"VALUES "
			"( "
			"   :THOR_VOUCHERS_KEY,"
		    "   :NAME,"
            "   :DESCRIPTION,"
            "   :PERCENTAGE,"
            "   :AMOUNT,"
            "   :VOUCHER_TYPE,"
            "   :VOUCHER_MODE,"
            "   :ROUNDING,"
            "   :PRIORITY,"
            "   :APPEARANCE_ORDER,"
            "   :MEMBERS_ONLY,"
            "   :MEMBERS_EXEMPT,"
            "   :APPLYTOMEMBERS,"
            "   :MAX_VALUE,"
            "   :VOUCHER_GROUP,"
            "   :ISTHORGROUP"
			");";
			insertQuery->ParamByName("THOR_VOUCHERS_KEY")->AsInteger = thorVoucherkeys[i];
			insertQuery->ParamByName("NAME")->AsString  = names[i];
			insertQuery->ParamByName("DESCRIPTION")->AsString  = description[i];
			insertQuery->ParamByName("PERCENTAGE")->AsDouble  = percentage[i];
            insertQuery->ParamByName("AMOUNT")->AsDouble = amount[i];
            insertQuery->ParamByName("VOUCHER_TYPE")->AsInteger = 0;
            insertQuery->ParamByName("VOUCHER_MODE")->AsInteger = vouchermode[i];
            insertQuery->ParamByName("ROUNDING")->AsDouble = 0.0100;
            insertQuery->ParamByName("PRIORITY")->AsInteger = 0;
            insertQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = 0;
            insertQuery->ParamByName("MEMBERS_ONLY")->AsString = "F";
            insertQuery->ParamByName("MEMBERS_EXEMPT")->AsString = "F";
            insertQuery->ParamByName("APPLYTOMEMBERS")->AsString = "F";
            insertQuery->ParamByName("MAX_VALUE")->AsDouble = 0;
            insertQuery->ParamByName("VOUCHER_GROUP")->AsInteger = 0;
            insertQuery->ParamByName("ISTHORGROUP")->AsString = "T";
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
void TApplyParser::create6_20TableForThorVoucher_Categories( TDBControl* const inDBControl )
{
	if ( !tableExists( "THORVOUCHERS_CATEGORIES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE THORVOUCHERS_CATEGORIES "
		"( "
		"   THORVOUCHERS_CATEGORIES_KEY Integer NOT NULL primary key,"
        "   Is_THORVOUCHERS T_TRUEFALSE default 'F',"
		"   THOR_VOUCHERS_KEY Integer NOT NULL,"
        "   CATEGORY_KEY Integer NOT NULL,"
        "   FOREIGN KEY (THOR_VOUCHERS_KEY) REFERENCES THORLINK_VOUCHERS (THOR_VOUCHERS_KEY) ON UPDATE CASCADE ON DELETE CASCADE, "
        "   FOREIGN KEY (CATEGORY_KEY) REFERENCES ARCCATEGORIES (CATEGORY_KEY) ON UPDATE CASCADE ON DELETE CASCADE "
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_20ThorVoucherCategoriesGenerator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_THORCATEGORIES_TYPE", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_THORCATEGORIES_TYPE;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_THORCATEGORIES_TYPE TO 0;", inDBControl
        );
    }
}
//---------------------------------------------------------------------------------------------------
void TApplyParser::update6_20TablesForOrderDiscounts( TDBControl* const inDBControl )
{   if ( !fieldExists("ORDERDISCOUNTS", "ISTHOR_DISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERDISCOUNTS ADD ISTHOR_DISCOUNT T_TRUEFALSE default 'F';",
		inDBControl );
	}

}


//::::::::::::::::::::::::Version 6.21::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_21Tables()
{
  Update6_21_OrderTable(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::Update6_21_OrderTable( TDBControl* const inDBControl )
{

 if ( !fieldExists("ORDERS", "IS_MANUALLY_ENTERED_WEIGHT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS ADD IS_MANUALLY_ENTERED_WEIGHT T_TRUEFALSE default 'F';",
		inDBControl );
	}

}

//::::::::::::::::::::::::Version 6.22::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_22Tables()
{
    Insert6_22_PricelevelDefaultValueInVariableTable(_dbControl);
    AutoCorrectPricelevel(_dbControl);
    UpdateArchive6_22(_dbControl);
    UpdateDayArchive6_22(_dbControl);
    UpdateOrders6_22(_dbControl);
    ModifyItemSizesColumns6_22(_dbControl);
    Update6_22_Thorlink_Vouchers(_dbControl);
}
//---------------------------------------------------------------------------------------------------
void TApplyParser::Insert6_22_PricelevelDefaultValueInVariableTable( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    int happyHour_ProfileKey;
    try
    {
        TIBSQL *IBInternalQuery = transaction.Query( transaction.AddQuery() );
        TIBSQL *sqlSetNoOfPriceLevels = transaction.Query( transaction.AddQuery() );
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT INTEGER_VAL "
        "FROM VARIABLES hhp "
        "WHERE "
        "hhp.VARIABLES_KEY  = :VARIABLES_KEY ;";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 5014;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->Eof)
        {
            sqlSetNoOfPriceLevels->SQL->Text =
            "INSERT INTO VARIABLES ("
            "VARIABLES_KEY,"
            "VARIABLE_NAME,"
            "VARIABLE_DESCRIPTION,"
            "VARIABLE_GROUP,"
            "INTEGER_VAL) "
            "VALUES ("
            ":VARIABLES_KEY,"
            ":VARIABLE_NAME,"
            ":VARIABLE_DESCRIPTION,"
            ":VARIABLE_GROUP,"
            ":INTEGER_VAL);";
            sqlSetNoOfPriceLevels->ParamByName("VARIABLES_KEY")->AsInteger = 5014;
            sqlSetNoOfPriceLevels->ParamByName("VARIABLE_NAME")->AsString = "No Of Price Levels.";
            sqlSetNoOfPriceLevels->ParamByName("VARIABLE_DESCRIPTION")->AsString = "No Of Price Levels for Happy Hour.Default is 2.";
            sqlSetNoOfPriceLevels->ParamByName("VARIABLE_GROUP")->AsInteger = 6;
            sqlSetNoOfPriceLevels->ParamByName("INTEGER_VAL")->AsInteger = 2;
            sqlSetNoOfPriceLevels->ExecQuery();
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------------------------------
void TApplyParser::AutoCorrectPricelevel( TDBControl* const inDBControl )
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    int happyHour_ProfileKey;
    int noOfPriceLevels;
    try
    {
        TIBSQL *IBInternalQuery              = transaction.Query( transaction.AddQuery() );
        TIBSQL *SelectItemsizeKey    = transaction.Query( transaction.AddQuery() );
        TIBSQL *PriceLevelOfEachItemSizeKey    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdatePriceLevelKeyOfItemSize    = transaction.Query( transaction.AddQuery() );
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "SELECT INTEGER_VAL "
        "FROM VARIABLES hhp "
        "WHERE "
        "hhp.VARIABLES_KEY  = :VARIABLES_KEY ;";
        IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 5014;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
            noOfPriceLevels=IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;
        SelectItemsizeKey->Close();
        SelectItemsizeKey->SQL->Text =
        "SELECT a.ITEMSIZE_KEY FROM ITEMSIZE a ";
        SelectItemsizeKey->ExecQuery();
        for(; !SelectItemsizeKey->Eof; SelectItemsizeKey->Next())
        {
            bool autoCorrectPriceLevelKeys=false;
            int itemSizeKey=SelectItemsizeKey->FieldByName("ITEMSIZE_KEY")->AsInteger;
            PriceLevelOfEachItemSizeKey->Close();
            PriceLevelOfEachItemSizeKey->SQL->Text =
            "SELECT a.PRICELEVELITEMSIZE_KEY, a.ITEMSIZE_KEY, a.PRICELEVEL_KEY, a.PRICE, a.ISENABLED "
            "FROM PRICELEVELITEMSIZE a "
            "WHERE "
            "a.ITEMSIZE_KEY  = :ITEMSIZE_KEY AND "
            "a.ISENABLED = :ISENABLED "
            "ORDER BY a.PRICELEVELITEMSIZE_KEY ;";
            PriceLevelOfEachItemSizeKey->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSizeKey;
            PriceLevelOfEachItemSizeKey->ParamByName("ISENABLED")->AsString = "T";
            PriceLevelOfEachItemSizeKey->ExecQuery();
            for(; !PriceLevelOfEachItemSizeKey->Eof; PriceLevelOfEachItemSizeKey->Next())
            {
                int PRICELEVEL_KEY = PriceLevelOfEachItemSizeKey->FieldByName("PRICELEVEL_KEY")->AsInteger;
                if(PRICELEVEL_KEY<=0 || PRICELEVEL_KEY > noOfPriceLevels)
                {
                    autoCorrectPriceLevelKeys=true;
                    break;
                }
            }
            if(autoCorrectPriceLevelKeys)
            {
                PriceLevelOfEachItemSizeKey->Close();
                PriceLevelOfEachItemSizeKey->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSizeKey;
                PriceLevelOfEachItemSizeKey->ExecQuery();
                int key =1;
                for(; !PriceLevelOfEachItemSizeKey->Eof; PriceLevelOfEachItemSizeKey->Next())
                {
                    UpdatePriceLevelKeyOfItemSize->Close();
                    UpdatePriceLevelKeyOfItemSize->SQL->Text =
                    "UPDATE PRICELEVELITEMSIZE a set a.PRICELEVEL_KEY = :PRICELEVEL_KEY "
                    "WHERE a.PRICELEVELITEMSIZE_KEY = :PRICELEVELITEMSIZE_KEY ";
                    UpdatePriceLevelKeyOfItemSize->ParamByName("PRICELEVEL_KEY")->AsInteger = key;
                    UpdatePriceLevelKeyOfItemSize->ParamByName("PRICELEVELITEMSIZE_KEY")->AsInteger = PriceLevelOfEachItemSizeKey->FieldByName("PRICELEVELITEMSIZE_KEY")->AsInteger;
                    UpdatePriceLevelKeyOfItemSize->ExecQuery();
                    key++;
                }
            }
        }
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateArchive6_22( TDBControl* const inDBControl )
{

    if ( !fieldExists("ARCHIVE", "PRICE_INCL", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ARCHIVE ADD PRICE_INCL NUMERIC(15,4);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateDayArchive6_22( TDBControl* const inDBControl )
{
    if ( !fieldExists("DAYARCHIVE", "PRICE_INCL", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE DAYARCHIVE ADD PRICE_INCL NUMERIC(15,4);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateOrders6_22( TDBControl* const inDBControl )
{
    if ( !fieldExists("ORDERS", "PRICE_INCL", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ORDERS ADD PRICE_INCL NUMERIC(15,4);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::Update6_22_Thorlink_Vouchers( TDBControl* const inDBControl )
{
    if ( !fieldExists("THORLINK_VOUCHERS", "VOUCHERS_CODE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE THORLINK_VOUCHERS ADD VOUCHERS_CODE VARCHAR(15);",
        inDBControl );
    }
    if ( !fieldExists("THORLINK_VOUCHERS", "VOUCHER_APPLIED", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE THORLINK_VOUCHERS ADD VOUCHER_APPLIED T_TRUEFALSE default 'F';",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyItemSizesColumns6_22( TDBControl* const inDBControl )
{
    if ( !fieldExists( "ITEMSIZE", "PRICE_FOR_POINTS", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ITEMSIZE "
        "ADD PRICE_FOR_POINTS NUMERIC(15,4) DEFAULT 0; ",
        inDBControl);
    }
}

//::::::::::::::::::::::::Version 6.23::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_23Tables()
{
    create6_23ThorVoucherKeysGenerator(_dbControl);
}
//----------------------------------------------------------------------------
void TApplyParser::create6_23ThorVoucherKeysGenerator(TDBControl* const inDBControl)
{
    if(!generatorExists("GEN_THORVOUCHER_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_THORVOUCHER_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_THORVOUCHER_KEY TO 31;", inDBControl
        );
    }
}

//::::::::::::::::::::::::Version 6.24::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_24Tables()
{
	UpdateContactsTable6_24(_dbControl);
	UpdateLoyaltyTransactionTable6_24(_dbControl);
	UpdateTierLevelsTable6_24(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateContactsTable6_24( TDBControl* const inDBControl )
{
	if ( !fieldExists( "CONTACTS", "MEMBER_CARD_CODE", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE CONTACTS "
		"ADD MEMBER_CARD_CODE VARCHAR(50);",
		inDBControl);
	}


    if ( !fieldExists( "CONTACTS", "IS_FIRSTVISIT_REWARDED", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE CONTACTS "
		"ADD IS_FIRSTVISIT_REWARDED CHAR(1);",
		inDBControl);
	}
    TDBTransaction transaction( *inDBControl );
	try
	{
		transaction.StartTransaction();
		TIBSQL *query = transaction.Query( transaction.AddQuery() );
		query->SQL->Text = "Update CONTACTS a set a.IS_FIRSTVISIT_REWARDED = 'T';";
		query->ExecQuery();
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateLoyaltyTransactionTable6_24( TDBControl* const inDBControl )
{
  if ( fieldExists( "LOYALTYPENDINGTRANSACTIONS", "EARNED_POINTS", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"DROP EARNED_POINTS;",
		inDBControl);
	}
  if ( fieldExists( "LOYALTYPENDINGTRANSACTIONS", "EARNED_POINTS_DELTA", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"DROP EARNED_POINTS_DELTA;",
		inDBControl);
	}
  if ( fieldExists( "LOYALTYPENDINGTRANSACTIONS", "LOADED_POINTS_DELTA", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"DROP LOADED_POINTS_DELTA;",
		inDBControl);
	}
  if ( fieldExists( "LOYALTYPENDINGTRANSACTIONS", "LOADED_POINTS", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"DROP LOADED_POINTS;",
		inDBControl);
	}
  if ( !fieldExists( "LOYALTYPENDINGTRANSACTIONS", "POINT_BALANCE", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"ADD POINT_BALANCE NUMERIC(15,4);",
		inDBControl);
	}
  if ( !fieldExists( "LOYALTYPENDINGTRANSACTIONS", "POINTS_DELTA", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"ADD POINTS_DELTA NUMERIC(15,4);",
		inDBControl);
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTierLevelsTable6_24( TDBControl* const inDBControl )
{
   executeQuery (
   "ALTER TABLE TIER_LEVELS ALTER POINTS_REQUIRED TYPE Numeric(15,2);",
   inDBControl);

}

//::::::::::::::::::::::::Version 6.25::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_25Tables()
{
    UpdateDiscountsTable6_25(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateDiscountsTable6_25(TDBControl* const inDBControl)
{
   if ( !fieldExists( "DISCOUNTS", "MAX_ITEM_AFFECTED", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE DISCOUNTS "
		"ADD MAX_ITEM_AFFECTED INTEGER DEFAULT 0;",
		inDBControl);
	}
   if ( !fieldExists( "DISCOUNTS", "MIN_ITEMS_REQUIRED", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE DISCOUNTS "
		"ADD MIN_ITEMS_REQUIRED INTEGER DEFAULT 0;",
		inDBControl);
	}
}
}
