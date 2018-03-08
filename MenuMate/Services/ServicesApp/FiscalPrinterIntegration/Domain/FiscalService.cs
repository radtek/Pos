using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalPrinterIntegration.Domain
{
    [DataContract]
    public class FiscalService
    {
        [DataMember]
        public string TableNo { get; set; }
        [DataMember]
        public string GuestName { get; set; }
        [DataMember]
        public string ChitNumber { get; set; }
        [DataMember]
        public string MemberName { get; set; }
        [DataMember]
        public string PartyName { get; set; }
        [DataMember]
        public string ItemDescription { get; set; }
        [DataMember]
        public string SizeName { get; set; }
        [DataMember]
        public string quantity { get; set; }
        [DataMember]
        public string ItemCategory { get; set; }
        [DataMember]
        public string PricePerUnit { get; set; }
        [DataMember]
        public string PriceTotal { get; set; }
        [DataMember]
        public string VATPercentage { get; set; }
    }
}
