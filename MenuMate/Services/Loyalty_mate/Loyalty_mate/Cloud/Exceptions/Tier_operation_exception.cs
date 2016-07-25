using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Exceptions
{
    public class Tier_operation_exception : Exception
    {
        public
        Tier_operation_exception(string message,System.Exception inner_exception = null)
            : base(message, inner_exception)
        {
        }
    }
}
