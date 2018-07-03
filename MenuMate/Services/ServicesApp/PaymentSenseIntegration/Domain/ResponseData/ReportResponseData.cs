using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain.ResponseData
{
    [DataContract]
    public class ReportResponseData
    {
        [DataMember]
        public ReceiptData[] reportLines { get; set; }
        [DataMember]
        public List<string> Report { get; set; }
        [DataMember]
        public string reportResult { get; set; }
        [DataMember]
        public string reportTime { get; set; }
        [DataMember]
        public string reportType { get; set; }
        [DataMember]
        public string tpi { get; set; }
        [DataMember]
        public string location { get; set; }
        [DataMember]
        public string requestId { get; set; }
        [DataMember]
        public string[] notifications { get; set; }
    }
}
