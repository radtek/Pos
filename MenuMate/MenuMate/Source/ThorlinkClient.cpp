//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThorlinkClient.h"
#include "MMLogging.h"
#include<list>
#include <vector>
#include <DateUtils.hpp>
#include <system.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)
TThorlinkClient::TThorlinkClient()
{
   OffLine           = true;
    ResponseDescription = "";
    TraderStatusName = "";
    TraderId = "";
    FirstName = "";
    LastName = "";
    BirthDate = "";
    EmailAddress = "";
    MobileNumber = "";
    Address = "";
    LoyaltyBalance = 0.00;
   initTLClient();
}

//---------------------------------------------------------------------------
void TThorlinkClient::initTLClient()
{
    bool useWSDL = false;
    AnsiString thorlinkURL = AnsiString("http://localhost:8739/MenumateServices/ThorLink/");

    thorclient = GetIWCFServiceThorlink(
                            useWSDL, thorlinkURL, NULL );
}

//---------------------------------------------------------------------------
bool TThorlinkClient::SetAuthorizationParameters(AnsiString AppKey,AnsiString SiteNo,AnsiString MerchantCode,AnsiString DeviceCode)
{
    try{
        DTO_ThorAuth *dto_thorauth=new DTO_ThorAuth();
        dto_thorauth->appKey =  AppKey;
        dto_thorauth->siteNo =  SiteNo;
        dto_thorauth->merchantCode =  MerchantCode;
        dto_thorauth->deviceCode =  DeviceCode;
        CoInitialize(NULL);
        thorclient->Initialize(dto_thorauth);

        delete dto_thorauth;
        return true;
    }
	catch(Exception & E)
    {
      return false;
	}
}

void TThorlinkClient::GetMemberInformation(AnsiString card)
{
    DTO_ThorlinkInquiry *thorInquiry=new DTO_ThorlinkInquiry();
    DTO_TMemberInfo *responseMemberInfo;
    thorInquiry->cardNo = card;

    responseMemberInfo = thorclient->GetMemberInformation(thorInquiry)  ;
    UnicodeString message = "OK. REQUEST SUCCESSFUL";
    int a=responseMemberInfo->voucherDetailsOfThor.get_length();
    if((responseMemberInfo->responseDescription.SubString(1,22).UpperCase() == message))
    {
        ResponseDescription = responseMemberInfo->responseDescription;
        TraderStatusName = responseMemberInfo->traderStatusName;
        TraderId = responseMemberInfo->traderId;
        FirstName = responseMemberInfo->firstName;
        LastName = responseMemberInfo->lastName;
        BirthDate = responseMemberInfo->birthDate;
        EmailAddress = responseMemberInfo->emailAddress;
        MobileNumber = responseMemberInfo->mobileNumber;
        Address = responseMemberInfo->address;
        LoyaltyBalance = responseMemberInfo->loyaltyBalance;
        if(responseMemberInfo->voucherDetailsOfThor.get_length() > 0)
        {

            thorclientvouchers.clear();
            for(int i=0;i<responseMemberInfo->voucherDetailsOfThor.get_length();i++)
            {
                TVoucherDetails details;
                details.Id = responseMemberInfo->voucherDetailsOfThor[i]->id;
                details.Type = responseMemberInfo->voucherDetailsOfThor[i]->type;
                details.Code = responseMemberInfo->voucherDetailsOfThor[i]->code;
                details.PluCode = responseMemberInfo->voucherDetailsOfThor[i]->pluCode;
                details.SecurityCode = responseMemberInfo->voucherDetailsOfThor[i]->securityCode;
                details.Name = responseMemberInfo->voucherDetailsOfThor[i]->name;
                details.Value = responseMemberInfo->voucherDetailsOfThor[i]->value;
                details.Active = responseMemberInfo->voucherDetailsOfThor[i]->active;
                details.MerchantName = responseMemberInfo->voucherDetailsOfThor[i]->merchantName;
                details.ImageUrl = responseMemberInfo->voucherDetailsOfThor[i]->imageUrl;
                details.StartDate = responseMemberInfo->voucherDetailsOfThor[i]->startDate;
                details.EndDate = responseMemberInfo->voucherDetailsOfThor[i]->endDate;
                details.ExpiryDate = responseMemberInfo->voucherDetailsOfThor[i]->expiryDate;
                details.AvailableInStore = responseMemberInfo->voucherDetailsOfThor[i]->availableInStore;
                thorclientvouchers.push_back(details) ;

            }
        }
    }
    else
    {
        ResponseDescription = responseMemberInfo->responseDescription;
    }
    UnicodeString s = "123";
//    thorclient->GetTenderInformation(s);
    delete thorInquiry;
}
// Data needs to be modified in order to be transferred to Services ;
// It is being converted from Vector type to Array by dynamically increasing the
// size
TPurchaseResponse TThorlinkClient::GetTenderInformation(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList){
    DTO_TenderList *tenderList = new DTO_TenderList;
    DTO_TPurchaseInfo *purchaseResponse = new DTO_TPurchaseInfo;
    ArrayOfDTO_TenderDetails tenderDetailsArray;
    ArrayOfDTO_ItemDetails itemDetailsArray;
    for (std::vector<TTenderDetails>::iterator it = tenderDetailsList.begin(); it != tenderDetailsList.end(); ++it)
    {
        TTenderDetails tender = *it;
        DTO_TenderDetails* dto_TenderDetails = CreateTenderItem(tender);
        //Increase the length of the dynamic array..
        tenderDetailsArray.Length = (tenderDetailsArray.Length + 1);
        tenderDetailsArray[(tenderDetailsArray.Length - 1)] = dto_TenderDetails;
    }
    for (std::vector<TItemDetailsThor>::iterator items = itemsList.begin(); items != itemsList.end(); ++items)
    {
        TItemDetailsThor item = *items;
        DTO_ItemDetails* dto_ItemDetails = CreateItemThor(item);
        //Increase the length of the dynamic array..
        itemDetailsArray.Length = (itemDetailsArray.Length + 1);
        itemDetailsArray[(itemDetailsArray.Length - 1)] = dto_ItemDetails;
    }
    tenderList->tenderItemDetails = tenderDetailsArray;
    tenderList->itemDetailsList = itemDetailsArray;
    purchaseResponse = thorclient->GetTenderInformation(tenderList);
    TPurchaseResponse response;
    response.code = purchaseResponse->ResponseCode;
    response.responseMessage = purchaseResponse->ResponseMessage;
    response.cardNumber = purchaseResponse->CardNumber;
    response.creditValue = purchaseResponse->CreditValue;
    response.loyaltyValue = purchaseResponse->LoyaltyValue;
    response.transactionValue = purchaseResponse->TransactionValue;
    response.transactionNumber = purchaseResponse->TransactionNumber;
    return response;
}DTO_TenderDetails* TThorlinkClient::CreateTenderItem(TTenderDetails tender){     DTO_TenderDetails *dto_Tender = new DTO_TenderDetails;    dto_Tender->thorTenderType = tender.tenderType;    dto_Tender->tenderValue = tender.tenderValue;    dto_Tender->tenderIdentifier = tender.tenderIdentifier;
    dto_Tender->cardNo = tender.cardNo;    if(tender.sendTransactionValue == false)    {       dto_Tender->sendTransactionValue = false;    }    else    {       dto_Tender->sendTransactionValue = true;    }    return dto_Tender;}DTO_ItemDetails* TThorlinkClient::CreateItemThor(TItemDetailsThor itemThor)
{
    DTO_ItemDetails *item = new DTO_ItemDetails;
    item->thirdPartyCode = itemThor.thirdPartyCode;
    item->qty = itemThor.qty;    item->unitPrice = itemThor.unitPrice;
    return item;
}
TPurchaseResponse TThorlinkClient::GetRefundInformation(TRefundTransaction refundTransaction)
{
    DTO_RefundDetails *refundDetails = new  DTO_RefundDetails;
    DTO_TPurchaseInfo *purchaseResponse = new DTO_TPurchaseInfo;
    refundDetails->CardNo = refundTransaction.cardNumber;
    refundDetails->CreditValue = refundTransaction.creditValue;
    refundDetails->LoyaltyValue = refundTransaction.loyaltyValue;
    refundDetails->TransactionValue = refundTransaction.transactionValue;
    purchaseResponse = thorclient->GetRefundInformation(refundDetails);
    TPurchaseResponse response;
    response.code = purchaseResponse->ResponseCode;
    response.responseMessage = purchaseResponse->ResponseMessage;
    return response;
}
