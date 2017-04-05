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
        public WCFServiceWalletPayments()
        {

        }

        public WalletActionResponse Login(WalletAccount inWalletAccount)
        {
            return new WalletActionResponse();
        }

        public WalletActionResponse DoTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            return WalletPaymentController.Instance.ProcessTransaction(inWalletAccount, inWalletTransaction);
        }
    }
}
