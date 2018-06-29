namespace Loyaltymate.Sevices
{
    public class RequestAddress
    {
        public const string BaseAddress = "http://menumatepos.com/";

        //Member
        public const string SaveMember = "api/MemberApi/Save";
        public const string GetAllMember = "api/MemberApi/MembersByCompany";
        public const string GetMemberByUniqueId = "api/MemberApi/MemberByUniqueId";
        public const string GetMemberByCardCode = "api/MemberApi/MemberByCode";
        public const string GetMemberByEmail = "api/MemberApi/GetMemberByEmail";
        public const string UpdateMemberCardCode = "api/v2Pos/MemberApi/UpdateMemberCardCode";
        //Points Transaction
        public const string PostTransaction = "api/MembershipTransaction/Save";
        public const string PostInvoiceTransaction = "api/MembershipTransaction/InvoiceTransaction";
        //Company
        public const string GetCompanyDetail = "api/CompanyApi/Company";

        //Gift Voucher
        public const string GetGiftCardBalance = "api/VoucherApi/GiftCard";
        public const string GetGiftCardInfo = "api/v2Pos/VoucherApi/GiftCardInfo";
        //Pocket Voucher
        public const string GetPocketVoucher = "api/VoucherApi/PocketVoucher";

        //Voucher Processing
        public const string ProcessVoucherTransaction = "api/v2Pos/VoucherApi/ProcessVouchers";
        
        public const string ReleaseVouchers = "api/VoucherApi/ReleaseVouchers";

    }
}
