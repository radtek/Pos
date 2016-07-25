using System;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a request is made using an expired or otherwise invalid
///   access token.
/// </summary>
public class Invalid_access_token_exception : Exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="token">
        ///   The token that the cloud deemed unworthy.
        /// </param>
        public
        Invalid_access_token_exception(Token token)
            : base("The access token used is invalid or has expired.")
        {
        }

        public readonly Token Token;
} /* Invalid_access_token_exception */

} /* Loyalty_mate.Cloud */
