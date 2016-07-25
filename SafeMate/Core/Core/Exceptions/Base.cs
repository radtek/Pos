using System;

namespace Safemate.Core.Exceptions
{
    public class Base : Exception
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
