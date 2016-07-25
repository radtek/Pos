//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------

// MENUMATE VERSION 5's PARSER.

namespace MenumateVersionParser
{
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.18.
void TApplyParser::upgrade5_18Tables()
{
    update5_18Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_18Tables()
{
	updateOrdersTable5_18( _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::updateOrdersTable5_18( TDBControl* const inDBControl )
{
	executeQuery(
		"ALTER TABLE ORDERS ADD ACTIVECHITNUMBER_KEY INTEGER DEFAULT NULL",
		inDBControl );

	executeQuery(
		"ALTER TABLE ORDERS ADD FOREIGN KEY (ACTIVECHITNUMBER_KEY) REFERENCES ACTIVECHITNUMBER(ACTIVECHITNUMBER_KEY)",
		inDBControl );
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.19.
void TApplyParser::upgrade5_19Tables()
{
    create5_19Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::create5_19Tables()
{
    createForcedOptionsTable(     _dbControl );
    createForcedOptionsGenerator( _dbControl );
	populateTableForcedOptions(   _dbControl );
	updateCourseIDsForcedOptions( _dbControl );
	updateItemIDsForcedOptions(   _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::createForcedOptionsTable( TDBControl* const inDBControl )
{
    std::string sql;

    if( !tableExists( "FORCEDOPTIONS", inDBControl ) )
    {
        // Create Table.
        // Many to Many relationship between Items and Options so that an item
        // can have as many Forced Options as needed ( currently the maximum is 32 )
        sql = "CREATE TABLE FORCEDOPTIONS ( "
              "  id     INTEGER NOT NULL PRIMARY KEY, "
              "  option INTEGER NOT NULL, "
              "  item   INTEGER NOT NULL, "
              "  FOREIGN KEY (option) REFERENCES OPTIONS(OPTIONS_KEY) ON DELETE CASCADE ON UPDATE CASCADE, "
              "  FOREIGN KEY (item)   REFERENCES ITEM(ITEM_KEY)       ON DELETE CASCADE ON UPDATE CASCADE "
              ");";
        executeQuery( sql, inDBControl );
    }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::

    if( !indexExists( "OPTION_ITEM", inDBControl ) )
    {
        // Create Indices.
        sql = "CREATE UNIQUE INDEX OPTION_ITEM ON FORCEDOPTIONS ( option, item );";
        executeQuery( sql, inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::createForcedOptionsGenerator( TDBControl* const inDBControl )
{
    if( !generatorExists( "GEN_FORCEDOPTIONS", inDBControl ) )
    {
        // Create Generator.
        std::string sql = createGeneratorQuery( "GEN_FORCEDOPTIONS" );
        executeQuery( sql, inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        // Set Generator to Zero.
        sql = setGeneratorToZeroQuery( "GEN_FORCEDOPTIONS" );
        executeQuery( sql, inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::populateTableForcedOptions( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::

    executeQuery( emptyTableQuery( "FORCEDOPTIONS" ), inDBControl );

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::

    std::vector<__int32> courses;
    readCourses( &transaction, courses );

	//.....................................

    std::vector<__int32>::iterator courseIT;

    for( courseIT = courses.begin(); courseIT < courses.end(); courseIT++ )
    {
		populateTableForcedOptionsWithCourse( *courseIT, &transaction );
    }
}
// ---------------------------------------------------------------------------
void TApplyParser::updateCourseIDsForcedOptions( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::

	AnsiString courseIdIndex = "";

	TIBSQL *selectQuery    = transaction.Query( transaction.AddQuery() );
	selectQuery->SQL->Text = "SELECT RDB$INDEX_NAME "
							 "FROM RDB$INDEX_SEGMENTS "
							 "WHERE RDB$FIELD_NAME = 'COURSE_ID';";

	transaction.StartTransaction();

	selectQuery->ExecQuery();

	if( !selectQuery->Eof )
	{
		courseIdIndex = selectQuery->FieldByName("RDB$INDEX_NAME")->AsString;

		executeQuery(
			inactiveIndexQuery( courseIdIndex.c_str() ),
			inDBControl ) ;
	}

	transaction.CommitTransaction();

	//.....................................................

	executeQuery( "UPDATE COURSE "
				  "SET COURSE_ID = COURSE_KEY",
				  inDBControl );

	//.....................................................

	if( courseIdIndex != "" )
	{
		executeQuery(
			activeIndexQuery( courseIdIndex.c_str() ),
			inDBControl ) ;
    }
}
// ---------------------------------------------------------------------------
void TApplyParser::updateItemIDsForcedOptions( TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::

	// Inactive Item_Index index in ItemSize table
	executeQuery(
		inactiveIndexQuery( "ITEM_INDEX" ),
		inDBControl ) ;

    //.....................................................

	TIBSQL *selectQuery    = transaction.Query( transaction.AddQuery() );
	selectQuery->SQL->Text = "SELECT ITEM_KEY, ITEM_ID "
							 "FROM ITEM ORDER BY ITEM_KEY;";

	transaction.StartTransaction();

	selectQuery->ExecQuery();

	for (;!selectQuery->Eof ; selectQuery->Next())
	{
		int itemKey = selectQuery->FieldByName("ITEM_KEY")->AsInteger;
		int itemID  = selectQuery->FieldByName("ITEM_ID")->AsInteger;

		//.....................................

		updateItemIDQuery( &transaction, "ARCHIVE",			   itemKey, itemID );
		updateItemIDQuery( &transaction, "DAYARCHIVE",		   itemKey, itemID );
		updateItemIDQuery( &transaction, "ITEM",			   itemKey, itemID );
		updateItemIDQuery( &transaction, "ITEMSIZE",		   itemKey, itemID );
		updateItemIDQuery( &transaction, "ORDERS",			   itemKey, itemID );
		updateItemIDQuery( &transaction, "HELDORDERITEMS", 	   itemKey, itemID );
		updateItemIDQuery( &transaction, "HELDORDERSIDEITEMS", itemKey, itemID );
	}

	transaction.CommitTransaction();

    //.....................................................

    // Active Item_Index index in ItemSize table
    executeQuery(
		activeIndexQuery( "ITEM_INDEX" ),
        inDBControl ) ;
}
// ---------------------------------------------------------------------------
void TApplyParser::updateItemIDQuery(
	TDBTransaction* const inTransaction,
	 		UnicodeString inTableName,
				  __int32 inItemKey,
			  	  __int32 inItemID )
{
	TIBSQL *updateQuery = inTransaction->Query( inTransaction->AddQuery() );

	updateQuery->SQL->Text = "UPDATE " + inTableName + " "
							 "SET ITEM_ID = :ITEM_KEY "
							 "WHERE ITEM_ID = :ITEM_ID;";

	updateQuery->ParamByName("ITEM_KEY")->AsInteger = inItemKey;
	updateQuery->ParamByName("ITEM_ID")->AsInteger  = inItemID;

    updateQuery->ExecQuery();

/*
    try
    {
        updateQuery->ExecQuery();
    }
    catch( Exception& exc )
    {
        UnicodeString text = "TABLE: "    + inTableName + " | " +
                             "Item Key: " + UnicodeString( SSTR( inItemKey ).c_str() ) + " | " +
                             "Item ID: "  + UnicodeString( SSTR( inItemID  ).c_str() ) + " | " +
                             exc.Message;
        _errorMsg.append( text.t_str() );
    }
*/
}
// ---------------------------------------------------------------------------
void TApplyParser::readCourses(
        TDBTransaction* const inTransaction,
		std::vector<__int32>& inCourses )
{
	inTransaction->StartTransaction();

	TIBSQL *query = inTransaction->Query( inTransaction->AddQuery() );
	query->SQL->Text = "SELECT COURSE_KEY as course FROM COURSE;";
	query->ExecQuery();

	//:::::::::::::::::::::::::::::::::

	inCourses.clear();
	if( query->RecordCount )
	{
		for( ; !query->Eof; query->Next( ) )
		{
			inCourses.push_back( query->FieldByName("course")->AsInteger );
		}
	}

	inTransaction->Commit();
}
// ---------------------------------------------------------------------------
void TApplyParser::populateTableForcedOptionsWithCourse(
                      __int32 inCourse,
		TDBTransaction* const inTransaction )
{
	std::vector<__int32> items;
	readItems( inTransaction, inCourse, items );

	//::::::::::::::::::::::::::::::::::::::::::

	std::vector<TOptionForcedMask> optionsForcedMasks;
	readOptionForcedMasks( inTransaction, inCourse, optionsForcedMasks );

	//::::::::::::::::::::::::::::::::::::::::::

	std::vector<TOptionForcedMask>::iterator ofmIT;

	for( ofmIT = optionsForcedMasks.begin(); ofmIT < optionsForcedMasks.end(); ofmIT++ )
	{
		addFieldForcedOptionItems( *ofmIT, items, inTransaction );
	}
}
// ---------------------------------------------------------------------------
void TApplyParser::readItems(
		TDBTransaction* const inTransaction,
                      __int32 inCourse,
        std::vector<__int32>& inItems )
{
	TIBSQL *query = inTransaction->Query( inTransaction->AddQuery() );
	query->SQL->Text = "SELECT ITEM.ITEM_KEY as item "
					"FROM COURSE INNER JOIN ITEM ON COURSE.COURSE_KEY = ITEM.COURSE_KEY "
					"WHERE COURSE.COURSE_KEY = :COURSE_KEY;";

	inTransaction->StartTransaction();

	query->ParamByName("COURSE_KEY")->AsInteger = inCourse;
	query->ExecQuery();

	//:::::::::::::::::::::::::::::::::

	inItems.clear();
	if( query->RecordCount )
	{
		for( ; !query->Eof; query->Next( ) )
		{
			inItems.push_back( query->FieldByName("item")->AsInteger );
		}
	}

	inTransaction->Commit();
}
// ---------------------------------------------------------------------------
void TApplyParser::readOptionForcedMasks(
        TDBTransaction* const inTransaction,
                      __int32 inCourse,
        std::vector<TOptionForcedMask>& inItems )
{
	TIBSQL *query    = inTransaction->Query( inTransaction->AddQuery() );
	query->SQL->Text = "SELECT OPTIONS.OPTIONS_KEY as optionKey, OPTIONS.FORCED_MASK as forcedMask "
					   "FROM COURSE INNER JOIN OPTIONS ON COURSE.COURSE_KEY = OPTIONS.COURSE_KEY "
					   "WHERE COURSE.COURSE_KEY = :COURSE_KEY;";

	inTransaction->StartTransaction();

	query->ParamByName("COURSE_KEY")->AsInteger = inCourse;
	query->ExecQuery();

	//:::::::::::::::::::::::::::::::::

	inItems.clear();
	if( query->RecordCount )
	{
		for( ; !query->Eof; query->Next( ) )
		{
			TOptionForcedMask *optionForcedMask = new TOptionForcedMask();

			optionForcedMask->optionKey  = query->FieldByName( "optionKey"  )->AsInteger;
			optionForcedMask->forcedMask = query->FieldByName( "forcedMask" )->AsInteger;

			inItems.push_back( *optionForcedMask );
		}
	}

	inTransaction->Commit();
}
// ---------------------------------------------------------------------------
void TApplyParser::addFieldForcedOptionItems(
            TOptionForcedMask inOptionForcedMask,
         std::vector<__int32> inItems,
        TDBTransaction* const inTransaction )
{
	std::vector<__int32>::iterator itemIT;

	int pos = 0;
	for( itemIT = inItems.begin(); itemIT < inItems.end(); itemIT++ )
	{
		unsigned int mask = 1 << pos++;
		if( ( inOptionForcedMask.forcedMask & mask ) == mask ) // this option is linked to the item at position pos
		{
			addFieldForcedOptionItem(
					inOptionForcedMask.optionKey,
					*itemIT,
					inTransaction );
		}
	}
}
// ---------------------------------------------------------------------------
void TApplyParser::addFieldForcedOptionItem(
                      __int32 inOption,
                      __int32 inItem,
		TDBTransaction* const inTransaction )
{
	UnicodeString sql;

	try
	{
		inTransaction->StartTransaction();

		TIBSQL *qrGenID    = inTransaction->Query( inTransaction->AddQuery() );

		qrGenID->SQL->Text = "SELECT GEN_ID(GEN_FORCEDOPTIONS, 1) FROM RDB$DATABASE";
		qrGenID->ExecQuery();

		int id = qrGenID->Fields[0]->AsInteger;

		//:::::::::::::::::::::::::::::::::::::::

		TIBSQL *query    = inTransaction->Query( inTransaction->AddQuery() );
		query->SQL->Text = "INSERT INTO FORCEDOPTIONS ( id, option, item ) "
						   "VALUES ( :id, :option, :item ); ";

		query->ParamByName("id"    )->AsInteger = id;
		query->ParamByName("option")->AsInteger = inOption;
		query->ParamByName("item"  )->AsInteger = inItem;

		sql = query->SQL->Text;

		query->ExecQuery();

		inTransaction->Commit();
	}
	catch( ... )
	{
        inTransaction->Rollback();
	}
}
// ---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.20.
void TApplyParser::upgrade5_20Tables()
{
    update5_20Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_20Tables()
{
    updatePointsTransactionTable5_20( _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::updatePointsTransactionTable5_20( TDBControl* const inDBControl )
{
	executeQuery(
        "UPDATE PointsTransactions SET Adjustment_Subtype = 1",
		inDBControl );

	executeQuery(
		"UPDATE PointsTransactions SET Adjustment_Type = 1 WHERE Adjustment_Type = 3",
        inDBControl );

	executeQuery(
        "UPDATE PointsTransactions SET Adjustment_Type = 2 WHERE Adjustment_Type = 4",
        inDBControl );

	executeQuery(
		"UPDATE PointsTransactions SET Adjustment_Type = 3 WHERE Adjustment_Type = 5",
        inDBControl );

	executeQuery(
		"UPDATE PointsTransactions SET Adjustment_Type = 4 WHERE Adjustment_Type = 6",
        inDBControl );

	executeQuery(
		"UPDATE PointsTransactions SET Adjustment_Type = 5 WHERE Adjustment_Type = 7",
		inDBControl );

	executeQuery(
		"UPDATE PointsTransactions SET Adjustment_Type = 1, Adjustment_Subtype = 2  WHERE Adjustment_Type = 8",
		inDBControl );

	syncPointsAdjustments5_20( inDBControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::syncPointsAdjustments5_20( TDBControl* const inDBControl )
{
	std::vector<__int32> contactVector;
	std::vector<__int32>::const_iterator cvIT;

	readContacts( contactVector, inDBControl );

	for( cvIT = contactVector.begin(); cvIT != contactVector.end(); cvIT++ )
	{
		__int32 contactKey = *cvIT;

		syncPointsAdjustment5_20( contactKey, inDBControl );
        fixEncryptionDataForHeldTransactions5_20( contactKey, inDBControl );
	}
}
// ---------------------------------------------------------------------------
void TApplyParser::syncPointsAdjustment5_20(
                              __int32 inContactKey,
                    TDBControl* const inDBControl )
{
	Currency points             = getContactPoints(            inContactKey, inDBControl );
	Currency transactionsPoints = getContactTransactionPoints( inContactKey, inDBControl );
	Currency difference         = points - transactionsPoints;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	if( difference != 0 )
	{
		adjustContactTransactionPoints( inContactKey, difference, inDBControl );
	}
}
// ---------------------------------------------------------------------------
void TApplyParser::fixEncryptionDataForHeldTransactions5_20(
                                              __int32 inContactKey,
                                    TDBControl* const inDBControl )
{
   /*
    this method will retrieve held transactions for the contact and will re-encrypt
    the misc field. this is needed as in this 5.20 parser we alter the adjustment types
    from what we had before. if the adjustment type is changed it will no longer valid with
    encrypted binary data which is stored.
   */

   std::vector<TPointsTransaction> validHeldPointsForContact = getHeldPointsTransactions(
                                                                                inContactKey,
                                                                                inDBControl);

    if(validHeldPointsForContact.size() > 0)
    {
        reEncryptMiscDataForPointsTransactions( inDBControl, validHeldPointsForContact );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.21.
void TApplyParser::upgrade5_21Tables()
{
    update5_21Tables();
}
//---------------------------------------------------------------------------
void
TApplyParser::update5_21Tables()
{
    update5_21Tables( _dbControl );
}
// ---------------------------------------------------------------------------
void
TApplyParser::update5_21Tables(
		TDBControl* const inDBControl )
{
	create_new_fields_if_not_present(inDBControl);
}
// ---------------------------------------------------------------------------
void
TApplyParser::create_new_fields_if_not_present(
		TDBControl* const inDBControl)
{
    if( !are_fields_present_5_21( inDBControl ) )
    {
		std::string inSQL = "alter table itemsize "
									 "    add available_quantity numeric(15, 4) "
									 "        default -1.0 not null, "
									 "    add default_quantity numeric(15, 4) "
									 "        default -1.0 not null, "
									 "    add warning_quantity numeric(15, 4) "
									 "        default 0.0 not null, "
									 "    add disable_when_count_reaches_zero "
									 "        smallint default 1 not null;";

		executeQuery(
				inSQL,
				inDBControl );
    }
}
// ---------------------------------------------------------------------------

using SqlPredicates::TFieldExistencePredicate;

// It uses predicates
bool
TApplyParser::are_fields_present_5_21( TDBControl* const inDBControl )
{
	TDBUtilities utilities(*inDBControl);
	std::vector<UnicodeString> fields_to_check_for;

	fields_to_check_for.push_back("available_quantity");
	fields_to_check_for.push_back("default_quantity");
	fields_to_check_for.push_back("warning_quantity");
	fields_to_check_for.push_back("disable_when_count_reaches_zero");

	return TFieldExistencePredicate("itemsize",
					fields_to_check_for,
					utilities)();
}
// ---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.22.
void TApplyParser::upgrade5_22Tables()
{
    update5_22Tables();
}
//---------------------------------------------------------------------------
void
TApplyParser::update5_22Tables()
{
    updateItemSizeTable5_22( _dbControl );
}
// ---------------------------------------------------------------------------
void
TApplyParser::updateItemSizeTable5_22(
		            TDBControl* const inDBControl)
{
    insert_new_fields(inDBControl);
}
// ---------------------------------------------------------------------------
void
TApplyParser::insert_new_fields(
		        TDBControl* const inDBControl)
{
    if( !are_fields_present_5_22( inDBControl ) )
    {
		std::string inSQL = "alter table itemsize "
                               "      add can_be_paid_for_using_points "
                               "          smallint "
                               "          default 1 "
                               "          not null;";

		executeQuery(
				inSQL,
				inDBControl );
    }
}
// ---------------------------------------------------------------------------

// It uses predicates
bool
TApplyParser::are_fields_present_5_22(
		        TDBControl* const inDBControl)
{
    std::vector<UnicodeString> fields;
    TDBUtilities utilities(*inDBControl);

    fields.push_back("can_be_paid_for_using_points");
    return TFieldExistencePredicate("itemsize", fields, utilities)();
}
// ---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.23.
#define _5_23TABLE_NAME "DISCOUNTCATEGORIES"
#define _5_23GEN_NAME  	"GEN_DISCOUNT_CATEGORIES"

void TApplyParser::upgrade5_23Tables()
{
    create5_23Tables();
    update5_23Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::create5_23Tables()
{
    create5_23DiscountCategoriesTable(     _dbControl );
    create5_23DiscountCategoriesGenerator( _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::update5_23Tables()
{
    update5_23DiscountsTable( _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::create5_23DiscountCategoriesTable( TDBControl* const inDBControl )
{
    std::string sql;

    if( !tableExists( _5_23TABLE_NAME, inDBControl ) )
    {
        // Create Table.
        // Many to Many relationship between Items and Options so that an item
        // can have as many Forced Options as needed ( currently the maximum is 32 )
        sql = "CREATE TABLE " _5_23TABLE_NAME " ( "
              "  "_5_23TABLE_NAME"_KEY     INTEGER NOT NULL PRIMARY KEY, "
              "  DISCOUNT_KEY INTEGER NOT NULL, "
              "  CATEGORY_KEY   INTEGER NOT NULL, "
              "  FOREIGN KEY (DISCOUNT_KEY) REFERENCES DISCOUNTS(DISCOUNT_KEY) ON DELETE CASCADE ON UPDATE CASCADE, "
              "  FOREIGN KEY (CATEGORY_KEY)   REFERENCES ARCCATEGORIES(CATEGORY_KEY) ON DELETE CASCADE ON UPDATE CASCADE "
              ");";
        executeQuery( sql, inDBControl );
    }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::

    if( !indexExists( "DISCOUNT_CATEGORY", inDBControl ) )
    {
        // Create Indices.
        sql = "CREATE UNIQUE INDEX DISCOUNT_CATEGORY ON "_5_23TABLE_NAME" ( DISCOUNT_KEY, CATEGORY_KEY );";
        executeQuery( sql, inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create5_23DiscountCategoriesGenerator( TDBControl* const inDBControl )
{
    if( !generatorExists( _5_23GEN_NAME, inDBControl ) )
    {
        // Create Generator.
        std::string sql = createGeneratorQuery( _5_23GEN_NAME );
        executeQuery( sql, inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        // Set Generator to Zero.
		sql = setGeneratorToZeroQuery( _5_23GEN_NAME );
        executeQuery( sql, inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::update5_23DiscountsTable( TDBControl* const inDBControl )
{
	if( !fieldExists( "DISCOUNTS", "DISCOUNT_GROUP", _dbControl ) )
	{
	    executeQuery(
			"ALTER TABLE DISCOUNTS ADD DISCOUNT_GROUP INTEGER DEFAULT 0",
		    inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.24.
void TApplyParser::upgrade5_24Tables()
{
	update5_24Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_24Tables()
{
	dropAndCreateForcedOptionsConstraints( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::dropAndCreateForcedOptionsConstraints( TDBControl* const inDBControl )
{
    executeQuery(
		"delete from rdb$relation_constraints "
        "where RDB$RELATION_NAME='FORCEDOPTIONS' and RDB$CONSTRAINT_TYPE ='FOREIGN KEY'",
		inDBControl );

    executeQuery(
		"alter table FORCEDOPTIONS add constraint FK_ITEM_KEY foreign key (ITEM) references ITEM(Item_Key) on update cascade on delete cascade",
		inDBControl );

	executeQuery(
		"alter table FORCEDOPTIONS add constraint FK_OPTION_KEY foreign key (OPTION) references Options(Options_Key) on update cascade on delete cascade",
		inDBControl );
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.25.
void TApplyParser::upgrade5_25Tables()
{
	create5_25Tables();
	create5_25Procedures();
	create5_25Views();
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25Tables()
{
	create5_25RoundingTimesTable(   _dbControl );
	populate5_25RoundingTimesTable( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25Procedures()
{
	create5_25RoundTimeProcedure( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25Views()
{
	create5_25RoundedContactTimeView( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25RoundingTimesTable( TDBControl* const inDBControl )
{
	if ( !tableExists( "ROUNDINGTIMES", _dbControl ) )
	{
		executeQuery(
			"CREATE TABLE ROUNDINGTIMES "
			"( "
			  "ROUNDINGTIMES_KEY   INTEGER NOT NULL, "
			  "ROUNDINGTIMES_VALUE INTEGER NOT NULL, "
			  "PRIMARY KEY (ROUNDINGTIMES_KEY), "
			  "UNIQUE (ROUNDINGTIMES_VALUE) "
			")",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::populate5_25RoundingTimesTable( TDBControl* const inDBControl )
{
	executeQuery("DELETE FROM ROUNDINGTIMES", inDBControl );
	int roundingTimes [7] = {0, 1, 5, 10, 15, 30, 60};
	for (int i = 0; i < 7; i++)
	{
		std::string key   = SSTR( i );
		std::string value = SSTR( roundingTimes[i] );
		executeQuery(
			"INSERT INTO ROUNDINGTIMES "
			"("
				"ROUNDINGTIMES_KEY,"
				"ROUNDINGTIMES_VALUE"
			")"
			" VALUES "
			"("
				"" + key + ","
				"" + value + ""
			")",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25RoundTimeProcedure( TDBControl* const inDBControl )
{
	if ( !procedureExists( "ROUNDTIME", _dbControl ) )
	{
		std::string profileKey = "(SELECT PROFILE_KEY FROM PROFILE WHERE PROFILE_TYPE = 4 AND NAME = 'Globals')";
		std::string roundingTimesKey = "(SELECT INTEGER_VAL FROM VARSPROFILE WHERE VARIABLES_KEY = 4072 AND PROFILE_KEY = " + profileKey + " )";

		executeQuery(
			"CREATE PROCEDURE ROUNDTIME ( inUnroundedTime Timestamp ) "
			"RETURNS ( outRoundedTime Timestamp ) "
			"AS "
				"DECLARE VARIABLE timeStampMinutes INT; "
				"DECLARE VARIABLE timeStampHours INT; "
				"DECLARE VARIABLE timeString CHAR(8); "
				"DECLARE VARIABLE minutesToRoundTo INT; "
			"BEGIN "
				"SELECT ROUNDINGTIMES_VALUE FROM ROUNDINGTIMES WHERE ROUNDINGTIMES_KEY = " + roundingTimesKey + " INTO minutesToRoundTo; "
				"IF ( minutesToRoundTo = 0 ) THEN "
					"outRoundedTime = InUnroundedTime; "
				"ELSE "
				"BEGIN "
					"timeStampMinutes = EXTRACT ( MINUTE FROM InUnroundedTime ); "
					"timeStampMinutes = ROUND ( CAST ( timeStampMinutes as DECIMAL(18,4) ) / CAST ( minutesToRoundTo as DECIMAL(18,4) ) ) * minutesToRoundTo; "
					"timeStampHours = EXTRACT ( HOUR FROM InUnroundedTime ); "
					"IF ( timeStampMinutes >= 60 ) THEN "
					"BEGIN "
						"timeStampMinutes = MOD ( timeStampMinutes, 60 ); "
						"timeStampHours = timeStampHours + 1; "
					"END "
					"timeString = LPAD ( TRIM ( CAST ( timeStampHours as CHAR(2) ) ), 2, '0' ) || ':' || LPAD ( TRIM ( CAST ( timeStampMinutes as CHAR(2) ) ), 2, '0' ) || ':00'; "
					"outRoundedTime = DATEADD(SECOND, DATEDIFF(SECOND, CAST ( InUnroundedTime AS TIME ), CAST ( timeString as TIME ) ), InUnroundedTime); "
				"END "
				"SUSPEND; "
			"END ",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create5_25RoundedContactTimeView( TDBControl* const inDBControl )
{
	if ( !tableExists( "ROUNDEDCONTACTTIME", _dbControl ) )
	{
		executeQuery(
			"CREATE VIEW ROUNDEDCONTACTTIME "
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
				"BREAKS "
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
				"BREAKS "
			"FROM "
				"CONTACTTIME",
			inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.26.
void TApplyParser::upgrade5_26Tables()
{
	update5_26Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_26Tables()
{
	updateOrdersTable5_26( _dbControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::updateOrdersTable5_26( TDBControl* const inDBControl )
{
	if( !fieldExists( "ITEMSIZE", "DEFAULT_PATRON_COUNT", _dbControl ) )
	{
		executeQuery(
			"ALTER TABLE ITEMSIZE ADD DEFAULT_PATRON_COUNT INTEGER DEFAULT 0 NOT NULL",
			inDBControl );
	}

	if( !fieldExists( "ORDERS", "PATRON_COUNT", _dbControl ) )
	{
		executeQuery(
			"ALTER TABLE ORDERS ADD PATRON_COUNT INTEGER DEFAULT 0 NOT NULL",
			inDBControl );
	}
}
//---------------------------------------------------------------------------


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.27.
void TApplyParser::upgrade5_27Tables()
{
	update5_27Tables();
}

void TApplyParser::update5_27Tables()
{
    create5_27Generators( _dbControl );
	create5_27Tables( _dbControl );
	alterColumnsForArchiveTables5_27( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_27Generators( TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_LOYALTYATTRIBUTES", _dbControl) )
    {
        executeQuery(
		    "CREATE GENERATOR GEN_LOYALTYATTRIBUTES",
            inDBControl);

        executeQuery(
            "SET GENERATOR GEN_LOYALTYATTRIBUTES TO 0",
		    inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create5_27Tables( TDBControl* const inDBControl )
{
    if ( !tableExists( "LOYALTYATTRIBUTES", _dbControl ) )
	{
        executeQuery(
		"create table LOYALTYATTRIBUTES"
        "("
        "   ATTRIB_KEY int primary key,"
        "   UUID varchar(60) unique,"
        "   CONTACTS_KEY int,"
        "   SITE_ID int"
        ")",
		inDBControl );
    }
}

void TApplyParser::alterColumnsForArchiveTables5_27( TDBControl* const inDBControl )
{
	if(!fieldExists("DAYARCBILLPAY", "PAYMENT_CARD_TYPE", _dbControl))
	{
		executeQuery(
			"alter table DAYARCBILLPAY add PAYMENT_CARD_TYPE varchar(20) default ''",
			inDBControl
		);
	}

	if(!fieldExists("ARCBILLPAY", "PAYMENT_CARD_TYPE", _dbControl))
	{
		executeQuery(
			"alter table ARCBILLPAY add PAYMENT_CARD_TYPE varchar(20) default ''",
			inDBControl
		);
	}
}
//---------------------------------------------------------------------------
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//  5.28.
void TApplyParser::upgrade5_28Tables()
{
	update5_28Tables(_dbControl);
}

void TApplyParser::update5_28Tables(TDBControl* const inDBControl)
{
	if(!fieldExists("ORDERS", "ITEM_HANDHELD_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add ITEM_HANDHELD_NAME varchar(50) default ''",
			inDBControl
		);
	}

	if(!fieldExists("ORDERS", "ITEM_RECEIPT_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add ITEM_RECEIPT_NAME varchar(50) default ''",
			inDBControl
		);
	}

	if(!fieldExists("ORDERS", "SIZE_HANDHELD_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add SIZE_HANDHELD_NAME varchar(50) default ''",
			inDBControl
		);
	}

	if(!fieldExists("ORDERS", "SIZE_RECEIPT_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add SIZE_RECEIPT_NAME varchar(50) default ''",
			inDBControl
		);
	}

	if(!fieldExists("ORDERS", "COURSE_HANDHELD_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add COURSE_HANDHELD_NAME varchar(50) default ''",
			inDBControl
		);
	}

    if(!fieldExists("ORDERS", "COURSE_RECEIPT_NAME", _dbControl))
	{
		executeQuery(
			"alter table ORDERS add COURSE_RECEIPT_NAME varchar(50) default ''",
			inDBControl
		);
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.29.
void TApplyParser::upgrade5_29Tables()
{
	update5_29Tables();
}

void TApplyParser::update5_29Tables()
{
    create5_29Generators( _dbControl );
	create5_29Tables( _dbControl );
    alterLoyaltyAttributesTable( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_29Generators( TDBControl* const inDBControl)
{
    if( !generatorExists("GEN_LOYALTYPRINTING", _dbControl) )
    {
        executeQuery(
		    "CREATE GENERATOR GEN_LOYALTYPRINTING",
            inDBControl);

        executeQuery(
            "SET GENERATOR GEN_LOYALTYPRINTING TO 0",
		    inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create5_29Tables( TDBControl* const inDBControl )
{
    if ( !tableExists( "LOYALTYPRINTING", _dbControl ) )
	{
        executeQuery(
		"create table LOYALTYPRINTING"
        "("
            " LOYALTYPRINTING_KEY INTEGER NOT NULL PRIMARY KEY,"
			" PHYSICALPRINTER_KEY INTEGER NOT NULL,"
            " DISPLAY_ORDER INTEGER,"
            " INSTRUCTION INTEGER,"
            " CAPTION VARCHAR(45),"
            " LINE_ABOVE T_TRUEFALSE,"
            " LINE_BELOW T_TRUEFALSE,"
            " DRAW_LINE_ABOVE T_TRUEFALSE,"
            " DRAW_LINE_BELOW T_TRUEFALSE,"
            " CUT T_TRUEFALSE,"
            " VISIBLE T_TRUEFALSE,"
            " PRINT_UNDERLINED T_TRUEFALSE,"
            " PRINT_BOLD T_TRUEFALSE,"
            " PRINT_DOUBLE_WIDTH T_TRUEFALSE,"
            " PRINT_DOUBLE_HEIGHT T_TRUEFALSE,"
            " PRINT_COLOUR INTEGER,"
            " PRINT_FONT INTEGER,"
            " GROUP_NUMBER INTEGER,"
            " CONSTRAINT LM_PHYSICALPRINTER_KEY FOREIGN KEY (PHYSICALPRINTER_KEY) REFERENCES PHYSICALPRINTER (PHYSICALPRINTER_KEY) ON UPDATE CASCADE ON DELETE CASCADE"
        ")",
		inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::alterLoyaltyAttributesTable( TDBControl* const inDBControl )
{
	if(!fieldExists("LOYALTYATTRIBUTES", "ACTIVATION_TOKEN", _dbControl))
	{
		executeQuery(
			"alter table LOYALTYATTRIBUTES add ACTIVATION_TOKEN varchar(20)",
			inDBControl
		);
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.30.
void TApplyParser::upgrade5_30Tables()
{
	update5_30Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_30Tables()
{
    alterRoundTimeProcedure( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::alterRoundTimeProcedure( TDBControl* const inDBControl )
{
	if(procedureExists("ROUNDTIME", _dbControl))
	{
		executeQuery(
            "ALTER PROCEDURE ROUNDTIME (INUNROUNDEDTIME Timestamp ) RETURNS (OUTROUNDEDTIME Timestamp ) "
            "AS "
            "   DECLARE VARIABLE timeStampMinutes INT; "
            "   DECLARE VARIABLE timeStampHours INT; "
            "   DECLARE VARIABLE timeString CHAR(8); "
            "   DECLARE VARIABLE minutesToRoundTo INT; "
            "BEGIN "
            "   SELECT "
            "       ROUNDINGTIMES_VALUE "
            "   FROM "
            "       ROUNDINGTIMES "
            "   WHERE "
            "       ROUNDINGTIMES_KEY = (SELECT INTEGER_VAL FROM VARSPROFILE WHERE VARIABLES_KEY = 4073 "   // Hard coded the variables key according to ManagerVariable.h enum vmVariables
            "       AND PROFILE_KEY = (SELECT PROFILE_KEY FROM PROFILE WHERE PROFILE_TYPE = 4 AND NAME = 'Globals') ) "
            "   INTO minutesToRoundTo; "
            "   IF ( minutesToRoundTo = 0 ) THEN "
            "       outRoundedTime = InUnroundedTime; "
            "   ELSE "
            "       BEGIN "
            "           timeStampMinutes = EXTRACT ( MINUTE FROM InUnroundedTime ); "
            "           timeStampMinutes = ROUND ( CAST ( timeStampMinutes as DECIMAL(18,4) ) / CAST ( minutesToRoundTo as DECIMAL(18,4) ) ) * minutesToRoundTo; "
            "           timeStampHours = EXTRACT ( HOUR FROM InUnroundedTime ); "
            "           IF ( timeStampMinutes >= 60 ) THEN "
            "               BEGIN "
            "                   timeStampMinutes = MOD ( timeStampMinutes, 60 ); "
            "                   timeStampHours = timeStampHours + 1; "
            "               END "
            "           timeString = LPAD ( TRIM ( CAST ( timeStampHours as CHAR(2) ) ), 2, '0' ) || ':' || LPAD ( TRIM ( CAST ( timeStampMinutes as CHAR(2) ) ), 2, '0' ) || ':00'; "
            "           outRoundedTime = DATEADD(SECOND, DATEDIFF(SECOND, CAST ( InUnroundedTime AS TIME ), CAST ( timeString as TIME ) ), InUnroundedTime); "
            "       END SUSPEND; "
            "END; ",
            inDBControl
        );

        executeQuery(
            "GRANT EXECUTE ON PROCEDURE ROUNDTIME TO  SYSDBA;",
			inDBControl
		);
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.31.
void TApplyParser::upgrade5_31Tables()
{
	update5_31Tables();
}



void TApplyParser::update5_31Tables()
{
	create5_31Generators( _dbControl );
	create5_31Tables( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_31Generators( TDBControl* const inDBControl)
{
	if( !generatorExists("GEN_DEAL", _dbControl) )
	{
		executeQuery(
			"CREATE GENERATOR GEN_DEAL",
			inDBControl);

		executeQuery(
			"SET GENERATOR GEN_DEAL TO 0",
			inDBControl );
	}

	if( !generatorExists("GEN_DEALLEVEL", _dbControl) )
	{
		executeQuery(
			"CREATE GENERATOR GEN_DEALLEVEL",
			inDBControl);

		executeQuery(
			"SET GENERATOR GEN_DEALLEVEL TO 0",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create5_31Tables( TDBControl* const inDBControl )
{
	if ( !tableExists( "DEAL", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DEAL"
		"("
			"DEAL_KEY INTEGER, "
			"CATEGORY_KEY INTEGER, "
			"DEAL_NAME VARCHAR(20), "
			"PRIORITY INTEGER, "
			"PRIMARY KEY (DEAL_KEY), "
			"FOREIGN KEY (CATEGORY_KEY) "
				"REFERENCES ARCCATEGORIES(CATEGORY_KEY) "
				"ON UPDATE CASCADE ON DELETE CASCADE"
		");",
		inDBControl );
	}

	if ( !tableExists( "DEALLEVEL", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DEALLEVEL"
		"("
			"DEALLEVEL_KEY INTEGER, "
			"DEAL_KEY INTEGER, "
			"QTY NUMERIC(15,4), "
			"PRICE NUMERIC(15,4), "
			"PRIMARY KEY (DEALLEVEL_KEY), "
			"FOREIGN KEY (DEAL_KEY) "
				"REFERENCES DEAL(DEAL_KEY) "
				"ON UPDATE CASCADE ON DELETE CASCADE"
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.32.
void TApplyParser::upgrade5_32Tables()
{
	update5_32Tables();
}



void TApplyParser::update5_32Tables()
{
	create5_32Generators( _dbControl );
	create5_32Tables( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_32Generators( TDBControl* const inDBControl)
{
	if( !generatorExists("GEN_LOYALTYPENDINGTRANSACTIONS", _dbControl) )
	{
		executeQuery(
			"CREATE GENERATOR GEN_LOYALTYPENDINGTRANSACTIONS;",
			inDBControl);

		executeQuery(
			"SET GENERATOR GEN_LOYALTYPENDINGTRANSACTIONS TO 0;",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create5_32Tables( TDBControl* const inDBControl )
{
	if ( !tableExists( "LOYALTYPENDINGTRANSACTIONS", _dbControl ) )
	{
		executeQuery(
		    "CREATE TABLE LOYALTYPENDINGTRANSACTIONS "
			"("
			"   TRANSACTION_NUMBER int primary key,"
			//"   UUID varchar(60),"
            "   CONTACT_KEY int,"
			"   SYNDICATE_CODE varchar(60),"
            "   EARNED_POINTS Numeric(15,4),"
			"   EARNED_POINTS_DELTA Numeric(15,4),"
			"   LOADED_POINTS_DELTA Numeric(15,4),"
			"   LOADED_POINTS Numeric(15,4),"
			"   OCCURRED_DATE Timestamp"
            ");",
			inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.33.
void TApplyParser::upgrade5_33Tables()
{
	update5_33Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_33Tables()
{
	create5_33Generators( _dbControl );
	create5_33Tables( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::create5_33Generators( TDBControl* const inDBControl)
{
	if( !generatorExists("GEN_HOTELCUSTOMER", _dbControl) )
	{
		executeQuery(
			"CREATE GENERATOR GEN_HOTELCUSTOMER;",
			inDBControl);

		executeQuery(
			"SET GENERATOR GEN_HOTELCUSTOMER TO 0;",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create5_33Tables( TDBControl* const inDBControl )
{
	if ( !tableExists( "HOTELCUSTOMER", _dbControl ) )
	{
		executeQuery(
			"CREATE TABLE HOTELCUSTOMER "
			"("
			"   HOTELCUSTOMER_KEY int primary key,"
			"   CUSTOMER_NAME varchar(60),"
			"   ROOM_NUMBER int,"
			"   INVOICE_KEY int"
			");",
			inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.34.
void TApplyParser::upgrade5_34Tables()
{
	alter5_34Procedures();
}
//---------------------------------------------------------------------------
void TApplyParser::alter5_34Procedures()
{
	alter5_34RoundTimeProcedure( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::alter5_34RoundTimeProcedure( TDBControl* const inDBControl )
{
	if ( procedureExists( "ROUNDTIME", _dbControl ) )
	{
		std::string profileKey = "(SELECT PROFILE_KEY FROM PROFILE WHERE PROFILE_TYPE = 4 AND NAME = 'Globals')";
		std::string roundingTimesKey = "(SELECT INTEGER_VAL FROM VARSPROFILE WHERE VARIABLES_KEY = 4073 AND PROFILE_KEY = " + profileKey + " )";

		executeQuery(
			"ALTER PROCEDURE ROUNDTIME ( inUnroundedTime Timestamp ) "
			"RETURNS ( outRoundedTime Timestamp ) "
			"AS "
				"DECLARE VARIABLE timeStampMinutes INT; "
				"DECLARE VARIABLE timeStampHours INT; "
				"DECLARE VARIABLE timeString CHAR(8); "
				"DECLARE VARIABLE minutesToRoundTo INT; "
    			"DECLARE VARIABLE increaseDate INT; "
			"BEGIN "
				"SELECT ROUNDINGTIMES_VALUE FROM ROUNDINGTIMES WHERE ROUNDINGTIMES_KEY = " + roundingTimesKey + " INTO minutesToRoundTo; "
				"IF ( minutesToRoundTo = 0 ) THEN "
					"outRoundedTime = InUnroundedTime; "
				"ELSE "
				"BEGIN "
					"timeStampMinutes = EXTRACT ( MINUTE FROM InUnroundedTime ); "
					"timeStampMinutes = ROUND ( CAST ( timeStampMinutes as DECIMAL(18,4) ) / CAST ( minutesToRoundTo as DECIMAL(18,4) ) ) * minutesToRoundTo; "
					"timeStampHours = EXTRACT ( HOUR FROM InUnroundedTime ); "
					"increaseDate = 0; "
					"IF ( timeStampMinutes >= 60 ) THEN "
					"BEGIN "
						"timeStampMinutes = MOD ( timeStampMinutes, 60 ); "
						"timeStampHours = timeStampHours + 1; "
						"IF ( timeStampHours >= 24 ) THEN "
						"BEGIN "
							"timeStampHours = MOD ( timeStampHours, 24 ); "
							"increaseDate = 1; "
						"END "
					"END "
					"timeString = LPAD ( TRIM ( CAST ( timeStampHours as CHAR(2) ) ), 2, '0' ) || ':' || LPAD ( TRIM ( CAST ( timeStampMinutes as CHAR(2) ) ), 2, '0' ) || ':00'; "
					"outRoundedTime = DATEADD(SECOND, DATEDIFF(SECOND, CAST ( InUnroundedTime AS TIME ), CAST ( timeString as TIME ) ), InUnroundedTime); "
					"IF ( increaseDate = 1 ) THEN "
						"outRoundedTime = DATEADD(DAY, 1, outRoundedTime); "
				"END "
				"SUSPEND; "
			"END ",
			inDBControl );
	}
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.35.
void TApplyParser::upgrade5_35Tables()
{
    update5_35Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_35Tables()
{
	clearLoyaltyPendingTransactions5_35( _dbControl );
	updateLoyaltyPendingTransactions5_35( _dbControl );
	updateLoyaltyAttributes5_35( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::clearLoyaltyPendingTransactions5_35( TDBControl* const inDBControl )
{
	if ( tableExists( "LOYALTYPENDINGTRANSACTIONS", _dbControl ) )
	{
		executeQuery(
		"DELETE FROM LOYALTYPENDINGTRANSACTIONS;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateLoyaltyPendingTransactions5_35( TDBControl* const inDBControl )
{
	if ( fieldExists( "LOYALTYPENDINGTRANSACTIONS", "SYNDICATE_CODE", _dbControl ) )
	{
		executeQuery(
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"DROP SYNDICATE_CODE, "
		"ADD SYNDCODES_KEY INTEGER, "
		"ALTER COLUMN SYNDCODES_KEY POSITION 3, "
		"ADD CONSTRAINT FK_SYNDCODES_KEY FOREIGN KEY (SYNDCODES_KEY) REFERENCES SYNDCODES(SYNDCODES_KEY) ON DELETE CASCADE;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateLoyaltyAttributes5_35( TDBControl* const inDBControl )
{
	if ( fieldExists( "LOYALTYATTRIBUTES", "CONTACTS_KEY", _dbControl ) )
	{
		executeQuery(
		"ALTER TABLE LOYALTYATTRIBUTES "
		"ADD CONSTRAINT FK_CONTACTS_KEY FOREIGN KEY (CONTACTS_KEY) REFERENCES CONTACTS(CONTACTS_KEY) ON UPDATE CASCADE ON DELETE CASCADE;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.36.
void TApplyParser::upgrade5_36Tables()
{
    update5_36Tables();
    update5_36Indexs();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_36Tables()
{
	addColumnsToLoyaltyAttributes5_36( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::update5_36Indexs()
{
    addIndexToItemSizePLU5_36( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::addColumnsToLoyaltyAttributes5_36( TDBControl* const inDBControl )
{
	if ( !fieldExists( "LOYALTYATTRIBUTES", "CREATION_TIME", _dbControl ) )
	{
		executeQuery(
		"ALTER TABLE LOYALTYATTRIBUTES "
		"ADD CREATION_TIME timestamp default current_timestamp",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::addIndexToItemSizePLU5_36( TDBControl* const inDBControl )
{
    if ( !indexExists( "PLU_INDEX", _dbControl ) )
    {
        executeQuery(
        "CREATE INDEX PLU_INDEX ON ITEMSIZE (PLU);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.37.
void TApplyParser::upgrade5_37Tables()
{
    update5_37Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_37Tables()
{
	modifyContactFreebieColumns5_37( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::modifyContactFreebieColumns5_37( TDBControl* const inDBControl )
{
	if ( fieldExists( "CONTACTFREEBIE", "ITEM_COUNT", _dbControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTFREEBIE "
		"alter column ITEM_COUNT type numeric(15,4)",
		inDBControl );
	}
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  5.38.
void TApplyParser::upgrade5_38Tables()
{
    update5_38Tables();
}
//---------------------------------------------------------------------------
void TApplyParser::update5_38Tables()
{
	modifyContactsColumns5_38( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::modifyContactsColumns5_38( TDBControl* const inDBControl )
{
	if ( fieldExists( "CONTACTS", "ACCESS_LEVEL", _dbControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTS "
		"ALTER COLUMN ACCESS_LEVEL TYPE BIGINT",
		inDBControl );
	}
}

//---------------------------------------------------------------------------

}
