using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class IsInaccessible : Base
    {
        ~IsInaccessible()
        {
        }

        public IsInaccessible(String message)
        : base(String.Format(
                   "CRM {0}'s data store is inaccessible",
                   message))
        {
        }

        public IsInaccessible(String message,
                              Exception innerException)
        : base(String.Format(
                   "CRM {0}'s data store is inaccessible",
                   message), innerException)
        {
        }
    };
};
