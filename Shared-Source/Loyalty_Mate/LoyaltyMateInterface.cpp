//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoyaltyMateInterface.h"
#include "PointsRulesSetUtils.h"
#include "DeviceRealTerminal.h"
#include "Modules.h"
#include "DateUtils.hpp"
#include "ManagerDiscount.h"
#include "DBMenu.h"
#include "Payment.h"
#include <map>

//---------------------------------------------------------------------------

//TLoyaltyMateInterface* TLoyaltyMateInterface::instance = NULL;

TLoyaltyMateInterface::TLoyaltyMateInterface()
{
    InitLMClient();
    InitSiteID();
    InitAutoSync();
}
//---------------------------------------------------------------------------
TLoyaltyMateInterface::~TLoyaltyMateInterface()
{
//
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC SECTION
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TLoyaltyMateInterface::RefreshSiteID()
{
    InitSiteID();
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMember(TSyndCode syndicateCode,TMMContactInfo &contactInfo,AnsiString &uuid )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        MemberInfo *wcfInfo = new MemberInfo();
        CreateWcfContactInfo(wcfInfo,contactInfo);
        if(contactInfo.MemberType==2)
            wcfInfo->Activated    = true;
        else
            wcfInfo->Activated    = false;     //trip

        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->SaveMember(syndicateCode.GetSyndCode(),wcfInfo );
        if( FAutoSync && wcfResponse->Successful)
        {
            contactInfo.CloudUUID = AnsiString(wcfResponse->MemberInfo->UniqueId);
            contactInfo.MemberCode = AnsiString(wcfResponse->MemberInfo->MemberCardCode);
            SyncLoyaltymateAttrs( &contactInfo );
            ReadContactInfo(wcfResponse, contactInfo, true );
        }
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMember(TSyndCode syndicateCode,AnsiString uuid,const TMMContactInfo* const contactInfo)
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        MemberInfo            *wcfInfo = new MemberInfo();
        TMMContactInfo contact_Info = ( TMMContactInfo )( *contactInfo );
        CreateWcfContactInfo(wcfInfo,contact_Info);
        wcfInfo->UniqueId = uuid;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->SaveMember(syndicateCode.GetSyndCode(),wcfInfo);
        delete wcfInfo;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetails(TSyndCode syndicateCode,AnsiString uuid,TMMContactInfo &outContactInfo,bool replacePoints )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse =  loyaltymateClient->GetMemberByUniqueId(syndicateCode.GetSyndCode(),CreateRequest(uuid));
        if(wcfResponse->Successful)
            ReadContactInfo(wcfResponse,outContactInfo,replacePoints );
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByCode(TSyndCode syndicateCode, AnsiString memberCode,TMMContactInfo &outContactInfo,bool replacePoints )
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetMemberByCardCode(syndicateCode.GetSyndCode(), CreateRequest(memberCode));
        if(wcfResponse->Successful)
            ReadContactInfo(wcfResponse, outContactInfo, replacePoints );
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByEmail(TSyndCode syndicateCode,AnsiString memberEmail,TMMContactInfo &contactInfo,
                                    bool replacePoints)
{
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetMemberByEmail(syndicateCode.GetSyndCode(), CreateRequest(memberEmail));
        if(wcfResponse->Successful)
            ReadContactInfo(wcfResponse, contactInfo, replacePoints );
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------  Need some changes
MMLoyaltyServiceResponse TLoyaltyMateInterface::PostTransaction(TSyndCode syndicateCode,AnsiString uuid,Currency pointsBalance,Currency pointsDelta,
                                            TDateTime occurredDate,int pointsType,AnsiString inInvoiceNumber)
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
        wcfInfo->InvoiceNumber = inInvoiceNumber;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->PostTransaction(syndicateCode.GetSyndCode(),wcfInfo);
        delete wcfInfo;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception &exc)
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::SyncCompanyDetails(TSyndCode syndicateCode)
{
    try
    {
        LoyaltyCompanyResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetCompanyInformation(syndicateCode.GetSyndCode());
        if(wcfResponse->Successful)
         {
            SyncCompanyInfo(wcfResponse->CompanyInfo);
         }
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMemberCardCode(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode)
{
    try
    {
        LoyaltyResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->UpdateMemberCardCode(syndicateCode.GetSyndCode(),uniqueId,memberCardCode);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetGiftVoucherBalance(TSyndCode syndicateCode,AnsiString giftVoucherNumber,TGiftCardDetail &GiftCardDetail)
{
  try
    {
        LoyaltyGiftCardResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetGiftCardBalance(syndicateCode.GetSyndCode(),CreateRequest(giftVoucherNumber));
        if(wcfResponse->Successful)
           ReadGiftCardInfo(wcfResponse->GiftCardInfo, GiftCardDetail );

        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetPocketVoucherDetail(TSyndCode syndicateCode,AnsiString pocketVoucherNumber,TVoucherDetail& VoucherDetail)
{
  try
    {
        LoyaltyVoucherResponse *wcfResponse;
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->GetPocketVoucherDetail(syndicateCode.GetSyndCode(), CreateRequest(pocketVoucherNumber));
        if(wcfResponse->Successful)
        {
            VoucherDetail.VoucherNumber = pocketVoucherNumber;
            ReadPocketVoucherInfo(wcfResponse->VoucherInfo, VoucherDetail );
        }
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::ProcessVoucherTransaction(TSyndCode syndicateCode,TVoucherUsageDetail& inVoucherUsageDetail)
{
    try
    {
        VoucherTransactionResponse *wcfResponse;
        VoucherTransactionInfo *wcfInfo = new VoucherTransactionInfo();
        wcfInfo->TransactionDate = new TXSDateTime();
        wcfInfo->TransactionReferenceNumber = inVoucherUsageDetail.ReferenceNumber;
        wcfInfo->GiftCardNumber = inVoucherUsageDetail.GiftCardNumber;
        wcfInfo->PointsRedeemed = inVoucherUsageDetail.PointsRedeemed;
        wcfInfo->PurchasedGiftCardNumber = inVoucherUsageDetail.PurchasedGiftCardNumber;
        wcfInfo->PointsPurchased = inVoucherUsageDetail.PointsPurchased;
        wcfInfo->VoucherName = inVoucherUsageDetail.VoucherName;
        wcfInfo->MemberVoucherDiscountAmount = RoundToNearest(inVoucherUsageDetail.MemberVoucherDiscountAmount,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
        wcfInfo->PocketVoucherNumber = inVoucherUsageDetail.PocketVoucherNumber;
        wcfInfo->PocketVoucherDiscountAmount = inVoucherUsageDetail.PocketVoucherDiscountAmount;
        wcfInfo->TotalSaleAmount = inVoucherUsageDetail.TotalSaleAmount;
        wcfInfo->SiteCode = GetCurrentSiteId();
        wcfInfo->InvoiceNumber =  inVoucherUsageDetail.InvoiceNumber;
        TDateTime transactionDate =  Now();
        wcfInfo->TransactionDate->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(transactionDate),MonthOf(transactionDate),DayOf(transactionDate),
                                                             HourOf(transactionDate),MinuteOf(transactionDate),SecondOf(transactionDate),MilliSecondOf(transactionDate));
        wcfInfo->MemberUniqueId = inVoucherUsageDetail.MemberUniqueId;
        if(!inVoucherUsageDetail.DiscountUsage.empty())
        {
           ArrayOfDiscountUsageInfo DiscountUsageArray;
           for(std::map<AnsiString,double>::iterator itDiscount = inVoucherUsageDetail.DiscountUsage.begin();
            itDiscount != inVoucherUsageDetail.DiscountUsage.end(); ++itDiscount)
            {
               DiscountUsageInfo* discountUsageInfo = new DiscountUsageInfo;
               discountUsageInfo->DiscountCode = itDiscount->first;
               discountUsageInfo->DiscountAmount = RoundToNearest(itDiscount->second,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
               DiscountUsageArray.Length = DiscountUsageArray.Length + 1;
               DiscountUsageArray[DiscountUsageArray.Length - 1] = discountUsageInfo;
            }
           wcfInfo->DiscountUsages = DiscountUsageArray;
        }

        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->ProcessVoucherTransaction(syndicateCode.GetSyndCode(),wcfInfo);
        if(wcfResponse->Successful && wcfResponse->GiftCardExpiryDate != NULL)
        {
           inVoucherUsageDetail.GiftCardExpiryDate = wcfResponse->GiftCardExpiryDate->AsUTCDateTime;
        }
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::ReleaseVouchers(TSyndCode syndicateCode,TReleasedVoucherDetail inReleasedVoucherDetail)
{
    try
    {
        LoyaltyResponse *wcfResponse;
        ReleasedVoucherInfo *wcfInfo = new ReleasedVoucherInfo();
        wcfInfo->TransactionReferenceNumber = inReleasedVoucherDetail.ReferenceNumber;
        wcfInfo->GiftCardNumber = inReleasedVoucherDetail.GiftCardNumber;
        wcfInfo->VoucherName = inReleasedVoucherDetail.VoucherName;
        wcfInfo->PocketVoucherNumber = inReleasedVoucherDetail.PocketVoucherNumber;
        if(!inReleasedVoucherDetail.DiscountUsage.empty())
        {
           ArrayOfDiscountUsageInfo DiscountUsageArray;
           for(std::vector<AnsiString>::iterator itDiscount = inReleasedVoucherDetail.DiscountUsage.begin();
            itDiscount != inReleasedVoucherDetail.DiscountUsage.end(); ++itDiscount)
            {
               DiscountUsageInfo* discountUsageInfo = new DiscountUsageInfo;
               discountUsageInfo->DiscountCode = *itDiscount;
               DiscountUsageArray.Length = DiscountUsageArray.Length + 1;
               DiscountUsageArray[DiscountUsageArray.Length - 1] = discountUsageInfo;
            }
           wcfInfo->DiscountCodes = DiscountUsageArray;
        }
        CoInitialize(NULL);
        wcfResponse = loyaltymateClient->ReleaseVouchers(syndicateCode.GetSyndCode(),wcfInfo);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TLoyaltyMateInterface::InitLMClient()
{
    bool useWSDL = false;
    AnsiString loyaltyMateURL = AnsiString(TGlobalSettings::Instance().ServiceURL);
    loyaltymateClient = GetIWCFServiceLoyaltyMate(useWSDL, loyaltyMateURL, NULL );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::InitSiteID()
{
    siteId = GetCurrentSiteId();
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::InitAutoSync()
{
    FAutoSync = true;
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
int TLoyaltyMateInterface::GetCurrentSiteId()
{
    return TGlobalSettings::Instance().SiteID;
}
//---------------------------------------------------------------------------
AnsiString TLoyaltyMateInterface::CreateWCFName( const TMMContactInfo* const inContactInfo )
{
    return inContactInfo->Name + " " + inContactInfo->Surname;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadPocketVoucherInfo(VoucherInfo* inVoucherInfo,TVoucherDetail& VoucherDetail)
{
    VoucherDetail.VoucherName  = inVoucherInfo->VoucherName;
    VoucherDetail.DiscountCode  = inVoucherInfo->DiscountCode;
    VoucherDetail.NumberOfUsesRemaining  = inVoucherInfo->NumberOfUsesRemaining;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadGiftCardInfo(GiftCardInfo* inVoucherInfo,TGiftCardDetail& GiftCardDetail)
{
    GiftCardDetail.ResponseMessage  = inVoucherInfo->ResponseMessage;
    GiftCardDetail.StatusCode  = inVoucherInfo->StatusCode;
    GiftCardDetail.PointBalance  = inVoucherInfo->PointBalance;
    if(inVoucherInfo->ExpiryDate != NULL)
     {
       GiftCardDetail.ExpiryDate = Dateutils::EncodeDateTime(inVoucherInfo->ExpiryDate->Year,
       inVoucherInfo->ExpiryDate->Month,inVoucherInfo->ExpiryDate->Day,inVoucherInfo->ExpiryDate->Hour,inVoucherInfo->ExpiryDate->Minute,
       inVoucherInfo->ExpiryDate->Second,inVoucherInfo->ExpiryDate->Millisecond);

     }

     if(inVoucherInfo->StartDate != NULL)
     {
       GiftCardDetail.StartDate = Dateutils::EncodeDateTime(inVoucherInfo->StartDate->Year,
       inVoucherInfo->StartDate->Month,inVoucherInfo->StartDate->Day,inVoucherInfo->StartDate->Hour,inVoucherInfo->StartDate->Minute,
       inVoucherInfo->StartDate->Second,inVoucherInfo->StartDate->Millisecond);

     }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadContactInfo(LoyaltyMemberResponse* inWCFResponse,TMMContactInfo& outContactInfo,bool replacePoints )
{
    ReadContactInfo(inWCFResponse->MemberInfo,outContactInfo,replacePoints );
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadContactInfo(MemberInfo* inMemberInfo,TMMContactInfo& inContactInfo,bool replacePoints )
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
      inContactInfo.SiteID = GetCurrentSiteId();
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
    inContactInfo.HasTransactions = inMemberInfo->HasTransactions;
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
    ReadMemberVouchers(inMemberInfo->MemberVouchers,inContactInfo);
    if(replacePoints) // Do we want to replace points or simply add them on to what we already have?
    {
        inContactInfo.Points.ClearPoints();

        // Putting in the Points Earned.
        TPointsTypePair typepair1( pttEarned,ptstLoyalty );
	    TPointsType type1( pasCloud, typepair1, pesNone);
        inContactInfo.Points.Load( type1, inMemberInfo->EarnedPoints );

        // Putting in the Points Loaded ( Purchased ).
        if(TGlobalSettings::Instance().EnableSeperateEarntPts)
        {
            TPointsTypePair typepair2( pttPurchased,ptstAccount );
            TPointsType type2( pasCloud, typepair2, pesNone );
            inContactInfo.Points.Load( type2, inMemberInfo->LoadedPoints );
        }
        else
        {
            TPointsTypePair typepair2( pttPurchased,ptstLoyalty );
            TPointsType type2( pasCloud, typepair2, pesNone );
            inContactInfo.Points.Load( type2, inMemberInfo->LoadedPoints );
        }

         // Putting in the Birthday Points .
        TPointsTypePair typepair3( pttBirthdayBonus,ptstLoyalty );
	    TPointsType type3( pasCloud, typepair3, pesNone);
        inContactInfo.Points.Load( type3, inMemberInfo->AvailableBirthDayPoint );

         // Putting in the First Vist Points.
        TPointsTypePair typepair4( pttFirstVisit,ptstLoyalty );
	    TPointsType type4( pasCloud, typepair4, pesNone);
        inContactInfo.Points.Load( type4, inMemberInfo->AvailableFirstVisitPoint);
    }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadMemberVouchers(DynamicArray<VoucherInfo*> memberVouchers,TMMContactInfo& inContactInfo)
{
    for(int i = 0; i< memberVouchers.Length;i++)
       {
         VoucherInfo* voucherInfo = memberVouchers[i];
         TVoucherDetail voucherDetail;
         voucherDetail.VoucherName = voucherInfo->VoucherName;
         voucherDetail.VoucherDescription = voucherInfo->VoucherDescription;
         voucherDetail.DiscountCode = voucherInfo->DiscountCode;
         voucherDetail.NumberOfUsesRemaining = voucherInfo->NumberOfUsesRemaining;
         inContactInfo.MemberVouchers.push_back(voucherDetail);
       }
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
void TLoyaltyMateInterface::ReadTierInfo(TierLevelInfo* inTierInfo,TTierLevel *TierLevel)
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
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ReadDiscountInfo(Database::TDBTransaction &DBTransaction,DiscountInfo* inDiscountInfo,TDiscount& discount)
{
  discount.Name =  inDiscountInfo->Name;
  discount.DiscountCode = inDiscountInfo->Code;
  discount.Description = inDiscountInfo->Description;
  discount.Rounding = StrToCurr(inDiscountInfo->RoundToDecimalPlaces->DecimalString);
  discount.Priority = inDiscountInfo->PriorityOrder;
  discount.AppearanceOrder = inDiscountInfo->AppearanceOrder;
  discount.MembersExempt = inDiscountInfo->IsMemberExemptDiscount;
  discount.MembersOnly = inDiscountInfo->IsMembersOnlyDiscount;
  discount.MaximumValue = fabs(StrToCurr(inDiscountInfo->MaximumValue->DecimalString));
  discount.Group = inDiscountInfo->DiscountGroup;
  discount.MaxItemAffected = inDiscountInfo->MaximumNumberOfItemsAllowed;
  discount.MinItemRequired = inDiscountInfo->MinimumNumberOfItemsAllowed;
  discount.IsCloudDiscount = true;
  discount.DailyUsageAllowedPerMember = inDiscountInfo->DailyUsageAllowedPerMember;
  discount.IsMembershipDiscount = inDiscountInfo->IsAutoMembersDiscount;
  switch((MMProductPriority)inDiscountInfo->ProductPriority)
  {
    case LowestPriceFirst:
    discount.ProductPriority = ppCheapest;
    break;
    case HighestPriceFirst:
    discount.ProductPriority =  ppHighest;
     break;
    case NotApllicable:
    discount.ProductPriority = ppNone;
     break;
  }

  switch((MMDisplayOption)inDiscountInfo->DisplayAs)
  {
     case FixedDescriptionAndAmount:
     discount.Type = dtFixed;
     break;
     case PromptForDescription:
     discount.Type = dtPromptDescription;
     break;
     case PromptForAmount:
     discount.Type = dtPromptAmount;
     break;
     case PromptForDescriptionAndAmount:
     discount.Type = dtPromptDescriptionAmount;
     break;
  }
  ExtractModeAndAmount(inDiscountInfo,discount);
  if(inDiscountInfo->IsCategoryFilterApplicable)
  {
     int categorKey = TDBMenu::GetOrCreateCloudCategory(DBTransaction,inDiscountInfo->Name);
     discount.CategoryFilterKeys.insert(categorKey);
  }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::ExtractModeAndAmount(DiscountInfo* inDiscountInfo,TDiscount& discount)
{
   Currency amount = fabs(StrToCurr(inDiscountInfo->Value->DecimalString));
   if((MMImplicationType)inDiscountInfo->ImplicationType == 2)
   {
      discount.Mode = DiscModePoints;
      discount.Amount = amount;
   }
   else
   {
     bool isSurcharge =  (MMImplicationType)inDiscountInfo->ImplicationType == Surcharge;

      switch((MMDiscountType)inDiscountInfo->DiscountType)
          {
             case Percent:
             discount.Mode = DiscModePercent;
             discount.PercentAmount = isSurcharge ? -1 * amount : amount;
             break;
             case Dollar:
             discount.Mode = DiscModeCurrency;
             discount.Amount = isSurcharge ? -1 * amount : amount;
             break;
             case Combo:
             discount.Mode = DiscModeCombo;
             discount.Amount = amount;
             discount.ComboAmount = amount;
             break;
             case ItemMode:
             discount.Mode = DiscModeItem;
             discount.Amount = isSurcharge ? -1 * amount : amount;
             break;
             case SetPrice:
             discount.Mode = DiscModeSetPrice;
             discount.Amount = amount;
             break;
          }
   }


}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncCompanyInfo(CompanyInfo* companyInfo)
{
      TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );
      transaction.StartTransaction();
      try
        {
          SyncTierLevels(transaction,companyInfo->TierLevels);
          SyncDiscounts(transaction,companyInfo->Discounts);
          if(companyInfo->HasGiftCardsAvailable)
          {
             CreateGiftVoucherPaymentType(transaction);
          }
          if(companyInfo->HasPocketVouchersAvailable)
          {
             CreateVoucherPaymentType(transaction);
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
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncTierLevels(Database::TDBTransaction &DBTransaction,DynamicArray<TierLevelInfo*> tierLevels)
{
      try
        {
           TDBTierLevel::ClearAll(DBTransaction);
           for(int i = 0; i< tierLevels.Length;i++)
           {
             TierLevelInfo* tierLevelInfo = tierLevels[i];
             TTierLevel* tierLevel = new TTierLevel;
             ReadTierInfo(tierLevelInfo,tierLevel);
             TDBTierLevel::AddTierLevel(DBTransaction,tierLevel);
           }
          TGlobalSettings::Instance().UseTierLevels = tierLevels.Length > 0;
  	      TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmUseTierLevels,TGlobalSettings::Instance().UseTierLevels);
          if(!TGlobalSettings::Instance().UseTierLevels)
	      {
             TGlobalSettings::Instance().AllowPointPaymentByValue = true;
             TGlobalSettings::Instance().AllowPointPaymentByWeight = false;
             TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowPointPaymentByWeight, TGlobalSettings::Instance().AllowPointPaymentByWeight);
             TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowPointPaymentByValue, TGlobalSettings::Instance().AllowPointPaymentByValue);
          }
	   }
      catch( Exception &exc )
        {
            TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
            throw;
        }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncDiscounts(Database::TDBTransaction &DBTransaction,DynamicArray<DiscountInfo*> discounts)
{
      try
        {
         if(!TGlobalSettings::Instance().IsCloudSyncedForDiscount && discounts.Length > 0)
         {
            ManagerDiscount->DeleteDiscounts(DBTransaction);
            DisableSyncSetting(DBTransaction);
         }
         else
         {
           std::vector<AnsiString> discountsCodes;
           for(int i = 0; i< discounts.Length;i++)
           {
              DiscountInfo* discountInfo = discounts[i];
              discountsCodes.push_back(discountInfo->Code);
           }
           ManagerDiscount->DeleteCloudDiscounts(DBTransaction,discountsCodes);
         }
        for(int i = 0; i< discounts.Length;i++)
           {
             DiscountInfo* discountInfo = discounts[i];
             TDiscount CloudDiscount;
             ReadDiscountInfo(DBTransaction,discountInfo,CloudDiscount);
             ManagerDiscount->SetDiscount(DBTransaction,0,CloudDiscount);
           }
	    }
      catch( Exception &exc )
        {
            TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
            throw;
        }
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::SyncLoyaltymateAttrs(const TMMContactInfo* const inContactInfo )
{
    if( !UpdateLoyaltymateAttrs( inContactInfo ) )
    {
        AddLoyaltymateAttrs( inContactInfo );
    }
}
//---------------------------------------------------------------------------
bool TLoyaltyMateInterface::UpdateLoyaltymateAttrs(const TMMContactInfo* const inContactInfo )
{
    if( ( inContactInfo->CloudUUID != NULL ) &&
        ( inContactInfo->CloudUUID != ""   ) &&
        ( inContactInfo->ContactKey > 0 )   )
    {
        AnsiString sql = "UPDATE LOYALTYATTRIBUTES "
                         "SET UUID    = '"        + inContactInfo->CloudUUID + "', "
                         "ACTIVATION_TOKEN  = '"        + inContactInfo->ActivationToken + "', "
                         "SITE_ID = "        + IntToStr(inContactInfo->SiteID) + " "
                         "WHERE CONTACTS_KEY = " + IntToStr(inContactInfo->ContactKey)  + "";

             return ExecuteQuery( sql, &TDeviceRealTerminal::Instance().DBControl) > 0;
    }

    return false;
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::AddLoyaltymateAttrs(const TMMContactInfo* const inContactInfo )
{
    if(inContactInfo->ContactKey == 0)
      return;

    int key = GenerateTableKey( "GEN_LOYALTYATTRIBUTES", &TDeviceRealTerminal::Instance().DBControl );
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
    ExecuteQuery( sql , &TDeviceRealTerminal::Instance().DBControl);
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyResponse* inWCFResponse )
{
    return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyMemberResponse* inWCFResponse )
{
     MMLoyaltyServiceResponse memberResponse = MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                AnsiString( inWCFResponse->MemberInfo->UniqueId.t_str() ) );
     memberResponse.MemberCode = inWCFResponse->MemberInfo->MemberCardCode;
     return memberResponse;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyCompanyResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyGiftCardResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyVoucherResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(VoucherTransactionResponse* inWCFResponse )
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateExceptionFailedResponse(AnsiString inMessage )
{

   if(inMessage.Pos("XML") > 0 || inMessage.Pos("The handle") > 0 )
   return MMLoyaltyServiceResponse(false,"Not able to connect with server.","",FailedDueToException,AnsiString( "" ) );
   else
     return MMLoyaltyServiceResponse(false,inMessage,"",FailedDueToException,AnsiString( "" ) );
}
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::ExecuteQuery(const AnsiString inSQL,TDBControl* const inDBControl )
{
    int result = 0;
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

    return result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int TLoyaltyMateInterface::GenerateTableKey(const AnsiString  inGeneratorName,TDBControl* const inDBControl )
{
    int result = -1;
    TDBTransaction transaction( *inDBControl );
    try
    {
        transaction.StartTransaction();
        TIBSQL *query = transaction.Query( transaction.AddQuery() );
        query->SQL->Text =  "SELECT GEN_ID(" + inGeneratorName + ", 1) FROM RDB$DATABASE";
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

    return result;
}
//---------------------------------------------------------------------------

void TLoyaltyMateInterface::CreateVoucherPaymentType(Database::TDBTransaction &DBTransaction)
{
    try
    {
        int PaymentKey = 0;
        UnicodeString glCode = "";
        TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY,GL_CODE FROM PAYMENTTYPES WHERE PAYMENT_NAME = :PAYMENT_NAME";
        IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Voucher";
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
        {
          PaymentKey = IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger ;
          glCode = IBInternalQuery->FieldByName("GL_CODE")->AsString ;
        }
        TPayment NewPayment;
        NewPayment.Name = "Voucher";
        NewPayment.SysNameOveride = "Voucher";
        NewPayment.SetPaymentAttribute(ePayTypeGetVoucherDetails);
        NewPayment.DisplayOrder = 1;
        NewPayment.GroupNumber = 0;
        NewPayment.Colour = clTeal;
        NewPayment.PaymentThirdPartyID = "10007242";
        NewPayment.AutoPopulateBlindBalance = true;
        NewPayment.GLCode = glCode;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentSave(DBTransaction, PaymentKey, NewPayment);
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
//---------------------------------------------------------------------------
void TLoyaltyMateInterface::CreateGiftVoucherPaymentType(Database::TDBTransaction &DBTransaction)
{
    try
    {
        int PaymentKey = 0;
        UnicodeString glCode = "";
        TIBSQL *IBInternalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY,GL_CODE FROM PAYMENTTYPES WHERE PAYMENT_NAME = :PAYMENT_NAME";
        IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Gift Card";
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
        {
          PaymentKey = IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger ;
          glCode = IBInternalQuery->FieldByName("GL_CODE")->AsString ;
        }
        TPayment NewPayment;
        NewPayment.Name = "Gift Card";
        NewPayment.SysNameOveride = "Gift Card";
        NewPayment.SetPaymentAttribute(ePayTypeGetVoucherDetails);
        NewPayment.DisplayOrder = 1;
        NewPayment.GroupNumber = 0;
        NewPayment.Colour = clTeal;
        NewPayment.PaymentThirdPartyID = "10007242";
        NewPayment.AutoPopulateBlindBalance = true;
        NewPayment.GLCode = glCode;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentSave(DBTransaction, PaymentKey, NewPayment);
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}

}

void  TLoyaltyMateInterface::DisableSyncSetting(Database::TDBTransaction &DBTransaction)
{
 	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	if (!(global_profile_key = mv.GetProfile(DBTransaction, eSystemProfiles, "Globals")))
	    global_profile_key = mv.SetProfile(DBTransaction, eSystemProfiles, "Globals");
    TGlobalSettings::Instance().IsCloudSyncedForDiscount = true;
	mv.SetProfileBool(DBTransaction, global_profile_key, vmIsCloudSynced,true);
}


RequestInfo* TLoyaltyMateInterface::CreateRequest(AnsiString requestKey)
{
   RequestInfo* requestInfo = new RequestInfo();
   requestInfo->SiteCode = GetCurrentSiteId();
   requestInfo->RequestKey = requestKey;
   requestInfo->RequestTime = new TXSDateTime();
   TDateTime requestDate = Now();
   requestInfo->RequestTime->AsUTCDateTime = Dateutils::EncodeDateTime(YearOf(requestDate),MonthOf(requestDate),DayOf(requestDate),
                                                             HourOf(requestDate),MinuteOf(requestDate),SecondOf(requestDate),MilliSecondOf(requestDate));

   return requestInfo;
}

//this function is not breaked into multiple function due to unable to perform link errors.
MMLoyaltyServiceResponse TLoyaltyMateInterface::SendMenu(TSiteMenuInfo menuInfo)
{
    LoyaltyResponse *wcfResponse;
    try
    {
        SiteMenuInfo *wcfInfo = new SiteMenuInfo();
        wcfInfo->CompanyId = menuInfo.CompanyId;
        wcfInfo->SiteId = menuInfo.SiteId;
        if(!menuInfo.MenuConsumables.empty())
        {
            ArrayOfMenuConsumableInfo menuConsumableArray;
            for(std::list<TMenuConsumableInfo>::iterator itMenuInfo = menuInfo.MenuConsumables.begin();
                    itMenuInfo != menuInfo.MenuConsumables.end(); ++itMenuInfo)
            {
                MenuConsumableInfo* menuConsumableInfo = new MenuConsumableInfo;
                menuConsumableInfo->Description = itMenuInfo->Description;
                menuConsumableInfo->IsPalmable = itMenuInfo->IsPalmable;
                menuConsumableInfo->MenuId = itMenuInfo->MenuId;
                menuConsumableInfo->Name = itMenuInfo->Name;
                menuConsumableInfo->Type = itMenuInfo->MenuType;

                ArrayOfCourseInfo courseInfoArray;

                for(std::list<TCourseInfo>::iterator itCourseInfo = itMenuInfo->SiteCourses.begin();
                    itCourseInfo != itMenuInfo->SiteCourses.end(); ++itCourseInfo)
                {
                    CourseInfo* courseInfo = new CourseInfo;
                    courseInfo->CourseId = itCourseInfo->CourseId;
                    courseInfo->Description = itCourseInfo->Description;
                    courseInfo->Name = itCourseInfo->Name;
                    courseInfo->ServingCourseDescription = itCourseInfo->ServingCourseDescription;
                    courseInfo->ServingCourseName = itCourseInfo->ServingCourseName;

                    ArrayOfSiteItemInfo siteItemInfoArray;

                    for(std::list<TSiteItemInfo>::iterator itSiteItemInfo = itCourseInfo->Items.begin();
                        itSiteItemInfo != itCourseInfo->Items.end(); ++itSiteItemInfo)
                    {
                        SiteItemInfo* siteItemInfo = new SiteItemInfo;
                        siteItemInfo->CompanyId = itSiteItemInfo->CompanyId;
                        siteItemInfo->Description = itSiteItemInfo->Description;
                        siteItemInfo->ItemUniqueId = itSiteItemInfo->ItemUniqueId;
                        siteItemInfo->Name = itSiteItemInfo->Name;
                        siteItemInfo->OnlyAsSide = itSiteItemInfo->OnlyAsSide;
                        siteItemInfo->SiteItemId = itSiteItemInfo->SiteItemId;

                        ArrayOfItemSizeInfo itemSizeInfoArray;

                        for(std::list<TItemSizeInfo>::iterator itItemSizeInfo = itSiteItemInfo->ItemSizes.begin();
                            itItemSizeInfo != itSiteItemInfo->ItemSizes.end(); ++itItemSizeInfo)
                        {
                            ItemSizeInfo* itemSizeInfo = new ItemSizeInfo;
                            itemSizeInfo->CanBePaidUsingPoints = itItemSizeInfo->CanBePaidUsingPoints;
                            itemSizeInfo->DefaultPatronCount = itItemSizeInfo->DefaultPatronCount;
                            itemSizeInfo->Description = itItemSizeInfo->Description;
                            itemSizeInfo->IsFree = itItemSizeInfo->IsFree;
                            itemSizeInfo->IsWeighted = itItemSizeInfo->IsWeighted;
                            itemSizeInfo->ItemSizeId = itItemSizeInfo->ItemSizeId;
                            itemSizeInfo->Name = itItemSizeInfo->Name;
                            itemSizeInfo->OrderingUniqueId = itItemSizeInfo->OrderingUniqueId;
                            itemSizeInfo->PointsPercentage = itItemSizeInfo->PointsPercentage;
                            itemSizeInfo->PointsPrice = itItemSizeInfo->PointsPrice;
                            itemSizeInfo->Price = itItemSizeInfo->Price;
                            itemSizeInfo->ThirdPartyId = itItemSizeInfo->ThirdPartyId;

                            ArrayOfItemSizeTaxProfileInfo itemSizeTaxProfileInfo;

                            for(std::list<TItemSizeTaxProfileInfo>::iterator itItemSizeTaxInfo = itItemSizeInfo->ItemSizeTaxProfiles.begin();
                            itItemSizeTaxInfo != itItemSizeInfo->ItemSizeTaxProfiles.end(); ++itItemSizeTaxInfo)
                            {
                                ItemSizeTaxProfileInfo* itemSizeTaxInfo = new ItemSizeTaxProfileInfo;
                                itemSizeTaxInfo->Description = itItemSizeTaxInfo->Description;
                                itemSizeTaxInfo->ItemSizeTaxProfileId = itItemSizeTaxInfo->ItemSizeTaxProfileId;
                                itemSizeTaxInfo->Name = itItemSizeTaxInfo->Name;
                                itemSizeTaxInfo->Priority = itItemSizeTaxInfo->Priority;
                                itemSizeTaxInfo->Rate = itItemSizeTaxInfo->Rate;
                                itemSizeTaxInfo->Type = itItemSizeTaxInfo->TaxProfileType;
                                itemSizeTaxProfileInfo.Length = (itemSizeTaxProfileInfo.Length + 1);
                                itemSizeTaxProfileInfo[itemSizeTaxProfileInfo.Length - 1] = itemSizeTaxInfo;
                            }
                            itemSizeInfo->ItemSizeTaxProfiles = itemSizeTaxProfileInfo;
                            itemSizeInfoArray.Length = (itemSizeInfoArray.Length + 1);
                            itemSizeInfoArray[itemSizeInfoArray.Length - 1] = itemSizeInfo;
                        }
                        siteItemInfo->ItemSizes = itemSizeInfoArray;

                        ArrayOfSideGroupInfo itemSideGroupArray;

                        for(std::list<TSideGroupInfo>::iterator itSideGroupInfo = itSiteItemInfo->SideGroup.begin();
                            itSideGroupInfo != itSiteItemInfo->SideGroup.end(); ++itSideGroupInfo)
                        {
                            SideGroupInfo* sideGroupInfo = new SideGroupInfo;
                            sideGroupInfo->AllowSkip = itSideGroupInfo->AllowSkip;
                            sideGroupInfo->Description = itSideGroupInfo->Description;
                            sideGroupInfo->MaxSelect = itSideGroupInfo->MaxSelect;
                            sideGroupInfo->Name = itSideGroupInfo->Name;
                            sideGroupInfo->SideGroupId = itSideGroupInfo->SideGroupId;

                            ArrayOfItemSideInfo itemSideInfoArray;

                            for(std::list<TItemSideInfo>::iterator itItemSideInfo = itSideGroupInfo->ItemSides.begin();
                                itItemSideInfo != itSideGroupInfo->ItemSides.end(); ++itItemSideInfo)
                            {
                                ItemSideInfo* itemSideInfo = new ItemSideInfo;
                                itemSideInfo->Description = itItemSideInfo->Description;
                                itemSideInfo->ItemUniqueId = itItemSideInfo->ItemUniqueId;
                                itemSideInfo->Name = itItemSideInfo->Name;
                                itemSideInfo->OnlyAsSide = itItemSideInfo->OnlyAsSide;
                                itemSideInfo->SiteItemId = itItemSideInfo->SiteItemId;

                                ArrayOfItemSizeInfo itemSizeInfoArray;

                                for(std::list<TItemSizeInfo>::iterator itItemSizeInfo = itItemSideInfo->ItemSizes.begin();
                                    itItemSizeInfo != itItemSideInfo->ItemSizes.end(); ++itItemSizeInfo)
                                {
                                    ItemSizeInfo* itemSizeInfo = new ItemSizeInfo;
                                    itemSizeInfo->CanBePaidUsingPoints = itItemSizeInfo->CanBePaidUsingPoints;
                                    itemSizeInfo->DefaultPatronCount = itItemSizeInfo->DefaultPatronCount;
                                    itemSizeInfo->Description = itItemSizeInfo->Description;
                                    itemSizeInfo->IsFree = itItemSizeInfo->IsFree;
                                    itemSizeInfo->IsWeighted = itItemSizeInfo->IsWeighted;
                                    itemSizeInfo->ItemSizeId = itItemSizeInfo->ItemSizeId;
                                    itemSizeInfo->Name = itItemSizeInfo->Name;
                                    itemSizeInfo->OrderingUniqueId = itItemSizeInfo->OrderingUniqueId;
                                    itemSizeInfo->PointsPercentage = itItemSizeInfo->PointsPercentage;
                                    itemSizeInfo->PointsPrice = itItemSizeInfo->PointsPrice;
                                    itemSizeInfo->Price = itItemSizeInfo->Price;
                                    itemSizeInfo->ThirdPartyId = itItemSizeInfo->ThirdPartyId;

                                    ArrayOfItemSizeTaxProfileInfo itemSizeTaxProfileInfo;

                                    for(std::list<TItemSizeTaxProfileInfo>::iterator itItemSizeTaxInfo = itItemSizeInfo->ItemSizeTaxProfiles.begin();
                                    itItemSizeTaxInfo != itItemSizeInfo->ItemSizeTaxProfiles.end(); ++itItemSizeTaxInfo)
                                    {
                                        ItemSizeTaxProfileInfo* itemSizeTaxInfo = new ItemSizeTaxProfileInfo;
                                        itemSizeTaxInfo->Description = itItemSizeTaxInfo->Description;
                                        itemSizeTaxInfo->ItemSizeTaxProfileId = itItemSizeTaxInfo->ItemSizeTaxProfileId;
                                        itemSizeTaxInfo->Name = itItemSizeTaxInfo->Name;
                                        itemSizeTaxInfo->Priority = itItemSizeTaxInfo->Priority;
                                        itemSizeTaxInfo->Rate = itItemSizeTaxInfo->Rate;
                                        itemSizeTaxInfo->Type = itItemSizeTaxInfo->TaxProfileType;
                                        itemSizeTaxProfileInfo.Length = (itemSizeTaxProfileInfo.Length + 1);
                                        itemSizeTaxProfileInfo[itemSizeTaxProfileInfo.Length - 1] = itemSizeTaxInfo;
                                    }
                                    itemSizeInfo->ItemSizeTaxProfiles = itemSizeTaxProfileInfo;
                                    itemSizeInfoArray.Length = (itemSizeInfoArray.Length + 1);
                                    itemSizeInfoArray[itemSizeInfoArray.Length - 1] = itemSizeInfo;
                                }
                                itemSideInfo->ItemSizes = itemSizeInfoArray;
                                itemSideInfoArray.Length = (itemSideInfoArray.Length + 1);
                                itemSideInfoArray[itemSideInfoArray.Length - 1] = itemSideInfo;
                            }
                            sideGroupInfo->ItemSides = itemSideInfoArray;
                            itemSideGroupArray.Length = (itemSideGroupArray.Length + 1);
                            itemSideGroupArray[itemSideGroupArray.Length - 1] = sideGroupInfo;
                        }
                        siteItemInfo->SideGroups = itemSideGroupArray;
                        siteItemInfoArray.Length = (siteItemInfoArray.Length + 1);
                        siteItemInfoArray[siteItemInfoArray.Length - 1] = siteItemInfo;
                    }
                    courseInfo->Items = siteItemInfoArray;
                    courseInfoArray.Length = (courseInfoArray.Length + 1);
                    courseInfoArray[courseInfoArray.Length - 1] = courseInfo;
                }
                menuConsumableInfo->SiteCourses = courseInfoArray;
                menuConsumableArray.Length = (menuConsumableArray.Length + 1);
                menuConsumableArray[menuConsumableArray.Length - 1] = menuConsumableInfo;
            }
            wcfInfo->MenuConsumables = menuConsumableArray;
        }
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
        wcfResponse = loyaltymateClient->SyncMenu(SyndicateCode, wcfInfo);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {     
        return CreateExceptionFailedResponse( exc.Message );
    }
}


AnsiString TLoyaltyMateInterface::GetSyndCodeForOnlineOrdering()
{
    AnsiString syndicateCode = "";

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TManagerSyndCode ManagerSyndicateCode;
        ManagerSyndicateCode.Initialise(dbTransaction);
        TSyndCode currentSyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        syndicateCode = currentSyndicateCode.GetSyndCode();
        dbTransaction.Commit();
    }
    catch( Exception& exc )
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,exc.Message);
		throw;
    }
    return syndicateCode;
}

MMLoyaltyServiceResponse TLoyaltyMateInterface::SendTaxSettings(TSiteTaxSettingsInfo taxSettingsInfo)
{
    LoyaltyResponse *wcfResponse;
    try
    {
        SiteTaxSettingsinfo *wcfInfo = new SiteTaxSettingsinfo();
        wcfInfo->SiteId = taxSettingsInfo.SiteId;

        if(!taxSettingsInfo.SiteTaxSettings.empty())
        {
            ArrayOfTaxSettingsInfo taxSettingsinfo;

            for(std::list<TTaxSettingsInfo>::iterator itTaxSettingInfo = taxSettingsInfo.SiteTaxSettings.begin();
                    itTaxSettingInfo != taxSettingsInfo.SiteTaxSettings.end(); ++itTaxSettingInfo)
            {
                TaxSettingsInfo* taxSetting = new TaxSettingsInfo;
                taxSetting->SettingType = itTaxSettingInfo->SettingType;
                taxSetting->Value = itTaxSettingInfo->Value;
                taxSettingsinfo.Length = (taxSettingsinfo.Length + 1);
                taxSettingsinfo[taxSettingsinfo.Length - 1] = taxSetting;
            }
            wcfInfo->SiteTaxSettings = taxSettingsinfo;
        }
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
        wcfResponse = loyaltymateClient->SyncTaxSettings(SyndicateCode, wcfInfo);
        delete wcfInfo;
        wcfInfo = NULL;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {   
        return CreateExceptionFailedResponse( exc.Message );
    }
}


#pragma package(smart_init)
