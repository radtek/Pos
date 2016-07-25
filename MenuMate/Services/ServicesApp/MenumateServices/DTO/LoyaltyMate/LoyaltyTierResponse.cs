using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyTierResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        TierLevelInfo _tierInfo;

        internal LoyaltyTierResponse(
                        bool inSuccesful,
                        string inMessage,
                        string inDescription,
                        LoyaltyResponseCode inResponseCode,
                        TierLevelInfo inTierInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _tierInfo = CreateTierInfo(inTierInfo);
        }

        #region Public

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }


        [DataMember]
        public TierLevelInfo TierInfo
        {
            get { return _tierInfo; }
            set { _tierInfo = value; }
        }

        public static LoyaltyTierResponse CreateResponseNoError(TierLevelInfo inTierInfo)
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful, inTierInfo);
        }


        public static LoyaltyTierResponse CreateResponseError(
                                       string inMessage,
                                       string inDescription,
                                       LoyaltyResponseCode inResponseCode,
                                       TierLevelInfo inTierInfo)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode, inTierInfo);
        }


        public static LoyaltyTierResponse CreateResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                TierLevelInfo inTierInfo)
        {
            return new LoyaltyTierResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode,
                                inTierInfo);
        }
        #endregion

        #region Private

        TierLevelInfo CreateTierInfo(TierLevelInfo tierInfo)
        {
            var result = new TierLevelInfo();
            result.TierId = tierInfo.TierId;
            result.Name = tierInfo.Name;
            result.Level = tierInfo.Level;
            result.BirthdayBonus = tierInfo.BirthdayBonus;
            result.PointsRequired = tierInfo.PointsRequired;
            result.PricedRedemptionRate = tierInfo.PricedRedemptionRate;
            result.WeighedRedemptionPoints = tierInfo.WeighedRedemptionPoints;
            result.WeighedRedemptionWeight = tierInfo.WeighedRedemptionWeight;
            result.SendMailToHeadOffice = tierInfo.SendMailToHeadOffice;
            result.SendMailToMember = tierInfo.SendMailToMember;
            result.ChangeCard = tierInfo.ChangeCard;
            return result;
        }
        #endregion
    }
}
