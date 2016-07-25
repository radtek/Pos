namespace Loyalty_mate.Cloud.Constants
{

    public static class Request_addresses
    {
        public const string Host = "staging.loyalty-mate.com";

        public static class Authentication
        {
            public const string Create_token = "oauth/token";
            public const string Refresh_token = "oauth/token";
        } /* Authentication */

        public static class Members
        {
            public const string Create_member = "pos/api/members";
            public const string Delete_member = "pos/api/members";
            public const string Get_all_member_details = "pos/api/members";
            public const string Get_member_details = "pos/api/members";
            public const string Update_member_details = "pos/api/members";
        } /* Members */

        public static class Transactions
        {
            public const string Post_transaction = "pos/api/transactions";
            public const string Get_PointsInRange = "pos/api/transactions";
        } /* Transactions */

        public static class ActivationTokens
        {
            public const string Get_member_id_by_token = "pos/api/activation_tokens";
        } /* Activation tokens */
        public static class Tiers
        {
            public const string Create_tier = "pos/api/tiers";
            public const string Delete_tier = "pos/api/tiers";
            public const string Get_tier_details = "pos/api/tiers";
            public const string Update_tier_details = "pos/api/tiers";
        }

    } /* Request_addresses */

} /* Loyalty_mate.Cloud */
