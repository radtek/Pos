using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.RegistrationModels
{
    public class ApiLicenceSetting
    {
        public string SettingType { get; set; }
        public string SettingSubType { get; set; }
        public bool IsActive { get; set; }
    }
}
