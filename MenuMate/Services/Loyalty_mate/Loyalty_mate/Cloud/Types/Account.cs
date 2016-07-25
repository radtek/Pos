using System;
namespace Loyalty_mate.Cloud {

[Serializable]
public sealed class Account {
        public
        Account()
        {
        }

        public
        Account(string email)
        {
                this.email = email;
        }

        public string email {
                get;
                set;
        }
} /* Account */

} /* Loyalty_mate.Cloud */
