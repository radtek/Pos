namespace WalletPayments.Model
{
    public class WalletTransactionInfo
    {
        public string ScannedCode { get; set; }
        public double Amount { get; set; }
        public AccountInformation AccountInformation { get; set; }
    }
}