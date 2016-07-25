using SCE = Safemate.Core.Exceptions;

using System;

namespace Safemate.Core.Backup.Exceptions
{
    public class Base : SCE.Base
    {
        public Base(String message, Exception innerException)
        : base(message, innerException) { }
    };
};
