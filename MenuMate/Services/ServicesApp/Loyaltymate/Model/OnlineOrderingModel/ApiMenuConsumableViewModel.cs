using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace Loyaltymate.Model.OnlineOrderingModel
{
    public class ApiMenuConsumableViewModel
    {
        public long MenuId { get; set; }
        public long SiteMenuCourseId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public MenuType Type { get; set; }
        public bool IsPalmable { get; set; }
        public List<ApiCourseViewModel> SiteCourses { get; set; }
    }
}