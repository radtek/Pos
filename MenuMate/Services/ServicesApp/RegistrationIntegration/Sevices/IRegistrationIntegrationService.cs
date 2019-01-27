using RegistrationIntegration.Models;

namespace RegistrationIntegration.Sevices
{
    public interface IRegistrationIntegrationService
    {
        bool PostTerminalRegistrationInfo(string inSyndicateCode, ApiTerminalViewModel siteOrderViewModel);
        ApiCompanySiteViewModel ValidateCompanyInfo(string inSyndicateCode, int siteCode);
    }
}