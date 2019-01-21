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
        //public UpdateRegistrationDetails(Site siteInfo);
        [OperationContract]
        RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, Terminal terminalRegistrationInfo);
    }
}
