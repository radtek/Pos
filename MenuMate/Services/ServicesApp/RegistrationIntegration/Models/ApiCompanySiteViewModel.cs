using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Models
{
    public class ApiCompanySiteViewModel
    {
        public long CompanyId { get; set; }
        public string CompanyName { get; set; }
        public bool IsCompanyActive { get; set; }
        public string SyndicateCode { get; set; }
        public string SiteName { get; set; }
        public long SiteCode { get; set; }
        public bool IsSiteActive { get; set; }
        public bool IsSuccessful { get; set; }
        public string Message { get; set; }
    }
}
