using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Exceptions
{
    public class LoyaltymateOperationException : Exception
    {
        public LoyaltymateOperationException(string message)
            : base(message)
        {
        }
    }
}
