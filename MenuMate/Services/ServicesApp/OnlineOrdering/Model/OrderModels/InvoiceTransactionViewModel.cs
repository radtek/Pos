using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiInvoiceTransactionViewModel
    {
        public long InvoiceTransactionId { get; set; }
        public string InvoiceNumber { get; set; }
        public double TotalSaleAmount { get; set; }
        public DateTime TransactionDate { get; set; }
        public long SiteId { get; set; }
        public string TerminalName { get; set; }
        public byte[] Receipt { get; set; }
        public string ReceiptPath { get; set; }
        public double Rounding { get; set; }
        public long UserReferenceId { get; set; }
        public UserType UserType { get; set; }
    }
}
