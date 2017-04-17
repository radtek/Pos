using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.Wallet_Payments;
using MenumateServices.Internal_Classes.WalletPayments;

namespace MenumateServices.WCFServices
{
    public class WCFServiceWalletPayments : IWCFServiceWalletPayments
    {
        public WalletActionResponse DoPurchaseTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            return WalletPaymentController.Instance.ProcessPurchaseTransaction(inWalletAccount, inWalletTransaction);
        }

        public WalletActionResponse DoRefundTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            return WalletPaymentController.Instance.ProcessRefundTransaction(inWalletAccount, inWalletTransaction);
        }
    }
}
