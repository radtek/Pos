using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class Reports
    {
        [DataMember]
        public string reportType { get; set; } // property is starting with small letter because of  format.
    }
}
