using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class PointsDetail : DomainObjectBase
    {
        public MembershipProfileViewModel MembershipProfile { get; set; }
        public PointType PointType { get; set; }
        public double Balance { get; set; }
        public long MembershipProfileId { get; set; }
    }
}