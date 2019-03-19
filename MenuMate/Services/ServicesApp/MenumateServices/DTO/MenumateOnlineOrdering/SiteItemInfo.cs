using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class SiteItemInfo
    {
        [DataMember]
        public long SiteItemId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public string ItemUniqueId { get; set; }
        [DataMember]
        public bool OnlyAsSide { get; set; }
        [DataMember]
        public long CompanyId { get; set; }
        [DataMember]
        public long SiteMenuCourseId { get; set; }
        [DataMember]
        public List<ItemSizeInfo> ItemSizes { get; set; }
        [DataMember]
        public List<SideGroupInfo> SideGroup { get; set; }
    }
}
