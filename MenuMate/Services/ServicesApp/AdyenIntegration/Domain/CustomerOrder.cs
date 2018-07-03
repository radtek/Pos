using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class CustomerOrder
    {
        [DataMember]
        public string CustomerOrderID { get; set; }
        [DataMember]
        public string SaleReferenceID { get; set; }
        [DataMember]
        public string OpenOrderState { get; set; }
        [DataMember]
        public string StartDate { get; set; }
        [DataMember]
        public string EndDate { get; set; }
        [DataMember]
        public string ForecastedAmount { get; set; }
        [DataMember]
        public string CurrentAmount { get; set; }
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public string AccessedBy { get; set; }
        [DataMember]
        public string AdditionalInformation { get; set; }
    }
}
