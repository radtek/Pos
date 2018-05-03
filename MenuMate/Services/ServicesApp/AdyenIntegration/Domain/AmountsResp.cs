using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class AmountsResp
    {
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public double AuthorizedAmount { get; set; }
        [DataMember]
        public double TotalRebatesAmount { get; set; }
        [DataMember]
        public double TotalFeesAmount { get; set; }
        [DataMember]
        public double CashBackAmount { get; set; }
        [DataMember]
        public double TipAmount { get; set; }
    }
}
