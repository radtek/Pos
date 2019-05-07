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
#include "SendEmail.h"
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
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
       LoyaltyMemberResponse *wcfResponse;
       MemberInfo *wcfInfo = new MemberInfo();
       CreateWcfContactInfo(wcfInfo,contactInfo);
       loyaltyLogs->Add("Created Contact Info                                        ");
       if(contactInfo.MemberType==2)
            wcfInfo->Activated    = true;
        else
            wcfInfo->Activated    = false;     //trip
       CoInitialize(NULL);
       loyaltyLogs->Add("Creating Connection  with Web services for add member               " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
       wcfResponse = loyaltymateClient->SaveMember(syndicateCode.GetSyndCode(),wcfInfo );
       loyaltyLogs->Add("Receive Response on pos of created Member                           " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
       if( FAutoSync && wcfResponse->Successful)
        {
            contactInfo.CloudUUID = AnsiString(wcfResponse->MemberInfo->UniqueId);
            contactInfo.MemberCode = AnsiString(wcfResponse->MemberInfo->MemberCardCode);
            SyncLoyaltymateAttrs( &contactInfo );
            loyaltyLogs->Add("Sync with LoyaltyAttribute done                                     ");
            ReadContactInfo(wcfResponse, contactInfo, true );
            loyaltyLogs->Add("Read Contact Info                                                   ");
            loyaltyLogs->Add("=================================================================================================");

        }
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
       loyaltyLogs->Add("Exception is                                                        " + exc.Message);
       loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMember(TSyndCode syndicateCode,AnsiString uuid,const TMMContactInfo* const contactInfo)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        MemberInfo            *wcfInfo = new MemberInfo();
        TMMContactInfo contact_Info = ( TMMContactInfo )( *contactInfo );
        CreateWcfContactInfo(wcfInfo,contact_Info);
        loyaltyLogs->Add("Created Contact Info for Update Member                              ");
        wcfInfo->UniqueId = uuid;
        CoInitialize(NULL);
        loyaltyLogs->Add("Sending Request for Updating Member on pos                          " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->SaveMember(syndicateCode.GetSyndCode(),wcfInfo);
        loyaltyLogs->Add("Received Response of Updated Member on pos                          " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        delete wcfInfo;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetails(TSyndCode syndicateCode,AnsiString uuid,TMMContactInfo &outContactInfo,bool replacePoints )
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);

        loyaltyLogs->Add("Sending Request of Member by UniqueId from pos                      " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse =  loyaltymateClient->GetMemberByUniqueId(syndicateCode.GetSyndCode(),CreateRequest(uuid));
        loyaltyLogs->Add("Received Response of Member by uniqueId on pos                      " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));

        if(wcfResponse->ResponseCode == GUIDNotFound && outContactInfo.EMail.Trim() != "")
        {
            TMMContactInfo contactInfo;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();

            CoInitialize(NULL);
            loyaltyLogs->Add("Sending Request of member by email from pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
            wcfResponse = loyaltymateClient->GetMemberByEmail(syndicateCode.GetSyndCode(), CreateRequest(outContactInfo.EMail));
            loyaltyLogs->Add("Received Response of Member by email on pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
            if(wcfResponse->Successful)
            {

                ReadContactInfo(wcfResponse,outContactInfo,replacePoints );
                int contactKey = TDBContacts::GetContactByEmail(DBTransaction, outContactInfo.EMail);

                loyaltyLogs->Add("Getting Contact key from Contact table                              ");

                if(contactKey && !TLoyaltyMateUtilities::IsUUIDExist(DBTransaction,outContactInfo.CloudUUID))
                {
                    TLoyaltyMateUtilities::UpdateUUID(DBTransaction, contactKey, outContactInfo.CloudUUID);
                    loyaltyLogs->Add("Update UID in LOYALTYATTRIBUTES table done                          ");

                }
                else
                {
                    wcfResponse->Successful = false;
                    wcfResponse->Message = "Failed to Download member.";
                    loyaltyLogs->Add("Failed to Download member.                                          ");

                }
            }
            DBTransaction.Commit();
            return CreateMMResponse( wcfResponse );
        }
        else if(wcfResponse->ResponseCode == MultipleGUIDExist)
        {
            AnsiString EmailBody = GetEmailBody(syndicateCode.GetSyndCode() , outContactInfo.EMail, uuid);
            loyaltyLogs->Add("Received Email Body                                                 ");
            SendEmail(EmailBody);
            loyaltyLogs->Add("Send Mail                                                           ");
        }
       if(wcfResponse->Successful)
        {
            ReadContactInfo(wcfResponse,outContactInfo,replacePoints );
            loyaltyLogs->Add("Read Member Details                                                 ");

        }
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByCode(TSyndCode syndicateCode, AnsiString memberCode,TMMContactInfo &outContactInfo,bool replacePoints )
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);

        loyaltyLogs->Add("Sending Request of Member by code from pos                          " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->GetMemberByCardCode(syndicateCode.GetSyndCode(), CreateRequest(memberCode));
        loyaltyLogs->Add("Received Response of Member by code on pos                          " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));

        if(wcfResponse->ResponseCode == GUIDNotFound && outContactInfo.EMail.Trim() != "")
        {
            TMMContactInfo contactInfo;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();

            CoInitialize(NULL);

            loyaltyLogs->Add("Sending Request of Member by email from pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
            wcfResponse = loyaltymateClient->GetMemberByEmail(syndicateCode.GetSyndCode(), CreateRequest(outContactInfo.EMail));
            loyaltyLogs->Add("Received Response of Member by email on pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));

            if(wcfResponse->Successful)
            {
                ReadContactInfo(wcfResponse,outContactInfo,replacePoints );
                loyaltyLogs->Add("Read Contact Info of Member Details By Code                         ");
                int contactKey = TDBContacts::GetContactByEmail(DBTransaction, outContactInfo.EMail);
                loyaltyLogs->Add("Getting Contact key from contact table using email                  ");

                if(contactKey && !TLoyaltyMateUtilities::IsUUIDExist(DBTransaction,outContactInfo.CloudUUID))
                {
                    TLoyaltyMateUtilities::UpdateUUID(DBTransaction, contactKey, outContactInfo.CloudUUID);
                    loyaltyLogs->Add("Updated in LOYALTYATTRIBUTES table done                             ");

                }
                else
                {
                    wcfResponse->Successful = false;
                    wcfResponse->Message = "Failed to Download member.";
                    loyaltyLogs->Add("Failed to Download member details by code                           ");

                }
            }
            DBTransaction.Commit();
            return CreateMMResponse( wcfResponse );
        }
        else if(wcfResponse->ResponseCode == MultipleGUIDExist)
        {

            AnsiString EmailBody = GetEmailBody(syndicateCode.GetSyndCode() , outContactInfo.EMail, "not available to print");
            loyaltyLogs->Add("Received Email Body for member detail by code                       ");
            SendEmail(EmailBody);
            loyaltyLogs->Add("Send Email for member detail by code                                " );

        }
        if(wcfResponse->Successful)
        {
            ReadContactInfo(wcfResponse, outContactInfo, replacePoints );
            loyaltyLogs->Add("Read Member Details by code                                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));

        }
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetMemberDetailsByEmail(TSyndCode syndicateCode,AnsiString memberEmail,TMMContactInfo &contactInfo,
                                    bool replacePoints)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyMemberResponse *wcfResponse;
        CoInitialize(NULL);
        loyaltyLogs->Add("Sending Request of Member details by Email from pos                 " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->GetMemberByEmail(syndicateCode.GetSyndCode(), CreateRequest(memberEmail));
        loyaltyLogs->Add("Received Response of member detail by Email on pos                  " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        if(wcfResponse->Successful)
        {
            ReadContactInfo(wcfResponse, contactInfo, replacePoints );
            loyaltyLogs->Add("Read Member Details by Email                                        ");

        }
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------  Need some changes
MMLoyaltyServiceResponse TLoyaltyMateInterface::PostTransaction(TSyndCode syndicateCode,AnsiString uuid,Currency pointsBalance,Currency pointsDelta,
                                            TDateTime occurredDate,int pointsType,AnsiString inInvoiceNumber)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
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
        loyaltyLogs->Add("Sending Request for Post Transaction from pos                       " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->PostTransaction(syndicateCode.GetSyndCode(),wcfInfo);
        loyaltyLogs->Add("Received Response of Post Transaction on pos                        " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        delete wcfInfo;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception &exc)
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::SyncCompanyDetails(TSyndCode syndicateCode)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyCompanyResponse *wcfResponse;
        CoInitialize(NULL);
        loyaltyLogs->Add("Sending Request for company information from pos                    " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->GetCompanyInformation(syndicateCode.GetSyndCode());
        loyaltyLogs->Add("Received Response company information from pos                      " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        if(wcfResponse->Successful)
         {
            SyncCompanyInfo(wcfResponse->CompanyInfo);
         }
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
//MMLoyaltyServiceResponse TLoyaltyMateInterface::SyncOnlineOrderingDetails(TSyndCode syndicateCode,int siteCode)
//{
//
//    try
//    {
//        LoyaltyOnlineOrderingResponse* response;
//        CoInitialize(NULL);
//        response = loyaltymateClient->SyncOnlineOrderingDetails(syndicateCode.GetSyndCode(),siteCode);
//        return CreateMMResponse( response );
//    }
//    catch( Exception& exc )
//    {
//        return CreateExceptionFailedResponse( exc.Message );
//    }
//}
//---------------------------------------------------------------------------
//bool TLoyaltyMateInterface::UnsetOrderingDetails(TSyndCode syndicateCode,int siteCode)
//{
//
//    bool isSuccesssful = false;
//    try
//    {
//        CoInitialize(NULL);
//        isSuccesssful = loyaltymateClient->UnsetOrderingDetails(syndicateCode.GetSyndCode(),siteCode);
//     }
//    catch( Exception& exc )
//    {
//    }
//    return isSuccesssful;
//}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::UpdateMemberCardCode(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        LoyaltyResponse *wcfResponse;
        CoInitialize(NULL);
        loyaltyLogs->Add("Sending Request for Update Member Card Code from pos                " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->UpdateMemberCardCode(syndicateCode.GetSyndCode(),uniqueId,memberCardCode);
        loyaltyLogs->Add("Received Response of Update Member Card Code on pos                 " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetGiftVoucherBalance(TSyndCode syndicateCode,AnsiString giftVoucherNumber,TGiftCardDetail &GiftCardDetail)
{
   std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
  try
    {

        LoyaltyGiftCardResponse *wcfResponse;
        CoInitialize(NULL);

        loyaltyLogs->Add("Sending Request for gift card balance from pos                      " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->GetGiftCardBalance(syndicateCode.GetSyndCode(),CreateRequest(giftVoucherNumber));
        loyaltyLogs->Add("Received Response of gift card balance on pos                       " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));

        if(wcfResponse->Successful)
        {
           ReadGiftCardInfo(wcfResponse->GiftCardInfo, GiftCardDetail );
           loyaltyLogs->Add("Read Gift Card info in pos                                          ");

        }
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);

        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::GetPocketVoucherDetail(TSyndCode syndicateCode,AnsiString pocketVoucherNumber,TVoucherDetail& VoucherDetail)
{
  std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
  try
    {

        LoyaltyVoucherResponse *wcfResponse;
        CoInitialize(NULL);
        loyaltyLogs->Add("Sending Request for pocket voucher from pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->GetPocketVoucherDetail(syndicateCode.GetSyndCode(), CreateRequest(pocketVoucherNumber));
        loyaltyLogs->Add("Received Response for pocket voucher on pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        if(wcfResponse->Successful)
        {
            VoucherDetail.VoucherNumber = pocketVoucherNumber;
            ReadPocketVoucherInfo(wcfResponse->VoucherInfo, VoucherDetail );
            loyaltyLogs->Add("Read Pocket voucher details on pos                                   ");

        }
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::ProcessVoucherTransaction(TSyndCode syndicateCode,TVoucherUsageDetail& inVoucherUsageDetail)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
    try
    {
        std::auto_ptr<TStringList> LoyaltyLogs(new TStringList);
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
        loyaltyLogs->Add("Sending Request for Process Voucher Transaction from pos            " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->ProcessVoucherTransaction(syndicateCode.GetSyndCode(),wcfInfo);
        loyaltyLogs->Add("Received Response of Process voucher on pos                         " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        if(wcfResponse->Successful && wcfResponse->GiftCardExpiryDate != NULL)
        {
           inVoucherUsageDetail.GiftCardExpiryDate = wcfResponse->GiftCardExpiryDate->AsUTCDateTime;
        }
        AddLoyaltyLogs(LoyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TLoyaltyMateInterface::ReleaseVouchers(TSyndCode syndicateCode,TReleasedVoucherDetail inReleasedVoucherDetail)
{
    std::auto_ptr<TStringList> loyaltyLogs(new TStringList);
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
        loyaltyLogs->Add("Sending Request for Release Voucher Transaction from pos            " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        wcfResponse = loyaltymateClient->ReleaseVouchers(syndicateCode.GetSyndCode(),wcfInfo);
        loyaltyLogs->Add("Received Response for Process Voucher Transaction on pos            " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        loyaltyLogs->Add("Exception is                                                        " + exc.Message);
        loyaltyLogs->Add("=================================================================================================");
        AddLoyaltyLogs(loyaltyLogs);
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
    std::auto_ptr<TStringList> LoyaltyLogs(new TStringList);
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
    LoyaltyLogs->Add("Read Member Vouchers                                                " + (AnsiString)Now().FormatString("hh:nn:ss am/pm"));
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
//MMLoyaltyServiceResponse TLoyaltyMateInterface::CreateMMResponse(LoyaltyOnlineOrderingResponse*  inWCFResponse)
//{
//     return MMLoyaltyServiceResponse(
//                inWCFResponse->IsSuccessful,
//                AnsiString( inWCFResponse->ResponseText.t_str() ),
//                "",
//                Successful,
//                "" ) ;
//}
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
//MMLoyaltyServiceResponse TLoyaltyMateInterface::SendMenu(TSiteMenuInfo menuInfo)
//{
//    LoyaltyResponse *wcfResponse;
//    try
//    {
//        SiteMenuInfo *wcfInfo = new SiteMenuInfo();
//        wcfInfo->CompanyId = menuInfo.CompanyId;
//        wcfInfo->SiteId = menuInfo.SiteId;
//        if(!menuInfo.MenuConsumables.empty())
//        {
//            ArrayOfMenuConsumableInfo menuConsumableArray;
//            for(std::list<TMenuConsumableInfo>::iterator itMenuInfo = menuInfo.MenuConsumables.begin();
//                    itMenuInfo != menuInfo.MenuConsumables.end(); ++itMenuInfo)
//            {
//                MenuConsumableInfo* menuConsumableInfo = new MenuConsumableInfo;
//                menuConsumableInfo->Description = itMenuInfo->Description;
//                menuConsumableInfo->IsPalmable = itMenuInfo->IsPalmable;
//                menuConsumableInfo->MenuId = itMenuInfo->MenuId;
//                menuConsumableInfo->Name = itMenuInfo->Name;
//                menuConsumableInfo->Type = itMenuInfo->MenuType;
//
//                ArrayOfCourseInfo courseInfoArray;
//
//                for(std::list<TCourseInfo>::iterator itCourseInfo = itMenuInfo->SiteCourses.begin();
//                    itCourseInfo != itMenuInfo->SiteCourses.end(); ++itCourseInfo)
//                {
//                    CourseInfo* courseInfo = new CourseInfo;
//                    courseInfo->CourseId = itCourseInfo->CourseId;
//                    courseInfo->Description = itCourseInfo->Description;
//                    courseInfo->Name = itCourseInfo->Name;
//                    courseInfo->ServingCourseDescription = itCourseInfo->ServingCourseDescription;
//                    courseInfo->ServingCourseName = itCourseInfo->ServingCourseName;
//
//                    ArrayOfSiteItemInfo siteItemInfoArray;
//
//                    for(std::list<TSiteItemInfo>::iterator itSiteItemInfo = itCourseInfo->Items.begin();
//                        itSiteItemInfo != itCourseInfo->Items.end(); ++itSiteItemInfo)
//                    {
//                        SiteItemInfo* siteItemInfo = new SiteItemInfo;
//                        siteItemInfo->CompanyId = itSiteItemInfo->CompanyId;
//                        siteItemInfo->Description = itSiteItemInfo->Description;
//                        siteItemInfo->ItemUniqueId = itSiteItemInfo->ItemUniqueId;
//                        siteItemInfo->Name = itSiteItemInfo->Name;
//                        siteItemInfo->OnlyAsSide = itSiteItemInfo->OnlyAsSide;
//                        siteItemInfo->SiteItemId = itSiteItemInfo->SiteItemId;
//
//                        ArrayOfItemSizeInfo itemSizeInfoArray;
//
//                        for(std::list<TItemSizeInf>::iterator itItemSizeInfo = itSiteItemInfo->ItemSizes.begin();
//                            itItemSizeInfo != itSiteItemInfo->ItemSizes.end(); ++itItemSizeInfo)
//                        {
//                            ItemSizeInfo* itemSizeInfo = new ItemSizeInfo;
//                            itemSizeInfo->CanBePaidUsingPoints = itItemSizeInfo->CanBePaidUsingPoints;
//                            itemSizeInfo->DefaultPatronCount = itItemSizeInfo->DefaultPatronCount;
//                            itemSizeInfo->Description = itItemSizeInfo->Description;
//                            itemSizeInfo->IsFree = itItemSizeInfo->IsFree;
//                            itemSizeInfo->IsWeighted = itItemSizeInfo->IsWeighted;
//                            itemSizeInfo->ItemSizeId = itItemSizeInfo->ItemSizeId;
//                            itemSizeInfo->Name = itItemSizeInfo->Name;
//                            itemSizeInfo->OrderingUniqueId = itItemSizeInfo->OrderingUniqueId;
//                            itemSizeInfo->PointsPercentage = itItemSizeInfo->PointsPercentage;
//                            itemSizeInfo->PointsPrice = itItemSizeInfo->PointsPrice;
//                            itemSizeInfo->Price = itItemSizeInfo->Price;
//                            itemSizeInfo->ThirdPartyId = itItemSizeInfo->ThirdPartyId;
//
//                            ArrayOfItemSizeTaxProfileInfo itemSizeTaxProfileInfo;
//
//                            for(std::list<TItemSizeTaxProfileInfo>::iterator itItemSizeTaxInfo = itItemSizeInfo->ItemSizeTaxProfiles.begin();
//                            itItemSizeTaxInfo != itItemSizeInfo->ItemSizeTaxProfiles.end(); ++itItemSizeTaxInfo)
//                            {
//                                ItemSizeTaxProfileInfo* itemSizeTaxInfo = new ItemSizeTaxProfileInfo;
//                                itemSizeTaxInfo->Description = itItemSizeTaxInfo->Description;
//                                itemSizeTaxInfo->ItemSizeTaxProfileId = itItemSizeTaxInfo->ItemSizeTaxProfileId;
//                                itemSizeTaxInfo->Name = itItemSizeTaxInfo->Name;
//                                itemSizeTaxInfo->Priority = itItemSizeTaxInfo->Priority;
//                                itemSizeTaxInfo->Rate = itItemSizeTaxInfo->Rate;
//                                itemSizeTaxInfo->Type = itItemSizeTaxInfo->TaxProfileType;
//                                itemSizeTaxProfileInfo.Length = (itemSizeTaxProfileInfo.Length + 1);
//                                itemSizeTaxProfileInfo[itemSizeTaxProfileInfo.Length - 1] = itemSizeTaxInfo;
//                            }
//                            itemSizeInfo->ItemSizeTaxProfiles = itemSizeTaxProfileInfo;
//                            itemSizeInfoArray.Length = (itemSizeInfoArray.Length + 1);
//                            itemSizeInfoArray[itemSizeInfoArray.Length - 1] = itemSizeInfo;
//                        }
//                        siteItemInfo->ItemSizes = itemSizeInfoArray;
//
//                        ArrayOfSideGroupInfo itemSideGroupArray;
//
//                        for(std::list<TSideGroupInfo>::iterator itSideGroupInfo = itSiteItemInfo->SideGroup.begin();
//                            itSideGroupInfo != itSiteItemInfo->SideGroup.end(); ++itSideGroupInfo)
//                        {
//                            SideGroupInfo* sideGroupInfo = new SideGroupInfo;
//                            sideGroupInfo->AllowSkip = itSideGroupInfo->AllowSkip;
//                            sideGroupInfo->Description = itSideGroupInfo->Description;
//                            sideGroupInfo->MaxSelect = itSideGroupInfo->MaxSelect;
//                            sideGroupInfo->Name = itSideGroupInfo->Name;
//                            sideGroupInfo->SideGroupId = itSideGroupInfo->SideGroupId;
//
//                            ArrayOfItemSideInfo itemSideInfoArray;
//
//                            for(std::list<TItemSideInfo>::iterator itItemSideInfo = itSideGroupInfo->ItemSides.begin();
//                                itItemSideInfo != itSideGroupInfo->ItemSides.end(); ++itItemSideInfo)
//                            {
//                                ItemSideInfo* itemSideInfo = new ItemSideInfo;
//                                itemSideInfo->Description = itItemSideInfo->Description;
//                                itemSideInfo->ItemUniqueId = itItemSideInfo->ItemUniqueId;
//                                itemSideInfo->Name = itItemSideInfo->Name;
//                                itemSideInfo->OnlyAsSide = itItemSideInfo->OnlyAsSide;
//                                itemSideInfo->SiteItemId = itItemSideInfo->SiteItemId;
//
//                                ArrayOfItemSizeInfo itemSizeInfoArray;
//
//                                for(std::list<TItemSizeInf>::iterator itItemSizeInfo = itItemSideInfo->ItemSizes.begin();
//                                    itItemSizeInfo != itItemSideInfo->ItemSizes.end(); ++itItemSizeInfo)
//                                {
//                                    ItemSizeInfo* itemSizeInfo = new ItemSizeInfo;
//                                    itemSizeInfo->CanBePaidUsingPoints = itItemSizeInfo->CanBePaidUsingPoints;
//                                    itemSizeInfo->DefaultPatronCount = itItemSizeInfo->DefaultPatronCount;
//                                    itemSizeInfo->Description = itItemSizeInfo->Description;
//                                    itemSizeInfo->IsFree = itItemSizeInfo->IsFree;
//                                    itemSizeInfo->IsWeighted = itItemSizeInfo->IsWeighted;
//                                    itemSizeInfo->ItemSizeId = itItemSizeInfo->ItemSizeId;
//                                    itemSizeInfo->Name = itItemSizeInfo->Name;
//                                    itemSizeInfo->OrderingUniqueId = itItemSizeInfo->OrderingUniqueId;
//                                    itemSizeInfo->PointsPercentage = itItemSizeInfo->PointsPercentage;
//                                    itemSizeInfo->PointsPrice = itItemSizeInfo->PointsPrice;
//                                    itemSizeInfo->Price = itItemSizeInfo->Price;
//                                    itemSizeInfo->ThirdPartyId = itItemSizeInfo->ThirdPartyId;
//
//                                    ArrayOfItemSizeTaxProfileInfo itemSizeTaxProfileInfo;
//
//                                    for(std::list<TItemSizeTaxProfileInfo>::iterator itItemSizeTaxInfo = itItemSizeInfo->ItemSizeTaxProfiles.begin();
//                                    itItemSizeTaxInfo != itItemSizeInfo->ItemSizeTaxProfiles.end(); ++itItemSizeTaxInfo)
//                                    {
//                                        ItemSizeTaxProfileInfo* itemSizeTaxInfo = new ItemSizeTaxProfileInfo;
//                                        itemSizeTaxInfo->Description = itItemSizeTaxInfo->Description;
//                                        itemSizeTaxInfo->ItemSizeTaxProfileId = itItemSizeTaxInfo->ItemSizeTaxProfileId;
//                                        itemSizeTaxInfo->Name = itItemSizeTaxInfo->Name;
//                                        itemSizeTaxInfo->Priority = itItemSizeTaxInfo->Priority;
//                                        itemSizeTaxInfo->Rate = itItemSizeTaxInfo->Rate;
//                                        itemSizeTaxInfo->Type = itItemSizeTaxInfo->TaxProfileType;
//                                        itemSizeTaxProfileInfo.Length = (itemSizeTaxProfileInfo.Length + 1);
//                                        itemSizeTaxProfileInfo[itemSizeTaxProfileInfo.Length - 1] = itemSizeTaxInfo;
//                                    }
//                                    itemSizeInfo->ItemSizeTaxProfiles = itemSizeTaxProfileInfo;
//                                    itemSizeInfoArray.Length = (itemSizeInfoArray.Length + 1);
//                                    itemSizeInfoArray[itemSizeInfoArray.Length - 1] = itemSizeInfo;
//                                }
//                                itemSideInfo->ItemSizes = itemSizeInfoArray;
//                                itemSideInfoArray.Length = (itemSideInfoArray.Length + 1);
//                                itemSideInfoArray[itemSideInfoArray.Length - 1] = itemSideInfo;
//                            }
//                            sideGroupInfo->ItemSides = itemSideInfoArray;
//                            itemSideGroupArray.Length = (itemSideGroupArray.Length + 1);
//                            itemSideGroupArray[itemSideGroupArray.Length - 1] = sideGroupInfo;
//                        }
//                        siteItemInfo->SideGroups = itemSideGroupArray;
//                        siteItemInfoArray.Length = (siteItemInfoArray.Length + 1);
//                        siteItemInfoArray[siteItemInfoArray.Length - 1] = siteItemInfo;
//                    }
//                    courseInfo->Items = siteItemInfoArray;
//                    courseInfoArray.Length = (courseInfoArray.Length + 1);
//                    courseInfoArray[courseInfoArray.Length - 1] = courseInfo;
//                }
//                menuConsumableInfo->SiteCourses = courseInfoArray;
//                menuConsumableArray.Length = (menuConsumableArray.Length + 1);
//                menuConsumableArray[menuConsumableArray.Length - 1] = menuConsumableInfo;
//            }
//            wcfInfo->MenuConsumables = menuConsumableArray;
//        }
//        CoInitialize(NULL);
//        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
//        wcfResponse = loyaltymateClient->SyncMenu(SyndicateCode, wcfInfo);
//        return CreateMMResponse( wcfResponse );
//    }
//    catch( Exception& exc )
//    {
//        return CreateExceptionFailedResponse( exc.Message );
//    }
//}


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
//
//MMLoyaltyServiceResponse TLoyaltyMateInterface::SendTaxSettings(TSiteTaxSettingsInfo taxSettingsInfo)
//{
//    LoyaltyResponse *wcfResponse;
//    try
//    {
//        SiteTaxSettingsinfo *wcfInfo = new SiteTaxSettingsinfo();
//        wcfInfo->SiteId = taxSettingsInfo.SiteId;
//
//        if(!taxSettingsInfo.SiteTaxSettings.empty())
//        {
//            ArrayOfTaxSettingsInfo taxSettingsinfo;
//
//            for(std::list<TTaxSettingsInfo>::iterator itTaxSettingInfo = taxSettingsInfo.SiteTaxSettings.begin();
//                    itTaxSettingInfo != taxSettingsInfo.SiteTaxSettings.end(); ++itTaxSettingInfo)
//            {
//                TaxSettingsInfo* taxSetting = new TaxSettingsInfo;
//                taxSetting->SettingType = itTaxSettingInfo->SettingType;
//                taxSetting->Value = itTaxSettingInfo->Value;
//                taxSettingsinfo.Length = (taxSettingsinfo.Length + 1);
//                taxSettingsinfo[taxSettingsinfo.Length - 1] = taxSetting;
//            }
//            wcfInfo->SiteTaxSettings = taxSettingsinfo;
//        }
//        CoInitialize(NULL);
//        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
//        wcfResponse = loyaltymateClient->SyncTaxSettings(SyndicateCode, wcfInfo);
//        delete wcfInfo;
//        wcfInfo = NULL;
//        return CreateMMResponse( wcfResponse );
//    }
//    catch( Exception& exc )
//    {
//        return CreateExceptionFailedResponse( exc.Message );
//    }
//}

//MMLoyaltyServiceResponse TLoyaltyMateInterface::PostOnlineOrderInvoiceInfo(TSiteOrderModel siteOrderModel)
//{
//    LoyaltyResponse *wcfResponse;
//    try
//    {
//        SiteOrderModel *wcfInfo = new SiteOrderModel();
//        wcfInfo->CompanyId = siteOrderModel.CompanyId;
//        wcfInfo->ContainerName = siteOrderModel.ContainerName;
//        wcfInfo->ContainerNumber = siteOrderModel.ContainerNumber;
//        wcfInfo->ContainerType = siteOrderModel.ContainerType;
//        wcfInfo->IsConfirmed = siteOrderModel.IsConfirmed;
//        wcfInfo->Location = siteOrderModel.Location;
//        wcfInfo->OrderGuid = siteOrderModel.OrderGuid;
//        wcfInfo->OrderId = siteOrderModel.OrderId;
//        wcfInfo->OrderType = siteOrderModel.OrderType;
//        wcfInfo->SiteId = siteOrderModel.SiteId;
//        wcfInfo->TerminalName = siteOrderModel.TerminalName;
//        wcfInfo->TotalAmount = siteOrderModel.TotalAmount;
//        wcfInfo->UserEmailId = siteOrderModel.UserEmailId;
//        wcfInfo->TransactionType = siteOrderModel.TransactionType;
//
//        TXSDateTime* transactionDate = new TXSDateTime;
//        transactionDate->AsDateTime = siteOrderModel.TransactionDate;
//
//        wcfInfo->TransactionDate = transactionDate;
//        wcfInfo->TransactionType = siteOrderModel.TransactionType;
//        wcfInfo->UserReferenceId = siteOrderModel.UserReferenceId;
//        wcfInfo->UserType = siteOrderModel.UserType;
//
//        if(!siteOrderModel.OrderItems.empty())
//        {
//            ArrayOfOrderItemModel orderItemsModelArray;
//
//            for(std::list<TOrderItemModel>::iterator itOrderItemModel = siteOrderModel.OrderItems.begin();
//                    itOrderItemModel != siteOrderModel.OrderItems.end(); ++itOrderItemModel)
//            {
//                TOrderItemModel itemModel = *itOrderItemModel;
//                OrderItemModel* orderItemModel = CreateOrderItemModel(itemModel);
//                orderItemsModelArray.Length = (orderItemsModelArray.Length + 1);
//                orderItemsModelArray[orderItemsModelArray.Length - 1] = orderItemModel;
//            }
//            wcfInfo->OrderItems = orderItemsModelArray;
//            wcfInfo->OrderInvoiceTransaction = CreateOrderInvoiceTransaction(siteOrderModel.OrderInvoiceTransaction);
//        }
//        CoInitialize(NULL);
//        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
//        wcfResponse = loyaltymateClient->PostOnlineOrderInvoiceInfo(SyndicateCode, wcfInfo);
//        delete wcfInfo;
//        wcfInfo = NULL;
//        return CreateMMResponse( wcfResponse );
//    }
//    catch( Exception& exc )
//    {
//        return CreateExceptionFailedResponse( exc.Message );
//    }
//}
//
//OrderItemModel* TLoyaltyMateInterface::CreateOrderItemModel(TOrderItemModel itemModel)
//{
//    OrderItemModel* orderItemModel = new OrderItemModel;
//    try
//    {
//        orderItemModel->Description = itemModel.Description;
//        orderItemModel->Name = itemModel.Name;
//        orderItemModel->OrderId = itemModel.OrderId;
//        orderItemModel->OrderItemId = itemModel.OrderItemId;
//        orderItemModel->Price = itemModel.Price;
//        orderItemModel->SiteItemId = itemModel.SiteItemId;
//        if(!itemModel.OrderItemSizes.empty())
//        {
//            ArrayOfOrderItemSizeModel orderItemSizeArray;
//
//            for(std::list<TOrderItemSizeModel>::iterator itOrderItemSizeModel = itemModel.OrderItemSizes.begin();
//                    itOrderItemSizeModel != itemModel.OrderItemSizes.end(); ++itOrderItemSizeModel)
//            {
//                TOrderItemSizeModel itemSizeModel = *itOrderItemSizeModel;
//                OrderItemSizeModel* orderItemSizeModel = CreateOrderItemSizeModel(itemSizeModel);
//                orderItemSizeArray.Length = (orderItemSizeArray.Length + 1);
//                orderItemSizeArray[orderItemSizeArray.Length - 1] = orderItemSizeModel;
//            }
//            orderItemModel->OrderItemSizes = orderItemSizeArray;
//        }
//    }
//    catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return orderItemModel;
//}
//
//OrderItemSizeModel* TLoyaltyMateInterface::CreateOrderItemSizeModel(TOrderItemSizeModel itemSizeModel)
//{
//    OrderItemSizeModel* orderItemSizeModel = new OrderItemSizeModel;
//    try
//    {
//        orderItemSizeModel->OrderItemSizeId = itemSizeModel.OrderItemSizeId;
//        orderItemSizeModel->BasePrice = itemSizeModel.BasePrice;
//        orderItemSizeModel->ItemSizeId = itemSizeModel.ItemSizeId;
//        orderItemSizeModel->MenuPrice = itemSizeModel.MenuPrice;
//        orderItemSizeModel->Name = itemSizeModel.Name;
//        orderItemSizeModel->OrderItemId = itemSizeModel.OrderItemId;
//        orderItemSizeModel->Price = itemSizeModel.Price;
//        orderItemSizeModel->PriceInclusive = itemSizeModel.PriceInclusive;
//        orderItemSizeModel->Quantity = itemSizeModel.Quantity;
//        orderItemSizeModel->ReferenceOrderItemSizeId = itemSizeModel.ReferenceOrderItemSizeId;
//
//        if(!itemSizeModel.OrderItemSizeDiscounts.empty())
//        {
//            ArrayOfOrderItemSizeDiscountModel orderItemSizeDiscountArray;
//
//            for(std::list<TOrderItemSizeDiscountModel>::iterator itOrderItemSizeDiscountModel = itemSizeModel.OrderItemSizeDiscounts.begin();
//                    itOrderItemSizeDiscountModel != itemSizeModel.OrderItemSizeDiscounts.end(); ++itOrderItemSizeDiscountModel)
//            {
//                TOrderItemSizeDiscountModel itemSizeDiscountModel = *itOrderItemSizeDiscountModel;
//                OrderItemSizeDiscountModel* orderItemSizeDiscountModel = CreateOrderItemSizeDiscountModel(itemSizeDiscountModel);
//                orderItemSizeDiscountArray.Length = (orderItemSizeDiscountArray.Length + 1);
//                orderItemSizeDiscountArray[orderItemSizeDiscountArray.Length - 1] = orderItemSizeDiscountModel;
//            }
//            orderItemSizeModel->OrderItemSizeDiscounts = orderItemSizeDiscountArray;
//        }
//
//        if(!itemSizeModel.OrderItemSizeTaxProfiles.empty())
//        {
//            ArrayOfOrderItemSizeTaxProfileModel orderItemSizeTaxProfileArray;
//
//            for(std::list<TOrderItemSizeTaxProfileModel>::iterator itOrderItemSizeTaxProfileModel = itemSizeModel.OrderItemSizeTaxProfiles.begin();
//                    itOrderItemSizeTaxProfileModel != itemSizeModel.OrderItemSizeTaxProfiles.end(); ++itOrderItemSizeTaxProfileModel)
//            {
//                TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel = *itOrderItemSizeTaxProfileModel;
//                OrderItemSizeTaxProfileModel* orderItemSizeTaxProfileModel = CreateOrderItemSizeTaxProfileModel(itemSizeTaxProfileModel);
//                orderItemSizeTaxProfileArray.Length = (orderItemSizeTaxProfileArray.Length + 1);
//                orderItemSizeTaxProfileArray[orderItemSizeTaxProfileArray.Length - 1] = orderItemSizeTaxProfileModel;
//            }
//            orderItemSizeModel->OrderItemSizeTaxProfiles = orderItemSizeTaxProfileArray;
//        }
//    }
//    catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return orderItemSizeModel;
//}
//
//OrderItemSizeDiscountModel* TLoyaltyMateInterface::CreateOrderItemSizeDiscountModel(TOrderItemSizeDiscountModel itemSizeDiscountModel)
//{
//    OrderItemSizeDiscountModel* orderItemSizeDiscountModel = new OrderItemSizeDiscountModel;
//    try
//    {
//        orderItemSizeDiscountModel->Code = itemSizeDiscountModel.Code;
//        orderItemSizeDiscountModel->Name = itemSizeDiscountModel.Name;
//        orderItemSizeDiscountModel->OrderItemSizeDiscountId = itemSizeDiscountModel.OrderItemSizeDiscountId;
//        orderItemSizeDiscountModel->OrderItemSizeId = itemSizeDiscountModel.OrderItemSizeId;
//        orderItemSizeDiscountModel->Value = itemSizeDiscountModel.Value;
//    }
//    catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return orderItemSizeDiscountModel;
//}
//
//OrderItemSizeTaxProfileModel* TLoyaltyMateInterface::CreateOrderItemSizeTaxProfileModel(TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel)
//{
//    OrderItemSizeTaxProfileModel* orderItemSizeTaxProfileModel = new OrderItemSizeTaxProfileModel;
//    try
//    {
//        orderItemSizeTaxProfileModel->CompanyId = itemSizeTaxProfileModel.CompanyId;
//        orderItemSizeTaxProfileModel->Description = itemSizeTaxProfileModel.Description;
//        orderItemSizeTaxProfileModel->ItemSizeTaxProfileId = itemSizeTaxProfileModel.ItemSizeTaxProfileId;
//        orderItemSizeTaxProfileModel->Name = itemSizeTaxProfileModel.Name;
//        orderItemSizeTaxProfileModel->OrderItemSizeId = itemSizeTaxProfileModel.OrderItemSizeId;
//        orderItemSizeTaxProfileModel->OrderItemSizeTaxProfileId = itemSizeTaxProfileModel.OrderItemSizeTaxProfileId;
//        orderItemSizeTaxProfileModel->Percentage = itemSizeTaxProfileModel.Percentage;
//        orderItemSizeTaxProfileModel->Priority = itemSizeTaxProfileModel.Priority;
//        orderItemSizeTaxProfileModel->Rate = itemSizeTaxProfileModel.Rate;
//        orderItemSizeTaxProfileModel->TaxProfileType = itemSizeTaxProfileModel.TaxProfileType;
//        orderItemSizeTaxProfileModel->Value = itemSizeTaxProfileModel.Value;
//    }
//    catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return orderItemSizeTaxProfileModel;
//}
//
//OrderInvoiceTransactionModel* TLoyaltyMateInterface::CreateOrderInvoiceTransaction(TOrderInvoiceTransactionModel orderinvoiceTransaction)
//{
//    OrderInvoiceTransactionModel* orderInvoiceTransactionModel = new OrderInvoiceTransactionModel;
//    try
//    {
//        orderInvoiceTransactionModel->OrderInvoiceTransactionId = orderinvoiceTransaction.OrderInvoiceTransactionId;
//        orderInvoiceTransactionModel->OrderId = orderinvoiceTransaction.OrderId;
//        orderInvoiceTransactionModel->InvoiceTransactionId = orderinvoiceTransaction.InvoiceTransactionId;
//        orderInvoiceTransactionModel->InvoiceTransaction = CreateOrderInvoiceTransaction(orderinvoiceTransaction.InvoiceTransaction);
//    }
//     catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return orderInvoiceTransactionModel;
//}
//
//InvoiceTransactionModel* TLoyaltyMateInterface::CreateOrderInvoiceTransaction(TInvoiceTransactionModel invoiceTransaction)
//{
//    InvoiceTransactionModel* invoiceTransactionModel = new InvoiceTransactionModel;
//    try
//    {
//         invoiceTransactionModel->InvoiceTransactionId = invoiceTransaction.InvoiceTransactionId;
//         invoiceTransactionModel->InvoiceNumber = invoiceTransaction.InvoiceNumber;
//         invoiceTransactionModel->TotalSaleAmount = invoiceTransaction.TotalSaleAmount;
//         TXSDateTime* transactionDate = new TXSDateTime;
//         transactionDate->AsDateTime = invoiceTransaction.TransactionDate;
//         invoiceTransactionModel->TransactionDate = transactionDate;
//         invoiceTransactionModel->SiteId = invoiceTransaction.SiteId;
//         invoiceTransactionModel->TerminalName = invoiceTransaction.TerminalName;
//         invoiceTransactionModel->Receipt = invoiceTransaction.Receipt;
//         invoiceTransactionModel->ReceiptPath = invoiceTransaction.ReceiptPath;
//         invoiceTransactionModel->Rounding = invoiceTransaction.Rounding;
//         invoiceTransactionModel->UserReferenceId = invoiceTransaction.UserReferenceId;
//         invoiceTransactionModel->UserType = invoiceTransaction.UserType;
//    }
//    catch(Exception& exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
//        throw;
//    }
//    return invoiceTransactionModel;
//}
//-----------------------------------------------------------------------------
AnsiString TLoyaltyMateInterface::GetEmailBody(AnsiString syndicateCode, AnsiString email, AnsiString uuid)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    AnsiString emailBody = "Unable to Download Member Details.\r";
    emailBody += "Multiple Email exist for same GUID having details as:\r";
    emailBody += "Email:- " + email + "\r";
    emailBody += "Original Syndicate Code:- " + syndicateCode + "\r";
    emailBody += "Cloud UUID:- " + uuid + "\r";
    emailBody += "Site ID " + IntToStr(TGlobalSettings::Instance().SiteID) + "\r";
    emailBody += "Contact Key:- " + IntToStr(TDBContacts::GetContactByEmail(DBTransaction, email)) + "\r\r";
    emailBody += "Thank You";
    DBTransaction.Commit();
    return emailBody;
}
//------------------------------------------------------------
void TLoyaltyMateInterface::SendEmail(AnsiString emailBody)
{
    try
    {
        UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
        AnsiString Dir = ExtractFilePath(Application->ExeName) + "MemberEmails";
        if (!DirectoryExists(Dir))
        {
           CreateDir(Dir);
        }

        AnsiString filename = Dir + "\\" + "MemberDetail.txt";
        AnsiString emailId = "ravish.sharma@menumate.com;development@menumate.com;tripurary.mishra@menumatetech.com";

        TMMProcessingState State(Screen->ActiveForm, "Sending Emails...", "Sending Emails...");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        SendEmail::Send(filename, "Multiple Email exist for same GUID.", emailId, emailBody, true);
        TDeviceRealTerminal::Instance().ProcessingController.Pop();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//----------------------------------------------------------
AnsiString TLoyaltyMateInterface::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\logs";

    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\LoyaltyMate Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "LoyaltyMatePosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;

}
//---------------------------------------------------------------------------------------------------
void TLoyaltyMateInterface::AddLoyaltyLogs(std::auto_ptr<TStringList> loyaltyLogs)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        for(int index = 0; index < loyaltyLogs->Count; index++)
        {
            AnsiString value = loyaltyLogs->operator [](index);
            List->Add(value);
        }
        List->SaveToFile(fileName);
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//--

#pragma package(smart_init)
