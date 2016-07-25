//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoyaltyMateInterface.h"
#include "PointsRulesSetUtils.h"
#include "DeviceRealTerminal.h"
#include "Modules.h"
#include "DateUtils.hpp"
//---------------------------------------------------------------------------

//TLoyaltyMateInterface* TLoyaltyMateInterface::instance = NULL;

TLoyaltyMateInterface::TLoyaltyMateInterface()
{
    initLMClient();
    initMembership();
    initSiteID();
    initAutoSync();
}
//---------------------------------------------------------------------------
TLoyaltyMateInterface::~TLoyaltyMateInterface()
{
    doneMembership();
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TLoyaltyMateInterface::RefreshSiteID()
{
    initSiteID();
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMember(
                                TSyndCode      syndicateCode,
                                TMMContactInfo contactInfo,
								AnsiString     &uuid )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        MemberInfo            *wcfInfo = new MemberInfo();
        CreateWcfContactInfo(wcfInfo,contactInfo);
        if(contactInfo.MemberType==2)
           wcfInfo->Activated    = true;
        else
           wcfInfo->Activated    = false;     //trip
         wcfInfo->EarnedPoints =  contactInfo.Points.getPointsBalance(ptstLoyalty);
         wcfInfo->LoadedPoints =  contactInfo.Points.getPointsBalance(ptstAccount);

        CoInitialize(NULL);

        wcfResponse =
             loyaltymateClient->SaveMember(syndicateCode.SyndCode,wcfInfo );

        if( FAutoSync
            && wcfResponse->Succesful)
        {
            contactInfo.CloudUUID = AnsiString(wcfResponse->MemberInfo->UniqueId);
            contactInfo.MemberCode = AnsiString(wcfResponse->MemberInfo->MemberCardCode);
            syncLoyaltymateAttrs( &contactInfo );
        }

        delete wcfInfo;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMember(
                                    TSyndCode      syndicateCode,
                                    AnsiString     uuid,
                                    const TMMContactInfo* const contactInfo)
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        MemberInfo            *wcfInfo = new MemberInfo();
        TMMContactInfo contact_Info = ( TMMContactInfo )( *contactInfo );
        CreateWcfContactInfo(wcfInfo,contact_Info);
        wcfInfo->UniqueId = uuid;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->SaveMember(syndicateCode.SyndCode,wcfInfo);
        delete wcfInfo;
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::DeleteMember(
                            TSyndCode  syndicateCode,
                            AnsiString uuid )
{
    try
    {
        LoyaltyResponse *wcfResponse;
        loyaltymateClient->DeleteMember(syndicateCode.SyndCode,uuid );
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetails(
                            TSyndCode       syndicateCode,
                            AnsiString      uuid,
                            TMMContactInfo &outContactInfo,
                            bool replacePoints )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse =  loyaltymateClient->GetMemberByUniqueId(syndicateCode.SyndCode,uuid );
        if(wcfResponse->Succesful)
            readContactInfo(wcfResponse,outContactInfo,replacePoints );
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByCode(
                            TSyndCode       syndicateCode,
                            AnsiString      memberCode,
                            TMMContactInfo &outContactInfo,
                            bool replacePoints )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetMemberByCardCode(syndicateCode.SyndCode, memberCode );
        if(wcfResponse->Succesful)
            readContactInfo(wcfResponse, outContactInfo, replacePoints );
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByEmail(
                                    TSyndCode       syndicateCode,
                                    AnsiString      memberEmail,
                                    TMMContactInfo &contactInfo,
                                    bool replacePoints)
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetMemberByEmail(syndicateCode.SyndCode, memberEmail );
        if(wcfResponse->Succesful)
            readContactInfo(wcfResponse, contactInfo, replacePoints );
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------  Need some changes
MMLoyaltyServiceResponse TLoyaltyMateInterface::PostTransaction(
                                            TSyndCode syndicateCode,
                                            AnsiString uuid,
                                            Currency pointsBalance,
                                            Currency pointsDelta,
                                            TDateTime occurredDate,
                                            int pointsType)
{
    try
    {
        LoyaltyResponse *wcfResponse;
        TransactionInfo *wcfInfo = new TransactionInfo();
        wcfInfo->TransactionDate = new TXSDateTime();
        wcfInfo->UniqueId         = uuid;
        wcfInfo->PointsDelta = pointsDelta;
        wcfInfo->TransactionDate->AsUTCDateTime = occurredDate;
        wcfInfo->PointsType = pointsType;
        wcfInfo->SiteCode = siteId;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->PostTransaction(syndicateCode.SyndCode,wcfInfo);
        delete wcfInfo;
        return createMMResponse( wcfResponse );
    }
    catch( Exception &exc)
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncContactInfo(
                                const TMMContactInfo* const inContactInfo )
{
    syncContactInfo( inContactInfo );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncPoints(
                                const TMMContactInfo* const inContactInfo )
{
    syncContactPoints( inContactInfo );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateTier(TSyndCode syndicateCode,TTierLevel *TierLevel)
{
    try
    {
      LoyaltyTierResponse  *wcfResponse;
      TierLevelInfo *wcfInfo = new TierLevelInfo();
      wcfInfo->TierId = 0;
      wcfInfo->Level = TierLevel->Level;
      wcfInfo->Name = TierLevel->Name;
      wcfInfo->BirthdayBonus = TierLevel->BirthDayBonus;
      wcfInfo->PointsRequired = TierLevel->PointRequired;
      wcfInfo->PricedRedemptionRate =TierLevel->PointRedemRate;
      wcfInfo->WeighedRedemptionPoints = TierLevel->WeighedRedemPoint;
      wcfInfo->WeighedRedemptionWeight = TierLevel->WeighedRedemWeight;
      wcfInfo->SendMailToHeadOffice = TierLevel->SendMailToHeadOffice;
      wcfInfo->SendMailToMember = TierLevel->SendMailToMember;
      wcfInfo->ChangeCard = TierLevel->ChangeCard;
      wcfInfo->AllowEarntLoyaltyRedemption = TierLevel->AllowEarntRedemption;
      CoInitialize(NULL);
      wcfResponse = loyaltymateClient->SaveTierLevel(syndicateCode.SyndCode,wcfInfo );
      if( FAutoSync  && wcfResponse->Succesful)
        {
             TierLevel->CloudId = wcfResponse->TierInfo->TierId;
             UpdateTierCloudId(TierLevel);
        }
      delete wcfInfo;
      return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateTier(TSyndCode syndicateCode,TTierLevel *TierLevel)
{
    try
    {
      LoyaltyTierResponse *wcfResponse;
      TierLevelInfo *wcfInfo = new TierLevelInfo();
      wcfInfo->TierId = GetTierCloudId(TierLevel->Level);
      wcfInfo->Level = TierLevel->Level;
      wcfInfo->Name = TierLevel->Name;
      wcfInfo->BirthdayBonus = TierLevel->BirthDayBonus;
      wcfInfo->PointsRequired = TierLevel->PointRequired;
      wcfInfo->PricedRedemptionRate =TierLevel->PointRedemRate;
      wcfInfo->WeighedRedemptionPoints = TierLevel->WeighedRedemPoint;
      wcfInfo->WeighedRedemptionWeight = TierLevel->WeighedRedemWeight;
      wcfInfo->SendMailToHeadOffice = TierLevel->SendMailToHeadOffice;
      wcfInfo->SendMailToMember = TierLevel->SendMailToMember;
      wcfInfo->ChangeCard = TierLevel->ChangeCard;
      wcfInfo->AllowEarntLoyaltyRedemption = TierLevel->AllowEarntRedemption;
      CoInitialize(NULL);
      wcfResponse = loyaltymateClient->SaveTierLevel(syndicateCode.SyndCode,wcfInfo);
      if( FAutoSync  && wcfResponse->Succesful)
        {
             TierLevel->CloudId = wcfResponse->TierInfo->TierId;
             UpdateTierCloudId(TierLevel);
        }
      delete wcfInfo;
      return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::DeleteTier(TSyndCode syndicateCode,TTierLevel *TierLevel)
{
    try
    {
        LoyaltyResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->DeleteTierLevel(syndicateCode.SyndCode,TierLevel->CloudId);
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetTier(TSyndCode syndicateCode,TTierLevel *TierLevel)
{
    try
    {
        LoyaltyTierResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetTierLevel(syndicateCode.SyndCode,TierLevel->CloudId );
        if(wcfResponse->Succesful)
         {
            readTierInfo(wcfResponse->TierInfo, TierLevel);
         }
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::SyncTierLevels(TSyndCode syndicateCode)
{
    try
    {
        LoyaltyTierListResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetAllTierLevel(syndicateCode.SyndCode);
        if(wcfResponse->Succesful)
         {
            syncTierLevels(wcfResponse->TierLevelList);
         }
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMemberCardCode(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode)
{
    try
    {
        LoyaltyResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->UpdateMemberCardCode(syndicateCode.SyndCode,uniqueId,memberCardCode);
        return createMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return createExceptionFailedResponse( exc.Message );
    }
}



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TLoyaltyMateInterface::initLMClient()
{
    bool useWSDL = false;
    AnsiString loyaltyMateURL = AnsiString(TGlobalSettings::Instance().ServiceURL);

    loyaltymateClient = GetIWCFServiceLoyaltyMate(
                            useWSDL, loyaltyMateURL, NULL );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::initMembership()
{
    membership = new TMembership( TDeviceRealTerminal::Instance().Modules );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::initSiteID()
{
    siteId = getCurrentSiteId();
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::initAutoSync()
{
    FAutoSync = true;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::doneMembership()
{
    try
    {
        delete membership;
    }
    catch( ... )
    {
    }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::CreateWcfContactInfo(MemberInfo* inMemberInfo,TMMContactInfo& inContactInfo)
{
    inMemberInfo->DateOfBirth = new TXSDateTime();
    inMemberInfo->YearStartDate = new TXSDateTime();
    inMemberInfo->LastVisitDate = new TXSDateTime();
    inMemberInfo->BirthdayBenefitDate = new TXSDateTime();
    inMemberInfo->FirstName = inContactInfo.Name;
    inMemberInfo->LastName =  inContactInfo.Surname;
    inMemberInfo->Mobile    = inContactInfo.Mobile;
    inMemberInfo->PhoneNumber    = inContactInfo.Phone;
    inMemberInfo->Email = inContactInfo.EMail;
    inMemberInfo->EarnedPoints = inContactInfo.Points.getPointsEarned();
    inMemberInfo->LoadedPoints = inContactInfo.Points.getPointsPurchased();
    inMemberInfo->Address1 = inContactInfo.LocationAddress;
    inMemberInfo->Title = inContactInfo.Title;
    inMemberInfo->HomeSiteId = inContactInfo.SiteID;
    inMemberInfo->MemberCardCode = inContactInfo.MemberCode;
    inMemberInfo->MemberType = inContactInfo.MemberType;
    inMemberInfo->TierId = GetTierCloudId(inContactInfo.TierLevel);
    inMemberInfo->MembershipNumber = inContactInfo.MembershipNumber;
    inMemberInfo->PointRule = TPointsRulesSetUtils().Compress(inContactInfo.Points.PointsRules);
    inMemberInfo->IsFirstVisitRewarded = inContactInfo.IsFirstVisitRewarded;
    if((double)inContactInfo.ActivationDate != 0)
    {
       inMemberInfo->YearStartDate->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(inContactInfo.ActivationDate),
       MonthOf(inContactInfo.ActivationDate),DayOf(inContactInfo.ActivationDate),HourOf(inContactInfo.ActivationDate),MinuteOf(inContactInfo.ActivationDate),
       SecondOf(inContactInfo.ActivationDate),MilliSecondOf(inContactInfo.ActivationDate));

    }
    if((double)inContactInfo.DateOfBirth != 0)
    {
       inMemberInfo->DateOfBirth->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(inContactInfo.DateOfBirth),
       MonthOf(inContactInfo.DateOfBirth),DayOf(inContactInfo.DateOfBirth),HourOf(inContactInfo.DateOfBirth),MinuteOf(inContactInfo.DateOfBirth),
       SecondOf(inContactInfo.DateOfBirth),MilliSecondOf(inContactInfo.DateOfBirth));
    }
    if((double)inContactInfo.LastVisit != 0)
     {
       inMemberInfo->LastVisitDate->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(inContactInfo.LastVisit),
       MonthOf(inContactInfo.LastVisit), DayOf(inContactInfo.LastVisit),HourOf(inContactInfo.LastVisit),MinuteOf(inContactInfo.LastVisit),
       SecondOf(inContactInfo.LastVisit),MilliSecondOf(inContactInfo.LastVisit));

     }
    if((double)inContactInfo.LastBirthdayProcessed != 0)
       inMemberInfo->BirthdayBenefitDate->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(inContactInfo.LastBirthdayProcessed),
       MonthOf(inContactInfo.LastBirthdayProcessed),DayOf(inContactInfo.LastBirthdayProcessed),HourOf(inContactInfo.LastBirthdayProcessed),MinuteOf(inContactInfo.LastBirthdayProcessed),
       SecondOf(inContactInfo.LastBirthdayProcessed),MilliSecondOf(inContactInfo.LastBirthdayProcessed));
}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::getCurrentSiteId()
{
    return TGlobalSettings::Instance().SiteID;
}
//---------------------------------------------------------------------------
AnsiString TLoyaltyMateInterface::createWCFName( const TMMContactInfo* const inContactInfo )
{
    return inContactInfo->Name + " " + inContactInfo->Surname;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::readContactInfo(
                                LoyaltyMemberResponse* inWCFResponse,
                                TMMContactInfo&            outContactInfo,
                                bool replacePoints )
{
    readContactInfo(
        inWCFResponse->MemberInfo,
        outContactInfo,
        replacePoints );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::readContactInfo(
                                MemberInfo* inMemberInfo,
                                TMMContactInfo& inContactInfo,
                                bool replacePoints )
{
    inContactInfo.CloudUUID   = inMemberInfo->UniqueId;
    inContactInfo.Mobile      = inMemberInfo->Mobile;
    inContactInfo.Phone      = inMemberInfo->PhoneNumber;
    inContactInfo.EMail       = inMemberInfo->Email;
    inContactInfo.Name        = inMemberInfo->FirstName;
    inContactInfo.Surname     = inMemberInfo->LastName;
    inContactInfo.TierLevel =  GetTierLevelFromCloudId(inMemberInfo->TierId);
    inContactInfo.LocationAddress = inMemberInfo->Address1 + ", " + inMemberInfo->Address2;;
    inContactInfo.Title = inMemberInfo->Title;
    inContactInfo.SiteID = inMemberInfo->HomeSiteId;
    if(inContactInfo.SiteID == 0)
    {
      inContactInfo.SiteID = getCurrentSiteId();
    }
    inContactInfo.MemberType = inMemberInfo->MemberType;
    inContactInfo.CurrentYearPoint = inMemberInfo->CurrentYearPoint;
    inContactInfo.PreviousYearPoint = inMemberInfo->PreviousYearPoint;
    inContactInfo.AvailableBDPoint = inMemberInfo->AvailableBirthDayPoint;
    inContactInfo.AvailableFVPoint = inMemberInfo->AvailableFirstVisitPoint;
    inContactInfo.MemberCode = inMemberInfo->MemberCardCode;
    inContactInfo.MembershipNumber = inMemberInfo->MembershipNumber;
	inContactInfo.PointRule = inMemberInfo->PointRule;
    inContactInfo.IsFirstVisitRewarded = inMemberInfo->IsFirstVisitRewarded;
    if(inMemberInfo->LastVisitDate != NULL)
    {
       inContactInfo.LastVisit = inMemberInfo->LastVisitDate->AsUTCDateTime;
    }
    if(inMemberInfo->BirthdayBenefitDate != NULL)
       inContactInfo.LastBirthdayProcessed =  inMemberInfo->BirthdayBenefitDate->AsUTCDateTime;
    if(inMemberInfo->DateOfBirth != NULL)
        inContactInfo.DateOfBirth = inMemberInfo->DateOfBirth->AsUTCDateTime;
    if(inMemberInfo->YearStartDate != NULL)
        inContactInfo.ActivationDate = inMemberInfo->YearStartDate->AsUTCDateTime;
    if(inMemberInfo->LastModified != NULL)
     {
       inContactInfo.LastModified = Dateutils::EncodeDateTime(inMemberInfo->LastModified->Year,
       inMemberInfo->LastModified->Month,inMemberInfo->LastModified->Day,inMemberInfo->LastModified->Hour,inMemberInfo->LastModified->Minute,
       inMemberInfo->LastModified->Second,inMemberInfo->LastModified->Millisecond);

     }

    if(replacePoints) // Do we want to replace points or simply add them on to what we already have?
    {
        inContactInfo.Points.Clear();

        // Putting in the Points Earned.
        TPointsTypePair typepair1( pttEarned,ptstLoyalty );
	    TPointsType type1( pasCloud, typepair1, pesNone);
        inContactInfo.Points.Load( type1, inMemberInfo->EarnedPoints );

        // Putting in the Points Loaded ( Purchased ).
        TPointsTypePair typepair2( pttPurchased,ptstAccount );
	    TPointsType type2( pasCloud, typepair2, pesNone );
        inContactInfo.Points.Load( type2, inMemberInfo->LoadedPoints );
    }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::readTierInfo(TierLevelInfo* inTierInfo,TTierLevel *TierLevel)
{
      TierLevel->CloudId = inTierInfo->TierId ;
      TierLevel->Level = inTierInfo->Level ;
      TierLevel->Name = inTierInfo->Name ;
      TierLevel->BirthDayBonus = inTierInfo->BirthdayBonus ;
      TierLevel->PointRequired = inTierInfo->PointsRequired ;
      TierLevel->PointRedemRate = inTierInfo->PricedRedemptionRate;
      TierLevel->WeighedRedemPoint = inTierInfo->WeighedRedemptionPoints ;
      TierLevel->WeighedRedemWeight = inTierInfo->WeighedRedemptionWeight ;
      TierLevel->SendMailToHeadOffice = inTierInfo->SendMailToHeadOffice ;
      TierLevel->SendMailToMember = inTierInfo->SendMailToMember ;
      TierLevel->ChangeCard = inTierInfo->ChangeCard ;
      TierLevel->AllowEarntRedemption = inTierInfo->AllowEarntLoyaltyRedemption;
}

void TLoyaltyMateInterface::syncTierLevels(DynamicArray<TierLevelInfo*> tierLevels)
{
      TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );
      transaction.StartTransaction();
      try
        {
           TDBTierLevel::ClearAll(transaction);
           for(int i = 0; i< tierLevels.Length;i++)
           {
             TierLevelInfo* tierLevelInfo = tierLevels[i];
             TTierLevel* tierLevel = new TTierLevel;
             readTierInfo(tierLevelInfo,tierLevel);
             TDBTierLevel::AddTierLevel(transaction,tierLevel);
           }
           transaction.Commit();
	   }
      catch( Exception &exc )
        {
            transaction.Rollback();
            TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
            throw;
        }
}


void TLoyaltyMateInterface::syncContactInfo(const TMMContactInfo* const inContactInfo )
{
	try
    {
        syncLoyaltymateAttrs( inContactInfo );

        TDBControl dbControl = TDeviceRealTerminal::Instance().DBControl;
        Database::TDBTransaction dbTransaction( dbControl );

        TMMContactInfo contactInfo = ( TMMContactInfo )( *inContactInfo );

		membership->SetContactDetails(
                        dbTransaction,
                        contactInfo.ContactKey,
                        contactInfo );
	}
	catch( Exception &exc )
    {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::syncLoyaltymateAttrs(
                                const TMMContactInfo* const inContactInfo )
{
    if( !updateLoyaltymateAttrs( inContactInfo ) )
    {
        addLoyaltymateAttrs( inContactInfo );
    }
}
//---------------------------------------------------------------------------
bool TLoyaltyMateInterface::updateLoyaltymateAttrs(
                                const TMMContactInfo* const inContactInfo )
{
    //TDBControl dbControl = TDeviceRealTerminal::Instance().DBControl;

    if( ( inContactInfo->CloudUUID != NULL ) &&
        ( inContactInfo->CloudUUID != ""   ) )
    {
        AnsiString sql = "UPDATE LOYALTYATTRIBUTES "
                         "SET UUID    = '"        + inContactInfo->CloudUUID + "', "
                         "ACTIVATION_TOKEN  = '"        + inContactInfo->ActivationToken + "', "
                         "SITE_ID = "        + IntToStr(inContactInfo->SiteID) + " "
                         "WHERE CONTACTS_KEY = " + IntToStr(inContactInfo->ContactKey)  + "";

        //return executeQuery( sql, &dbControl ) > 0;
        return executeQuery( sql, &TDeviceRealTerminal::Instance().DBControl) > 0;
    }

    return false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::addLoyaltymateAttrs(
                                const TMMContactInfo* const inContactInfo )
{
    //TDBControl dbControl = TDeviceRealTerminal::Instance().DBControl;

    //int key = generateTableKey( "GEN_LOYALTYATTRIBUTES", &dbControl );
    int key = generateTableKey( "GEN_LOYALTYATTRIBUTES", &TDeviceRealTerminal::Instance().DBControl );

    AnsiString sql = "INSERT INTO LOYALTYATTRIBUTES "
                     "("
                        "ATTRIB_KEY, "
                        "UUID, "
                        "SITE_ID, "
                        "CONTACTS_KEY, "
                        "ACTIVATION_TOKEN "
                     ") "
                     "VALUES "
                     "("
                        "" + IntToStr(key) + ", "
                        "'" + inContactInfo->CloudUUID + "', "
                        "" + IntToStr(inContactInfo->SiteID) + ", "
                        "" + IntToStr(inContactInfo->ContactKey) + ","
                        "'" + inContactInfo->ActivationToken + "'"
                     ")";

	//executeQuery( sql, &dbControl );
    executeQuery( sql , &TDeviceRealTerminal::Instance().DBControl);
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::syncContactPoints(
                                const TMMContactInfo* const inContactInfo )
{
/*
 	try
    {
        TDBControl dbControl = TDeviceRealTerminal::Instance().DBControl;
        Database::TDBTransaction dbTransaction( dbControl );

        TMMContactInfo contactInfo = ( TMMContactInfo )( *inContactInfo );

		Currency CurrentLocalBalance =
                membership->LoyaltyGetValue( dbTransaction, contactInfo.ContactKey );

		Currency SyncAdjustment =
                contactInfo.Points.getPointsBalance() - CurrentLocalBalance;

		if( SyncAdjustment != 0 )
        {
			TPointsTransaction pointsTransaction;

			pointsTransaction.ContactKey = contactInfo.ContactKey;
			pointsTransaction.TimeStamp = Now();
			pointsTransaction.ExportStatus = pesNone;
			pointsTransaction.PointsTransactionType = pttSync;
			pointsTransaction.PointsTransactionAccountType = ptstLoyalty;
			pointsTransaction.Adjustment = SyncAdjustment;
			pointsTransaction.InvoiceNumber = "NA LoyaltyMate Sync";

			membership->LoyaltyAddValue( dbTransaction, pointsTransaction);
		}
	}
	catch( Exception &exc )
    {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
		throw;
	}
*/

/*
    // Putting in the Points Earned.
    TPointsTypePair typepair( pttEarned,ptstLoyalty );
    TPointsType type( pasDatabase, typepair, pesExported);
    inContactInfo->Points.Load( type, inEarnedPoints );

    // Putting in the Points Loaded ( Purchased ).
    TPointsTypePair typepair( pttPurchased,ptstAccount );
    TPointsType type( pasDatabase, typepair, pesExported );
    inContactInfo->Points.Load( type, inLoadedPoints );

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TDBControl dbControl = TDeviceRealTerminal::Instance().DBControl;
    Database::TDBTransaction dbTransaction( dbControl );

    inContactInfo.ContactKey = membership->GetInternalKey( dbTransaction, inContactInfo->ExternalKey );
    membership->SetContactDetails( dbTransaction, inContactInfo->ContactKey, *inContactInfo );
    membership->SyncPointsDetails( dbTransaction, *inContactInfo );

    // Points are all synced now discard the Bally ones.
    // they will be reloaded as part of Get contact details
    inContactInfo->Points.Clear();
    inContactInfo->Points.PointsRules << eprNoPointsPurchases;
    inContactInfo->Points.PointsRules >> eprAllowedNegitive;
*/
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyResponse* inWCFResponse )
{
    return MMLoyaltyServiceResponse(
                inWCFResponse->Succesful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyMemberResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Succesful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                AnsiString( inWCFResponse->MemberInfo->UniqueId.t_str() ) );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyMemberListResponse* inWCFResponse )
{
    return MMLoyaltyServiceResponse(
                    inWCFResponse->Succesful,
                    AnsiString( inWCFResponse->Message.t_str() ),
                    AnsiString( inWCFResponse->Description.t_str() ),
                    ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                    "" );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyPointsInfoResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Succesful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                AnsiString( inWCFResponse->PointsInfo->UniqueId.t_str() ) );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyTierResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Succesful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                AnsiString( inWCFResponse->TierInfo->Name.t_str() ) );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createMMResponse(
                            LoyaltyTierListResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Succesful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::createExceptionFailedResponse(
                            AnsiString inMessage )
{
    return MMLoyaltyServiceResponse(
                    false,
                    inMessage,
                    "",
                    FailedDueToException,
                    AnsiString( "" ) );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::UpdateTierCloudId(const TTierLevel* const inTierInfo )
{
  if( ( inTierInfo->CloudId != NULL ) &&
        ( inTierInfo->TierKey != 0   ) )
    {
        AnsiString sql = "UPDATE TIER_LEVELS "
                         "SET CLOUD_ID    = '" + IntToStr(inTierInfo->CloudId) + "' "
                         "WHERE TIER_ID = " + IntToStr(inTierInfo->TierKey)  + "";
        executeQuery( sql, &TDeviceRealTerminal::Instance().DBControl);
    }

}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::generateTableKey(
                                const AnsiString  inGeneratorName,
                                TDBControl* const inDBControl )
{
    int result = -1;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TDBTransaction transaction( *inDBControl );

    try
    {
        transaction.StartTransaction();
        TIBSQL *query = transaction.Query( transaction.AddQuery() );

        query->SQL->Text =
            "SELECT GEN_ID(" + inGeneratorName + ", 1) FROM RDB$DATABASE";

            query->ExecQuery();

            result = query->Fields[0]->AsInteger;
        transaction.Commit();
    }
    catch( Exception &exc )
    {
        transaction.Rollback();
		TManagerLogs::Instance().Add( __FUNC__, ERRORLOG, exc.Message );
        throw;
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::executeQuery(
                                const AnsiString inSQL,
                                TDBControl* const inDBControl )
{
    int result = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TDBTransaction transaction( *inDBControl );

    try
    {
        transaction.StartTransaction();
        TIBSQL *query = transaction.Query( transaction.AddQuery() );

        query->SQL->Text = UnicodeString( inSQL.c_str() );

            query->ExecQuery();

            result = query->RowsAffected;
        transaction.Commit();
    }
    catch( Exception &exc )
    {
        transaction.Rollback();
		TManagerLogs::Instance().Add( __FUNC__, ERRORLOG, exc.Message );
        throw;
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::GetTierCloudId(int tierLevel)
{
  int tierCloudId = 0;
  TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );
  transaction.StartTransaction();
  tierCloudId = TDBTierLevel::GetTierCloudId(transaction,tierLevel);
  transaction.Commit();
  return tierCloudId;
}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::GetTierLevelFromCloudId(int cloudId)
{
  int tierLevel = 0;
  TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl  );
  transaction.StartTransaction();
  tierLevel = TDBTierLevel::GetTierLevelFromCloudId(transaction,cloudId);
  transaction.Commit();
  return tierLevel;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
