namespace WalletPayments.Model
{
    public class WalletTransactionInfo
    {
        public string ScannedCode { get; set; }
        public double Amount { get; set; }
        public string OrderRefernce { get; set; }
        public double RefundFee { get; set; }
        public AccountInformation AccountInformation { get; set; }
    }
}