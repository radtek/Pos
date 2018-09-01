using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyaltymate.Enum;

namespace Loyaltymate.Model.OnlineOrderingModel.OrderModels
{
    public class ApiSiteOrderViewModel
    {
        public long CompanyId { get; set; }
        public long SiteId { get; set; }
        public long OrderId { get; set; }
        public string Location { get; set; }
        public double TotalAmount { get; set; }
        public int ContainerNumber { get; set; }
        public OrderContainerType ContainerType { get; set; }
        public string ContainerName { get; set; }
        public string OrderGuid { get; set; }
        public long UserReferenceId { get; set; }
        public UserType UserType { get; set; }
        public string TerminalName { get; set; }
        public DateTime TransactionDate { get; set; }
        public OrderType OrderType { get; set; }
        public List<ApiOrderItemViewModel> OrderItems { get; set; }
        public SiteSettingType TransactionType { get; set; }
        public bool IsConfirmed { get; set; }
    }
}


