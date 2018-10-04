using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyaltymate.Enum;

namespace Loyaltymate.Model.OnlineOrderingModel.OrderModels
{
    public class ApiSiteOrderTypeInfoViewModel
    {
        public long SiteId { get; set; }
        public SiteSettingType SiteTransactionType { get; set; }
        public string SiteTransactionTypeName { get; set; }
        public string Text { get; set; }
    }
}
