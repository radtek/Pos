using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class Items
    {
        [DataMember]
        public string SuperCat
        {
            get;
            set;
        }
        [DataMember]
        public string SuperCat_Desc
        {
            get;
            set;
        }
        [DataMember]
        public string MiddleCat
        {
            get;
            set;
        }
        [DataMember]
        public string MiddleCat_Desc
        {
            get;
            set;
        }
        [DataMember]
        public string ArticleCat
        {
            get;
            set;
        }
        [DataMember]
        public string ArticleCat_Desc
        {
            get;
            set;
        }
        [DataMember]
        public string Article
        {
            get;
            set;
        }
        [DataMember]
        public string Article_Desc
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
        public string PricePerUnit
        {
            get;
            set;
        }
        [DataMember]
        public string PriceTotal
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
        public string VatPercentage
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
