using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.SiHot
{
    [DataContract]
    public class DTORoomRequest
    {
        [DataMember]
        public string TransNo
        {
            get;
            set;
        }
        [DataMember]
        public string RoomReq
        {
            get;
            set;
        }
    }
}
