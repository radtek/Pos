using System;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.LoyaltyMate;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceLoyaltyMate" in both code and config file together.
    public class WCFServiceLoyaltyMate : IWCFServiceLoyaltyMate
    {

        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo)
        {
            return LoyaltyMember.Instance.SaveMember(inSyndicateCode, inInfo);
        }

        public LoyaltyResponse DeleteMember(string inSyndicateCode, string inUniqueId)
        {
            return LoyaltyMember.Instance.DeleteMember(inSyndicateCode, inUniqueId);
        }

        public LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, string inUniqueId)
        {
            return LoyaltyMember.Instance.GetMemberByUniqueCode(inSyndicateCode, inUniqueId);
        }

        public LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, string inMemberCode)
        {
            return LoyaltyMember.Instance.GetByCardCode(inSyndicateCode, inMemberCode);
        }

        public LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, string inMemberEmail)
        {
            return LoyaltyMember.Instance.GetByEmail(inSyndicateCode, inMemberEmail);
        }

        public LoyaltyMemberListResponse GetMemberList(string inSyndicateCode)
        {
            return LoyaltyMember.Instance.GetMemberList(inSyndicateCode);
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            return LoyaltyMember.Instance.PostTransaction(inSyndicateCode, transaction);
        }

        public LoyaltyTierResponse SaveTierLevel(string inSyndicateCode, TierLevelInfo inInfo)
        {
            return LoyaltyTier.Instance.SaveTierLevel(inSyndicateCode, inInfo);
        }

        public LoyaltyResponse DeleteTierLevel(string inSyndicateCode, int tierLevelId)
        {
            return LoyaltyTier.Instance.Delete(inSyndicateCode, tierLevelId);
        }

        public LoyaltyTierResponse GetTierLevel(string inSyndicateCode, int tierId)
        {
            return LoyaltyTier.Instance.GetTierLevel(inSyndicateCode, tierId);
        }

        public LoyaltyTierListResponse GetAllTierLevel(string inSyndicateCode)
        {
            return LoyaltyTier.Instance.GetAllTierLevel(inSyndicateCode);
        }

        public LoyaltyPointsInfoResponse GetPointsInRange(string inSyndicateCode, PointsInfo inInfo)
        {
            return LoyaltyMember.Instance.GetPointsInRange(inSyndicateCode, inInfo);
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            return LoyaltyMember.Instance.UpdateMemberCardCode(inSyndicateCode, uniqueId, memberCardCode);
        }

    }
}
