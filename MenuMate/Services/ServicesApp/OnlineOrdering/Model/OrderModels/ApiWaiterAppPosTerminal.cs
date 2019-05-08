using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiWaiterAppPosTerminalViewModel
    {
        public long SiteCode { get; set; }
        public string DeviceId { get; set; }
        public string Name { get; set; }
        public string MacAddress { get; set; }
    }
}
