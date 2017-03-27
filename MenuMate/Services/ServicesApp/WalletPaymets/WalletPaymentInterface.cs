using WalletPayments.Controller.Interface;
using WalletPayments.Enums;
using WalletPayments.Factory;
using WalletPayments.Model;

namespace WalletPayments
{
    public class WalletPaymentInterface
    {
        private WalletType _walletType;

        private readonly IWalletController _walletController;
        public WalletPaymentInterface(WalletType inWalletType)
        {
            _walletType = inWalletType;
            _walletController = new WalletControllerFactory().GetWalletController(inWalletType);
        }
        public void Login()
        {
            _walletController.Login(new LoginModel());
        }
        public void DoPurchaseTransaction()
        {
            _walletController.DoPurchaseTransaction(new TransactionModel());
        }
        public void DoRefundTransaction()
        {
            _walletController.DoRefundTransaction(new TransactionModel());
        }
        public void DoTransactionEnquiry()
        {
            _walletController.DoTransactionEnquiry(new TransactionModel());
        }
    }
}