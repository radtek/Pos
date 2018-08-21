using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class CourseInfo
    {
        [DataMember]
        public long CourseId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public string ServingCourseName { get; set; }
        [DataMember]
        public string ServingCourseDescription { get; set; }
        [DataMember]
        public List<SiteItemInfo> Items { get; set; }
    }
}
