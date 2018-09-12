using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.OnlineOrdering.OrderModels
{
    [DataContract]
    public class SiteOrderModel
    {
        [DataMember]
        public long CompanyId { get; set; }
        [DataMember]
        public long SiteId { get; set; }
        [DataMember]
        public long OrderId { get; set; }
        [DataMember]
        public string Location { get; set; }
        [DataMember]
        public double TotalAmount { get; set; }
        [DataMember]
        public int ContainerNumber { get; set; }
        [DataMember]
        public OrderContainerType ContainerType { get; set; }
        [DataMember]
        public string ContainerName { get; set; }
        [DataMember]
        public string OrderGuid { get; set; }
        [DataMember]
        public long UserReferenceId { get; set; }
        [DataMember]
        public UserType UserType { get; set; }
        [DataMember]
        public string TerminalName { get; set; }
        [DataMember]
        public DateTime TransactionDate { get; set; }
        [DataMember]
        public OrderType OrderType { get; set; }
        [DataMember]
        public List<OrderItemModel> OrderItems { get; set; }
        [DataMember]
        public SiteSettingType TransactionType { get; set; }
        [DataMember]
        public bool IsConfirmed { get; set; }
        [DataMember]
        public string UserEmailId { get; set; }
        [DataMember]
        public OrderInvoiceTransactionModel OrderInvoiceTransaction { get; set; }

    }
}


