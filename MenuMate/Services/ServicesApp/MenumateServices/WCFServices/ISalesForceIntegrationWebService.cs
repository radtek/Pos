using System.ServiceModel;
using SalesForceCommunication.Domain;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface ISalesForceIntegrationWebService
    {
        [OperationContract]
        bool UpdateSalesForceVersion(string versionNumber, string accountName);

        [OperationContract]
        bool EnableMobiToGo(string accountName);

        [OperationContract]
        bool DisableMobiToGo(string accountName);

        [OperationContract]
        bool UpdateSalesForceModulesStatus(SalesForceModuleStatus salesForceModuleStatus);
    }
}