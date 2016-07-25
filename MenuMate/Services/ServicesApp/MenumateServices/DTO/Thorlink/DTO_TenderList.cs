using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_TenderList
    {
        [DataMember]
        public List<DTO_TenderDetails> tenderItemDetails
        {
            get;
            set;
        }
        [DataMember]
        public List<DTO_ItemDetails> itemDetailsList
        {
            get;
            set;
        }
    }
}
