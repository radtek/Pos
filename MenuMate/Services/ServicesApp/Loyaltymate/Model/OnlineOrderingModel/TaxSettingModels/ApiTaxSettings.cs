using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyaltymate.Enum;

namespace Loyaltymate.Model.OnlineOrderingModel.TaxSettingModel
{
    public class ApiTaxSettings
    {
        public SiteSettingType SettingType { get; set; }
        public string Value { get; set; }
    }
}
