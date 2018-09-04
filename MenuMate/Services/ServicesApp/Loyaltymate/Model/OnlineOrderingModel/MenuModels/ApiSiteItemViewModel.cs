﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel
{
    public class ApiSiteItemViewModel
    {
        public long SiteItemId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string ItemUniqueId { get; set; }
        public bool OnlyAsSide { get; set; }
        public long CompanyId { get; set; }
        public long SiteMenuCourseId { get; set; }
        public string ImagePath { get; set; }
        public List<ApiItemSizeViewModel> ItemSizes { get; set; }
        public List<ApiSideGroupViewModel> SideGroups { get; set; }
    }
}
