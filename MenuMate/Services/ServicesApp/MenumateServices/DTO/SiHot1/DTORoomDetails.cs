using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.SiHot
{
    [DataContract]
    public class DTORoomDetails
    {
        [DataMember]
        public string TransNo
        {
            get;
            set;
        }
        [DataMember]
        public List<DTOGuestDetails> GuestDetailsList
        {
            get;
            set;
        }
    }
}
