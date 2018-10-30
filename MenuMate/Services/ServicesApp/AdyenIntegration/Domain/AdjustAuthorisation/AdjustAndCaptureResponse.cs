using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.AdjustAuthorisation
{
    [DataContract]
    public class AdjustAndCaptureResponse
    {
        [DataMember]
        public string pspReference { get; set; }
        [DataMember]
        public string[] response { get; set; }
    }
}
