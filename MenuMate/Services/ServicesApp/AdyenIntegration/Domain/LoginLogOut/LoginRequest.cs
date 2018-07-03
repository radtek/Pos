using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.LoginLogOut
{
    [DataContract]
    public class LoginRequest
    {
        [DataMember]
        public string DateTime { get; set; }
        [DataMember]
        public SaleSoftware SaleSoftware { get; set; }
        [DataMember]
        public SaleTerminalData SaleTerminalData { get; set; }
        [DataMember]
        public string OperatorLanguage { get; set; }
        [DataMember]
        public string OperatorID { get; set; }
    }
}
