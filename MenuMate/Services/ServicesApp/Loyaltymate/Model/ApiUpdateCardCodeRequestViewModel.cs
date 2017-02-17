using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model
{
    public class ApiUpdateCardCodeRequestViewModel
    {
        public string UniqueId { get; set; }
        public string MemberCardCode { get; set; }
    }
}
