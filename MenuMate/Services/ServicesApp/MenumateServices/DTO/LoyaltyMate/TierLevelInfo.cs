using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class TierLevelInfo
    {
        [DataMember]
        public int TierId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public int Level { get; set; }
        [DataMember]
        public double PointsRequired { get; set; }
        [DataMember]
        public double BirthdayBonus { get; set; }
        [DataMember]
        public double PricedRedemptionRate { get; set; }
        [DataMember]
        public double WeighedRedemptionPoints { get; set; }
        [DataMember]
        public double WeighedRedemptionWeight { get; set; }
        [DataMember]
        public bool SendMailToHeadOffice { get; set; }
        [DataMember]
        public bool SendMailToMember { get; set; }
        [DataMember]
        public bool ChangeCard { get; set; }
        [DataMember]
        public bool AllowEarntLoyaltyRedemption { get; set; }
    }
}
