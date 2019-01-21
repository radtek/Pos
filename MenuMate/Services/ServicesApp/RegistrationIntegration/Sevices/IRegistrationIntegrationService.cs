using RegistrationIntegration.ViewModels;


namespace RegistrationIntegration.Sevices
{
    public interface IRegistrationIntegrationService
    {
        bool PostTerminalRegistrationInfo(string inSyndicateCode, ApiTerminalViewModel siteOrderViewModel);
    }
}