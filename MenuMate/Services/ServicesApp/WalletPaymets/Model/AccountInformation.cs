namespace WalletPayments.Model
{
    public class AccountInformation
    {
        public int UserId { get; set; }
        public int ShopId { get; set; }
        public int CompanyId { get; set; }
        public int ExpiresIn { get; set; }
        public string RefreshToken { get; set; }
        public int IsRefunds { get; set; }
        public string SignKey { get; set; }
        public string Token { get; set; }
        public string UserName { get; set; }
        public string Platform { get; set; }
        public string ShopName { get; set; }
        public int AccountId { get; set; }
        public int MerchentId { get; set; }
        public string Password { get; set; }
        public string MerchantName { get; set; }
        public string CompanyName { get; set; }
    }
}