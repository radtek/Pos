using System;
using System.Collections.Generic;
namespace Loyaltymate.Model
{
    public class MembershipProfileViewModel : DomainObjectBase
    {
        public long MemberId { get; set; }
        public long CompanyId { get; set; }
        public string MembershipNumber { get; set; }
        public DateTime? LastTransactionDate { get; set; }
        public DateTime? LastBirthdayBenefitDate { get; set; }
        public DateTime? YearStartDate { get; set; }
        public string CardCode { get; set; }
        public string CardNumber { get; set; }
        public long? HomeSiteCode { get; set; }
        public long? TierLevelId { get; set; }
        public int PointRule { get; set; }
        public double EarnedPoints { get; set; }
        public double LoadedPoints { get; set; }
        public double CurrentYearPoint { get; set; }
        public double PreviousYearPoint { get; set; }
        public double AvailableBirthDayPoint { get; set; }
        public double AvailableFirstVisitPoint { get; set; }
        public DateTime LastModified { get; set; }
        public bool IsFirstVisitRewarded { get; set; }
    }
}