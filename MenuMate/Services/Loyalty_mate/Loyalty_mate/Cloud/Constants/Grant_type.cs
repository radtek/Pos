namespace Loyalty_mate.Cloud.Constants {

/// <summary>
///   Constants representing the methods in which we may
///   authenticate with the cloud.
/// </summary>
public static class Grant_type {
        /// <summary>
        ///   Specifies that we want to create a new authentication token.
        ///   I.e. begin a new session for interaction with the cloud.
        /// </summary>
        public const string New_token = "password";
        /// <summary>
        ///   Specifies that we want to refresh our current credentials; I.e.
        ///   signal to the cloud that our session should be considered active.
        /// </summary>
        public const string Refresh_existing_token = "refresh_token";
} /* Grant_type */

} /* Loyalty_mate.Cloud */
