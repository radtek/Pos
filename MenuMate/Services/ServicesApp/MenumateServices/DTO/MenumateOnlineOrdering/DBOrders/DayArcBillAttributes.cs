using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class DayArcBillAttributes
    {
        public long ArcBillId { get; set; }
        public string TerminalName { get; set; }
        public string StaffName { get; set; }
        public double Total { get; set; }
        public double Discount { get; set; }
        public int PatronCount { get; set; }
        public long SecurityRef { get; set; }
        public string BilledLocation { get; set; }
        public long InvoiceNumber { get; set; }
        public int SaleType { get; set; }
        public double RoundingAdjustment { get; set; }
        public bool IsPrintRequired { get; set; }
        public AppType ApplicationType { get; set; }
        public long OnlinOrderId { get; set; }
        public string OrderGuid { get; set; }
        public long SiteId { get; set; }
    }
}
