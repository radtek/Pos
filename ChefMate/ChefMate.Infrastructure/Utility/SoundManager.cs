using Chefmate.Core.Enums;
using Chefmate.Infrastructure.Controller;
using Chefmate.SoundPlayer;

namespace Chefmate.Infrastructure.Utility
{

    public class SoundManager
    {
        public static bool PlaySound(SoundEvents soundEvent)
        {
            string soundName = string.Empty;
            switch (soundEvent)
            {
                case SoundEvents.NewOrderFromMm:
                    soundName = ChefmateController.Instance.CurrentSettings.NewOrderMmSound;
                    break;
                case SoundEvents.CancelOrderFromMm:
                    soundName = ChefmateController.Instance.CurrentSettings.CancelNoticeMmSound;
                    break;
                case SoundEvents.RefundOrderFromMm:
                    soundName = ChefmateController.Instance.CurrentSettings.RefundNoticeMmSound;
                    break;
                case SoundEvents.TransferOrderFromMm:
                    soundName = ChefmateController.Instance.CurrentSettings.TransferNoticeMmSound;
                    break;
                case SoundEvents.BumpedOrderArrivedFromCm:
                    soundName = ChefmateController.Instance.CurrentSettings.OrderArrivedCmSound;
                    break;
                case SoundEvents.TransferOrderArrivedFromCm:
                    soundName = ChefmateController.Instance.CurrentSettings.TransferOrderCmSound;
                    break;

                default:
                    break;
            }

            SoundPlayerUtilities.Instance.PlaySound(soundName);
            return true;
        }

    }
}
