using OnlineOrdering.Model;
using System.Collections.Generic;
using OnlineOrdering.Model.MenuModels;
using OnlineOrdering.Model.TaxSettingsModels;
using OnlineOrdering.Model.OrderModels;
using OnlineOrdering.Model.NotificationModels;

namespace OnlineOrdering.Services
{
    public interface IOnlineOrderingService
    {
        bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenus, List<string> stringList);
        bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings, List<string> stringList);
        bool UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel, List<string> stringList);
        bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel, List<string> stringList);
        ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int inSiteCode, List<string> stringList);
        bool UnsetOrderingDetails(string inSyndicateCode, int inSiteCode, List<string> stringList);
        OnlineOrderingDetailsResponse GetOnlineOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList);
        bool SendZedRequestNotification(string inSyndicateCode, ApiZedRequestNotificationViewModel apiZedRequestNotificationViewModel, List<string> stringList);
    }
}
