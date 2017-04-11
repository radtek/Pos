using WalletPayments.Model;
using WalletPaymets.Model;

namespace WalletPayments.Controller.Interface
{
    public interface IWalletController
    {
        WalletResponse Login(WalletAccountInfo inWalletAccountInfo);
        WalletResponse DoPurchaseTransaction(WalletTransactionInfo inWalletTransactionInfo);
        WalletResponse DoRefundTransaction(WalletTransactionInfo inWalletTransactionInfo);
        void DoTransactionEnquiry(QueryOrderInfo inQueryOrderInfo);
    }
}