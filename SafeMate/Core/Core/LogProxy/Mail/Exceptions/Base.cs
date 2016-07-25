using SCE = Safemate.Core.Exceptions;

using System;

namespace Safemate.Core.LogProxy.Mail.Exceptions
{
    public class Base : SCE.Base
    {
        ~Base()
        {
        }

        public Base(String message, Exception innerException)
        : base(message, innerException)
        {
        }
    };
};
