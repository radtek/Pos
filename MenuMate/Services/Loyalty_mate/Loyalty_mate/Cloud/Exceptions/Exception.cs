using System;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   The base exception type for Loyalty mate.
/// </summary>
public class Exception : System.Exception {
        public
        Exception(string message,
                  System.Exception inner_exception = null)
            : base(message, inner_exception)
        {
        }
} /* Exception */

} /* Loyalty_mate.Cloud */
