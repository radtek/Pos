using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Enum
{
    public enum OrderStatus
    {
        IsNotValidated = 0,
        IsValidated,
        PaymentReceived,
        IsConfirmed,
        InvalidTable,
        ItemNotFound,
        PosOrderError,
        PosPaymentError
    }
}
