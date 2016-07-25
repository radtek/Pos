using System;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a member operation fails.
/// </summary>
public class Operation_timed_out_exception : Exception {
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
        Operation_timed_out_exception()
            : base("The operation has timed out")
        {
        }
} /* Member_operation_exception */

} /* Loyalty_mate.Cloud */
