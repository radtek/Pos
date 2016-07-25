using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.WebMate.DTO
{
    /// <summary>
    /// 
    /// </summary>
    [DataContract]
    public enum WebOrderStatus
    {
        [EnumMember]
        Unknown,
        [EnumMember]
        Committed,
        [EnumMember]
        NotAccepted,
        [EnumMember]
        Accepted,
        [EnumMember]
        Incomplete
    }
}
