using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class StoreTicketDetails
    {
        [DataMember]
        public string TransNo { get; set; }
        [DataMember]
        public string StoreTicket { get; set; }
        [DataMember]
        public string Billno { get; set; }
        [DataMember]
        public string Cashno { get; set; }
        [DataMember]
        public string Signature { get; set; }
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Document { get; set; }
        [DataMember]
        public string IPAddress { get; set; }
        [DataMember]
        public int PortNumber { get; set; }

    }
}
