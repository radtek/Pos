using SCE = Safemate.Core.Exceptions;

using FBDS = FirebirdSql.Data.Services;

using System;

namespace Safemate.Core.Backup.Exceptions
{
    public class RequestUnsupported : Base
    {
        public RequestUnsupported(String message,
                                  Exception innerException)
        : base(message, innerException)
        {
        }
    }
};
