using System;

namespace Loyalty_mate.Cloud {

/// <summary>
///   A unique token used to authenticate us to the cloud.
/// </summary>
public sealed class Token {
        /// <summary>
        ///   Creates a token.
        /// </summary>
        /// <param name="access_token">
        ///   A string to be used for accessing the API.
        /// </param>
        /// <param name="refresh_token">
        ///   A unique ID identifying the authenticated session.
        /// </param>
        /// <param name="expiry_date">
        ///   When the token expires.
        /// </param>
        public
        Token(string access_token,
              string refresh_token,
              Nullable<DateTime> expiry_date)
        {
                Access_token = access_token;
                Refresh_token = refresh_token;
                Expiry_date = expiry_date;
        }

        /// <summary>
        ///   Checks whether the token needs to be refreshed.
        /// </summary>
        /// <returns>
        ///   True if the token must be refreshed.
        /// </returns>
        public bool
        Has_expired()
        {
                return Expiry_date.HasValue
                       ? DateTime.Now >= Expiry_date
                       : true;
        }

        public readonly string Access_token;
        public readonly string Refresh_token;
        public Nullable<DateTime> Expiry_date;
} /* Token */

} /* Loyalty_mate.Cloud */
