using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate.Cloud.Exceptions
{
    public class Tier_already_exists_exception : Tier_operation_exception
    {
        public readonly Tier Tier;
        public
       Tier_already_exists_exception(Tier tier)
            : base("The tier already exists for this group in the cloud.")
        {
            Tier = tier;
        }


    }
}
