using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model
{
    public class OnlineOrderingDetailsResponse
    {
        public bool IsLoyaltyMateOrderingEnabled { get; set; }
        public bool IsWaiterOrderingEnabled { get; set; }
        public bool IsSuccessful { get; set; }
        public string Message { get; set; }
    }
}
