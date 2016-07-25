using System.Collections.Generic;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate.Interfaces
{

    public interface I_Loyalty_mate_service<Member_type>
    {
        Member_type Create_member(Member_type member);
        void Delete_member(string uuid);
        bool Does_member_exist(string uuid);
        void Update_member_details(Member_type member);

        Dictionary<string, Member_type> Get_all_member_details();
        Member_type Get_member_details(string uuid);
        Member_type Get_member_by_code(string memberCode,string siteId);


    } /* I_Loyalty_mate_service */

    public interface I_Loyalty_mate_service_transactions<TransactionType>
    {
        void Post_transaction(TransactionType transaction);
    } /* I_Loyalty_mate_service_transactions */

    public interface I_Loyalty_mate_service_activation_tokens
    {
        MemberActivationInfo Get_member_by_token(string activationToken);
    } /* I_Loyalty_mate_service_activation_tokens */

    public interface I_Loyalty_mate_service_tiers<TierType>
    {
        TierType Create_tier(TierType tier);
        void Delete_tier(TierType tier);
        void Update_tier_details(TierType tier);
        bool Does_tier_exist(string tierid);
        TierType Get_tier_details(string tierid);
    } /* I_Loyalty_mate_service_tiers */


    public interface I_Loyalty_mate_service_points<PointsDetail>
    {
        PointsDetail GetPointsInRange(PointsDetail pointsDetail); 
    }

} /* Loyalty_mate */
