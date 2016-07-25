using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.DTO.LoyaltyMate;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceLoyaltyMate" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceLoyaltyMate
    {
        // Member Operations
        [OperationContract]
        LoyaltyMemberResponse SaveMember(string inSyndicateCode,MemberInfo inInfo);

        [OperationContract]
        LoyaltyResponse DeleteMember(string inSyndicateCode,string inUniqueId);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, string inUniqueId);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, string inMemberCode);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, string inMemberEmail);

        [OperationContract]
        LoyaltyMemberListResponse GetMemberList(string inSyndicateCode);

        [OperationContract]
        LoyaltyResponse PostTransaction(string inSyndicateCode,TransactionInfo transaction);

        [OperationContract]
        LoyaltyTierResponse SaveTierLevel(string inSyndicateCode,TierLevelInfo inInfo);

        [OperationContract]
        LoyaltyResponse DeleteTierLevel(string inSyndicateCode,int tierLevelId);

        [OperationContract]
        LoyaltyTierResponse GetTierLevel(string inSyndicateCode,int tierId);
      
        [OperationContract]
        LoyaltyTierListResponse GetAllTierLevel(string inSyndicateCode);

        [OperationContract]
        LoyaltyPointsInfoResponse GetPointsInRange(string inSyndicateCode,PointsInfo inInfo);

        [OperationContract]
        LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId,string memberCardCode);
    }
}
