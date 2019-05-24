using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Services
{
    public class RequestAddress
    {
        //public const string BaseAddress = "http://menumateloyalty.com/";
        public const string BaseAddress = "http://menumatepos.com/";//"http://lmonlineordering.azurewebsites.net/";//"http://menumatepos.com/";

        //public const string SyncSiteMenu = "api/v5/MenuConsumableApi/SyncSiteMenu";

        //public const string SyncSiteTaxSettings = "api/v5/SiteApi/GetSiteTaxSetting";

        //public const string UpdateOrderStatus = "api/v5/OrderApi/SaveOrderConfirmation";

        //public const string PostOnlineOrderInvoiceInfo = "api/v5/OrderApi/SaveOrderBillingInformation";

        //public const string IsOnlineOrderingEnableBySiteId = "api/v5/SiteApi/IsOnlineOrderingEnableBySiteId/";

        //public const string UnsetOrderingDetails = "api/v5/SiteApi/SignalRDisconnectBySite/";

        public const string SyncSiteMenu = "api/v6/MenuConsumableApi/SyncSiteMenu";

        public const string SyncSiteTaxSettings = "api/v6/SiteApi/GetSiteTaxSetting";

        public const string UpdateOrderStatus = "api/v6/OrderApi/SaveOrderConfirmation";

        public const string PostOnlineOrderInvoiceInfo = "api/v6/OrderApi/SaveOrderBillingInformation";

        public const string IsOnlineOrderingEnableBySiteId = "api/v6/SiteApi/IsOnlineOrderingEnableBySiteId/";

        public const string UnsetOrderingDetails = "api/v6/SiteApi/SignalRDisconnectBySite/";

        public const string GetOnlineOrderingDetailsBySiteCode = "api/v6/SiteApi/GetOnlineOrderingDetailsBySiteCode/";

        public const string SendZedRequestNotification = "api/v6/SiteApi/SendZedRequestNotification";
    }
}
