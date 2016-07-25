namespace Loyaltymate.Sevices
{
    public class RequestAddress
    {
        public const string BaseAddress = "http://loyaltymate.azurewebsites.net/";
        //Tier Level
        public const string GetAllTierLevel = "api/TierLevelApi/TierLevelByCompany";
        public const string GetTierLevel = "api/TierLevelApi/GetTierLevel";
        public const string SaveTierLevel = "api/TierLevelApi/Save";
        public const string DeleteTierLevel = "api/TierLevelApi/Delete";
        //Member
        public const string SaveMember = "api/MemberApi/Save";
        public const string GetAllMember = "api/MemberApi/MembersByCompany";
        public const string GetMemberByUniqueId = "api/MemberApi/MemberByUniqueId";
        public const string GetMemberByCardCode = "api/MemberApi/MemberByCode";
        public const string DeleteMember = "api/MemberApi/Member/Delete";
        public const string GetMemberByEmail = "api/MemberApi/GetMemberByEmail";
        public const string UpdateMemberCardCode = "api/MemberApi/UpdateMemberCardCode";
        //Points Transaction
        public const string PostTransaction = "api/MembershipTransaction/Save";
        public const string GetPointsInDateRange = "api/MembershipTransaction/GetPointsInDateRange";

        
    }
}
