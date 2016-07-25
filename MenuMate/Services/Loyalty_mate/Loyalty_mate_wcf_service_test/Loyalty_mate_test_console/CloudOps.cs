using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Loyalty_mate_test_console.LoyaltyMateWCFServiceRef;

namespace Loyalty_mate_test_console
{
    public static class CloudOps
    {
        public static WCFServiceLoyaltyMateClient client;

        static CloudOps()
        {
            client = new WCFServiceLoyaltyMateClient();
        }

        private static void openConnection()
        {
            if (client.State == System.ServiceModel.CommunicationState.Closed || client.State == System.ServiceModel.CommunicationState.Faulted)
            {
                client.Open();
            }
        }

        private static void closeConnection()
        {
        }

        public static DTO_LoyaltyMemberResponse CreateMember(int siteId, string syndicateCode, DTO_MemberInfo info)
        {
            openConnection();
            return client.CreateMember(syndicateCode, siteId, info);
        }

        public static DTO_LoyaltyMemberResponse GetMember(int siteId, string syndicateCode, string id)
        {
            openConnection();
            return client.GetMember(syndicateCode, siteId, id);
        }

        public static DTO_LoyaltyResponse DeleteMember(int siteId, string syndicateCode, string id)
        {
            openConnection();
            return client.DeleteMember(syndicateCode, siteId, id);
        }

        public static DTO_LoyaltyMemberListResponse GetMemberList(int siteId, string syndicateCode)
        {
            openConnection();
            return client.GetMemberList(syndicateCode, siteId);
        }

        public static DTO_LoyaltyMemberResponse UpdateMember(int siteId, string syndicateCode, DTO_MemberInfo info , bool forceCreate)
        {
            openConnection();
            return client.UpdateMember(syndicateCode, siteId, info, forceCreate);
        }
    }
}
