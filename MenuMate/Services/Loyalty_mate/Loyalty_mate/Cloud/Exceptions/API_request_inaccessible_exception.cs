using System;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a 404 is encountered when trying to call a Cloud method; This
///   usually represents a change in the API.
/// </summary>
public class API_request_inaccessible_exception : Exception {
        public
        API_request_inaccessible_exception(System.Exception inner_exception)
            : base("We hit an unexpected 404 during a cloud request.",
                   inner_exception)
        {
        }
} /* API_request_inaccessible_exception */

} /* Loyalty_mate.Cloud */
