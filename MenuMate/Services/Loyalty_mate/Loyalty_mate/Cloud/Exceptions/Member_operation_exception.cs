using System;

namespace Loyalty_mate.Cloud.Exceptions.Exceptions {

/// <summary>
///   Thrown when a member operation fails.
/// </summary>
public class Member_operation_exception : Exception {
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
        Member_operation_exception(string message,
                                   System.Exception inner_exception = null)
            : base(message, inner_exception)
        {
        }
} /* Member_operation_exception */

} /* Loyalty_mate.Cloud */
