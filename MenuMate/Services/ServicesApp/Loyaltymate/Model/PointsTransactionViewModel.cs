using System;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class PointsTransactionViewModel : DomainObjectBase
    {
        public long MembershipProfileId { get; set; }
        public DateTime TransactionDate { get; set; }
        public PointType PointType { get; set; }
        public double PointsDelta { get; set; }
        public long SiteCode { get; set; }
        public string UniqueId { get; set; }
    }
}