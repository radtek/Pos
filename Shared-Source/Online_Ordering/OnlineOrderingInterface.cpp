//---------------------------------------------------------------------------


#pragma hdrstop

#include "OnlineOrderingInterface.h"
#include "DeviceRealTerminal.h"
#include "Modules.h"
#include "DateUtils.hpp"
#include <map>


//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TOnlineOrderingInterface::TOnlineOrderingInterface()
{
    InitOnlineOrderingClient();
}
//---------------------------------------------------------------------------
TOnlineOrderingInterface::~TOnlineOrderingInterface()
{

}
//---------------------------------------------------------------------------
void TOnlineOrderingInterface::InitOnlineOrderingClient()
{
    bool useWSDL = false;
    AnsiString onlineOrderingURL = AnsiString("http://localhost:8750/MenumateServices/OnlineOrdering/");
    onlineOrderingClient = GetIWCFServiceOnlineOrdering(useWSDL, onlineOrderingURL, NULL );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::SendMenu(TSiteMenuInfo menuInfo)
{
    OOLoyaltyResponse *wcfResponse;
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

                        for(std::list<TItemSizeInf>::iterator itItemSizeInfo = itSiteItemInfo->ItemSizes.begin();
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

                                for(std::list<TItemSizeInf>::iterator itItemSizeInfo = itItemSideInfo->ItemSizes.begin();
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
        wcfResponse = onlineOrderingClient->SyncMenu(SyndicateCode, wcfInfo);
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::SendTaxSettings(TSiteTaxSettingsInfo taxSettingsInfo)
{
    OOLoyaltyResponse *wcfResponse;
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
        wcfResponse = onlineOrderingClient->SyncTaxSettings(SyndicateCode, wcfInfo);
        delete wcfInfo;
        wcfInfo = NULL;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::PostOnlineOrderInvoiceInfo(TSiteOrderModel siteOrderModel)
{
    OOLoyaltyResponse *wcfResponse;
    try
    {
        SiteOrderModel *wcfInfo = new SiteOrderModel();
        wcfInfo->CompanyId = siteOrderModel.CompanyId;
        wcfInfo->ContainerName = siteOrderModel.ContainerName;
        wcfInfo->ContainerNumber = siteOrderModel.ContainerNumber;
        wcfInfo->ContainerType = siteOrderModel.ContainerType;
        wcfInfo->IsConfirmed = siteOrderModel.IsConfirmed;
        wcfInfo->Location = siteOrderModel.Location;
        wcfInfo->OrderGuid = siteOrderModel.OrderGuid;
        wcfInfo->OrderId = siteOrderModel.OrderId;
        wcfInfo->OrderType = siteOrderModel.OrderType;
        wcfInfo->SiteId = siteOrderModel.SiteId;
        wcfInfo->TerminalName = siteOrderModel.TerminalName;
        wcfInfo->TotalAmount = siteOrderModel.TotalAmount;
        wcfInfo->UserEmailId = siteOrderModel.UserEmailId;
        wcfInfo->TransactionType = siteOrderModel.TransactionType;

        TXSDateTime* transactionDate = new TXSDateTime;
        transactionDate->AsDateTime = siteOrderModel.TransactionDate;

        wcfInfo->TransactionDate = transactionDate;
        wcfInfo->TransactionType = siteOrderModel.TransactionType;
        wcfInfo->UserReferenceId = siteOrderModel.UserReferenceId;
        wcfInfo->UserType = siteOrderModel.UserType;

        if(!siteOrderModel.OrderItems.empty())
        {
            ArrayOfOrderItemModel orderItemsModelArray;

            for(std::list<TOrderItemModel>::iterator itOrderItemModel = siteOrderModel.OrderItems.begin();
                    itOrderItemModel != siteOrderModel.OrderItems.end(); ++itOrderItemModel)
            {
                TOrderItemModel itemModel = *itOrderItemModel;
                OrderItemModel* orderItemModel = CreateOrderItemModel(itemModel);
                orderItemsModelArray.Length = (orderItemsModelArray.Length + 1);
                orderItemsModelArray[orderItemsModelArray.Length - 1] = orderItemModel;
            }
            wcfInfo->OrderItems = orderItemsModelArray;
            wcfInfo->OrderInvoiceTransaction = CreateOrderInvoiceTransaction(siteOrderModel.OrderInvoiceTransaction);
        }
        CoInitialize(NULL);
        AnsiString SyndicateCode = GetSyndCodeForOnlineOrdering();
        wcfResponse = onlineOrderingClient->PostOnlineOrderInvoiceInfo(SyndicateCode, wcfInfo);
        delete wcfInfo;
        wcfInfo = NULL;
        return CreateMMResponse( wcfResponse );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
AnsiString TOnlineOrderingInterface::GetSyndCodeForOnlineOrdering()
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
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::CreateMMResponse(OOLoyaltyResponse* inWCFResponse )
{
    return MMLoyaltyServiceResponse(
                inWCFResponse->Successful,
                AnsiString( inWCFResponse->Message.t_str() ),
                AnsiString( inWCFResponse->Description.t_str() ),
                ( MMLoyaltyResponseCode )inWCFResponse->ResponseCode,
                "" );
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::CreateMMResponse(LoyaltyOOResponse*  inWCFResponse)
{
     return MMLoyaltyServiceResponse(
                inWCFResponse->IsSuccessful,
                AnsiString( inWCFResponse->ResponseText.t_str() ),
                "",
                Successful,
                "" ) ;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::CreateExceptionFailedResponse(AnsiString inMessage )
{
   if(inMessage.Pos("XML") > 0 || inMessage.Pos("The handle") > 0 )
   return MMLoyaltyServiceResponse(false,"Not able to connect with server.","",FailedDueToException,AnsiString( "" ) );
   else
     return MMLoyaltyServiceResponse(false,inMessage,"",FailedDueToException,AnsiString( "" ) );
}
//---------------------------------------------------------------------------
OrderItemModel* TOnlineOrderingInterface::CreateOrderItemModel(TOrderItemModel itemModel)
{
    OrderItemModel* orderItemModel = new OrderItemModel;
    try
    {
        orderItemModel->Description = itemModel.Description;
        orderItemModel->Name = itemModel.Name;
        orderItemModel->OrderId = itemModel.OrderId;
        orderItemModel->OrderItemId = itemModel.OrderItemId;
        orderItemModel->Price = itemModel.Price;
        orderItemModel->SiteItemId = itemModel.SiteItemId;
        if(!itemModel.OrderItemSizes.empty())
        {
            ArrayOfOrderItemSizeModel orderItemSizeArray;

            for(std::list<TOrderItemSizeModel>::iterator itOrderItemSizeModel = itemModel.OrderItemSizes.begin();
                    itOrderItemSizeModel != itemModel.OrderItemSizes.end(); ++itOrderItemSizeModel)
            {
                TOrderItemSizeModel itemSizeModel = *itOrderItemSizeModel;
                OrderItemSizeModel* orderItemSizeModel = CreateOrderItemSizeModel(itemSizeModel);
                orderItemSizeArray.Length = (orderItemSizeArray.Length + 1);
                orderItemSizeArray[orderItemSizeArray.Length - 1] = orderItemSizeModel;
            }
            orderItemModel->OrderItemSizes = orderItemSizeArray;
        }
    }
    catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return orderItemModel;
}
//---------------------------------------------------------------------------
OrderInvoiceTransactionModel* TOnlineOrderingInterface::CreateOrderInvoiceTransaction(TOrderInvoiceTransactionModel orderinvoiceTransaction)
{
    OrderInvoiceTransactionModel* orderInvoiceTransactionModel = new OrderInvoiceTransactionModel;
    try
    {
        orderInvoiceTransactionModel->OrderInvoiceTransactionId = orderinvoiceTransaction.OrderInvoiceTransactionId;
        orderInvoiceTransactionModel->OrderId = orderinvoiceTransaction.OrderId;
        orderInvoiceTransactionModel->InvoiceTransactionId = orderinvoiceTransaction.InvoiceTransactionId;
        orderInvoiceTransactionModel->InvoiceTransaction = CreateOrderInvoiceTransaction(orderinvoiceTransaction.InvoiceTransaction);
    }
     catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return orderInvoiceTransactionModel;
}
//---------------------------------------------------------------------------
InvoiceTransactionModel* TOnlineOrderingInterface::CreateOrderInvoiceTransaction(TInvoiceTransactionModel invoiceTransaction)
{
    InvoiceTransactionModel* invoiceTransactionModel = new InvoiceTransactionModel;
    try
    {
         invoiceTransactionModel->InvoiceTransactionId = invoiceTransaction.InvoiceTransactionId;
         invoiceTransactionModel->InvoiceNumber = invoiceTransaction.InvoiceNumber;
         invoiceTransactionModel->TotalSaleAmount = invoiceTransaction.TotalSaleAmount;
         TXSDateTime* transactionDate = new TXSDateTime;
         transactionDate->AsDateTime = invoiceTransaction.TransactionDate;
         invoiceTransactionModel->TransactionDate = transactionDate;
         invoiceTransactionModel->SiteId = invoiceTransaction.SiteId;
         invoiceTransactionModel->TerminalName = invoiceTransaction.TerminalName;
         invoiceTransactionModel->Receipt = invoiceTransaction.Receipt;
         invoiceTransactionModel->ReceiptPath = invoiceTransaction.ReceiptPath;
         invoiceTransactionModel->Rounding = invoiceTransaction.Rounding;
         invoiceTransactionModel->UserReferenceId = invoiceTransaction.UserReferenceId;
         invoiceTransactionModel->UserType = invoiceTransaction.UserType;
    }
    catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return invoiceTransactionModel;
}
//---------------------------------------------------------------------------
OrderItemSizeModel* TOnlineOrderingInterface::CreateOrderItemSizeModel(TOrderItemSizeModel itemSizeModel)
{
    OrderItemSizeModel* orderItemSizeModel = new OrderItemSizeModel;
    try
    {
        orderItemSizeModel->OrderItemSizeId = itemSizeModel.OrderItemSizeId;
        orderItemSizeModel->BasePrice = itemSizeModel.BasePrice;
        orderItemSizeModel->ItemSizeId = itemSizeModel.ItemSizeId;
        orderItemSizeModel->MenuPrice = itemSizeModel.MenuPrice;
        orderItemSizeModel->Name = itemSizeModel.Name;
        orderItemSizeModel->OrderItemId = itemSizeModel.OrderItemId;
        orderItemSizeModel->Price = itemSizeModel.Price;
        orderItemSizeModel->PriceInclusive = itemSizeModel.PriceInclusive;
        orderItemSizeModel->Quantity = itemSizeModel.Quantity;
        orderItemSizeModel->ReferenceOrderItemSizeId = itemSizeModel.ReferenceOrderItemSizeId;

        if(!itemSizeModel.OrderItemSizeDiscounts.empty())
        {
            ArrayOfOrderItemSizeDiscountModel orderItemSizeDiscountArray;

            for(std::list<TOrderItemSizeDiscountModel>::iterator itOrderItemSizeDiscountModel = itemSizeModel.OrderItemSizeDiscounts.begin();
                    itOrderItemSizeDiscountModel != itemSizeModel.OrderItemSizeDiscounts.end(); ++itOrderItemSizeDiscountModel)
            {
                TOrderItemSizeDiscountModel itemSizeDiscountModel = *itOrderItemSizeDiscountModel;
                OrderItemSizeDiscountModel* orderItemSizeDiscountModel = CreateOrderItemSizeDiscountModel(itemSizeDiscountModel);
                orderItemSizeDiscountArray.Length = (orderItemSizeDiscountArray.Length + 1);
                orderItemSizeDiscountArray[orderItemSizeDiscountArray.Length - 1] = orderItemSizeDiscountModel;
            }
            orderItemSizeModel->OrderItemSizeDiscounts = orderItemSizeDiscountArray;
        }

        if(!itemSizeModel.OrderItemSizeTaxProfiles.empty())
        {
            ArrayOfOrderItemSizeTaxProfileModel orderItemSizeTaxProfileArray;

            for(std::list<TOrderItemSizeTaxProfileModel>::iterator itOrderItemSizeTaxProfileModel = itemSizeModel.OrderItemSizeTaxProfiles.begin();
                    itOrderItemSizeTaxProfileModel != itemSizeModel.OrderItemSizeTaxProfiles.end(); ++itOrderItemSizeTaxProfileModel)
            {
                TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel = *itOrderItemSizeTaxProfileModel;
                OrderItemSizeTaxProfileModel* orderItemSizeTaxProfileModel = CreateOrderItemSizeTaxProfileModel(itemSizeTaxProfileModel);
                orderItemSizeTaxProfileArray.Length = (orderItemSizeTaxProfileArray.Length + 1);
                orderItemSizeTaxProfileArray[orderItemSizeTaxProfileArray.Length - 1] = orderItemSizeTaxProfileModel;
            }
            orderItemSizeModel->OrderItemSizeTaxProfiles = orderItemSizeTaxProfileArray;
        }
    }
    catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return orderItemSizeModel;
}
//---------------------------------------------------------------------------
OrderItemSizeDiscountModel* TOnlineOrderingInterface::CreateOrderItemSizeDiscountModel(TOrderItemSizeDiscountModel itemSizeDiscountModel)
{
    OrderItemSizeDiscountModel* orderItemSizeDiscountModel = new OrderItemSizeDiscountModel;
    try
    {
        orderItemSizeDiscountModel->Code = itemSizeDiscountModel.Code;
        orderItemSizeDiscountModel->Name = itemSizeDiscountModel.Name;
        orderItemSizeDiscountModel->OrderItemSizeDiscountId = itemSizeDiscountModel.OrderItemSizeDiscountId;
        orderItemSizeDiscountModel->OrderItemSizeId = itemSizeDiscountModel.OrderItemSizeId;
        orderItemSizeDiscountModel->Value = itemSizeDiscountModel.Value;
    }
    catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return orderItemSizeDiscountModel;
}
//---------------------------------------------------------------------------
OrderItemSizeTaxProfileModel* TOnlineOrderingInterface::CreateOrderItemSizeTaxProfileModel(TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel)
{
    OrderItemSizeTaxProfileModel* orderItemSizeTaxProfileModel = new OrderItemSizeTaxProfileModel;
    try
    {
        orderItemSizeTaxProfileModel->CompanyId = itemSizeTaxProfileModel.CompanyId;
        orderItemSizeTaxProfileModel->Description = itemSizeTaxProfileModel.Description;
        orderItemSizeTaxProfileModel->ItemSizeTaxProfileId = itemSizeTaxProfileModel.ItemSizeTaxProfileId;
        orderItemSizeTaxProfileModel->Name = itemSizeTaxProfileModel.Name;
        orderItemSizeTaxProfileModel->OrderItemSizeId = itemSizeTaxProfileModel.OrderItemSizeId;
        orderItemSizeTaxProfileModel->OrderItemSizeTaxProfileId = itemSizeTaxProfileModel.OrderItemSizeTaxProfileId;
        orderItemSizeTaxProfileModel->Percentage = itemSizeTaxProfileModel.Percentage;
        orderItemSizeTaxProfileModel->Priority = itemSizeTaxProfileModel.Priority;
        orderItemSizeTaxProfileModel->Rate = itemSizeTaxProfileModel.Rate;
        orderItemSizeTaxProfileModel->TaxProfileType = itemSizeTaxProfileModel.TaxProfileType;
        orderItemSizeTaxProfileModel->Value = itemSizeTaxProfileModel.Value;
    }
    catch(Exception& exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, exc.Message);
        throw;
    }
    return orderItemSizeTaxProfileModel;
}
//---------------------------------------------------------------------------
MMLoyaltyServiceResponse TOnlineOrderingInterface::SyncOnlineOrderingDetails(TSyndCode syndicateCode,int siteCode)
{
    try
    {
        LoyaltyOOResponse* response;
        CoInitialize(NULL);
        response = onlineOrderingClient->SyncOnlineOrderingDetails(syndicateCode.GetSyndCode(),siteCode);
        return CreateMMResponse( response );
    }
    catch( Exception& exc )
    {
        return CreateExceptionFailedResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
bool TOnlineOrderingInterface::UnsetOrderingDetails(TSyndCode syndicateCode,int siteCode)
{

    bool isSuccesssful = false;
    try
    {
        CoInitialize(NULL);
        isSuccesssful = onlineOrderingClient->UnsetOrderingDetails(syndicateCode.GetSyndCode(),siteCode);
     }
    catch( Exception& exc )
    {
    }
    return isSuccesssful;
}
//---------------------------------------------------------------------------
MMOnlineOrderingResponse TOnlineOrderingInterface::GetOnlineOrderingDetails(TSyndCode syndicateCode,int siteCode)
{
    try
    {
        OnlineOrderingDetails* response;
        CoInitialize(NULL);
        response = onlineOrderingClient->GetOnlineOrderingDetailsBySiteCode(syndicateCode.GetSyndCode(),siteCode);
        MMOnlineOrderingResponse mmResponse =  CreateMMOnlineOrderingResponse( response );
        return mmResponse;
    }
    catch( Exception& exc )
    {
        return CreateExceptionMMOnlineOrderingResponse( exc.Message );
    }
}
//---------------------------------------------------------------------------
MMOnlineOrderingResponse TOnlineOrderingInterface::CreateMMOnlineOrderingResponse(OnlineOrderingDetails* response)
{
    return MMOnlineOrderingResponse(
            response->IsSuccessful,
            response->IsMemberOrderingEnabled,
            response->IsWaiterOrderingEnabled,
            AnsiString( response->ResponseText.t_str() ),
            "",
            Successful) ;
}
//---------------------------------------------------------------------------
MMOnlineOrderingResponse TOnlineOrderingInterface::CreateExceptionMMOnlineOrderingResponse(UnicodeString inMessage)
{
   if(inMessage.Pos("XML") > 0 || inMessage.Pos("The handle") > 0 )
       return MMOnlineOrderingResponse(false,false,false,"Not able to connect with server.","",FailedDueToException);
   else
       return MMOnlineOrderingResponse(false,false,false,inMessage,"",FailedDueToException);
}
//---------------------------------------------------------------------------
