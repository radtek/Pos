using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel
{
    public class ApiSideGroupViewModel
    {
        public long SideGroupId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public int MaxSelect { get; set; }
        public bool AllowSkip { get; set; }
        public List<ApiItemSideViewModel> ItemSides { get; set; }
    }
}
