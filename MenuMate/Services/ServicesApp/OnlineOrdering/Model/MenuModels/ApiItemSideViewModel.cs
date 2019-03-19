using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.MenuModels
{
    public class ApiItemSideViewModel
    {
        public long SiteItemId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string ItemUniqueId { get; set; }
        public bool OnlyAsSide { get; set; }
        public List<ApiItemSizeViewModel> ItemSizes { get; set; }
    }
}
