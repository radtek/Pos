using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.OnlineOrdering
{
    [DataContract]
    public class SideGroupInfo
    {
        [DataMember]
        public long SideGroupId { get; set; }
        [DataMember]
        public long SiteItemId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public int MaxSelect { get; set; }
        [DataMember]
        public bool AllowSkip { get; set; }
        [DataMember]
        public List<ItemSideInfo> ItemSides { get; set; }
    }
}
