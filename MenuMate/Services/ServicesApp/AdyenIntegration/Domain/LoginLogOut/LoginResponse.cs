using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using AdyenIntegration.Domain;
using AdyenIntegration.Domain;

namespace AdyenIntegration.Domain.LoginLogOut
{
    [DataContract]
    public class LoginResponse
    {
        [DataMember]
        public POISystemData POISystemData { get; set; }
        [DataMember]
        public Response Response { get; set; }
    }
}
