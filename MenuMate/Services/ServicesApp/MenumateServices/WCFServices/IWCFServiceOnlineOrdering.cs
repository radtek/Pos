using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.DTO.MenumateOnlineOrdering;
using MenumateServices.DTO.MenumateOnlineOrdering.OrderModels;
using MenumateServices.DTO.MenumateOnlineOrdering.MenuModels;


namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceOnlineOrdering
    {
        [OperationContract]
        LoyaltyResponse SyncMenu(string inSyndicateCode, SiteMenuInfo siteViewModel);

        [OperationContract]
        LoyaltyResponse SyncTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings);

        [OperationContract]
        void GetOrdersFromWeb(string inSyndicateCode, string orders);

        [OperationContract]
        LoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel);

        [OperationContract]
        LoyaltyOnlineOrderingResponse SyncOnlineOrderingDetails(string inSyndicateCode, int siteCode);

        [OperationContract]
        bool UnsetOrderingDetails(string inSyndicateCode, int siteCode);
    }
}
