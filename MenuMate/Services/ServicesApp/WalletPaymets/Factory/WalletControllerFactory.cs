using WalletPayments.Controller.Impl;
using WalletPayments.Controller.Interface;
using WalletPayments.Enums;

namespace WalletPayments.Factory
{
    public class WalletControllerFactory
    {
        public IWalletController GetWalletController(WalletType inWalletType)
        {
            switch (inWalletType)
            {
                case WalletType.JioWallet:
                    return new JioWalletController();
                case WalletType.WeChatWallet:
                    return new WeChatWalletController();
            }
            return null;
        }
    }
}