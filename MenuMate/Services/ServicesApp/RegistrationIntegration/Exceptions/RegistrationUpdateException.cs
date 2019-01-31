using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Exceptions
{
    public class RegistrationUpdateException : Exception
    {
        public RegistrationUpdateException(string message)
            : base(message)
        {
        }
    }
}
