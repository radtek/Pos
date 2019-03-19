using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.TaxSettingsModels
{
    public class ApiTaxSettings
    {
        public SiteSettingType SettingType { get; set; }
        public string Value { get; set; }
    }
}
