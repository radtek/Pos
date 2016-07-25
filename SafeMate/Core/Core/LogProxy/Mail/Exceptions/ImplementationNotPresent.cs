using System;

namespace Safemate.Core.LogProxy.Mail.Exceptions
{
    public class ImplementationNotPresent : Base
    {
        ~ImplementationNotPresent()
        {
        }

        public ImplementationNotPresent(String message,
                                        Exception innerException)
        : base(String.Format(
                   "Unimplemented Mail interface \"{0}\" requested.",
                   message), innerException)
        {
        }
    };
};
