using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
    [DataContract]
    public class InitResponse
    {
        [DataMember]
        public int ResponseCode { get; set; }
    }
}
