using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiSiteOrderPaymentViewModel
    {
        public long Id { get; set; }
        public long OrderId { get; set; }
        public string PaymentName { get; set; }
        public double Amount { get; set; }
        public string Note { get; set; }
        public double Tip { get; set; }
        public byte[] PaymentReceipt { get; set; }
        public string ReceiptPath { get; set; }
        public string CardType { get; set; }
    }
}
