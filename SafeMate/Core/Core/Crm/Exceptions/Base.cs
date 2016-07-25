using SCE = Safemate.Core.Exceptions;

using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class Base : SCE.Base
    {
        ~Base()
        {
        }

        public Base()
        : base()
        {
        }

        public Base(String message)
        : base(message)
        {
        }

        public Base(String message, Exception innerException)
        : base(message, innerException)
        {
        }
    };
};
