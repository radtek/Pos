using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud.Types
{
    /// <summary>
    /// A simple class to hold the information, when used to retrieve members uuid with an activation token
    /// </summary>
    public class MemberActivationInfo
    {
        /// <summary>
        /// owner uuid
        /// </summary>
        public string uuid { get; set; }

        /// <summary>
        /// activation token string
        /// </summary>
        public string value { get; set; }

        /// <summary>
        /// created date
        /// </summary>
        public DateTime created_at { get; set; }

        /// <summary>
        /// whether the token is used or not
        /// </summary>
        public bool expired { get; set; }
    }
}
