using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
    [DataContract]
    public class SignaturItemLocal
    {
        [DataMember]
        public long SignatureFormat { get; set; }
        [DataMember]
        public long SignatureType { get; set; }
        [DataMember]
        public string Caption { get; set; }
        [DataMember]
        public string Data { get; set; }        
    }
}
