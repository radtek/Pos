using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Exceptions
{
    public class LoyaltymateOperationException : Exception
    {
        public LoyaltymateOperationException(string message)
            : base(message)
        {
        }
    }
}
