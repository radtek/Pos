using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Models
{
    public class ApiCompanySiteViewModel
    {
        public string CompanyName { get; set; }
        public bool IsSuccessful { get; set; }
        public string Message { get; set; }
    }
}
