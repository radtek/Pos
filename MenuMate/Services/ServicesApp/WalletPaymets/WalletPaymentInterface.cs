using WalletPayments.Controller.Interface;
using WalletPayments.Enums;
using WalletPayments.Factory;
using WalletPayments.Model;
using WalletPaymets.Model;

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

        public WalletResponse Login(WalletAccountInfo inWalletAccountInfo)
        {
            return _walletController.Login(inWalletAccountInfo);
        }

        public WalletResponse DoPurchaseTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
            return _walletController.DoPurchaseTransaction(inWalletTransactionInfo);
        }

        public void DoRefundTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
            _walletController.DoRefundTransaction(inWalletTransactionInfo);
        }

        public void DoTransactionEnquiry(QueryOrderInfo inQueryOrderInfo)
        {
            _walletController.DoTransactionEnquiry(inQueryOrderInfo);
        }
    }
}