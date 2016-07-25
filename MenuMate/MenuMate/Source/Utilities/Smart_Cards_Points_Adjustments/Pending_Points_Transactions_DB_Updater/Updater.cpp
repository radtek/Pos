//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Updater.h"
#include "blowfish.h"
#include "MMRegistry.h"
#include "enumContacts.h"
#include "enumPoints.h"
#define MMBaseKey "\\Software\\Wow6432Node\\IQWorks\\MenuMate\\"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IBSQL"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmUpdater *frmUpdater;
//---------------------------------------------------------------------------
__fastcall TfrmUpdater::TfrmUpdater(TComponent* Owner)
    : TForm(Owner)
{
    initialize();
}
//---------------------------------------------------------------------------
void TfrmUpdater::initialize()
{
    encryptPassword = "InTheBeginningGodCreatedTheHeavensAndTheEarthMenuMateCameLater";
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdater::btnBrowseClick(TObject *Sender)
{
    if(OpenBinFileDialog->Execute(this->Handle))
    {
        binFilePath = OpenBinFileDialog->FileName;
        lblBinFilePath->Caption = binFilePath;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdater::btnUpdateDBClick(TObject *Sender)
{
    if(initializeDBConnection())
    {
        if( isBinFileExists(binFilePath))
        {
            std::auto_ptr<TMemoryStream> binFileStream (new TMemoryStream());
            loadStreamFromFile( binFilePath, binFileStream.get() );
            decryptStream(
                    binFileStream.get(),
                    encryptPassword );

    #if defined(_DEBUG)
            binFileStream->SaveToFile("Decrypted.xml");
    #endif

            xmlDocument = new TiXmlDocument();
            loadXmlDocumentFromStream(
                                binFileStream.get(),
                                xmlDocument);

            updatePointsTransactionsFromXmlFile( xmlDocument );

            delete xmlDocument;
        }
    }
    else
    {
        Application->MessageBox(
                    L"Either database path or server name is invalid. Connection failed.",
                    L"Connection failed",
                    MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdater::btnSetDBClick(TObject *Sender)
{
    setDBLocation();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdater::FormShow(TObject *Sender)
{
    displayDBLocation();
}
//---------------------------------------------------------------------------
bool TfrmUpdater::isBinFileExists( AnsiString path )
{
    return FileExists( path );
}
//---------------------------------------------------------------------------
void TfrmUpdater::loadStreamFromFile(
                AnsiString filePath,
                TMemoryStream* outStream)
{
    outStream->LoadFromFile( filePath );
}
//---------------------------------------------------------------------------
void TfrmUpdater::decryptStream(
                    TMemoryStream* inStream,
                    AnsiString password)
{
    CBlowFish BF;
    BF.Initialize(password.c_str(),password.Length()+1);
    BF.Decode((BYTE *)inStream->Memory,(BYTE *)inStream->Memory,inStream->Size);
}
//---------------------------------------------------------------------------
void TfrmUpdater::loadXmlDocumentFromStream(
                    TMemoryStream* inStream,
                    TiXmlDocument* outXmlDocument)
{
    outXmlDocument->Parse((const char*)inStream->Memory);

#if defined(_DEBUG)
    //if(!outXmlDocument->LoadFile("C:\\Documents and Settings\\Administrator\\Desktop\\temp\\SCXml\\Decrypted.xml", TIXML_ENCODING_UTF8))
    //    throw new Exception("Failed to load xml file");
#endif
}
//---------------------------------------------------------------------------
void TfrmUpdater::updatePointsTransactionsFromXmlFile(
                        TiXmlDocument* inXmlDocument )
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
        int numberOfAdjustments = getNumberOfChildNodes(inXmlDocument->RootElement());
        if( numberOfAdjustments > 0 )
        {
            TiXmlElement* adjustmentContactElement = inXmlDocument->RootElement()->FirstChildElement();

            while( adjustmentContactElement != NULL)
            {
                TAdjustmentContact contact;
                loadAdjustmentContactFromXmlElement(
                                                adjustmentContactElement,
                                                contact);

                updateContactWithAdjustments(
                                        DBTransaction,
                                        contact);

                adjustmentContactElement = adjustmentContactElement->NextSiblingElement();
            }
        }

        DBTransaction.Commit();
        Application->MessageBox(
                    L"Sync records created successfully",
                    L"Success",
                    MB_OK);
    }
    catch( Exception &ex )
    {
        Application->MessageBox(
                    L"Failed to complete some operations. You may have to run the process again.",
                    L"Failed",
                    MB_OK);
        DBTransaction.Undo();
    }
}
//---------------------------------------------------------------------------
int TfrmUpdater::getNumberOfChildNodes(TiXmlElement* element)
{
    int result = 0;

    try
    {
        if( element )
        {
            TiXmlElement* childElement = element->FirstChildElement();
            while( childElement != NULL )
            {
                result++;
                childElement = childElement->NextSiblingElement();
            }
        }
    }
    catch( ... )
    {
        result = 0;
    }

    return result;
}
//---------------------------------------------------------------------------
void TfrmUpdater::loadAdjustmentContactFromXmlElement(
                            TiXmlElement* inElement,
                            TAdjustmentContact &contact )
{
    contact.Name = AnsiString( inElement->Attribute("Name") );
    contact.MemberNumber = AnsiString( inElement->Attribute("MemberNumber") );
    contact.SiteId = StrToInt( inElement->Attribute("SiteId") );

    int numberOfTransactionsToUpdate = getNumberOfChildNodes( inElement );

    if( numberOfTransactionsToUpdate > 0 )
    {
        TAdjustmentTransaction transaction;
        TiXmlElement* transactionElement = inElement->FirstChildElement();

        while(transactionElement != NULL )
        {
            loadAdjustmentTransactionFromXmlElement(
                                                transactionElement,
                                                transaction);
            contact.Adjustments.push_back(transaction);
            transactionElement = transactionElement->NextSiblingElement();
        }
    }
}
//---------------------------------------------------------------------------
void TfrmUpdater::loadAdjustmentTransactionFromXmlElement(
                                          TiXmlElement* inElement,
                                          TAdjustmentTransaction &transaction)
{
    try
    {
        transaction.Adjustment = StrToCurr( inElement->Attribute("Amount") ); //StrToCurr( inElement->Attribute("Adjustment") );
        transaction.AdjustmentType = pttSync;     //StrToInt( inElement->Attribute("AdjustmentType") );
        transaction.AdjustmentSubType = ptstLoyalty;  //StrToInt( inElement->Attribute("AdjustmentSubType") );
        //transaction.TimeStamp =
        transaction.InvoiceNumber = 0;      //StrToInt( inElement->Attribute("InvoiceNumber") );
    }
    catch( Exception &ex )
    {
        throw ex;
    }
}
//---------------------------------------------------------------------------
AnsiString TfrmUpdater::getDBLocation()
{
    dbLocation = "";
    serverName = "";
    RegistryRead(MMBaseKey "Database", "DatabasePath", dbLocation);
    RegistryRead(MMBaseKey "Database", "InterbaseIP", serverName);

    if (ExtractFileExt(dbLocation).UpperCase() == ".GDB" || ExtractFileExt(dbLocation).UpperCase() == ".IB" )
    {
        dbLocation = ChangeFileExt(dbLocation, ".FDB");
    }

    if (dbLocation == "")
    {
        setDBLocation();
    }

    return dbLocation;
}
//---------------------------------------------------------------------------
AnsiString TfrmUpdater::getServerName()
{
    serverName = txtServerName->Text;
    return serverName;
}
//---------------------------------------------------------------------------
void TfrmUpdater::displayDBLocation()
{
    lblDBPath->Caption = getDBLocation();
}
//---------------------------------------------------------------------------
void TfrmUpdater::setDBLocation()
{
    if(OpenDBDialog->Execute(this->Handle))
    {
        dbLocation = OpenDBDialog->FileName;
    }
}
//---------------------------------------------------------------------------
bool TfrmUpdater::initializeDBConnection()
{
    bool status = false;
    try
    {
        if(serverName == "")
        {
            getServerName();
        }

        if(dbLocation != "" && serverName != "")
        {
            DBControl.Disconnect();
            DBControl.Init(Database::TDBSettings(serverName, dbLocation, false));
            DBControl.Connect();
            Application->ProcessMessages();
            status = true;
        }
    }
    catch(Exception & E)
    {
        dbLocation = "";
    }

    return status;
}
//---------------------------------------------------------------------------
void TfrmUpdater::updateContactWithAdjustments(
                            Database::TDBTransaction &transaction,
                            TAdjustmentContact &contact )
{
    contact.ContactKey = getContactIdByMemberNumberSiteId(
                                                transaction,
                                                contact.MemberNumber,
                                                contact.SiteId);
    if( contact.ContactKey == 0 )
    {
       setContactDetails(
                    transaction,
                    contact );
    }

    if( contact.ContactKey != 0
        && contact.Adjustments.size() > 0 )
    {
        std::vector<TAdjustmentTransaction>::iterator it = contact.Adjustments.begin();
        while( it != contact.Adjustments.end() )
        {
            it->ContactKey = contact.ContactKey;
            setAdjustmentForContact(
                                    transaction,
                                    *it);
            it++;
        }
    }
}
//---------------------------------------------------------------------------
int TfrmUpdater::getContactIdByMemberNumberSiteId(
                        Database::TDBTransaction &transaction,
                        AnsiString memberNumber,
                        int siteId )
{
    int result = 0;
    try
    {
        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
                                "SELECT "
                                 "CONTACTS_KEY "
                                "FROM "
                                 "CONTACTS "
                                "WHERE "
                                    "MEMBER_NUMBER = :MEMBER_NUMBER AND "
                                 "SITE_ID = :SITE_ID";
        IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = memberNumber;
        IBInternalQuery->ParamByName("SITE_ID")->AsInteger = siteId;
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
        {
            result = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
        }
    }
    catch(Exception &E)
    {
        throw new Exception("Failed to search member in the database");
    }

    return result;
}
//---------------------------------------------------------------------------
int TfrmUpdater::getNextContactKey()
{
    return getNextKeyFromGenerator("GEN_CONTACTS");
}
//---------------------------------------------------------------------------
int TfrmUpdater::getNextPointsTransactionKey()
{
    return getNextKeyFromGenerator("GEN_POINTSTRANSACTIONS");
}
//---------------------------------------------------------------------------
void TfrmUpdater::setContactDetails(
                Database::TDBTransaction &transaction,
                TAdjustmentContact &contact )
{
    try
    {
        int contactKey = getNextContactKey();

        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
        IBInternalQuery->SQL->Text =
             "INSERT INTO CONTACTS (" "CONTACTS_KEY," "CONTACT_TYPE," "NAME, LAST_NAME," "PIN," "SITE_ID,"
             "CONTACTS_3RDPARTY_KEY," "MEMBER_NUMBER, ACCOUNT_PROFILE) " "VALUES (" ":CONTACTS_KEY, :CONTACT_TYPE, :NAME, :LAST_NAME, :PIN, :SITE_ID,"
             ":CONTACTS_3RDPARTY_KEY, :MEMBER_NUMBER, :ACCOUNT_PROFILE);";
         IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
         IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eMember;
         IBInternalQuery->ParamByName("NAME")->AsString = contact.Name;
         IBInternalQuery->ParamByName("LAST_NAME")->AsString = ""; // todo get from contact object
         IBInternalQuery->ParamByName("PIN")->AsString = "";
         IBInternalQuery->ParamByName("SITE_ID")->AsInteger = contact.SiteId;
         IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = 0;
         IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = contact.MemberNumber;
         IBInternalQuery->ParamByName("ACCOUNT_PROFILE")->AsInteger = 0;
         IBInternalQuery->ExecQuery();

         if(IBInternalQuery->RowsAffected > 0)
             contact.ContactKey = contactKey;
    }
    catch( ... )
    {
        throw new Exception ("Failed to create contact");
    }
}
//---------------------------------------------------------------------------
void TfrmUpdater::setAdjustmentForContact(
                        Database::TDBTransaction &transaction,
                        TAdjustmentTransaction &adjustmentTransaction )
{
    TPointsTransaction pointsTransaction;
    pointsTransaction.ContactKey = adjustmentTransaction.ContactKey;
    pointsTransaction.TimeStamp = Now();
    pointsTransaction.PointsTransactionType = adjustmentTransaction.AdjustmentType;
    pointsTransaction.PointsTransactionAccountType = adjustmentTransaction.AdjustmentSubType;
    pointsTransaction.ExportStatus = pesNone;
    pointsTransaction.Adjustment = adjustmentTransaction.Adjustment;

    insertPointsTransactionToDB(
                            transaction,
                            pointsTransaction);
}
//---------------------------------------------------------------------------
void TfrmUpdater::insertPointsTransactionToDB(
                            Database::TDBTransaction &transaction,
                            TPointsTransaction pointsTransaction)
{
    try
    {
        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
        pointsTransaction.PointsTransactionsKey = getNextPointsTransactionKey();

        IBInternalQuery->SQL->Text = getPointsTransactionInsertQuery();
        IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = pointsTransaction.PointsTransactionsKey;
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = pointsTransaction.ContactKey;
        IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = pointsTransaction.TimeStamp;
        //IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = ;
        IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = pointsTransaction.PointsTransactionType;
        IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = pointsTransaction.PointsTransactionAccountType;
        IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pointsTransaction.ExportStatus;
        //IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = NULL;
        IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = pointsTransaction.Adjustment;

        TMemoryStream *Streamcheck = pointsTransaction.GetAsStream();
		Streamcheck->Position = 0;
		IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);

        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RowsAffected <= 0)
            throw new Exception("Failed to insert points transaction");
    }
    catch( ... )
    {
        throw new Exception ("Failed to create points adjustment record");
    }
}
//---------------------------------------------------------------------------
UnicodeString TfrmUpdater::getPointsTransactionInsertQuery()
{
    UnicodeString query="INSERT INTO POINTSTRANSACTIONS "
			"("
				"POINTSTRANSACTIONS_KEY,"
				"CONTACTS_KEY,"
				"TIME_STAMP,"
				//"TIME_STAMP_EXPORTED,"
				"ADJUSTMENT_TYPE,"
				"ADJUSTMENT_SUBTYPE,"
				"ADJUSTMENT,"
				"EXPORTED_STATUS,"
				"MISC"
                //,"
				//"INVOICE_NUMBER"
			")"
			" VALUES "
			"("
				":POINTSTRANSACTIONS_KEY,"
				":CONTACTS_KEY,"
				":TIME_STAMP,"
				//":TIME_STAMP_EXPORTED,"
				":ADJUSTMENT_TYPE,"
				":ADJUSTMENT_SUBTYPE,"
				":ADJUSTMENT,"
				":EXPORTED_STATUS,"
				":MISC"
                //,"
				//":INVOICE_NUMBER"
			");";

    return query;
}
//---------------------------------------------------------------------------
int TfrmUpdater::getNextKeyFromGenerator(
                                AnsiString generatorName )
{
    int result = 0;
    Database::TDBTransaction transaction( DBControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(" + generatorName + ", 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
        {
            result = IBInternalQuery->Fields[0]->AsInteger;
        }
    }
    catch(Exception &E)
    {
        throw new Exception("Failed to search member in the database");
    }

    transaction.Commit();
    return result;
}
//---------------------------------------------------------------------------

