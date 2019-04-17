using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class OnlineOrderAttributes
    {
        public long OnlineOrderId { get; set; }
        public long ProfileId { get; set; }
        public byte[] EftposReceipt { get; set; }
        public long InvoiceNumber { get; set; }
        public AppType AppType { get; set; }
        public bool IsPosted { get; set; }
        public string TerminalName { get; set; }

    }
}
