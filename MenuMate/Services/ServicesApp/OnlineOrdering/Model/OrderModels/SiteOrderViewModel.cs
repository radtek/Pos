using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiSiteOrderViewModel
    {
        public long CompanyId { get; set; }
        public long SiteId { get; set; } //save in db
        public long OrderId { get; set; } //save in db
        public string Location { get; set; }
        public double TotalAmount { get; set; }
        public double TotalTax { get; set; }
        public string ContainerNumber { get; set; }
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
        public OrderStatus OrderStatus { get; set; }
        public string UserEmailId { get; set; }
        public Guid? MemberGuid { get; set; }
        public ApiOrderInvoiceTransactionViewModel OrderInvoiceTransaction { get; set; }
        public bool IsHappyHourApplied { get; set; }
        public List<OrderPayment> OrderPayments { get; set; }
        public PaymentType PaymentType { get; set; }
        public long StaffReferenceId { get; set; }
    }
}
