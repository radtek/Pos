using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PocketVoucherIntegration.RedemptionDetails
{
    [DataContract]
    public class RedemptionResponseDetails
    {
        [DataMember]
        public string ResponseCode
        {
            get;
            set;
        }

        [DataMember]
        public string ResponseMessage
        {
            get;
            set;
        }

        [DataMember]
        public string RedemptionStatus
        {
            get;
            set;
        }
    }
}
