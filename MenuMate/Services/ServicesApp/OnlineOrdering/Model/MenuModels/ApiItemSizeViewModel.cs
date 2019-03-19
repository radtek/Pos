using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.MenuModels
{
    public class ApiItemSizeViewModel
    {
        public long ItemSizeId { get; set; }
        public long SiteItemId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public bool IsWeighted { get; set; }
        public double Price { get; set; }
        public bool IsFree { get; set; }
        public double PointsPercentage { get; set; }
        public double PointsPrice { get; set; }
        public bool CanBePaidUsingPoints { get; set; }
        public string OrderingUniqueId { get; set; }
        public string ThirdPartyId { get; set; }
        public int DefaultPatronCount { get; set; }
        public List<ApiItemSizeTaxProfileViewModel> ItemSizeTaxProfiles { get; set; }
    }
}
