using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate.Cloud.Exceptions
{
    public class Invalid_tier_details_exception : Tier_operation_exception
    {
        public readonly Tier Tier;
        public Invalid_tier_details_exception(Tier tier)
            : base("Invalid tier details")
        {
            Tier = tier;
        }

        public Invalid_tier_details_exception(string message, Tier tier)
            : base(message)
        {
            Tier = tier;
        }
    }
}
