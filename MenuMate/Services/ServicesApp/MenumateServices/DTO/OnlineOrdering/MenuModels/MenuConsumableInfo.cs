using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.OnlineOrdering.MenuModels
{
    [DataContract]
    public class MenuConsumableInfo
    {
        [DataMember]
        public long MenuId { get; set; }
        [DataMember]
        public long SiteMenuCourseId { get; set; }
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