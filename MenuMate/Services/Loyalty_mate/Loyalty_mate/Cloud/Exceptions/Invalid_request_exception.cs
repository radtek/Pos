using System;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a member operation fails.
/// </summary>
public class Invalid_request_exception : Exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="message">
        ///   A message describing the exception.
        /// </param>
        /// <param name="inner_exception">
        ///   The exception that caused the operation to fail.
        /// </param>
        public
        Invalid_request_exception()
            : base("Invalid request recieved at the cloud")
        {
        }
} /* Member_operation_exception */

} /* Loyalty_mate.Cloud */
