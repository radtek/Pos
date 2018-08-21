using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class MenuConsumableInfo
    {
        [DataMember]
        public long MenuId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public MenuType Type { get; set; }
        [DataMember]
        public bool IsPalmable { get; set; }
        [DataMember]
        public List<CourseInfo> SiteCourses { get; set; }
    }
}