using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.DTO.OnlineOrdering;
using MenumateServices.DTO.OnlineOrdering.OrderModels;
using MenumateServices.DTO.OnlineOrdering.MenuModels;
using MenumateServices.DTO.MenumateRegistration;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceLoyaltyMate" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceLoyaltyMate
    {
        // Member Operations
        [OperationContract]
        LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs);

        [OperationContract]
        VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction, List<string> loyaltyLogs);

        [OperationContract]
        LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo, List<string> loyaltyLogs);

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

        //[OperationContract]
        //void UpdateRegistrationDetails(Site siteInfo);
    }
}
