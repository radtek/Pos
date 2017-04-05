using WalletPayments.Controller.Interface;
using WalletPayments.Model;
using WalletPaymets.Model;

namespace WalletPayments.Controller.Impl
{
    public class JioWalletController : IWalletController
    {
        public WalletResponse Login(WalletAccountInfo inWalletAccountInfo)
        {
            return new WalletResponse();
        }

        public WalletResponse DoPurchaseTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
            return new WalletResponse(); 
        }

        public void DoRefundTransaction(WalletTransactionInfo inWalletTransactionInfo)
        {
           // throw new System.NotImplementedException();
        }

        public void DoTransactionEnquiry(QueryOrderInfo inQueryOrderInfo)
        {
            //throw new System.NotImplementedException();
        }
    }
}