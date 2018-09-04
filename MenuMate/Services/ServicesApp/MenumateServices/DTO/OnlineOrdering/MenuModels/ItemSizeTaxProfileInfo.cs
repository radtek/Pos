using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.OnlineOrdering.MenuModels
{
    [DataContract]
    public class ItemSizeTaxProfileInfo
    {
        [DataMember]
        public long ItemSizeTaxProfileId { get; set; }
        [DataMember]
        public long TaxProfileId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public double Rate { get; set; }
        [DataMember]
        public TaxProfileType Type { get; set; }
        [DataMember]
        public int Priority { get; set; }
    }
}
