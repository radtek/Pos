using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Diagnosis
{
    [DataContract]
    public class EventNotification
    {
        [DataMember]
        public string RejectedMessage { get; set; }
        [DataMember]
        public string EventToNotify { get; set; }
        [DataMember]
        public string TimeStamp { get; set; }
        [DataMember]
        public string EventDetails { get; set; }
    }
}
