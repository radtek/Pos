using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiOrderDevicesViewModel
    {
        public long OrderId { get; set; }
        public DeviceType DeviceType { get; set; }
        public string DeviceId { get; set; }
        public string DeviceName { get; set; }
        public string MacAddress { get; set; }
    }
}
