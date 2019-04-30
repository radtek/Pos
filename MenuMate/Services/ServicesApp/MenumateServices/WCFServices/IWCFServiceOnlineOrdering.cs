using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using MenumateServices.DTO.MenumateOnlineOrdering;
using MenumateServices.DTO.MenumateOnlineOrdering.OrderModels;
using MenumateServices.DTO.MenumateOnlineOrdering.MenuModels;


namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceOnlineOrdering
    {
        [OperationContract]
        OOLoyaltyResponse SyncMenu(string inSyndicateCode, SiteMenuInfo siteViewModel);

        [OperationContract]
        OOLoyaltyResponse SyncTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings);

        [OperationContract]
        void GetOrdersFromWeb(string inSyndicateCode, string orders);

        [OperationContract]
        OOLoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel);

        [OperationContract]
        LoyaltyOOResponse SyncOnlineOrderingDetails(string inSyndicateCode, int siteCode);

        [OperationContract]
        bool UnsetOrderingDetails(string inSyndicateCode, int siteCode);

        [OperationContract]
        void InsertWaiterTerminal(string terminalInfo);

        [OperationContract]
        void CreateRequestForAppZed(string zedRequest);


        //Add CreateTerminals
    }
}
