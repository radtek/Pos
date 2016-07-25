using SCE = Safemate.Core.Exceptions;

using System;

namespace Safemate.Core.Backup.Exceptions
{
    public class BackupFailure : Base
    {
        public BackupFailure(String message,
                             Exception innerException)
        : base(message, innerException) { }
    };
};
