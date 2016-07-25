using Loyaltymate.Model;
using System.Collections.Generic;

namespace Loyaltymate.Sevices
{
    public interface ILoyaltymateService
    {
        bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode, string uniqueId);
        MemberViewModel SaveMember(MemberViewModel member, string syndicateCode);
        MemberViewModel GetMemberByUniqueId(string uniqueId, string syndicateCode);
        MemberViewModel GetMemberByCardCode(string cardCode, string syndicateCode);
        MemberViewModel GetMemberByEmail(string inMemberEmail, string inSyndicateCode);
        IEnumerable<MemberViewModel> GetAllMember(string syndicateCode);
        bool DeleteMember(string uniqueId, string syndicateCode);
        TierLevelViewModel SaveTierLevel(TierLevelViewModel tierLevel, string syndicateCode);
        IEnumerable<TierLevelViewModel> GetAllTierLevels(string syndicateCode);
        TierLevelViewModel GetTierLevel(long tierId, string syndicateCode);
        bool DeleteTierLevel(long tierId, string syndicateCode);
        PointQuery GetPointsInRange(PointQuery pointQuery, string syndicateCode);
        bool UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode);
    }
}