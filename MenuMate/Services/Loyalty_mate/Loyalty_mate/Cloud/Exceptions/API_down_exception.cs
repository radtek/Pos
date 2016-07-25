namespace Loyalty_mate.Cloud.Exceptions {

using System;

/// <summary>
///   Thrown when the Cloud signals that the API is down.
/// </summary>
public class API_down_exception : Exception {
        public
        API_down_exception()
            : base("The Loyalty mate API isn't active.")
        {
        }
} /* API_down_exception */

} /* Loyalty_mate.Cloud */
