using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleTerminalData
    {
        [DataMember]
        public string TerminalEnvironment { get; set; }
        //[DataMember]
        // cluster
        [DataMember]
        public SaleProfile SaleProfile { get; set; }
        [DataMember]
        public string TotalsGroupID { get; set; } 
    }
}
