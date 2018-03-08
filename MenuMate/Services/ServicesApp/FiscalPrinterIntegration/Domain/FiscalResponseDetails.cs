using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalPrinterIntegration.Domain
{
    [DataContract]
    public class FiscalResponseDetails
    {
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string Response { get; set; }
    }
}
