using OnlineOrdering.Model;
using System.Collections.Generic;
using OnlineOrdering.Model.MenuModels;
using OnlineOrdering.Model.TaxSettingsModels;
using OnlineOrdering.Model.OrderModels;

namespace OnlineOrdering.Services
{
    public interface IOnlineOrderingService
    {
        bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenus);
        bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings);
        bool UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel);
        bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel);
        ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int inSiteCode);
        bool UnsetOrderingDetails(string inSyndicateCode, int inSiteCode);
    }
}
