using SCC = Safemate.Core.Crm;

using System;

namespace Safemate.Core.Crm.Exceptions
{
    public class MissingAttribute : Base
    {
        ~MissingAttribute()
        {
        }

        public MissingAttribute(SCC.Base faultingCrm)
        : base(String.Format(
                   "CRM {0} failed to access a required attribute.",
                   faultingCrm.Type)) { }

        public MissingAttribute(SCC.Base faultingCrm,
                                Exception innerException)
        : base(String.Format(
                   "CRM {0} failed to access a required attribute.",
                   faultingCrm.Type), innerException) { }
    };
};
