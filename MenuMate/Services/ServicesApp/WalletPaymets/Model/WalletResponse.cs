using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WalletPaymets.Model
{
    public class WalletResponse
    {
        public bool ResponseSuccessful { get; set; }
        public string ResponseMessage { get; set; }
        public string RefundTransactionId { get; set; }
        public string OrderNo { get; set; }
        public string RefundFee { get; set; }
        public string ApplyTime { get; set; }
        public string OutRefundNo { get; set; }
        public string RefundStatus { get; set; }
        public int ExpiresIn { get; set; }
        public string SecurityToken { get; set; }
        public string SignKey { get; set; }
        public string OrderId { get; set; }
    }
}
