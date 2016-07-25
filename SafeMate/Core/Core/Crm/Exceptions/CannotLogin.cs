using SCC = Safemate.Core.Crm;

using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class CannotLogin : Base
    {
        public CannotLogin(String message)
        : base(message) { }

        public CannotLogin(SCC.Base faultingCrm)
        : base(String.Format("Login rejected by {0} CRM",
                             faultingCrm.Type)) { }

        public CannotLogin(SCC.Base faultingCrm, Exception innerException)
        : base(String.Format("Login rejected by {0} CRM",
                             faultingCrm.Type), innerException) { }
    }
};
