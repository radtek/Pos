using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class AmountsReq
    {
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public double RequestedAmount { get; set; }
        [DataMember]
        public double TipAmount { get; set; }
        //[DataMember]
        //public double CashBackAmount { get; set; }
        //[DataMember]
        //public double PaidAmount { get; set; }
        //[DataMember]
        //public double MinimumAmountToDeliver { get; set; }
        //[DataMember]
        //public double MaximumCashBackAmount { get; set; }
        //[DataMember]
        //public double MinimumSplitAmount { get; set; }
    }
}
