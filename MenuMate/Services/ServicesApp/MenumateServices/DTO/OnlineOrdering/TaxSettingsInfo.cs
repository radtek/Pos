using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.OnlineOrdering
{
    public class TaxSettingsInfo
    {
        public TaxSettingType SettingType { get; set; }
        public string Value { get; set; }
    }
}
