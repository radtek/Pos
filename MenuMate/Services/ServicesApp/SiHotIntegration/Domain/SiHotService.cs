using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class SiHotService
    {
        [DataMember]
        public string SuperCategory { get; set; }
        [DataMember]
        public string SuperCategory_Desc { get; set; }
        [DataMember]
        public string MiddleCategory { get; set; }
        [DataMember]
        public string MiddleCategory_Desc { get; set; }
        [DataMember]
        public string ArticleCategory { get; set; }
        [DataMember]
        public string ArticleCategory_Desc { get; set; }
        [DataMember]
        public string ArticleNo { get; set; }
        [DataMember]
        public string ArticleNo_Desc { get; set; }
        [DataMember]
        public string Amount { get; set; }
        [DataMember]
        public string PricePerUnit { get; set; }
        [DataMember]
        public string PriceTotal { get; set; }
        [DataMember]
        public string Billno { get; set; }
        [DataMember]
        public string VATPercentage { get; set; }
        [DataMember]
        public string Cashno { get; set; }
        [DataMember]
        public string Cashier { get; set; }
        [DataMember]
        public string Source { get; set; }


    }
}
