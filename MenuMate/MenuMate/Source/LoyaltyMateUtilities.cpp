//---------------------------------------------------------------------------


#pragma hdrstop
#include "LoyaltyMateUtilities.h"
#include "DBContacts.h"
#include "ManagerSyndCode.h"
#include "CrcTable.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// LoyaltyUtilities Methods
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int TLoyaltyMateUtilities::DeleteTransaction(int contactKey,int transactionNumber)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();
    query->SQL->Text =  "DELETE FROM LOYALTYPENDINGTRANSACTIONS WHERE TRANSACTION_NUMBER=:TRANSACTION_NUMBER AND CONTACT_KEY=:CONTACT_KEY";

    query->ParamByName("TRANSACTION_NUMBER")->AsInteger = transactionNumber;
    query->ParamByName("CONTACT_KEY")->AsInteger = contactKey;
    query->ExecQuery();

    DBTransaction.Commit();
}
//-------------------------------------------------------------------------
void TLoyaltyMateUtilities::SetTransaction(Database::TDBTransaction &DBTransaction,TLoyaltyMateTransaction transaction)
{
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();
    query->SQL->Text = "SELECT GEN_ID(GEN_LOYALTYPENDINGTRANSACTIONS, 1) FROM RDB$DATABASE";
    query->ExecQuery();
    int nextLoyaltyTransactionID = query->Fields[0]->AsInteger;
    query->Close();
    query->SQL->Text =  "INSERT INTO LOYALTYPENDINGTRANSACTIONS"
                        "("
                        "   TRANSACTION_NUMBER,"
                        "   CONTACT_KEY,"
						"   SYNDCODES_KEY,"
                        "   POINT_BALANCE,"
                        "   POINTS_DELTA,"
                        "   OCCURRED_DATE,"
                        "   POINTS_TYPE, "
                        "   INVOICE_NUMBER"
                        ")"
                        "Values"
                        "("
                        "   :TRANSACTION_NUMBER,"
                        "   :CONTACT_KEY,"
                        "   :SYNDCODES_KEY,"
                        "   :POINT_BALANCE,"
                        "   :POINTS_DELTA,"
                        "   :OCCURRED_DATE,"
                        "   :POINTS_TYPE, "
                        "   :INVOICE_NUMBER "
                        ");";

    query->ParamByName("TRANSACTION_NUMBER")->AsInteger = nextLoyaltyTransactionID;
    query->ParamByName("CONTACT_KEY")->AsInteger = transaction.ContactKey;
    query->ParamByName("SYNDCODES_KEY")->AsInteger = transaction.SyndicateCode.SyndCodeKey;
    query->ParamByName("POINT_BALANCE")->AsCurrency = transaction.PointsBalance;
    query->ParamByName("POINTS_DELTA")->AsCurrency = transaction.PointsDelta;
    query->ParamByName("OCCURRED_DATE")->AsDateTime = transaction.OccurredDate;
    query->ParamByName("POINTS_TYPE")->AsInteger = transaction.PointsType;
    query->ParamByName("INVOICE_NUMBER")->AsString = transaction.InvoiceNumber;
    query->ExecQuery();
}
//-------------------------------------------------------------------------
std::vector<int> TLoyaltyMateUtilities::GetAllContactsWithPendingTransactions()
{
    std::vector<int> result;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();
    query->SQL->Text =  "SELECT "
                        "   DISTINCT(LPT.CONTACT_KEY) "
                        "FROM "
                        "   LOYALTYPENDINGTRANSACTIONS LPT"
                        "   LEFT JOIN DAYARCBILL d ON LPT.INVOICE_NUMBER = d.INVOICE_NUMBER "
                        "   JOIN LOYALTYATTRIBUTES ATTR ON LPT.CONTACT_KEY = ATTR.CONTACTS_KEY "
                        "   WHERE LPT.IS_AVAILABLE_FOR_POSTING = :IS_AVAILABLE_FOR_POSTING"
                        "   AND d.TERMINAL_NAME = :TERMINAL_NAME "
                        "   UNION ALL "
                        "SELECT "
                        "   DISTINCT(LPT.CONTACT_KEY) "
                        "FROM "
                        "   LOYALTYPENDINGTRANSACTIONS LPT"
                        "   LEFT JOIN ARCBILL d ON LPT.INVOICE_NUMBER = d.INVOICE_NUMBER "
                        "   JOIN LOYALTYATTRIBUTES ATTR ON LPT.CONTACT_KEY = ATTR.CONTACTS_KEY "
                        "   WHERE LPT.IS_AVAILABLE_FOR_POSTING = :IS_AVAILABLE_FOR_POSTING"
                        "   AND d.TERMINAL_NAME = :TERMINAL_NAME ";
    query->ParamByName("IS_AVAILABLE_FOR_POSTING")->AsString = "T";
    query->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
    query->ExecQuery();
    while(!query->Eof)
    {
        result.push_back(query->Fields[0]->AsInteger);
        query->Next();
    }
    DBTransaction.Commit();
    return result;
}
//-------------------------------------------------------------------------
std::vector<TLoyaltyMateTransaction> TLoyaltyMateUtilities::GetPendingTransactionForContact(int contactKey)
{
	TManagerSyndCode managerSyndCode;
    std::vector<TLoyaltyMateTransaction> result;

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	managerSyndCode.Initialise(DBTransaction);

    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();

    query->SQL->Text = "SELECT "
                        "   LPT.TRANSACTION_NUMBER,"
                        "   ATTR.UUID,"
                        "   LPT.CONTACT_KEY,"
                        "   LPT.SYNDCODES_KEY,"
                        "   LPT.POINT_BALANCE,"
                        "   LPT.POINTS_DELTA,"
                        "   LPT.OCCURRED_DATE, "
                        "   LPT.POINTS_TYPE, "
                        "   LPT.INVOICE_NUMBER "
                        "FROM "
                        "   LOYALTYPENDINGTRANSACTIONS LPT "
                        "   JOIN LOYALTYATTRIBUTES ATTR ON LPT.CONTACT_KEY = ATTR.CONTACTS_KEY "
                        "WHERE "
                        "   LPT.CONTACT_KEY=:CONTACT_KEY "
                        "ORDER BY "
                        "   LPT.OCCURRED_DATE ASC";

    query->ParamByName("CONTACT_KEY")->AsInteger = contactKey;
    query->ExecQuery();

    while(!query->Eof)
    {
        TLoyaltyMateTransaction transaction;
        transaction.TransactionNumber = query->FieldByName("TRANSACTION_NUMBER")->AsInteger;
        transaction.CloudUUID = query->FieldByName("UUID")->AsString;
        transaction.ContactKey = query->FieldByName("CONTACT_KEY")->AsInteger;
        transaction.SyndicateCode = managerSyndCode.SyndCodeByKey(query->FieldByName("SYNDCODES_KEY")->AsInteger);
        transaction.PointsBalance = query->FieldByName("POINT_BALANCE")->AsCurrency;
        transaction.PointsDelta = query->FieldByName("POINTS_DELTA")->AsCurrency;
        transaction.OccurredDate = query->FieldByName("OCCURRED_DATE")->AsDateTime;
        transaction.PointsType = query->FieldByName("POINTS_TYPE")->AsInteger;
        transaction.InvoiceNumber = query->FieldByName("INVOICE_NUMBER")->AsString;
        result.push_back(transaction);
        query->Next();
   }
  DBTransaction.Commit();
  return result;
}
//-------------------------------------------------------------------------
bool TLoyaltyMateUtilities::GetContactByCloudUUID(Database::TDBTransaction &DBTransaction,UnicodeString uuid,int &contactKey,int &siteId)
{
   bool result = false;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY, SITE_ID FROM LOYALTYATTRIBUTES WHERE UUID=:UUID";
      IBInternalQuery->ParamByName("UUID")->AsString = uuid;
      IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount)
      {
         contactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
         siteId = IBInternalQuery->FieldByName("SITE_ID")->AsInteger;
            result = true;
      }
   }
	catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return result;
}
//-------------------------------------------------------------------------
void TLoyaltyMateUtilities::SetLoyaltymateTransactions(Database::TDBTransaction &DBTransaction,int contactKey,TSyndCode syndicateCode,
                                                       TContactPoints PointsToLoad,AnsiString inInvoiceNumber)
{
    TPointsStore PointsToArchive = PointsToLoad.getExcludingSource(pasDatabase);
	for (TPointsStore::iterator ptrPointsTransaction = PointsToArchive.begin();
	ptrPointsTransaction != PointsToArchive.end(); ptrPointsTransaction++)
	{
        std::pair<TPointsType, Currency> Detials = *ptrPointsTransaction;
        TLoyaltyMateTransaction transaction;
        transaction.ContactKey = contactKey;
        transaction.SyndicateCode = syndicateCode;
        transaction.OccurredDate = Now();
        if(!TGlobalSettings::Instance().RefundingItems && (Detials.first.AccType.first == pttRefund))
           Detials.second = Detials.second * (-1);
        transaction.PointsDelta = Detials.second;
        transaction.PointsType = TLoyaltyMateUtilities::GetPointsType(Detials.first.AccType.first);
        transaction.InvoiceNumber = inInvoiceNumber;
        if(transaction.PointsDelta != 0)
          TLoyaltyMateUtilities::SetTransaction(DBTransaction,transaction);
	}
}
//-------------------------------------------------------------------------
int TLoyaltyMateUtilities::GetPointsType(TPointsTransactionType pointType)
{
    switch(pointType)
    {
       case pttPurchased:
           return 8;
       case pttEarned:
           return 1;
       case pttRedeemed:
           return 6;
       case pttBirthdayBonus:
           return 2;
       case pttFirstVisit:
           return 3;
       case pttRefund:
           return 1;
       case pttRedeemedBD:
           return 4;
       case pttRedeemedFV:
           return 5;
    }
}
//-------------------------------------------------------------------------
UnicodeString TLoyaltyMateUtilities::GetMemberCloudIdIfRegistered(Database::TDBTransaction &DBTransaction,int contactKey,int siteId)
{
   UnicodeString result = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT UUID FROM LOYALTYATTRIBUTES WHERE CONTACTS_KEY=:CONTACTS_KEY AND SITE_ID=:SITE_ID";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
      IBInternalQuery->ParamByName("SITE_ID")->AsInteger = siteId;
	  IBInternalQuery->ExecQuery();

      if(!IBInternalQuery->Eof)
      {
		result = IBInternalQuery->FieldByName("UUID")->AsString;
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return result;
}
//------------------------------------------------------------------------
UnicodeString TLoyaltyMateUtilities::GetMemberCloudId(Database::TDBTransaction &DBTransaction,int contactKey)
{
   UnicodeString result = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT UUID FROM LOYALTYATTRIBUTES WHERE CONTACTS_KEY=:CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
	  IBInternalQuery->ExecQuery();

      if(!IBInternalQuery->Eof)
      {
		result = IBInternalQuery->FieldByName("UUID")->AsString;
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
   return result;
}
//-------------------------------------------------------------------------
bool TLoyaltyMateUtilities::SetMemberCloudId(Database::TDBTransaction &DBTransaction,int contactKey,int siteId,UnicodeString uuid,UnicodeString activationToken)
{
   bool result = false;
   try
   {
	  int storedContactKey = 0;
      int storedSiteId = 0;
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  GetContactByCloudUUID(DBTransaction, uuid, storedContactKey, storedSiteId);

	  if(contactKey == storedContactKey && siteId == storedSiteId)
	  {
		 result = true;
	  }
	  else if(storedContactKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "UPDATE LOYALTYATTRIBUTES SET CONTACTS_KEY = :CONTACTS_KEY, SITE_ID = :SITE_ID WHERE UUID = :UUID";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
		 IBInternalQuery->ParamByName("SITE_ID")->AsInteger = siteId;
		 IBInternalQuery->ParamByName("UUID")->AsString = uuid;
		 IBInternalQuery->ExecQuery();
		 result = true;
	  }
	  else
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOYALTYATTRIBUTES, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 int attribKey = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "INSERT INTO LOYALTYATTRIBUTES(ATTRIB_KEY,UUID,CONTACTS_KEY,SITE_ID,ACTIVATION_TOKEN) VALUES (:ATTRIB_KEY, :UUID, :CONTACTS_KEY, :SITE_ID, :ACTIVATION_TOKEN)";
		 IBInternalQuery->ParamByName("ATTRIB_KEY")->AsInteger = attribKey;
		 IBInternalQuery->ParamByName("UUID")->AsString = uuid;
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
		 IBInternalQuery->ParamByName("SITE_ID")->AsInteger = siteId;
         IBInternalQuery->ParamByName("ACTIVATION_TOKEN")->AsString = activationToken;
		 IBInternalQuery->ExecQuery();
		 result = true;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return result;
}
//-------------------------------------------------------------------------
bool TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(UnicodeString uuid)
{
	bool lengthCheck = uuid.Length() > 0;
	bool disabledCheck = uuid != GetLoyaltyMateDisabledCloudUUID();
	return lengthCheck && disabledCheck;
}
//-------------------------------------------------------------------------
UnicodeString TLoyaltyMateUtilities::GetLoyaltyMateDisabledCloudUUID()
{
    return "00000000-0000-0000-0000-000000000000";
}

AnsiString TLoyaltyMateUtilities::GetAutoGeneratedMemberEmail(Database::TDBTransaction &DBTransaction)
{
    AnsiString GenMailId = "mm" + TLoyaltyMateUtilities::GetUniqueNumber(DBTransaction) + "@lm.com";
    return GenMailId;
}

AnsiString TLoyaltyMateUtilities::GetAutoGeneratedGiftCardEmail(Database::TDBTransaction &DBTransaction)
{
    AnsiString GenMailId = "giftcard" + TLoyaltyMateUtilities::GetUniqueNumber(DBTransaction) + "@lm.com";
    return GenMailId;
}



AnsiString TLoyaltyMateUtilities::GetUniqueNumber(Database::TDBTransaction &DBTransaction)
{
   AnsiString value = "";
   value=TDateTime::CurrentDateTime().FormatString("yyyymmddhhmmss");

   std::auto_ptr<TLoyaltyMateUtilities> LoyaltyMateUtilities;
   int CalcCRC = LoyaltyMateUtilities->StreamCheckCRC(value);
   UnicodeString hex_token = IntToHex(CalcCRC, 2);
   value = hex_token;
   TManagerSyndCode ManagerSyndicateCode;
  ManagerSyndicateCode.Initialise(DBTransaction);
  int nextNumber = TDBContacts::GenerateNextMemberEmailNumber(DBTransaction);
  AnsiString siteString = IntToStr(TGlobalSettings::Instance().SiteID);
  AnsiString syndicate = ManagerSyndicateCode.GetCommunicationSyndCodeString();

  AnsiString uniqueString = siteString + syndicate.SubString(1,10) + IntToStr(nextNumber) + value;

  /*TGUID uniqueId;
  CreateGUID(uniqueId);
  AnsiString uniqueString = Sysutils::GUIDToString(uniqueId);
  uniqueString = uniqueString.SubString(2,uniqueString.Length()-2);
  uniqueString = AnsiReplaceStr(uniqueString,"-","");*/
  return uniqueString;
}
//----------------------------------------------------------------------------------
 int TLoyaltyMateUtilities::StreamCheckCRC(AnsiString value)
{
    std::string data = value.c_str();
    __int32 maxBufferSize = data.size();

    __int32 itPos = 0;
    char   *buffer = new char[maxBufferSize];
    data.copy( buffer, maxBufferSize, itPos );
    TBytes bytes = CreateByteArray( buffer, maxBufferSize);

    int crc = 0;
    for (int i = 0; i < bytes.Length; i++)
        crc = (crc << 8) ^ XMODEMCRCTable[((crc >> 8) ^ bytes[i]) & 0x00FF];
    return crc & 0xFFFF;
}

 //---------------------------------------------------------------------------------
 TBytes TLoyaltyMateUtilities::CreateByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;


	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];


	}

	return result;
}


//------------------------------------------------------------------------------------
bool TLoyaltyMateUtilities::HasPendingTransactions(Database::TDBTransaction &DBTransaction,int inContactKey)
{
    bool result = true;
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();
    query->SQL->Text =  "SELECT * FROM LOYALTYPENDINGTRANSACTIONS WHERE CONTACT_KEY = :CONTACT_KEY ";
    query->ParamByName("CONTACT_KEY")->AsInteger = inContactKey;
    query->ExecQuery();
    result = !query->Eof;
    return result;
}
//------------------------------------------------------------------------------------
void TLoyaltyMateUtilities::UpdatePendingTransactions(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString paramValue)
{
    try
    {
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();
        query->SQL->Text =  "UPDATE LOYALTYPENDINGTRANSACTIONS LPT SET LPT.IS_AVAILABLE_FOR_POSTING = :IS_AVAILABLE_FOR_POSTING "
                            "WHERE LPT.CONTACT_KEY = :CONTACT_KEY  ";
        query->ParamByName("CONTACT_KEY")->AsInteger = inContactKey;
        query->ParamByName("IS_AVAILABLE_FOR_POSTING")->AsString = paramValue;
        query->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
    }
}
//------------------------------------------------------------------------------------
void TLoyaltyMateUtilities::UpdateUUID(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString uuid)
{
    try
    {
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();
        query->SQL->Text =  "UPDATE LOYALTYATTRIBUTES a SET a.UUID = :UUID "
                            "WHERE a.CONTACTS_KEY = :CONTACTS_KEY  ";
        query->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
        query->ParamByName("UUID")->AsString = uuid;
        query->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
    }
}
//------------------------------------------------------------------------------------
void TLoyaltyMateUtilities::MakeAllPendingTransactionsAvailable(UnicodeString paramValue)
{
    Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
	dBTransaction.StartTransaction();
    try
    {
        TIBSQL* query = dBTransaction.Query(dBTransaction.AddQuery());
        query->Close();
        query->SQL->Text =  "UPDATE LOYALTYPENDINGTRANSACTIONS LPT SET LPT.IS_AVAILABLE_FOR_POSTING = :IS_AVAILABLE_FOR_POSTING ";
        query->ParamByName("IS_AVAILABLE_FOR_POSTING")->AsString = paramValue;
        query->ExecQuery();
        dBTransaction.Commit();
    }
    catch(Exception &E)
    {
        dBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
    }
}
//-------------------------------------------------------------------------
bool TLoyaltyMateUtilities::IsUUIDExist(Database::TDBTransaction &DBTransaction,UnicodeString uuid)
{
   bool result = false;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM LOYALTYATTRIBUTES WHERE UUID=:UUID";
      IBInternalQuery->ParamByName("UUID")->AsString = uuid;
      IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RecordCount)
         result = true;
   }
	catch(Exception &E)
   {
	    TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
        throw;
   }
	return result;
}
//------------------------------------------------------------------------------------
