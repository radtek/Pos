using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Exceptions
{
    /// <summary>
    /// An exception to indicate an attempt to retrieve user guid with an activation code
    /// </summary>
    public class Activation_token_not_found_exception : Exception
    {
        public Activation_token_not_found_exception()
            : base("Activation token is not found in the system")
        {

        }
    }
}
