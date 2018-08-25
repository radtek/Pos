using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel.TaxSettingModel
{
    public class ApiSiteTaxSettings
    {
        public long SiteId { get; set; }
        public List<ApiTaxSettings> SiteTaxSettings { get; set; }
    }
}
