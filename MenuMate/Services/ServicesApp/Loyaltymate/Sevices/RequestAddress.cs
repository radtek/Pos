namespace Loyaltymate.Sevices
{
    public class RequestAddress
    {
      // public const string BaseAddress = "http://menumateloyalty.com/";
      public const string BaseAddress = "http://menumatepos.com/";//"http://lmonlineordering.azurewebsites.net/";//"http://menumatepos.com/";
//"http://menumateloyalty.com/";"http://lmonlineordering.azurewebsites.net/"

        //Member
        public const string SaveMember = "api/v3Pos/MemberApi/Save";
        public const string GetAllMember = "api/v3Pos/MemberApi/MembersByCompany";
        public const string GetMemberByUniqueId = "api/v3Pos/MemberApi/MemberByUniqueId";
        public const string GetMemberByCardCode = "api/v3Pos/MemberApi/MemberByCode";
        public const string GetMemberByEmail = "api/v3Pos/MemberApi/GetMemberByEmail";
        public const string UpdateMemberCardCode = "api/v3Pos/MemberApi/UpdateMemberCardCode";
        //Points Transaction
        public const string PostTransaction = "api/v3Pos/MembershipTransaction/Save";
        public const string PostInvoiceTransaction = "api/v3Pos/MembershipTransaction/InvoiceTransaction";
        //Company
        public const string GetCompanyDetail = "api/v3Pos/CompanyApi/Company";

        //Gift Voucher
        public const string GetGiftCardBalance = "api/v3Pos/VoucherApi/GiftCard";
        public const string GetGiftCardInfo = "api/v3Pos/VoucherApi/GiftCardInfo";
        //Pocket Voucher
        public const string GetPocketVoucher = "api/v3Pos/VoucherApi/PocketVoucher";

        //Voucher Processing
        public const string ProcessVoucherTransaction = "api/v3Pos/VoucherApi/ProcessVouchers";
        
        public const string ReleaseVouchers = "api/v3Pos/VoucherApi/ReleaseVouchers";

        public const string SyncSiteMenu = "api/v5/MenuConsumableApi/SyncSiteMenu";

        public const string SyncSiteTaxSettings = "api/v5/SiteApi/GetSiteTaxSetting"; 

        public const string UpdateOrderStatus = "api/v5/OrderApi/SaveOrderConfirmation";

        public const string PostOnlineOrderInvoiceInfo = "api/v5/OrderApi/SaveOrderBillingInformation";

        public const string IsOnlineOrderingEnableBySiteId = "api/v5/SiteApi/IsOnlineOrderingEnableBySiteId/";

        public const string UnsetOrderingDetails = "api/v5/SiteApi/SignalRDisconnectBySite/";
    }
}
