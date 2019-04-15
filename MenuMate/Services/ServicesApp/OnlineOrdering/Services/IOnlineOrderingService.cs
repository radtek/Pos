using OnlineOrdering.Model;
using System.Collections.Generic;
using OnlineOrdering.Model.MenuModels;
using OnlineOrdering.Model.TaxSettingsModels;
using OnlineOrdering.Model.OrderModels;

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
    }
}
