using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class OutputContent
    {
        [DataMember]
        public string OutputFormat { get; set; }
        //[DataMember]
        //public PredefinedContent PredefinedContent { get; set; }
        [DataMember]
        public OutputText[] OutputText { get; set; }
    }
}
