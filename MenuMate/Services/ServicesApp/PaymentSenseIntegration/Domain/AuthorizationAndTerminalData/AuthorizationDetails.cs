using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class AuthorizationDetails
    {
        [DataMember]
        public string URL { get; set; }
        [DataMember]
        public string UserName { get; set; }
        [DataMember]
        public string Password { get; set; } //Api Key
        [DataMember]
        public string TerminalId { get; set; }//tpi
    }
}
