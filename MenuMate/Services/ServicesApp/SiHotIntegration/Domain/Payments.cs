using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class Payments
    {
        [DataMember]
        public string Type
        {
            get;
            set;
        }
        [DataMember]
        public string Amount
        {
            get;
            set;
        }
        [DataMember]
        public string PaymentDescription
        {
            get;
            set;
        }
        [DataMember]
        public string BillNo
        {
            get;
            set;
        }
        [DataMember]
        public string CashNo
        {
            get;
            set;
        }
        [DataMember]
        public string Cashier
        {
            get;
            set;
        }
        [DataMember]
        public string Source
        {
            get;
            set;
        }
    }
}
