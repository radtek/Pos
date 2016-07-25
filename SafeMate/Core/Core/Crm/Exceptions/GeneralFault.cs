using SCC = Safemate.Core.Crm;

using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class GeneralFault : Base
    {
        public GeneralFault(SCC.Base faultingCrm,
                            Exception innerException)
        : base(String.Format(
                   "General failure reports by {0} CRM.",
                   faultingCrm.Type), innerException) { }
    }
};
