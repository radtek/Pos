using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.AdjustAuthorisation
{
    [DataContract]
    public class ResultNotifications
    {
        [DataMember]
        public string live { get; set; }
        [DataMember]
        public string currency { get; set; }
        [DataMember]
        public string value { get; set; }
        [DataMember]
        public string eventCode { get; set; }
        [DataMember]
        public string eventDate { get; set; }
        [DataMember]
        public string merchantAccountCode { get; set; }
        [DataMember]
        public string merchantReference { get; set; }
        [DataMember]
        public string operations { get; set; }
        [DataMember]
        public string originalReference { get; set; }
        [DataMember]
        public string paymentMethod { get; set; }
        [DataMember]
        public string pspReference { get; set; }
        [DataMember]
        public string reason { get; set; }
        [DataMember]
        public string success { get; set; }
    }
}
