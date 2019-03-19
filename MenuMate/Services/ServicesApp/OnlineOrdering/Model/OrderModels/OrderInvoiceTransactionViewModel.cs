using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiOrderInvoiceTransactionViewModel
    {
        public long OrderInvoiceTransactionId { get; set; }
        public long OrderId { get; set; }
        public long InvoiceTransactionId { get; set; }
        public ApiInvoiceTransactionViewModel InvoiceTransaction { get; set; }
    }
}
