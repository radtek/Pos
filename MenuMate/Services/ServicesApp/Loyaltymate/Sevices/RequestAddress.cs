namespace Loyaltymate.Sevices
{
    public class RequestAddress
    {
        //public const string BaseAddress = "http://menumateloyalty.com/";
        //public const string BaseAddress = "http://menumatepos.com/";
//"http://menumateloyalty.com/";"http://lmonlineordering.azurewebsites.net/"
     

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

        public const string SyncSiteMenu = "api/v5/MenuConsumableApi/SyncSiteMenu";

        public const string SyncSiteTaxSettings = "api/v5/SiteApi/GetSiteTaxSetting"; 

        public const string UpdateOrderStatus = "api/v5/OrderApi/SaveOrderConfirmation";

        public const string PostOnlineOrderInvoiceInfo = "api/v5/OrderApi/SaveOrderBillingInformation";

        public const string IsOnlineOrderingEnableBySiteId = "api/v5/SiteApi/IsOnlineOrderingEnableBySiteId/";

        public const string UnsetOrderingDetails = "api/v5/SiteApi/SignalRDisconnectBySite/";
    }
}
