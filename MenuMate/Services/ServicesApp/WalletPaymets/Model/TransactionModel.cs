namespace WalletPayments.Model
{
    public class TransactionModel
    {
        public string QrCode { get; set; }
        public double Amount { get; set; }
        public AccountInformation AccountInformation { get; set; }
    }
}