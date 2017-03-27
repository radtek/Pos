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
                case WalletType.Jio:
                    return new JioWalletController();
                case WalletType.WeChat:
                    return new WeChatWalletController();
            }
            return null;
        }
    }
}