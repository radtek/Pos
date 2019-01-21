using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IRegistrationIntegrationWebService
    {
        //public UpdateRegistrationDetails(Site siteInfo);
        [OperationContract]
        void show();
    }
}
