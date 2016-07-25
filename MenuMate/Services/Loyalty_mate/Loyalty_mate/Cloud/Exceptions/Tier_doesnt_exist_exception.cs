using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyalty_mate.Cloud.Exceptions.Exceptions;

namespace Loyalty_mate.Cloud.Exceptions
{
    public class Tier_doesnt_exist_exception : Member_operation_exception
    {
        public readonly string TierId;
        public Tier_doesnt_exist_exception(string tierid)
            : base("The tier with this id doesn't exist for this group.")
        {
            TierId = tierid;
        }


    }
}
