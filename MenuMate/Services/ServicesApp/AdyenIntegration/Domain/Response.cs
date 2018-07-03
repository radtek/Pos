using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class Response
    {
        [DataMember]
        public string Result { get; set; }
        [DataMember]
        public string ErrorCondition { get; set; }
        [DataMember]
        public string AdditionalResponse { get; set; }
    }
}
