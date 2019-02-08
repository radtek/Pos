using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Models
{
    public class ApiTerminalViewModel
    {
        public long SiteCode { get; set; }
        public string SyndicateCode { get; set; }
        public string TerminalName { get; set; }
        public string TerminalDescription { get; set; }
        public string StaffName { get; set; }
        public string MacAdress { get; set; }
        public string ComputerName { get; set; }
        public string OperatingSystemName { get; set; }
        public string MenumateVersion { get; set; }
        public DateTime RegistrationTime { get; set; }
        public List<ApiLicenceSetting> ApiLicenceSettings { get; set; }
    }
}
