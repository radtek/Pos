using System;

namespace Loyalty_mate.Types {

/// <summary>
///   Represents basic user credentials.
/// </summary>
public sealed class Credentials {
        /// <summary>
        ///   Creates the credentials.
        /// </summary>
        /// <param name="username">
        ///   The username to use when logging in.
        /// </param>
        /// <param name="password">
        ///   The password to use when logging in.
        /// </param>
        /// <param name="client_id">
        ///   Identifies the user to the cloud; used for all
        ///   authentication operations.
        /// </param>
        /// <param name="client_secret">
        ///   Validates the client's ID; used for all authentication
        ///   operations.
        /// </param>
        /// <param name="site_name">
        ///   The site's name; used for forming request URIs.
        /// </param>
        public
        Credentials(string username,
                    string password,
                    string client_id,
                    string client_secret,
                    string site_name)
        {
                Username = username;
                Password = password;
                Client_ID = client_id;
                Client_secret = client_secret;
                Site_name = site_name;
        }
        
        /// <summary>
        /// Default constructor
        /// </summary>
        public
        Credentials() 
        {
            Username = "";
            Password = "";
            Client_ID = "";
            Client_secret = "";
            Site_name = "";
        }

        /// <summary>
        /// Verify whether every field has some value
        /// </summary>
        /// <returns></returns>
        public bool
        IsValid()
        {
            return !string.IsNullOrEmpty(this.Username)
                && !string.IsNullOrEmpty(this.Password)
                && !string.IsNullOrEmpty(this.Client_ID)
                && !string.IsNullOrEmpty(this.Client_secret)
                && !string.IsNullOrEmpty(this.Site_name);
        }

        public string Username { get; internal set; }
        public string Password { get; internal set; }
        public string Client_ID { get; internal set; }
        public string Client_secret { get; internal set; }
        public string Site_name { get; internal set; }
} /* Credentials */

} /* Loyalty_mate.Types */
