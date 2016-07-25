using SCE = Safemate.Core.Exceptions;

using FBDS = FirebirdSql.Data.Services;

using System;

namespace Safemate.Core.Backup.Exceptions
{
    public class ValidationRestorationFailure : Base
    {
        public ValidationRestorationFailure(String message,
                                            Exception innerException)
        : base(message, innerException) { }
    }
};
