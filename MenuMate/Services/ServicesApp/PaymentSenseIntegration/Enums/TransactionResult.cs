using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PaymentSenseIntegration.Enums
{
    public enum TransactionResult
    {
        SUCCESSFUL = 1,
        DECLINED,
        VOID, 
        UNSUCCESSFUL, 
        CANCELLED, 
        TIMED_OUT
    }
}
