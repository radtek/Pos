using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using AdyenIntegration.Domain;

namespace AdyenIntegration.Domain.LoginLogOut
{
    [DataContract]
    public class LogoutResponse
    {
        [DataMember]
        public Response Response { get; set; }
    }
}
