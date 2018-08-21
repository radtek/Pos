using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model
{
    public class ApiItemSideViewModel
    {
        public long SiteItemId { get; set; }
        public long SideGroupId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string ItemUniqueId { get; set; }
        public bool OnlyAsSide { get; set; }
        public long CompanyId { get; set; }
        public List<ApiItemSizeViewModel> ItemSizes { get; set; }
    }
}
