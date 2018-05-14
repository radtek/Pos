using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PaymentSenseIntegration.Enums
{
    public enum TransactionType
    {
        LOGON = 1,
        REFUND, 
        REFUND_REVERSAL, 
        SALE, 
        SALE_REVERSAL
    }
}
