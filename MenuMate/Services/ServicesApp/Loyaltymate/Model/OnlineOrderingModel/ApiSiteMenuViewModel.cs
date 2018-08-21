using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model
{
    public class ApiSiteMenuViewModel
    {
        public long CompanyId { get; set; }
        public long SiteId { get; set; }
        public List<ApiMenuConsumableViewModel> MenuConsumables { get; set; }
    }
}
