using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;
using OnlineOrdering.Model.OrderModels;

namespace OnlineOrdering.Model.NotificationModels
{
    public class ApiZedRequestNotificationViewModel
    {
        public ApiWaiterAppPosTerminalViewModel ApiWaiterAppPosTerminalViewModel { get; set; }
        public ZedRequestStatus ZedRequestStatus { get; set; }
        public string Message { get; set; }
        public string ProcessingTerminal { get; set; }
    }
}
