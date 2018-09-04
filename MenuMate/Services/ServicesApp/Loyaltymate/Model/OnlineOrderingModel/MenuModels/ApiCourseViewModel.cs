﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel
{
    public class ApiCourseViewModel
    {
        public long CourseId { get; set; }
        public long SiteMenuCourseId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string ServingCourseName { get; set; }
        public string ServingCourseDescription { get; set; }
        public List<ApiSiteItemViewModel> Items { get; set; }
    }
}