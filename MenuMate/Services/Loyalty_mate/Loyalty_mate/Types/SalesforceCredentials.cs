using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Types
{
    /// <summary>
    /// Basic credential details needed to setup a connection with salesforce
    /// </summary>
    public class SalesforceCredentials
    {
        public string UserName { get; set; }
        public string Password { get; set; }
        public string SecurityToken { get; set; }

        public SalesforceCredentials()
        {
            this.UserName = "";
            this.Password = "";
            this.SecurityToken = "";
        }

        /// <summary>
        /// Verify whether every field has some value
        /// </summary>
        /// <returns></returns>
        public bool IsValid()
        {
            return !string.IsNullOrEmpty(this.UserName)
                    && !string.IsNullOrEmpty(this.Password)
                    && !string.IsNullOrEmpty(this.SecurityToken);
        }
    }
}
