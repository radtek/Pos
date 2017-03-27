using WalletPayments.Model;

namespace WalletPayments.Controller.Interface
{
    public interface IWalletController
    {
        void Login(LoginModel loginModel);
        void DoPurchaseTransaction(TransactionModel transactionModel);
        void DoRefundTransaction(TransactionModel transactionModel);
        void DoTransactionEnquiry(TransactionModel transactionModel);
    }
}