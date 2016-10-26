using System;
using System.Collections.Generic;

namespace Loyaltymate.Model
{
    public class ApiMembershipProfileViewModel
    {
        public long MembershipProfileId { get; set; }
        public string MembershipNumber { get; set; }
        public DateTime? LastTransactionDate { get; set; }
        public DateTime? LastBirthdayBenefitDate { get; set; }
        public DateTime YearStartDate { get; set; }
        public string CardCode { get; set; }
        public string CardNumber { get; set; }
        public long MemberId { get; set; }
        public long CompanyId { get; set; }
        public string CompanyName { get; set; }
        public long HomeSiteCode { get; set; }
        public string HomeSiteName { get; set; }
        public long? HomeSiteId { get; set; }
        public long? TierLevelId { get; set; }
        public string TierLevelName { get; set; }
        public int PointRule { get; set; }
        public double EarnedPoints { get; set; }
        public double LoadedPoints { get; set; }
        public double CurrentYearPoint { get; set; }
        public double PreviousYearPoint { get; set; }
        public double AvailableBirthDayPoint { get; set; }
        public double AvailableFirstVisitPoint { get; set; }
        public double TotalFirstVisit { get; set; }
        public double TotalBirthday { get; set; }
        public double TotalEarned { get; set; }
        public double TotalRedeemed { get; set; }
        public double TotalPurchased { get; set; }
        public double PointBalance { get; set; }
        public bool IsFirstVisitRewarded { get; set; }
        public DateTime LastModified { get; set; }
        public IList<ApiVoucherViewModel> Vouchers { get; set; }
    }
}