using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class AccountingCategory
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Code { get; set; }
        [DataMember]
        public string ExternalCode { get; set; }
        [DataMember]
        public string LedgerAccountCode { get; set; }
        [DataMember]
        public string PostingAccountCode { get; set; }
        [DataMember]
        public string CostCenterCode { get; set; }
        [DataMember]
        public string Classification { get; set; }
    }
}
