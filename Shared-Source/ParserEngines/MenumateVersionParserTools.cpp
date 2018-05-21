//---------------------------------------------------------------------------

#pragma hdrstop

#include "MenumateVersionParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace MenumateVersionParser
{
// Tool Queries
std::string TApplyParser::emptyTableQuery( std::string inName )
{
	return "DELETE FROM "  + inName + ";";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherATableExistsQuery()
{
	return "SELECT * FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherAGeneratorExistsQuery()
{
	return "SELECT * FROM RDB$GENERATORS WHERE RDB$GENERATOR_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherAnIndexExistsQuery()
{
	return "SELECT * FROM RDB$INDICES WHERE RDB$INDEX_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherAConstraintExistsQuery()
{
	return "SELECT * FROM RDB$RELATION_CONSTRAINTS WHERE RDB$CONSTRAINT_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherAFieldExistsQuery()
{
	return "SELECT * FROM RDB$RELATION_FIELDS WHERE RDB$RELATION_NAME = :name_one AND RDB$FIELD_NAME = :name_two;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherAProcedureExistsQuery()
{
	return "SELECT * FROM RDB$PROCEDURES WHERE RDB$PROCEDURE_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherATriggerExistsQuery()
{
	return "SELECT * FROM RDB$TRIGGERS WHERE RDB$TRIGGER_NAME = :name;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::activeIndexQuery( std::string inIndexName )
{
	return "ALTER INDEX " +  inIndexName + " ACTIVE;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::inactiveIndexQuery( std::string inIndexName )
{
	return "ALTER INDEX " +  inIndexName + " INACTIVE;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::versionNumberQuery()
{
	return "SELECT version_number "
           "FROM   VersionHistory "
           "WHERE  version_number = :versionNumber;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::insertVersionNumberQuery( std::string inVersion )
{
    return "INSERT INTO VersionHistory "
           "VALUES( '" + inVersion + "', current_timestamp, 'Upgrade' );";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::updateVersionNumberQuery( std::string inVersion )
{
    return "UPDATE VersionHistory "
           "SET Time_Stamp = current_timestamp "
           "WHERE Version_Number = '" + inVersion + "';";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::createGeneratorQuery( std::string inName )
{
	return "CREATE GENERATOR "  + inName + ";";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::setGeneratorToZeroQuery( std::string inName )
{
	return "SET GENERATOR "  + inName + " TO 0;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::contactPointsQuery( __int32 inContactKey )
{
    // total_spendt field holds the total amount of points a Contact has!.
	AnsiString result = "SELECT total_spent as points "
                        "FROM   Contacts "
                        "WHERE  contacts_key = " + IntToStr( inContactKey ) + ";";
    return result.c_str();
}
// ---------------------------------------------------------------------------
std::string TApplyParser::contactTransactionPointsQuery( __int32 inContactKey )
{
    // adjustment field holds the amount of points a Contact gets from each transaction!.
	AnsiString result = "SELECT Sum( adjustment ) as points "
                        "FROM   PointsTransactions "
                        "WHERE  contacts_key = " + IntToStr( inContactKey ) + ";";
    return result.c_str();
}
// ---------------------------------------------------------------------------
std::string TApplyParser::adjustContactTransactionPointsQuery(
                                __int32 inKey,
                                __int32 inContactKey,
                                Currency inPoints )
{
    // modified the following query to insert the sync record in to the database. Even if we insert the sync record
    // this should not affect the points on the card. Not setting the misc or invoice number so this will be displayed
    // nice, on the points history.

    __int32 pointsTransactionType        = 4; // pttSync     => adjustment_type field.
    __int32 pointsTransactionAccountType = 1; // ptstAccount => adjustment_subtype field.
    __int32 exportedStatus               = 1;

    AnsiString result =  "INSERT INTO PointsTransactions("
                            "POINTSTRANSACTIONS_KEY,"
                            "CONTACTS_KEY,"
                            "TIME_STAMP,"
                            "ADJUSTMENT_TYPE,"
                            "ADJUSTMENT_SUBTYPE,"
                            "ADJUSTMENT,"
                            "EXPORTED_STATUS,"
                            "TIME_STAMP_EXPORTED)"
                         "VALUES( " + IntToStr( inKey ) + ", " + IntToStr( inContactKey ) +
                         ", current_timestamp, " + IntToStr( pointsTransactionType )+
                         ", " + IntToStr( pointsTransactionAccountType ) +
                         ", " + FloatToStr( inPoints ) + ", " + IntToStr( exportedStatus ) + ", current_timestamp);";

    return result.c_str();
}
// ---------------------------------------------------------------------------
std::string TApplyParser::readContactsQuery()
{
	return "SELECT contacts_key as key "
           "FROM   Contacts;";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::getForeignKeysListForTableQuery( std::string inTableName )
{
    return "SELECT RDB$CONSTRAINT_NAME FK_LIST FROM RDB$RELATION_CONSTRAINTS "
           "WHERE RDB$RELATION_NAME = '" + inTableName + "' AND RDB$CONSTRAINT_TYPE = 'FOREIGN KEY'";
}
// ---------------------------------------------------------------------------
std::string TApplyParser::checkWhetherADomainExistsQuery()
{
	return "SELECT RDB$FIELD_NAME FROM RDB$FIELDS WHERE UPPER(RDB$FIELD_NAME) = :name ";
}
// ---------------------------------------------------------------------------
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
// Tool Functions
bool TApplyParser::tableExists(
                        std::string inTableName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
                inTableName,
				checkWhetherATableExistsQuery(),
			    inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::generatorExists(
                        std::string inGeneratorName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
                inGeneratorName,
                checkWhetherAGeneratorExistsQuery(),
			    inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::indexExists(
				        std::string inIndexName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
                inIndexName,
                checkWhetherAnIndexExistsQuery(),
			    inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::constraintExists(
                        std::string inConstraintName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
                inConstraintName,
                checkWhetherAConstraintExistsQuery(),
			    inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::fieldExists(
                        std::string inTableName,
                        std::string inFieldName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
				inTableName,
				inFieldName,
                checkWhetherAFieldExistsQuery(),
				inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::procedureExists(
					 std::string  inProcedureName,
			Database::TDBControl* inDBControl )
{
	return dbObjectExists(
				inProcedureName,
				checkWhetherAProcedureExistsQuery(),
				inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::triggerExists(
                        std::string inTriggerName,
                        TDBControl* inDBControl )
{
    return dbObjectExists(
                inTriggerName,
                checkWhetherATriggerExistsQuery(),
			    inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::dbObjectExists(
                        std::string inName,
                        std::string inSQL,
                        TDBControl* inDBControl )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	TDBTransaction transaction( *inDBControl );
	TIBSQL *query = transaction.Query( transaction.AddQuery() );

	transaction.StartTransaction();

		query->SQL->Text                       = UnicodeString( inSQL.c_str()  );
		query->ParamByName( "name" )->AsString = UnicodeString( inName.c_str() );
		query->ExecQuery();

		__int32 recordCount = query->RecordCount;
		result = recordCount > 0;

	transaction.Commit();

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TApplyParser::dbObjectExists(
                        std::string inNameOne,
                        std::string inNameTwo,
                        std::string inSQL,
                        TDBControl* inDBControl )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TDBTransaction transaction( *inDBControl );
	TIBSQL *query = transaction.Query( transaction.AddQuery() );

	transaction.StartTransaction();

		query->SQL->Text                       	   = UnicodeString( inSQL.c_str()  );
		query->ParamByName( "name_one" )->AsString = UnicodeString( inNameOne.c_str() );
		query->ParamByName( "name_two" )->AsString = UnicodeString( inNameTwo.c_str() );
		query->ExecQuery();

		__int32 recordCount = query->RecordCount;
		result = recordCount > 0;

	transaction.Commit();

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------
bool TApplyParser::DomainExists(std::string inDomainName, TDBControl* inDBControl )
{
    return dbObjectExists(inDomainName, checkWhetherADomainExistsQuery(), inDBControl );
}
//---------------------------------------------------------------------------
// It only checks for the existance of the version number in the
// VersionHistory Table. If the version number is in the VersionHistory table
// then the Parser assumes the updates have already been applied
bool TApplyParser::updateAlreadyApplied(
                        const std::string inVersion,
                        TDBControl* const inDBControl )
{
	return versionNumberAlreadyUpdated( inVersion, inDBControl );
}
//---------------------------------------------------------------------------
bool TApplyParser::versionNumberAlreadyUpdated(
                        const std::string inVersion,
                        TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

	TIBSQL *query    = transaction.Query( transaction.AddQuery() );
	query->SQL->Text = UnicodeString( versionNumberQuery().c_str() );

	transaction.StartTransaction();

	query->ParamByName( "versionNumber" )->AsString = UnicodeString( inVersion.c_str() );

	try
	{
        const bool result = ( query->ExecQuery(), query->RecordCount != 0 );
		transaction.Commit();

		return result;
	}
	catch (...)
	{
		transaction.Rollback();

		return false;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateVersionNumber(
                        const std::string inVersion,
                        TDBControl* const inDBControl )
{
    if( !versionNumberAlreadyUpdated( inVersion, inDBControl ) )
    {
        executeQuery( insertVersionNumberQuery( inVersion ), inDBControl );
    }
    else
    {
        executeQuery( updateVersionNumberQuery( inVersion ), inDBControl );
    }
}
// ---------------------------------------------------------------------------
void TApplyParser::executeQuery(
                        const std::string inSQL,
                        TDBControl* const inDBControl )
{
    TDBTransaction transaction( *inDBControl );

    try
    {
        TIBSQL *query = transaction.Query( transaction.AddQuery() );
        query->ParamCheck = false;

        query->SQL->Text = UnicodeString( inSQL.c_str() );

        transaction.StartTransaction();
            query->ExecQuery();
        transaction.Commit();
    }
    catch(Exception &E)
    {
        transaction.Rollback();
        throw;
    }
}
//---------------------------------------------------------------------------
void TApplyParser::readKeys(
                          const std::string inSQL,
                      std::vector<__int32>& inKeyVector,
                          TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

    //..............................................

	TIBSQL *query = transaction.Query( transaction.AddQuery() );
	query->SQL->Text = UnicodeString( inSQL.c_str() );

	transaction.StartTransaction();
		query->ExecQuery();

        inKeyVector.clear();
        while( !query->Eof )
        {
            inKeyVector.push_back( query->FieldByName( "key" )->AsInteger );
            query->Next();
        }
	transaction.Commit();

}
// ---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::getForeignKeysListForTable(const std::string inSQL,std::vector<UnicodeString>& inFKeysList,TDBControl* const inDBControl )
{
	TDBTransaction transaction( *inDBControl );

    //..............................................

	TIBSQL *query = transaction.Query( transaction.AddQuery() );
	query->SQL->Text = UnicodeString( inSQL.c_str() );

	transaction.StartTransaction();
		query->ExecQuery();

        inFKeysList.clear();
        while( !query->Eof )
        {
            inFKeysList.push_back( query->FieldByName( "FK_LIST" )->AsString );
            query->Next();
        }
	transaction.Commit();
}
// ---------------------------------------------------------------------------
void TApplyParser::readContacts(std::vector<__int32>& inContactVector,TDBControl* const inDBControl )
{
    readKeys(readContactsQuery(),inContactVector,inDBControl );
}
// ---------------------------------------------------------------------------
Currency TApplyParser::getPoints(const std::string inSQL,TDBControl* const inDBControl )
{
    Currency result = 0;
	TDBTransaction transaction( *inDBControl );
	TIBSQL *query = transaction.Query( transaction.AddQuery() );
	query->SQL->Text = UnicodeString( inSQL.c_str() );
	transaction.StartTransaction();
	query->ExecQuery();
    result = query->FieldByName( "points" )->AsCurrency;
	transaction.Commit();
    return result;
}
// ---------------------------------------------------------------------------
Currency TApplyParser::getContactPoints(__int32 inContactKey,TDBControl* const inDBControl )
{
    return getPoints(contactPointsQuery( inContactKey ),inDBControl );
}
// ---------------------------------------------------------------------------
Currency TApplyParser::getContactTransactionPoints(__int32 inContactKey, TDBControl* const inDBControl )
{
    return getPoints( contactTransactionPointsQuery( inContactKey ), inDBControl );
}
// ---------------------------------------------------------------------------
void TApplyParser::adjustContactTransactionPoints(__int32 inContactKey,Currency inPoints,TDBControl* const inDBControl )
{
    TDBTransaction transaction( *inDBControl );

    //:::::::::::::::::::::::::::::::::::::::

    transaction.StartTransaction();

    TIBSQL *qrGenID    = transaction.Query( transaction.AddQuery() );

    qrGenID->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
    qrGenID->ExecQuery();

    int key = qrGenID->Fields[0]->AsInteger;

    transaction.Commit();

    //:::::::::::::::::::::::::::::::::::::::

    executeQuery(
        adjustContactTransactionPointsQuery( key, inContactKey, inPoints ),
        inDBControl );
}
// ---------------------------------------------------------------------------
std::vector<TPointsTransaction> TApplyParser::getHeldPointsTransactions(__int32 inContactKey,TDBControl* const inDBControl )
{
    std::vector<TPointsTransaction> result;

    TDBTransaction transaction( *inDBControl );
    //::::::::::::::::::::::::::::::::::::::::::
    transaction.StartTransaction();

    TIBSQL *qrGetHeldTransactions = transaction.Query(transaction.AddQuery());
    qrGetHeldTransactions->SQL->Text = "SELECT "
                                            "a.POINTSTRANSACTIONS_KEY, "
                                            "a.CONTACTS_KEY, "
                                            "a.TIME_STAMP, "
                                            "a.ADJUSTMENT_TYPE, "
                                            "a.ADJUSTMENT_SUBTYPE, "
                                            "a.ADJUSTMENT, "
                                            "a.EXPORTED_STATUS, "
                                            "a.TIME_STAMP_EXPORTED, "
                                            "a.MISC, "
                                            "a.INVOICE_NUMBER "
                                        "FROM POINTSTRANSACTIONS a "
                                        "WHERE a.CONTACTS_KEY = :CONTACTS_KEY AND a.EXPORTED_STATUS = :EXPORTED_STATUS";

     qrGetHeldTransactions->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
     qrGetHeldTransactions->ParamByName("EXPORTED_STATUS")->AsInteger = pesNone;
     qrGetHeldTransactions->ExecQuery();

     TPointsTransaction heldTransaction;
     TPointsTransaction validatingTransaction;

     while(!qrGetHeldTransactions->Eof)
     {
        heldTransaction.PointsTransactionsKey = qrGetHeldTransactions->FieldByName("POINTSTRANSACTIONS_KEY")->AsInteger;
		heldTransaction.ContactKey = qrGetHeldTransactions->FieldByName("CONTACTS_KEY")->AsInteger;
		heldTransaction.TimeStamp = qrGetHeldTransactions->FieldByName("TIME_STAMP")->AsDateTime;
		heldTransaction.TimeStampExported = qrGetHeldTransactions->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
		heldTransaction.PointsTransactionType = qrGetHeldTransactions->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
		heldTransaction.PointsTransactionAccountType = qrGetHeldTransactions->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
		heldTransaction.Adjustment = fabs(qrGetHeldTransactions->FieldByName("ADJUSTMENT")->AsCurrency);
		heldTransaction.ExportStatus = qrGetHeldTransactions->FieldByName("EXPORTED_STATUS")->AsInteger;
		heldTransaction.InvoiceNumber = qrGetHeldTransactions->FieldByName("INVOICE_NUMBER")->AsString;

        TMemoryStream *Streamcheck = heldTransaction.GetAsStream();
        Streamcheck->Position = 0;
        qrGetHeldTransactions->FieldByName("MISC")->SaveToStream(Streamcheck);

        if (!heldTransaction.Validate())
        {
            /*
                at this point transaction is invalid with its stored data. it might be be because the parser changed the adjustment type
                just to be sure we need to revert the adjustment type to what it was before, and then re-validate.. if it valid then we need to
                update the binary data again with newer values
            */

            validatingTransaction = heldTransaction;

            switch(heldTransaction.PointsTransactionType)
            {
                case 1:
                    validatingTransaction.PointsTransactionType = 3;
                    break;
                case 2:
                    validatingTransaction.PointsTransactionType = 4;
                    break;
                case 3:
                    validatingTransaction.PointsTransactionType = 5;
                    break;
                case 4:
                    validatingTransaction.PointsTransactionType = 6;
                    break;
                case 5:
                    validatingTransaction.PointsTransactionType = 7;
                    break;
                default:
                    break;
            }

            // another check
            if( heldTransaction.PointsTransactionType == 1
                && heldTransaction.PointsTransactionAccountType == 2)
            {
                validatingTransaction.PointsTransactionType = 8;
            }

            validatingTransaction.PointsTransactionAccountType = 0;

            //check whether this is a valid transaction
            Streamcheck = validatingTransaction.GetAsStream();
            Streamcheck->Position = 0;
            qrGetHeldTransactions->FieldByName("MISC")->SaveToStream(Streamcheck);

            if(validatingTransaction.Validate())
            {
                // add it to collection. so the parser can continue with updates
                result.push_back(heldTransaction);
            }
        }
        qrGetHeldTransactions->Next();
     }

    transaction.Commit();
    return result;
}
//---------------------------------------------------------------------------
void TApplyParser::reEncryptMiscDataForPointsTransactions(
                                TDBControl* const inDBControl,
                                std::vector<TPointsTransaction> transactionList)
{
    TDBTransaction transaction( *inDBControl );
    //::::::::::::::::::::::::::::::::::::::::::
    transaction.StartTransaction();

    TIBSQL* qrSetPointsTransactionEncryptData = transaction.Query(transaction.AddQuery());
    qrSetPointsTransactionEncryptData->SQL->Text = "UPDATE POINTSTRANSACTIONS SET MISC=:MISC WHERE POINTSTRANSACTIONS_KEY = :POINTSTRANSACTIONS_KEY";

    for(
        std::vector<TPointsTransaction>::iterator it = transactionList.begin();
        it != transactionList.end();
        it ++)
    {
        TMemoryStream *Streamcheck = (*it).GetAsStream();
		Streamcheck->Position = 0;
		qrSetPointsTransactionEncryptData->ParamByName("MISC")->LoadFromStream(Streamcheck);
		qrSetPointsTransactionEncryptData->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = (*it).PointsTransactionsKey;
		qrSetPointsTransactionEncryptData->ExecQuery();
    }

    transaction.Commit();
}

}
