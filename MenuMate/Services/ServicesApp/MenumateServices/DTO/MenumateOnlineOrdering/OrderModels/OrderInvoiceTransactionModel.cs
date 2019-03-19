using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
{
    [DataContract]
    public class OrderInvoiceTransactionModel
    {
        [DataMember]
        public long OrderInvoiceTransactionId { get; set; }
        [DataMember]
        public long OrderId { get; set; }
        [DataMember]
        public long InvoiceTransactionId { get; set; }
        [DataMember]
        public InvoiceTransactionModel InvoiceTransaction { get; set; }
    }
}
