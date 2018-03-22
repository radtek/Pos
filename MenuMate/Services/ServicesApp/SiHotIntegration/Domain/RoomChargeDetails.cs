using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class RoomChargeDetails
    {
        [DataMember]
        public string TransNo { get; set; }
        [DataMember]
        public string AccountNumber { get; set; }
        [DataMember]
        public List<SiHotService> ItemList { get; set; }
        [DataMember]
        public List<SiHotPayment> PaymentList { get; set; }
        [DataMember]
        public string Covers { get; set; }
        [DataMember]
        public string CoverType { get; set; }
        [DataMember]
        public string CashNo { get; set; }
        [DataMember]
        public string LinkID { get; set; }
        [DataMember]
        public string LinkText { get; set; }
        [DataMember]
        public string Shift { get; set; }
        [DataMember]
        public string Date { get; set; }
        [DataMember]
        public string Time { get; set; }
        [DataMember]
        public string IPAddress { get; set; }
        [DataMember]
        public int PortNumber { get; set; }
        [DataMember]
        public string Total { get; set; }
    }
}
