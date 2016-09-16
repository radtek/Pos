namespace Loyaltymate.Model
{
    public class ApiTierLevelViewModel
    {
        public long CompanyId { get; set; }
        public long TierLevelId { get; set; }
        public string Name { get; set; }
        public int TierLevelNumber { get; set; }
        public string TierCode { get; set; }
        public double PointsRequired { get; set; }
        public double BirthdayBonus { get; set; }
        public double RedeemPointsRate { get; set; }
        public double WeightedRedeemPoints { get; set; }
        public double WeightedRedeemPointsValue { get; set; }
        public bool ChangeCard { get; set; }
        public bool SendMailToUser { get; set; }
        public bool SendMailToHeadQuarter { get; set; }
        public bool AllowEarntLoyaltyRedemption { get; set; }
    }
}