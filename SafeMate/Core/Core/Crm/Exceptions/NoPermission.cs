using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class NoPermission : Base
    {
        public NoPermission(String message)
        : base(message) { }
    }
};
