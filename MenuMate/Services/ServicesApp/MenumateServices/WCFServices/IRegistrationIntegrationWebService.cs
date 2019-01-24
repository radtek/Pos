using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MenumateServices.DTO.MenumateRegistration;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IRegistrationIntegrationWebService
    {
        [OperationContract]
        RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, TerminalModel terminalRegistrationInfo);
        [OperationContract]
        RegistrationWebResponse ValidateCompanyInfo(string inSyndicateCode, int siteCode);
    }
}
