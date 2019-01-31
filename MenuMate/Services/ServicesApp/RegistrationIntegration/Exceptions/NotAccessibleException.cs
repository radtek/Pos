using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegistrationIntegration.Exceptions
{
    public class NotAccessibleException : Exception
    {
        public NotAccessibleException()
            : base("406 : Site Code is inactive/not found.")
        {

        }
    }
}
