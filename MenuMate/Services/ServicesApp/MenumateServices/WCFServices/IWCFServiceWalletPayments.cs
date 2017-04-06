using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MenumateServices.DTO.Wallet_Payments;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceWalletPayments
    {
        [OperationContract]
        WalletActionResponse DoPurchaseTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction);

        [OperationContract]
        WalletActionResponse DoRefundTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction);
    }
}
