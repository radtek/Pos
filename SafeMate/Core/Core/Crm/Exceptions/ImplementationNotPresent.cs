using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class ImplementationNotPresent : Base
    {
        ~ImplementationNotPresent()
        {
        }

        public ImplementationNotPresent(String message,
                                        Exception innerException)
        : base(String.Format(
                   "Unimplemented CRM interface \"{0}\" requested.",
                   message), innerException)
        {
        }
    };
};
