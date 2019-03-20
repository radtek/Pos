//---------------------------------------------------------------------------

#ifndef OnlineOrderingInterfaceH
#define OnlineOrderingInterfaceH
#include <StdCtrls.hpp>
#include "LogsDB.h"
#include "MM_DBCore.h"
#include "LoyaltyMateUtilities.h"
#include "OnlineOrderingWSDL.h"
#include "OnlineOrderingAttributes.h"
//---------------------------------------------------------------------------

class TOnlineOrderingInterface
{
    public:
    TOnlineOrderingInterface();
    ~TOnlineOrderingInterface();
    MMLoyaltyServiceResponse SendMenu(TSiteMenuInfo menuInfo);
    MMLoyaltyServiceResponse SendTaxSettings(TSiteTaxSettingsInfo taxSettingsInfo);
    MMLoyaltyServiceResponse PostOnlineOrderInvoiceInfo(TSiteOrderModel siteOrderModel);
    MMLoyaltyServiceResponse SyncOnlineOrderingDetails(TSyndCode syndicateCode,int siteCode);
    bool UnsetOrderingDetails(TSyndCode syndicateCode,int siteCode);

    private:
    _di_IWCFServiceOnlineOrdering onlineOrderingClient;
    void InitOnlineOrderingClient();
    AnsiString GetSyndCodeForOnlineOrdering();
    MMLoyaltyServiceResponse CreateMMResponse(OOLoyaltyResponse* inWCFResponse );
    MMLoyaltyServiceResponse CreateExceptionFailedResponse(AnsiString inMessage );
    OrderItemModel* CreateOrderItemModel(TOrderItemModel itemModel);
    OrderInvoiceTransactionModel* CreateOrderInvoiceTransaction(TOrderInvoiceTransactionModel orderinvoiceTransaction);
    OrderItemSizeModel* CreateOrderItemSizeModel(TOrderItemSizeModel itemSizeModel);
    OrderItemSizeDiscountModel* CreateOrderItemSizeDiscountModel(TOrderItemSizeDiscountModel itemSizeDiscountModel);
    InvoiceTransactionModel* CreateOrderInvoiceTransaction(TInvoiceTransactionModel invoiceTransaction);
    OrderItemSizeTaxProfileModel* CreateOrderItemSizeTaxProfileModel(TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel);
    MMLoyaltyServiceResponse CreateMMResponse(LoyaltyOOResponse*  inWCFResponse);

};
#endif
