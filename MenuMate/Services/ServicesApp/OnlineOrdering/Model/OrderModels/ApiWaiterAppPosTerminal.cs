using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiWaiterAppPosTerminalViewModel
    {
        public long SiteCode { get; set; }
        public string DeviceId { get; set; }
        public string Name { get; set; }
        public string MacAddress { get; set; }
        public DeviceType DeviceType { get; set; }
    }
}
