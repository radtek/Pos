using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_ItemDetails
    {
        [DataMember]
        public string thirdPartyCode
        {
            get;
            set;
        }

        [DataMember]
        public double qty
        {
            get;
            set;
        }

        [DataMember]
        public double unitPrice
        {
            get;
            set;
        }      
    }
}
