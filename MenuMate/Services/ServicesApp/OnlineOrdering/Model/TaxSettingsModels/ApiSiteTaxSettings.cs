using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.TaxSettingsModels
{
    public class ApiSiteTaxSettings
    {
        public long SiteId { get; set; }
        public List<ApiTaxSettings> ApiTaxSettings { get; set; }
    }
}
